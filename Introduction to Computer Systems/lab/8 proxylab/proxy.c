/*
 * proxy.c
 * name: 刘沛雨
 * ID: 2100012289
 * @brief - A tiny web proxy that supports HTTP request
 */

#include "cache.h"
#include "csapp.h"
#include "sbuf.h"
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

/* Data structure */
typedef struct {
    char host[MAXLINE];
    char port[MAXLINE];
    char file_path[MAXLINE];
} url_data;

/* Function prototypes for the proxy */
void doit(int fd, int stamp);
void parse_url(char *url, url_data *URL);
int set_request(rio_t *client_buf, url_data *URL, char *request);
void *thread(void *vargp);

/* User agent header */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

/* Global variables */
sbuf_t sbuf;
cache my_cache;

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);

    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    int global_stamp = 0;

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    /* Check command-line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    cache_init(&my_cache, MAX_BLOCK_NUM);
    sbuf_init(&sbuf, SBUFSIZE);

    /* Create worker threads */
    for (int i = 0; i < NTHREADS; i++) {
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        global_stamp++;
        item tmp;
        tmp.stamp = global_stamp;
        tmp.fd = connfd;
        sbuf_insert(&sbuf, tmp);
    }

    return 0;
}

/*
 * doit - handle one HTTP request/response transaction
 */
void doit(int clientfd, int stamp) {
    int serverfd;

    url_data *URL;
    char buf[MAXLINE], method[MAXLINE], url[MAXLINE], version[MAXLINE];
    char request[MAXLINE];
    rio_t client_buf, server_buf;

    /* Read request line and headers */
    Rio_readinitb(&client_buf, clientfd);
    ssize_t m;
    m = Rio_readlineb(&client_buf, buf, INT_MAX);
    if (m > MAXLINE || m == -1 || m == 0)
        return;
    sscanf(buf, "%s %s %s", method, url, version);
    if (strcasecmp(method, "GET")) {
        printf("Proxy does not implement this method\n");
        return;
    }

    /* Parse URL to get URI and Host */
    URL = (url_data *)Malloc(sizeof(url_data));
    parse_url(url, URL);

    /* Check if the file is cached */
    int pos;
    if ((pos = find_block(&my_cache, URL->file_path)) != -1) {
        read_block(&my_cache, pos, clientfd, stamp);
        return;
    }

    /* Set HTTP request */
    if (set_request(&client_buf, URL, request) == -1)
        return;

    /* Connect to the server */
    serverfd = Open_clientfd(URL->host, URL->port);

    /* Send HTTP request to the server */
    Rio_readinitb(&server_buf, serverfd);
    Rio_writen(serverfd, request, strlen(request));

    /* Acquire HTTP response and forward it to the client */
    size_t n;
    int file_size = 0;
    char tmp_cache[MAX_OBJECT_SIZE];
    char *tmp = tmp_cache;
    while ((n = Rio_readnb(&server_buf, buf, MAXLINE)) != 0) {
        Rio_writen(clientfd, buf, n);
        file_size += n;
        if (file_size <= MAX_OBJECT_SIZE) {
            memcpy(tmp, buf, n);
            tmp += n;
        }
    }
    if (file_size > 0 && file_size <= MAX_OBJECT_SIZE)
        write_block(&my_cache, URL->file_path, tmp_cache, file_size, stamp);
    
    /* Free allocated space */
    Free(URL);
    /* Close the serverfd */
    Close(serverfd);
}

/* Parse URL to get host, port, and file path */
void parse_url(char *url, url_data *URL) {
    char *host_pos = strstr(url, "//");
    if (host_pos == NULL) {
        char *path_pos = strstr(url, "/");
        if (path_pos)
            strcpy(URL->file_path, path_pos);
        strcpy(URL->port, "80");
    } else {
        host_pos += 2;
        strcpy(URL->host, host_pos);
        char *port_pos = strstr(URL->host, ":");
        char *path_pos = strstr(URL->host, "/");
        if (path_pos) {
            strcpy(URL->file_path, path_pos);
            *path_pos = '\0';
        }
        if (port_pos) {
            strcpy(URL->port, port_pos + 1);
            *port_pos = '\0';
        } else {
            strcpy(URL->port, "80");
        }
    }
}

/* Set the HTTP request headers */
int set_request(rio_t *client_buf, url_data *URL, char *request) {
    char tmp[2 * MAXLINE], buf[2 * MAXLINE];
    sprintf(request, "GET %s HTTP/1.0\r\n", URL->file_path);
    size_t n;
    while ((n = Rio_readlineb(client_buf, buf, INT_MAX)) != 0) {
        if (n > MAXLINE)
            return -1;
        if (!strcmp(buf, "\r\n"))
            break;
        if (strstr(buf, "User-Agent:"))
            continue;
        if (strstr(buf, "Connection:"))
            continue;
        if (strstr(buf, "Proxy-Connection:"))
            continue;
        sprintf(tmp, "%s", buf);
        strcat(request, tmp);
    }
    if (!strstr(request, "Host:")) {
        sprintf(tmp, "Host: %s\r\n", URL->host);
        strcat(request, tmp);
    }
    sprintf(tmp, "%s", user_agent_hdr);
    strcat(request, tmp);
    sprintf(tmp, "Connection: close\r\n");
    strcat(request, tmp);
    sprintf(tmp, "Proxy-Connection: close\r\n\r\n");
    strcat(request, tmp);
    return 0;
}

/* Worker thread function */
void *thread(void *vargp) {
    Pthread_detach(Pthread_self());
    while (1) {
        item item = sbuf_remove(&sbuf);
        int connfd = item.fd;
        doit(connfd, item.stamp);
        Close(connfd);
    }
}
