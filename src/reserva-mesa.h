//
// Created by nicjl on 13/01/2025.
//

#ifndef RESERVA_MESA_H
#define RESERVA_MESA_H

#include <pthread.h>

// struct de reserva de mesas
typedef struct {
    int *mesas;           // array de mesas (1 = ocupada, 0 = disponível)
    int num_mesas;        // total de mesas
    pthread_mutex_t mutex; // mutex da reserva da mesa
} ReservaMesas;

// manipulação das reservas
void inicializar_reservas(ReservaMesas *reservas, int num_mesas);
int reservar_mesa(ReservaMesas *reservas, int mesa_id);
void liberar_mesa(ReservaMesas *reservas, int mesa_id);
void destruir_reservas(ReservaMesas *reservas);

void* reserva_mesa(void *arg);

#endif //RESERVA_MESA_H
