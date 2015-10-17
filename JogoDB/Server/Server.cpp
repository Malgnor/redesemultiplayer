#include <WinSock2.h>
#include <WS2tcpip.h>

#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\prepared_statement.h>
#include <cppconn\resultset.h>
#include <cppconn\exception.h>

#include <iostream>
#include <sstream>

#define PORTA_SERVIDOR 9000
#define CMD_AUTH_REQUEST 1

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
		sql::Driver* driver;
		driver = get_driver_instance();

		sql::Connection* conexão;
		conexão = driver->connect("localhost", "root", "root");

		conexão->setSchema("jogodb");

		try{

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

			for (;;){

				clienteSocket = accept(principalSocket, (sockaddr*)&clienteEndereco, &clienteEnderecoTam);

				for (;;){
					try{
						int comando;
						int size;
						char* login;
						char* senha;
						receberTcp(clienteSocket, &comando, sizeof(int));

						switch (comando)
						{
						case CMD_AUTH_REQUEST:
							receberTcp(clienteSocket, &size, sizeof(int));
							login = new char[size];
							receberTcp(clienteSocket, login, size);
							receberTcp(clienteSocket, &size, sizeof(int));
							senha = new char[size];
							receberTcp(clienteSocket, senha, size);

							sql::PreparedStatement* ps;
							ps = conexão->prepareStatement("SELECT * FROM Usuarios WHERE Login = ? AND Senha = ?");

							ps->setString(1, login);
							ps->setString(2, senha);

							sql::ResultSet* rs;
							rs = ps->executeQuery();

							if (rs->next()){
								std::cout << "Usuario valido!" << std::endl;
							}
							else {
								std::cout << "Usuario e/ou senha invalidos" << std::endl;
							}

							break;
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

		conexão->close();
	} catch (sql::SQLException& ex){
		std::cout << "Ocorreu um erro ao conectar ao db: " << ex.what();
		system("pause");
		return 1;
	}

	return 0;
}