#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#define MAX_MSG_SIZE 1

int msg_erro(char* msg){
	std::cout << "(" << msg << ")Error code: " << WSAGetLastError() << std::endl;
	system("pause");
	return 1;
}

int main(){
	
	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (result != 0){
		return msg_erro("WSAStartup");
	}

	sockaddr_in endereçoServidor;
	endereçoServidor.sin_addr.s_addr = inet_addr("0.0.0.0");
	endereçoServidor.sin_family = AF_INET;
	endereçoServidor.sin_port = htons(9999);

	SOCKET principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (principalSocket == INVALID_SOCKET){
		return msg_erro("socket");
	}

	result = bind(principalSocket, (sockaddr*)&endereçoServidor, sizeof(endereçoServidor));

	if (result == SOCKET_ERROR){
		return msg_erro("bind");
	}

	result = listen(principalSocket, SOMAXCONN);

	if (result == SOCKET_ERROR){
		return msg_erro("listen");
	}

	SOCKET clienteSocket;
	sockaddr_in clienteEndereço;
	int clienteEndereçoTam = sizeof(clienteEndereço);

	char buffer[MAX_MSG_SIZE];
	char sBuffer[MAX_MSG_SIZE];
	while (true){
		clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereço, &clienteEndereçoTam);

		std::cout << "Cliente conectado! IP: " << inet_ntoa(clienteEndereço.sin_addr)
			<< ":" << ntohs(clienteEndereço.sin_port) << std::endl;

		result = recv(clienteSocket, buffer, MAX_MSG_SIZE, NULL);

		if (result == SOCKET_ERROR){
			msg_erro("recv");
		}
		else {
			std::cout << "PacketSize: " << result << std::endl
				<< "PacketReceived: " << buffer[0] << std::endl;
			sBuffer[0] = toupper(buffer[0]);
			send(clienteSocket, sBuffer, MAX_MSG_SIZE, NULL);
		}
	}

	closesocket(principalSocket);

	WSACleanup();

	system("pause");
	return 0;
}
