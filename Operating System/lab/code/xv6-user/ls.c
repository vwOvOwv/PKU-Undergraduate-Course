#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "xv6-user/user.h"

char*
fmtname(char *name)
{
  static char buf[STAT_MAX_NAME+1];
  int len = strlen(name);

  // Return blank-padded name.
  if(len >= STAT_MAX_NAME)
    return name;
  memmove(buf, name, len);
  memset(buf + len, ' ', STAT_MAX_NAME - len);
  buf[STAT_MAX_NAME] = '\0';
  return buf;
}

void
ls(char *path)
{
  int fd;
  struct stat st;
  char *types[] = {
    [T_DIR]   "DIR ",
    [T_FILE]  "FILE",
  };

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR){
    while(readdir(fd, &st) == 1){
      printf("%s %s\t%d\n", fmtname(st.name), types[st.type], st.size);
    }
  } else {
    printf("%s %s\t%l\n", fmtname(st.name), types[st.type], st.size);
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
