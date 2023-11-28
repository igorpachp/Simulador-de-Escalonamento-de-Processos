#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* Esta função aloca um espaço em memória para
 * uma nova entrada de processo em uma lista e
 * inicializa seus campos.
 * 
 * Entradas:
 *      p -> um ponteiro para um processo.
 *      arr -> o tempo de chegada.
 *      exit -> o tempo de saida.
 *      type -> o tipo de dispositivo de I/O acionado.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido.
 *      item -> o ponteiro para a entrada recém criada.
 */
ListItem* newListItem(Process* p, int arr, int exit, IOType type) {
    ListItem* item = (ListItem*) malloc(sizeof(ListItem));
    item->process = p;
    item->arrivalTime = arr;
    item->exitTime = exit;
    item->type = type;
    item->next = NULL;

    return item;
}

/* A função a seguir aloca um espaço em memória para
 * uma nova lista, inicializando seu tamanho em 0 e
 * suas referências como NULL.
 * 
 * Saidas:
 *      l -> o ponteiro para a lista recém criada.
 */
List* newList() {
    List* l = (List*) malloc(sizeof(List));

    l->size = 0;
    l->first = NULL;

    return l;
}

/* Esta função libera o espaço de memória ocupado pela
 * lista 'l' cuidando para que os espaços ocupados pelas
 * entradas de processos contidas na lista também sejam
 * liberados.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma lista de processos.
 */
void freeList(List* l) { 
    while(removeElement(l, 0) != NULL) {
        continue;
    }

    free(l); 
}

/* A função append adiciona a entrada 'item' a lista
 * 'l'. Após a inserção, a entrada inserida será a 
 * última da lista.
 * 
 * Entradas:
 *      l -> um ponteiro para uma lista de processos.
 *      item -> um ponteiro para uma entrada de lista.
 */
void append(List* l, ListItem* item) {
    if (l->first == NULL) {
        l->first = item;
    } else {
        ListItem* p = l->first;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = item;
    }

    l->size++;
}

/* A função removeElemento remove a primeira entrada da 
 * lista 'l' igual ao elemento informado. A função 
 * também atualiza o tamanho da lista e libera o espaço
 * previamente ocupado pelo elemento removido.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma lista de processos.
 *      e -> um ponteiro para um item.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido,
 *              ou quando a lista não possui entradas.
 *      aux -> o ponteiro para a entrada recém removida.
 */
ListItem* removeElement(List* l, ListItem* e) {
    if (l->first == NULL) return NULL;

    if (l->first->next == NULL) {
        if (l->first == e) {
            free(l->first);
            l->first = NULL;
            l->size--;
            return e;
        } else
            return NULL;
    }

    if (l->first == e && l->first->next != NULL) {
        ListItem* prox = l->first->next;
        free(l->first);
        l->first = prox;
        return prox;
    }

    ListItem* previous = l->first;
    ListItem* current = l->first->next;

    while (current != e && current->next != NULL) {
        previous = current;
        current = current->next;
    }

    if (current == e) {
        if (current->next == NULL) {
            previous->next = NULL;
        } else {
            previous->next = current->next;
        }
        l->size--;

        free(current);

        return e;
    } else
        return NULL;
}

void printList(List* l) {
    ListItem* item = l->first;

    printf("\nSize: %d\n", l->size);
    while (item != NULL) {
        printf("Arrival: %d, Exit: %d, IOType: %s\n", item->arrivalTime, item->exitTime, IOtoString(item->type));
        item = item->next;
    }
}
