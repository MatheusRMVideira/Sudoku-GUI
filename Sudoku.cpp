/*  Alexandre dos Santos Gualberto
	Guilherme Calca
	Matheus Rezende Milani Videira
*/
#if defined (_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
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

int main(int argc, char** argv) {
	//Inicializacao do Allegro
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();
	al_install_keyboard();

	//Inicializa todos os recursos necessarios do Allegro
	ALLEGRO_DISPLAY* tela = NULL;
	ALLEGRO_EVENT_QUEUE* filaDeEvento = NULL;
	ALLEGRO_EVENT evento;
	tela = al_create_display(700, 480);
	al_set_window_title(tela, "Sudoku");
	filaDeEvento = al_create_event_queue();

	//Registra as fontes de evento para captura de cliques e das operacoes com a janela (fechar)
	al_register_event_source(filaDeEvento, al_get_mouse_event_source());
	al_register_event_source(filaDeEvento, al_get_display_event_source(tela));


	//Carrega a fonte no memsmo diretorio que o programa
	//Caso o SO seja windows e ele nao encontre a fonte padrao,
	//carrega a fonte Arial disponivel no sistema
	ALLEGRO_FONT* fonte = al_load_font("Ubuntu-Regular.ttf", 30, NULL);
	ALLEGRO_FONT* fonte2 = al_load_font("Ubuntu-Regular.ttf", 12, NULL);
	if (!fonte) {
#if defined(_WIN32)
		al_destroy_font(fonte);
		std::string pathDir = "/Fonts/arial.ttf";
		fonte = al_load_font(strcat(getenv("windir"), pathDir.c_str()), 30, NULL);
#endif
		if (!fonte) {
			al_show_native_message_box(tela, "Erro ao carregar fonte", "Nao foi possivel carregar a fonte",
				"Baixe a fonte Ubuntu-regular em https://fonts.google.com/specimen/Ubuntu e extraia no mesmo diretorio deste programa",
				NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_font(fonte);
			al_destroy_font(fonte2);
			al_destroy_event_queue(filaDeEvento);
			al_destroy_display(tela);
			return 0;
		}
	}
	if (!fonte2) {
#if defined(_WIN32)
		al_destroy_font(fonte2);
		std::string pathDir = "/Fonts/arial.ttf";
		fonte2 = al_load_font(strcat(getenv("windir"), pathDir.c_str()), -12, NULL);
#endif
		if (!fonte2) {
			al_show_native_message_box(tela, "Erro ao carregar fonte", "Nao foi possivel carregar a fonte",
				"Baixe a fonte Ubuntu-regular em https://fonts.google.com/specimen/Ubuntu e extraia no mesmo diretorio deste programa",
				NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_font(fonte);
			al_destroy_font(fonte2);
			al_destroy_event_queue(filaDeEvento);
			al_destroy_display(tela);
			return 0;
		}
	}

	//Inicializa o jogo e o menu
	Tabuleiro jogo;
	MenuLista* menu = new MenuLista();

	jogo.geraTabuleiro(tela, fonte); //Gera o tabuleiro

	menu->insere(new MenuNovoJogo); //Cria um novo jogo
	menu->insere(new MenuInserir); //Insere o numero em um quadrado
	menu->insere(new MenuAnota); //Anota numeros em um quadrado
	menu->insere(new MenuRemover); //Remove o numero de um quadrado
	menu->insere(new MenuVerificar); //Verifica se o valor de um quadrado esta correto
	menu->insere(new MenuVerificarTudo); //Verifica se todos os quadrados estao corretos
	menu->insere(new MenuResolver); //Resolve automaticamente o sudoku atual
	menu->insere(new MenuCreditos); //Creditos
	menu->insere(new MenuSair); //Termina a execucao do programa


	int entrada; //Captura o botao que foi clicado
	bool fechar; //Captura se o usuario deseja fechar o programa
	while (true) {
		fechar = false;
		//Imprime o menu principal
		al_clear_to_color(al_map_rgb(230, 230, 230));
		jogo.imprime(tela, fonte, fonte2);
		menu->imprime(tela, fonte);
		al_flip_display();
		//Espera uma entrada o usuario
		al_flush_event_queue(filaDeEvento);
		do {
			al_wait_for_event(filaDeEvento, &evento);
		} while (evento.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && //Usuario clicou
			evento.type != ALLEGRO_EVENT_DISPLAY_SWITCH_IN && //Usuario voltou a janela do programa
			evento.type != ALLEGRO_EVENT_DISPLAY_CLOSE); //Usuario fechou a janela do programa
		
		//Caso o usuario tenha clicado, captura o botao e executa o codigo dele
		if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			entrada = clicouBotao(evento); //retorna o indice do botao, -1 caso nao tenha sido nenhum
			if (entrada != -1) {
				menu->get(entrada)->comportamento(&jogo, tela, fonte, fonte2, &fechar); //executa o codigo correspondente ao botao clicado
			}
		}
		//Caso o usuario tenha clicado no botao de saida, ou tenha clicado no botao para fechar a janela
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE || clicouBotao(evento) == 8 || fechar) {
			//Exibe mensagem de alerta confirmando a saida do programa
			int x = al_show_native_message_box(tela, "Sair", "Deseja sair do programa?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO);
			//Caso o usuario deseje sair, finaliza o programa
			if (x == 1) {
				delete menu;
				al_destroy_font(fonte);
				al_destroy_font(fonte2);
				al_destroy_display(tela);
				al_destroy_event_queue(filaDeEvento);
				al_uninstall_system();
				return 0;
			}
		}
	}
}

