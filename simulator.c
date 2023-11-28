#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "queue.h"
#include "list.h"
#include "process.h"

// -- Declarações de variáveis ------------------------------
// Tabela de processos
TableItem* processTable[MAX_PROCESS];
Process* processes[MAX_PROCESS];
int completedCounter = 0;

// filas de processos
Queue* highPrioQ;
Queue* lowPrioQ;

// filas de I/O
Queue* diskQ;
Queue* tapeQ;
Queue* printerQ;

// Gerenciamento de IO
List* IOList;

// processo em execução
Process* runningProcess = NULL;
TableItem* runningProcessInfo = NULL;

int currentTime = 0;            // Tempo desde a inicialização do escalonador
int currentProcessingTime = 0;  // Tempo desde o início do quantum
int startedProcs = 0;           // Processos iniciados
int finishedProcs = 0;          // Processos finalizados

// quantidade de processos em cada estado de I/O
int diskProcs = 0;
int tapeProcs = 0;
int printerProcs = 0;
// ----------------------------------------------------------

// -- Funções -----------------------------------------------
void init() {
    setRandomSeed();

    // Inicialização da Tabela de Processos
    for (int i = 0; i < MAX_PROCESS; i++) {
        processTable[i] = newTableItem();
    }

    // Inicialização das Filas de Prioridade
    highPrioQ = newQueue();
    lowPrioQ = newQueue();
    diskQ = newQueue();
    tapeQ = newQueue();
    printerQ = newQueue();

    // Inicialização da Lista de IO
    IOList = newList();
}

void newProcessHandler() {
    // Novos Processos no Instante Atual sao Inseridos na Fila de Alta Prioridade
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processTable[i] == NULL) continue;

        if (processTable[i]->arrivalTime == currentTime) {
            Process* aux = (Process*) malloc(sizeof(Process));
            processes[startedProcs] = aux;
            aux->PID = ++startedProcs;
            aux->PPID = 0;
            aux->status = READY;
            aux->initialTime = currentTime;
            aux->currentTime = 0;

            processTable[i]->PID = aux->PID;
            push(highPrioQ, aux);
            printf("\nProcesso #%d criado com ID %d!", i + 1, startedProcs);
        }
    }
}

void fetchNextProcess() {
    // Encontra o Proximo Processo Pronto para Execucao
    if (highPrioQ->size > 0) {
        runningProcess = pop(highPrioQ);
        runningProcess->currentTime++;
        runningProcess->status = RUN;
        currentProcessingTime = 0;
    } else if (lowPrioQ->size > 0) {
        runningProcess = pop(lowPrioQ);
        runningProcess->currentTime++;
        runningProcess->status = RUN;
        currentProcessingTime = 0;
    } else {
        printf("\nNenhum processo nas filas de pronto...\n");
        return;
    }

    printf("\nO processo %d esta executando!\n", runningProcess->PID);

    // Obtem a definicao do processo em relacao ao PID
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processTable[i] != NULL &&
            processTable[i]->PID != -1 &&
            processTable[i]->PID == runningProcess->PID) {
                runningProcessInfo = processTable[i];
                return;
        }
    }
}

void fetchFinishedIO() {
    // Verifica na lista de IO se algum processo deverá sair no instante atual
    ListItem* current = IOList->first;

    while (current != NULL) {
        ListItem* next = current->next;

        if (current->exitTime == currentTime) {
            printf("\nProcesso %d terminou uma operacao de IO!", current->process->PID);
            current->process->status = READY;

            if (current->type == DISK) {
                push(lowPrioQ, current->process);
                diskProcs--;
            } else if (current->type == TAPE) {
                push(highPrioQ, current->process);
                tapeProcs--;
            } else if (current->type == PRINTER) {
                push(highPrioQ, current->process);
                printerProcs--;
            }

            removeElement(IOList, current);
        }
        current = next;
    }
}

