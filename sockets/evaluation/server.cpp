#include <iostream>
#include <stdlib.h>   // atoi   getenv
#include <string.h>   // memset   strncpy   strerror
#include <errno.h>
#include <sys/socket.h>   // socket   bind  listen  accept
#include <arpa/inet.h>    // htonl htons
#include <unistd.h>   // close
#include <fstream>    // file writting
#include <algorithm>

const int err_func_name_len = 30;
const int up_len = 8;

struct ErrorLog{
  char func_name_ [err_func_name_len];
  int error_code_;
};

void DisplayError(ErrorLog err_log){
  std::cout << "Error in " << err_log.func_name_ << ": " << strerror(err_log.error_code_) << "\n";
}

bool SearchUp(std::string new_up){
  std::cout << "Searching for " << new_up << "\n";
  std::string up_list = "UP200231 UP200994 UP200953 UP200667 UP200045 UP200423 UP200920 UP200604 UP200053 UP200738 UP200966 UP200424 UP200512 UP200760 UP200511 UP210934";
  bool found = false;
  if(up_list.find(new_up, 0) != std::string::npos){
    found = true;
  }
  return found;
}

void WriteLog(std::string up, bool is_tcp, const int port, const char *client_ip, const int bytes_in, const char *end_signal,  const char *msg){
  std::cout << "Writting log\n";
  std::string folder = "logs/";
  std::string proto = is_tcp ? "_TCP_" : "_UDP_";
  static bool flag = true;
  std::string file_name = folder + up + proto + std::to_string(port);

  std::ofstream write_stream;
  write_stream.open(file_name);
  write_stream << client_ip << "," << bytes_in << "," << end_signal << "," << msg << "\n";
  write_stream.close();

}

int CreateSocket(bool is_tcp){
  std::cout << "Creating socket\n";
  int socket_fd = socket(AF_INET, is_tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
  if(socket_fd == -1){  
    throw ErrorLog {"Create Socket", errno};
  }
  return socket_fd;
}

void AssociateSockAddrToSock(int socket_fd, int port){
  std::cout << "Binding socket\n";

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
  std::cout << "Listening\n";
  int listen_result = listen(socket_fd, 20);
  if(listen_result == -1){
    throw ErrorLog {"Listen socket\0", errno};
  }
}

int CreateConnection(int socket_fd, struct sockaddr_in *client_sockaddr, socklen_t *client_sockaddr_len){
  std::cout << "Accept\n";
  int accept_result = accept(socket_fd, reinterpret_cast<sockaddr*>(client_sockaddr), client_sockaddr_len);
  if(accept_result == -1){
    throw ErrorLog {"Accept socket\0", errno};
  }

  return accept_result;
}

bool RecvMessage(const int conn_fd, char *in_buffer, const int buffer_size, const int param_bytes_in, const char *end_signal, const int end_signal_size, struct sockaddr_in *client_sockaddr, socklen_t *client_sockaddr_len, std::string &up){
  std::cout << "Ready to receive\n";

  int total_bytes_in = param_bytes_in != 0 ? param_bytes_in : buffer_size -1;

  int current_bytes_in = 0;
  while(true){
    int bytes_in = recvfrom(conn_fd, &in_buffer[current_bytes_in], total_bytes_in - current_bytes_in, 0, reinterpret_cast<sockaddr*>(client_sockaddr), client_sockaddr_len);
    if(bytes_in == 0){
      std::cout << "Connection ended while listening, partial message was: " << in_buffer;
      return false;
    }else if(bytes_in == -1){
      throw ErrorLog {"Recv Message\0", errno};
    }
    std::cout << "Processing received message\n";
    current_bytes_in += bytes_in;

      char client_addr [INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(client_sockaddr->sin_addr), client_addr, INET_ADDRSTRLEN);

      std::cout << "Client ip: " << client_addr << "\n";

      std::cout << "Last message bytes: " << bytes_in << " total message bytes: " << current_bytes_in << "\n";
      std::cout << ""
    if(param_bytes_in > 0){
      // END BY MSG WIDTH
      std::cout << "Check if end by msg width " << current_bytes_in << " vs " << total_bytes_in << "\n";
      if(current_bytes_in >= total_bytes_in){
        break;
      }
    }else{
      // END BY end_signal
      std::cout << "Check if end by end_signal\n";

      std::string msg;
      msg.assign(in_buffer, total_bytes_in);
      if(msg.find(end_signal, 0) != std::string::npos){
        //return true;
        break;
      }
      return false;
      /*
      int last_meaningful_byte = current_bytes_in;
      for(; last_meaningful_byte > end_signal_size; --last_meaningful_byte){
        if(strncmp(&in_buffer[last_meaningful_byte], "\0", 1) != 0){
          std::cout << "Must break;";
          break;
        }
      }
      std::cout << "Last_meaningful byte: " << last_meaningful_byte << " with char " << in_buffer[last_meaningful_byte - end_signal_size] << in_buffer[last_meaningful_byte]<< "\n";
      if(strncmp(&in_buffer[last_meaningful_byte - end_signal_size -1], end_signal, end_signal_size) == 0){
        std::cout << "Must break;";
        break;
      }
      */
    }
  }

  std::cout << "Checked done, msg: " << in_buffer << "\n";
  if(current_bytes_in >= up_len){
    // CHECK UP 
    //std::string up;
    up.assign(in_buffer, up_len);
    return SearchUp(up);
  }
  

  return false;
}

void SendMessage(const int conn_fd, const int param_bytes_out, sockaddr_in *client_sockaddr, socklen_t client_sockaddr_len){
  std::cout << "Ready to send message\n";
  std::cout << client_sockaddr_len << "\n";
  char out_buffer[param_bytes_out];
  memset(out_buffer, 0, param_bytes_out);
  strncpy(out_buffer, "Felicidades has aprobado la evaluación\0", strlen("Felicidades has aprobado la evaluación\0"));

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
  try{
    
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
      std::cout << "-"<<end_signal<<"-"<< strlen(end_signal) << "\n:";
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

    std::cout << "Listening " << (is_tcp ? "TCP" : "UDP") << " connections on port " << port << " waiting " << bytes_in << " bytes or end_signal: " <<  end_signal << "\n";

    int conn_fd = socket_fd;

    struct sockaddr_in client_sockaddr;
    socklen_t client_sockaddr_len = 0;
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));

    char client_ip [INET_ADDRSTRLEN];
    std::string up;

    int in_buffer_size = 4094;
    char in_buffer[in_buffer_size];
    memset(in_buffer, 0, in_buffer_size);

    bool loop = true;
    while(loop){
      if(is_tcp){
        conn_fd = CreateConnection(socket_fd, &client_sockaddr, &client_sockaddr_len);
      }
      loop = !RecvMessage(conn_fd, in_buffer, in_buffer_size, bytes_in, end_signal, strlen(end_signal), &client_sockaddr, &client_sockaddr_len, up);

      inet_ntop(AF_INET, &(client_sockaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
      

      if(loop){
        if(is_tcp){
          close(conn_fd);
        }
        std::cout<< "Up not found try again\n";
      }
    }
    //up.assign(in_buffer, up_len);

    WriteLog(up, is_tcp, port, client_ip, bytes_in, end_signal, in_buffer);
    //SendMessage(conn_fd, bytes_out, &client_sockaddr, client_sockaddr_len);

    if(is_tcp){
      close(conn_fd);
    }
    close(socket_fd);


  }catch(ErrorLog err_log){
    DisplayError(err_log);
    return -1;
  }catch(const std::exception &e){
    std::cout << e.what() << "\n";
    return -1;
  }

  return 0;
}
