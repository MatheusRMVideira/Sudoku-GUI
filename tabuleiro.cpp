#ifndef TABULEIRO_CPP
#define TABULEIRO_CPP

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#if defined(__linux__)
#  include <unistd.h>
#elif defined(_WIN32)
#  include <windows.h>
#  define sleep(s) Sleep((s)*1000)
#endif

#include "Pilha.h"
#include "tabuleiro.h"


Tabuleiro::Tabuleiro() {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			tabuleiro[i][j].setValor(0);
		}
	}
}

//Gera o tabuleiro a ser jogado
void Tabuleiro::geraTabuleiro(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
	int quantSol = 0;
	int x, y, valor1, valor2;
	srand(time(NULL));
	bool terminou = false;
	Tabuleiro temp;
	int numeroRemovido = 0;
	int iteracao = 0;

	al_clear_to_color(al_map_rgb(230, 230, 230));
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 350, 40, ALLEGRO_ALIGN_CENTER, "Gerando tabuleiro");
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 350, 80, ALLEGRO_ALIGN_CENTER, "Por favor aguarde");
	al_flip_display();

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			tabuleiro[i][j].setValor(0);
		}
	}

	resolveTabuleiro(&terminou, &quantSol, 0, false, NULL, NULL); //Cria um tabuleiro valido completo

	//Retira numeros aleatoriamente ate ficar com o minimo possivel para uma unica solucao
	quantSol = 0;
	while (numeroRemovido < 51 && iteracao < 75) {
		for (y = 0; y < 9; y++) {
			for (x = 0; x < 9; x++) {
				temp.insere(tabuleiro[y][x].getValor(), x, y);
			}
		}
		quantSol = 0;
		x = rand() % 9;
		y = rand() % 9;
		valor1 = tabuleiro[y][x].getValor();
		valor2 = tabuleiro[8 - y][8 - x].getValor();

		if (valor1 != 0) {
			tabuleiro[y][x].setValor(0);
			tabuleiro[y][x].setEditavel(true);
			temp.apaga(x, y);
			numeroRemovido++;

		}
		if (valor2 != 0) {
			tabuleiro[8 - y][8 - x].setValor(0);
			tabuleiro[8 - y][8 - x].setEditavel(true);
			temp.apaga(8 - x, 8 - y);
			numeroRemovido++;
		}
		if(valor1 != 0 || valor2 != 0){
			temp.resolveTabuleiro(&terminou, &quantSol, 1, false, NULL, NULL); //Para cada numero removido, procura 2 solucoes
		}

		if (quantSol >= 2 && valor1 != 0) {
			tabuleiro[y][x].setValor(valor1);
			tabuleiro[y][x].setEditavel(false);
			numeroRemovido--;
		}
		if(quantSol >= 2 && valor2 != 0){
			tabuleiro[8 - y][8 - x].setValor(valor2);
			tabuleiro[8 - y][8 - x].setEditavel(false);
			numeroRemovido--;
		}
		iteracao++;
	}
}


