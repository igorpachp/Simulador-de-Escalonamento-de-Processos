#include <stdlib.h>
#include <stdio.h>
// #include <time.h>
#include "util.h"
#include "process.h"

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
int requestedTimeIsTaken(TableItem *p, int r) {
    for (int i = 0; i < IO_LIMIT; i++) {
        if (p->diskRequest[i] == r ||
            p->tapeRequest[i] == r ||
            p->printerRequest[i] == r)
            return 1;
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
TableItem *newTableItem() {
    TableItem *newTI = (TableItem *) malloc(sizeof(TableItem));

    // aleatorizando tempos de chegada e serviço
    newTI->arrivalTime = getRandomInt(0, MAX_ARRIVAL);
    newTI->burstTime = getRandomInt(1, MAX_BURST_TIME);
    newTI->PID = -1; // ainda não inicializado

    // Inicializa as entradas com -1
    for (int i = 0; i < IO_LIMIT; i++) {
        newTI->diskRequest[i] = -1;
        newTI->tapeRequest[i] = -1;
        newTI->printerRequest[i] = -1;
    }

    // Obtem as entradas em IO
    for (int i = 0; i < getRandomInt(0, IO_LIMIT); i++) {
        int requests = getRandomInt(1, newTI->burstTime - 1);
        if (!requestedTimeIsTaken(newTI, requests))
            newTI->diskRequest[i] = requests;
    }
    for (int i = 0; i < getRandomInt(0, IO_LIMIT); i++) {
        int requests = getRandomInt(1, newTI->burstTime - 1);
        if (!requestedTimeIsTaken(newTI, requests))
            newTI->tapeRequest[i] = requests;
    }
    for (int i = 0; i < getRandomInt(0, IO_LIMIT); i++) {
        int requests = getRandomInt(1, newTI->burstTime - 1);
        if (!requestedTimeIsTaken(newTI, requests))
            newTI->printerRequest[i] = requests;
    }

    return newTI;
}

void printTableItem(TableItem *ti) {
    printf("\nPID: %d\n", ti->PID);
    printf("Arrival Time: %d\n", ti->arrivalTime);
    printf("Burst Time: %d\n", ti->burstTime);
    printf("Disk requests: \t");
    for (int i = 0; i < IO_LIMIT; i++) {
        if (ti->diskRequest[i] != -1)
            printf("%-2d ", ti->diskRequest[i]);
        else
            printf("*  ");
    }
    printf("\nTape requests: \t");
    for (int i = 0; i < IO_LIMIT; i++) {
        if (ti->tapeRequest[i] != -1)
            printf("%-2d ", ti->tapeRequest[i]);
        else
            printf("*  ");
    }
    printf("\nPrinter requests: ");
    for (int i = 0; i < IO_LIMIT; i++) {
        if (ti->printerRequest[i] != -1)
            printf("%-2d ", ti->printerRequest[i]);
        else
            printf("*  ");
    }

    printf("\n");
}