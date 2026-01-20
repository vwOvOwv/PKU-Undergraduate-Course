#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/fcntl.h"
#include "kernel/include/param.h"
#include "xv6-user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "Usage: mv old_name new_name\n");
        exit(1);
    }

    char src[MAXPATH];
    char dst[MAXPATH];
    strcpy(src, argv[1]);
    strcpy(dst, argv[2]);
    int fd = open(dst, O_RDONLY);
    if (fd >= 0) {
        struct stat st;
        fstat(fd, &st);
        close(fd);
        if (st.type == T_DIR) {
            char *ps, *pd;
            for (ps = src + strlen(src) - 1; ps >= src; ps--) { // trim '/' in tail
                if (*ps != '/') {
                    *(ps + 1) = '\0';
                    break;
                }
            }
            for (; ps >= src && *ps != '/'; ps--);
            ps++;
            pd = dst + strlen(dst);
            *pd++ = '/';
            while (*ps) {
                *pd++ = *ps++;
                if (pd >= dst + MAXPATH) {
                    fprintf(2, "mv: fail! final dst path too long (exceed MAX=%d)!\n", MAXPATH);
                    exit(-1);
                }
            }
        } else {
            fprintf(2, "mv: fail! %s exists!\n", dst);
            exit(-1);
        }
    }
    printf("moving [%s] to [%s]\n", src, dst);
    if (rename(src, dst) < 0) {
        fprintf(2, "mv: fail to rename %s to %s!\n", src, dst);
        exit(-1);
    }
    exit(0);
}