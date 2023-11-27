#include <stdlib.h>
#include <time.h>
#include "util.h"

void setRandomSeed() {
    srand(time(0));
}

int getRandomInt(int floor, int ceil) {
    if (ceil < floor) return -1;

    return rand() % ceil + floor;
}