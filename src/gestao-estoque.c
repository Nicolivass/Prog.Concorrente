//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_INGREDIENTS 100  // Capacidade máxima do estoque
#define ITEM_TYPES 5         // Tipos diferentes de ingredientes

int stock[ITEM_TYPES];       // Estoque para cada tipo de ingrediente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_full[ITEM_TYPES];  // Semáforos para gerenciar consumo
sem_t sem_empty[ITEM_TYPES]; // Semáforos para gerenciar reabastecimento

// Função para os chefs consumirem ingredientes
void* chef(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % ITEM_TYPES;  // Seleciona um tipo de ingrediente aleatoriamente
        int amount = rand() % 5 + 1;    // Quantidade a ser consumida (1 a 5)

        sem_wait(&sem_full[item]);      // Espera o ingrediente estar disponível
        pthread_mutex_lock(&mutex);     // Protege acesso ao estoque

        if (stock[item] >= amount) {
            stock[item] -= amount;
            printf("Chef %d consumed %d of ingredient %d. Remaining: %d\n", id, amount, item, stock[item]);
        } else {
            printf("Chef %d could not consume %d of ingredient %d. Insufficient stock!\n", id, amount, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty[item]);    // Libera espaço para reabastecimento
        sleep(rand() % 3 + 1);         // Simula tempo de consumo
    }
    return NULL;
}

// Função para os entregadores reabastecerem ingredientes
void* delivery(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % ITEM_TYPES;  // Seleciona um tipo de ingrediente aleatoriamente
        int amount = rand() % 10 + 5;   // Quantidade a ser reabastecida (5 a 15)

        sem_wait(&sem_empty[item]);     // Espera espaço no estoque
        pthread_mutex_lock(&mutex);     // Protege acesso ao estoque

        if (stock[item] + amount <= MAX_INGREDIENTS) {
            stock[item] += amount;
            printf("Deliverer %d replenished %d of ingredient %d. Total: %d\n", id, amount, item, stock[item]);
        } else {
            printf("Deliverer %d could not replenish %d of ingredient %d. Stock full!\n", id, amount, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full[item]);     // Libera ingrediente para consumo
        sleep(rand() % 5 + 1);         // Simula tempo de entrega
    }
    return NULL;
}

int main() {
    pthread_t chefs[3], deliverers[2];
    int chef_ids[3] = {1, 2, 3};
    int deliverer_ids[2] = {1, 2};

    // Inicializa estoque e semáforos
    for (int i = 0; i < ITEM_TYPES; i++) {
        stock[i] = rand() % (MAX_INGREDIENTS / 2); // Estoque inicial aleatório
        sem_init(&sem_full[i], 0, stock[i]);      // Quantidade inicial de itens disponíveis
        sem_init(&sem_empty[i], 0, MAX_INGREDIENTS - stock[i]); // Espaço restante
    }

    // Cria threads de chefs e entregadores
    for (int i = 0; i < 3; i++) {
        pthread_create(&chefs[i], NULL, chef, &chef_ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&deliverers[i], NULL, delivery, &deliverer_ids[i]);
    }

    // Espera as threads (nunca termina neste exemplo)
    for (int i = 0; i < 3; i++) {
        pthread_join(chefs[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(deliverers[i], NULL);
    }

    // Destrói os semáforos
    for (int i = 0; i < ITEM_TYPES; i++) {
        sem_destroy(&sem_full[i]);
        sem_destroy(&sem_empty[i]);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}
