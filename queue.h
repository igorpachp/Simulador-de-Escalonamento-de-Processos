#ifndef __QUEUE_H__
#define __QUEUE_H__ "queue_lib"

#include "util.h"
#include "process.h"

// estrutura responsável por encadear as entradas em uma fila
typedef struct _QueueItem {
    Process* process;
    struct _QueueItem* next;
} QueueItem;

// estrutura que mantém os dados da fila
typedef struct _Queue {
    int size;
    QueueItem *head, *tail;
} Queue;

QueueItem* newQueueItem(Process* queuedProcess);    // Criar um novo item para uma fila
Queue* newQueue();                                  // Criar uma nova fila
void push(Queue* Q, Process* p);                    // Adicionar um novo processo a uma fila
Process* pop(Queue* Q);                             // Remover um item de uma fila
void freeQueue(Queue* Q);                           // Liberando o espaço de uma fila e de seus itens

#endif
