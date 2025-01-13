//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ENTREGADORES 3  // Número de entregadores disponíveis
#define MAX_PEDIDOS 10      // Capacidade máxima da fila de pedidos

typedef struct {
    int pedidos[MAX_PEDIDOS];  // Fila de pedidos prontos
    int inicio, fim;           // Indicadores de início e fim da fila
    pthread_mutex_t mutex;     // Mutex para proteger a fila
    pthread_cond_t cond;       // Condição para gerenciar a espera de novos pedidos
} FilaPedidos;

FilaPedidos fila_pedidos;
pthread_mutex_t mutex_entregadores;
int entregadores_disponiveis[NUM_ENTREGADORES];  // 1 = disponível, 0 = ocupado

// Adiciona um pedido à fila
void adicionar_pedido(int pedido_id) {
    pthread_mutex_lock(&fila_pedidos.mutex);

    // Verifica se há espaço na fila
    while ((fila_pedidos.fim + 1) % MAX_PEDIDOS == fila_pedidos.inicio) {
        printf("Fila de pedidos cheia! Esperando espaço...\n");
        pthread_cond_wait(&fila_pedidos.cond, &fila_pedidos.mutex);
    }

    fila_pedidos.pedidos[fila_pedidos.fim] = pedido_id;
    fila_pedidos.fim = (fila_pedidos.fim + 1) % MAX_PEDIDOS;
    printf("Pedido %d adicionado à fila.\n", pedido_id);
    pthread_cond_signal(&fila_pedidos.cond);

    pthread_mutex_unlock(&fila_pedidos.mutex);
}

// Remove um pedido da fila
int remover_pedido() {
    pthread_mutex_lock(&fila_pedidos.mutex);

    // Espera até que haja pedidos na fila
    while (fila_pedidos.inicio == fila_pedidos.fim) {
        printf("Fila de pedidos vazia! Esperando pedidos...\n");
        pthread_cond_wait(&fila_pedidos.cond, &fila_pedidos.mutex);
    }

    int pedido_id = fila_pedidos.pedidos[fila_pedidos.inicio];
    fila_pedidos.inicio = (fila_pedidos.inicio + 1) % MAX_PEDIDOS;
    pthread_cond_signal(&fila_pedidos.cond);

    pthread_mutex_unlock(&fila_pedidos.mutex);
    return pedido_id;
}

// Distribui pedidos para entregadores disponíveis
void* distribuidor(void* arg) {
    while (1) {
        int pedido_id = remover_pedido();

        pthread_mutex_lock(&mutex_entregadores);
        int entregador_id = -1;

        // Procura um entregador disponível
        for (int i = 0; i < NUM_ENTREGADORES; i++) {
            if (entregadores_disponiveis[i] == 1) {
                entregador_id = i;
                entregadores_disponiveis[i] = 0;  // Marca como ocupado
                break;
            }
        }

        if (entregador_id != -1) {
            printf("Pedido %d atribuído ao entregador %d.\n", pedido_id, entregador_id);
        } else {
            printf("Nenhum entregador disponível para o pedido %d.\n", pedido_id);
        }
        pthread_mutex_unlock(&mutex_entregadores);

        // Simula o tempo de entrega
        if (entregador_id != -1) {
            sleep(rand() % 3 + 1);
            pthread_mutex_lock(&mutex_entregadores);
            entregadores_disponiveis[entregador_id] = 1;  // Marca como disponível novamente
            printf("Entregador %d finalizou o pedido %d.\n", entregador_id, pedido_id);
            pthread_mutex_unlock(&mutex_entregadores);
        }
    }
    return NULL;
}

// Simula a chegada de novos pedidos
void* produtor_pedidos(void* arg) {
    int pedido_id = 1;

    while (1) {
        adicionar_pedido(pedido_id);
        pedido_id++;
        sleep(rand() % 2 + 1);  // Intervalo entre pedidos
    }
    return NULL;
}

int main() {
    pthread_t thread_distribuidor, thread_produtor;

    // Inicializa fila de pedidos
    fila_pedidos.inicio = 0;
    fila_pedidos.fim = 0;
    pthread_mutex_init(&fila_pedidos.mutex, NULL);
    pthread_cond_init(&fila_pedidos.cond, NULL);

    // Inicializa entregadores como disponíveis
    pthread_mutex_init(&mutex_entregadores, NULL);
    for (int i = 0; i < NUM_ENTREGADORES; i++) {
        entregadores_disponiveis[i] = 1;
    }

    // Cria threads para distribuidor e produtor de pedidos
    pthread_create(&thread_distribuidor, NULL, distribuidor, NULL);
    pthread_create(&thread_produtor, NULL, produtor_pedidos, NULL);

    // Espera as threads (simulação contínua)
    pthread_join(thread_distribuidor, NULL);
    pthread_join(thread_produtor, NULL);

    // Limpa recursos
    pthread_mutex_destroy(&fila_pedidos.mutex);
    pthread_cond_destroy(&fila_pedidos.cond);
    pthread_mutex_destroy(&mutex_entregadores);

    return 0;
}
