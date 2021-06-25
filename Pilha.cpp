#ifndef PILHA_CPP
#define PILHA_CPP

#include <iostream>
#include "Pilha.h"

Node::Node(){
	valor = 0;
	prox = NULL;
}

//Construtor
Pilha::Pilha() { 
	topo = NULL;
};

//Adiciona um n� entre a Cabe�a e o pr�ximo n� (Push)
void Pilha::empilha(int x) { 
	Node* novo = new Node;
	novo->valor = x;
	novo->prox = topo;
	topo = novo;
}
//Remove o n� logo ap�s a Cabe�a, retorna o valor dele (Pop)
int Pilha::desempilha() { 
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

//Verifica se a Pilha est� vazia (Cabe�a aponta para NULL), retorna valor Booleano
bool Pilha::vazia() { 
	return topo == NULL;
}
//Destrutor, destroi todos os elementos da pilha
Pilha::~Pilha() {
	Node* aux;
	while (!vazia()) {
		aux = topo->prox;
		delete topo;
		topo = aux;
	}
}

#endif
