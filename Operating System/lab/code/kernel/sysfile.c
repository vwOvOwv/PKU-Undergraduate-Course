//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//


#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/stat.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/sleeplock.h"
#include "include/file.h"
#include "include/pipe.h"
#include "include/fcntl.h"
#include "include/fat32.h"
#include "include/syscall.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/vm.h"


// Fetch the nth word-sized system call argument as a file descriptor
// and return both the descriptor and the corresponding struct file.
int
argfd(int n, int *pfd, struct file **pf)
{
  int fd;
  struct file *f;

  if(argint(n, &fd) < 0)
    return -1;
  if(fd < 0 || fd >= NOFILE || (f=myproc()->ofile[fd]) == NULL)
    return -1;
  if(pfd)
    *pfd = fd;
  if(pf)
    *pf = f;
  return 0;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
int
fdalloc(struct file *f)
{
  int fd;
  struct proc *p = myproc();

  for(fd = 0; fd < NOFILE; fd++){
    if(p->ofile[fd] == 0){
      p->ofile[fd] = f;
      return fd;
    }
  }
  return -1;
}

uint64
sys_dup(void)
{
  struct file *f;
  int fd;

  if(argfd(0, 0, &f) < 0)
    return -1;
  if((fd=fdalloc(f)) < 0)
    return -1;
  filedup(f);
  return fd;
}

uint64
sys_read(void)
{
  struct file *f;
  int n;
  uint64 p;

  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    return -1;
  return fileread(f, p, n);
}

uint64
sys_write(void)
{
  struct file *f;
  int n;
  uint64 p;

  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    return -1;

  return filewrite(f, p, n);
}

uint64
sys_close(void)
{
  int fd;
  struct file *f;

  if(argfd(0, &fd, &f) < 0)
    return -1;
  myproc()->ofile[fd] = 0;
  fileclose(f);
  return 0;
}


struct kstat {
	uint64 st_dev;
        uint64 st_ino;
        unsigned int st_mode;
        uint32 st_nlink;
        uint32 st_uid;
        uint32 st_gid;
        uint64 st_rdev;
        unsigned long __pad;
        long int st_size;
        uint32 st_blksize;
        int __pad2;
        uint64 st_blocks;
        long st_atime_sec;
        long st_atime_nsec;
        long st_mtime_sec;
        long st_mtime_nsec;
        long st_ctime_sec;
        long st_ctime_nsec;
        unsigned __unused[2];
};

uint64
sys_fstat(void)
{
  struct file *f;
  uint64 addr; // user pointer to struct kstat
  struct stat st;
  struct kstat kst = {
    .st_dev = 0,
	  .st_ino = 0,
	  .st_mode = 0,
	  .st_nlink = 0,
	  .st_uid = 0,
	  .st_gid = 0,
	  .st_rdev = 0,
    .__pad = 0,
    .st_size = 0,
    .st_blksize = 0,
    .__pad2 = 0,
	  .st_blocks = 0,
	  .st_atime_sec = 0,
	  .st_atime_nsec = 0,
	  .st_mtime_sec = 0,
	  .st_mtime_nsec = 0,
	  .st_ctime_sec = 0,
	  .st_ctime_nsec = 0,
    .__unused = {0, 0}
  };

  if(argfd(0, 0, &f) < 0 || argaddr(1, &addr) < 0)
    return -1;

  if(f->type == FD_ENTRY){
    elock(f->ep);
    estat(f->ep, &st);
    kst.st_blocks = f->ep->clus_cnt;
    eunlock(f->ep);
    kst.st_dev = st.dev;
    kst.st_size = st.size;
    kst.st_nlink = f->ref;
    kst.st_mode = f->readable | f->writable;

    if(copyout2(addr, (char *)&kst, sizeof(kst)) < 0)
      return -1;
    return 0;
  }
  return -1;
}

static struct dirent*
create(char *path, short type, int mode)
{
  struct dirent *ep, *dp;
  char name[FAT32_MAX_FILENAME + 1];

  if((dp = enameparent(path, name)) == NULL)
    return NULL;

  if (type == T_DIR) {
    mode = ATTR_DIRECTORY;
  } else if (mode & O_RDONLY) {
    mode = ATTR_READ_ONLY;
  } else {
    mode = 0;  
  }

  elock(dp);
  if ((ep = ealloc(dp, name, mode)) == NULL) {
    eunlock(dp);
    eput(dp);
    return NULL;
  }
  
  if ((type == T_DIR && !(ep->attribute & ATTR_DIRECTORY)) ||
      (type == T_FILE && (ep->attribute & ATTR_DIRECTORY))) {
    eunlock(dp);
    eput(ep);
    eput(dp);
    return NULL;
  }

  eunlock(dp);
  eput(dp);

  elock(ep);
  return ep;
}

uint64
sys_open(void) // open和openat逻辑是一样的
{
  // printf("called\n");
  char path[FAT32_MAX_PATH];
  int fd, omode;
  struct file *f;
  struct dirent *ep;

  if(argint(0, &fd), argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0){
    return -1;
  }


  // printf("%d\n%s\n%d\n", fd, path, omode);
  if(omode & O_CREATE){ // 修改O_CREATE的值和测试用例一致
    ep = create(path, T_FILE, omode);
    if(ep == NULL){
      return -1;
    }
  } else {
    if((ep = ename(path)) == NULL){
      // printf("called1\n");
      return -1;
    }
    elock(ep);
    if((ep->attribute & ATTR_DIRECTORY) && omode != O_RDONLY && omode != O_DIRECTORY){
      eunlock(ep);
      eput(ep);
      // printf("called2\n");
      return -1;
    }
  }
  // printf("called\n");
  if((f = filealloc()) == NULL || (fd = fdalloc(f)) < 0){
    if (f) {
      fileclose(f);
    }
    eunlock(ep);
    eput(ep);
    // printf("called3\n");
    return -1;
  }

  if(!(ep->attribute & ATTR_DIRECTORY) && (omode & O_TRUNC)){
    etrunc(ep);
  }

  f->type = FD_ENTRY;
  f->off = (omode & O_APPEND) ? ep->file_size : 0;
  f->ep = ep;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

  eunlock(ep);
  // printf("sys_open end\n");

  return fd;
}

uint64
sys_mkdir(void)
{
  int dirfd, omode;
  char path[FAT32_MAX_PATH];
  struct dirent *ep;

  if(argint(0, &dirfd) < 0 || argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0
      || (ep = create(path, T_DIR, omode)) == 0){
    return -1;
  }
  eunlock(ep);
  eput(ep);
  return 0;
}

uint64
sys_chdir(void)
{
  char path[FAT32_MAX_PATH];
  struct dirent *ep;
  struct proc *p = myproc();
  
  if(argstr(0, path, FAT32_MAX_PATH) < 0 || (ep = ename(path)) == NULL){
    return -1;
  }
  elock(ep);
  if(!(ep->attribute & ATTR_DIRECTORY)){
    eunlock(ep);
    eput(ep);
    return -1;
  }
  eunlock(ep);
  eput(p->cwd);
  p->cwd = ep;
  return 0;
}

uint64
sys_pipe(void)
{
  uint64 fdarray; // user pointer to array of two integers
  struct file *rf, *wf;
  int fd0, fd1;
  struct proc *p = myproc();

  if(argaddr(0, &fdarray) < 0)
    return -1;
  if(pipealloc(&rf, &wf) < 0)
    return -1;
  fd0 = -1;
  if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
    if(fd0 >= 0)
      p->ofile[fd0] = 0;
    fileclose(rf);
    fileclose(wf);
    return -1;
  }
  // if(copyout(p->pagetable, fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
  //    copyout(p->pagetable, fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0){
  if(copyout2(fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
     copyout2(fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0){
    p->ofile[fd0] = 0;
    p->ofile[fd1] = 0;
    fileclose(rf);
    fileclose(wf);
    return -1;
  }
  return 0;
}

// To open console device.
uint64
sys_dev(void)
{
  int fd, omode;
  int major, minor;
  struct file *f;

  if(argint(0, &omode) < 0 || argint(1, &major) < 0 || argint(2, &minor) < 0){
    return -1;
  }

  if(omode & O_CREATE){
    panic("dev file on FAT");
  }

  if(major < 0 || major >= NDEV)
    return -1;

  if((f = filealloc()) == NULL || (fd = fdalloc(f)) < 0){
    if(f)
      fileclose(f);
    return -1;
  }

  f->type = FD_DEVICE;
  f->off = 0;
  f->ep = 0;
  f->major = major;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

  return fd;
}

// To support ls command
uint64
sys_readdir(void)
{
  struct file *f;
  uint64 p;

  if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0)
    return -1;
  return dirnext(f, p);
}

// get absolute cwd string
uint64
sys_getcwd(void)
{
  // printf("called\n");
  struct proc *p;
  uint64 addr;
  int size;

  if (argaddr(0, &addr) < 0 || argint(1, &size))
    return NULL;
    
  struct dirent *de = myproc()->cwd;
  char path[FAT32_MAX_PATH];
  char *s;
  int len;
  if (de->parent == NULL) {
    s = "/";
  } else {
    s = path + FAT32_MAX_PATH - 1;  //从后向前填充
    *s = '\0';
    while (de->parent) {
      len = strlen(de->filename);
      s -= len;
      if (s <= path)          // can't reach root "/"
        return NULL;
      strncpy(s, de->filename, len);
      *--s = '/';
      de = de->parent;
    }
  }

  if (addr == NULL) {
    p = myproc();
    // Ensure there is enough space in the stack
    if (p->trapframe->sp < (strlen(s) + 1))
      return NULL;
    p->trapframe->sp -= strlen(s) + 1;
    addr = p->trapframe->sp;
  }
  else{
    int path_length = strlen(s) + 1;  // 缓冲区空间不足
      if (size < path_length)
        return NULL;
  }
  
  // if (copyout(myproc()->pagetable, addr, s, strlen(s) + 1) < 0)
  if (copyout2(addr, s, strlen(s) + 1) < 0)
    return NULL;
  
  return addr;
}

// Is the directory dp empty except for "." and ".." ?
static int
isdirempty(struct dirent *dp)
{
  struct dirent ep;
  int count;
  int ret;
  ep.valid = 0;
  ret = enext(dp, &ep, 2 * 32, &count);   // skip the "." and ".."
  return ret == -1;
}

uint64
sys_remove(void)
{
  char path[FAT32_MAX_PATH];
  struct dirent *ep;
  int len;
  if((len = argstr(0, path, FAT32_MAX_PATH)) <= 0)
    return -1;

  char *s = path + len - 1;
  while (s >= path && *s == '/') {
    s--;
  }
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    return -1;
  }
  
  if((ep = ename(path)) == NULL){
    return -1;
  }
  elock(ep);
  if((ep->attribute & ATTR_DIRECTORY) && !isdirempty(ep)){
      eunlock(ep);
      eput(ep);
      return -1;
  }
  elock(ep->parent);      // Will this lead to deadlock?
  eremove(ep);
  eunlock(ep->parent);
  eunlock(ep);
  eput(ep);

  return 0;
}

