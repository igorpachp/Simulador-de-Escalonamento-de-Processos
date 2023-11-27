#include <stdio.h>
#include "../queue.h"

int main() {
    Queue* Q = newQueue();
    QueueItem* item = NULL;

    push(Q, 1);
    push(Q, 2);
    push(Q, 3);
    printf("Queue has size: %d, head id is: %d, tail id is: %d\n", Q->size, Q->head->id, Q->tail->id);

    item = pop(Q);
    printf("popped item id is: %d\n", item->id);
    printf("Queue has size: %d, head id is: %d, tail id is: %d\n", Q->size, Q->head->id, Q->tail->id);

    freeQueueItem(item);
    if (item == NULL) puts("removido");

    item = pop(Q);
    printf("popped item id is: %d\n", item->id);
    printf("Queue has size: %d, head id is: %d, tail id is: %d\n", Q->size, Q->head->id, Q->tail->id);
    freeQueueItem(item);

    freeQueue(Q);
}