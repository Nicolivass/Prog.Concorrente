//
// Created by nicjl on 13/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_INGREDIENTS 100  // capacidade máxima do estoque
#define ITEM_TYPES 5         // tipos de ingredientes

int estoque[ITEM_TYPES];       // estoque para cada tipo de ingrediente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_cheio[ITEM_TYPES];  // sem para consumo
sem_t sem_vazio[ITEM_TYPES]; // sem para reabastecimento

// chefs consumindo ingredientes
void* chef(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % ITEM_TYPES;  // escolhe um tipo de ingrediente aleatorio
        int qtd = rand() % 5 + 1;    // qtd a ser consumida (até 5)

        sem_wait(&sem_cheio[item]);      // Espera o ingrediente estar disponível
        pthread_mutex_lock(&mutex);     // Protege acesso ao estoque

        if (estoque[item] >= qtd) {
            estoque[item] -= qtd;
            printf("Chef %d consumiu %d do ingrediente %d. Resta: %d\n", id, qtd, item, estoque[item]);
        } else {
            printf("Chef %d não conseguiu consumir %d do ingrediente %d. Estoque insuficiente!\n", id, qtd, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_vazio[item]);    // libera espaço para reabastecimento
        sleep(rand() % 3 + 1);         // tempo de consumo
    }
    return NULL;
}

// entregadores reabastecendo ingredientes
void* delivery(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % ITEM_TYPES;  // Seleciona um tipo de ingrediente aleatoriamente
        int qtd = rand() % 10 + 5;   // Quantidade a ser reabastecida (5 a 15)

        sem_wait(&sem_vazio[item]);     // espera espaço no estoque
        pthread_mutex_lock(&mutex);     // pega acesso ao estoque

        if (estoque[item] + qtd <= MAX_INGREDIENTS) {
            estoque[item] += qtd;
            printf("Entregador %d repos %d do ingrediente %d. Total: %d\n", id, qtd, item, estoque[item]);
        } else {
            printf("Entregador %d não pode repor %d do igrediente %d. Estoque cheio!\n", id, estoque, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_cheio[item]);     // libera ingrediente para consumo
        sleep(rand() % 5 + 1);         // tempo de entrega
    }
    return NULL;
}

int main() {
    pthread_t chefs[3], deliverers[2];
    int chef_ids[3] = {1, 2, 3};
    int deliverer_ids[2] = {1, 2};

    // estoque e semáforos
    for (int i = 0; i < ITEM_TYPES; i++) {
        estoque[i] = rand() % (MAX_INGREDIENTS / 2); // estoque inicial aleatório
        sem_init(&sem_cheio[i], 0, estoque[i]);      // qtd inicial de itens disponíveis
        sem_init(&sem_vazio[i], 0, MAX_INGREDIENTS - estoque[i]); // restante
    }

    // threads de chefs e entregadores
    for (int i = 0; i < 3; i++) {
        pthread_create(&chefs[i], NULL, chef, &chef_ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&deliverers[i], NULL, delivery, &deliverer_ids[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(chefs[i], NULL);// não termina
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(deliverers[i], NULL);
    }
    for (int i = 0; i < ITEM_TYPES; i++) {
        sem_destroy(&sem_cheio[i]);
        sem_destroy(&sem_vazio[i]);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}
