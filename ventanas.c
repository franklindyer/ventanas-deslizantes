#include <stdio.h>
#include <stdlib.h>
#include "lista_eventos.c"

#define VERBOSE 1

#define TERM_TRANS 1
#define LLEGADA_DATOS 2
#define LLEGADA_ACK 3
#define TIMEOUT 4

/* El buffer del emisor contiene una lista de segmentos emitidos,
    y el buffer del receptor contiene una lista de los recibidos */
struct Ventana {
    int shift;
    long buffer;
};

struct Ventana* nueva_ventana() {
    struct Ventana* ventana = malloc(sizeof(struct Ventana));
    ventana->shift = 0;
    ventana->buffer = 0;
    ventana->buffer = 0;
    return ventana;
}

void actualizar_shift(struct Ventana* ventana) {
    if ((ventana->buffer) & 1) {
        ventana->buffer = (ventana->buffer) >> 1;
        ventana->buffer = (ventana->buffer) >> 1;
        ventana->shift += 1;
        actualizar_shift(ventana);
    }
    return;
}

int debe_emitir(int largo, long emitidos) {
    return (((1 << largo) - 1) & ~emitidos);
}

int sig_emision(int shift, long emitidos) {
    if ((emitidos & 1) == 0) return shift;
    return sig_emision(shift + 1, emitidos >> 1);
}

int empezar_transmision(struct Evento* princ, struct Ventana* emisor, float transmis, int largo, int total) {
    int sig_emis = sig_emision(emisor->shift, emisor->buffer);
    if (debe_emitir(largo, emisor->buffer) && sig_emis < total) {
        insertar_evento(princ, TERM_TRANS, transmis, sig_emis);
        return sig_emis;
    }
    return 0;
}

void simular_ventanas_deslizantes(int largo, int total, float transmis, float timeout) {
    /* Inicializar la lista de eventos */
    struct Evento* princ = evento_nulo();
    float hora_actual = 0;
    float espera = 0;
    int evento_actual = 0;
    float propag_sig = 0;
    int num_trans = 0;

    /* Inicializar los agentes */
    struct Ventana* emisor = nueva_ventana();
    int transmitiendo = 0;
    struct Ventana* receptor = nueva_ventana();

    princ = insertar_evento(princ, TERM_TRANS, transmis, emisor->shift);
    emisor->buffer = 1;
    transmitiendo = 1;    
    if (VERBOSE) printf("0 : Transimision del primer segmento.\n");

    while (emisor->shift < total) {
        espera = adelante(princ);
        hora_actual += espera;
        num_trans = princ->datos;    

        evento_actual = princ->tipo;

        if (evento_actual == TERM_TRANS) {
            if (VERBOSE) printf("%.4f : fin de transmision de segmento %d.\n", hora_actual, num_trans);
            transmitiendo = 0;

            /* Insertar evento para la llegada del segmento */
            scanf("%f", &propag_sig);
            if (propag_sig != -1) {
                insertar_evento(princ, LLEGADA_DATOS, propag_sig, num_trans);
            } else {
                if (VERBOSE) printf("El segmento %d se pierde en camino.\n", num_trans);
            }

            /* Insertar evento para el timeout */
            insertar_evento(princ, TIMEOUT, timeout, num_trans);

            /* Actualizar la lista de emitidos */
            emisor->buffer = (emisor->buffer) | (1 << (num_trans - emisor->shift));

            /* Empezar nueva transmision si es apropiado */
            transmitiendo = empezar_transmision(princ, emisor, transmis, largo, total);
            if (transmitiendo && VERBOSE)
                printf("%.4f : principio de transmision de segmento %d.\n", hora_actual, transmitiendo);
            transmitiendo = !(!transmitiendo);
        }

        if (evento_actual == LLEGADA_DATOS) {
            if (VERBOSE) printf("%.4f : llega el segmento %d.\n", hora_actual, num_trans);

            /* Actualizar la lista de recibidos */
            receptor->buffer = (receptor->buffer) | (1 << (num_trans - receptor->shift));

            /* Actualizar el shift de receptor */
            actualizar_shift(receptor);

            /* Enviarle un ACK al emisor que indica el último segmento contiguo recibido */
            scanf("%f", &propag_sig);
            if (propag_sig != -1) {
                insertar_evento(princ, LLEGADA_ACK, propag_sig, receptor->shift);
            } else {
                if (VERBOSE) printf("El ACK para los segmentos hasta %d se pierde en camino.\n", num_trans);
            }
        }

        if (evento_actual == LLEGADA_ACK) {
            if (VERBOSE) printf("%.4f : llega un ACK para los segmentos hasta %d.\n", hora_actual, num_trans - 1);

            /* Actualizar el shift de emisor */
            int dif = num_trans - emisor->shift;
            emisor->buffer = (emisor->buffer) >> dif;
            emisor->shift = num_trans;

            /* Si ha parado de transmitir entonces debe empezar de nuevo */
            if (transmitiendo == 0) {
                transmitiendo = empezar_transmision(princ, emisor, transmis, largo, total);
                if (transmitiendo && VERBOSE)
                    printf("%.4f : principio de transmision de segmento %d.\n", hora_actual, transmitiendo);
                transmitiendo = !(!transmitiendo);
            }
        }

        if (evento_actual == TIMEOUT) {
    
            /* Fingir que nunca se envió el segmento en primer lugar */
            if (num_trans >= emisor->shift) {
                if (VERBOSE) printf("%.4f : ocurre un timeout para el segmento %d.\n", hora_actual, num_trans);
                emisor->buffer = (emisor->buffer) & ~(1 << (num_trans - emisor->shift));
 
                /* Si ha parado de transmitir entonces debe empezar de nuevo */
                if (transmitiendo == 0) {
                    transmitiendo = empezar_transmision(princ, emisor, transmis, largo, total);
                    if (transmitiendo && VERBOSE)
                        printf("%.4f : principio de transmision de segmento %d.\n", hora_actual, transmitiendo);
                    transmitiendo = !(!transmitiendo);
                }
            }
            
        }

        princ = pasar(princ);
    }
}

int main(int argc, char* argv[]) {

    int largo = atoi(argv[1]);
    int total = atoi(argv[2]);
    float transmis = atof(argv[3]);
    float timeout = atof(argv[4]);   
 
    simular_ventanas_deslizantes(largo, total, transmis, timeout);
    return 0;
}
