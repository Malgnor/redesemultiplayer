#include "Teste.h"
#include <cstring>
#include <iostream>


Teste::Teste()
{
}

Teste::Teste(int _a, float _b, double _c, char _d[10])
	: a(_a), b(_b), c(_c)
{
	strcpy(d, _d);
}

void Teste::print(){
	std::cout << "A: " << a << std::endl
		<< "B:" << b << std::endl
		<< "C:" << c << std::endl
		<< "D: " << d << std::endl;
}


Teste::~Teste()
{
}
