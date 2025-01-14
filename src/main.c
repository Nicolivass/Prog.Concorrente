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
#include "atendimento.h"
#include "pedidos-delivery.h"


// Recursos compartilhados globais
FilaPedidos fila_pedidos;        // Para pedidos prontos
int estoque[NUM_INGREDIENTES];   // Para controle de estoque
int mesas_disponiveis[NUM_MESAS]; // Para reserva de mesas
FilaAtendimento filas_atendentes[NUM_ATENDENTES]; // Para atendimento multi-fila
int entregadores_disponiveis[NUM_ENTREGADORES];   // Para entregadores

pthread_mutex_t mutex_estoque;  // Protege o estoque
pthread_mutex_t mutex_mesas;    // Protege as mesas
pthread_mutex_t mutex_entregadores; // Protege entregadores

// Função principal de inicialização
int main() {
    pthread_t threads[10]; // Array de threads (expandir conforme necessidade)
    int ids[10];

    // Inicializa os recursos compartilhados
    inicializa_fila(&fila_pedidos, MAX_PEDIDOS);
    inicializa_estoque(estoque, NUM_INGREDIENTES);
    inicializa_mesas(mesas_disponiveis, NUM_MESAS);
    inicializa_filas_atendimento(filas_atendentes, NUM_ATENDENTES);
    inicializa_entregadores(entregadores_disponiveis, NUM_ENTREGADORES);

    pthread_mutex_init(&mutex_estoque, NULL);
    pthread_mutex_init(&mutex_mesas, NULL);
    pthread_mutex_init(&mutex_entregadores, NULL);

    // Cria threads para os subsistemas
    ids[0] = 0;
    pthread_create(&threads[0], NULL, gestao-pedidos, (void*)&ids[0]);
    ids[1] = 0;
    pthread_create(&threads[1], NULL, gestao-estoque, (void*)&ids[1]);
    ids[2] = 0;
    pthread_create(&threads[2], NULL, reserva-mesa, (void*)&ids[2]);
    ids[3] = 0;
    pthread_create(&threads[3], NULL, atendimento, (void*)&ids[3]);
    ids[4] = 0;
    pthread_create(&threads[4], NULL, pedidos-delivery, (void*)&ids[4]);

    // Simula a chegada de novos pedidos periodicamente
    for (int i = 0; i < 50; i++) {
        sleep(1);
        printf("Novo pedido criado: %d\n", i + 1);
        adicionar_pedido(&fila_pedidos, i + 1);
    }

    // Aguarda as threads (se apropriado)
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libera recursos
    destruir_fila(&fila_pedidos);
    pthread_mutex_destroy(&mutex_estoque);
    pthread_mutex_destroy(&mutex_mesas);
    pthread_mutex_destroy(&mutex_entregadores);

    return 0;
}
