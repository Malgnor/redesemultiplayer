#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <Teste.h>

int main(){

	WSAData wsaDAta;
	WSAStartup(MAKEWORD(2, 2), &wsaDAta);

	SOCKET principalSocket;
	principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in enderecoServidor;
	//enderecoServidor.sin_addr.s_addr = inet_addr("0.0.0.0");
	enderecoServidor.sin_addr.s_addr = ADDR_ANY;
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	bind(principalSocket, (SOCKADDR*)&enderecoServidor, sizeof(enderecoServidor));

	listen(principalSocket, SOMAXCONN);

	sockaddr_in enderecoCliente;
	int enderecoClienteTam = sizeof(enderecoCliente);
	do{
		SOCKET clienteSocket;
		clienteSocket = accept(principalSocket, (SOCKADDR*)&enderecoCliente, &enderecoClienteTam);
		std::cout << "Cliente conectado!" << std::endl;

		Teste teste;
		if (recv(clienteSocket, (char*)&teste, sizeof(Teste), NULL) == SOCKET_ERROR){
			std::cout << "Error" << std::endl;
		}

		teste.print();


	} while (1);

		WSACleanup();

		return 0;
}