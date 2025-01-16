//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "gestao-pedidos.h"
#include "gestao-estoque.h"
#include "reserva-mesa.h"

// Definindo o número máximo de threads para cada subsistema
#define NUM_THREADS 5

// Recursos compartilhados globais
FilaPedidos fila_pedidos;           // Para pedidos prontos
int estoque[NUM_INGREDIENTES];      // Para controle de estoque
int mesas_disponiveis[NUM_MESAS];   // Para reserva de mesas
FilaAtendimento filas_atendentes[NUM_ATENDENTES]; // Para atendimento multi-fila
int entregadores_disponiveis[NUM_ENTREGADORES];   // Para entregadores

pthread_mutex_t mutex_estoque;       // Protege o estoque
pthread_mutex_t mutex_mesas;         // Protege as mesas
pthread_mutex_t mutex_entregadores;  // Protege entregadores

// Função para inicializar todos os módulos de forma centralizada
void inicializa_sistemas() {
    inicializa_fila(&fila_pedidos, MAX_PEDIDOS);
    inicializa_estoque(estoque, NUM_INGREDIENTES);
    inicializa_mesas(mesas_disponiveis, NUM_MESAS);
    inicializa_filas_atendimento(filas_atendentes, NUM_ATENDENTES);
    inicializa_entregadores(entregadores_disponiveis, NUM_ENTREGADORES);

    pthread_mutex_init(&mutex_estoque, NULL);
    pthread_mutex_init(&mutex_mesas, NULL);
    pthread_mutex_init(&mutex_entregadores, NULL);
}

// Função para destruir todos os módulos após o término
void destruir_sistemas() {
    destruir_fila(&fila_pedidos);
    pthread_mutex_destroy(&mutex_estoque);
    pthread_mutex_destroy(&mutex_mesas);
    pthread_mutex_destroy(&mutex_entregadores);
}

// Função para criar as threads para cada subsistema
void criar_threads(pthread_t *threads, int *ids) {
    pthread_create(&threads[0], NULL, gestao_pedidos, (void*)&ids[0]);
    pthread_create(&threads[1], NULL, gestao_estoque, (void*)&ids[1]);
    pthread_create(&threads[2], NULL, reserva_mesa, (void*)&ids[2]);
    pthread_create(&threads[3], NULL, atendimento, (void*)&ids[3]);
    pthread_create(&threads[4], NULL, pedidos_delivery, (void*)&ids[4]);
}

int main() {
    pthread_t threads[NUM_THREADS]; // Array de threads
    int ids[NUM_THREADS];           // Array de IDs para as threads

    // Inicializa os subsistemas
    inicializa_sistemas();

    // Cria as threads para os subsistemas
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i; // Passa o índice como ID
    }
    criar_threads(threads, ids);

    // Simula a chegada de novos pedidos periodicamente
    for (int i = 0; i < 50; i++) {
        sleep(1);
        printf("Novo pedido criado: %d\n", i + 1);
        adicionar_pedido(&fila_pedidos, i + 1);
    }

    // Aguarda as threads (se apropriado)
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libera recursos
    destruir_sistemas();

    return 0;
}
