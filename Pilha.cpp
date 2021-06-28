#ifndef PILHA_CPP
#define PILHA_CPP

#include <iostream>
#include "Pilha.h"

//Constructor
Node::Node(){
	valor = 0;
	prox = NULL;
}

//Construtor
Pilha::Pilha() { 
	topo = NULL;
};

//Adiciona um nó entre a Cabeça e o próximo noh (Push)
void Pilha::empilha(int x) { 
	Node* novo = new Node;
	novo->valor = x;
	novo->prox = topo;
	topo = novo;
}
//Remove o noh logo apos a Cabeça, retorna o valor dele (Pop)
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

//Retorna se a pilha esta vazia
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
