/* 
 * mycont.c - Parent forks a child; child sents SIGCONT to parent.
 */ 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "config.h"

void sigalrm_handler() 
{
    exit(0);
}

char buf[MAXBUF];
int syncfd;

int main(int argc, char **argv) 
{
    signal(SIGALRM, sigalrm_handler);
    alarm(JOB_TIMEOUT);

    if(fork() == 0) {
        /* Child. */
        setpgid(getpid(), 0);   // Detach from parent process group

        int standalone = 1;
        char *str;
        char *cmdp;
        struct stat stat;

        if ((str = getenv("SYNCFD")) != NULL) {
            syncfd = atoi(str);              /* Get descriptor number */
            if (fstat(syncfd, &stat) != -1)  /* Is is open? */
                standalone = 0;
        }

        if (!standalone) {
            alarm(JOB_TIMEOUT);
            cmdp = "";
            ssize_t rc;
            if ((rc = send(syncfd, cmdp, strlen(cmdp), 0)) < 0) {
                perror("send");
                exit(1);
            }
            if ((rc = recv(syncfd, buf, MAXBUF, 0)) < 0) {
                perror("recv");
                exit(1);
            }
            /* Send SIGCONT after handshake */
            kill(getppid(), SIGCONT);
            exit(0);
        }
    }

    if (argc > 1)
        alarm(atoi(argv[1]));
    else
        alarm(JOB_TIMEOUT);

    while(1);
    exit(0);
}
