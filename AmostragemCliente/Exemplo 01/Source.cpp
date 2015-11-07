#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>

#include <c2d2/chien2d2.h>
#include <c2d2/chienaudio2.h>

#include <time.h>

int main(int ac, char **av)
{
	srand((unsigned)time(NULL));
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockaddr_in meuEndereco;
	meuEndereco.sin_addr.s_addr = INADDR_ANY;
	meuEndereco.sin_family = AF_INET;
	meuEndereco.sin_port = htons(9000 + (rand()%1000));

	SOCKET meuSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(meuSocket, (sockaddr*)&meuEndereco, sizeof(meuEndereco));

	sockaddr_in destinatario;
	destinatario.sin_addr.s_addr = inet_addr("10.96.26.85");
	destinatario.sin_family = AF_INET;
	destinatario.sin_port = htons(8888);
	sockaddr_in enderecoRemetente;
	int enderecoRemetenteTam = sizeof(enderecoRemetente);

	int comando = 0;
	int cliente;
	int clientes = 0;
	int x[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int y[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	sendto(meuSocket, (char*)&comando, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
	int r = recvfrom(meuSocket, (char*)&cliente, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);

	C2D2_Inicia(800, 600, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Amostragem");
	
	C2D2_Botao *teclado;
	teclado = C2D2_PegaTeclas();
	C2D2_Mouse *mouse;
	mouse = C2D2_PegaMouse();

	C2D2_TrocaCorLimpezaTela(0, 120, 0);

	unsigned int cursorSprite;
	cursorSprite  = C2D2_CarregaSpriteSet("cursor.png", 25, 25);
	unsigned long proximaAtualizacaoFrame = C2D2_TempoSistema() + 100;



	while (!teclado[C2D2_ESC].pressionado)
	{
	
		if (proximaAtualizacaoFrame < C2D2_TempoSistema())
		{
			proximaAtualizacaoFrame = C2D2_TempoSistema() + 100;
			comando = 1;
			sendto(meuSocket, (char*)&comando, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
			sendto(meuSocket, (char*)&cliente, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
			sendto(meuSocket, (char*)&mouse->x, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
			sendto(meuSocket, (char*)&mouse->y, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
			comando = 2;
			sendto(meuSocket, (char*)&comando, sizeof(int), NULL, (sockaddr*)&destinatario, sizeof(destinatario));
			recvfrom(meuSocket, (char*)&clientes, sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
			for (int i = 0; i < clientes; i++){
				recvfrom(meuSocket, (char*)&x[i], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
				recvfrom(meuSocket, (char*)&y[i], sizeof(int), NULL, (sockaddr*)&enderecoRemetente, &enderecoRemetenteTam);
			}

		}

		C2D2_LimpaTela();
		C2D2_DesenhaSprite(cursorSprite,
						   0, mouse->x, mouse->y);

		for (int i = 0; i < clientes; i++){
			if (i+1 == cliente) continue;
			C2D2_DesenhaSprite(cursorSprite,
				0, x[i], y[i]);
		}
		
		C2D2_Sincroniza(C2D2_FPS_PADRAO);		
	}

	C2D2_RemoveSpriteSet(cursorSprite);

	C2D2_Encerra();

	closesocket(meuSocket);
	WSACleanup();
	return 0;
}