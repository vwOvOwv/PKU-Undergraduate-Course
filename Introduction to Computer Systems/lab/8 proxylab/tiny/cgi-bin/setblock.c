#include "csapp.h"

int main(void) {
    char *query_string;
    int block;
    char fallback_no_value[] = "0,0";

    if ((query_string = getenv("QUERY_STRING")) == NULL)
        query_string = fallback_no_value;

    sscanf(query_string, "%*d,%d", &block);
    
    FILE *f=Fopen("/tmp/tiny.blocked.flag","w");
    fprintf(f, "%d\n", block);
    fclose(f);
  
    printf("Connection: close\r\n");
    printf("Content-Type: text/plain\r\n\r\n");
    printf("ok");
    fflush(stdout);

    exit(0);
}

