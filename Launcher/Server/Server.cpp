#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define VERSAO_CLIENTE_ATUAL 1

int MsgErro(char* msg){
	std::cout << "(" << msg << ")Error code: " << WSAGetLastError() << std::endl;
	system("pause");
	return 1;
}

int main(){


	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0){
		return MsgErro("WSAStartup");
	}

	sockaddr_in enderecoServidor;
	inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

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
	clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);

	int versaoCliente;
	int bytesRecebidos;
	int totalBytesRecebidos = 0;
	do{
		bytesRecebidos = recv(clienteSocket, (char*)&versaoCliente + totalBytesRecebidos, sizeof(int) - totalBytesRecebidos, NULL);

		if (bytesRecebidos == SOCKET_ERROR || bytesRecebidos == 0){
			return MsgErro("recv");
		}

		totalBytesRecebidos += bytesRecebidos;

	} while (totalBytesRecebidos != sizeof(int));

	if (VERSAO_CLIENTE_ATUAL != versaoCliente){
		FILE* file;
		int e;
		char path[255];
		sprintf_s(path, "wow%d.exe", VERSAO_CLIENTE_ATUAL);
		e = fopen_s(&file, path, "r");

		if (e == 0){
			return 1;
		}

	}

	closesocket(principalSocket);

	WSACleanup();

	system("pause");
	return 0;
}