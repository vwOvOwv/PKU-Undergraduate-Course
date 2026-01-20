# K210 多核启动移植记录
2020/10/18，将 xv6 移植到 k210 的进展：成功启动了 k210 的双核。  

## xv6 在 qemu 上的启动过程
xv6 在 qemu 里面是没有使用 SBI 实现来启动的，这种情况下它的内核被放在地址 0x80000000 处，qemu 将内核加载后直接执行内核的第一行指令。  
这个第一行指令是在 kernel 目录下的汇编文件 entry.S 里面：  
```
	# qemu -kernel loads the kernel at 0x80000000
        # and causes each CPU to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
_entry:
	# set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        la sp, stack0
        li a0, 1024*4
	csrr a1, mhartid
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
	# jump to start() in start.c
        call start
spin:
        j spin

```
在这个汇编文件里面内核会做一些准备工作，这里是会分配内核栈空间，每个核分配 4096 byte 的空间，然后跳转到 start.c 文件里面的 start() 函数中运行。  
在 start（） 函数中，xv6 内核还是运行在 M 态，做一些 M 态级别的准备工作，比如设置中断代理，M 态中断使能和入口，还有时钟中断的初始化。  
这些准备工作完成之后，xv6 内核就切换到了 S 态去运行，并跳转到 main.c 文件中的 main 函数中执行。  
在 main 函数中会进行内核中各个模块比如内存管理，中断，进程管理的初始化。  
在 xv6 中，会先启动第 0 个核，在第 0 个核中初始化完成后，再去启动其他核，最后会进入多核调度过程。  

## SBI 实现提供的 Bootloader 功能
现在我们想要使用 SBI 实现的 Bootloader 功能让内核跑在 k210 板子上，SBI 运行在 M 态，我们的 OS 内核运行在 S 态，因此我们的内核不能执行 M 态特权指令。
也就是说，我们的内核启动方式必定和在 qemu 上启动过程不一样。  
原 xv6 代码里面的 M 态指令实现的功能，都由 SBI 层为我们实现。  

## 在 k210 板子上用 SBI 实现启动双核
k210 有两个核，我们尝试来启动 k210 的两个 CPU 核。  
首先我们需要写一个链接脚本，指定内核的入口点：  
```linker
OUTPUT_ARCH(riscv)
ENTRY(_start)

BASE_ADDRESS = 0xffffffff80020000;

SECTIONS
{
    /* Load the kernel at this address: "." means the current address */
    . = BASE_ADDRESS;

    kernel_start = .;

    . = ALIGN(4K);

    text_start = .;
    .text : {
        *(.text .text.*)
        . = ALIGN(0x1000);
        _trampoline = .;
        *(trampsec)
        . = ALIGN(0x1000);
        ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page");
        PROVIDE(etext = .);
    }

    . = ALIGN(4K);
    rodata_start = .;
    .rodata : {
        srodata = .;
        *(.rodata .rodata.*)
        erodata = .;
    }

    . = ALIGN(4K);
    data_start = .;
    .data : {
        sdata = .;
        *(.data .data.*)
        edata = .;
    }

    . = ALIGN(4K);
    bss_start = .;
    .bss : {
	    *(.bss.stack)
        sbss_clear = .;
        *(.sbss .bss .bss.*)
        ebss_clear = .;
    }

    . = ALIGN(4K);
    PROVIDE(kernel_end = .);
}
```
这个链接脚本指定了入口点为 _start，并将链接目标的起始地址设置为了 0xffffffff80020000，我们编译出的内核镜像的入口点 _start 就是放在了该地址处。  
这个入口点我们在汇编文件 entry_k210.S 里定义：  

```
    .section .text.entry
    .globl _start
_start:
    add t0, a0, 1
    slli t0, t0, 14
    lui sp, %hi(boot_stack)
    add sp, sp, t0

    lui t0, %hi(main)
    addi t0, t0, %lo(main)
    jr t0

    .section .bss.stack
    .align 12
    .globl boot_stack
boot_stack:
    .space 4096 * 4 * 2
    .globl boot_stack_top
boot_stack_top:
```

这个汇编文件里面做的工作和之前提到的 entry.S 里面做的工作大同小异，都是分配内核栈空间，然后跳转。  
这里跳转的目标地址就和 xv6 在 qemu 上的情况不一样了，在 qemu 里面的情况是 xv6 代码跳转到 start.c 里面的 start() 代码里面运行 M 态代码，做一下 M 态的准备工作。  
而我们之前提到，如果加入了 SBI 层的话，我们的内核代码就是完全跑在 S 态的，无法运行 M 态指令，M 态下的准备工作 SBI 实现都给我们完成了。  
因此我们这里直接跳转到 main.c 文件里面的 main() 函数。在这个函数里面我们会完成一些模块的初始化工作，比如内存管理，中断，进程管理。  
关于在 k210 上的多核启动，我在经过和 RustSBI 的作者商讨之后决定以这样的方式启动多核：  
首先我们启动第 0 个核，在这个核上初始化完成之后我们给其他每个核发一个 ipi 来唤醒其他的核，这里用到了 SBI 接口：  
```C
for(int i = 1; i < NCPU; i++) {
    unsigned long mask = 1 << i;
    sbi_send_ipi(&mask);
```

整个 main 函数代码如下：  
```C
// start() jumps here in supervisor mode on all CPUs.
void
main(unsigned long hartid, unsigned long dtb_pa)
{
  
  printf("hart %d enter main()...\n", hartid);
  // If it's the 0st hart
  if (hartid == 0) {
    printf("\n");
    printf("xv6-k210 kernel is booting\n");
    printf("\n");

    ... // Init Code

    for(int i = 1; i < NCPU; i++) {
    unsigned long mask = 1 << i;
    sbi_send_ipi(&mask);
    }
  }
  while(1);        
}
```
运行后可以看到终端打印出两行字符串：  
```
hart 0 enter main()...
hart 1 enter main()...
```
这样子就成功地启动 k210 上的双核了！  

## 如果本文档有错，请在该项目上开 issue 提出或 提交 pr 或发到作者邮箱，谢谢  
作者邮箱:linuxgnulover@gmail.com  

