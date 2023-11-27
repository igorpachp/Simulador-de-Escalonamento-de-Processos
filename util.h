#ifndef __UTIL_H__
#define __UTIL_H__ "util_lib"

#define QUANTUM         2
#define MAX_PROCESS     5
#define MAX_ARRIVAL     15
#define MAX_BURST_TIME  15

#define IO_LIMIT        5   // limite de vezes que um processo pode requisitar dispositivos de IO
#define IO_OPTIONS      3   // número de diferentes tipos de dispositivos IO
#define DISC_TIME       2   // tempo de IO em disco
#define TAPE_TIME       4   // tempo de IO em fita
#define PRINTER_TIME    8   // tempo de IO em impressora

typedef enum _States {NEW, READY, RUN, COMPLETE, WAIT} States;
typedef enum _IOType {DISK, TAPE, PRINTER} IOType;

void setRandomSeed();
int getRandomInt(int floor, int ceil);

#endif
