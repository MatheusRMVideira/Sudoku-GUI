#ifndef FUNCOES_AUX_CPP
#define FUNCOES_AUX_CPP

#include "allegro5/allegro.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>


bool clicouRegiao(int bX1, int bY1, int bX2, int bY2, ALLEGRO_EVENT pointer) {
	if (pointer.mouse.x >= bX1 && pointer.mouse.x <= bX2 && pointer.mouse.y >= bY1 && pointer.mouse.y <= bY2)
		return 1;
	else
		return 0;
}

void drawBotao(ALLEGRO_FONT* fonte, const char* titulo, int i) {

	al_draw_filled_rectangle(475, i * 40 + 40, 675, i * 40 + 75, al_map_rgb(200, 200, 200));
	al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, i * 40+40, ALLEGRO_ALIGN_CENTRE, titulo);
}

int clicouBotao(ALLEGRO_EVENT pointer) {
	int x = pointer.mouse.x - 40;
	int y = pointer.mouse.y - 40;
	if (x >= 475 && x <= 675 && y >= 0) {
		return y / 40;
	}
	return -1;
}
#endif