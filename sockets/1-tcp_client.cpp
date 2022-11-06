#include <sys/socket.h> // socket() connect()
#include <arpa/inet.h>  // inet_pton()
#include <iostream>     // iostream
#include <string.h>
#include <unistd.h>


int main(){
  
  ////// CREATE SOCKET DESCRIPTOR  
  // File descriptor are non-negative integers that act as an abstract handle to "Files" or I/O resources (pipes sockets or data streams)
  int sock_fd;

  try{
    std::cout << AF_INET << SOCK_STREAM << "\n";
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_fd == -1){
      throw -1;
    }
  }catch(int error_code){
    std::cout << "Error al intentar crear socket\n";
    return error_code;
  }

  //sleep(30);

  struct sockaddr_in server_socket_address; // Defined in /usr/include/netinet/in.h
  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_port = htons(8080);
  //server_socket_address.sin_addr = 

  try{
    if(inet_pton(AF_INET, "10.0.0.28", &server_socket_address.sin_addr) != 1){
      throw -1;
    }
  }catch(int error_code){
    std::cout << "Error al intentar convertir una IPv4 a network address structure\n";
    return error_code;
  }

  try{
    int connect_response;
    connect_response = connect(sock_fd, reinterpret_cast<sockaddr*>(&server_socket_address), sizeof(server_socket_address));
    if(connect_response == -1){
      throw -1;
    }
  }catch(int error_code){
    std::cout << "Error al intentar que el socket se conecte a un servidor\n";
  }

  //sleep(30);
  char message_to_send [4094] = "Huge message with lots of bytes and will be truncated";
  int message_size = strlen(message_to_send);  // Each char is a byte

  //write(sock_fd, message_to_send, message_size);
  send(sock_fd, message_to_send, message_size, 0);

  char server_response [4094];
  memset(server_response, 0, 4094);

  int bytes_in = 1;
  std::cout << "PRE\n\n";
  while(bytes_in > 0){
    printf("start loop\n");
    bytes_in = recv(sock_fd, server_response, 4093, 0);
    std::cout << "\n-------" << bytes_in << "------------\n";
  //while( recv(sock_fd, server_response, 4093, 0) > 0){
    //std::cout << "More data\n";
    printf("%s", server_response);
    printf("finish printing\n");
    memset(server_response, 0, 4094);
    printf("finish cleaning\n");
  }
  std::cout << "POS\n\n";
  // MISSING validation
  std::cout << "END OF DATA-----------\n";
  std::cout << "Closed! \n";
  close(sock_fd);

  //sleep(30);

  return 0;
}
