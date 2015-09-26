#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#define MAX_PACKET_SIZE 512

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
	destinatario.sin_addr.s_addr = inet_addr("127.0.0.1");
	destinatario.sin_family = AF_INET;
	destinatario.sin_port = htons(8888);

	char buffer[] = "asdffasdas asd312 23 423";

	sendto(meuSocket, buffer, MAX_PACKET_SIZE, NULL, (sockaddr*)&destinatario, sizeof(destinatario));

	closesocket(meuSocket);
	WSACleanup();
	system("pause");
	return 0;
}