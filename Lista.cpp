#ifndef LISTA_CPP
#define LISTA_CPP

#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/mouse.h>
#include "menuItem.h"
#include "Lista.h"

MenuLista::MenuLista() { //Construtor, inicializa primeiro e ultimo como NULL
	F = NULL;
}

MenuItem* MenuLista::get(int pos) { //Retorna ponteiro para o noh desejado de acordo com a sua chave
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


void MenuLista::insere(MenuItem* novoItem) { //Insere um noh a lista
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

void MenuLista::imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) { //Imprime a lista
	MenuItem* aux;
	int i = 0;
	aux = F->next;
	while (aux != NULL) {
		al_draw_filled_rectangle(475, i, 675, i + 35, al_map_rgb(200, 200, 200));
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, i,
			ALLEGRO_ALIGN_CENTRE, aux->titulo());
		aux = aux->next;
		i = i + 40;
	}


}

int MenuLista::total() { //retorna o total de elementos na lista
	MenuItem* aux;
	int i = 0;
	aux = F->next;
	while (aux != NULL) {
		aux = aux->next;
		i++;
	}
	return i;
}

Lista::Lista() {
	ch = 0;
	prox = NULL;
}

Lista* Lista::get(int x){
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

void Lista::insere(int x) {
	Lista* novo = new Lista;
	Lista* aux = this;
	while (aux->prox != NULL) {
		aux = aux->prox;
	}
	aux->prox = 
}

#endif