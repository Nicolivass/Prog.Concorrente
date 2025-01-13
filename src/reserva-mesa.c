//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_MESAS 10  // Número total de mesas no restaurante

int mesas[TOTAL_MESAS];               // Array que representa o estado das mesas (0 = livre, 1 = reservada)
pthread_mutex_t mutex_mesas[TOTAL_MESAS];  // Um mutex para cada mesa

// Função para simular a reserva de uma mesa
void* cliente(void* arg) {
    int id = *(int*)arg;

    while (1) {
        int mesa = rand() % TOTAL_MESAS;  // Escolhe uma mesa aleatoriamente

        printf("Cliente %d tentando reservar a mesa %d...\n", id, mesa);
        if (pthread_mutex_trylock(&mutex_mesas[mesa]) == 0) {  // Tenta bloquear o mutex da mesa
            if (mesas[mesa] == 0) {  // Verifica se a mesa está livre
                mesas[mesa] = 1;  // Reserva a mesa
                printf("Cliente %d reservou a mesa %d com sucesso!\n", id, mesa);
            } else {
                printf("Cliente %d não conseguiu reservar a mesa %d. Já está ocupada.\n", id, mesa);
            }
            pthread_mutex_unlock(&mutex_mesas[mesa]);  // Desbloqueia o mutex da mesa
        } else {
            printf("Cliente %d não conseguiu acessar a mesa %d. Mutex em uso.\n", id, mesa);
        }

        sleep(rand() % 3 + 1);  // Espera antes de tentar novamente
    }
    return NULL;
}

int main() {
    pthread_t clientes[5];  // 5 clientes simulados
    int ids[5] = {1, 2, 3, 4, 5};

    // Inicializa as mesas como livres e os mutexes
    for (int i = 0; i < TOTAL_MESAS; i++) {
        mesas[i] = 0;
        pthread_mutex_init(&mutex_mesas[i], NULL);
    }

    // Cria as threads dos clientes
    for (int i = 0; i < 5; i++) {
        pthread_create(&clientes[i], NULL, cliente, &ids[i]);
    }

    // Espera as threads dos clientes (nunca termina neste exemplo)
    for (int i = 0; i < 5; i++) {
        pthread_join(clientes[i], NULL);
    }

    // Destrói os mutexes
    for (int i = 0; i < TOTAL_MESAS; i++) {
        pthread_mutex_destroy(&mutex_mesas[i]);
    }

    return 0;
}
