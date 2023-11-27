#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "process.h"

IORequest* newIORequest(IOType type, int time){
    IORequest* newRequest = (IORequest*) malloc(sizeof(IORequest));

    newRequest->time = time;
    newRequest->type = type;

    return newRequest;
}

void freeIORequest(IORequest* request){
    if (request == NULL) return;

    free(request);
}

int requestedTimeIsTaken(TableItem* process, int request) {
    if (process == NULL) return 0;

    for (int i = 0; i < IO_LIMIT; i++) {
        if (process->ioRequest[i] != NULL && process->ioRequest[i]->time == request) {
            return 1;
        }
    }

    return 0;
}

TableItem* newTableItem(int newID) {
    TableItem* novo = (TableItem*) malloc(sizeof(TableItem));

    novo->arrivalTime = getRandomInt(0, MAX_ARRIVAL);
    novo->burstTime = getRandomInt(1, MAX_BURST_TIME);
    novo->PID = -1; // ainda não inicializado

    for (int i = 0; i < IO_LIMIT; i++) {
        novo->ioRequest[i] = NULL;
    }

    // determinando número de requisições de IO
    int IOrequests = 0;
    if (novo->burstTime > 1) {
        IOrequests = getRandomInt(0, IO_LIMIT < novo->burstTime ? IO_LIMIT : novo->burstTime);
    }

    // determinando o tipo de dispositivo e tempo de requisição
    for (int i = 0; i < IOrequests; i++) {
        IOType newType = (IOType) getRandomInt(0, IO_OPTIONS);
        int newTime = getRandomInt(1, novo->burstTime - 1);

        // verificando conflito de tempo
        while (requestedTimeIsTaken(novo, newTime)) {
            newTime = getRandomInt(1, novo->burstTime - 1);
        }

        novo->ioRequest[i] = newIORequest(newType, newTime);
    }

    // ordenando requisições
    for (int j = 0; j < IOrequests - 1; j++) {
        for (int k = j+1; k < IOrequests; k++) {
            if (novo->ioRequest[j]->time > novo->ioRequest[k]->time) {
                IORequest* aux = novo->ioRequest[j];
                novo->ioRequest[j] = novo->ioRequest[k];
                novo->ioRequest[k] = aux;
            }
        }
    }

    return novo;
}

void freeTableItem(TableItem* ti) {
    if (ti == NULL) return;

    for (int i = 0; i < IO_LIMIT; i++) {
        freeIORequest(ti->ioRequest[i]);
    }

    free(ti);
}

void printProcess(Process* p) {
    if (p == NULL) return;

    printf("\nPID: %d\n", p->PID);
    printf("PPID: %d\n", p->PPID);
    switch (p->status)
    {
    case NEW:
        printf("STATUS: %s\n", "NEW");
        break;
    case READY:
        printf("STATUS: %s\n", "READY");
        break;
    case RUN:
        printf("STATUS: %s\n", "RUN");
        break;
    case COMPLETE:
        printf("STATUS: %s\n", "COMPLETE");
        break;
    default:
        printf("STATUS: %s\n", "WAIT");
        break;
    }
    printf("Initial Time: %d\n", p->initialTime);
    printf("Current Time: %d\n", p->currentTime);
    printf("Completion Time: %d\n", p->completionTime);

    printf("\n");
}

void printIORequest(IORequest* r) {
    if (r == NULL) return;

    printf("\nRequest Time: %d\n", r->time);
    printf("Request Type: %d\n", r->type);

    printf("\n");
}

void printTableItem(TableItem* ti) {
    if (ti == NULL) return;

    printf("\nPID: %d\n", ti->PID);
    printf("Arrival Time: %d\n", ti->arrivalTime);
    printf("Burst Time: %d\n", ti->burstTime);

    printf("IO Requests ---------\n");
    for (int i = 0; i < IO_LIMIT; i++) {
        printIORequest(ti->ioRequest[i]);
    }
    printf("---------------------\n");

    printf("\n");
}

