#include <iostream>
#include <stdlib.h>   // atoi
#include <string.h>   // memset   strncpy   strerror
#include <errno.h>

const int err_func_name_len = 30;

struct ErrorLog{
  char func_name_ [err_func_name_len];
  int error_code_;
};

void DisplayError(ErrorLog err_log){
  std::cout << "Error in " << err_log.func_name_ << ": " << strerror(err_log.error_code_) << "\n";
}

int CreateSocket(){
  
}

int main(int argc, char **argv){
  if(argc != 5){
    std::cout << "Missing arguments use as: tcp_server <port> <bytes_in/0> <end_signal/0> <bytes_out>\n";
    return -1;
  }
  
  int port;
  int bytes_in;
  std::string end_signal;
  int bytes_out;
  try{
    port = atoi(argv[1]);
    if(port == 0){
      throw ErrorLog {"Port conversion\0", -1};
    }

    bytes_in = atoi(argv[2]);
    if(bytes_in == 0){
      end_signal = argv[3];
      if(end_signal == "0\0"){
        throw ErrorLog {"No bytes_in nor end_signal", -1};
      }
    }

    bytes_out = atoi(argv[4]);
    if(bytes_out == 0){
      throw ErrorLog {"Bytes_out conversion\0", -1};
    }

    int socket_fd = CreateSocket();


  }catch(ErrorLog err_log){
    DisplayError(err_log);
    return -1;
  }

  return 0;
}
