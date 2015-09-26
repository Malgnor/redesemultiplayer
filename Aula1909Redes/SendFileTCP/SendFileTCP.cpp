#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
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
	enderecoServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	//enderecoServidor.sin_addr.s_addr = inet_addr("10.96.26.75");
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);

	result = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

	if (result == SOCKET_ERROR){
		return MsgErro("conenct");
	}

	FILE *img = fopen("img.jpg", "rb");
	fseek(img, 0, SEEK_END);
	unsigned long filesize = ftell(img);
	char *buffer = (char*)malloc(sizeof(char)*filesize);
	rewind(img);
	fread(buffer, sizeof(char), filesize, img);

	send(meuSocket, (char*)&filesize, sizeof(filesize), 0);
	std::cout << filesize << std::endl;
	int sent = 0;
	while (sent < filesize)
	{
		int n = send(meuSocket, buffer + sent, filesize - sent, 0);
		if (n == -1)
			break;

		sent += n;
	}

	delete buffer;

	closesocket(meuSocket);

	WSACleanup();

	system("pause");
	return 0;
}