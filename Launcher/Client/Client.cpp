#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

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

	int minhaVersao = 1;
	result = send(meuSocket, (char*)&minhaVersao, sizeof(int), NULL);

	if (result == SOCKET_ERROR){
		return MsgErro("send");
	}

	int bytesRecebidos = 0;
	int totalBytesRecebidos = 0;
	int versaoAtualizada;

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}