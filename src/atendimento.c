//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ATENDENTES 3  // Número de atendentes
#define MAX_CLIENTES 10   // Tamanho máximo de cada fila

typedef struct {
    int fila[MAX_CLIENTES];  // Fila de clientes
    int inicio, fim;         // Indicadores de início e fim da fila
    pthread_mutex_t mutex;   // Mutex para proteger a fila
    pthread_cond_t cond;     // Condição para gerenciar espera de clientes
} FilaAtendimento;

FilaAtendimento atendentes[NUM_ATENDENTES];  // Array de filas para os atendentes

// Adiciona um cliente à fila
void adicionar_cliente(FilaAtendimento* fila, int cliente_id) {
    pthread_mutex_lock(&fila->mutex);

    // Verifica se há espaço na fila
    if ((fila->fim + 1) % MAX_CLIENTES == fila->inicio) {
        printf("Fila cheia! Cliente %d não pode ser adicionado.\n", cliente_id);
    } else {
        fila->fila[fila->fim] = cliente_id;
        fila->fim = (fila->fim + 1) % MAX_CLIENTES;
        printf("Cliente %d entrou na fila.\n", cliente_id);
        pthread_cond_signal(&fila->cond);  // Notifica atendente
    }

    pthread_mutex_unlock(&fila->mutex);
}

// Remove um cliente da fila
int atender_cliente(FilaAtendimento* fila) {
    pthread_mutex_lock(&fila->mutex);

    // Espera até que haja clientes na fila
    while (fila->inicio == fila->fim) {
        pthread_cond_wait(&fila->cond, &fila->mutex);
    }

    int cliente_id = fila->fila[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_CLIENTES;
    printf("Atendendo cliente %d.\n", cliente_id);

    pthread_mutex_unlock(&fila->mutex);
    return cliente_id;
}

// Função para simular clientes entrando em filas aleatórias
void* cliente(void* arg) {
    int cliente_id = *(int*)arg;
    int fila_id = rand() % NUM_ATENDENTES;

    adicionar_cliente(&atendentes[fila_id], cliente_id);

    sleep(rand() % 3 + 1);  // Simula o tempo entre clientes
    return NULL;
}

// Função para simular atendentes atendendo clientes
void* atendente(void* arg) {
    int atendente_id = *(int*)arg;

    while (1) {
        int cliente_id = atender_cliente(&atendentes[atendente_id]);
        printf("Atendente %d terminou de atender o cliente %d.\n", atendente_id, cliente_id);
        sleep(rand() % 2 + 1);  // Simula o tempo de atendimento
    }
    return NULL;
}

int main() {
    pthread_t threads_clientes[20];  // 20 clientes simulados
    pthread_t threads_atendentes[NUM_ATENDENTES];
    int clientes_ids[20];
    int atendentes_ids[NUM_ATENDENTES];

    // Inicializa as filas e seus mutexes
    for (int i = 0; i < NUM_ATENDENTES; i++) {
        atendentes[i].inicio = 0;
        atendentes[i].fim = 0;
        pthread_mutex_init(&atendentes[i].mutex, NULL);
        pthread_cond_init(&atendentes[i].cond, NULL);
        atendentes_ids[i] = i;
        pthread_create(&threads_atendentes[i], NULL, atendente, &atendentes_ids[i]);
    }

    // Cria threads de clientes
    for (int i = 0; i < 20; i++) {
        clientes_ids[i] = i + 1;
        pthread_create(&threads_clientes[i], NULL, cliente, &clientes_ids[i]);
        sleep(rand() % 2);  // Intervalo entre clientes
    }

    // Espera threads de clientes (simulação contínua, atendentes não são esperados)
    for (int i = 0; i < 20; i++) {
        pthread_join(threads_clientes[i], NULL);
    }

    // Destrói mutexes e variáveis de condição
    for (int i = 0; i < NUM_ATENDENTES; i++) {
        pthread_mutex_destroy(&atendentes[i].mutex);
        pthread_cond_destroy(&atendentes[i].cond);
    }

    return 0;
}
