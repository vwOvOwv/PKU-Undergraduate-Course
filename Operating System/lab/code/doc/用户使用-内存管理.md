# 页表相关代码

## 创建地址空间
大多数用于操纵地址空间和页表的xv6代码位于vm.c[(kernel/vm.c:1)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L1)中。中心数据结构是pagetable_t ，它实际上是一个指向RISC-V根页表页的指针；一个pagetable_t可以是内核页表，也可以是一个进程页表。主要功能是：walk（查找虚拟地址对应的页表项（PTE））和mappages （将映射关系填入页表中）。以kvm开头的函数用于操纵内核页表；以uvm开头的函数用于操纵用户页表；两者都使用其他功能。copyout和copyin将数据从用户虚拟地址复制，或复制到用户虚拟地址，这里的用户虚拟地址是作为系统调用参数提供的；它们位于vm.c中，因为它们需要显式转换这些地址才能找到相应的物理内存。

在刚进入启动阶段时，main调用kvminit[(kernel/vm.c:22)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L22)以创建内核的页表。该调用发生在xv6在RISC-V上启用分页之前，因此地址直接引用物理内存。kvminit首先分配一页物理内存来保存根页表页面。然后，它调用kvmmap来将转换关系填入页表中。这些转换包括内核的指令和数据、高达PHYSTOP的物理内存、以及实际上是设备的内存范围。

kvmmap （kernel / vm.c：118）调用mappages[(kernel/vm.c:149)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L22)，这会将映射关系填写到页表中，以将一系列虚拟地址映射到相应的物理地址范围。它以页面间隔的距离对范围内的每个虚拟地址分别执行此操作。对于要映射的每个虚拟地址，mappages调用walk来查找该地址的PTE地址。然后初始化PTE来保存相关的物理页号（PPN）、所需的权限（PTE\_W ，PTE\_X和/或PTE\_R ），并初始化PTE\_V以将PTE标记为有效（置1）[(kernel/vm.c:161)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L161)。

walk[(kernel/vm.c:72)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L72)模拟RISC-V分页硬件，因为它在PTE中查找虚拟地址。它使用每个级别的9位虚拟地址来查找下一级页表或最终页面[(kernel/vm.c:78)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L78)的PTE 。如果PTE无效，则尚未分配所需的页面。如果设置了alloc参数，walk将分配一个新的页表页并将其物理地址写入PTE中。它返回树中最底层（叶子节点）中的PTE地址[(kernel/vm.c:88)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L88)。

上面的代码取决于将物理内存直接映射到内核虚拟地址空间中。例如，当walk下降页表的级别时，它将从PTE[(kernel/vm.c:80)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L80)中提取下一级页表的（物理）地址，然后将该地址作为虚拟地址以便在提取下一级[(kernel/vm.c:78)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L78)PTE 。

main调用kvminithart[(kernel/vm.c:53)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L53)填写内核页表的映射关系。它将根页表页的物理地址写入寄存器satp 。此后，CPU将使用内核页表转换地址。由于内核使用直接映射，因此下一条指令的当前虚拟地址将映射到正确的物理内存地址。

