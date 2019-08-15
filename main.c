#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h> 
#include <time.h> 
#define M 6
#define N 6
#define K 3

int finalizar();   //Assinatura
int inicializar(); //Assinatura
void clear();      //Assinatura
void getMatriz(); //Assinatura
void setGrid(); //Assinatura
void virarCarta(int x,int y); //Assinatura
void getImages(); //Assinatura
void find(int value); //Assinatura
void flip(int valor, int x, int y, int true_false); //Assinatura
int has_ended(); //Assinatura

int positions[4];
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_BITMAP *imagens[M][N];
ALLEGRO_FONT *fonte = NULL; 
first_card[2];

int matriz[M][N] = { 
    { 1,2,3,4,5,6 }, 
    { 1,2,3,4,5,6 },
    { 7,8,9,10,11,12 }, 
    { 7,8,9,10,11,12 },
    { 13,14,15,16,17,18 }, 
    { 13,14,15,16,17,18 },
    };

int Aux[K] = {0, NULL, NULL};
int acertos = 0, erros = 0;

int main(int argc, char *argv[])
{
    int ultimaLocXMouse, ultimaLocYMouse;
    int *pontuacao = 1000000;
    getMatriz();
    //para permitir movimento

    //Declaro a janela
    if(!inicializar()) {
        fprintf(stderr, "Falha ao inicializar a Allegro 5\n");
        return -1; }

    
    window = al_create_display(1200, 700);
    //dou um valor em pixels para  a janela

    if (!window) {
        fprintf(stderr, "Falha ao criar a janela\n");
        return -1; }

    al_clear_to_color(al_map_rgb(255, 255, 255));
    //Pinta a tela toda de alguma cor
    al_flip_display();
    //Mudando de matriz
    al_clear_to_color(al_map_rgb(255, 255, 255));
    //Pintando a secundária de branco também

    al_install_keyboard();
    al_install_mouse();
    //inicializa o teclado nesse escopo
    ALLEGRO_KEYBOARD_STATE keyState; //Guarda o teclado
    ALLEGRO_MOUSE_STATE estadoMouse; //Guarda  mouse
    fonte = al_load_font("VarelaRound.ttf", 20, 0);

    if (!fonte){
        al_destroy_display(window);
        fprintf(stderr, "Falha ao carregar fonte\n");
        return -1; }

    al_flip_display(); //troca a matriz secundária pela primária
    //Assim mudado o que aparece na tela (secundária desenha, primária se vê)

    al_flip_display();

    for (int imparPar = 0;; imparPar++) { //loop infinito

        if (has_ended() == 0)
             break;

        al_rest(0.03);
        //Define o FPS

        al_get_keyboard_state(&keyState);
        //pega qualquer entrada pelo keyboard

        al_get_mouse_state(&estadoMouse);
 
        al_get_mouse_state(&estadoMouse);

        if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
            break;

		if (estadoMouse.buttons == 1) {

            al_rest(0.1);
            int comparacao_um_x = 30;
            int comparacao_dois_x = 130;
            int comparacao_um_y = 30;
            int comparacao_dois_y = 130;

            for (int j = 0 ; j < 6 ; j ++){
                for (int i = 0 ; i < 6 ; i ++){
                    if (estadoMouse.x > comparacao_um_x && estadoMouse.x < comparacao_dois_x )
                        estadoMouse.x = comparacao_um_x;;

                        if (estadoMouse.y > comparacao_um_y && estadoMouse.y < comparacao_dois_y ) {
                            estadoMouse.y = comparacao_um_y;

                                virarCarta(estadoMouse.y, estadoMouse.x);

                        }

                    comparacao_um_x += 110;
                    comparacao_dois_x += 110;
                }
                comparacao_um_y += 110;
                comparacao_dois_y += 110;

                comparacao_um_x = 30;
                comparacao_dois_x = 130;

            }
		}

        imparPar--;
        //pro imparPar nunca extrapolar a memória

		al_flip_display();

        al_clear_to_color(al_map_rgb(255, 255, 255));
        
        setGrid();
        getImages();

        al_draw_textf(
            fonte, 
            al_map_rgb(0, 0, 0), 
            850, 30, 
            ALLEGRO_ALIGN_LEFT, 
            "Pontuação: %d", pontuacao
        );

        al_draw_textf(
            fonte, 
            al_map_rgb(0, 0, 0), 
            850, 70, 
            ALLEGRO_ALIGN_LEFT, 
            "Acertos: %d", acertos
        );

        al_draw_textf(
            fonte, 
            al_map_rgb(0, 0, 0), 
            965, 70, 
            ALLEGRO_ALIGN_LEFT, 
            "Erros: %d", erros
        );

        al_draw_textf(
            fonte, 
            al_map_rgb(0, 0, 0), 
            890, 160, 
            ALLEGRO_ALIGN_LEFT, 
            "Campeões", pontuacao
        );

        pontuacao -= 27;

    }

    al_rest(30);

    finalizar();
}

