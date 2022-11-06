#include <iostream>
#include <errno.h>  // strerror()
#include <string.h> // strerrror()
#include <sys/socket.h>   // socket()
#include <arpa/inet.h>    // htons()

int CreateSocket(){
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_fd == -1){
    throw errno;
  }
  return socket_fd;
}

void AssignSocketAddressToSocket(int socket_fd){
  //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 

  struct sockaddr_in my_socket_address;
  my_socket_address.sin_family = AF_INET;
  my_socket_address.sin_port = htons(8085);
  my_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

  socklen_t my_socket_size = sizeof(my_socket_address);

  int bind_result = bind(
      socket_fd,
      reinterpret_cast<sockaddr*>(&my_socket_address),
      //sizeof(my_socket_address)
      my_socket_size
  );
}

void Send(int socket_fd){
  char buffer_out [50] = "Hola server";
  
  struct sockaddr_in server_sockaddr;
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &server_sockaddr.sin_addr);
  socklen_t server_sockaddr_len = sizeof(server_sockaddr);

  int bytes_out = sendto(socket_fd, buffer_out, strlen(buffer_out), 0, reinterpret_cast<sockaddr*>(&server_sockaddr), server_sockaddr_len);
}

void Recv(int socket_fd){
  char buffer_in [50];
  memset(buffer_in, 0, 50);
  
  struct sockaddr_in client_sockaddr;
  //server_sockaddr.sin_family = AF_INET;
  //server_sockaddr.sin_port = htons(8081);
  //inet_pton(AF_INET, "127.0.0.1", &server_sockaddr.sin_addr);
  socklen_t client_sockaddr_len;

  int bytes_in = recvfrom(socket_fd, buffer_in, 49, 0, reinterpret_cast<sockaddr*>(&client_sockaddr), &client_sockaddr_len);

  std::cout << buffer_in << "\n";
}

int main(){
  try{
     int socket_fd = CreateSocket(); 

     AssignSocketAddressToSocket(socket_fd);

     Send(socket_fd);
     Recv(socket_fd);

  }catch(int err_code){
    std::cout << "Error: " << strerror(err_code) << "\n";
  }
  return 0; 
}
