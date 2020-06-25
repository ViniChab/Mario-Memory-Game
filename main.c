#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <time.h>

#define M 6
#define N 6
#define K 3

/* Strucs */

typedef struct
{
  char nome[50];
  int acertos;
  int erros;
  char idade[3];
} Jogador;

typedef struct
{
  FILE *fp;
  FILE *fp_nome;
  FILE *fp_idade;
} Campeao;

typedef struct
{
  char nome[50];
  char idade[5];
} Nomes;

/*
 * Declaração de assinaturas
 */

int finalizar();                                                    //Assinatura
int inicializar();                                                  //Assinatura
void clear();                                                       //Assinatura
void getMatriz();                                                   //Assinatura
void setGrid();                                                     //Assinatura
void virarCarta(int x, int y);                                      //Assinatura
void getImages();                                                   //Assinatura
void find(int value);                                               //Assinatura
void flip(int valor_carta, int x, int y, int erro_acerto_primeira); //Assinatura
int has_ended();                                                    //Assinatura
void endGame(int *scoreboard[7], int pontuacao);                    //Assinatura

/*
 * Variáveis
 */

Jogador jogador;
Nomes nomes[6];
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_BITMAP *imagens[M][N];
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_SAMPLE *acerto = NULL;
ALLEGRO_SAMPLE *erro = NULL;
ALLEGRO_SAMPLE *hit_bloco = NULL;

int dados_carta_virada[K] = {0, -1, -1};
int positions[4];
int card_pixel_position[2];

int matriz[M][N] = {
    {1, 2, 3, 4, 5, 6},
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 17, 18},
    {13, 14, 15, 16, 17, 18},
};

/*
 * Main
 */

