//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PEDIDOS 10
// máximo da fila de pedidos

typedef struct {
    int prioridade;
    int id_pedido;
} Pedido; //pedido com id e prioridade

Pedido queue[MAX_PEDIDOS]; //cria fila com pedidos
int cont = 0;  // n° de pedidos atualmente na fila

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_vazio; //sem que indica que fila ta vazia
sem_t sem_cheio; //sem que indica que fila ta cheia

// adicionar pedido na fila de forma decrescente
void adicionar(Pedido pedido) {
    int i = cont - 1;
    while (i >= 0 && queue[i].prioridade < pedido.prioridade) {
        queue[i + 1] = queue[i];
        i--;
    }
    queue[i + 1] = pedido;
    cont++;
}

// remover pedido com maior prioridade da fila
Pedido remover() {
    Pedido pedido = queue[0];
    for (int i = 0; i < cont - 1; i++) {
        queue[i] = queue[i + 1];
    }
    cont--;
    return pedido;
}

// atendente criando pedidos
void* criar(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        Pedido pedido = {rand() % 10 + 1, id * 100 + i};
        sem_wait(&sem_vazio);
        pthread_mutex_lock(&mutex);

        adicionar(pedido);
        printf("Atendente %d adicionou pedido %d com prioridade %d\n", id, pedido.id_pedido, pedido.prioridade);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_cheio);
        sleep(rand() % 2 + 1);  //tempo de criação de pedido
    }
    return NULL;
}

// chef preparando pedidos
void* preparar(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&sem_cheio);
        pthread_mutex_lock(&mutex);

        if (cont > 0) {
            Pedido pedido = remover();
            printf("Chef %d preparou pedido %d com prioridade %d\n", id, pedido.id_pedido, pedido.prioridade);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_vazio);
        sleep(rand() % 3 + 1);  // tempo de preparo
    }
    return NULL;
}

int main() {
    pthread_t atendentes[2], chefs[2];
    int ids[2] = {1, 2};

    sem_init(&sem_vazio, 0, MAX_PEDIDOS);
    sem_init(&sem_cheio, 0, 0);

    //threads atendentes e chefs
    for (int i = 0; i < 2; i++) {
        pthread_create(&atendentes[i], NULL, criar, &ids[i]);
        pthread_create(&chefs[i], NULL, preparar, &ids[i]);
    }

    // espera threads atendentes
    for (int i = 0; i < 2; i++) {
        pthread_join(atendentes[i], NULL);
    }

    // acaba chefs
    sleep(5);
    for (int i = 0; i < 2; i++) {
        pthread_cancel(chefs[i]);
    }

    sem_destroy(&sem_vazio);
    sem_destroy(&sem_cheio);
    pthread_mutex_destroy(&mutex);

    return 0;
}
