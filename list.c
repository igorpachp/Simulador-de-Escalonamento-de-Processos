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
    if (p == NULL) return NULL;

    ListItem* item = (ListItem*) malloc(sizeof(ListItem));
    item->process = p;
    item->arrivalTime = arr;
    item->exitTime = exit;
    item->device = type;
    item->next = NULL;

    return item;
}

/* Esta função libera o espaço de memória ocupado
 * por uma entrada de processo em uma lista. Nesta
 * função, o espaço ocupado pelo processo referenciado
 * internamente também seja NÃO é liberado.
 * 
 * Esta função não retém informações sobre a entrada de
 * processo previamente apontada pelo elemento liberado,
 * ficando a cargo do programador guardar esta informação
 * antes da chamada da função, se isto for de interesse.
 * 
 * Entradas:
 *      item -> a entrada que deve ser liberada.
 */
void freeListItem (ListItem* item){
    if (item == NULL) return;

    free(item);
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
    l->last = NULL;

    return l;
}

/* Esta função libera o espaço de memória ocupado pela
 * lista 'l' cuidando para que os espaços ocupados pelas
 * entradas de processos contidas na lista também sejam
 * liberados.
 * 
 * Os processos referenciados pelas entradas NÂO são
 * liberados, caso seja de interesse o programador deve
 * salvar as referências antes de executar a função para
 * poder liberar os processos.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma lista de processos.
 */
void freeList(List* l) {
    if (l == NULL) return;

    while (l->size > 0) {
        freeListItem(removeFirst(l));
    }

    free(l);
}

/* A função append adiciona a entrada 'item' a lista
 * 'l'. * Após a inserção, a entrada inserida será a 
 * última da lista.
 * 
 * Entradas:
 *      l -> um ponteiro para uma lista de processos.
 *      item -> um ponteiro para uma entrada de lista.
 */
void append(List* l, ListItem* item) {
    if (l == NULL || item == NULL) return;

    if (l->size == 0) {
        l->first = l->last = item;
    } else {
        l->last->next = item;
        l->last = item;
    }

    l->size++;
}

/* A função removeFirst remove a primeira entrada da 
 * lista 'l'. Após a remoção, o primeiro processo da
 * lista será aquele referenciado como o próximo da 
 * entrada removida. A função também atualiza o 
 * tamanho da lista.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma lista de processos.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido,
 *              ou quando a lista não possui entradas.
 *      aux -> o ponteiro para a entrada recém removida.
 */
ListItem* removeFirst(List* l) {
    if (l == NULL || l->size == 0) return NULL;

    ListItem* aux = l->first;
    l->first = l->first->next;
    l->size--;

    if (l->size == 0) l->last = NULL;

    return aux;
}

void printList(List* l) {
    ListItem* item = l->first;

    printf("\nSize: %d\n", l->size);
    while (item != NULL) {
        printf("Arrival: %d, Exit: %d, IOType: %d\n", item->arrivalTime, item->exitTime, item->device);
        item = item->next;
    }
}
