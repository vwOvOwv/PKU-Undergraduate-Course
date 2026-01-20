
#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/sysinfo.h"
#include "include/kalloc.h"
#include "include/vm.h"
#include "include/string.h"
#include "include/printf.h"

// Fetch the uint64 at addr from the current process.
int
fetchaddr(uint64 addr, uint64 *ip)
{
	struct proc *p = myproc();
	if(addr >= p->sz || addr+sizeof(uint64) > p->sz)
		return -1;
	// if(copyin(p->pagetable, (char *)ip, addr, sizeof(*ip)) != 0)
	if(copyin2((char *)ip, addr, sizeof(*ip)) != 0)
		return -1;
	return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Returns length of string, not including nul, or -1 for error.
int
fetchstr(uint64 addr, char *buf, int max)
{
	// struct proc *p = myproc();
	// int err = copyinstr(p->pagetable, buf, addr, max);
	int err = copyinstr2(buf, addr, max);
	if(err < 0)
		return err;
	return strlen(buf);
}

static uint64
argraw(int n)
{
	struct proc *p = myproc();
	switch (n) {
	case 0:
		return p->trapframe->a0;
	case 1:
		return p->trapframe->a1;
	case 2:
		return p->trapframe->a2;
	case 3:
		return p->trapframe->a3;
	case 4:
		return p->trapframe->a4;
	case 5:
		return p->trapframe->a5;
	}
	panic("argraw");
	return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
	*ip = argraw(n);
	return 0;
}

// Retrieve an argument as a pointer.
// Doesn't check for legality, since
// copyin/copyout will do that.
int
argaddr(int n, uint64 *ip)
{
	*ip = argraw(n);
	return 0;
}

// Fetch the nth word-sized system call argument as a null-terminated string.
// Copies into buf, at most max.
// Returns string length if OK (including nul), -1 if error.
int
argstr(int n, char *buf, int max)
{
	uint64 addr;
	if(argaddr(n, &addr) < 0)
		return -1;
	return fetchstr(addr, buf, max);
}

extern uint64 sys_chdir(void);
extern uint64 sys_close(void);
extern uint64 sys_dup(void);
extern uint64 sys_exec(void);
extern uint64 sys_exit(void);
extern uint64 sys_fork(void);
extern uint64 sys_fstat(void);
extern uint64 sys_getpid(void);
extern uint64 sys_kill(void);
extern uint64 sys_mkdir(void);
extern uint64 sys_open(void);
extern uint64 sys_pipe(void);
extern uint64 sys_read(void);
extern uint64 sys_sbrk(void);
extern uint64 sys_sleep(void);
extern uint64 sys_wait(void);
extern uint64 sys_waitpid(void);
extern uint64 sys_write(void);
extern uint64 sys_times(void);
extern uint64 sys_test_proc(void);
extern uint64 sys_dev(void);
extern uint64 sys_readdir(void);
extern uint64 sys_getcwd(void);
extern uint64 sys_remove(void);
extern uint64 sys_trace(void);
extern uint64 sys_sysinfo(void);
extern uint64 sys_rename(void);
extern uint64 sys_dup3(void);
extern uint64 sys_getppid(void);
extern uint64 sys_clone(void);
extern uint64 sys_getdents(void);
extern uint64 sys_sched_yield(void);
extern uint64 sys_uname(void);
extern uint64 sys_unlink(void);
extern uint64 sys_get_time(void);
extern uint64 sys_nanosleep(void);
extern uint64 sys_mount(void);
extern uint64 sys_umount(void);
extern uint64 sys_mmap(void);
extern uint64 sys_munmap(void);

static uint64 (*syscalls[])(void) = {
	[SYS_fork]        sys_fork,
	[SYS_exit]        sys_exit,
	[SYS_wait]        sys_wait,
	[SYS_waitpid]     sys_waitpid,
	[SYS_pipe]        sys_pipe,
	[SYS_read]        sys_read,
	[SYS_kill]        sys_kill,
	[SYS_exec]        sys_exec,
	[SYS_fstat]       sys_fstat,
	[SYS_chdir]       sys_chdir,
	[SYS_dup]         sys_dup,
	[SYS_getpid]      sys_getpid,
	[SYS_sbrk]        sys_sbrk,
	[SYS_sleep]       sys_sleep,
	[SYS_times]       sys_times,
	[SYS_open]        sys_open,
	[SYS_write]       sys_write,
	[SYS_mkdir]       sys_mkdir,
	[SYS_close]       sys_close,
	[SYS_test_proc]   sys_test_proc,
	[SYS_dev]         sys_dev,
	[SYS_readdir]     sys_readdir,
	[SYS_getcwd]      sys_getcwd,
	[SYS_remove]      sys_remove,
	[SYS_trace]       sys_trace,
	[SYS_sysinfo]     sys_sysinfo,
	[SYS_rename]      sys_rename,
	[SYS_dup3]        sys_dup3,
	[SYS_getppid]     sys_getppid,
	[SYS_clone]       sys_clone,
	[SYS_getdents]    sys_getdents,
	[SYS_sched_yield] sys_sched_yield,
	[SYS_uname]       sys_uname,
	[SYS_unlinkat]    sys_unlink,
	[SYS_gettimeofday]sys_get_time,
	[SYS_nanosleep]   sys_nanosleep,
	[SYS_mount]			sys_mount,
	[SYS_umount]		sys_umount,
	[SYS_mmap]			sys_mmap,
	[SYS_munmap]		sys_munmap
};

static char *sysnames[] = {
	[SYS_fork]        "fork",
	[SYS_exit]        "exit",
	[SYS_wait]        "wait",
	[SYS_waitpid]     "waitpid",
	[SYS_pipe]        "pipe",
	[SYS_read]        "read",
	[SYS_kill]        "kill",
	[SYS_exec]        "exec",
	[SYS_fstat]       "fstat",
	[SYS_chdir]       "chdir",
	[SYS_dup]         "dup",
	[SYS_getpid]      "getpid",
	[SYS_sbrk]        "sbrk",
	[SYS_sleep]       "sleep",
	[SYS_times]       "times",
	[SYS_open]        "open",
	[SYS_write]       "write",
	[SYS_mkdir]       "mkdir",
	[SYS_close]       "close",
	[SYS_test_proc]   "test_proc",
	[SYS_dev]         "dev",
	[SYS_readdir]     "readdir",
	[SYS_getcwd]      "getcwd",
	[SYS_remove]      "remove",
	[SYS_trace]       "trace",
	[SYS_sysinfo]     "sysinfo",
	[SYS_rename]      "rename",
	[SYS_dup3]        "dup3",
	[SYS_getppid]     "getppid",
	[SYS_clone]       "clone",
	[SYS_getdents]    "getdents",
	[SYS_sched_yield] "sched_yield",
	[SYS_uname]       "uname",
	[SYS_unlinkat]    "unlink",
	[SYS_gettimeofday]"get_time",
	[SYS_nanosleep]   "nanosleep",
	[SYS_mount]			"mount",
	[SYS_umount]		"umount",
	[SYS_mmap]			"mmap",
	[SYS_munmap]		"munmap"		
};

void
syscall(void)
{
	int num;
	struct proc *p = myproc();

	num = p->trapframe->a7;
	if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
		p->trapframe->a0 = syscalls[num]();
			// trace
		if ((p->tmask & (1 << num)) != 0) {
		printf("pid %d: %s -> %d\n", p->pid, sysnames[num], p->trapframe->a0);
		}
	} else {
		printf("pid %d %s: unknown sys call %d\n",
				p->pid, p->name, num);
		p->trapframe->a0 = -1;
	}
}

