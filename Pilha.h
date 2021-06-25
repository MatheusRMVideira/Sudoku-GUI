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
	void empilha(int x); //Adiciona um n� com o valor fornecido ao topo da pilha
	int desempilha(); //Retira um n� do topo da pilha, retorna o valor dele
	bool vazia(); //Verifica se a pilha est� vazia
	~Pilha(); //Destrutor
};

#endif