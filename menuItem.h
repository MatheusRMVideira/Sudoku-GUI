#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#ifndef MENUITEM_H
#define MENUITEM_H

class Tabuleiro;

class MenuItem { //Noh de um TAD Fila
private:
	MenuItem* next;
	friend class MenuLista;
public:
	virtual void comportamento(Tabuleiro* tabuleiro, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) = 0; //Comportamento do botao
	virtual char const* titulo() = 0; //Titulo do botao
};

#endif