int main(int argc, char *argv[])
{
  //declarando e inicializando
  unsigned int *pontuacao = malloc(sizeof(int));
  Campeao campeao;
  pontuacao = 100000000;
  campeao.fp = fopen("champions_score.txt", "r");
  campeao.fp_nome = fopen("champions_nomes.txt", "r");
  campeao.fp_idade = fopen("champions_ages.txt", "r");
  ALLEGRO_KEYBOARD_STATE keyState; //Guarda o teclado
  ALLEGRO_MOUSE_STATE estadoMouse; //Guarda  mouse
  int *scoreboard[7];
  jogador.acertos = 0;
  jogador.erros = 0;
  int keypress = 0;
  char alfabeto[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
                       'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  char numeral[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  for (int i = 0; i <= 7; i++)
    fscanf(campeao.fp, "%d", &scoreboard[i]);

  for (int i = 0; i < 7; i++){
    fscanf(campeao.fp_nome, "%s", nomes[i].nome);
    fscanf(campeao.fp_idade, "%s", nomes[i].idade);
  }

  //Declaro a janela
  if (!inicializar())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro 5\n");
    return -1;
  }

  window = al_create_display(1200, 700);
  //dou um valor em pixels para  a janela

  if (!window)
  {
    fprintf(stderr, "Falha ao criar a janela\n");
    return -1;
  }

  al_clear_to_color(al_map_rgb(255, 255, 255));
  //Pinta a tela toda de alguma cor

  al_install_keyboard();
  al_install_mouse();
  //inicializa o teclado nesse escopo

  fonte = al_load_font("VarelaRound.ttf", 20, 0);
  ALLEGRO_BITMAP *background = al_load_bitmap("./sprites/background.jpg");
  ALLEGRO_BITMAP *title = al_load_bitmap("./sprites/title.png");

  if (!fonte)
  {
    al_destroy_display(window);
    fprintf(stderr, "Falha ao carregar fonte\n");
    return -1;
  }

  //declara a 'fila' que vai trabalhar com os dadso de entrada pelo teclado
  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  for (;;)
  {
    al_draw_bitmap(title, 0, 0, 0);
    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        350, 160,
        ALLEGRO_ALIGN_LEFT,
        "What's your name?");
    al_flip_display();

    ALLEGRO_EVENT events;
    al_wait_for_event(event_queue, &events);
    if (events.type = ALLEGRO_EVENT_KEY_UP)
    {
      if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)
      {
        if (keypress == 2)
          break;
        keypress++;
      }
      else if (keypress == 0)
      {
        jogador.nome[strlen(jogador.nome)] = alfabeto[events.keyboard.keycode - 1];
        for (int i = 0; i < strlen(jogador.nome); i++)
        {
          al_draw_textf(
              fonte,
              al_map_rgb(0, 0, 0),
              350 + (i * 17), 190,
              ALLEGRO_ALIGN_LEFT,
              "%c", jogador.nome[i]);
          al_flip_display();
        }
        keypress++;
      }
      else if (keypress == 1)
      {
        keypress++;
      }
      else
        keypress = 0;
    }

    al_flip_display();
  };

  al_flip_display();
  al_clear_to_color(al_map_rgb(255, 255, 255));
  //removendo o box de nome

  for (;;)
  {
    al_draw_bitmap(title, 0, 0, 0);
    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        350, 160,
        ALLEGRO_ALIGN_LEFT,
        "What's your age?");
    al_flip_display();

    ALLEGRO_EVENT events;
    al_wait_for_event(event_queue, &events);
    if (events.type = ALLEGRO_EVENT_KEY_UP)
    {
      if (events.keyboard.keycode == ALLEGRO_KEY_ENTER)
      {
        break;
      }
      else if (keypress == 0)
      {
        jogador.idade[strlen(jogador.idade)] = numeral[events.keyboard.keycode - 37];
        for (int i = 0; i < strlen(jogador.idade); i++)
        {
          al_draw_textf(
              fonte,
              al_map_rgb(0, 0, 0),
              350 + (i * 17), 190,
              ALLEGRO_ALIGN_LEFT,
              "%c", jogador.idade[i]);
          al_flip_display();
        }
        printf("\n");
        keypress++;
      }
      else if (keypress == 1)
      {
        keypress++;
      }
      else
        keypress = 0;
    }

    al_flip_display();
  };

  al_flip_display();
  al_clear_to_color(al_map_rgb(255, 255, 255));
  //removendo o box de nome

  getMatriz();
  //para permitir movimento
  setGrid();
  //preparando a grid de blocos
  getImages();
  //preparando as imagens numa matriz filha da getMatriz()

  for (;;)
  {
    al_rest(0.001);
    //Define o FPS

    al_get_keyboard_state(&keyState);
    al_get_mouse_state(&estadoMouse);

    if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
      break;

    if (estadoMouse.buttons == 1)
    {
      int comparacao_um_x = 30;
      int comparacao_dois_x = 130;
      int comparacao_um_y = 30;
      int comparacao_dois_y = 130;

      for (int j = 0; j < 6; j++)
      {
        for (int i = 0; i < 6; i++)
        {
          if ((estadoMouse.x > comparacao_um_x && estadoMouse.x < comparacao_dois_x) &&
              (estadoMouse.y > comparacao_um_y && estadoMouse.y < comparacao_dois_y))
            virarCarta(comparacao_um_y, comparacao_um_x);

          comparacao_um_x += 110;
          comparacao_dois_x += 110;
        }
        comparacao_um_y += 110;
        comparacao_dois_y += 110;

        comparacao_um_x = 30;
        comparacao_dois_x = 130;
      }
    }

    al_flip_display();

    al_draw_bitmap(background, 720, 0, 0);

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        890, 50,
        ALLEGRO_ALIGN_LEFT,
        "Score: %d", pontuacao, (jogador.erros + 1));

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        800, 90,
        ALLEGRO_ALIGN_LEFT,
        "Successes: %d", jogador.acertos);

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        980, 90,
        ALLEGRO_ALIGN_LEFT,
        "Mistakes: %d", jogador.erros);

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        890, 200,
        ALLEGRO_ALIGN_LEFT,
        "Best Players", pontuacao);

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        800, 130,
        ALLEGRO_ALIGN_LEFT,
        "Name:", pontuacao);

    for (int i = 0; i < strlen(jogador.nome); i++)
    {
      al_draw_textf(
          fonte,
          al_map_rgb(0, 0, 0),
          870 + (i * 17), 130,
          ALLEGRO_ALIGN_LEFT,
          "%c", jogador.nome[i]);
    }

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        800, 160,
        ALLEGRO_ALIGN_LEFT,
        "Age:", pontuacao);

    for (int i = 0; i < strlen(jogador.idade); i++)
    {
      al_draw_textf(
          fonte,
          al_map_rgb(0, 0, 0),
          850 + (i * 17), 160,
          ALLEGRO_ALIGN_LEFT,
          "%c", jogador.idade[i]);
    }

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        780, 240,
        ALLEGRO_ALIGN_LEFT,
        "Name");

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        930, 240,
        ALLEGRO_ALIGN_LEFT,
        "Score");

    al_draw_textf(
        fonte,
        al_map_rgb(0, 0, 0),
        1080, 240,
        ALLEGRO_ALIGN_LEFT,
        "Age");

    for (int i = 1; i <= 7; i++)
      al_draw_textf(
          fonte,
          al_map_rgb(0, 0, 0),
          920, 240 + (i * 50),
          ALLEGRO_ALIGN_LEFT,
          "%d", scoreboard[i]);

    for (int i = 0; i < 7; i++)
      al_draw_textf(
          fonte,
          al_map_rgb(0, 0, 0),
          750, 240 + ((i + 1) * 50),
          ALLEGRO_ALIGN_LEFT,
          "%dº %s", (i + 1), nomes[i].nome);

    for (int i = 0; i < 7; i++)
      al_draw_textf(
          fonte,
          al_map_rgb(0, 0, 0),
          1080, 240 + ((i + 1) * 50),
          ALLEGRO_ALIGN_LEFT,
          "%s", nomes[i].idade);

    pontuacao -= 1;

    if (jogador.acertos == 18)
    {
      fclose(campeao.fp);
      fclose(campeao.fp_idade);
      fclose(campeao.fp_nome);
      endGame(scoreboard, pontuacao);
    }
  }
  finalizar();
  return 0;
}

