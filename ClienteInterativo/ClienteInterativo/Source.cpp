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

	SOCKET meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (meuSocket == INVALID_SOCKET){
		return msg_erro("socket");
	}

	sockaddr_in endereçoServidor;
	//endereçoServidor.sin_addr.s_addr = inet_addr("10.96.25.144");
	endereçoServidor.sin_addr.s_addr = inet_addr("10.96.24.165");
	//endereçoServidor.sin_addr.s_addr = inet_addr("10.96.25.1");
	//endereçoServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	endereçoServidor.sin_family = AF_INET;
	endereçoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&endereçoServidor, sizeof(endereçoServidor));

	if (result == SOCKET_ERROR){
		return msg_erro("conenct");
	}

	char buffer[] = { 'g' };
	char rBuffer[MAX_MSG_SIZE];
	send(meuSocket, buffer, 1, NULL);
	recv(meuSocket, rBuffer, MAX_MSG_SIZE, NULL);
	if (result == SOCKET_ERROR){
		msg_erro("recv");
	}
	else {
		std::cout << rBuffer[0] << std::endl;
	}

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}