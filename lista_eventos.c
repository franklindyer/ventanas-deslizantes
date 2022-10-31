#include <stdio.h>
#include <stdlib.h>

struct Evento {
    int tipo;
    int datos;
    float cuando;
    struct Evento* sig_evento;
};

struct Evento* evento_nulo() {
    struct Evento* nulo = malloc(sizeof(struct Evento));
    nulo->tipo = 0;
    nulo->cuando = 0;
    nulo->sig_evento = nulo;
    return nulo;
}

struct Evento* insertar_evento(struct Evento* princ, int tipo, float cuando, int datos) {
    if ((princ->tipo) == 0 || cuando < princ->cuando) {
        struct Evento* nuevo = malloc(sizeof(struct Evento));
        nuevo->tipo = tipo;
        nuevo->cuando = cuando;
        nuevo->sig_evento = princ;
        nuevo->datos = datos;
        return nuevo;
    }

    princ->sig_evento = insertar_evento(princ->sig_evento, tipo, cuando, datos);
    return princ;
}

void trasladar_tiempo(struct Evento* princ, float duracion) {
    if ((princ->tipo) == 0) return;
    princ->cuando += duracion;
    trasladar_tiempo(princ->sig_evento, duracion);
    return;
}

float adelante(struct Evento* princ) {
    float espera = princ->cuando;
    trasladar_tiempo(princ, -espera);
    return espera;
}

struct Evento* pasar(struct Evento* princ) {
    return princ->sig_evento;
}

void free_eventos(struct Evento* princ) {
    struct Evento* siguiente = princ->sig_evento;
    int tipo = princ->tipo;
    free(princ);
    if (tipo != 0) free_eventos(siguiente);
}

/*
int main() {
    struct Evento* princ = evento_nulo();

    princ = insertar_evento(princ, 1, 3, 0);
    princ = insertar_evento(princ, 2, 10, 0);
    princ = insertar_evento(princ, 3, 5, 0);
    while (princ->tipo != 0) {
        printf("Esperamos %.3f segundos\n", adelante(princ));
        printf("Ocurre un evento tipo %d\n", princ->tipo);
        princ = pasar(princ);
    }

    return 0;
}
*/
