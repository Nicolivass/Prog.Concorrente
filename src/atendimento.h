//
// Created by nicjl on 13/01/2025.
//

#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H

#include <pthread.h>

// Estrutura que representa o atendimento por fila
typedef struct {
    int *filas;            // Array de filas (representando atendentes)
    int num_filas;         // Número de filas de atendentes
    pthread_mutex_t *mutex; // Mutex para cada fila
} AtendimentoMultiFila;

// Funções para manipulação das filas de atendimento
void inicializar_atendimento(AtendimentoMultiFila *atendimento, int num_filas);
void adicionar_cliente(AtendimentoMultiFila *atendimento, int fila_id);
void atender_cliente(AtendimentoMultiFila *atendimento, int fila_id);
void destruir_atendimento(AtendimentoMultiFila *atendimento);

void* atendimento_cliente(void *arg);

#endif //ATENDIMENTO_H
