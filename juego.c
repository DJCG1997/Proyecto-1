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
    RIGHT // 3
};

int teclas[4] = {0, 0, 0, 0};

typedef struct jugador {
    int x; // posicion x de la nave
    int y; // posicion y de la nave
    ALLEGRO_BITMAP *nave; // imagen a renderizar
} jugador_t;

// funcion ayuda que dibuja a nuestra navecita
void dibujarJugador(jugador_t *jugador) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(jugador->nave, jugador->x, jugador->y, 0);
    al_flip_display();
}

void moverArriba(jugador_t *jugador) {
    // su codigo aqui
}

void moverAbajo(jugador_t *jugador) {
    // su codigo aqui
}

void moverDerecha(jugador_t *jugador) {
    // su codigo aqui
}

void moverIzquierda(jugador_t *jugador) {
    // su codigo aqui
}

int main(int argc, char **argv) {
    // Nuestra pantalla
    ALLEGRO_DISPLAY *display = NULL;
    // Con esto podemos manejar eventos
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    // Timer para actulizar eventos
    ALLEGRO_TIMER *timer = NULL;

    // Tratamos de inicializar allegro
    if(!al_init()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar allegro 5");
        return -1;
    }

    // Creamos un nuevo display de 640x480 para empezar
    display = al_create_display(640, 480);
    // Si no se pudo crear el display al_create_display devuelve false (0)
    if(!display) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        return -1;
    }

    // Tratamos de agregar el addon de imagenes de allegro
    if(!al_init_image_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el addon de imagenes");
        // tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    // Tratamos de instalar el teclado en allegro
    if(!al_install_keyboard()) {
        fprintf(stderr, "%s\n", "No se pudo instalar el teclado");
        // tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    // evitamos que se suspenda la computadora mientras esta el juego abierto
    al_inhibit_screensaver(1);
    // le ponemos un titulo a nuestro display
    al_set_window_title(display, "Ejercicio 4: Juego en C");
    // al principio queremos que tenga fondo negro
    al_clear_to_color(al_map_rgb(0, 0, 0));
    // hacemos que se muestre lo que dibujamos
    al_flip_display();

    // creamos el timer
    timer = al_create_timer(1.0 / FPS);

    // Creamos los eventos del juego
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Creamos un jugador (miren como se usa malloc :) )
    // e inicializamos su posicion (0, 0)
    jugador_t *player = (jugador_t *)malloc(sizeof(jugador_t));
    player->nave = al_load_bitmap("spacecraft.png");
    player->x = 0;
    player->y = 0;

    // si la imagen de la nave no se pudo cargar
    if(!player->nave) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    // dibujemos al jugador por primera vez
    dibujarJugador(player);

    // srand a un numero que tire el reloj
    srand(time(NULL));

    // comenzamos el timer
    al_start_timer(timer);

    // bandera para salir del juego se preciona escape
    int terminar = 0;

    // una variable que recibe eventos (?)
    ALLEGRO_EVENT ev;

    // loop del juego
    while(!terminar) {
        al_wait_for_event(event_queue, &ev);
        // si el evento es key_up
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_UP:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_DOWN:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_LEFT:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_RIGHT:
                    // su codigo aqui
                break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_DOWN:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_LEFT:
                    // su codigo aqui
                break;
                case ALLEGRO_KEY_RIGHT:
                    // su codigo aqui
                break;
            }
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(teclas[UP])
                // su codigo aqui
            else if(teclas[DOWN])
                // su codigo aqui
            else if(teclas[LEFT])
                // su codigo aqui
            else if(teclas[RIGHT])
                // su codigo aqui
        }
        // dibujamos al jugador
        dibujarJugador(player);
    }

    // siemple hay que limpiar memoria
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_timer(timer);
    free(player);
    return 0;
}
