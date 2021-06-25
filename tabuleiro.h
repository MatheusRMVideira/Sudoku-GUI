#include <allegro5/display.h>
#include "Lista.h"
#include "Pilha.h"
#ifndef TABULEIRO_H
#define TABULEIRO_H


class Quadrado {
private:
	int valor;
	bool editavel; //Se eh possivel ou nao modificar o quadrado
	bool erro; //Se o quadrado esta errado;
	Lista* anotacao;
public:
	Quadrado();
	int getValor();
	void setValor(int x);
	bool getEditavel();
	void setEditavel(bool x);
	bool getErro();
	void setErro(bool x);
	bool getVazia();
	void apaga(int x);
	bool anotar(int x);
	bool anotado(int x);
	int quantAnotacao();
	std::string anotaString();
	void destructAnotacao();
	~Quadrado();
};


class Tabuleiro {
private:
	Quadrado tabuleiro[9][9]; //Tabuleiro que eh jogado
	bool checkLimite(int x, int y); //Verifica se x e y estao entre 0 e 8
	Pilha* getImpossivel(Pilha* pilha, int x, int y);
public:
	Tabuleiro();
	bool insere(int valor, int x, int y); //Insere um valor no tabuleiro
	bool apaga(int x, int y); //Apaga um valor do tabuleiro
	void geraTabuleiro(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte); //Gera um novo tabuleiro valido
	void resolveTabuleiro(bool* terminou, int* quantSol, bool modo, bool imprimir, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2);
	void imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2);
	bool valido(int x, int y); //Verifica se uma posicao eh valida
	bool anotaVazia(int x, int y);
	void anotaRemove(int valor, int x, int y);
	bool anotaInsere(int valor, int x, int y);
	bool anotaContem(int valor, int x, int y);
	std::string anotaString(int x, int y);
	~Tabuleiro();
};

#endif
