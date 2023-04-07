#include <iostream>
#include <sys/types.h>      
#include <sys/socket.h>//socket
#include <arpa/inet.h> //hton
#include <cstring>
#include <string.h>
#include <errno.h> //sterror
#include <unistd.h>

int CreateSocket(){

    int socket_fd = socket(AF_INET,SOCK_DGRAM,0);//SOCK_DGRAM
    if(socket_fd==-1){
        throw errno;
    }
    return socket_fd;
}
void AssingSocketAddrToSocket(int socket_fd, int port){
    struct sockaddr_in my_socket_address;
    my_socket_address.sin_family = AF_INET;
    my_socket_address.sin_port = htons(port);
    my_socket_address.sin_addr.s_addr = htonl(INADDR_ANY); // escuchar en todas las interfaces (Distintas IP)
    //std::cout<<"HOla"<<"\n";

    socklen_t my_socket_size = sizeof(my_socket_address);
    int bind_result = bind(socket_fd, reinterpret_cast<sockaddr*>(&my_socket_address) , my_socket_size );//asocia la direccion que se va a usar con un puerto
    if(bind_result == -1){//bind une la direccion (socketaddress) con un puerto 
        throw bind_result;
    }
} 
void SendClientMessage(int socket_fd, int port){
    int buffer_size = 29;
    char send_buffer[buffer_size]="UP200053";
    //memset(send_buffer,0,buffer_size);
    int bytes_enviados=28;
    int total_bytes_out=0;

    struct sockaddr_in server_socket_address;
    server_socket_address.sin_family = AF_INET;
    server_socket_address.sin_port = htons(port);
    inet_pton(AF_INET,"10.0.0.28",&server_socket_address.sin_addr);
    while(total_bytes_out <= bytes_enviados){
        int bytes_out = sendto(socket_fd, send_buffer,buffer_size-1, 0, reinterpret_cast<sockaddr*>(&server_socket_address) , sizeof(server_socket_address) );
        total_bytes_out = total_bytes_out + bytes_out;
        std::cout<<bytes_out<<"\n";
    }
}


void RecvMessage(int socket_fd){


    int buffer_size = 4094;
    char in_buffer [buffer_size];
    memset(in_buffer,0,buffer_size);//la dirección inicia en el primer espacio, no es necesario el &
    struct sockaddr_in src_socket_address;
    memset(&src_socket_address,0, sizeof(src_socket_address) );//apuntador porque pide dirección 
    socklen_t sockaddr_lenght; //se llena con la funcion de recvfrom
    int bytes_in = recvfrom(socket_fd,in_buffer, buffer_size-1, 0, reinterpret_cast <sockaddr*>(&src_socket_address), &sockaddr_lenght );
    std::cout<< in_buffer <<"\n";
}


int main (){
    try{
        int socket_fd= CreateSocket();
        AssingSocketAddrToSocket(socket_fd,8333);
        SendClientMessage(socket_fd,8300);
        RecvMessage(socket_fd);

    }catch(int error_code){
        std::cout<<"Error: "<< strerror(error_code)<<"\n";
    }

    return 0;
}
