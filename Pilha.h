#ifndef PILHA_H
#define PILHA_H

class Node {
private:
	int valor;
	Node* prox;
	friend class Pilha;
public:
	Node();
};

class Pilha {
private:
	Node* topo;
public:
	Pilha(); //Construtor
	void empilha(int x); //Adiciona um nó com o valor fornecido ao topo da pilha
	int desempilha(); //Retira um nó do topo da pilha, retorna o valor dele
	bool vazia(); //Verifica se a pilha está vazia
	~Pilha(); //Destrutor
};

#endif