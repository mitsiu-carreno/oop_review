#include <iostream>
#include <stdlib.h>   // atoi   getenv
#include <string.h>   // memset   strncpy   strerror
#include <errno.h>
#include <sys/socket.h>   // socket   bind  listen  accept
#include <arpa/inet.h>    // htonl htons
#include <unistd.h>   // close
#include <algorithm>
#include <pqxx/pqxx>  // postgres connection

const int err_func_name_len = 30;
const int up_len = 8;

struct ErrorLog{
  char func_name_ [err_func_name_len];
  int error_code_;
};

void DisplayError(ErrorLog err_log){
  std::cout << "Error in " << err_log.func_name_ << ": " << strerror(err_log.error_code_) << "\n";
}

void GetEnv(const char *env_key, std::string &env_value){
  const char *tmp_env = getenv(env_key);
  env_value = (tmp_env ? tmp_env : "");
  if(env_value.empty()){
    std::cout << env_key << " not found\n";
    throw ErrorLog {"Loading env\0", -1};
  }
  // SANITIZATION
  int pos = env_value.find(' ');
  if(pos != std::string::npos){
    env_value = env_value.substr(0, pos);
  }
}

pqxx::connection* PostgresConnection(){
    std::string db_name;
    std::string db_user;
    std::string db_pass;
    std::string db_host;
    std::string db_port;

    GetEnv("DB_NAME", db_name);
    GetEnv("DB_USER", db_user);
    GetEnv("DB_PASS", db_pass);
    GetEnv("DB_HOST", db_host);
    GetEnv("DB_PORT", db_port);

    std::string connection_string = "dbname = " + db_name + " user = " + db_user + " password = " + db_pass + " hostaddr = " + db_host + " port = " + db_port;
    pqxx::connection *postg_conn = new pqxx::connection(connection_string);
    if(postg_conn->is_open()){
      std::cout << "Opened database successfully: " << postg_conn->dbname() << "\n";
    }else{
      throw ErrorLog {"Can't open database\0", -1};
    }

    //C.disconnect();
    return postg_conn;
}

bool SearchUp(pqxx::connection *postg_conn, std::string up){
  try{
    std::cout << "Searching for: " << up << "\n";
      
    postg_conn->prepare("findUp", "SELECT COUNT(*) FROM up WHERE up = $1");

    pqxx::nontransaction postg_nont(*postg_conn);

    pqxx::result postg_res(postg_nont.exec_prepared("findUp", up));

    return postg_res.begin()[0].as<bool>(); 

    //for(pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c){
    //  std::cout << "res = " << c[0].as<int>() << "\n";
    //}

  }catch(const std::exception &e){
    std::cout << e.what();
  }

  return false;
}

