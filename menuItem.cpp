#ifndef MENUITEM_CPP
#define MENUITEM_CPP

#include <iostream>
#include <sstream>
#include <string>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro.h>
#include <allegro5/mouse.h>
#include "tabuleiro.h"
#include "menuItem.h"
#include "Pilha.h"
#include "funcoesAux.h"

//Define as funcoes do menu, permitindo implementacao rapida e individual

class MenuPadrao : public MenuItem { //Caso default, eh o primeiro elemento da lista
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
	}
	virtual const char const* titulo() {
		return "";
	}
};

class MenuNovoJogo : public MenuItem { //Cria um novo jogo
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x = al_show_native_message_box(tela, "Novo jogo", "Deseja criar um novo jogo?", "Seu jogo atual sera perdido", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		if (x == 1) {
			delete jogoAtual;
			jogoAtual = new Tabuleiro();
			jogoAtual->geraTabuleiro(tela, fonte);
		}
	}
	virtual const char const* titulo() {
		return "Novo Jogo";
	}
};

class MenuInserir : public MenuItem { //Insere um valor no tabuleiro
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x, y ,i ,j;

		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_keyboard_event_source());

		x = y = -1;
		while (true) {
			al_clear_to_color(al_map_rgb(230, 230, 230));
			if (x != -1) {
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
			}
			jogoAtual->imprime(tela, fonte);
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Inserindo");
			al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado e digite um numero para inserir");
			drawBotao(fonte, "Voltar", 9);
			al_flip_display();
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN &&
				evento.type != ALLEGRO_EVENT_KEY_DOWN);
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						if (clicouRegiao(j * 50, i * 50, (j + 1) * 50, (i + 1) * 50, evento)) {
							x = j;
							y = i;
						}
					}
				}
			}
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN && x != -1) {
				if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
					jogoAtual->insere(evento.keyboard.keycode - 27, x, y);
				}
				if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
					jogoAtual->insere(evento.keyboard.keycode - 37, x, y);
				}
			}
			if (clicouBotao(evento) == 9) {
				al_destroy_event_queue(filaDeEvento);
				return;
			}
		}
	}
	virtual const char const* titulo() {
		return "Inserir celula";
	}
};

class MenuRemover : public MenuItem { //Remove um valor do tabuleiro
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x, y;

		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());

		al_clear_to_color(al_map_rgb(230, 230, 230));
		jogoAtual->imprime(tela, fonte);
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Removendo");
		al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para remove-lo");

		drawBotao(fonte, "Voltar", 9);
		al_flip_display();
		al_flush_event_queue(filaDeEvento);
		while (true) {
			al_clear_to_color(al_map_rgb(230, 230, 230));
			jogoAtual->imprime(tela, fonte);
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Removendo");
			al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para remove-lo");

			drawBotao(fonte, "Voltar", 9);
			al_flip_display();

			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
			
			for (y = 0; y < 9; y++) {
				for (x = 0; x < 9; x++) {
					if (clicouRegiao(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, evento)) {
						jogoAtual->apaga(x, y);
					}
				}
			}
			if (clicouBotao(evento) == 9) {
				al_destroy_event_queue(filaDeEvento);
				return;
			}
		}
	}
	virtual const char const* titulo() {
		return "Retirar celula";
	}
};

class MenuVerificar : public MenuItem { //Verifica se uma posicao esta correta
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x, y;

		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());

		al_clear_to_color(al_map_rgb(230, 230, 230));
		jogoAtual->imprime(tela, fonte);
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Verificando");
		al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para ver se e valido");

		drawBotao(fonte, "Voltar", 9);
		al_flip_display();
		al_flush_event_queue(filaDeEvento);
		while (true) {
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
			if (clicouBotao(evento) == 9) {
				al_destroy_event_queue(filaDeEvento);
				return;
			}
			for (y = 0; y < 9; y++) {
				for (x = 0; x < 9; x++) {
					if (clicouRegiao(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, evento)) {
						al_clear_to_color(al_map_rgb(230, 230, 230));
						al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Verificando");
						al_draw_filled_rectangle(475, 400, 675, 435, al_map_rgb(200, 200, 200));
						al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 320, ALLEGRO_ALIGN_CENTRE, "Voltar");

						if (jogoAtual->valido(x, y)) {
							al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 200));
							jogoAtual->imprime(tela, fonte);
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Valido!");
							al_flip_display();
						}
						else {
							al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(240, 200, 200));
							jogoAtual->imprime(tela, fonte);
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Invalido");
							al_flip_display();
						}
					}
				}
			}
		}
	}
	virtual const char const* titulo() {
		return "Verificar celula";
	}
};

class MenuCreditos : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());

		al_clear_to_color(al_map_rgb(230, 230, 230));
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 40, ALLEGRO_ALIGN_LEFT, "Feito por:");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 80, ALLEGRO_ALIGN_LEFT, "Alexandre dos Santos Gualberto");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 120, ALLEGRO_ALIGN_LEFT, "Guilherme Calca");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 160, ALLEGRO_ALIGN_LEFT, "Matheus Rezende Milani Videira");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 200, ALLEGRO_ALIGN_LEFT, "Para a disciplina:");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 240, ALLEGRO_ALIGN_LEFT, "Algoritmos e Estruturas de Dados 1");


		drawBotao(fonte, "Voltar", 9);
		al_flip_display();
		al_flush_event_queue(filaDeEvento);
		while (true) {
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
			if (clicouBotao(evento) == 9) {
				al_destroy_event_queue(filaDeEvento);
				return;
			}
		}
	}
	virtual const char const* titulo() {
		return "Creditos";
	}
};