void getImages() {

    ALLEGRO_BITMAP *image_list[18] = {
        al_load_bitmap ("./sprites/1.jpg"), al_load_bitmap("./sprites/2.jpg"),
        al_load_bitmap ("./sprites/3.jpg"), al_load_bitmap("./sprites/4.jpg"),
        al_load_bitmap ("./sprites/5.jpg"), al_load_bitmap("./sprites/6.jpg"),
        al_load_bitmap ("./sprites/7.jpg"), al_load_bitmap("./sprites/8.jpg"),
        al_load_bitmap ("./sprites/9.jpg"), al_load_bitmap("./sprites/10.jpg"),
        al_load_bitmap("./sprites/11.jpg"), al_load_bitmap("./sprites/12.jpg"),
        al_load_bitmap("./sprites/13.jpg"), al_load_bitmap("./sprites/14.jpg"),
        al_load_bitmap("./sprites/15.jpg"), al_load_bitmap("./sprites/16.jpg"),
        al_load_bitmap("./sprites/17.jpg"), al_load_bitmap("./sprites/18.jpg")
    };

    for(int i = 1; i <= 18 ; i ++ ) {
        find(i);
        imagens[positions[0]][positions[1]] = image_list[i-1];
        imagens[positions[2]][positions[3]] = image_list[i-1];
    }
}

void find(int value) {
    int flag = -2;
    
    for(int i = 0 ; i < 6 ; i++) 
        for(int j = 0 ; j < 6 ; j++) {
            if (matriz[i][j] == value) {
                flag += 2;
                positions[flag] = i;
                positions[flag+1] = j;
            }
            if (flag == 2) {
                return;
            }
        }
}

void setGrid() {
    ALLEGRO_BITMAP *bloco = al_load_bitmap("./sprites/block.png");

    int pos_x = 30, pos_y =30;

    for (int j = 1 ; j < 7 ; j ++){
        for (int i = 1 ; i < 7 ; i ++){
            
            if (!matriz[j-1][i-1] == 0) {
                al_draw_bitmap(bloco, pos_x, pos_y, NULL);
            } else {
                al_draw_bitmap(imagens[j-1][i-1], pos_x, pos_y, 0);
            }

            pos_x = 30 + (110*i);

        }
        pos_x = 30;
        pos_y = 30 + (110*j);
    }
}

void virarCarta(int x,int y) {

    int true_false = 0; //false

    if (matriz[x/100][y/100] != 0 ) {
        if (Aux[0] == 0){
            Aux[0] = matriz[x/100][y/100];
            Aux[1] = x;
            Aux[2] = y;
            matriz[x/100][y/100] = 0;
            true_false = 2; //primeira carta
        } else if(Aux[0] == matriz[x/100][y/100]){
            matriz[Aux[1]/100][Aux[2]/100] = 0;
            matriz[x/100][y/100] = 0;
            Aux[0] = 0;
            Aux[1] = NULL;
            Aux[2] = NULL; 
            true_false = 1; //acerto
            acertos++;
        } else {
            matriz[Aux[1]/100][Aux[2]/100] = Aux[0];
            Aux[0] = 0;
            Aux[1] = NULL;
            Aux[2] = NULL; //erro
            erros++;
        }

    }

    flip(matriz[x/100][y/100], x/100, y/100, true_false);
}

void flip(int valor, int x, int y, int true_false) {
    int pos_x = 30, pos_y =30;
    ALLEGRO_BITMAP *bloco = al_load_bitmap("./sprites/block.png");
    find(valor);
    
    pos_x = pos_x + (110*x);
    pos_y = pos_y + (110*y);
    
    al_draw_bitmap(imagens[positions[0]][positions[1]], pos_y, pos_x, 0);

        al_flip_display();
    
    if(true_false == 0) {
        al_rest(1);
        al_draw_bitmap(bloco, pos_y, pos_x, 0);
        al_draw_bitmap(bloco, first_card[0], first_card[1], 0);
    } else if(true_false == 2) {
        first_card[0] = pos_y;
        first_card[1] = pos_x;
    } 
}

int has_ended() {
    int flag = 0;

    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 6 ; j++) {
            if (matriz[i][j] != 0)
                flag = 1;
        }
    }

    return flag;
}

void getMatriz() {
    srand(time(NULL)); 
    int aux[4], Primero_valor, Segundo_valor;
    
    for( int j = 0 ; j < 200 ; j++) {
        for( int i = 0 ; i < 4; i ++)
            aux[i] = (rand() % 6);

        Primero_valor = matriz[aux[0]][aux[1]];
        Segundo_valor = matriz[aux[2]][aux[3]];
        matriz[aux[0]][aux[1]] = Segundo_valor;
        matriz[aux[2]][aux[3]] = Primero_valor;
    }

    printf("Matriz cola: \n");
    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 6 ; j++) {
            if (matriz[i][j] >= 10)
                printf("%d ", matriz[i][j]);
            else
                printf("0%d ", matriz[i][j]);
        }
        printf("\n");
    }

}

int inicializar()
{

    if (!al_init_image_addon())
        return 0;
    if (!al_init())
        return 0;
    if (!al_install_keyboard())
        return 0;
    if(!al_install_mouse())
        return 0;
    if(!al_init_font_addon())
            return 0;
    if (!al_init_ttf_addon())
            return 0;


    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao criar inicializar sistemas de primitivas\n");
        return -1;
    }
    
    return 1;
}

int finalizar()
{
    al_uninstall_keyboard();
    al_uninstall_mouse();
    //Para de esperar entrada

    al_destroy_font(fonte);//desalocar a memoria da fonte
    al_destroy_display(window);
    //fecha o programa
    return 0;
}