/*
 * cache.c
 * name: 刘沛雨
 * ID: 2100012289
 * @brief - A simple cache implementation for a web proxy 
            that supports HTTP requests.
 */

#include "cache.h"
#include "csapp.h"

/* Function to initialize the cache */
void cache_init(cache *cp, int n) {
    cp->block_cnt = 0;
    for (int i = 0; i < n; i++) {
        cp->blocks[i].reader_cnt = 0;
        cp->blocks[i].marker[0] = '\0';
        cp->blocks[i].object[0] = '\0';
        cp->blocks[i].file_size = 0;
        Sem_init(&(cp->blocks[i].mutex), 0, 1);
        Sem_init(&(cp->blocks[i].w), 0, 1);
    }
}

/* Function to find a block in the cache based on the file path */
int find_block(cache *my_cache, char *file_path) {
    for (int i = 0; i < my_cache->block_cnt; i++) {
        if (!strcmp(file_path, my_cache->blocks[i].marker)) {
            return i;
        }
    }
    return -1;
}

/* Function to read a block from the cache and send it to the client */
void read_block(cache *my_cache, int pos, int clientfd, int stamp) {
    P(&my_cache->blocks[pos].mutex);
    my_cache->blocks[pos].reader_cnt++;
    if (my_cache->blocks[pos].reader_cnt == 1) {
        P(&my_cache->blocks[pos].w);
    }
    V(&my_cache->blocks[pos].mutex);
    my_cache->blocks[pos].stamp = stamp;
    Rio_writen(clientfd, my_cache->blocks[pos].object, 
                my_cache->blocks[pos].file_size);
    P(&my_cache->blocks[pos].mutex);
    my_cache->blocks[pos].reader_cnt--;
    if (my_cache->blocks[pos].reader_cnt == 0)
        V(&my_cache->blocks[pos].w);
    V(&my_cache->blocks[pos].mutex);
}

/* Function to write a block to the cache */
void write_block(cache *my_cache, char *marker, char *tmp_cache, 
                  int file_size, int stamp) {
    if (my_cache->block_cnt != MAX_BLOCK_NUM) {
        P(&my_cache->blocks[my_cache->block_cnt].w);
        strcpy(my_cache->blocks[my_cache->block_cnt].marker, marker);
        my_cache->blocks[my_cache->block_cnt].stamp = stamp;
        my_cache->blocks[my_cache->block_cnt].file_size = file_size;
        memset(my_cache->blocks[my_cache->block_cnt].object, 0, 
              MAX_OBJECT_SIZE);
        memcpy(my_cache->blocks[my_cache->block_cnt].object, 
               tmp_cache, file_size);
        V(&my_cache->blocks[my_cache->block_cnt].w);
        my_cache->block_cnt++;
    } else {
        int min_stamp = INT_MAX;
        int evicted_pos = -1;
        for (int i = 0; i < my_cache->block_cnt; i++) {
            P(&my_cache->blocks[i].w);
            if (my_cache->blocks[i].stamp < min_stamp) {
                min_stamp = my_cache->blocks[i].stamp;
                evicted_pos = i;
            }
            V(&my_cache->blocks[i].w);
        }
        P(&my_cache->blocks[evicted_pos].w);
        strcpy(my_cache->blocks[evicted_pos].marker, marker);
        my_cache->blocks[evicted_pos].stamp = stamp;
        my_cache->blocks[evicted_pos].file_size = file_size;
        memset(my_cache->blocks[evicted_pos].object, 0, MAX_OBJECT_SIZE);
        memcpy(my_cache->blocks[evicted_pos].object, tmp_cache, file_size);
        V(&my_cache->blocks[evicted_pos].w);
    }
}
