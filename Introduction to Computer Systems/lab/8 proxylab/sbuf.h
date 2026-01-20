/*
 * sbuf.h
 * name: 刘沛雨
 * ID: 2100012289
 * @brief - Implementation of a bounded buffer (sbuf) with semaphores 
            for synchronization.
 */

#include "csapp.h"

/* Structure to represent an item */
typedef struct {
    int fd;      /* File descriptor */
    int stamp;   /* Stamp value */
} item;

/* Structure for a bounded buffer (sbuf) */
typedef struct {
    item *buf;       /* Buffer array */
    int n;           /* Maximum number of slots */
    int front;       /* buf[(front+1)%n] is the first item */
    int rear;        /* buf[rear%n] is the last item */
    sem_t mutex;     /* Protects accesses to buf */
    sem_t slots;     /* Counts available slots */
    sem_t items;     /* Counts available items */
} sbuf_t;

/* Function prototypes for the sbuf (bounded buffer) */
void sbuf_init(sbuf_t *sp, int n);
void sbuf_insert(sbuf_t *sp, item item);
item sbuf_remove(sbuf_t *sp);
