#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/sysinfo.h"
#include "xv6-user/user.h"

int main()
{
    struct sysinfo info;
    if (sysinfo(&info) < 0) {
        printf("sysinfo fail!\n");
    } else {
        printf("memory left: %d KB\n", info.freemem >> 10);
        printf("process amount: %d\n", info.nproc);
    }
    exit(0);
}
