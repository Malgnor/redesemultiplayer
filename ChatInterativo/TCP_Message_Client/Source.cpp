#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>

#define MAX_MSG_SIZE 1
#define _ASDF_

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

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	//enderecoServidor.sin_addr.s_addr = inet_addr("10.96.24.106");
	//enderecoServidor.sin_addr.s_addr = inet_addr("10.96.24.104");
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	if (result == SOCKET_ERROR){
		return msg_erro("connect");
	}

	std::string msg;

	do{
		std::getline(std::cin, msg);
		msg += '\n';

#ifdef _ASDF_
		int tam = msg.size();
		result = send(meuSocket, (char*)&tam, sizeof(int), 0);
		if (result == SOCKET_ERROR){
			return msg_erro("send");
		}
#endif

		result = send(meuSocket, msg.c_str(), strlen(msg.c_str()), 0);
		if (result == SOCKET_ERROR){
			return msg_erro("send");
		}
	} while (msg.size() > 1);
	
	closesocket(meuSocket);
	std::cout << "Desconectado!" << std::endl;
	WSACleanup();

	system("pause");
	return 0;
}