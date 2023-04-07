/* 
funciones udp 
socket (x)
bind (x)
send (x)
recvfrom (x)
close ()
*/ 
//correo del profe : mitsiu.carreno@alumnos.upa.edu.mx 
#include<iostream>
#include<sys/types.h> 
#include<sys/socket.h> 
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>
#include<string.h>
using namespace std;

int CreateSocket(){
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_fd ==-1){
		throw errno;
		}
	cout<<"createsocket"<<endl; 
	return socket_fd;	
}

void BindConnect(int socket_fd){
	struct sockaddr_in server_socket_addr;
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(8900);
	server_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	int bind_result = bind(socket_fd, reinterpret_cast<sockaddr*>(&server_socket_addr), sizeof(server_socket_addr));
	if (bind_result==-1){
		throw errno;
	}
		cout<<"connectto server"<<endl; 
	
}

void SendAndRecieve(int socketfd){
	char send_buffer[71] = "UP200231 finish";
	struct sockaddr_in server_socket_addr;
	server_socket_addr.sin_family = AF_INET;
	server_socket_addr.sin_port = htons(8900);
	int convert_flag = inet_pton(AF_INET, "10.0.0.28", &server_socket_addr.sin_addr.s_addr);
	if(convert_flag != 1){
		std::cout << "Error al convertir la flag (linea 48)\n";
		throw errno;
	}
	int send_msg = sendto(
		socketfd, 
		send_buffer, 
		sizeof(send_buffer), 
		0,
        reinterpret_cast<sockaddr *>(&server_socket_addr), 
		sizeof(server_socket_addr));
	if(send_msg == -1){
		std::cout << "Error al mandar el mensaje\n";
		throw errno;
	}
	std::cout << "Mensaje enviado\n";


	struct sockaddr_in emisor_sockaddr;
	memset(&emisor_sockaddr, 0, sizeof(emisor_sockaddr));
	socklen_t emisor_sockaddr_len = sizeof(emisor_sockaddr); 

	char recv_buffer[4094];
	memset(recv_buffer, 0, 4094);

	int recv_msg = recvfrom(
		socketfd, 
		recv_buffer, 
		sizeof(recv_buffer) -1, 
		0,
		reinterpret_cast<sockaddr *>(&emisor_sockaddr),
		&emisor_sockaddr_len
	);

	if(recv_msg == -1){
		std::cout << "Error al recibir mensaje!!!\n";
		throw errno;
	}

	printf("%s \n", recv_buffer);

	int closing = close(socketfd);
    if(closing == -1)
    	throw -1;

}


int main(){
    try {
        int socket_fd = CreateSocket();
		BindConnect(socket_fd);
		SendAndRecieve(socket_fd);
    } catch (int e){
        std::cout << "Error" << e << ": " << strerror(e) << "\n";
		return e;
    }

    return 0;
}

