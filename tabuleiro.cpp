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

//Define as bibliotecas corretas, e unifica a funcao sleep()
#if defined(__linux__)
#  include <unistd.h>
#elif defined(_WIN32)
#  include <windows.h>
#  define sleep(s) Sleep((s)*1000)
#endif

#include "Pilha.h"
#include "tabuleiro.h"

//Construtor, cria um tabuleiro vazio
Tabuleiro::Tabuleiro() {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			tabuleiro[i][j].setValor(0);
			tabuleiro[i][j].setErro(false);
			tabuleiro[i][j].setEditavel(true);
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
	
	//Imprime uma tela de espera enquanto gera o tabuleiro
	al_clear_to_color(al_map_rgb(230, 230, 230));
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 350, 40, ALLEGRO_ALIGN_CENTER, "Gerando tabuleiro");
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 350, 80, ALLEGRO_ALIGN_CENTER, "Por favor aguarde");
	al_flip_display();

	//Garante que o tabuleiro esteja com todos os valores zerados
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			tabuleiro[i][j].setValor(0);
			tabuleiro[i][j].setErro(false);
			tabuleiro[i][j].setEditavel(true);
		}
	}

	//Cria um tabuleiro valido completo
	resolveTabuleiro(&terminou, &quantSol, false, false, NULL, NULL, NULL);

	//Retira numeros aleatoriamente ate ficar com o minimo possivel para uma unica solucao
	//Utiliza simetria para garantir mais eficiencia ao remover os valores, já que dificulta a criacao de mais de 1 solucao
	//Mesmo que o tabuleiro fique perceptivelmente simetrico
	while (numeroRemovido < 51 && iteracao < 75) { //Numero maximo de iteracoes para impedir um laco muito grande
		//Copia o tabuleiro atual para um temporario, que sera modificado
		for (y = 0; y < 9; y++) {
			for (x = 0; x < 9; x++) {
				temp.insere(tabuleiro[y][x].getValor(), x, y);
			}
		}
		//Gera aleatoriamente a posicao dos valores que serao removidos 
		quantSol = 0;
		x = rand() % 9;
		y = rand() % 9;
		//Armazena os valores a serem removidos, para devolve-los caso criem mais de 1 solucao
		valor1 = tabuleiro[y][x].getValor();
		valor2 = tabuleiro[8 - y][8 - x].getValor();
		//Remove o primeiro valor do tabuleiro do jogo e do temporario
		if (valor1 != 0) {
			tabuleiro[y][x].setValor(0);
			tabuleiro[y][x].setEditavel(true);
			temp.apaga(x, y);
			numeroRemovido++;

		}
		//Remove o segundo valor do tabuleiro do jogo e do temporario
		if (valor2 != 0) {
			tabuleiro[8 - y][8 - x].setValor(0);
			tabuleiro[8 - y][8 - x].setEditavel(true);
			temp.apaga(8 - x, 8 - y);
			numeroRemovido++;
		}
		//Caso tenha removido algum valor, verifica a quantidade de solucoes
		if (valor1 != 0 || valor2 != 0) {
			terminou = false;
			temp.resolveTabuleiro(&terminou, &quantSol, true, false, NULL, NULL, NULL); //Para cada numero removido, procura 2 solucoes
		}
		//Se criou mais de 1 solucao, devolve o valor
		if (quantSol >= 2 && valor1 != 0) {
			tabuleiro[y][x].setValor(valor1);
			tabuleiro[y][x].setEditavel(false);
			numeroRemovido--;
		}
		//Se criou mais de 1 solucao, devolve o valor
		if (quantSol >= 2 && valor2 != 0) {
			tabuleiro[8 - y][8 - x].setValor(valor2);
			tabuleiro[8 - y][8 - x].setEditavel(false);
			numeroRemovido--;
		}
		iteracao++;
	}
}


