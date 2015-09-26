#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <chrono>

int MsgErro(char* msg){
	std::cout << "(" << msg << ")Error code: " << WSAGetLastError() << std::endl;
	return 1;
}

void TratarCliente(SOCKET clienteSocket, sockaddr_in clienteEndereco){
	std::cout << "Cliente conectado! IP: " << inet_ntoa(clienteEndereco.sin_addr)
		<< ":" << ntohs(clienteEndereco.sin_port) << std::endl;
	char buffer[1];
	while (true){
		int result = recv(clienteSocket, buffer, 1, 0);
		if (result == SOCKET_ERROR){
			MsgErro("recv");
			break;
		}
		std::cout << "\tCliente: " << inet_ntoa(clienteEndereco.sin_addr)
			<< ":" << ntohs(clienteEndereco.sin_port) << " = " << buffer[0] << std::endl;
		buffer[0] = toupper(buffer[0]);
		result = send(clienteSocket, buffer, 1, NULL);
		if (result == SOCKET_ERROR){
			MsgErro("send");
			break;
		}		
	}
	std::cout << "Cliente desconectado! IP: " << inet_ntoa(clienteEndereco.sin_addr)
		<< ":" << ntohs(clienteEndereco.sin_port) << std::endl;
}

int main(){

	WSADATA wsaData;
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0){
		return MsgErro("WSAStartup");
	}

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_addr.s_addr = inet_addr("0.0.0.0");
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

	while (true){
		SOCKET clienteSocket;
		clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);
		std::thread* th;
		th = new std::thread(TratarCliente, clienteSocket, clienteEndereco);
		
	}

	closesocket(principalSocket);

	WSACleanup();

	system("pause");
	return 0;
}