uint64 
sys_test_proc(void) {
	int n;
	argint(0, &n);
	printf("hello world from proc %d, hart %d, arg %d\n", myproc()->pid, r_tp(), n);
	return 0;
}

uint64
sys_sysinfo(void)
{
	uint64 addr;
	// struct proc *p = myproc();

	if (argaddr(0, &addr) < 0) {
		return -1;
	}

	struct sysinfo info;
	info.freemem = freemem_amount();
	info.nproc = procnum();

	// if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0) {
	if (copyout2(addr, (char *)&info, sizeof(info)) < 0) {
		return -1;
	}

	return 0;
}

uint64
sys_uname(void)
{
	uint64 addr;

	if (argaddr(0, &addr) < 0) {
		return -1;
	}

	struct utsname info;
	char *sysname = "\n Sysname: An os with no worse alternative\n";
	char *nodename = "Nodename: The worst os\n";
	char *release = "Release: The worst os you can experience. /··\\\n";
	char *version = "Version: Worst 1.0\n";
	char *machine = "Machine: RISC-V\n";
	char *domainname = "DomainName: Local domain";
	safestrcpy(info.sysname, sysname, strlen(sysname) + 1);
	safestrcpy(info.nodename, nodename, strlen(nodename) + 1);
	safestrcpy(info.release, release, strlen(release) + 1);
	safestrcpy(info.version, version, strlen(version) + 1);
	safestrcpy(info.machine, machine, strlen(machine) + 1);
	safestrcpy(info.domainname, domainname, strlen(domainname) + 1);

	// if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0) {
	if (copyout2(addr, (char *)&info, sizeof(info)) < 0) {
		return -1;
	}

	return 0;
}