//Metodo para resolver, completar ou criar um tabuleiro de maneira valida
// modo = false para criar uma solucao
// modo = true para contar solucoes
// imprime, variavel de controle para imprimir o progresso do tabuleiro
//Utiliza backtracking para chegar a uma solucao
// TAD Pilha usado para armazenar os numeros impossiveis de serem utilizados
void Tabuleiro::resolveTabuleiro(bool* terminou, int* quantSol, bool modo, bool imprimir, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2) {
	int i, x, y;
	int aux;

	short int nroPossivel[] = { 1, 2, 3 , 4, 5, 6, 7, 8, 9 };

	//Para cada posicao do tabuleiro
	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			//Se o valor for diferente de 0 e for editavel, limpa o valor
			//Retira os numeros inseridos pelo usuario, que podem impedir o algoritmo de
			//chegar a uma solucao
			if (tabuleiro[y][x].getValor() != 0 && tabuleiro[y][x].getEditavel()) {
				tabuleiro[y][x].setValor(0);
				tabuleiro[y][x].setErro(false);
			}
			//Se o quadrado estiver vazio
			if (tabuleiro[y][x].getValor() == 0) {
				//Cria uma Pilha de todos os numeros que estao na mesma linha, coluna e quadrado 3x3 que o atual
				Pilha* nroImpossivel = new Pilha;
				getImpossivel(nroImpossivel, x, y);
				//Desempilha esses numeros, removendo eles do vetor de numeros possiveis
				while (!nroImpossivel->vazia()) { 
					aux = nroImpossivel->desempilha();
					nroPossivel[aux - 1] = 0;
				}
				delete nroImpossivel;
				//Embaralha o vetor de numeros possiveis para aleatoriedade na hora de gerar um tabuleiro
				std::random_shuffle(&nroPossivel[0], &nroPossivel[8]);
				//Para cada numero do vetor e, enquanto nao terminou ou enquanto nao achou 2 solucoes
				for (i = 0; (i < 9) && (!modo && !*terminou) || (modo && (*quantSol < 2)); i++) {
					//Se o numero do vetor nroPossivel for diferente de 0, insere no tabuleiro e chama a funcao novamente
					if (nroPossivel[i] != 0) {
						tabuleiro[y][x].setValor(nroPossivel[i]);
						tabuleiro[y][x].setEditavel(modo);
						if (imprimir) {
							al_clear_to_color(al_map_rgb(230, 230, 230));
							imprime(tela, fonte, fonte2);
							al_flip_display();
							sleep(0.1);
						}
						resolveTabuleiro(terminou, quantSol, modo, imprimir, tela, fonte, fonte2); //Recursividade 
						//Caso tenha saido da recursao
						//mas nao tenha terminado ou achado mais de 2 solucoes
						//Torna a posicao do tabuleiro disponivel novamente
						if ((!modo && !*terminou) || (modo && (*quantSol < 2))) {
							tabuleiro[y][x].setValor(0);
							tabuleiro[y][x].setEditavel(true);
						}
					}
				}
				//Apos tentar todos os valores possiveis na posicao
				return; //fim do caminho, retornar recursividade
			}
		}
	}
	//Percorreu todo o tabuleiro e nao achou um em branco
	*terminou = true; //flag para indicar que o tabuleiro esta completo
	*quantSol = *quantSol + 1; //quantidade de solucoes
};


//Retorna uma Pilha de todos os numeros que estao na mesma linha, coluna e quadrado 3x3 que o atual
Pilha* Tabuleiro::getImpossivel(Pilha* pilha, int x, int y) {
	int i, j, x1, y1;
	//Na mesma linha
	for (i = 0; i < 9; i++) {
		if (tabuleiro[y][i].getValor() != 0) {
			pilha->empilha(tabuleiro[y][i].getValor());
		}
	}
	//Na mesma coluna
	for (i = 0; i < 9; i++) {
		if (tabuleiro[i][x].getValor() != 0) {
			pilha->empilha(tabuleiro[i][x].getValor());
		}
	}
	//No mesmo quadrado 3x3
	x1 = (x / 3) * 3;
	y1 = (y / 3) * 3;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (tabuleiro[y1 + i][x1 + j].getValor() != 0) { 
				pilha->empilha(tabuleiro[y1 + i][x1 + j].getValor());
			}
		}
	}
	return pilha;
}

//Criado para simplificar ifs de protecao de dado
//Retorna true caso x e y estejam entre [0:8]
bool Tabuleiro::checkLimite(int x, int y) { 
	return (x >= 0 && x <= 8 && y >= 0 && y <= 8);
}

