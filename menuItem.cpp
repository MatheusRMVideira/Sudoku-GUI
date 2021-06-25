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

//Cria um novo jogo
class MenuNovoJogo : public MenuItem { 
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		//Exibe mensagem de confirmacao, cria novo jogo caso usuario deseje
		int x = al_show_native_message_box(tela, "Novo jogo", "Deseja criar um novo jogo?", "Seu jogo atual sera perdido", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		if (x == 1) {
			jogoAtual->geraTabuleiro(tela, fonte);
		}
	}
	//Retorna o titulo do botao
	virtual char const* titulo() {
		return "Novo Jogo";
	}
};

//Menu para inserir valores no tabuleiro
class MenuInserir : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		int x, y, i, j;

		//Cria uma nova fila de eventos para a captura de informacoes
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_keyboard_event_source());
		al_register_event_source(filaDeEvento, al_get_display_event_source(tela));

		x = y = -1;
		while (true) {
			//Imprime o display
			al_clear_to_color(al_map_rgb(230, 230, 230));
			if (x != -1) { //Caso um quadrado esteja selecionado, imprime ele com uma cor diferente
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
			}
			jogoAtual->imprime(tela, fonte, fonte2);
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Inserindo");
			al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado e digite um numero para inserir");
			drawBotao(fonte, "Voltar", 9);
			al_flip_display();

			//Espera a entrada de informacoes
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //Usuario clicou
				evento.type != ALLEGRO_EVENT_KEY_DOWN && //Usuario apertou uma tecla
				evento.type != ALLEGRO_EVENT_DISPLAY_SWITCH_IN && //Usuario voltou a janela do programa
				evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //usuario fechou a janela do programa

			//Caso o usuario tenha clicado, descobre o quadrado que foi clicado
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						if (clicouRegiao(j * 50, i * 50, (j + 1) * 50, (i + 1) * 50, evento)) {
							//x e y sao os indices correspondentes ao quadrado selecionado pelo usuario
							x = j;
							y = i;
						}
					}
				}
			}
			//Caso o usuario tenha apertado uma tecla
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN && x != -1) {
				//Caso tenha apertado uma tecla de numero acima das letras (com ids [28:36])
				if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
					//Converte de [28:36] para [1:9] e insere no quadrado selecionado previamente
					jogoAtual->insere(evento.keyboard.keycode - 27, x, y);
				}
				//Caso tenha apertado uma tecla de numero no teclado numerico (com ids [38:46])
				if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
					//Converte de [38:46] para [1:9] e insere no quadrado selecionado previamente
					jogoAtual->insere(evento.keyboard.keycode - 37, x, y);
				}
			}
			//Caso o usuario tenha clicado no botao de voltar ou no botao para fechar a janela
			if (clicouBotao(evento) == 9 || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				al_destroy_event_queue(filaDeEvento);
				//seta fechar em verdadeiro caso o usuario tenha clicado para fechar a janela
				*fechar = (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE);
				return; //retorna ao menu principal
			}
		}
	}
	//Retorna o titulo do botao
	virtual char const* titulo() {
		return "Inserir celula";
	}
};

//Menu para remover valor do tabuleiro
class MenuRemover : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		int x, y;

		//Cria uma nova fila de eventos para a captura de informacoes
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_display_event_source(tela));

		while (true) {
			//Imprime o display
			al_clear_to_color(al_map_rgb(230, 230, 230));
			jogoAtual->imprime(tela, fonte, fonte2);
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Removendo");
			al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para remove-lo");
			drawBotao(fonte, "Voltar", 9);
			al_flip_display();

			//Espera a entrada de informacoes
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario tenha clicado
				evento.type != ALLEGRO_EVENT_DISPLAY_SWITCH_IN && //O usuario tenha voltado a janela
				evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario tenha clicado para fechar a janela

			//Caso o usuario tenha clicado
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				//Verifica se ele clicou em algum quadrado
				for (y = 0; y < 9; y++) {
					for (x = 0; x < 9; x++) {
						//Caso tenha clicado em algum quadrado, apaga o valor dele
						if (clicouRegiao(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, evento)) {
							jogoAtual->apaga(x, y);
						}
					}
				}
			}
			//Caso o usuario clicou no botao para voltar ou clicou para fechar a janela
			if (clicouBotao(evento) == 9 || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				al_destroy_event_queue(filaDeEvento);
				//Seta fechar para verdadeiro caso clicou para fechar a janela
				*fechar = (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE);
				return; //Retorna para o menu principal
			}
		}
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Retirar celula";
	}
};

