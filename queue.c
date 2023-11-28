#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* Esta função aloca um espaço em memória para
 * uma nova entrada de processo em uma fila. A 
 * função também inicializa esta entrada com
 * o processo recebido.
 * 
 * Entradas:
 *      queuedProcess -> um ponteiro para um processo.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido.
 *      item -> o ponteiro para a entrada recém criada.
 */
QueueItem* newQueueItem(Process* queuedProcess) {
    QueueItem* item = (QueueItem*) malloc(sizeof(QueueItem));

    item->process = queuedProcess;
    item->next = NULL;

    return item;
}

/* A função a seguir aloca um espaço em memória para
 * uma nova fila, inicializando seu tamanho em 0 e
 * suas referências como NULL.
 * 
 * Saidas:
 *      Q -> o ponteiro para a fila recém criada.
 */
Queue* newQueue() {
    Queue* Q = (Queue*) malloc(sizeof(Queue));

    Q->size = 0;
    Q->head = NULL;
    Q->tail = NULL;

    return Q;
}

/* A função push adiciona o processo 'p' a fila 'Q'.
 * Após a inserção, o processo inserido será o último
 * da fila. A função também é responsável por chamar
 * a rotina que cria insere o processo em uma entrada
 * para que possa ser devidamente inserido na fila.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma fila de processos.
 *      p -> um ponteiro para um processo.
 */
void push(Queue* Q, Process* p) {
    QueueItem* elem = newQueueItem(p);

    if (Q->tail == NULL) {
        Q->head = elem;
        Q->tail = elem;
    } else {
        Q->tail->next = elem;
        Q->tail = elem;
    }

    Q->size++;
}

/* A função pop remove a primeira entrada da fila 'Q'.
 * Após a remoção, o primeiro processo da fila será 
 * aquele referenciado como o próximo da entrada
 * removida. A função também atualiza o tamanho da fila
 * e libera o espaço usado pelo item removido.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma fila de processos.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido,
 *              ou quando a fila não possui entradas.
 *      aux -> o ponteiro para o processo contido na entrada recém removida.
 */
Process* pop(Queue* Q) {
    if (Q->head == NULL) return NULL;

    QueueItem* aux = Q->head;
    Process* head = Q->head->process;

    Q->head = Q->head->next;
    Q->size--;

    if (Q->head == NULL) Q->tail = NULL;
    free(aux);

    return head;
}

/* Esta função libera o espaço de memória ocupado pela
 * fila 'Q' cuidando para que os espaços ocupados pelas
 * entradas de processos contidas na fila também sejam
 * liberados.
 * 
 * Os processos referenciados pelas entradas também são
 * liberados, caso seja de interesse o programador deve
 * salvar seu conteúdo antes da chamada da função.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma fila de processos.
 */
void freeQueue(Queue* Q) { 
    while(pop(Q) != NULL) {
        continue;
    }
    free(Q); 
}
