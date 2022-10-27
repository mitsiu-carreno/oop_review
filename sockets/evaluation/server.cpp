#include <iostream>
#include <stdlib.h>   // atoi
#include <string.h>   // memset   strncpy   strerror
#include <errno.h>
#include <sys/socket.h>   // socket   bind  listen  accept
#include <arpa/inet.h>    // htonl htons
#include <unistd.h>   // close

const int err_func_name_len = 30;

struct ErrorLog{
  char func_name_ [err_func_name_len];
  int error_code_;
};

void DisplayError(ErrorLog err_log){
  std::cout << "Error in " << err_log.func_name_ << ": " << strerror(err_log.error_code_) << "\n";
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

int CreateConnection(int socket_fd, int port){
  int listen_result = listen(socket_fd, 20);
  if(listen_result == -1){
    throw ErrorLog {"Listen socket\0", errno};
  }
  std::cout << "Listening TCP connections on port " << port << "\n";

  struct sockaddr_in client_sockaddr;
  socklen_t client_sockaddr_len;
  int accept_result = accept(socket_fd, reinterpret_cast<sockaddr*>(&client_sockaddr), &client_sockaddr_len);
  if(accept_result == -1){
    throw ErrorLog {"Accept socket\0", errno};
  }

  return accept_result;
}

bool RecvMessage(const int conn_fd, const int param_bytes_in, const char *end_signal, const int end_signal_size, struct sockaddr_in &client_sockaddr, socklen_t &client_sockaddr_len){
  int buffer_size = 4094;
  char in_buffer [buffer_size];
  memset(in_buffer, 0, buffer_size);

  int total_bytes_in = param_bytes_in != 0 ? param_bytes_in : buffer_size -1;
  //struct sockaddr_in client_sockaddr;
  //socklen_t client_sockaddr_len;

  int current_bytes_in = 0;
  while(true){
    int bytes_in = recvfrom(conn_fd, &in_buffer[current_bytes_in], total_bytes_in - current_bytes_in, 0, reinterpret_cast<sockaddr*>(&client_sockaddr), &client_sockaddr_len);
    if(bytes_in == 0){
      // TODO handle this
      std::cout << "Connection ended while listening, partial message was: " << in_buffer;
      return false;
    }else if(bytes_in == -1){
      throw ErrorLog {"Recv Message\0", errno};
    }
    current_bytes_in += bytes_in;

    if(param_bytes_in > 0){
      // END BY MSG WIDTH
      if(current_bytes_in >= total_bytes_in){
        break;
      }
    }else{
      // END BY end_signal
      int last_meaningful_byte = current_bytes_in;
      for(; last_meaningful_byte > 3; --last_meaningful_byte){
        if(strncmp(&in_buffer[last_meaningful_byte], "\0", 1) != 0){
          break;
        }
      }
      if(strncmp(&in_buffer[last_meaningful_byte - 3], end_signal, 3) == 0){
        break;
      }
    }
  }

  std::cout << in_buffer << "\n";
  // TODO search in db
  return true;
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

    int conn_fd;
    if(is_tcp){
      conn_fd = CreateConnection(socket_fd, port);
    }else{
      conn_fd = socket_fd;
    }

    struct sockaddr_in client_sockaddr;
    socklen_t client_sockaddr_len;

    bool loop = true;
    while(loop){
      loop = !RecvMessage(conn_fd, bytes_in, end_signal, strlen(end_signal), client_sockaddr, client_sockaddr_len);
    }

    close(socket_fd);

  }catch(ErrorLog err_log){
    DisplayError(err_log);
    return -1;
  }

  return 0;
}
