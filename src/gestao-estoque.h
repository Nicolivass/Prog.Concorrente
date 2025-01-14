//
// Created by nicjl on 13/01/2025.
//

#ifndef GESTAO_ESTOQUE_H
#define GESTAO_ESTOQUE_H

#include <pthread.h>

// Estrutura que representa o controle de estoque
typedef struct {
    int *ingredientes;    // Array de ingredientes (quantidades)
    int tamanho;          // Número total de ingredientes
    pthread_mutex_t mutex; // Mutex para garantir acesso exclusivo ao estoque
    pthread_cond_t cond_reabastecimento; // Condição para o reabastecimento
} ControleEstoque;

// Funções para manipulação do estoque
void inicializar_estoque(ControleEstoque *estoque, int tamanho);
void consumir_ingrediente(ControleEstoque *estoque, int ingrediente_id, int quantidade);
void reabastecer_estoque(ControleEstoque *estoque, int ingrediente_id, int quantidade);
void destruir_estoque(ControleEstoque *estoque);

void* controle_estoque(void *arg);

#endif //GESTAO_ESTOQUE_H
