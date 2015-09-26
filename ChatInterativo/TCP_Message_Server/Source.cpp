#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

//#define _ASDF_

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
		std::cout << "Conectado!" << std::endl;

#ifdef _ASDF_
		char buffer[2];
		int recebido;
		do{
			recebido = recv(clienteSocket, buffer, 1, NULL);

			buffer[1] = '\0';

			if (recebido == SOCKET_ERROR || recebido == 0){
				std::cout << std::endl << "Desconectado!" << std::endl;
				break;
			}

			std::cout << buffer;

#else
		int recebido;
		do{
			int tam;
			recebido = recv(clienteSocket, (char*)&tam, sizeof(int), NULL);

			if (recebido == SOCKET_ERROR || recebido == 0){
				std::cout << std::endl << "Desconectado!" << std::endl;
				break;
			}
			char* buffer = new char[tam+1];
			recebido = recv(clienteSocket, buffer, tam, NULL);

			buffer[tam] = '\0';

			if (recebido == SOCKET_ERROR || recebido == 0){
				std::cout << std::endl << "Desconectado!" << std::endl;
				break;
			}

			std::cout << buffer;
			delete[] buffer;
#endif
		} while (1);
	} while (1);

	WSACleanup();

	return 0;
}