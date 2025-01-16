//
// Created by nicjl on 13/01/2025.
//

#ifndef GESTAO_ESTOQUE_H
#define GESTAO_ESTOQUE_H

#include <pthread.h>

// struct que representa o controle de estoque
typedef struct {
    int *ingredientes;    // array de ingredientes (quantidades)
    int tamanho;          // n° total de ingredientes
    pthread_mutex_t mutex; // mutex de acesso exclusivo ao estoque
    pthread_cond_t cond_reabastecimento; // cond para o reabastecimento
} ControleEstoque;

// manipulação do estoque
void inicializar_estoque(ControleEstoque *estoque, int tamanho);
void consumir_ingrediente(ControleEstoque *estoque, int ingrediente_id, int quantidade);
void reabastecer_estoque(ControleEstoque *estoque, int ingrediente_id, int quantidade);
void destruir_estoque(ControleEstoque *estoque);

void* controle_estoque(void *arg);

#endif //GESTAO_ESTOQUE_H
