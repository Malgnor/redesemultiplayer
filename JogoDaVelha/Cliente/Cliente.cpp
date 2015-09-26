#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

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

	SOCKET meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (meuSocket == INVALID_SOCKET){
		return MsgErro("socket");
	}

	sockaddr_in enderecoServidor;
	inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	if (result == SOCKET_ERROR){
		return MsgErro("connect");
	}
	for (;;){
		int comando;
		recv(meuSocket, (char*)&comando, sizeof(int), NULL);
		switch (comando)
		{
		case CMD_ENVIAR_JOGADA_REQUEST:
			int jogada[2];
			std::cout << "Informe a coodernada x:" << std::endl;
			std::cin >> jogada[0];
			std::cout << "Informe a coodernada y:" << std::endl;
			std::cin >> jogada[1];
			comando = CMD_ENVIAR_JOGADA_RESPOSTA;
			send(meuSocket, (char*)&comando, sizeof(int), NULL);
			send(meuSocket, (char*)jogada, sizeof(int)*2, NULL);
			break;
		}


	}

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}