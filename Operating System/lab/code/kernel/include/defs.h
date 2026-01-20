struct buf;
struct context;
struct dirent;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;
// enum spi_device_num_t;
// enum spi_work_mode_t;
// enum spi_frame_format_t;
// bio.c
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
void            bpin(struct buf*);
void            bunpin(struct buf*);

// console.c
void            consoleinit(void);
void            consoleintr(int);
void            consputc(int);

// timer.c
void timerinit();
void set_next_timeout();
void timer_tick();

// disk.c
void            disk_init(void);
void            disk_read(struct buf *b);
void            disk_write(struct buf *b);
void            disk_intr(void);

// exec.c
int             exec(char*, char**);

// fat32.c
int             fat32_init(void);
struct dirent*  dirlookup(struct dirent *entry, char *filename, uint *poff);
struct dirent*  ealloc(struct dirent *dp, char *name, int dir);
struct dirent*  edup(struct dirent *entry);
void            eupdate(struct dirent *entry);
void            etrunc(struct dirent *entry);
void            eput(struct dirent *entry);
void            estat(struct dirent *ep, struct stat *st);
void            elock(struct dirent *entry);
void            eunlock(struct dirent *entry);
int             enext(struct dirent *dp, struct dirent *ep, uint off, int *count);
struct dirent*  ename(char *path);
struct dirent*  enameparent(char *path, char *name);
int             eread(struct dirent *entry, int user_dst, uint64 dst, uint off, uint n);
int             ewrite(struct dirent *entry, int user_src, uint64 src, uint off, uint n);

// file.c
struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, uint64, int n);
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);
int             dirnext(struct file *f, uint64 addr);

// fs.c
// void            fsinit(int);
// int             dirlink(struct inode*, char*, uint);
// struct inode*   dirlookup(struct inode*, char*, uint*);
// struct inode*   ialloc(uint, short);
// struct inode*   idup(struct inode*);
// void            iinit();
// void            ilock(struct inode*);
// void            iput(struct inode*);
// void            iunlock(struct inode*);
// void            iunlockput(struct inode*);
// void            iupdate(struct inode*);
// int             namecmp(const char*, const char*);
// struct inode*   namei(char*);
// struct inode*   nameiparent(char*, char*);
// int             readi(struct inode*, int, uint64, uint, uint);
// void            stati(struct inode*, struct stat*);
// int             writei(struct inode*, int, uint64, uint, uint);
// void            itrunc(struct inode*);

// kalloc.c
void*           kalloc(void);
void            kfree(void *);
void            kinit(void);
uint64          freemem_amount(void);

// log.c
// void            initlog(int, struct superblock*);
// void            log_write(struct buf*);
// void            begin_op(void);
// void            end_op(void);

// pipe.c
int             pipealloc(struct file**, struct file**);
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, uint64, int);
int             pipewrite(struct pipe*, uint64, int);

// printf.c
void            printstring(const char* s);
void            printf(char*, ...);
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

// proc.c
void            reg_info(void);
int             cpuid(void);
void            exit(int);
int             fork(void);
int             growproc(int);
pagetable_t     proc_pagetable(struct proc *);
void            proc_freepagetable(pagetable_t, uint64);
int             kill(int);
struct cpu*     mycpu(void);
struct cpu*     getmycpu(void);
struct proc*    myproc();
void            procinit(void);
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
void            setproc(struct proc*);
void            sleep(void*, struct spinlock*);
void            userinit(void);
int             wait(uint64);
void            wakeup(void*);
void            yield(void);
int             either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int             either_copyin(void *dst, int user_src, uint64 src, uint64 len);
void            procdump(void);
void            test_proc_init(int);

// swtch.S
void            swtch(struct context*, struct context*);

// spinlock.c
void            acquire(struct spinlock*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);

// intr.c
void            push_off(void);
void            pop_off(void);

// sleeplock.c
void            acquiresleep(struct sleeplock*);
void            releasesleep(struct sleeplock*);
int             holdingsleep(struct sleeplock*);
void            initsleeplock(struct sleeplock*, char*);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);
void            wnstr(wchar *dst, char const *src, int len);
void            snstr(char *dst, wchar const *src, int len);
int             wcsncmp(wchar const *s1, wchar const *s2, int len);

// syscall.c
int             argint(int, int*);
int             argstr(int, char*, int);
int             argaddr(int, uint64 *);
int             fetchstr(uint64, char*, int);
int             fetchaddr(uint64, uint64*);
void            syscall();

// trap.c
extern uint     ticks;
void            trapinit(void);
void            trapinithart(void);
extern struct spinlock tickslock;
void            usertrapret(void);
#ifndef QEMU
void            supervisor_external_handler(void);
#endif
void            device_init(unsigned long, uint64);

// uart.c
void            uartinit(void);
void            uartintr(void);
void            uartputc(int);
void            uartputc_sync(int);
int             uartgetc(void);

// vm.c
void            kvminit(void);
void            kvminithart(void);
uint64          kvmpa(uint64);
void            kvmmap(uint64, uint64, uint64, int);
int             mappages(pagetable_t, uint64, uint64, uint64, int);
pagetable_t     uvmcreate(void);
void            uvminit(pagetable_t, uchar *, uint);
uint64          uvmalloc(pagetable_t, uint64, uint64);
uint64          uvmdealloc(pagetable_t, uint64, uint64);
int             uvmcopy(pagetable_t, pagetable_t, uint64);
void            uvmfree(pagetable_t, uint64);
void            uvmunmap(pagetable_t, uint64, uint64, int);
void            uvmclear(pagetable_t, uint64);
uint64          walkaddr(pagetable_t, uint64);
int             copyout(pagetable_t, uint64, char *, uint64);
int             copyin(pagetable_t, char *, uint64, uint64);
int             copyinstr(pagetable_t, char *, uint64, uint64);

// virtio_disk.c
void            virtio_disk_init(void);
void            virtio_disk_rw(struct buf *b, int write);
void            virtio_disk_intr(void);

// plic.c
void            plicinit(void);
void            plicinithart(void);
int             plic_claim(void);
void            plic_complete(int);

// logo.c
void            print_logo(void);

// test.c
void            test_kalloc(void);
void            test_vm(unsigned long);
void            test_sdcard(void);
// spi.c
// void spi_init(spi_device_num_t spi_num, spi_work_mode_t work_mode, spi_frame_format_t frame_format,
//               uint64 data_bit_length, uint32 endian);


// void            ptesprintf(pagetable_t, int);
// int             vmprint(pagetable_t);

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))