//Insere um valor em um quadrado
//Retorna true caso insira e false caso contrario
bool Tabuleiro::insere(int valor, int x, int y) {
	int i;
	//Verifica se os argumentos estao dentro dos valores esperados
	// e se o quadrado desejado eh editavel
	if (checkLimite(x, y) && tabuleiro[y][x].getEditavel()) {
		//Insere o valor
		tabuleiro[y][x].setValor(valor);
		tabuleiro[y][x].setErro(false);
		//Caso exista uma anotacao no quadrado, destroi ela
		if (!tabuleiro[y][x].getVazia()) {
			tabuleiro[y][x].destructAnotacao();
		}
		return true;
	}
	return false;
}

//Apaga um valor de uma posicao
//Retorna true caso apague, false caso contrario
bool Tabuleiro::apaga(int x, int y) {
	//Verifica se os argumentos estao dentro dos valores esperados
	// e se o quadrado desejado eh editavel
	if (checkLimite(x, y) && tabuleiro[y][x].getEditavel()) {
		tabuleiro[y][x].setValor(0);
		tabuleiro[y][x].setErro(false);
		return true;
	}
	return false;
}

//Imprime o tabuleiro e as coordenadas
void Tabuleiro::imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2) {
	int x, y, i;
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
			al_draw_text(fonte, al_map_rgb(0, 0, 0), x * 50 + 25.5, y * 50 + 7,
				ALLEGRO_ALIGN_CENTRE, texto.c_str());
			if (tabuleiro[y][x].getValor() == 0 && !tabuleiro[y][x].getVazia()) {
				for (i = 1; i <= 9; i++) {
					if (tabuleiro[y][x].anotado(i)) {

						al_draw_text(fonte2, al_map_rgb(100, 100, 100), x * 50 + ((i % 3 == 0) ? 2 * 15 : ((i % 3) - 1) * 15) + 10, y * 50 + ((i - 1) / 3) * 15,
							ALLEGRO_ALIGN_CENTRE, std::to_string(i).c_str());
					}
				}
			}
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

void Tabuleiro::anotaRemove(int valor, int x, int y) {
	if (checkLimite(x, y)) {
		tabuleiro[y][x].apaga(valor);
	}
}
bool Tabuleiro::anotaInsere(int valor, int x, int y) {
	if (checkLimite(x, y) && valor >= 1 && valor <= 9) {
		return tabuleiro[y][x].anotar(valor);
	}
	return false;
}
bool Tabuleiro::anotaContem(int valor, int x, int y) {
	if (checkLimite(x, y) && valor >= 1 && valor <= 9) {
		return tabuleiro[y][x].anotado(valor);
	}
	return false;
}

std::string Tabuleiro::anotaString(int x, int y) {
	if (checkLimite(x, y)) {
		return tabuleiro[y][x].anotaString();
	}
	return "";
}

Tabuleiro::~Tabuleiro() {
	int x, y;
	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			tabuleiro[x][y].destructAnotacao();
		}
	}
}


Quadrado::Quadrado() {
	valor = 0;
	editavel = true;
	erro = false;
	anotacao = NULL;
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
	if (anotacao != NULL) {
		return anotacao->vazia();
	}
	return true;
}
void Quadrado::apaga(int x) {
	if (anotacao == NULL) {
		return;
	}
	if (anotacao->vazia()) {
		delete anotacao;
		anotacao = NULL;
		return;
	}
	anotacao->apaga(x);


}
bool Quadrado::anotar(int x) {
	if (anotacao == NULL) {
		anotacao = new Lista;
	}
	return anotacao->insere(x);
}

bool Quadrado::anotado(int x) {
	if (anotacao == NULL) {
		return false;
	}
	return anotacao->naLista(x);
}

int Quadrado::quantAnotacao() {
	if (anotacao == NULL) {
		return 0;
	}
	return anotacao->quantElemento();
}

std::string Quadrado::anotaString() {
	if (anotacao == NULL) {
		return "";
	}
	return anotacao->paraString();
}

void Quadrado::destructAnotacao() {
	if (anotacao != NULL) {
		delete anotacao;
		anotacao = NULL;
	}
}

Quadrado::~Quadrado() {
	destructAnotacao();
	valor = 0;
	editavel = erro = false;
}

#endif