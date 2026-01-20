/*
 * cache.h
 * name: 刘沛雨
 * ID: 2100012289
 * @brief - Header file for a cache implementation used in a tiny web proxy
 */

#include "csapp.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Recommended max cache and object sizes */
#define MAX_BLOCK_NUM 10
#define MAX_OBJECT_SIZE 102400
#define NTHREADS 4
#define SBUFSIZE 16
#define INT_MAX 0x7fffffff

/* Data structure for the cache */
typedef struct {
    char object[MAX_OBJECT_SIZE];
    char marker[MAXLINE];
    int stamp;
    int file_size;

    int reader_cnt;
    sem_t w;
    sem_t mutex;
} cache_block;

typedef struct {
    cache_block blocks[MAX_BLOCK_NUM];
    int block_cnt;
} cache;

/* Function prototypes for the cache */
void cache_init(cache *cp, int n);
int find_block(cache *my_cache, char *file_path);
void read_block(cache *my_cache, int pos, int clientfd, int stamp);
void write_block(cache *my_cache, char *marker, char *tmp_cache,
                 int file_size, int stamp);