void startIORequisition(IOType t) {
    // Trata para qual fila de IO deve ser inserido o processo corrente
    if (t == DISK) {
        push(diskQ, runningProcess);
        runningProcess->status = WAIT;
        printf("\nO processo %d esta na fila de disco!\n", runningProcess->PID);
    } else if (t == TAPE) {
        push(tapeQ, runningProcess);
        runningProcess->status = WAIT;
        printf("\nO processo %d esta na fila de fita!\n", runningProcess->PID);
    } else if (t == PRINTER) {
        push(printerQ, runningProcess);
        runningProcess->status = WAIT;
        printf("\nO processo %d esta na fila de impressora!\n", runningProcess->PID);
    }

    runningProcess = NULL;
    runningProcessInfo = NULL;
}

void IOQueueHandler() {
    // Encontra o Proximo Processo Pronto para iniciar IO
    if (diskQ->size > 0 && diskProcs == 0) {
        Process* IOproc = pop(diskQ);
        append(IOList, newListItem(IOproc, currentTime, currentTime + DISK_TIME, DISK));
        diskProcs++;
        printf("\nO processo %d esta usando o disco!\n", IOproc->PID);
    }
    if (tapeQ->size > 0 && tapeProcs == 0) {
        Process* IOproc = pop(tapeQ);
        append(IOList, newListItem(IOproc, currentTime, currentTime + TAPE_TIME, TAPE));
        tapeProcs++;
        printf("\nO processo %d esta usando a fita!\n", IOproc->PID);
    }
    if (printerQ->size > 0 && printerProcs == 0) {
        Process* IOproc = pop(printerQ);
        append(IOList, newListItem(IOproc, currentTime, currentTime + PRINTER_TIME, PRINTER));
        printerProcs++;
        printf("\nO processo %d esta usando a impressora!\n", IOproc->PID);
    }
}

void preemptionHandler() {
    printf("\nO processo %d foi interrompido!\n", runningProcess->PID);

    runningProcess->status = READY;
    currentProcessingTime = 0;
    push(lowPrioQ, runningProcess);

    runningProcess = NULL;
    runningProcessInfo = NULL;
}

void closeProcess() {
    printf("\nO processo %d acabou!\n", runningProcess->PID);

    runningProcess->completionTime = currentTime;
    runningProcess->status = COMPLETE;

    Process* aux = runningProcess;
    processes[completedCounter++] = aux;

    runningProcess = NULL;
    runningProcessInfo = NULL;

    finishedProcs++;
}

void currentProcessHandler() {
    // verificando se o processo atual possui
    // requisição de IO no instante atual de tempo
    for (int i = 0; i < IO_LIMIT; i++) {
        if (runningProcess == NULL) break;

        if (runningProcessInfo->diskRequest[i] == runningProcess->currentTime) {
            startIORequisition(DISK);
        } else if (runningProcessInfo->tapeRequest[i] == runningProcess->currentTime) {
            startIORequisition(TAPE);
        } else if (runningProcessInfo->printerRequest[i] == runningProcess->currentTime) {
            startIORequisition(PRINTER);
        }
    }

    // preempção e término de processo
    if (runningProcess != NULL) {
        if (runningProcess->currentTime == runningProcessInfo->burstTime) {
            closeProcess();
        } else if (currentProcessingTime == QUANTUM) {
            preemptionHandler();
        } else {
            runningProcess->currentTime++;
            currentProcessingTime++;
        }
    }

    // caso nenhum processo esteja sendo executado
    if (runningProcess == NULL) {
        fetchNextProcess();
        currentProcessingTime++;
    }
}

void printProcessTable() {
    printf("\n========== Tabela de Processos ==========\n\n");

    for (int i = 0; i < MAX_PROCESS; i++) {
        printf("- - - - - - - - - - - - - - - - - - - - -");
        printf("\n #%d:\n", i + 1);
        printTableItem(processTable[i]);
    }
    printf("- - - - - - - - - - - - - - - - - - - - -");
    printf("\n");
}