// Must hold too many locks at a time! It's possible to raise a deadlock.
// Because this op takes some steps, we can't promise
uint64
sys_rename(void)
{
  char old[FAT32_MAX_PATH], new[FAT32_MAX_PATH];
  if (argstr(0, old, FAT32_MAX_PATH) < 0 || argstr(1, new, FAT32_MAX_PATH) < 0) {
      return -1;
  }

  struct dirent *src = NULL, *dst = NULL, *pdst = NULL;
  int srclock = 0;
  char *name;
  if ((src = ename(old)) == NULL || (pdst = enameparent(new, old)) == NULL
      || (name = formatname(old)) == NULL) {
    goto fail;          // src doesn't exist || dst parent doesn't exist || illegal new name
  }
  for (struct dirent *ep = pdst; ep != NULL; ep = ep->parent) {
    if (ep == src) {    // In what universe can we move a directory into its child?
      goto fail;
    }
  }

  uint off;
  elock(src);     // must hold child's lock before acquiring parent's, because we do so in other similar cases
  srclock = 1;
  elock(pdst);
  dst = dirlookup(pdst, name, &off);
  if (dst != NULL) {
    eunlock(pdst);
    if (src == dst) {
      goto fail;
    } else if (src->attribute & dst->attribute & ATTR_DIRECTORY) {
      elock(dst);
      if (!isdirempty(dst)) {    // it's ok to overwrite an empty dir
        eunlock(dst);
        goto fail;
      }
      elock(pdst);
    } else {                    // src is not a dir || dst exists and is not an dir
      goto fail;
    }
  }

  if (dst) {
    eremove(dst);
    eunlock(dst);
  }
  memmove(src->filename, name, FAT32_MAX_FILENAME);
  emake(pdst, src, off);
  if (src->parent != pdst) {
    eunlock(pdst);
    elock(src->parent);
  }
  eremove(src);
  eunlock(src->parent);
  struct dirent *psrc = src->parent;  // src must not be root, or it won't pass the for-loop test
  src->parent = edup(pdst);
  src->off = off;
  src->valid = 1;
  eunlock(src);

  eput(psrc);
  if (dst) {
    eput(dst);
  }
  eput(pdst);
  eput(src);

  return 0;

fail:
  if (srclock)
    eunlock(src);
  if (dst)
    eput(dst);
  if (pdst)
    eput(pdst);
  if (src)
    eput(src);
  return -1;
}

