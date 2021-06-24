#ifndef PILHA_CPP
#define PILHA_CPP

#include <iostream>
#include "Pilha.h"


Pilha::Pilha() {
	topo = NULL;
};


void Pilha::empilha(int x) { //Adiciona um n� entre a Cabe�a e o pr�ximo n� (Push)
	Node* novo = new Node;
	novo->valor = x;
	novo->prox = topo;
	topo = novo;
}

int Pilha::desempilha() { //Remove o n� logo ap�s a Cabe�a, retorna o valor dele (Pop)
	Node* aux;
	int x;
	if (vazia()) {
		return 0;
	}
	x = topo->valor;
	aux = topo->prox;
	delete topo;
	topo = aux;
	return x;
}

bool Pilha::vazia() { //Verifica se a Pilha est� vazia (Cabe�a aponta para NULL), retorna valor Booleano
	return topo == NULL;
}

Pilha::~Pilha() {
	Node* aux;
	while (!vazia()) {
		aux = topo->prox;
		delete topo;
		topo = aux;
	}
}

#endif
