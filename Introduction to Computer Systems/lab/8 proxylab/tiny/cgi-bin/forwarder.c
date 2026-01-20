#include "csapp.h"

int getblock(void) {
    FILE *f=fopen("/tmp/tiny.blocked.flag","r");
    if(!f)
        return 0;

    int block;
    if(fscanf(f, "%d", &block)<1)
        return 0;
    fclose(f);
    return block==1;
}

int main(void) {
    char *query_string;
    char fn[MAXLINE],type[MAXLINE],content_buf[MAXLINE];
    int serial;
    char fallback_no_value[] = "0,test_files/text.txt,text/plain;charset=utf-8";

    if ((query_string = getenv("QUERY_STRING")) == NULL)
        query_string = fallback_no_value;
        
    sscanf(query_string, "%d,%[^,],%[^,]", &serial, fn, type);


    printf("Connection: close\r\n");
    printf("Content-Type: %s\r\n\r\n", type);
    
    if(serial<0 && getblock())
        exit(1);

    FILE *f=Fopen(fn, "r");
    while(!feof(f)) {
        int len=Fread((void*)content_buf, 1, MAXLINE, f);
        fwrite((void*)content_buf, 1, len, stdout);
    }
    fflush(stdout);

    exit(0);
}

