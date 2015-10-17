#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>

#define PORTA_SERVIDOR 9000
#define CMD_AUTH_REQUEST 1

int main(){

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in enderecoServidor;
	inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

	connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	int comando = 1;
	send(meuSocket, (char*)&comando, sizeof(int), NULL);

	std::string login;
	std::string senha;

	std::cout << "Informe seu login: ";
	std::getline(std::cin, login);

	std::cout << "Informe sua senha: ";
	std::getline(std::cin, senha);
	int size = login.size();
	send(meuSocket, (char*)&size, sizeof(int), NULL);
	send(meuSocket, (char*)login.c_str(), sizeof(int), NULL);
	size = senha.size();
	send(meuSocket, (char*)&size, sizeof(int), NULL);
	send(meuSocket, (char*)senha.c_str(), sizeof(int), NULL);

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}