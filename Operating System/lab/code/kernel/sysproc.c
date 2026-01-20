
#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/timer.h"
#include "include/kalloc.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/vm.h"

extern int exec(char *path, char **argv);
extern int argfd(int n, int *pfd, struct file **pf);

uint64
sys_exec(void)
{
  char path[FAT32_MAX_PATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
    return -1;
  }
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= NELEM(argv)){
      goto bad;
    }
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
      goto bad;
    }
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    argv[i] = kalloc();
    if(argv[i] == 0)
      goto bad;
    if(fetchstr(uarg, argv[i], PGSIZE) < 0)
      goto bad;
  }

  int ret = exec(path, argv);

  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);
  return -1;
}

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  
  return wait(-1, p, 0);
}

uint64
sys_waitpid(void)
{
  uint64 p;
  int pid, options;
  if(argint(0, &pid) < 0 || argaddr(1, &p) < 0 || argint(2, &options)) // 注意看测试用例的syscall.c！！
    return -1;
  
  return wait(pid, p, options);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;  // 扩张/收缩到 n bytes

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(n == 0) // n = 0，用于获取进程所占物理内存的大小
    return addr;
  if(growproc(n - addr) < 0)  // 扩张/收缩
    return -1;
  return 0;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}


struct tms              
{                     
	long tms_utime;  
	long tms_stime;  
	long tms_cutime; 
	long tms_cstime; 
};

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_times(void)
{
  uint64 addr;
  uint xticks;
  struct tms mytimes;

  if(argaddr(0, &addr) < 0)
    return -1;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  mytimes.tms_utime = xticks * 1000 / 200 ; // 以ms为单位
  mytimes.tms_stime = xticks * 1000 / 200;
  mytimes.tms_cutime = xticks * 1000 / 200;
  mytimes.tms_cstime = xticks * 1000 / 200;
  
  if(copyout2(addr, (char *)&mytimes, sizeof(mytimes)) < 0)
    return -1;

  return xticks;
}

uint64
sys_trace(void)
{
  int mask;
  if(argint(0, &mask) < 0) {
    return -1;
  }
  myproc()->tmask = mask;
  return 0;
}

uint64
sys_getppid(void)
{
  return myproc()->parent->pid;
}

uint64
sys_clone(void)
{
  return clone();
}

uint64
sys_sched_yield(void)
{
  yield();
  return 0;
}

struct timespec {
	long   tv_sec;      
	long   tv_usec;     
};

uint64
sys_get_time(void)
{
  uint xticks;
  uint64 addr;
  int num;
  struct timespec tmp;
  if(argaddr(0, &addr) < 0 || argint(1, &num) < 0)
    return -1;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  // printf("time = %d\n", fre);

  tmp.tv_sec = xticks / 200;
  tmp.tv_usec = (xticks * 1000000 / 200) % 1000000;
  if(copyout2(addr, (char*)&tmp, sizeof(tmp)) < 0)
    return -1;
  
  return 0;
}

uint64
sys_nanosleep(void)
{
  uint64 addr1, addr2;
  uint xticks0;
  struct timespec tv;

  if(argaddr(0, &addr1) < 0 || argaddr(1, &addr2) < 0)
    return -1;
  if(copyin2((char *)&tv, addr1, sizeof(struct timespec)) < 0)
    return -1;
  
  acquire(&tickslock);
  xticks0 = ticks;
  while((ticks - xticks0) / 200 < tv.tv_sec){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_mmap(void){
  // struct file *f;
  // uint64 vaddr, len, off;
  // int prot, flags, fd;
  // if(argaddr(0, &vaddr) < 0)
  //   return -1;
  // if(argaddr(1, &len) < 0)
  //   return -1;
  // if(argint(2, &prot) < 0)
  //   return -1;
  // if(argint(3, &flags) < 0)
  //   return -1;
  // if(argfd(4, &fd, &f) < 0)
  //   return -1;
  // if(argaddr(5, &off) < 0)
  //   return -1;
  
	// return vaddr;
  return 0;
}

uint64
sys_munmap(void){
	return 0;
}