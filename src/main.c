//
// Created by nicjl on 13/01/2025.
//


//src/main.c
//src/gestao-pedidos.c
//src/gestao-estoque.c
//src/reserva-mesa.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "gestao-estoque.h"
#include "gestao-pedidos.h"
#include "reserva-mesa.h"

#define TOTAL_CLIENTES 5
#define MAX_PEDIDOS 10

//iniciar os módulos
void* iniciar_estoque(void* arg) {
    ControleEstoque* estoque = (ControleEstoque*)arg;
    inicializar_estoque(estoque, 5);  // inicia estoque com os tipos de ingredientes
    return NULL;
}

void* iniciar_pedidos(void* arg) {
    FilaPedidos* fila = (FilaPedidos*)arg;
    inicializa_fila(fila, MAX_PEDIDOS);  // Inicializa a fila de pedidos
    return NULL;
}

void* iniciar_reservas(void* arg) {
    ReservaMesas* reservas = (ReservaMesas*)arg;
    inicializar_reservas(reservas, 10);  // Inicializa o sistema de reservas de mesas
    return NULL;
}

int main() {
    pthread_t clientes[TOTAL_CLIENTES];
    int ids_clientes[TOTAL_CLIENTES];
    ControleEstoque estoque;
    FilaPedidos fila_pedidos;
    ReservaMesas reservas;

    // Inicializa as estruturas necessárias para os módulos
    pthread_t estoque_thread, pedidos_thread, reservas_thread;
    pthread_create(&estoque_thread, NULL, iniciar_estoque, &estoque);
    pthread_create(&pedidos_thread, NULL, iniciar_pedidos, &fila_pedidos);
    pthread_create(&reservas_thread, NULL, iniciar_reservas, &reservas);

    // threads para clientes (simulação de interação com o restaurante)
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        ids_clientes[i] = i + 1;
        pthread_create(&clientes[i], NULL, cliente, &ids_clientes[i]);
    }

    // termina estoque, pedidos e reservas
    pthread_join(estoque_thread, NULL);
    pthread_join(pedidos_thread, NULL);
    pthread_join(reservas_thread, NULL);

    // threads nunca terminam
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        pthread_join(clientes[i], NULL);
    }

    // destroi
    destruir_estoque(&estoque);
    destruir_fila(&fila_pedidos);
    destruir_reservas(&reservas);

    return 0;
}