//Verifica se o valor de um quadrado eh valido
class MenuVerificar : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		int x, y;

		//Inicializa nova fila de eventos
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_display_event_source(tela));

		//Imprime o display
		al_clear_to_color(al_map_rgb(230, 230, 230));
		jogoAtual->imprime(tela, fonte, fonte2);
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Verificando");
		al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para ver se e valido");
		drawBotao(fonte, "Voltar", 9);
		al_flip_display();

		while (true) {
			//Espera a entrada de informacoes
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario clicou
				evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario clicou para fechar a janela

			//Caso o usuario tenha fecha clicado
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				//Procura o quadrado que foi clicado
				for (y = 0; y < 9; y++) {
					for (x = 0; x < 9; x++) {
						//Caso o quadrado foi clicado
						if (clicouRegiao(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, evento)) {
							//Imprime parte lateral do display
							al_clear_to_color(al_map_rgb(230, 230, 230));
							al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Verificando");
							drawBotao(fonte, "Voltar", 9);
							//Verifica se o valor do quadrado eh valido
							if (jogoAtual->valido(x, y)) {
								//Se for valido, imprime quadrado com a cor verde e a palavra "Valido!"
								al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 200));
								al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Valido!");
								
							}
							else {
								//Se for invalido, imprime quadrado com a cor vermelha e a palavra "Invalido"
								al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(240, 200, 200));
								al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Invalido");
							}
							//Imprime o resto do tabuleiro e exibe o display
							jogoAtual->imprime(tela, fonte, fonte2);
							al_flip_display();
						}
					}
				}
			}
			//Caso o usuario clicou no botao para voltar ou para fechar a janela
			if (clicouBotao(evento) == 9 || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				al_destroy_event_queue(filaDeEvento);
				//Seta fechar como verdadeiro caso clicou para fechar a janela
				*fechar = (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE);
				return; //Retorna ao menu principal
			}
		}
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Verificar celula";
	}
};

//Imprime os creditos da realizacao do programa
class MenuCreditos : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		//Inicializa a fila de evento
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_display_event_source(tela));

		//Imprime o display com o nome dos membros da equipe
		al_clear_to_color(al_map_rgb(230, 230, 230));
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 40, ALLEGRO_ALIGN_LEFT, "Feito por:");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 80, ALLEGRO_ALIGN_LEFT, "Alexandre dos Santos Gualberto");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 120, ALLEGRO_ALIGN_LEFT, "Guilherme Calca");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 160, ALLEGRO_ALIGN_LEFT, "Matheus Rezende Milani Videira");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 200, ALLEGRO_ALIGN_LEFT, "Para a disciplina:");
		al_draw_text(fonte, al_map_rgb(0, 0, 0), 25, 240, ALLEGRO_ALIGN_LEFT, "Algoritmos e Estruturas de Dados 1");
		drawBotao(fonte, "Voltar", 9);
		al_flip_display();

		while (true) {
			//Espera a entrada de informacoes
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario clicou
				evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario clicou para fechar a janela

			//Caso o usuario clicou no botao para voltar ou para fechar a janela
			if (clicouBotao(evento) == 9 || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				al_destroy_event_queue(filaDeEvento);
				//Seta fechar como verdadeiro caso o usuario clicou para fechar a janela
				*fechar = (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE);
				return; //retorna para o menu principal
			}
		}
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Creditos";
	}
};

