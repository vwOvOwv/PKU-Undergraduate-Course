#include "csapp.h"

#define MANYTIMES_RATE 1024

int main(void) {
    char *query_string;
    char content[MAXLINE];
    int times,delay_ms;
    int manytimes_flag=0;
    char fallback_no_value[] = "1,-1,[novalue],0";

    if ((query_string = getenv("QUERY_STRING")) == NULL)
        query_string = fallback_no_value;
        
    sscanf(query_string, "%*d,%d,%[^,],%d", &times, content, &delay_ms);

    if(times<0)
        times=-times, manytimes_flag=1;

    printf("Connection: close\r\n");
    printf("Content-Length: %d\r\n", (int)strlen(content)*times*(manytimes_flag?MANYTIMES_RATE:1));
    printf("Content-Type: text/plain\r\n\r\n");
    while(times--) {
        if(delay_ms) {
            fflush(stdout);
            usleep(delay_ms*1000);
        }
        if(manytimes_flag)
            for(int i=0; i<MANYTIMES_RATE; i++)
                printf("%s", content);
        else
            printf("%s", content);
    }
    fflush(stdout);

    exit(0);
}