//Metodo para resolver, completar ou criar um tabuleiro de maneira valida
// modo = 0 para criar uma solucao e 1 para contar solucoes
//Utiliza backtracking para chegar a uma solucao
// TAD Pilha usado para armazenar os numeros impossiveis de serem utilizados
void Tabuleiro::resolveTabuleiro(bool* terminou, int* quantSol, int modo, bool imprimir, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
	int i, x, y;
	int aux;
	int nroPossivel[9];
	Pilha* nroImpossivel = new Pilha;

	for (i = 0; i < 9; i++) {
		nroPossivel[i] = i + 1;
	}

	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			if (tabuleiro[y][x].getValor() != 0 && tabuleiro[y][x].getEditavel() && !valido(x, y)) {
				tabuleiro[y][x].setValor(0);
				tabuleiro[y][x].setErro(false);
			}
			if (tabuleiro[y][x].getValor() == 0) { //Percorre todo o tabuleiro e procura por espacos vazios
				getImpossivel(nroImpossivel, x, y);
				while (!nroImpossivel->vazia()) { //Transforma numeros impossiveis, em um array de numeros possiveis
					aux = nroImpossivel->desempilha();
					nroPossivel[aux - 1] = 0;
				}
				std::random_shuffle(&nroPossivel[0], &nroPossivel[8]); //Embaralha array de numeros possiveis para aleatoriedade
				for (i = 0; (i < 9) && (((modo == 0) && !*terminou) || ((modo == 1) && (*quantSol < 2))); i++) { //Testa todos os numeros possiveis
					if (nroPossivel[i] != 0) {
						tabuleiro[y][x].setValor(nroPossivel[i]);
						tabuleiro[y][x].setEditavel(modo == 1);
						if (imprimir) {
							al_clear_to_color(al_map_rgb(230, 230, 230));
							imprime(tela, fonte);
							al_flip_display();
							sleep(0.1);
						}
						resolveTabuleiro(terminou, quantSol, modo, imprimir, tela, fonte); //Recursividade 
						if (((modo == 0) && !*terminou) || ((modo == 1) && (*quantSol < 2))) { //Volta posicao caso tenha esgotado todas as possibilidades seguintes
							tabuleiro[y][x].setValor(0);
							tabuleiro[y][x].setEditavel(true);
						}
					}
				}
				delete nroImpossivel;
				return; //fim do caminho, retornar recursividade
			}
		}
	}
	*terminou = true; //flag para indicar que o tabuleiro esta completo
	*quantSol = *quantSol + 1; //quantidade de solucoes
};


//Retorna Pilha contendo todos os numeros impossiveis para uma posicao
Pilha* Tabuleiro::getImpossivel(Pilha* pilha, int x, int y) {
	int i, j, x1, y1;

	for (i = 0; i < 9; i++) {
		if (tabuleiro[y][i].getValor() != 0) { //Linha
			pilha->empilha(tabuleiro[y][i].getValor());
		}
	}
	for (i = 0; i < 9; i++) {
		if (tabuleiro[i][x].getValor() != 0) { //Coluna
			pilha->empilha(tabuleiro[i][x].getValor());
		}
	}
	x1 = (x / 3) * 3;
	y1 = (y / 3) * 3;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (tabuleiro[y1 + i][x1 + j].getValor() != 0) { //Quadrado 3x3
				pilha->empilha(tabuleiro[y1 + i][x1 + j].getValor());
			}
		}
	}
	return pilha;
}

//Cast de char para int e reducao para [0;8]
//Utilizado pelos metodos insere(), apaga() e valido() com char y;
int Tabuleiro::castCharToInt(char y) {
	int intY;
	intY = (int)(y);
	intY = (intY >= 65 && intY <= 73) ? intY + 32 : intY; //converte de letra maiuscula para minuscula
	intY = intY - 97; //Reduz para [0;8]
	if (intY >= 0 && intY < 9) {
		return intY;
	}
	return -1;
}

//Criado para simplificar ifs de protecao de dado
bool Tabuleiro::checkLimite(int x, int y) { //int X [0;8]; int Y [0;8]
	return (x >= 0 && x <= 8 && y >= 0 && y <= 8);
}

//Insere um valor em uma posicao
bool Tabuleiro::insere(int valor, int x, int y) {
	if (checkLimite(x, y) && tabuleiro[y][x].getEditavel()) { //Editavel evita alterar posicoes criadas pelo programa
		tabuleiro[y][x].setValor(valor);
		tabuleiro[y][x].setErro(false);
		return true;
	}
	return false;
}

//Remove um valor de uma posicao
bool Tabuleiro::apaga(int x, int y) {

	if (checkLimite(x, y) && tabuleiro[y][x].getEditavel()) {
		tabuleiro[y][x].setValor(0);
		tabuleiro[y][x].setErro(false);
		return true;
	}
	return false;
}

