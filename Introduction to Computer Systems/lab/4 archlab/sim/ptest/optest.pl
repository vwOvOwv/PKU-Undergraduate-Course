#!/usr/bin/perl 
#!/usr/local/bin/perl 
# Test single instructions in pipeline

use Getopt::Std;
use lib ".";
require tester;

cmdline();

@vals = (0x100, 0x020, 0x004);

@instr = ("rrmovq", "addq", "subq", "andq", "xorq");
@regs = ("rdx", "rbx", "rsp");

foreach $t (@instr) {
    foreach $ra (@regs) {
	foreach $rb (@regs) {
	    $tname = "op-$t-$ra-$rb";
	    open (YFILE, ">$tname.ys") || die "Can't write to $tname.ys\n";
	    print YFILE <<STUFF;
	      irmovq \$$vals[0], %$ra
	      irmovq \$$vals[1], %$rb
	      nop
	      nop
	      nop
	      $t %$ra,%$rb
	      nop
	      nop
	      halt
STUFF
	    close YFILE;
	    run_test($tname);
	}
    }
}

if ($testiaddq) {
    foreach $ra (@regs) {
	foreach $val (@vals) {
	    $tname = "op-iaddq-$val-$ra";
	    open (YFILE, ">$tname.ys") || die "Can't write to $tname.ys\n";
	    print YFILE <<STUFF;
	    irmovq \$$val, %$ra
	    nop
	    nop
	    nop
            iaddq \$-32, %$ra
	    nop
	    nop
	    halt
STUFF
            close YFILE;
	    run_test($tname);
	}
    }
}

@instr = ("pushq", "popq");
@regs = ("rdx", "rsp");

foreach $t (@instr) {
    foreach $ra (@regs) {
	$tname = "op-$t-$ra";
	open (YFILE, ">$tname.ys") || die "Can't write to $tname.ys\n";
	print YFILE <<STUFF;
    irmovq \$0x200,%rsp
		
	irmovq \$$vals[1], %rax
	nop
	nop
        nop
        rmmovq %rax, 0(%rsp)
	irmovq \$$vals[2], %rax
	nop
        nop
        nop
	rmmovq %rax, -4(%rsp)
	irmovq \$$vals[0], %rdx
	nop
        nop
        nop
	$t %$ra
	nop
	nop
        halt
STUFF
	close YFILE;
	&run_test($tname);
    }
}

#new instr jm -zxp
if($testjm) {
	$tname = "op-jm";
	open(YFILE, ">$tname.ys") || die "Can't write to $tname.ys\n";
	print YFILE <<STUFF;
	irmovq \$0x1ff, %rax #这是jm指令的rB
	irmovq \$0x200, %rcx #这是jm要读取的内存的地址，这个地址处存放跳转地址
	irmovq \$0x15, %rbx  #不重要
	irmovq \$0x2a, %rbp  #也不重要
	irmovq \$0x400, %rdx #这是从内存中读取到的跳转地址
	rmmovq %rdx, 0(%rcx) #将跳转地址0x400写到内存
	irmovq \$0x60351010, %rsi
	rmmovq %rsi, 0(%rdx) #指令add %rbx, %rbp ; nop ;nop;
	irmovq \$0x10603510, %rsi
	rmmovq %rsi, 8(%rdx) #指令nop ; add %rbx, %rbp; nop;
	irmovq \$0x10100000, %rsi
	rmmovq %rsi, 16(%rbx) #指令nop ; nop ; halt ;
	nop
	nop
	nop	
	jm \$0x1 ,%rax #0x1ff + 0x1 = 0x200 内存地址
	halt
STUFF
	close TFILE
	&run_test($tname);
}

&test_stat();