void endGame(int *scoreboard[7], int pontuacao)
{
  int aux, aux2, aux3;
  Campeao Campeao;
  Campeao.fp = fopen("champions_score.txt", "w");
  Campeao.fp_nome = fopen("champions_nomes.txt", "w");
  Campeao.fp_idade = fopen("champions_ages.txt", "w");

  for (int i = 0; i < 7; i++){
    fscanf(Campeao.fp_nome, "%s", nomes[i].nome);
    fscanf(Campeao.fp_idade, "%s", nomes[i].idade);
  }
  int menorValor = scoreboard[1];
  for (int i = 1; i <= 7; i++)
  {
    if (scoreboard[i] < menorValor)
      menorValor = scoreboard[i];
  }

  rewind(Campeao.fp);
  rewind(Campeao.fp_nome);
  rewind(Campeao.fp_idade);
  fprintf(Campeao.fp, "00000\n");

  printf("menor valor: %d\n", menorValor);
  for (int i = 1; i <= 7; i++)
  {
    if (scoreboard[i] == menorValor){
      fprintf(Campeao.fp, "%d\n", pontuacao);
      fprintf(Campeao.fp_nome, "%s ", jogador.nome);
      fprintf(Campeao.fp_idade, "%s ", jogador.idade);
    } else{
      fprintf(Campeao.fp, "%d\n", scoreboard[i]);
      fprintf(Campeao.fp_nome, "%s ", nomes[i -1].nome);
      fprintf(Campeao.fp_idade, "%s ", nomes[i -1].idade);
    }
  }

  fclose(Campeao.fp);
  fclose(Campeao.fp_nome);
  fclose(Campeao.fp_idade);
  al_rest(2);
  finalizar();
}

void getImages()
{
  ALLEGRO_BITMAP *image_list[18] = {
      al_load_bitmap("./sprites/1.jpg"), al_load_bitmap("./sprites/2.jpg"),
      al_load_bitmap("./sprites/3.jpg"), al_load_bitmap("./sprites/4.jpg"),
      al_load_bitmap("./sprites/5.jpg"), al_load_bitmap("./sprites/6.jpg"),
      al_load_bitmap("./sprites/7.jpg"), al_load_bitmap("./sprites/8.jpg"),
      al_load_bitmap("./sprites/9.jpg"), al_load_bitmap("./sprites/10.jpg"),
      al_load_bitmap("./sprites/11.jpg"), al_load_bitmap("./sprites/12.jpg"),
      al_load_bitmap("./sprites/13.jpg"), al_load_bitmap("./sprites/14.jpg"),
      al_load_bitmap("./sprites/15.jpg"), al_load_bitmap("./sprites/16.jpg"),
      al_load_bitmap("./sprites/17.jpg"), al_load_bitmap("./sprites/18.jpg")};

  for (int i = 1; i <= 18; i++)
  {
    find(i);
    imagens[positions[0]][positions[1]] = image_list[i - 1];
    imagens[positions[2]][positions[3]] = image_list[i - 1];
  }
}

void find(int value)
{
  int flag = -2;

  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++)
    {
      if (matriz[i][j] == value)
      {
        flag += 2;
        positions[flag] = i;
        positions[flag + 1] = j;
      }
      if (flag == 2)
      {
        return;
      }
    }
}

void setGrid()
{
  ALLEGRO_BITMAP *bloco = al_load_bitmap("./sprites/block.png");

  int pos_x = 30, pos_y = 30;

  for (int j = 1; j < 7; j++)
  {
    for (int i = 1; i < 7; i++)
    {

      if (!matriz[j - 1][i - 1] == 0)
      {
        al_draw_bitmap(bloco, pos_x, pos_y, 0);
      }
      else
      {
        al_draw_bitmap(imagens[j - 1][i - 1], pos_x, pos_y, 0);
      }

      pos_x = 30 + (110 * i);
    }
    pos_x = 30;
    pos_y = 30 + (110 * j);
  }
}

