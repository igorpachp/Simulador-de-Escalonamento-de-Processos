#include <stdio.h>
#include <stdlib.h>
#include "../util.h"
#include "../process.h"
#include "../list.h"

int main() {
    Process* p;
    List* l = newList();
    ListItem* items[10];
    ListItem* LI = NULL;

    for (int i = 0; i < 10; i++) {
        p = (Process*) malloc(sizeof(Process));
        p->PID = i;
        p->PPID = p->PID-1;
        p->status = READY;
        p->initialTime = 0;
        p->currentTime = 5;
        p->completionTime = 10;

        items[i] = newListItem(p, i, i + 5, TAPE);
    }

    for (int i = 0; i < 10; i++) {
        append(l, items[i]);
    }
    printf("List has size: %d, id for process in head is: %d, id for process in tail is: %d\n", l->size, l->first->process->PID, l->last->process->PID);

    LI = removeFirst(l);
    printf("removed item id is: %d\n", LI->process->PID);
    printf("List has size: %d, id for process in head is: %d, id for process in tail is: %d\n", l->size, l->first->process->PID, l->last->process->PID);
    free(LI->process);
    freeListItem(LI);

    LI = removeFirst(l);
    printf("removed item id is: %d\n", LI->process->PID);
    printf("List has size: %d, id for process in head is: %d, id for process in tail is: %d\n", l->size, l->first->process->PID, l->last->process->PID);
    free(LI->process);
    freeListItem(LI);

    LI = l->first;
    for (int i = 0; i < l->size; i++) {
        free(LI->process);
        LI = LI->next;
    }
    freeList(l);
}