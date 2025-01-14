//
// Created by nicjl on 13/01/2025.
//

#ifndef PEDIDOS_DELIVERY_H
#define PEDIDOS_DELIVERY_H

#include <pthread.h>

// Estrutura que representa a distribuição de pedidos
typedef struct {
    int *pedidos_prontos;    // Fila de pedidos prontos para entrega
    int *entregadores;       // Array de entregadores disponíveis
    int num_entregadores;    // Número de entregadores
    pthread_mutex_t mutex;   // Mutex para controle da distribuição
    pthread_cond_t cond_distribuicao; // Condição para sinalizar que há pedidos
} PedidosDelivery;

// Funções para manipulação da distribuição de pedidos
void inicializar_delivery(PedidosDelivery *delivery, int num_entregadores);
void atribuir_pedido(PedidosDelivery *delivery, int pedido_id, int entregador_id);
void finalizar_entrega(PedidosDelivery *delivery, int entregador_id);
void destruir_delivery(PedidosDelivery *delivery);

void* distribuicao_pedidos(void *arg);

#endif //PEDIDOS_DELIVERY_H
