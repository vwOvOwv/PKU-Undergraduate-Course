  # 代码段.
  .text
  # `main` 函数, 程序的入口.
  .globl main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
  sw    s0, 8(sp)
  sw    s1, 4(sp)
  la    s0, hello_str
  li    s1, 0
1:
  add   a0, s0, s1
  lbu   a0, 0(a0)
  beqz  a0, 1f
  call  putch
  addi  s1, s1, 1
  j     1b
1:
  li    a0, 0
  lw    s1, 4(sp)
  lw    s0, 8(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

  # 数据段.
  .data
  # 字符串 "Hello, world!\n\0".
hello_str:
  .asciz "Hello, world!\n"
