#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <Teste.h>

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
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	if (result == SOCKET_ERROR){
		return msg_erro("connect");
	}
	int a;
	float b;
	double c;
	char d[10];

	std::cout << "A: ";
	std::cin >> a;

	std::cout << "B: ";
	std::cin >> b;

	std::cout << "C: ";
	std::cin >> c;

	std::cout << "D: ";
	std::cin >> d;


	Teste teste(a, b, c, d);

	result = send(meuSocket, (char*)&teste, sizeof(Teste), 0);
	if (result == SOCKET_ERROR){
		return msg_erro("send");
	}
	closesocket(meuSocket);
	std::cout << "Desconectado!" << std::endl;
	WSACleanup();

	system("pause");
	return 0;
}