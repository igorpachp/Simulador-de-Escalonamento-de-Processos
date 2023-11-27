#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "process.h"

/* Esta função aloca o espaço de memória para uma
 * requisição de I/O e inicializa o tipo de dispositivo
 * e o tempo relativo em que a requisição ocorre.
 * 
 * Entradas:
 *      type -> o tipo de dispositivo I/O.
 *      time -> o instante de tempo.
 * Saidas:
 *      newRequest -> a requisição recém alocada.
 */
IORequest* newIORequest(IOType type, int time) {
    IORequest* newRequest = (IORequest*) malloc(sizeof(IORequest));

    newRequest->time = time;
    newRequest->type = type;

    return newRequest;
}

/* Esta função libera o espaço de memória usado por uma
 * requisição de I/O.
 * 
 * Entradas:
 *      r -> um ponteiro para uma requisição de I/O.
 */
void freeIORequest(IORequest* r) {
    if (r == NULL) return;

    free(r);
}

/* A função a seguir verifica se uma entrada da tabela
 * de processos já possui uma requisição de I/O em um
 * dado tempo.
 * 
 * Entradas:
 *      p -> um ponteiro para a tabela de processos.
 *      r -> o instante de tempo.
 * Saidas:
 *      0 -> quando o ponteiro fornecido é inválido,
 *          ou quando o instante de tempo está livre.
 *      1 -> já existe outra requisição neste instante.
 */
int requestedTimeIsTaken(TableItem* p, int r) {
    if (p == NULL) return 0;

    for (int i = 0; i < IO_LIMIT; i++) {
        if (p->ioRequest[i] != NULL && p->ioRequest[i]->time == r) {
            return 1;
        }
    }

    return 0;
}

/* A função a seguir aloca espaço na memória para uma 
 * nova entrada na tabela de processos, determina o
 * número de requisições de I/O que o processo fará e
 * gera tempos de chegada e serviço aleatoriamente.
 * 
 * Entradas:
 *      newID -> o id para o processo.
 * Saidas:
 *      novo -> o ponteiro para a nova entrada da tabela.
 */
TableItem* newTableItem(int newID) {
    TableItem* newTI = (TableItem*) malloc(sizeof(TableItem));

    // aleatorizando tempos de chegada e serviço
    newTI->arrivalTime = getRandomInt(0, MAX_ARRIVAL);
    newTI->burstTime = getRandomInt(1, MAX_BURST_TIME);
    newTI->PID = -1; // ainda não inicializado

    // inicializando vetor de requisições
    for (int i = 0; i < IO_LIMIT; i++) {
        newTI->ioRequest[i] = NULL;
    }

    // determinando número de requisições de IO
    int IOrequests = 0;
    if (newTI->burstTime > 1) {
        // garantindo que nenhuma requisição acontece
        // no primeiro ou último instante de execução
        IOrequests = getRandomInt(0, IO_LIMIT < newTI->burstTime ? IO_LIMIT : newTI->burstTime);
    }

    // determinando aleatoriamente o tipo de dispositivo e tempo de requisição
    for (int i = 0; i < IOrequests; i++) {
        IOType newType = (IOType) getRandomInt(0, IO_OPTIONS);
        int newTime = getRandomInt(1, newTI->burstTime - 1);

        // evitando conflito de tempo
        while (requestedTimeIsTaken(newTI, newTime)) {
            newTime = getRandomInt(1, newTI->burstTime - 1);
        }

        newTI->ioRequest[i] = newIORequest(newType, newTime);
    }

    // ordenando requisições
    for (int j = 0; j < IOrequests - 1; j++) {
        for (int k = j+1; k < IOrequests; k++) {
            if (newTI->ioRequest[j]->time > newTI->ioRequest[k]->time) {
                IORequest* aux = newTI->ioRequest[j];
                newTI->ioRequest[j] = newTI->ioRequest[k];
                newTI->ioRequest[k] = aux;
            }
        }
    }

    return newTI;
}

/* A função a seguir libera o espaço de memória ocupado
 * por uma entrada na tabela de processos e suas
 * requisições de I/O.
 * 
 * Entradas:
 *      ti -> um ponteiro para a tabela de processos.
 */
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
