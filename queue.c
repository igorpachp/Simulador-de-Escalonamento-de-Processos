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
QueueItem* newQueueItem (Process* queuedProcess) {
    if (queuedProcess == NULL) return NULL;

    QueueItem* item = (QueueItem*) malloc(sizeof(QueueItem));

    item->process = queuedProcess;
    item->next = NULL;

    return item;
}

/* Esta função libera o espaço de memória ocupado
 * por uma entrada de processo em uma fila,
 * cuidando para que o espaço ocupado pelo processo
 * referenciado internamente também seja liberado.
 * 
 * Esta função não retém informações sobre a entrada de
 * processo previamente apontada pelo elemento liberado,
 * ficando a cargo do programador guardar esta informação
 * antes da chamada da função, se isto for de interesse.
 * 
 * Entradas:
 *      item -> a entrada que deve ser liberada.
 */
void freeQueueItem (QueueItem* item){
    if (item == NULL) return;
    if (item->process != NULL) free(item->process);

    free(item);
}

/* A função a seguir aloca um espaço em memória para
 * uma nova fila, inicializando seu tamanho em 0 e suas
 * suas referências como NULL.
 * 
 * Saidas:
 *      Q -> o ponteiro para a fila recém criada.
 */
Queue* newQueue () {
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
    if (Q == NULL || p == NULL) return;

    QueueItem* item = newQueueItem(p);

    if (Q->size == 0) {
        Q->head = Q->tail = item;
    } else {
        Q->tail->next = item;
        Q->tail = item;
    }

    Q->size++;
}

/* A função pop remove a primeira entrada da fila 'Q'.
 * Após a remoção, o processo primeiro processo da fila
 * será aquele referenciado como o próximo da entrada
 * removida. A função também atualiza o tamanho da fila.
 * 
 * Entradas:
 *      Q -> um ponteiro para uma fila de processos.
 * Saidas:
 *      NULL -> quando o ponteiro fornecido é inválido,
 *              ou quando a fila não possui entradas.
 *      aux -> o ponteiro para a entrada recém removida.
 */
QueueItem* pop(Queue* Q) {
    if (Q == NULL || Q->size == 0) return NULL;

    QueueItem* aux = Q->head;
    Q->head = Q->head->next;
    Q->size--;

    if (Q->size == 0) Q->tail = NULL;

    return aux;
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
    if (Q == NULL) return;

    while (Q->size > 0) {
        freeQueueItem(pop(Q));
    }

    free(Q);
}