uint64
sys_dup3(void)
{
  struct file *f;
  int old_fd, new_fd;

  // printf("called\n");
  if (argfd(0, &old_fd, &f) < 0) // 获取第一个参数old_fd，并将其转换为文件结构指针f
    return -1;
  if (argint(1, &new_fd) < 0) // 获取第二个参数new_fd
    return -1;

  // printf("%d, %d\n", old_fd, new_fd);

  // 检查new_fd是否合法
  if (new_fd < 0 || new_fd > NOFILE)  // NOFILE在param.h中，需要改大一些
    return -1;

  // 如果new_fd已经打开，先关闭它
  if (myproc()->ofile[new_fd]) {
    fileclose(myproc()->ofile[new_fd]);
  }

  myproc()->ofile[new_fd] = f;
  filedup(f);
  // printf("%d, %d\n", old_fd, new_fd);
  return new_fd;
}

uint64
sys_getdents(void)
{
  int fd, len;
  uint64 addr;
  struct file *f;
  int nread = 0;  
  struct dirent de;
  int count = 0;
  int ret;
  struct linux_dirent64 tmp;

  if(argint(0, &fd) < 0 || argint(2, &len) < 0 || argaddr(1, &addr) < 0){
    return -1;
  }
  if(fd < 0 || fd >= NOFILE || (f = myproc()->ofile[fd]) == NULL)
    return -1;
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    return -1;

  while(nread + (int)(sizeof(tmp)) <= len){
    elock(f->ep);
    while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
      f->off += count * 32;
    }
    eunlock(f->ep);
    if (ret == -1)  // meet the end of dir
      return 0;

    f->off += count * 32;
    safestrcpy(tmp.d_name, de.filename, strlen(de.filename) + 1);
    
    if(copyout2(addr, (char *)&tmp, sizeof(tmp)) < 0)
      return -1;

    addr += sizeof(tmp);
    nread += (int)(sizeof(tmp));
  }
  return nread;
}

uint64
sys_unlink(void)
{ 

  char path[FAT32_MAX_PATH];
  struct dirent *ep;
  int fd, flags, len;
  if(argint(0, &fd) < 0 || argint(2, &flags) < 0)
    return -1;
  if((len = argstr(1, path, FAT32_MAX_PATH)) <= 0)
    return -1;
  
  char *s = path + len - 1;
  while (s >= path && *s == '/') {
    s--;
  }
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    return -1;
  }
  
  if((ep = ename(path)) == NULL){
    return -1;
  }
  elock(ep);
  if((ep->attribute & ATTR_DIRECTORY) && !isdirempty(ep)){
      eunlock(ep);
      eput(ep);
      return -1;
  }
  elock(ep->parent);
  eremove(ep);
  eunlock(ep->parent);
  eunlock(ep);
  eput(ep);
  return 0;
}

uint64
sys_mount(void){

	return 0;
}

uint64
sys_umount(void){
	return 0;
}