void virarCarta(int x, int y)
{
  //carta zero está virada
  if (matriz[x / 100][y / 100] != 0)
  {
    al_play_sample(hit_bloco, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
    int valor_carta = matriz[x / 100][y / 100];
    int erro_acerto_primeira = 0;
    // 0 - erro
    // 1 - acerto
    // 2 - primeira carta

    if (dados_carta_virada[0] == 0)
    {
      dados_carta_virada[0] = matriz[x / 100][y / 100];
      dados_carta_virada[1] = x;
      dados_carta_virada[2] = y;
      matriz[x / 100][y / 100] = 0;
      erro_acerto_primeira = 2; //primeira carta
    }
    else if (dados_carta_virada[0] == matriz[x / 100][y / 100])
    {
      matriz[x / 100][y / 100] = 0;
      dados_carta_virada[0] = 0;
      dados_carta_virada[1] = -1;
      dados_carta_virada[2] = -1;
      erro_acerto_primeira = 1; //acerto
      jogador.acertos++;
    }
    else
    {
      matriz[dados_carta_virada[1] / 100][dados_carta_virada[2] / 100] = dados_carta_virada[0];
      dados_carta_virada[0] = 0;
      dados_carta_virada[1] = -1;
      dados_carta_virada[2] = -1; //erro
      jogador.erros++;
    }
    flip(valor_carta, x / 100, y / 100, erro_acerto_primeira);
  }
}

void flip(int valor_carta, int x, int y, int erro_acerto_primeira)
{
  int pos_x = 30, pos_y = 30;
  ALLEGRO_BITMAP *bloco = al_load_bitmap("./sprites/block.png");
  find(valor_carta);

  pos_x = pos_x + (110 * x);
  pos_y = pos_y + (110 * y);

  al_draw_bitmap(imagens[positions[0]][positions[1]], pos_y, pos_x, 0);

  al_flip_display();

  if (erro_acerto_primeira == 0)
  {
    al_rest(1);
    al_draw_bitmap(bloco, pos_y, pos_x, 0);
    al_draw_bitmap(bloco, card_pixel_position[0], card_pixel_position[1], 0);
    al_play_sample(erro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  }
  else if (erro_acerto_primeira == 1)
  {
    al_rest(1);
    al_play_sample(acerto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  }
  else if (erro_acerto_primeira == 2)
  {
    card_pixel_position[0] = pos_y;
    card_pixel_position[1] = pos_x;
  }
}

void getMatriz()
{
  srand(time(NULL));
  int aux[4], Primero_valor, Segundo_valor;

  for (int j = 0; j < 200; j++)
  {
    for (int i = 0; i < 4; i++)
      aux[i] = (rand() % 6);

    Primero_valor = matriz[aux[0]][aux[1]];
    Segundo_valor = matriz[aux[2]][aux[3]];
    matriz[aux[0]][aux[1]] = Segundo_valor;
    matriz[aux[2]][aux[3]] = Primero_valor;
  }

  printf("Matriz cola: \n");
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
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

  //musica de fundo
  ALLEGRO_AUDIO_STREAM *musica = NULL;

  if (!al_init_image_addon())
    return 0;
  if (!al_init())
    return 0;
  if (!al_install_keyboard())
    return 0;
  if (!al_install_mouse())
    return 0;
  if (!al_init_font_addon())
    return 0;
  if (!al_init_ttf_addon())
    return 0;

  if (!al_init_primitives_addon())
  {
    fprintf(stderr, "Falha ao criar inicializar sistemas de primitivas\n");
    return -1;
  }

  if (!al_install_audio())
  {
    fprintf(stderr, "Falha ao inicializar o audio");
    return 0;
  }

  if (!al_init_acodec_addon())
  {
    fprintf(stderr, "Falha ao inicializar o codec de audio");
    return 0;
  }

  if (!al_reserve_samples(5))
  {
    fprintf(stderr, "Falha ao reservar amostrar de audio");
    return 0;
  }

  acerto = al_load_sample("./songs/correct.ogg");
  if (!acerto)
  {
    fprintf(stderr, "Audio nao carregado");
    return 0;
  }

  erro = al_load_sample("./songs/wrong.ogg");
  if (!acerto)
  {
    fprintf(stderr, "Audio nao carregado");
    return 0;
  }

  hit_bloco = al_load_sample("./songs/coin.ogg");
  if (!acerto)
  {
    fprintf(stderr, "Audio nao carregado");
    return 0;
  }

  musica = al_load_audio_stream("./songs/music.ogg", 4, 1024);
  if (!musica)
  {
    fprintf(stderr, "Audio nao carregado");
    al_destroy_sample(acerto);
    al_destroy_sample(erro);
    al_destroy_sample(hit_bloco);
    return 0;
  }

  al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());

  al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

  return 1;
}

int finalizar()
{
  al_uninstall_keyboard();
  al_uninstall_mouse();

  al_destroy_font(fonte);
  al_destroy_display(window);

  //fecha o programa
  return 0;
}