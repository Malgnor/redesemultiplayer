#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

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
	int comando;
	int clientes = 0;
	int cliente;
	int x[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int y[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (;;){
		std::cout << "Esperando comando..." << std::endl;
		recvfrom(meuSocket, (char*)&comando, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
		switch (comando)
		{
		case 0:
			std::cout << "Comando 0" << std::endl;
			clientes++;
			sendto(meuSocket, (char*)&clientes, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, sizeof(enderecoRemetente));
			break;
		case 1:
			std::cout << "Comando 1" << std::endl;
			recvfrom(meuSocket, (char*)&cliente, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
			recvfrom(meuSocket, (char*)&x[cliente-1], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
			recvfrom(meuSocket, (char*)&y[cliente-1], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
			break;
		case 2:
			std::cout << "Comando 2" << std::endl;
			sendto(meuSocket, (char*)&clientes, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, sizeof(enderecoRemetente));
			for (int i = 0; i < clientes; i++){
				sendto(meuSocket, (char*)&x[i], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, sizeof(enderecoRemetente));
				sendto(meuSocket, (char*)&y[i], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, sizeof(enderecoRemetente));
			}
			break;
		default:
			break;
		}
	}

	closesocket(meuSocket);
	WSACleanup();
	system("pause");
	return 0;
}