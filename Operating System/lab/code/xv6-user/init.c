#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/file.h"
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

// char *argv[] = { "sh", 0 };
char *argv[] = {0};
char *tests[] = {
    "brk",
    "chdir",
    "clone",
    "close",
    "dup",
    "dup2",
    "execve",
    "exit",
    "fork",
    "fstat",
    "getcwd",
    "getdents",
    "getpid",
    "getppid",
    "gettimeofday",
    "mkdir_",
    "mmap",
    "mount",
    "munmap",
    "open",
    "openat",
    "pipe",
    "read",
    "sleep",
    "test_echo",
    "times",
    "umount",
    "uname",
    "unlink",
    "wait",
    "waitpid",
    "write",
    "yield",
};

int
main(void)
{
    int pid, wpid;
    // int pid;
    dev(O_RDWR, CONSOLE, 0);
    dup(0);  // stdout
    dup(0);  // stderr

    printf("init: starting tests\n");

    for(;;){
        for(int i = 0; i < 33; i++){
            pid = fork();
            if(pid < 0){
                printf("init: fork failed\n");
                exit(1);
            }
            if(pid == 0){
                exec(tests[i], argv);
                printf("init: exec test failed\n");
                exit(1);
            }
            else{
                wait((int*)0);
            }
        }
        (*(volatile uint32 *) 0x100000) = 0x5555; // write PHYSICAL address

        for(;;){
        // this call to wait() returns if the shell exits,
        // or if a parentless process exits.
        wpid = wait((int *) 0);
        if(wpid == pid){
            // the shell exited; restart it.
            break;
        } else if(wpid < 0){
            printf("init: wait returned an error\n");
            exit(1);
        } else {
            // it was a parentless process; do nothing.
        }
        }
    }
    return 0;
}
