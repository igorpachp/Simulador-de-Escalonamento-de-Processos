#ifndef __UTIL_H__
#define __UTIL_H__ "util_lib"

#define QUANTUM         2   // tamanho do time slice para processos em execução
#define MAX_PROCESS     5   // número de processos na tabela
#define MAX_ARRIVAL     15  // maior instante de tempo permitido para entrada de um novo processo
#define MAX_BURST_TIME  15  // limite de tempo de execução necessário para um processo

#define IO_LIMIT        5   // limite de vezes que um processo pode requisitar dispositivos de IO
#define IO_OPTIONS      3   // número de diferentes tipos de dispositivos IO
#define DISC_TIME       2   // tempo de IO em disco
#define TAPE_TIME       4   // tempo de IO em fita
#define PRINTER_TIME    8   // tempo de IO em impressora

// enum de estados possíveis para os processos
typedef enum _States {NEW, READY, RUN, COMPLETE, WAIT} States;
// enum com os diferentes tipos de dispositivos de entrada
typedef enum _IOType {DISK, TAPE, PRINTER} IOType;

void setRandomSeed();                   // gera uma semente para números aleatórios
int getRandomInt(int floor, int ceil);  // gera um número aleatório no intervalo [floor, ceil[

#endif
