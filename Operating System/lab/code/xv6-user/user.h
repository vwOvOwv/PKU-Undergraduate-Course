#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/fcntl.h"

struct stat;
struct rtcdate;
struct sysinfo;
// struct dirent;
// struct utsname;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
// int waitpid(int, int*, int);
int pipe(int*);
int write(int fd, const void *buf, int len);
int read(int fd, void *buf, int len);
int close(int fd);
int kill(int pid);
int exec(char*, char**);
int open(const char *filename, int mode);
int fstat(int fd, struct stat*);
int mkdir(const char *dirname);
int chdir(const char *dirname);
int dup(int fd);
int getpid(void);
char* sbrk(int size);
int sleep(int ticks);
int times(void);
int test_proc(int);
int dev(int, short, short);
int readdir(int fd, struct stat*);
int getcwd(char *buf, int size);
int remove(char *filename);
int trace(int mask);
int sysinfo(struct sysinfo *);
int rename(char *old, char *new);
// int clone(int (*fn)(void *), void* func_args, void* child_stack, int stack_size, int flags);
// int getppid(void);
// int getdents(int fd, struct dirent* buf, int len);
// int sched_yield(void);
// int uname(struct utsname *);
// int unlink(int fd, char *path, int flags);
// int gettimeofday(void);
// int sleep(unsigned long long time);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