procinit[(kernel/proc.c:26)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/proc.c#L26)被main调用，为每个进程分配一个内核堆栈。它将每个堆栈映射到KSTACK生成的虚拟地址，从而为无效的堆栈保护页留出空间。kvmmap将实现映射关系的页表项（PTE）添加到内核页表中，调用kvminithart时将内核页表重新加载到satp中，以便硬件得知新的PTE。

每个RISC-V CPU都将页表项缓存在转换后备缓冲区（TLB）中，并且当xv6更改页表时，它必须告诉CPU使相应的缓存的TLB条目无效。如果没有，那么在某个时候TLB可能会使用旧的缓存映射，指向同时已分配给另一个进程的物理页面，结果导致某个进程可能会在某些页面上乱写其他进程的内存。RISC-V具有一条指令sfence.vma ，该指令刷新当前CPU的TLB。SATP寄存器重新加载后，XV6在kvminithart中执行sfence.vma，并在返回用户空间之前切换到用户页表的trampoline代码中[(kernel/trampoline.S:79)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/trampoline.S#L79) 。

## 物理内存分配器
分配器代码详见在kalloc.c[(kernel/kalloc.c：1)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/kalloc.c#L1)。分配器的数据结构是一个空闲链表，这个链表保存了可用于分配的物理内存页。每个空闲页面的list元素都是一个结构体run[(kernel/kalloc.c：17)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/kalloc.c#L17)。那么问题来了——分配器从哪里获取内存来保存该数据结构？它将每个自由页面的结构体run存储在自由页面本身中，因为那里没有其他存储。空闲链表受自旋锁（spin lock）[(kernel/kalloc.c：21-24)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/kalloc.c#L21-L24)保护。列表和锁包装在一个结构体中，以说明锁保护结构体中的字段。现在可以忽略锁以及关于锁的获取和释放的调用。

函数main调用kinit初始化分配器[(kernel/kalloc.c：27)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/kalloc.c#L27)。kinit初始化空闲链表，以容纳内核末尾与PHYSTOP之间的每个页面。xv6应该通过解析硬件提供的配置信息来确定有多少物理内存可用。相反，xv6假定计算机具有128MB的RAM。kinit调用freerange通过对kfree的每页调用将内存添加到空闲列表。PTE只能引用在4096字节边界上对齐的物理地址（是4096的倍数），因此freerange使用PGROUNDUP来确保它仅释放对齐的物理地址。分配器开始时没有内存。这些对kfree的调用使它有所管理。

分配器有时将地址视为整数以便对其执行算术运算（比如遍历freerange中的所有页时），有时使用地址作为读写内存的指针（比如操纵存储在每个页中的运行结构时）；地址的这种双重使用是分配器代码充满C语言中强制类型转换的主要原因。另一个原因是释放和分配固有地改变了内存的类型。

函数kfree[(kernel/kalloc.c：47)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/kalloc.c#L47)首先将内存中释放的每个字节设置为值1。这将导致释放内存（dangling use）之后使用内存的代码读取垃圾，而不是旧的有效内容。希望这将导致此类代码更快地破解。然后kfree将页面添加到空闲列表中：将物理地址（PA）强制转换为指向run结构体的指针，在r->next中记录空闲列表的原来开始的地方，并将空闲列表设置为r 。kalloc删除并返回空闲列表中的第一个元素。

## sbrk
sbrk是一个系统调用，用于进程缩小或增加其内存。系统调用由函数growproc[(kernel/proc.c:239)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/proc.c#L239)实现。growproc调用uvmalloc或uvmdealloc，这取决于n是正的还是负的。uvmalloc[(kernel/vm.c:229)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L229)使用kalloc分配物理内存，并使用mappages将PTE添加到用户页表中 。uvmdealloc调用uvmunmap （kernel / vm.c：174），它使用walk来查找PTE ，然后使用kfree释放它们引用的物理内存。

xv6通过使用进程的页表不仅告诉硬件如何映射用户虚拟地址，而且还是向该进程分配了哪些物理内存页的唯一记录。这就是释放用户内存（在uvmunmap中）需要检查用户页表的原因。

## exec
exec是创建地址空间的用户部分的系统调用。它用存储在文件系统中的文件初始化地址空间的用户部分。exec[(kernel/exec.c:13)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L13)使用namei[(kernel/exec.c:26)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L26)打开指定的二进制路径。然后，它读取ELF格式的头部（header）。xv6应用程序以广泛使用的ELF格式描述，在[(kernel/elf.h)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/elf.h)中定义。一个ELF二进制由一个ELF头部、结构体elfhdr[(kernel/elf.h:6)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/elf.h#L6)、一系列程序节头和结构体proghdr[(kernel/elf.h:25)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/elf.h#L25)组成。每个程序都描述了必须加载到内存中的应用程序的一个部分。xv6程序只有一个程序节头，但是其他系统可能有单独的节用于存储指令和数据。

第一步是快速检查文件是否可能包含ELF二进制文件。ELF二进制文件以四字节的魔数0x7F，字符'E'，字符'L'，字符'F'或ELF\_MAGIC[(kernel/elf.h:3)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/elf.h#L3)开头。如果ELF头部具有正确的魔数，则exec认为二进制文件格式正确。

exec调用proc\_pagetable[(kernel/exec.c:38)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L38)分配一个没有用户映射的新页表，调用uvmalloc[(kernel/exec.c:52)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L52)为每个ELF段分配内存，并用loadseg[(kernel/exec.c:10)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L10)将每个段加载到内存中。loadseg调用walkaddr查找分配内存的物理地址，在该地址上写入ELF段的每一页，调用readi从文件中读。

程序段头文件的filez可能小于memsz ，这表明它们之间的间隙应该用零填充（对于C全局变量），而不是从文件中读取。
对于/init ，filesz为2112字节，而memsz为2136字节，因此uvmalloc分配了足够的物理内存来容纳2136字节，但仅从file/init读取2112字节。

现在exec分配并初始化用户堆栈。它仅分配一个堆栈页面。exec一次将一个字符串参数复制到堆栈的顶部，并在ustack中记录指向它们的指针。它将空指针放在传递给main的argv列表的末尾。ustack中的前三个条目是伪造的返回程序计数器，argc和argv指针。

exec在堆栈页面的下方放置了一个无法访问的页面，因此尝试使用多个页面的程序将出错。这个无法访问的页面还允许exec处理过大的参数。在这种情况下，exec调用copyout[(kernel/vm.c:355)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/vm.c#L355)函数以将参数复制到堆栈时会发现目标页面不可访问，从而返回-1。

在准备新内存映像的过程中，如果exec检测到诸如无效程序段之类的错误，它将跳转到bad标签处，释放新映像，并返回-1。exec必须等待释放旧映像，直到确定系统调用成功为止：如果旧映像不存在，则系统调用无法向其返回-1。exec只有在创建映像时才有可能会发生错误。映像完成后，exec将开始使用新页表[(kernel/exec.c:113)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L113)并释放旧页表[(kernel/exec.c:117)](https://github.com/mit-pdos/xv6-riscv/blob/riscv//kernel/exec.c#L117)。

exec将ELF文件中的字节加载到ELF文件指定地址处的内存中。用户或进程可以将所需的任何地址放入ELF文件中。因此exec是有风险的，因为ELF文件中的地址可能会偶然或有目的地引用内核。粗心的内核可能导致崩溃，甚至会恶意破坏内核的隔离机制（即，安全漏洞攻击）。xv6执行了许多检查以避免这些风险。例如，if（ph.vaddr + ph.memsz < ph.vaddr ）检查总和是否溢出64位整数。危险在于，用户可能用ph.vaddr构造一个ELF二进制文件，该ph.vaddr指向用户选择的地址，而ph.memsz足够大，使得总和溢出到0x1000，这看起来像是一个有效值。在旧版本的xv6中，用户地址空间也包含内核（但在用户模式下不可读/可写），用户可以选择与内核内存相对应的地址，从而将数据从ELF二进制文件复制到内核中。在xv6的RISC-V版本中，不会发生这种情况，因为内核具有自己的单独的页表。loadeg加载到进程的页表中，而不是内核的页表中。


