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
	meuEndereco.sin_port = htons(8888);

	SOCKET meuSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(meuSocket, (sockaddr*)&meuEndereco, sizeof(meuEndereco));
	sockaddr_in enderecoRemetente;
	int enderecoRemetenteTam = sizeof(enderecoRemetente);
	char buffer[MAX_PACKET_SIZE];

	for(;;){
		std::cout << "Waiting..." << std::endl;
		int size = recvfrom(meuSocket, buffer, MAX_PACKET_SIZE, NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
		std::cout << "From: " << inet_ntoa(enderecoRemetente.sin_addr) << ":" << htons(enderecoRemetente.sin_port) << std::endl
			<< "PacketSize: " << size << std::endl
			<< "PacketData: " << buffer << std::endl;
	}

	closesocket(meuSocket);
	WSACleanup();
	system("pause");
	return 0;
}