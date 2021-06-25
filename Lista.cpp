#ifndef LISTA_CPP
#define LISTA_CPP

#include <iostream>
#include <string>
#include <allegro5/allegro_primitives.h>
#include "menuItem.h"
#include "Lista.h"
#include "funcoesAux.h"

//Construtor
MenuLista::MenuLista() { 
	F = NULL;
}

//Retorna ponteiro para o noh desejado de acordo com a sua chave
MenuItem* MenuLista::get(int pos) {
	MenuItem* itemTemp = F;
	int i = 0;

	if (F == NULL) {
		return NULL;
	}

	while (i != pos && (itemTemp->next != NULL)) {
		itemTemp = itemTemp->next;
		i++;
	}

	if (i == pos) {
		return itemTemp;
	}
	return F; //Retorna a primeira posicao (Caso default) caso o noh desejado nao exista
}

//Insere um noh no final da lista
void MenuLista::insere(MenuItem* novoItem) { 
	MenuItem* aux;
	if (F == NULL) {
		F = novoItem;
		F->next = NULL;
	}
	else {
		aux = F;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		novoItem->next = NULL;
		aux->next = novoItem;
	}
}

//Imprime a lista em forma de botoes
void MenuLista::imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) { 
	MenuItem* aux = F;
	int i = 0;
	while (aux != NULL) {
		drawBotao(fonte, aux->titulo(), i);
		aux = aux->next;
		i++;
	}


}

//retorna o total de elementos na lista
int MenuLista::total() { 
	MenuItem* aux = F;
	int i = 0;
	while (aux != NULL) {
		aux = aux->next;
		i++;
	}
	return i;
}

//Construtor
Lista::Lista() {
	ch = 0;
	prox = NULL;
}

//Retorna o ponteiro para o noh da lista com a chave indicada
//Retorna NULL caso nao exista
Lista* Lista::get(int x) {
	Lista* aux;
	if (prox == NULL) {
		return NULL;
	}
	aux = this;
	while (aux->prox != NULL && x != ch) {
		aux = aux->prox;
	}
	if (x == ch) {
		return aux;
	}
	return NULL;
}

//Insere uma chave na lista, de maneira que a lista fique ordenada
//Retorna true caso realize a insercao com sucesso
bool Lista::insere(int x) {
	Lista* posterior = this->prox;
	Lista* anterior = this;
	Lista* novo = new Lista;
	novo->ch = x;
	if (anterior == NULL) {
		return false;
	}
	while (posterior != NULL && x > posterior->ch) {
		anterior = anterior->prox;
		posterior = posterior->prox;
	}
	if (posterior != NULL && x == posterior->ch) {
		return false;
	}
	novo->prox = posterior;
	anterior->prox = novo;
	return true;
}

//Apaga um elemento da lista
void Lista::apaga(int x) {
	Lista* posterior;
	Lista* anterior = this;
	Lista* alvo = this->prox;
	if (anterior == NULL) {
		return;
	}
	while (alvo != NULL && x > alvo->ch) {
		anterior = anterior->prox;
		alvo = alvo->prox;
	}
	if (alvo != NULL && x == alvo->ch) {
		posterior = alvo->prox;
		delete alvo;
		anterior->prox = posterior;
	}
}

//Retorna true caso uma chave esteja na lista,
//false caso contrario
bool Lista::naLista(int x) {
	Lista* aux = this;
	while (aux != NULL && x > aux->ch) {
		aux = aux->prox;
	}
	if (aux == NULL) {
		return false;
	}
	if (x == aux->ch) {
		return true;
	}
	return false;
}

//Retorna se a lista esta vazia ou nao
bool Lista::vazia() {
	return prox == NULL;
}

//Retorna a quantidade de elementos na lista
int Lista::quantElemento() {
	int i = 0;
	Lista* aux = this;
	while (aux != NULL) {
		aux = aux->prox;
		i++;
	}
	return i;
}

//Converte os elementos da lista para uma string
//Cada elemento eh separado por um espaço
//retorna a strig
std::string Lista::paraString() {
	std::string texto = "";
	Lista* aux = this->prox;
	while (aux != NULL) {
		texto = texto + std::to_string(aux->ch) + " ";
		aux = aux->prox;
	}
	return texto;
}

//Destrutor
Lista::~Lista() {
	ch = 0;
	if (prox != NULL) {
		delete prox;
	}
	prox = NULL;
}
#endif