void printIOTable() {
    ListItem* current = IOList->first;

    while (current != NULL) {
        printf("\n[%d: %s", current->process->PID, IOtoString(current->type));

        // if (current->type == DISK)
        //     printf("DISCO");
        // else if (current->type == TAPE)
        //     printf("FITA");
        // else if (current->type == PRINTER)
        //     printf("IMPRESSORA");

        printf(" de %d ate %d] ", current->arrivalTime, current->exitTime);
        current = current->next;
    }
}

void printCurrentState() {
    printf("\n-- Estado antes do fim do instante %d --\n\n", currentTime);

    if (runningProcess != NULL) {
        printf("Processo %d ativo: [tempo de servico: %d] [tempo no quantum: %d]\n", 
                runningProcess->PID, runningProcess->currentTime, currentProcessingTime
        );
    } else {
        printf("Nenhum processo ativo! \n");
    }

    if (runningProcessInfo != NULL) {
        printf("Informacoes do processo %d: [tempo total de servico: %d]\n",
            runningProcessInfo->PID, runningProcessInfo->burstTime
        );
    }

    if (highPrioQ->size > 0) {
        printf("Fila de alta prioridade: %d processos, %d e o primeiro\n",
            highPrioQ->size, highPrioQ->head->process->PID
        );
    } else {
        printf("Fila de alta prioridade: %d processos\n", highPrioQ->size);
    }

    if (lowPrioQ->size > 0) {
        printf("Fila de baixa prioridade: %d processos, %d e o primeiro\n",
            lowPrioQ->size, lowPrioQ->head->process->PID
        );
    } else {
        printf("Fila de baixa prioridade: %d processos\n", lowPrioQ->size);
    }

    if (diskQ->size > 0) {
        printf("Fila de disco: %d processos, %d e o primeiro\n",
               diskQ->size, diskQ->head->process->PID);
    } else {
        printf("Fila de disco: %d processos\n", diskQ->size);
    }

    if (tapeQ->size > 0) {
        printf("Fila de fita: %d processos, %d e o primeiro\n",
               tapeQ->size, tapeQ->head->process->PID);
    } else {
        printf("Fila de fita: %d Processos\n", tapeQ->size);
    }

    if (printerQ->size > 0) {
        printf("Fila de impressora: %d processos, %d e o primeiro\n",
            printerQ->size, printerQ->head->process->PID);
    } else {
        printf("Fila de impressora: %d processos\n", printerQ->size);
    }

    if (IOList->size > 0 && IOList->first != NULL) {
        printf("Lista de IOs em tratamento: ");
        printIOTable();
        printf("\n");
    } else
        printf("Lista de IOs em tratamento: Vazia\n");

    printf("\n\n");
}


// ================ MAIN ================ //

int main() {
    printf("== Inicializando o Escalonador =============\n");
    // Inicializa Filas, Lista de IO, Tabela de Definição de Processos
    init();
    printProcessTable();

    // Iterando por Unidade de Tempo
    while (finishedProcs < MAX_PROCESS) {
        printf("== Instante %d iniciado ========\n", currentTime);

        newProcessHandler();
        fetchFinishedIO();
        currentProcessHandler();
        IOQueueHandler();

        printCurrentState();

        currentTime++;
    }

    printf("== Dados dos processos finalizados ==\n");
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processes[i] != NULL)
            printf(
                "Processo %d turnaround:  %d - %d = %d\n", 
                processes[i]->PID,
                processes[i]->completionTime,
                processes[i]->initialTime,
                processes[i]->completionTime - processes[i]->initialTime
            );
        else
            printf("nulo\n");
    }

    // liberando memória antes do término
    freeList(IOList);
    freeQueue(highPrioQ);
    freeQueue(lowPrioQ);
    freeQueue(diskQ);
    freeQueue(tapeQ);
    freeQueue(printerQ);

    for (int i = 0; i < MAX_PROCESS; i++) {
        free(processes[i]);
        free(processTable[i]);
    }

    printf("\n\n== Escalonador executado com sucesso =============\n\n");

    return 0;
}