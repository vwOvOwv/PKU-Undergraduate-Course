
xv6-user/initcode.o:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <start>:
#include "include/sysnum.h"

# exec(init, argv)
.globl start
start:
        la a0, init
   0:	00000517          	auipc	a0,0x0
   4:	00053503          	ld	a0,0(a0) # 0 <start>
        la a1, argv
   8:	00000597          	auipc	a1,0x0
   c:	0005b583          	ld	a1,0(a1) # 8 <start+0x8>
        li a7, SYS_exec
  10:	0dd00893          	li	a7,221
        ecall
  14:	00000073          	ecall

0000000000000018 <exit>:

# for(;;) exit();
exit:
        li a7, SYS_exit
  18:	05d00893          	li	a7,93
        ecall
  1c:	00000073          	ecall
        jal exit
  20:	ff9ff0ef          	jal	ra,18 <exit>

0000000000000024 <init>:
  24:	696e692f          	.word	0x696e692f
  28:	           	Address 0x0000000000000028 is out of bounds.


000000000000002b <argv>:
	...