int CreateSocket(bool is_tcp){
  int socket_fd = socket(AF_INET, is_tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
  if(socket_fd == -1){  
    throw ErrorLog {"Create Socket", errno};
  }
  return socket_fd;
}

void AssociateSockAddrToSock(int socket_fd, int port){

  struct sockaddr_in server_sockaddr;
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(port);
  server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


  int bind_result = bind(socket_fd, reinterpret_cast<sockaddr*>(&server_sockaddr), sizeof(server_sockaddr));
  if(bind_result == -1){
    throw ErrorLog {"Bind socket", errno};
  }
}

void StartListeningConnections(int socket_fd, int port){
  int listen_result = listen(socket_fd, 20);
  if(listen_result == -1){
    throw ErrorLog {"Listen socket\0", errno};
  }
}

int CreateConnection(int socket_fd, struct sockaddr_in *client_sockaddr, socklen_t *client_sockaddr_len){
  int accept_result = accept(socket_fd, reinterpret_cast<sockaddr*>(client_sockaddr), client_sockaddr_len);
  if(accept_result == -1){
    throw ErrorLog {"Accept socket\0", errno};
  }

  return accept_result;
}

bool RecvMessage(const int conn_fd, const int param_bytes_in, const char *end_signal, const int end_signal_size, struct sockaddr_in *client_sockaddr, socklen_t *client_sockaddr_len, pqxx::connection *postg_conn){
  int buffer_size = 4094;
  char in_buffer [buffer_size];
  memset(in_buffer, 0, buffer_size);

  int total_bytes_in = param_bytes_in != 0 ? param_bytes_in : buffer_size -1;

  int current_bytes_in = 0;
  while(true){
    int bytes_in = recvfrom(conn_fd, &in_buffer[current_bytes_in], total_bytes_in - current_bytes_in - 1, 0, reinterpret_cast<sockaddr*>(client_sockaddr), client_sockaddr_len);
    if(bytes_in == 0){
      std::cout << "Connection ended while listening, partial message was: " << in_buffer;
      return false;
    }else if(bytes_in == -1){
      throw ErrorLog {"Recv Message\0", errno};
    }
    current_bytes_in += bytes_in;

      char client_addr [INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(client_sockaddr->sin_addr), client_addr, INET_ADDRSTRLEN);

      std::cout << client_addr << "\n";
    if(param_bytes_in > 0){
      // END BY MSG WIDTH
      if(current_bytes_in >= total_bytes_in){
        break;
      }
    }else{
      // END BY end_signal
      int last_meaningful_byte = current_bytes_in;
      for(; last_meaningful_byte > end_signal_size; --last_meaningful_byte){
        if(strncmp(&in_buffer[last_meaningful_byte], "\0", 1) != 0){
          break;
        }
      }
      if(strncmp(&in_buffer[last_meaningful_byte - end_signal_size], end_signal, end_signal_size) == 0){
        break;
      }
    }
  }

  std::cout << in_buffer << "\n";
  if(current_bytes_in >= up_len){
    // CHECK UP 
    std::string up;
    up.assign(in_buffer, up_len);
    bool found = SearchUp(postg_conn, up);
    
    postg_conn->prepare("new_log", "INSERT INTO logs (up, proto, port, bytes_in, end_signal, bytes_out, msg) VALUES ($1, $2, $3, $4, $5, $6, $7)");

    pqxx::work postg_t(*postg_conn);

    pqxx::result postg_res(postg_t.exec_prepared("new_log", "UP200994", "UDP", 8080, 0, "end", 15, "hola"));

    postg_t.commit();

    return found;
  }
  return false;
}

void SendMessage(const int conn_fd, const int param_bytes_out, sockaddr_in *client_sockaddr, socklen_t client_sockaddr_len){
  std::cout << client_sockaddr_len << "\n";
  char out_buffer [param_bytes_out] = "Felicidades has aprobado la evaluación";

  int current_bytes_out = 0;
  while(true){
    int bytes_out = sendto(conn_fd, &out_buffer[current_bytes_out], strlen(out_buffer) - current_bytes_out, 0, reinterpret_cast<sockaddr*>(client_sockaddr), client_sockaddr_len);
    if(bytes_out == -1){
      throw ErrorLog {"Send Message\0", errno};
    }
    current_bytes_out += bytes_out;
    if(current_bytes_out == strlen(out_buffer)){
      return;
    }
  }
}

int main(int argc, char **argv){
  if(argc != 6){
    std::cout << "Missing arguments use as: tcp_server <TCP/UDP> <port> <bytes_in/0> <end_signal/0> <bytes_out>\n";
    return -1;
  }
  
  int port;
  int bytes_in;
  //std::string end_signal;
  int end_signal_size = 100;
  char end_signal[end_signal_size];
  int bytes_out;
  bool is_tcp;
  pqxx::connection *postg_conn;
  try{
    
    postg_conn = PostgresConnection();

    std::string proto = argv[1];
    if(proto == "TCP\0" || proto == "tcp\0"){
      is_tcp = true;
    }else if(proto == "UDP\0" || proto == "udp\0"){
      is_tcp = false;
    }else{
      std::cout << "Unsupported proto, [TCP/UDP]\n";
      throw ErrorLog {"Unsupported proto\0", -1};
    }

    port = atoi(argv[2]);
    if(port == 0){
      throw ErrorLog {"Port conversion\0", -1};
    }else if(port < 0){
      throw ErrorLog {"Invalid port number\0", -1};
    }

    bytes_in = atoi(argv[3]);
    if(bytes_in == 0){
      if(strlen(argv[4]) >= end_signal_size){
        throw ErrorLog {"End signal is too long", -1};
      }
      strcpy(end_signal, argv[4]);
      if(end_signal[0] == '0'){
        throw ErrorLog {"No bytes_in nor end_signal", -1};
      }
    }else if(bytes_in < 0){
      throw ErrorLog {"Invalid bytes_in number\0", -1};
    }

    bytes_out = atoi(argv[5]);
    if(bytes_out == 0){
      throw ErrorLog {"Bytes_out conversion\0", -1};
    }else if(bytes_out < 0){
      throw ErrorLog {"Invalid bytes_out number\0", -1};
    }

    int socket_fd = CreateSocket(is_tcp);

    AssociateSockAddrToSock(socket_fd, port);

    if(is_tcp){
      StartListeningConnections(socket_fd, port);
    }

    std::cout << "Listening " << (is_tcp ? "TCP" : "UDP") << " connections on port " << port << "\n";

    int conn_fd = socket_fd;

    struct sockaddr_in client_sockaddr;
    socklen_t client_sockaddr_len = 0;
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));

    bool loop = true;
    while(loop){
      if(is_tcp){
        conn_fd = CreateConnection(socket_fd, &client_sockaddr, &client_sockaddr_len);
      }
      loop = !RecvMessage(conn_fd, bytes_in, end_signal, strlen(end_signal), &client_sockaddr, &client_sockaddr_len, postg_conn);
      if(loop){
        if(is_tcp){
          close(conn_fd);
        }
        std::cout<< "Up not found try again\n";
      }
    }

    SendMessage(conn_fd, bytes_out, &client_sockaddr, client_sockaddr_len);

    if(is_tcp){
      close(conn_fd);
    }
    close(socket_fd);

    postg_conn->disconnect();
    delete postg_conn;

  }catch(ErrorLog err_log){
    DisplayError(err_log);
    return -1;
  }catch(const std::exception &e){
    std::cout << e.what() << "\n";
    return -1;
  }

  return 0;
}
