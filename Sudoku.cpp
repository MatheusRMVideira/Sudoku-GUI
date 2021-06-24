/*  Alexandre dos Santos Gualberto
	Guilherme Calca
	Matheus Rezende Milani Videira
*/

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/mouse.h>

#include "tabuleiro.h"
#include "Lista.h"
#include "menuItem.cpp"
#include "funcoesAux.h"

int pegaEntrada(ALLEGRO_EVENT pointer, MenuLista* lista);

int main(int argc, char** argv) {
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();
	al_install_keyboard();

	ALLEGRO_DISPLAY* tela = NULL;
	ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
	ALLEGRO_EVENT evento;
	tela = al_create_display(700, 480);
	al_set_window_title(tela, "Sudoku");
	filaDeEvento = al_create_event_queue();

	//al_register_event_source(filaDeEvento, al_get_display_event_source(tela));
	al_register_event_source(filaDeEvento, al_get_mouse_event_source());
	
	ALLEGRO_FONT* fonte = al_load_font("Ubuntu-Regular.ttf", 30, NULL);
	if (!fonte) {
		#if defined(__linux__)
			al_show_native_message_box(tela, "Erro ao carregaor fonte", "Nao foi possivel carregar a fonte",
				                       "Baixe a fonte Ubuntu-regular em https://fonts.google.com/specimen/Ubuntu e extraia no mesmo diretorio deste programa",
										NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return 0;
		#elif defined(_WIN32)
			std::string pathDir = "/Fonts/arial.ttf";
			fonte = al_load_font(strcat(getenv("windir"), pathDir.c_str()), 30, NULL);
			if (!fonte) {
				al_show_native_message_box(tela, "Erro ao carregaor fonte", "Nao foi possivel carregar a fonte",
					"Baixe a fonte Ubuntu-regular em https://fonts.google.com/specimen/Ubuntu e extraia no mesmo diretorio deste programa",
					NULL, ALLEGRO_MESSAGEBOX_ERROR);
				return 0;
			}
		#endif
	}
	Tabuleiro* jogo = new Tabuleiro();
	MenuLista* menu = new MenuLista();

	jogo->geraTabuleiro(tela, fonte);
	menu->insere(new MenuPadrao); //Usado para prevenir execucao de ponteiro NULL
	menu->insere(new MenuNovoJogo); //Cria um novo jogo
	menu->insere(new MenuInserir); //Insere o numero em um quadrado
	menu->insere(new MenuAnota); //Anota numeros em um quadrado
	menu->insere(new MenuRemover); //Remove o numero de um quadrado
	menu->insere(new MenuVerificar); //Verifica se o valor de um quadrado esta correto
	menu->insere(new MenuVerificarTudo); //Verifica se todos os quadrados estao corretos
	menu->insere(new MenuResolver); //Resolve automaticamente o sudoku atual
	menu->insere(new MenuCreditos); //Creditos
	menu->insere(new MenuSair); //Termina a execucao do programa


	int entrada;

	while (true) {
		al_clear_to_color(al_map_rgb(230, 230, 230));
		jogo->imprime(tela, fonte);
		menu->imprime(tela, fonte);
		al_flip_display();
		al_flush_event_queue(filaDeEvento);
		do {
			al_wait_for_event(filaDeEvento, &evento);
		} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && 
				 evento.type != ALLEGRO_EVENT_DISPLAY_SWITCH_IN &&
				 evento.type != ALLEGRO_EVENT_DISPLAY_FOUND &&
				 evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE);
		if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			entrada = pegaEntrada(evento, menu) + 1;
			menu->get(entrada)->comportamento(jogo, tela, fonte);
		}
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			delete jogo;
			delete menu;
			al_destroy_event_queue(filaDeEvento);
			al_destroy_font(fonte);
			al_destroy_display(tela);
			return 0;
		}
	}
}

int pegaEntrada(ALLEGRO_EVENT pointer, MenuLista* lista) {
	int i;
	int total = lista->total();
	for (i = 0; i < total; i++) {
		if (clicouRegiao(475, i * 40, 675, i * 40 + 35, pointer)) {
			return i;
		}
	}
	return -1;
}
