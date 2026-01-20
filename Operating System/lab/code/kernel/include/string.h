#ifndef __STRING_H
#define __STRING_H

#include "types.h"

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
char*           strchr(const char *s, char c);

#endif