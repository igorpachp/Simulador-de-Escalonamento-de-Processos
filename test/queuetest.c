#include <stdio.h>
#include <stdlib.h>
#include "../util.h"
#include "../process.h"
#include "../queue.h"

int main() {
    Process* p[10];
    Queue* Q = newQueue();
    QueueItem* QI = NULL;

    for (int i = 0; i < 10; i++) {
        p[i] = (Process*) malloc(sizeof(Process));
        p[i]->PID = i;
        p[i]->PPID = p[i]->PID-1;
        p[i]->status = READY;
        p[i]->initialTime = 0;
        p[i]->currentTime = 5;
        p[i]->completionTime = 10;
        printProcess(p[i]);
    }

    // QueueItem* QI = newQueueItem(p[2]);

    for (int i = 0; i < 10; i++) {
        push(Q, p[i]);
    }
    printf("Queue has size: %d, id for process in head is: %d, id for process in tail is: %d\n", Q->size, Q->head->process->PID, Q->tail->process->PID);

    QI = pop(Q);
    printf("popped item id is: %d\n", QI->process->PID);
    printf("Queue has size: %d, id for process in head is: %d, id for process in tail is: %d\n", Q->size, Q->head->process->PID, Q->tail->process->PID);
    freeQueueItem(QI);

    QI = pop(Q);
    printf("popped item id is: %d\n", QI->process->PID);
    printf("Queue has size: %d, id for process in head is: %d, id for process in tail is: %d\n", Q->size, Q->head->process->PID, Q->tail->process->PID);
    freeQueueItem(QI);

    freeQueue(Q);
}