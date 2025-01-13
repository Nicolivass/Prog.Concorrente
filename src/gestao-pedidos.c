//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PEDIDOS 10  // tamanho máximo da fila

typedef struct {
    int priority;
    int order_id;
} Order;

Order queue[MAX_PEDIDOS];
int count = 0;  // Número de pedidos na fila

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_empty;
sem_t sem_full;

// Função para adicionar pedido à fila
void enqueue(Order order) {
    int i = count - 1;
    while (i >= 0 && queue[i].priority < order.priority) {
        queue[i + 1] = queue[i];
        i--;
    }
    queue[i + 1] = order;
    count++;
}

// Função para remover pedido da fila
Order dequeue() {
    Order order = queue[0];
    for (int i = 0; i < count - 1; i++) {
        queue[i] = queue[i + 1];
    }
    count--;
    return order;
}

// Função da thread produtora (chef criando pedidos)
void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        Order order = {rand() % 10 + 1, id * 100 + i};
        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex);

        enqueue(order);
        printf("Producer %d added order %d with priority %d\n", id, order.order_id, order.priority);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);
        sleep(rand() % 2 + 1);  // Simula tempo de criação de pedido
    }
    return NULL;
}

// Função da thread consumidora (chef processando pedidos)
void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex);

        if (count > 0) {
            Order order = dequeue();
            printf("Consumer %d processed order %d with priority %d\n", id, order.order_id, order.priority);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);
        sleep(rand() % 3 + 1);  // Simula tempo de processamento
    }
    return NULL;
}

int main() {
    pthread_t producers[2], consumers[2];
    int ids[2] = {1, 2};

    sem_init(&sem_empty, 0, MAX_PEDIDOS);
    sem_init(&sem_full, 0, 0);

    // Criação de threads produtoras e consumidoras
    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &ids[i]);
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    // Aguarda threads produtoras finalizarem
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
    }

    // Finaliza consumidores após os produtores terminarem
    sleep(5);
    for (int i = 0; i < 2; i++) {
        pthread_cancel(consumers[i]);
    }

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
