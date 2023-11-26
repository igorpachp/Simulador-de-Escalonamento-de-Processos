#ifndef __QUEUE_H__
#define __QUEUE_H__ "queue_lib"

#include <stdio.h>
#include <stdlib.h>

typedef struct _QueueItem {
    int id;
    struct _QueueItem* next;
} QueueItem;

typedef struct _Queue {
    int size;
    QueueItem *head, *tail;
} Queue;

QueueItem* newQueueItem (int qiId);                     // Criar um novo item para uma fila
void freeQueueItem (QueueItem* item);                   // Liberando o espaço de um item
Queue* newQueue ();                                     // criar uma nova fila
void push(Queue* Q, int id);                            // Adicionar um novo item a uma fila
QueueItem* pop(Queue* Q);                               // Remover um item de uma fila
void freeQueue (Queue* Q);                   // Liberando o espaço de uma fila e de seus itens

#endif
