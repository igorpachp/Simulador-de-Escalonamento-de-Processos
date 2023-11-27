#ifndef __LIST_H__
#define __LIST_H__ "list_lib"

#include "util.h"
#include "process.h"

typedef struct _ListItem {
    int arrivalTime, exitTime;
    IOType device;
    Process* process;
    struct _ListItem* next;
} ListItem;

typedef struct _List {
    int size;
    ListItem *first, *last;
} List;

// Criar um novo item para uma lista
ListItem* newListItem(Process* p, int arr, int exit, IOType type);
// Liberando o espaço de um item
void freeListItem(ListItem* item);
// criar uma nova lista
List* newList();
// Liberando o espaço de uma lista e de seus itens
void freeList(List* L);
// Adicionar um novo item ao inicio de uma lista
void append(List* l, ListItem* item);
// Remover o primeiro item de uma lista
ListItem* removeFirst(List* l);

void printList(List* l);

#endif
