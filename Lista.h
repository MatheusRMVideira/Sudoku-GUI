
#ifndef LISTA_H
#define LISTA_H

class MenuItem;

class MenuLista {
private:
	MenuItem* F; //Ponteiro para o primeiro noh da Fila
public:
	MenuLista(); //Construtor
	MenuItem* get(int pos); //Retorna ponteiro para o noh desejado de acordo com a sua chave
	void insere(MenuItem* novoItem); //Insere um noh a lista
	void imprime(ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte); //Imprime a lista
	int total();
};

class Lista {
private:
	int ch;
	Lista* prox; //Ponteiro para o primeiro noh da Fila
public:
	Lista(); //Construtor
	Lista* get(int x); //Retorna ponteiro para o noh desejado de acordo com a sua chave
	void insere(int x); //Insere um noh a lista
	void retira(int x); //Retira um elemento da lista
	bool naLista(int x); //Retorna se uma chave esta ou nao na lsita
	bool vazia(); //Retorna se a lista esta vazia ou nao
	int quantElemento(); //Retorna a quantidade de elemento na lista
};
#endif