//Imprime o tabuleiro e as coordenadas
void Tabuleiro::imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
	int x, y;
	std::string texto;

	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			if (!tabuleiro[y][x].getEditavel()) {
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 200, 200));
			}
			if (tabuleiro[y][x].getErro()) {
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgba(240, 200, 200, 255));
			}
			al_draw_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(0, 0, 0), 2);
			texto = (tabuleiro[y][x].getValor() == 0) ? "" : std::to_string(tabuleiro[y][x].getValor());
			al_draw_text(fonte, al_map_rgb(0, 0, 0), x * 50 + 25.5, y * 50+7,
				ALLEGRO_ALIGN_CENTRE, texto.c_str());
		}
	}

	al_draw_line((450 / 3), 450, (450 / 3), 0, al_map_rgb(0, 0, 0), 5);
	al_draw_line((2 * 450 / 3), 450, (2 * 450 / 3), 0, al_map_rgb(0, 0, 0), 5);
	al_draw_line(450, (450 / 3), 0, (450 / 3), al_map_rgb(0, 0, 0), 5);
	al_draw_line(450, (2 * 450 / 3), 0, (2 * 450 / 3), al_map_rgb(0, 0, 0), 5);

}

//Metodo similar a getImpossivel(), porem esta retorna se posicao eh valida
bool Tabuleiro::valido(int x, int y) {
	int i, j, x1, y1;
	int valor = tabuleiro[y][x].getValor();
	if (checkLimite(x, y) && valor >= 1 && valor <= 9 && tabuleiro[y][x].getEditavel()) {
		for (i = 0; i < 9; i++) {
			if (tabuleiro[y][i].getValor() == valor && i != x && !tabuleiro[y][i].getErro()) { //Linha
				tabuleiro[y][x].setErro(true);
				return false;
			}
		}
		for (i = 0; i < 9; i++) {
			if (tabuleiro[i][x].getValor() == valor && i != y && !tabuleiro[i][x].getErro()) { //Coluna
				tabuleiro[y][x].setErro(true);
				return false;
			}
		}
		x1 = (x / 3) * 3;
		y1 = (y / 3) * 3;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (tabuleiro[y1 + i][x1 + j].getValor() == valor && y1 + i != y && x1 + j != x && !tabuleiro[y1 + i][x1 + j].getErro()) { //Quadrado 3x3
					tabuleiro[y][x].setErro(true);
					return false;
				}
			}
		}
		tabuleiro[y][x].setErro(false);
		return true;
	}
	return false;
}

bool Tabuleiro::anotaVazia(int x, int y) {
	if (checkLimite(x, y)) {
		return tabuleiro[y][x].getVazia();
	}
	return true;
}

int Tabuleiro::anotaDesempilha(int x, int y) {
	if (checkLimite(x, y)) {
		return tabuleiro[y][x].getPilhaValor();
	}
	return 0;
}
void Tabuleiro::anotaEmpilha(int valor, int x, int y) {
	if (checkLimite(x, y) && valor >= 1 && valor <= 9) {
		return tabuleiro[y][x].setPilhaValor(valor);
	}
}



Quadrado::Quadrado() {
	valor = 0;
	editavel = true;
	erro = false;
	anota = NULL;
}

int Quadrado::getValor() {
	return valor;
}

void Quadrado::setValor(int x) {
	if (x >= 0 && x <= 9) {
		valor = x;
		erro = false;
	}
}

bool Quadrado::getEditavel() {
	return editavel;
}

void Quadrado::setEditavel(bool x) {
	editavel = x;
}

bool Quadrado::getErro() {
	return erro;
}

void Quadrado::setErro(bool x) {
	erro = x;
}

bool Quadrado::getVazia() {
	if (anota != NULL) {
		return anota->vazia();
	}
	return true;
}
int Quadrado::getPilhaValor() {
	if (anota == NULL) {
		return 0;
	}
	if (anota->vazia()) {
		delete anota;
		anota = NULL;
		return 0;
	}
	return anota->desempilha();

}
void Quadrado::setPilhaValor(int x) {
	if (anota == NULL) {
		anota = new Pilha;
	}
	anota->empilha(x);
}

#endif