class MenuResolver : public MenuItem { //Resolve automaticamente o Sudoku
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		bool semUt0 = false;
		int semUt1 = 0;
		jogoAtual->resolveTabuleiro(&semUt0, &semUt1, 0, true, tela, fonte);
	}
	virtual const char const* titulo() {
		return "Auto-resolver";
	}
};

class MenuVerificarTudo : public MenuItem { //Verifica se todas as posicoes estao corretas
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x, y;
		for (y = 0; y < 9; y++) {
			for (x = 0; x < 9; x++) {
				jogoAtual->valido(x, y);
			}
		}
	}
	virtual const char const* titulo() {
		return "Exibir erros";
	}
};

class MenuAnota : public MenuItem {
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x, y, i, j, valor, intAux;
		bool sair;
		Pilha* aux;
		std::string texto;
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_keyboard_event_source());

		x = y = -1;
		al_flush_event_queue(filaDeEvento);
		while (true) {
			al_clear_to_color(al_map_rgb(230, 230, 230));
			jogoAtual->imprime(tela, fonte);
			if (x == -1 || y == -1) {
				al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
				al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para ver e editar suas anotacoes");
			}
			else {
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
				al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotado:");

				drawBotao(fonte, "Adicionar", 7);
				drawBotao(fonte, "Excluir", 8);

				texto = "";
				aux = new Pilha;
				while (!jogoAtual->anotaVazia(x, y)) {
					valor = jogoAtual->anotaDesempilha(x, y);
					if (valor != 0) {
						texto = texto + std::to_string(valor) + ", ";
						aux->empilha(valor);
					}
				}
				while (!aux->vazia()) {
					jogoAtual->anotaEmpilha(aux->desempilha(), x, y);
				}
				delete aux;
				al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, texto.c_str());

				al_flip_display();

			}

			jogoAtual->imprime(tela, fonte);
			drawBotao(fonte, "Voltar", 9);
			al_flip_display();

			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);

			for (i = 0; i < 9; i++) {
				for (j = 0; j < 9; j++) {
					if (clicouRegiao(j * 50, i * 50, (j + 1) * 50, (i + 1) * 50, evento)) {
						x = j;
						y = i;
					}
				}
			}
			switch (clicouBotao(evento)) {
			case 7:
				if (x != -1) {
					sair = false;
					while (clicouBotao(evento) != 9 && !sair) {
						al_clear_to_color(al_map_rgb(230, 230, 230));
						al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
						jogoAtual->imprime(tela, fonte);
						al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
						al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Digite um numero para anota-lo");
						al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 240, 200, 40, 0, texto.c_str());
						drawBotao(fonte, "Voltar", 9);
						al_flip_display();
						al_flush_event_queue(filaDeEvento);
						do {
							al_wait_for_event(filaDeEvento, &evento);
						} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN &&
							evento.type != ALLEGRO_EVENT_KEY_DOWN);
						if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
							if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
								jogoAtual->anotaEmpilha(evento.keyboard.keycode - 27, x, y);
								texto = texto + std::to_string(evento.keyboard.keycode - 27) + ", ";
							}
							else if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
								jogoAtual->anotaEmpilha(evento.keyboard.keycode - 37, x, y);
								texto = texto + std::to_string(evento.keyboard.keycode - 37) + ", ";
							}
						}
						if (clicouBotao(evento) == 9) {
							sair = true;
						}
					}
				}
				break;

			case 8:
				if (x != -1) {
					sair = false;
					while (clicouBotao(evento) != 9 && !sair) {
						al_clear_to_color(al_map_rgb(230, 230, 230));
						al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
						jogoAtual->imprime(tela, fonte);
						al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
						al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Digite um numero para remove-lo");
						al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 240, 200, 40, 0, texto.c_str());
						drawBotao(fonte, "Voltar", 9);
						al_flip_display();
						al_flush_event_queue(filaDeEvento);
						do {
							al_wait_for_event(filaDeEvento, &evento);
						} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN &&
							evento.type != ALLEGRO_EVENT_KEY_DOWN);
						if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
							valor = 0;
							if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
								jogoAtual->anotaEmpilha(evento.keyboard.keycode - 27, x, y);
								valor = evento.keyboard.keycode - 27;
							}
							else if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
								valor = evento.keyboard.keycode - 37, x, y;
							}
							if (valor != 0) {
								aux = new Pilha;
								texto = "";
								while (!jogoAtual->anotaVazia(x, y)) {
									intAux = jogoAtual->anotaDesempilha(x, y);
									if (valor != intAux && valor != 0) {
										texto = texto + std::to_string(intAux) + ", ";
										aux->empilha(intAux);
									}
								}
								while (!aux->vazia()) {
									jogoAtual->anotaEmpilha(aux->desempilha(), x, y);
								}
								delete aux;
							}

						}
					}
				}
				break;
			case 9: al_destroy_event_queue(filaDeEvento);
				return;
			default:
				break;
			}
		}
	}
	virtual const char const* titulo() {
		return "Anotacoes";
	}
};

class MenuSair : public MenuItem { //Sai do programa
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte) {
		int x = al_show_native_message_box(tela, "Sair", "Deseja sair do programa?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		if (x == 1) {
			delete jogoAtual;
			exit(0);
		}
	}
	virtual const char const* titulo() {
		return "Sair";
	}
};

#endif