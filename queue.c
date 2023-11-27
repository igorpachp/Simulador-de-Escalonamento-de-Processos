#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

QueueItem* newQueueItem (Process* queuedProcess) {
    if (queuedProcess == NULL) return NULL;

    QueueItem* item = (QueueItem*) malloc(sizeof(QueueItem));

    item->process = queuedProcess;
    item->next = NULL;

    return item;
}

void freeQueueItem (QueueItem* item){
    if (item == NULL) return;
    if (item->process != NULL) free(item->process);

    free(item);
}

Queue* newQueue () {
    Queue* Q = (Queue*) malloc(sizeof(Queue));

    Q->size = 0;
    Q->head = NULL;
    Q->tail = NULL;

    return Q;
}

void push(Queue* Q, Process* p) {
    QueueItem* item = newQueueItem(p);

    if (Q->size == 0) {
        Q->head = Q->tail = item;
    } else {
        Q->tail->next = item;
        Q->tail = item;
    }

    Q->size++;
}

QueueItem* pop(Queue* Q) {
    if (Q->size == 0) return NULL;

    QueueItem* aux = Q->head;
    Q->head = Q->head->next;
    Q->size--;

    if (Q->size == 0) Q->tail = NULL;

    return aux;
}

void freeQueue(Queue* Q) {
    if (Q == NULL) return;

    while (Q->size > 0) {
        freeQueueItem(pop(Q));
    }

    free(Q);
}
