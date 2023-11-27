#include <stdio.h>
#include "../process.h"

int main() {
    setRandomSeed();

    IORequest* req = newIORequest(0, 1);
    printIORequest(req);
    freeIORequest(req);

    TableItem* item = newTableItem(1);
    printTableItem(item);
    freeTableItem(item);

}