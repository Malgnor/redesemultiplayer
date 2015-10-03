#include "ClienteJogo.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <exception>

#define PORTA_SERVIDOR 9000

enum Comandos {
	NUMERO_ULTIMA_VERSAO_CLIENTE_REQ,
	EXECUTAR_ULTIMAR_VERSAO_CLIENTE_REQ
};

int MsgErro(char* msg){
	std::cout << "(" << msg << ")Error code: " << WSAGetLastError() << std::endl;
	system("pause");
	return 1;
}

void receberTcp(const SOCKET& socket, void* buffer, const long& tamanhoBuffer){
	long bytesRecebidos = 0;
	long totalBytesRecebidos = 0;

	do {
		bytesRecebidos = recv(socket, (char*)buffer + totalBytesRecebidos, tamanhoBuffer - totalBytesRecebidos, NULL);

		if (bytesRecebidos == SOCKET_ERROR || bytesRecebidos == 0){
			std::stringstream erro;
			erro << __FUNCTION__
				<< " - A conexão foi encerrado durante a transmissão de forma inesperada. "
				<< WSAGetLastError()
				<< std::endl;

			throw std::runtime_error(erro.str());
		}

		totalBytesRecebidos += bytesRecebidos;
	} while (totalBytesRecebidos != tamanhoBuffer);
}

int main(){

	try{
		ClienteJogo* ultimoCliente = new ClienteJogo();

		WSADATA wsaData;
		int result;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (result != 0){
			return MsgErro("WSAStartup");
		}

		sockaddr_in enderecoServidor;
		inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);
		enderecoServidor.sin_family = AF_INET;
		enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

		SOCKET principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (principalSocket == INVALID_SOCKET){
			return MsgErro("socket");
		}

		result = bind(principalSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

		if (result == SOCKET_ERROR){
			return MsgErro("bind");
		}

		result = listen(principalSocket, SOMAXCONN);

		if (result == SOCKET_ERROR){
			return MsgErro("listen");
		}

		sockaddr_in clienteEndereco;
		int clienteEnderecoTam = sizeof(clienteEndereco);

		SOCKET clienteSocket;

		for (;;){
	
			clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);

			for (;;){
				try{
					int comando;
					int ultimaVersaoCliente;
					receberTcp(clienteSocket, &comando, sizeof(int));

					switch (comando)
					{
					default:
						break;
					}
				}
				catch (std::exception& ex){
					std::cout << ex.what();
					break;
				}
			}
		}

		int 

		closesocket(principalSocket);

		WSACleanup();
	}
	catch (std::exception& ex){
		std::cout << ex.what();
		system("pause");
		return 1;
	}
	return 0;
}