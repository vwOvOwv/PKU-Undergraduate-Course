#ifndef __PRINTF_H
#define __PRINTF_H

void printfinit(void);

void printf(char *fmt, ...);

void panic(char *s) __attribute__((noreturn));

void backtrace();

void print_logo();

#endif 
