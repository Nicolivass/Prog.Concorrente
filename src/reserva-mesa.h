//
// Created by nicjl on 13/01/2025.
//

#ifndef RESERVA_MESA_H
#define RESERVA_MESA_H

#include <pthread.h>

// Estrutura que representa a reserva de mesas
typedef struct {
    int *mesas;           // Array de mesas (1 = ocupada, 0 = disponível)
    int num_mesas;        // Total de mesas
    pthread_mutex_t mutex; // Mutex para garantir que a mesa não seja reservada simultaneamente
} ReservaMesas;

// Funções para manipulação das reservas
void inicializar_reservas(ReservaMesas *reservas, int num_mesas);
int reservar_mesa(ReservaMesas *reservas, int mesa_id);
void liberar_mesa(ReservaMesas *reservas, int mesa_id);
void destruir_reservas(ReservaMesas *reservas);

void* reserva_mesa(void *arg);

#endif //RESERVA_MESA_H
