#include <allegro5/display.h>
#include "Lista.h"
#include "Pilha.h"
#ifndef TABULEIRO_H
#define TABULEIRO_H


class Quadrado {
private:
	int valor;
	bool editavel; //Se eh possivel ou nao modificar o quadrado
	bool erro; //Se o quadrado esta errado
	Lista* anotacao;
public:
	Quadrado(); //Constructor
	int getValor(); //Retorna o valor do quadrado
	void setValor(int x); //Salva valor no quadrado
	bool getEditavel(); //Retorna editavel
	void setEditavel(bool x); //Salva editavel
	bool getErro(); //Retorna erro
	void setErro(bool x); //Salva erro
	bool getVazia(); //Retorna se anotacao esta vazia
	void apaga(int x); //Apaga um valor da anotacao
	bool anotar(int x); //Salva um valor na anotacao
	bool anotado(int x); //Retorna se um valor esta na anotacao
	int quantAnotacao(); //Retorna a quantidade de valores na anotacao
	std::string anotaString(); //Retorna uma string contendo os valores da anotacao
	void destructAnotacao(); //Destroi a anotacao
	~Quadrado(); //Destructor
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
	//Metodo para resolver, completar ou criar um tabuleiro de maneira valida
	// modo = false para criar uma solucao
	// modo = true para contar solucoes
	void resolveTabuleiro(bool* terminou, int* quantSol, bool modo, bool imprimir, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2);
	void imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2); //Imprime o tabuleiro
	bool valido(int x, int y); //Retorna se uma posicao eh valida
	bool anotaVazia(int x, int y); //Retorna se anotacao de uma posicao esta vazia
	void anotaRemove(int valor, int x, int y); //Remove um valor da anotacao de uma posicao
	bool anotaInsere(int valor, int x, int y); //Insere um valor na anotacao de uma posicao
	bool anotaContem(int valor, int x, int y); //Retorna se a anotacao de uma posicao contem um valor
	std::string anotaString(int x, int y); //Retorna uma string contendo os valores da anotacao de uma posicao
	~Tabuleiro(); //Destructor
};

#endif
