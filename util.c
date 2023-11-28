#include <stdlib.h>
#include <time.h>
#include "util.h"

char* IOtoString(IOType t) {
    char* IOTypeString[] = {"Disco", "Fita", "Impressora"};

    return IOTypeString[t];
}

void setRandomSeed() {
    srand(time(0));
}

int getRandomInt(int floor, int ceil) {
    if (ceil < floor) return -1;

    return rand() % ceil + floor;
}
