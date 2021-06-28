#ifndef FUNCOESAUX_H
#define FUNCOESAUX_H

#include "allegro5/allegro.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

bool clicouRegiao(int bX1, int bY1, int bX2, int bY2, ALLEGRO_EVENT pointer); //Retorna se o usuario clicou em uma regiao
void drawBotao(ALLEGRO_FONT* fonte, const char* titulo, int i); //Desenha um botao no "indice" i
int clicouBotao(ALLEGRO_EVENT pointer); //Retorna o "indice" do botao que o usuario clicou
#endif