#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\prepared_statement.h>
#include <cppconn\resultset.h>
#include <cppconn\exception.h>

#include <iostream>

int main(){

	std::string login;
	std::string senha;

	std::cout << "Informe seu login: ";
	std::getline(std::cin, login);

	std::cout << "Informe sua senha: ";
	std::getline(std::cin, senha);

	try{
		sql::Driver* driver;
		driver = get_driver_instance();

		sql::Connection* conexão;
		conexão = driver->connect("localhost", "root", "root");

		conexão->setSchema("jogodb");

		sql::PreparedStatement* ps;
		ps = conexão->prepareStatement("SELECT * FROM Usuarios WHERE Login = ? AND Senha = ?");
		ps->setString(1, login.c_str());
		ps->setString(2, senha.c_str());

		sql::ResultSet* rs;
		rs = ps->executeQuery();

		if (rs->next()){
			std::cout << "Usuario valido!" << std::endl;
		}
		else {
			std::cout << "Usuario e/ou senha invalidos" << std::endl;
		}

		conexão->close();
	}
	catch (sql::SQLException& ex){
		std::cout << "Ocorreu um erro ao conectar ao db: " << ex.what();
	}
	system("pause");
	return 0;
}