//
// Created by nicjl on 13/01/2025.
//

#ifndef GESTAO_PEDIDOS_H
#define GESTAO_PEDIDOS_H

#include <pthread.h>

// struct & funções públicas
typedef struct {
    int *pedidos;
    int capacidade;
    int frente;
    int tras;
    pthread_mutex_t mutex;
    pthread_cond_t cond_prod;
    pthread_cond_t cond_cons;
} FilaPedidos;

void inicializa_fila(FilaPedidos *fila, int capacidade);
void adicionar_pedido(FilaPedidos *fila, int pedido);
int remover_pedido(FilaPedidos *fila);
void destruir_fila(FilaPedidos *fila);

void* gestao_pedidos(void *arg);

#endif //GESTAO_PEDIDOS_H
