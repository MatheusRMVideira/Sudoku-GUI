#include <allegro5/display.h>
#ifndef TABULEIRO_H
#define TABULEIRO_H

class Pilha;

class Quadrado {
private:
	int valor;
	bool editavel; //Se eh possivel ou nao modificar o quadrado
	bool erro; //Se o quadrado esta errado;
	Pilha* anota;
public:
	Quadrado();
	int getValor();
	void setValor(int x);
	bool getEditavel();
	void setEditavel(bool x);
	bool getErro();
	void setErro(bool x);
	bool getVazia();
	int getPilhaValor();
	void setPilhaValor(int x);
};


class Tabuleiro {
private:
	Quadrado tabuleiro[9][9]; //Tabuleiro que eh jogado
	int castCharToInt(char y); //Converte char [a;i] para int [0;8]
	bool checkLimite(int x, int y); //Verifica se x e y estao entre 0 e 8
	Pilha* getImpossivel(Pilha* pilha, int x, int y);
public:
	Tabuleiro();
	bool insere(int valor, int x, int y); //Insere um valor no tabuleiro
	bool apaga(int x, int y); //Apaga um valor do tabuleiro
	void geraTabuleiro(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte); //Gera um novo tabuleiro valido
	void resolveTabuleiro(bool* terminou, int* quantSol, int modo, bool imprimir, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte);
	void imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte);
	bool valido(int x, int y); //Verifica se uma posicao eh valida
	bool anotaVazia(int x, int y);
	int anotaDesempilha(int x, int y);
	void anotaEmpilha(int valor, int x, int y);
};

#endif
