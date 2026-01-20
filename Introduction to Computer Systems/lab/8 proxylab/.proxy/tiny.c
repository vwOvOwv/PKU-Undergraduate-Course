/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
#include "csapp.h"
#include <stdio.h>

void doit(int fd);
void read_requesthdrs(rio_t *rp, char *req_header_buf);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs, char *headers);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

void sigchld_handler(int sig) { // reap all children
    int bkp_errno = errno;
    while(waitpid(-1, NULL, WNOHANG)>0);
    errno=bkp_errno;
}

int main(int argc, char **argv) 
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigchld_handler);

    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }

    listenfd = open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
	doit(connfd);                                             //line:netp:tiny:doit
	close(connfd);                                            //line:netp:tiny:close
    }
}
/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    char req_header_buf[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    rio_readinitb(&rio, fd);
    if (!rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
    if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                                                    //line:netp:doit:endrequesterr
    read_requesthdrs(&rio, req_header_buf);              //line:netp:doit:readrequesthdrs

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);       //line:netp:doit:staticcheck
    if (stat(filename, &sbuf) < 0) {                     //line:netp:doit:beginnotfound
	clienterror(fd, filename, "404", "Not found",
		    "Tiny couldn't find this file");
	return;
    }                                                    //line:netp:doit:endnotfound

    if (is_static) { /* Serve static content */          
	if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { //line:netp:doit:readable
	    clienterror(fd, filename, "403", "Forbidden",
			"Tiny couldn't read the file");
	    return;
	}
	serve_static(fd, filename, sbuf.st_size);        //line:netp:doit:servestatic
    }
    else { /* Serve dynamic content */
	if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { //line:netp:doit:executable
	    clienterror(fd, filename, "403", "Forbidden",
			"Tiny couldn't run the CGI program");
	    return;
	}
	serve_dynamic(fd, filename, cgiargs, req_header_buf);//line:netp:doit:servedynamic
    }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp, char *req_header_buf) 
{
    char buf[MAXLINE];
    req_header_buf[0]='\0';

    rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    strcat(req_header_buf, buf);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
        rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
        strcat(req_header_buf, buf);
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs) 
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
        strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
        strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
        strcat(filename, uri);                           //line:netp:parseuri:endconvert1
        if (uri[strlen(uri)-1] == '/')                   //line:netp:parseuri:slashcheck
            strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
        return 1;
    }
    else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
        ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
        if (ptr) {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else 
            strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
        strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
        strcat(filename, uri);                           //line:netp:parseuri:endconvert2
        return 0;
    }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);       //line:netp:servestatic:getfiletype
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-overflow"
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sVary: *\r\n", buf);
    sprintf(buf, "%sCache-Control: no-cache, no-store, must-revalidate\r\n", buf);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
#pragma GCC diagnostic pop
    rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
    printf("Response headers:\n");
    printf("%s", buf);

    /* Send response body to client */
    srcfd = open(filename, O_RDONLY, 0);    //line:netp:servestatic:open
    srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);//line:netp:servestatic:mmap
    close(srcfd);                           //line:netp:servestatic:close
    rio_writen(fd, srcp, filesize);         //line:netp:servestatic:write
    munmap(srcp, filesize);                 //line:netp:servestatic:munmap
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else if (strstr(filename, ".css"))
        strcpy(filetype, "text/css");
    else if (strstr(filename, ".js"))
        strcpy(filetype, "application/javascript");
    else
        strcpy(filetype, "text/plain");
}  
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs, char *headers) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    sprintf(buf, "%sVary: *\r\n", buf);
    sprintf(buf, "%sCache-Control: no-cache, no-store, must-revalidate\r\n", buf);
    rio_writen(fd, buf, strlen(buf));
  
    int pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Tiny failed to fork CGI process!\n");
        return;
    }
  
    if (pid == 0) { /* Child */ //line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
        setenv("REQUEST_HEADERS", headers, 1);
        dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
        execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    } else {
        // change in proxylab:
        // parent do not wait for /cgi-bin/repeater
        // allowing it to run in the background
        if(strstr(filename, "repeater")==NULL)
            wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
    }
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-overflow"
    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
#pragma GCC diagnostic pop

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}
/* $end clienterror */
