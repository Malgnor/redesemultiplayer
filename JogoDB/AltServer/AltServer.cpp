#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <sstream>

#define PORTA_SERVIDOR 9000
#define CMD_AUTH_REQUEST 1
#define CMD_AUTH_SUCESS 2
#define CMD_AUTH_FAIL 3

void receberTcp(const SOCKET& socket, void* buffer, const long& tamanhoBuffer) {
    long bytesRecebidos = 0;
    long totalBytesRecebidos = 0;

    do {
        bytesRecebidos = recv(socket, (char*)buffer + totalBytesRecebidos, tamanhoBuffer - totalBytesRecebidos, NULL);

        if(bytesRecebidos == SOCKET_ERROR || bytesRecebidos == 0) {
            std::stringstream erro;
            erro << __FUNCTION__
                << " - A conexão foi encerrado durante a transmissão de forma inesperada. "
                << WSAGetLastError()
                << std::endl;

            throw std::runtime_error(erro.str());
        }

        totalBytesRecebidos += bytesRecebidos;
    } while(totalBytesRecebidos != tamanhoBuffer);
}

int main() {

    try {

        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        sockaddr_in enderecoServidor;
        inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);
        enderecoServidor.sin_family = AF_INET;
        enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

        SOCKET principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        bind(principalSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));

        listen(principalSocket, SOMAXCONN);

        sockaddr_in clienteEndereco;
        int clienteEnderecoTam = sizeof(clienteEndereco);

        SOCKET clienteSocket;

        for(;;) {

            clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);

            for(;;) {
                try {
                    int comando;
                    int size;
                    char* login;
                    char* senha;
                    receberTcp(clienteSocket, &comando, sizeof(int));

                    switch(comando) {
                        case CMD_AUTH_REQUEST:
                            receberTcp(clienteSocket, &size, sizeof(int));
                            login = new char[size+1];
                            receberTcp(clienteSocket, login, size);
                            login[size] = '\0';
                            receberTcp(clienteSocket, &size, sizeof(int));
                            senha = new char[size+1];
                            receberTcp(clienteSocket, senha, size);
                            senha[size] = '\0';
                            std::cout << "Login: " << login << "\tSenha: " << senha << std::endl;
                            comando = CMD_AUTH_SUCESS;
                            send(clienteSocket, (char*)&comando, sizeof(int), NULL);

                            delete login;
                            delete senha;
                            break;
                        default:
                            break;
                    }
                } catch(std::exception& ex) {
                    std::cout << ex.what();
                    break;
                }
            }
        }

        closesocket(principalSocket);

        WSACleanup();
    } catch(std::exception& ex) {
        std::cout << ex.what();
        system("pause");
        return 1;
    }

    return 0;
}