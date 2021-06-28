#ifndef FUNCOES_AUX_CPP
#define FUNCOES_AUX_CPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

//Verifica se o usuario clicou em uma regiao definida por x1, x2, y1, y2
//  (x1,y2) --- (x2,y2)
//     |           |
//  (x1,y1) --- (x2,y1)
//
//Retorna true se usuario clicou na regiao
bool clicouRegiao(int x1, int y1, int x2, int y2, ALLEGRO_EVENT pointer) {
	if (pointer.mouse.x >= x1 && pointer.mouse.x <= x2 && pointer.mouse.y >= y1 && pointer.mouse.y <= y2)
		return true;
	else
		return false;
}

//Imprime um botao na "altura" i
//  +--------+ 
//  | botao1 | i = 1
//  +--------+
//  +--------+
//  | botao2 | i = 2
//  +--------+
void drawBotao(ALLEGRO_FONT* fonte, const char* titulo, int i) {

	al_draw_filled_rectangle(475, i * 40 + 40, 675, i * 40 + 75, al_map_rgb(200, 200, 200));
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, i * 40 + 40, ALLEGRO_ALIGN_CENTRE, titulo);
}

//Verifica se um usuario clicou em um botao criado pela funcao drawBotao
//Retorna a "altura" i do botao
int clicouBotao(ALLEGRO_EVENT pointer) {
	int x = pointer.mouse.x - 40;
	int y = pointer.mouse.y - 40;
	if (x >= 475 && x <= 675 && y >= 0) {
		return y / 40;
	}
	return -1;
}
#endif
