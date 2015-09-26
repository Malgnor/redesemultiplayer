#pragma once
class Teste
{
public:
	Teste();
	Teste(int _a, float _b, double _c, char _d[10]);
	void print();
	~Teste();
private:
	int a;
	float b;
	double c;
	char d[10];
};

