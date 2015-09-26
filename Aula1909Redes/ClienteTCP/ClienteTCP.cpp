#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

int MsgErro(char* msg){
	std::cout << "(" << msg << ")Error code: " << WSAGetLastError() << std::endl;
	system("pause");
	return 1;
}

int main(){

	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0){
		return MsgErro("WSAStartup");
	}

	SOCKET meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (meuSocket == INVALID_SOCKET){
		return MsgErro("socket");
	}

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	//enderecoServidor.sin_addr.s_addr = inet_addr("10.96.26.75");
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	if (result == SOCKET_ERROR){
		return MsgErro("conenct");
	}

	while (true){
		char buffer = getchar();
		result = send(meuSocket, &buffer, 1, 0);
		if (result == SOCKET_ERROR){
			return MsgErro("send");
		}
		result = recv(meuSocket, &buffer, 1, 0);
		if (result == SOCKET_ERROR){
			return MsgErro("recv");
		}
		std::cout << buffer;
	}

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}