//Resolve automaticamente o Sudoku
class MenuResolver : public MenuItem { 
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		bool semUt0 = false;
		int semUt1 = 0;
		//Funcao para resolver o sudoku
		jogoAtual->resolveTabuleiro(&semUt0, &semUt1, false, true, tela, fonte, fonte2);
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Auto-resolver";
	}
};

//Verifica se o valor de todos os quadrados estao corretos
class MenuVerificarTudo : public MenuItem {
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		int x, y;
		//Para todos os quadrados, verifica se o valor esta correto
		for (y = 0; y < 9; y++) {
			for (x = 0; x < 9; x++) {
				jogoAtual->valido(x, y);
			}
		}
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Exibir erros";
	}
};

class MenuAnota : public MenuItem {
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {
		int x, y, i, j, valor;
		bool sair;
		std::string texto;

		//Inicializa a fila de eventos
		ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
		ALLEGRO_EVENT evento;
		filaDeEvento = al_create_event_queue();
		al_register_event_source(filaDeEvento, al_get_mouse_event_source());
		al_register_event_source(filaDeEvento, al_get_keyboard_event_source());
		al_register_event_source(filaDeEvento, al_get_display_event_source(tela));

		x = y = -1;
		while (true) {
			//Imprime o display
			al_clear_to_color(al_map_rgb(230, 230, 230));
			jogoAtual->imprime(tela, fonte, fonte2);
			if (x == -1 || y == -1) { //Caso nenhum quadrado esteja selecionado, imprime instrucoes
				al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
				al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Clique em um quadrado para ver e editar suas anotacoes");
			}
			else { //Senao imprime o quadrado selecionado com cor diferente e os numeros anotados nele
				al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
				al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotado:");

				//Botoes para adicionar ou excluir anotacoes
				drawBotao(fonte, "Adicionar", 7);
				drawBotao(fonte, "Excluir", 8);

				//Imprime as anotacoes contidas no quadrado
				al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, jogoAtual->anotaString(x, y).c_str());

			}
			//Imprime o botao para voltar
			drawBotao(fonte, "Voltar", 9);
			//Exibe todas as impressoes no display
			al_flip_display();

			//Espera a entrada de informacoes
			al_flush_event_queue(filaDeEvento);
			do {
				al_wait_for_event(filaDeEvento, &evento);
			} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario clicou
				evento.type != ALLEGRO_EVENT_DISPLAY_SWITCH_IN && //O usuario voltou a janela
				evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario clicou para fechar a janela
			
			//Caso o usuario clicou
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				//Verifica se ele clicou em algum quadrado
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						if (clicouRegiao(j * 50, i * 50, (j + 1) * 50, (i + 1) * 50, evento)) {
							//Se clicou, atualiza x e y para os indices correspondentes ao quadrado
							x = j;
							y = i;
						}
					}
				}
				//Verifica se clicou em algum botao
				switch (clicouBotao(evento)) {
				case 7: //Botao adicionar
					if (x != -1) { //Caso possua um quadrado selecionado
						while (clicouBotao(evento) != 9) { //Enquanto nao clicar no botao para voltar
							//Imprime display
							al_clear_to_color(al_map_rgb(230, 230, 230));
							al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
							jogoAtual->imprime(tela, fonte, fonte2);
							al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Digite um numero para anota-lo");
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 240, 200, 40, 0, jogoAtual->anotaString(x, y).c_str());
							drawBotao(fonte, "Voltar", 9);
							al_flip_display();

							//Espera a entrada de informacoes
							al_flush_event_queue(filaDeEvento);
							do {
								al_wait_for_event(filaDeEvento, &evento);
							} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario clicou
								evento.type != ALLEGRO_EVENT_KEY_DOWN && //O usuario apertou uma tecla
								evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario clicou para fechar a janela
							
							//Caso o usuario apertou uma tecla
							if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
								//Caso apertou uma tecla de numero acima das alfabeticas (com ids [28:36])
								if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
									//Converte de [28:36] para [1:9] e insere na anotacao
									jogoAtual->anotaInsere(evento.keyboard.keycode - 27, x, y);
									
								}
								//Caso apertou uma tecla de numero no teclado numerico (com ids [38:46])
								else if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
									//Converte de [38:46] para [1:9] e insere na anotacoao
									jogoAtual->anotaInsere(evento.keyboard.keycode - 37, x, y);
								}
							}
							//Caso clicou para fechar a janela
							if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
								al_destroy_event_queue(filaDeEvento);
								*fechar = true;
								return; //Retorna ao menu principal
							}
						}
					}
					break;

				case 8: //Botao remover
					if (x != -1) { //Caso possua um quadrado selecionado
						while (clicouBotao(evento) != 9) { //Enquanto nao clicar no botao para voltar
							//Imprime display
							al_clear_to_color(al_map_rgb(230, 230, 230));
							al_draw_filled_rectangle(x * 50, y * 50, (x + 1) * 50, (y + 1) * 50, al_map_rgb(200, 240, 240));
							jogoAtual->imprime(tela, fonte, fonte2);
							al_draw_text(fonte, al_map_rgb(0, 0, 0), 575, 40, ALLEGRO_ALIGN_CENTRE, "Anotando");
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 120, 200, 40, 0, "Digite um numero para remove-lo");
							al_draw_multiline_text(fonte, al_map_rgb(0, 0, 0), 475, 240, 200, 40, 0, jogoAtual->anotaString(x, y).c_str());
							drawBotao(fonte, "Voltar", 9);
							al_flip_display();

							//Espera a entrada de informacoes
							al_flush_event_queue(filaDeEvento);
							do {
								al_wait_for_event(filaDeEvento, &evento);
							} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //O usuario clicou
								evento.type != ALLEGRO_EVENT_KEY_DOWN && //O usuario apertou uma tecla
								evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //O usuario clicou para fechar a janela
							
							//Caso o usuario apertou uma tecla
							if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
								//Caso apertou uma tecla de numero acima das alfabeticas (com ids [28:36])
								if (evento.keyboard.keycode >= 28 && evento.keyboard.keycode <= 36) {
									//Converte de [28:36] para [1:9] e remove da anotacao
									jogoAtual->anotaRemove(evento.keyboard.keycode - 27, x, y);
								}
								//Caso apertou uma tecla de numero no teclado numerico (com ids [38:46))
								else if (evento.keyboard.keycode >= 38 && evento.keyboard.keycode <= 46) {
									//Converte de [38:46] para [1:9] e remove da anotacao
									jogoAtual->anotaRemove(evento.keyboard.keycode - 37, x, y);
								}
							}
							//Caso clicou para fechar a janela
							if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
								al_destroy_event_queue(filaDeEvento);
								*fechar = true;
								return; //Retorna ao menu principal
							}
						}
					}
					break;
				case 9: al_destroy_event_queue(filaDeEvento); //Botao voltar
					return; //Retorna ao menu principal
				default:
					break;
				}
			}
			//Caso clicou para fechar a janela
			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				al_destroy_event_queue(filaDeEvento);
				*fechar = true;
				return; //Retorna ao menu principal
			}
		}
	}
	//Titulo do botao
	virtual char const* titulo() {
		return "Anotacoes";
	}
};

//Botao para sair do programa
class MenuSair : public MenuItem { //Sai do programa
public:
	virtual void comportamento(Tabuleiro* jogoAtual, ALLEGRO_DISPLAY* tela, ALLEGRO_FONT* fonte, ALLEGRO_FONT* fonte2, bool* fechar) {}
	//Titulo do botao
	virtual char const* titulo() {
		return "Sair";
	}
};

#endif