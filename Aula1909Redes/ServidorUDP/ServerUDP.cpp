#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#define MAX_PACKET_SIZE 512

struct Mensagem {
	char* ip;
	char* mensagem;
};

std::vector<Mensagem*> mensagens;

void ReceberMensagens(SOCKET servidorSocket){
	char *buffer = new char[MAX_PACKET_SIZE];
	sockaddr_in enderecoCliente;
	int enderecoClienteTam = sizeof(enderecoCliente);

	while (true){
		int recebido;
		recebido = recvfrom(servidorSocket, buffer, MAX_PACKET_SIZE, NULL, (sockaddr*)&enderecoCliente, &enderecoClienteTam);

		Mensagem* m = new Mensagem;
		m->ip = new char[16];
		inet_ntop(AF_INET, &enderecoCliente.sin_addr, m->ip, 16);
		//m->porta = new char[5];
		////strcpy_s(m->porta, (char*)htons(enderecoCliente.sin_port));
		//strcpy_s(m->porta, 5, (char*)htons(enderecoCliente.sin_port));
		m->mensagem = new char[MAX_PACKET_SIZE];
		memcpy(m->mensagem, buffer, MAX_PACKET_SIZE);
		mensagens.push_back(m);
	}

}

int main(){
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in meuEndereco;
	meuEndereco.sin_addr.s_addr = INADDR_ANY;
	meuEndereco.sin_family = AF_INET;
	meuEndereco.sin_port = htons(9999);

	SOCKET meuSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(meuSocket, (sockaddr*)&meuEndereco, sizeof(meuEndereco));

	std::thread* th;
	th = new std::thread(ReceberMensagens, meuSocket);

	for (;;){
		std::this_thread::sleep_for(std::chrono::seconds(5));

		for (unsigned int i = 0; i < mensagens.size(); i++){
			std::cout << mensagens[i]->ip << std::endl
				<< mensagens[i]->mensagem << std::endl;
		}
		mensagens.clear();
	}

	closesocket(meuSocket);
	WSACleanup();
	system("pause");
	return 0;
}