#ifndef _CLIENTEJOGO_H_
#define _CLIENTEJOGO_H_

class ClienteJogo {
private:
	char* m_dados;
	long m_tamanho;
	int m_versao;
	void CarregarDadosDoDisco();
	void CarregarVersaoDoDisco();
public:
	ClienteJogo();
	char* Dados() const;
	long Tamanho() const;
	int Versao() const;
	~ClienteJogo();
};

#endif