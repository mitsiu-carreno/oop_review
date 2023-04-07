#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>


int main(){
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if(socket_fd < 0){
    return -1;
  }

  ///////////
  struct sockaddr_in socket_address = {};
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  socket_address.sin_port = htons(8081);

  int bind_result = bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&socket_address), sizeof(socket_address));

  if(bind_result < 0){
    return -1;
  }

  //////////////////////////
  const int recv_buffer_size = 30;
  char recv_buffer [recv_buffer_size] = "Hola desde la app 4 end\0";
/*
  struct sockaddr_in server_sock_addr;
  server_sock_addr.sin_family = AF_INET;
  server_sock_addr.sin_port = htons(8080);
  inet_pton(AF_INET, "10.0.0.28", &server_sock_addr.sin_addr);
*/

  // new
  sockaddr_storage server_sock_addr = {};
  addrinfo *result_list = NULL;
  addrinfo hints = {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  if(getaddrinfo("10.0.0.28", "8080", &hints, &result_list) == 0){
    memcpy(&server_sock_addr, result_list->ai_addr, result_list->ai_addrlen);
    freeaddrinfo(result_list);
  }else{
    return -1;
  }

  // end new

  sendto(socket_fd, recv_buffer, recv_buffer_size-1, 0, (sockaddr*)(&server_sock_addr), sizeof(server_sock_addr));


  ///////////////

  struct sockaddr_in client_sock_addr;
  socklen_t client_sock_addr_size;

  std::cout << "Ready to send/recv messages in 8081\n";

  char buffer [2094];
  memset(&buffer, 0, 2094);

  recvfrom(socket_fd, buffer, 2094, 0, reinterpret_cast<sockaddr*>(&client_sock_addr), &client_sock_addr_size);

  printf("%s\n", buffer);
  
  return 0;
}
