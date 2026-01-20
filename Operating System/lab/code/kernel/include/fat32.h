#ifndef __FAT32_H
#define __FAT32_H

#include "sleeplock.h"
#include "stat.h"

#define ATTR_READ_ONLY      0x01
#define ATTR_HIDDEN         0x02
#define ATTR_SYSTEM         0x04
#define ATTR_VOLUME_ID      0x08
#define ATTR_DIRECTORY      0x10
#define ATTR_ARCHIVE        0x20
#define ATTR_LONG_NAME      0x0F

#define LAST_LONG_ENTRY     0x40
#define FAT32_EOC           0x0ffffff8
#define EMPTY_ENTRY         0xe5
#define END_OF_ENTRY        0x00
#define CHAR_LONG_NAME      13
#define CHAR_SHORT_NAME     11

#define FAT32_MAX_FILENAME  255
#define FAT32_MAX_PATH      260
#define ENTRY_CACHE_NUM     50

struct linux_dirent64 {
    uint64        d_ino;
    uint64         d_off;
    unsigned short  d_reclen;
    unsigned char   d_type;
    char            d_name[];
};

struct dirent {
    char  filename[FAT32_MAX_FILENAME + 1];
    uint8   attribute;
    // uint8   create_time_tenth;
    // uint16  create_time;
    // uint16  create_date;
    // uint16  last_access_date;
    uint32  first_clus;
    // uint16  last_write_time;
    // uint16  last_write_date;
    uint32  file_size;

    uint32  cur_clus;
    uint    clus_cnt;

    /* for OS */
    uint8   dev;
    uint8   dirty;
    short   valid;
    int     ref;
    uint32  off;            // offset in the parent dir entry, for writing convenience
    struct dirent *parent;  // because FAT32 doesn't have such thing like inum, use this for cache trick
    struct dirent *next;
    struct dirent *prev;
    struct sleeplock    lock;
};

int             fat32_init(void);
struct dirent*  dirlookup(struct dirent *entry, char *filename, uint *poff);
char*           formatname(char *name);
void            emake(struct dirent *dp, struct dirent *ep, uint off);
struct dirent*  ealloc(struct dirent *dp, char *name, int attr);
struct dirent*  edup(struct dirent *entry);
void            eupdate(struct dirent *entry);
void            etrunc(struct dirent *entry);
void            eremove(struct dirent *entry);
void            eput(struct dirent *entry);
void            estat(struct dirent *ep, struct stat *st);
void            elock(struct dirent *entry);
void            eunlock(struct dirent *entry);
int             enext(struct dirent *dp, struct dirent *ep, uint off, int *count);
struct dirent*  ename(char *path);
struct dirent*  enameparent(char *path, char *name);
int             eread(struct dirent *entry, int user_dst, uint64 dst, uint off, uint n);
int             ewrite(struct dirent *entry, int user_src, uint64 src, uint off, uint n);

#endif