#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

#define FPS 30.0
#define CANTMOV 5

enum KEYS{
    UP,   // 0
    DOWN, // 1
    LEFT, // 2
    RIGHT, // 3
    //--------------------MIO
    X   //4
    //--------------------MIO
};

int teclas[5] = {0, 0, 0, 0, 0};

//CREACION DE ESTRUCTURAS DE IMAGENES-------------------------------------------
typedef struct jugador {
    int x; // posicion x de la nave
    int y; // posicion y de la nave
    ALLEGRO_BITMAP *nave; // imagen a renderizar
} jugador_t;

typedef struct shot {
  int x;
  int y;
  ALLEGRO_BITMAP *bala;
} shot_t;

typedef struct enemigos {
  int x;
  int y;
  ALLEGRO_BITMAP *aliens;
} enemigos_t;
//------------------------------------------------------------------------------

//FUNCION PARA CREAR LA NAVE----------------------------------------------------
void dibujarJugador(jugador_t *jugador) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(jugador->nave, jugador->x, jugador->y, 0);
    al_flip_display();
}

void dibujarBala(shot_t *shot){
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_bitmap(shot->bala, shot->x, shot->y, 0);
  al_flip_display();
}

void dibujarEnemigos(enemigos_t *enemigos, int anchoX){
  for(int i=anchoX-250; i>=15; i=i-30){
    al_draw_bitmap(enemigos->aliens, i, 100, 0);
    al_flip_display();
  }
}
//------------------------------------------------------------------------------

//CREACION DE METODOS DE MOVIMIENTO---------------------------------------------
void moverDerecha(jugador_t *jugador) {
    (*jugador).x += 20;
}

void moverIzquierda(jugador_t *jugador) {
    (*jugador).x -= 20;
}

void moverBala(shot_t *shot, int posY, jugador_t *jugador){
  (*shot).y -=16;
  dibujarJugador(jugador);
  dibujarBala(shot);
}
//------------------------------------------------------------------------------

