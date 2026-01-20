    .section .text.entry
    .globl _start
_start:
    add t0, a0, 1
    slli t0, t0, 14
    // lui sp, %hi(boot_stack)
	la sp, boot_stack
    add sp, sp, t0

    // add by retrhelo, write tp reg 
    // csrr t1, mhartid 
    // mv tp, t1

    // lui t0, %hi(main)
    // addi t0, t0, %lo(main)
    // jr t0
    call main

loop:
    j loop

    .section .bss.stack
    .align 12
    .globl boot_stack
boot_stack:
    .space 4096 * 4 * 2
    .globl boot_stack_top
boot_stack_top:
