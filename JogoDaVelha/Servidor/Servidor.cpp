#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define PORTA_SERVIDOR 9000
#define CMD_ENVIAR_JOGADA_REQUEST 1
#define CMD_ENVIAR_JOGADA_RESPOSTA 2

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

	SOCKET clienteSocket[2];
	sockaddr_in clienteEndereco;
	int clienteEnderecoTam = sizeof(sockaddr_in);

	for (;;){
		for (int i = 0; i < 2; i++){
			clienteSocket[i] = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);
		}

		int jogador = 0;
		int comando = CMD_ENVIAR_JOGADA_REQUEST;
		send(clienteSocket[jogador], (char*)&comando, sizeof(int), NULL);
		for (;;){

		}
	}

	WSACleanup();

	system("pause");
	return 0;
}