//METODO MAIN PRINCIPAL---------------------------------------------------------------------
int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;          // Nuestra pantalla
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;  // Con esto podemos manejar eventos
    ALLEGRO_TIMER *timer = NULL;              // Timer para actulizar eventos
    //ALLEGRO_DISPLAY *fondo = al_create_display(SpriteFondo, NULL);

    //DECLARACION DE TAMAÑO DE PANTALLA-----------------------------------------
    int anchoX = 740;
    int anchoY = 480;
    //--------------------------------------------------------------------------

    //SE TRATA DE INICIALIZZAR ALLEGO-------------------------------------------
    if(!al_init()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar allegro 5");
        return -1;
    }
    //--------------------------------------------------------------------------

    //SE CREA LA PANTALLA-------------------------------------------------------
    display = al_create_display(anchoX, anchoY);
    //--------------------------------------------------------------------------

    //SI NO SE PUEDE CREAR LA PANTALLA al_create_display DEVUELVE FALSE (0)
    if(!display) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        return -1;
    }
    //--------------------------------------------------------------------------

    //SE TRATA DE AGREGAR EL ADDON DE IMAGENES DE ALLEGO------------------------
    if(!al_init_image_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el addon de imagenes");
        al_destroy_display(display);
        return -1;
    }
    //--------------------------------------------------------------------------

    //SE TRATA DE CARGAR EL TECLADO DE ALLEGRO----------------------------------
    if(!al_install_keyboard()) {
        fprintf(stderr, "%s\n", "No se pudo instalar el teclado");
        al_destroy_display(display);
        return -1;
    }
    //--------------------------------------------------------------------------

    //CONTROLLES DE PANTALLA----------------------------------------------------
    al_inhibit_screensaver(1);    //EVITAMOS QUE SE SUSPENDA MIESNTRAS SE JUEGA
    al_set_window_title(display, "GALAGA"); //SE LE PONE TITULO A LA PANTALLA
    al_clear_to_color(al_map_rgb(0, 0, 0)); //SE LE CARGA UN FONDO NEGRO
    al_flip_display();  //SE MUESTRA LO QUE CREAMOS

    //CREACION DEL TIMER--------------------------------------------------------
    timer = al_create_timer(1.0 / FPS);
    //--------------------------------------------------------------------------

    //CREACION DE EVENTOS DEL JUEGO---------------------------------------------
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //--------------------------------------------------------------------------

    //SE CREA EL JUGADOR Y SE INICIALIZA----------------------------------------
    jugador_t *player = (jugador_t *)malloc(sizeof(jugador_t));
    player->nave = al_load_bitmap("SpriteNave.png");
    player->x = (anchoX-200)/2;
    player->y = anchoY - 60;

    shot_t *disparo = (shot_t *)malloc(sizeof(shot_t));
    disparo->bala = al_load_bitmap("SpriteBala.png");
    disparo->x = 0;
    disparo->y = 0;

    enemigos_t *malos = (enemigos_t *)malloc(sizeof(enemigos_t));
    malos->aliens = al_load_bitmap("SpriteAlien1.png");
    malos->x = 0;
    malos->y = 0;
    //--------------------------------------------------------------------------

    //SI NO SE PUEDEN CARGAR LAS IMAGENES---------------------------------------
    if(!player->nave) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    if(!malos->aliens){
      fprintf(stderr, "%s\n", "No se pudo crear un display");
      al_destroy_display(display);
      al_destroy_event_queue(event_queue);
      al_destroy_timer(timer);
      return -1;
    }

    if(!disparo->bala){
      fprintf(stderr, "%s\n", "No se pudo crear un display");
      al_destroy_display(display);
      al_destroy_event_queue(event_queue);
      al_destroy_timer(timer);
      return -1;
    }
    //--------------------------------------------------------------------------

    //SE DIBUJA AL JUGADOR------------------------------------------------------
    dibujarEnemigos(malos, anchoX);
    dibujarJugador(player);

    //--------------------------------------------------------------------------

    //SRAND HACIA UN NUMERO QUE LANCE EL RELOJ----------------------------------
    srand(time(NULL));
    //--------------------------------------------------------------------------

    //SE INICIALIZA EL TIMER----------------------------------------------------
    al_start_timer(timer);
    //--------------------------------------------------------------------------

    //BANDERA DE SALIDA AL PRECIONAR esc----------------------------------------
    int terminar = 0;
    //--------------------------------------------------------------------------

    //VARIABLE QUE RECIBE EVENTOS-----------------------------------------------
    ALLEGRO_EVENT ev;
    //--------------------------------------------------------------------------

//BUCLE DEL JUEGO---------------------------------------------------------------
    while(!terminar) {
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    terminar = 1;
                break;
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 0;
                break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 0;
                break;
                case ALLEGRO_KEY_X:
                  teclas[X] = 0;
                break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 1;
                break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 1;
                break;
                case ALLEGRO_KEY_X:
                  teclas[X] = 1;
                  disparo->x = player->x;
                  disparo->y = player->y;
                break;
            }
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(teclas[LEFT] && player->x >= 20.0)
                moverIzquierda(player);
            else if(teclas[RIGHT] && player->x <= anchoX -250 - 20.0)
                moverDerecha(player);
            else if(teclas[X]){
              while(disparo->y >= 0){
                moverBala(disparo, disparo->y, player);
              }

            }
        }

        //SE DIBUJA AL JUGADOR--------------------------------------------------
        dibujarJugador(player);
        //----------------------------------------------------------------------
    }
    //--------------------------------------------------------------------------

    //SE LIMPIA LA MEMORIA------------------------------------------------------
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_bitmap(disparo->bala);
    al_destroy_timer(timer);
    free(player);
    free(disparo);
    //--------------------------------------------------------------------------
    return 0;
}
