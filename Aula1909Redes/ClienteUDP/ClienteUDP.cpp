#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>

#define MAX_PACKET_SIZE 512

struct Mensagem {
	char* cliente;
	char* mensagem;
};

int main(){
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in meuEndereco;
	meuEndereco.sin_addr.s_addr = INADDR_ANY;
	meuEndereco.sin_family = AF_INET;
	meuEndereco.sin_port = htons(8889);

	SOCKET meuSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(meuSocket, (sockaddr*)&meuEndereco, sizeof(meuEndereco));

	sockaddr_in destinatario;
	//destinatario.sin_addr.s_addr = inet_addr("10.96.26.75");
	destinatario.sin_addr.s_addr = inet_addr("127.0.0.1");
	destinatario.sin_family = AF_INET;
	destinatario.sin_port = htons(9999);

	std::string msg;

	do{
		std::getline(std::cin, msg);

		sendto(meuSocket, msg.c_str(), MAX_PACKET_SIZE, NULL, (sockaddr*)&destinatario, sizeof(destinatario));
	} while (msg.size() > 1);


	closesocket(meuSocket);
	WSACleanup();
	system("pause");
	return 0;
}