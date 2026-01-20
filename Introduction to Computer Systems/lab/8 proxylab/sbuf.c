/*
 * sbuf.c
 * name: 刘沛雨
 * ID: 2100012289
 * @brief - Implementation of a bounded buffer (sbuf) with semaphores 
            for synchronization.
 */

#include "sbuf.h"

/* Initialize the bounded buffer (sbuf) */
void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = Calloc(n, sizeof(item));  /* Allocate memory for the buffer */
    sp->n = n;                           /* Set the maximum number of slots */
    sp->front = sp->rear = 0;            /* Initialize front and rear pointers*/
    Sem_init(&sp->mutex, 0, 1);          /* Initialize the mutex semaphore */
    Sem_init(&sp->slots, 0, n);          /* Initialize the slots semaphore */
    Sem_init(&sp->items, 0, 0);          /* Initialize the items semaphore */
}

/* Insert an item into the bounded buffer (sbuf) */
void sbuf_insert(sbuf_t *sp, item item) {
    P(&sp->slots);                           /* Wait for an available slot */
    P(&sp->mutex);                           /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item;  /* Insert the item */
    V(&sp->mutex);                           /* Unlock the buffer */
    V(&sp->items);                           /* Announce an available item */
}

/* Remove an item from the bounded buffer (sbuf) */
item sbuf_remove(sbuf_t *sp) {
    item item;
    P(&sp->items);                            /* Wait for an available item */
    P(&sp->mutex);                            /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)]; /* Remove the item */
    V(&sp->mutex);                            /* Unlock the buffer */
    V(&sp->slots);                            /* Announce an available slot */
    return item;
}
