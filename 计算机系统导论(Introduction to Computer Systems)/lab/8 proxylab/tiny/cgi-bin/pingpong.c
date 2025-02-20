#include "csapp.h"

int main(void) {
    char *query_string, *headers;
    char content[MAXLINE];
    char fallback_no_value[] = "[no value]";

    if ((query_string = getenv("QUERY_STRING")) == NULL)
        query_string = fallback_no_value;
    if ((headers = getenv("REQUEST_HEADERS")) == NULL)
        headers = fallback_no_value;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-overflow"
    sprintf(content, "This is ping-pong service.\r\nYour query string: ");
    sprintf(content, "%s%s\r\n", content, query_string);
    sprintf(content, "%sRequest headers:\r\n%s", content, headers);
    sprintf(content, "%s(headers end)\r\n", content);
#pragma GCC diagnostic pop
  
    printf("Connection: close\r\n");
    printf("Content-Length: %d\r\n", (int)strlen(content));
    printf("Content-Type: text/plain\r\n");
    printf("X-Powered-By: Ako from Team NFO\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}

