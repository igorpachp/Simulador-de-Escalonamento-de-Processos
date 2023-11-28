#ifndef __PROCESS_H__
#define __PROCESS_H__ "process_lib"

#include "util.h"

// A instância de um processo, também usada como PCB
typedef struct _Process {
    int PID;
    int PPID;
    States status;
	int initialTime;
    int currentTime;
	int completionTime;
} Process;


// Estrutura responsável por agregar as informações
// da tabela de processos 
typedef struct _TableItem {
    int PID;
    int arrivalTime;
    int burstTime;

    // Para os tempos de entrada e saída em cada fila de IO
    int diskRequest[IO_LIMIT];
    int tapeRequest[IO_LIMIT];
    int printerRequest[IO_LIMIT];
} TableItem;

int requestedTimeIsTaken(TableItem *p, int r);                      // verifica se uma entrada da tabela de processos já possui requsição de IO em determinado tempo
TableItem *newTableItem();                                          // cria uma nova entrada na tabela de processos

void printTableItem(TableItem *ti);                                 // exibe as informações de uma entrada na tabela de processos

#endif
