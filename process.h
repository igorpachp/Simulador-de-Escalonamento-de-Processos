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

// Estrutura auxiliar para agregar informações sobre
// requisições de dispositivos de I/O
typedef struct _IORequest {
    IOType type;
    int time;
} IORequest;

// Estrutura responsável por agregar as informações
// da tabela de processos 
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

void printProcess(Process* p);                                      // exibe as informaçõs de um processo
void printIORequest(IORequest* r);                                  // exibe as informações de uma requisição de IO
void printTableItem(TableItem* ti);                                 // exibe as informações de uma entrada na tabela de processos

#endif
