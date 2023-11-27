#ifndef __PROCESS_H__
#define __PROCESS_H__ "process_lib"

#include "util.h"

typedef struct _Process {
    int PID;
    int PPID;
    States status;
    int initialTime;
    int currentTime;
    int completionTime;
} Process;

typedef struct _IORequest {
    IOType type;
    int time;
} IORequest;

typedef struct _TableItem {
    int PID;
    int arrivalTime;
    int burstTime;
    IORequest* ioRequest[IO_LIMIT];
} TableItem;

IORequest* newIORequest(IOType type, int time);                     // cria nova requisição de dispositivo de IO
void freeIORequest(IORequest* request);                             // libera o espaço de memória ocupado por uma requisição de IO
int requestedTimeIsTaken(TableItem* process, int request);          // verifica se uma entrada da tabela de processos já possui requsição de IO em determinado tempo
TableItem* newTableItem(int newID);                                 // cria uma nova entrada na tabela de processos
void freeTableItem(TableItem* process);                             // cria uma nova entrada na tabela de processos

void printIORequest(IORequest* request);                            // exibe as informações de uma requisição de IO
void printTableItem(TableItem* ti);                                 // exibe as informações de uma entrada na tabela de processos

#endif
