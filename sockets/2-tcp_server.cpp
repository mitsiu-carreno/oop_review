#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>


int CreateSocket(){
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1){
    throw 'a';
  }
  return socket_fd;
}

void AssignSocketAddressToSocket(int sock_fd){
  struct sockaddr_in server_socket_address;
  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_port = htons(8888); 
  server_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_result;
  bind_result = bind(sock_fd, reinterpret_cast<sockaddr *>(&server_socket_address), sizeof(server_socket_address));
  if(bind_result != 0){
    throw 3.14f;
  }
}

void StartListeningIncomingConnections(int sock_fd){
  int listen_result;
  listen_result = listen(sock_fd, 10);
  if(listen_result != 0){
    throw 3.14l;
  }
}

void HandleConnectionCommunication(int conn_fd){
  char recv_buffer [4094];
  memset(recv_buffer, 0, 4094);
  //while(true){
    int bytes_in = recv(conn_fd, recv_buffer, 4093, 0);
    if(bytes_in == -1){
      std::cout << "Error receiving message\n";
      //break;
    }else if(bytes_in == 0){
      std::cout << "Disconnected\n";
      //break;
    }

    printf("%s", recv_buffer);
    memset(recv_buffer, 0, 4094);
    printf("cut\n");
    //printf("%s\n", recv_buffer[bytes_in-1]);
    std::cout << "---" << recv_buffer[bytes_in -1] << "------\n";
    //if(recv_buffer[bytes_in -1] == '\n'){
      //break;
    //}
  //}
  
  char send_buffer [4094] = "HTTP/1.0 200 OK\r\n\r\nHELLO FRIEND";

  send(conn_fd, send_buffer, 4093, 0);
  close(conn_fd);
}

void HandleConnectionRequest(int sock_fd){
  std::cout << "Esperando conexión de cliente...\n";

  struct sockaddr_in client_socket_address;
  socklen_t client_address_size;
  //memset(&client_address_size, 0, client_address_size);

  int conn_fd = accept(sock_fd, reinterpret_cast<sockaddr *>(&client_socket_address), &client_address_size);
  if(conn_fd == -1){
    throw true;
  }

  std::cout << inet_ntohl(client_socket_address.sin_addr) << "\n";

  HandleConnectionCommunication(conn_fd);

}

int main(){
  int sock_fd;

  try{
    int socket_fd = CreateSocket();

    AssignSocketAddressToSocket(socket_fd);

    StartListeningIncomingConnections(socket_fd);

    HandleConnectionRequest(socket_fd);

  }catch(char err_code){
    std::cout << "Error al intentar crear un socket\n";
    return -1;
  }catch(float err_code){
    std::cout << "Error al intentar asignar ip y puerto al socket\n";
    return -1;
  }catch(long err_code){
    std::cout << "Error al intentar comenzar a recibir conexiones en socket\n";
    return -1;
  }catch(bool err_code){
    std::cout << "Error al intentar aceptar conexión de socket activo\n";
    return -1;
  }


  return 0;
}
