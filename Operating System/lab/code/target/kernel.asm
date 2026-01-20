
target/kernel:     file format elf64-littleriscv


Disassembly of section .text:

0000000080200000 <_entry>:
    80200000:	00150293          	addi	t0,a0,1
    80200004:	02ba                	slli	t0,t0,0xe
    80200006:	00033117          	auipc	sp,0x33
    8020000a:	32213103          	ld	sp,802(sp) # 80233328 <_GLOBAL_OFFSET_TABLE_+0x50>
    8020000e:	9116                	add	sp,sp,t0
    80200010:	203000ef          	jal	ra,80200a12 <main>

0000000080200014 <loop>:
    80200014:	a001                	j	80200014 <loop>

0000000080200016 <printint>:
    }
}

static void
printint(int xx, int base, int sign)
{
    80200016:	7179                	addi	sp,sp,-48
    80200018:	f406                	sd	ra,40(sp)
    8020001a:	f022                	sd	s0,32(sp)
    8020001c:	ec26                	sd	s1,24(sp)
    8020001e:	e84a                	sd	s2,16(sp)
    80200020:	1800                	addi	s0,sp,48
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    80200022:	c219                	beqz	a2,80200028 <printint+0x12>
    80200024:	08054663          	bltz	a0,802000b0 <printint+0x9a>
    x = -xx;
  else
    x = xx;
    80200028:	2501                	sext.w	a0,a0
    8020002a:	4881                	li	a7,0

  i = 0;
    8020002c:	fd040913          	addi	s2,s0,-48
    x = xx;
    80200030:	86ca                	mv	a3,s2
  i = 0;
    80200032:	4701                	li	a4,0
  do {
    buf[i++] = digits[x % base];
    80200034:	2581                	sext.w	a1,a1
    80200036:	00009617          	auipc	a2,0x9
    8020003a:	34a60613          	addi	a2,a2,842 # 80209380 <digits>
    8020003e:	883a                	mv	a6,a4
    80200040:	2705                	addiw	a4,a4,1
    80200042:	02b577bb          	remuw	a5,a0,a1
    80200046:	1782                	slli	a5,a5,0x20
    80200048:	9381                	srli	a5,a5,0x20
    8020004a:	97b2                	add	a5,a5,a2
    8020004c:	0007c783          	lbu	a5,0(a5)
    80200050:	00f68023          	sb	a5,0(a3)
  } while((x /= base) != 0);
    80200054:	0005079b          	sext.w	a5,a0
    80200058:	02b5553b          	divuw	a0,a0,a1
    8020005c:	0685                	addi	a3,a3,1
    8020005e:	feb7f0e3          	bgeu	a5,a1,8020003e <printint+0x28>

  if(sign)
    80200062:	00088c63          	beqz	a7,8020007a <printint+0x64>
    buf[i++] = '-';
    80200066:	fe070793          	addi	a5,a4,-32
    8020006a:	00878733          	add	a4,a5,s0
    8020006e:	02d00793          	li	a5,45
    80200072:	fef70823          	sb	a5,-16(a4)
    80200076:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
    8020007a:	02e05563          	blez	a4,802000a4 <printint+0x8e>
    8020007e:	fd040493          	addi	s1,s0,-48
    80200082:	94ba                	add	s1,s1,a4
    80200084:	197d                	addi	s2,s2,-1
    80200086:	993a                	add	s2,s2,a4
    80200088:	377d                	addiw	a4,a4,-1
    8020008a:	1702                	slli	a4,a4,0x20
    8020008c:	9301                	srli	a4,a4,0x20
    8020008e:	40e90933          	sub	s2,s2,a4
    consputc(buf[i]);
    80200092:	fff4c503          	lbu	a0,-1(s1)
    80200096:	00007097          	auipc	ra,0x7
    8020009a:	628080e7          	jalr	1576(ra) # 802076be <consputc>
  while(--i >= 0)
    8020009e:	14fd                	addi	s1,s1,-1
    802000a0:	ff2499e3          	bne	s1,s2,80200092 <printint+0x7c>
}
    802000a4:	70a2                	ld	ra,40(sp)
    802000a6:	7402                	ld	s0,32(sp)
    802000a8:	64e2                	ld	s1,24(sp)
    802000aa:	6942                	ld	s2,16(sp)
    802000ac:	6145                	addi	sp,sp,48
    802000ae:	8082                	ret
    x = -xx;
    802000b0:	40a0053b          	negw	a0,a0
  if(sign && (sign = xx < 0))
    802000b4:	4885                	li	a7,1
    x = -xx;
    802000b6:	bf9d                	j	8020002c <printint+0x16>

00000000802000b8 <printstring>:
void printstring(const char* s) {
    802000b8:	1101                	addi	sp,sp,-32
    802000ba:	ec06                	sd	ra,24(sp)
    802000bc:	e822                	sd	s0,16(sp)
    802000be:	e426                	sd	s1,8(sp)
    802000c0:	1000                	addi	s0,sp,32
    802000c2:	84aa                	mv	s1,a0
    while (*s)
    802000c4:	00054503          	lbu	a0,0(a0)
    802000c8:	c909                	beqz	a0,802000da <printstring+0x22>
        consputc(*s++);
    802000ca:	0485                	addi	s1,s1,1
    802000cc:	00007097          	auipc	ra,0x7
    802000d0:	5f2080e7          	jalr	1522(ra) # 802076be <consputc>
    while (*s)
    802000d4:	0004c503          	lbu	a0,0(s1)
    802000d8:	f96d                	bnez	a0,802000ca <printstring+0x12>
}
    802000da:	60e2                	ld	ra,24(sp)
    802000dc:	6442                	ld	s0,16(sp)
    802000de:	64a2                	ld	s1,8(sp)
    802000e0:	6105                	addi	sp,sp,32
    802000e2:	8082                	ret

00000000802000e4 <backtrace>:
  for(;;)
    ;
}

void backtrace()
{
    802000e4:	7179                	addi	sp,sp,-48
    802000e6:	f406                	sd	ra,40(sp)
    802000e8:	f022                	sd	s0,32(sp)
    802000ea:	ec26                	sd	s1,24(sp)
    802000ec:	e84a                	sd	s2,16(sp)
    802000ee:	e44e                	sd	s3,8(sp)
    802000f0:	1800                	addi	s0,sp,48

static inline uint64
r_fp()
{
  uint64 x;
  asm volatile("mv %0, s0" : "=r" (x) );
    802000f2:	8922                	mv	s2,s0
  uint64 *fp = (uint64 *)r_fp();
    802000f4:	84ca                	mv	s1,s2
  uint64 *bottom = (uint64 *)PGROUNDUP((uint64)fp);
    802000f6:	6785                	lui	a5,0x1
    802000f8:	17fd                	addi	a5,a5,-1 # fff <_entry-0x801ff001>
    802000fa:	993e                	add	s2,s2,a5
    802000fc:	77fd                	lui	a5,0xfffff
    802000fe:	00f97933          	and	s2,s2,a5
  printf("backtrace:\n");
    80200102:	00009517          	auipc	a0,0x9
    80200106:	efe50513          	addi	a0,a0,-258 # 80209000 <etext>
    8020010a:	00000097          	auipc	ra,0x0
    8020010e:	084080e7          	jalr	132(ra) # 8020018e <printf>
  while (fp < bottom) {
    80200112:	0324f263          	bgeu	s1,s2,80200136 <backtrace+0x52>
    uint64 ra = *(fp - 1);
    printf("%p\n", ra - 4);
    80200116:	00009997          	auipc	s3,0x9
    8020011a:	53a98993          	addi	s3,s3,1338 # 80209650 <digits+0x2d0>
    8020011e:	ff84b583          	ld	a1,-8(s1)
    80200122:	15f1                	addi	a1,a1,-4
    80200124:	854e                	mv	a0,s3
    80200126:	00000097          	auipc	ra,0x0
    8020012a:	068080e7          	jalr	104(ra) # 8020018e <printf>
    fp = (uint64 *)*(fp - 2);
    8020012e:	ff04b483          	ld	s1,-16(s1)
  while (fp < bottom) {
    80200132:	ff24e6e3          	bltu	s1,s2,8020011e <backtrace+0x3a>
  }
}
    80200136:	70a2                	ld	ra,40(sp)
    80200138:	7402                	ld	s0,32(sp)
    8020013a:	64e2                	ld	s1,24(sp)
    8020013c:	6942                	ld	s2,16(sp)
    8020013e:	69a2                	ld	s3,8(sp)
    80200140:	6145                	addi	sp,sp,48
    80200142:	8082                	ret

0000000080200144 <panic>:
{
    80200144:	1101                	addi	sp,sp,-32
    80200146:	ec06                	sd	ra,24(sp)
    80200148:	e822                	sd	s0,16(sp)
    8020014a:	e426                	sd	s1,8(sp)
    8020014c:	1000                	addi	s0,sp,32
    8020014e:	84aa                	mv	s1,a0
  printf("panic: ");
    80200150:	00009517          	auipc	a0,0x9
    80200154:	ec050513          	addi	a0,a0,-320 # 80209010 <etext+0x10>
    80200158:	00000097          	auipc	ra,0x0
    8020015c:	036080e7          	jalr	54(ra) # 8020018e <printf>
  printf(s);
    80200160:	8526                	mv	a0,s1
    80200162:	00000097          	auipc	ra,0x0
    80200166:	02c080e7          	jalr	44(ra) # 8020018e <printf>
  printf("\n");
    8020016a:	00009517          	auipc	a0,0x9
    8020016e:	f6e50513          	addi	a0,a0,-146 # 802090d8 <etext+0xd8>
    80200172:	00000097          	auipc	ra,0x0
    80200176:	01c080e7          	jalr	28(ra) # 8020018e <printf>
  backtrace();
    8020017a:	00000097          	auipc	ra,0x0
    8020017e:	f6a080e7          	jalr	-150(ra) # 802000e4 <backtrace>
  panicked = 1; // freeze uart output from other CPUs
    80200182:	4785                	li	a5,1
    80200184:	0003c717          	auipc	a4,0x3c
    80200188:	e6f72e23          	sw	a5,-388(a4) # 8023c000 <panicked>
  for(;;)
    8020018c:	a001                	j	8020018c <panic+0x48>

000000008020018e <printf>:
{
    8020018e:	7131                	addi	sp,sp,-192
    80200190:	fc86                	sd	ra,120(sp)
    80200192:	f8a2                	sd	s0,112(sp)
    80200194:	f4a6                	sd	s1,104(sp)
    80200196:	f0ca                	sd	s2,96(sp)
    80200198:	ecce                	sd	s3,88(sp)
    8020019a:	e8d2                	sd	s4,80(sp)
    8020019c:	e4d6                	sd	s5,72(sp)
    8020019e:	e0da                	sd	s6,64(sp)
    802001a0:	fc5e                	sd	s7,56(sp)
    802001a2:	f862                	sd	s8,48(sp)
    802001a4:	f466                	sd	s9,40(sp)
    802001a6:	f06a                	sd	s10,32(sp)
    802001a8:	ec6e                	sd	s11,24(sp)
    802001aa:	0100                	addi	s0,sp,128
    802001ac:	8a2a                	mv	s4,a0
    802001ae:	e40c                	sd	a1,8(s0)
    802001b0:	e810                	sd	a2,16(s0)
    802001b2:	ec14                	sd	a3,24(s0)
    802001b4:	f018                	sd	a4,32(s0)
    802001b6:	f41c                	sd	a5,40(s0)
    802001b8:	03043823          	sd	a6,48(s0)
    802001bc:	03143c23          	sd	a7,56(s0)
  locking = pr.locking;
    802001c0:	0003cd97          	auipc	s11,0x3c
    802001c4:	e60dad83          	lw	s11,-416(s11) # 8023c020 <pr+0x18>
  if(locking)
    802001c8:	020d9b63          	bnez	s11,802001fe <printf+0x70>
  if (fmt == 0)
    802001cc:	040a0263          	beqz	s4,80200210 <printf+0x82>
  va_start(ap, fmt);
    802001d0:	00840793          	addi	a5,s0,8
    802001d4:	f8f43423          	sd	a5,-120(s0)
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    802001d8:	000a4503          	lbu	a0,0(s4)
    802001dc:	14050f63          	beqz	a0,8020033a <printf+0x1ac>
    802001e0:	4981                	li	s3,0
    if(c != '%'){
    802001e2:	02500a93          	li	s5,37
    switch(c){
    802001e6:	07000b93          	li	s7,112
  consputc('x');
    802001ea:	4d41                	li	s10,16
    consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
    802001ec:	00009b17          	auipc	s6,0x9
    802001f0:	194b0b13          	addi	s6,s6,404 # 80209380 <digits>
    switch(c){
    802001f4:	07300c93          	li	s9,115
    802001f8:	06400c13          	li	s8,100
    802001fc:	a82d                	j	80200236 <printf+0xa8>
    acquire(&pr.lock);
    802001fe:	0003c517          	auipc	a0,0x3c
    80200202:	e0a50513          	addi	a0,a0,-502 # 8023c008 <pr>
    80200206:	00000097          	auipc	ra,0x0
    8020020a:	4ee080e7          	jalr	1262(ra) # 802006f4 <acquire>
    8020020e:	bf7d                	j	802001cc <printf+0x3e>
    panic("null fmt");
    80200210:	00009517          	auipc	a0,0x9
    80200214:	e1050513          	addi	a0,a0,-496 # 80209020 <etext+0x20>
    80200218:	00000097          	auipc	ra,0x0
    8020021c:	f2c080e7          	jalr	-212(ra) # 80200144 <panic>
      consputc(c);
    80200220:	00007097          	auipc	ra,0x7
    80200224:	49e080e7          	jalr	1182(ra) # 802076be <consputc>
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    80200228:	2985                	addiw	s3,s3,1
    8020022a:	013a07b3          	add	a5,s4,s3
    8020022e:	0007c503          	lbu	a0,0(a5) # fffffffffffff000 <ebss_clear+0xffffffff7fda7000>
    80200232:	10050463          	beqz	a0,8020033a <printf+0x1ac>
    if(c != '%'){
    80200236:	ff5515e3          	bne	a0,s5,80200220 <printf+0x92>
    c = fmt[++i] & 0xff;
    8020023a:	2985                	addiw	s3,s3,1
    8020023c:	013a07b3          	add	a5,s4,s3
    80200240:	0007c783          	lbu	a5,0(a5)
    80200244:	0007849b          	sext.w	s1,a5
    if(c == 0)
    80200248:	cbed                	beqz	a5,8020033a <printf+0x1ac>
    switch(c){
    8020024a:	05778a63          	beq	a5,s7,8020029e <printf+0x110>
    8020024e:	02fbf663          	bgeu	s7,a5,8020027a <printf+0xec>
    80200252:	09978863          	beq	a5,s9,802002e2 <printf+0x154>
    80200256:	07800713          	li	a4,120
    8020025a:	0ce79563          	bne	a5,a4,80200324 <printf+0x196>
      printint(va_arg(ap, int), 16, 1);
    8020025e:	f8843783          	ld	a5,-120(s0)
    80200262:	00878713          	addi	a4,a5,8
    80200266:	f8e43423          	sd	a4,-120(s0)
    8020026a:	4605                	li	a2,1
    8020026c:	85ea                	mv	a1,s10
    8020026e:	4388                	lw	a0,0(a5)
    80200270:	00000097          	auipc	ra,0x0
    80200274:	da6080e7          	jalr	-602(ra) # 80200016 <printint>
      break;
    80200278:	bf45                	j	80200228 <printf+0x9a>
    switch(c){
    8020027a:	09578f63          	beq	a5,s5,80200318 <printf+0x18a>
    8020027e:	0b879363          	bne	a5,s8,80200324 <printf+0x196>
      printint(va_arg(ap, int), 10, 1);
    80200282:	f8843783          	ld	a5,-120(s0)
    80200286:	00878713          	addi	a4,a5,8
    8020028a:	f8e43423          	sd	a4,-120(s0)
    8020028e:	4605                	li	a2,1
    80200290:	45a9                	li	a1,10
    80200292:	4388                	lw	a0,0(a5)
    80200294:	00000097          	auipc	ra,0x0
    80200298:	d82080e7          	jalr	-638(ra) # 80200016 <printint>
      break;
    8020029c:	b771                	j	80200228 <printf+0x9a>
      printptr(va_arg(ap, uint64));
    8020029e:	f8843783          	ld	a5,-120(s0)
    802002a2:	00878713          	addi	a4,a5,8
    802002a6:	f8e43423          	sd	a4,-120(s0)
    802002aa:	0007b903          	ld	s2,0(a5)
  consputc('0');
    802002ae:	03000513          	li	a0,48
    802002b2:	00007097          	auipc	ra,0x7
    802002b6:	40c080e7          	jalr	1036(ra) # 802076be <consputc>
  consputc('x');
    802002ba:	07800513          	li	a0,120
    802002be:	00007097          	auipc	ra,0x7
    802002c2:	400080e7          	jalr	1024(ra) # 802076be <consputc>
    802002c6:	84ea                	mv	s1,s10
    consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
    802002c8:	03c95793          	srli	a5,s2,0x3c
    802002cc:	97da                	add	a5,a5,s6
    802002ce:	0007c503          	lbu	a0,0(a5)
    802002d2:	00007097          	auipc	ra,0x7
    802002d6:	3ec080e7          	jalr	1004(ra) # 802076be <consputc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    802002da:	0912                	slli	s2,s2,0x4
    802002dc:	34fd                	addiw	s1,s1,-1
    802002de:	f4ed                	bnez	s1,802002c8 <printf+0x13a>
    802002e0:	b7a1                	j	80200228 <printf+0x9a>
      if((s = va_arg(ap, char*)) == 0)
    802002e2:	f8843783          	ld	a5,-120(s0)
    802002e6:	00878713          	addi	a4,a5,8
    802002ea:	f8e43423          	sd	a4,-120(s0)
    802002ee:	6384                	ld	s1,0(a5)
    802002f0:	cc89                	beqz	s1,8020030a <printf+0x17c>
      for(; *s; s++)
    802002f2:	0004c503          	lbu	a0,0(s1)
    802002f6:	d90d                	beqz	a0,80200228 <printf+0x9a>
        consputc(*s);
    802002f8:	00007097          	auipc	ra,0x7
    802002fc:	3c6080e7          	jalr	966(ra) # 802076be <consputc>
      for(; *s; s++)
    80200300:	0485                	addi	s1,s1,1
    80200302:	0004c503          	lbu	a0,0(s1)
    80200306:	f96d                	bnez	a0,802002f8 <printf+0x16a>
    80200308:	b705                	j	80200228 <printf+0x9a>
        s = "(null)";
    8020030a:	00009497          	auipc	s1,0x9
    8020030e:	d0e48493          	addi	s1,s1,-754 # 80209018 <etext+0x18>
      for(; *s; s++)
    80200312:	02800513          	li	a0,40
    80200316:	b7cd                	j	802002f8 <printf+0x16a>
      consputc('%');
    80200318:	8556                	mv	a0,s5
    8020031a:	00007097          	auipc	ra,0x7
    8020031e:	3a4080e7          	jalr	932(ra) # 802076be <consputc>
      break;
    80200322:	b719                	j	80200228 <printf+0x9a>
      consputc('%');
    80200324:	8556                	mv	a0,s5
    80200326:	00007097          	auipc	ra,0x7
    8020032a:	398080e7          	jalr	920(ra) # 802076be <consputc>
      consputc(c);
    8020032e:	8526                	mv	a0,s1
    80200330:	00007097          	auipc	ra,0x7
    80200334:	38e080e7          	jalr	910(ra) # 802076be <consputc>
      break;
    80200338:	bdc5                	j	80200228 <printf+0x9a>
  if(locking)
    8020033a:	020d9163          	bnez	s11,8020035c <printf+0x1ce>
}
    8020033e:	70e6                	ld	ra,120(sp)
    80200340:	7446                	ld	s0,112(sp)
    80200342:	74a6                	ld	s1,104(sp)
    80200344:	7906                	ld	s2,96(sp)
    80200346:	69e6                	ld	s3,88(sp)
    80200348:	6a46                	ld	s4,80(sp)
    8020034a:	6aa6                	ld	s5,72(sp)
    8020034c:	6b06                	ld	s6,64(sp)
    8020034e:	7be2                	ld	s7,56(sp)
    80200350:	7c42                	ld	s8,48(sp)
    80200352:	7ca2                	ld	s9,40(sp)
    80200354:	7d02                	ld	s10,32(sp)
    80200356:	6de2                	ld	s11,24(sp)
    80200358:	6129                	addi	sp,sp,192
    8020035a:	8082                	ret
    release(&pr.lock);
    8020035c:	0003c517          	auipc	a0,0x3c
    80200360:	cac50513          	addi	a0,a0,-852 # 8023c008 <pr>
    80200364:	00000097          	auipc	ra,0x0
    80200368:	3e4080e7          	jalr	996(ra) # 80200748 <release>
}
    8020036c:	bfc9                	j	8020033e <printf+0x1b0>

000000008020036e <printfinit>:

void
printfinit(void)
{
    8020036e:	1141                	addi	sp,sp,-16
    80200370:	e406                	sd	ra,8(sp)
    80200372:	e022                	sd	s0,0(sp)
    80200374:	0800                	addi	s0,sp,16
  initlock(&pr.lock, "pr");
    80200376:	00009597          	auipc	a1,0x9
    8020037a:	cba58593          	addi	a1,a1,-838 # 80209030 <etext+0x30>
    8020037e:	0003c517          	auipc	a0,0x3c
    80200382:	c8a50513          	addi	a0,a0,-886 # 8023c008 <pr>
    80200386:	00000097          	auipc	ra,0x0
    8020038a:	32a080e7          	jalr	810(ra) # 802006b0 <initlock>
  pr.locking = 1;   // changed, used to be 1
    8020038e:	4785                	li	a5,1
    80200390:	0003c717          	auipc	a4,0x3c
    80200394:	c8f72823          	sw	a5,-880(a4) # 8023c020 <pr+0x18>
}
    80200398:	60a2                	ld	ra,8(sp)
    8020039a:	6402                	ld	s0,0(sp)
    8020039c:	0141                	addi	sp,sp,16
    8020039e:	8082                	ret

00000000802003a0 <print_logo>:

#ifdef QEMU
void print_logo() {
    802003a0:	1141                	addi	sp,sp,-16
    802003a2:	e406                	sd	ra,8(sp)
    802003a4:	e022                	sd	s0,0(sp)
    802003a6:	0800                	addi	s0,sp,16
    printf("  (`-.            (`-.                            .-')       ('-.    _   .-')\n");
    802003a8:	00009517          	auipc	a0,0x9
    802003ac:	c9050513          	addi	a0,a0,-880 # 80209038 <etext+0x38>
    802003b0:	00000097          	auipc	ra,0x0
    802003b4:	dde080e7          	jalr	-546(ra) # 8020018e <printf>
    printf(" ( OO ).        _(OO  )_                        .(  OO)    _(  OO)  ( '.( OO )_ \n");
    802003b8:	00009517          	auipc	a0,0x9
    802003bc:	cd050513          	addi	a0,a0,-816 # 80209088 <etext+0x88>
    802003c0:	00000097          	auipc	ra,0x0
    802003c4:	dce080e7          	jalr	-562(ra) # 8020018e <printf>
    printf("(_/.  \\_)-. ,--(_/   ,. \\  ,--.                (_)---\\_)  (,------.  ,--.   ,--.) ,--. ,--.  \n");
    802003c8:	00009517          	auipc	a0,0x9
    802003cc:	d1850513          	addi	a0,a0,-744 # 802090e0 <etext+0xe0>
    802003d0:	00000097          	auipc	ra,0x0
    802003d4:	dbe080e7          	jalr	-578(ra) # 8020018e <printf>
    printf(" \\  `.'  /  \\   \\   /(__/ /  .'       .-')     '  .-.  '   |  .---'  |   `.'   |  |  | |  |   \n");
    802003d8:	00009517          	auipc	a0,0x9
    802003dc:	d6850513          	addi	a0,a0,-664 # 80209140 <etext+0x140>
    802003e0:	00000097          	auipc	ra,0x0
    802003e4:	dae080e7          	jalr	-594(ra) # 8020018e <printf>
    printf("  \\     /\\   \\   \\ /   / .  / -.    _(  OO)   ,|  | |  |   |  |      |         |  |  | | .-')\n");
    802003e8:	00009517          	auipc	a0,0x9
    802003ec:	db850513          	addi	a0,a0,-584 # 802091a0 <etext+0x1a0>
    802003f0:	00000097          	auipc	ra,0x0
    802003f4:	d9e080e7          	jalr	-610(ra) # 8020018e <printf>
    printf("   \\   \\ |    \\   '   /, | .-.  '  (,------. (_|  | |  |  (|  '--.   |  |'.'|  |  |  |_|( OO )\n");
    802003f8:	00009517          	auipc	a0,0x9
    802003fc:	e0850513          	addi	a0,a0,-504 # 80209200 <etext+0x200>
    80200400:	00000097          	auipc	ra,0x0
    80200404:	d8e080e7          	jalr	-626(ra) # 8020018e <printf>
    printf("  .'    \\_)    \\     /__)' \\  |  |  '------'   |  | |  |   |  .--'   |  |   |  |  |  | | `-' /\n");
    80200408:	00009517          	auipc	a0,0x9
    8020040c:	e5850513          	addi	a0,a0,-424 # 80209260 <etext+0x260>
    80200410:	00000097          	auipc	ra,0x0
    80200414:	d7e080e7          	jalr	-642(ra) # 8020018e <printf>
    printf(" /  .'.  \\      \\   /    \\  `'  /              '  '-'  '-. |  `---.  |  |   |  | ('  '-'(_.-'\n");
    80200418:	00009517          	auipc	a0,0x9
    8020041c:	ea850513          	addi	a0,a0,-344 # 802092c0 <etext+0x2c0>
    80200420:	00000097          	auipc	ra,0x0
    80200424:	d6e080e7          	jalr	-658(ra) # 8020018e <printf>
    printf("'--'   '--'      `-'      `----'                `-----'--' `------'  `--'   `--'   `-----'\n");
    80200428:	00009517          	auipc	a0,0x9
    8020042c:	ef850513          	addi	a0,a0,-264 # 80209320 <etext+0x320>
    80200430:	00000097          	auipc	ra,0x0
    80200434:	d5e080e7          	jalr	-674(ra) # 8020018e <printf>
}
    80200438:	60a2                	ld	ra,8(sp)
    8020043a:	6402                	ld	s0,0(sp)
    8020043c:	0141                	addi	sp,sp,16
    8020043e:	8082                	ret

0000000080200440 <kfree>:
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
    80200440:	1101                	addi	sp,sp,-32
    80200442:	ec06                	sd	ra,24(sp)
    80200444:	e822                	sd	s0,16(sp)
    80200446:	e426                	sd	s1,8(sp)
    80200448:	e04a                	sd	s2,0(sp)
    8020044a:	1000                	addi	s0,sp,32
  struct run *r;
  
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < kernel_end || (uint64)pa >= PHYSTOP)
    8020044c:	03451793          	slli	a5,a0,0x34
    80200450:	e3ad                	bnez	a5,802004b2 <kfree+0x72>
    80200452:	84aa                	mv	s1,a0
    80200454:	00033797          	auipc	a5,0x33
    80200458:	eac7b783          	ld	a5,-340(a5) # 80233300 <_GLOBAL_OFFSET_TABLE_+0x28>
    8020045c:	04f56b63          	bltu	a0,a5,802004b2 <kfree+0x72>
    80200460:	40300793          	li	a5,1027
    80200464:	07d6                	slli	a5,a5,0x15
    80200466:	04f57663          	bgeu	a0,a5,802004b2 <kfree+0x72>
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);
    8020046a:	6605                	lui	a2,0x1
    8020046c:	4585                	li	a1,1
    8020046e:	00000097          	auipc	ra,0x0
    80200472:	322080e7          	jalr	802(ra) # 80200790 <memset>

  r = (struct run*)pa;

  acquire(&kmem.lock);
    80200476:	0003c917          	auipc	s2,0x3c
    8020047a:	bb290913          	addi	s2,s2,-1102 # 8023c028 <kmem>
    8020047e:	854a                	mv	a0,s2
    80200480:	00000097          	auipc	ra,0x0
    80200484:	274080e7          	jalr	628(ra) # 802006f4 <acquire>
  r->next = kmem.freelist;
    80200488:	01893783          	ld	a5,24(s2)
    8020048c:	e09c                	sd	a5,0(s1)
  kmem.freelist = r;
    8020048e:	00993c23          	sd	s1,24(s2)
  kmem.npage++;
    80200492:	02093783          	ld	a5,32(s2)
    80200496:	0785                	addi	a5,a5,1
    80200498:	02f93023          	sd	a5,32(s2)
  release(&kmem.lock);
    8020049c:	854a                	mv	a0,s2
    8020049e:	00000097          	auipc	ra,0x0
    802004a2:	2aa080e7          	jalr	682(ra) # 80200748 <release>
}
    802004a6:	60e2                	ld	ra,24(sp)
    802004a8:	6442                	ld	s0,16(sp)
    802004aa:	64a2                	ld	s1,8(sp)
    802004ac:	6902                	ld	s2,0(sp)
    802004ae:	6105                	addi	sp,sp,32
    802004b0:	8082                	ret
    panic("kfree");
    802004b2:	00009517          	auipc	a0,0x9
    802004b6:	ee650513          	addi	a0,a0,-282 # 80209398 <digits+0x18>
    802004ba:	00000097          	auipc	ra,0x0
    802004be:	c8a080e7          	jalr	-886(ra) # 80200144 <panic>

00000000802004c2 <freerange>:
{
    802004c2:	7179                	addi	sp,sp,-48
    802004c4:	f406                	sd	ra,40(sp)
    802004c6:	f022                	sd	s0,32(sp)
    802004c8:	ec26                	sd	s1,24(sp)
    802004ca:	e84a                	sd	s2,16(sp)
    802004cc:	e44e                	sd	s3,8(sp)
    802004ce:	e052                	sd	s4,0(sp)
    802004d0:	1800                	addi	s0,sp,48
  p = (char*)PGROUNDUP((uint64)pa_start);
    802004d2:	6785                	lui	a5,0x1
    802004d4:	fff78713          	addi	a4,a5,-1 # fff <_entry-0x801ff001>
    802004d8:	00e504b3          	add	s1,a0,a4
    802004dc:	777d                	lui	a4,0xfffff
    802004de:	8cf9                	and	s1,s1,a4
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    802004e0:	94be                	add	s1,s1,a5
    802004e2:	0095ee63          	bltu	a1,s1,802004fe <freerange+0x3c>
    802004e6:	892e                	mv	s2,a1
    kfree(p);
    802004e8:	7a7d                	lui	s4,0xfffff
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    802004ea:	6985                	lui	s3,0x1
    kfree(p);
    802004ec:	01448533          	add	a0,s1,s4
    802004f0:	00000097          	auipc	ra,0x0
    802004f4:	f50080e7          	jalr	-176(ra) # 80200440 <kfree>
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    802004f8:	94ce                	add	s1,s1,s3
    802004fa:	fe9979e3          	bgeu	s2,s1,802004ec <freerange+0x2a>
}
    802004fe:	70a2                	ld	ra,40(sp)
    80200500:	7402                	ld	s0,32(sp)
    80200502:	64e2                	ld	s1,24(sp)
    80200504:	6942                	ld	s2,16(sp)
    80200506:	69a2                	ld	s3,8(sp)
    80200508:	6a02                	ld	s4,0(sp)
    8020050a:	6145                	addi	sp,sp,48
    8020050c:	8082                	ret

000000008020050e <kinit>:
{
    8020050e:	1101                	addi	sp,sp,-32
    80200510:	ec06                	sd	ra,24(sp)
    80200512:	e822                	sd	s0,16(sp)
    80200514:	e426                	sd	s1,8(sp)
    80200516:	1000                	addi	s0,sp,32
  initlock(&kmem.lock, "kmem");
    80200518:	0003c497          	auipc	s1,0x3c
    8020051c:	b1048493          	addi	s1,s1,-1264 # 8023c028 <kmem>
    80200520:	00009597          	auipc	a1,0x9
    80200524:	e8058593          	addi	a1,a1,-384 # 802093a0 <digits+0x20>
    80200528:	8526                	mv	a0,s1
    8020052a:	00000097          	auipc	ra,0x0
    8020052e:	186080e7          	jalr	390(ra) # 802006b0 <initlock>
  kmem.freelist = 0;
    80200532:	0004bc23          	sd	zero,24(s1)
  kmem.npage = 0;
    80200536:	0204b023          	sd	zero,32(s1)
  freerange(kernel_end, (void*)PHYSTOP);
    8020053a:	40300493          	li	s1,1027
    8020053e:	01549593          	slli	a1,s1,0x15
    80200542:	00033517          	auipc	a0,0x33
    80200546:	dbe53503          	ld	a0,-578(a0) # 80233300 <_GLOBAL_OFFSET_TABLE_+0x28>
    8020054a:	00000097          	auipc	ra,0x0
    8020054e:	f78080e7          	jalr	-136(ra) # 802004c2 <freerange>
  printf("kernel_end: %p, phystop: %p\n", kernel_end, (void*)PHYSTOP);
    80200552:	01549613          	slli	a2,s1,0x15
    80200556:	00033597          	auipc	a1,0x33
    8020055a:	daa5b583          	ld	a1,-598(a1) # 80233300 <_GLOBAL_OFFSET_TABLE_+0x28>
    8020055e:	00009517          	auipc	a0,0x9
    80200562:	e4a50513          	addi	a0,a0,-438 # 802093a8 <digits+0x28>
    80200566:	00000097          	auipc	ra,0x0
    8020056a:	c28080e7          	jalr	-984(ra) # 8020018e <printf>
  printf("kinit\n");
    8020056e:	00009517          	auipc	a0,0x9
    80200572:	e5a50513          	addi	a0,a0,-422 # 802093c8 <digits+0x48>
    80200576:	00000097          	auipc	ra,0x0
    8020057a:	c18080e7          	jalr	-1000(ra) # 8020018e <printf>
}
    8020057e:	60e2                	ld	ra,24(sp)
    80200580:	6442                	ld	s0,16(sp)
    80200582:	64a2                	ld	s1,8(sp)
    80200584:	6105                	addi	sp,sp,32
    80200586:	8082                	ret

0000000080200588 <kalloc>:
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
    80200588:	1101                	addi	sp,sp,-32
    8020058a:	ec06                	sd	ra,24(sp)
    8020058c:	e822                	sd	s0,16(sp)
    8020058e:	e426                	sd	s1,8(sp)
    80200590:	1000                	addi	s0,sp,32
  struct run *r;

  acquire(&kmem.lock);
    80200592:	0003c497          	auipc	s1,0x3c
    80200596:	a9648493          	addi	s1,s1,-1386 # 8023c028 <kmem>
    8020059a:	8526                	mv	a0,s1
    8020059c:	00000097          	auipc	ra,0x0
    802005a0:	158080e7          	jalr	344(ra) # 802006f4 <acquire>
  r = kmem.freelist;
    802005a4:	6c84                	ld	s1,24(s1)
  if(r) {
    802005a6:	c89d                	beqz	s1,802005dc <kalloc+0x54>
    kmem.freelist = r->next;
    802005a8:	609c                	ld	a5,0(s1)
    802005aa:	0003c517          	auipc	a0,0x3c
    802005ae:	a7e50513          	addi	a0,a0,-1410 # 8023c028 <kmem>
    802005b2:	ed1c                	sd	a5,24(a0)
    kmem.npage--;
    802005b4:	711c                	ld	a5,32(a0)
    802005b6:	17fd                	addi	a5,a5,-1
    802005b8:	f11c                	sd	a5,32(a0)
  }
  release(&kmem.lock);
    802005ba:	00000097          	auipc	ra,0x0
    802005be:	18e080e7          	jalr	398(ra) # 80200748 <release>

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
    802005c2:	6605                	lui	a2,0x1
    802005c4:	4595                	li	a1,5
    802005c6:	8526                	mv	a0,s1
    802005c8:	00000097          	auipc	ra,0x0
    802005cc:	1c8080e7          	jalr	456(ra) # 80200790 <memset>
  return (void*)r;
}
    802005d0:	8526                	mv	a0,s1
    802005d2:	60e2                	ld	ra,24(sp)
    802005d4:	6442                	ld	s0,16(sp)
    802005d6:	64a2                	ld	s1,8(sp)
    802005d8:	6105                	addi	sp,sp,32
    802005da:	8082                	ret
  release(&kmem.lock);
    802005dc:	0003c517          	auipc	a0,0x3c
    802005e0:	a4c50513          	addi	a0,a0,-1460 # 8023c028 <kmem>
    802005e4:	00000097          	auipc	ra,0x0
    802005e8:	164080e7          	jalr	356(ra) # 80200748 <release>
  if(r)
    802005ec:	b7d5                	j	802005d0 <kalloc+0x48>

00000000802005ee <freemem_amount>:

uint64
freemem_amount(void)
{
    802005ee:	1141                	addi	sp,sp,-16
    802005f0:	e422                	sd	s0,8(sp)
    802005f2:	0800                	addi	s0,sp,16
  return kmem.npage << PGSHIFT;
}
    802005f4:	0003c517          	auipc	a0,0x3c
    802005f8:	a5453503          	ld	a0,-1452(a0) # 8023c048 <kmem+0x20>
    802005fc:	0532                	slli	a0,a0,0xc
    802005fe:	6422                	ld	s0,8(sp)
    80200600:	0141                	addi	sp,sp,16
    80200602:	8082                	ret

0000000080200604 <push_off>:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

void
push_off(void)
{
    80200604:	1101                	addi	sp,sp,-32
    80200606:	ec06                	sd	ra,24(sp)
    80200608:	e822                	sd	s0,16(sp)
    8020060a:	e426                	sd	s1,8(sp)
    8020060c:	1000                	addi	s0,sp,32
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    8020060e:	100024f3          	csrr	s1,sstatus
    80200612:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() & ~SSTATUS_SIE);
    80200616:	9bf5                	andi	a5,a5,-3
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80200618:	10079073          	csrw	sstatus,a5
  int old = intr_get();

  intr_off();
  //printf("\e[32mpush_off()\e[0m: cpuid(): %d\n", cpuid());
  if(mycpu()->noff == 0)
    8020061c:	00001097          	auipc	ra,0x1
    80200620:	4a0080e7          	jalr	1184(ra) # 80201abc <mycpu>
    80200624:	5d3c                	lw	a5,120(a0)
    80200626:	cf89                	beqz	a5,80200640 <push_off+0x3c>
    mycpu()->intena = old;
  mycpu()->noff += 1;
    80200628:	00001097          	auipc	ra,0x1
    8020062c:	494080e7          	jalr	1172(ra) # 80201abc <mycpu>
    80200630:	5d3c                	lw	a5,120(a0)
    80200632:	2785                	addiw	a5,a5,1
    80200634:	dd3c                	sw	a5,120(a0)
}
    80200636:	60e2                	ld	ra,24(sp)
    80200638:	6442                	ld	s0,16(sp)
    8020063a:	64a2                	ld	s1,8(sp)
    8020063c:	6105                	addi	sp,sp,32
    8020063e:	8082                	ret
    mycpu()->intena = old;
    80200640:	00001097          	auipc	ra,0x1
    80200644:	47c080e7          	jalr	1148(ra) # 80201abc <mycpu>
  return (x & SSTATUS_SIE) != 0;
    80200648:	8085                	srli	s1,s1,0x1
    8020064a:	8885                	andi	s1,s1,1
    8020064c:	dd64                	sw	s1,124(a0)
    8020064e:	bfe9                	j	80200628 <push_off+0x24>

0000000080200650 <pop_off>:

void
pop_off(void)
{
    80200650:	1141                	addi	sp,sp,-16
    80200652:	e406                	sd	ra,8(sp)
    80200654:	e022                	sd	s0,0(sp)
    80200656:	0800                	addi	s0,sp,16
  struct cpu *c = mycpu();
    80200658:	00001097          	auipc	ra,0x1
    8020065c:	464080e7          	jalr	1124(ra) # 80201abc <mycpu>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80200660:	100027f3          	csrr	a5,sstatus
  return (x & SSTATUS_SIE) != 0;
    80200664:	8b89                	andi	a5,a5,2

  //printf("\e[31mpop_off()\e[0m: cpuid(): %d\n", cpuid());
  if(intr_get())
    80200666:	e78d                	bnez	a5,80200690 <pop_off+0x40>
    panic("pop_off - interruptible");
  if(c->noff < 1) {
    80200668:	5d3c                	lw	a5,120(a0)
    8020066a:	02f05b63          	blez	a5,802006a0 <pop_off+0x50>
    //printf("c->noff = %d\n", c->noff);
    panic("pop_off");
  }
  //printf("c->noff: %d\n", c->noff);
  //printf("c: %x\n", c);
  c->noff -= 1;
    8020066e:	37fd                	addiw	a5,a5,-1
    80200670:	0007871b          	sext.w	a4,a5
    80200674:	dd3c                	sw	a5,120(a0)
  if(c->noff == 0 && c->intena)
    80200676:	eb09                	bnez	a4,80200688 <pop_off+0x38>
    80200678:	5d7c                	lw	a5,124(a0)
    8020067a:	c799                	beqz	a5,80200688 <pop_off+0x38>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    8020067c:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() | SSTATUS_SIE);
    80200680:	0027e793          	ori	a5,a5,2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80200684:	10079073          	csrw	sstatus,a5
    intr_on();
}
    80200688:	60a2                	ld	ra,8(sp)
    8020068a:	6402                	ld	s0,0(sp)
    8020068c:	0141                	addi	sp,sp,16
    8020068e:	8082                	ret
    panic("pop_off - interruptible");
    80200690:	00009517          	auipc	a0,0x9
    80200694:	d4050513          	addi	a0,a0,-704 # 802093d0 <digits+0x50>
    80200698:	00000097          	auipc	ra,0x0
    8020069c:	aac080e7          	jalr	-1364(ra) # 80200144 <panic>
    panic("pop_off");
    802006a0:	00009517          	auipc	a0,0x9
    802006a4:	d4850513          	addi	a0,a0,-696 # 802093e8 <digits+0x68>
    802006a8:	00000097          	auipc	ra,0x0
    802006ac:	a9c080e7          	jalr	-1380(ra) # 80200144 <panic>

00000000802006b0 <initlock>:
#include "include/intr.h"
#include "include/printf.h"

void
initlock(struct spinlock *lk, char *name)
{
    802006b0:	1141                	addi	sp,sp,-16
    802006b2:	e422                	sd	s0,8(sp)
    802006b4:	0800                	addi	s0,sp,16
  lk->name = name;
    802006b6:	e50c                	sd	a1,8(a0)
  lk->locked = 0;
    802006b8:	00052023          	sw	zero,0(a0)
  lk->cpu = 0;
    802006bc:	00053823          	sd	zero,16(a0)
}
    802006c0:	6422                	ld	s0,8(sp)
    802006c2:	0141                	addi	sp,sp,16
    802006c4:	8082                	ret

00000000802006c6 <holding>:
// Interrupts must be off.
int
holding(struct spinlock *lk)
{
  int r;
  r = (lk->locked && lk->cpu == mycpu());
    802006c6:	411c                	lw	a5,0(a0)
    802006c8:	e399                	bnez	a5,802006ce <holding+0x8>
    802006ca:	4501                	li	a0,0
  return r;
}
    802006cc:	8082                	ret
{
    802006ce:	1101                	addi	sp,sp,-32
    802006d0:	ec06                	sd	ra,24(sp)
    802006d2:	e822                	sd	s0,16(sp)
    802006d4:	e426                	sd	s1,8(sp)
    802006d6:	1000                	addi	s0,sp,32
  r = (lk->locked && lk->cpu == mycpu());
    802006d8:	6904                	ld	s1,16(a0)
    802006da:	00001097          	auipc	ra,0x1
    802006de:	3e2080e7          	jalr	994(ra) # 80201abc <mycpu>
    802006e2:	40a48533          	sub	a0,s1,a0
    802006e6:	00153513          	seqz	a0,a0
}
    802006ea:	60e2                	ld	ra,24(sp)
    802006ec:	6442                	ld	s0,16(sp)
    802006ee:	64a2                	ld	s1,8(sp)
    802006f0:	6105                	addi	sp,sp,32
    802006f2:	8082                	ret

00000000802006f4 <acquire>:
{
    802006f4:	1101                	addi	sp,sp,-32
    802006f6:	ec06                	sd	ra,24(sp)
    802006f8:	e822                	sd	s0,16(sp)
    802006fa:	e426                	sd	s1,8(sp)
    802006fc:	1000                	addi	s0,sp,32
    802006fe:	84aa                	mv	s1,a0
  push_off(); // disable interrupts to avoid deadlock.
    80200700:	00000097          	auipc	ra,0x0
    80200704:	f04080e7          	jalr	-252(ra) # 80200604 <push_off>
  if(holding(lk))
    80200708:	8526                	mv	a0,s1
    8020070a:	00000097          	auipc	ra,0x0
    8020070e:	fbc080e7          	jalr	-68(ra) # 802006c6 <holding>
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0)
    80200712:	4705                	li	a4,1
  if(holding(lk))
    80200714:	e115                	bnez	a0,80200738 <acquire+0x44>
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0)
    80200716:	87ba                	mv	a5,a4
    80200718:	0cf4a7af          	amoswap.w.aq	a5,a5,(s1)
    8020071c:	2781                	sext.w	a5,a5
    8020071e:	ffe5                	bnez	a5,80200716 <acquire+0x22>
  __sync_synchronize();
    80200720:	0ff0000f          	fence
  lk->cpu = mycpu();
    80200724:	00001097          	auipc	ra,0x1
    80200728:	398080e7          	jalr	920(ra) # 80201abc <mycpu>
    8020072c:	e888                	sd	a0,16(s1)
}
    8020072e:	60e2                	ld	ra,24(sp)
    80200730:	6442                	ld	s0,16(sp)
    80200732:	64a2                	ld	s1,8(sp)
    80200734:	6105                	addi	sp,sp,32
    80200736:	8082                	ret
    panic("acquire");
    80200738:	00009517          	auipc	a0,0x9
    8020073c:	cb850513          	addi	a0,a0,-840 # 802093f0 <digits+0x70>
    80200740:	00000097          	auipc	ra,0x0
    80200744:	a04080e7          	jalr	-1532(ra) # 80200144 <panic>

0000000080200748 <release>:
{
    80200748:	1101                	addi	sp,sp,-32
    8020074a:	ec06                	sd	ra,24(sp)
    8020074c:	e822                	sd	s0,16(sp)
    8020074e:	e426                	sd	s1,8(sp)
    80200750:	1000                	addi	s0,sp,32
    80200752:	84aa                	mv	s1,a0
  if(!holding(lk))
    80200754:	00000097          	auipc	ra,0x0
    80200758:	f72080e7          	jalr	-142(ra) # 802006c6 <holding>
    8020075c:	c115                	beqz	a0,80200780 <release+0x38>
  lk->cpu = 0;
    8020075e:	0004b823          	sd	zero,16(s1)
  __sync_synchronize();
    80200762:	0ff0000f          	fence
  __sync_lock_release(&lk->locked);
    80200766:	0f50000f          	fence	iorw,ow
    8020076a:	0804a02f          	amoswap.w	zero,zero,(s1)
  pop_off();
    8020076e:	00000097          	auipc	ra,0x0
    80200772:	ee2080e7          	jalr	-286(ra) # 80200650 <pop_off>
}
    80200776:	60e2                	ld	ra,24(sp)
    80200778:	6442                	ld	s0,16(sp)
    8020077a:	64a2                	ld	s1,8(sp)
    8020077c:	6105                	addi	sp,sp,32
    8020077e:	8082                	ret
    panic("release");
    80200780:	00009517          	auipc	a0,0x9
    80200784:	c7850513          	addi	a0,a0,-904 # 802093f8 <digits+0x78>
    80200788:	00000097          	auipc	ra,0x0
    8020078c:	9bc080e7          	jalr	-1604(ra) # 80200144 <panic>

0000000080200790 <memset>:
#include "include/types.h"

void*
memset(void *dst, int c, uint n)
{
    80200790:	1141                	addi	sp,sp,-16
    80200792:	e422                	sd	s0,8(sp)
    80200794:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    80200796:	ca19                	beqz	a2,802007ac <memset+0x1c>
    80200798:	87aa                	mv	a5,a0
    8020079a:	1602                	slli	a2,a2,0x20
    8020079c:	9201                	srli	a2,a2,0x20
    8020079e:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
    802007a2:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
    802007a6:	0785                	addi	a5,a5,1
    802007a8:	fee79de3          	bne	a5,a4,802007a2 <memset+0x12>
  }
  return dst;
}
    802007ac:	6422                	ld	s0,8(sp)
    802007ae:	0141                	addi	sp,sp,16
    802007b0:	8082                	ret

00000000802007b2 <memcmp>:

int
memcmp(const void *v1, const void *v2, uint n)
{
    802007b2:	1141                	addi	sp,sp,-16
    802007b4:	e422                	sd	s0,8(sp)
    802007b6:	0800                	addi	s0,sp,16
  const uchar *s1, *s2;

  s1 = v1;
  s2 = v2;
  while(n-- > 0){
    802007b8:	ca05                	beqz	a2,802007e8 <memcmp+0x36>
    802007ba:	fff6069b          	addiw	a3,a2,-1 # fff <_entry-0x801ff001>
    802007be:	1682                	slli	a3,a3,0x20
    802007c0:	9281                	srli	a3,a3,0x20
    802007c2:	0685                	addi	a3,a3,1
    802007c4:	96aa                	add	a3,a3,a0
    if(*s1 != *s2)
    802007c6:	00054783          	lbu	a5,0(a0)
    802007ca:	0005c703          	lbu	a4,0(a1)
    802007ce:	00e79863          	bne	a5,a4,802007de <memcmp+0x2c>
      return *s1 - *s2;
    s1++, s2++;
    802007d2:	0505                	addi	a0,a0,1
    802007d4:	0585                	addi	a1,a1,1
  while(n-- > 0){
    802007d6:	fed518e3          	bne	a0,a3,802007c6 <memcmp+0x14>
  }

  return 0;
    802007da:	4501                	li	a0,0
    802007dc:	a019                	j	802007e2 <memcmp+0x30>
      return *s1 - *s2;
    802007de:	40e7853b          	subw	a0,a5,a4
}
    802007e2:	6422                	ld	s0,8(sp)
    802007e4:	0141                	addi	sp,sp,16
    802007e6:	8082                	ret
  return 0;
    802007e8:	4501                	li	a0,0
    802007ea:	bfe5                	j	802007e2 <memcmp+0x30>

00000000802007ec <memmove>:

void*
memmove(void *dst, const void *src, uint n)
{
    802007ec:	1141                	addi	sp,sp,-16
    802007ee:	e422                	sd	s0,8(sp)
    802007f0:	0800                	addi	s0,sp,16
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    802007f2:	02a5e563          	bltu	a1,a0,8020081c <memmove+0x30>
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
    802007f6:	fff6069b          	addiw	a3,a2,-1
    802007fa:	ce11                	beqz	a2,80200816 <memmove+0x2a>
    802007fc:	1682                	slli	a3,a3,0x20
    802007fe:	9281                	srli	a3,a3,0x20
    80200800:	0685                	addi	a3,a3,1
    80200802:	96ae                	add	a3,a3,a1
    80200804:	87aa                	mv	a5,a0
      *d++ = *s++;
    80200806:	0585                	addi	a1,a1,1
    80200808:	0785                	addi	a5,a5,1
    8020080a:	fff5c703          	lbu	a4,-1(a1)
    8020080e:	fee78fa3          	sb	a4,-1(a5)
    while(n-- > 0)
    80200812:	fed59ae3          	bne	a1,a3,80200806 <memmove+0x1a>

  return dst;
}
    80200816:	6422                	ld	s0,8(sp)
    80200818:	0141                	addi	sp,sp,16
    8020081a:	8082                	ret
  if(s < d && s + n > d){
    8020081c:	02061713          	slli	a4,a2,0x20
    80200820:	9301                	srli	a4,a4,0x20
    80200822:	00e587b3          	add	a5,a1,a4
    80200826:	fcf578e3          	bgeu	a0,a5,802007f6 <memmove+0xa>
    d += n;
    8020082a:	972a                	add	a4,a4,a0
    while(n-- > 0)
    8020082c:	fff6069b          	addiw	a3,a2,-1
    80200830:	d27d                	beqz	a2,80200816 <memmove+0x2a>
    80200832:	02069613          	slli	a2,a3,0x20
    80200836:	9201                	srli	a2,a2,0x20
    80200838:	fff64613          	not	a2,a2
    8020083c:	963e                	add	a2,a2,a5
      *--d = *--s;
    8020083e:	17fd                	addi	a5,a5,-1
    80200840:	177d                	addi	a4,a4,-1 # ffffffffffffefff <ebss_clear+0xffffffff7fda6fff>
    80200842:	0007c683          	lbu	a3,0(a5)
    80200846:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
    8020084a:	fef61ae3          	bne	a2,a5,8020083e <memmove+0x52>
    8020084e:	b7e1                	j	80200816 <memmove+0x2a>

0000000080200850 <memcpy>:

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, uint n)
{
    80200850:	1141                	addi	sp,sp,-16
    80200852:	e406                	sd	ra,8(sp)
    80200854:	e022                	sd	s0,0(sp)
    80200856:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
    80200858:	00000097          	auipc	ra,0x0
    8020085c:	f94080e7          	jalr	-108(ra) # 802007ec <memmove>
}
    80200860:	60a2                	ld	ra,8(sp)
    80200862:	6402                	ld	s0,0(sp)
    80200864:	0141                	addi	sp,sp,16
    80200866:	8082                	ret

0000000080200868 <strncmp>:

int
strncmp(const char *p, const char *q, uint n)
{
    80200868:	1141                	addi	sp,sp,-16
    8020086a:	e422                	sd	s0,8(sp)
    8020086c:	0800                	addi	s0,sp,16
  while(n > 0 && *p && *p == *q)
    8020086e:	ce11                	beqz	a2,8020088a <strncmp+0x22>
    80200870:	00054783          	lbu	a5,0(a0)
    80200874:	cf89                	beqz	a5,8020088e <strncmp+0x26>
    80200876:	0005c703          	lbu	a4,0(a1)
    8020087a:	00f71a63          	bne	a4,a5,8020088e <strncmp+0x26>
    n--, p++, q++;
    8020087e:	367d                	addiw	a2,a2,-1
    80200880:	0505                	addi	a0,a0,1
    80200882:	0585                	addi	a1,a1,1
  while(n > 0 && *p && *p == *q)
    80200884:	f675                	bnez	a2,80200870 <strncmp+0x8>
  if(n == 0)
    return 0;
    80200886:	4501                	li	a0,0
    80200888:	a809                	j	8020089a <strncmp+0x32>
    8020088a:	4501                	li	a0,0
    8020088c:	a039                	j	8020089a <strncmp+0x32>
  if(n == 0)
    8020088e:	ca09                	beqz	a2,802008a0 <strncmp+0x38>
  return (uchar)*p - (uchar)*q;
    80200890:	00054503          	lbu	a0,0(a0)
    80200894:	0005c783          	lbu	a5,0(a1)
    80200898:	9d1d                	subw	a0,a0,a5
}
    8020089a:	6422                	ld	s0,8(sp)
    8020089c:	0141                	addi	sp,sp,16
    8020089e:	8082                	ret
    return 0;
    802008a0:	4501                	li	a0,0
    802008a2:	bfe5                	j	8020089a <strncmp+0x32>

00000000802008a4 <strncpy>:

char*
strncpy(char *s, const char *t, int n)
{
    802008a4:	1141                	addi	sp,sp,-16
    802008a6:	e422                	sd	s0,8(sp)
    802008a8:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    802008aa:	872a                	mv	a4,a0
    802008ac:	8832                	mv	a6,a2
    802008ae:	367d                	addiw	a2,a2,-1
    802008b0:	01005963          	blez	a6,802008c2 <strncpy+0x1e>
    802008b4:	0705                	addi	a4,a4,1
    802008b6:	0005c783          	lbu	a5,0(a1)
    802008ba:	fef70fa3          	sb	a5,-1(a4)
    802008be:	0585                	addi	a1,a1,1
    802008c0:	f7f5                	bnez	a5,802008ac <strncpy+0x8>
    ;
  while(n-- > 0)
    802008c2:	86ba                	mv	a3,a4
    802008c4:	00c05c63          	blez	a2,802008dc <strncpy+0x38>
    *s++ = 0;
    802008c8:	0685                	addi	a3,a3,1
    802008ca:	fe068fa3          	sb	zero,-1(a3)
  while(n-- > 0)
    802008ce:	40d707bb          	subw	a5,a4,a3
    802008d2:	37fd                	addiw	a5,a5,-1
    802008d4:	010787bb          	addw	a5,a5,a6
    802008d8:	fef048e3          	bgtz	a5,802008c8 <strncpy+0x24>
  return os;
}
    802008dc:	6422                	ld	s0,8(sp)
    802008de:	0141                	addi	sp,sp,16
    802008e0:	8082                	ret

00000000802008e2 <safestrcpy>:

// Like strncpy but guaranteed to NUL-terminate.
char*
safestrcpy(char *s, const char *t, int n)
{
    802008e2:	1141                	addi	sp,sp,-16
    802008e4:	e422                	sd	s0,8(sp)
    802008e6:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  if(n <= 0)
    802008e8:	02c05363          	blez	a2,8020090e <safestrcpy+0x2c>
    802008ec:	fff6069b          	addiw	a3,a2,-1
    802008f0:	1682                	slli	a3,a3,0x20
    802008f2:	9281                	srli	a3,a3,0x20
    802008f4:	96ae                	add	a3,a3,a1
    802008f6:	87aa                	mv	a5,a0
    return os;
  while(--n > 0 && (*s++ = *t++) != 0)
    802008f8:	00d58963          	beq	a1,a3,8020090a <safestrcpy+0x28>
    802008fc:	0585                	addi	a1,a1,1
    802008fe:	0785                	addi	a5,a5,1
    80200900:	fff5c703          	lbu	a4,-1(a1)
    80200904:	fee78fa3          	sb	a4,-1(a5)
    80200908:	fb65                	bnez	a4,802008f8 <safestrcpy+0x16>
    ;
  *s = 0;
    8020090a:	00078023          	sb	zero,0(a5)
  return os;
}
    8020090e:	6422                	ld	s0,8(sp)
    80200910:	0141                	addi	sp,sp,16
    80200912:	8082                	ret

0000000080200914 <strlen>:

int
strlen(const char *s)
{
    80200914:	1141                	addi	sp,sp,-16
    80200916:	e422                	sd	s0,8(sp)
    80200918:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
    8020091a:	00054783          	lbu	a5,0(a0)
    8020091e:	cf91                	beqz	a5,8020093a <strlen+0x26>
    80200920:	0505                	addi	a0,a0,1
    80200922:	87aa                	mv	a5,a0
    80200924:	4685                	li	a3,1
    80200926:	9e89                	subw	a3,a3,a0
    80200928:	00f6853b          	addw	a0,a3,a5
    8020092c:	0785                	addi	a5,a5,1
    8020092e:	fff7c703          	lbu	a4,-1(a5)
    80200932:	fb7d                	bnez	a4,80200928 <strlen+0x14>
    ;
  return n;
}
    80200934:	6422                	ld	s0,8(sp)
    80200936:	0141                	addi	sp,sp,16
    80200938:	8082                	ret
  for(n = 0; s[n]; n++)
    8020093a:	4501                	li	a0,0
    8020093c:	bfe5                	j	80200934 <strlen+0x20>

000000008020093e <wnstr>:

// convert uchar string into wide char string 
void wnstr(wchar *dst, char const *src, int len) {
    8020093e:	1141                	addi	sp,sp,-16
    80200940:	e422                	sd	s0,8(sp)
    80200942:	0800                	addi	s0,sp,16
  while (len -- && *src) {
    80200944:	c20d                	beqz	a2,80200966 <wnstr+0x28>
    80200946:	02061793          	slli	a5,a2,0x20
    8020094a:	01f7d613          	srli	a2,a5,0x1f
    8020094e:	00c50733          	add	a4,a0,a2
    80200952:	0005c783          	lbu	a5,0(a1)
    80200956:	cb81                	beqz	a5,80200966 <wnstr+0x28>
    *(uchar*)dst = *src++;
    80200958:	0585                	addi	a1,a1,1
    8020095a:	00f50023          	sb	a5,0(a0)
    dst ++;
    8020095e:	0509                	addi	a0,a0,2
  while (len -- && *src) {
    80200960:	fee519e3          	bne	a0,a4,80200952 <wnstr+0x14>
    dst ++;
    80200964:	853a                	mv	a0,a4
  }

  *dst = 0;
    80200966:	00051023          	sh	zero,0(a0)
}
    8020096a:	6422                	ld	s0,8(sp)
    8020096c:	0141                	addi	sp,sp,16
    8020096e:	8082                	ret

0000000080200970 <snstr>:

// convert wide char string into uchar string 
void snstr(char *dst, wchar const *src, int len) {
    80200970:	1141                	addi	sp,sp,-16
    80200972:	e422                	sd	s0,8(sp)
    80200974:	0800                	addi	s0,sp,16
  while (len -- && *src) {
    80200976:	fff6071b          	addiw	a4,a2,-1
    8020097a:	02061693          	slli	a3,a2,0x20
    8020097e:	9281                	srli	a3,a3,0x20
    80200980:	96aa                	add	a3,a3,a0
    80200982:	c61d                	beqz	a2,802009b0 <snstr+0x40>
    80200984:	0005d783          	lhu	a5,0(a1)
    80200988:	cb89                	beqz	a5,8020099a <snstr+0x2a>
    *dst++ = (uchar)(*src & 0xff);
    8020098a:	0505                	addi	a0,a0,1
    8020098c:	fef50fa3          	sb	a5,-1(a0)
    src ++;
    80200990:	0589                	addi	a1,a1,2
  while (len -- && *src) {
    80200992:	377d                	addiw	a4,a4,-1
    80200994:	fed518e3          	bne	a0,a3,80200984 <snstr+0x14>
    *dst++ = (uchar)(*src & 0xff);
    80200998:	8536                	mv	a0,a3
  }
  while(len-- > 0)
    8020099a:	02071793          	slli	a5,a4,0x20
    8020099e:	9381                	srli	a5,a5,0x20
    802009a0:	97aa                	add	a5,a5,a0
    802009a2:	00e05763          	blez	a4,802009b0 <snstr+0x40>
    *dst++ = 0;
    802009a6:	0505                	addi	a0,a0,1
    802009a8:	fe050fa3          	sb	zero,-1(a0)
  while(len-- > 0)
    802009ac:	fea79de3          	bne	a5,a0,802009a6 <snstr+0x36>
}
    802009b0:	6422                	ld	s0,8(sp)
    802009b2:	0141                	addi	sp,sp,16
    802009b4:	8082                	ret

00000000802009b6 <wcsncmp>:

int wcsncmp(wchar const *s1, wchar const *s2, int len) {
    802009b6:	1141                	addi	sp,sp,-16
    802009b8:	e422                	sd	s0,8(sp)
    802009ba:	0800                	addi	s0,sp,16
    802009bc:	872a                	mv	a4,a0
  int ret = 0;

  while (len-- && *s1) {
    802009be:	02061793          	slli	a5,a2,0x20
    802009c2:	01f7d613          	srli	a2,a5,0x1f
    802009c6:	962e                	add	a2,a2,a1
    802009c8:	00c58f63          	beq	a1,a2,802009e6 <wcsncmp+0x30>
    802009cc:	00075783          	lhu	a5,0(a4)
    802009d0:	cb89                	beqz	a5,802009e2 <wcsncmp+0x2c>
    ret = (int)(*s1++ - *s2++);
    802009d2:	0709                	addi	a4,a4,2
    802009d4:	0589                	addi	a1,a1,2
    802009d6:	ffe5d683          	lhu	a3,-2(a1)
    802009da:	40d7853b          	subw	a0,a5,a3
    if (ret) break;
    802009de:	d56d                	beqz	a0,802009c8 <wcsncmp+0x12>
    802009e0:	a021                	j	802009e8 <wcsncmp+0x32>
    802009e2:	4501                	li	a0,0
    802009e4:	a011                	j	802009e8 <wcsncmp+0x32>
    802009e6:	4501                	li	a0,0
  }

  return ret;
}
    802009e8:	6422                	ld	s0,8(sp)
    802009ea:	0141                	addi	sp,sp,16
    802009ec:	8082                	ret

00000000802009ee <strchr>:

char*
strchr(const char *s, char c)
{
    802009ee:	1141                	addi	sp,sp,-16
    802009f0:	e422                	sd	s0,8(sp)
    802009f2:	0800                	addi	s0,sp,16
  for(; *s; s++)
    802009f4:	00054783          	lbu	a5,0(a0)
    802009f8:	cb99                	beqz	a5,80200a0e <strchr+0x20>
    if(*s == c)
    802009fa:	00f58763          	beq	a1,a5,80200a08 <strchr+0x1a>
  for(; *s; s++)
    802009fe:	0505                	addi	a0,a0,1
    80200a00:	00054783          	lbu	a5,0(a0)
    80200a04:	fbfd                	bnez	a5,802009fa <strchr+0xc>
      return (char*)s;
  return 0;
    80200a06:	4501                	li	a0,0
    80200a08:	6422                	ld	s0,8(sp)
    80200a0a:	0141                	addi	sp,sp,16
    80200a0c:	8082                	ret
  return 0;
    80200a0e:	4501                	li	a0,0
    80200a10:	bfe5                	j	80200a08 <strchr+0x1a>

0000000080200a12 <main>:

volatile static int started = 0;

void
main(unsigned long hartid, unsigned long dtb_pa)
{
    80200a12:	1101                	addi	sp,sp,-32
    80200a14:	ec06                	sd	ra,24(sp)
    80200a16:	e822                	sd	s0,16(sp)
    80200a18:	1000                	addi	s0,sp,32
    80200a1a:	85aa                	mv	a1,a0
  asm volatile("mv tp, %0" : : "r" (hartid & 0x1));
    80200a1c:	00157793          	andi	a5,a0,1
    80200a20:	823e                	mv	tp,a5
    started = 1;
  }
  else
  {
    // hart 1
    while (started == 0)
    80200a22:	0003b717          	auipc	a4,0x3b
    80200a26:	62e70713          	addi	a4,a4,1582 # 8023c050 <started>
  if (hartid == 0) {
    80200a2a:	c531                	beqz	a0,80200a76 <main+0x64>
    while (started == 0)
    80200a2c:	431c                	lw	a5,0(a4)
    80200a2e:	2781                	sext.w	a5,a5
    80200a30:	dff5                	beqz	a5,80200a2c <main+0x1a>
      ;
    __sync_synchronize();
    80200a32:	0ff0000f          	fence
    #ifdef DEBUG
    printf("hart %d enter main()...\n", hartid);
    80200a36:	00009517          	auipc	a0,0x9
    80200a3a:	9ca50513          	addi	a0,a0,-1590 # 80209400 <digits+0x80>
    80200a3e:	fffff097          	auipc	ra,0xfffff
    80200a42:	750080e7          	jalr	1872(ra) # 8020018e <printf>
    #endif
    kvminithart();
    80200a46:	00000097          	auipc	ra,0x0
    80200a4a:	0f0080e7          	jalr	240(ra) # 80200b36 <kvminithart>
    trapinithart();
    80200a4e:	00002097          	auipc	ra,0x2
    80200a52:	e7c080e7          	jalr	-388(ra) # 802028ca <trapinithart>
    plicinithart();  // ask PLIC for device interrupts
    80200a56:	00007097          	auipc	ra,0x7
    80200a5a:	a16080e7          	jalr	-1514(ra) # 8020746c <plicinithart>
    printf("hart 1 init done\n");
    80200a5e:	00009517          	auipc	a0,0x9
    80200a62:	9da50513          	addi	a0,a0,-1574 # 80209438 <digits+0xb8>
    80200a66:	fffff097          	auipc	ra,0xfffff
    80200a6a:	728080e7          	jalr	1832(ra) # 8020018e <printf>
  }
  scheduler();
    80200a6e:	00001097          	auipc	ra,0x1
    80200a72:	736080e7          	jalr	1846(ra) # 802021a4 <scheduler>
    consoleinit();
    80200a76:	00007097          	auipc	ra,0x7
    80200a7a:	e18080e7          	jalr	-488(ra) # 8020788e <consoleinit>
    printfinit();   // init a lock for printf 
    80200a7e:	00000097          	auipc	ra,0x0
    80200a82:	8f0080e7          	jalr	-1808(ra) # 8020036e <printfinit>
    print_logo();
    80200a86:	00000097          	auipc	ra,0x0
    80200a8a:	91a080e7          	jalr	-1766(ra) # 802003a0 <print_logo>
    printf("hart %d enter main()...\n", hartid);
    80200a8e:	4581                	li	a1,0
    80200a90:	00009517          	auipc	a0,0x9
    80200a94:	97050513          	addi	a0,a0,-1680 # 80209400 <digits+0x80>
    80200a98:	fffff097          	auipc	ra,0xfffff
    80200a9c:	6f6080e7          	jalr	1782(ra) # 8020018e <printf>
    kinit();         // physical page allocator
    80200aa0:	00000097          	auipc	ra,0x0
    80200aa4:	a6e080e7          	jalr	-1426(ra) # 8020050e <kinit>
    kvminit();       // create kernel page table
    80200aa8:	00000097          	auipc	ra,0x0
    80200aac:	2ec080e7          	jalr	748(ra) # 80200d94 <kvminit>
    kvminithart();   // turn on paging
    80200ab0:	00000097          	auipc	ra,0x0
    80200ab4:	086080e7          	jalr	134(ra) # 80200b36 <kvminithart>
    timerinit();     // init a lock for timer
    80200ab8:	00005097          	auipc	ra,0x5
    80200abc:	ec6080e7          	jalr	-314(ra) # 8020597e <timerinit>
    trapinithart();  // install kernel trap vector, including interrupt handler
    80200ac0:	00002097          	auipc	ra,0x2
    80200ac4:	e0a080e7          	jalr	-502(ra) # 802028ca <trapinithart>
    procinit();
    80200ac8:	00001097          	auipc	ra,0x1
    80200acc:	f5e080e7          	jalr	-162(ra) # 80201a26 <procinit>
    plicinit();
    80200ad0:	00007097          	auipc	ra,0x7
    80200ad4:	970080e7          	jalr	-1680(ra) # 80207440 <plicinit>
    plicinithart();
    80200ad8:	00007097          	auipc	ra,0x7
    80200adc:	994080e7          	jalr	-1644(ra) # 8020746c <plicinithart>
    disk_init();
    80200ae0:	00005097          	auipc	ra,0x5
    80200ae4:	f48080e7          	jalr	-184(ra) # 80205a28 <disk_init>
    binit();         // buffer cache
    80200ae8:	00003097          	auipc	ra,0x3
    80200aec:	da0080e7          	jalr	-608(ra) # 80203888 <binit>
    fileinit();      // file table
    80200af0:	00003097          	auipc	ra,0x3
    80200af4:	1b4080e7          	jalr	436(ra) # 80203ca4 <fileinit>
    userinit();      // first user process
    80200af8:	00001097          	auipc	ra,0x1
    80200afc:	2ee080e7          	jalr	750(ra) # 80201de6 <userinit>
    printf("hart 0 init done\n");
    80200b00:	00009517          	auipc	a0,0x9
    80200b04:	92050513          	addi	a0,a0,-1760 # 80209420 <digits+0xa0>
    80200b08:	fffff097          	auipc	ra,0xfffff
    80200b0c:	686080e7          	jalr	1670(ra) # 8020018e <printf>
      unsigned long mask = 1 << i;
    80200b10:	4789                	li	a5,2
    80200b12:	fef43423          	sd	a5,-24(s0)
	SBI_CALL_0(SBI_CLEAR_IPI);
}

static inline void sbi_send_ipi(const unsigned long *hart_mask)
{
	SBI_CALL_1(SBI_SEND_IPI, hart_mask);
    80200b16:	fe840513          	addi	a0,s0,-24
    80200b1a:	4581                	li	a1,0
    80200b1c:	4601                	li	a2,0
    80200b1e:	4681                	li	a3,0
    80200b20:	4891                	li	a7,4
    80200b22:	00000073          	ecall
    __sync_synchronize();
    80200b26:	0ff0000f          	fence
    started = 1;
    80200b2a:	4785                	li	a5,1
    80200b2c:	0003b717          	auipc	a4,0x3b
    80200b30:	52f72223          	sw	a5,1316(a4) # 8023c050 <started>
    80200b34:	bf2d                	j	80200a6e <main+0x5c>

0000000080200b36 <kvminithart>:

// Switch h/w page table register to the kernel's page table,
// and enable paging.
void
kvminithart()
{
    80200b36:	1141                	addi	sp,sp,-16
    80200b38:	e406                	sd	ra,8(sp)
    80200b3a:	e022                	sd	s0,0(sp)
    80200b3c:	0800                	addi	s0,sp,16
  w_satp(MAKE_SATP(kernel_pagetable));
    80200b3e:	0003b797          	auipc	a5,0x3b
    80200b42:	51a7b783          	ld	a5,1306(a5) # 8023c058 <kernel_pagetable>
    80200b46:	83b1                	srli	a5,a5,0xc
    80200b48:	577d                	li	a4,-1
    80200b4a:	177e                	slli	a4,a4,0x3f
    80200b4c:	8fd9                	or	a5,a5,a4
  asm volatile("csrw satp, %0" : : "r" (x));
    80200b4e:	18079073          	csrw	satp,a5
static inline void
sfence_vma()
{
  // the zero, zero means flush all TLB entries.
  // asm volatile("sfence.vma zero, zero");
  asm volatile("sfence.vma");
    80200b52:	12000073          	sfence.vma
  // reg_info();
  sfence_vma();
  #ifdef DEBUG
  printf("kvminithart\n");
    80200b56:	00009517          	auipc	a0,0x9
    80200b5a:	8fa50513          	addi	a0,a0,-1798 # 80209450 <digits+0xd0>
    80200b5e:	fffff097          	auipc	ra,0xfffff
    80200b62:	630080e7          	jalr	1584(ra) # 8020018e <printf>
  #endif
}
    80200b66:	60a2                	ld	ra,8(sp)
    80200b68:	6402                	ld	s0,0(sp)
    80200b6a:	0141                	addi	sp,sp,16
    80200b6c:	8082                	ret

0000000080200b6e <walk>:
//   21..29 -- 9 bits of level-1 index.
//   12..20 -- 9 bits of level-0 index.
//    0..11 -- 12 bits of byte offset within the page.
pte_t *
walk(pagetable_t pagetable, uint64 va, int alloc)
{
    80200b6e:	7139                	addi	sp,sp,-64
    80200b70:	fc06                	sd	ra,56(sp)
    80200b72:	f822                	sd	s0,48(sp)
    80200b74:	f426                	sd	s1,40(sp)
    80200b76:	f04a                	sd	s2,32(sp)
    80200b78:	ec4e                	sd	s3,24(sp)
    80200b7a:	e852                	sd	s4,16(sp)
    80200b7c:	e456                	sd	s5,8(sp)
    80200b7e:	e05a                	sd	s6,0(sp)
    80200b80:	0080                	addi	s0,sp,64
    80200b82:	84aa                	mv	s1,a0
    80200b84:	89ae                	mv	s3,a1
    80200b86:	8ab2                	mv	s5,a2
  
  if(va >= MAXVA)
    80200b88:	57fd                	li	a5,-1
    80200b8a:	83e9                	srli	a5,a5,0x1a
    80200b8c:	4a79                	li	s4,30
    panic("walk");

  for(int level = 2; level > 0; level--) {
    80200b8e:	4b31                	li	s6,12
  if(va >= MAXVA)
    80200b90:	04b7f263          	bgeu	a5,a1,80200bd4 <walk+0x66>
    panic("walk");
    80200b94:	00009517          	auipc	a0,0x9
    80200b98:	8cc50513          	addi	a0,a0,-1844 # 80209460 <digits+0xe0>
    80200b9c:	fffff097          	auipc	ra,0xfffff
    80200ba0:	5a8080e7          	jalr	1448(ra) # 80200144 <panic>
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pde_t*)kalloc()) == NULL)
    80200ba4:	060a8663          	beqz	s5,80200c10 <walk+0xa2>
    80200ba8:	00000097          	auipc	ra,0x0
    80200bac:	9e0080e7          	jalr	-1568(ra) # 80200588 <kalloc>
    80200bb0:	84aa                	mv	s1,a0
    80200bb2:	c529                	beqz	a0,80200bfc <walk+0x8e>
        return NULL;
      memset(pagetable, 0, PGSIZE);
    80200bb4:	6605                	lui	a2,0x1
    80200bb6:	4581                	li	a1,0
    80200bb8:	00000097          	auipc	ra,0x0
    80200bbc:	bd8080e7          	jalr	-1064(ra) # 80200790 <memset>
      *pte = PA2PTE(pagetable) | PTE_V;
    80200bc0:	00c4d793          	srli	a5,s1,0xc
    80200bc4:	07aa                	slli	a5,a5,0xa
    80200bc6:	0017e793          	ori	a5,a5,1
    80200bca:	00f93023          	sd	a5,0(s2)
  for(int level = 2; level > 0; level--) {
    80200bce:	3a5d                	addiw	s4,s4,-9 # ffffffffffffeff7 <ebss_clear+0xffffffff7fda6ff7>
    80200bd0:	036a0063          	beq	s4,s6,80200bf0 <walk+0x82>
    pte_t *pte = &pagetable[PX(level, va)];
    80200bd4:	0149d933          	srl	s2,s3,s4
    80200bd8:	1ff97913          	andi	s2,s2,511
    80200bdc:	090e                	slli	s2,s2,0x3
    80200bde:	9926                	add	s2,s2,s1
    if(*pte & PTE_V) {
    80200be0:	00093483          	ld	s1,0(s2)
    80200be4:	0014f793          	andi	a5,s1,1
    80200be8:	dfd5                	beqz	a5,80200ba4 <walk+0x36>
      pagetable = (pagetable_t)PTE2PA(*pte);
    80200bea:	80a9                	srli	s1,s1,0xa
    80200bec:	04b2                	slli	s1,s1,0xc
    80200bee:	b7c5                	j	80200bce <walk+0x60>
    }
  }
  return &pagetable[PX(0, va)];
    80200bf0:	00c9d513          	srli	a0,s3,0xc
    80200bf4:	1ff57513          	andi	a0,a0,511
    80200bf8:	050e                	slli	a0,a0,0x3
    80200bfa:	9526                	add	a0,a0,s1
}
    80200bfc:	70e2                	ld	ra,56(sp)
    80200bfe:	7442                	ld	s0,48(sp)
    80200c00:	74a2                	ld	s1,40(sp)
    80200c02:	7902                	ld	s2,32(sp)
    80200c04:	69e2                	ld	s3,24(sp)
    80200c06:	6a42                	ld	s4,16(sp)
    80200c08:	6aa2                	ld	s5,8(sp)
    80200c0a:	6b02                	ld	s6,0(sp)
    80200c0c:	6121                	addi	sp,sp,64
    80200c0e:	8082                	ret
        return NULL;
    80200c10:	4501                	li	a0,0
    80200c12:	b7ed                	j	80200bfc <walk+0x8e>

0000000080200c14 <walkaddr>:
walkaddr(pagetable_t pagetable, uint64 va)
{
  pte_t *pte;
  uint64 pa;

  if(va >= MAXVA)
    80200c14:	57fd                	li	a5,-1
    80200c16:	83e9                	srli	a5,a5,0x1a
    80200c18:	00b7f463          	bgeu	a5,a1,80200c20 <walkaddr+0xc>
    return NULL;
    80200c1c:	4501                	li	a0,0
    return NULL;
  if((*pte & PTE_U) == 0)
    return NULL;
  pa = PTE2PA(*pte);
  return pa;
}
    80200c1e:	8082                	ret
{
    80200c20:	1141                	addi	sp,sp,-16
    80200c22:	e406                	sd	ra,8(sp)
    80200c24:	e022                	sd	s0,0(sp)
    80200c26:	0800                	addi	s0,sp,16
  pte = walk(pagetable, va, 0);
    80200c28:	4601                	li	a2,0
    80200c2a:	00000097          	auipc	ra,0x0
    80200c2e:	f44080e7          	jalr	-188(ra) # 80200b6e <walk>
  if(pte == 0)
    80200c32:	c105                	beqz	a0,80200c52 <walkaddr+0x3e>
  if((*pte & PTE_V) == 0)
    80200c34:	611c                	ld	a5,0(a0)
  if((*pte & PTE_U) == 0)
    80200c36:	0117f693          	andi	a3,a5,17
    80200c3a:	4745                	li	a4,17
    return NULL;
    80200c3c:	4501                	li	a0,0
  if((*pte & PTE_U) == 0)
    80200c3e:	00e68663          	beq	a3,a4,80200c4a <walkaddr+0x36>
}
    80200c42:	60a2                	ld	ra,8(sp)
    80200c44:	6402                	ld	s0,0(sp)
    80200c46:	0141                	addi	sp,sp,16
    80200c48:	8082                	ret
  pa = PTE2PA(*pte);
    80200c4a:	83a9                	srli	a5,a5,0xa
    80200c4c:	00c79513          	slli	a0,a5,0xc
  return pa;
    80200c50:	bfcd                	j	80200c42 <walkaddr+0x2e>
    return NULL;
    80200c52:	4501                	li	a0,0
    80200c54:	b7fd                	j	80200c42 <walkaddr+0x2e>

0000000080200c56 <kwalkaddr>:
  return kwalkaddr(kernel_pagetable, va);
}

uint64
kwalkaddr(pagetable_t kpt, uint64 va)
{
    80200c56:	1101                	addi	sp,sp,-32
    80200c58:	ec06                	sd	ra,24(sp)
    80200c5a:	e822                	sd	s0,16(sp)
    80200c5c:	e426                	sd	s1,8(sp)
    80200c5e:	1000                	addi	s0,sp,32
  uint64 off = va % PGSIZE;
    80200c60:	03459793          	slli	a5,a1,0x34
    80200c64:	0347d493          	srli	s1,a5,0x34
  pte_t *pte;
  uint64 pa;
  
  pte = walk(kpt, va, 0);
    80200c68:	4601                	li	a2,0
    80200c6a:	00000097          	auipc	ra,0x0
    80200c6e:	f04080e7          	jalr	-252(ra) # 80200b6e <walk>
  if(pte == 0)
    80200c72:	cd09                	beqz	a0,80200c8c <kwalkaddr+0x36>
    panic("kvmpa");
  if((*pte & PTE_V) == 0)
    80200c74:	6108                	ld	a0,0(a0)
    80200c76:	00157793          	andi	a5,a0,1
    80200c7a:	c38d                	beqz	a5,80200c9c <kwalkaddr+0x46>
    panic("kvmpa");
  pa = PTE2PA(*pte);
    80200c7c:	8129                	srli	a0,a0,0xa
    80200c7e:	0532                	slli	a0,a0,0xc
  return pa+off;
}
    80200c80:	9526                	add	a0,a0,s1
    80200c82:	60e2                	ld	ra,24(sp)
    80200c84:	6442                	ld	s0,16(sp)
    80200c86:	64a2                	ld	s1,8(sp)
    80200c88:	6105                	addi	sp,sp,32
    80200c8a:	8082                	ret
    panic("kvmpa");
    80200c8c:	00008517          	auipc	a0,0x8
    80200c90:	7dc50513          	addi	a0,a0,2012 # 80209468 <digits+0xe8>
    80200c94:	fffff097          	auipc	ra,0xfffff
    80200c98:	4b0080e7          	jalr	1200(ra) # 80200144 <panic>
    panic("kvmpa");
    80200c9c:	00008517          	auipc	a0,0x8
    80200ca0:	7cc50513          	addi	a0,a0,1996 # 80209468 <digits+0xe8>
    80200ca4:	fffff097          	auipc	ra,0xfffff
    80200ca8:	4a0080e7          	jalr	1184(ra) # 80200144 <panic>

0000000080200cac <kvmpa>:
{
    80200cac:	1141                	addi	sp,sp,-16
    80200cae:	e406                	sd	ra,8(sp)
    80200cb0:	e022                	sd	s0,0(sp)
    80200cb2:	0800                	addi	s0,sp,16
    80200cb4:	85aa                	mv	a1,a0
  return kwalkaddr(kernel_pagetable, va);
    80200cb6:	0003b517          	auipc	a0,0x3b
    80200cba:	3a253503          	ld	a0,930(a0) # 8023c058 <kernel_pagetable>
    80200cbe:	00000097          	auipc	ra,0x0
    80200cc2:	f98080e7          	jalr	-104(ra) # 80200c56 <kwalkaddr>
}
    80200cc6:	60a2                	ld	ra,8(sp)
    80200cc8:	6402                	ld	s0,0(sp)
    80200cca:	0141                	addi	sp,sp,16
    80200ccc:	8082                	ret

0000000080200cce <mappages>:
// physical addresses starting at pa. va and size might not
// be page-aligned. Returns 0 on success, -1 if walk() couldn't
// allocate a needed page-table page.
int
mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
{
    80200cce:	715d                	addi	sp,sp,-80
    80200cd0:	e486                	sd	ra,72(sp)
    80200cd2:	e0a2                	sd	s0,64(sp)
    80200cd4:	fc26                	sd	s1,56(sp)
    80200cd6:	f84a                	sd	s2,48(sp)
    80200cd8:	f44e                	sd	s3,40(sp)
    80200cda:	f052                	sd	s4,32(sp)
    80200cdc:	ec56                	sd	s5,24(sp)
    80200cde:	e85a                	sd	s6,16(sp)
    80200ce0:	e45e                	sd	s7,8(sp)
    80200ce2:	0880                	addi	s0,sp,80
    80200ce4:	8aaa                	mv	s5,a0
    80200ce6:	8b3a                	mv	s6,a4
  uint64 a, last;
  pte_t *pte;

  a = PGROUNDDOWN(va);
    80200ce8:	777d                	lui	a4,0xfffff
    80200cea:	00e5f7b3          	and	a5,a1,a4
  last = PGROUNDDOWN(va + size - 1);
    80200cee:	fff60993          	addi	s3,a2,-1 # fff <_entry-0x801ff001>
    80200cf2:	99ae                	add	s3,s3,a1
    80200cf4:	00e9f9b3          	and	s3,s3,a4
  a = PGROUNDDOWN(va);
    80200cf8:	893e                	mv	s2,a5
    80200cfa:	40f68a33          	sub	s4,a3,a5
    if(*pte & PTE_V)
      panic("remap");
    *pte = PA2PTE(pa) | perm | PTE_V;
    if(a == last)
      break;
    a += PGSIZE;
    80200cfe:	6b85                	lui	s7,0x1
    80200d00:	012a04b3          	add	s1,s4,s2
    if((pte = walk(pagetable, a, 1)) == NULL)
    80200d04:	4605                	li	a2,1
    80200d06:	85ca                	mv	a1,s2
    80200d08:	8556                	mv	a0,s5
    80200d0a:	00000097          	auipc	ra,0x0
    80200d0e:	e64080e7          	jalr	-412(ra) # 80200b6e <walk>
    80200d12:	c51d                	beqz	a0,80200d40 <mappages+0x72>
    if(*pte & PTE_V)
    80200d14:	611c                	ld	a5,0(a0)
    80200d16:	8b85                	andi	a5,a5,1
    80200d18:	ef81                	bnez	a5,80200d30 <mappages+0x62>
    *pte = PA2PTE(pa) | perm | PTE_V;
    80200d1a:	80b1                	srli	s1,s1,0xc
    80200d1c:	04aa                	slli	s1,s1,0xa
    80200d1e:	0164e4b3          	or	s1,s1,s6
    80200d22:	0014e493          	ori	s1,s1,1
    80200d26:	e104                	sd	s1,0(a0)
    if(a == last)
    80200d28:	03390863          	beq	s2,s3,80200d58 <mappages+0x8a>
    a += PGSIZE;
    80200d2c:	995e                	add	s2,s2,s7
    if((pte = walk(pagetable, a, 1)) == NULL)
    80200d2e:	bfc9                	j	80200d00 <mappages+0x32>
      panic("remap");
    80200d30:	00008517          	auipc	a0,0x8
    80200d34:	74050513          	addi	a0,a0,1856 # 80209470 <digits+0xf0>
    80200d38:	fffff097          	auipc	ra,0xfffff
    80200d3c:	40c080e7          	jalr	1036(ra) # 80200144 <panic>
      return -1;
    80200d40:	557d                	li	a0,-1
    pa += PGSIZE;
  }
  return 0;
}
    80200d42:	60a6                	ld	ra,72(sp)
    80200d44:	6406                	ld	s0,64(sp)
    80200d46:	74e2                	ld	s1,56(sp)
    80200d48:	7942                	ld	s2,48(sp)
    80200d4a:	79a2                	ld	s3,40(sp)
    80200d4c:	7a02                	ld	s4,32(sp)
    80200d4e:	6ae2                	ld	s5,24(sp)
    80200d50:	6b42                	ld	s6,16(sp)
    80200d52:	6ba2                	ld	s7,8(sp)
    80200d54:	6161                	addi	sp,sp,80
    80200d56:	8082                	ret
  return 0;
    80200d58:	4501                	li	a0,0
    80200d5a:	b7e5                	j	80200d42 <mappages+0x74>

0000000080200d5c <kvmmap>:
{
    80200d5c:	1141                	addi	sp,sp,-16
    80200d5e:	e406                	sd	ra,8(sp)
    80200d60:	e022                	sd	s0,0(sp)
    80200d62:	0800                	addi	s0,sp,16
    80200d64:	8736                	mv	a4,a3
  if(mappages(kernel_pagetable, va, sz, pa, perm) != 0)
    80200d66:	86ae                	mv	a3,a1
    80200d68:	85aa                	mv	a1,a0
    80200d6a:	0003b517          	auipc	a0,0x3b
    80200d6e:	2ee53503          	ld	a0,750(a0) # 8023c058 <kernel_pagetable>
    80200d72:	00000097          	auipc	ra,0x0
    80200d76:	f5c080e7          	jalr	-164(ra) # 80200cce <mappages>
    80200d7a:	e509                	bnez	a0,80200d84 <kvmmap+0x28>
}
    80200d7c:	60a2                	ld	ra,8(sp)
    80200d7e:	6402                	ld	s0,0(sp)
    80200d80:	0141                	addi	sp,sp,16
    80200d82:	8082                	ret
    panic("kvmmap");
    80200d84:	00008517          	auipc	a0,0x8
    80200d88:	6f450513          	addi	a0,a0,1780 # 80209478 <digits+0xf8>
    80200d8c:	fffff097          	auipc	ra,0xfffff
    80200d90:	3b8080e7          	jalr	952(ra) # 80200144 <panic>

0000000080200d94 <kvminit>:
{
    80200d94:	1101                	addi	sp,sp,-32
    80200d96:	ec06                	sd	ra,24(sp)
    80200d98:	e822                	sd	s0,16(sp)
    80200d9a:	e426                	sd	s1,8(sp)
    80200d9c:	1000                	addi	s0,sp,32
  kernel_pagetable = (pagetable_t) kalloc();
    80200d9e:	fffff097          	auipc	ra,0xfffff
    80200da2:	7ea080e7          	jalr	2026(ra) # 80200588 <kalloc>
    80200da6:	0003b717          	auipc	a4,0x3b
    80200daa:	2aa73923          	sd	a0,690(a4) # 8023c058 <kernel_pagetable>
  memset(kernel_pagetable, 0, PGSIZE);
    80200dae:	6605                	lui	a2,0x1
    80200db0:	4581                	li	a1,0
    80200db2:	00000097          	auipc	ra,0x0
    80200db6:	9de080e7          	jalr	-1570(ra) # 80200790 <memset>
  kvmmap(UART_V, UART, PGSIZE, PTE_R | PTE_W);
    80200dba:	4699                	li	a3,6
    80200dbc:	6605                	lui	a2,0x1
    80200dbe:	100005b7          	lui	a1,0x10000
    80200dc2:	3f100513          	li	a0,1009
    80200dc6:	0572                	slli	a0,a0,0x1c
    80200dc8:	00000097          	auipc	ra,0x0
    80200dcc:	f94080e7          	jalr	-108(ra) # 80200d5c <kvmmap>
  kvmmap(VIRTIO0_V, VIRTIO0, PGSIZE, PTE_R | PTE_W);
    80200dd0:	4699                	li	a3,6
    80200dd2:	6605                	lui	a2,0x1
    80200dd4:	100015b7          	lui	a1,0x10001
    80200dd8:	03f10537          	lui	a0,0x3f10
    80200ddc:	0505                	addi	a0,a0,1 # 3f10001 <_entry-0x7c2effff>
    80200dde:	0532                	slli	a0,a0,0xc
    80200de0:	00000097          	auipc	ra,0x0
    80200de4:	f7c080e7          	jalr	-132(ra) # 80200d5c <kvmmap>
  kvmmap(CLINT_V, CLINT, 0x10000, PTE_R | PTE_W);
    80200de8:	4699                	li	a3,6
    80200dea:	6641                	lui	a2,0x10
    80200dec:	020005b7          	lui	a1,0x2000
    80200df0:	01f81537          	lui	a0,0x1f81
    80200df4:	0536                	slli	a0,a0,0xd
    80200df6:	00000097          	auipc	ra,0x0
    80200dfa:	f66080e7          	jalr	-154(ra) # 80200d5c <kvmmap>
  kvmmap(PLIC_V, PLIC, 0x4000, PTE_R | PTE_W);
    80200dfe:	4699                	li	a3,6
    80200e00:	6611                	lui	a2,0x4
    80200e02:	0c0005b7          	lui	a1,0xc000
    80200e06:	00fc3537          	lui	a0,0xfc3
    80200e0a:	053a                	slli	a0,a0,0xe
    80200e0c:	00000097          	auipc	ra,0x0
    80200e10:	f50080e7          	jalr	-176(ra) # 80200d5c <kvmmap>
  kvmmap(PLIC_V + 0x200000, PLIC + 0x200000, 0x4000, PTE_R | PTE_W);
    80200e14:	4699                	li	a3,6
    80200e16:	6611                	lui	a2,0x4
    80200e18:	0c2005b7          	lui	a1,0xc200
    80200e1c:	1f861537          	lui	a0,0x1f861
    80200e20:	0526                	slli	a0,a0,0x9
    80200e22:	00000097          	auipc	ra,0x0
    80200e26:	f3a080e7          	jalr	-198(ra) # 80200d5c <kvmmap>
  kvmmap(KERNBASE, KERNBASE, (uint64)etext - KERNBASE, PTE_R | PTE_X);
    80200e2a:	00032497          	auipc	s1,0x32
    80200e2e:	4ce4b483          	ld	s1,1230(s1) # 802332f8 <_GLOBAL_OFFSET_TABLE_+0x20>
    80200e32:	46a9                	li	a3,10
    80200e34:	bff00613          	li	a2,-1025
    80200e38:	0656                	slli	a2,a2,0x15
    80200e3a:	9626                	add	a2,a2,s1
    80200e3c:	40100593          	li	a1,1025
    80200e40:	05d6                	slli	a1,a1,0x15
    80200e42:	852e                	mv	a0,a1
    80200e44:	00000097          	auipc	ra,0x0
    80200e48:	f18080e7          	jalr	-232(ra) # 80200d5c <kvmmap>
  kvmmap((uint64)etext, (uint64)etext, PHYSTOP - (uint64)etext, PTE_R | PTE_W);
    80200e4c:	4699                	li	a3,6
    80200e4e:	40300613          	li	a2,1027
    80200e52:	0656                	slli	a2,a2,0x15
    80200e54:	8e05                	sub	a2,a2,s1
    80200e56:	85a6                	mv	a1,s1
    80200e58:	8526                	mv	a0,s1
    80200e5a:	00000097          	auipc	ra,0x0
    80200e5e:	f02080e7          	jalr	-254(ra) # 80200d5c <kvmmap>
  kvmmap(TRAMPOLINE, (uint64)trampoline, PGSIZE, PTE_R | PTE_X);
    80200e62:	46a9                	li	a3,10
    80200e64:	6605                	lui	a2,0x1
    80200e66:	00032597          	auipc	a1,0x32
    80200e6a:	47a5b583          	ld	a1,1146(a1) # 802332e0 <_GLOBAL_OFFSET_TABLE_+0x8>
    80200e6e:	04000537          	lui	a0,0x4000
    80200e72:	157d                	addi	a0,a0,-1 # 3ffffff <_entry-0x7c200001>
    80200e74:	0532                	slli	a0,a0,0xc
    80200e76:	00000097          	auipc	ra,0x0
    80200e7a:	ee6080e7          	jalr	-282(ra) # 80200d5c <kvmmap>
  printf("kvminit\n");
    80200e7e:	00008517          	auipc	a0,0x8
    80200e82:	60250513          	addi	a0,a0,1538 # 80209480 <digits+0x100>
    80200e86:	fffff097          	auipc	ra,0xfffff
    80200e8a:	308080e7          	jalr	776(ra) # 8020018e <printf>
}
    80200e8e:	60e2                	ld	ra,24(sp)
    80200e90:	6442                	ld	s0,16(sp)
    80200e92:	64a2                	ld	s1,8(sp)
    80200e94:	6105                	addi	sp,sp,32
    80200e96:	8082                	ret

0000000080200e98 <vmunmap>:
// Remove npages of mappings starting from va. va must be
// page-aligned. The mappings must exist.
// Optionally free the physical memory.
void
vmunmap(pagetable_t pagetable, uint64 va, uint64 npages, int do_free)
{
    80200e98:	715d                	addi	sp,sp,-80
    80200e9a:	e486                	sd	ra,72(sp)
    80200e9c:	e0a2                	sd	s0,64(sp)
    80200e9e:	fc26                	sd	s1,56(sp)
    80200ea0:	f84a                	sd	s2,48(sp)
    80200ea2:	f44e                	sd	s3,40(sp)
    80200ea4:	f052                	sd	s4,32(sp)
    80200ea6:	ec56                	sd	s5,24(sp)
    80200ea8:	e85a                	sd	s6,16(sp)
    80200eaa:	e45e                	sd	s7,8(sp)
    80200eac:	0880                	addi	s0,sp,80
  uint64 a;
  pte_t *pte;

  if((va % PGSIZE) != 0)
    80200eae:	03459793          	slli	a5,a1,0x34
    80200eb2:	e795                	bnez	a5,80200ede <vmunmap+0x46>
    80200eb4:	8a2a                	mv	s4,a0
    80200eb6:	892e                	mv	s2,a1
    80200eb8:	8ab6                	mv	s5,a3
    panic("vmunmap: not aligned");

  for(a = va; a < va + npages*PGSIZE; a += PGSIZE){
    80200eba:	0632                	slli	a2,a2,0xc
    80200ebc:	00b609b3          	add	s3,a2,a1
    if((pte = walk(pagetable, a, 0)) == 0)
      panic("vmunmap: walk");
    if((*pte & PTE_V) == 0)
      panic("vmunmap: not mapped");
    if(PTE_FLAGS(*pte) == PTE_V)
    80200ec0:	4b85                	li	s7,1
  for(a = va; a < va + npages*PGSIZE; a += PGSIZE){
    80200ec2:	6b05                	lui	s6,0x1
    80200ec4:	0735e263          	bltu	a1,s3,80200f28 <vmunmap+0x90>
      uint64 pa = PTE2PA(*pte);
      kfree((void*)pa);
    }
    *pte = 0;
  }
}
    80200ec8:	60a6                	ld	ra,72(sp)
    80200eca:	6406                	ld	s0,64(sp)
    80200ecc:	74e2                	ld	s1,56(sp)
    80200ece:	7942                	ld	s2,48(sp)
    80200ed0:	79a2                	ld	s3,40(sp)
    80200ed2:	7a02                	ld	s4,32(sp)
    80200ed4:	6ae2                	ld	s5,24(sp)
    80200ed6:	6b42                	ld	s6,16(sp)
    80200ed8:	6ba2                	ld	s7,8(sp)
    80200eda:	6161                	addi	sp,sp,80
    80200edc:	8082                	ret
    panic("vmunmap: not aligned");
    80200ede:	00008517          	auipc	a0,0x8
    80200ee2:	5b250513          	addi	a0,a0,1458 # 80209490 <digits+0x110>
    80200ee6:	fffff097          	auipc	ra,0xfffff
    80200eea:	25e080e7          	jalr	606(ra) # 80200144 <panic>
      panic("vmunmap: walk");
    80200eee:	00008517          	auipc	a0,0x8
    80200ef2:	5ba50513          	addi	a0,a0,1466 # 802094a8 <digits+0x128>
    80200ef6:	fffff097          	auipc	ra,0xfffff
    80200efa:	24e080e7          	jalr	590(ra) # 80200144 <panic>
      panic("vmunmap: not mapped");
    80200efe:	00008517          	auipc	a0,0x8
    80200f02:	5ba50513          	addi	a0,a0,1466 # 802094b8 <digits+0x138>
    80200f06:	fffff097          	auipc	ra,0xfffff
    80200f0a:	23e080e7          	jalr	574(ra) # 80200144 <panic>
      panic("vmunmap: not a leaf");
    80200f0e:	00008517          	auipc	a0,0x8
    80200f12:	5c250513          	addi	a0,a0,1474 # 802094d0 <digits+0x150>
    80200f16:	fffff097          	auipc	ra,0xfffff
    80200f1a:	22e080e7          	jalr	558(ra) # 80200144 <panic>
    *pte = 0;
    80200f1e:	0004b023          	sd	zero,0(s1)
  for(a = va; a < va + npages*PGSIZE; a += PGSIZE){
    80200f22:	995a                	add	s2,s2,s6
    80200f24:	fb3972e3          	bgeu	s2,s3,80200ec8 <vmunmap+0x30>
    if((pte = walk(pagetable, a, 0)) == 0)
    80200f28:	4601                	li	a2,0
    80200f2a:	85ca                	mv	a1,s2
    80200f2c:	8552                	mv	a0,s4
    80200f2e:	00000097          	auipc	ra,0x0
    80200f32:	c40080e7          	jalr	-960(ra) # 80200b6e <walk>
    80200f36:	84aa                	mv	s1,a0
    80200f38:	d95d                	beqz	a0,80200eee <vmunmap+0x56>
    if((*pte & PTE_V) == 0)
    80200f3a:	6108                	ld	a0,0(a0)
    80200f3c:	00157793          	andi	a5,a0,1
    80200f40:	dfdd                	beqz	a5,80200efe <vmunmap+0x66>
    if(PTE_FLAGS(*pte) == PTE_V)
    80200f42:	3ff57793          	andi	a5,a0,1023
    80200f46:	fd7784e3          	beq	a5,s7,80200f0e <vmunmap+0x76>
    if(do_free){
    80200f4a:	fc0a8ae3          	beqz	s5,80200f1e <vmunmap+0x86>
      uint64 pa = PTE2PA(*pte);
    80200f4e:	8129                	srli	a0,a0,0xa
      kfree((void*)pa);
    80200f50:	0532                	slli	a0,a0,0xc
    80200f52:	fffff097          	auipc	ra,0xfffff
    80200f56:	4ee080e7          	jalr	1262(ra) # 80200440 <kfree>
    80200f5a:	b7d1                	j	80200f1e <vmunmap+0x86>

0000000080200f5c <uvmcreate>:

// create an empty user page table.
// returns 0 if out of memory.
pagetable_t
uvmcreate()
{
    80200f5c:	1101                	addi	sp,sp,-32
    80200f5e:	ec06                	sd	ra,24(sp)
    80200f60:	e822                	sd	s0,16(sp)
    80200f62:	e426                	sd	s1,8(sp)
    80200f64:	1000                	addi	s0,sp,32
  pagetable_t pagetable;
  pagetable = (pagetable_t) kalloc();
    80200f66:	fffff097          	auipc	ra,0xfffff
    80200f6a:	622080e7          	jalr	1570(ra) # 80200588 <kalloc>
    80200f6e:	84aa                	mv	s1,a0
  if(pagetable == NULL)
    80200f70:	c519                	beqz	a0,80200f7e <uvmcreate+0x22>
    return NULL;
  memset(pagetable, 0, PGSIZE);
    80200f72:	6605                	lui	a2,0x1
    80200f74:	4581                	li	a1,0
    80200f76:	00000097          	auipc	ra,0x0
    80200f7a:	81a080e7          	jalr	-2022(ra) # 80200790 <memset>
  return pagetable;
}
    80200f7e:	8526                	mv	a0,s1
    80200f80:	60e2                	ld	ra,24(sp)
    80200f82:	6442                	ld	s0,16(sp)
    80200f84:	64a2                	ld	s1,8(sp)
    80200f86:	6105                	addi	sp,sp,32
    80200f88:	8082                	ret

0000000080200f8a <uvminit>:
// Load the user initcode into address 0 of pagetable,
// for the very first process.
// sz must be less than a page.
void
uvminit(pagetable_t pagetable, pagetable_t kpagetable, uchar *src, uint sz)
{
    80200f8a:	7139                	addi	sp,sp,-64
    80200f8c:	fc06                	sd	ra,56(sp)
    80200f8e:	f822                	sd	s0,48(sp)
    80200f90:	f426                	sd	s1,40(sp)
    80200f92:	f04a                	sd	s2,32(sp)
    80200f94:	ec4e                	sd	s3,24(sp)
    80200f96:	e852                	sd	s4,16(sp)
    80200f98:	e456                	sd	s5,8(sp)
    80200f9a:	0080                	addi	s0,sp,64
  char *mem;

  if(sz >= PGSIZE)
    80200f9c:	6785                	lui	a5,0x1
    80200f9e:	06f6fe63          	bgeu	a3,a5,8020101a <uvminit+0x90>
    80200fa2:	89aa                	mv	s3,a0
    80200fa4:	8aae                	mv	s5,a1
    80200fa6:	8a32                	mv	s4,a2
    80200fa8:	8936                	mv	s2,a3
    panic("inituvm: more than a page");
  mem = kalloc();
    80200faa:	fffff097          	auipc	ra,0xfffff
    80200fae:	5de080e7          	jalr	1502(ra) # 80200588 <kalloc>
    80200fb2:	84aa                	mv	s1,a0
  memset(mem, 0, PGSIZE);
    80200fb4:	6605                	lui	a2,0x1
    80200fb6:	4581                	li	a1,0
    80200fb8:	fffff097          	auipc	ra,0xfffff
    80200fbc:	7d8080e7          	jalr	2008(ra) # 80200790 <memset>
  mappages(pagetable, 0, PGSIZE, (uint64)mem, PTE_W|PTE_R|PTE_X|PTE_U);
    80200fc0:	4779                	li	a4,30
    80200fc2:	86a6                	mv	a3,s1
    80200fc4:	6605                	lui	a2,0x1
    80200fc6:	4581                	li	a1,0
    80200fc8:	854e                	mv	a0,s3
    80200fca:	00000097          	auipc	ra,0x0
    80200fce:	d04080e7          	jalr	-764(ra) # 80200cce <mappages>
  mappages(pagetable, 0x100000, PGSIZE, 0x100000, PTE_W|PTE_R|PTE_U);
    80200fd2:	4759                	li	a4,22
    80200fd4:	001006b7          	lui	a3,0x100
    80200fd8:	6605                	lui	a2,0x1
    80200fda:	001005b7          	lui	a1,0x100
    80200fde:	854e                	mv	a0,s3
    80200fe0:	00000097          	auipc	ra,0x0
    80200fe4:	cee080e7          	jalr	-786(ra) # 80200cce <mappages>
  mappages(kpagetable, 0, PGSIZE, (uint64)mem, PTE_W|PTE_R|PTE_X);
    80200fe8:	4739                	li	a4,14
    80200fea:	86a6                	mv	a3,s1
    80200fec:	6605                	lui	a2,0x1
    80200fee:	4581                	li	a1,0
    80200ff0:	8556                	mv	a0,s5
    80200ff2:	00000097          	auipc	ra,0x0
    80200ff6:	cdc080e7          	jalr	-804(ra) # 80200cce <mappages>
  memmove(mem, src, sz);
    80200ffa:	864a                	mv	a2,s2
    80200ffc:	85d2                	mv	a1,s4
    80200ffe:	8526                	mv	a0,s1
    80201000:	fffff097          	auipc	ra,0xfffff
    80201004:	7ec080e7          	jalr	2028(ra) # 802007ec <memmove>
}
    80201008:	70e2                	ld	ra,56(sp)
    8020100a:	7442                	ld	s0,48(sp)
    8020100c:	74a2                	ld	s1,40(sp)
    8020100e:	7902                	ld	s2,32(sp)
    80201010:	69e2                	ld	s3,24(sp)
    80201012:	6a42                	ld	s4,16(sp)
    80201014:	6aa2                	ld	s5,8(sp)
    80201016:	6121                	addi	sp,sp,64
    80201018:	8082                	ret
    panic("inituvm: more than a page");
    8020101a:	00008517          	auipc	a0,0x8
    8020101e:	4ce50513          	addi	a0,a0,1230 # 802094e8 <digits+0x168>
    80201022:	fffff097          	auipc	ra,0xfffff
    80201026:	122080e7          	jalr	290(ra) # 80200144 <panic>

000000008020102a <uvmdealloc>:
// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
// need to be less than oldsz.  oldsz can be larger than the actual
// process size.  Returns the new process size.
uint64
uvmdealloc(pagetable_t pagetable, pagetable_t kpagetable, uint64 oldsz, uint64 newsz)
{
    8020102a:	7179                	addi	sp,sp,-48
    8020102c:	f406                	sd	ra,40(sp)
    8020102e:	f022                	sd	s0,32(sp)
    80201030:	ec26                	sd	s1,24(sp)
    80201032:	e84a                	sd	s2,16(sp)
    80201034:	e44e                	sd	s3,8(sp)
    80201036:	e052                	sd	s4,0(sp)
    80201038:	1800                	addi	s0,sp,48
  if(newsz >= oldsz)
    return oldsz;
    8020103a:	8932                	mv	s2,a2
  if(newsz >= oldsz)
    8020103c:	02c6f063          	bgeu	a3,a2,8020105c <uvmdealloc+0x32>
    80201040:	89aa                	mv	s3,a0
    80201042:	852e                	mv	a0,a1
    80201044:	8936                	mv	s2,a3

  if(PGROUNDUP(newsz) < PGROUNDUP(oldsz)){
    80201046:	6785                	lui	a5,0x1
    80201048:	17fd                	addi	a5,a5,-1 # fff <_entry-0x801ff001>
    8020104a:	00f68a33          	add	s4,a3,a5
    8020104e:	777d                	lui	a4,0xfffff
    80201050:	00ea7a33          	and	s4,s4,a4
    80201054:	963e                	add	a2,a2,a5
    80201056:	8e79                	and	a2,a2,a4
    80201058:	00ca6b63          	bltu	s4,a2,8020106e <uvmdealloc+0x44>
    vmunmap(kpagetable, PGROUNDUP(newsz), npages, 0);
    vmunmap(pagetable, PGROUNDUP(newsz), npages, 1);
  }

  return newsz;
}
    8020105c:	854a                	mv	a0,s2
    8020105e:	70a2                	ld	ra,40(sp)
    80201060:	7402                	ld	s0,32(sp)
    80201062:	64e2                	ld	s1,24(sp)
    80201064:	6942                	ld	s2,16(sp)
    80201066:	69a2                	ld	s3,8(sp)
    80201068:	6a02                	ld	s4,0(sp)
    8020106a:	6145                	addi	sp,sp,48
    8020106c:	8082                	ret
    int npages = (PGROUNDUP(oldsz) - PGROUNDUP(newsz)) / PGSIZE;
    8020106e:	414604b3          	sub	s1,a2,s4
    80201072:	80b1                	srli	s1,s1,0xc
    vmunmap(kpagetable, PGROUNDUP(newsz), npages, 0);
    80201074:	2481                	sext.w	s1,s1
    80201076:	4681                	li	a3,0
    80201078:	8626                	mv	a2,s1
    8020107a:	85d2                	mv	a1,s4
    8020107c:	00000097          	auipc	ra,0x0
    80201080:	e1c080e7          	jalr	-484(ra) # 80200e98 <vmunmap>
    vmunmap(pagetable, PGROUNDUP(newsz), npages, 1);
    80201084:	4685                	li	a3,1
    80201086:	8626                	mv	a2,s1
    80201088:	85d2                	mv	a1,s4
    8020108a:	854e                	mv	a0,s3
    8020108c:	00000097          	auipc	ra,0x0
    80201090:	e0c080e7          	jalr	-500(ra) # 80200e98 <vmunmap>
    80201094:	b7e1                	j	8020105c <uvmdealloc+0x32>

0000000080201096 <uvmalloc>:
  if(newsz < oldsz)
    80201096:	0ec6e763          	bltu	a3,a2,80201184 <uvmalloc+0xee>
{
    8020109a:	7139                	addi	sp,sp,-64
    8020109c:	fc06                	sd	ra,56(sp)
    8020109e:	f822                	sd	s0,48(sp)
    802010a0:	f426                	sd	s1,40(sp)
    802010a2:	f04a                	sd	s2,32(sp)
    802010a4:	ec4e                	sd	s3,24(sp)
    802010a6:	e852                	sd	s4,16(sp)
    802010a8:	e456                	sd	s5,8(sp)
    802010aa:	e05a                	sd	s6,0(sp)
    802010ac:	0080                	addi	s0,sp,64
    802010ae:	8a2a                	mv	s4,a0
    802010b0:	8aae                	mv	s5,a1
    802010b2:	8b36                	mv	s6,a3
  oldsz = PGROUNDUP(oldsz);
    802010b4:	6785                	lui	a5,0x1
    802010b6:	17fd                	addi	a5,a5,-1 # fff <_entry-0x801ff001>
    802010b8:	963e                	add	a2,a2,a5
    802010ba:	77fd                	lui	a5,0xfffff
    802010bc:	00f679b3          	and	s3,a2,a5
  for(a = oldsz; a < newsz; a += PGSIZE){
    802010c0:	0cd9f463          	bgeu	s3,a3,80201188 <uvmalloc+0xf2>
    802010c4:	894e                	mv	s2,s3
    mem = kalloc();
    802010c6:	fffff097          	auipc	ra,0xfffff
    802010ca:	4c2080e7          	jalr	1218(ra) # 80200588 <kalloc>
    802010ce:	84aa                	mv	s1,a0
    if(mem == NULL){
    802010d0:	c129                	beqz	a0,80201112 <uvmalloc+0x7c>
    memset(mem, 0, PGSIZE);
    802010d2:	6605                	lui	a2,0x1
    802010d4:	4581                	li	a1,0
    802010d6:	fffff097          	auipc	ra,0xfffff
    802010da:	6ba080e7          	jalr	1722(ra) # 80200790 <memset>
    if (mappages(pagetable, a, PGSIZE, (uint64)mem, PTE_W|PTE_X|PTE_R|PTE_U) != 0) {
    802010de:	4779                	li	a4,30
    802010e0:	86a6                	mv	a3,s1
    802010e2:	6605                	lui	a2,0x1
    802010e4:	85ca                	mv	a1,s2
    802010e6:	8552                	mv	a0,s4
    802010e8:	00000097          	auipc	ra,0x0
    802010ec:	be6080e7          	jalr	-1050(ra) # 80200cce <mappages>
    802010f0:	e521                	bnez	a0,80201138 <uvmalloc+0xa2>
    if (mappages(kpagetable, a, PGSIZE, (uint64)mem, PTE_W|PTE_X|PTE_R) != 0){
    802010f2:	4739                	li	a4,14
    802010f4:	86a6                	mv	a3,s1
    802010f6:	6605                	lui	a2,0x1
    802010f8:	85ca                	mv	a1,s2
    802010fa:	8556                	mv	a0,s5
    802010fc:	00000097          	auipc	ra,0x0
    80201100:	bd2080e7          	jalr	-1070(ra) # 80200cce <mappages>
    80201104:	e929                	bnez	a0,80201156 <uvmalloc+0xc0>
  for(a = oldsz; a < newsz; a += PGSIZE){
    80201106:	6785                	lui	a5,0x1
    80201108:	993e                	add	s2,s2,a5
    8020110a:	fb696ee3          	bltu	s2,s6,802010c6 <uvmalloc+0x30>
  return newsz;
    8020110e:	855a                	mv	a0,s6
    80201110:	a811                	j	80201124 <uvmalloc+0x8e>
      uvmdealloc(pagetable, kpagetable, a, oldsz);
    80201112:	86ce                	mv	a3,s3
    80201114:	864a                	mv	a2,s2
    80201116:	85d6                	mv	a1,s5
    80201118:	8552                	mv	a0,s4
    8020111a:	00000097          	auipc	ra,0x0
    8020111e:	f10080e7          	jalr	-240(ra) # 8020102a <uvmdealloc>
      return 0;
    80201122:	4501                	li	a0,0
}
    80201124:	70e2                	ld	ra,56(sp)
    80201126:	7442                	ld	s0,48(sp)
    80201128:	74a2                	ld	s1,40(sp)
    8020112a:	7902                	ld	s2,32(sp)
    8020112c:	69e2                	ld	s3,24(sp)
    8020112e:	6a42                	ld	s4,16(sp)
    80201130:	6aa2                	ld	s5,8(sp)
    80201132:	6b02                	ld	s6,0(sp)
    80201134:	6121                	addi	sp,sp,64
    80201136:	8082                	ret
      kfree(mem);
    80201138:	8526                	mv	a0,s1
    8020113a:	fffff097          	auipc	ra,0xfffff
    8020113e:	306080e7          	jalr	774(ra) # 80200440 <kfree>
      uvmdealloc(pagetable, kpagetable, a, oldsz);
    80201142:	86ce                	mv	a3,s3
    80201144:	864a                	mv	a2,s2
    80201146:	85d6                	mv	a1,s5
    80201148:	8552                	mv	a0,s4
    8020114a:	00000097          	auipc	ra,0x0
    8020114e:	ee0080e7          	jalr	-288(ra) # 8020102a <uvmdealloc>
      return 0;
    80201152:	4501                	li	a0,0
    80201154:	bfc1                	j	80201124 <uvmalloc+0x8e>
      int npages = (a - oldsz) / PGSIZE;
    80201156:	41390633          	sub	a2,s2,s3
    8020115a:	8231                	srli	a2,a2,0xc
    8020115c:	0006049b          	sext.w	s1,a2
      vmunmap(pagetable, oldsz, npages + 1, 1);   // plus the page allocated above.
    80201160:	4685                	li	a3,1
    80201162:	2605                	addiw	a2,a2,1 # 1001 <_entry-0x801fefff>
    80201164:	85ce                	mv	a1,s3
    80201166:	8552                	mv	a0,s4
    80201168:	00000097          	auipc	ra,0x0
    8020116c:	d30080e7          	jalr	-720(ra) # 80200e98 <vmunmap>
      vmunmap(kpagetable, oldsz, npages, 0);
    80201170:	4681                	li	a3,0
    80201172:	8626                	mv	a2,s1
    80201174:	85ce                	mv	a1,s3
    80201176:	8556                	mv	a0,s5
    80201178:	00000097          	auipc	ra,0x0
    8020117c:	d20080e7          	jalr	-736(ra) # 80200e98 <vmunmap>
      return 0;
    80201180:	4501                	li	a0,0
    80201182:	b74d                	j	80201124 <uvmalloc+0x8e>
    return oldsz;
    80201184:	8532                	mv	a0,a2
}
    80201186:	8082                	ret
  return newsz;
    80201188:	8536                	mv	a0,a3
    8020118a:	bf69                	j	80201124 <uvmalloc+0x8e>

000000008020118c <freewalk>:

// Recursively free page-table pages.
// All leaf mappings must already have been removed.
void
freewalk(pagetable_t pagetable)
{
    8020118c:	7179                	addi	sp,sp,-48
    8020118e:	f406                	sd	ra,40(sp)
    80201190:	f022                	sd	s0,32(sp)
    80201192:	ec26                	sd	s1,24(sp)
    80201194:	e84a                	sd	s2,16(sp)
    80201196:	e44e                	sd	s3,8(sp)
    80201198:	e052                	sd	s4,0(sp)
    8020119a:	1800                	addi	s0,sp,48
    8020119c:	8a2a                	mv	s4,a0
  // there are 2^9 = 512 PTEs in a page table.
  for(int i = 0; i < 512; i++){
    8020119e:	84aa                	mv	s1,a0
    802011a0:	6905                	lui	s2,0x1
    802011a2:	992a                	add	s2,s2,a0
    pte_t pte = pagetable[i];
    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
    802011a4:	4985                	li	s3,1
    802011a6:	a829                	j	802011c0 <freewalk+0x34>
      // this PTE points to a lower-level page table.
      uint64 child = PTE2PA(pte);
    802011a8:	83a9                	srli	a5,a5,0xa
      freewalk((pagetable_t)child);
    802011aa:	00c79513          	slli	a0,a5,0xc
    802011ae:	00000097          	auipc	ra,0x0
    802011b2:	fde080e7          	jalr	-34(ra) # 8020118c <freewalk>
      pagetable[i] = 0;
    802011b6:	0004b023          	sd	zero,0(s1)
  for(int i = 0; i < 512; i++){
    802011ba:	04a1                	addi	s1,s1,8
    802011bc:	03248163          	beq	s1,s2,802011de <freewalk+0x52>
    pte_t pte = pagetable[i];
    802011c0:	609c                	ld	a5,0(s1)
    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
    802011c2:	00f7f713          	andi	a4,a5,15
    802011c6:	ff3701e3          	beq	a4,s3,802011a8 <freewalk+0x1c>
    } else if(pte & PTE_V){
    802011ca:	8b85                	andi	a5,a5,1
    802011cc:	d7fd                	beqz	a5,802011ba <freewalk+0x2e>
      panic("freewalk: leaf");
    802011ce:	00008517          	auipc	a0,0x8
    802011d2:	33a50513          	addi	a0,a0,826 # 80209508 <digits+0x188>
    802011d6:	fffff097          	auipc	ra,0xfffff
    802011da:	f6e080e7          	jalr	-146(ra) # 80200144 <panic>
    }
  }
  kfree((void*)pagetable);
    802011de:	8552                	mv	a0,s4
    802011e0:	fffff097          	auipc	ra,0xfffff
    802011e4:	260080e7          	jalr	608(ra) # 80200440 <kfree>
}
    802011e8:	70a2                	ld	ra,40(sp)
    802011ea:	7402                	ld	s0,32(sp)
    802011ec:	64e2                	ld	s1,24(sp)
    802011ee:	6942                	ld	s2,16(sp)
    802011f0:	69a2                	ld	s3,8(sp)
    802011f2:	6a02                	ld	s4,0(sp)
    802011f4:	6145                	addi	sp,sp,48
    802011f6:	8082                	ret

00000000802011f8 <uvmfree>:

// Free user memory pages,
// then free page-table pages.
void
uvmfree(pagetable_t pagetable, uint64 sz)
{
    802011f8:	1101                	addi	sp,sp,-32
    802011fa:	ec06                	sd	ra,24(sp)
    802011fc:	e822                	sd	s0,16(sp)
    802011fe:	e426                	sd	s1,8(sp)
    80201200:	1000                	addi	s0,sp,32
    80201202:	84aa                	mv	s1,a0
  if(sz > 0)
    80201204:	e999                	bnez	a1,8020121a <uvmfree+0x22>
    vmunmap(pagetable, 0, PGROUNDUP(sz)/PGSIZE, 1);
  freewalk(pagetable);
    80201206:	8526                	mv	a0,s1
    80201208:	00000097          	auipc	ra,0x0
    8020120c:	f84080e7          	jalr	-124(ra) # 8020118c <freewalk>
}
    80201210:	60e2                	ld	ra,24(sp)
    80201212:	6442                	ld	s0,16(sp)
    80201214:	64a2                	ld	s1,8(sp)
    80201216:	6105                	addi	sp,sp,32
    80201218:	8082                	ret
    vmunmap(pagetable, 0, PGROUNDUP(sz)/PGSIZE, 1);
    8020121a:	6785                	lui	a5,0x1
    8020121c:	17fd                	addi	a5,a5,-1 # fff <_entry-0x801ff001>
    8020121e:	95be                	add	a1,a1,a5
    80201220:	4685                	li	a3,1
    80201222:	00c5d613          	srli	a2,a1,0xc
    80201226:	4581                	li	a1,0
    80201228:	00000097          	auipc	ra,0x0
    8020122c:	c70080e7          	jalr	-912(ra) # 80200e98 <vmunmap>
    80201230:	bfd9                	j	80201206 <uvmfree+0xe>

0000000080201232 <uvmcopy>:
  pte_t *pte;
  uint64 pa, i = 0, ki = 0;
  uint flags;
  char *mem;

  while (i < sz){
    80201232:	10068563          	beqz	a3,8020133c <uvmcopy+0x10a>
{
    80201236:	715d                	addi	sp,sp,-80
    80201238:	e486                	sd	ra,72(sp)
    8020123a:	e0a2                	sd	s0,64(sp)
    8020123c:	fc26                	sd	s1,56(sp)
    8020123e:	f84a                	sd	s2,48(sp)
    80201240:	f44e                	sd	s3,40(sp)
    80201242:	f052                	sd	s4,32(sp)
    80201244:	ec56                	sd	s5,24(sp)
    80201246:	e85a                	sd	s6,16(sp)
    80201248:	e45e                	sd	s7,8(sp)
    8020124a:	e062                	sd	s8,0(sp)
    8020124c:	0880                	addi	s0,sp,80
    8020124e:	8baa                	mv	s7,a0
    80201250:	8a2e                	mv	s4,a1
    80201252:	8b32                	mv	s6,a2
    80201254:	8ab6                	mv	s5,a3
  uint64 pa, i = 0, ki = 0;
    80201256:	4981                	li	s3,0
    80201258:	a011                	j	8020125c <uvmcopy+0x2a>
    memmove(mem, (char*)pa, PGSIZE);
    if(mappages(new, i, PGSIZE, (uint64)mem, flags) != 0) {
      kfree(mem);
      goto err;
    }
    i += PGSIZE;
    8020125a:	89e2                	mv	s3,s8
    if((pte = walk(old, i, 0)) == NULL)
    8020125c:	4601                	li	a2,0
    8020125e:	85ce                	mv	a1,s3
    80201260:	855e                	mv	a0,s7
    80201262:	00000097          	auipc	ra,0x0
    80201266:	90c080e7          	jalr	-1780(ra) # 80200b6e <walk>
    8020126a:	c135                	beqz	a0,802012ce <uvmcopy+0x9c>
    if((*pte & PTE_V) == 0)
    8020126c:	6118                	ld	a4,0(a0)
    8020126e:	00177793          	andi	a5,a4,1
    80201272:	c7b5                	beqz	a5,802012de <uvmcopy+0xac>
    pa = PTE2PA(*pte);
    80201274:	00a75593          	srli	a1,a4,0xa
    80201278:	00c59c13          	slli	s8,a1,0xc
    flags = PTE_FLAGS(*pte);
    8020127c:	3ff77493          	andi	s1,a4,1023
    if((mem = kalloc()) == NULL)
    80201280:	fffff097          	auipc	ra,0xfffff
    80201284:	308080e7          	jalr	776(ra) # 80200588 <kalloc>
    80201288:	892a                	mv	s2,a0
    8020128a:	c92d                	beqz	a0,802012fc <uvmcopy+0xca>
    memmove(mem, (char*)pa, PGSIZE);
    8020128c:	6605                	lui	a2,0x1
    8020128e:	85e2                	mv	a1,s8
    80201290:	fffff097          	auipc	ra,0xfffff
    80201294:	55c080e7          	jalr	1372(ra) # 802007ec <memmove>
    if(mappages(new, i, PGSIZE, (uint64)mem, flags) != 0) {
    80201298:	2481                	sext.w	s1,s1
    8020129a:	8726                	mv	a4,s1
    8020129c:	86ca                	mv	a3,s2
    8020129e:	6605                	lui	a2,0x1
    802012a0:	85ce                	mv	a1,s3
    802012a2:	8552                	mv	a0,s4
    802012a4:	00000097          	auipc	ra,0x0
    802012a8:	a2a080e7          	jalr	-1494(ra) # 80200cce <mappages>
    802012ac:	e129                	bnez	a0,802012ee <uvmcopy+0xbc>
    i += PGSIZE;
    802012ae:	6c05                	lui	s8,0x1
    802012b0:	9c4e                	add	s8,s8,s3
    if(mappages(knew, ki, PGSIZE, (uint64)mem, flags & ~PTE_U) != 0){
    802012b2:	3ef4f713          	andi	a4,s1,1007
    802012b6:	86ca                	mv	a3,s2
    802012b8:	6605                	lui	a2,0x1
    802012ba:	85ce                	mv	a1,s3
    802012bc:	855a                	mv	a0,s6
    802012be:	00000097          	auipc	ra,0x0
    802012c2:	a10080e7          	jalr	-1520(ra) # 80200cce <mappages>
    802012c6:	ed05                	bnez	a0,802012fe <uvmcopy+0xcc>
  while (i < sz){
    802012c8:	f95c69e3          	bltu	s8,s5,8020125a <uvmcopy+0x28>
    802012cc:	a8a1                	j	80201324 <uvmcopy+0xf2>
      panic("uvmcopy: pte should exist");
    802012ce:	00008517          	auipc	a0,0x8
    802012d2:	24a50513          	addi	a0,a0,586 # 80209518 <digits+0x198>
    802012d6:	fffff097          	auipc	ra,0xfffff
    802012da:	e6e080e7          	jalr	-402(ra) # 80200144 <panic>
      panic("uvmcopy: page not present");
    802012de:	00008517          	auipc	a0,0x8
    802012e2:	25a50513          	addi	a0,a0,602 # 80209538 <digits+0x1b8>
    802012e6:	fffff097          	auipc	ra,0xfffff
    802012ea:	e5e080e7          	jalr	-418(ra) # 80200144 <panic>
      kfree(mem);
    802012ee:	854a                	mv	a0,s2
    802012f0:	fffff097          	auipc	ra,0xfffff
    802012f4:	150080e7          	jalr	336(ra) # 80200440 <kfree>
      goto err;
    802012f8:	8c4e                	mv	s8,s3
    802012fa:	a011                	j	802012fe <uvmcopy+0xcc>
    802012fc:	8c4e                	mv	s8,s3
    ki += PGSIZE;
  }
  return 0;

 err:
  vmunmap(knew, 0, ki / PGSIZE, 0);
    802012fe:	4681                	li	a3,0
    80201300:	00c9d613          	srli	a2,s3,0xc
    80201304:	4581                	li	a1,0
    80201306:	855a                	mv	a0,s6
    80201308:	00000097          	auipc	ra,0x0
    8020130c:	b90080e7          	jalr	-1136(ra) # 80200e98 <vmunmap>
  vmunmap(new, 0, i / PGSIZE, 1);
    80201310:	4685                	li	a3,1
    80201312:	00cc5613          	srli	a2,s8,0xc
    80201316:	4581                	li	a1,0
    80201318:	8552                	mv	a0,s4
    8020131a:	00000097          	auipc	ra,0x0
    8020131e:	b7e080e7          	jalr	-1154(ra) # 80200e98 <vmunmap>
  return -1;
    80201322:	557d                	li	a0,-1
}
    80201324:	60a6                	ld	ra,72(sp)
    80201326:	6406                	ld	s0,64(sp)
    80201328:	74e2                	ld	s1,56(sp)
    8020132a:	7942                	ld	s2,48(sp)
    8020132c:	79a2                	ld	s3,40(sp)
    8020132e:	7a02                	ld	s4,32(sp)
    80201330:	6ae2                	ld	s5,24(sp)
    80201332:	6b42                	ld	s6,16(sp)
    80201334:	6ba2                	ld	s7,8(sp)
    80201336:	6c02                	ld	s8,0(sp)
    80201338:	6161                	addi	sp,sp,80
    8020133a:	8082                	ret
  return 0;
    8020133c:	4501                	li	a0,0
}
    8020133e:	8082                	ret

0000000080201340 <uvmclear>:

// mark a PTE invalid for user access.
// used by exec for the user stack guard page.
void
uvmclear(pagetable_t pagetable, uint64 va)
{
    80201340:	1141                	addi	sp,sp,-16
    80201342:	e406                	sd	ra,8(sp)
    80201344:	e022                	sd	s0,0(sp)
    80201346:	0800                	addi	s0,sp,16
  pte_t *pte;
  
  pte = walk(pagetable, va, 0);
    80201348:	4601                	li	a2,0
    8020134a:	00000097          	auipc	ra,0x0
    8020134e:	824080e7          	jalr	-2012(ra) # 80200b6e <walk>
  if(pte == NULL)
    80201352:	c901                	beqz	a0,80201362 <uvmclear+0x22>
    panic("uvmclear");
  *pte &= ~PTE_U;
    80201354:	611c                	ld	a5,0(a0)
    80201356:	9bbd                	andi	a5,a5,-17
    80201358:	e11c                	sd	a5,0(a0)
}
    8020135a:	60a2                	ld	ra,8(sp)
    8020135c:	6402                	ld	s0,0(sp)
    8020135e:	0141                	addi	sp,sp,16
    80201360:	8082                	ret
    panic("uvmclear");
    80201362:	00008517          	auipc	a0,0x8
    80201366:	1f650513          	addi	a0,a0,502 # 80209558 <digits+0x1d8>
    8020136a:	fffff097          	auipc	ra,0xfffff
    8020136e:	dda080e7          	jalr	-550(ra) # 80200144 <panic>

0000000080201372 <copyout>:
int
copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
{
  uint64 n, va0, pa0;

  while(len > 0){
    80201372:	c6bd                	beqz	a3,802013e0 <copyout+0x6e>
{
    80201374:	715d                	addi	sp,sp,-80
    80201376:	e486                	sd	ra,72(sp)
    80201378:	e0a2                	sd	s0,64(sp)
    8020137a:	fc26                	sd	s1,56(sp)
    8020137c:	f84a                	sd	s2,48(sp)
    8020137e:	f44e                	sd	s3,40(sp)
    80201380:	f052                	sd	s4,32(sp)
    80201382:	ec56                	sd	s5,24(sp)
    80201384:	e85a                	sd	s6,16(sp)
    80201386:	e45e                	sd	s7,8(sp)
    80201388:	e062                	sd	s8,0(sp)
    8020138a:	0880                	addi	s0,sp,80
    8020138c:	8b2a                	mv	s6,a0
    8020138e:	8c2e                	mv	s8,a1
    80201390:	8a32                	mv	s4,a2
    80201392:	89b6                	mv	s3,a3
    va0 = PGROUNDDOWN(dstva);
    80201394:	7bfd                	lui	s7,0xfffff
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == NULL)
      return -1;
    n = PGSIZE - (dstva - va0);
    80201396:	6a85                	lui	s5,0x1
    80201398:	a015                	j	802013bc <copyout+0x4a>
    if(n > len)
      n = len;
    memmove((void *)(pa0 + (dstva - va0)), src, n);
    8020139a:	9562                	add	a0,a0,s8
    8020139c:	0004861b          	sext.w	a2,s1
    802013a0:	85d2                	mv	a1,s4
    802013a2:	41250533          	sub	a0,a0,s2
    802013a6:	fffff097          	auipc	ra,0xfffff
    802013aa:	446080e7          	jalr	1094(ra) # 802007ec <memmove>

    len -= n;
    802013ae:	409989b3          	sub	s3,s3,s1
    src += n;
    802013b2:	9a26                	add	s4,s4,s1
    dstva = va0 + PGSIZE;
    802013b4:	01590c33          	add	s8,s2,s5
  while(len > 0){
    802013b8:	02098263          	beqz	s3,802013dc <copyout+0x6a>
    va0 = PGROUNDDOWN(dstva);
    802013bc:	017c7933          	and	s2,s8,s7
    pa0 = walkaddr(pagetable, va0);
    802013c0:	85ca                	mv	a1,s2
    802013c2:	855a                	mv	a0,s6
    802013c4:	00000097          	auipc	ra,0x0
    802013c8:	850080e7          	jalr	-1968(ra) # 80200c14 <walkaddr>
    if(pa0 == NULL)
    802013cc:	cd01                	beqz	a0,802013e4 <copyout+0x72>
    n = PGSIZE - (dstva - va0);
    802013ce:	418904b3          	sub	s1,s2,s8
    802013d2:	94d6                	add	s1,s1,s5
    802013d4:	fc99f3e3          	bgeu	s3,s1,8020139a <copyout+0x28>
    802013d8:	84ce                	mv	s1,s3
    802013da:	b7c1                	j	8020139a <copyout+0x28>
  }
  return 0;
    802013dc:	4501                	li	a0,0
    802013de:	a021                	j	802013e6 <copyout+0x74>
    802013e0:	4501                	li	a0,0
}
    802013e2:	8082                	ret
      return -1;
    802013e4:	557d                	li	a0,-1
}
    802013e6:	60a6                	ld	ra,72(sp)
    802013e8:	6406                	ld	s0,64(sp)
    802013ea:	74e2                	ld	s1,56(sp)
    802013ec:	7942                	ld	s2,48(sp)
    802013ee:	79a2                	ld	s3,40(sp)
    802013f0:	7a02                	ld	s4,32(sp)
    802013f2:	6ae2                	ld	s5,24(sp)
    802013f4:	6b42                	ld	s6,16(sp)
    802013f6:	6ba2                	ld	s7,8(sp)
    802013f8:	6c02                	ld	s8,0(sp)
    802013fa:	6161                	addi	sp,sp,80
    802013fc:	8082                	ret

00000000802013fe <copyout2>:

int
copyout2(uint64 dstva, char *src, uint64 len)
{
    802013fe:	7179                	addi	sp,sp,-48
    80201400:	f406                	sd	ra,40(sp)
    80201402:	f022                	sd	s0,32(sp)
    80201404:	ec26                	sd	s1,24(sp)
    80201406:	e84a                	sd	s2,16(sp)
    80201408:	e44e                	sd	s3,8(sp)
    8020140a:	1800                	addi	s0,sp,48
    8020140c:	84aa                	mv	s1,a0
    8020140e:	89ae                	mv	s3,a1
    80201410:	8932                	mv	s2,a2
  uint64 sz = myproc()->sz;
    80201412:	00000097          	auipc	ra,0x0
    80201416:	6c6080e7          	jalr	1734(ra) # 80201ad8 <myproc>
    8020141a:	653c                	ld	a5,72(a0)
  if (dstva + len > sz || dstva >= sz) {
    8020141c:	01248733          	add	a4,s1,s2
    80201420:	02e7e463          	bltu	a5,a4,80201448 <copyout2+0x4a>
    80201424:	02f4f463          	bgeu	s1,a5,8020144c <copyout2+0x4e>
    return -1;
  }
  memmove((void *)dstva, src, len);
    80201428:	0009061b          	sext.w	a2,s2
    8020142c:	85ce                	mv	a1,s3
    8020142e:	8526                	mv	a0,s1
    80201430:	fffff097          	auipc	ra,0xfffff
    80201434:	3bc080e7          	jalr	956(ra) # 802007ec <memmove>
  return 0;
    80201438:	4501                	li	a0,0
}
    8020143a:	70a2                	ld	ra,40(sp)
    8020143c:	7402                	ld	s0,32(sp)
    8020143e:	64e2                	ld	s1,24(sp)
    80201440:	6942                	ld	s2,16(sp)
    80201442:	69a2                	ld	s3,8(sp)
    80201444:	6145                	addi	sp,sp,48
    80201446:	8082                	ret
    return -1;
    80201448:	557d                	li	a0,-1
    8020144a:	bfc5                	j	8020143a <copyout2+0x3c>
    8020144c:	557d                	li	a0,-1
    8020144e:	b7f5                	j	8020143a <copyout2+0x3c>

0000000080201450 <copyin>:
int
copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  uint64 n, va0, pa0;

  while(len > 0){
    80201450:	caa5                	beqz	a3,802014c0 <copyin+0x70>
{
    80201452:	715d                	addi	sp,sp,-80
    80201454:	e486                	sd	ra,72(sp)
    80201456:	e0a2                	sd	s0,64(sp)
    80201458:	fc26                	sd	s1,56(sp)
    8020145a:	f84a                	sd	s2,48(sp)
    8020145c:	f44e                	sd	s3,40(sp)
    8020145e:	f052                	sd	s4,32(sp)
    80201460:	ec56                	sd	s5,24(sp)
    80201462:	e85a                	sd	s6,16(sp)
    80201464:	e45e                	sd	s7,8(sp)
    80201466:	e062                	sd	s8,0(sp)
    80201468:	0880                	addi	s0,sp,80
    8020146a:	8b2a                	mv	s6,a0
    8020146c:	8a2e                	mv	s4,a1
    8020146e:	8c32                	mv	s8,a2
    80201470:	89b6                	mv	s3,a3
    va0 = PGROUNDDOWN(srcva);
    80201472:	7bfd                	lui	s7,0xfffff
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == NULL)
      return -1;
    n = PGSIZE - (srcva - va0);
    80201474:	6a85                	lui	s5,0x1
    80201476:	a01d                	j	8020149c <copyin+0x4c>
    if(n > len)
      n = len;
    memmove(dst, (void *)(pa0 + (srcva - va0)), n);
    80201478:	018505b3          	add	a1,a0,s8
    8020147c:	0004861b          	sext.w	a2,s1
    80201480:	412585b3          	sub	a1,a1,s2
    80201484:	8552                	mv	a0,s4
    80201486:	fffff097          	auipc	ra,0xfffff
    8020148a:	366080e7          	jalr	870(ra) # 802007ec <memmove>

    len -= n;
    8020148e:	409989b3          	sub	s3,s3,s1
    dst += n;
    80201492:	9a26                	add	s4,s4,s1
    srcva = va0 + PGSIZE;
    80201494:	01590c33          	add	s8,s2,s5
  while(len > 0){
    80201498:	02098263          	beqz	s3,802014bc <copyin+0x6c>
    va0 = PGROUNDDOWN(srcva);
    8020149c:	017c7933          	and	s2,s8,s7
    pa0 = walkaddr(pagetable, va0);
    802014a0:	85ca                	mv	a1,s2
    802014a2:	855a                	mv	a0,s6
    802014a4:	fffff097          	auipc	ra,0xfffff
    802014a8:	770080e7          	jalr	1904(ra) # 80200c14 <walkaddr>
    if(pa0 == NULL)
    802014ac:	cd01                	beqz	a0,802014c4 <copyin+0x74>
    n = PGSIZE - (srcva - va0);
    802014ae:	418904b3          	sub	s1,s2,s8
    802014b2:	94d6                	add	s1,s1,s5
    802014b4:	fc99f2e3          	bgeu	s3,s1,80201478 <copyin+0x28>
    802014b8:	84ce                	mv	s1,s3
    802014ba:	bf7d                	j	80201478 <copyin+0x28>
  }
  return 0;
    802014bc:	4501                	li	a0,0
    802014be:	a021                	j	802014c6 <copyin+0x76>
    802014c0:	4501                	li	a0,0
}
    802014c2:	8082                	ret
      return -1;
    802014c4:	557d                	li	a0,-1
}
    802014c6:	60a6                	ld	ra,72(sp)
    802014c8:	6406                	ld	s0,64(sp)
    802014ca:	74e2                	ld	s1,56(sp)
    802014cc:	7942                	ld	s2,48(sp)
    802014ce:	79a2                	ld	s3,40(sp)
    802014d0:	7a02                	ld	s4,32(sp)
    802014d2:	6ae2                	ld	s5,24(sp)
    802014d4:	6b42                	ld	s6,16(sp)
    802014d6:	6ba2                	ld	s7,8(sp)
    802014d8:	6c02                	ld	s8,0(sp)
    802014da:	6161                	addi	sp,sp,80
    802014dc:	8082                	ret

00000000802014de <copyin2>:

int
copyin2(char *dst, uint64 srcva, uint64 len)
{
    802014de:	7179                	addi	sp,sp,-48
    802014e0:	f406                	sd	ra,40(sp)
    802014e2:	f022                	sd	s0,32(sp)
    802014e4:	ec26                	sd	s1,24(sp)
    802014e6:	e84a                	sd	s2,16(sp)
    802014e8:	e44e                	sd	s3,8(sp)
    802014ea:	1800                	addi	s0,sp,48
    802014ec:	89aa                	mv	s3,a0
    802014ee:	84ae                	mv	s1,a1
    802014f0:	8932                	mv	s2,a2
  uint64 sz = myproc()->sz;
    802014f2:	00000097          	auipc	ra,0x0
    802014f6:	5e6080e7          	jalr	1510(ra) # 80201ad8 <myproc>
    802014fa:	653c                	ld	a5,72(a0)
  if (srcva + len > sz || srcva >= sz) {
    802014fc:	01248733          	add	a4,s1,s2
    80201500:	02e7e463          	bltu	a5,a4,80201528 <copyin2+0x4a>
    80201504:	02f4f463          	bgeu	s1,a5,8020152c <copyin2+0x4e>
    return -1;
  }
  memmove(dst, (void *)srcva, len);
    80201508:	0009061b          	sext.w	a2,s2
    8020150c:	85a6                	mv	a1,s1
    8020150e:	854e                	mv	a0,s3
    80201510:	fffff097          	auipc	ra,0xfffff
    80201514:	2dc080e7          	jalr	732(ra) # 802007ec <memmove>
  return 0;
    80201518:	4501                	li	a0,0
}
    8020151a:	70a2                	ld	ra,40(sp)
    8020151c:	7402                	ld	s0,32(sp)
    8020151e:	64e2                	ld	s1,24(sp)
    80201520:	6942                	ld	s2,16(sp)
    80201522:	69a2                	ld	s3,8(sp)
    80201524:	6145                	addi	sp,sp,48
    80201526:	8082                	ret
    return -1;
    80201528:	557d                	li	a0,-1
    8020152a:	bfc5                	j	8020151a <copyin2+0x3c>
    8020152c:	557d                	li	a0,-1
    8020152e:	b7f5                	j	8020151a <copyin2+0x3c>

0000000080201530 <copyinstr>:
copyinstr(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
  uint64 n, va0, pa0;
  int got_null = 0;

  while(got_null == 0 && max > 0){
    80201530:	c2dd                	beqz	a3,802015d6 <copyinstr+0xa6>
{
    80201532:	715d                	addi	sp,sp,-80
    80201534:	e486                	sd	ra,72(sp)
    80201536:	e0a2                	sd	s0,64(sp)
    80201538:	fc26                	sd	s1,56(sp)
    8020153a:	f84a                	sd	s2,48(sp)
    8020153c:	f44e                	sd	s3,40(sp)
    8020153e:	f052                	sd	s4,32(sp)
    80201540:	ec56                	sd	s5,24(sp)
    80201542:	e85a                	sd	s6,16(sp)
    80201544:	e45e                	sd	s7,8(sp)
    80201546:	0880                	addi	s0,sp,80
    80201548:	8a2a                	mv	s4,a0
    8020154a:	8b2e                	mv	s6,a1
    8020154c:	8bb2                	mv	s7,a2
    8020154e:	84b6                	mv	s1,a3
    va0 = PGROUNDDOWN(srcva);
    80201550:	7afd                	lui	s5,0xfffff
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == NULL)
      return -1;
    n = PGSIZE - (srcva - va0);
    80201552:	6985                	lui	s3,0x1
    80201554:	a02d                	j	8020157e <copyinstr+0x4e>
      n = max;

    char *p = (char *) (pa0 + (srcva - va0));
    while(n > 0){
      if(*p == '\0'){
        *dst = '\0';
    80201556:	00078023          	sb	zero,0(a5)
    8020155a:	4785                	li	a5,1
      dst++;
    }

    srcva = va0 + PGSIZE;
  }
  if(got_null){
    8020155c:	37fd                	addiw	a5,a5,-1
    8020155e:	0007851b          	sext.w	a0,a5
    return 0;
  } else {
    return -1;
  }
}
    80201562:	60a6                	ld	ra,72(sp)
    80201564:	6406                	ld	s0,64(sp)
    80201566:	74e2                	ld	s1,56(sp)
    80201568:	7942                	ld	s2,48(sp)
    8020156a:	79a2                	ld	s3,40(sp)
    8020156c:	7a02                	ld	s4,32(sp)
    8020156e:	6ae2                	ld	s5,24(sp)
    80201570:	6b42                	ld	s6,16(sp)
    80201572:	6ba2                	ld	s7,8(sp)
    80201574:	6161                	addi	sp,sp,80
    80201576:	8082                	ret
    srcva = va0 + PGSIZE;
    80201578:	01390bb3          	add	s7,s2,s3
  while(got_null == 0 && max > 0){
    8020157c:	c8a9                	beqz	s1,802015ce <copyinstr+0x9e>
    va0 = PGROUNDDOWN(srcva);
    8020157e:	015bf933          	and	s2,s7,s5
    pa0 = walkaddr(pagetable, va0);
    80201582:	85ca                	mv	a1,s2
    80201584:	8552                	mv	a0,s4
    80201586:	fffff097          	auipc	ra,0xfffff
    8020158a:	68e080e7          	jalr	1678(ra) # 80200c14 <walkaddr>
    if(pa0 == NULL)
    8020158e:	c131                	beqz	a0,802015d2 <copyinstr+0xa2>
    n = PGSIZE - (srcva - va0);
    80201590:	417906b3          	sub	a3,s2,s7
    80201594:	96ce                	add	a3,a3,s3
    80201596:	00d4f363          	bgeu	s1,a3,8020159c <copyinstr+0x6c>
    8020159a:	86a6                	mv	a3,s1
    char *p = (char *) (pa0 + (srcva - va0));
    8020159c:	955e                	add	a0,a0,s7
    8020159e:	41250533          	sub	a0,a0,s2
    while(n > 0){
    802015a2:	daf9                	beqz	a3,80201578 <copyinstr+0x48>
    802015a4:	87da                	mv	a5,s6
      if(*p == '\0'){
    802015a6:	41650633          	sub	a2,a0,s6
    802015aa:	fff48593          	addi	a1,s1,-1
    802015ae:	95da                	add	a1,a1,s6
    while(n > 0){
    802015b0:	96da                	add	a3,a3,s6
      if(*p == '\0'){
    802015b2:	00f60733          	add	a4,a2,a5
    802015b6:	00074703          	lbu	a4,0(a4) # fffffffffffff000 <ebss_clear+0xffffffff7fda7000>
    802015ba:	df51                	beqz	a4,80201556 <copyinstr+0x26>
        *dst = *p;
    802015bc:	00e78023          	sb	a4,0(a5)
      --max;
    802015c0:	40f584b3          	sub	s1,a1,a5
      dst++;
    802015c4:	0785                	addi	a5,a5,1
    while(n > 0){
    802015c6:	fed796e3          	bne	a5,a3,802015b2 <copyinstr+0x82>
      dst++;
    802015ca:	8b3e                	mv	s6,a5
    802015cc:	b775                	j	80201578 <copyinstr+0x48>
    802015ce:	4781                	li	a5,0
    802015d0:	b771                	j	8020155c <copyinstr+0x2c>
      return -1;
    802015d2:	557d                	li	a0,-1
    802015d4:	b779                	j	80201562 <copyinstr+0x32>
  int got_null = 0;
    802015d6:	4781                	li	a5,0
  if(got_null){
    802015d8:	37fd                	addiw	a5,a5,-1
    802015da:	0007851b          	sext.w	a0,a5
}
    802015de:	8082                	ret

00000000802015e0 <copyinstr2>:

int
copyinstr2(char *dst, uint64 srcva, uint64 max)
{
    802015e0:	7179                	addi	sp,sp,-48
    802015e2:	f406                	sd	ra,40(sp)
    802015e4:	f022                	sd	s0,32(sp)
    802015e6:	ec26                	sd	s1,24(sp)
    802015e8:	e84a                	sd	s2,16(sp)
    802015ea:	e44e                	sd	s3,8(sp)
    802015ec:	1800                	addi	s0,sp,48
    802015ee:	89aa                	mv	s3,a0
    802015f0:	84ae                	mv	s1,a1
    802015f2:	8932                	mv	s2,a2
  int got_null = 0;
  uint64 sz = myproc()->sz;
    802015f4:	00000097          	auipc	ra,0x0
    802015f8:	4e4080e7          	jalr	1252(ra) # 80201ad8 <myproc>
    802015fc:	6534                	ld	a3,72(a0)
  while(srcva < sz && max > 0){
    802015fe:	04d4f363          	bgeu	s1,a3,80201644 <copyinstr2+0x64>
    80201602:	04090363          	beqz	s2,80201648 <copyinstr2+0x68>
    80201606:	01298633          	add	a2,s3,s2
    8020160a:	8e85                	sub	a3,a3,s1
    8020160c:	96ce                	add	a3,a3,s3
    8020160e:	87ce                	mv	a5,s3
    char *p = (char *)srcva;
    if(*p == '\0'){
    80201610:	413485b3          	sub	a1,s1,s3
    80201614:	00b78733          	add	a4,a5,a1
    80201618:	00074703          	lbu	a4,0(a4)
    8020161c:	cb11                	beqz	a4,80201630 <copyinstr2+0x50>
      *dst = '\0';
      got_null = 1;
      break;
    } else {
      *dst = *p;
    8020161e:	00e78023          	sb	a4,0(a5)
    }
    --max;
    srcva++;
    dst++;
    80201622:	0785                	addi	a5,a5,1
  while(srcva < sz && max > 0){
    80201624:	02d78463          	beq	a5,a3,8020164c <copyinstr2+0x6c>
    80201628:	fec796e3          	bne	a5,a2,80201614 <copyinstr2+0x34>
  }
  if(got_null){
    return 0;
  } else {
    return -1;
    8020162c:	557d                	li	a0,-1
    8020162e:	a021                	j	80201636 <copyinstr2+0x56>
      *dst = '\0';
    80201630:	00078023          	sb	zero,0(a5)
    return 0;
    80201634:	4501                	li	a0,0
  }
}
    80201636:	70a2                	ld	ra,40(sp)
    80201638:	7402                	ld	s0,32(sp)
    8020163a:	64e2                	ld	s1,24(sp)
    8020163c:	6942                	ld	s2,16(sp)
    8020163e:	69a2                	ld	s3,8(sp)
    80201640:	6145                	addi	sp,sp,48
    80201642:	8082                	ret
    return -1;
    80201644:	557d                	li	a0,-1
    80201646:	bfc5                	j	80201636 <copyinstr2+0x56>
    80201648:	557d                	li	a0,-1
    8020164a:	b7f5                	j	80201636 <copyinstr2+0x56>
    8020164c:	557d                	li	a0,-1
    8020164e:	b7e5                	j	80201636 <copyinstr2+0x56>

0000000080201650 <kfreewalk>:
}

// only free page table, not physical pages
void
kfreewalk(pagetable_t kpt)
{
    80201650:	7179                	addi	sp,sp,-48
    80201652:	f406                	sd	ra,40(sp)
    80201654:	f022                	sd	s0,32(sp)
    80201656:	ec26                	sd	s1,24(sp)
    80201658:	e84a                	sd	s2,16(sp)
    8020165a:	e44e                	sd	s3,8(sp)
    8020165c:	e052                	sd	s4,0(sp)
    8020165e:	1800                	addi	s0,sp,48
    80201660:	8a2a                	mv	s4,a0
  for (int i = 0; i < 512; i++) {
    80201662:	84aa                	mv	s1,a0
    80201664:	6905                	lui	s2,0x1
    80201666:	992a                	add	s2,s2,a0
    pte_t pte = kpt[i];
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
    80201668:	4985                	li	s3,1
    8020166a:	a829                	j	80201684 <kfreewalk+0x34>
      kfreewalk((pagetable_t) PTE2PA(pte));
    8020166c:	83a9                	srli	a5,a5,0xa
    8020166e:	00c79513          	slli	a0,a5,0xc
    80201672:	00000097          	auipc	ra,0x0
    80201676:	fde080e7          	jalr	-34(ra) # 80201650 <kfreewalk>
      kpt[i] = 0;
    8020167a:	0004b023          	sd	zero,0(s1)
  for (int i = 0; i < 512; i++) {
    8020167e:	04a1                	addi	s1,s1,8
    80201680:	01248963          	beq	s1,s2,80201692 <kfreewalk+0x42>
    pte_t pte = kpt[i];
    80201684:	609c                	ld	a5,0(s1)
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
    80201686:	00f7f713          	andi	a4,a5,15
    8020168a:	ff3701e3          	beq	a4,s3,8020166c <kfreewalk+0x1c>
    } else if (pte & PTE_V) {
    8020168e:	8b85                	andi	a5,a5,1
    80201690:	d7fd                	beqz	a5,8020167e <kfreewalk+0x2e>
      break;
    }
  }
  kfree((void *) kpt);
    80201692:	8552                	mv	a0,s4
    80201694:	fffff097          	auipc	ra,0xfffff
    80201698:	dac080e7          	jalr	-596(ra) # 80200440 <kfree>
}
    8020169c:	70a2                	ld	ra,40(sp)
    8020169e:	7402                	ld	s0,32(sp)
    802016a0:	64e2                	ld	s1,24(sp)
    802016a2:	6942                	ld	s2,16(sp)
    802016a4:	69a2                	ld	s3,8(sp)
    802016a6:	6a02                	ld	s4,0(sp)
    802016a8:	6145                	addi	sp,sp,48
    802016aa:	8082                	ret

00000000802016ac <kvmfreeusr>:

void
kvmfreeusr(pagetable_t kpt)
{
    802016ac:	1101                	addi	sp,sp,-32
    802016ae:	ec06                	sd	ra,24(sp)
    802016b0:	e822                	sd	s0,16(sp)
    802016b2:	e426                	sd	s1,8(sp)
    802016b4:	1000                	addi	s0,sp,32
    802016b6:	84aa                	mv	s1,a0
  pte_t pte;
  for (int i = 0; i < PX(2, MAXUVA); i++) {
    pte = kpt[i];
    802016b8:	6108                	ld	a0,0(a0)
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
    802016ba:	00f57713          	andi	a4,a0,15
    802016be:	4785                	li	a5,1
    802016c0:	00f70d63          	beq	a4,a5,802016da <kvmfreeusr+0x2e>
    pte = kpt[i];
    802016c4:	6488                	ld	a0,8(s1)
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
    802016c6:	00f57713          	andi	a4,a0,15
    802016ca:	4785                	li	a5,1
    802016cc:	02f70063          	beq	a4,a5,802016ec <kvmfreeusr+0x40>
      kfreewalk((pagetable_t) PTE2PA(pte));
      kpt[i] = 0;
    }
  }
}
    802016d0:	60e2                	ld	ra,24(sp)
    802016d2:	6442                	ld	s0,16(sp)
    802016d4:	64a2                	ld	s1,8(sp)
    802016d6:	6105                	addi	sp,sp,32
    802016d8:	8082                	ret
      kfreewalk((pagetable_t) PTE2PA(pte));
    802016da:	8129                	srli	a0,a0,0xa
    802016dc:	0532                	slli	a0,a0,0xc
    802016de:	00000097          	auipc	ra,0x0
    802016e2:	f72080e7          	jalr	-142(ra) # 80201650 <kfreewalk>
      kpt[i] = 0;
    802016e6:	0004b023          	sd	zero,0(s1)
    802016ea:	bfe9                	j	802016c4 <kvmfreeusr+0x18>
      kfreewalk((pagetable_t) PTE2PA(pte));
    802016ec:	8129                	srli	a0,a0,0xa
    802016ee:	0532                	slli	a0,a0,0xc
    802016f0:	00000097          	auipc	ra,0x0
    802016f4:	f60080e7          	jalr	-160(ra) # 80201650 <kfreewalk>
      kpt[i] = 0;
    802016f8:	0004b423          	sd	zero,8(s1)
}
    802016fc:	bfd1                	j	802016d0 <kvmfreeusr+0x24>

00000000802016fe <kvmfree>:

void
kvmfree(pagetable_t kpt, int stack_free)
{
    802016fe:	1101                	addi	sp,sp,-32
    80201700:	ec06                	sd	ra,24(sp)
    80201702:	e822                	sd	s0,16(sp)
    80201704:	e426                	sd	s1,8(sp)
    80201706:	1000                	addi	s0,sp,32
    80201708:	84aa                	mv	s1,a0
  if (stack_free) {
    8020170a:	e185                	bnez	a1,8020172a <kvmfree+0x2c>
    pte_t pte = kpt[PX(2, VKSTACK)];
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
      kfreewalk((pagetable_t) PTE2PA(pte));
    }
  }
  kvmfreeusr(kpt);
    8020170c:	8526                	mv	a0,s1
    8020170e:	00000097          	auipc	ra,0x0
    80201712:	f9e080e7          	jalr	-98(ra) # 802016ac <kvmfreeusr>
  kfree(kpt);
    80201716:	8526                	mv	a0,s1
    80201718:	fffff097          	auipc	ra,0xfffff
    8020171c:	d28080e7          	jalr	-728(ra) # 80200440 <kfree>
}
    80201720:	60e2                	ld	ra,24(sp)
    80201722:	6442                	ld	s0,16(sp)
    80201724:	64a2                	ld	s1,8(sp)
    80201726:	6105                	addi	sp,sp,32
    80201728:	8082                	ret
    vmunmap(kpt, VKSTACK, 1, 1);
    8020172a:	4685                	li	a3,1
    8020172c:	4605                	li	a2,1
    8020172e:	0fb00593          	li	a1,251
    80201732:	05fa                	slli	a1,a1,0x1e
    80201734:	fffff097          	auipc	ra,0xfffff
    80201738:	764080e7          	jalr	1892(ra) # 80200e98 <vmunmap>
    pte_t pte = kpt[PX(2, VKSTACK)];
    8020173c:	7d84b503          	ld	a0,2008(s1)
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0) {
    80201740:	00f57713          	andi	a4,a0,15
    80201744:	4785                	li	a5,1
    80201746:	fcf713e3          	bne	a4,a5,8020170c <kvmfree+0xe>
      kfreewalk((pagetable_t) PTE2PA(pte));
    8020174a:	8129                	srli	a0,a0,0xa
    8020174c:	0532                	slli	a0,a0,0xc
    8020174e:	00000097          	auipc	ra,0x0
    80201752:	f02080e7          	jalr	-254(ra) # 80201650 <kfreewalk>
    80201756:	bf5d                	j	8020170c <kvmfree+0xe>

0000000080201758 <proc_kpagetable>:
{
    80201758:	1101                	addi	sp,sp,-32
    8020175a:	ec06                	sd	ra,24(sp)
    8020175c:	e822                	sd	s0,16(sp)
    8020175e:	e426                	sd	s1,8(sp)
    80201760:	1000                	addi	s0,sp,32
  pagetable_t kpt = (pagetable_t) kalloc();
    80201762:	fffff097          	auipc	ra,0xfffff
    80201766:	e26080e7          	jalr	-474(ra) # 80200588 <kalloc>
    8020176a:	84aa                	mv	s1,a0
  if (kpt == NULL)
    8020176c:	c91d                	beqz	a0,802017a2 <proc_kpagetable+0x4a>
  memmove(kpt, kernel_pagetable, PGSIZE);
    8020176e:	6605                	lui	a2,0x1
    80201770:	0003b597          	auipc	a1,0x3b
    80201774:	8e85b583          	ld	a1,-1816(a1) # 8023c058 <kernel_pagetable>
    80201778:	fffff097          	auipc	ra,0xfffff
    8020177c:	074080e7          	jalr	116(ra) # 802007ec <memmove>
  char *pstack = kalloc();
    80201780:	fffff097          	auipc	ra,0xfffff
    80201784:	e08080e7          	jalr	-504(ra) # 80200588 <kalloc>
    80201788:	86aa                	mv	a3,a0
  if(pstack == NULL)
    8020178a:	c115                	beqz	a0,802017ae <proc_kpagetable+0x56>
  if (mappages(kpt, VKSTACK, PGSIZE, (uint64)pstack, PTE_R | PTE_W) != 0)
    8020178c:	4719                	li	a4,6
    8020178e:	6605                	lui	a2,0x1
    80201790:	0fb00593          	li	a1,251
    80201794:	05fa                	slli	a1,a1,0x1e
    80201796:	8526                	mv	a0,s1
    80201798:	fffff097          	auipc	ra,0xfffff
    8020179c:	536080e7          	jalr	1334(ra) # 80200cce <mappages>
    802017a0:	e519                	bnez	a0,802017ae <proc_kpagetable+0x56>
}
    802017a2:	8526                	mv	a0,s1
    802017a4:	60e2                	ld	ra,24(sp)
    802017a6:	6442                	ld	s0,16(sp)
    802017a8:	64a2                	ld	s1,8(sp)
    802017aa:	6105                	addi	sp,sp,32
    802017ac:	8082                	ret
  kvmfree(kpt, 1);
    802017ae:	4585                	li	a1,1
    802017b0:	8526                	mv	a0,s1
    802017b2:	00000097          	auipc	ra,0x0
    802017b6:	f4c080e7          	jalr	-180(ra) # 802016fe <kvmfree>
  return NULL;
    802017ba:	4481                	li	s1,0
    802017bc:	b7dd                	j	802017a2 <proc_kpagetable+0x4a>

00000000802017be <vmprint>:

void vmprint(pagetable_t pagetable)
{
    802017be:	7119                	addi	sp,sp,-128
    802017c0:	fc86                	sd	ra,120(sp)
    802017c2:	f8a2                	sd	s0,112(sp)
    802017c4:	f4a6                	sd	s1,104(sp)
    802017c6:	f0ca                	sd	s2,96(sp)
    802017c8:	ecce                	sd	s3,88(sp)
    802017ca:	e8d2                	sd	s4,80(sp)
    802017cc:	e4d6                	sd	s5,72(sp)
    802017ce:	e0da                	sd	s6,64(sp)
    802017d0:	fc5e                	sd	s7,56(sp)
    802017d2:	f862                	sd	s8,48(sp)
    802017d4:	f466                	sd	s9,40(sp)
    802017d6:	f06a                	sd	s10,32(sp)
    802017d8:	ec6e                	sd	s11,24(sp)
    802017da:	0100                	addi	s0,sp,128
    802017dc:	8baa                	mv	s7,a0
    802017de:	f8a43423          	sd	a0,-120(s0)
  const int capacity = 512;
  printf("page table %p\n", pagetable);
    802017e2:	85aa                	mv	a1,a0
    802017e4:	00008517          	auipc	a0,0x8
    802017e8:	d8450513          	addi	a0,a0,-636 # 80209568 <digits+0x1e8>
    802017ec:	fffff097          	auipc	ra,0xfffff
    802017f0:	9a2080e7          	jalr	-1630(ra) # 8020018e <printf>
  for (pte_t *pte = (pte_t *) pagetable; pte < pagetable + capacity; pte++) {
    802017f4:	6d85                	lui	s11,0x1
    802017f6:	9dde                	add	s11,s11,s7
    802017f8:	6c85                	lui	s9,0x1

      for (pte_t *pte2 = (pte_t *) pt2; pte2 < pt2 + capacity; pte2++) {
        if (*pte2 & PTE_V)
        {
          pagetable_t pt3 = (pagetable_t) PTE2PA(*pte2);
          printf(".. ..%d: pte %p pa %p\n", pte2 - pt2, *pte2, pt3);
    802017fa:	00008d17          	auipc	s10,0x8
    802017fe:	d96d0d13          	addi	s10,s10,-618 # 80209590 <digits+0x210>

          for (pte_t *pte3 = (pte_t *) pt3; pte3 < pt3 + capacity; pte3++)
            if (*pte3 & PTE_V)
              printf(".. .. ..%d: pte %p pa %p\n", pte3 - pt3, *pte3, PTE2PA(*pte3));
    80201802:	00008a17          	auipc	s4,0x8
    80201806:	da6a0a13          	addi	s4,s4,-602 # 802095a8 <digits+0x228>
    8020180a:	a885                	j	8020187a <vmprint+0xbc>
          for (pte_t *pte3 = (pte_t *) pt3; pte3 < pt3 + capacity; pte3++)
    8020180c:	04a1                	addi	s1,s1,8
    8020180e:	197d                	addi	s2,s2,-1 # fff <_entry-0x801ff001>
    80201810:	02090263          	beqz	s2,80201834 <vmprint+0x76>
            if (*pte3 & PTE_V)
    80201814:	6090                	ld	a2,0(s1)
    80201816:	00167793          	andi	a5,a2,1
    8020181a:	dbed                	beqz	a5,8020180c <vmprint+0x4e>
              printf(".. .. ..%d: pte %p pa %p\n", pte3 - pt3, *pte3, PTE2PA(*pte3));
    8020181c:	00a65693          	srli	a3,a2,0xa
    80201820:	413485b3          	sub	a1,s1,s3
    80201824:	06b2                	slli	a3,a3,0xc
    80201826:	858d                	srai	a1,a1,0x3
    80201828:	8552                	mv	a0,s4
    8020182a:	fffff097          	auipc	ra,0xfffff
    8020182e:	964080e7          	jalr	-1692(ra) # 8020018e <printf>
    80201832:	bfe9                	j	8020180c <vmprint+0x4e>
      for (pte_t *pte2 = (pte_t *) pt2; pte2 < pt2 + capacity; pte2++) {
    80201834:	0aa1                	addi	s5,s5,8 # fffffffffffff008 <ebss_clear+0xffffffff7fda7008>
    80201836:	1b7d                	addi	s6,s6,-1 # fff <_entry-0x801ff001>
    80201838:	020b0e63          	beqz	s6,80201874 <vmprint+0xb6>
        if (*pte2 & PTE_V)
    8020183c:	000ab603          	ld	a2,0(s5)
    80201840:	00167793          	andi	a5,a2,1
    80201844:	dbe5                	beqz	a5,80201834 <vmprint+0x76>
          pagetable_t pt3 = (pagetable_t) PTE2PA(*pte2);
    80201846:	00a65993          	srli	s3,a2,0xa
    8020184a:	09b2                	slli	s3,s3,0xc
          printf(".. ..%d: pte %p pa %p\n", pte2 - pt2, *pte2, pt3);
    8020184c:	418a85b3          	sub	a1,s5,s8
    80201850:	86ce                	mv	a3,s3
    80201852:	858d                	srai	a1,a1,0x3
    80201854:	856a                	mv	a0,s10
    80201856:	fffff097          	auipc	ra,0xfffff
    8020185a:	938080e7          	jalr	-1736(ra) # 8020018e <printf>
          for (pte_t *pte3 = (pte_t *) pt3; pte3 < pt3 + capacity; pte3++)
    8020185e:	00898713          	addi	a4,s3,8 # 1008 <_entry-0x801feff8>
    80201862:	019987b3          	add	a5,s3,s9
    80201866:	20000913          	li	s2,512
    8020186a:	00e7f363          	bgeu	a5,a4,80201870 <vmprint+0xb2>
    8020186e:	4905                	li	s2,1
    80201870:	84ce                	mv	s1,s3
    80201872:	b74d                	j	80201814 <vmprint+0x56>
  for (pte_t *pte = (pte_t *) pagetable; pte < pagetable + capacity; pte++) {
    80201874:	0ba1                	addi	s7,s7,8 # fffffffffffff008 <ebss_clear+0xffffffff7fda7008>
    80201876:	05bb8463          	beq	s7,s11,802018be <vmprint+0x100>
    if (*pte & PTE_V)
    8020187a:	000bb603          	ld	a2,0(s7)
    8020187e:	00167793          	andi	a5,a2,1
    80201882:	dbed                	beqz	a5,80201874 <vmprint+0xb6>
      pagetable_t pt2 = (pagetable_t) PTE2PA(*pte); 
    80201884:	00a65c13          	srli	s8,a2,0xa
    80201888:	0c32                	slli	s8,s8,0xc
      printf("..%d: pte %p pa %p\n", pte - pagetable, *pte, pt2);
    8020188a:	f8843783          	ld	a5,-120(s0)
    8020188e:	40fb87b3          	sub	a5,s7,a5
    80201892:	86e2                	mv	a3,s8
    80201894:	4037d593          	srai	a1,a5,0x3
    80201898:	00008517          	auipc	a0,0x8
    8020189c:	ce050513          	addi	a0,a0,-800 # 80209578 <digits+0x1f8>
    802018a0:	fffff097          	auipc	ra,0xfffff
    802018a4:	8ee080e7          	jalr	-1810(ra) # 8020018e <printf>
      for (pte_t *pte2 = (pte_t *) pt2; pte2 < pt2 + capacity; pte2++) {
    802018a8:	019c0733          	add	a4,s8,s9
    802018ac:	008c0793          	addi	a5,s8,8 # 1008 <_entry-0x801feff8>
    802018b0:	20000b13          	li	s6,512
    802018b4:	00f77363          	bgeu	a4,a5,802018ba <vmprint+0xfc>
    802018b8:	4b05                	li	s6,1
    802018ba:	8ae2                	mv	s5,s8
    802018bc:	b741                	j	8020183c <vmprint+0x7e>
        }
      }
    }
  }
  return;
    802018be:	70e6                	ld	ra,120(sp)
    802018c0:	7446                	ld	s0,112(sp)
    802018c2:	74a6                	ld	s1,104(sp)
    802018c4:	7906                	ld	s2,96(sp)
    802018c6:	69e6                	ld	s3,88(sp)
    802018c8:	6a46                	ld	s4,80(sp)
    802018ca:	6aa6                	ld	s5,72(sp)
    802018cc:	6b06                	ld	s6,64(sp)
    802018ce:	7be2                	ld	s7,56(sp)
    802018d0:	7c42                	ld	s8,48(sp)
    802018d2:	7ca2                	ld	s9,40(sp)
    802018d4:	7d02                	ld	s10,32(sp)
    802018d6:	6de2                	ld	s11,24(sp)
    802018d8:	6109                	addi	sp,sp,128
    802018da:	8082                	ret

00000000802018dc <wakeup1>:

// Wake up p if it is sleeping in wait(); used by exit().
// Caller must hold p->lock.
static void
wakeup1(struct proc *p)
{
    802018dc:	1101                	addi	sp,sp,-32
    802018de:	ec06                	sd	ra,24(sp)
    802018e0:	e822                	sd	s0,16(sp)
    802018e2:	e426                	sd	s1,8(sp)
    802018e4:	1000                	addi	s0,sp,32
    802018e6:	84aa                	mv	s1,a0
  if(!holding(&p->lock))
    802018e8:	fffff097          	auipc	ra,0xfffff
    802018ec:	dde080e7          	jalr	-546(ra) # 802006c6 <holding>
    802018f0:	c909                	beqz	a0,80201902 <wakeup1+0x26>
    panic("wakeup1");
  if(p->chan == p && p->state == SLEEPING) {
    802018f2:	749c                	ld	a5,40(s1)
    802018f4:	00978f63          	beq	a5,s1,80201912 <wakeup1+0x36>
    p->state = RUNNABLE;
  }
}
    802018f8:	60e2                	ld	ra,24(sp)
    802018fa:	6442                	ld	s0,16(sp)
    802018fc:	64a2                	ld	s1,8(sp)
    802018fe:	6105                	addi	sp,sp,32
    80201900:	8082                	ret
    panic("wakeup1");
    80201902:	00008517          	auipc	a0,0x8
    80201906:	cc650513          	addi	a0,a0,-826 # 802095c8 <digits+0x248>
    8020190a:	fffff097          	auipc	ra,0xfffff
    8020190e:	83a080e7          	jalr	-1990(ra) # 80200144 <panic>
  if(p->chan == p && p->state == SLEEPING) {
    80201912:	4c98                	lw	a4,24(s1)
    80201914:	4785                	li	a5,1
    80201916:	fef711e3          	bne	a4,a5,802018f8 <wakeup1+0x1c>
    p->state = RUNNABLE;
    8020191a:	4789                	li	a5,2
    8020191c:	cc9c                	sw	a5,24(s1)
}
    8020191e:	bfe9                	j	802018f8 <wakeup1+0x1c>

0000000080201920 <reg_info>:
void reg_info(void) {
    80201920:	1141                	addi	sp,sp,-16
    80201922:	e406                	sd	ra,8(sp)
    80201924:	e022                	sd	s0,0(sp)
    80201926:	0800                	addi	s0,sp,16
  printf("register info: {\n");
    80201928:	00008517          	auipc	a0,0x8
    8020192c:	ca850513          	addi	a0,a0,-856 # 802095d0 <digits+0x250>
    80201930:	fffff097          	auipc	ra,0xfffff
    80201934:	85e080e7          	jalr	-1954(ra) # 8020018e <printf>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80201938:	100025f3          	csrr	a1,sstatus
  printf("sstatus: %p\n", r_sstatus());
    8020193c:	00008517          	auipc	a0,0x8
    80201940:	cac50513          	addi	a0,a0,-852 # 802095e8 <digits+0x268>
    80201944:	fffff097          	auipc	ra,0xfffff
    80201948:	84a080e7          	jalr	-1974(ra) # 8020018e <printf>
  asm volatile("csrr %0, sip" : "=r" (x) );
    8020194c:	144025f3          	csrr	a1,sip
  printf("sip: %p\n", r_sip());
    80201950:	00008517          	auipc	a0,0x8
    80201954:	ca850513          	addi	a0,a0,-856 # 802095f8 <digits+0x278>
    80201958:	fffff097          	auipc	ra,0xfffff
    8020195c:	836080e7          	jalr	-1994(ra) # 8020018e <printf>
  asm volatile("csrr %0, sie" : "=r" (x) );
    80201960:	104025f3          	csrr	a1,sie
  printf("sie: %p\n", r_sie());
    80201964:	00008517          	auipc	a0,0x8
    80201968:	ca450513          	addi	a0,a0,-860 # 80209608 <digits+0x288>
    8020196c:	fffff097          	auipc	ra,0xfffff
    80201970:	822080e7          	jalr	-2014(ra) # 8020018e <printf>
  asm volatile("csrr %0, sepc" : "=r" (x) );
    80201974:	141025f3          	csrr	a1,sepc
  printf("sepc: %p\n", r_sepc());
    80201978:	00008517          	auipc	a0,0x8
    8020197c:	ca050513          	addi	a0,a0,-864 # 80209618 <digits+0x298>
    80201980:	fffff097          	auipc	ra,0xfffff
    80201984:	80e080e7          	jalr	-2034(ra) # 8020018e <printf>
  asm volatile("csrr %0, stvec" : "=r" (x) );
    80201988:	105025f3          	csrr	a1,stvec
  printf("stvec: %p\n", r_stvec());
    8020198c:	00008517          	auipc	a0,0x8
    80201990:	c9c50513          	addi	a0,a0,-868 # 80209628 <digits+0x2a8>
    80201994:	ffffe097          	auipc	ra,0xffffe
    80201998:	7fa080e7          	jalr	2042(ra) # 8020018e <printf>
  asm volatile("csrr %0, satp" : "=r" (x) );
    8020199c:	180025f3          	csrr	a1,satp
  printf("satp: %p\n", r_satp());
    802019a0:	00008517          	auipc	a0,0x8
    802019a4:	c9850513          	addi	a0,a0,-872 # 80209638 <digits+0x2b8>
    802019a8:	ffffe097          	auipc	ra,0xffffe
    802019ac:	7e6080e7          	jalr	2022(ra) # 8020018e <printf>
  asm volatile("csrr %0, scause" : "=r" (x) );
    802019b0:	142025f3          	csrr	a1,scause
  printf("scause: %p\n", r_scause());
    802019b4:	00008517          	auipc	a0,0x8
    802019b8:	c9450513          	addi	a0,a0,-876 # 80209648 <digits+0x2c8>
    802019bc:	ffffe097          	auipc	ra,0xffffe
    802019c0:	7d2080e7          	jalr	2002(ra) # 8020018e <printf>
  asm volatile("csrr %0, stval" : "=r" (x) );
    802019c4:	143025f3          	csrr	a1,stval
  printf("stval: %p\n", r_stval());
    802019c8:	00008517          	auipc	a0,0x8
    802019cc:	c9050513          	addi	a0,a0,-880 # 80209658 <digits+0x2d8>
    802019d0:	ffffe097          	auipc	ra,0xffffe
    802019d4:	7be080e7          	jalr	1982(ra) # 8020018e <printf>
  asm volatile("mv %0, sp" : "=r" (x) );
    802019d8:	858a                	mv	a1,sp
  printf("sp: %p\n", r_sp());
    802019da:	00008517          	auipc	a0,0x8
    802019de:	c8e50513          	addi	a0,a0,-882 # 80209668 <digits+0x2e8>
    802019e2:	ffffe097          	auipc	ra,0xffffe
    802019e6:	7ac080e7          	jalr	1964(ra) # 8020018e <printf>
  asm volatile("mv %0, tp" : "=r" (x) );
    802019ea:	8592                	mv	a1,tp
  printf("tp: %p\n", r_tp());
    802019ec:	00008517          	auipc	a0,0x8
    802019f0:	c8450513          	addi	a0,a0,-892 # 80209670 <digits+0x2f0>
    802019f4:	ffffe097          	auipc	ra,0xffffe
    802019f8:	79a080e7          	jalr	1946(ra) # 8020018e <printf>
  asm volatile("mv %0, ra" : "=r" (x) );
    802019fc:	8586                	mv	a1,ra
  printf("ra: %p\n", r_ra());
    802019fe:	00008517          	auipc	a0,0x8
    80201a02:	c7a50513          	addi	a0,a0,-902 # 80209678 <digits+0x2f8>
    80201a06:	ffffe097          	auipc	ra,0xffffe
    80201a0a:	788080e7          	jalr	1928(ra) # 8020018e <printf>
  printf("}\n");
    80201a0e:	00008517          	auipc	a0,0x8
    80201a12:	c7250513          	addi	a0,a0,-910 # 80209680 <digits+0x300>
    80201a16:	ffffe097          	auipc	ra,0xffffe
    80201a1a:	778080e7          	jalr	1912(ra) # 8020018e <printf>
}
    80201a1e:	60a2                	ld	ra,8(sp)
    80201a20:	6402                	ld	s0,0(sp)
    80201a22:	0141                	addi	sp,sp,16
    80201a24:	8082                	ret

0000000080201a26 <procinit>:
{
    80201a26:	7179                	addi	sp,sp,-48
    80201a28:	f406                	sd	ra,40(sp)
    80201a2a:	f022                	sd	s0,32(sp)
    80201a2c:	ec26                	sd	s1,24(sp)
    80201a2e:	e84a                	sd	s2,16(sp)
    80201a30:	e44e                	sd	s3,8(sp)
    80201a32:	1800                	addi	s0,sp,48
  initlock(&pid_lock, "nextpid");
    80201a34:	00008597          	auipc	a1,0x8
    80201a38:	c5458593          	addi	a1,a1,-940 # 80209688 <digits+0x308>
    80201a3c:	0003a517          	auipc	a0,0x3a
    80201a40:	62450513          	addi	a0,a0,1572 # 8023c060 <pid_lock>
    80201a44:	fffff097          	auipc	ra,0xfffff
    80201a48:	c6c080e7          	jalr	-916(ra) # 802006b0 <initlock>
  for(p = proc; p < &proc[NPROC]; p++) {
    80201a4c:	0003a497          	auipc	s1,0x3a
    80201a50:	73448493          	addi	s1,s1,1844 # 8023c180 <proc>
      initlock(&p->lock, "proc");
    80201a54:	00008997          	auipc	s3,0x8
    80201a58:	c3c98993          	addi	s3,s3,-964 # 80209690 <digits+0x310>
  for(p = proc; p < &proc[NPROC]; p++) {
    80201a5c:	00048917          	auipc	s2,0x48
    80201a60:	37490913          	addi	s2,s2,884 # 80249dd0 <bcache>
      initlock(&p->lock, "proc");
    80201a64:	85ce                	mv	a1,s3
    80201a66:	8526                	mv	a0,s1
    80201a68:	fffff097          	auipc	ra,0xfffff
    80201a6c:	c48080e7          	jalr	-952(ra) # 802006b0 <initlock>
  for(p = proc; p < &proc[NPROC]; p++) {
    80201a70:	46848493          	addi	s1,s1,1128
    80201a74:	ff2498e3          	bne	s1,s2,80201a64 <procinit+0x3e>
  memset(cpus, 0, sizeof(cpus));
    80201a78:	10000613          	li	a2,256
    80201a7c:	4581                	li	a1,0
    80201a7e:	0003a517          	auipc	a0,0x3a
    80201a82:	5fa50513          	addi	a0,a0,1530 # 8023c078 <cpus>
    80201a86:	fffff097          	auipc	ra,0xfffff
    80201a8a:	d0a080e7          	jalr	-758(ra) # 80200790 <memset>
  printf("procinit\n");
    80201a8e:	00008517          	auipc	a0,0x8
    80201a92:	c0a50513          	addi	a0,a0,-1014 # 80209698 <digits+0x318>
    80201a96:	ffffe097          	auipc	ra,0xffffe
    80201a9a:	6f8080e7          	jalr	1784(ra) # 8020018e <printf>
}
    80201a9e:	70a2                	ld	ra,40(sp)
    80201aa0:	7402                	ld	s0,32(sp)
    80201aa2:	64e2                	ld	s1,24(sp)
    80201aa4:	6942                	ld	s2,16(sp)
    80201aa6:	69a2                	ld	s3,8(sp)
    80201aa8:	6145                	addi	sp,sp,48
    80201aaa:	8082                	ret

0000000080201aac <cpuid>:
{
    80201aac:	1141                	addi	sp,sp,-16
    80201aae:	e422                	sd	s0,8(sp)
    80201ab0:	0800                	addi	s0,sp,16
  asm volatile("mv %0, tp" : "=r" (x) );
    80201ab2:	8512                	mv	a0,tp
}
    80201ab4:	2501                	sext.w	a0,a0
    80201ab6:	6422                	ld	s0,8(sp)
    80201ab8:	0141                	addi	sp,sp,16
    80201aba:	8082                	ret

0000000080201abc <mycpu>:
mycpu(void) {
    80201abc:	1141                	addi	sp,sp,-16
    80201abe:	e422                	sd	s0,8(sp)
    80201ac0:	0800                	addi	s0,sp,16
    80201ac2:	8792                	mv	a5,tp
  struct cpu *c = &cpus[id];
    80201ac4:	2781                	sext.w	a5,a5
    80201ac6:	079e                	slli	a5,a5,0x7
}
    80201ac8:	0003a517          	auipc	a0,0x3a
    80201acc:	5b050513          	addi	a0,a0,1456 # 8023c078 <cpus>
    80201ad0:	953e                	add	a0,a0,a5
    80201ad2:	6422                	ld	s0,8(sp)
    80201ad4:	0141                	addi	sp,sp,16
    80201ad6:	8082                	ret

0000000080201ad8 <myproc>:
myproc(void) {
    80201ad8:	1101                	addi	sp,sp,-32
    80201ada:	ec06                	sd	ra,24(sp)
    80201adc:	e822                	sd	s0,16(sp)
    80201ade:	e426                	sd	s1,8(sp)
    80201ae0:	1000                	addi	s0,sp,32
  push_off();
    80201ae2:	fffff097          	auipc	ra,0xfffff
    80201ae6:	b22080e7          	jalr	-1246(ra) # 80200604 <push_off>
    80201aea:	8792                	mv	a5,tp
  struct proc *p = c->proc;
    80201aec:	2781                	sext.w	a5,a5
    80201aee:	079e                	slli	a5,a5,0x7
    80201af0:	0003a717          	auipc	a4,0x3a
    80201af4:	57070713          	addi	a4,a4,1392 # 8023c060 <pid_lock>
    80201af8:	97ba                	add	a5,a5,a4
    80201afa:	6f84                	ld	s1,24(a5)
  pop_off();
    80201afc:	fffff097          	auipc	ra,0xfffff
    80201b00:	b54080e7          	jalr	-1196(ra) # 80200650 <pop_off>
}
    80201b04:	8526                	mv	a0,s1
    80201b06:	60e2                	ld	ra,24(sp)
    80201b08:	6442                	ld	s0,16(sp)
    80201b0a:	64a2                	ld	s1,8(sp)
    80201b0c:	6105                	addi	sp,sp,32
    80201b0e:	8082                	ret

0000000080201b10 <forkret>:
{
    80201b10:	1101                	addi	sp,sp,-32
    80201b12:	ec06                	sd	ra,24(sp)
    80201b14:	e822                	sd	s0,16(sp)
    80201b16:	e426                	sd	s1,8(sp)
    80201b18:	1000                	addi	s0,sp,32
  release(&myproc()->lock);
    80201b1a:	00000097          	auipc	ra,0x0
    80201b1e:	fbe080e7          	jalr	-66(ra) # 80201ad8 <myproc>
    80201b22:	fffff097          	auipc	ra,0xfffff
    80201b26:	c26080e7          	jalr	-986(ra) # 80200748 <release>
  if (first) {
    80201b2a:	00009797          	auipc	a5,0x9
    80201b2e:	4d67a783          	lw	a5,1238(a5) # 8020b000 <first.1>
    80201b32:	eb91                	bnez	a5,80201b46 <forkret+0x36>
  usertrapret();
    80201b34:	00001097          	auipc	ra,0x1
    80201b38:	de2080e7          	jalr	-542(ra) # 80202916 <usertrapret>
}
    80201b3c:	60e2                	ld	ra,24(sp)
    80201b3e:	6442                	ld	s0,16(sp)
    80201b40:	64a2                	ld	s1,8(sp)
    80201b42:	6105                	addi	sp,sp,32
    80201b44:	8082                	ret
    first = 0;
    80201b46:	00009797          	auipc	a5,0x9
    80201b4a:	4a07ad23          	sw	zero,1210(a5) # 8020b000 <first.1>
    fat32_init();
    80201b4e:	00004097          	auipc	ra,0x4
    80201b52:	5c4080e7          	jalr	1476(ra) # 80206112 <fat32_init>
    myproc()->cwd = ename("/");
    80201b56:	00000097          	auipc	ra,0x0
    80201b5a:	f82080e7          	jalr	-126(ra) # 80201ad8 <myproc>
    80201b5e:	84aa                	mv	s1,a0
    80201b60:	00008517          	auipc	a0,0x8
    80201b64:	b4850513          	addi	a0,a0,-1208 # 802096a8 <digits+0x328>
    80201b68:	00006097          	auipc	ra,0x6
    80201b6c:	89e080e7          	jalr	-1890(ra) # 80207406 <ename>
    80201b70:	44a4b423          	sd	a0,1096(s1)
    80201b74:	b7c1                	j	80201b34 <forkret+0x24>

0000000080201b76 <allocpid>:
allocpid() {
    80201b76:	1101                	addi	sp,sp,-32
    80201b78:	ec06                	sd	ra,24(sp)
    80201b7a:	e822                	sd	s0,16(sp)
    80201b7c:	e426                	sd	s1,8(sp)
    80201b7e:	e04a                	sd	s2,0(sp)
    80201b80:	1000                	addi	s0,sp,32
  acquire(&pid_lock);
    80201b82:	0003a917          	auipc	s2,0x3a
    80201b86:	4de90913          	addi	s2,s2,1246 # 8023c060 <pid_lock>
    80201b8a:	854a                	mv	a0,s2
    80201b8c:	fffff097          	auipc	ra,0xfffff
    80201b90:	b68080e7          	jalr	-1176(ra) # 802006f4 <acquire>
  pid = nextpid;
    80201b94:	00009797          	auipc	a5,0x9
    80201b98:	46c78793          	addi	a5,a5,1132 # 8020b000 <first.1>
    80201b9c:	43c4                	lw	s1,4(a5)
  nextpid = nextpid + 1;
    80201b9e:	0014871b          	addiw	a4,s1,1
    80201ba2:	c3d8                	sw	a4,4(a5)
  release(&pid_lock);
    80201ba4:	854a                	mv	a0,s2
    80201ba6:	fffff097          	auipc	ra,0xfffff
    80201baa:	ba2080e7          	jalr	-1118(ra) # 80200748 <release>
}
    80201bae:	8526                	mv	a0,s1
    80201bb0:	60e2                	ld	ra,24(sp)
    80201bb2:	6442                	ld	s0,16(sp)
    80201bb4:	64a2                	ld	s1,8(sp)
    80201bb6:	6902                	ld	s2,0(sp)
    80201bb8:	6105                	addi	sp,sp,32
    80201bba:	8082                	ret

0000000080201bbc <proc_pagetable>:
{
    80201bbc:	1101                	addi	sp,sp,-32
    80201bbe:	ec06                	sd	ra,24(sp)
    80201bc0:	e822                	sd	s0,16(sp)
    80201bc2:	e426                	sd	s1,8(sp)
    80201bc4:	e04a                	sd	s2,0(sp)
    80201bc6:	1000                	addi	s0,sp,32
    80201bc8:	892a                	mv	s2,a0
  pagetable = uvmcreate();
    80201bca:	fffff097          	auipc	ra,0xfffff
    80201bce:	392080e7          	jalr	914(ra) # 80200f5c <uvmcreate>
    80201bd2:	84aa                	mv	s1,a0
  if(pagetable == 0)
    80201bd4:	c121                	beqz	a0,80201c14 <proc_pagetable+0x58>
  if(mappages(pagetable, TRAMPOLINE, PGSIZE,
    80201bd6:	4729                	li	a4,10
    80201bd8:	00031697          	auipc	a3,0x31
    80201bdc:	7086b683          	ld	a3,1800(a3) # 802332e0 <_GLOBAL_OFFSET_TABLE_+0x8>
    80201be0:	6605                	lui	a2,0x1
    80201be2:	040005b7          	lui	a1,0x4000
    80201be6:	15fd                	addi	a1,a1,-1 # 3ffffff <_entry-0x7c200001>
    80201be8:	05b2                	slli	a1,a1,0xc
    80201bea:	fffff097          	auipc	ra,0xfffff
    80201bee:	0e4080e7          	jalr	228(ra) # 80200cce <mappages>
    80201bf2:	02054863          	bltz	a0,80201c22 <proc_pagetable+0x66>
  if(mappages(pagetable, TRAPFRAME, PGSIZE,
    80201bf6:	4719                	li	a4,6
    80201bf8:	06093683          	ld	a3,96(s2)
    80201bfc:	6605                	lui	a2,0x1
    80201bfe:	020005b7          	lui	a1,0x2000
    80201c02:	15fd                	addi	a1,a1,-1 # 1ffffff <_entry-0x7e200001>
    80201c04:	05b6                	slli	a1,a1,0xd
    80201c06:	8526                	mv	a0,s1
    80201c08:	fffff097          	auipc	ra,0xfffff
    80201c0c:	0c6080e7          	jalr	198(ra) # 80200cce <mappages>
    80201c10:	02054163          	bltz	a0,80201c32 <proc_pagetable+0x76>
}
    80201c14:	8526                	mv	a0,s1
    80201c16:	60e2                	ld	ra,24(sp)
    80201c18:	6442                	ld	s0,16(sp)
    80201c1a:	64a2                	ld	s1,8(sp)
    80201c1c:	6902                	ld	s2,0(sp)
    80201c1e:	6105                	addi	sp,sp,32
    80201c20:	8082                	ret
    uvmfree(pagetable, 0);
    80201c22:	4581                	li	a1,0
    80201c24:	8526                	mv	a0,s1
    80201c26:	fffff097          	auipc	ra,0xfffff
    80201c2a:	5d2080e7          	jalr	1490(ra) # 802011f8 <uvmfree>
    return NULL;
    80201c2e:	4481                	li	s1,0
    80201c30:	b7d5                	j	80201c14 <proc_pagetable+0x58>
    vmunmap(pagetable, TRAMPOLINE, 1, 0);
    80201c32:	4681                	li	a3,0
    80201c34:	4605                	li	a2,1
    80201c36:	040005b7          	lui	a1,0x4000
    80201c3a:	15fd                	addi	a1,a1,-1 # 3ffffff <_entry-0x7c200001>
    80201c3c:	05b2                	slli	a1,a1,0xc
    80201c3e:	8526                	mv	a0,s1
    80201c40:	fffff097          	auipc	ra,0xfffff
    80201c44:	258080e7          	jalr	600(ra) # 80200e98 <vmunmap>
    uvmfree(pagetable, 0);
    80201c48:	4581                	li	a1,0
    80201c4a:	8526                	mv	a0,s1
    80201c4c:	fffff097          	auipc	ra,0xfffff
    80201c50:	5ac080e7          	jalr	1452(ra) # 802011f8 <uvmfree>
    return NULL;
    80201c54:	4481                	li	s1,0
    80201c56:	bf7d                	j	80201c14 <proc_pagetable+0x58>

0000000080201c58 <proc_freepagetable>:
{
    80201c58:	1101                	addi	sp,sp,-32
    80201c5a:	ec06                	sd	ra,24(sp)
    80201c5c:	e822                	sd	s0,16(sp)
    80201c5e:	e426                	sd	s1,8(sp)
    80201c60:	e04a                	sd	s2,0(sp)
    80201c62:	1000                	addi	s0,sp,32
    80201c64:	84aa                	mv	s1,a0
    80201c66:	892e                	mv	s2,a1
  vmunmap(pagetable, TRAMPOLINE, 1, 0);
    80201c68:	4681                	li	a3,0
    80201c6a:	4605                	li	a2,1
    80201c6c:	040005b7          	lui	a1,0x4000
    80201c70:	15fd                	addi	a1,a1,-1 # 3ffffff <_entry-0x7c200001>
    80201c72:	05b2                	slli	a1,a1,0xc
    80201c74:	fffff097          	auipc	ra,0xfffff
    80201c78:	224080e7          	jalr	548(ra) # 80200e98 <vmunmap>
  vmunmap(pagetable, TRAPFRAME, 1, 0);
    80201c7c:	4681                	li	a3,0
    80201c7e:	4605                	li	a2,1
    80201c80:	020005b7          	lui	a1,0x2000
    80201c84:	15fd                	addi	a1,a1,-1 # 1ffffff <_entry-0x7e200001>
    80201c86:	05b6                	slli	a1,a1,0xd
    80201c88:	8526                	mv	a0,s1
    80201c8a:	fffff097          	auipc	ra,0xfffff
    80201c8e:	20e080e7          	jalr	526(ra) # 80200e98 <vmunmap>
  uvmfree(pagetable, sz);
    80201c92:	85ca                	mv	a1,s2
    80201c94:	8526                	mv	a0,s1
    80201c96:	fffff097          	auipc	ra,0xfffff
    80201c9a:	562080e7          	jalr	1378(ra) # 802011f8 <uvmfree>
}
    80201c9e:	60e2                	ld	ra,24(sp)
    80201ca0:	6442                	ld	s0,16(sp)
    80201ca2:	64a2                	ld	s1,8(sp)
    80201ca4:	6902                	ld	s2,0(sp)
    80201ca6:	6105                	addi	sp,sp,32
    80201ca8:	8082                	ret

0000000080201caa <freeproc>:
{
    80201caa:	1101                	addi	sp,sp,-32
    80201cac:	ec06                	sd	ra,24(sp)
    80201cae:	e822                	sd	s0,16(sp)
    80201cb0:	e426                	sd	s1,8(sp)
    80201cb2:	1000                	addi	s0,sp,32
    80201cb4:	84aa                	mv	s1,a0
  if(p->trapframe)
    80201cb6:	7128                	ld	a0,96(a0)
    80201cb8:	c509                	beqz	a0,80201cc2 <freeproc+0x18>
    kfree((void*)p->trapframe);
    80201cba:	ffffe097          	auipc	ra,0xffffe
    80201cbe:	786080e7          	jalr	1926(ra) # 80200440 <kfree>
  p->trapframe = 0;
    80201cc2:	0604b023          	sd	zero,96(s1)
  if (p->kpagetable) {
    80201cc6:	6ca8                	ld	a0,88(s1)
    80201cc8:	c511                	beqz	a0,80201cd4 <freeproc+0x2a>
    kvmfree(p->kpagetable, 1);
    80201cca:	4585                	li	a1,1
    80201ccc:	00000097          	auipc	ra,0x0
    80201cd0:	a32080e7          	jalr	-1486(ra) # 802016fe <kvmfree>
  p->kpagetable = 0;
    80201cd4:	0404bc23          	sd	zero,88(s1)
  if(p->pagetable)
    80201cd8:	68a8                	ld	a0,80(s1)
    80201cda:	c511                	beqz	a0,80201ce6 <freeproc+0x3c>
    proc_freepagetable(p->pagetable, p->sz);
    80201cdc:	64ac                	ld	a1,72(s1)
    80201cde:	00000097          	auipc	ra,0x0
    80201ce2:	f7a080e7          	jalr	-134(ra) # 80201c58 <proc_freepagetable>
  p->pagetable = 0;
    80201ce6:	0404b823          	sd	zero,80(s1)
  p->sz = 0;
    80201cea:	0404b423          	sd	zero,72(s1)
  p->pid = 0;
    80201cee:	0204ac23          	sw	zero,56(s1)
  p->parent = 0;
    80201cf2:	0204b023          	sd	zero,32(s1)
  p->name[0] = 0;
    80201cf6:	44048823          	sb	zero,1104(s1)
  p->chan = 0;
    80201cfa:	0204b423          	sd	zero,40(s1)
  p->killed = 0;
    80201cfe:	0204a823          	sw	zero,48(s1)
  p->xstate = 0;
    80201d02:	0204aa23          	sw	zero,52(s1)
  p->state = UNUSED;
    80201d06:	0004ac23          	sw	zero,24(s1)
}
    80201d0a:	60e2                	ld	ra,24(sp)
    80201d0c:	6442                	ld	s0,16(sp)
    80201d0e:	64a2                	ld	s1,8(sp)
    80201d10:	6105                	addi	sp,sp,32
    80201d12:	8082                	ret

0000000080201d14 <allocproc>:
{
    80201d14:	1101                	addi	sp,sp,-32
    80201d16:	ec06                	sd	ra,24(sp)
    80201d18:	e822                	sd	s0,16(sp)
    80201d1a:	e426                	sd	s1,8(sp)
    80201d1c:	e04a                	sd	s2,0(sp)
    80201d1e:	1000                	addi	s0,sp,32
  for(p = proc; p < &proc[NPROC]; p++) {
    80201d20:	0003a497          	auipc	s1,0x3a
    80201d24:	46048493          	addi	s1,s1,1120 # 8023c180 <proc>
    80201d28:	00048917          	auipc	s2,0x48
    80201d2c:	0a890913          	addi	s2,s2,168 # 80249dd0 <bcache>
    acquire(&p->lock);
    80201d30:	8526                	mv	a0,s1
    80201d32:	fffff097          	auipc	ra,0xfffff
    80201d36:	9c2080e7          	jalr	-1598(ra) # 802006f4 <acquire>
    if(p->state == UNUSED) {
    80201d3a:	4c9c                	lw	a5,24(s1)
    80201d3c:	cf81                	beqz	a5,80201d54 <allocproc+0x40>
      release(&p->lock);
    80201d3e:	8526                	mv	a0,s1
    80201d40:	fffff097          	auipc	ra,0xfffff
    80201d44:	a08080e7          	jalr	-1528(ra) # 80200748 <release>
  for(p = proc; p < &proc[NPROC]; p++) {
    80201d48:	46848493          	addi	s1,s1,1128
    80201d4c:	ff2492e3          	bne	s1,s2,80201d30 <allocproc+0x1c>
  return NULL;
    80201d50:	4481                	li	s1,0
    80201d52:	a085                	j	80201db2 <allocproc+0x9e>
  p->pid = allocpid();
    80201d54:	00000097          	auipc	ra,0x0
    80201d58:	e22080e7          	jalr	-478(ra) # 80201b76 <allocpid>
    80201d5c:	dc88                	sw	a0,56(s1)
  if((p->trapframe = (struct trapframe *)kalloc()) == NULL){
    80201d5e:	fffff097          	auipc	ra,0xfffff
    80201d62:	82a080e7          	jalr	-2006(ra) # 80200588 <kalloc>
    80201d66:	892a                	mv	s2,a0
    80201d68:	f0a8                	sd	a0,96(s1)
    80201d6a:	c939                	beqz	a0,80201dc0 <allocproc+0xac>
  if ((p->pagetable = proc_pagetable(p)) == NULL ||
    80201d6c:	8526                	mv	a0,s1
    80201d6e:	00000097          	auipc	ra,0x0
    80201d72:	e4e080e7          	jalr	-434(ra) # 80201bbc <proc_pagetable>
    80201d76:	e8a8                	sd	a0,80(s1)
    80201d78:	c939                	beqz	a0,80201dce <allocproc+0xba>
      (p->kpagetable = proc_kpagetable()) == NULL) {
    80201d7a:	00000097          	auipc	ra,0x0
    80201d7e:	9de080e7          	jalr	-1570(ra) # 80201758 <proc_kpagetable>
    80201d82:	eca8                	sd	a0,88(s1)
  if ((p->pagetable = proc_pagetable(p)) == NULL ||
    80201d84:	c529                	beqz	a0,80201dce <allocproc+0xba>
  p->kstack = VKSTACK;
    80201d86:	0fb00793          	li	a5,251
    80201d8a:	07fa                	slli	a5,a5,0x1e
    80201d8c:	e0bc                	sd	a5,64(s1)
  memset(&p->context, 0, sizeof(p->context));
    80201d8e:	07000613          	li	a2,112
    80201d92:	4581                	li	a1,0
    80201d94:	06848513          	addi	a0,s1,104
    80201d98:	fffff097          	auipc	ra,0xfffff
    80201d9c:	9f8080e7          	jalr	-1544(ra) # 80200790 <memset>
  p->context.ra = (uint64)forkret;
    80201da0:	00000797          	auipc	a5,0x0
    80201da4:	d7078793          	addi	a5,a5,-656 # 80201b10 <forkret>
    80201da8:	f4bc                	sd	a5,104(s1)
  p->context.sp = p->kstack + PGSIZE;
    80201daa:	60bc                	ld	a5,64(s1)
    80201dac:	6705                	lui	a4,0x1
    80201dae:	97ba                	add	a5,a5,a4
    80201db0:	f8bc                	sd	a5,112(s1)
}
    80201db2:	8526                	mv	a0,s1
    80201db4:	60e2                	ld	ra,24(sp)
    80201db6:	6442                	ld	s0,16(sp)
    80201db8:	64a2                	ld	s1,8(sp)
    80201dba:	6902                	ld	s2,0(sp)
    80201dbc:	6105                	addi	sp,sp,32
    80201dbe:	8082                	ret
    release(&p->lock);
    80201dc0:	8526                	mv	a0,s1
    80201dc2:	fffff097          	auipc	ra,0xfffff
    80201dc6:	986080e7          	jalr	-1658(ra) # 80200748 <release>
    return NULL;
    80201dca:	84ca                	mv	s1,s2
    80201dcc:	b7dd                	j	80201db2 <allocproc+0x9e>
    freeproc(p);
    80201dce:	8526                	mv	a0,s1
    80201dd0:	00000097          	auipc	ra,0x0
    80201dd4:	eda080e7          	jalr	-294(ra) # 80201caa <freeproc>
    release(&p->lock);
    80201dd8:	8526                	mv	a0,s1
    80201dda:	fffff097          	auipc	ra,0xfffff
    80201dde:	96e080e7          	jalr	-1682(ra) # 80200748 <release>
    return NULL;
    80201de2:	4481                	li	s1,0
    80201de4:	b7f9                	j	80201db2 <allocproc+0x9e>

0000000080201de6 <userinit>:
{
    80201de6:	1101                	addi	sp,sp,-32
    80201de8:	ec06                	sd	ra,24(sp)
    80201dea:	e822                	sd	s0,16(sp)
    80201dec:	e426                	sd	s1,8(sp)
    80201dee:	e04a                	sd	s2,0(sp)
    80201df0:	1000                	addi	s0,sp,32
  p = allocproc();
    80201df2:	00000097          	auipc	ra,0x0
    80201df6:	f22080e7          	jalr	-222(ra) # 80201d14 <allocproc>
    80201dfa:	84aa                	mv	s1,a0
  initproc = p;
    80201dfc:	0003a797          	auipc	a5,0x3a
    80201e00:	36a7be23          	sd	a0,892(a5) # 8023c178 <initproc>
  uvminit(p->pagetable , p->kpagetable, initcode, sizeof(initcode));
    80201e04:	6905                	lui	s2,0x1
    80201e06:	c2090693          	addi	a3,s2,-992 # c20 <_entry-0x801ff3e0>
    80201e0a:	00009617          	auipc	a2,0x9
    80201e0e:	1fe60613          	addi	a2,a2,510 # 8020b008 <initcode>
    80201e12:	6d2c                	ld	a1,88(a0)
    80201e14:	6928                	ld	a0,80(a0)
    80201e16:	fffff097          	auipc	ra,0xfffff
    80201e1a:	174080e7          	jalr	372(ra) # 80200f8a <uvminit>
  p->sz = PGSIZE;
    80201e1e:	0524b423          	sd	s2,72(s1)
  p->trapframe->epc = 0x0;      // user program counter
    80201e22:	70bc                	ld	a5,96(s1)
    80201e24:	0007bc23          	sd	zero,24(a5)
  p->trapframe->sp = PGSIZE;  // user stack pointer
    80201e28:	70bc                	ld	a5,96(s1)
    80201e2a:	0327b823          	sd	s2,48(a5)
  safestrcpy(p->name, "initcode", sizeof(p->name));
    80201e2e:	4641                	li	a2,16
    80201e30:	00008597          	auipc	a1,0x8
    80201e34:	88058593          	addi	a1,a1,-1920 # 802096b0 <digits+0x330>
    80201e38:	45048513          	addi	a0,s1,1104
    80201e3c:	fffff097          	auipc	ra,0xfffff
    80201e40:	aa6080e7          	jalr	-1370(ra) # 802008e2 <safestrcpy>
  p->state = RUNNABLE;
    80201e44:	4789                	li	a5,2
    80201e46:	cc9c                	sw	a5,24(s1)
  p->tmask = 0;
    80201e48:	4604a023          	sw	zero,1120(s1)
  release(&p->lock);
    80201e4c:	8526                	mv	a0,s1
    80201e4e:	fffff097          	auipc	ra,0xfffff
    80201e52:	8fa080e7          	jalr	-1798(ra) # 80200748 <release>
  printf("userinit\n");
    80201e56:	00008517          	auipc	a0,0x8
    80201e5a:	86a50513          	addi	a0,a0,-1942 # 802096c0 <digits+0x340>
    80201e5e:	ffffe097          	auipc	ra,0xffffe
    80201e62:	330080e7          	jalr	816(ra) # 8020018e <printf>
}
    80201e66:	60e2                	ld	ra,24(sp)
    80201e68:	6442                	ld	s0,16(sp)
    80201e6a:	64a2                	ld	s1,8(sp)
    80201e6c:	6902                	ld	s2,0(sp)
    80201e6e:	6105                	addi	sp,sp,32
    80201e70:	8082                	ret

0000000080201e72 <growproc>:
{
    80201e72:	1101                	addi	sp,sp,-32
    80201e74:	ec06                	sd	ra,24(sp)
    80201e76:	e822                	sd	s0,16(sp)
    80201e78:	e426                	sd	s1,8(sp)
    80201e7a:	e04a                	sd	s2,0(sp)
    80201e7c:	1000                	addi	s0,sp,32
    80201e7e:	84aa                	mv	s1,a0
  struct proc *p = myproc();
    80201e80:	00000097          	auipc	ra,0x0
    80201e84:	c58080e7          	jalr	-936(ra) # 80201ad8 <myproc>
    80201e88:	892a                	mv	s2,a0
  sz = p->sz;
    80201e8a:	6530                	ld	a2,72(a0)
    80201e8c:	0006079b          	sext.w	a5,a2
  if(n > 0){
    80201e90:	00904f63          	bgtz	s1,80201eae <growproc+0x3c>
  } else if(n < 0){
    80201e94:	0204ce63          	bltz	s1,80201ed0 <growproc+0x5e>
  p->sz = sz;
    80201e98:	1782                	slli	a5,a5,0x20
    80201e9a:	9381                	srli	a5,a5,0x20
    80201e9c:	04f93423          	sd	a5,72(s2)
  return 0;
    80201ea0:	4501                	li	a0,0
}
    80201ea2:	60e2                	ld	ra,24(sp)
    80201ea4:	6442                	ld	s0,16(sp)
    80201ea6:	64a2                	ld	s1,8(sp)
    80201ea8:	6902                	ld	s2,0(sp)
    80201eaa:	6105                	addi	sp,sp,32
    80201eac:	8082                	ret
    if((sz = uvmalloc(p->pagetable, p->kpagetable, sz, sz + n)) == 0) {
    80201eae:	00f486bb          	addw	a3,s1,a5
    80201eb2:	1682                	slli	a3,a3,0x20
    80201eb4:	9281                	srli	a3,a3,0x20
    80201eb6:	1602                	slli	a2,a2,0x20
    80201eb8:	9201                	srli	a2,a2,0x20
    80201eba:	6d2c                	ld	a1,88(a0)
    80201ebc:	6928                	ld	a0,80(a0)
    80201ebe:	fffff097          	auipc	ra,0xfffff
    80201ec2:	1d8080e7          	jalr	472(ra) # 80201096 <uvmalloc>
    80201ec6:	0005079b          	sext.w	a5,a0
    80201eca:	f7f9                	bnez	a5,80201e98 <growproc+0x26>
      return -1;
    80201ecc:	557d                	li	a0,-1
    80201ece:	bfd1                	j	80201ea2 <growproc+0x30>
    sz = uvmdealloc(p->pagetable, p->kpagetable, sz, sz + n);
    80201ed0:	00f486bb          	addw	a3,s1,a5
    80201ed4:	1682                	slli	a3,a3,0x20
    80201ed6:	9281                	srli	a3,a3,0x20
    80201ed8:	1602                	slli	a2,a2,0x20
    80201eda:	9201                	srli	a2,a2,0x20
    80201edc:	6d2c                	ld	a1,88(a0)
    80201ede:	6928                	ld	a0,80(a0)
    80201ee0:	fffff097          	auipc	ra,0xfffff
    80201ee4:	14a080e7          	jalr	330(ra) # 8020102a <uvmdealloc>
    80201ee8:	0005079b          	sext.w	a5,a0
    80201eec:	b775                	j	80201e98 <growproc+0x26>

0000000080201eee <fork>:
{
    80201eee:	7139                	addi	sp,sp,-64
    80201ef0:	fc06                	sd	ra,56(sp)
    80201ef2:	f822                	sd	s0,48(sp)
    80201ef4:	f426                	sd	s1,40(sp)
    80201ef6:	f04a                	sd	s2,32(sp)
    80201ef8:	ec4e                	sd	s3,24(sp)
    80201efa:	e852                	sd	s4,16(sp)
    80201efc:	e456                	sd	s5,8(sp)
    80201efe:	0080                	addi	s0,sp,64
  struct proc *p = myproc();
    80201f00:	00000097          	auipc	ra,0x0
    80201f04:	bd8080e7          	jalr	-1064(ra) # 80201ad8 <myproc>
    80201f08:	8aaa                	mv	s5,a0
  if((np = allocproc()) == NULL){
    80201f0a:	00000097          	auipc	ra,0x0
    80201f0e:	e0a080e7          	jalr	-502(ra) # 80201d14 <allocproc>
    80201f12:	c965                	beqz	a0,80202002 <fork+0x114>
    80201f14:	8a2a                	mv	s4,a0
  if(uvmcopy(p->pagetable, np->pagetable, np->kpagetable, p->sz) < 0){
    80201f16:	048ab683          	ld	a3,72(s5)
    80201f1a:	6d30                	ld	a2,88(a0)
    80201f1c:	692c                	ld	a1,80(a0)
    80201f1e:	050ab503          	ld	a0,80(s5)
    80201f22:	fffff097          	auipc	ra,0xfffff
    80201f26:	310080e7          	jalr	784(ra) # 80201232 <uvmcopy>
    80201f2a:	04054e63          	bltz	a0,80201f86 <fork+0x98>
  np->sz = p->sz;
    80201f2e:	048ab783          	ld	a5,72(s5)
    80201f32:	04fa3423          	sd	a5,72(s4)
  np->parent = p;
    80201f36:	035a3023          	sd	s5,32(s4)
  np->tmask = p->tmask;
    80201f3a:	460aa783          	lw	a5,1120(s5)
    80201f3e:	46fa2023          	sw	a5,1120(s4)
  *(np->trapframe) = *(p->trapframe);
    80201f42:	060ab683          	ld	a3,96(s5)
    80201f46:	87b6                	mv	a5,a3
    80201f48:	060a3703          	ld	a4,96(s4)
    80201f4c:	12068693          	addi	a3,a3,288
    80201f50:	0007b803          	ld	a6,0(a5)
    80201f54:	6788                	ld	a0,8(a5)
    80201f56:	6b8c                	ld	a1,16(a5)
    80201f58:	6f90                	ld	a2,24(a5)
    80201f5a:	01073023          	sd	a6,0(a4) # 1000 <_entry-0x801ff000>
    80201f5e:	e708                	sd	a0,8(a4)
    80201f60:	eb0c                	sd	a1,16(a4)
    80201f62:	ef10                	sd	a2,24(a4)
    80201f64:	02078793          	addi	a5,a5,32
    80201f68:	02070713          	addi	a4,a4,32
    80201f6c:	fed792e3          	bne	a5,a3,80201f50 <fork+0x62>
  np->trapframe->a0 = 0;
    80201f70:	060a3783          	ld	a5,96(s4)
    80201f74:	0607b823          	sd	zero,112(a5)
  for(i = 0; i < NOFILE; i++)
    80201f78:	0d8a8493          	addi	s1,s5,216
    80201f7c:	0d8a0913          	addi	s2,s4,216
    80201f80:	448a8993          	addi	s3,s5,1096
    80201f84:	a03d                	j	80201fb2 <fork+0xc4>
    freeproc(np);
    80201f86:	8552                	mv	a0,s4
    80201f88:	00000097          	auipc	ra,0x0
    80201f8c:	d22080e7          	jalr	-734(ra) # 80201caa <freeproc>
    release(&np->lock);
    80201f90:	8552                	mv	a0,s4
    80201f92:	ffffe097          	auipc	ra,0xffffe
    80201f96:	7b6080e7          	jalr	1974(ra) # 80200748 <release>
    return -1;
    80201f9a:	54fd                	li	s1,-1
    80201f9c:	a889                	j	80201fee <fork+0x100>
      np->ofile[i] = filedup(p->ofile[i]);
    80201f9e:	00002097          	auipc	ra,0x2
    80201fa2:	dd8080e7          	jalr	-552(ra) # 80203d76 <filedup>
    80201fa6:	00a93023          	sd	a0,0(s2)
  for(i = 0; i < NOFILE; i++)
    80201faa:	04a1                	addi	s1,s1,8
    80201fac:	0921                	addi	s2,s2,8
    80201fae:	01348563          	beq	s1,s3,80201fb8 <fork+0xca>
    if(p->ofile[i])
    80201fb2:	6088                	ld	a0,0(s1)
    80201fb4:	f56d                	bnez	a0,80201f9e <fork+0xb0>
    80201fb6:	bfd5                	j	80201faa <fork+0xbc>
  np->cwd = edup(p->cwd);
    80201fb8:	448ab503          	ld	a0,1096(s5)
    80201fbc:	00005097          	auipc	ra,0x5
    80201fc0:	a44080e7          	jalr	-1468(ra) # 80206a00 <edup>
    80201fc4:	44aa3423          	sd	a0,1096(s4)
  safestrcpy(np->name, p->name, sizeof(p->name));
    80201fc8:	4641                	li	a2,16
    80201fca:	450a8593          	addi	a1,s5,1104
    80201fce:	450a0513          	addi	a0,s4,1104
    80201fd2:	fffff097          	auipc	ra,0xfffff
    80201fd6:	910080e7          	jalr	-1776(ra) # 802008e2 <safestrcpy>
  pid = np->pid;
    80201fda:	038a2483          	lw	s1,56(s4)
  np->state = RUNNABLE;
    80201fde:	4789                	li	a5,2
    80201fe0:	00fa2c23          	sw	a5,24(s4)
  release(&np->lock);
    80201fe4:	8552                	mv	a0,s4
    80201fe6:	ffffe097          	auipc	ra,0xffffe
    80201fea:	762080e7          	jalr	1890(ra) # 80200748 <release>
}
    80201fee:	8526                	mv	a0,s1
    80201ff0:	70e2                	ld	ra,56(sp)
    80201ff2:	7442                	ld	s0,48(sp)
    80201ff4:	74a2                	ld	s1,40(sp)
    80201ff6:	7902                	ld	s2,32(sp)
    80201ff8:	69e2                	ld	s3,24(sp)
    80201ffa:	6a42                	ld	s4,16(sp)
    80201ffc:	6aa2                	ld	s5,8(sp)
    80201ffe:	6121                	addi	sp,sp,64
    80202000:	8082                	ret
    return -1;
    80202002:	54fd                	li	s1,-1
    80202004:	b7ed                	j	80201fee <fork+0x100>

0000000080202006 <clone>:
{
    80202006:	7139                	addi	sp,sp,-64
    80202008:	fc06                	sd	ra,56(sp)
    8020200a:	f822                	sd	s0,48(sp)
    8020200c:	f426                	sd	s1,40(sp)
    8020200e:	f04a                	sd	s2,32(sp)
    80202010:	ec4e                	sd	s3,24(sp)
    80202012:	e852                	sd	s4,16(sp)
    80202014:	e456                	sd	s5,8(sp)
    80202016:	0080                	addi	s0,sp,64
  struct proc *p = myproc();
    80202018:	00000097          	auipc	ra,0x0
    8020201c:	ac0080e7          	jalr	-1344(ra) # 80201ad8 <myproc>
    80202020:	8aaa                	mv	s5,a0
  if((np = allocproc()) == NULL){
    80202022:	00000097          	auipc	ra,0x0
    80202026:	cf2080e7          	jalr	-782(ra) # 80201d14 <allocproc>
    8020202a:	10050863          	beqz	a0,8020213a <clone+0x134>
    8020202e:	8a2a                	mv	s4,a0
  if(uvmcopy(p->pagetable, np->pagetable, np->kpagetable, p->sz) < 0){
    80202030:	048ab683          	ld	a3,72(s5)
    80202034:	6d30                	ld	a2,88(a0)
    80202036:	692c                	ld	a1,80(a0)
    80202038:	050ab503          	ld	a0,80(s5)
    8020203c:	fffff097          	auipc	ra,0xfffff
    80202040:	1f6080e7          	jalr	502(ra) # 80201232 <uvmcopy>
    80202044:	06054d63          	bltz	a0,802020be <clone+0xb8>
  np->sz = p->sz;
    80202048:	048ab783          	ld	a5,72(s5)
    8020204c:	04fa3423          	sd	a5,72(s4)
  np->parent = p;
    80202050:	035a3023          	sd	s5,32(s4)
  np->tmask = p->tmask;
    80202054:	460aa783          	lw	a5,1120(s5)
    80202058:	46fa2023          	sw	a5,1120(s4)
  *(np->trapframe) = *(p->trapframe);
    8020205c:	060ab683          	ld	a3,96(s5)
    80202060:	87b6                	mv	a5,a3
    80202062:	060a3703          	ld	a4,96(s4)
    80202066:	12068693          	addi	a3,a3,288
    8020206a:	0007b803          	ld	a6,0(a5)
    8020206e:	6788                	ld	a0,8(a5)
    80202070:	6b8c                	ld	a1,16(a5)
    80202072:	6f90                	ld	a2,24(a5)
    80202074:	01073023          	sd	a6,0(a4)
    80202078:	e708                	sd	a0,8(a4)
    8020207a:	eb0c                	sd	a1,16(a4)
    8020207c:	ef10                	sd	a2,24(a4)
    8020207e:	02078793          	addi	a5,a5,32
    80202082:	02070713          	addi	a4,a4,32
    80202086:	fed792e3          	bne	a5,a3,8020206a <clone+0x64>
  uint64 stack = p->trapframe->a1;
    8020208a:	060ab783          	ld	a5,96(s5)
    8020208e:	7fbc                	ld	a5,120(a5)
  if(stack!=0){
    80202090:	cf81                	beqz	a5,802020a8 <clone+0xa2>
    uint64 fn = *((uint64 *)((char *)(p->trapframe->a1)));
    80202092:	6394                	ld	a3,0(a5)
    uint64 arg = *((uint64 *)((char *)(p->trapframe->a1) + 8));
    80202094:	6798                	ld	a4,8(a5)
    np->trapframe->sp = stack;
    80202096:	060a3603          	ld	a2,96(s4)
    8020209a:	fa1c                	sd	a5,48(a2)
    np->trapframe->epc = fn;
    8020209c:	060a3783          	ld	a5,96(s4)
    802020a0:	ef94                	sd	a3,24(a5)
    np->trapframe->a1 = arg;
    802020a2:	060a3783          	ld	a5,96(s4)
    802020a6:	ffb8                	sd	a4,120(a5)
  np->trapframe->a0 = 0;
    802020a8:	060a3783          	ld	a5,96(s4)
    802020ac:	0607b823          	sd	zero,112(a5)
  for(i = 0; i < NOFILE; i++)
    802020b0:	0d8a8493          	addi	s1,s5,216
    802020b4:	0d8a0913          	addi	s2,s4,216
    802020b8:	448a8993          	addi	s3,s5,1096
    802020bc:	a03d                	j	802020ea <clone+0xe4>
    freeproc(np);
    802020be:	8552                	mv	a0,s4
    802020c0:	00000097          	auipc	ra,0x0
    802020c4:	bea080e7          	jalr	-1046(ra) # 80201caa <freeproc>
    release(&np->lock);
    802020c8:	8552                	mv	a0,s4
    802020ca:	ffffe097          	auipc	ra,0xffffe
    802020ce:	67e080e7          	jalr	1662(ra) # 80200748 <release>
    return -1;
    802020d2:	54fd                	li	s1,-1
    802020d4:	a889                	j	80202126 <clone+0x120>
      np->ofile[i] = filedup(p->ofile[i]);
    802020d6:	00002097          	auipc	ra,0x2
    802020da:	ca0080e7          	jalr	-864(ra) # 80203d76 <filedup>
    802020de:	00a93023          	sd	a0,0(s2)
  for(i = 0; i < NOFILE; i++)
    802020e2:	04a1                	addi	s1,s1,8
    802020e4:	0921                	addi	s2,s2,8
    802020e6:	01348563          	beq	s1,s3,802020f0 <clone+0xea>
    if(p->ofile[i])
    802020ea:	6088                	ld	a0,0(s1)
    802020ec:	f56d                	bnez	a0,802020d6 <clone+0xd0>
    802020ee:	bfd5                	j	802020e2 <clone+0xdc>
  np->cwd = edup(p->cwd);
    802020f0:	448ab503          	ld	a0,1096(s5)
    802020f4:	00005097          	auipc	ra,0x5
    802020f8:	90c080e7          	jalr	-1780(ra) # 80206a00 <edup>
    802020fc:	44aa3423          	sd	a0,1096(s4)
  safestrcpy(np->name, p->name, sizeof(p->name));
    80202100:	4641                	li	a2,16
    80202102:	450a8593          	addi	a1,s5,1104
    80202106:	450a0513          	addi	a0,s4,1104
    8020210a:	ffffe097          	auipc	ra,0xffffe
    8020210e:	7d8080e7          	jalr	2008(ra) # 802008e2 <safestrcpy>
  pid = np->pid;
    80202112:	038a2483          	lw	s1,56(s4)
  np->state = RUNNABLE;
    80202116:	4789                	li	a5,2
    80202118:	00fa2c23          	sw	a5,24(s4)
  release(&np->lock);
    8020211c:	8552                	mv	a0,s4
    8020211e:	ffffe097          	auipc	ra,0xffffe
    80202122:	62a080e7          	jalr	1578(ra) # 80200748 <release>
}
    80202126:	8526                	mv	a0,s1
    80202128:	70e2                	ld	ra,56(sp)
    8020212a:	7442                	ld	s0,48(sp)
    8020212c:	74a2                	ld	s1,40(sp)
    8020212e:	7902                	ld	s2,32(sp)
    80202130:	69e2                	ld	s3,24(sp)
    80202132:	6a42                	ld	s4,16(sp)
    80202134:	6aa2                	ld	s5,8(sp)
    80202136:	6121                	addi	sp,sp,64
    80202138:	8082                	ret
    return -1;
    8020213a:	54fd                	li	s1,-1
    8020213c:	b7ed                	j	80202126 <clone+0x120>

000000008020213e <reparent>:
{
    8020213e:	7179                	addi	sp,sp,-48
    80202140:	f406                	sd	ra,40(sp)
    80202142:	f022                	sd	s0,32(sp)
    80202144:	ec26                	sd	s1,24(sp)
    80202146:	e84a                	sd	s2,16(sp)
    80202148:	e44e                	sd	s3,8(sp)
    8020214a:	e052                	sd	s4,0(sp)
    8020214c:	1800                	addi	s0,sp,48
    8020214e:	892a                	mv	s2,a0
  for(pp = proc; pp < &proc[NPROC]; pp++){
    80202150:	0003a497          	auipc	s1,0x3a
    80202154:	03048493          	addi	s1,s1,48 # 8023c180 <proc>
      pp->parent = initproc;
    80202158:	0003aa17          	auipc	s4,0x3a
    8020215c:	f08a0a13          	addi	s4,s4,-248 # 8023c060 <pid_lock>
  for(pp = proc; pp < &proc[NPROC]; pp++){
    80202160:	00048997          	auipc	s3,0x48
    80202164:	c7098993          	addi	s3,s3,-912 # 80249dd0 <bcache>
    80202168:	a029                	j	80202172 <reparent+0x34>
    8020216a:	46848493          	addi	s1,s1,1128
    8020216e:	03348363          	beq	s1,s3,80202194 <reparent+0x56>
    if(pp->parent == p){
    80202172:	709c                	ld	a5,32(s1)
    80202174:	ff279be3          	bne	a5,s2,8020216a <reparent+0x2c>
      acquire(&pp->lock);
    80202178:	8526                	mv	a0,s1
    8020217a:	ffffe097          	auipc	ra,0xffffe
    8020217e:	57a080e7          	jalr	1402(ra) # 802006f4 <acquire>
      pp->parent = initproc;
    80202182:	118a3783          	ld	a5,280(s4)
    80202186:	f09c                	sd	a5,32(s1)
      release(&pp->lock);
    80202188:	8526                	mv	a0,s1
    8020218a:	ffffe097          	auipc	ra,0xffffe
    8020218e:	5be080e7          	jalr	1470(ra) # 80200748 <release>
    80202192:	bfe1                	j	8020216a <reparent+0x2c>
}
    80202194:	70a2                	ld	ra,40(sp)
    80202196:	7402                	ld	s0,32(sp)
    80202198:	64e2                	ld	s1,24(sp)
    8020219a:	6942                	ld	s2,16(sp)
    8020219c:	69a2                	ld	s3,8(sp)
    8020219e:	6a02                	ld	s4,0(sp)
    802021a0:	6145                	addi	sp,sp,48
    802021a2:	8082                	ret

00000000802021a4 <scheduler>:
{
    802021a4:	715d                	addi	sp,sp,-80
    802021a6:	e486                	sd	ra,72(sp)
    802021a8:	e0a2                	sd	s0,64(sp)
    802021aa:	fc26                	sd	s1,56(sp)
    802021ac:	f84a                	sd	s2,48(sp)
    802021ae:	f44e                	sd	s3,40(sp)
    802021b0:	f052                	sd	s4,32(sp)
    802021b2:	ec56                	sd	s5,24(sp)
    802021b4:	e85a                	sd	s6,16(sp)
    802021b6:	e45e                	sd	s7,8(sp)
    802021b8:	e062                	sd	s8,0(sp)
    802021ba:	0880                	addi	s0,sp,80
    802021bc:	8792                	mv	a5,tp
  int id = r_tp();
    802021be:	2781                	sext.w	a5,a5
  c->proc = 0;
    802021c0:	00779b13          	slli	s6,a5,0x7
    802021c4:	0003a717          	auipc	a4,0x3a
    802021c8:	e9c70713          	addi	a4,a4,-356 # 8023c060 <pid_lock>
    802021cc:	975a                	add	a4,a4,s6
    802021ce:	00073c23          	sd	zero,24(a4)
        swtch(&c->context, &p->context);
    802021d2:	0003a717          	auipc	a4,0x3a
    802021d6:	eae70713          	addi	a4,a4,-338 # 8023c080 <cpus+0x8>
    802021da:	9b3a                	add	s6,s6,a4
        c->proc = p;
    802021dc:	079e                	slli	a5,a5,0x7
    802021de:	0003aa97          	auipc	s5,0x3a
    802021e2:	e82a8a93          	addi	s5,s5,-382 # 8023c060 <pid_lock>
    802021e6:	9abe                	add	s5,s5,a5
        w_satp(MAKE_SATP(p->kpagetable));
    802021e8:	5a7d                	li	s4,-1
    802021ea:	1a7e                	slli	s4,s4,0x3f
        w_satp(MAKE_SATP(kernel_pagetable));
    802021ec:	00031b97          	auipc	s7,0x31
    802021f0:	134bbb83          	ld	s7,308(s7) # 80233320 <_GLOBAL_OFFSET_TABLE_+0x48>
    802021f4:	a8a5                	j	8020226c <scheduler+0xc8>
      release(&p->lock);
    802021f6:	8526                	mv	a0,s1
    802021f8:	ffffe097          	auipc	ra,0xffffe
    802021fc:	550080e7          	jalr	1360(ra) # 80200748 <release>
    for(p = proc; p < &proc[NPROC]; p++) {
    80202200:	46848493          	addi	s1,s1,1128
    80202204:	05248a63          	beq	s1,s2,80202258 <scheduler+0xb4>
      acquire(&p->lock);
    80202208:	8526                	mv	a0,s1
    8020220a:	ffffe097          	auipc	ra,0xffffe
    8020220e:	4ea080e7          	jalr	1258(ra) # 802006f4 <acquire>
      if(p->state == RUNNABLE) {
    80202212:	4c9c                	lw	a5,24(s1)
    80202214:	ff3791e3          	bne	a5,s3,802021f6 <scheduler+0x52>
        p->state = RUNNING;
    80202218:	478d                	li	a5,3
    8020221a:	cc9c                	sw	a5,24(s1)
        c->proc = p;
    8020221c:	009abc23          	sd	s1,24(s5)
        w_satp(MAKE_SATP(p->kpagetable));
    80202220:	6cbc                	ld	a5,88(s1)
    80202222:	83b1                	srli	a5,a5,0xc
    80202224:	0147e7b3          	or	a5,a5,s4
  asm volatile("csrw satp, %0" : : "r" (x));
    80202228:	18079073          	csrw	satp,a5
  asm volatile("sfence.vma");
    8020222c:	12000073          	sfence.vma
        swtch(&c->context, &p->context);
    80202230:	06848593          	addi	a1,s1,104
    80202234:	855a                	mv	a0,s6
    80202236:	00000097          	auipc	ra,0x0
    8020223a:	62a080e7          	jalr	1578(ra) # 80202860 <swtch>
        w_satp(MAKE_SATP(kernel_pagetable));
    8020223e:	000bb783          	ld	a5,0(s7)
    80202242:	83b1                	srli	a5,a5,0xc
    80202244:	0147e7b3          	or	a5,a5,s4
  asm volatile("csrw satp, %0" : : "r" (x));
    80202248:	18079073          	csrw	satp,a5
  asm volatile("sfence.vma");
    8020224c:	12000073          	sfence.vma
        c->proc = 0;
    80202250:	000abc23          	sd	zero,24(s5)
        found = 1;
    80202254:	4c05                	li	s8,1
    80202256:	b745                	j	802021f6 <scheduler+0x52>
    if(found == 0) {
    80202258:	000c1a63          	bnez	s8,8020226c <scheduler+0xc8>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    8020225c:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() | SSTATUS_SIE);
    80202260:	0027e793          	ori	a5,a5,2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80202264:	10079073          	csrw	sstatus,a5
      asm volatile("wfi");
    80202268:	10500073          	wfi
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    8020226c:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() | SSTATUS_SIE);
    80202270:	0027e793          	ori	a5,a5,2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80202274:	10079073          	csrw	sstatus,a5
    int found = 0;
    80202278:	4c01                	li	s8,0
    for(p = proc; p < &proc[NPROC]; p++) {
    8020227a:	0003a497          	auipc	s1,0x3a
    8020227e:	f0648493          	addi	s1,s1,-250 # 8023c180 <proc>
      if(p->state == RUNNABLE) {
    80202282:	4989                	li	s3,2
    for(p = proc; p < &proc[NPROC]; p++) {
    80202284:	00048917          	auipc	s2,0x48
    80202288:	b4c90913          	addi	s2,s2,-1204 # 80249dd0 <bcache>
    8020228c:	bfb5                	j	80202208 <scheduler+0x64>

000000008020228e <sched>:
{
    8020228e:	7179                	addi	sp,sp,-48
    80202290:	f406                	sd	ra,40(sp)
    80202292:	f022                	sd	s0,32(sp)
    80202294:	ec26                	sd	s1,24(sp)
    80202296:	e84a                	sd	s2,16(sp)
    80202298:	e44e                	sd	s3,8(sp)
    8020229a:	1800                	addi	s0,sp,48
  struct proc *p = myproc();
    8020229c:	00000097          	auipc	ra,0x0
    802022a0:	83c080e7          	jalr	-1988(ra) # 80201ad8 <myproc>
    802022a4:	84aa                	mv	s1,a0
  if(!holding(&p->lock))
    802022a6:	ffffe097          	auipc	ra,0xffffe
    802022aa:	420080e7          	jalr	1056(ra) # 802006c6 <holding>
    802022ae:	c93d                	beqz	a0,80202324 <sched+0x96>
  asm volatile("mv %0, tp" : "=r" (x) );
    802022b0:	8792                	mv	a5,tp
  if(mycpu()->noff != 1)
    802022b2:	2781                	sext.w	a5,a5
    802022b4:	079e                	slli	a5,a5,0x7
    802022b6:	0003a717          	auipc	a4,0x3a
    802022ba:	daa70713          	addi	a4,a4,-598 # 8023c060 <pid_lock>
    802022be:	97ba                	add	a5,a5,a4
    802022c0:	0907a703          	lw	a4,144(a5)
    802022c4:	4785                	li	a5,1
    802022c6:	06f71763          	bne	a4,a5,80202334 <sched+0xa6>
  if(p->state == RUNNING)
    802022ca:	4c98                	lw	a4,24(s1)
    802022cc:	478d                	li	a5,3
    802022ce:	06f70b63          	beq	a4,a5,80202344 <sched+0xb6>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    802022d2:	100027f3          	csrr	a5,sstatus
  return (x & SSTATUS_SIE) != 0;
    802022d6:	8b89                	andi	a5,a5,2
  if(intr_get())
    802022d8:	efb5                	bnez	a5,80202354 <sched+0xc6>
  asm volatile("mv %0, tp" : "=r" (x) );
    802022da:	8792                	mv	a5,tp
  intena = mycpu()->intena;
    802022dc:	0003a917          	auipc	s2,0x3a
    802022e0:	d8490913          	addi	s2,s2,-636 # 8023c060 <pid_lock>
    802022e4:	2781                	sext.w	a5,a5
    802022e6:	079e                	slli	a5,a5,0x7
    802022e8:	97ca                	add	a5,a5,s2
    802022ea:	0947a983          	lw	s3,148(a5)
    802022ee:	8792                	mv	a5,tp
  swtch(&p->context, &mycpu()->context);
    802022f0:	2781                	sext.w	a5,a5
    802022f2:	079e                	slli	a5,a5,0x7
    802022f4:	0003a597          	auipc	a1,0x3a
    802022f8:	d8c58593          	addi	a1,a1,-628 # 8023c080 <cpus+0x8>
    802022fc:	95be                	add	a1,a1,a5
    802022fe:	06848513          	addi	a0,s1,104
    80202302:	00000097          	auipc	ra,0x0
    80202306:	55e080e7          	jalr	1374(ra) # 80202860 <swtch>
    8020230a:	8792                	mv	a5,tp
  mycpu()->intena = intena;
    8020230c:	2781                	sext.w	a5,a5
    8020230e:	079e                	slli	a5,a5,0x7
    80202310:	993e                	add	s2,s2,a5
    80202312:	09392a23          	sw	s3,148(s2)
}
    80202316:	70a2                	ld	ra,40(sp)
    80202318:	7402                	ld	s0,32(sp)
    8020231a:	64e2                	ld	s1,24(sp)
    8020231c:	6942                	ld	s2,16(sp)
    8020231e:	69a2                	ld	s3,8(sp)
    80202320:	6145                	addi	sp,sp,48
    80202322:	8082                	ret
    panic("sched p->lock");
    80202324:	00007517          	auipc	a0,0x7
    80202328:	3ac50513          	addi	a0,a0,940 # 802096d0 <digits+0x350>
    8020232c:	ffffe097          	auipc	ra,0xffffe
    80202330:	e18080e7          	jalr	-488(ra) # 80200144 <panic>
    panic("sched locks");
    80202334:	00007517          	auipc	a0,0x7
    80202338:	3ac50513          	addi	a0,a0,940 # 802096e0 <digits+0x360>
    8020233c:	ffffe097          	auipc	ra,0xffffe
    80202340:	e08080e7          	jalr	-504(ra) # 80200144 <panic>
    panic("sched running");
    80202344:	00007517          	auipc	a0,0x7
    80202348:	3ac50513          	addi	a0,a0,940 # 802096f0 <digits+0x370>
    8020234c:	ffffe097          	auipc	ra,0xffffe
    80202350:	df8080e7          	jalr	-520(ra) # 80200144 <panic>
    panic("sched interruptible");
    80202354:	00007517          	auipc	a0,0x7
    80202358:	3ac50513          	addi	a0,a0,940 # 80209700 <digits+0x380>
    8020235c:	ffffe097          	auipc	ra,0xffffe
    80202360:	de8080e7          	jalr	-536(ra) # 80200144 <panic>

0000000080202364 <exit>:
{
    80202364:	7179                	addi	sp,sp,-48
    80202366:	f406                	sd	ra,40(sp)
    80202368:	f022                	sd	s0,32(sp)
    8020236a:	ec26                	sd	s1,24(sp)
    8020236c:	e84a                	sd	s2,16(sp)
    8020236e:	e44e                	sd	s3,8(sp)
    80202370:	e052                	sd	s4,0(sp)
    80202372:	1800                	addi	s0,sp,48
    80202374:	8a2a                	mv	s4,a0
  struct proc *p = myproc();
    80202376:	fffff097          	auipc	ra,0xfffff
    8020237a:	762080e7          	jalr	1890(ra) # 80201ad8 <myproc>
    8020237e:	89aa                	mv	s3,a0
  if(p == initproc)
    80202380:	0003a797          	auipc	a5,0x3a
    80202384:	df87b783          	ld	a5,-520(a5) # 8023c178 <initproc>
    80202388:	0d850493          	addi	s1,a0,216
    8020238c:	44850913          	addi	s2,a0,1096
    80202390:	02a79363          	bne	a5,a0,802023b6 <exit+0x52>
    panic("init exiting");
    80202394:	00007517          	auipc	a0,0x7
    80202398:	38450513          	addi	a0,a0,900 # 80209718 <digits+0x398>
    8020239c:	ffffe097          	auipc	ra,0xffffe
    802023a0:	da8080e7          	jalr	-600(ra) # 80200144 <panic>
      fileclose(f);
    802023a4:	00002097          	auipc	ra,0x2
    802023a8:	a24080e7          	jalr	-1500(ra) # 80203dc8 <fileclose>
      p->ofile[fd] = 0;
    802023ac:	0004b023          	sd	zero,0(s1)
  for(int fd = 0; fd < NOFILE; fd++){
    802023b0:	04a1                	addi	s1,s1,8
    802023b2:	01248563          	beq	s1,s2,802023bc <exit+0x58>
    if(p->ofile[fd]){
    802023b6:	6088                	ld	a0,0(s1)
    802023b8:	f575                	bnez	a0,802023a4 <exit+0x40>
    802023ba:	bfdd                	j	802023b0 <exit+0x4c>
  eput(p->cwd);
    802023bc:	4489b503          	ld	a0,1096(s3)
    802023c0:	00005097          	auipc	ra,0x5
    802023c4:	922080e7          	jalr	-1758(ra) # 80206ce2 <eput>
  p->cwd = 0;
    802023c8:	4409b423          	sd	zero,1096(s3)
  acquire(&initproc->lock);
    802023cc:	0003a497          	auipc	s1,0x3a
    802023d0:	c9448493          	addi	s1,s1,-876 # 8023c060 <pid_lock>
    802023d4:	1184b503          	ld	a0,280(s1)
    802023d8:	ffffe097          	auipc	ra,0xffffe
    802023dc:	31c080e7          	jalr	796(ra) # 802006f4 <acquire>
  wakeup1(initproc);
    802023e0:	1184b503          	ld	a0,280(s1)
    802023e4:	fffff097          	auipc	ra,0xfffff
    802023e8:	4f8080e7          	jalr	1272(ra) # 802018dc <wakeup1>
  release(&initproc->lock);
    802023ec:	1184b503          	ld	a0,280(s1)
    802023f0:	ffffe097          	auipc	ra,0xffffe
    802023f4:	358080e7          	jalr	856(ra) # 80200748 <release>
  acquire(&p->lock);
    802023f8:	854e                	mv	a0,s3
    802023fa:	ffffe097          	auipc	ra,0xffffe
    802023fe:	2fa080e7          	jalr	762(ra) # 802006f4 <acquire>
  struct proc *original_parent = p->parent;
    80202402:	0209b483          	ld	s1,32(s3)
  release(&p->lock);
    80202406:	854e                	mv	a0,s3
    80202408:	ffffe097          	auipc	ra,0xffffe
    8020240c:	340080e7          	jalr	832(ra) # 80200748 <release>
  acquire(&original_parent->lock);
    80202410:	8526                	mv	a0,s1
    80202412:	ffffe097          	auipc	ra,0xffffe
    80202416:	2e2080e7          	jalr	738(ra) # 802006f4 <acquire>
  acquire(&p->lock);
    8020241a:	854e                	mv	a0,s3
    8020241c:	ffffe097          	auipc	ra,0xffffe
    80202420:	2d8080e7          	jalr	728(ra) # 802006f4 <acquire>
  reparent(p);
    80202424:	854e                	mv	a0,s3
    80202426:	00000097          	auipc	ra,0x0
    8020242a:	d18080e7          	jalr	-744(ra) # 8020213e <reparent>
  wakeup1(original_parent);
    8020242e:	8526                	mv	a0,s1
    80202430:	fffff097          	auipc	ra,0xfffff
    80202434:	4ac080e7          	jalr	1196(ra) # 802018dc <wakeup1>
  p->xstate = status;
    80202438:	0349aa23          	sw	s4,52(s3)
  p->state = ZOMBIE;
    8020243c:	4791                	li	a5,4
    8020243e:	00f9ac23          	sw	a5,24(s3)
  release(&original_parent->lock);
    80202442:	8526                	mv	a0,s1
    80202444:	ffffe097          	auipc	ra,0xffffe
    80202448:	304080e7          	jalr	772(ra) # 80200748 <release>
  sched();
    8020244c:	00000097          	auipc	ra,0x0
    80202450:	e42080e7          	jalr	-446(ra) # 8020228e <sched>
  panic("zombie exit");
    80202454:	00007517          	auipc	a0,0x7
    80202458:	2d450513          	addi	a0,a0,724 # 80209728 <digits+0x3a8>
    8020245c:	ffffe097          	auipc	ra,0xffffe
    80202460:	ce8080e7          	jalr	-792(ra) # 80200144 <panic>

0000000080202464 <yield>:
{
    80202464:	1101                	addi	sp,sp,-32
    80202466:	ec06                	sd	ra,24(sp)
    80202468:	e822                	sd	s0,16(sp)
    8020246a:	e426                	sd	s1,8(sp)
    8020246c:	1000                	addi	s0,sp,32
  struct proc *p = myproc();
    8020246e:	fffff097          	auipc	ra,0xfffff
    80202472:	66a080e7          	jalr	1642(ra) # 80201ad8 <myproc>
    80202476:	84aa                	mv	s1,a0
  acquire(&p->lock);
    80202478:	ffffe097          	auipc	ra,0xffffe
    8020247c:	27c080e7          	jalr	636(ra) # 802006f4 <acquire>
  p->state = RUNNABLE;
    80202480:	4789                	li	a5,2
    80202482:	cc9c                	sw	a5,24(s1)
  sched();
    80202484:	00000097          	auipc	ra,0x0
    80202488:	e0a080e7          	jalr	-502(ra) # 8020228e <sched>
  release(&p->lock);
    8020248c:	8526                	mv	a0,s1
    8020248e:	ffffe097          	auipc	ra,0xffffe
    80202492:	2ba080e7          	jalr	698(ra) # 80200748 <release>
}
    80202496:	60e2                	ld	ra,24(sp)
    80202498:	6442                	ld	s0,16(sp)
    8020249a:	64a2                	ld	s1,8(sp)
    8020249c:	6105                	addi	sp,sp,32
    8020249e:	8082                	ret

00000000802024a0 <sleep>:
{
    802024a0:	7179                	addi	sp,sp,-48
    802024a2:	f406                	sd	ra,40(sp)
    802024a4:	f022                	sd	s0,32(sp)
    802024a6:	ec26                	sd	s1,24(sp)
    802024a8:	e84a                	sd	s2,16(sp)
    802024aa:	e44e                	sd	s3,8(sp)
    802024ac:	1800                	addi	s0,sp,48
    802024ae:	89aa                	mv	s3,a0
    802024b0:	892e                	mv	s2,a1
  struct proc *p = myproc();
    802024b2:	fffff097          	auipc	ra,0xfffff
    802024b6:	626080e7          	jalr	1574(ra) # 80201ad8 <myproc>
    802024ba:	84aa                	mv	s1,a0
  if(lk != &p->lock){  //DOC: sleeplock0
    802024bc:	05250663          	beq	a0,s2,80202508 <sleep+0x68>
    acquire(&p->lock);  //DOC: sleeplock1
    802024c0:	ffffe097          	auipc	ra,0xffffe
    802024c4:	234080e7          	jalr	564(ra) # 802006f4 <acquire>
    release(lk);
    802024c8:	854a                	mv	a0,s2
    802024ca:	ffffe097          	auipc	ra,0xffffe
    802024ce:	27e080e7          	jalr	638(ra) # 80200748 <release>
  p->chan = chan;
    802024d2:	0334b423          	sd	s3,40(s1)
  p->state = SLEEPING;
    802024d6:	4785                	li	a5,1
    802024d8:	cc9c                	sw	a5,24(s1)
  sched();
    802024da:	00000097          	auipc	ra,0x0
    802024de:	db4080e7          	jalr	-588(ra) # 8020228e <sched>
  p->chan = 0;
    802024e2:	0204b423          	sd	zero,40(s1)
    release(&p->lock);
    802024e6:	8526                	mv	a0,s1
    802024e8:	ffffe097          	auipc	ra,0xffffe
    802024ec:	260080e7          	jalr	608(ra) # 80200748 <release>
    acquire(lk);
    802024f0:	854a                	mv	a0,s2
    802024f2:	ffffe097          	auipc	ra,0xffffe
    802024f6:	202080e7          	jalr	514(ra) # 802006f4 <acquire>
}
    802024fa:	70a2                	ld	ra,40(sp)
    802024fc:	7402                	ld	s0,32(sp)
    802024fe:	64e2                	ld	s1,24(sp)
    80202500:	6942                	ld	s2,16(sp)
    80202502:	69a2                	ld	s3,8(sp)
    80202504:	6145                	addi	sp,sp,48
    80202506:	8082                	ret
  p->chan = chan;
    80202508:	03353423          	sd	s3,40(a0)
  p->state = SLEEPING;
    8020250c:	4785                	li	a5,1
    8020250e:	cd1c                	sw	a5,24(a0)
  sched();
    80202510:	00000097          	auipc	ra,0x0
    80202514:	d7e080e7          	jalr	-642(ra) # 8020228e <sched>
  p->chan = 0;
    80202518:	0204b423          	sd	zero,40(s1)
  if(lk != &p->lock){
    8020251c:	bff9                	j	802024fa <sleep+0x5a>

000000008020251e <wait>:
{
    8020251e:	7159                	addi	sp,sp,-112
    80202520:	f486                	sd	ra,104(sp)
    80202522:	f0a2                	sd	s0,96(sp)
    80202524:	eca6                	sd	s1,88(sp)
    80202526:	e8ca                	sd	s2,80(sp)
    80202528:	e4ce                	sd	s3,72(sp)
    8020252a:	e0d2                	sd	s4,64(sp)
    8020252c:	fc56                	sd	s5,56(sp)
    8020252e:	f85a                	sd	s6,48(sp)
    80202530:	f45e                	sd	s7,40(sp)
    80202532:	f062                	sd	s8,32(sp)
    80202534:	ec66                	sd	s9,24(sp)
    80202536:	1880                	addi	s0,sp,112
    80202538:	8a2a                	mv	s4,a0
    8020253a:	8c2e                	mv	s8,a1
  struct proc *p = myproc();
    8020253c:	fffff097          	auipc	ra,0xfffff
    80202540:	59c080e7          	jalr	1436(ra) # 80201ad8 <myproc>
    80202544:	892a                	mv	s2,a0
  acquire(&p->lock);
    80202546:	ffffe097          	auipc	ra,0xffffe
    8020254a:	1ae080e7          	jalr	430(ra) # 802006f4 <acquire>
    havekids = 0;
    8020254e:	4c81                	li	s9,0
        if(np->state == ZOMBIE && (np->pid == upid || upid == -1)){
    80202550:	4a91                	li	s5,4
        havekids = 1;
    80202552:	4b05                	li	s6,1
        if(np->state == ZOMBIE && (np->pid == upid || upid == -1)){
    80202554:	5bfd                	li	s7,-1
    for(np = proc; np < &proc[NPROC]; np++){
    80202556:	00048997          	auipc	s3,0x48
    8020255a:	87a98993          	addi	s3,s3,-1926 # 80249dd0 <bcache>
    havekids = 0;
    8020255e:	8766                	mv	a4,s9
    for(np = proc; np < &proc[NPROC]; np++){
    80202560:	0003a497          	auipc	s1,0x3a
    80202564:	c2048493          	addi	s1,s1,-992 # 8023c180 <proc>
    80202568:	a03d                	j	80202596 <wait+0x78>
            release(&np->lock);
    8020256a:	8526                	mv	a0,s1
    8020256c:	ffffe097          	auipc	ra,0xffffe
    80202570:	1dc080e7          	jalr	476(ra) # 80200748 <release>
            release(&p->lock);
    80202574:	854a                	mv	a0,s2
    80202576:	ffffe097          	auipc	ra,0xffffe
    8020257a:	1d2080e7          	jalr	466(ra) # 80200748 <release>
            return -1;
    8020257e:	557d                	li	a0,-1
    80202580:	a841                	j	80202610 <wait+0xf2>
        release(&np->lock);
    80202582:	8526                	mv	a0,s1
    80202584:	ffffe097          	auipc	ra,0xffffe
    80202588:	1c4080e7          	jalr	452(ra) # 80200748 <release>
        havekids = 1;
    8020258c:	875a                	mv	a4,s6
    for(np = proc; np < &proc[NPROC]; np++){
    8020258e:	46848493          	addi	s1,s1,1128
    80202592:	07348563          	beq	s1,s3,802025fc <wait+0xde>
      if(np->parent == p){
    80202596:	709c                	ld	a5,32(s1)
    80202598:	ff279be3          	bne	a5,s2,8020258e <wait+0x70>
        acquire(&np->lock);
    8020259c:	8526                	mv	a0,s1
    8020259e:	ffffe097          	auipc	ra,0xffffe
    802025a2:	156080e7          	jalr	342(ra) # 802006f4 <acquire>
        if(np->state == ZOMBIE && (np->pid == upid || upid == -1)){
    802025a6:	4c9c                	lw	a5,24(s1)
    802025a8:	fd579de3          	bne	a5,s5,80202582 <wait+0x64>
    802025ac:	5c9c                	lw	a5,56(s1)
    802025ae:	01478563          	beq	a5,s4,802025b8 <wait+0x9a>
    802025b2:	fd7a18e3          	bne	s4,s7,80202582 <wait+0x64>
    802025b6:	8a3e                	mv	s4,a5
          status = np->xstate << 8; // note
    802025b8:	58dc                	lw	a5,52(s1)
    802025ba:	0087979b          	slliw	a5,a5,0x8
    802025be:	f8f42e23          	sw	a5,-100(s0)
          if(addr != 0 && copyout2(addr, (char *)&status, sizeof(status)) < 0) {
    802025c2:	000c0c63          	beqz	s8,802025da <wait+0xbc>
    802025c6:	4611                	li	a2,4
    802025c8:	f9c40593          	addi	a1,s0,-100
    802025cc:	8562                	mv	a0,s8
    802025ce:	fffff097          	auipc	ra,0xfffff
    802025d2:	e30080e7          	jalr	-464(ra) # 802013fe <copyout2>
    802025d6:	f8054ae3          	bltz	a0,8020256a <wait+0x4c>
          freeproc(np);
    802025da:	8526                	mv	a0,s1
    802025dc:	fffff097          	auipc	ra,0xfffff
    802025e0:	6ce080e7          	jalr	1742(ra) # 80201caa <freeproc>
          release(&np->lock);
    802025e4:	8526                	mv	a0,s1
    802025e6:	ffffe097          	auipc	ra,0xffffe
    802025ea:	162080e7          	jalr	354(ra) # 80200748 <release>
          release(&p->lock);
    802025ee:	854a                	mv	a0,s2
    802025f0:	ffffe097          	auipc	ra,0xffffe
    802025f4:	158080e7          	jalr	344(ra) # 80200748 <release>
          return pid;
    802025f8:	8552                	mv	a0,s4
    802025fa:	a819                	j	80202610 <wait+0xf2>
    if(!havekids || p->killed){
    802025fc:	c701                	beqz	a4,80202604 <wait+0xe6>
    802025fe:	03092783          	lw	a5,48(s2)
    80202602:	c785                	beqz	a5,8020262a <wait+0x10c>
      release(&p->lock);
    80202604:	854a                	mv	a0,s2
    80202606:	ffffe097          	auipc	ra,0xffffe
    8020260a:	142080e7          	jalr	322(ra) # 80200748 <release>
      return -1;
    8020260e:	557d                	li	a0,-1
}
    80202610:	70a6                	ld	ra,104(sp)
    80202612:	7406                	ld	s0,96(sp)
    80202614:	64e6                	ld	s1,88(sp)
    80202616:	6946                	ld	s2,80(sp)
    80202618:	69a6                	ld	s3,72(sp)
    8020261a:	6a06                	ld	s4,64(sp)
    8020261c:	7ae2                	ld	s5,56(sp)
    8020261e:	7b42                	ld	s6,48(sp)
    80202620:	7ba2                	ld	s7,40(sp)
    80202622:	7c02                	ld	s8,32(sp)
    80202624:	6ce2                	ld	s9,24(sp)
    80202626:	6165                	addi	sp,sp,112
    80202628:	8082                	ret
    sleep(p, &p->lock);  //DOC: wait-sleep
    8020262a:	85ca                	mv	a1,s2
    8020262c:	854a                	mv	a0,s2
    8020262e:	00000097          	auipc	ra,0x0
    80202632:	e72080e7          	jalr	-398(ra) # 802024a0 <sleep>
    havekids = 0;
    80202636:	b725                	j	8020255e <wait+0x40>

0000000080202638 <wakeup>:
{
    80202638:	7139                	addi	sp,sp,-64
    8020263a:	fc06                	sd	ra,56(sp)
    8020263c:	f822                	sd	s0,48(sp)
    8020263e:	f426                	sd	s1,40(sp)
    80202640:	f04a                	sd	s2,32(sp)
    80202642:	ec4e                	sd	s3,24(sp)
    80202644:	e852                	sd	s4,16(sp)
    80202646:	e456                	sd	s5,8(sp)
    80202648:	0080                	addi	s0,sp,64
    8020264a:	8a2a                	mv	s4,a0
  for(p = proc; p < &proc[NPROC]; p++) {
    8020264c:	0003a497          	auipc	s1,0x3a
    80202650:	b3448493          	addi	s1,s1,-1228 # 8023c180 <proc>
    if(p->state == SLEEPING && p->chan == chan) {
    80202654:	4985                	li	s3,1
      p->state = RUNNABLE;
    80202656:	4a89                	li	s5,2
  for(p = proc; p < &proc[NPROC]; p++) {
    80202658:	00047917          	auipc	s2,0x47
    8020265c:	77890913          	addi	s2,s2,1912 # 80249dd0 <bcache>
    80202660:	a811                	j	80202674 <wakeup+0x3c>
    release(&p->lock);
    80202662:	8526                	mv	a0,s1
    80202664:	ffffe097          	auipc	ra,0xffffe
    80202668:	0e4080e7          	jalr	228(ra) # 80200748 <release>
  for(p = proc; p < &proc[NPROC]; p++) {
    8020266c:	46848493          	addi	s1,s1,1128
    80202670:	03248063          	beq	s1,s2,80202690 <wakeup+0x58>
    acquire(&p->lock);
    80202674:	8526                	mv	a0,s1
    80202676:	ffffe097          	auipc	ra,0xffffe
    8020267a:	07e080e7          	jalr	126(ra) # 802006f4 <acquire>
    if(p->state == SLEEPING && p->chan == chan) {
    8020267e:	4c9c                	lw	a5,24(s1)
    80202680:	ff3791e3          	bne	a5,s3,80202662 <wakeup+0x2a>
    80202684:	749c                	ld	a5,40(s1)
    80202686:	fd479ee3          	bne	a5,s4,80202662 <wakeup+0x2a>
      p->state = RUNNABLE;
    8020268a:	0154ac23          	sw	s5,24(s1)
    8020268e:	bfd1                	j	80202662 <wakeup+0x2a>
}
    80202690:	70e2                	ld	ra,56(sp)
    80202692:	7442                	ld	s0,48(sp)
    80202694:	74a2                	ld	s1,40(sp)
    80202696:	7902                	ld	s2,32(sp)
    80202698:	69e2                	ld	s3,24(sp)
    8020269a:	6a42                	ld	s4,16(sp)
    8020269c:	6aa2                	ld	s5,8(sp)
    8020269e:	6121                	addi	sp,sp,64
    802026a0:	8082                	ret

00000000802026a2 <kill>:
// Kill the process with the given pid.
// The victim won't exit until it tries to return
// to user space (see usertrap() in trap.c).
int
kill(int pid)
{
    802026a2:	7179                	addi	sp,sp,-48
    802026a4:	f406                	sd	ra,40(sp)
    802026a6:	f022                	sd	s0,32(sp)
    802026a8:	ec26                	sd	s1,24(sp)
    802026aa:	e84a                	sd	s2,16(sp)
    802026ac:	e44e                	sd	s3,8(sp)
    802026ae:	1800                	addi	s0,sp,48
    802026b0:	892a                	mv	s2,a0
  struct proc *p;

  for(p = proc; p < &proc[NPROC]; p++){
    802026b2:	0003a497          	auipc	s1,0x3a
    802026b6:	ace48493          	addi	s1,s1,-1330 # 8023c180 <proc>
    802026ba:	00047997          	auipc	s3,0x47
    802026be:	71698993          	addi	s3,s3,1814 # 80249dd0 <bcache>
    acquire(&p->lock);
    802026c2:	8526                	mv	a0,s1
    802026c4:	ffffe097          	auipc	ra,0xffffe
    802026c8:	030080e7          	jalr	48(ra) # 802006f4 <acquire>
    if(p->pid == pid){
    802026cc:	5c9c                	lw	a5,56(s1)
    802026ce:	01278d63          	beq	a5,s2,802026e8 <kill+0x46>
        p->state = RUNNABLE;
      }
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
    802026d2:	8526                	mv	a0,s1
    802026d4:	ffffe097          	auipc	ra,0xffffe
    802026d8:	074080e7          	jalr	116(ra) # 80200748 <release>
  for(p = proc; p < &proc[NPROC]; p++){
    802026dc:	46848493          	addi	s1,s1,1128
    802026e0:	ff3491e3          	bne	s1,s3,802026c2 <kill+0x20>
  }
  return -1;
    802026e4:	557d                	li	a0,-1
    802026e6:	a821                	j	802026fe <kill+0x5c>
      p->killed = 1;
    802026e8:	4785                	li	a5,1
    802026ea:	d89c                	sw	a5,48(s1)
      if(p->state == SLEEPING){
    802026ec:	4c98                	lw	a4,24(s1)
    802026ee:	00f70f63          	beq	a4,a5,8020270c <kill+0x6a>
      release(&p->lock);
    802026f2:	8526                	mv	a0,s1
    802026f4:	ffffe097          	auipc	ra,0xffffe
    802026f8:	054080e7          	jalr	84(ra) # 80200748 <release>
      return 0;
    802026fc:	4501                	li	a0,0
}
    802026fe:	70a2                	ld	ra,40(sp)
    80202700:	7402                	ld	s0,32(sp)
    80202702:	64e2                	ld	s1,24(sp)
    80202704:	6942                	ld	s2,16(sp)
    80202706:	69a2                	ld	s3,8(sp)
    80202708:	6145                	addi	sp,sp,48
    8020270a:	8082                	ret
        p->state = RUNNABLE;
    8020270c:	4789                	li	a5,2
    8020270e:	cc9c                	sw	a5,24(s1)
    80202710:	b7cd                	j	802026f2 <kill+0x50>

0000000080202712 <either_copyout>:
// Copy to either a user address, or kernel address,
// depending on usr_dst.
// Returns 0 on success, -1 on error.
int
either_copyout(int user_dst, uint64 dst, void *src, uint64 len)
{
    80202712:	1101                	addi	sp,sp,-32
    80202714:	ec06                	sd	ra,24(sp)
    80202716:	e822                	sd	s0,16(sp)
    80202718:	e426                	sd	s1,8(sp)
    8020271a:	1000                	addi	s0,sp,32
    8020271c:	84aa                	mv	s1,a0
    8020271e:	852e                	mv	a0,a1
    80202720:	85b2                	mv	a1,a2
    80202722:	8636                	mv	a2,a3
  // struct proc *p = myproc();
  if(user_dst){
    80202724:	c891                	beqz	s1,80202738 <either_copyout+0x26>
    // return copyout(p->pagetable, dst, src, len);
    return copyout2(dst, src, len);
    80202726:	fffff097          	auipc	ra,0xfffff
    8020272a:	cd8080e7          	jalr	-808(ra) # 802013fe <copyout2>
  } else {
    memmove((char *)dst, src, len);
    return 0;
  }
}
    8020272e:	60e2                	ld	ra,24(sp)
    80202730:	6442                	ld	s0,16(sp)
    80202732:	64a2                	ld	s1,8(sp)
    80202734:	6105                	addi	sp,sp,32
    80202736:	8082                	ret
    memmove((char *)dst, src, len);
    80202738:	0006861b          	sext.w	a2,a3
    8020273c:	ffffe097          	auipc	ra,0xffffe
    80202740:	0b0080e7          	jalr	176(ra) # 802007ec <memmove>
    return 0;
    80202744:	8526                	mv	a0,s1
    80202746:	b7e5                	j	8020272e <either_copyout+0x1c>

0000000080202748 <either_copyin>:
// Copy from either a user address, or kernel address,
// depending on usr_src.
// Returns 0 on success, -1 on error.
int
either_copyin(void *dst, int user_src, uint64 src, uint64 len)
{
    80202748:	1101                	addi	sp,sp,-32
    8020274a:	ec06                	sd	ra,24(sp)
    8020274c:	e822                	sd	s0,16(sp)
    8020274e:	e426                	sd	s1,8(sp)
    80202750:	1000                	addi	s0,sp,32
    80202752:	84ae                	mv	s1,a1
    80202754:	85b2                	mv	a1,a2
    80202756:	8636                	mv	a2,a3
  // struct proc *p = myproc();
  if(user_src){
    80202758:	c891                	beqz	s1,8020276c <either_copyin+0x24>
    // return copyin(p->pagetable, dst, src, len);
    return copyin2(dst, src, len);
    8020275a:	fffff097          	auipc	ra,0xfffff
    8020275e:	d84080e7          	jalr	-636(ra) # 802014de <copyin2>
  } else {
    memmove(dst, (char*)src, len);
    return 0;
  }
}
    80202762:	60e2                	ld	ra,24(sp)
    80202764:	6442                	ld	s0,16(sp)
    80202766:	64a2                	ld	s1,8(sp)
    80202768:	6105                	addi	sp,sp,32
    8020276a:	8082                	ret
    memmove(dst, (char*)src, len);
    8020276c:	0006861b          	sext.w	a2,a3
    80202770:	ffffe097          	auipc	ra,0xffffe
    80202774:	07c080e7          	jalr	124(ra) # 802007ec <memmove>
    return 0;
    80202778:	8526                	mv	a0,s1
    8020277a:	b7e5                	j	80202762 <either_copyin+0x1a>

000000008020277c <procdump>:
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
    8020277c:	715d                	addi	sp,sp,-80
    8020277e:	e486                	sd	ra,72(sp)
    80202780:	e0a2                	sd	s0,64(sp)
    80202782:	fc26                	sd	s1,56(sp)
    80202784:	f84a                	sd	s2,48(sp)
    80202786:	f44e                	sd	s3,40(sp)
    80202788:	f052                	sd	s4,32(sp)
    8020278a:	ec56                	sd	s5,24(sp)
    8020278c:	e85a                	sd	s6,16(sp)
    8020278e:	e45e                	sd	s7,8(sp)
    80202790:	0880                	addi	s0,sp,80
  [ZOMBIE]    "zombie"
  };
  struct proc *p;
  char *state;

  printf("\nPID\tSTATE\tNAME\tMEM\n");
    80202792:	00007517          	auipc	a0,0x7
    80202796:	fae50513          	addi	a0,a0,-82 # 80209740 <digits+0x3c0>
    8020279a:	ffffe097          	auipc	ra,0xffffe
    8020279e:	9f4080e7          	jalr	-1548(ra) # 8020018e <printf>
  for(p = proc; p < &proc[NPROC]; p++){
    802027a2:	0003a497          	auipc	s1,0x3a
    802027a6:	e2e48493          	addi	s1,s1,-466 # 8023c5d0 <proc+0x450>
    802027aa:	00048917          	auipc	s2,0x48
    802027ae:	a7690913          	addi	s2,s2,-1418 # 8024a220 <bcache+0x450>
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
    802027b2:	4b11                	li	s6,4
      state = states[p->state];
    else
      state = "???";
    802027b4:	00007997          	auipc	s3,0x7
    802027b8:	f8498993          	addi	s3,s3,-124 # 80209738 <digits+0x3b8>
    printf("%d\t%s\t%s\t%d", p->pid, state, p->name, p->sz);
    802027bc:	00007a97          	auipc	s5,0x7
    802027c0:	f9ca8a93          	addi	s5,s5,-100 # 80209758 <digits+0x3d8>
    printf("\n");
    802027c4:	00007a17          	auipc	s4,0x7
    802027c8:	914a0a13          	addi	s4,s4,-1772 # 802090d8 <etext+0xd8>
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
    802027cc:	00009b97          	auipc	s7,0x9
    802027d0:	45cb8b93          	addi	s7,s7,1116 # 8020bc28 <states.0>
    802027d4:	a01d                	j	802027fa <procdump+0x7e>
    printf("%d\t%s\t%s\t%d", p->pid, state, p->name, p->sz);
    802027d6:	bf86b703          	ld	a4,-1032(a3)
    802027da:	be86a583          	lw	a1,-1048(a3)
    802027de:	8556                	mv	a0,s5
    802027e0:	ffffe097          	auipc	ra,0xffffe
    802027e4:	9ae080e7          	jalr	-1618(ra) # 8020018e <printf>
    printf("\n");
    802027e8:	8552                	mv	a0,s4
    802027ea:	ffffe097          	auipc	ra,0xffffe
    802027ee:	9a4080e7          	jalr	-1628(ra) # 8020018e <printf>
  for(p = proc; p < &proc[NPROC]; p++){
    802027f2:	46848493          	addi	s1,s1,1128
    802027f6:	03248263          	beq	s1,s2,8020281a <procdump+0x9e>
    if(p->state == UNUSED)
    802027fa:	86a6                	mv	a3,s1
    802027fc:	bc84a783          	lw	a5,-1080(s1)
    80202800:	dbed                	beqz	a5,802027f2 <procdump+0x76>
      state = "???";
    80202802:	864e                	mv	a2,s3
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
    80202804:	fcfb69e3          	bltu	s6,a5,802027d6 <procdump+0x5a>
    80202808:	02079713          	slli	a4,a5,0x20
    8020280c:	01d75793          	srli	a5,a4,0x1d
    80202810:	97de                	add	a5,a5,s7
    80202812:	6390                	ld	a2,0(a5)
    80202814:	f269                	bnez	a2,802027d6 <procdump+0x5a>
      state = "???";
    80202816:	864e                	mv	a2,s3
    80202818:	bf7d                	j	802027d6 <procdump+0x5a>
  }
}
    8020281a:	60a6                	ld	ra,72(sp)
    8020281c:	6406                	ld	s0,64(sp)
    8020281e:	74e2                	ld	s1,56(sp)
    80202820:	7942                	ld	s2,48(sp)
    80202822:	79a2                	ld	s3,40(sp)
    80202824:	7a02                	ld	s4,32(sp)
    80202826:	6ae2                	ld	s5,24(sp)
    80202828:	6b42                	ld	s6,16(sp)
    8020282a:	6ba2                	ld	s7,8(sp)
    8020282c:	6161                	addi	sp,sp,80
    8020282e:	8082                	ret

0000000080202830 <procnum>:

uint64
procnum(void)
{
    80202830:	1141                	addi	sp,sp,-16
    80202832:	e422                	sd	s0,8(sp)
    80202834:	0800                	addi	s0,sp,16
  int num = 0;
  struct proc *p;

  for (p = proc; p < &proc[NPROC]; p++) {
    80202836:	0003a797          	auipc	a5,0x3a
    8020283a:	94a78793          	addi	a5,a5,-1718 # 8023c180 <proc>
  int num = 0;
    8020283e:	4501                	li	a0,0
  for (p = proc; p < &proc[NPROC]; p++) {
    80202840:	00047697          	auipc	a3,0x47
    80202844:	59068693          	addi	a3,a3,1424 # 80249dd0 <bcache>
    80202848:	a029                	j	80202852 <procnum+0x22>
    8020284a:	46878793          	addi	a5,a5,1128
    8020284e:	00d78663          	beq	a5,a3,8020285a <procnum+0x2a>
    if (p->state != UNUSED) {
    80202852:	4f98                	lw	a4,24(a5)
    80202854:	db7d                	beqz	a4,8020284a <procnum+0x1a>
      num++;
    80202856:	2505                	addiw	a0,a0,1
    80202858:	bfcd                	j	8020284a <procnum+0x1a>
    }
  }

  return num;
}
    8020285a:	6422                	ld	s0,8(sp)
    8020285c:	0141                	addi	sp,sp,16
    8020285e:	8082                	ret

0000000080202860 <swtch>:
    80202860:	00153023          	sd	ra,0(a0)
    80202864:	00253423          	sd	sp,8(a0)
    80202868:	e900                	sd	s0,16(a0)
    8020286a:	ed04                	sd	s1,24(a0)
    8020286c:	03253023          	sd	s2,32(a0)
    80202870:	03353423          	sd	s3,40(a0)
    80202874:	03453823          	sd	s4,48(a0)
    80202878:	03553c23          	sd	s5,56(a0)
    8020287c:	05653023          	sd	s6,64(a0)
    80202880:	05753423          	sd	s7,72(a0)
    80202884:	05853823          	sd	s8,80(a0)
    80202888:	05953c23          	sd	s9,88(a0)
    8020288c:	07a53023          	sd	s10,96(a0)
    80202890:	07b53423          	sd	s11,104(a0)
    80202894:	0005b083          	ld	ra,0(a1)
    80202898:	0085b103          	ld	sp,8(a1)
    8020289c:	6980                	ld	s0,16(a1)
    8020289e:	6d84                	ld	s1,24(a1)
    802028a0:	0205b903          	ld	s2,32(a1)
    802028a4:	0285b983          	ld	s3,40(a1)
    802028a8:	0305ba03          	ld	s4,48(a1)
    802028ac:	0385ba83          	ld	s5,56(a1)
    802028b0:	0405bb03          	ld	s6,64(a1)
    802028b4:	0485bb83          	ld	s7,72(a1)
    802028b8:	0505bc03          	ld	s8,80(a1)
    802028bc:	0585bc83          	ld	s9,88(a1)
    802028c0:	0605bd03          	ld	s10,96(a1)
    802028c4:	0685bd83          	ld	s11,104(a1)
    802028c8:	8082                	ret

00000000802028ca <trapinithart>:
// }

// set up to take exceptions and traps while in the kernel.
void
trapinithart(void)
{
    802028ca:	1141                	addi	sp,sp,-16
    802028cc:	e406                	sd	ra,8(sp)
    802028ce:	e022                	sd	s0,0(sp)
    802028d0:	0800                	addi	s0,sp,16
  asm volatile("csrw stvec, %0" : : "r" (x));
    802028d2:	00031797          	auipc	a5,0x31
    802028d6:	a1e7b783          	ld	a5,-1506(a5) # 802332f0 <_GLOBAL_OFFSET_TABLE_+0x18>
    802028da:	10579073          	csrw	stvec,a5
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    802028de:	100027f3          	csrr	a5,sstatus
  w_stvec((uint64)kernelvec);
  w_sstatus(r_sstatus() | SSTATUS_SIE);
    802028e2:	0027e793          	ori	a5,a5,2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    802028e6:	10079073          	csrw	sstatus,a5
  asm volatile("csrr %0, sie" : "=r" (x) );
    802028ea:	104027f3          	csrr	a5,sie
  // enable supervisor-mode timer interrupts.
  w_sie(r_sie() | SIE_SEIE | SIE_SSIE | SIE_STIE);
    802028ee:	2227e793          	ori	a5,a5,546
  asm volatile("csrw sie, %0" : : "r" (x));
    802028f2:	10479073          	csrw	sie,a5
  set_next_timeout();
    802028f6:	00003097          	auipc	ra,0x3
    802028fa:	0c0080e7          	jalr	192(ra) # 802059b6 <set_next_timeout>
  #ifdef DEBUG
  printf("trapinithart\n");
    802028fe:	00007517          	auipc	a0,0x7
    80202902:	e9250513          	addi	a0,a0,-366 # 80209790 <digits+0x410>
    80202906:	ffffe097          	auipc	ra,0xffffe
    8020290a:	888080e7          	jalr	-1912(ra) # 8020018e <printf>
  #endif
}
    8020290e:	60a2                	ld	ra,8(sp)
    80202910:	6402                	ld	s0,0(sp)
    80202912:	0141                	addi	sp,sp,16
    80202914:	8082                	ret

0000000080202916 <usertrapret>:
//
// return to user space
//
void
usertrapret(void)
{
    80202916:	1141                	addi	sp,sp,-16
    80202918:	e406                	sd	ra,8(sp)
    8020291a:	e022                	sd	s0,0(sp)
    8020291c:	0800                	addi	s0,sp,16
  struct proc *p = myproc();
    8020291e:	fffff097          	auipc	ra,0xfffff
    80202922:	1ba080e7          	jalr	442(ra) # 80201ad8 <myproc>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202926:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() & ~SSTATUS_SIE);
    8020292a:	9bf5                	andi	a5,a5,-3
  asm volatile("csrw sstatus, %0" : : "r" (x));
    8020292c:	10079073          	csrw	sstatus,a5
  // kerneltrap() to usertrap(), so turn off interrupts until
  // we're back in user space, where usertrap() is correct.
  intr_off();

  // send syscalls, interrupts, and exceptions to trampoline.S
  w_stvec(TRAMPOLINE + (uservec - trampoline));
    80202930:	00031697          	auipc	a3,0x31
    80202934:	9b06b683          	ld	a3,-1616(a3) # 802332e0 <_GLOBAL_OFFSET_TABLE_+0x8>
    80202938:	00031717          	auipc	a4,0x31
    8020293c:	9d873703          	ld	a4,-1576(a4) # 80233310 <_GLOBAL_OFFSET_TABLE_+0x38>
    80202940:	8f15                	sub	a4,a4,a3
    80202942:	040007b7          	lui	a5,0x4000
    80202946:	17fd                	addi	a5,a5,-1 # 3ffffff <_entry-0x7c200001>
    80202948:	07b2                	slli	a5,a5,0xc
    8020294a:	973e                	add	a4,a4,a5
  asm volatile("csrw stvec, %0" : : "r" (x));
    8020294c:	10571073          	csrw	stvec,a4

  // set up trapframe values that uservec will need when
  // the process next re-enters the kernel.
  p->trapframe->kernel_satp = r_satp();         // kernel page table
    80202950:	7138                	ld	a4,96(a0)
  asm volatile("csrr %0, satp" : "=r" (x) );
    80202952:	18002673          	csrr	a2,satp
    80202956:	e310                	sd	a2,0(a4)
  p->trapframe->kernel_sp = p->kstack + PGSIZE; // process's kernel stack
    80202958:	7130                	ld	a2,96(a0)
    8020295a:	6138                	ld	a4,64(a0)
    8020295c:	6585                	lui	a1,0x1
    8020295e:	972e                	add	a4,a4,a1
    80202960:	e618                	sd	a4,8(a2)
  p->trapframe->kernel_trap = (uint64)usertrap;
    80202962:	7138                	ld	a4,96(a0)
    80202964:	00000617          	auipc	a2,0x0
    80202968:	0f460613          	addi	a2,a2,244 # 80202a58 <usertrap>
    8020296c:	eb10                	sd	a2,16(a4)
  p->trapframe->kernel_hartid = r_tp();         // hartid for cpuid()
    8020296e:	7138                	ld	a4,96(a0)
  asm volatile("mv %0, tp" : "=r" (x) );
    80202970:	8612                	mv	a2,tp
    80202972:	f310                	sd	a2,32(a4)
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202974:	10002773          	csrr	a4,sstatus
  // set up the registers that trampoline.S's sret will use
  // to get to user space.
  
  // set S Previous Privilege mode to User.
  unsigned long x = r_sstatus();
  x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
    80202978:	eff77713          	andi	a4,a4,-257
  x |= SSTATUS_SPIE; // enable interrupts in user mode
    8020297c:	02076713          	ori	a4,a4,32
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80202980:	10071073          	csrw	sstatus,a4
  w_sstatus(x);

  // set S Exception Program Counter to the saved user pc.
  w_sepc(p->trapframe->epc);
    80202984:	7138                	ld	a4,96(a0)
  asm volatile("csrw sepc, %0" : : "r" (x));
    80202986:	6f18                	ld	a4,24(a4)
    80202988:	14171073          	csrw	sepc,a4

  // tell trampoline.S the user page table to switch to.
  // printf("[usertrapret]p->pagetable: %p\n", p->pagetable);
  uint64 satp = MAKE_SATP(p->pagetable);
    8020298c:	692c                	ld	a1,80(a0)
    8020298e:	81b1                	srli	a1,a1,0xc

  // jump to trampoline.S at the top of memory, which 
  // switches to the user page table, restores user registers,
  // and switches to user mode with sret.
  uint64 fn = TRAMPOLINE + (userret - trampoline);
    80202990:	00031717          	auipc	a4,0x31
    80202994:	95873703          	ld	a4,-1704(a4) # 802332e8 <_GLOBAL_OFFSET_TABLE_+0x10>
    80202998:	8f15                	sub	a4,a4,a3
    8020299a:	97ba                	add	a5,a5,a4
  ((void (*)(uint64,uint64))fn)(TRAPFRAME, satp);
    8020299c:	577d                	li	a4,-1
    8020299e:	177e                	slli	a4,a4,0x3f
    802029a0:	8dd9                	or	a1,a1,a4
    802029a2:	02000537          	lui	a0,0x2000
    802029a6:	157d                	addi	a0,a0,-1 # 1ffffff <_entry-0x7e200001>
    802029a8:	0536                	slli	a0,a0,0xd
    802029aa:	9782                	jalr	a5
}
    802029ac:	60a2                	ld	ra,8(sp)
    802029ae:	6402                	ld	s0,0(sp)
    802029b0:	0141                	addi	sp,sp,16
    802029b2:	8082                	ret

00000000802029b4 <devintr>:
// Check if it's an external/software interrupt, 
// and handle it. 
// returns  2 if timer interrupt, 
//          1 if other device, 
//          0 if not recognized. 
int devintr(void) {
    802029b4:	1101                	addi	sp,sp,-32
    802029b6:	ec06                	sd	ra,24(sp)
    802029b8:	e822                	sd	s0,16(sp)
    802029ba:	e426                	sd	s1,8(sp)
    802029bc:	1000                	addi	s0,sp,32
  asm volatile("csrr %0, scause" : "=r" (x) );
    802029be:	14202773          	csrr	a4,scause
	uint64 scause = r_scause();

	#ifdef QEMU 
	// handle external interrupt 
	if ((0x8000000000000000L & scause) && 9 == (scause & 0xff)) 
    802029c2:	00074d63          	bltz	a4,802029dc <devintr+0x28>
		sbi_set_mie();
		#endif 

		return 1;
	}
	else if (0x8000000000000005L == scause) {
    802029c6:	57fd                	li	a5,-1
    802029c8:	17fe                	slli	a5,a5,0x3f
    802029ca:	0795                	addi	a5,a5,5
		timer_tick();
		return 2;
	}
	else { return 0;}
    802029cc:	4501                	li	a0,0
	else if (0x8000000000000005L == scause) {
    802029ce:	06f70f63          	beq	a4,a5,80202a4c <devintr+0x98>
}
    802029d2:	60e2                	ld	ra,24(sp)
    802029d4:	6442                	ld	s0,16(sp)
    802029d6:	64a2                	ld	s1,8(sp)
    802029d8:	6105                	addi	sp,sp,32
    802029da:	8082                	ret
	if ((0x8000000000000000L & scause) && 9 == (scause & 0xff)) 
    802029dc:	0ff77793          	zext.b	a5,a4
    802029e0:	46a5                	li	a3,9
    802029e2:	fed792e3          	bne	a5,a3,802029c6 <devintr+0x12>
		int irq = plic_claim();
    802029e6:	00005097          	auipc	ra,0x5
    802029ea:	ad8080e7          	jalr	-1320(ra) # 802074be <plic_claim>
    802029ee:	84aa                	mv	s1,a0
		if (UART_IRQ == irq) {
    802029f0:	47a9                	li	a5,10
    802029f2:	02f50163          	beq	a0,a5,80202a14 <devintr+0x60>
		else if (DISK_IRQ == irq) {
    802029f6:	4785                	li	a5,1
    802029f8:	04f50563          	beq	a0,a5,80202a42 <devintr+0x8e>
		return 1;
    802029fc:	4505                	li	a0,1
		else if (irq) {
    802029fe:	d8f1                	beqz	s1,802029d2 <devintr+0x1e>
			printf("unexpected interrupt irq = %d\n", irq);
    80202a00:	85a6                	mv	a1,s1
    80202a02:	00007517          	auipc	a0,0x7
    80202a06:	d9e50513          	addi	a0,a0,-610 # 802097a0 <digits+0x420>
    80202a0a:	ffffd097          	auipc	ra,0xffffd
    80202a0e:	784080e7          	jalr	1924(ra) # 8020018e <printf>
    80202a12:	a821                	j	80202a2a <devintr+0x76>
	return SBI_CALL_0(SBI_CONSOLE_GETCHAR);
    80202a14:	4501                	li	a0,0
    80202a16:	4581                	li	a1,0
    80202a18:	4601                	li	a2,0
    80202a1a:	4681                	li	a3,0
    80202a1c:	4889                	li	a7,2
    80202a1e:	00000073          	ecall
    80202a22:	2501                	sext.w	a0,a0
			if (-1 != c) {
    80202a24:	57fd                	li	a5,-1
    80202a26:	00f51963          	bne	a0,a5,80202a38 <devintr+0x84>
		if (irq) { plic_complete(irq);}
    80202a2a:	8526                	mv	a0,s1
    80202a2c:	00005097          	auipc	ra,0x5
    80202a30:	abc080e7          	jalr	-1348(ra) # 802074e8 <plic_complete>
		return 1;
    80202a34:	4505                	li	a0,1
    80202a36:	bf71                	j	802029d2 <devintr+0x1e>
				consoleintr(c);
    80202a38:	00005097          	auipc	ra,0x5
    80202a3c:	cc4080e7          	jalr	-828(ra) # 802076fc <consoleintr>
    80202a40:	b7ed                	j	80202a2a <devintr+0x76>
			disk_intr();
    80202a42:	00003097          	auipc	ra,0x3
    80202a46:	032080e7          	jalr	50(ra) # 80205a74 <disk_intr>
    80202a4a:	b7c5                	j	80202a2a <devintr+0x76>
		timer_tick();
    80202a4c:	00003097          	auipc	ra,0x3
    80202a50:	f8e080e7          	jalr	-114(ra) # 802059da <timer_tick>
		return 2;
    80202a54:	4509                	li	a0,2
    80202a56:	bfb5                	j	802029d2 <devintr+0x1e>

0000000080202a58 <usertrap>:
{
    80202a58:	1101                	addi	sp,sp,-32
    80202a5a:	ec06                	sd	ra,24(sp)
    80202a5c:	e822                	sd	s0,16(sp)
    80202a5e:	e426                	sd	s1,8(sp)
    80202a60:	e04a                	sd	s2,0(sp)
    80202a62:	1000                	addi	s0,sp,32
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202a64:	100027f3          	csrr	a5,sstatus
  if((r_sstatus() & SSTATUS_SPP) != 0)
    80202a68:	1007f793          	andi	a5,a5,256
    80202a6c:	e3ad                	bnez	a5,80202ace <usertrap+0x76>
  asm volatile("csrw stvec, %0" : : "r" (x));
    80202a6e:	00031797          	auipc	a5,0x31
    80202a72:	8827b783          	ld	a5,-1918(a5) # 802332f0 <_GLOBAL_OFFSET_TABLE_+0x18>
    80202a76:	10579073          	csrw	stvec,a5
  struct proc *p = myproc();
    80202a7a:	fffff097          	auipc	ra,0xfffff
    80202a7e:	05e080e7          	jalr	94(ra) # 80201ad8 <myproc>
    80202a82:	84aa                	mv	s1,a0
  p->trapframe->epc = r_sepc();
    80202a84:	713c                	ld	a5,96(a0)
  asm volatile("csrr %0, sepc" : "=r" (x) );
    80202a86:	14102773          	csrr	a4,sepc
    80202a8a:	ef98                	sd	a4,24(a5)
  asm volatile("csrr %0, scause" : "=r" (x) );
    80202a8c:	14202773          	csrr	a4,scause
  if(r_scause() == 8){
    80202a90:	47a1                	li	a5,8
    80202a92:	04f71c63          	bne	a4,a5,80202aea <usertrap+0x92>
    if(p->killed)
    80202a96:	591c                	lw	a5,48(a0)
    80202a98:	e3b9                	bnez	a5,80202ade <usertrap+0x86>
    p->trapframe->epc += 4;
    80202a9a:	70b8                	ld	a4,96(s1)
    80202a9c:	6f1c                	ld	a5,24(a4)
    80202a9e:	0791                	addi	a5,a5,4
    80202aa0:	ef1c                	sd	a5,24(a4)
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202aa2:	100027f3          	csrr	a5,sstatus
  w_sstatus(r_sstatus() | SSTATUS_SIE);
    80202aa6:	0027e793          	ori	a5,a5,2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80202aaa:	10079073          	csrw	sstatus,a5
    syscall();
    80202aae:	00000097          	auipc	ra,0x0
    80202ab2:	6fc080e7          	jalr	1788(ra) # 802031aa <syscall>
  if(p->killed)
    80202ab6:	589c                	lw	a5,48(s1)
    80202ab8:	ebd1                	bnez	a5,80202b4c <usertrap+0xf4>
  usertrapret();
    80202aba:	00000097          	auipc	ra,0x0
    80202abe:	e5c080e7          	jalr	-420(ra) # 80202916 <usertrapret>
}
    80202ac2:	60e2                	ld	ra,24(sp)
    80202ac4:	6442                	ld	s0,16(sp)
    80202ac6:	64a2                	ld	s1,8(sp)
    80202ac8:	6902                	ld	s2,0(sp)
    80202aca:	6105                	addi	sp,sp,32
    80202acc:	8082                	ret
    panic("usertrap: not from user mode");
    80202ace:	00007517          	auipc	a0,0x7
    80202ad2:	cf250513          	addi	a0,a0,-782 # 802097c0 <digits+0x440>
    80202ad6:	ffffd097          	auipc	ra,0xffffd
    80202ada:	66e080e7          	jalr	1646(ra) # 80200144 <panic>
      exit(-1);
    80202ade:	557d                	li	a0,-1
    80202ae0:	00000097          	auipc	ra,0x0
    80202ae4:	884080e7          	jalr	-1916(ra) # 80202364 <exit>
    80202ae8:	bf4d                	j	80202a9a <usertrap+0x42>
  else if((which_dev = devintr()) != 0){
    80202aea:	00000097          	auipc	ra,0x0
    80202aee:	eca080e7          	jalr	-310(ra) # 802029b4 <devintr>
    80202af2:	892a                	mv	s2,a0
    80202af4:	c501                	beqz	a0,80202afc <usertrap+0xa4>
  if(p->killed)
    80202af6:	589c                	lw	a5,48(s1)
    80202af8:	c3b1                	beqz	a5,80202b3c <usertrap+0xe4>
    80202afa:	a825                	j	80202b32 <usertrap+0xda>
  asm volatile("csrr %0, scause" : "=r" (x) );
    80202afc:	142025f3          	csrr	a1,scause
    printf("\nusertrap(): unexpected scause %p pid=%d %s\n", r_scause(), p->pid, p->name);
    80202b00:	45048693          	addi	a3,s1,1104
    80202b04:	5c90                	lw	a2,56(s1)
    80202b06:	00007517          	auipc	a0,0x7
    80202b0a:	cda50513          	addi	a0,a0,-806 # 802097e0 <digits+0x460>
    80202b0e:	ffffd097          	auipc	ra,0xffffd
    80202b12:	680080e7          	jalr	1664(ra) # 8020018e <printf>
  asm volatile("csrr %0, sepc" : "=r" (x) );
    80202b16:	141025f3          	csrr	a1,sepc
  asm volatile("csrr %0, stval" : "=r" (x) );
    80202b1a:	14302673          	csrr	a2,stval
    printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
    80202b1e:	00007517          	auipc	a0,0x7
    80202b22:	cf250513          	addi	a0,a0,-782 # 80209810 <digits+0x490>
    80202b26:	ffffd097          	auipc	ra,0xffffd
    80202b2a:	668080e7          	jalr	1640(ra) # 8020018e <printf>
    p->killed = 1;
    80202b2e:	4785                	li	a5,1
    80202b30:	d89c                	sw	a5,48(s1)
    exit(-1);
    80202b32:	557d                	li	a0,-1
    80202b34:	00000097          	auipc	ra,0x0
    80202b38:	830080e7          	jalr	-2000(ra) # 80202364 <exit>
  if(which_dev == 2)
    80202b3c:	4789                	li	a5,2
    80202b3e:	f6f91ee3          	bne	s2,a5,80202aba <usertrap+0x62>
    yield();
    80202b42:	00000097          	auipc	ra,0x0
    80202b46:	922080e7          	jalr	-1758(ra) # 80202464 <yield>
    80202b4a:	bf85                	j	80202aba <usertrap+0x62>
  int which_dev = 0;
    80202b4c:	4901                	li	s2,0
    80202b4e:	b7d5                	j	80202b32 <usertrap+0xda>

0000000080202b50 <kerneltrap>:
kerneltrap() {
    80202b50:	7179                	addi	sp,sp,-48
    80202b52:	f406                	sd	ra,40(sp)
    80202b54:	f022                	sd	s0,32(sp)
    80202b56:	ec26                	sd	s1,24(sp)
    80202b58:	e84a                	sd	s2,16(sp)
    80202b5a:	e44e                	sd	s3,8(sp)
    80202b5c:	1800                	addi	s0,sp,48
  asm volatile("csrr %0, sepc" : "=r" (x) );
    80202b5e:	14102973          	csrr	s2,sepc
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202b62:	100024f3          	csrr	s1,sstatus
  asm volatile("csrr %0, scause" : "=r" (x) );
    80202b66:	142029f3          	csrr	s3,scause
  if((sstatus & SSTATUS_SPP) == 0)
    80202b6a:	1004f793          	andi	a5,s1,256
    80202b6e:	cb85                	beqz	a5,80202b9e <kerneltrap+0x4e>
  asm volatile("csrr %0, sstatus" : "=r" (x) );
    80202b70:	100027f3          	csrr	a5,sstatus
  return (x & SSTATUS_SIE) != 0;
    80202b74:	8b89                	andi	a5,a5,2
  if(intr_get() != 0)
    80202b76:	ef85                	bnez	a5,80202bae <kerneltrap+0x5e>
  if((which_dev = devintr()) == 0){
    80202b78:	00000097          	auipc	ra,0x0
    80202b7c:	e3c080e7          	jalr	-452(ra) # 802029b4 <devintr>
    80202b80:	cd1d                	beqz	a0,80202bbe <kerneltrap+0x6e>
  if(which_dev == 2 && myproc() != 0 && myproc()->state == RUNNING) {
    80202b82:	4789                	li	a5,2
    80202b84:	08f50b63          	beq	a0,a5,80202c1a <kerneltrap+0xca>
  asm volatile("csrw sepc, %0" : : "r" (x));
    80202b88:	14191073          	csrw	sepc,s2
  asm volatile("csrw sstatus, %0" : : "r" (x));
    80202b8c:	10049073          	csrw	sstatus,s1
}
    80202b90:	70a2                	ld	ra,40(sp)
    80202b92:	7402                	ld	s0,32(sp)
    80202b94:	64e2                	ld	s1,24(sp)
    80202b96:	6942                	ld	s2,16(sp)
    80202b98:	69a2                	ld	s3,8(sp)
    80202b9a:	6145                	addi	sp,sp,48
    80202b9c:	8082                	ret
    panic("kerneltrap: not from supervisor mode");
    80202b9e:	00007517          	auipc	a0,0x7
    80202ba2:	c9250513          	addi	a0,a0,-878 # 80209830 <digits+0x4b0>
    80202ba6:	ffffd097          	auipc	ra,0xffffd
    80202baa:	59e080e7          	jalr	1438(ra) # 80200144 <panic>
    panic("kerneltrap: interrupts enabled");
    80202bae:	00007517          	auipc	a0,0x7
    80202bb2:	caa50513          	addi	a0,a0,-854 # 80209858 <digits+0x4d8>
    80202bb6:	ffffd097          	auipc	ra,0xffffd
    80202bba:	58e080e7          	jalr	1422(ra) # 80200144 <panic>
    printf("\nscause %p\n", scause);
    80202bbe:	85ce                	mv	a1,s3
    80202bc0:	00007517          	auipc	a0,0x7
    80202bc4:	cb850513          	addi	a0,a0,-840 # 80209878 <digits+0x4f8>
    80202bc8:	ffffd097          	auipc	ra,0xffffd
    80202bcc:	5c6080e7          	jalr	1478(ra) # 8020018e <printf>
  asm volatile("csrr %0, sepc" : "=r" (x) );
    80202bd0:	141025f3          	csrr	a1,sepc
  asm volatile("csrr %0, stval" : "=r" (x) );
    80202bd4:	14302673          	csrr	a2,stval
  asm volatile("mv %0, tp" : "=r" (x) );
    80202bd8:	8692                	mv	a3,tp
    printf("sepc=%p stval=%p hart=%d\n", r_sepc(), r_stval(), r_tp());
    80202bda:	00007517          	auipc	a0,0x7
    80202bde:	cae50513          	addi	a0,a0,-850 # 80209888 <digits+0x508>
    80202be2:	ffffd097          	auipc	ra,0xffffd
    80202be6:	5ac080e7          	jalr	1452(ra) # 8020018e <printf>
    struct proc *p = myproc();
    80202bea:	fffff097          	auipc	ra,0xfffff
    80202bee:	eee080e7          	jalr	-274(ra) # 80201ad8 <myproc>
    if (p != 0) {
    80202bf2:	cd01                	beqz	a0,80202c0a <kerneltrap+0xba>
      printf("pid: %d, name: %s\n", p->pid, p->name);
    80202bf4:	45050613          	addi	a2,a0,1104
    80202bf8:	5d0c                	lw	a1,56(a0)
    80202bfa:	00007517          	auipc	a0,0x7
    80202bfe:	cae50513          	addi	a0,a0,-850 # 802098a8 <digits+0x528>
    80202c02:	ffffd097          	auipc	ra,0xffffd
    80202c06:	58c080e7          	jalr	1420(ra) # 8020018e <printf>
    panic("kerneltrap");
    80202c0a:	00007517          	auipc	a0,0x7
    80202c0e:	cb650513          	addi	a0,a0,-842 # 802098c0 <digits+0x540>
    80202c12:	ffffd097          	auipc	ra,0xffffd
    80202c16:	532080e7          	jalr	1330(ra) # 80200144 <panic>
  if(which_dev == 2 && myproc() != 0 && myproc()->state == RUNNING) {
    80202c1a:	fffff097          	auipc	ra,0xfffff
    80202c1e:	ebe080e7          	jalr	-322(ra) # 80201ad8 <myproc>
    80202c22:	d13d                	beqz	a0,80202b88 <kerneltrap+0x38>
    80202c24:	fffff097          	auipc	ra,0xfffff
    80202c28:	eb4080e7          	jalr	-332(ra) # 80201ad8 <myproc>
    80202c2c:	4d18                	lw	a4,24(a0)
    80202c2e:	478d                	li	a5,3
    80202c30:	f4f71ce3          	bne	a4,a5,80202b88 <kerneltrap+0x38>
    yield();
    80202c34:	00000097          	auipc	ra,0x0
    80202c38:	830080e7          	jalr	-2000(ra) # 80202464 <yield>
    80202c3c:	b7b1                	j	80202b88 <kerneltrap+0x38>

0000000080202c3e <trapframedump>:

void trapframedump(struct trapframe *tf)
{
    80202c3e:	1101                	addi	sp,sp,-32
    80202c40:	ec06                	sd	ra,24(sp)
    80202c42:	e822                	sd	s0,16(sp)
    80202c44:	e426                	sd	s1,8(sp)
    80202c46:	1000                	addi	s0,sp,32
    80202c48:	84aa                	mv	s1,a0
  printf("a0: %p\t", tf->a0);
    80202c4a:	792c                	ld	a1,112(a0)
    80202c4c:	00007517          	auipc	a0,0x7
    80202c50:	c8450513          	addi	a0,a0,-892 # 802098d0 <digits+0x550>
    80202c54:	ffffd097          	auipc	ra,0xffffd
    80202c58:	53a080e7          	jalr	1338(ra) # 8020018e <printf>
  printf("a1: %p\t", tf->a1);
    80202c5c:	7cac                	ld	a1,120(s1)
    80202c5e:	00007517          	auipc	a0,0x7
    80202c62:	c7a50513          	addi	a0,a0,-902 # 802098d8 <digits+0x558>
    80202c66:	ffffd097          	auipc	ra,0xffffd
    80202c6a:	528080e7          	jalr	1320(ra) # 8020018e <printf>
  printf("a2: %p\t", tf->a2);
    80202c6e:	60cc                	ld	a1,128(s1)
    80202c70:	00007517          	auipc	a0,0x7
    80202c74:	c7050513          	addi	a0,a0,-912 # 802098e0 <digits+0x560>
    80202c78:	ffffd097          	auipc	ra,0xffffd
    80202c7c:	516080e7          	jalr	1302(ra) # 8020018e <printf>
  printf("a3: %p\n", tf->a3);
    80202c80:	64cc                	ld	a1,136(s1)
    80202c82:	00007517          	auipc	a0,0x7
    80202c86:	c6650513          	addi	a0,a0,-922 # 802098e8 <digits+0x568>
    80202c8a:	ffffd097          	auipc	ra,0xffffd
    80202c8e:	504080e7          	jalr	1284(ra) # 8020018e <printf>
  printf("a4: %p\t", tf->a4);
    80202c92:	68cc                	ld	a1,144(s1)
    80202c94:	00007517          	auipc	a0,0x7
    80202c98:	c5c50513          	addi	a0,a0,-932 # 802098f0 <digits+0x570>
    80202c9c:	ffffd097          	auipc	ra,0xffffd
    80202ca0:	4f2080e7          	jalr	1266(ra) # 8020018e <printf>
  printf("a5: %p\t", tf->a5);
    80202ca4:	6ccc                	ld	a1,152(s1)
    80202ca6:	00007517          	auipc	a0,0x7
    80202caa:	c5250513          	addi	a0,a0,-942 # 802098f8 <digits+0x578>
    80202cae:	ffffd097          	auipc	ra,0xffffd
    80202cb2:	4e0080e7          	jalr	1248(ra) # 8020018e <printf>
  printf("a6: %p\t", tf->a6);
    80202cb6:	70cc                	ld	a1,160(s1)
    80202cb8:	00007517          	auipc	a0,0x7
    80202cbc:	c4850513          	addi	a0,a0,-952 # 80209900 <digits+0x580>
    80202cc0:	ffffd097          	auipc	ra,0xffffd
    80202cc4:	4ce080e7          	jalr	1230(ra) # 8020018e <printf>
  printf("a7: %p\n", tf->a7);
    80202cc8:	74cc                	ld	a1,168(s1)
    80202cca:	00007517          	auipc	a0,0x7
    80202cce:	c3e50513          	addi	a0,a0,-962 # 80209908 <digits+0x588>
    80202cd2:	ffffd097          	auipc	ra,0xffffd
    80202cd6:	4bc080e7          	jalr	1212(ra) # 8020018e <printf>
  printf("t0: %p\t", tf->t0);
    80202cda:	64ac                	ld	a1,72(s1)
    80202cdc:	00007517          	auipc	a0,0x7
    80202ce0:	c3450513          	addi	a0,a0,-972 # 80209910 <digits+0x590>
    80202ce4:	ffffd097          	auipc	ra,0xffffd
    80202ce8:	4aa080e7          	jalr	1194(ra) # 8020018e <printf>
  printf("t1: %p\t", tf->t1);
    80202cec:	68ac                	ld	a1,80(s1)
    80202cee:	00007517          	auipc	a0,0x7
    80202cf2:	c2a50513          	addi	a0,a0,-982 # 80209918 <digits+0x598>
    80202cf6:	ffffd097          	auipc	ra,0xffffd
    80202cfa:	498080e7          	jalr	1176(ra) # 8020018e <printf>
  printf("t2: %p\t", tf->t2);
    80202cfe:	6cac                	ld	a1,88(s1)
    80202d00:	00007517          	auipc	a0,0x7
    80202d04:	c2050513          	addi	a0,a0,-992 # 80209920 <digits+0x5a0>
    80202d08:	ffffd097          	auipc	ra,0xffffd
    80202d0c:	486080e7          	jalr	1158(ra) # 8020018e <printf>
  printf("t3: %p\n", tf->t3);
    80202d10:	1004b583          	ld	a1,256(s1)
    80202d14:	00007517          	auipc	a0,0x7
    80202d18:	c1450513          	addi	a0,a0,-1004 # 80209928 <digits+0x5a8>
    80202d1c:	ffffd097          	auipc	ra,0xffffd
    80202d20:	472080e7          	jalr	1138(ra) # 8020018e <printf>
  printf("t4: %p\t", tf->t4);
    80202d24:	1084b583          	ld	a1,264(s1)
    80202d28:	00007517          	auipc	a0,0x7
    80202d2c:	c0850513          	addi	a0,a0,-1016 # 80209930 <digits+0x5b0>
    80202d30:	ffffd097          	auipc	ra,0xffffd
    80202d34:	45e080e7          	jalr	1118(ra) # 8020018e <printf>
  printf("t5: %p\t", tf->t5);
    80202d38:	1104b583          	ld	a1,272(s1)
    80202d3c:	00007517          	auipc	a0,0x7
    80202d40:	bfc50513          	addi	a0,a0,-1028 # 80209938 <digits+0x5b8>
    80202d44:	ffffd097          	auipc	ra,0xffffd
    80202d48:	44a080e7          	jalr	1098(ra) # 8020018e <printf>
  printf("t6: %p\t", tf->t6);
    80202d4c:	1184b583          	ld	a1,280(s1)
    80202d50:	00007517          	auipc	a0,0x7
    80202d54:	bf050513          	addi	a0,a0,-1040 # 80209940 <digits+0x5c0>
    80202d58:	ffffd097          	auipc	ra,0xffffd
    80202d5c:	436080e7          	jalr	1078(ra) # 8020018e <printf>
  printf("s0: %p\n", tf->s0);
    80202d60:	70ac                	ld	a1,96(s1)
    80202d62:	00007517          	auipc	a0,0x7
    80202d66:	be650513          	addi	a0,a0,-1050 # 80209948 <digits+0x5c8>
    80202d6a:	ffffd097          	auipc	ra,0xffffd
    80202d6e:	424080e7          	jalr	1060(ra) # 8020018e <printf>
  printf("s1: %p\t", tf->s1);
    80202d72:	74ac                	ld	a1,104(s1)
    80202d74:	00007517          	auipc	a0,0x7
    80202d78:	bdc50513          	addi	a0,a0,-1060 # 80209950 <digits+0x5d0>
    80202d7c:	ffffd097          	auipc	ra,0xffffd
    80202d80:	412080e7          	jalr	1042(ra) # 8020018e <printf>
  printf("s2: %p\t", tf->s2);
    80202d84:	78cc                	ld	a1,176(s1)
    80202d86:	00007517          	auipc	a0,0x7
    80202d8a:	bd250513          	addi	a0,a0,-1070 # 80209958 <digits+0x5d8>
    80202d8e:	ffffd097          	auipc	ra,0xffffd
    80202d92:	400080e7          	jalr	1024(ra) # 8020018e <printf>
  printf("s3: %p\t", tf->s3);
    80202d96:	7ccc                	ld	a1,184(s1)
    80202d98:	00007517          	auipc	a0,0x7
    80202d9c:	bc850513          	addi	a0,a0,-1080 # 80209960 <digits+0x5e0>
    80202da0:	ffffd097          	auipc	ra,0xffffd
    80202da4:	3ee080e7          	jalr	1006(ra) # 8020018e <printf>
  printf("s4: %p\n", tf->s4);
    80202da8:	60ec                	ld	a1,192(s1)
    80202daa:	00007517          	auipc	a0,0x7
    80202dae:	bbe50513          	addi	a0,a0,-1090 # 80209968 <digits+0x5e8>
    80202db2:	ffffd097          	auipc	ra,0xffffd
    80202db6:	3dc080e7          	jalr	988(ra) # 8020018e <printf>
  printf("s5: %p\t", tf->s5);
    80202dba:	64ec                	ld	a1,200(s1)
    80202dbc:	00007517          	auipc	a0,0x7
    80202dc0:	bb450513          	addi	a0,a0,-1100 # 80209970 <digits+0x5f0>
    80202dc4:	ffffd097          	auipc	ra,0xffffd
    80202dc8:	3ca080e7          	jalr	970(ra) # 8020018e <printf>
  printf("s6: %p\t", tf->s6);
    80202dcc:	68ec                	ld	a1,208(s1)
    80202dce:	00007517          	auipc	a0,0x7
    80202dd2:	baa50513          	addi	a0,a0,-1110 # 80209978 <digits+0x5f8>
    80202dd6:	ffffd097          	auipc	ra,0xffffd
    80202dda:	3b8080e7          	jalr	952(ra) # 8020018e <printf>
  printf("s7: %p\t", tf->s7);
    80202dde:	6cec                	ld	a1,216(s1)
    80202de0:	00007517          	auipc	a0,0x7
    80202de4:	ba050513          	addi	a0,a0,-1120 # 80209980 <digits+0x600>
    80202de8:	ffffd097          	auipc	ra,0xffffd
    80202dec:	3a6080e7          	jalr	934(ra) # 8020018e <printf>
  printf("s8: %p\n", tf->s8);
    80202df0:	70ec                	ld	a1,224(s1)
    80202df2:	00007517          	auipc	a0,0x7
    80202df6:	b9650513          	addi	a0,a0,-1130 # 80209988 <digits+0x608>
    80202dfa:	ffffd097          	auipc	ra,0xffffd
    80202dfe:	394080e7          	jalr	916(ra) # 8020018e <printf>
  printf("s9: %p\t", tf->s9);
    80202e02:	74ec                	ld	a1,232(s1)
    80202e04:	00007517          	auipc	a0,0x7
    80202e08:	b8c50513          	addi	a0,a0,-1140 # 80209990 <digits+0x610>
    80202e0c:	ffffd097          	auipc	ra,0xffffd
    80202e10:	382080e7          	jalr	898(ra) # 8020018e <printf>
  printf("s10: %p\t", tf->s10);
    80202e14:	78ec                	ld	a1,240(s1)
    80202e16:	00007517          	auipc	a0,0x7
    80202e1a:	b8250513          	addi	a0,a0,-1150 # 80209998 <digits+0x618>
    80202e1e:	ffffd097          	auipc	ra,0xffffd
    80202e22:	370080e7          	jalr	880(ra) # 8020018e <printf>
  printf("s11: %p\t", tf->s11);
    80202e26:	7cec                	ld	a1,248(s1)
    80202e28:	00007517          	auipc	a0,0x7
    80202e2c:	b8050513          	addi	a0,a0,-1152 # 802099a8 <digits+0x628>
    80202e30:	ffffd097          	auipc	ra,0xffffd
    80202e34:	35e080e7          	jalr	862(ra) # 8020018e <printf>
  printf("ra: %p\n", tf->ra);
    80202e38:	748c                	ld	a1,40(s1)
    80202e3a:	00007517          	auipc	a0,0x7
    80202e3e:	83e50513          	addi	a0,a0,-1986 # 80209678 <digits+0x2f8>
    80202e42:	ffffd097          	auipc	ra,0xffffd
    80202e46:	34c080e7          	jalr	844(ra) # 8020018e <printf>
  printf("sp: %p\t", tf->sp);
    80202e4a:	788c                	ld	a1,48(s1)
    80202e4c:	00007517          	auipc	a0,0x7
    80202e50:	b6c50513          	addi	a0,a0,-1172 # 802099b8 <digits+0x638>
    80202e54:	ffffd097          	auipc	ra,0xffffd
    80202e58:	33a080e7          	jalr	826(ra) # 8020018e <printf>
  printf("gp: %p\t", tf->gp);
    80202e5c:	7c8c                	ld	a1,56(s1)
    80202e5e:	00007517          	auipc	a0,0x7
    80202e62:	b6250513          	addi	a0,a0,-1182 # 802099c0 <digits+0x640>
    80202e66:	ffffd097          	auipc	ra,0xffffd
    80202e6a:	328080e7          	jalr	808(ra) # 8020018e <printf>
  printf("tp: %p\t", tf->tp);
    80202e6e:	60ac                	ld	a1,64(s1)
    80202e70:	00007517          	auipc	a0,0x7
    80202e74:	b5850513          	addi	a0,a0,-1192 # 802099c8 <digits+0x648>
    80202e78:	ffffd097          	auipc	ra,0xffffd
    80202e7c:	316080e7          	jalr	790(ra) # 8020018e <printf>
  printf("epc: %p\n", tf->epc);
    80202e80:	6c8c                	ld	a1,24(s1)
    80202e82:	00007517          	auipc	a0,0x7
    80202e86:	b4e50513          	addi	a0,a0,-1202 # 802099d0 <digits+0x650>
    80202e8a:	ffffd097          	auipc	ra,0xffffd
    80202e8e:	304080e7          	jalr	772(ra) # 8020018e <printf>
}
    80202e92:	60e2                	ld	ra,24(sp)
    80202e94:	6442                	ld	s0,16(sp)
    80202e96:	64a2                	ld	s1,8(sp)
    80202e98:	6105                	addi	sp,sp,32
    80202e9a:	8082                	ret

0000000080202e9c <argraw>:
	return strlen(buf);
}

static uint64
argraw(int n)
{
    80202e9c:	1101                	addi	sp,sp,-32
    80202e9e:	ec06                	sd	ra,24(sp)
    80202ea0:	e822                	sd	s0,16(sp)
    80202ea2:	e426                	sd	s1,8(sp)
    80202ea4:	1000                	addi	s0,sp,32
    80202ea6:	84aa                	mv	s1,a0
	struct proc *p = myproc();
    80202ea8:	fffff097          	auipc	ra,0xfffff
    80202eac:	c30080e7          	jalr	-976(ra) # 80201ad8 <myproc>
	switch (n) {
    80202eb0:	4795                	li	a5,5
    80202eb2:	0497e163          	bltu	a5,s1,80202ef4 <argraw+0x58>
    80202eb6:	048a                	slli	s1,s1,0x2
    80202eb8:	00007717          	auipc	a4,0x7
    80202ebc:	dcc70713          	addi	a4,a4,-564 # 80209c84 <digits+0x904>
    80202ec0:	94ba                	add	s1,s1,a4
    80202ec2:	409c                	lw	a5,0(s1)
    80202ec4:	97ba                	add	a5,a5,a4
    80202ec6:	8782                	jr	a5
	case 0:
		return p->trapframe->a0;
    80202ec8:	713c                	ld	a5,96(a0)
    80202eca:	7ba8                	ld	a0,112(a5)
	case 5:
		return p->trapframe->a5;
	}
	panic("argraw");
	return -1;
}
    80202ecc:	60e2                	ld	ra,24(sp)
    80202ece:	6442                	ld	s0,16(sp)
    80202ed0:	64a2                	ld	s1,8(sp)
    80202ed2:	6105                	addi	sp,sp,32
    80202ed4:	8082                	ret
		return p->trapframe->a1;
    80202ed6:	713c                	ld	a5,96(a0)
    80202ed8:	7fa8                	ld	a0,120(a5)
    80202eda:	bfcd                	j	80202ecc <argraw+0x30>
		return p->trapframe->a2;
    80202edc:	713c                	ld	a5,96(a0)
    80202ede:	63c8                	ld	a0,128(a5)
    80202ee0:	b7f5                	j	80202ecc <argraw+0x30>
		return p->trapframe->a3;
    80202ee2:	713c                	ld	a5,96(a0)
    80202ee4:	67c8                	ld	a0,136(a5)
    80202ee6:	b7dd                	j	80202ecc <argraw+0x30>
		return p->trapframe->a4;
    80202ee8:	713c                	ld	a5,96(a0)
    80202eea:	6bc8                	ld	a0,144(a5)
    80202eec:	b7c5                	j	80202ecc <argraw+0x30>
		return p->trapframe->a5;
    80202eee:	713c                	ld	a5,96(a0)
    80202ef0:	6fc8                	ld	a0,152(a5)
    80202ef2:	bfe9                	j	80202ecc <argraw+0x30>
	panic("argraw");
    80202ef4:	00007517          	auipc	a0,0x7
    80202ef8:	aec50513          	addi	a0,a0,-1300 # 802099e0 <digits+0x660>
    80202efc:	ffffd097          	auipc	ra,0xffffd
    80202f00:	248080e7          	jalr	584(ra) # 80200144 <panic>

0000000080202f04 <sys_uname>:
	return 0;
}

uint64
sys_uname(void)
{
    80202f04:	7161                	addi	sp,sp,-432
    80202f06:	f706                	sd	ra,424(sp)
    80202f08:	f322                	sd	s0,416(sp)
    80202f0a:	ef26                	sd	s1,408(sp)
    80202f0c:	eb4a                	sd	s2,400(sp)
    80202f0e:	1b00                	addi	s0,sp,432
	*ip = argraw(n);
    80202f10:	4501                	li	a0,0
    80202f12:	00000097          	auipc	ra,0x0
    80202f16:	f8a080e7          	jalr	-118(ra) # 80202e9c <argraw>
    80202f1a:	892a                	mv	s2,a0
	char *nodename = "Nodename: The worst os\n";
	char *release = "Release: The worst os you can experience. /··\\\n";
	char *version = "Version: Worst 1.0\n";
	char *machine = "Machine: RISC-V\n";
	char *domainname = "DomainName: Local domain";
	safestrcpy(info.sysname, sysname, strlen(sysname) + 1);
    80202f1c:	00007517          	auipc	a0,0x7
    80202f20:	acc50513          	addi	a0,a0,-1332 # 802099e8 <digits+0x668>
    80202f24:	ffffe097          	auipc	ra,0xffffe
    80202f28:	9f0080e7          	jalr	-1552(ra) # 80200914 <strlen>
    80202f2c:	0015061b          	addiw	a2,a0,1
    80202f30:	00007597          	auipc	a1,0x7
    80202f34:	ab858593          	addi	a1,a1,-1352 # 802099e8 <digits+0x668>
    80202f38:	e5840493          	addi	s1,s0,-424
    80202f3c:	8526                	mv	a0,s1
    80202f3e:	ffffe097          	auipc	ra,0xffffe
    80202f42:	9a4080e7          	jalr	-1628(ra) # 802008e2 <safestrcpy>
	safestrcpy(info.nodename, nodename, strlen(nodename) + 1);
    80202f46:	00007517          	auipc	a0,0x7
    80202f4a:	ad250513          	addi	a0,a0,-1326 # 80209a18 <digits+0x698>
    80202f4e:	ffffe097          	auipc	ra,0xffffe
    80202f52:	9c6080e7          	jalr	-1594(ra) # 80200914 <strlen>
    80202f56:	0015061b          	addiw	a2,a0,1
    80202f5a:	00007597          	auipc	a1,0x7
    80202f5e:	abe58593          	addi	a1,a1,-1346 # 80209a18 <digits+0x698>
    80202f62:	e9940513          	addi	a0,s0,-359
    80202f66:	ffffe097          	auipc	ra,0xffffe
    80202f6a:	97c080e7          	jalr	-1668(ra) # 802008e2 <safestrcpy>
	safestrcpy(info.release, release, strlen(release) + 1);
    80202f6e:	00007517          	auipc	a0,0x7
    80202f72:	ac250513          	addi	a0,a0,-1342 # 80209a30 <digits+0x6b0>
    80202f76:	ffffe097          	auipc	ra,0xffffe
    80202f7a:	99e080e7          	jalr	-1634(ra) # 80200914 <strlen>
    80202f7e:	0015061b          	addiw	a2,a0,1
    80202f82:	00007597          	auipc	a1,0x7
    80202f86:	aae58593          	addi	a1,a1,-1362 # 80209a30 <digits+0x6b0>
    80202f8a:	eda40513          	addi	a0,s0,-294
    80202f8e:	ffffe097          	auipc	ra,0xffffe
    80202f92:	954080e7          	jalr	-1708(ra) # 802008e2 <safestrcpy>
	safestrcpy(info.version, version, strlen(version) + 1);
    80202f96:	00007517          	auipc	a0,0x7
    80202f9a:	ad250513          	addi	a0,a0,-1326 # 80209a68 <digits+0x6e8>
    80202f9e:	ffffe097          	auipc	ra,0xffffe
    80202fa2:	976080e7          	jalr	-1674(ra) # 80200914 <strlen>
    80202fa6:	0015061b          	addiw	a2,a0,1
    80202faa:	00007597          	auipc	a1,0x7
    80202fae:	abe58593          	addi	a1,a1,-1346 # 80209a68 <digits+0x6e8>
    80202fb2:	f1b40513          	addi	a0,s0,-229
    80202fb6:	ffffe097          	auipc	ra,0xffffe
    80202fba:	92c080e7          	jalr	-1748(ra) # 802008e2 <safestrcpy>
	safestrcpy(info.machine, machine, strlen(machine) + 1);
    80202fbe:	00007517          	auipc	a0,0x7
    80202fc2:	ac250513          	addi	a0,a0,-1342 # 80209a80 <digits+0x700>
    80202fc6:	ffffe097          	auipc	ra,0xffffe
    80202fca:	94e080e7          	jalr	-1714(ra) # 80200914 <strlen>
    80202fce:	0015061b          	addiw	a2,a0,1
    80202fd2:	00007597          	auipc	a1,0x7
    80202fd6:	aae58593          	addi	a1,a1,-1362 # 80209a80 <digits+0x700>
    80202fda:	f5c40513          	addi	a0,s0,-164
    80202fde:	ffffe097          	auipc	ra,0xffffe
    80202fe2:	904080e7          	jalr	-1788(ra) # 802008e2 <safestrcpy>
	safestrcpy(info.domainname, domainname, strlen(domainname) + 1);
    80202fe6:	00007517          	auipc	a0,0x7
    80202fea:	ab250513          	addi	a0,a0,-1358 # 80209a98 <digits+0x718>
    80202fee:	ffffe097          	auipc	ra,0xffffe
    80202ff2:	926080e7          	jalr	-1754(ra) # 80200914 <strlen>
    80202ff6:	0015061b          	addiw	a2,a0,1
    80202ffa:	00007597          	auipc	a1,0x7
    80202ffe:	a9e58593          	addi	a1,a1,-1378 # 80209a98 <digits+0x718>
    80203002:	f9d40513          	addi	a0,s0,-99
    80203006:	ffffe097          	auipc	ra,0xffffe
    8020300a:	8dc080e7          	jalr	-1828(ra) # 802008e2 <safestrcpy>

	// if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0) {
	if (copyout2(addr, (char *)&info, sizeof(info)) < 0) {
    8020300e:	18600613          	li	a2,390
    80203012:	85a6                	mv	a1,s1
    80203014:	854a                	mv	a0,s2
    80203016:	ffffe097          	auipc	ra,0xffffe
    8020301a:	3e8080e7          	jalr	1000(ra) # 802013fe <copyout2>
		return -1;
	}

	return 0;
    8020301e:	957d                	srai	a0,a0,0x3f
    80203020:	70ba                	ld	ra,424(sp)
    80203022:	741a                	ld	s0,416(sp)
    80203024:	64fa                	ld	s1,408(sp)
    80203026:	695a                	ld	s2,400(sp)
    80203028:	615d                	addi	sp,sp,432
    8020302a:	8082                	ret

000000008020302c <sys_sysinfo>:
{
    8020302c:	7179                	addi	sp,sp,-48
    8020302e:	f406                	sd	ra,40(sp)
    80203030:	f022                	sd	s0,32(sp)
    80203032:	ec26                	sd	s1,24(sp)
    80203034:	1800                	addi	s0,sp,48
	*ip = argraw(n);
    80203036:	4501                	li	a0,0
    80203038:	00000097          	auipc	ra,0x0
    8020303c:	e64080e7          	jalr	-412(ra) # 80202e9c <argraw>
    80203040:	84aa                	mv	s1,a0
	info.freemem = freemem_amount();
    80203042:	ffffd097          	auipc	ra,0xffffd
    80203046:	5ac080e7          	jalr	1452(ra) # 802005ee <freemem_amount>
    8020304a:	fca43823          	sd	a0,-48(s0)
	info.nproc = procnum();
    8020304e:	fffff097          	auipc	ra,0xfffff
    80203052:	7e2080e7          	jalr	2018(ra) # 80202830 <procnum>
    80203056:	fca43c23          	sd	a0,-40(s0)
	if (copyout2(addr, (char *)&info, sizeof(info)) < 0) {
    8020305a:	4641                	li	a2,16
    8020305c:	fd040593          	addi	a1,s0,-48
    80203060:	8526                	mv	a0,s1
    80203062:	ffffe097          	auipc	ra,0xffffe
    80203066:	39c080e7          	jalr	924(ra) # 802013fe <copyout2>
}
    8020306a:	957d                	srai	a0,a0,0x3f
    8020306c:	70a2                	ld	ra,40(sp)
    8020306e:	7402                	ld	s0,32(sp)
    80203070:	64e2                	ld	s1,24(sp)
    80203072:	6145                	addi	sp,sp,48
    80203074:	8082                	ret

0000000080203076 <fetchaddr>:
{
    80203076:	1101                	addi	sp,sp,-32
    80203078:	ec06                	sd	ra,24(sp)
    8020307a:	e822                	sd	s0,16(sp)
    8020307c:	e426                	sd	s1,8(sp)
    8020307e:	e04a                	sd	s2,0(sp)
    80203080:	1000                	addi	s0,sp,32
    80203082:	84aa                	mv	s1,a0
    80203084:	892e                	mv	s2,a1
	struct proc *p = myproc();
    80203086:	fffff097          	auipc	ra,0xfffff
    8020308a:	a52080e7          	jalr	-1454(ra) # 80201ad8 <myproc>
	if(addr >= p->sz || addr+sizeof(uint64) > p->sz)
    8020308e:	653c                	ld	a5,72(a0)
    80203090:	02f4f763          	bgeu	s1,a5,802030be <fetchaddr+0x48>
    80203094:	00848713          	addi	a4,s1,8
    80203098:	02e7e563          	bltu	a5,a4,802030c2 <fetchaddr+0x4c>
	if(copyin2((char *)ip, addr, sizeof(*ip)) != 0)
    8020309c:	4621                	li	a2,8
    8020309e:	85a6                	mv	a1,s1
    802030a0:	854a                	mv	a0,s2
    802030a2:	ffffe097          	auipc	ra,0xffffe
    802030a6:	43c080e7          	jalr	1084(ra) # 802014de <copyin2>
    802030aa:	00a03533          	snez	a0,a0
    802030ae:	40a00533          	neg	a0,a0
}
    802030b2:	60e2                	ld	ra,24(sp)
    802030b4:	6442                	ld	s0,16(sp)
    802030b6:	64a2                	ld	s1,8(sp)
    802030b8:	6902                	ld	s2,0(sp)
    802030ba:	6105                	addi	sp,sp,32
    802030bc:	8082                	ret
		return -1;
    802030be:	557d                	li	a0,-1
    802030c0:	bfcd                	j	802030b2 <fetchaddr+0x3c>
    802030c2:	557d                	li	a0,-1
    802030c4:	b7fd                	j	802030b2 <fetchaddr+0x3c>

00000000802030c6 <fetchstr>:
{
    802030c6:	1101                	addi	sp,sp,-32
    802030c8:	ec06                	sd	ra,24(sp)
    802030ca:	e822                	sd	s0,16(sp)
    802030cc:	e426                	sd	s1,8(sp)
    802030ce:	1000                	addi	s0,sp,32
    802030d0:	84ae                	mv	s1,a1
	int err = copyinstr2(buf, addr, max);
    802030d2:	85aa                	mv	a1,a0
    802030d4:	8526                	mv	a0,s1
    802030d6:	ffffe097          	auipc	ra,0xffffe
    802030da:	50a080e7          	jalr	1290(ra) # 802015e0 <copyinstr2>
	if(err < 0)
    802030de:	00054763          	bltz	a0,802030ec <fetchstr+0x26>
	return strlen(buf);
    802030e2:	8526                	mv	a0,s1
    802030e4:	ffffe097          	auipc	ra,0xffffe
    802030e8:	830080e7          	jalr	-2000(ra) # 80200914 <strlen>
}
    802030ec:	60e2                	ld	ra,24(sp)
    802030ee:	6442                	ld	s0,16(sp)
    802030f0:	64a2                	ld	s1,8(sp)
    802030f2:	6105                	addi	sp,sp,32
    802030f4:	8082                	ret

00000000802030f6 <argint>:
{
    802030f6:	1101                	addi	sp,sp,-32
    802030f8:	ec06                	sd	ra,24(sp)
    802030fa:	e822                	sd	s0,16(sp)
    802030fc:	e426                	sd	s1,8(sp)
    802030fe:	1000                	addi	s0,sp,32
    80203100:	84ae                	mv	s1,a1
	*ip = argraw(n);
    80203102:	00000097          	auipc	ra,0x0
    80203106:	d9a080e7          	jalr	-614(ra) # 80202e9c <argraw>
    8020310a:	c088                	sw	a0,0(s1)
}
    8020310c:	4501                	li	a0,0
    8020310e:	60e2                	ld	ra,24(sp)
    80203110:	6442                	ld	s0,16(sp)
    80203112:	64a2                	ld	s1,8(sp)
    80203114:	6105                	addi	sp,sp,32
    80203116:	8082                	ret

0000000080203118 <sys_test_proc>:
sys_test_proc(void) {
    80203118:	1101                	addi	sp,sp,-32
    8020311a:	ec06                	sd	ra,24(sp)
    8020311c:	e822                	sd	s0,16(sp)
    8020311e:	1000                	addi	s0,sp,32
	argint(0, &n);
    80203120:	fec40593          	addi	a1,s0,-20
    80203124:	4501                	li	a0,0
    80203126:	00000097          	auipc	ra,0x0
    8020312a:	fd0080e7          	jalr	-48(ra) # 802030f6 <argint>
	printf("hello world from proc %d, hart %d, arg %d\n", myproc()->pid, r_tp(), n);
    8020312e:	fffff097          	auipc	ra,0xfffff
    80203132:	9aa080e7          	jalr	-1622(ra) # 80201ad8 <myproc>
    80203136:	8612                	mv	a2,tp
    80203138:	fec42683          	lw	a3,-20(s0)
    8020313c:	5d0c                	lw	a1,56(a0)
    8020313e:	00007517          	auipc	a0,0x7
    80203142:	97a50513          	addi	a0,a0,-1670 # 80209ab8 <digits+0x738>
    80203146:	ffffd097          	auipc	ra,0xffffd
    8020314a:	048080e7          	jalr	72(ra) # 8020018e <printf>
}
    8020314e:	4501                	li	a0,0
    80203150:	60e2                	ld	ra,24(sp)
    80203152:	6442                	ld	s0,16(sp)
    80203154:	6105                	addi	sp,sp,32
    80203156:	8082                	ret

0000000080203158 <argaddr>:
{
    80203158:	1101                	addi	sp,sp,-32
    8020315a:	ec06                	sd	ra,24(sp)
    8020315c:	e822                	sd	s0,16(sp)
    8020315e:	e426                	sd	s1,8(sp)
    80203160:	1000                	addi	s0,sp,32
    80203162:	84ae                	mv	s1,a1
	*ip = argraw(n);
    80203164:	00000097          	auipc	ra,0x0
    80203168:	d38080e7          	jalr	-712(ra) # 80202e9c <argraw>
    8020316c:	e088                	sd	a0,0(s1)
}
    8020316e:	4501                	li	a0,0
    80203170:	60e2                	ld	ra,24(sp)
    80203172:	6442                	ld	s0,16(sp)
    80203174:	64a2                	ld	s1,8(sp)
    80203176:	6105                	addi	sp,sp,32
    80203178:	8082                	ret

000000008020317a <argstr>:
{
    8020317a:	1101                	addi	sp,sp,-32
    8020317c:	ec06                	sd	ra,24(sp)
    8020317e:	e822                	sd	s0,16(sp)
    80203180:	e426                	sd	s1,8(sp)
    80203182:	e04a                	sd	s2,0(sp)
    80203184:	1000                	addi	s0,sp,32
    80203186:	84ae                	mv	s1,a1
    80203188:	8932                	mv	s2,a2
	*ip = argraw(n);
    8020318a:	00000097          	auipc	ra,0x0
    8020318e:	d12080e7          	jalr	-750(ra) # 80202e9c <argraw>
	return fetchstr(addr, buf, max);
    80203192:	864a                	mv	a2,s2
    80203194:	85a6                	mv	a1,s1
    80203196:	00000097          	auipc	ra,0x0
    8020319a:	f30080e7          	jalr	-208(ra) # 802030c6 <fetchstr>
}
    8020319e:	60e2                	ld	ra,24(sp)
    802031a0:	6442                	ld	s0,16(sp)
    802031a2:	64a2                	ld	s1,8(sp)
    802031a4:	6902                	ld	s2,0(sp)
    802031a6:	6105                	addi	sp,sp,32
    802031a8:	8082                	ret

00000000802031aa <syscall>:
{
    802031aa:	7179                	addi	sp,sp,-48
    802031ac:	f406                	sd	ra,40(sp)
    802031ae:	f022                	sd	s0,32(sp)
    802031b0:	ec26                	sd	s1,24(sp)
    802031b2:	e84a                	sd	s2,16(sp)
    802031b4:	e44e                	sd	s3,8(sp)
    802031b6:	1800                	addi	s0,sp,48
	struct proc *p = myproc();
    802031b8:	fffff097          	auipc	ra,0xfffff
    802031bc:	920080e7          	jalr	-1760(ra) # 80201ad8 <myproc>
    802031c0:	84aa                	mv	s1,a0
	num = p->trapframe->a7;
    802031c2:	06053983          	ld	s3,96(a0)
    802031c6:	0a89b783          	ld	a5,168(s3)
    802031ca:	0007891b          	sext.w	s2,a5
	if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    802031ce:	37fd                	addiw	a5,a5,-1
    802031d0:	6709                	lui	a4,0x2
    802031d2:	76570713          	addi	a4,a4,1893 # 2765 <_entry-0x801fd89b>
    802031d6:	04f76763          	bltu	a4,a5,80203224 <syscall+0x7a>
    802031da:	00391713          	slli	a4,s2,0x3
    802031de:	00009797          	auipc	a5,0x9
    802031e2:	a7278793          	addi	a5,a5,-1422 # 8020bc50 <syscalls>
    802031e6:	97ba                	add	a5,a5,a4
    802031e8:	639c                	ld	a5,0(a5)
    802031ea:	cf8d                	beqz	a5,80203224 <syscall+0x7a>
		p->trapframe->a0 = syscalls[num]();
    802031ec:	9782                	jalr	a5
    802031ee:	06a9b823          	sd	a0,112(s3)
		if ((p->tmask & (1 << num)) != 0) {
    802031f2:	4604a783          	lw	a5,1120(s1)
    802031f6:	4127d7bb          	sraw	a5,a5,s2
    802031fa:	8b85                	andi	a5,a5,1
    802031fc:	c3b9                	beqz	a5,80203242 <syscall+0x98>
		printf("pid %d: %s -> %d\n", p->pid, sysnames[num], p->trapframe->a0);
    802031fe:	70b8                	ld	a4,96(s1)
    80203200:	090e                	slli	s2,s2,0x3
    80203202:	0001c797          	auipc	a5,0x1c
    80203206:	58678793          	addi	a5,a5,1414 # 8021f788 <sysnames>
    8020320a:	97ca                	add	a5,a5,s2
    8020320c:	7b34                	ld	a3,112(a4)
    8020320e:	6390                	ld	a2,0(a5)
    80203210:	5c8c                	lw	a1,56(s1)
    80203212:	00007517          	auipc	a0,0x7
    80203216:	8d650513          	addi	a0,a0,-1834 # 80209ae8 <digits+0x768>
    8020321a:	ffffd097          	auipc	ra,0xffffd
    8020321e:	f74080e7          	jalr	-140(ra) # 8020018e <printf>
    80203222:	a005                	j	80203242 <syscall+0x98>
		printf("pid %d %s: unknown sys call %d\n",
    80203224:	86ca                	mv	a3,s2
    80203226:	45048613          	addi	a2,s1,1104
    8020322a:	5c8c                	lw	a1,56(s1)
    8020322c:	00007517          	auipc	a0,0x7
    80203230:	8d450513          	addi	a0,a0,-1836 # 80209b00 <digits+0x780>
    80203234:	ffffd097          	auipc	ra,0xffffd
    80203238:	f5a080e7          	jalr	-166(ra) # 8020018e <printf>
		p->trapframe->a0 = -1;
    8020323c:	70bc                	ld	a5,96(s1)
    8020323e:	577d                	li	a4,-1
    80203240:	fbb8                	sd	a4,112(a5)
}
    80203242:	70a2                	ld	ra,40(sp)
    80203244:	7402                	ld	s0,32(sp)
    80203246:	64e2                	ld	s1,24(sp)
    80203248:	6942                	ld	s2,16(sp)
    8020324a:	69a2                	ld	s3,8(sp)
    8020324c:	6145                	addi	sp,sp,48
    8020324e:	8082                	ret

0000000080203250 <sys_exec>:
extern int exec(char *path, char **argv);
extern int argfd(int n, int *pfd, struct file **pf);

uint64
sys_exec(void)
{
    80203250:	d9010113          	addi	sp,sp,-624
    80203254:	26113423          	sd	ra,616(sp)
    80203258:	26813023          	sd	s0,608(sp)
    8020325c:	24913c23          	sd	s1,600(sp)
    80203260:	25213823          	sd	s2,592(sp)
    80203264:	25313423          	sd	s3,584(sp)
    80203268:	25413023          	sd	s4,576(sp)
    8020326c:	23513c23          	sd	s5,568(sp)
    80203270:	23613823          	sd	s6,560(sp)
    80203274:	23713423          	sd	s7,552(sp)
    80203278:	1c80                	addi	s0,sp,624
  char path[FAT32_MAX_PATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
    8020327a:	10400613          	li	a2,260
    8020327e:	ea840593          	addi	a1,s0,-344
    80203282:	4501                	li	a0,0
    80203284:	00000097          	auipc	ra,0x0
    80203288:	ef6080e7          	jalr	-266(ra) # 8020317a <argstr>
    return -1;
    8020328c:	597d                	li	s2,-1
  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
    8020328e:	0c054c63          	bltz	a0,80203366 <sys_exec+0x116>
    80203292:	da040593          	addi	a1,s0,-608
    80203296:	4505                	li	a0,1
    80203298:	00000097          	auipc	ra,0x0
    8020329c:	ec0080e7          	jalr	-320(ra) # 80203158 <argaddr>
    802032a0:	0c054363          	bltz	a0,80203366 <sys_exec+0x116>
  }
  memset(argv, 0, sizeof(argv));
    802032a4:	da840a13          	addi	s4,s0,-600
    802032a8:	10000613          	li	a2,256
    802032ac:	4581                	li	a1,0
    802032ae:	8552                	mv	a0,s4
    802032b0:	ffffd097          	auipc	ra,0xffffd
    802032b4:	4e0080e7          	jalr	1248(ra) # 80200790 <memset>
  for(i=0;; i++){
    if(i >= NELEM(argv)){
    802032b8:	84d2                	mv	s1,s4
  memset(argv, 0, sizeof(argv));
    802032ba:	89d2                	mv	s3,s4
    802032bc:	4901                	li	s2,0
      goto bad;
    }
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
    802032be:	d9840a93          	addi	s5,s0,-616
    if(i >= NELEM(argv)){
    802032c2:	02000b13          	li	s6,32
    802032c6:	00090b9b          	sext.w	s7,s2
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
    802032ca:	00391513          	slli	a0,s2,0x3
    802032ce:	85d6                	mv	a1,s5
    802032d0:	da043783          	ld	a5,-608(s0)
    802032d4:	953e                	add	a0,a0,a5
    802032d6:	00000097          	auipc	ra,0x0
    802032da:	da0080e7          	jalr	-608(ra) # 80203076 <fetchaddr>
    802032de:	02054a63          	bltz	a0,80203312 <sys_exec+0xc2>
      goto bad;
    }
    if(uarg == 0){
    802032e2:	d9843783          	ld	a5,-616(s0)
    802032e6:	c3b9                	beqz	a5,8020332c <sys_exec+0xdc>
      argv[i] = 0;
      break;
    }
    argv[i] = kalloc();
    802032e8:	ffffd097          	auipc	ra,0xffffd
    802032ec:	2a0080e7          	jalr	672(ra) # 80200588 <kalloc>
    802032f0:	85aa                	mv	a1,a0
    802032f2:	00a9b023          	sd	a0,0(s3)
    if(argv[i] == 0)
    802032f6:	cd11                	beqz	a0,80203312 <sys_exec+0xc2>
      goto bad;
    if(fetchstr(uarg, argv[i], PGSIZE) < 0)
    802032f8:	6605                	lui	a2,0x1
    802032fa:	d9843503          	ld	a0,-616(s0)
    802032fe:	00000097          	auipc	ra,0x0
    80203302:	dc8080e7          	jalr	-568(ra) # 802030c6 <fetchstr>
    80203306:	00054663          	bltz	a0,80203312 <sys_exec+0xc2>
    if(i >= NELEM(argv)){
    8020330a:	0905                	addi	s2,s2,1
    8020330c:	09a1                	addi	s3,s3,8
    8020330e:	fb691ce3          	bne	s2,s6,802032c6 <sys_exec+0x76>
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    80203312:	100a0a13          	addi	s4,s4,256
    80203316:	6088                	ld	a0,0(s1)
    80203318:	c531                	beqz	a0,80203364 <sys_exec+0x114>
    kfree(argv[i]);
    8020331a:	ffffd097          	auipc	ra,0xffffd
    8020331e:	126080e7          	jalr	294(ra) # 80200440 <kfree>
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    80203322:	04a1                	addi	s1,s1,8
    80203324:	ff4499e3          	bne	s1,s4,80203316 <sys_exec+0xc6>
  return -1;
    80203328:	597d                	li	s2,-1
    8020332a:	a835                	j	80203366 <sys_exec+0x116>
      argv[i] = 0;
    8020332c:	0b8e                	slli	s7,s7,0x3
    8020332e:	fb0b8793          	addi	a5,s7,-80
    80203332:	00878bb3          	add	s7,a5,s0
    80203336:	de0bbc23          	sd	zero,-520(s7)
  int ret = exec(path, argv);
    8020333a:	da840593          	addi	a1,s0,-600
    8020333e:	ea840513          	addi	a0,s0,-344
    80203342:	00001097          	auipc	ra,0x1
    80203346:	12c080e7          	jalr	300(ra) # 8020446e <exec>
    8020334a:	892a                	mv	s2,a0
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    8020334c:	100a0a13          	addi	s4,s4,256
    80203350:	6088                	ld	a0,0(s1)
    80203352:	c911                	beqz	a0,80203366 <sys_exec+0x116>
    kfree(argv[i]);
    80203354:	ffffd097          	auipc	ra,0xffffd
    80203358:	0ec080e7          	jalr	236(ra) # 80200440 <kfree>
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    8020335c:	04a1                	addi	s1,s1,8
    8020335e:	ff4499e3          	bne	s1,s4,80203350 <sys_exec+0x100>
    80203362:	a011                	j	80203366 <sys_exec+0x116>
  return -1;
    80203364:	597d                	li	s2,-1
}
    80203366:	854a                	mv	a0,s2
    80203368:	26813083          	ld	ra,616(sp)
    8020336c:	26013403          	ld	s0,608(sp)
    80203370:	25813483          	ld	s1,600(sp)
    80203374:	25013903          	ld	s2,592(sp)
    80203378:	24813983          	ld	s3,584(sp)
    8020337c:	24013a03          	ld	s4,576(sp)
    80203380:	23813a83          	ld	s5,568(sp)
    80203384:	23013b03          	ld	s6,560(sp)
    80203388:	22813b83          	ld	s7,552(sp)
    8020338c:	27010113          	addi	sp,sp,624
    80203390:	8082                	ret

0000000080203392 <sys_exit>:

uint64
sys_exit(void)
{
    80203392:	1101                	addi	sp,sp,-32
    80203394:	ec06                	sd	ra,24(sp)
    80203396:	e822                	sd	s0,16(sp)
    80203398:	1000                	addi	s0,sp,32
  int n;
  if(argint(0, &n) < 0)
    8020339a:	fec40593          	addi	a1,s0,-20
    8020339e:	4501                	li	a0,0
    802033a0:	00000097          	auipc	ra,0x0
    802033a4:	d56080e7          	jalr	-682(ra) # 802030f6 <argint>
    return -1;
    802033a8:	57fd                	li	a5,-1
  if(argint(0, &n) < 0)
    802033aa:	00054963          	bltz	a0,802033bc <sys_exit+0x2a>
  exit(n);
    802033ae:	fec42503          	lw	a0,-20(s0)
    802033b2:	fffff097          	auipc	ra,0xfffff
    802033b6:	fb2080e7          	jalr	-78(ra) # 80202364 <exit>
  return 0;  // not reached
    802033ba:	4781                	li	a5,0
}
    802033bc:	853e                	mv	a0,a5
    802033be:	60e2                	ld	ra,24(sp)
    802033c0:	6442                	ld	s0,16(sp)
    802033c2:	6105                	addi	sp,sp,32
    802033c4:	8082                	ret

00000000802033c6 <sys_getpid>:

uint64
sys_getpid(void)
{
    802033c6:	1141                	addi	sp,sp,-16
    802033c8:	e406                	sd	ra,8(sp)
    802033ca:	e022                	sd	s0,0(sp)
    802033cc:	0800                	addi	s0,sp,16
  return myproc()->pid;
    802033ce:	ffffe097          	auipc	ra,0xffffe
    802033d2:	70a080e7          	jalr	1802(ra) # 80201ad8 <myproc>
}
    802033d6:	5d08                	lw	a0,56(a0)
    802033d8:	60a2                	ld	ra,8(sp)
    802033da:	6402                	ld	s0,0(sp)
    802033dc:	0141                	addi	sp,sp,16
    802033de:	8082                	ret

00000000802033e0 <sys_fork>:

uint64
sys_fork(void)
{
    802033e0:	1141                	addi	sp,sp,-16
    802033e2:	e406                	sd	ra,8(sp)
    802033e4:	e022                	sd	s0,0(sp)
    802033e6:	0800                	addi	s0,sp,16
  return fork();
    802033e8:	fffff097          	auipc	ra,0xfffff
    802033ec:	b06080e7          	jalr	-1274(ra) # 80201eee <fork>
}
    802033f0:	60a2                	ld	ra,8(sp)
    802033f2:	6402                	ld	s0,0(sp)
    802033f4:	0141                	addi	sp,sp,16
    802033f6:	8082                	ret

00000000802033f8 <sys_wait>:

uint64
sys_wait(void)
{
    802033f8:	1101                	addi	sp,sp,-32
    802033fa:	ec06                	sd	ra,24(sp)
    802033fc:	e822                	sd	s0,16(sp)
    802033fe:	1000                	addi	s0,sp,32
  uint64 p;
  if(argaddr(0, &p) < 0)
    80203400:	fe840593          	addi	a1,s0,-24
    80203404:	4501                	li	a0,0
    80203406:	00000097          	auipc	ra,0x0
    8020340a:	d52080e7          	jalr	-686(ra) # 80203158 <argaddr>
    8020340e:	87aa                	mv	a5,a0
    return -1;
    80203410:	557d                	li	a0,-1
  if(argaddr(0, &p) < 0)
    80203412:	0007c963          	bltz	a5,80203424 <sys_wait+0x2c>
  
  return wait(-1, p, 0);
    80203416:	4601                	li	a2,0
    80203418:	fe843583          	ld	a1,-24(s0)
    8020341c:	fffff097          	auipc	ra,0xfffff
    80203420:	102080e7          	jalr	258(ra) # 8020251e <wait>
}
    80203424:	60e2                	ld	ra,24(sp)
    80203426:	6442                	ld	s0,16(sp)
    80203428:	6105                	addi	sp,sp,32
    8020342a:	8082                	ret

000000008020342c <sys_waitpid>:

uint64
sys_waitpid(void)
{
    8020342c:	1101                	addi	sp,sp,-32
    8020342e:	ec06                	sd	ra,24(sp)
    80203430:	e822                	sd	s0,16(sp)
    80203432:	1000                	addi	s0,sp,32
  uint64 p;
  int pid, options;
  if(argint(0, &pid) < 0 || argaddr(1, &p) < 0 || argint(2, &options)) // 注意看测试用例的syscall.c！！
    80203434:	fe440593          	addi	a1,s0,-28
    80203438:	4501                	li	a0,0
    8020343a:	00000097          	auipc	ra,0x0
    8020343e:	cbc080e7          	jalr	-836(ra) # 802030f6 <argint>
    return -1;
    80203442:	57fd                	li	a5,-1
  if(argint(0, &pid) < 0 || argaddr(1, &p) < 0 || argint(2, &options)) // 注意看测试用例的syscall.c！！
    80203444:	04054063          	bltz	a0,80203484 <sys_waitpid+0x58>
    80203448:	fe840593          	addi	a1,s0,-24
    8020344c:	4505                	li	a0,1
    8020344e:	00000097          	auipc	ra,0x0
    80203452:	d0a080e7          	jalr	-758(ra) # 80203158 <argaddr>
    return -1;
    80203456:	57fd                	li	a5,-1
  if(argint(0, &pid) < 0 || argaddr(1, &p) < 0 || argint(2, &options)) // 注意看测试用例的syscall.c！！
    80203458:	02054663          	bltz	a0,80203484 <sys_waitpid+0x58>
    8020345c:	fe040593          	addi	a1,s0,-32
    80203460:	4509                	li	a0,2
    80203462:	00000097          	auipc	ra,0x0
    80203466:	c94080e7          	jalr	-876(ra) # 802030f6 <argint>
    return -1;
    8020346a:	57fd                	li	a5,-1
  if(argint(0, &pid) < 0 || argaddr(1, &p) < 0 || argint(2, &options)) // 注意看测试用例的syscall.c！！
    8020346c:	ed01                	bnez	a0,80203484 <sys_waitpid+0x58>
  
  return wait(pid, p, options);
    8020346e:	fe042603          	lw	a2,-32(s0)
    80203472:	fe843583          	ld	a1,-24(s0)
    80203476:	fe442503          	lw	a0,-28(s0)
    8020347a:	fffff097          	auipc	ra,0xfffff
    8020347e:	0a4080e7          	jalr	164(ra) # 8020251e <wait>
    80203482:	87aa                	mv	a5,a0
}
    80203484:	853e                	mv	a0,a5
    80203486:	60e2                	ld	ra,24(sp)
    80203488:	6442                	ld	s0,16(sp)
    8020348a:	6105                	addi	sp,sp,32
    8020348c:	8082                	ret

000000008020348e <sys_sbrk>:

uint64
sys_sbrk(void)
{
    8020348e:	1101                	addi	sp,sp,-32
    80203490:	ec06                	sd	ra,24(sp)
    80203492:	e822                	sd	s0,16(sp)
    80203494:	1000                	addi	s0,sp,32
  int addr;
  int n;  // 扩张/收缩到 n bytes

  if(argint(0, &n) < 0)
    80203496:	fec40593          	addi	a1,s0,-20
    8020349a:	4501                	li	a0,0
    8020349c:	00000097          	auipc	ra,0x0
    802034a0:	c5a080e7          	jalr	-934(ra) # 802030f6 <argint>
    802034a4:	87aa                	mv	a5,a0
    return -1;
    802034a6:	557d                	li	a0,-1
  if(argint(0, &n) < 0)
    802034a8:	0007ca63          	bltz	a5,802034bc <sys_sbrk+0x2e>
  addr = myproc()->sz;
    802034ac:	ffffe097          	auipc	ra,0xffffe
    802034b0:	62c080e7          	jalr	1580(ra) # 80201ad8 <myproc>
    802034b4:	4528                	lw	a0,72(a0)
  if(n == 0) // n = 0，用于获取进程所占物理内存的大小
    802034b6:	fec42783          	lw	a5,-20(s0)
    802034ba:	e789                	bnez	a5,802034c4 <sys_sbrk+0x36>
    return addr;
  if(growproc(n - addr) < 0)  // 扩张/收缩
    return -1;
  return 0;
}
    802034bc:	60e2                	ld	ra,24(sp)
    802034be:	6442                	ld	s0,16(sp)
    802034c0:	6105                	addi	sp,sp,32
    802034c2:	8082                	ret
  if(growproc(n - addr) < 0)  // 扩张/收缩
    802034c4:	40a7853b          	subw	a0,a5,a0
    802034c8:	fffff097          	auipc	ra,0xfffff
    802034cc:	9aa080e7          	jalr	-1622(ra) # 80201e72 <growproc>
    802034d0:	957d                	srai	a0,a0,0x3f
    802034d2:	b7ed                	j	802034bc <sys_sbrk+0x2e>

00000000802034d4 <sys_sleep>:

uint64
sys_sleep(void)
{
    802034d4:	7139                	addi	sp,sp,-64
    802034d6:	fc06                	sd	ra,56(sp)
    802034d8:	f822                	sd	s0,48(sp)
    802034da:	f426                	sd	s1,40(sp)
    802034dc:	f04a                	sd	s2,32(sp)
    802034de:	ec4e                	sd	s3,24(sp)
    802034e0:	0080                	addi	s0,sp,64
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    802034e2:	fcc40593          	addi	a1,s0,-52
    802034e6:	4501                	li	a0,0
    802034e8:	00000097          	auipc	ra,0x0
    802034ec:	c0e080e7          	jalr	-1010(ra) # 802030f6 <argint>
    return -1;
    802034f0:	57fd                	li	a5,-1
  if(argint(0, &n) < 0)
    802034f2:	06054763          	bltz	a0,80203560 <sys_sleep+0x8c>
  acquire(&tickslock);
    802034f6:	00030517          	auipc	a0,0x30
    802034fa:	e1253503          	ld	a0,-494(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    802034fe:	ffffd097          	auipc	ra,0xffffd
    80203502:	1f6080e7          	jalr	502(ra) # 802006f4 <acquire>
  ticks0 = ticks;
    80203506:	00030797          	auipc	a5,0x30
    8020350a:	e127b783          	ld	a5,-494(a5) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
    8020350e:	0007a903          	lw	s2,0(a5)
  while(ticks - ticks0 < n){
    80203512:	fcc42783          	lw	a5,-52(s0)
    80203516:	cf85                	beqz	a5,8020354e <sys_sleep+0x7a>
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
    80203518:	00030997          	auipc	s3,0x30
    8020351c:	df09b983          	ld	s3,-528(s3) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    80203520:	00030497          	auipc	s1,0x30
    80203524:	df84b483          	ld	s1,-520(s1) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
    if(myproc()->killed){
    80203528:	ffffe097          	auipc	ra,0xffffe
    8020352c:	5b0080e7          	jalr	1456(ra) # 80201ad8 <myproc>
    80203530:	591c                	lw	a5,48(a0)
    80203532:	ef9d                	bnez	a5,80203570 <sys_sleep+0x9c>
    sleep(&ticks, &tickslock);
    80203534:	85ce                	mv	a1,s3
    80203536:	8526                	mv	a0,s1
    80203538:	fffff097          	auipc	ra,0xfffff
    8020353c:	f68080e7          	jalr	-152(ra) # 802024a0 <sleep>
  while(ticks - ticks0 < n){
    80203540:	409c                	lw	a5,0(s1)
    80203542:	412787bb          	subw	a5,a5,s2
    80203546:	fcc42703          	lw	a4,-52(s0)
    8020354a:	fce7efe3          	bltu	a5,a4,80203528 <sys_sleep+0x54>
  }
  release(&tickslock);
    8020354e:	00030517          	auipc	a0,0x30
    80203552:	dba53503          	ld	a0,-582(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    80203556:	ffffd097          	auipc	ra,0xffffd
    8020355a:	1f2080e7          	jalr	498(ra) # 80200748 <release>
  return 0;
    8020355e:	4781                	li	a5,0
}
    80203560:	853e                	mv	a0,a5
    80203562:	70e2                	ld	ra,56(sp)
    80203564:	7442                	ld	s0,48(sp)
    80203566:	74a2                	ld	s1,40(sp)
    80203568:	7902                	ld	s2,32(sp)
    8020356a:	69e2                	ld	s3,24(sp)
    8020356c:	6121                	addi	sp,sp,64
    8020356e:	8082                	ret
      release(&tickslock);
    80203570:	00030517          	auipc	a0,0x30
    80203574:	d9853503          	ld	a0,-616(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    80203578:	ffffd097          	auipc	ra,0xffffd
    8020357c:	1d0080e7          	jalr	464(ra) # 80200748 <release>
      return -1;
    80203580:	57fd                	li	a5,-1
    80203582:	bff9                	j	80203560 <sys_sleep+0x8c>

0000000080203584 <sys_kill>:

uint64
sys_kill(void)
{
    80203584:	1101                	addi	sp,sp,-32
    80203586:	ec06                	sd	ra,24(sp)
    80203588:	e822                	sd	s0,16(sp)
    8020358a:	1000                	addi	s0,sp,32
  int pid;

  if(argint(0, &pid) < 0)
    8020358c:	fec40593          	addi	a1,s0,-20
    80203590:	4501                	li	a0,0
    80203592:	00000097          	auipc	ra,0x0
    80203596:	b64080e7          	jalr	-1180(ra) # 802030f6 <argint>
    8020359a:	87aa                	mv	a5,a0
    return -1;
    8020359c:	557d                	li	a0,-1
  if(argint(0, &pid) < 0)
    8020359e:	0007c863          	bltz	a5,802035ae <sys_kill+0x2a>
  return kill(pid);
    802035a2:	fec42503          	lw	a0,-20(s0)
    802035a6:	fffff097          	auipc	ra,0xfffff
    802035aa:	0fc080e7          	jalr	252(ra) # 802026a2 <kill>
}
    802035ae:	60e2                	ld	ra,24(sp)
    802035b0:	6442                	ld	s0,16(sp)
    802035b2:	6105                	addi	sp,sp,32
    802035b4:	8082                	ret

00000000802035b6 <sys_times>:

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_times(void)
{
    802035b6:	715d                	addi	sp,sp,-80
    802035b8:	e486                	sd	ra,72(sp)
    802035ba:	e0a2                	sd	s0,64(sp)
    802035bc:	fc26                	sd	s1,56(sp)
    802035be:	0880                	addi	s0,sp,80
  uint64 addr;
  uint xticks;
  struct tms mytimes;

  if(argaddr(0, &addr) < 0)
    802035c0:	fd840593          	addi	a1,s0,-40
    802035c4:	4501                	li	a0,0
    802035c6:	00000097          	auipc	ra,0x0
    802035ca:	b92080e7          	jalr	-1134(ra) # 80203158 <argaddr>
    return -1;
    802035ce:	54fd                	li	s1,-1
  if(argaddr(0, &addr) < 0)
    802035d0:	06054763          	bltz	a0,8020363e <sys_times+0x88>

  acquire(&tickslock);
    802035d4:	00030517          	auipc	a0,0x30
    802035d8:	d3453503          	ld	a0,-716(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    802035dc:	ffffd097          	auipc	ra,0xffffd
    802035e0:	118080e7          	jalr	280(ra) # 802006f4 <acquire>
  xticks = ticks;
    802035e4:	00030797          	auipc	a5,0x30
    802035e8:	d347b783          	ld	a5,-716(a5) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
    802035ec:	4384                	lw	s1,0(a5)
  release(&tickslock);
    802035ee:	00030517          	auipc	a0,0x30
    802035f2:	d1a53503          	ld	a0,-742(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    802035f6:	ffffd097          	auipc	ra,0xffffd
    802035fa:	152080e7          	jalr	338(ra) # 80200748 <release>
  mytimes.tms_utime = xticks * 1000 / 200 ; // 以ms为单位
    802035fe:	3e800793          	li	a5,1000
    80203602:	029787bb          	mulw	a5,a5,s1
    80203606:	0c800713          	li	a4,200
    8020360a:	02e7d7bb          	divuw	a5,a5,a4
    8020360e:	1782                	slli	a5,a5,0x20
    80203610:	9381                	srli	a5,a5,0x20
    80203612:	faf43c23          	sd	a5,-72(s0)
  mytimes.tms_stime = xticks * 1000 / 200;
    80203616:	fcf43023          	sd	a5,-64(s0)
  mytimes.tms_cutime = xticks * 1000 / 200;
    8020361a:	fcf43423          	sd	a5,-56(s0)
  mytimes.tms_cstime = xticks * 1000 / 200;
    8020361e:	fcf43823          	sd	a5,-48(s0)
  
  if(copyout2(addr, (char *)&mytimes, sizeof(mytimes)) < 0)
    80203622:	02000613          	li	a2,32
    80203626:	fb840593          	addi	a1,s0,-72
    8020362a:	fd843503          	ld	a0,-40(s0)
    8020362e:	ffffe097          	auipc	ra,0xffffe
    80203632:	dd0080e7          	jalr	-560(ra) # 802013fe <copyout2>
    80203636:	00054a63          	bltz	a0,8020364a <sys_times+0x94>
    return -1;

  return xticks;
    8020363a:	1482                	slli	s1,s1,0x20
    8020363c:	9081                	srli	s1,s1,0x20
}
    8020363e:	8526                	mv	a0,s1
    80203640:	60a6                	ld	ra,72(sp)
    80203642:	6406                	ld	s0,64(sp)
    80203644:	74e2                	ld	s1,56(sp)
    80203646:	6161                	addi	sp,sp,80
    80203648:	8082                	ret
    return -1;
    8020364a:	54fd                	li	s1,-1
    8020364c:	bfcd                	j	8020363e <sys_times+0x88>

000000008020364e <sys_trace>:

uint64
sys_trace(void)
{
    8020364e:	1101                	addi	sp,sp,-32
    80203650:	ec06                	sd	ra,24(sp)
    80203652:	e822                	sd	s0,16(sp)
    80203654:	1000                	addi	s0,sp,32
  int mask;
  if(argint(0, &mask) < 0) {
    80203656:	fec40593          	addi	a1,s0,-20
    8020365a:	4501                	li	a0,0
    8020365c:	00000097          	auipc	ra,0x0
    80203660:	a9a080e7          	jalr	-1382(ra) # 802030f6 <argint>
    return -1;
    80203664:	57fd                	li	a5,-1
  if(argint(0, &mask) < 0) {
    80203666:	00054b63          	bltz	a0,8020367c <sys_trace+0x2e>
  }
  myproc()->tmask = mask;
    8020366a:	ffffe097          	auipc	ra,0xffffe
    8020366e:	46e080e7          	jalr	1134(ra) # 80201ad8 <myproc>
    80203672:	fec42783          	lw	a5,-20(s0)
    80203676:	46f52023          	sw	a5,1120(a0)
  return 0;
    8020367a:	4781                	li	a5,0
}
    8020367c:	853e                	mv	a0,a5
    8020367e:	60e2                	ld	ra,24(sp)
    80203680:	6442                	ld	s0,16(sp)
    80203682:	6105                	addi	sp,sp,32
    80203684:	8082                	ret

0000000080203686 <sys_getppid>:

uint64
sys_getppid(void)
{
    80203686:	1141                	addi	sp,sp,-16
    80203688:	e406                	sd	ra,8(sp)
    8020368a:	e022                	sd	s0,0(sp)
    8020368c:	0800                	addi	s0,sp,16
  return myproc()->parent->pid;
    8020368e:	ffffe097          	auipc	ra,0xffffe
    80203692:	44a080e7          	jalr	1098(ra) # 80201ad8 <myproc>
    80203696:	711c                	ld	a5,32(a0)
}
    80203698:	5f88                	lw	a0,56(a5)
    8020369a:	60a2                	ld	ra,8(sp)
    8020369c:	6402                	ld	s0,0(sp)
    8020369e:	0141                	addi	sp,sp,16
    802036a0:	8082                	ret

00000000802036a2 <sys_clone>:

uint64
sys_clone(void)
{
    802036a2:	1141                	addi	sp,sp,-16
    802036a4:	e406                	sd	ra,8(sp)
    802036a6:	e022                	sd	s0,0(sp)
    802036a8:	0800                	addi	s0,sp,16
  return clone();
    802036aa:	fffff097          	auipc	ra,0xfffff
    802036ae:	95c080e7          	jalr	-1700(ra) # 80202006 <clone>
}
    802036b2:	60a2                	ld	ra,8(sp)
    802036b4:	6402                	ld	s0,0(sp)
    802036b6:	0141                	addi	sp,sp,16
    802036b8:	8082                	ret

00000000802036ba <sys_sched_yield>:

uint64
sys_sched_yield(void)
{
    802036ba:	1141                	addi	sp,sp,-16
    802036bc:	e406                	sd	ra,8(sp)
    802036be:	e022                	sd	s0,0(sp)
    802036c0:	0800                	addi	s0,sp,16
  yield();
    802036c2:	fffff097          	auipc	ra,0xfffff
    802036c6:	da2080e7          	jalr	-606(ra) # 80202464 <yield>
  return 0;
}
    802036ca:	4501                	li	a0,0
    802036cc:	60a2                	ld	ra,8(sp)
    802036ce:	6402                	ld	s0,0(sp)
    802036d0:	0141                	addi	sp,sp,16
    802036d2:	8082                	ret

00000000802036d4 <sys_get_time>:
	long   tv_usec;     
};

uint64
sys_get_time(void)
{
    802036d4:	7139                	addi	sp,sp,-64
    802036d6:	fc06                	sd	ra,56(sp)
    802036d8:	f822                	sd	s0,48(sp)
    802036da:	f426                	sd	s1,40(sp)
    802036dc:	0080                	addi	s0,sp,64
  uint xticks;
  uint64 addr;
  int num;
  struct timespec tmp;
  if(argaddr(0, &addr) < 0 || argint(1, &num) < 0)
    802036de:	fd840593          	addi	a1,s0,-40
    802036e2:	4501                	li	a0,0
    802036e4:	00000097          	auipc	ra,0x0
    802036e8:	a74080e7          	jalr	-1420(ra) # 80203158 <argaddr>
    return -1;
    802036ec:	57fd                	li	a5,-1
  if(argaddr(0, &addr) < 0 || argint(1, &num) < 0)
    802036ee:	08054263          	bltz	a0,80203772 <sys_get_time+0x9e>
    802036f2:	fd440593          	addi	a1,s0,-44
    802036f6:	4505                	li	a0,1
    802036f8:	00000097          	auipc	ra,0x0
    802036fc:	9fe080e7          	jalr	-1538(ra) # 802030f6 <argint>
    return -1;
    80203700:	57fd                	li	a5,-1
  if(argaddr(0, &addr) < 0 || argint(1, &num) < 0)
    80203702:	06054863          	bltz	a0,80203772 <sys_get_time+0x9e>

  acquire(&tickslock);
    80203706:	00030517          	auipc	a0,0x30
    8020370a:	c0253503          	ld	a0,-1022(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    8020370e:	ffffd097          	auipc	ra,0xffffd
    80203712:	fe6080e7          	jalr	-26(ra) # 802006f4 <acquire>
  xticks = ticks;
    80203716:	00030797          	auipc	a5,0x30
    8020371a:	c027b783          	ld	a5,-1022(a5) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
    8020371e:	4384                	lw	s1,0(a5)
  release(&tickslock);
    80203720:	00030517          	auipc	a0,0x30
    80203724:	be853503          	ld	a0,-1048(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    80203728:	ffffd097          	auipc	ra,0xffffd
    8020372c:	020080e7          	jalr	32(ra) # 80200748 <release>
  // printf("time = %d\n", fre);

  tmp.tv_sec = xticks / 200;
    80203730:	0c800713          	li	a4,200
    80203734:	02e4d7bb          	divuw	a5,s1,a4
    80203738:	1782                	slli	a5,a5,0x20
    8020373a:	9381                	srli	a5,a5,0x20
    8020373c:	fcf43023          	sd	a5,-64(s0)
  tmp.tv_usec = (xticks * 1000000 / 200) % 1000000;
    80203740:	000f47b7          	lui	a5,0xf4
    80203744:	2407879b          	addiw	a5,a5,576 # f4240 <_entry-0x8010bdc0>
    80203748:	02f484bb          	mulw	s1,s1,a5
    8020374c:	02e4d4bb          	divuw	s1,s1,a4
    80203750:	02f4f4bb          	remuw	s1,s1,a5
    80203754:	1482                	slli	s1,s1,0x20
    80203756:	9081                	srli	s1,s1,0x20
    80203758:	fc943423          	sd	s1,-56(s0)
  if(copyout2(addr, (char*)&tmp, sizeof(tmp)) < 0)
    8020375c:	4641                	li	a2,16
    8020375e:	fc040593          	addi	a1,s0,-64
    80203762:	fd843503          	ld	a0,-40(s0)
    80203766:	ffffe097          	auipc	ra,0xffffe
    8020376a:	c98080e7          	jalr	-872(ra) # 802013fe <copyout2>
    8020376e:	43f55793          	srai	a5,a0,0x3f
    return -1;
  
  return 0;
}
    80203772:	853e                	mv	a0,a5
    80203774:	70e2                	ld	ra,56(sp)
    80203776:	7442                	ld	s0,48(sp)
    80203778:	74a2                	ld	s1,40(sp)
    8020377a:	6121                	addi	sp,sp,64
    8020377c:	8082                	ret

000000008020377e <sys_nanosleep>:

uint64
sys_nanosleep(void)
{
    8020377e:	715d                	addi	sp,sp,-80
    80203780:	e486                	sd	ra,72(sp)
    80203782:	e0a2                	sd	s0,64(sp)
    80203784:	fc26                	sd	s1,56(sp)
    80203786:	f84a                	sd	s2,48(sp)
    80203788:	f44e                	sd	s3,40(sp)
    8020378a:	f052                	sd	s4,32(sp)
    8020378c:	0880                	addi	s0,sp,80
  uint64 addr1, addr2;
  uint xticks0;
  struct timespec tv;

  if(argaddr(0, &addr1) < 0 || argaddr(1, &addr2) < 0)
    8020378e:	fc840593          	addi	a1,s0,-56
    80203792:	4501                	li	a0,0
    80203794:	00000097          	auipc	ra,0x0
    80203798:	9c4080e7          	jalr	-1596(ra) # 80203158 <argaddr>
    return -1;
    8020379c:	57fd                	li	a5,-1
  if(argaddr(0, &addr1) < 0 || argaddr(1, &addr2) < 0)
    8020379e:	0a054463          	bltz	a0,80203846 <sys_nanosleep+0xc8>
    802037a2:	fc040593          	addi	a1,s0,-64
    802037a6:	4505                	li	a0,1
    802037a8:	00000097          	auipc	ra,0x0
    802037ac:	9b0080e7          	jalr	-1616(ra) # 80203158 <argaddr>
    return -1;
    802037b0:	57fd                	li	a5,-1
  if(argaddr(0, &addr1) < 0 || argaddr(1, &addr2) < 0)
    802037b2:	08054a63          	bltz	a0,80203846 <sys_nanosleep+0xc8>
  if(copyin2((char *)&tv, addr1, sizeof(struct timespec)) < 0)
    802037b6:	4641                	li	a2,16
    802037b8:	fc843583          	ld	a1,-56(s0)
    802037bc:	fb040513          	addi	a0,s0,-80
    802037c0:	ffffe097          	auipc	ra,0xffffe
    802037c4:	d1e080e7          	jalr	-738(ra) # 802014de <copyin2>
    return -1;
    802037c8:	57fd                	li	a5,-1
  if(copyin2((char *)&tv, addr1, sizeof(struct timespec)) < 0)
    802037ca:	06054e63          	bltz	a0,80203846 <sys_nanosleep+0xc8>
  
  acquire(&tickslock);
    802037ce:	00030517          	auipc	a0,0x30
    802037d2:	b3a53503          	ld	a0,-1222(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    802037d6:	ffffd097          	auipc	ra,0xffffd
    802037da:	f1e080e7          	jalr	-226(ra) # 802006f4 <acquire>
  xticks0 = ticks;
    802037de:	00030797          	auipc	a5,0x30
    802037e2:	b3a7b783          	ld	a5,-1222(a5) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
    802037e6:	0007a903          	lw	s2,0(a5)
  while((ticks - xticks0) / 200 < tv.tv_sec){
    802037ea:	fb043783          	ld	a5,-80(s0)
    802037ee:	04f05363          	blez	a5,80203834 <sys_nanosleep+0xb6>
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
    802037f2:	00030a17          	auipc	s4,0x30
    802037f6:	b16a3a03          	ld	s4,-1258(s4) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    802037fa:	00030497          	auipc	s1,0x30
    802037fe:	b1e4b483          	ld	s1,-1250(s1) # 80233318 <_GLOBAL_OFFSET_TABLE_+0x40>
  while((ticks - xticks0) / 200 < tv.tv_sec){
    80203802:	0c800993          	li	s3,200
    if(myproc()->killed){
    80203806:	ffffe097          	auipc	ra,0xffffe
    8020380a:	2d2080e7          	jalr	722(ra) # 80201ad8 <myproc>
    8020380e:	591c                	lw	a5,48(a0)
    80203810:	e7a1                	bnez	a5,80203858 <sys_nanosleep+0xda>
    sleep(&ticks, &tickslock);
    80203812:	85d2                	mv	a1,s4
    80203814:	8526                	mv	a0,s1
    80203816:	fffff097          	auipc	ra,0xfffff
    8020381a:	c8a080e7          	jalr	-886(ra) # 802024a0 <sleep>
  while((ticks - xticks0) / 200 < tv.tv_sec){
    8020381e:	409c                	lw	a5,0(s1)
    80203820:	412787bb          	subw	a5,a5,s2
    80203824:	0337d7bb          	divuw	a5,a5,s3
    80203828:	1782                	slli	a5,a5,0x20
    8020382a:	9381                	srli	a5,a5,0x20
    8020382c:	fb043703          	ld	a4,-80(s0)
    80203830:	fce7cbe3          	blt	a5,a4,80203806 <sys_nanosleep+0x88>
  }
  release(&tickslock);
    80203834:	00030517          	auipc	a0,0x30
    80203838:	ad453503          	ld	a0,-1324(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    8020383c:	ffffd097          	auipc	ra,0xffffd
    80203840:	f0c080e7          	jalr	-244(ra) # 80200748 <release>
  return 0;
    80203844:	4781                	li	a5,0
}
    80203846:	853e                	mv	a0,a5
    80203848:	60a6                	ld	ra,72(sp)
    8020384a:	6406                	ld	s0,64(sp)
    8020384c:	74e2                	ld	s1,56(sp)
    8020384e:	7942                	ld	s2,48(sp)
    80203850:	79a2                	ld	s3,40(sp)
    80203852:	7a02                	ld	s4,32(sp)
    80203854:	6161                	addi	sp,sp,80
    80203856:	8082                	ret
      release(&tickslock);
    80203858:	00030517          	auipc	a0,0x30
    8020385c:	ab053503          	ld	a0,-1360(a0) # 80233308 <_GLOBAL_OFFSET_TABLE_+0x30>
    80203860:	ffffd097          	auipc	ra,0xffffd
    80203864:	ee8080e7          	jalr	-280(ra) # 80200748 <release>
      return -1;
    80203868:	57fd                	li	a5,-1
    8020386a:	bff1                	j	80203846 <sys_nanosleep+0xc8>

000000008020386c <sys_mmap>:

uint64
sys_mmap(void){
    8020386c:	1141                	addi	sp,sp,-16
    8020386e:	e422                	sd	s0,8(sp)
    80203870:	0800                	addi	s0,sp,16
  // if(argaddr(5, &off) < 0)
  //   return -1;
  
	// return vaddr;
  return 0;
}
    80203872:	4501                	li	a0,0
    80203874:	6422                	ld	s0,8(sp)
    80203876:	0141                	addi	sp,sp,16
    80203878:	8082                	ret

000000008020387a <sys_munmap>:

uint64
sys_munmap(void){
    8020387a:	1141                	addi	sp,sp,-16
    8020387c:	e422                	sd	s0,8(sp)
    8020387e:	0800                	addi	s0,sp,16
	return 0;
    80203880:	4501                	li	a0,0
    80203882:	6422                	ld	s0,8(sp)
    80203884:	0141                	addi	sp,sp,16
    80203886:	8082                	ret

0000000080203888 <binit>:
  struct buf head;
} bcache;

void
binit(void)
{
    80203888:	7139                	addi	sp,sp,-64
    8020388a:	fc06                	sd	ra,56(sp)
    8020388c:	f822                	sd	s0,48(sp)
    8020388e:	f426                	sd	s1,40(sp)
    80203890:	f04a                	sd	s2,32(sp)
    80203892:	ec4e                	sd	s3,24(sp)
    80203894:	e852                	sd	s4,16(sp)
    80203896:	e456                	sd	s5,8(sp)
    80203898:	0080                	addi	s0,sp,64
  struct buf *b;

  initlock(&bcache.lock, "bcache");
    8020389a:	00006597          	auipc	a1,0x6
    8020389e:	40658593          	addi	a1,a1,1030 # 80209ca0 <digits+0x920>
    802038a2:	00046517          	auipc	a0,0x46
    802038a6:	52e50513          	addi	a0,a0,1326 # 80249dd0 <bcache>
    802038aa:	ffffd097          	auipc	ra,0xffffd
    802038ae:	e06080e7          	jalr	-506(ra) # 802006b0 <initlock>

  // Create linked list of buffers
  bcache.head.prev = &bcache.head;
    802038b2:	0004a797          	auipc	a5,0x4a
    802038b6:	51e78793          	addi	a5,a5,1310 # 8024ddd0 <bcache+0x4000>
    802038ba:	0004b717          	auipc	a4,0x4b
    802038be:	b7e70713          	addi	a4,a4,-1154 # 8024e438 <bcache+0x4668>
    802038c2:	6ae7b823          	sd	a4,1712(a5)
  bcache.head.next = &bcache.head;
    802038c6:	6ae7bc23          	sd	a4,1720(a5)
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    802038ca:	00046497          	auipc	s1,0x46
    802038ce:	51e48493          	addi	s1,s1,1310 # 80249de8 <bcache+0x18>
    b->refcnt = 0;
    b->sectorno = ~0;
    802038d2:	5a7d                	li	s4,-1
    b->dev = ~0;
    b->next = bcache.head.next;
    802038d4:	893e                	mv	s2,a5
    b->prev = &bcache.head;
    802038d6:	89ba                	mv	s3,a4
    initsleeplock(&b->lock, "buffer");
    802038d8:	00006a97          	auipc	s5,0x6
    802038dc:	3d0a8a93          	addi	s5,s5,976 # 80209ca8 <digits+0x928>
    b->refcnt = 0;
    802038e0:	0404a023          	sw	zero,64(s1)
    b->sectorno = ~0;
    802038e4:	0144a623          	sw	s4,12(s1)
    b->dev = ~0;
    802038e8:	0144a423          	sw	s4,8(s1)
    b->next = bcache.head.next;
    802038ec:	6b893783          	ld	a5,1720(s2)
    802038f0:	e8bc                	sd	a5,80(s1)
    b->prev = &bcache.head;
    802038f2:	0534b423          	sd	s3,72(s1)
    initsleeplock(&b->lock, "buffer");
    802038f6:	85d6                	mv	a1,s5
    802038f8:	01048513          	addi	a0,s1,16
    802038fc:	00000097          	auipc	ra,0x0
    80203900:	27e080e7          	jalr	638(ra) # 80203b7a <initsleeplock>
    bcache.head.next->prev = b;
    80203904:	6b893783          	ld	a5,1720(s2)
    80203908:	e7a4                	sd	s1,72(a5)
    bcache.head.next = b;
    8020390a:	6a993c23          	sd	s1,1720(s2)
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    8020390e:	25848493          	addi	s1,s1,600
    80203912:	fd3497e3          	bne	s1,s3,802038e0 <binit+0x58>
  }
  #ifdef DEBUG
  printf("binit\n");
    80203916:	00006517          	auipc	a0,0x6
    8020391a:	39a50513          	addi	a0,a0,922 # 80209cb0 <digits+0x930>
    8020391e:	ffffd097          	auipc	ra,0xffffd
    80203922:	870080e7          	jalr	-1936(ra) # 8020018e <printf>
  #endif
}
    80203926:	70e2                	ld	ra,56(sp)
    80203928:	7442                	ld	s0,48(sp)
    8020392a:	74a2                	ld	s1,40(sp)
    8020392c:	7902                	ld	s2,32(sp)
    8020392e:	69e2                	ld	s3,24(sp)
    80203930:	6a42                	ld	s4,16(sp)
    80203932:	6aa2                	ld	s5,8(sp)
    80203934:	6121                	addi	sp,sp,64
    80203936:	8082                	ret

0000000080203938 <bread>:
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf* 
bread(uint dev, uint sectorno) {
    80203938:	7179                	addi	sp,sp,-48
    8020393a:	f406                	sd	ra,40(sp)
    8020393c:	f022                	sd	s0,32(sp)
    8020393e:	ec26                	sd	s1,24(sp)
    80203940:	e84a                	sd	s2,16(sp)
    80203942:	e44e                	sd	s3,8(sp)
    80203944:	1800                	addi	s0,sp,48
    80203946:	892a                	mv	s2,a0
    80203948:	89ae                	mv	s3,a1
  acquire(&bcache.lock);
    8020394a:	00046517          	auipc	a0,0x46
    8020394e:	48650513          	addi	a0,a0,1158 # 80249dd0 <bcache>
    80203952:	ffffd097          	auipc	ra,0xffffd
    80203956:	da2080e7          	jalr	-606(ra) # 802006f4 <acquire>
  for(b = bcache.head.next; b != &bcache.head; b = b->next){
    8020395a:	0004b497          	auipc	s1,0x4b
    8020395e:	b2e4b483          	ld	s1,-1234(s1) # 8024e488 <bcache+0x46b8>
    80203962:	0004b797          	auipc	a5,0x4b
    80203966:	ad678793          	addi	a5,a5,-1322 # 8024e438 <bcache+0x4668>
    8020396a:	02f48f63          	beq	s1,a5,802039a8 <bread+0x70>
    8020396e:	873e                	mv	a4,a5
    80203970:	a021                	j	80203978 <bread+0x40>
    80203972:	68a4                	ld	s1,80(s1)
    80203974:	02e48a63          	beq	s1,a4,802039a8 <bread+0x70>
    if(b->dev == dev && b->sectorno == sectorno){
    80203978:	449c                	lw	a5,8(s1)
    8020397a:	ff279ce3          	bne	a5,s2,80203972 <bread+0x3a>
    8020397e:	44dc                	lw	a5,12(s1)
    80203980:	ff3799e3          	bne	a5,s3,80203972 <bread+0x3a>
      b->refcnt++;
    80203984:	40bc                	lw	a5,64(s1)
    80203986:	2785                	addiw	a5,a5,1
    80203988:	c0bc                	sw	a5,64(s1)
      release(&bcache.lock);
    8020398a:	00046517          	auipc	a0,0x46
    8020398e:	44650513          	addi	a0,a0,1094 # 80249dd0 <bcache>
    80203992:	ffffd097          	auipc	ra,0xffffd
    80203996:	db6080e7          	jalr	-586(ra) # 80200748 <release>
      acquiresleep(&b->lock);
    8020399a:	01048513          	addi	a0,s1,16
    8020399e:	00000097          	auipc	ra,0x0
    802039a2:	216080e7          	jalr	534(ra) # 80203bb4 <acquiresleep>
      return b;
    802039a6:	a8b9                	j	80203a04 <bread+0xcc>
  for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
    802039a8:	0004b497          	auipc	s1,0x4b
    802039ac:	ad84b483          	ld	s1,-1320(s1) # 8024e480 <bcache+0x46b0>
    802039b0:	0004b797          	auipc	a5,0x4b
    802039b4:	a8878793          	addi	a5,a5,-1400 # 8024e438 <bcache+0x4668>
    802039b8:	00f48863          	beq	s1,a5,802039c8 <bread+0x90>
    802039bc:	873e                	mv	a4,a5
    if(b->refcnt == 0) {
    802039be:	40bc                	lw	a5,64(s1)
    802039c0:	cf81                	beqz	a5,802039d8 <bread+0xa0>
  for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
    802039c2:	64a4                	ld	s1,72(s1)
    802039c4:	fee49de3          	bne	s1,a4,802039be <bread+0x86>
  panic("bget: no buffers");
    802039c8:	00006517          	auipc	a0,0x6
    802039cc:	2f050513          	addi	a0,a0,752 # 80209cb8 <digits+0x938>
    802039d0:	ffffc097          	auipc	ra,0xffffc
    802039d4:	774080e7          	jalr	1908(ra) # 80200144 <panic>
      b->dev = dev;
    802039d8:	0124a423          	sw	s2,8(s1)
      b->sectorno = sectorno;
    802039dc:	0134a623          	sw	s3,12(s1)
      b->valid = 0;
    802039e0:	0004a023          	sw	zero,0(s1)
      b->refcnt = 1;
    802039e4:	4785                	li	a5,1
    802039e6:	c0bc                	sw	a5,64(s1)
      release(&bcache.lock);
    802039e8:	00046517          	auipc	a0,0x46
    802039ec:	3e850513          	addi	a0,a0,1000 # 80249dd0 <bcache>
    802039f0:	ffffd097          	auipc	ra,0xffffd
    802039f4:	d58080e7          	jalr	-680(ra) # 80200748 <release>
      acquiresleep(&b->lock);
    802039f8:	01048513          	addi	a0,s1,16
    802039fc:	00000097          	auipc	ra,0x0
    80203a00:	1b8080e7          	jalr	440(ra) # 80203bb4 <acquiresleep>
  struct buf *b;

  b = bget(dev, sectorno);
  if (!b->valid) {
    80203a04:	409c                	lw	a5,0(s1)
    80203a06:	cb89                	beqz	a5,80203a18 <bread+0xe0>
    disk_read(b);
    b->valid = 1;
  }

  return b;
}
    80203a08:	8526                	mv	a0,s1
    80203a0a:	70a2                	ld	ra,40(sp)
    80203a0c:	7402                	ld	s0,32(sp)
    80203a0e:	64e2                	ld	s1,24(sp)
    80203a10:	6942                	ld	s2,16(sp)
    80203a12:	69a2                	ld	s3,8(sp)
    80203a14:	6145                	addi	sp,sp,48
    80203a16:	8082                	ret
    disk_read(b);
    80203a18:	8526                	mv	a0,s1
    80203a1a:	00002097          	auipc	ra,0x2
    80203a1e:	026080e7          	jalr	38(ra) # 80205a40 <disk_read>
    b->valid = 1;
    80203a22:	4785                	li	a5,1
    80203a24:	c09c                	sw	a5,0(s1)
  return b;
    80203a26:	b7cd                	j	80203a08 <bread+0xd0>

0000000080203a28 <bwrite>:

// Write b's contents to disk.  Must be locked.
void 
bwrite(struct buf *b) {
    80203a28:	1101                	addi	sp,sp,-32
    80203a2a:	ec06                	sd	ra,24(sp)
    80203a2c:	e822                	sd	s0,16(sp)
    80203a2e:	e426                	sd	s1,8(sp)
    80203a30:	1000                	addi	s0,sp,32
    80203a32:	84aa                	mv	s1,a0
  if(!holdingsleep(&b->lock))
    80203a34:	0541                	addi	a0,a0,16
    80203a36:	00000097          	auipc	ra,0x0
    80203a3a:	218080e7          	jalr	536(ra) # 80203c4e <holdingsleep>
    80203a3e:	c919                	beqz	a0,80203a54 <bwrite+0x2c>
    panic("bwrite");
  disk_write(b);
    80203a40:	8526                	mv	a0,s1
    80203a42:	00002097          	auipc	ra,0x2
    80203a46:	018080e7          	jalr	24(ra) # 80205a5a <disk_write>
}
    80203a4a:	60e2                	ld	ra,24(sp)
    80203a4c:	6442                	ld	s0,16(sp)
    80203a4e:	64a2                	ld	s1,8(sp)
    80203a50:	6105                	addi	sp,sp,32
    80203a52:	8082                	ret
    panic("bwrite");
    80203a54:	00006517          	auipc	a0,0x6
    80203a58:	27c50513          	addi	a0,a0,636 # 80209cd0 <digits+0x950>
    80203a5c:	ffffc097          	auipc	ra,0xffffc
    80203a60:	6e8080e7          	jalr	1768(ra) # 80200144 <panic>

0000000080203a64 <brelse>:

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
    80203a64:	1101                	addi	sp,sp,-32
    80203a66:	ec06                	sd	ra,24(sp)
    80203a68:	e822                	sd	s0,16(sp)
    80203a6a:	e426                	sd	s1,8(sp)
    80203a6c:	e04a                	sd	s2,0(sp)
    80203a6e:	1000                	addi	s0,sp,32
    80203a70:	84aa                	mv	s1,a0
  if(!holdingsleep(&b->lock))
    80203a72:	01050913          	addi	s2,a0,16
    80203a76:	854a                	mv	a0,s2
    80203a78:	00000097          	auipc	ra,0x0
    80203a7c:	1d6080e7          	jalr	470(ra) # 80203c4e <holdingsleep>
    80203a80:	c92d                	beqz	a0,80203af2 <brelse+0x8e>
    panic("brelse");

  releasesleep(&b->lock);
    80203a82:	854a                	mv	a0,s2
    80203a84:	00000097          	auipc	ra,0x0
    80203a88:	186080e7          	jalr	390(ra) # 80203c0a <releasesleep>

  acquire(&bcache.lock);
    80203a8c:	00046517          	auipc	a0,0x46
    80203a90:	34450513          	addi	a0,a0,836 # 80249dd0 <bcache>
    80203a94:	ffffd097          	auipc	ra,0xffffd
    80203a98:	c60080e7          	jalr	-928(ra) # 802006f4 <acquire>
  b->refcnt--;
    80203a9c:	40bc                	lw	a5,64(s1)
    80203a9e:	37fd                	addiw	a5,a5,-1
    80203aa0:	0007871b          	sext.w	a4,a5
    80203aa4:	c0bc                	sw	a5,64(s1)
  if (b->refcnt == 0) {
    80203aa6:	eb05                	bnez	a4,80203ad6 <brelse+0x72>
    // no one is waiting for it.
    b->next->prev = b->prev;
    80203aa8:	68bc                	ld	a5,80(s1)
    80203aaa:	64b8                	ld	a4,72(s1)
    80203aac:	e7b8                	sd	a4,72(a5)
    b->prev->next = b->next;
    80203aae:	64bc                	ld	a5,72(s1)
    80203ab0:	68b8                	ld	a4,80(s1)
    80203ab2:	ebb8                	sd	a4,80(a5)
    b->next = bcache.head.next;
    80203ab4:	0004a797          	auipc	a5,0x4a
    80203ab8:	31c78793          	addi	a5,a5,796 # 8024ddd0 <bcache+0x4000>
    80203abc:	6b87b703          	ld	a4,1720(a5)
    80203ac0:	e8b8                	sd	a4,80(s1)
    b->prev = &bcache.head;
    80203ac2:	0004b717          	auipc	a4,0x4b
    80203ac6:	97670713          	addi	a4,a4,-1674 # 8024e438 <bcache+0x4668>
    80203aca:	e4b8                	sd	a4,72(s1)
    bcache.head.next->prev = b;
    80203acc:	6b87b703          	ld	a4,1720(a5)
    80203ad0:	e724                	sd	s1,72(a4)
    bcache.head.next = b;
    80203ad2:	6a97bc23          	sd	s1,1720(a5)
  }
  
  release(&bcache.lock);
    80203ad6:	00046517          	auipc	a0,0x46
    80203ada:	2fa50513          	addi	a0,a0,762 # 80249dd0 <bcache>
    80203ade:	ffffd097          	auipc	ra,0xffffd
    80203ae2:	c6a080e7          	jalr	-918(ra) # 80200748 <release>
}
    80203ae6:	60e2                	ld	ra,24(sp)
    80203ae8:	6442                	ld	s0,16(sp)
    80203aea:	64a2                	ld	s1,8(sp)
    80203aec:	6902                	ld	s2,0(sp)
    80203aee:	6105                	addi	sp,sp,32
    80203af0:	8082                	ret
    panic("brelse");
    80203af2:	00006517          	auipc	a0,0x6
    80203af6:	1e650513          	addi	a0,a0,486 # 80209cd8 <digits+0x958>
    80203afa:	ffffc097          	auipc	ra,0xffffc
    80203afe:	64a080e7          	jalr	1610(ra) # 80200144 <panic>

0000000080203b02 <bpin>:

void
bpin(struct buf *b) {
    80203b02:	1101                	addi	sp,sp,-32
    80203b04:	ec06                	sd	ra,24(sp)
    80203b06:	e822                	sd	s0,16(sp)
    80203b08:	e426                	sd	s1,8(sp)
    80203b0a:	1000                	addi	s0,sp,32
    80203b0c:	84aa                	mv	s1,a0
  acquire(&bcache.lock);
    80203b0e:	00046517          	auipc	a0,0x46
    80203b12:	2c250513          	addi	a0,a0,706 # 80249dd0 <bcache>
    80203b16:	ffffd097          	auipc	ra,0xffffd
    80203b1a:	bde080e7          	jalr	-1058(ra) # 802006f4 <acquire>
  b->refcnt++;
    80203b1e:	40bc                	lw	a5,64(s1)
    80203b20:	2785                	addiw	a5,a5,1
    80203b22:	c0bc                	sw	a5,64(s1)
  release(&bcache.lock);
    80203b24:	00046517          	auipc	a0,0x46
    80203b28:	2ac50513          	addi	a0,a0,684 # 80249dd0 <bcache>
    80203b2c:	ffffd097          	auipc	ra,0xffffd
    80203b30:	c1c080e7          	jalr	-996(ra) # 80200748 <release>
}
    80203b34:	60e2                	ld	ra,24(sp)
    80203b36:	6442                	ld	s0,16(sp)
    80203b38:	64a2                	ld	s1,8(sp)
    80203b3a:	6105                	addi	sp,sp,32
    80203b3c:	8082                	ret

0000000080203b3e <bunpin>:

void
bunpin(struct buf *b) {
    80203b3e:	1101                	addi	sp,sp,-32
    80203b40:	ec06                	sd	ra,24(sp)
    80203b42:	e822                	sd	s0,16(sp)
    80203b44:	e426                	sd	s1,8(sp)
    80203b46:	1000                	addi	s0,sp,32
    80203b48:	84aa                	mv	s1,a0
  acquire(&bcache.lock);
    80203b4a:	00046517          	auipc	a0,0x46
    80203b4e:	28650513          	addi	a0,a0,646 # 80249dd0 <bcache>
    80203b52:	ffffd097          	auipc	ra,0xffffd
    80203b56:	ba2080e7          	jalr	-1118(ra) # 802006f4 <acquire>
  b->refcnt--;
    80203b5a:	40bc                	lw	a5,64(s1)
    80203b5c:	37fd                	addiw	a5,a5,-1
    80203b5e:	c0bc                	sw	a5,64(s1)
  release(&bcache.lock);
    80203b60:	00046517          	auipc	a0,0x46
    80203b64:	27050513          	addi	a0,a0,624 # 80249dd0 <bcache>
    80203b68:	ffffd097          	auipc	ra,0xffffd
    80203b6c:	be0080e7          	jalr	-1056(ra) # 80200748 <release>
}
    80203b70:	60e2                	ld	ra,24(sp)
    80203b72:	6442                	ld	s0,16(sp)
    80203b74:	64a2                	ld	s1,8(sp)
    80203b76:	6105                	addi	sp,sp,32
    80203b78:	8082                	ret

0000000080203b7a <initsleeplock>:
#include "include/proc.h"
#include "include/sleeplock.h"

void
initsleeplock(struct sleeplock *lk, char *name)
{
    80203b7a:	1101                	addi	sp,sp,-32
    80203b7c:	ec06                	sd	ra,24(sp)
    80203b7e:	e822                	sd	s0,16(sp)
    80203b80:	e426                	sd	s1,8(sp)
    80203b82:	e04a                	sd	s2,0(sp)
    80203b84:	1000                	addi	s0,sp,32
    80203b86:	84aa                	mv	s1,a0
    80203b88:	892e                	mv	s2,a1
  initlock(&lk->lk, "sleep lock");
    80203b8a:	00006597          	auipc	a1,0x6
    80203b8e:	15658593          	addi	a1,a1,342 # 80209ce0 <digits+0x960>
    80203b92:	0521                	addi	a0,a0,8
    80203b94:	ffffd097          	auipc	ra,0xffffd
    80203b98:	b1c080e7          	jalr	-1252(ra) # 802006b0 <initlock>
  lk->name = name;
    80203b9c:	0324b023          	sd	s2,32(s1)
  lk->locked = 0;
    80203ba0:	0004a023          	sw	zero,0(s1)
  lk->pid = 0;
    80203ba4:	0204a423          	sw	zero,40(s1)
}
    80203ba8:	60e2                	ld	ra,24(sp)
    80203baa:	6442                	ld	s0,16(sp)
    80203bac:	64a2                	ld	s1,8(sp)
    80203bae:	6902                	ld	s2,0(sp)
    80203bb0:	6105                	addi	sp,sp,32
    80203bb2:	8082                	ret

0000000080203bb4 <acquiresleep>:

void
acquiresleep(struct sleeplock *lk)
{
    80203bb4:	1101                	addi	sp,sp,-32
    80203bb6:	ec06                	sd	ra,24(sp)
    80203bb8:	e822                	sd	s0,16(sp)
    80203bba:	e426                	sd	s1,8(sp)
    80203bbc:	e04a                	sd	s2,0(sp)
    80203bbe:	1000                	addi	s0,sp,32
    80203bc0:	84aa                	mv	s1,a0
  acquire(&lk->lk);
    80203bc2:	00850913          	addi	s2,a0,8
    80203bc6:	854a                	mv	a0,s2
    80203bc8:	ffffd097          	auipc	ra,0xffffd
    80203bcc:	b2c080e7          	jalr	-1236(ra) # 802006f4 <acquire>
  while (lk->locked) {
    80203bd0:	409c                	lw	a5,0(s1)
    80203bd2:	cb89                	beqz	a5,80203be4 <acquiresleep+0x30>
    sleep(lk, &lk->lk);
    80203bd4:	85ca                	mv	a1,s2
    80203bd6:	8526                	mv	a0,s1
    80203bd8:	fffff097          	auipc	ra,0xfffff
    80203bdc:	8c8080e7          	jalr	-1848(ra) # 802024a0 <sleep>
  while (lk->locked) {
    80203be0:	409c                	lw	a5,0(s1)
    80203be2:	fbed                	bnez	a5,80203bd4 <acquiresleep+0x20>
  }
  lk->locked = 1;
    80203be4:	4785                	li	a5,1
    80203be6:	c09c                	sw	a5,0(s1)
  lk->pid = myproc()->pid;
    80203be8:	ffffe097          	auipc	ra,0xffffe
    80203bec:	ef0080e7          	jalr	-272(ra) # 80201ad8 <myproc>
    80203bf0:	5d1c                	lw	a5,56(a0)
    80203bf2:	d49c                	sw	a5,40(s1)
  release(&lk->lk);
    80203bf4:	854a                	mv	a0,s2
    80203bf6:	ffffd097          	auipc	ra,0xffffd
    80203bfa:	b52080e7          	jalr	-1198(ra) # 80200748 <release>
}
    80203bfe:	60e2                	ld	ra,24(sp)
    80203c00:	6442                	ld	s0,16(sp)
    80203c02:	64a2                	ld	s1,8(sp)
    80203c04:	6902                	ld	s2,0(sp)
    80203c06:	6105                	addi	sp,sp,32
    80203c08:	8082                	ret

0000000080203c0a <releasesleep>:

void
releasesleep(struct sleeplock *lk)
{
    80203c0a:	1101                	addi	sp,sp,-32
    80203c0c:	ec06                	sd	ra,24(sp)
    80203c0e:	e822                	sd	s0,16(sp)
    80203c10:	e426                	sd	s1,8(sp)
    80203c12:	e04a                	sd	s2,0(sp)
    80203c14:	1000                	addi	s0,sp,32
    80203c16:	84aa                	mv	s1,a0
  acquire(&lk->lk);
    80203c18:	00850913          	addi	s2,a0,8
    80203c1c:	854a                	mv	a0,s2
    80203c1e:	ffffd097          	auipc	ra,0xffffd
    80203c22:	ad6080e7          	jalr	-1322(ra) # 802006f4 <acquire>
  lk->locked = 0;
    80203c26:	0004a023          	sw	zero,0(s1)
  lk->pid = 0;
    80203c2a:	0204a423          	sw	zero,40(s1)
  wakeup(lk);
    80203c2e:	8526                	mv	a0,s1
    80203c30:	fffff097          	auipc	ra,0xfffff
    80203c34:	a08080e7          	jalr	-1528(ra) # 80202638 <wakeup>
  release(&lk->lk);
    80203c38:	854a                	mv	a0,s2
    80203c3a:	ffffd097          	auipc	ra,0xffffd
    80203c3e:	b0e080e7          	jalr	-1266(ra) # 80200748 <release>
}
    80203c42:	60e2                	ld	ra,24(sp)
    80203c44:	6442                	ld	s0,16(sp)
    80203c46:	64a2                	ld	s1,8(sp)
    80203c48:	6902                	ld	s2,0(sp)
    80203c4a:	6105                	addi	sp,sp,32
    80203c4c:	8082                	ret

0000000080203c4e <holdingsleep>:

int
holdingsleep(struct sleeplock *lk)
{
    80203c4e:	7179                	addi	sp,sp,-48
    80203c50:	f406                	sd	ra,40(sp)
    80203c52:	f022                	sd	s0,32(sp)
    80203c54:	ec26                	sd	s1,24(sp)
    80203c56:	e84a                	sd	s2,16(sp)
    80203c58:	e44e                	sd	s3,8(sp)
    80203c5a:	1800                	addi	s0,sp,48
    80203c5c:	84aa                	mv	s1,a0
  int r;
  
  acquire(&lk->lk);
    80203c5e:	00850913          	addi	s2,a0,8
    80203c62:	854a                	mv	a0,s2
    80203c64:	ffffd097          	auipc	ra,0xffffd
    80203c68:	a90080e7          	jalr	-1392(ra) # 802006f4 <acquire>
  r = lk->locked && (lk->pid == myproc()->pid);
    80203c6c:	409c                	lw	a5,0(s1)
    80203c6e:	ef99                	bnez	a5,80203c8c <holdingsleep+0x3e>
    80203c70:	4481                	li	s1,0
  release(&lk->lk);
    80203c72:	854a                	mv	a0,s2
    80203c74:	ffffd097          	auipc	ra,0xffffd
    80203c78:	ad4080e7          	jalr	-1324(ra) # 80200748 <release>
  return r;
}
    80203c7c:	8526                	mv	a0,s1
    80203c7e:	70a2                	ld	ra,40(sp)
    80203c80:	7402                	ld	s0,32(sp)
    80203c82:	64e2                	ld	s1,24(sp)
    80203c84:	6942                	ld	s2,16(sp)
    80203c86:	69a2                	ld	s3,8(sp)
    80203c88:	6145                	addi	sp,sp,48
    80203c8a:	8082                	ret
  r = lk->locked && (lk->pid == myproc()->pid);
    80203c8c:	0284a983          	lw	s3,40(s1)
    80203c90:	ffffe097          	auipc	ra,0xffffe
    80203c94:	e48080e7          	jalr	-440(ra) # 80201ad8 <myproc>
    80203c98:	5d04                	lw	s1,56(a0)
    80203c9a:	413484b3          	sub	s1,s1,s3
    80203c9e:	0014b493          	seqz	s1,s1
    80203ca2:	bfc1                	j	80203c72 <holdingsleep+0x24>

0000000080203ca4 <fileinit>:
  struct file file[NFILE];
} ftable;

void
fileinit(void)
{
    80203ca4:	1101                	addi	sp,sp,-32
    80203ca6:	ec06                	sd	ra,24(sp)
    80203ca8:	e822                	sd	s0,16(sp)
    80203caa:	e426                	sd	s1,8(sp)
    80203cac:	e04a                	sd	s2,0(sp)
    80203cae:	1000                	addi	s0,sp,32
  initlock(&ftable.lock, "ftable");
    80203cb0:	00006597          	auipc	a1,0x6
    80203cb4:	04058593          	addi	a1,a1,64 # 80209cf0 <digits+0x970>
    80203cb8:	0004b517          	auipc	a0,0x4b
    80203cbc:	a7850513          	addi	a0,a0,-1416 # 8024e730 <ftable>
    80203cc0:	ffffd097          	auipc	ra,0xffffd
    80203cc4:	9f0080e7          	jalr	-1552(ra) # 802006b0 <initlock>
  struct file *f;
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    80203cc8:	0004b497          	auipc	s1,0x4b
    80203ccc:	a8048493          	addi	s1,s1,-1408 # 8024e748 <ftable+0x18>
    80203cd0:	0004d917          	auipc	s2,0x4d
    80203cd4:	9b890913          	addi	s2,s2,-1608 # 80250688 <tickslock>
    memset(f, 0, sizeof(struct file));
    80203cd8:	02800613          	li	a2,40
    80203cdc:	4581                	li	a1,0
    80203cde:	8526                	mv	a0,s1
    80203ce0:	ffffd097          	auipc	ra,0xffffd
    80203ce4:	ab0080e7          	jalr	-1360(ra) # 80200790 <memset>
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    80203ce8:	02848493          	addi	s1,s1,40
    80203cec:	ff2496e3          	bne	s1,s2,80203cd8 <fileinit+0x34>
  }
  #ifdef DEBUG
  printf("fileinit\n");
    80203cf0:	00006517          	auipc	a0,0x6
    80203cf4:	00850513          	addi	a0,a0,8 # 80209cf8 <digits+0x978>
    80203cf8:	ffffc097          	auipc	ra,0xffffc
    80203cfc:	496080e7          	jalr	1174(ra) # 8020018e <printf>
  #endif
}
    80203d00:	60e2                	ld	ra,24(sp)
    80203d02:	6442                	ld	s0,16(sp)
    80203d04:	64a2                	ld	s1,8(sp)
    80203d06:	6902                	ld	s2,0(sp)
    80203d08:	6105                	addi	sp,sp,32
    80203d0a:	8082                	ret

0000000080203d0c <filealloc>:

// Allocate a file structure.
struct file*
filealloc(void)
{
    80203d0c:	1101                	addi	sp,sp,-32
    80203d0e:	ec06                	sd	ra,24(sp)
    80203d10:	e822                	sd	s0,16(sp)
    80203d12:	e426                	sd	s1,8(sp)
    80203d14:	1000                	addi	s0,sp,32
  struct file *f;

  acquire(&ftable.lock);
    80203d16:	0004b517          	auipc	a0,0x4b
    80203d1a:	a1a50513          	addi	a0,a0,-1510 # 8024e730 <ftable>
    80203d1e:	ffffd097          	auipc	ra,0xffffd
    80203d22:	9d6080e7          	jalr	-1578(ra) # 802006f4 <acquire>
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    80203d26:	0004b497          	auipc	s1,0x4b
    80203d2a:	a2248493          	addi	s1,s1,-1502 # 8024e748 <ftable+0x18>
    80203d2e:	0004d717          	auipc	a4,0x4d
    80203d32:	95a70713          	addi	a4,a4,-1702 # 80250688 <tickslock>
    if(f->ref == 0){
    80203d36:	40dc                	lw	a5,4(s1)
    80203d38:	cf99                	beqz	a5,80203d56 <filealloc+0x4a>
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    80203d3a:	02848493          	addi	s1,s1,40
    80203d3e:	fee49ce3          	bne	s1,a4,80203d36 <filealloc+0x2a>
      f->ref = 1;
      release(&ftable.lock);
      return f;
    }
  }
  release(&ftable.lock);
    80203d42:	0004b517          	auipc	a0,0x4b
    80203d46:	9ee50513          	addi	a0,a0,-1554 # 8024e730 <ftable>
    80203d4a:	ffffd097          	auipc	ra,0xffffd
    80203d4e:	9fe080e7          	jalr	-1538(ra) # 80200748 <release>
  return NULL;
    80203d52:	4481                	li	s1,0
    80203d54:	a819                	j	80203d6a <filealloc+0x5e>
      f->ref = 1;
    80203d56:	4785                	li	a5,1
    80203d58:	c0dc                	sw	a5,4(s1)
      release(&ftable.lock);
    80203d5a:	0004b517          	auipc	a0,0x4b
    80203d5e:	9d650513          	addi	a0,a0,-1578 # 8024e730 <ftable>
    80203d62:	ffffd097          	auipc	ra,0xffffd
    80203d66:	9e6080e7          	jalr	-1562(ra) # 80200748 <release>
}
    80203d6a:	8526                	mv	a0,s1
    80203d6c:	60e2                	ld	ra,24(sp)
    80203d6e:	6442                	ld	s0,16(sp)
    80203d70:	64a2                	ld	s1,8(sp)
    80203d72:	6105                	addi	sp,sp,32
    80203d74:	8082                	ret

0000000080203d76 <filedup>:

// Increment ref count for file f.
struct file*
filedup(struct file *f)
{
    80203d76:	1101                	addi	sp,sp,-32
    80203d78:	ec06                	sd	ra,24(sp)
    80203d7a:	e822                	sd	s0,16(sp)
    80203d7c:	e426                	sd	s1,8(sp)
    80203d7e:	1000                	addi	s0,sp,32
    80203d80:	84aa                	mv	s1,a0
  acquire(&ftable.lock);
    80203d82:	0004b517          	auipc	a0,0x4b
    80203d86:	9ae50513          	addi	a0,a0,-1618 # 8024e730 <ftable>
    80203d8a:	ffffd097          	auipc	ra,0xffffd
    80203d8e:	96a080e7          	jalr	-1686(ra) # 802006f4 <acquire>
  if(f->ref < 1)
    80203d92:	40dc                	lw	a5,4(s1)
    80203d94:	02f05263          	blez	a5,80203db8 <filedup+0x42>
    panic("filedup");
  f->ref++;
    80203d98:	2785                	addiw	a5,a5,1
    80203d9a:	c0dc                	sw	a5,4(s1)
  release(&ftable.lock);
    80203d9c:	0004b517          	auipc	a0,0x4b
    80203da0:	99450513          	addi	a0,a0,-1644 # 8024e730 <ftable>
    80203da4:	ffffd097          	auipc	ra,0xffffd
    80203da8:	9a4080e7          	jalr	-1628(ra) # 80200748 <release>
  return f;
}
    80203dac:	8526                	mv	a0,s1
    80203dae:	60e2                	ld	ra,24(sp)
    80203db0:	6442                	ld	s0,16(sp)
    80203db2:	64a2                	ld	s1,8(sp)
    80203db4:	6105                	addi	sp,sp,32
    80203db6:	8082                	ret
    panic("filedup");
    80203db8:	00006517          	auipc	a0,0x6
    80203dbc:	f5050513          	addi	a0,a0,-176 # 80209d08 <digits+0x988>
    80203dc0:	ffffc097          	auipc	ra,0xffffc
    80203dc4:	384080e7          	jalr	900(ra) # 80200144 <panic>

0000000080203dc8 <fileclose>:

// Close file f.  (Decrement ref count, close when reaches 0.)
void
fileclose(struct file *f)
{
    80203dc8:	7139                	addi	sp,sp,-64
    80203dca:	fc06                	sd	ra,56(sp)
    80203dcc:	f822                	sd	s0,48(sp)
    80203dce:	f426                	sd	s1,40(sp)
    80203dd0:	f04a                	sd	s2,32(sp)
    80203dd2:	ec4e                	sd	s3,24(sp)
    80203dd4:	e852                	sd	s4,16(sp)
    80203dd6:	e456                	sd	s5,8(sp)
    80203dd8:	0080                	addi	s0,sp,64
    80203dda:	84aa                	mv	s1,a0
  struct file ff;

  acquire(&ftable.lock);
    80203ddc:	0004b517          	auipc	a0,0x4b
    80203de0:	95450513          	addi	a0,a0,-1708 # 8024e730 <ftable>
    80203de4:	ffffd097          	auipc	ra,0xffffd
    80203de8:	910080e7          	jalr	-1776(ra) # 802006f4 <acquire>
  if(f->ref < 1)
    80203dec:	40dc                	lw	a5,4(s1)
    80203dee:	04f05863          	blez	a5,80203e3e <fileclose+0x76>
    panic("fileclose");
  if(--f->ref > 0){
    80203df2:	37fd                	addiw	a5,a5,-1
    80203df4:	0007871b          	sext.w	a4,a5
    80203df8:	c0dc                	sw	a5,4(s1)
    80203dfa:	04e04a63          	bgtz	a4,80203e4e <fileclose+0x86>
    release(&ftable.lock);
    return;
  }
  ff = *f;
    80203dfe:	0004a903          	lw	s2,0(s1)
    80203e02:	0094ca03          	lbu	s4,9(s1)
    80203e06:	0104b983          	ld	s3,16(s1)
    80203e0a:	0184ba83          	ld	s5,24(s1)
  f->ref = 0;
    80203e0e:	0004a223          	sw	zero,4(s1)
  f->type = FD_NONE;
    80203e12:	0004a023          	sw	zero,0(s1)
  release(&ftable.lock);
    80203e16:	0004b517          	auipc	a0,0x4b
    80203e1a:	91a50513          	addi	a0,a0,-1766 # 8024e730 <ftable>
    80203e1e:	ffffd097          	auipc	ra,0xffffd
    80203e22:	92a080e7          	jalr	-1750(ra) # 80200748 <release>

  if(ff.type == FD_PIPE){
    80203e26:	4785                	li	a5,1
    80203e28:	04f90463          	beq	s2,a5,80203e70 <fileclose+0xa8>
    pipeclose(ff.pipe, ff.writable);
  } else if(ff.type == FD_ENTRY){
    80203e2c:	4789                	li	a5,2
    80203e2e:	02f91863          	bne	s2,a5,80203e5e <fileclose+0x96>
    eput(ff.ep);
    80203e32:	8556                	mv	a0,s5
    80203e34:	00003097          	auipc	ra,0x3
    80203e38:	eae080e7          	jalr	-338(ra) # 80206ce2 <eput>
    80203e3c:	a00d                	j	80203e5e <fileclose+0x96>
    panic("fileclose");
    80203e3e:	00006517          	auipc	a0,0x6
    80203e42:	ed250513          	addi	a0,a0,-302 # 80209d10 <digits+0x990>
    80203e46:	ffffc097          	auipc	ra,0xffffc
    80203e4a:	2fe080e7          	jalr	766(ra) # 80200144 <panic>
    release(&ftable.lock);
    80203e4e:	0004b517          	auipc	a0,0x4b
    80203e52:	8e250513          	addi	a0,a0,-1822 # 8024e730 <ftable>
    80203e56:	ffffd097          	auipc	ra,0xffffd
    80203e5a:	8f2080e7          	jalr	-1806(ra) # 80200748 <release>
  } else if (ff.type == FD_DEVICE) {

  }
}
    80203e5e:	70e2                	ld	ra,56(sp)
    80203e60:	7442                	ld	s0,48(sp)
    80203e62:	74a2                	ld	s1,40(sp)
    80203e64:	7902                	ld	s2,32(sp)
    80203e66:	69e2                	ld	s3,24(sp)
    80203e68:	6a42                	ld	s4,16(sp)
    80203e6a:	6aa2                	ld	s5,8(sp)
    80203e6c:	6121                	addi	sp,sp,64
    80203e6e:	8082                	ret
    pipeclose(ff.pipe, ff.writable);
    80203e70:	85d2                	mv	a1,s4
    80203e72:	854e                	mv	a0,s3
    80203e74:	00000097          	auipc	ra,0x0
    80203e78:	3a0080e7          	jalr	928(ra) # 80204214 <pipeclose>
    80203e7c:	b7cd                	j	80203e5e <fileclose+0x96>

0000000080203e7e <filestat>:
filestat(struct file *f, uint64 addr)
{
  // struct proc *p = myproc();
  struct stat st;
  
  if(f->type == FD_ENTRY){
    80203e7e:	4118                	lw	a4,0(a0)
    80203e80:	4789                	li	a5,2
    80203e82:	04f71e63          	bne	a4,a5,80203ede <filestat+0x60>
{
    80203e86:	7159                	addi	sp,sp,-112
    80203e88:	f486                	sd	ra,104(sp)
    80203e8a:	f0a2                	sd	s0,96(sp)
    80203e8c:	eca6                	sd	s1,88(sp)
    80203e8e:	e8ca                	sd	s2,80(sp)
    80203e90:	e4ce                	sd	s3,72(sp)
    80203e92:	1880                	addi	s0,sp,112
    80203e94:	84aa                	mv	s1,a0
    80203e96:	892e                	mv	s2,a1
    elock(f->ep);
    80203e98:	6d08                	ld	a0,24(a0)
    80203e9a:	00003097          	auipc	ra,0x3
    80203e9e:	dc4080e7          	jalr	-572(ra) # 80206c5e <elock>
    estat(f->ep, &st);
    80203ea2:	f9840993          	addi	s3,s0,-104
    80203ea6:	85ce                	mv	a1,s3
    80203ea8:	6c88                	ld	a0,24(s1)
    80203eaa:	00003097          	auipc	ra,0x3
    80203eae:	f70080e7          	jalr	-144(ra) # 80206e1a <estat>
    eunlock(f->ep);
    80203eb2:	6c88                	ld	a0,24(s1)
    80203eb4:	00003097          	auipc	ra,0x3
    80203eb8:	de0080e7          	jalr	-544(ra) # 80206c94 <eunlock>
    // if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    if(copyout2(addr, (char *)&st, sizeof(st)) < 0)
    80203ebc:	03800613          	li	a2,56
    80203ec0:	85ce                	mv	a1,s3
    80203ec2:	854a                	mv	a0,s2
    80203ec4:	ffffd097          	auipc	ra,0xffffd
    80203ec8:	53a080e7          	jalr	1338(ra) # 802013fe <copyout2>
    80203ecc:	41f5551b          	sraiw	a0,a0,0x1f
      return -1;
    return 0;
  }
  return -1;
}
    80203ed0:	70a6                	ld	ra,104(sp)
    80203ed2:	7406                	ld	s0,96(sp)
    80203ed4:	64e6                	ld	s1,88(sp)
    80203ed6:	6946                	ld	s2,80(sp)
    80203ed8:	69a6                	ld	s3,72(sp)
    80203eda:	6165                	addi	sp,sp,112
    80203edc:	8082                	ret
  return -1;
    80203ede:	557d                	li	a0,-1
}
    80203ee0:	8082                	ret

0000000080203ee2 <fileread>:

// Read from file f.
// addr is a user virtual address.
int
fileread(struct file *f, uint64 addr, int n)
{
    80203ee2:	7179                	addi	sp,sp,-48
    80203ee4:	f406                	sd	ra,40(sp)
    80203ee6:	f022                	sd	s0,32(sp)
    80203ee8:	ec26                	sd	s1,24(sp)
    80203eea:	e84a                	sd	s2,16(sp)
    80203eec:	e44e                	sd	s3,8(sp)
    80203eee:	1800                	addi	s0,sp,48
  int r = 0;

  if(f->readable == 0)
    80203ef0:	00854783          	lbu	a5,8(a0)
    80203ef4:	c3d5                	beqz	a5,80203f98 <fileread+0xb6>
    80203ef6:	84aa                	mv	s1,a0
    80203ef8:	89ae                	mv	s3,a1
    80203efa:	8932                	mv	s2,a2
    return -1;

  switch (f->type) {
    80203efc:	411c                	lw	a5,0(a0)
    80203efe:	4709                	li	a4,2
    80203f00:	06e78263          	beq	a5,a4,80203f64 <fileread+0x82>
    80203f04:	470d                	li	a4,3
    80203f06:	02e78b63          	beq	a5,a4,80203f3c <fileread+0x5a>
    80203f0a:	4705                	li	a4,1
    80203f0c:	00e78a63          	beq	a5,a4,80203f20 <fileread+0x3e>
          if((r = eread(f->ep, 1, addr, f->off, n)) > 0)
            f->off += r;
        eunlock(f->ep);
        break;
    default:
      panic("fileread");
    80203f10:	00006517          	auipc	a0,0x6
    80203f14:	e1050513          	addi	a0,a0,-496 # 80209d20 <digits+0x9a0>
    80203f18:	ffffc097          	auipc	ra,0xffffc
    80203f1c:	22c080e7          	jalr	556(ra) # 80200144 <panic>
        r = piperead(f->pipe, addr, n);
    80203f20:	6908                	ld	a0,16(a0)
    80203f22:	00000097          	auipc	ra,0x0
    80203f26:	466080e7          	jalr	1126(ra) # 80204388 <piperead>
    80203f2a:	892a                	mv	s2,a0
  }

  return r;
}
    80203f2c:	854a                	mv	a0,s2
    80203f2e:	70a2                	ld	ra,40(sp)
    80203f30:	7402                	ld	s0,32(sp)
    80203f32:	64e2                	ld	s1,24(sp)
    80203f34:	6942                	ld	s2,16(sp)
    80203f36:	69a2                	ld	s3,8(sp)
    80203f38:	6145                	addi	sp,sp,48
    80203f3a:	8082                	ret
        if(f->major < 0 || f->major >= NDEV || !devsw[f->major].read)
    80203f3c:	02451783          	lh	a5,36(a0)
    80203f40:	03079693          	slli	a3,a5,0x30
    80203f44:	92c1                	srli	a3,a3,0x30
    80203f46:	4725                	li	a4,9
    80203f48:	04d76a63          	bltu	a4,a3,80203f9c <fileread+0xba>
    80203f4c:	0792                	slli	a5,a5,0x4
    80203f4e:	0004a717          	auipc	a4,0x4a
    80203f52:	74270713          	addi	a4,a4,1858 # 8024e690 <devsw>
    80203f56:	97ba                	add	a5,a5,a4
    80203f58:	639c                	ld	a5,0(a5)
    80203f5a:	c3b9                	beqz	a5,80203fa0 <fileread+0xbe>
        r = devsw[f->major].read(1, addr, n);
    80203f5c:	4505                	li	a0,1
    80203f5e:	9782                	jalr	a5
    80203f60:	892a                	mv	s2,a0
        break;
    80203f62:	b7e9                	j	80203f2c <fileread+0x4a>
        elock(f->ep);
    80203f64:	6d08                	ld	a0,24(a0)
    80203f66:	00003097          	auipc	ra,0x3
    80203f6a:	cf8080e7          	jalr	-776(ra) # 80206c5e <elock>
          if((r = eread(f->ep, 1, addr, f->off, n)) > 0)
    80203f6e:	874a                	mv	a4,s2
    80203f70:	5094                	lw	a3,32(s1)
    80203f72:	864e                	mv	a2,s3
    80203f74:	4585                	li	a1,1
    80203f76:	6c88                	ld	a0,24(s1)
    80203f78:	00002097          	auipc	ra,0x2
    80203f7c:	3e2080e7          	jalr	994(ra) # 8020635a <eread>
    80203f80:	892a                	mv	s2,a0
    80203f82:	00a05563          	blez	a0,80203f8c <fileread+0xaa>
            f->off += r;
    80203f86:	509c                	lw	a5,32(s1)
    80203f88:	9fa9                	addw	a5,a5,a0
    80203f8a:	d09c                	sw	a5,32(s1)
        eunlock(f->ep);
    80203f8c:	6c88                	ld	a0,24(s1)
    80203f8e:	00003097          	auipc	ra,0x3
    80203f92:	d06080e7          	jalr	-762(ra) # 80206c94 <eunlock>
        break;
    80203f96:	bf59                	j	80203f2c <fileread+0x4a>
    return -1;
    80203f98:	597d                	li	s2,-1
    80203f9a:	bf49                	j	80203f2c <fileread+0x4a>
          return -1;
    80203f9c:	597d                	li	s2,-1
    80203f9e:	b779                	j	80203f2c <fileread+0x4a>
    80203fa0:	597d                	li	s2,-1
    80203fa2:	b769                	j	80203f2c <fileread+0x4a>

0000000080203fa4 <filewrite>:

// Write to file f.
// addr is a user virtual address.
int
filewrite(struct file *f, uint64 addr, int n)
{
    80203fa4:	7179                	addi	sp,sp,-48
    80203fa6:	f406                	sd	ra,40(sp)
    80203fa8:	f022                	sd	s0,32(sp)
    80203faa:	ec26                	sd	s1,24(sp)
    80203fac:	e84a                	sd	s2,16(sp)
    80203fae:	e44e                	sd	s3,8(sp)
    80203fb0:	e052                	sd	s4,0(sp)
    80203fb2:	1800                	addi	s0,sp,48
  int ret = 0;

  if(f->writable == 0)
    80203fb4:	00954783          	lbu	a5,9(a0)
    80203fb8:	cbc5                	beqz	a5,80204068 <filewrite+0xc4>
    80203fba:	84aa                	mv	s1,a0
    80203fbc:	892e                	mv	s2,a1
    80203fbe:	89b2                	mv	s3,a2
    return -1;

  if(f->type == FD_PIPE){
    80203fc0:	411c                	lw	a5,0(a0)
    80203fc2:	4705                	li	a4,1
    80203fc4:	04e78963          	beq	a5,a4,80204016 <filewrite+0x72>
    ret = pipewrite(f->pipe, addr, n);
  } else if(f->type == FD_DEVICE){
    80203fc8:	470d                	li	a4,3
    80203fca:	04e78d63          	beq	a5,a4,80204024 <filewrite+0x80>
    if(f->major < 0 || f->major >= NDEV || !devsw[f->major].write)
      return -1;
    ret = devsw[f->major].write(1, addr, n);
  } else if(f->type == FD_ENTRY){
    80203fce:	4709                	li	a4,2
    80203fd0:	08e79463          	bne	a5,a4,80204058 <filewrite+0xb4>
    elock(f->ep);
    80203fd4:	6d08                	ld	a0,24(a0)
    80203fd6:	00003097          	auipc	ra,0x3
    80203fda:	c88080e7          	jalr	-888(ra) # 80206c5e <elock>
    if (ewrite(f->ep, 1, addr, f->off, n) == n) {
    80203fde:	00098a1b          	sext.w	s4,s3
    80203fe2:	8752                	mv	a4,s4
    80203fe4:	5094                	lw	a3,32(s1)
    80203fe6:	864a                	mv	a2,s2
    80203fe8:	4585                	li	a1,1
    80203fea:	6c88                	ld	a0,24(s1)
    80203fec:	00002097          	auipc	ra,0x2
    80203ff0:	466080e7          	jalr	1126(ra) # 80206452 <ewrite>
      ret = n;
      f->off += n;
    } else {
      ret = -1;
    80203ff4:	597d                	li	s2,-1
    if (ewrite(f->ep, 1, addr, f->off, n) == n) {
    80203ff6:	05350b63          	beq	a0,s3,8020404c <filewrite+0xa8>
    }
    eunlock(f->ep);
    80203ffa:	6c88                	ld	a0,24(s1)
    80203ffc:	00003097          	auipc	ra,0x3
    80204000:	c98080e7          	jalr	-872(ra) # 80206c94 <eunlock>
  } else {
    panic("filewrite");
  }

  return ret;
}
    80204004:	854a                	mv	a0,s2
    80204006:	70a2                	ld	ra,40(sp)
    80204008:	7402                	ld	s0,32(sp)
    8020400a:	64e2                	ld	s1,24(sp)
    8020400c:	6942                	ld	s2,16(sp)
    8020400e:	69a2                	ld	s3,8(sp)
    80204010:	6a02                	ld	s4,0(sp)
    80204012:	6145                	addi	sp,sp,48
    80204014:	8082                	ret
    ret = pipewrite(f->pipe, addr, n);
    80204016:	6908                	ld	a0,16(a0)
    80204018:	00000097          	auipc	ra,0x0
    8020401c:	26c080e7          	jalr	620(ra) # 80204284 <pipewrite>
    80204020:	892a                	mv	s2,a0
    80204022:	b7cd                	j	80204004 <filewrite+0x60>
    if(f->major < 0 || f->major >= NDEV || !devsw[f->major].write)
    80204024:	02451783          	lh	a5,36(a0)
    80204028:	03079693          	slli	a3,a5,0x30
    8020402c:	92c1                	srli	a3,a3,0x30
    8020402e:	4725                	li	a4,9
    80204030:	02d76e63          	bltu	a4,a3,8020406c <filewrite+0xc8>
    80204034:	0792                	slli	a5,a5,0x4
    80204036:	0004a717          	auipc	a4,0x4a
    8020403a:	65a70713          	addi	a4,a4,1626 # 8024e690 <devsw>
    8020403e:	97ba                	add	a5,a5,a4
    80204040:	679c                	ld	a5,8(a5)
    80204042:	c79d                	beqz	a5,80204070 <filewrite+0xcc>
    ret = devsw[f->major].write(1, addr, n);
    80204044:	4505                	li	a0,1
    80204046:	9782                	jalr	a5
    80204048:	892a                	mv	s2,a0
    8020404a:	bf6d                	j	80204004 <filewrite+0x60>
      f->off += n;
    8020404c:	509c                	lw	a5,32(s1)
    8020404e:	014787bb          	addw	a5,a5,s4
    80204052:	d09c                	sw	a5,32(s1)
      ret = n;
    80204054:	894e                	mv	s2,s3
    80204056:	b755                	j	80203ffa <filewrite+0x56>
    panic("filewrite");
    80204058:	00006517          	auipc	a0,0x6
    8020405c:	cd850513          	addi	a0,a0,-808 # 80209d30 <digits+0x9b0>
    80204060:	ffffc097          	auipc	ra,0xffffc
    80204064:	0e4080e7          	jalr	228(ra) # 80200144 <panic>
    return -1;
    80204068:	597d                	li	s2,-1
    8020406a:	bf69                	j	80204004 <filewrite+0x60>
      return -1;
    8020406c:	597d                	li	s2,-1
    8020406e:	bf59                	j	80204004 <filewrite+0x60>
    80204070:	597d                	li	s2,-1
    80204072:	bf49                	j	80204004 <filewrite+0x60>

0000000080204074 <dirnext>:
int
dirnext(struct file *f, uint64 addr)
{
  // struct proc *p = myproc();

  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    80204074:	00854783          	lbu	a5,8(a0)
    80204078:	cfdd                	beqz	a5,80204136 <dirnext+0xc2>
{
    8020407a:	7141                	addi	sp,sp,-496
    8020407c:	f786                	sd	ra,488(sp)
    8020407e:	f3a2                	sd	s0,480(sp)
    80204080:	efa6                	sd	s1,472(sp)
    80204082:	ebca                	sd	s2,464(sp)
    80204084:	e7ce                	sd	s3,456(sp)
    80204086:	e3d2                	sd	s4,448(sp)
    80204088:	ff56                	sd	s5,440(sp)
    8020408a:	1b80                	addi	s0,sp,496
    8020408c:	84aa                	mv	s1,a0
    8020408e:	8aae                	mv	s5,a1
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    80204090:	6d18                	ld	a4,24(a0)
    80204092:	10074783          	lbu	a5,256(a4)
    80204096:	8bc1                	andi	a5,a5,16
    return -1;
    80204098:	557d                	li	a0,-1
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    8020409a:	eb91                	bnez	a5,802040ae <dirnext+0x3a>
  // if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
  if(copyout2(addr, (char *)&st, sizeof(st)) < 0)
    return -1;

  return 1;
    8020409c:	70be                	ld	ra,488(sp)
    8020409e:	741e                	ld	s0,480(sp)
    802040a0:	64fe                	ld	s1,472(sp)
    802040a2:	695e                	ld	s2,464(sp)
    802040a4:	69be                	ld	s3,456(sp)
    802040a6:	6a1e                	ld	s4,448(sp)
    802040a8:	7afa                	ld	s5,440(sp)
    802040aa:	617d                	addi	sp,sp,496
    802040ac:	8082                	ret
  int count = 0;
    802040ae:	e0042e23          	sw	zero,-484(s0)
  elock(f->ep);
    802040b2:	853a                	mv	a0,a4
    802040b4:	00003097          	auipc	ra,0x3
    802040b8:	baa080e7          	jalr	-1110(ra) # 80206c5e <elock>
  while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
    802040bc:	e1c40a13          	addi	s4,s0,-484
    802040c0:	e5840993          	addi	s3,s0,-424
    802040c4:	a801                	j	802040d4 <dirnext+0x60>
    f->off += count * 32;
    802040c6:	e1c42783          	lw	a5,-484(s0)
    802040ca:	0057979b          	slliw	a5,a5,0x5
    802040ce:	5098                	lw	a4,32(s1)
    802040d0:	9fb9                	addw	a5,a5,a4
    802040d2:	d09c                	sw	a5,32(s1)
  while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
    802040d4:	86d2                	mv	a3,s4
    802040d6:	5090                	lw	a2,32(s1)
    802040d8:	85ce                	mv	a1,s3
    802040da:	6c88                	ld	a0,24(s1)
    802040dc:	00003097          	auipc	ra,0x3
    802040e0:	d86080e7          	jalr	-634(ra) # 80206e62 <enext>
    802040e4:	892a                	mv	s2,a0
    802040e6:	d165                	beqz	a0,802040c6 <dirnext+0x52>
  eunlock(f->ep);
    802040e8:	6c88                	ld	a0,24(s1)
    802040ea:	00003097          	auipc	ra,0x3
    802040ee:	baa080e7          	jalr	-1110(ra) # 80206c94 <eunlock>
  if (ret == -1)
    802040f2:	57fd                	li	a5,-1
    return 0;
    802040f4:	4501                	li	a0,0
  if (ret == -1)
    802040f6:	faf903e3          	beq	s2,a5,8020409c <dirnext+0x28>
  f->off += count * 32;
    802040fa:	e1c42783          	lw	a5,-484(s0)
    802040fe:	0057979b          	slliw	a5,a5,0x5
    80204102:	5098                	lw	a4,32(s1)
    80204104:	9fb9                	addw	a5,a5,a4
    80204106:	d09c                	sw	a5,32(s1)
  estat(&de, &st);
    80204108:	e2040493          	addi	s1,s0,-480
    8020410c:	85a6                	mv	a1,s1
    8020410e:	e5840513          	addi	a0,s0,-424
    80204112:	00003097          	auipc	ra,0x3
    80204116:	d08080e7          	jalr	-760(ra) # 80206e1a <estat>
  if(copyout2(addr, (char *)&st, sizeof(st)) < 0)
    8020411a:	03800613          	li	a2,56
    8020411e:	85a6                	mv	a1,s1
    80204120:	8556                	mv	a0,s5
    80204122:	ffffd097          	auipc	ra,0xffffd
    80204126:	2dc080e7          	jalr	732(ra) # 802013fe <copyout2>
    return -1;
    8020412a:	fff54513          	not	a0,a0
    8020412e:	957d                	srai	a0,a0,0x3f
    80204130:	8909                	andi	a0,a0,2
    80204132:	157d                	addi	a0,a0,-1
    80204134:	b7a5                	j	8020409c <dirnext+0x28>
    return -1;
    80204136:	557d                	li	a0,-1
    80204138:	8082                	ret

000000008020413a <pipealloc>:
#include "include/kalloc.h"
#include "include/vm.h"

int
pipealloc(struct file **f0, struct file **f1)
{
    8020413a:	7179                	addi	sp,sp,-48
    8020413c:	f406                	sd	ra,40(sp)
    8020413e:	f022                	sd	s0,32(sp)
    80204140:	ec26                	sd	s1,24(sp)
    80204142:	e84a                	sd	s2,16(sp)
    80204144:	e44e                	sd	s3,8(sp)
    80204146:	e052                	sd	s4,0(sp)
    80204148:	1800                	addi	s0,sp,48
    8020414a:	84aa                	mv	s1,a0
    8020414c:	8a2e                	mv	s4,a1
  struct pipe *pi;

  pi = 0;
  *f0 = *f1 = 0;
    8020414e:	0005b023          	sd	zero,0(a1)
    80204152:	00053023          	sd	zero,0(a0)
  if((*f0 = filealloc()) == NULL || (*f1 = filealloc()) == NULL)
    80204156:	00000097          	auipc	ra,0x0
    8020415a:	bb6080e7          	jalr	-1098(ra) # 80203d0c <filealloc>
    8020415e:	e088                	sd	a0,0(s1)
    80204160:	c551                	beqz	a0,802041ec <pipealloc+0xb2>
    80204162:	00000097          	auipc	ra,0x0
    80204166:	baa080e7          	jalr	-1110(ra) # 80203d0c <filealloc>
    8020416a:	00aa3023          	sd	a0,0(s4)
    8020416e:	c92d                	beqz	a0,802041e0 <pipealloc+0xa6>
    goto bad;
  if((pi = (struct pipe*)kalloc()) == NULL)
    80204170:	ffffc097          	auipc	ra,0xffffc
    80204174:	418080e7          	jalr	1048(ra) # 80200588 <kalloc>
    80204178:	892a                	mv	s2,a0
    8020417a:	c125                	beqz	a0,802041da <pipealloc+0xa0>
    goto bad;
  pi->readopen = 1;
    8020417c:	4985                	li	s3,1
    8020417e:	23352023          	sw	s3,544(a0)
  pi->writeopen = 1;
    80204182:	23352223          	sw	s3,548(a0)
  pi->nwrite = 0;
    80204186:	20052e23          	sw	zero,540(a0)
  pi->nread = 0;
    8020418a:	20052c23          	sw	zero,536(a0)
  initlock(&pi->lock, "pipe");
    8020418e:	00006597          	auipc	a1,0x6
    80204192:	a2a58593          	addi	a1,a1,-1494 # 80209bb8 <digits+0x838>
    80204196:	ffffc097          	auipc	ra,0xffffc
    8020419a:	51a080e7          	jalr	1306(ra) # 802006b0 <initlock>
  (*f0)->type = FD_PIPE;
    8020419e:	609c                	ld	a5,0(s1)
    802041a0:	0137a023          	sw	s3,0(a5)
  (*f0)->readable = 1;
    802041a4:	609c                	ld	a5,0(s1)
    802041a6:	01378423          	sb	s3,8(a5)
  (*f0)->writable = 0;
    802041aa:	609c                	ld	a5,0(s1)
    802041ac:	000784a3          	sb	zero,9(a5)
  (*f0)->pipe = pi;
    802041b0:	609c                	ld	a5,0(s1)
    802041b2:	0127b823          	sd	s2,16(a5)
  (*f1)->type = FD_PIPE;
    802041b6:	000a3783          	ld	a5,0(s4)
    802041ba:	0137a023          	sw	s3,0(a5)
  (*f1)->readable = 0;
    802041be:	000a3783          	ld	a5,0(s4)
    802041c2:	00078423          	sb	zero,8(a5)
  (*f1)->writable = 1;
    802041c6:	000a3783          	ld	a5,0(s4)
    802041ca:	013784a3          	sb	s3,9(a5)
  (*f1)->pipe = pi;
    802041ce:	000a3783          	ld	a5,0(s4)
    802041d2:	0127b823          	sd	s2,16(a5)
  return 0;
    802041d6:	4501                	li	a0,0
    802041d8:	a025                	j	80204200 <pipealloc+0xc6>

 bad:
  if(pi)
    kfree((char*)pi);
  if(*f0)
    802041da:	6088                	ld	a0,0(s1)
    802041dc:	e501                	bnez	a0,802041e4 <pipealloc+0xaa>
    802041de:	a039                	j	802041ec <pipealloc+0xb2>
    802041e0:	6088                	ld	a0,0(s1)
    802041e2:	c51d                	beqz	a0,80204210 <pipealloc+0xd6>
    fileclose(*f0);
    802041e4:	00000097          	auipc	ra,0x0
    802041e8:	be4080e7          	jalr	-1052(ra) # 80203dc8 <fileclose>
  if(*f1)
    802041ec:	000a3783          	ld	a5,0(s4)
    fileclose(*f1);
  return -1;
    802041f0:	557d                	li	a0,-1
  if(*f1)
    802041f2:	c799                	beqz	a5,80204200 <pipealloc+0xc6>
    fileclose(*f1);
    802041f4:	853e                	mv	a0,a5
    802041f6:	00000097          	auipc	ra,0x0
    802041fa:	bd2080e7          	jalr	-1070(ra) # 80203dc8 <fileclose>
  return -1;
    802041fe:	557d                	li	a0,-1
}
    80204200:	70a2                	ld	ra,40(sp)
    80204202:	7402                	ld	s0,32(sp)
    80204204:	64e2                	ld	s1,24(sp)
    80204206:	6942                	ld	s2,16(sp)
    80204208:	69a2                	ld	s3,8(sp)
    8020420a:	6a02                	ld	s4,0(sp)
    8020420c:	6145                	addi	sp,sp,48
    8020420e:	8082                	ret
  return -1;
    80204210:	557d                	li	a0,-1
    80204212:	b7fd                	j	80204200 <pipealloc+0xc6>

0000000080204214 <pipeclose>:

void
pipeclose(struct pipe *pi, int writable)
{
    80204214:	1101                	addi	sp,sp,-32
    80204216:	ec06                	sd	ra,24(sp)
    80204218:	e822                	sd	s0,16(sp)
    8020421a:	e426                	sd	s1,8(sp)
    8020421c:	e04a                	sd	s2,0(sp)
    8020421e:	1000                	addi	s0,sp,32
    80204220:	84aa                	mv	s1,a0
    80204222:	892e                	mv	s2,a1
  acquire(&pi->lock);
    80204224:	ffffc097          	auipc	ra,0xffffc
    80204228:	4d0080e7          	jalr	1232(ra) # 802006f4 <acquire>
  if(writable){
    8020422c:	02090d63          	beqz	s2,80204266 <pipeclose+0x52>
    pi->writeopen = 0;
    80204230:	2204a223          	sw	zero,548(s1)
    wakeup(&pi->nread);
    80204234:	21848513          	addi	a0,s1,536
    80204238:	ffffe097          	auipc	ra,0xffffe
    8020423c:	400080e7          	jalr	1024(ra) # 80202638 <wakeup>
  } else {
    pi->readopen = 0;
    wakeup(&pi->nwrite);
  }
  if(pi->readopen == 0 && pi->writeopen == 0){
    80204240:	2204b783          	ld	a5,544(s1)
    80204244:	eb95                	bnez	a5,80204278 <pipeclose+0x64>
    release(&pi->lock);
    80204246:	8526                	mv	a0,s1
    80204248:	ffffc097          	auipc	ra,0xffffc
    8020424c:	500080e7          	jalr	1280(ra) # 80200748 <release>
    kfree((char*)pi);
    80204250:	8526                	mv	a0,s1
    80204252:	ffffc097          	auipc	ra,0xffffc
    80204256:	1ee080e7          	jalr	494(ra) # 80200440 <kfree>
  } else
    release(&pi->lock);
}
    8020425a:	60e2                	ld	ra,24(sp)
    8020425c:	6442                	ld	s0,16(sp)
    8020425e:	64a2                	ld	s1,8(sp)
    80204260:	6902                	ld	s2,0(sp)
    80204262:	6105                	addi	sp,sp,32
    80204264:	8082                	ret
    pi->readopen = 0;
    80204266:	2204a023          	sw	zero,544(s1)
    wakeup(&pi->nwrite);
    8020426a:	21c48513          	addi	a0,s1,540
    8020426e:	ffffe097          	auipc	ra,0xffffe
    80204272:	3ca080e7          	jalr	970(ra) # 80202638 <wakeup>
    80204276:	b7e9                	j	80204240 <pipeclose+0x2c>
    release(&pi->lock);
    80204278:	8526                	mv	a0,s1
    8020427a:	ffffc097          	auipc	ra,0xffffc
    8020427e:	4ce080e7          	jalr	1230(ra) # 80200748 <release>
}
    80204282:	bfe1                	j	8020425a <pipeclose+0x46>

0000000080204284 <pipewrite>:

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
    80204284:	7159                	addi	sp,sp,-112
    80204286:	f486                	sd	ra,104(sp)
    80204288:	f0a2                	sd	s0,96(sp)
    8020428a:	eca6                	sd	s1,88(sp)
    8020428c:	e8ca                	sd	s2,80(sp)
    8020428e:	e4ce                	sd	s3,72(sp)
    80204290:	e0d2                	sd	s4,64(sp)
    80204292:	fc56                	sd	s5,56(sp)
    80204294:	f85a                	sd	s6,48(sp)
    80204296:	f45e                	sd	s7,40(sp)
    80204298:	f062                	sd	s8,32(sp)
    8020429a:	ec66                	sd	s9,24(sp)
    8020429c:	1880                	addi	s0,sp,112
    8020429e:	84aa                	mv	s1,a0
    802042a0:	8b2e                	mv	s6,a1
    802042a2:	8ab2                	mv	s5,a2
  int i;
  char ch;
  struct proc *pr = myproc();
    802042a4:	ffffe097          	auipc	ra,0xffffe
    802042a8:	834080e7          	jalr	-1996(ra) # 80201ad8 <myproc>
    802042ac:	892a                	mv	s2,a0

  acquire(&pi->lock);
    802042ae:	8526                	mv	a0,s1
    802042b0:	ffffc097          	auipc	ra,0xffffc
    802042b4:	444080e7          	jalr	1092(ra) # 802006f4 <acquire>
  for(i = 0; i < n; i++){
    802042b8:	09505763          	blez	s5,80204346 <pipewrite+0xc2>
    802042bc:	4b81                	li	s7,0
    while(pi->nwrite == pi->nread + PIPESIZE){  //DOC: pipewrite-full
      if(pi->readopen == 0 || pr->killed){
        release(&pi->lock);
        return -1;
      }
      wakeup(&pi->nread);
    802042be:	21848a13          	addi	s4,s1,536
      sleep(&pi->nwrite, &pi->lock);
    802042c2:	21c48993          	addi	s3,s1,540
    }
    // if(copyin(pr->pagetable, &ch, addr + i, 1) == -1)
    if(copyin2(&ch, addr + i, 1) == -1)
    802042c6:	f9f40c93          	addi	s9,s0,-97
    802042ca:	5c7d                	li	s8,-1
    while(pi->nwrite == pi->nread + PIPESIZE){  //DOC: pipewrite-full
    802042cc:	2184a783          	lw	a5,536(s1)
    802042d0:	21c4a703          	lw	a4,540(s1)
    802042d4:	2007879b          	addiw	a5,a5,512
    802042d8:	02f71b63          	bne	a4,a5,8020430e <pipewrite+0x8a>
      if(pi->readopen == 0 || pr->killed){
    802042dc:	2204a783          	lw	a5,544(s1)
    802042e0:	c3c1                	beqz	a5,80204360 <pipewrite+0xdc>
    802042e2:	03092783          	lw	a5,48(s2)
    802042e6:	efad                	bnez	a5,80204360 <pipewrite+0xdc>
      wakeup(&pi->nread);
    802042e8:	8552                	mv	a0,s4
    802042ea:	ffffe097          	auipc	ra,0xffffe
    802042ee:	34e080e7          	jalr	846(ra) # 80202638 <wakeup>
      sleep(&pi->nwrite, &pi->lock);
    802042f2:	85a6                	mv	a1,s1
    802042f4:	854e                	mv	a0,s3
    802042f6:	ffffe097          	auipc	ra,0xffffe
    802042fa:	1aa080e7          	jalr	426(ra) # 802024a0 <sleep>
    while(pi->nwrite == pi->nread + PIPESIZE){  //DOC: pipewrite-full
    802042fe:	2184a783          	lw	a5,536(s1)
    80204302:	21c4a703          	lw	a4,540(s1)
    80204306:	2007879b          	addiw	a5,a5,512
    8020430a:	fcf709e3          	beq	a4,a5,802042dc <pipewrite+0x58>
    if(copyin2(&ch, addr + i, 1) == -1)
    8020430e:	4605                	li	a2,1
    80204310:	85da                	mv	a1,s6
    80204312:	8566                	mv	a0,s9
    80204314:	ffffd097          	auipc	ra,0xffffd
    80204318:	1ca080e7          	jalr	458(ra) # 802014de <copyin2>
    8020431c:	03850663          	beq	a0,s8,80204348 <pipewrite+0xc4>
      break;
    pi->data[pi->nwrite++ % PIPESIZE] = ch;
    80204320:	21c4a783          	lw	a5,540(s1)
    80204324:	0017871b          	addiw	a4,a5,1
    80204328:	20e4ae23          	sw	a4,540(s1)
    8020432c:	1ff7f793          	andi	a5,a5,511
    80204330:	97a6                	add	a5,a5,s1
    80204332:	f9f44703          	lbu	a4,-97(s0)
    80204336:	00e78c23          	sb	a4,24(a5)
  for(i = 0; i < n; i++){
    8020433a:	2b85                	addiw	s7,s7,1
    8020433c:	0b05                	addi	s6,s6,1
    8020433e:	f97a97e3          	bne	s5,s7,802042cc <pipewrite+0x48>
    80204342:	8bd6                	mv	s7,s5
    80204344:	a011                	j	80204348 <pipewrite+0xc4>
    80204346:	4b81                	li	s7,0
  }
  wakeup(&pi->nread);
    80204348:	21848513          	addi	a0,s1,536
    8020434c:	ffffe097          	auipc	ra,0xffffe
    80204350:	2ec080e7          	jalr	748(ra) # 80202638 <wakeup>
  release(&pi->lock);
    80204354:	8526                	mv	a0,s1
    80204356:	ffffc097          	auipc	ra,0xffffc
    8020435a:	3f2080e7          	jalr	1010(ra) # 80200748 <release>
  return i;
    8020435e:	a039                	j	8020436c <pipewrite+0xe8>
        release(&pi->lock);
    80204360:	8526                	mv	a0,s1
    80204362:	ffffc097          	auipc	ra,0xffffc
    80204366:	3e6080e7          	jalr	998(ra) # 80200748 <release>
        return -1;
    8020436a:	5bfd                	li	s7,-1
}
    8020436c:	855e                	mv	a0,s7
    8020436e:	70a6                	ld	ra,104(sp)
    80204370:	7406                	ld	s0,96(sp)
    80204372:	64e6                	ld	s1,88(sp)
    80204374:	6946                	ld	s2,80(sp)
    80204376:	69a6                	ld	s3,72(sp)
    80204378:	6a06                	ld	s4,64(sp)
    8020437a:	7ae2                	ld	s5,56(sp)
    8020437c:	7b42                	ld	s6,48(sp)
    8020437e:	7ba2                	ld	s7,40(sp)
    80204380:	7c02                	ld	s8,32(sp)
    80204382:	6ce2                	ld	s9,24(sp)
    80204384:	6165                	addi	sp,sp,112
    80204386:	8082                	ret

0000000080204388 <piperead>:

int
piperead(struct pipe *pi, uint64 addr, int n)
{
    80204388:	715d                	addi	sp,sp,-80
    8020438a:	e486                	sd	ra,72(sp)
    8020438c:	e0a2                	sd	s0,64(sp)
    8020438e:	fc26                	sd	s1,56(sp)
    80204390:	f84a                	sd	s2,48(sp)
    80204392:	f44e                	sd	s3,40(sp)
    80204394:	f052                	sd	s4,32(sp)
    80204396:	ec56                	sd	s5,24(sp)
    80204398:	e85a                	sd	s6,16(sp)
    8020439a:	0880                	addi	s0,sp,80
    8020439c:	84aa                	mv	s1,a0
    8020439e:	892e                	mv	s2,a1
    802043a0:	8a32                	mv	s4,a2
  int i;
  struct proc *pr = myproc();
    802043a2:	ffffd097          	auipc	ra,0xffffd
    802043a6:	736080e7          	jalr	1846(ra) # 80201ad8 <myproc>
    802043aa:	89aa                	mv	s3,a0
  char ch;

  acquire(&pi->lock);
    802043ac:	8526                	mv	a0,s1
    802043ae:	ffffc097          	auipc	ra,0xffffc
    802043b2:	346080e7          	jalr	838(ra) # 802006f4 <acquire>
  while(pi->nread == pi->nwrite && pi->writeopen){  //DOC: pipe-empty
    802043b6:	2184a703          	lw	a4,536(s1)
    802043ba:	21c4a783          	lw	a5,540(s1)
    if(pr->killed){
      release(&pi->lock);
      return -1;
    }
    sleep(&pi->nread, &pi->lock); //DOC: piperead-sleep
    802043be:	21848a93          	addi	s5,s1,536
  while(pi->nread == pi->nwrite && pi->writeopen){  //DOC: pipe-empty
    802043c2:	02f71463          	bne	a4,a5,802043ea <piperead+0x62>
    802043c6:	2244a783          	lw	a5,548(s1)
    802043ca:	c385                	beqz	a5,802043ea <piperead+0x62>
    if(pr->killed){
    802043cc:	0309a783          	lw	a5,48(s3)
    802043d0:	ebc1                	bnez	a5,80204460 <piperead+0xd8>
    sleep(&pi->nread, &pi->lock); //DOC: piperead-sleep
    802043d2:	85a6                	mv	a1,s1
    802043d4:	8556                	mv	a0,s5
    802043d6:	ffffe097          	auipc	ra,0xffffe
    802043da:	0ca080e7          	jalr	202(ra) # 802024a0 <sleep>
  while(pi->nread == pi->nwrite && pi->writeopen){  //DOC: pipe-empty
    802043de:	2184a703          	lw	a4,536(s1)
    802043e2:	21c4a783          	lw	a5,540(s1)
    802043e6:	fef700e3          	beq	a4,a5,802043c6 <piperead+0x3e>
  }
  for(i = 0; i < n; i++){  //DOC: piperead-copy
    802043ea:	4981                	li	s3,0
    if(pi->nread == pi->nwrite)
      break;
    ch = pi->data[pi->nread++ % PIPESIZE];
    // if(copyout(pr->pagetable, addr + i, &ch, 1) == -1)
    if(copyout2(addr + i, &ch, 1) == -1)
    802043ec:	fbf40b13          	addi	s6,s0,-65
    802043f0:	5afd                	li	s5,-1
  for(i = 0; i < n; i++){  //DOC: piperead-copy
    802043f2:	05405163          	blez	s4,80204434 <piperead+0xac>
    if(pi->nread == pi->nwrite)
    802043f6:	2184a783          	lw	a5,536(s1)
    802043fa:	21c4a703          	lw	a4,540(s1)
    802043fe:	02f70b63          	beq	a4,a5,80204434 <piperead+0xac>
    ch = pi->data[pi->nread++ % PIPESIZE];
    80204402:	0017871b          	addiw	a4,a5,1
    80204406:	20e4ac23          	sw	a4,536(s1)
    8020440a:	1ff7f793          	andi	a5,a5,511
    8020440e:	97a6                	add	a5,a5,s1
    80204410:	0187c783          	lbu	a5,24(a5)
    80204414:	faf40fa3          	sb	a5,-65(s0)
    if(copyout2(addr + i, &ch, 1) == -1)
    80204418:	4605                	li	a2,1
    8020441a:	85da                	mv	a1,s6
    8020441c:	854a                	mv	a0,s2
    8020441e:	ffffd097          	auipc	ra,0xffffd
    80204422:	fe0080e7          	jalr	-32(ra) # 802013fe <copyout2>
    80204426:	01550763          	beq	a0,s5,80204434 <piperead+0xac>
  for(i = 0; i < n; i++){  //DOC: piperead-copy
    8020442a:	2985                	addiw	s3,s3,1
    8020442c:	0905                	addi	s2,s2,1
    8020442e:	fd3a14e3          	bne	s4,s3,802043f6 <piperead+0x6e>
    80204432:	89d2                	mv	s3,s4
      break;
  }
  wakeup(&pi->nwrite);  //DOC: piperead-wakeup
    80204434:	21c48513          	addi	a0,s1,540
    80204438:	ffffe097          	auipc	ra,0xffffe
    8020443c:	200080e7          	jalr	512(ra) # 80202638 <wakeup>
  release(&pi->lock);
    80204440:	8526                	mv	a0,s1
    80204442:	ffffc097          	auipc	ra,0xffffc
    80204446:	306080e7          	jalr	774(ra) # 80200748 <release>
  return i;
}
    8020444a:	854e                	mv	a0,s3
    8020444c:	60a6                	ld	ra,72(sp)
    8020444e:	6406                	ld	s0,64(sp)
    80204450:	74e2                	ld	s1,56(sp)
    80204452:	7942                	ld	s2,48(sp)
    80204454:	79a2                	ld	s3,40(sp)
    80204456:	7a02                	ld	s4,32(sp)
    80204458:	6ae2                	ld	s5,24(sp)
    8020445a:	6b42                	ld	s6,16(sp)
    8020445c:	6161                	addi	sp,sp,80
    8020445e:	8082                	ret
      release(&pi->lock);
    80204460:	8526                	mv	a0,s1
    80204462:	ffffc097          	auipc	ra,0xffffc
    80204466:	2e6080e7          	jalr	742(ra) # 80200748 <release>
      return -1;
    8020446a:	59fd                	li	s3,-1
    8020446c:	bff9                	j	8020444a <piperead+0xc2>

000000008020446e <exec>:
  return 0;
}


int exec(char *path, char **argv)
{
    8020446e:	dd010113          	addi	sp,sp,-560
    80204472:	22113423          	sd	ra,552(sp)
    80204476:	22813023          	sd	s0,544(sp)
    8020447a:	20913c23          	sd	s1,536(sp)
    8020447e:	21213823          	sd	s2,528(sp)
    80204482:	21313423          	sd	s3,520(sp)
    80204486:	21413023          	sd	s4,512(sp)
    8020448a:	ffd6                	sd	s5,504(sp)
    8020448c:	fbda                	sd	s6,496(sp)
    8020448e:	f7de                	sd	s7,488(sp)
    80204490:	f3e2                	sd	s8,480(sp)
    80204492:	efe6                	sd	s9,472(sp)
    80204494:	ebea                	sd	s10,464(sp)
    80204496:	e7ee                	sd	s11,456(sp)
    80204498:	1c00                	addi	s0,sp,560
    8020449a:	89aa                	mv	s3,a0
    8020449c:	dea43023          	sd	a0,-544(s0)
    802044a0:	dcb43c23          	sd	a1,-552(s0)
  struct elfhdr elf;
  struct dirent *ep;
  struct proghdr ph;
  pagetable_t pagetable = 0, oldpagetable;
  pagetable_t kpagetable = 0, oldkpagetable;
  struct proc *p = myproc();
    802044a4:	ffffd097          	auipc	ra,0xffffd
    802044a8:	634080e7          	jalr	1588(ra) # 80201ad8 <myproc>
    802044ac:	84aa                	mv	s1,a0

  // Make a copy of p->kpt without old user space, 
  // but with the same kstack we are using now, which can't be changed
  if ((kpagetable = (pagetable_t)kalloc()) == NULL) {
    802044ae:	ffffc097          	auipc	ra,0xffffc
    802044b2:	0da080e7          	jalr	218(ra) # 80200588 <kalloc>
    802044b6:	e0a43423          	sd	a0,-504(s0)
    802044ba:	2a050f63          	beqz	a0,80204778 <exec+0x30a>
    802044be:	892a                	mv	s2,a0
    return -1;
  }
  memmove(kpagetable, p->kpagetable, PGSIZE);
    802044c0:	6605                	lui	a2,0x1
    802044c2:	6cac                	ld	a1,88(s1)
    802044c4:	ffffc097          	auipc	ra,0xffffc
    802044c8:	328080e7          	jalr	808(ra) # 802007ec <memmove>
  for (int i = 0; i < PX(2, MAXUVA); i++) {
    kpagetable[i] = 0;
    802044cc:	00093023          	sd	zero,0(s2)
    802044d0:	00093423          	sd	zero,8(s2)
  }

  if((ep = ename(path)) == NULL) {
    802044d4:	854e                	mv	a0,s3
    802044d6:	00003097          	auipc	ra,0x3
    802044da:	f30080e7          	jalr	-208(ra) # 80207406 <ename>
    802044de:	8a2a                	mv	s4,a0
    802044e0:	c529                	beqz	a0,8020452a <exec+0xbc>
    #ifdef DEBUG
    printf("[exec] %s not found\n", path);
    #endif
    goto bad;
  }
  elock(ep);
    802044e2:	00002097          	auipc	ra,0x2
    802044e6:	77c080e7          	jalr	1916(ra) # 80206c5e <elock>

  // Check ELF header
  if(eread(ep, 0, (uint64) &elf, 0, sizeof(elf)) != sizeof(elf))
    802044ea:	04000713          	li	a4,64
    802044ee:	4681                	li	a3,0
    802044f0:	e4840613          	addi	a2,s0,-440
    802044f4:	4581                	li	a1,0
    802044f6:	8552                	mv	a0,s4
    802044f8:	00002097          	auipc	ra,0x2
    802044fc:	e62080e7          	jalr	-414(ra) # 8020635a <eread>
    80204500:	04000793          	li	a5,64
    80204504:	00f51a63          	bne	a0,a5,80204518 <exec+0xaa>
    goto bad;
  if(elf.magic != ELF_MAGIC)
    80204508:	e4842703          	lw	a4,-440(s0)
    8020450c:	464c47b7          	lui	a5,0x464c4
    80204510:	57f78793          	addi	a5,a5,1407 # 464c457f <_entry-0x39d3ba81>
    80204514:	02f70663          	beq	a4,a5,80204540 <exec+0xd2>
  kvmfree(oldkpagetable, 0);
  return argc; // this ends up in a0, the first argument to main(argc, argv)

 bad:
  #ifdef DEBUG
  printf("[exec] reach bad\n");
    80204518:	00006517          	auipc	a0,0x6
    8020451c:	86050513          	addi	a0,a0,-1952 # 80209d78 <digits+0x9f8>
    80204520:	ffffc097          	auipc	ra,0xffffc
    80204524:	c6e080e7          	jalr	-914(ra) # 8020018e <printf>
  #endif
  if(pagetable)
    80204528:	ac9d                	j	8020479e <exec+0x330>
    printf("[exec] %s not found\n", path);
    8020452a:	de043583          	ld	a1,-544(s0)
    8020452e:	00006517          	auipc	a0,0x6
    80204532:	81250513          	addi	a0,a0,-2030 # 80209d40 <digits+0x9c0>
    80204536:	ffffc097          	auipc	ra,0xffffc
    8020453a:	c58080e7          	jalr	-936(ra) # 8020018e <printf>
    goto bad;
    8020453e:	bfe9                	j	80204518 <exec+0xaa>
  if((pagetable = proc_pagetable(p)) == NULL)
    80204540:	8526                	mv	a0,s1
    80204542:	ffffd097          	auipc	ra,0xffffd
    80204546:	67a080e7          	jalr	1658(ra) # 80201bbc <proc_pagetable>
    8020454a:	8b2a                	mv	s6,a0
    8020454c:	d571                	beqz	a0,80204518 <exec+0xaa>
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    8020454e:	e6842783          	lw	a5,-408(s0)
    80204552:	e8045703          	lhu	a4,-384(s0)
    80204556:	cb35                	beqz	a4,802045ca <exec+0x15c>
  uint64 argc, sz = 0, sp, ustack[MAXARG+1], stackbase;
    80204558:	4481                	li	s1,0
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    8020455a:	e0043023          	sd	zero,-512(s0)
    if(eread(ep, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph))
    8020455e:	e1040713          	addi	a4,s0,-496
    80204562:	dee43423          	sd	a4,-536(s0)
    if(ph.vaddr % PGSIZE != 0)
    80204566:	6a85                	lui	s5,0x1
    80204568:	fffa8713          	addi	a4,s5,-1 # fff <_entry-0x801ff001>
    8020456c:	dce43823          	sd	a4,-560(s0)
  for(i = 0; i < sz; i += PGSIZE){
    80204570:	6d85                	lui	s11,0x1
    80204572:	7d7d                	lui	s10,0xfffff
    80204574:	a4e9                	j	8020483e <exec+0x3d0>
      panic("loadseg: address should exist");
    80204576:	00005517          	auipc	a0,0x5
    8020457a:	7e250513          	addi	a0,a0,2018 # 80209d58 <digits+0x9d8>
    8020457e:	ffffc097          	auipc	ra,0xffffc
    80204582:	bc6080e7          	jalr	-1082(ra) # 80200144 <panic>
    if(eread(ep, 0, (uint64)pa, offset+i, n) != n)
    80204586:	874a                	mv	a4,s2
    80204588:	009c86bb          	addw	a3,s9,s1
    8020458c:	4581                	li	a1,0
    8020458e:	8552                	mv	a0,s4
    80204590:	00002097          	auipc	ra,0x2
    80204594:	dca080e7          	jalr	-566(ra) # 8020635a <eread>
    80204598:	2501                	sext.w	a0,a0
    8020459a:	1ea91363          	bne	s2,a0,80204780 <exec+0x312>
  for(i = 0; i < sz; i += PGSIZE){
    8020459e:	009d84bb          	addw	s1,s11,s1
    802045a2:	013d09bb          	addw	s3,s10,s3
    802045a6:	2774fc63          	bgeu	s1,s7,8020481e <exec+0x3b0>
    pa = walkaddr(pagetable, va + i);
    802045aa:	02049593          	slli	a1,s1,0x20
    802045ae:	9181                	srli	a1,a1,0x20
    802045b0:	95e2                	add	a1,a1,s8
    802045b2:	855a                	mv	a0,s6
    802045b4:	ffffc097          	auipc	ra,0xffffc
    802045b8:	660080e7          	jalr	1632(ra) # 80200c14 <walkaddr>
    802045bc:	862a                	mv	a2,a0
    if(pa == NULL)
    802045be:	dd45                	beqz	a0,80204576 <exec+0x108>
      n = PGSIZE;
    802045c0:	8956                	mv	s2,s5
    if(sz - i < PGSIZE)
    802045c2:	fd59f2e3          	bgeu	s3,s5,80204586 <exec+0x118>
      n = sz - i;
    802045c6:	894e                	mv	s2,s3
    802045c8:	bf7d                	j	80204586 <exec+0x118>
  uint64 argc, sz = 0, sp, ustack[MAXARG+1], stackbase;
    802045ca:	4481                	li	s1,0
  eunlock(ep);
    802045cc:	8552                	mv	a0,s4
    802045ce:	00002097          	auipc	ra,0x2
    802045d2:	6c6080e7          	jalr	1734(ra) # 80206c94 <eunlock>
  eput(ep);
    802045d6:	8552                	mv	a0,s4
    802045d8:	00002097          	auipc	ra,0x2
    802045dc:	70a080e7          	jalr	1802(ra) # 80206ce2 <eput>
  p = myproc();
    802045e0:	ffffd097          	auipc	ra,0xffffd
    802045e4:	4f8080e7          	jalr	1272(ra) # 80201ad8 <myproc>
    802045e8:	8aaa                	mv	s5,a0
  uint64 oldsz = p->sz;
    802045ea:	04853d03          	ld	s10,72(a0)
  sz = PGROUNDUP(sz);
    802045ee:	6785                	lui	a5,0x1
    802045f0:	17fd                	addi	a5,a5,-1 # fff <_entry-0x801ff001>
    802045f2:	97a6                	add	a5,a5,s1
    802045f4:	777d                	lui	a4,0xfffff
    802045f6:	8ff9                	and	a5,a5,a4
    802045f8:	def43823          	sd	a5,-528(s0)
  if((sz1 = uvmalloc(pagetable, kpagetable, sz, sz + 2*PGSIZE)) == 0)
    802045fc:	6689                	lui	a3,0x2
    802045fe:	96be                	add	a3,a3,a5
    80204600:	863e                	mv	a2,a5
    80204602:	e0843583          	ld	a1,-504(s0)
    80204606:	855a                	mv	a0,s6
    80204608:	ffffd097          	auipc	ra,0xffffd
    8020460c:	a8e080e7          	jalr	-1394(ra) # 80201096 <uvmalloc>
    80204610:	8caa                	mv	s9,a0
  ep = 0;
    80204612:	4a01                	li	s4,0
  if((sz1 = uvmalloc(pagetable, kpagetable, sz, sz + 2*PGSIZE)) == 0)
    80204614:	16050663          	beqz	a0,80204780 <exec+0x312>
  uvmclear(pagetable, sz-2*PGSIZE);
    80204618:	75f9                	lui	a1,0xffffe
    8020461a:	95aa                	add	a1,a1,a0
    8020461c:	855a                	mv	a0,s6
    8020461e:	ffffd097          	auipc	ra,0xffffd
    80204622:	d22080e7          	jalr	-734(ra) # 80201340 <uvmclear>
  stackbase = sp - PGSIZE;
    80204626:	7bfd                	lui	s7,0xfffff
    80204628:	9be6                	add	s7,s7,s9
  for(argc = 0; argv[argc]; argc++) {
    8020462a:	dd843783          	ld	a5,-552(s0)
    8020462e:	6388                	ld	a0,0(a5)
    80204630:	c925                	beqz	a0,802046a0 <exec+0x232>
    80204632:	e8840993          	addi	s3,s0,-376
    80204636:	f8840c13          	addi	s8,s0,-120
  sp = sz;
    8020463a:	8966                	mv	s2,s9
  for(argc = 0; argv[argc]; argc++) {
    8020463c:	4481                	li	s1,0
    sp -= strlen(argv[argc]) + 1;
    8020463e:	ffffc097          	auipc	ra,0xffffc
    80204642:	2d6080e7          	jalr	726(ra) # 80200914 <strlen>
    80204646:	0015079b          	addiw	a5,a0,1
    8020464a:	40f907b3          	sub	a5,s2,a5
    sp -= sp % 16; // riscv sp must be 16-byte aligned
    8020464e:	ff07f913          	andi	s2,a5,-16
    if(sp < stackbase)
    80204652:	1b796a63          	bltu	s2,s7,80204806 <exec+0x398>
    if(copyout(pagetable, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
    80204656:	dd843d83          	ld	s11,-552(s0)
    8020465a:	000dba03          	ld	s4,0(s11) # 1000 <_entry-0x801ff000>
    8020465e:	8552                	mv	a0,s4
    80204660:	ffffc097          	auipc	ra,0xffffc
    80204664:	2b4080e7          	jalr	692(ra) # 80200914 <strlen>
    80204668:	0015069b          	addiw	a3,a0,1
    8020466c:	8652                	mv	a2,s4
    8020466e:	85ca                	mv	a1,s2
    80204670:	855a                	mv	a0,s6
    80204672:	ffffd097          	auipc	ra,0xffffd
    80204676:	d00080e7          	jalr	-768(ra) # 80201372 <copyout>
    8020467a:	18054a63          	bltz	a0,8020480e <exec+0x3a0>
    ustack[argc] = sp;
    8020467e:	0129b023          	sd	s2,0(s3)
  for(argc = 0; argv[argc]; argc++) {
    80204682:	0485                	addi	s1,s1,1
    80204684:	008d8793          	addi	a5,s11,8
    80204688:	dcf43c23          	sd	a5,-552(s0)
    8020468c:	008db503          	ld	a0,8(s11)
    80204690:	c911                	beqz	a0,802046a4 <exec+0x236>
    if(argc >= MAXARG)
    80204692:	09a1                	addi	s3,s3,8
    80204694:	fb8995e3          	bne	s3,s8,8020463e <exec+0x1d0>
  sz = sz1;
    80204698:	df943823          	sd	s9,-528(s0)
  ep = 0;
    8020469c:	4a01                	li	s4,0
    8020469e:	a0cd                	j	80204780 <exec+0x312>
  sp = sz;
    802046a0:	8966                	mv	s2,s9
  for(argc = 0; argv[argc]; argc++) {
    802046a2:	4481                	li	s1,0
  ustack[argc] = 0;
    802046a4:	00349793          	slli	a5,s1,0x3
    802046a8:	f9078793          	addi	a5,a5,-112
    802046ac:	97a2                	add	a5,a5,s0
    802046ae:	ee07bc23          	sd	zero,-264(a5)
  sp -= (argc+1) * sizeof(uint64);
    802046b2:	00148693          	addi	a3,s1,1
    802046b6:	068e                	slli	a3,a3,0x3
    802046b8:	40d90933          	sub	s2,s2,a3
  sp -= sp % 16;
    802046bc:	ff097913          	andi	s2,s2,-16
  if(sp < stackbase)
    802046c0:	01797663          	bgeu	s2,s7,802046cc <exec+0x25e>
  sz = sz1;
    802046c4:	df943823          	sd	s9,-528(s0)
  ep = 0;
    802046c8:	4a01                	li	s4,0
    802046ca:	a85d                	j	80204780 <exec+0x312>
  if(copyout(pagetable, sp, (char *)ustack, (argc+1)*sizeof(uint64)) < 0)
    802046cc:	e8840613          	addi	a2,s0,-376
    802046d0:	85ca                	mv	a1,s2
    802046d2:	855a                	mv	a0,s6
    802046d4:	ffffd097          	auipc	ra,0xffffd
    802046d8:	c9e080e7          	jalr	-866(ra) # 80201372 <copyout>
    802046dc:	12054d63          	bltz	a0,80204816 <exec+0x3a8>
  p->trapframe->a1 = sp;
    802046e0:	060ab783          	ld	a5,96(s5)
    802046e4:	0727bc23          	sd	s2,120(a5)
  for(last=s=path; *s; s++)
    802046e8:	de043783          	ld	a5,-544(s0)
    802046ec:	0007c703          	lbu	a4,0(a5)
    802046f0:	cf11                	beqz	a4,8020470c <exec+0x29e>
    802046f2:	0785                	addi	a5,a5,1
    if(*s == '/')
    802046f4:	02f00693          	li	a3,47
    802046f8:	a039                	j	80204706 <exec+0x298>
      last = s+1;
    802046fa:	def43023          	sd	a5,-544(s0)
  for(last=s=path; *s; s++)
    802046fe:	0785                	addi	a5,a5,1
    80204700:	fff7c703          	lbu	a4,-1(a5)
    80204704:	c701                	beqz	a4,8020470c <exec+0x29e>
    if(*s == '/')
    80204706:	fed71ce3          	bne	a4,a3,802046fe <exec+0x290>
    8020470a:	bfc5                	j	802046fa <exec+0x28c>
  safestrcpy(p->name, last, sizeof(p->name));
    8020470c:	4641                	li	a2,16
    8020470e:	de043583          	ld	a1,-544(s0)
    80204712:	450a8513          	addi	a0,s5,1104
    80204716:	ffffc097          	auipc	ra,0xffffc
    8020471a:	1cc080e7          	jalr	460(ra) # 802008e2 <safestrcpy>
  oldpagetable = p->pagetable;
    8020471e:	050ab503          	ld	a0,80(s5)
  oldkpagetable = p->kpagetable;
    80204722:	058ab983          	ld	s3,88(s5)
  p->pagetable = pagetable;
    80204726:	056ab823          	sd	s6,80(s5)
  p->kpagetable = kpagetable;
    8020472a:	e0843783          	ld	a5,-504(s0)
    8020472e:	04fabc23          	sd	a5,88(s5)
  p->sz = sz;
    80204732:	059ab423          	sd	s9,72(s5)
  p->trapframe->epc = elf.entry;  // initial program counter = main
    80204736:	060ab783          	ld	a5,96(s5)
    8020473a:	e6043703          	ld	a4,-416(s0)
    8020473e:	ef98                	sd	a4,24(a5)
  p->trapframe->sp = sp; // initial stack pointer
    80204740:	060ab783          	ld	a5,96(s5)
    80204744:	0327b823          	sd	s2,48(a5)
  proc_freepagetable(oldpagetable, oldsz);
    80204748:	85ea                	mv	a1,s10
    8020474a:	ffffd097          	auipc	ra,0xffffd
    8020474e:	50e080e7          	jalr	1294(ra) # 80201c58 <proc_freepagetable>
  w_satp(MAKE_SATP(p->kpagetable));
    80204752:	058ab783          	ld	a5,88(s5)
    80204756:	83b1                	srli	a5,a5,0xc
    80204758:	577d                	li	a4,-1
    8020475a:	177e                	slli	a4,a4,0x3f
    8020475c:	8fd9                	or	a5,a5,a4
  asm volatile("csrw satp, %0" : : "r" (x));
    8020475e:	18079073          	csrw	satp,a5
  asm volatile("sfence.vma");
    80204762:	12000073          	sfence.vma
  kvmfree(oldkpagetable, 0);
    80204766:	4581                	li	a1,0
    80204768:	854e                	mv	a0,s3
    8020476a:	ffffd097          	auipc	ra,0xffffd
    8020476e:	f94080e7          	jalr	-108(ra) # 802016fe <kvmfree>
  return argc; // this ends up in a0, the first argument to main(argc, argv)
    80204772:	0004851b          	sext.w	a0,s1
    80204776:	a889                	j	802047c8 <exec+0x35a>
    return -1;
    80204778:	557d                	li	a0,-1
    8020477a:	a0b9                	j	802047c8 <exec+0x35a>
    8020477c:	de943823          	sd	s1,-528(s0)
  printf("[exec] reach bad\n");
    80204780:	00005517          	auipc	a0,0x5
    80204784:	5f850513          	addi	a0,a0,1528 # 80209d78 <digits+0x9f8>
    80204788:	ffffc097          	auipc	ra,0xffffc
    8020478c:	a06080e7          	jalr	-1530(ra) # 8020018e <printf>
    proc_freepagetable(pagetable, sz);
    80204790:	df043583          	ld	a1,-528(s0)
    80204794:	855a                	mv	a0,s6
    80204796:	ffffd097          	auipc	ra,0xffffd
    8020479a:	4c2080e7          	jalr	1218(ra) # 80201c58 <proc_freepagetable>
  if(kpagetable)
    kvmfree(kpagetable, 0);
    8020479e:	4581                	li	a1,0
    802047a0:	e0843503          	ld	a0,-504(s0)
    802047a4:	ffffd097          	auipc	ra,0xffffd
    802047a8:	f5a080e7          	jalr	-166(ra) # 802016fe <kvmfree>
  if(ep){
    eunlock(ep);
    eput(ep);
  }
  return -1;
    802047ac:	557d                	li	a0,-1
  if(ep){
    802047ae:	000a0d63          	beqz	s4,802047c8 <exec+0x35a>
    eunlock(ep);
    802047b2:	8552                	mv	a0,s4
    802047b4:	00002097          	auipc	ra,0x2
    802047b8:	4e0080e7          	jalr	1248(ra) # 80206c94 <eunlock>
    eput(ep);
    802047bc:	8552                	mv	a0,s4
    802047be:	00002097          	auipc	ra,0x2
    802047c2:	524080e7          	jalr	1316(ra) # 80206ce2 <eput>
  return -1;
    802047c6:	557d                	li	a0,-1
}
    802047c8:	22813083          	ld	ra,552(sp)
    802047cc:	22013403          	ld	s0,544(sp)
    802047d0:	21813483          	ld	s1,536(sp)
    802047d4:	21013903          	ld	s2,528(sp)
    802047d8:	20813983          	ld	s3,520(sp)
    802047dc:	20013a03          	ld	s4,512(sp)
    802047e0:	7afe                	ld	s5,504(sp)
    802047e2:	7b5e                	ld	s6,496(sp)
    802047e4:	7bbe                	ld	s7,488(sp)
    802047e6:	7c1e                	ld	s8,480(sp)
    802047e8:	6cfe                	ld	s9,472(sp)
    802047ea:	6d5e                	ld	s10,464(sp)
    802047ec:	6dbe                	ld	s11,456(sp)
    802047ee:	23010113          	addi	sp,sp,560
    802047f2:	8082                	ret
    802047f4:	de943823          	sd	s1,-528(s0)
    802047f8:	b761                	j	80204780 <exec+0x312>
    802047fa:	de943823          	sd	s1,-528(s0)
    802047fe:	b749                	j	80204780 <exec+0x312>
    80204800:	de943823          	sd	s1,-528(s0)
    80204804:	bfb5                	j	80204780 <exec+0x312>
  sz = sz1;
    80204806:	df943823          	sd	s9,-528(s0)
  ep = 0;
    8020480a:	4a01                	li	s4,0
    8020480c:	bf95                	j	80204780 <exec+0x312>
  sz = sz1;
    8020480e:	df943823          	sd	s9,-528(s0)
  ep = 0;
    80204812:	4a01                	li	s4,0
    80204814:	b7b5                	j	80204780 <exec+0x312>
  sz = sz1;
    80204816:	df943823          	sd	s9,-528(s0)
  ep = 0;
    8020481a:	4a01                	li	s4,0
    8020481c:	b795                	j	80204780 <exec+0x312>
    if((sz1 = uvmalloc(pagetable, kpagetable, sz, ph.vaddr + ph.memsz)) == 0)
    8020481e:	df043483          	ld	s1,-528(s0)
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    80204822:	e0043783          	ld	a5,-512(s0)
    80204826:	0017869b          	addiw	a3,a5,1
    8020482a:	e0d43023          	sd	a3,-512(s0)
    8020482e:	df843783          	ld	a5,-520(s0)
    80204832:	0387879b          	addiw	a5,a5,56
    80204836:	e8045703          	lhu	a4,-384(s0)
    8020483a:	d8e6d9e3          	bge	a3,a4,802045cc <exec+0x15e>
    if(eread(ep, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph))
    8020483e:	2781                	sext.w	a5,a5
    80204840:	def43c23          	sd	a5,-520(s0)
    80204844:	03800713          	li	a4,56
    80204848:	86be                	mv	a3,a5
    8020484a:	de843603          	ld	a2,-536(s0)
    8020484e:	4581                	li	a1,0
    80204850:	8552                	mv	a0,s4
    80204852:	00002097          	auipc	ra,0x2
    80204856:	b08080e7          	jalr	-1272(ra) # 8020635a <eread>
    8020485a:	03800793          	li	a5,56
    8020485e:	f0f51fe3          	bne	a0,a5,8020477c <exec+0x30e>
    if(ph.type != ELF_PROG_LOAD)
    80204862:	e1042783          	lw	a5,-496(s0)
    80204866:	4705                	li	a4,1
    80204868:	fae79de3          	bne	a5,a4,80204822 <exec+0x3b4>
    if(ph.memsz < ph.filesz)
    8020486c:	e3843683          	ld	a3,-456(s0)
    80204870:	e3043783          	ld	a5,-464(s0)
    80204874:	f8f6e0e3          	bltu	a3,a5,802047f4 <exec+0x386>
    if(ph.vaddr + ph.memsz < ph.vaddr)
    80204878:	e2043783          	ld	a5,-480(s0)
    8020487c:	96be                	add	a3,a3,a5
    8020487e:	f6f6eee3          	bltu	a3,a5,802047fa <exec+0x38c>
    if((sz1 = uvmalloc(pagetable, kpagetable, sz, ph.vaddr + ph.memsz)) == 0)
    80204882:	8626                	mv	a2,s1
    80204884:	e0843583          	ld	a1,-504(s0)
    80204888:	855a                	mv	a0,s6
    8020488a:	ffffd097          	auipc	ra,0xffffd
    8020488e:	80c080e7          	jalr	-2036(ra) # 80201096 <uvmalloc>
    80204892:	dea43823          	sd	a0,-528(s0)
    80204896:	d52d                	beqz	a0,80204800 <exec+0x392>
    if(ph.vaddr % PGSIZE != 0)
    80204898:	e2043c03          	ld	s8,-480(s0)
    8020489c:	dd043783          	ld	a5,-560(s0)
    802048a0:	00fc77b3          	and	a5,s8,a5
    802048a4:	ec079ee3          	bnez	a5,80204780 <exec+0x312>
    if(loadseg(pagetable, ph.vaddr, ep, ph.off, ph.filesz) < 0)
    802048a8:	e1842c83          	lw	s9,-488(s0)
    802048ac:	e3042b83          	lw	s7,-464(s0)
  for(i = 0; i < sz; i += PGSIZE){
    802048b0:	f60b87e3          	beqz	s7,8020481e <exec+0x3b0>
    802048b4:	89de                	mv	s3,s7
    802048b6:	4481                	li	s1,0
    802048b8:	b9cd                	j	802045aa <exec+0x13c>

00000000802048ba <create>:
  return -1;
}

static struct dirent*
create(char *path, short type, int mode)
{
    802048ba:	7169                	addi	sp,sp,-304
    802048bc:	f606                	sd	ra,296(sp)
    802048be:	f222                	sd	s0,288(sp)
    802048c0:	ee26                	sd	s1,280(sp)
    802048c2:	ea4a                	sd	s2,272(sp)
    802048c4:	e64e                	sd	s3,264(sp)
    802048c6:	1a00                	addi	s0,sp,304
    802048c8:	892e                	mv	s2,a1
  struct dirent *ep, *dp;
  char name[FAT32_MAX_FILENAME + 1];

  if((dp = enameparent(path, name)) == NULL)
    802048ca:	ed040593          	addi	a1,s0,-304
    802048ce:	00003097          	auipc	ra,0x3
    802048d2:	b56080e7          	jalr	-1194(ra) # 80207424 <enameparent>
    802048d6:	84aa                	mv	s1,a0
    802048d8:	c945                	beqz	a0,80204988 <create+0xce>
    mode = ATTR_READ_ONLY;
  } else {
    mode = 0;  
  }

  elock(dp);
    802048da:	00002097          	auipc	ra,0x2
    802048de:	384080e7          	jalr	900(ra) # 80206c5e <elock>
  if (type == T_DIR) {
    802048e2:	fff90613          	addi	a2,s2,-1
    802048e6:	00163613          	seqz	a2,a2
  if ((ep = ealloc(dp, name, mode)) == NULL) {
    802048ea:	0046161b          	slliw	a2,a2,0x4
    802048ee:	ed040593          	addi	a1,s0,-304
    802048f2:	8526                	mv	a0,s1
    802048f4:	00003097          	auipc	ra,0x3
    802048f8:	856080e7          	jalr	-1962(ra) # 8020714a <ealloc>
    802048fc:	89aa                	mv	s3,a0
    802048fe:	c529                	beqz	a0,80204948 <create+0x8e>
    eunlock(dp);
    eput(dp);
    return NULL;
  }
  
  if ((type == T_DIR && !(ep->attribute & ATTR_DIRECTORY)) ||
    80204900:	0009079b          	sext.w	a5,s2
    80204904:	4705                	li	a4,1
    80204906:	04e78c63          	beq	a5,a4,8020495e <create+0xa4>
    8020490a:	2901                	sext.w	s2,s2
    8020490c:	4789                	li	a5,2
    8020490e:	00f91663          	bne	s2,a5,8020491a <create+0x60>
      (type == T_FILE && (ep->attribute & ATTR_DIRECTORY))) {
    80204912:	10054783          	lbu	a5,256(a0)
    80204916:	8bc1                	andi	a5,a5,16
    80204918:	e7b9                	bnez	a5,80204966 <create+0xac>
    eput(ep);
    eput(dp);
    return NULL;
  }

  eunlock(dp);
    8020491a:	8526                	mv	a0,s1
    8020491c:	00002097          	auipc	ra,0x2
    80204920:	378080e7          	jalr	888(ra) # 80206c94 <eunlock>
  eput(dp);
    80204924:	8526                	mv	a0,s1
    80204926:	00002097          	auipc	ra,0x2
    8020492a:	3bc080e7          	jalr	956(ra) # 80206ce2 <eput>

  elock(ep);
    8020492e:	854e                	mv	a0,s3
    80204930:	00002097          	auipc	ra,0x2
    80204934:	32e080e7          	jalr	814(ra) # 80206c5e <elock>
  return ep;
}
    80204938:	854e                	mv	a0,s3
    8020493a:	70b2                	ld	ra,296(sp)
    8020493c:	7412                	ld	s0,288(sp)
    8020493e:	64f2                	ld	s1,280(sp)
    80204940:	6952                	ld	s2,272(sp)
    80204942:	69b2                	ld	s3,264(sp)
    80204944:	6155                	addi	sp,sp,304
    80204946:	8082                	ret
    eunlock(dp);
    80204948:	8526                	mv	a0,s1
    8020494a:	00002097          	auipc	ra,0x2
    8020494e:	34a080e7          	jalr	842(ra) # 80206c94 <eunlock>
    eput(dp);
    80204952:	8526                	mv	a0,s1
    80204954:	00002097          	auipc	ra,0x2
    80204958:	38e080e7          	jalr	910(ra) # 80206ce2 <eput>
    return NULL;
    8020495c:	bff1                	j	80204938 <create+0x7e>
  if ((type == T_DIR && !(ep->attribute & ATTR_DIRECTORY)) ||
    8020495e:	10054783          	lbu	a5,256(a0)
    80204962:	8bc1                	andi	a5,a5,16
    80204964:	fbdd                	bnez	a5,8020491a <create+0x60>
    eunlock(dp);
    80204966:	8526                	mv	a0,s1
    80204968:	00002097          	auipc	ra,0x2
    8020496c:	32c080e7          	jalr	812(ra) # 80206c94 <eunlock>
    eput(ep);
    80204970:	854e                	mv	a0,s3
    80204972:	00002097          	auipc	ra,0x2
    80204976:	370080e7          	jalr	880(ra) # 80206ce2 <eput>
    eput(dp);
    8020497a:	8526                	mv	a0,s1
    8020497c:	00002097          	auipc	ra,0x2
    80204980:	366080e7          	jalr	870(ra) # 80206ce2 <eput>
    return NULL;
    80204984:	4981                	li	s3,0
    80204986:	bf4d                	j	80204938 <create+0x7e>
    return NULL;
    80204988:	89aa                	mv	s3,a0
    8020498a:	b77d                	j	80204938 <create+0x7e>

000000008020498c <isdirempty>:
}

// Is the directory dp empty except for "." and ".." ?
static int
isdirempty(struct dirent *dp)
{
    8020498c:	7109                	addi	sp,sp,-384
    8020498e:	fe86                	sd	ra,376(sp)
    80204990:	faa2                	sd	s0,368(sp)
    80204992:	0300                	addi	s0,sp,384
  struct dirent ep;
  int count;
  int ret;
  ep.valid = 0;
    80204994:	f8041f23          	sh	zero,-98(s0)
  ret = enext(dp, &ep, 2 * 32, &count);   // skip the "." and ".."
    80204998:	e8440693          	addi	a3,s0,-380
    8020499c:	04000613          	li	a2,64
    802049a0:	e8840593          	addi	a1,s0,-376
    802049a4:	00002097          	auipc	ra,0x2
    802049a8:	4be080e7          	jalr	1214(ra) # 80206e62 <enext>
  return ret == -1;
    802049ac:	0505                	addi	a0,a0,1
}
    802049ae:	00153513          	seqz	a0,a0
    802049b2:	70f6                	ld	ra,376(sp)
    802049b4:	7456                	ld	s0,368(sp)
    802049b6:	6119                	addi	sp,sp,384
    802049b8:	8082                	ret

00000000802049ba <argfd>:
{
    802049ba:	7179                	addi	sp,sp,-48
    802049bc:	f406                	sd	ra,40(sp)
    802049be:	f022                	sd	s0,32(sp)
    802049c0:	ec26                	sd	s1,24(sp)
    802049c2:	e84a                	sd	s2,16(sp)
    802049c4:	1800                	addi	s0,sp,48
    802049c6:	892e                	mv	s2,a1
    802049c8:	84b2                	mv	s1,a2
  if(argint(n, &fd) < 0)
    802049ca:	fdc40593          	addi	a1,s0,-36
    802049ce:	ffffe097          	auipc	ra,0xffffe
    802049d2:	728080e7          	jalr	1832(ra) # 802030f6 <argint>
    802049d6:	04054163          	bltz	a0,80204a18 <argfd+0x5e>
  if(fd < 0 || fd >= NOFILE || (f=myproc()->ofile[fd]) == NULL)
    802049da:	fdc42703          	lw	a4,-36(s0)
    802049de:	06d00793          	li	a5,109
    802049e2:	02e7ed63          	bltu	a5,a4,80204a1c <argfd+0x62>
    802049e6:	ffffd097          	auipc	ra,0xffffd
    802049ea:	0f2080e7          	jalr	242(ra) # 80201ad8 <myproc>
    802049ee:	fdc42703          	lw	a4,-36(s0)
    802049f2:	01a70793          	addi	a5,a4,26 # fffffffffffff01a <ebss_clear+0xffffffff7fda701a>
    802049f6:	078e                	slli	a5,a5,0x3
    802049f8:	953e                	add	a0,a0,a5
    802049fa:	651c                	ld	a5,8(a0)
    802049fc:	c395                	beqz	a5,80204a20 <argfd+0x66>
  if(pfd)
    802049fe:	00090463          	beqz	s2,80204a06 <argfd+0x4c>
    *pfd = fd;
    80204a02:	00e92023          	sw	a4,0(s2)
  return 0;
    80204a06:	4501                	li	a0,0
  if(pf)
    80204a08:	c091                	beqz	s1,80204a0c <argfd+0x52>
    *pf = f;
    80204a0a:	e09c                	sd	a5,0(s1)
}
    80204a0c:	70a2                	ld	ra,40(sp)
    80204a0e:	7402                	ld	s0,32(sp)
    80204a10:	64e2                	ld	s1,24(sp)
    80204a12:	6942                	ld	s2,16(sp)
    80204a14:	6145                	addi	sp,sp,48
    80204a16:	8082                	ret
    return -1;
    80204a18:	557d                	li	a0,-1
    80204a1a:	bfcd                	j	80204a0c <argfd+0x52>
    return -1;
    80204a1c:	557d                	li	a0,-1
    80204a1e:	b7fd                	j	80204a0c <argfd+0x52>
    80204a20:	557d                	li	a0,-1
    80204a22:	b7ed                	j	80204a0c <argfd+0x52>

0000000080204a24 <fdalloc>:
{
    80204a24:	1101                	addi	sp,sp,-32
    80204a26:	ec06                	sd	ra,24(sp)
    80204a28:	e822                	sd	s0,16(sp)
    80204a2a:	e426                	sd	s1,8(sp)
    80204a2c:	1000                	addi	s0,sp,32
    80204a2e:	84aa                	mv	s1,a0
  struct proc *p = myproc();
    80204a30:	ffffd097          	auipc	ra,0xffffd
    80204a34:	0a8080e7          	jalr	168(ra) # 80201ad8 <myproc>
    80204a38:	862a                	mv	a2,a0
  for(fd = 0; fd < NOFILE; fd++){
    80204a3a:	0d850793          	addi	a5,a0,216
    80204a3e:	4501                	li	a0,0
    80204a40:	06e00693          	li	a3,110
    if(p->ofile[fd] == 0){
    80204a44:	6398                	ld	a4,0(a5)
    80204a46:	c719                	beqz	a4,80204a54 <fdalloc+0x30>
  for(fd = 0; fd < NOFILE; fd++){
    80204a48:	2505                	addiw	a0,a0,1
    80204a4a:	07a1                	addi	a5,a5,8
    80204a4c:	fed51ce3          	bne	a0,a3,80204a44 <fdalloc+0x20>
  return -1;
    80204a50:	557d                	li	a0,-1
    80204a52:	a031                	j	80204a5e <fdalloc+0x3a>
      p->ofile[fd] = f;
    80204a54:	01a50793          	addi	a5,a0,26
    80204a58:	078e                	slli	a5,a5,0x3
    80204a5a:	963e                	add	a2,a2,a5
    80204a5c:	e604                	sd	s1,8(a2)
}
    80204a5e:	60e2                	ld	ra,24(sp)
    80204a60:	6442                	ld	s0,16(sp)
    80204a62:	64a2                	ld	s1,8(sp)
    80204a64:	6105                	addi	sp,sp,32
    80204a66:	8082                	ret

0000000080204a68 <sys_dup>:
{
    80204a68:	7179                	addi	sp,sp,-48
    80204a6a:	f406                	sd	ra,40(sp)
    80204a6c:	f022                	sd	s0,32(sp)
    80204a6e:	ec26                	sd	s1,24(sp)
    80204a70:	e84a                	sd	s2,16(sp)
    80204a72:	1800                	addi	s0,sp,48
  if(argfd(0, 0, &f) < 0)
    80204a74:	fd840613          	addi	a2,s0,-40
    80204a78:	4581                	li	a1,0
    80204a7a:	4501                	li	a0,0
    80204a7c:	00000097          	auipc	ra,0x0
    80204a80:	f3e080e7          	jalr	-194(ra) # 802049ba <argfd>
    return -1;
    80204a84:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0)
    80204a86:	02054363          	bltz	a0,80204aac <sys_dup+0x44>
  if((fd=fdalloc(f)) < 0)
    80204a8a:	fd843903          	ld	s2,-40(s0)
    80204a8e:	854a                	mv	a0,s2
    80204a90:	00000097          	auipc	ra,0x0
    80204a94:	f94080e7          	jalr	-108(ra) # 80204a24 <fdalloc>
    80204a98:	84aa                	mv	s1,a0
    return -1;
    80204a9a:	57fd                	li	a5,-1
  if((fd=fdalloc(f)) < 0)
    80204a9c:	00054863          	bltz	a0,80204aac <sys_dup+0x44>
  filedup(f);
    80204aa0:	854a                	mv	a0,s2
    80204aa2:	fffff097          	auipc	ra,0xfffff
    80204aa6:	2d4080e7          	jalr	724(ra) # 80203d76 <filedup>
  return fd;
    80204aaa:	87a6                	mv	a5,s1
}
    80204aac:	853e                	mv	a0,a5
    80204aae:	70a2                	ld	ra,40(sp)
    80204ab0:	7402                	ld	s0,32(sp)
    80204ab2:	64e2                	ld	s1,24(sp)
    80204ab4:	6942                	ld	s2,16(sp)
    80204ab6:	6145                	addi	sp,sp,48
    80204ab8:	8082                	ret

0000000080204aba <sys_read>:
{
    80204aba:	7179                	addi	sp,sp,-48
    80204abc:	f406                	sd	ra,40(sp)
    80204abe:	f022                	sd	s0,32(sp)
    80204ac0:	1800                	addi	s0,sp,48
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204ac2:	fe840613          	addi	a2,s0,-24
    80204ac6:	4581                	li	a1,0
    80204ac8:	4501                	li	a0,0
    80204aca:	00000097          	auipc	ra,0x0
    80204ace:	ef0080e7          	jalr	-272(ra) # 802049ba <argfd>
    return -1;
    80204ad2:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204ad4:	04054163          	bltz	a0,80204b16 <sys_read+0x5c>
    80204ad8:	fe440593          	addi	a1,s0,-28
    80204adc:	4509                	li	a0,2
    80204ade:	ffffe097          	auipc	ra,0xffffe
    80204ae2:	618080e7          	jalr	1560(ra) # 802030f6 <argint>
    return -1;
    80204ae6:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204ae8:	02054763          	bltz	a0,80204b16 <sys_read+0x5c>
    80204aec:	fd840593          	addi	a1,s0,-40
    80204af0:	4505                	li	a0,1
    80204af2:	ffffe097          	auipc	ra,0xffffe
    80204af6:	666080e7          	jalr	1638(ra) # 80203158 <argaddr>
    return -1;
    80204afa:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204afc:	00054d63          	bltz	a0,80204b16 <sys_read+0x5c>
  return fileread(f, p, n);
    80204b00:	fe442603          	lw	a2,-28(s0)
    80204b04:	fd843583          	ld	a1,-40(s0)
    80204b08:	fe843503          	ld	a0,-24(s0)
    80204b0c:	fffff097          	auipc	ra,0xfffff
    80204b10:	3d6080e7          	jalr	982(ra) # 80203ee2 <fileread>
    80204b14:	87aa                	mv	a5,a0
}
    80204b16:	853e                	mv	a0,a5
    80204b18:	70a2                	ld	ra,40(sp)
    80204b1a:	7402                	ld	s0,32(sp)
    80204b1c:	6145                	addi	sp,sp,48
    80204b1e:	8082                	ret

0000000080204b20 <sys_write>:
{
    80204b20:	7179                	addi	sp,sp,-48
    80204b22:	f406                	sd	ra,40(sp)
    80204b24:	f022                	sd	s0,32(sp)
    80204b26:	1800                	addi	s0,sp,48
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204b28:	fe840613          	addi	a2,s0,-24
    80204b2c:	4581                	li	a1,0
    80204b2e:	4501                	li	a0,0
    80204b30:	00000097          	auipc	ra,0x0
    80204b34:	e8a080e7          	jalr	-374(ra) # 802049ba <argfd>
    return -1;
    80204b38:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204b3a:	04054163          	bltz	a0,80204b7c <sys_write+0x5c>
    80204b3e:	fe440593          	addi	a1,s0,-28
    80204b42:	4509                	li	a0,2
    80204b44:	ffffe097          	auipc	ra,0xffffe
    80204b48:	5b2080e7          	jalr	1458(ra) # 802030f6 <argint>
    return -1;
    80204b4c:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204b4e:	02054763          	bltz	a0,80204b7c <sys_write+0x5c>
    80204b52:	fd840593          	addi	a1,s0,-40
    80204b56:	4505                	li	a0,1
    80204b58:	ffffe097          	auipc	ra,0xffffe
    80204b5c:	600080e7          	jalr	1536(ra) # 80203158 <argaddr>
    return -1;
    80204b60:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argaddr(1, &p) < 0)
    80204b62:	00054d63          	bltz	a0,80204b7c <sys_write+0x5c>
  return filewrite(f, p, n);
    80204b66:	fe442603          	lw	a2,-28(s0)
    80204b6a:	fd843583          	ld	a1,-40(s0)
    80204b6e:	fe843503          	ld	a0,-24(s0)
    80204b72:	fffff097          	auipc	ra,0xfffff
    80204b76:	432080e7          	jalr	1074(ra) # 80203fa4 <filewrite>
    80204b7a:	87aa                	mv	a5,a0
}
    80204b7c:	853e                	mv	a0,a5
    80204b7e:	70a2                	ld	ra,40(sp)
    80204b80:	7402                	ld	s0,32(sp)
    80204b82:	6145                	addi	sp,sp,48
    80204b84:	8082                	ret

0000000080204b86 <sys_close>:
{
    80204b86:	1101                	addi	sp,sp,-32
    80204b88:	ec06                	sd	ra,24(sp)
    80204b8a:	e822                	sd	s0,16(sp)
    80204b8c:	1000                	addi	s0,sp,32
  if(argfd(0, &fd, &f) < 0)
    80204b8e:	fe040613          	addi	a2,s0,-32
    80204b92:	fec40593          	addi	a1,s0,-20
    80204b96:	4501                	li	a0,0
    80204b98:	00000097          	auipc	ra,0x0
    80204b9c:	e22080e7          	jalr	-478(ra) # 802049ba <argfd>
    return -1;
    80204ba0:	57fd                	li	a5,-1
  if(argfd(0, &fd, &f) < 0)
    80204ba2:	02054463          	bltz	a0,80204bca <sys_close+0x44>
  myproc()->ofile[fd] = 0;
    80204ba6:	ffffd097          	auipc	ra,0xffffd
    80204baa:	f32080e7          	jalr	-206(ra) # 80201ad8 <myproc>
    80204bae:	fec42783          	lw	a5,-20(s0)
    80204bb2:	07e9                	addi	a5,a5,26
    80204bb4:	078e                	slli	a5,a5,0x3
    80204bb6:	953e                	add	a0,a0,a5
    80204bb8:	00053423          	sd	zero,8(a0)
  fileclose(f);
    80204bbc:	fe043503          	ld	a0,-32(s0)
    80204bc0:	fffff097          	auipc	ra,0xfffff
    80204bc4:	208080e7          	jalr	520(ra) # 80203dc8 <fileclose>
  return 0;
    80204bc8:	4781                	li	a5,0
}
    80204bca:	853e                	mv	a0,a5
    80204bcc:	60e2                	ld	ra,24(sp)
    80204bce:	6442                	ld	s0,16(sp)
    80204bd0:	6105                	addi	sp,sp,32
    80204bd2:	8082                	ret

0000000080204bd4 <sys_fstat>:
{
    80204bd4:	7151                	addi	sp,sp,-240
    80204bd6:	f586                	sd	ra,232(sp)
    80204bd8:	f1a2                	sd	s0,224(sp)
    80204bda:	eda6                	sd	s1,216(sp)
    80204bdc:	1980                	addi	s0,sp,240
  struct kstat kst = {
    80204bde:	08000613          	li	a2,128
    80204be2:	4581                	li	a1,0
    80204be4:	f1840513          	addi	a0,s0,-232
    80204be8:	ffffc097          	auipc	ra,0xffffc
    80204bec:	ba8080e7          	jalr	-1112(ra) # 80200790 <memset>
  if(argfd(0, 0, &f) < 0 || argaddr(1, &addr) < 0)
    80204bf0:	fd840613          	addi	a2,s0,-40
    80204bf4:	4581                	li	a1,0
    80204bf6:	4501                	li	a0,0
    80204bf8:	00000097          	auipc	ra,0x0
    80204bfc:	dc2080e7          	jalr	-574(ra) # 802049ba <argfd>
    80204c00:	87aa                	mv	a5,a0
    return -1;
    80204c02:	557d                	li	a0,-1
  if(argfd(0, 0, &f) < 0 || argaddr(1, &addr) < 0)
    80204c04:	0207c263          	bltz	a5,80204c28 <sys_fstat+0x54>
    80204c08:	fd040593          	addi	a1,s0,-48
    80204c0c:	4505                	li	a0,1
    80204c0e:	ffffe097          	auipc	ra,0xffffe
    80204c12:	54a080e7          	jalr	1354(ra) # 80203158 <argaddr>
    80204c16:	08054263          	bltz	a0,80204c9a <sys_fstat+0xc6>
  if(f->type == FD_ENTRY){
    80204c1a:	fd843483          	ld	s1,-40(s0)
    80204c1e:	4098                	lw	a4,0(s1)
    80204c20:	4789                	li	a5,2
  return -1;
    80204c22:	557d                	li	a0,-1
  if(f->type == FD_ENTRY){
    80204c24:	00f70763          	beq	a4,a5,80204c32 <sys_fstat+0x5e>
}
    80204c28:	70ae                	ld	ra,232(sp)
    80204c2a:	740e                	ld	s0,224(sp)
    80204c2c:	64ee                	ld	s1,216(sp)
    80204c2e:	616d                	addi	sp,sp,240
    80204c30:	8082                	ret
    elock(f->ep);
    80204c32:	6c88                	ld	a0,24(s1)
    80204c34:	00002097          	auipc	ra,0x2
    80204c38:	02a080e7          	jalr	42(ra) # 80206c5e <elock>
    estat(f->ep, &st);
    80204c3c:	f9840593          	addi	a1,s0,-104
    80204c40:	6c88                	ld	a0,24(s1)
    80204c42:	00002097          	auipc	ra,0x2
    80204c46:	1d8080e7          	jalr	472(ra) # 80206e1a <estat>
    kst.st_blocks = f->ep->clus_cnt;
    80204c4a:	6c9c                	ld	a5,24(s1)
    80204c4c:	1107e783          	lwu	a5,272(a5)
    80204c50:	f4f43c23          	sd	a5,-168(s0)
    eunlock(f->ep);
    80204c54:	6c88                	ld	a0,24(s1)
    80204c56:	00002097          	auipc	ra,0x2
    80204c5a:	03e080e7          	jalr	62(ra) # 80206c94 <eunlock>
    kst.st_dev = st.dev;
    80204c5e:	fbc42783          	lw	a5,-68(s0)
    80204c62:	f0f43c23          	sd	a5,-232(s0)
    kst.st_size = st.size;
    80204c66:	fc843783          	ld	a5,-56(s0)
    80204c6a:	f4f43423          	sd	a5,-184(s0)
    kst.st_nlink = f->ref;
    80204c6e:	40dc                	lw	a5,4(s1)
    80204c70:	f2f42623          	sw	a5,-212(s0)
    kst.st_mode = f->readable | f->writable;
    80204c74:	0084c783          	lbu	a5,8(s1)
    80204c78:	0094c703          	lbu	a4,9(s1)
    80204c7c:	8fd9                	or	a5,a5,a4
    80204c7e:	f2f42423          	sw	a5,-216(s0)
    if(copyout2(addr, (char *)&kst, sizeof(kst)) < 0)
    80204c82:	08000613          	li	a2,128
    80204c86:	f1840593          	addi	a1,s0,-232
    80204c8a:	fd043503          	ld	a0,-48(s0)
    80204c8e:	ffffc097          	auipc	ra,0xffffc
    80204c92:	770080e7          	jalr	1904(ra) # 802013fe <copyout2>
    80204c96:	957d                	srai	a0,a0,0x3f
    80204c98:	bf41                	j	80204c28 <sys_fstat+0x54>
    return -1;
    80204c9a:	557d                	li	a0,-1
    80204c9c:	b771                	j	80204c28 <sys_fstat+0x54>

0000000080204c9e <sys_open>:
{
    80204c9e:	7169                	addi	sp,sp,-304
    80204ca0:	f606                	sd	ra,296(sp)
    80204ca2:	f222                	sd	s0,288(sp)
    80204ca4:	ee26                	sd	s1,280(sp)
    80204ca6:	ea4a                	sd	s2,272(sp)
    80204ca8:	1a00                	addi	s0,sp,304
  if(argint(0, &fd), argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0){
    80204caa:	ed440593          	addi	a1,s0,-300
    80204cae:	4501                	li	a0,0
    80204cb0:	ffffe097          	auipc	ra,0xffffe
    80204cb4:	446080e7          	jalr	1094(ra) # 802030f6 <argint>
    80204cb8:	10400613          	li	a2,260
    80204cbc:	ed840593          	addi	a1,s0,-296
    80204cc0:	4505                	li	a0,1
    80204cc2:	ffffe097          	auipc	ra,0xffffe
    80204cc6:	4b8080e7          	jalr	1208(ra) # 8020317a <argstr>
    return -1;
    80204cca:	57fd                	li	a5,-1
  if(argint(0, &fd), argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0){
    80204ccc:	0a054063          	bltz	a0,80204d6c <sys_open+0xce>
    80204cd0:	ed040593          	addi	a1,s0,-304
    80204cd4:	4509                	li	a0,2
    80204cd6:	ffffe097          	auipc	ra,0xffffe
    80204cda:	420080e7          	jalr	1056(ra) # 802030f6 <argint>
    80204cde:	0e054763          	bltz	a0,80204dcc <sys_open+0x12e>
  if(omode & O_CREATE){ // 修改O_CREATE的值和测试用例一致
    80204ce2:	ed042603          	lw	a2,-304(s0)
    80204ce6:	04067793          	andi	a5,a2,64
    80204cea:	cbc1                	beqz	a5,80204d7a <sys_open+0xdc>
    ep = create(path, T_FILE, omode);
    80204cec:	4589                	li	a1,2
    80204cee:	ed840513          	addi	a0,s0,-296
    80204cf2:	00000097          	auipc	ra,0x0
    80204cf6:	bc8080e7          	jalr	-1080(ra) # 802048ba <create>
    80204cfa:	84aa                	mv	s1,a0
    if(ep == NULL){
    80204cfc:	c971                	beqz	a0,80204dd0 <sys_open+0x132>
  if((f = filealloc()) == NULL || (fd = fdalloc(f)) < 0){
    80204cfe:	fffff097          	auipc	ra,0xfffff
    80204d02:	00e080e7          	jalr	14(ra) # 80203d0c <filealloc>
    80204d06:	892a                	mv	s2,a0
    80204d08:	cd69                	beqz	a0,80204de2 <sys_open+0x144>
    80204d0a:	00000097          	auipc	ra,0x0
    80204d0e:	d1a080e7          	jalr	-742(ra) # 80204a24 <fdalloc>
    80204d12:	eca42a23          	sw	a0,-300(s0)
    80204d16:	0c054163          	bltz	a0,80204dd8 <sys_open+0x13a>
  if(!(ep->attribute & ATTR_DIRECTORY) && (omode & O_TRUNC)){
    80204d1a:	1004c783          	lbu	a5,256(s1)
    80204d1e:	8bc1                	andi	a5,a5,16
    80204d20:	e791                	bnez	a5,80204d2c <sys_open+0x8e>
    80204d22:	ed042783          	lw	a5,-304(s0)
    80204d26:	4007f793          	andi	a5,a5,1024
    80204d2a:	ebd9                	bnez	a5,80204dc0 <sys_open+0x122>
  f->type = FD_ENTRY;
    80204d2c:	4789                	li	a5,2
    80204d2e:	00f92023          	sw	a5,0(s2)
  f->off = (omode & O_APPEND) ? ep->file_size : 0;
    80204d32:	ed042783          	lw	a5,-304(s0)
    80204d36:	0047f693          	andi	a3,a5,4
    80204d3a:	4701                	li	a4,0
    80204d3c:	c299                	beqz	a3,80204d42 <sys_open+0xa4>
    80204d3e:	1084a703          	lw	a4,264(s1)
    80204d42:	02e92023          	sw	a4,32(s2)
  f->ep = ep;
    80204d46:	00993c23          	sd	s1,24(s2)
  f->readable = !(omode & O_WRONLY);
    80204d4a:	0017c713          	xori	a4,a5,1
    80204d4e:	8b05                	andi	a4,a4,1
    80204d50:	00e90423          	sb	a4,8(s2)
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
    80204d54:	8b8d                	andi	a5,a5,3
    80204d56:	00f037b3          	snez	a5,a5
    80204d5a:	00f904a3          	sb	a5,9(s2)
  eunlock(ep);
    80204d5e:	8526                	mv	a0,s1
    80204d60:	00002097          	auipc	ra,0x2
    80204d64:	f34080e7          	jalr	-204(ra) # 80206c94 <eunlock>
  return fd;
    80204d68:	ed442783          	lw	a5,-300(s0)
}
    80204d6c:	853e                	mv	a0,a5
    80204d6e:	70b2                	ld	ra,296(sp)
    80204d70:	7412                	ld	s0,288(sp)
    80204d72:	64f2                	ld	s1,280(sp)
    80204d74:	6952                	ld	s2,272(sp)
    80204d76:	6155                	addi	sp,sp,304
    80204d78:	8082                	ret
    if((ep = ename(path)) == NULL){
    80204d7a:	ed840513          	addi	a0,s0,-296
    80204d7e:	00002097          	auipc	ra,0x2
    80204d82:	688080e7          	jalr	1672(ra) # 80207406 <ename>
    80204d86:	84aa                	mv	s1,a0
    80204d88:	c531                	beqz	a0,80204dd4 <sys_open+0x136>
    elock(ep);
    80204d8a:	00002097          	auipc	ra,0x2
    80204d8e:	ed4080e7          	jalr	-300(ra) # 80206c5e <elock>
    if((ep->attribute & ATTR_DIRECTORY) && omode != O_RDONLY && omode != O_DIRECTORY){
    80204d92:	1004c783          	lbu	a5,256(s1)
    80204d96:	8bc1                	andi	a5,a5,16
    80204d98:	d3bd                	beqz	a5,80204cfe <sys_open+0x60>
    80204d9a:	ed042783          	lw	a5,-304(s0)
    80204d9e:	ffe00737          	lui	a4,0xffe00
    80204da2:	177d                	addi	a4,a4,-1 # ffffffffffdfffff <ebss_clear+0xffffffff7fba7fff>
    80204da4:	8ff9                	and	a5,a5,a4
    80204da6:	dfa1                	beqz	a5,80204cfe <sys_open+0x60>
      eunlock(ep);
    80204da8:	8526                	mv	a0,s1
    80204daa:	00002097          	auipc	ra,0x2
    80204dae:	eea080e7          	jalr	-278(ra) # 80206c94 <eunlock>
      eput(ep);
    80204db2:	8526                	mv	a0,s1
    80204db4:	00002097          	auipc	ra,0x2
    80204db8:	f2e080e7          	jalr	-210(ra) # 80206ce2 <eput>
      return -1;
    80204dbc:	57fd                	li	a5,-1
    80204dbe:	b77d                	j	80204d6c <sys_open+0xce>
    etrunc(ep);
    80204dc0:	8526                	mv	a0,s1
    80204dc2:	00002097          	auipc	ra,0x2
    80204dc6:	e32080e7          	jalr	-462(ra) # 80206bf4 <etrunc>
    80204dca:	b78d                	j	80204d2c <sys_open+0x8e>
    return -1;
    80204dcc:	57fd                	li	a5,-1
    80204dce:	bf79                	j	80204d6c <sys_open+0xce>
      return -1;
    80204dd0:	57fd                	li	a5,-1
    80204dd2:	bf69                	j	80204d6c <sys_open+0xce>
      return -1;
    80204dd4:	57fd                	li	a5,-1
    80204dd6:	bf59                	j	80204d6c <sys_open+0xce>
      fileclose(f);
    80204dd8:	854a                	mv	a0,s2
    80204dda:	fffff097          	auipc	ra,0xfffff
    80204dde:	fee080e7          	jalr	-18(ra) # 80203dc8 <fileclose>
    eunlock(ep);
    80204de2:	8526                	mv	a0,s1
    80204de4:	00002097          	auipc	ra,0x2
    80204de8:	eb0080e7          	jalr	-336(ra) # 80206c94 <eunlock>
    eput(ep);
    80204dec:	8526                	mv	a0,s1
    80204dee:	00002097          	auipc	ra,0x2
    80204df2:	ef4080e7          	jalr	-268(ra) # 80206ce2 <eput>
    return -1;
    80204df6:	57fd                	li	a5,-1
    80204df8:	bf95                	j	80204d6c <sys_open+0xce>

0000000080204dfa <sys_mkdir>:
{
    80204dfa:	7169                	addi	sp,sp,-304
    80204dfc:	f606                	sd	ra,296(sp)
    80204dfe:	f222                	sd	s0,288(sp)
    80204e00:	ee26                	sd	s1,280(sp)
    80204e02:	1a00                	addi	s0,sp,304
  if(argint(0, &dirfd) < 0 || argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0
    80204e04:	fdc40593          	addi	a1,s0,-36
    80204e08:	4501                	li	a0,0
    80204e0a:	ffffe097          	auipc	ra,0xffffe
    80204e0e:	2ec080e7          	jalr	748(ra) # 802030f6 <argint>
    return -1;
    80204e12:	57fd                	li	a5,-1
  if(argint(0, &dirfd) < 0 || argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0
    80204e14:	04054d63          	bltz	a0,80204e6e <sys_mkdir+0x74>
    80204e18:	10400613          	li	a2,260
    80204e1c:	ed040593          	addi	a1,s0,-304
    80204e20:	4505                	li	a0,1
    80204e22:	ffffe097          	auipc	ra,0xffffe
    80204e26:	358080e7          	jalr	856(ra) # 8020317a <argstr>
    return -1;
    80204e2a:	57fd                	li	a5,-1
  if(argint(0, &dirfd) < 0 || argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0
    80204e2c:	04054163          	bltz	a0,80204e6e <sys_mkdir+0x74>
    80204e30:	fd840593          	addi	a1,s0,-40
    80204e34:	4509                	li	a0,2
    80204e36:	ffffe097          	auipc	ra,0xffffe
    80204e3a:	2c0080e7          	jalr	704(ra) # 802030f6 <argint>
    return -1;
    80204e3e:	57fd                	li	a5,-1
  if(argint(0, &dirfd) < 0 || argstr(1, path, FAT32_MAX_PATH) < 0 || argint(2, &omode) < 0
    80204e40:	02054763          	bltz	a0,80204e6e <sys_mkdir+0x74>
      || (ep = create(path, T_DIR, omode)) == 0){
    80204e44:	fd842603          	lw	a2,-40(s0)
    80204e48:	4585                	li	a1,1
    80204e4a:	ed040513          	addi	a0,s0,-304
    80204e4e:	00000097          	auipc	ra,0x0
    80204e52:	a6c080e7          	jalr	-1428(ra) # 802048ba <create>
    80204e56:	84aa                	mv	s1,a0
    80204e58:	c10d                	beqz	a0,80204e7a <sys_mkdir+0x80>
  eunlock(ep);
    80204e5a:	00002097          	auipc	ra,0x2
    80204e5e:	e3a080e7          	jalr	-454(ra) # 80206c94 <eunlock>
  eput(ep);
    80204e62:	8526                	mv	a0,s1
    80204e64:	00002097          	auipc	ra,0x2
    80204e68:	e7e080e7          	jalr	-386(ra) # 80206ce2 <eput>
  return 0;
    80204e6c:	4781                	li	a5,0
}
    80204e6e:	853e                	mv	a0,a5
    80204e70:	70b2                	ld	ra,296(sp)
    80204e72:	7412                	ld	s0,288(sp)
    80204e74:	64f2                	ld	s1,280(sp)
    80204e76:	6155                	addi	sp,sp,304
    80204e78:	8082                	ret
    return -1;
    80204e7a:	57fd                	li	a5,-1
    80204e7c:	bfcd                	j	80204e6e <sys_mkdir+0x74>

0000000080204e7e <sys_chdir>:
{
    80204e7e:	7169                	addi	sp,sp,-304
    80204e80:	f606                	sd	ra,296(sp)
    80204e82:	f222                	sd	s0,288(sp)
    80204e84:	ee26                	sd	s1,280(sp)
    80204e86:	ea4a                	sd	s2,272(sp)
    80204e88:	1a00                	addi	s0,sp,304
  struct proc *p = myproc();
    80204e8a:	ffffd097          	auipc	ra,0xffffd
    80204e8e:	c4e080e7          	jalr	-946(ra) # 80201ad8 <myproc>
    80204e92:	892a                	mv	s2,a0
  if(argstr(0, path, FAT32_MAX_PATH) < 0 || (ep = ename(path)) == NULL){
    80204e94:	10400613          	li	a2,260
    80204e98:	ed840593          	addi	a1,s0,-296
    80204e9c:	4501                	li	a0,0
    80204e9e:	ffffe097          	auipc	ra,0xffffe
    80204ea2:	2dc080e7          	jalr	732(ra) # 8020317a <argstr>
    return -1;
    80204ea6:	57fd                	li	a5,-1
  if(argstr(0, path, FAT32_MAX_PATH) < 0 || (ep = ename(path)) == NULL){
    80204ea8:	04054063          	bltz	a0,80204ee8 <sys_chdir+0x6a>
    80204eac:	ed840513          	addi	a0,s0,-296
    80204eb0:	00002097          	auipc	ra,0x2
    80204eb4:	556080e7          	jalr	1366(ra) # 80207406 <ename>
    80204eb8:	84aa                	mv	s1,a0
    80204eba:	c931                	beqz	a0,80204f0e <sys_chdir+0x90>
  elock(ep);
    80204ebc:	00002097          	auipc	ra,0x2
    80204ec0:	da2080e7          	jalr	-606(ra) # 80206c5e <elock>
  if(!(ep->attribute & ATTR_DIRECTORY)){
    80204ec4:	1004c783          	lbu	a5,256(s1)
    80204ec8:	8bc1                	andi	a5,a5,16
    80204eca:	c795                	beqz	a5,80204ef6 <sys_chdir+0x78>
  eunlock(ep);
    80204ecc:	8526                	mv	a0,s1
    80204ece:	00002097          	auipc	ra,0x2
    80204ed2:	dc6080e7          	jalr	-570(ra) # 80206c94 <eunlock>
  eput(p->cwd);
    80204ed6:	44893503          	ld	a0,1096(s2)
    80204eda:	00002097          	auipc	ra,0x2
    80204ede:	e08080e7          	jalr	-504(ra) # 80206ce2 <eput>
  p->cwd = ep;
    80204ee2:	44993423          	sd	s1,1096(s2)
  return 0;
    80204ee6:	4781                	li	a5,0
}
    80204ee8:	853e                	mv	a0,a5
    80204eea:	70b2                	ld	ra,296(sp)
    80204eec:	7412                	ld	s0,288(sp)
    80204eee:	64f2                	ld	s1,280(sp)
    80204ef0:	6952                	ld	s2,272(sp)
    80204ef2:	6155                	addi	sp,sp,304
    80204ef4:	8082                	ret
    eunlock(ep);
    80204ef6:	8526                	mv	a0,s1
    80204ef8:	00002097          	auipc	ra,0x2
    80204efc:	d9c080e7          	jalr	-612(ra) # 80206c94 <eunlock>
    eput(ep);
    80204f00:	8526                	mv	a0,s1
    80204f02:	00002097          	auipc	ra,0x2
    80204f06:	de0080e7          	jalr	-544(ra) # 80206ce2 <eput>
    return -1;
    80204f0a:	57fd                	li	a5,-1
    80204f0c:	bff1                	j	80204ee8 <sys_chdir+0x6a>
    return -1;
    80204f0e:	57fd                	li	a5,-1
    80204f10:	bfe1                	j	80204ee8 <sys_chdir+0x6a>

0000000080204f12 <sys_pipe>:
{
    80204f12:	7139                	addi	sp,sp,-64
    80204f14:	fc06                	sd	ra,56(sp)
    80204f16:	f822                	sd	s0,48(sp)
    80204f18:	f426                	sd	s1,40(sp)
    80204f1a:	0080                	addi	s0,sp,64
  struct proc *p = myproc();
    80204f1c:	ffffd097          	auipc	ra,0xffffd
    80204f20:	bbc080e7          	jalr	-1092(ra) # 80201ad8 <myproc>
    80204f24:	84aa                	mv	s1,a0
  if(argaddr(0, &fdarray) < 0)
    80204f26:	fd840593          	addi	a1,s0,-40
    80204f2a:	4501                	li	a0,0
    80204f2c:	ffffe097          	auipc	ra,0xffffe
    80204f30:	22c080e7          	jalr	556(ra) # 80203158 <argaddr>
    return -1;
    80204f34:	57fd                	li	a5,-1
  if(argaddr(0, &fdarray) < 0)
    80204f36:	0c054e63          	bltz	a0,80205012 <sys_pipe+0x100>
  if(pipealloc(&rf, &wf) < 0)
    80204f3a:	fc840593          	addi	a1,s0,-56
    80204f3e:	fd040513          	addi	a0,s0,-48
    80204f42:	fffff097          	auipc	ra,0xfffff
    80204f46:	1f8080e7          	jalr	504(ra) # 8020413a <pipealloc>
    return -1;
    80204f4a:	57fd                	li	a5,-1
  if(pipealloc(&rf, &wf) < 0)
    80204f4c:	0c054363          	bltz	a0,80205012 <sys_pipe+0x100>
  fd0 = -1;
    80204f50:	fcf42223          	sw	a5,-60(s0)
  if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
    80204f54:	fd043503          	ld	a0,-48(s0)
    80204f58:	00000097          	auipc	ra,0x0
    80204f5c:	acc080e7          	jalr	-1332(ra) # 80204a24 <fdalloc>
    80204f60:	fca42223          	sw	a0,-60(s0)
    80204f64:	08054a63          	bltz	a0,80204ff8 <sys_pipe+0xe6>
    80204f68:	fc843503          	ld	a0,-56(s0)
    80204f6c:	00000097          	auipc	ra,0x0
    80204f70:	ab8080e7          	jalr	-1352(ra) # 80204a24 <fdalloc>
    80204f74:	fca42023          	sw	a0,-64(s0)
    80204f78:	06054763          	bltz	a0,80204fe6 <sys_pipe+0xd4>
  if(copyout2(fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
    80204f7c:	4611                	li	a2,4
    80204f7e:	fc440593          	addi	a1,s0,-60
    80204f82:	fd843503          	ld	a0,-40(s0)
    80204f86:	ffffc097          	auipc	ra,0xffffc
    80204f8a:	478080e7          	jalr	1144(ra) # 802013fe <copyout2>
    80204f8e:	00054f63          	bltz	a0,80204fac <sys_pipe+0x9a>
     copyout2(fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0){
    80204f92:	4611                	li	a2,4
    80204f94:	fc040593          	addi	a1,s0,-64
    80204f98:	fd843503          	ld	a0,-40(s0)
    80204f9c:	0511                	addi	a0,a0,4
    80204f9e:	ffffc097          	auipc	ra,0xffffc
    80204fa2:	460080e7          	jalr	1120(ra) # 802013fe <copyout2>
  return 0;
    80204fa6:	4781                	li	a5,0
  if(copyout2(fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
    80204fa8:	06055563          	bgez	a0,80205012 <sys_pipe+0x100>
    p->ofile[fd0] = 0;
    80204fac:	fc442783          	lw	a5,-60(s0)
    80204fb0:	07e9                	addi	a5,a5,26
    80204fb2:	078e                	slli	a5,a5,0x3
    80204fb4:	97a6                	add	a5,a5,s1
    80204fb6:	0007b423          	sd	zero,8(a5)
    p->ofile[fd1] = 0;
    80204fba:	fc042783          	lw	a5,-64(s0)
    80204fbe:	07e9                	addi	a5,a5,26
    80204fc0:	078e                	slli	a5,a5,0x3
    80204fc2:	00f48533          	add	a0,s1,a5
    80204fc6:	00053423          	sd	zero,8(a0)
    fileclose(rf);
    80204fca:	fd043503          	ld	a0,-48(s0)
    80204fce:	fffff097          	auipc	ra,0xfffff
    80204fd2:	dfa080e7          	jalr	-518(ra) # 80203dc8 <fileclose>
    fileclose(wf);
    80204fd6:	fc843503          	ld	a0,-56(s0)
    80204fda:	fffff097          	auipc	ra,0xfffff
    80204fde:	dee080e7          	jalr	-530(ra) # 80203dc8 <fileclose>
    return -1;
    80204fe2:	57fd                	li	a5,-1
    80204fe4:	a03d                	j	80205012 <sys_pipe+0x100>
    if(fd0 >= 0)
    80204fe6:	fc442783          	lw	a5,-60(s0)
    80204fea:	0007c763          	bltz	a5,80204ff8 <sys_pipe+0xe6>
      p->ofile[fd0] = 0;
    80204fee:	07e9                	addi	a5,a5,26
    80204ff0:	078e                	slli	a5,a5,0x3
    80204ff2:	97a6                	add	a5,a5,s1
    80204ff4:	0007b423          	sd	zero,8(a5)
    fileclose(rf);
    80204ff8:	fd043503          	ld	a0,-48(s0)
    80204ffc:	fffff097          	auipc	ra,0xfffff
    80205000:	dcc080e7          	jalr	-564(ra) # 80203dc8 <fileclose>
    fileclose(wf);
    80205004:	fc843503          	ld	a0,-56(s0)
    80205008:	fffff097          	auipc	ra,0xfffff
    8020500c:	dc0080e7          	jalr	-576(ra) # 80203dc8 <fileclose>
    return -1;
    80205010:	57fd                	li	a5,-1
}
    80205012:	853e                	mv	a0,a5
    80205014:	70e2                	ld	ra,56(sp)
    80205016:	7442                	ld	s0,48(sp)
    80205018:	74a2                	ld	s1,40(sp)
    8020501a:	6121                	addi	sp,sp,64
    8020501c:	8082                	ret

000000008020501e <sys_dev>:
{
    8020501e:	7179                	addi	sp,sp,-48
    80205020:	f406                	sd	ra,40(sp)
    80205022:	f022                	sd	s0,32(sp)
    80205024:	ec26                	sd	s1,24(sp)
    80205026:	1800                	addi	s0,sp,48
  if(argint(0, &omode) < 0 || argint(1, &major) < 0 || argint(2, &minor) < 0){
    80205028:	fdc40593          	addi	a1,s0,-36
    8020502c:	4501                	li	a0,0
    8020502e:	ffffe097          	auipc	ra,0xffffe
    80205032:	0c8080e7          	jalr	200(ra) # 802030f6 <argint>
    80205036:	08054a63          	bltz	a0,802050ca <sys_dev+0xac>
    8020503a:	fd840593          	addi	a1,s0,-40
    8020503e:	4505                	li	a0,1
    80205040:	ffffe097          	auipc	ra,0xffffe
    80205044:	0b6080e7          	jalr	182(ra) # 802030f6 <argint>
    80205048:	08054763          	bltz	a0,802050d6 <sys_dev+0xb8>
    8020504c:	fd440593          	addi	a1,s0,-44
    80205050:	4509                	li	a0,2
    80205052:	ffffe097          	auipc	ra,0xffffe
    80205056:	0a4080e7          	jalr	164(ra) # 802030f6 <argint>
    8020505a:	08054063          	bltz	a0,802050da <sys_dev+0xbc>
  if(omode & O_CREATE){
    8020505e:	fdc42783          	lw	a5,-36(s0)
    80205062:	0407f793          	andi	a5,a5,64
    80205066:	ebb1                	bnez	a5,802050ba <sys_dev+0x9c>
  if(major < 0 || major >= NDEV)
    80205068:	fd842703          	lw	a4,-40(s0)
    8020506c:	47a5                	li	a5,9
    return -1;
    8020506e:	557d                	li	a0,-1
  if(major < 0 || major >= NDEV)
    80205070:	04e7ee63          	bltu	a5,a4,802050cc <sys_dev+0xae>
  if((f = filealloc()) == NULL || (fd = fdalloc(f)) < 0){
    80205074:	fffff097          	auipc	ra,0xfffff
    80205078:	c98080e7          	jalr	-872(ra) # 80203d0c <filealloc>
    8020507c:	84aa                	mv	s1,a0
    8020507e:	c125                	beqz	a0,802050de <sys_dev+0xc0>
    80205080:	00000097          	auipc	ra,0x0
    80205084:	9a4080e7          	jalr	-1628(ra) # 80204a24 <fdalloc>
    80205088:	04054d63          	bltz	a0,802050e2 <sys_dev+0xc4>
  f->type = FD_DEVICE;
    8020508c:	478d                	li	a5,3
    8020508e:	c09c                	sw	a5,0(s1)
  f->off = 0;
    80205090:	0204a023          	sw	zero,32(s1)
  f->ep = 0;
    80205094:	0004bc23          	sd	zero,24(s1)
  f->major = major;
    80205098:	fd842783          	lw	a5,-40(s0)
    8020509c:	02f49223          	sh	a5,36(s1)
  f->readable = !(omode & O_WRONLY);
    802050a0:	fdc42783          	lw	a5,-36(s0)
    802050a4:	0017c713          	xori	a4,a5,1
    802050a8:	8b05                	andi	a4,a4,1
    802050aa:	00e48423          	sb	a4,8(s1)
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
    802050ae:	8b8d                	andi	a5,a5,3
    802050b0:	00f037b3          	snez	a5,a5
    802050b4:	00f484a3          	sb	a5,9(s1)
  return fd;
    802050b8:	a811                	j	802050cc <sys_dev+0xae>
    panic("dev file on FAT");
    802050ba:	00005517          	auipc	a0,0x5
    802050be:	cd650513          	addi	a0,a0,-810 # 80209d90 <digits+0xa10>
    802050c2:	ffffb097          	auipc	ra,0xffffb
    802050c6:	082080e7          	jalr	130(ra) # 80200144 <panic>
    return -1;
    802050ca:	557d                	li	a0,-1
}
    802050cc:	70a2                	ld	ra,40(sp)
    802050ce:	7402                	ld	s0,32(sp)
    802050d0:	64e2                	ld	s1,24(sp)
    802050d2:	6145                	addi	sp,sp,48
    802050d4:	8082                	ret
    return -1;
    802050d6:	557d                	li	a0,-1
    802050d8:	bfd5                	j	802050cc <sys_dev+0xae>
    802050da:	557d                	li	a0,-1
    802050dc:	bfc5                	j	802050cc <sys_dev+0xae>
    return -1;
    802050de:	557d                	li	a0,-1
    802050e0:	b7f5                	j	802050cc <sys_dev+0xae>
      fileclose(f);
    802050e2:	8526                	mv	a0,s1
    802050e4:	fffff097          	auipc	ra,0xfffff
    802050e8:	ce4080e7          	jalr	-796(ra) # 80203dc8 <fileclose>
    return -1;
    802050ec:	557d                	li	a0,-1
    802050ee:	bff9                	j	802050cc <sys_dev+0xae>

00000000802050f0 <sys_readdir>:
{
    802050f0:	1101                	addi	sp,sp,-32
    802050f2:	ec06                	sd	ra,24(sp)
    802050f4:	e822                	sd	s0,16(sp)
    802050f6:	1000                	addi	s0,sp,32
  if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0)
    802050f8:	fe840613          	addi	a2,s0,-24
    802050fc:	4581                	li	a1,0
    802050fe:	4501                	li	a0,0
    80205100:	00000097          	auipc	ra,0x0
    80205104:	8ba080e7          	jalr	-1862(ra) # 802049ba <argfd>
    return -1;
    80205108:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0)
    8020510a:	02054563          	bltz	a0,80205134 <sys_readdir+0x44>
    8020510e:	fe040593          	addi	a1,s0,-32
    80205112:	4505                	li	a0,1
    80205114:	ffffe097          	auipc	ra,0xffffe
    80205118:	044080e7          	jalr	68(ra) # 80203158 <argaddr>
    return -1;
    8020511c:	57fd                	li	a5,-1
  if(argfd(0, 0, &f) < 0 || argaddr(1, &p) < 0)
    8020511e:	00054b63          	bltz	a0,80205134 <sys_readdir+0x44>
  return dirnext(f, p);
    80205122:	fe043583          	ld	a1,-32(s0)
    80205126:	fe843503          	ld	a0,-24(s0)
    8020512a:	fffff097          	auipc	ra,0xfffff
    8020512e:	f4a080e7          	jalr	-182(ra) # 80204074 <dirnext>
    80205132:	87aa                	mv	a5,a0
}
    80205134:	853e                	mv	a0,a5
    80205136:	60e2                	ld	ra,24(sp)
    80205138:	6442                	ld	s0,16(sp)
    8020513a:	6105                	addi	sp,sp,32
    8020513c:	8082                	ret

000000008020513e <sys_getcwd>:
{
    8020513e:	714d                	addi	sp,sp,-336
    80205140:	e686                	sd	ra,328(sp)
    80205142:	e2a2                	sd	s0,320(sp)
    80205144:	fe26                	sd	s1,312(sp)
    80205146:	fa4a                	sd	s2,304(sp)
    80205148:	f64e                	sd	s3,296(sp)
    8020514a:	f252                	sd	s4,288(sp)
    8020514c:	ee56                	sd	s5,280(sp)
    8020514e:	0a80                	addi	s0,sp,336
  if (argaddr(0, &addr) < 0 || argint(1, &size))
    80205150:	fb840593          	addi	a1,s0,-72
    80205154:	4501                	li	a0,0
    80205156:	ffffe097          	auipc	ra,0xffffe
    8020515a:	002080e7          	jalr	2(ra) # 80203158 <argaddr>
    return NULL;
    8020515e:	4481                	li	s1,0
  if (argaddr(0, &addr) < 0 || argint(1, &size))
    80205160:	00054a63          	bltz	a0,80205174 <sys_getcwd+0x36>
    80205164:	fb440593          	addi	a1,s0,-76
    80205168:	4505                	li	a0,1
    8020516a:	ffffe097          	auipc	ra,0xffffe
    8020516e:	f8c080e7          	jalr	-116(ra) # 802030f6 <argint>
    80205172:	c919                	beqz	a0,80205188 <sys_getcwd+0x4a>
}
    80205174:	8526                	mv	a0,s1
    80205176:	60b6                	ld	ra,328(sp)
    80205178:	6416                	ld	s0,320(sp)
    8020517a:	74f2                	ld	s1,312(sp)
    8020517c:	7952                	ld	s2,304(sp)
    8020517e:	79b2                	ld	s3,296(sp)
    80205180:	7a12                	ld	s4,288(sp)
    80205182:	6af2                	ld	s5,280(sp)
    80205184:	6171                	addi	sp,sp,336
    80205186:	8082                	ret
  struct dirent *de = myproc()->cwd;
    80205188:	ffffd097          	auipc	ra,0xffffd
    8020518c:	950080e7          	jalr	-1712(ra) # 80201ad8 <myproc>
    80205190:	44853483          	ld	s1,1096(a0)
  if (de->parent == NULL) {
    80205194:	1204b783          	ld	a5,288(s1)
    80205198:	c7d1                	beqz	a5,80205224 <sys_getcwd+0xe6>
    *s = '\0';
    8020519a:	fa0409a3          	sb	zero,-77(s0)
    s = path + FAT32_MAX_PATH - 1;  //从后向前填充
    8020519e:	fb340993          	addi	s3,s0,-77
      if (s <= path)          // can't reach root "/"
    802051a2:	eb040a13          	addi	s4,s0,-336
      *--s = '/';
    802051a6:	02f00a93          	li	s5,47
      len = strlen(de->filename);
    802051aa:	8526                	mv	a0,s1
    802051ac:	ffffb097          	auipc	ra,0xffffb
    802051b0:	768080e7          	jalr	1896(ra) # 80200914 <strlen>
    802051b4:	862a                	mv	a2,a0
      s -= len;
    802051b6:	40a98933          	sub	s2,s3,a0
      if (s <= path)          // can't reach root "/"
    802051ba:	0b2a7963          	bgeu	s4,s2,8020526c <sys_getcwd+0x12e>
      strncpy(s, de->filename, len);
    802051be:	85a6                	mv	a1,s1
    802051c0:	854a                	mv	a0,s2
    802051c2:	ffffb097          	auipc	ra,0xffffb
    802051c6:	6e2080e7          	jalr	1762(ra) # 802008a4 <strncpy>
      *--s = '/';
    802051ca:	fff90993          	addi	s3,s2,-1
    802051ce:	ff590fa3          	sb	s5,-1(s2)
      de = de->parent;
    802051d2:	1204b483          	ld	s1,288(s1)
    while (de->parent) {
    802051d6:	1204b783          	ld	a5,288(s1)
    802051da:	fbe1                	bnez	a5,802051aa <sys_getcwd+0x6c>
  if (addr == NULL) {
    802051dc:	fb843483          	ld	s1,-72(s0)
    802051e0:	e4b9                	bnez	s1,8020522e <sys_getcwd+0xf0>
    p = myproc();
    802051e2:	ffffd097          	auipc	ra,0xffffd
    802051e6:	8f6080e7          	jalr	-1802(ra) # 80201ad8 <myproc>
    802051ea:	892a                	mv	s2,a0
    if (p->trapframe->sp < (strlen(s) + 1))
    802051ec:	713c                	ld	a5,96(a0)
    802051ee:	0307ba03          	ld	s4,48(a5)
    802051f2:	854e                	mv	a0,s3
    802051f4:	ffffb097          	auipc	ra,0xffffb
    802051f8:	720080e7          	jalr	1824(ra) # 80200914 <strlen>
    802051fc:	2505                	addiw	a0,a0,1
    802051fe:	f6aa6be3          	bltu	s4,a0,80205174 <sys_getcwd+0x36>
    p->trapframe->sp -= strlen(s) + 1;
    80205202:	854e                	mv	a0,s3
    80205204:	ffffb097          	auipc	ra,0xffffb
    80205208:	710080e7          	jalr	1808(ra) # 80200914 <strlen>
    8020520c:	06093703          	ld	a4,96(s2)
    80205210:	2505                	addiw	a0,a0,1
    80205212:	7b1c                	ld	a5,48(a4)
    80205214:	8f89                	sub	a5,a5,a0
    80205216:	fb1c                	sd	a5,48(a4)
    addr = p->trapframe->sp;
    80205218:	06093783          	ld	a5,96(s2)
    8020521c:	7b9c                	ld	a5,48(a5)
    8020521e:	faf43c23          	sd	a5,-72(s0)
    80205222:	a005                	j	80205242 <sys_getcwd+0x104>
    s = "/";
    80205224:	00004997          	auipc	s3,0x4
    80205228:	48498993          	addi	s3,s3,1156 # 802096a8 <digits+0x328>
    8020522c:	bf45                	j	802051dc <sys_getcwd+0x9e>
    int path_length = strlen(s) + 1;  // 缓冲区空间不足
    8020522e:	854e                	mv	a0,s3
    80205230:	ffffb097          	auipc	ra,0xffffb
    80205234:	6e4080e7          	jalr	1764(ra) # 80200914 <strlen>
      if (size < path_length)
    80205238:	fb442783          	lw	a5,-76(s0)
        return NULL;
    8020523c:	4481                	li	s1,0
      if (size < path_length)
    8020523e:	f2f55be3          	bge	a0,a5,80205174 <sys_getcwd+0x36>
  if (copyout2(addr, s, strlen(s) + 1) < 0)
    80205242:	fb843483          	ld	s1,-72(s0)
    80205246:	854e                	mv	a0,s3
    80205248:	ffffb097          	auipc	ra,0xffffb
    8020524c:	6cc080e7          	jalr	1740(ra) # 80200914 <strlen>
    80205250:	0015061b          	addiw	a2,a0,1
    80205254:	85ce                	mv	a1,s3
    80205256:	8526                	mv	a0,s1
    80205258:	ffffc097          	auipc	ra,0xffffc
    8020525c:	1a6080e7          	jalr	422(ra) # 802013fe <copyout2>
    return NULL;
    80205260:	4481                	li	s1,0
  if (copyout2(addr, s, strlen(s) + 1) < 0)
    80205262:	f00549e3          	bltz	a0,80205174 <sys_getcwd+0x36>
  return addr;
    80205266:	fb843483          	ld	s1,-72(s0)
    8020526a:	b729                	j	80205174 <sys_getcwd+0x36>
        return NULL;
    8020526c:	4481                	li	s1,0
    8020526e:	b719                	j	80205174 <sys_getcwd+0x36>

0000000080205270 <sys_remove>:

uint64
sys_remove(void)
{
    80205270:	7169                	addi	sp,sp,-304
    80205272:	f606                	sd	ra,296(sp)
    80205274:	f222                	sd	s0,288(sp)
    80205276:	ee26                	sd	s1,280(sp)
    80205278:	1a00                	addi	s0,sp,304
  char path[FAT32_MAX_PATH];
  struct dirent *ep;
  int len;
  if((len = argstr(0, path, FAT32_MAX_PATH)) <= 0)
    8020527a:	10400613          	li	a2,260
    8020527e:	ed840593          	addi	a1,s0,-296
    80205282:	4501                	li	a0,0
    80205284:	ffffe097          	auipc	ra,0xffffe
    80205288:	ef6080e7          	jalr	-266(ra) # 8020317a <argstr>
    8020528c:	0ca05c63          	blez	a0,80205364 <sys_remove+0xf4>
    return -1;

  char *s = path + len - 1;
    80205290:	157d                	addi	a0,a0,-1
    80205292:	ed840713          	addi	a4,s0,-296
    80205296:	00a707b3          	add	a5,a4,a0
  while (s >= path && *s == '/') {
    8020529a:	02f00693          	li	a3,47
    8020529e:	863a                	mv	a2,a4
    802052a0:	00e7e963          	bltu	a5,a4,802052b2 <sys_remove+0x42>
    802052a4:	0007c703          	lbu	a4,0(a5)
    802052a8:	06d71c63          	bne	a4,a3,80205320 <sys_remove+0xb0>
    s--;
    802052ac:	17fd                	addi	a5,a5,-1
  while (s >= path && *s == '/') {
    802052ae:	fec7fbe3          	bgeu	a5,a2,802052a4 <sys_remove+0x34>
  }
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    return -1;
  }
  
  if((ep = ename(path)) == NULL){
    802052b2:	ed840513          	addi	a0,s0,-296
    802052b6:	00002097          	auipc	ra,0x2
    802052ba:	150080e7          	jalr	336(ra) # 80207406 <ename>
    802052be:	84aa                	mv	s1,a0
    802052c0:	c555                	beqz	a0,8020536c <sys_remove+0xfc>
    return -1;
  }
  elock(ep);
    802052c2:	00002097          	auipc	ra,0x2
    802052c6:	99c080e7          	jalr	-1636(ra) # 80206c5e <elock>
  if((ep->attribute & ATTR_DIRECTORY) && !isdirempty(ep)){
    802052ca:	1004c783          	lbu	a5,256(s1)
    802052ce:	8bc1                	andi	a5,a5,16
    802052d0:	c799                	beqz	a5,802052de <sys_remove+0x6e>
    802052d2:	8526                	mv	a0,s1
    802052d4:	fffff097          	auipc	ra,0xfffff
    802052d8:	6b8080e7          	jalr	1720(ra) # 8020498c <isdirempty>
    802052dc:	c925                	beqz	a0,8020534c <sys_remove+0xdc>
      eunlock(ep);
      eput(ep);
      return -1;
  }
  elock(ep->parent);      // Will this lead to deadlock?
    802052de:	1204b503          	ld	a0,288(s1)
    802052e2:	00002097          	auipc	ra,0x2
    802052e6:	97c080e7          	jalr	-1668(ra) # 80206c5e <elock>
  eremove(ep);
    802052ea:	8526                	mv	a0,s1
    802052ec:	00002097          	auipc	ra,0x2
    802052f0:	83e080e7          	jalr	-1986(ra) # 80206b2a <eremove>
  eunlock(ep->parent);
    802052f4:	1204b503          	ld	a0,288(s1)
    802052f8:	00002097          	auipc	ra,0x2
    802052fc:	99c080e7          	jalr	-1636(ra) # 80206c94 <eunlock>
  eunlock(ep);
    80205300:	8526                	mv	a0,s1
    80205302:	00002097          	auipc	ra,0x2
    80205306:	992080e7          	jalr	-1646(ra) # 80206c94 <eunlock>
  eput(ep);
    8020530a:	8526                	mv	a0,s1
    8020530c:	00002097          	auipc	ra,0x2
    80205310:	9d6080e7          	jalr	-1578(ra) # 80206ce2 <eput>

  return 0;
    80205314:	4501                	li	a0,0
}
    80205316:	70b2                	ld	ra,296(sp)
    80205318:	7412                	ld	s0,288(sp)
    8020531a:	64f2                	ld	s1,280(sp)
    8020531c:	6155                	addi	sp,sp,304
    8020531e:	8082                	ret
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    80205320:	ed840713          	addi	a4,s0,-296
    80205324:	f8e7e7e3          	bltu	a5,a4,802052b2 <sys_remove+0x42>
    80205328:	0007c683          	lbu	a3,0(a5)
    8020532c:	02e00713          	li	a4,46
    80205330:	f8e691e3          	bne	a3,a4,802052b2 <sys_remove+0x42>
    80205334:	ed840713          	addi	a4,s0,-296
    80205338:	02e78863          	beq	a5,a4,80205368 <sys_remove+0xf8>
    8020533c:	fff7c703          	lbu	a4,-1(a5)
    80205340:	02f00793          	li	a5,47
    80205344:	f6f717e3          	bne	a4,a5,802052b2 <sys_remove+0x42>
    return -1;
    80205348:	557d                	li	a0,-1
    8020534a:	b7f1                	j	80205316 <sys_remove+0xa6>
      eunlock(ep);
    8020534c:	8526                	mv	a0,s1
    8020534e:	00002097          	auipc	ra,0x2
    80205352:	946080e7          	jalr	-1722(ra) # 80206c94 <eunlock>
      eput(ep);
    80205356:	8526                	mv	a0,s1
    80205358:	00002097          	auipc	ra,0x2
    8020535c:	98a080e7          	jalr	-1654(ra) # 80206ce2 <eput>
      return -1;
    80205360:	557d                	li	a0,-1
    80205362:	bf55                	j	80205316 <sys_remove+0xa6>
    return -1;
    80205364:	557d                	li	a0,-1
    80205366:	bf45                	j	80205316 <sys_remove+0xa6>
    return -1;
    80205368:	557d                	li	a0,-1
    8020536a:	b775                	j	80205316 <sys_remove+0xa6>
    return -1;
    8020536c:	557d                	li	a0,-1
    8020536e:	b765                	j	80205316 <sys_remove+0xa6>

0000000080205370 <sys_rename>:

// Must hold too many locks at a time! It's possible to raise a deadlock.
// Because this op takes some steps, we can't promise
uint64
sys_rename(void)
{
    80205370:	db010113          	addi	sp,sp,-592
    80205374:	24113423          	sd	ra,584(sp)
    80205378:	24813023          	sd	s0,576(sp)
    8020537c:	22913c23          	sd	s1,568(sp)
    80205380:	23213823          	sd	s2,560(sp)
    80205384:	23313423          	sd	s3,552(sp)
    80205388:	23413023          	sd	s4,544(sp)
    8020538c:	0c80                	addi	s0,sp,592
  char old[FAT32_MAX_PATH], new[FAT32_MAX_PATH];
  if (argstr(0, old, FAT32_MAX_PATH) < 0 || argstr(1, new, FAT32_MAX_PATH) < 0) {
    8020538e:	10400613          	li	a2,260
    80205392:	ec840593          	addi	a1,s0,-312
    80205396:	4501                	li	a0,0
    80205398:	ffffe097          	auipc	ra,0xffffe
    8020539c:	de2080e7          	jalr	-542(ra) # 8020317a <argstr>
      return -1;
    802053a0:	57fd                	li	a5,-1
  if (argstr(0, old, FAT32_MAX_PATH) < 0 || argstr(1, new, FAT32_MAX_PATH) < 0) {
    802053a2:	0c054d63          	bltz	a0,8020547c <sys_rename+0x10c>
    802053a6:	10400613          	li	a2,260
    802053aa:	dc040593          	addi	a1,s0,-576
    802053ae:	4505                	li	a0,1
    802053b0:	ffffe097          	auipc	ra,0xffffe
    802053b4:	dca080e7          	jalr	-566(ra) # 8020317a <argstr>
      return -1;
    802053b8:	57fd                	li	a5,-1
  if (argstr(0, old, FAT32_MAX_PATH) < 0 || argstr(1, new, FAT32_MAX_PATH) < 0) {
    802053ba:	0c054163          	bltz	a0,8020547c <sys_rename+0x10c>
  }

  struct dirent *src = NULL, *dst = NULL, *pdst = NULL;
  int srclock = 0;
  char *name;
  if ((src = ename(old)) == NULL || (pdst = enameparent(new, old)) == NULL
    802053be:	ec840513          	addi	a0,s0,-312
    802053c2:	00002097          	auipc	ra,0x2
    802053c6:	044080e7          	jalr	68(ra) # 80207406 <ename>
    802053ca:	84aa                	mv	s1,a0
    802053cc:	1a050f63          	beqz	a0,8020558a <sys_rename+0x21a>
    802053d0:	ec840593          	addi	a1,s0,-312
    802053d4:	dc040513          	addi	a0,s0,-576
    802053d8:	00002097          	auipc	ra,0x2
    802053dc:	04c080e7          	jalr	76(ra) # 80207424 <enameparent>
    802053e0:	892a                	mv	s2,a0
    802053e2:	cd35                	beqz	a0,8020545e <sys_rename+0xee>
      || (name = formatname(old)) == NULL) {
    802053e4:	ec840513          	addi	a0,s0,-312
    802053e8:	00001097          	auipc	ra,0x1
    802053ec:	196080e7          	jalr	406(ra) # 8020657e <formatname>
    802053f0:	89aa                	mv	s3,a0
    802053f2:	c535                	beqz	a0,8020545e <sys_rename+0xee>
    goto fail;          // src doesn't exist || dst parent doesn't exist || illegal new name
  }
  for (struct dirent *ep = pdst; ep != NULL; ep = ep->parent) {
    if (ep == src) {    // In what universe can we move a directory into its child?
    802053f4:	07248563          	beq	s1,s2,8020545e <sys_rename+0xee>
  for (struct dirent *ep = pdst; ep != NULL; ep = ep->parent) {
    802053f8:	87ca                	mv	a5,s2
    802053fa:	1207b783          	ld	a5,288(a5)
    802053fe:	c781                	beqz	a5,80205406 <sys_rename+0x96>
    if (ep == src) {    // In what universe can we move a directory into its child?
    80205400:	fef49de3          	bne	s1,a5,802053fa <sys_rename+0x8a>
    80205404:	a8a9                	j	8020545e <sys_rename+0xee>
      goto fail;
    }
  }

  uint off;
  elock(src);     // must hold child's lock before acquiring parent's, because we do so in other similar cases
    80205406:	8526                	mv	a0,s1
    80205408:	00002097          	auipc	ra,0x2
    8020540c:	856080e7          	jalr	-1962(ra) # 80206c5e <elock>
  srclock = 1;
  elock(pdst);
    80205410:	854a                	mv	a0,s2
    80205412:	00002097          	auipc	ra,0x2
    80205416:	84c080e7          	jalr	-1972(ra) # 80206c5e <elock>
  dst = dirlookup(pdst, name, &off);
    8020541a:	dbc40613          	addi	a2,s0,-580
    8020541e:	85ce                	mv	a1,s3
    80205420:	854a                	mv	a0,s2
    80205422:	00002097          	auipc	ra,0x2
    80205426:	bca080e7          	jalr	-1078(ra) # 80206fec <dirlookup>
    8020542a:	8a2a                	mv	s4,a0
  if (dst != NULL) {
    8020542c:	c155                	beqz	a0,802054d0 <sys_rename+0x160>
    eunlock(pdst);
    8020542e:	854a                	mv	a0,s2
    80205430:	00002097          	auipc	ra,0x2
    80205434:	864080e7          	jalr	-1948(ra) # 80206c94 <eunlock>
    if (src == dst) {
    80205438:	01448963          	beq	s1,s4,8020544a <sys_rename+0xda>
      goto fail;
    } else if (src->attribute & dst->attribute & ATTR_DIRECTORY) {
    8020543c:	1004c783          	lbu	a5,256(s1)
    80205440:	100a4703          	lbu	a4,256(s4)
    80205444:	8ff9                	and	a5,a5,a4
    80205446:	8bc1                	andi	a5,a5,16
    80205448:	ebb1                	bnez	a5,8020549c <sys_rename+0x12c>

  return 0;

fail:
  if (srclock)
    eunlock(src);
    8020544a:	8526                	mv	a0,s1
    8020544c:	00002097          	auipc	ra,0x2
    80205450:	848080e7          	jalr	-1976(ra) # 80206c94 <eunlock>
  if (dst)
    eput(dst);
    80205454:	8552                	mv	a0,s4
    80205456:	00002097          	auipc	ra,0x2
    8020545a:	88c080e7          	jalr	-1908(ra) # 80206ce2 <eput>
  if (pdst)
    8020545e:	00090763          	beqz	s2,8020546c <sys_rename+0xfc>
    eput(pdst);
    80205462:	854a                	mv	a0,s2
    80205464:	00002097          	auipc	ra,0x2
    80205468:	87e080e7          	jalr	-1922(ra) # 80206ce2 <eput>
  if (src)
    eput(src);
  return -1;
    8020546c:	57fd                	li	a5,-1
  if (src)
    8020546e:	c499                	beqz	s1,8020547c <sys_rename+0x10c>
    eput(src);
    80205470:	8526                	mv	a0,s1
    80205472:	00002097          	auipc	ra,0x2
    80205476:	870080e7          	jalr	-1936(ra) # 80206ce2 <eput>
  return -1;
    8020547a:	57fd                	li	a5,-1
}
    8020547c:	853e                	mv	a0,a5
    8020547e:	24813083          	ld	ra,584(sp)
    80205482:	24013403          	ld	s0,576(sp)
    80205486:	23813483          	ld	s1,568(sp)
    8020548a:	23013903          	ld	s2,560(sp)
    8020548e:	22813983          	ld	s3,552(sp)
    80205492:	22013a03          	ld	s4,544(sp)
    80205496:	25010113          	addi	sp,sp,592
    8020549a:	8082                	ret
      elock(dst);
    8020549c:	8552                	mv	a0,s4
    8020549e:	00001097          	auipc	ra,0x1
    802054a2:	7c0080e7          	jalr	1984(ra) # 80206c5e <elock>
      if (!isdirempty(dst)) {    // it's ok to overwrite an empty dir
    802054a6:	8552                	mv	a0,s4
    802054a8:	fffff097          	auipc	ra,0xfffff
    802054ac:	4e4080e7          	jalr	1252(ra) # 8020498c <isdirempty>
    802054b0:	c579                	beqz	a0,8020557e <sys_rename+0x20e>
      elock(pdst);
    802054b2:	854a                	mv	a0,s2
    802054b4:	00001097          	auipc	ra,0x1
    802054b8:	7aa080e7          	jalr	1962(ra) # 80206c5e <elock>
    eremove(dst);
    802054bc:	8552                	mv	a0,s4
    802054be:	00001097          	auipc	ra,0x1
    802054c2:	66c080e7          	jalr	1644(ra) # 80206b2a <eremove>
    eunlock(dst);
    802054c6:	8552                	mv	a0,s4
    802054c8:	00001097          	auipc	ra,0x1
    802054cc:	7cc080e7          	jalr	1996(ra) # 80206c94 <eunlock>
  memmove(src->filename, name, FAT32_MAX_FILENAME);
    802054d0:	0ff00613          	li	a2,255
    802054d4:	85ce                	mv	a1,s3
    802054d6:	8526                	mv	a0,s1
    802054d8:	ffffb097          	auipc	ra,0xffffb
    802054dc:	314080e7          	jalr	788(ra) # 802007ec <memmove>
  emake(pdst, src, off);
    802054e0:	dbc42603          	lw	a2,-580(s0)
    802054e4:	85a6                	mv	a1,s1
    802054e6:	854a                	mv	a0,s2
    802054e8:	00001097          	auipc	ra,0x1
    802054ec:	14e080e7          	jalr	334(ra) # 80206636 <emake>
  if (src->parent != pdst) {
    802054f0:	1204b783          	ld	a5,288(s1)
    802054f4:	01278d63          	beq	a5,s2,8020550e <sys_rename+0x19e>
    eunlock(pdst);
    802054f8:	854a                	mv	a0,s2
    802054fa:	00001097          	auipc	ra,0x1
    802054fe:	79a080e7          	jalr	1946(ra) # 80206c94 <eunlock>
    elock(src->parent);
    80205502:	1204b503          	ld	a0,288(s1)
    80205506:	00001097          	auipc	ra,0x1
    8020550a:	758080e7          	jalr	1880(ra) # 80206c5e <elock>
  eremove(src);
    8020550e:	8526                	mv	a0,s1
    80205510:	00001097          	auipc	ra,0x1
    80205514:	61a080e7          	jalr	1562(ra) # 80206b2a <eremove>
  eunlock(src->parent);
    80205518:	1204b503          	ld	a0,288(s1)
    8020551c:	00001097          	auipc	ra,0x1
    80205520:	778080e7          	jalr	1912(ra) # 80206c94 <eunlock>
  struct dirent *psrc = src->parent;  // src must not be root, or it won't pass the for-loop test
    80205524:	1204b983          	ld	s3,288(s1)
  src->parent = edup(pdst);
    80205528:	854a                	mv	a0,s2
    8020552a:	00001097          	auipc	ra,0x1
    8020552e:	4d6080e7          	jalr	1238(ra) # 80206a00 <edup>
    80205532:	12a4b023          	sd	a0,288(s1)
  src->off = off;
    80205536:	dbc42783          	lw	a5,-580(s0)
    8020553a:	10f4ae23          	sw	a5,284(s1)
  src->valid = 1;
    8020553e:	4785                	li	a5,1
    80205540:	10f49b23          	sh	a5,278(s1)
  eunlock(src);
    80205544:	8526                	mv	a0,s1
    80205546:	00001097          	auipc	ra,0x1
    8020554a:	74e080e7          	jalr	1870(ra) # 80206c94 <eunlock>
  eput(psrc);
    8020554e:	854e                	mv	a0,s3
    80205550:	00001097          	auipc	ra,0x1
    80205554:	792080e7          	jalr	1938(ra) # 80206ce2 <eput>
  if (dst) {
    80205558:	000a0763          	beqz	s4,80205566 <sys_rename+0x1f6>
    eput(dst);
    8020555c:	8552                	mv	a0,s4
    8020555e:	00001097          	auipc	ra,0x1
    80205562:	784080e7          	jalr	1924(ra) # 80206ce2 <eput>
  eput(pdst);
    80205566:	854a                	mv	a0,s2
    80205568:	00001097          	auipc	ra,0x1
    8020556c:	77a080e7          	jalr	1914(ra) # 80206ce2 <eput>
  eput(src);
    80205570:	8526                	mv	a0,s1
    80205572:	00001097          	auipc	ra,0x1
    80205576:	770080e7          	jalr	1904(ra) # 80206ce2 <eput>
  return 0;
    8020557a:	4781                	li	a5,0
    8020557c:	b701                	j	8020547c <sys_rename+0x10c>
        eunlock(dst);
    8020557e:	8552                	mv	a0,s4
    80205580:	00001097          	auipc	ra,0x1
    80205584:	714080e7          	jalr	1812(ra) # 80206c94 <eunlock>
        goto fail;
    80205588:	b5c9                	j	8020544a <sys_rename+0xda>
  struct dirent *src = NULL, *dst = NULL, *pdst = NULL;
    8020558a:	892a                	mv	s2,a0
  if (dst)
    8020558c:	bdc9                	j	8020545e <sys_rename+0xee>

000000008020558e <sys_dup3>:

uint64
sys_dup3(void)
{
    8020558e:	1101                	addi	sp,sp,-32
    80205590:	ec06                	sd	ra,24(sp)
    80205592:	e822                	sd	s0,16(sp)
    80205594:	1000                	addi	s0,sp,32
  struct file *f;
  int old_fd, new_fd;

  // printf("called\n");
  if (argfd(0, &old_fd, &f) < 0) // 获取第一个参数old_fd，并将其转换为文件结构指针f
    80205596:	fe840613          	addi	a2,s0,-24
    8020559a:	fe440593          	addi	a1,s0,-28
    8020559e:	4501                	li	a0,0
    802055a0:	fffff097          	auipc	ra,0xfffff
    802055a4:	41a080e7          	jalr	1050(ra) # 802049ba <argfd>
    return -1;
    802055a8:	57fd                	li	a5,-1
  if (argfd(0, &old_fd, &f) < 0) // 获取第一个参数old_fd，并将其转换为文件结构指针f
    802055aa:	06054e63          	bltz	a0,80205626 <sys_dup3+0x98>
  if (argint(1, &new_fd) < 0) // 获取第二个参数new_fd
    802055ae:	fe040593          	addi	a1,s0,-32
    802055b2:	4505                	li	a0,1
    802055b4:	ffffe097          	auipc	ra,0xffffe
    802055b8:	b42080e7          	jalr	-1214(ra) # 802030f6 <argint>
    802055bc:	06054a63          	bltz	a0,80205630 <sys_dup3+0xa2>
    return -1;

  // printf("%d, %d\n", old_fd, new_fd);

  // 检查new_fd是否合法
  if (new_fd < 0 || new_fd > NOFILE)  // NOFILE在param.h中，需要改大一些
    802055c0:	fe042683          	lw	a3,-32(s0)
    802055c4:	06e00713          	li	a4,110
    return -1;
    802055c8:	57fd                	li	a5,-1
  if (new_fd < 0 || new_fd > NOFILE)  // NOFILE在param.h中，需要改大一些
    802055ca:	04d76e63          	bltu	a4,a3,80205626 <sys_dup3+0x98>

  // 如果new_fd已经打开，先关闭它
  if (myproc()->ofile[new_fd]) {
    802055ce:	ffffc097          	auipc	ra,0xffffc
    802055d2:	50a080e7          	jalr	1290(ra) # 80201ad8 <myproc>
    802055d6:	fe042783          	lw	a5,-32(s0)
    802055da:	07e9                	addi	a5,a5,26
    802055dc:	078e                	slli	a5,a5,0x3
    802055de:	953e                	add	a0,a0,a5
    802055e0:	651c                	ld	a5,8(a0)
    802055e2:	cf99                	beqz	a5,80205600 <sys_dup3+0x72>
    fileclose(myproc()->ofile[new_fd]);
    802055e4:	ffffc097          	auipc	ra,0xffffc
    802055e8:	4f4080e7          	jalr	1268(ra) # 80201ad8 <myproc>
    802055ec:	fe042783          	lw	a5,-32(s0)
    802055f0:	07e9                	addi	a5,a5,26
    802055f2:	078e                	slli	a5,a5,0x3
    802055f4:	953e                	add	a0,a0,a5
    802055f6:	6508                	ld	a0,8(a0)
    802055f8:	ffffe097          	auipc	ra,0xffffe
    802055fc:	7d0080e7          	jalr	2000(ra) # 80203dc8 <fileclose>
  }

  myproc()->ofile[new_fd] = f;
    80205600:	ffffc097          	auipc	ra,0xffffc
    80205604:	4d8080e7          	jalr	1240(ra) # 80201ad8 <myproc>
    80205608:	fe843703          	ld	a4,-24(s0)
    8020560c:	fe042783          	lw	a5,-32(s0)
    80205610:	07e9                	addi	a5,a5,26
    80205612:	078e                	slli	a5,a5,0x3
    80205614:	97aa                	add	a5,a5,a0
    80205616:	e798                	sd	a4,8(a5)
  filedup(f);
    80205618:	853a                	mv	a0,a4
    8020561a:	ffffe097          	auipc	ra,0xffffe
    8020561e:	75c080e7          	jalr	1884(ra) # 80203d76 <filedup>
  // printf("%d, %d\n", old_fd, new_fd);
  return new_fd;
    80205622:	fe042783          	lw	a5,-32(s0)
}
    80205626:	853e                	mv	a0,a5
    80205628:	60e2                	ld	ra,24(sp)
    8020562a:	6442                	ld	s0,16(sp)
    8020562c:	6105                	addi	sp,sp,32
    8020562e:	8082                	ret
    return -1;
    80205630:	57fd                	li	a5,-1
    80205632:	bfd5                	j	80205626 <sys_dup3+0x98>

0000000080205634 <sys_getdents>:

uint64
sys_getdents(void)
{
    80205634:	7141                	addi	sp,sp,-496
    80205636:	f786                	sd	ra,488(sp)
    80205638:	f3a2                	sd	s0,480(sp)
    8020563a:	efa6                	sd	s1,472(sp)
    8020563c:	ebca                	sd	s2,464(sp)
    8020563e:	e7ce                	sd	s3,456(sp)
    80205640:	e3d2                	sd	s4,448(sp)
    80205642:	ff56                	sd	s5,440(sp)
    80205644:	fb5a                	sd	s6,432(sp)
    80205646:	f75e                	sd	s7,424(sp)
    80205648:	f362                	sd	s8,416(sp)
    8020564a:	1b80                	addi	s0,sp,496
  int fd, len;
  uint64 addr;
  struct file *f;
  int nread = 0;  
  struct dirent de;
  int count = 0;
    8020564c:	e2042a23          	sw	zero,-460(s0)
  int ret;
  struct linux_dirent64 tmp;

  if(argint(0, &fd) < 0 || argint(2, &len) < 0 || argaddr(1, &addr) < 0){
    80205650:	fac40593          	addi	a1,s0,-84
    80205654:	4501                	li	a0,0
    80205656:	ffffe097          	auipc	ra,0xffffe
    8020565a:	aa0080e7          	jalr	-1376(ra) # 802030f6 <argint>
    return -1;
    8020565e:	57fd                	li	a5,-1
  if(argint(0, &fd) < 0 || argint(2, &len) < 0 || argaddr(1, &addr) < 0){
    80205660:	10054e63          	bltz	a0,8020577c <sys_getdents+0x148>
    80205664:	fa840593          	addi	a1,s0,-88
    80205668:	4509                	li	a0,2
    8020566a:	ffffe097          	auipc	ra,0xffffe
    8020566e:	a8c080e7          	jalr	-1396(ra) # 802030f6 <argint>
    return -1;
    80205672:	57fd                	li	a5,-1
  if(argint(0, &fd) < 0 || argint(2, &len) < 0 || argaddr(1, &addr) < 0){
    80205674:	10054463          	bltz	a0,8020577c <sys_getdents+0x148>
    80205678:	fa040593          	addi	a1,s0,-96
    8020567c:	4505                	li	a0,1
    8020567e:	ffffe097          	auipc	ra,0xffffe
    80205682:	ada080e7          	jalr	-1318(ra) # 80203158 <argaddr>
    80205686:	0e054a63          	bltz	a0,8020577a <sys_getdents+0x146>
  }
  if(fd < 0 || fd >= NOFILE || (f = myproc()->ofile[fd]) == NULL)
    8020568a:	fac42683          	lw	a3,-84(s0)
    8020568e:	06d00713          	li	a4,109
    return -1;
    80205692:	57fd                	li	a5,-1
  if(fd < 0 || fd >= NOFILE || (f = myproc()->ofile[fd]) == NULL)
    80205694:	0ed76463          	bltu	a4,a3,8020577c <sys_getdents+0x148>
    80205698:	ffffc097          	auipc	ra,0xffffc
    8020569c:	440080e7          	jalr	1088(ra) # 80201ad8 <myproc>
    802056a0:	fac42783          	lw	a5,-84(s0)
    802056a4:	07e9                	addi	a5,a5,26
    802056a6:	078e                	slli	a5,a5,0x3
    802056a8:	953e                	add	a0,a0,a5
    802056aa:	6504                	ld	s1,8(a0)
    802056ac:	c4ed                	beqz	s1,80205796 <sys_getdents+0x162>
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    802056ae:	0084c703          	lbu	a4,8(s1)
    return -1;
    802056b2:	57fd                	li	a5,-1
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    802056b4:	c761                	beqz	a4,8020577c <sys_getdents+0x148>
    802056b6:	6c9c                	ld	a5,24(s1)
    802056b8:	1007c703          	lbu	a4,256(a5)
    802056bc:	8b41                	andi	a4,a4,16
    return -1;
    802056be:	57fd                	li	a5,-1
  if(f->readable == 0 || !(f->ep->attribute & ATTR_DIRECTORY))
    802056c0:	cf55                	beqz	a4,8020577c <sys_getdents+0x148>

  while(nread + (int)(sizeof(tmp)) <= len){
    802056c2:	fa842703          	lw	a4,-88(s0)
    802056c6:	47dd                	li	a5,23
    802056c8:	0ae7d763          	bge	a5,a4,80205776 <sys_getdents+0x142>
  int nread = 0;  
    802056cc:	4a81                	li	s5,0
    elock(f->ep);
    while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
    802056ce:	e3440a13          	addi	s4,s0,-460
    802056d2:	e3840993          	addi	s3,s0,-456
      f->off += count * 32;
    }
    eunlock(f->ep);
    if (ret == -1)  // meet the end of dir
    802056d6:	5b7d                	li	s6,-1
      return 0;

    f->off += count * 32;
    safestrcpy(tmp.d_name, de.filename, strlen(de.filename) + 1);
    802056d8:	e1840c13          	addi	s8,s0,-488
    802056dc:	e2b40b93          	addi	s7,s0,-469
    802056e0:	a069                	j	8020576a <sys_getdents+0x136>
      f->off += count * 32;
    802056e2:	e3442783          	lw	a5,-460(s0)
    802056e6:	0057979b          	slliw	a5,a5,0x5
    802056ea:	5098                	lw	a4,32(s1)
    802056ec:	9fb9                	addw	a5,a5,a4
    802056ee:	d09c                	sw	a5,32(s1)
    while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
    802056f0:	86d2                	mv	a3,s4
    802056f2:	5090                	lw	a2,32(s1)
    802056f4:	85ce                	mv	a1,s3
    802056f6:	6c88                	ld	a0,24(s1)
    802056f8:	00001097          	auipc	ra,0x1
    802056fc:	76a080e7          	jalr	1898(ra) # 80206e62 <enext>
    80205700:	892a                	mv	s2,a0
    80205702:	d165                	beqz	a0,802056e2 <sys_getdents+0xae>
    eunlock(f->ep);
    80205704:	6c88                	ld	a0,24(s1)
    80205706:	00001097          	auipc	ra,0x1
    8020570a:	58e080e7          	jalr	1422(ra) # 80206c94 <eunlock>
    if (ret == -1)  // meet the end of dir
    8020570e:	09690663          	beq	s2,s6,8020579a <sys_getdents+0x166>
    f->off += count * 32;
    80205712:	e3442783          	lw	a5,-460(s0)
    80205716:	0057979b          	slliw	a5,a5,0x5
    8020571a:	5098                	lw	a4,32(s1)
    8020571c:	9fb9                	addw	a5,a5,a4
    8020571e:	d09c                	sw	a5,32(s1)
    safestrcpy(tmp.d_name, de.filename, strlen(de.filename) + 1);
    80205720:	854e                	mv	a0,s3
    80205722:	ffffb097          	auipc	ra,0xffffb
    80205726:	1f2080e7          	jalr	498(ra) # 80200914 <strlen>
    8020572a:	0015061b          	addiw	a2,a0,1
    8020572e:	85ce                	mv	a1,s3
    80205730:	855e                	mv	a0,s7
    80205732:	ffffb097          	auipc	ra,0xffffb
    80205736:	1b0080e7          	jalr	432(ra) # 802008e2 <safestrcpy>
    
    if(copyout2(addr, (char *)&tmp, sizeof(tmp)) < 0)
    8020573a:	4661                	li	a2,24
    8020573c:	85e2                	mv	a1,s8
    8020573e:	fa043503          	ld	a0,-96(s0)
    80205742:	ffffc097          	auipc	ra,0xffffc
    80205746:	cbc080e7          	jalr	-836(ra) # 802013fe <copyout2>
    8020574a:	04054a63          	bltz	a0,8020579e <sys_getdents+0x16a>
      return -1;

    addr += sizeof(tmp);
    8020574e:	fa043783          	ld	a5,-96(s0)
    80205752:	07e1                	addi	a5,a5,24
    80205754:	faf43023          	sd	a5,-96(s0)
    nread += (int)(sizeof(tmp));
    80205758:	018a879b          	addiw	a5,s5,24
  while(nread + (int)(sizeof(tmp)) <= len){
    8020575c:	fa842703          	lw	a4,-88(s0)
    80205760:	02fa8a9b          	addiw	s5,s5,47
    80205764:	00eadc63          	bge	s5,a4,8020577c <sys_getdents+0x148>
    nread += (int)(sizeof(tmp));
    80205768:	8abe                	mv	s5,a5
    elock(f->ep);
    8020576a:	6c88                	ld	a0,24(s1)
    8020576c:	00001097          	auipc	ra,0x1
    80205770:	4f2080e7          	jalr	1266(ra) # 80206c5e <elock>
    while ((ret = enext(f->ep, &de, f->off, &count)) == 0) {  // skip empty entry
    80205774:	bfb5                	j	802056f0 <sys_getdents+0xbc>
  int nread = 0;  
    80205776:	4781                	li	a5,0
  }
  return nread;
    80205778:	a011                	j	8020577c <sys_getdents+0x148>
    return -1;
    8020577a:	57fd                	li	a5,-1
}
    8020577c:	853e                	mv	a0,a5
    8020577e:	70be                	ld	ra,488(sp)
    80205780:	741e                	ld	s0,480(sp)
    80205782:	64fe                	ld	s1,472(sp)
    80205784:	695e                	ld	s2,464(sp)
    80205786:	69be                	ld	s3,456(sp)
    80205788:	6a1e                	ld	s4,448(sp)
    8020578a:	7afa                	ld	s5,440(sp)
    8020578c:	7b5a                	ld	s6,432(sp)
    8020578e:	7bba                	ld	s7,424(sp)
    80205790:	7c1a                	ld	s8,416(sp)
    80205792:	617d                	addi	sp,sp,496
    80205794:	8082                	ret
    return -1;
    80205796:	57fd                	li	a5,-1
    80205798:	b7d5                	j	8020577c <sys_getdents+0x148>
      return 0;
    8020579a:	4781                	li	a5,0
    8020579c:	b7c5                	j	8020577c <sys_getdents+0x148>
      return -1;
    8020579e:	57fd                	li	a5,-1
    802057a0:	bff1                	j	8020577c <sys_getdents+0x148>

00000000802057a2 <sys_unlink>:

uint64
sys_unlink(void)
{ 
    802057a2:	7169                	addi	sp,sp,-304
    802057a4:	f606                	sd	ra,296(sp)
    802057a6:	f222                	sd	s0,288(sp)
    802057a8:	ee26                	sd	s1,280(sp)
    802057aa:	1a00                	addi	s0,sp,304

  char path[FAT32_MAX_PATH];
  struct dirent *ep;
  int fd, flags, len;
  if(argint(0, &fd) < 0 || argint(2, &flags) < 0)
    802057ac:	ed440593          	addi	a1,s0,-300
    802057b0:	4501                	li	a0,0
    802057b2:	ffffe097          	auipc	ra,0xffffe
    802057b6:	944080e7          	jalr	-1724(ra) # 802030f6 <argint>
    return -1;
    802057ba:	57fd                	li	a5,-1
  if(argint(0, &fd) < 0 || argint(2, &flags) < 0)
    802057bc:	0a054a63          	bltz	a0,80205870 <sys_unlink+0xce>
    802057c0:	ed040593          	addi	a1,s0,-304
    802057c4:	4509                	li	a0,2
    802057c6:	ffffe097          	auipc	ra,0xffffe
    802057ca:	930080e7          	jalr	-1744(ra) # 802030f6 <argint>
    return -1;
    802057ce:	57fd                	li	a5,-1
  if(argint(0, &fd) < 0 || argint(2, &flags) < 0)
    802057d0:	0a054063          	bltz	a0,80205870 <sys_unlink+0xce>
  if((len = argstr(1, path, FAT32_MAX_PATH)) <= 0)
    802057d4:	10400613          	li	a2,260
    802057d8:	ed840593          	addi	a1,s0,-296
    802057dc:	4505                	li	a0,1
    802057de:	ffffe097          	auipc	ra,0xffffe
    802057e2:	99c080e7          	jalr	-1636(ra) # 8020317a <argstr>
    802057e6:	0ca05d63          	blez	a0,802058c0 <sys_unlink+0x11e>
    return -1;
  
  char *s = path + len - 1;
    802057ea:	157d                	addi	a0,a0,-1
    802057ec:	ed840713          	addi	a4,s0,-296
    802057f0:	00a707b3          	add	a5,a4,a0
  while (s >= path && *s == '/') {
    802057f4:	02f00693          	li	a3,47
    802057f8:	863a                	mv	a2,a4
    802057fa:	00e7e963          	bltu	a5,a4,8020580c <sys_unlink+0x6a>
    802057fe:	0007c703          	lbu	a4,0(a5)
    80205802:	06d71d63          	bne	a4,a3,8020587c <sys_unlink+0xda>
    s--;
    80205806:	17fd                	addi	a5,a5,-1
  while (s >= path && *s == '/') {
    80205808:	fec7fbe3          	bgeu	a5,a2,802057fe <sys_unlink+0x5c>
  }
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    return -1;
  }
  
  if((ep = ename(path)) == NULL){
    8020580c:	ed840513          	addi	a0,s0,-296
    80205810:	00002097          	auipc	ra,0x2
    80205814:	bf6080e7          	jalr	-1034(ra) # 80207406 <ename>
    80205818:	84aa                	mv	s1,a0
    8020581a:	c55d                	beqz	a0,802058c8 <sys_unlink+0x126>
    return -1;
  }
  elock(ep);
    8020581c:	00001097          	auipc	ra,0x1
    80205820:	442080e7          	jalr	1090(ra) # 80206c5e <elock>
  if((ep->attribute & ATTR_DIRECTORY) && !isdirempty(ep)){
    80205824:	1004c783          	lbu	a5,256(s1)
    80205828:	8bc1                	andi	a5,a5,16
    8020582a:	c799                	beqz	a5,80205838 <sys_unlink+0x96>
    8020582c:	8526                	mv	a0,s1
    8020582e:	fffff097          	auipc	ra,0xfffff
    80205832:	15e080e7          	jalr	350(ra) # 8020498c <isdirempty>
    80205836:	c92d                	beqz	a0,802058a8 <sys_unlink+0x106>
      eunlock(ep);
      eput(ep);
      return -1;
  }
  elock(ep->parent);
    80205838:	1204b503          	ld	a0,288(s1)
    8020583c:	00001097          	auipc	ra,0x1
    80205840:	422080e7          	jalr	1058(ra) # 80206c5e <elock>
  eremove(ep);
    80205844:	8526                	mv	a0,s1
    80205846:	00001097          	auipc	ra,0x1
    8020584a:	2e4080e7          	jalr	740(ra) # 80206b2a <eremove>
  eunlock(ep->parent);
    8020584e:	1204b503          	ld	a0,288(s1)
    80205852:	00001097          	auipc	ra,0x1
    80205856:	442080e7          	jalr	1090(ra) # 80206c94 <eunlock>
  eunlock(ep);
    8020585a:	8526                	mv	a0,s1
    8020585c:	00001097          	auipc	ra,0x1
    80205860:	438080e7          	jalr	1080(ra) # 80206c94 <eunlock>
  eput(ep);
    80205864:	8526                	mv	a0,s1
    80205866:	00001097          	auipc	ra,0x1
    8020586a:	47c080e7          	jalr	1148(ra) # 80206ce2 <eput>
  return 0;
    8020586e:	4781                	li	a5,0
}
    80205870:	853e                	mv	a0,a5
    80205872:	70b2                	ld	ra,296(sp)
    80205874:	7412                	ld	s0,288(sp)
    80205876:	64f2                	ld	s1,280(sp)
    80205878:	6155                	addi	sp,sp,304
    8020587a:	8082                	ret
  if (s >= path && *s == '.' && (s == path || *--s == '/')) {
    8020587c:	ed840713          	addi	a4,s0,-296
    80205880:	f8e7e6e3          	bltu	a5,a4,8020580c <sys_unlink+0x6a>
    80205884:	0007c683          	lbu	a3,0(a5)
    80205888:	02e00713          	li	a4,46
    8020588c:	f8e690e3          	bne	a3,a4,8020580c <sys_unlink+0x6a>
    80205890:	ed840713          	addi	a4,s0,-296
    80205894:	02e78863          	beq	a5,a4,802058c4 <sys_unlink+0x122>
    80205898:	fff7c703          	lbu	a4,-1(a5)
    8020589c:	02f00793          	li	a5,47
    802058a0:	f6f716e3          	bne	a4,a5,8020580c <sys_unlink+0x6a>
    return -1;
    802058a4:	57fd                	li	a5,-1
    802058a6:	b7e9                	j	80205870 <sys_unlink+0xce>
      eunlock(ep);
    802058a8:	8526                	mv	a0,s1
    802058aa:	00001097          	auipc	ra,0x1
    802058ae:	3ea080e7          	jalr	1002(ra) # 80206c94 <eunlock>
      eput(ep);
    802058b2:	8526                	mv	a0,s1
    802058b4:	00001097          	auipc	ra,0x1
    802058b8:	42e080e7          	jalr	1070(ra) # 80206ce2 <eput>
      return -1;
    802058bc:	57fd                	li	a5,-1
    802058be:	bf4d                	j	80205870 <sys_unlink+0xce>
    return -1;
    802058c0:	57fd                	li	a5,-1
    802058c2:	b77d                	j	80205870 <sys_unlink+0xce>
    return -1;
    802058c4:	57fd                	li	a5,-1
    802058c6:	b76d                	j	80205870 <sys_unlink+0xce>
    return -1;
    802058c8:	57fd                	li	a5,-1
    802058ca:	b75d                	j	80205870 <sys_unlink+0xce>

00000000802058cc <sys_mount>:

uint64
sys_mount(void){
    802058cc:	1141                	addi	sp,sp,-16
    802058ce:	e422                	sd	s0,8(sp)
    802058d0:	0800                	addi	s0,sp,16

	return 0;
}
    802058d2:	4501                	li	a0,0
    802058d4:	6422                	ld	s0,8(sp)
    802058d6:	0141                	addi	sp,sp,16
    802058d8:	8082                	ret

00000000802058da <sys_umount>:

uint64
sys_umount(void){
    802058da:	1141                	addi	sp,sp,-16
    802058dc:	e422                	sd	s0,8(sp)
    802058de:	0800                	addi	s0,sp,16
	return 0;
    802058e0:	4501                	li	a0,0
    802058e2:	6422                	ld	s0,8(sp)
    802058e4:	0141                	addi	sp,sp,16
    802058e6:	8082                	ret
	...

00000000802058f0 <kernelvec>:
    802058f0:	7111                	addi	sp,sp,-256
    802058f2:	e006                	sd	ra,0(sp)
    802058f4:	e40a                	sd	sp,8(sp)
    802058f6:	e80e                	sd	gp,16(sp)
    802058f8:	ec12                	sd	tp,24(sp)
    802058fa:	f016                	sd	t0,32(sp)
    802058fc:	f41a                	sd	t1,40(sp)
    802058fe:	f81e                	sd	t2,48(sp)
    80205900:	fc22                	sd	s0,56(sp)
    80205902:	e0a6                	sd	s1,64(sp)
    80205904:	e4aa                	sd	a0,72(sp)
    80205906:	e8ae                	sd	a1,80(sp)
    80205908:	ecb2                	sd	a2,88(sp)
    8020590a:	f0b6                	sd	a3,96(sp)
    8020590c:	f4ba                	sd	a4,104(sp)
    8020590e:	f8be                	sd	a5,112(sp)
    80205910:	fcc2                	sd	a6,120(sp)
    80205912:	e146                	sd	a7,128(sp)
    80205914:	e54a                	sd	s2,136(sp)
    80205916:	e94e                	sd	s3,144(sp)
    80205918:	ed52                	sd	s4,152(sp)
    8020591a:	f156                	sd	s5,160(sp)
    8020591c:	f55a                	sd	s6,168(sp)
    8020591e:	f95e                	sd	s7,176(sp)
    80205920:	fd62                	sd	s8,184(sp)
    80205922:	e1e6                	sd	s9,192(sp)
    80205924:	e5ea                	sd	s10,200(sp)
    80205926:	e9ee                	sd	s11,208(sp)
    80205928:	edf2                	sd	t3,216(sp)
    8020592a:	f1f6                	sd	t4,224(sp)
    8020592c:	f5fa                	sd	t5,232(sp)
    8020592e:	f9fe                	sd	t6,240(sp)
    80205930:	a20fd0ef          	jal	ra,80202b50 <kerneltrap>
    80205934:	6082                	ld	ra,0(sp)
    80205936:	6122                	ld	sp,8(sp)
    80205938:	61c2                	ld	gp,16(sp)
    8020593a:	7282                	ld	t0,32(sp)
    8020593c:	7322                	ld	t1,40(sp)
    8020593e:	73c2                	ld	t2,48(sp)
    80205940:	7462                	ld	s0,56(sp)
    80205942:	6486                	ld	s1,64(sp)
    80205944:	6526                	ld	a0,72(sp)
    80205946:	65c6                	ld	a1,80(sp)
    80205948:	6666                	ld	a2,88(sp)
    8020594a:	7686                	ld	a3,96(sp)
    8020594c:	7726                	ld	a4,104(sp)
    8020594e:	77c6                	ld	a5,112(sp)
    80205950:	7866                	ld	a6,120(sp)
    80205952:	688a                	ld	a7,128(sp)
    80205954:	692a                	ld	s2,136(sp)
    80205956:	69ca                	ld	s3,144(sp)
    80205958:	6a6a                	ld	s4,152(sp)
    8020595a:	7a8a                	ld	s5,160(sp)
    8020595c:	7b2a                	ld	s6,168(sp)
    8020595e:	7bca                	ld	s7,176(sp)
    80205960:	7c6a                	ld	s8,184(sp)
    80205962:	6c8e                	ld	s9,192(sp)
    80205964:	6d2e                	ld	s10,200(sp)
    80205966:	6dce                	ld	s11,208(sp)
    80205968:	6e6e                	ld	t3,216(sp)
    8020596a:	7e8e                	ld	t4,224(sp)
    8020596c:	7f2e                	ld	t5,232(sp)
    8020596e:	7fce                	ld	t6,240(sp)
    80205970:	6111                	addi	sp,sp,256
    80205972:	10200073          	sret
	...

000000008020597e <timerinit>:
#include "include/proc.h"

struct spinlock tickslock;
uint ticks;

void timerinit() {
    8020597e:	1141                	addi	sp,sp,-16
    80205980:	e406                	sd	ra,8(sp)
    80205982:	e022                	sd	s0,0(sp)
    80205984:	0800                	addi	s0,sp,16
    initlock(&tickslock, "time");
    80205986:	00004597          	auipc	a1,0x4
    8020598a:	41a58593          	addi	a1,a1,1050 # 80209da0 <digits+0xa20>
    8020598e:	0004b517          	auipc	a0,0x4b
    80205992:	cfa50513          	addi	a0,a0,-774 # 80250688 <tickslock>
    80205996:	ffffb097          	auipc	ra,0xffffb
    8020599a:	d1a080e7          	jalr	-742(ra) # 802006b0 <initlock>
    #ifdef DEBUG
    printf("timerinit\n");
    8020599e:	00004517          	auipc	a0,0x4
    802059a2:	40a50513          	addi	a0,a0,1034 # 80209da8 <digits+0xa28>
    802059a6:	ffffa097          	auipc	ra,0xffffa
    802059aa:	7e8080e7          	jalr	2024(ra) # 8020018e <printf>
    #endif
}
    802059ae:	60a2                	ld	ra,8(sp)
    802059b0:	6402                	ld	s0,0(sp)
    802059b2:	0141                	addi	sp,sp,16
    802059b4:	8082                	ret

00000000802059b6 <set_next_timeout>:

void
set_next_timeout() {
    802059b6:	1141                	addi	sp,sp,-16
    802059b8:	e422                	sd	s0,8(sp)
    802059ba:	0800                	addi	s0,sp,16
  asm volatile("rdtime %0" : "=r" (x) );
    802059bc:	c0102573          	rdtime	a0
    // if comment the `printf` line below
    // the timer will not work.

    // this bug seems to disappear automatically
    // printf("");
    sbi_set_timer(r_time() + INTERVAL);
    802059c0:	67b1                	lui	a5,0xc
    802059c2:	35078793          	addi	a5,a5,848 # c350 <_entry-0x801f3cb0>
    802059c6:	953e                	add	a0,a0,a5
	SBI_CALL_1(SBI_SET_TIMER, stime_value);
    802059c8:	4581                	li	a1,0
    802059ca:	4601                	li	a2,0
    802059cc:	4681                	li	a3,0
    802059ce:	4881                	li	a7,0
    802059d0:	00000073          	ecall
}
    802059d4:	6422                	ld	s0,8(sp)
    802059d6:	0141                	addi	sp,sp,16
    802059d8:	8082                	ret

00000000802059da <timer_tick>:

void timer_tick() {
    802059da:	1101                	addi	sp,sp,-32
    802059dc:	ec06                	sd	ra,24(sp)
    802059de:	e822                	sd	s0,16(sp)
    802059e0:	e426                	sd	s1,8(sp)
    802059e2:	1000                	addi	s0,sp,32
    acquire(&tickslock);
    802059e4:	0004b497          	auipc	s1,0x4b
    802059e8:	ca448493          	addi	s1,s1,-860 # 80250688 <tickslock>
    802059ec:	8526                	mv	a0,s1
    802059ee:	ffffb097          	auipc	ra,0xffffb
    802059f2:	d06080e7          	jalr	-762(ra) # 802006f4 <acquire>
    ticks++;
    802059f6:	4c9c                	lw	a5,24(s1)
    802059f8:	2785                	addiw	a5,a5,1
    802059fa:	cc9c                	sw	a5,24(s1)
    wakeup(&ticks);
    802059fc:	0004b517          	auipc	a0,0x4b
    80205a00:	ca450513          	addi	a0,a0,-860 # 802506a0 <ticks>
    80205a04:	ffffd097          	auipc	ra,0xffffd
    80205a08:	c34080e7          	jalr	-972(ra) # 80202638 <wakeup>
    release(&tickslock);
    80205a0c:	8526                	mv	a0,s1
    80205a0e:	ffffb097          	auipc	ra,0xffffb
    80205a12:	d3a080e7          	jalr	-710(ra) # 80200748 <release>
    set_next_timeout();
    80205a16:	00000097          	auipc	ra,0x0
    80205a1a:	fa0080e7          	jalr	-96(ra) # 802059b6 <set_next_timeout>
}
    80205a1e:	60e2                	ld	ra,24(sp)
    80205a20:	6442                	ld	s0,16(sp)
    80205a22:	64a2                	ld	s1,8(sp)
    80205a24:	6105                	addi	sp,sp,32
    80205a26:	8082                	ret

0000000080205a28 <disk_init>:
#else
#include "include/virtio.h"
#endif 

void disk_init(void)
{
    80205a28:	1141                	addi	sp,sp,-16
    80205a2a:	e406                	sd	ra,8(sp)
    80205a2c:	e022                	sd	s0,0(sp)
    80205a2e:	0800                	addi	s0,sp,16
    #ifdef QEMU
    virtio_disk_init();
    80205a30:	00002097          	auipc	ra,0x2
    80205a34:	f34080e7          	jalr	-204(ra) # 80207964 <virtio_disk_init>
	#else 
	sdcard_init();
    #endif
}
    80205a38:	60a2                	ld	ra,8(sp)
    80205a3a:	6402                	ld	s0,0(sp)
    80205a3c:	0141                	addi	sp,sp,16
    80205a3e:	8082                	ret

0000000080205a40 <disk_read>:

void disk_read(struct buf *b)
{
    80205a40:	1141                	addi	sp,sp,-16
    80205a42:	e406                	sd	ra,8(sp)
    80205a44:	e022                	sd	s0,0(sp)
    80205a46:	0800                	addi	s0,sp,16
    #ifdef QEMU
	virtio_disk_rw(b, 0);
    80205a48:	4581                	li	a1,0
    80205a4a:	00002097          	auipc	ra,0x2
    80205a4e:	0b0080e7          	jalr	176(ra) # 80207afa <virtio_disk_rw>
    #else 
	sdcard_read_sector(b->data, b->sectorno);
	#endif
}
    80205a52:	60a2                	ld	ra,8(sp)
    80205a54:	6402                	ld	s0,0(sp)
    80205a56:	0141                	addi	sp,sp,16
    80205a58:	8082                	ret

0000000080205a5a <disk_write>:

void disk_write(struct buf *b)
{
    80205a5a:	1141                	addi	sp,sp,-16
    80205a5c:	e406                	sd	ra,8(sp)
    80205a5e:	e022                	sd	s0,0(sp)
    80205a60:	0800                	addi	s0,sp,16
    #ifdef QEMU
	virtio_disk_rw(b, 1);
    80205a62:	4585                	li	a1,1
    80205a64:	00002097          	auipc	ra,0x2
    80205a68:	096080e7          	jalr	150(ra) # 80207afa <virtio_disk_rw>
    #else 
	sdcard_write_sector(b->data, b->sectorno);
	#endif
}
    80205a6c:	60a2                	ld	ra,8(sp)
    80205a6e:	6402                	ld	s0,0(sp)
    80205a70:	0141                	addi	sp,sp,16
    80205a72:	8082                	ret

0000000080205a74 <disk_intr>:

void disk_intr(void)
{
    80205a74:	1141                	addi	sp,sp,-16
    80205a76:	e406                	sd	ra,8(sp)
    80205a78:	e022                	sd	s0,0(sp)
    80205a7a:	0800                	addi	s0,sp,16
    #ifdef QEMU
    virtio_disk_intr();
    80205a7c:	00002097          	auipc	ra,0x2
    80205a80:	30a080e7          	jalr	778(ra) # 80207d86 <virtio_disk_intr>
    #else 
    dmac_intr(DMAC_CHANNEL0);
    #endif
}
    80205a84:	60a2                	ld	ra,8(sp)
    80205a86:	6402                	ld	s0,0(sp)
    80205a88:	0141                	addi	sp,sp,16
    80205a8a:	8082                	ret

0000000080205a8c <read_fat>:
/**
 * Read the FAT table content corresponded to the given cluster number.
 * @param   cluster     the number of cluster which you want to read its content in FAT table
 */
static uint32 read_fat(uint32 cluster)
{
    80205a8c:	1101                	addi	sp,sp,-32
    80205a8e:	ec06                	sd	ra,24(sp)
    80205a90:	e822                	sd	s0,16(sp)
    80205a92:	e426                	sd	s1,8(sp)
    80205a94:	e04a                	sd	s2,0(sp)
    80205a96:	1000                	addi	s0,sp,32
    if (cluster >= FAT32_EOC) {
    80205a98:	100007b7          	lui	a5,0x10000
    80205a9c:	17dd                	addi	a5,a5,-9 # ffffff7 <_entry-0x70200009>
        return cluster;
    80205a9e:	84aa                	mv	s1,a0
    if (cluster >= FAT32_EOC) {
    80205aa0:	00a7ea63          	bltu	a5,a0,80205ab4 <read_fat+0x28>
    }
    if (cluster > fat.data_clus_cnt + 1) {     // because cluster number starts at 2, not 0
    80205aa4:	0004b797          	auipc	a5,0x4b
    80205aa8:	c0c7a783          	lw	a5,-1012(a5) # 802506b0 <fat+0x8>
    80205aac:	2785                	addiw	a5,a5,1
        return 0;
    80205aae:	4481                	li	s1,0
    if (cluster > fat.data_clus_cnt + 1) {     // because cluster number starts at 2, not 0
    80205ab0:	00a7f963          	bgeu	a5,a0,80205ac2 <read_fat+0x36>
    // here should be a cache layer for FAT table, but not implemented yet.
    struct buf *b = bread(0, fat_sec);
    uint32 next_clus = *(uint32 *)(b->data + fat_offset_of_clus(cluster));
    brelse(b);
    return next_clus;
}
    80205ab4:	8526                	mv	a0,s1
    80205ab6:	60e2                	ld	ra,24(sp)
    80205ab8:	6442                	ld	s0,16(sp)
    80205aba:	64a2                	ld	s1,8(sp)
    80205abc:	6902                	ld	s2,0(sp)
    80205abe:	6105                	addi	sp,sp,32
    80205ac0:	8082                	ret
    return fat.bpb.rsvd_sec_cnt + (cluster << 2) / fat.bpb.byts_per_sec + fat.bpb.fat_sz * (fat_num - 1);
    80205ac2:	0025149b          	slliw	s1,a0,0x2
    80205ac6:	0004b917          	auipc	s2,0x4b
    80205aca:	be290913          	addi	s2,s2,-1054 # 802506a8 <fat>
    80205ace:	01095783          	lhu	a5,16(s2)
    80205ad2:	02f4d7bb          	divuw	a5,s1,a5
    80205ad6:	01495583          	lhu	a1,20(s2)
    struct buf *b = bread(0, fat_sec);
    80205ada:	9dbd                	addw	a1,a1,a5
    80205adc:	4501                	li	a0,0
    80205ade:	ffffe097          	auipc	ra,0xffffe
    80205ae2:	e5a080e7          	jalr	-422(ra) # 80203938 <bread>
    return (cluster << 2) % fat.bpb.byts_per_sec;
    80205ae6:	01095783          	lhu	a5,16(s2)
    80205aea:	02f4f4bb          	remuw	s1,s1,a5
    uint32 next_clus = *(uint32 *)(b->data + fat_offset_of_clus(cluster));
    80205aee:	1482                	slli	s1,s1,0x20
    80205af0:	9081                	srli	s1,s1,0x20
    80205af2:	009507b3          	add	a5,a0,s1
    80205af6:	4fa4                	lw	s1,88(a5)
    brelse(b);
    80205af8:	ffffe097          	auipc	ra,0xffffe
    80205afc:	f6c080e7          	jalr	-148(ra) # 80203a64 <brelse>
    return next_clus;
    80205b00:	bf55                	j	80205ab4 <read_fat+0x28>

0000000080205b02 <alloc_clus>:
        brelse(b);
    }
}

static uint32 alloc_clus(uint8 dev)
{
    80205b02:	711d                	addi	sp,sp,-96
    80205b04:	ec86                	sd	ra,88(sp)
    80205b06:	e8a2                	sd	s0,80(sp)
    80205b08:	e4a6                	sd	s1,72(sp)
    80205b0a:	e0ca                	sd	s2,64(sp)
    80205b0c:	fc4e                	sd	s3,56(sp)
    80205b0e:	f852                	sd	s4,48(sp)
    80205b10:	f456                	sd	s5,40(sp)
    80205b12:	f05a                	sd	s6,32(sp)
    80205b14:	ec5e                	sd	s7,24(sp)
    80205b16:	e862                	sd	s8,16(sp)
    80205b18:	e466                	sd	s9,8(sp)
    80205b1a:	1080                	addi	s0,sp,96
    // should we keep a free cluster list? instead of searching fat every time.
    struct buf *b;
    uint32 sec = fat.bpb.rsvd_sec_cnt;
    80205b1c:	0004b797          	auipc	a5,0x4b
    80205b20:	b8c78793          	addi	a5,a5,-1140 # 802506a8 <fat>
    80205b24:	0147db83          	lhu	s7,20(a5)
    uint32 const ent_per_sec = fat.bpb.byts_per_sec / sizeof(uint32);
    80205b28:	0107d903          	lhu	s2,16(a5)
    for (uint32 i = 0; i < fat.bpb.fat_sz; i++, sec++) {
    80205b2c:	539c                	lw	a5,32(a5)
    80205b2e:	10078363          	beqz	a5,80205c34 <alloc_clus+0x132>
    80205b32:	0029591b          	srliw	s2,s2,0x2
    80205b36:	0009099b          	sext.w	s3,s2
    80205b3a:	4b01                	li	s6,0
        b = bread(dev, sec);
    80205b3c:	00050a9b          	sext.w	s5,a0
        for (uint32 j = 0; j < ent_per_sec; j++) {
    80205b40:	4c01                	li	s8,0
    for (uint32 i = 0; i < fat.bpb.fat_sz; i++, sec++) {
    80205b42:	0004bc97          	auipc	s9,0x4b
    80205b46:	b66c8c93          	addi	s9,s9,-1178 # 802506a8 <fat>
    80205b4a:	a0c9                	j	80205c0c <alloc_clus+0x10a>
            if (((uint32 *)(b->data))[j] == 0) {
                ((uint32 *)(b->data))[j] = FAT32_EOC + 7;
    80205b4c:	100007b7          	lui	a5,0x10000
    80205b50:	17fd                	addi	a5,a5,-1 # fffffff <_entry-0x70200001>
    80205b52:	c29c                	sw	a5,0(a3)
                bwrite(b);
    80205b54:	8552                	mv	a0,s4
    80205b56:	ffffe097          	auipc	ra,0xffffe
    80205b5a:	ed2080e7          	jalr	-302(ra) # 80203a28 <bwrite>
                brelse(b);
    80205b5e:	8552                	mv	a0,s4
    80205b60:	ffffe097          	auipc	ra,0xffffe
    80205b64:	f04080e7          	jalr	-252(ra) # 80203a64 <brelse>
                uint32 clus = i * ent_per_sec + j;
    80205b68:	0369093b          	mulw	s2,s2,s6
    80205b6c:	0099093b          	addw	s2,s2,s1
    80205b70:	00090a9b          	sext.w	s5,s2
    return ((cluster - 2) * fat.bpb.sec_per_clus) + fat.first_data_sec;
    80205b74:	0004b717          	auipc	a4,0x4b
    80205b78:	b3470713          	addi	a4,a4,-1228 # 802506a8 <fat>
    80205b7c:	01274783          	lbu	a5,18(a4)
    80205b80:	ffe9099b          	addiw	s3,s2,-2
    80205b84:	02f989bb          	mulw	s3,s3,a5
    80205b88:	4318                	lw	a4,0(a4)
    80205b8a:	00e989bb          	addw	s3,s3,a4
    for (int i = 0; i < fat.bpb.sec_per_clus; i++) {
    80205b8e:	c7b1                	beqz	a5,80205bda <alloc_clus+0xd8>
    80205b90:	4901                	li	s2,0
    80205b92:	0004ba17          	auipc	s4,0x4b
    80205b96:	b16a0a13          	addi	s4,s4,-1258 # 802506a8 <fat>
        b = bread(0, sec++);
    80205b9a:	013905bb          	addw	a1,s2,s3
    80205b9e:	4501                	li	a0,0
    80205ba0:	ffffe097          	auipc	ra,0xffffe
    80205ba4:	d98080e7          	jalr	-616(ra) # 80203938 <bread>
    80205ba8:	84aa                	mv	s1,a0
        memset(b->data, 0, BSIZE);
    80205baa:	20000613          	li	a2,512
    80205bae:	4581                	li	a1,0
    80205bb0:	05850513          	addi	a0,a0,88
    80205bb4:	ffffb097          	auipc	ra,0xffffb
    80205bb8:	bdc080e7          	jalr	-1060(ra) # 80200790 <memset>
        bwrite(b);
    80205bbc:	8526                	mv	a0,s1
    80205bbe:	ffffe097          	auipc	ra,0xffffe
    80205bc2:	e6a080e7          	jalr	-406(ra) # 80203a28 <bwrite>
        brelse(b);
    80205bc6:	8526                	mv	a0,s1
    80205bc8:	ffffe097          	auipc	ra,0xffffe
    80205bcc:	e9c080e7          	jalr	-356(ra) # 80203a64 <brelse>
    for (int i = 0; i < fat.bpb.sec_per_clus; i++) {
    80205bd0:	2905                	addiw	s2,s2,1
    80205bd2:	012a4783          	lbu	a5,18(s4)
    80205bd6:	fcf942e3          	blt	s2,a5,80205b9a <alloc_clus+0x98>
            }
        }
        brelse(b);
    }
    panic("no clusters");
}
    80205bda:	8556                	mv	a0,s5
    80205bdc:	60e6                	ld	ra,88(sp)
    80205bde:	6446                	ld	s0,80(sp)
    80205be0:	64a6                	ld	s1,72(sp)
    80205be2:	6906                	ld	s2,64(sp)
    80205be4:	79e2                	ld	s3,56(sp)
    80205be6:	7a42                	ld	s4,48(sp)
    80205be8:	7aa2                	ld	s5,40(sp)
    80205bea:	7b02                	ld	s6,32(sp)
    80205bec:	6be2                	ld	s7,24(sp)
    80205bee:	6c42                	ld	s8,16(sp)
    80205bf0:	6ca2                	ld	s9,8(sp)
    80205bf2:	6125                	addi	sp,sp,96
    80205bf4:	8082                	ret
        brelse(b);
    80205bf6:	8552                	mv	a0,s4
    80205bf8:	ffffe097          	auipc	ra,0xffffe
    80205bfc:	e6c080e7          	jalr	-404(ra) # 80203a64 <brelse>
    for (uint32 i = 0; i < fat.bpb.fat_sz; i++, sec++) {
    80205c00:	2b05                	addiw	s6,s6,1
    80205c02:	2b85                	addiw	s7,s7,1 # fffffffffffff001 <ebss_clear+0xffffffff7fda7001>
    80205c04:	020ca783          	lw	a5,32(s9)
    80205c08:	02fb7663          	bgeu	s6,a5,80205c34 <alloc_clus+0x132>
        b = bread(dev, sec);
    80205c0c:	85de                	mv	a1,s7
    80205c0e:	8556                	mv	a0,s5
    80205c10:	ffffe097          	auipc	ra,0xffffe
    80205c14:	d28080e7          	jalr	-728(ra) # 80203938 <bread>
    80205c18:	8a2a                	mv	s4,a0
        for (uint32 j = 0; j < ent_per_sec; j++) {
    80205c1a:	fc098ee3          	beqz	s3,80205bf6 <alloc_clus+0xf4>
    80205c1e:	05850793          	addi	a5,a0,88
    80205c22:	84e2                	mv	s1,s8
            if (((uint32 *)(b->data))[j] == 0) {
    80205c24:	86be                	mv	a3,a5
    80205c26:	4398                	lw	a4,0(a5)
    80205c28:	d315                	beqz	a4,80205b4c <alloc_clus+0x4a>
        for (uint32 j = 0; j < ent_per_sec; j++) {
    80205c2a:	2485                	addiw	s1,s1,1
    80205c2c:	0791                	addi	a5,a5,4
    80205c2e:	fe999be3          	bne	s3,s1,80205c24 <alloc_clus+0x122>
    80205c32:	b7d1                	j	80205bf6 <alloc_clus+0xf4>
    panic("no clusters");
    80205c34:	00004517          	auipc	a0,0x4
    80205c38:	18450513          	addi	a0,a0,388 # 80209db8 <digits+0xa38>
    80205c3c:	ffffa097          	auipc	ra,0xffffa
    80205c40:	508080e7          	jalr	1288(ra) # 80200144 <panic>

0000000080205c44 <write_fat>:
    if (cluster > fat.data_clus_cnt + 1) {
    80205c44:	0004b797          	auipc	a5,0x4b
    80205c48:	a6c7a783          	lw	a5,-1428(a5) # 802506b0 <fat+0x8>
    80205c4c:	2785                	addiw	a5,a5,1
    80205c4e:	06a7e963          	bltu	a5,a0,80205cc0 <write_fat+0x7c>
{
    80205c52:	7179                	addi	sp,sp,-48
    80205c54:	f406                	sd	ra,40(sp)
    80205c56:	f022                	sd	s0,32(sp)
    80205c58:	ec26                	sd	s1,24(sp)
    80205c5a:	e84a                	sd	s2,16(sp)
    80205c5c:	e44e                	sd	s3,8(sp)
    80205c5e:	e052                	sd	s4,0(sp)
    80205c60:	1800                	addi	s0,sp,48
    80205c62:	89ae                	mv	s3,a1
    return fat.bpb.rsvd_sec_cnt + (cluster << 2) / fat.bpb.byts_per_sec + fat.bpb.fat_sz * (fat_num - 1);
    80205c64:	0025149b          	slliw	s1,a0,0x2
    80205c68:	0004ba17          	auipc	s4,0x4b
    80205c6c:	a40a0a13          	addi	s4,s4,-1472 # 802506a8 <fat>
    80205c70:	010a5783          	lhu	a5,16(s4)
    80205c74:	02f4d7bb          	divuw	a5,s1,a5
    80205c78:	014a5583          	lhu	a1,20(s4)
    struct buf *b = bread(0, fat_sec);
    80205c7c:	9dbd                	addw	a1,a1,a5
    80205c7e:	4501                	li	a0,0
    80205c80:	ffffe097          	auipc	ra,0xffffe
    80205c84:	cb8080e7          	jalr	-840(ra) # 80203938 <bread>
    80205c88:	892a                	mv	s2,a0
    return (cluster << 2) % fat.bpb.byts_per_sec;
    80205c8a:	010a5783          	lhu	a5,16(s4)
    80205c8e:	02f4f4bb          	remuw	s1,s1,a5
    *(uint32 *)(b->data + off) = content;
    80205c92:	1482                	slli	s1,s1,0x20
    80205c94:	9081                	srli	s1,s1,0x20
    80205c96:	94aa                	add	s1,s1,a0
    80205c98:	0534ac23          	sw	s3,88(s1)
    bwrite(b);
    80205c9c:	ffffe097          	auipc	ra,0xffffe
    80205ca0:	d8c080e7          	jalr	-628(ra) # 80203a28 <bwrite>
    brelse(b);
    80205ca4:	854a                	mv	a0,s2
    80205ca6:	ffffe097          	auipc	ra,0xffffe
    80205caa:	dbe080e7          	jalr	-578(ra) # 80203a64 <brelse>
    return 0;
    80205cae:	4501                	li	a0,0
}
    80205cb0:	70a2                	ld	ra,40(sp)
    80205cb2:	7402                	ld	s0,32(sp)
    80205cb4:	64e2                	ld	s1,24(sp)
    80205cb6:	6942                	ld	s2,16(sp)
    80205cb8:	69a2                	ld	s3,8(sp)
    80205cba:	6a02                	ld	s4,0(sp)
    80205cbc:	6145                	addi	sp,sp,48
    80205cbe:	8082                	ret
        return -1;
    80205cc0:	557d                	li	a0,-1
}
    80205cc2:	8082                	ret

0000000080205cc4 <reloc_clus>:
 * @param   off         the offset from the beginning of the relative file
 * @param   alloc       whether alloc new cluster when meeting end of FAT chains
 * @return              the offset from the new cur_clus
 */
static int reloc_clus(struct dirent *entry, uint off, int alloc)
{
    80205cc4:	715d                	addi	sp,sp,-80
    80205cc6:	e486                	sd	ra,72(sp)
    80205cc8:	e0a2                	sd	s0,64(sp)
    80205cca:	fc26                	sd	s1,56(sp)
    80205ccc:	f84a                	sd	s2,48(sp)
    80205cce:	f44e                	sd	s3,40(sp)
    80205cd0:	f052                	sd	s4,32(sp)
    80205cd2:	ec56                	sd	s5,24(sp)
    80205cd4:	e85a                	sd	s6,16(sp)
    80205cd6:	e45e                	sd	s7,8(sp)
    80205cd8:	0880                	addi	s0,sp,80
    80205cda:	84aa                	mv	s1,a0
    80205cdc:	8a2e                	mv	s4,a1
    int clus_num = off / fat.byts_per_clus;
    80205cde:	0004bb97          	auipc	s7,0x4b
    80205ce2:	9d6bab83          	lw	s7,-1578(s7) # 802506b4 <fat+0xc>
    80205ce6:	0375d9bb          	divuw	s3,a1,s7
    while (clus_num > entry->clus_cnt) {
    80205cea:	11052703          	lw	a4,272(a0)
    80205cee:	07377563          	bgeu	a4,s3,80205d58 <reloc_clus+0x94>
    80205cf2:	8b32                	mv	s6,a2
        int clus = read_fat(entry->cur_clus);
        if (clus >= FAT32_EOC) {
    80205cf4:	10000ab7          	lui	s5,0x10000
    80205cf8:	1add                	addi	s5,s5,-9 # ffffff7 <_entry-0x70200009>
    80205cfa:	a81d                	j	80205d30 <reloc_clus+0x6c>
            if (alloc) {
                clus = alloc_clus(entry->dev);
    80205cfc:	1144c503          	lbu	a0,276(s1)
    80205d00:	00000097          	auipc	ra,0x0
    80205d04:	e02080e7          	jalr	-510(ra) # 80205b02 <alloc_clus>
    80205d08:	0005091b          	sext.w	s2,a0
                write_fat(entry->cur_clus, clus);
    80205d0c:	85ca                	mv	a1,s2
    80205d0e:	10c4a503          	lw	a0,268(s1)
    80205d12:	00000097          	auipc	ra,0x0
    80205d16:	f32080e7          	jalr	-206(ra) # 80205c44 <write_fat>
                entry->cur_clus = entry->first_clus;
                entry->clus_cnt = 0;
                return -1;
            }
        }
        entry->cur_clus = clus;
    80205d1a:	1124a623          	sw	s2,268(s1)
        entry->clus_cnt++;
    80205d1e:	1104a783          	lw	a5,272(s1)
    80205d22:	2785                	addiw	a5,a5,1
    80205d24:	0007871b          	sext.w	a4,a5
    80205d28:	10f4a823          	sw	a5,272(s1)
    while (clus_num > entry->clus_cnt) {
    80205d2c:	03377663          	bgeu	a4,s3,80205d58 <reloc_clus+0x94>
        int clus = read_fat(entry->cur_clus);
    80205d30:	10c4a503          	lw	a0,268(s1)
    80205d34:	00000097          	auipc	ra,0x0
    80205d38:	d58080e7          	jalr	-680(ra) # 80205a8c <read_fat>
    80205d3c:	0005091b          	sext.w	s2,a0
        if (clus >= FAT32_EOC) {
    80205d40:	fd2adde3          	bge	s5,s2,80205d1a <reloc_clus+0x56>
            if (alloc) {
    80205d44:	fa0b1ce3          	bnez	s6,80205cfc <reloc_clus+0x38>
                entry->cur_clus = entry->first_clus;
    80205d48:	1044a783          	lw	a5,260(s1)
    80205d4c:	10f4a623          	sw	a5,268(s1)
                entry->clus_cnt = 0;
    80205d50:	1004a823          	sw	zero,272(s1)
                return -1;
    80205d54:	557d                	li	a0,-1
    80205d56:	a881                	j	80205da6 <reloc_clus+0xe2>
    }
    if (clus_num < entry->clus_cnt) {
    80205d58:	04e9f163          	bgeu	s3,a4,80205d9a <reloc_clus+0xd6>
        entry->cur_clus = entry->first_clus;
    80205d5c:	1044a783          	lw	a5,260(s1)
    80205d60:	10f4a623          	sw	a5,268(s1)
        entry->clus_cnt = 0;
    80205d64:	1004a823          	sw	zero,272(s1)
        while (entry->clus_cnt < clus_num) {
    80205d68:	037a6963          	bltu	s4,s7,80205d9a <reloc_clus+0xd6>
            entry->cur_clus = read_fat(entry->cur_clus);
            if (entry->cur_clus >= FAT32_EOC) {
    80205d6c:	10000937          	lui	s2,0x10000
    80205d70:	195d                	addi	s2,s2,-9 # ffffff7 <_entry-0x70200009>
            entry->cur_clus = read_fat(entry->cur_clus);
    80205d72:	10c4a503          	lw	a0,268(s1)
    80205d76:	00000097          	auipc	ra,0x0
    80205d7a:	d16080e7          	jalr	-746(ra) # 80205a8c <read_fat>
    80205d7e:	2501                	sext.w	a0,a0
    80205d80:	10a4a623          	sw	a0,268(s1)
            if (entry->cur_clus >= FAT32_EOC) {
    80205d84:	02a96c63          	bltu	s2,a0,80205dbc <reloc_clus+0xf8>
                panic("reloc_clus");
            }
            entry->clus_cnt++;
    80205d88:	1104a783          	lw	a5,272(s1)
    80205d8c:	2785                	addiw	a5,a5,1
    80205d8e:	0007871b          	sext.w	a4,a5
    80205d92:	10f4a823          	sw	a5,272(s1)
        while (entry->clus_cnt < clus_num) {
    80205d96:	fd376ee3          	bltu	a4,s3,80205d72 <reloc_clus+0xae>
        }
    }
    return off % fat.byts_per_clus;
    80205d9a:	0004b797          	auipc	a5,0x4b
    80205d9e:	91a7a783          	lw	a5,-1766(a5) # 802506b4 <fat+0xc>
    80205da2:	02fa753b          	remuw	a0,s4,a5
}
    80205da6:	60a6                	ld	ra,72(sp)
    80205da8:	6406                	ld	s0,64(sp)
    80205daa:	74e2                	ld	s1,56(sp)
    80205dac:	7942                	ld	s2,48(sp)
    80205dae:	79a2                	ld	s3,40(sp)
    80205db0:	7a02                	ld	s4,32(sp)
    80205db2:	6ae2                	ld	s5,24(sp)
    80205db4:	6b42                	ld	s6,16(sp)
    80205db6:	6ba2                	ld	s7,8(sp)
    80205db8:	6161                	addi	sp,sp,80
    80205dba:	8082                	ret
                panic("reloc_clus");
    80205dbc:	00004517          	auipc	a0,0x4
    80205dc0:	00c50513          	addi	a0,a0,12 # 80209dc8 <digits+0xa48>
    80205dc4:	ffffa097          	auipc	ra,0xffffa
    80205dc8:	380080e7          	jalr	896(ra) # 80200144 <panic>

0000000080205dcc <rw_clus>:
{
    80205dcc:	7119                	addi	sp,sp,-128
    80205dce:	fc86                	sd	ra,120(sp)
    80205dd0:	f8a2                	sd	s0,112(sp)
    80205dd2:	f4a6                	sd	s1,104(sp)
    80205dd4:	f0ca                	sd	s2,96(sp)
    80205dd6:	ecce                	sd	s3,88(sp)
    80205dd8:	e8d2                	sd	s4,80(sp)
    80205dda:	e4d6                	sd	s5,72(sp)
    80205ddc:	e0da                	sd	s6,64(sp)
    80205dde:	fc5e                	sd	s7,56(sp)
    80205de0:	f862                	sd	s8,48(sp)
    80205de2:	f466                	sd	s9,40(sp)
    80205de4:	f06a                	sd	s10,32(sp)
    80205de6:	ec6e                	sd	s11,24(sp)
    80205de8:	0100                	addi	s0,sp,128
    80205dea:	f8c43423          	sd	a2,-120(s0)
    80205dee:	8b36                	mv	s6,a3
    80205df0:	8c3e                	mv	s8,a5
    if (off + n > fat.byts_per_clus)
    80205df2:	00f706bb          	addw	a3,a4,a5
    80205df6:	0004b797          	auipc	a5,0x4b
    80205dfa:	8be7a783          	lw	a5,-1858(a5) # 802506b4 <fat+0xc>
    80205dfe:	02d7ef63          	bltu	a5,a3,80205e3c <rw_clus+0x70>
    80205e02:	8cae                	mv	s9,a1
    uint sec = first_sec_of_clus(cluster) + off / fat.bpb.byts_per_sec;
    80205e04:	0004b797          	auipc	a5,0x4b
    80205e08:	8a478793          	addi	a5,a5,-1884 # 802506a8 <fat>
    80205e0c:	0107d683          	lhu	a3,16(a5)
    return ((cluster - 2) * fat.bpb.sec_per_clus) + fat.first_data_sec;
    80205e10:	ffe5099b          	addiw	s3,a0,-2
    80205e14:	0127c603          	lbu	a2,18(a5)
    80205e18:	02c989bb          	mulw	s3,s3,a2
    80205e1c:	439c                	lw	a5,0(a5)
    80205e1e:	00f989bb          	addw	s3,s3,a5
    uint sec = first_sec_of_clus(cluster) + off / fat.bpb.byts_per_sec;
    80205e22:	02d757bb          	divuw	a5,a4,a3
    80205e26:	00f989bb          	addw	s3,s3,a5
    off = off % fat.bpb.byts_per_sec;
    80205e2a:	02d77abb          	remuw	s5,a4,a3
    for (tot = 0; tot < n; tot += m, off += m, data += m, sec++) {
    80205e2e:	0e0c0363          	beqz	s8,80205f14 <rw_clus+0x148>
    80205e32:	4a01                	li	s4,0
        m = BSIZE - off % BSIZE;
    80205e34:	20000d93          	li	s11,512
        if (bad == -1) {
    80205e38:	5d7d                	li	s10,-1
    80205e3a:	a095                	j	80205e9e <rw_clus+0xd2>
        panic("offset out of range");
    80205e3c:	00004517          	auipc	a0,0x4
    80205e40:	f9c50513          	addi	a0,a0,-100 # 80209dd8 <digits+0xa58>
    80205e44:	ffffa097          	auipc	ra,0xffffa
    80205e48:	300080e7          	jalr	768(ra) # 80200144 <panic>
                bwrite(bp);
    80205e4c:	854a                	mv	a0,s2
    80205e4e:	ffffe097          	auipc	ra,0xffffe
    80205e52:	bda080e7          	jalr	-1062(ra) # 80203a28 <bwrite>
        brelse(bp);
    80205e56:	854a                	mv	a0,s2
    80205e58:	ffffe097          	auipc	ra,0xffffe
    80205e5c:	c0c080e7          	jalr	-1012(ra) # 80203a64 <brelse>
        if (bad == -1) {
    80205e60:	a02d                	j	80205e8a <rw_clus+0xbe>
            bad = either_copyout(user, data, bp->data + (off % BSIZE), m);
    80205e62:	05890613          	addi	a2,s2,88
    80205e66:	1682                	slli	a3,a3,0x20
    80205e68:	9281                	srli	a3,a3,0x20
    80205e6a:	963a                	add	a2,a2,a4
    80205e6c:	85da                	mv	a1,s6
    80205e6e:	f8843503          	ld	a0,-120(s0)
    80205e72:	ffffd097          	auipc	ra,0xffffd
    80205e76:	8a0080e7          	jalr	-1888(ra) # 80202712 <either_copyout>
    80205e7a:	8baa                	mv	s7,a0
        brelse(bp);
    80205e7c:	854a                	mv	a0,s2
    80205e7e:	ffffe097          	auipc	ra,0xffffe
    80205e82:	be6080e7          	jalr	-1050(ra) # 80203a64 <brelse>
        if (bad == -1) {
    80205e86:	07ab8763          	beq	s7,s10,80205ef4 <rw_clus+0x128>
    for (tot = 0; tot < n; tot += m, off += m, data += m, sec++) {
    80205e8a:	01448a3b          	addw	s4,s1,s4
    80205e8e:	01548abb          	addw	s5,s1,s5
    80205e92:	1482                	slli	s1,s1,0x20
    80205e94:	9081                	srli	s1,s1,0x20
    80205e96:	9b26                	add	s6,s6,s1
    80205e98:	2985                	addiw	s3,s3,1
    80205e9a:	058a7d63          	bgeu	s4,s8,80205ef4 <rw_clus+0x128>
        bp = bread(0, sec);
    80205e9e:	85ce                	mv	a1,s3
    80205ea0:	4501                	li	a0,0
    80205ea2:	ffffe097          	auipc	ra,0xffffe
    80205ea6:	a96080e7          	jalr	-1386(ra) # 80203938 <bread>
    80205eaa:	892a                	mv	s2,a0
        m = BSIZE - off % BSIZE;
    80205eac:	1ffaf713          	andi	a4,s5,511
        if (n - tot < m) {
    80205eb0:	414c07bb          	subw	a5,s8,s4
        m = BSIZE - off % BSIZE;
    80205eb4:	40ed863b          	subw	a2,s11,a4
    80205eb8:	86be                	mv	a3,a5
    80205eba:	2781                	sext.w	a5,a5
    80205ebc:	0006059b          	sext.w	a1,a2
    80205ec0:	00f5f363          	bgeu	a1,a5,80205ec6 <rw_clus+0xfa>
    80205ec4:	86b2                	mv	a3,a2
    80205ec6:	0006849b          	sext.w	s1,a3
        if (write) {
    80205eca:	f80c8ce3          	beqz	s9,80205e62 <rw_clus+0x96>
            if ((bad = either_copyin(bp->data + (off % BSIZE), user, data, m)) != -1) {
    80205ece:	05890513          	addi	a0,s2,88
    80205ed2:	1682                	slli	a3,a3,0x20
    80205ed4:	9281                	srli	a3,a3,0x20
    80205ed6:	865a                	mv	a2,s6
    80205ed8:	f8843583          	ld	a1,-120(s0)
    80205edc:	953a                	add	a0,a0,a4
    80205ede:	ffffd097          	auipc	ra,0xffffd
    80205ee2:	86a080e7          	jalr	-1942(ra) # 80202748 <either_copyin>
    80205ee6:	f7a513e3          	bne	a0,s10,80205e4c <rw_clus+0x80>
        brelse(bp);
    80205eea:	854a                	mv	a0,s2
    80205eec:	ffffe097          	auipc	ra,0xffffe
    80205ef0:	b78080e7          	jalr	-1160(ra) # 80203a64 <brelse>
}
    80205ef4:	8552                	mv	a0,s4
    80205ef6:	70e6                	ld	ra,120(sp)
    80205ef8:	7446                	ld	s0,112(sp)
    80205efa:	74a6                	ld	s1,104(sp)
    80205efc:	7906                	ld	s2,96(sp)
    80205efe:	69e6                	ld	s3,88(sp)
    80205f00:	6a46                	ld	s4,80(sp)
    80205f02:	6aa6                	ld	s5,72(sp)
    80205f04:	6b06                	ld	s6,64(sp)
    80205f06:	7be2                	ld	s7,56(sp)
    80205f08:	7c42                	ld	s8,48(sp)
    80205f0a:	7ca2                	ld	s9,40(sp)
    80205f0c:	7d02                	ld	s10,32(sp)
    80205f0e:	6de2                	ld	s11,24(sp)
    80205f10:	6109                	addi	sp,sp,128
    80205f12:	8082                	ret
    for (tot = 0; tot < n; tot += m, off += m, data += m, sec++) {
    80205f14:	8a62                	mv	s4,s8
    80205f16:	bff9                	j	80205ef4 <rw_clus+0x128>

0000000080205f18 <eget>:
// by their whole path. But when parsing a path, we open all the directories through it, 
// which forms a linked list from the final file to the root. Thus, we use the "parent" pointer 
// to recognize whether an entry with the "name" as given is really the file we want in the right path.
// Should never get root by eget, it's easy to understand.
static struct dirent *eget(struct dirent *parent, char *name)
{
    80205f18:	7139                	addi	sp,sp,-64
    80205f1a:	fc06                	sd	ra,56(sp)
    80205f1c:	f822                	sd	s0,48(sp)
    80205f1e:	f426                	sd	s1,40(sp)
    80205f20:	f04a                	sd	s2,32(sp)
    80205f22:	ec4e                	sd	s3,24(sp)
    80205f24:	e852                	sd	s4,16(sp)
    80205f26:	e456                	sd	s5,8(sp)
    80205f28:	0080                	addi	s0,sp,64
    80205f2a:	8a2a                	mv	s4,a0
    80205f2c:	8aae                	mv	s5,a1
    struct dirent *ep;
    acquire(&ecache.lock);
    80205f2e:	0004b517          	auipc	a0,0x4b
    80205f32:	90a50513          	addi	a0,a0,-1782 # 80250838 <ecache>
    80205f36:	ffffa097          	auipc	ra,0xffffa
    80205f3a:	7be080e7          	jalr	1982(ra) # 802006f4 <acquire>
    if (name) {
    80205f3e:	060a8b63          	beqz	s5,80205fb4 <eget+0x9c>
        for (ep = root.next; ep != &root; ep = ep->next) {          // LRU algo
    80205f42:	0004b497          	auipc	s1,0x4b
    80205f46:	8b64b483          	ld	s1,-1866(s1) # 802507f8 <root+0x128>
    80205f4a:	0004a797          	auipc	a5,0x4a
    80205f4e:	78678793          	addi	a5,a5,1926 # 802506d0 <root>
    80205f52:	06f48163          	beq	s1,a5,80205fb4 <eget+0x9c>
            if (ep->valid == 1 && ep->parent == parent
    80205f56:	4905                	li	s2,1
        for (ep = root.next; ep != &root; ep = ep->next) {          // LRU algo
    80205f58:	89be                	mv	s3,a5
    80205f5a:	a029                	j	80205f64 <eget+0x4c>
    80205f5c:	1284b483          	ld	s1,296(s1)
    80205f60:	05348a63          	beq	s1,s3,80205fb4 <eget+0x9c>
            if (ep->valid == 1 && ep->parent == parent
    80205f64:	11649783          	lh	a5,278(s1)
    80205f68:	ff279ae3          	bne	a5,s2,80205f5c <eget+0x44>
    80205f6c:	1204b783          	ld	a5,288(s1)
    80205f70:	ff4796e3          	bne	a5,s4,80205f5c <eget+0x44>
                && strncmp(ep->filename, name, FAT32_MAX_FILENAME) == 0) {
    80205f74:	0ff00613          	li	a2,255
    80205f78:	85d6                	mv	a1,s5
    80205f7a:	8526                	mv	a0,s1
    80205f7c:	ffffb097          	auipc	ra,0xffffb
    80205f80:	8ec080e7          	jalr	-1812(ra) # 80200868 <strncmp>
    80205f84:	fd61                	bnez	a0,80205f5c <eget+0x44>
                if (ep->ref++ == 0) {
    80205f86:	1184a783          	lw	a5,280(s1)
    80205f8a:	0017871b          	addiw	a4,a5,1
    80205f8e:	10e4ac23          	sw	a4,280(s1)
    80205f92:	eb81                	bnez	a5,80205fa2 <eget+0x8a>
                    ep->parent->ref++;
    80205f94:	1204b703          	ld	a4,288(s1)
    80205f98:	11872783          	lw	a5,280(a4)
    80205f9c:	2785                	addiw	a5,a5,1
    80205f9e:	10f72c23          	sw	a5,280(a4)
                }
                release(&ecache.lock);
    80205fa2:	0004b517          	auipc	a0,0x4b
    80205fa6:	89650513          	addi	a0,a0,-1898 # 80250838 <ecache>
    80205faa:	ffffa097          	auipc	ra,0xffffa
    80205fae:	79e080e7          	jalr	1950(ra) # 80200748 <release>
                // edup(ep->parent);
                return ep;
    80205fb2:	a085                	j	80206012 <eget+0xfa>
            }
        }
    }
    for (ep = root.prev; ep != &root; ep = ep->prev) {              // LRU algo
    80205fb4:	0004b497          	auipc	s1,0x4b
    80205fb8:	84c4b483          	ld	s1,-1972(s1) # 80250800 <root+0x130>
    80205fbc:	0004a797          	auipc	a5,0x4a
    80205fc0:	71478793          	addi	a5,a5,1812 # 802506d0 <root>
    80205fc4:	00f48a63          	beq	s1,a5,80205fd8 <eget+0xc0>
    80205fc8:	873e                	mv	a4,a5
        if (ep->ref == 0) {
    80205fca:	1184a783          	lw	a5,280(s1)
    80205fce:	cf89                	beqz	a5,80205fe8 <eget+0xd0>
    for (ep = root.prev; ep != &root; ep = ep->prev) {              // LRU algo
    80205fd0:	1304b483          	ld	s1,304(s1)
    80205fd4:	fee49be3          	bne	s1,a4,80205fca <eget+0xb2>
            ep->dirty = 0;
            release(&ecache.lock);
            return ep;
        }
    }
    panic("eget: insufficient ecache");
    80205fd8:	00004517          	auipc	a0,0x4
    80205fdc:	e1850513          	addi	a0,a0,-488 # 80209df0 <digits+0xa70>
    80205fe0:	ffffa097          	auipc	ra,0xffffa
    80205fe4:	164080e7          	jalr	356(ra) # 80200144 <panic>
            ep->ref = 1;
    80205fe8:	4785                	li	a5,1
    80205fea:	10f4ac23          	sw	a5,280(s1)
            ep->dev = parent->dev;
    80205fee:	114a4783          	lbu	a5,276(s4)
    80205ff2:	10f48a23          	sb	a5,276(s1)
            ep->off = 0;
    80205ff6:	1004ae23          	sw	zero,284(s1)
            ep->valid = 0;
    80205ffa:	10049b23          	sh	zero,278(s1)
            ep->dirty = 0;
    80205ffe:	10048aa3          	sb	zero,277(s1)
            release(&ecache.lock);
    80206002:	0004b517          	auipc	a0,0x4b
    80206006:	83650513          	addi	a0,a0,-1994 # 80250838 <ecache>
    8020600a:	ffffa097          	auipc	ra,0xffffa
    8020600e:	73e080e7          	jalr	1854(ra) # 80200748 <release>
    return 0;
}
    80206012:	8526                	mv	a0,s1
    80206014:	70e2                	ld	ra,56(sp)
    80206016:	7442                	ld	s0,48(sp)
    80206018:	74a2                	ld	s1,40(sp)
    8020601a:	7902                	ld	s2,32(sp)
    8020601c:	69e2                	ld	s3,24(sp)
    8020601e:	6a42                	ld	s4,16(sp)
    80206020:	6aa2                	ld	s5,8(sp)
    80206022:	6121                	addi	sp,sp,64
    80206024:	8082                	ret

0000000080206026 <read_entry_name>:
 * @param   buffer      pointer to the array that stores the name
 * @param   raw_entry   pointer to the entry in a sector buffer
 * @param   islong      if non-zero, read as l-n-e, otherwise s-n-e.
 */
static void read_entry_name(char *buffer, union dentry *d)
{
    80206026:	7139                	addi	sp,sp,-64
    80206028:	fc06                	sd	ra,56(sp)
    8020602a:	f822                	sd	s0,48(sp)
    8020602c:	f426                	sd	s1,40(sp)
    8020602e:	f04a                	sd	s2,32(sp)
    80206030:	ec4e                	sd	s3,24(sp)
    80206032:	0080                	addi	s0,sp,64
    80206034:	84aa                	mv	s1,a0
    80206036:	892e                	mv	s2,a1
    if (d->lne.attr == ATTR_LONG_NAME) {                       // long entry branch
    80206038:	00b5c703          	lbu	a4,11(a1)
    8020603c:	47bd                	li	a5,15
    8020603e:	08f70563          	beq	a4,a5,802060c8 <read_entry_name+0xa2>
        snstr(buffer, d->lne.name2, NELEM(d->lne.name2));
        buffer += NELEM(d->lne.name2);
        snstr(buffer, d->lne.name3, NELEM(d->lne.name3));
    } else {
        // assert: only "." and ".." will enter this branch
        memset(buffer, 0, CHAR_SHORT_NAME + 2); // plus '.' and '\0'
    80206042:	4635                	li	a2,13
    80206044:	4581                	li	a1,0
    80206046:	ffffa097          	auipc	ra,0xffffa
    8020604a:	74a080e7          	jalr	1866(ra) # 80200790 <memset>
        int i;
        for (i = 0; d->sne.name[i] != ' ' && i < 8; i++) {
    8020604e:	00094703          	lbu	a4,0(s2)
    80206052:	02000793          	li	a5,32
    80206056:	0af70c63          	beq	a4,a5,8020610e <read_entry_name+0xe8>
    8020605a:	4785                	li	a5,1
    8020605c:	02000613          	li	a2,32
    80206060:	45a5                	li	a1,9
            buffer[i] = d->sne.name[i];
    80206062:	00f486b3          	add	a3,s1,a5
    80206066:	fee68fa3          	sb	a4,-1(a3) # 1fff <_entry-0x801fe001>
        for (i = 0; d->sne.name[i] != ' ' && i < 8; i++) {
    8020606a:	0007869b          	sext.w	a3,a5
    8020606e:	00f90733          	add	a4,s2,a5
    80206072:	00074703          	lbu	a4,0(a4)
    80206076:	00c70563          	beq	a4,a2,80206080 <read_entry_name+0x5a>
    8020607a:	0785                	addi	a5,a5,1
    8020607c:	feb793e3          	bne	a5,a1,80206062 <read_entry_name+0x3c>
        }
        if (d->sne.name[8] != ' ') {
    80206080:	00894703          	lbu	a4,8(s2)
    80206084:	02000793          	li	a5,32
    80206088:	00f70963          	beq	a4,a5,8020609a <read_entry_name+0x74>
            buffer[i++] = '.';
    8020608c:	00d487b3          	add	a5,s1,a3
    80206090:	02e00713          	li	a4,46
    80206094:	00e78023          	sb	a4,0(a5)
    80206098:	2685                	addiw	a3,a3,1
        }
        for (int j = 8; j < CHAR_SHORT_NAME; j++, i++) {
    8020609a:	00890793          	addi	a5,s2,8
    8020609e:	94b6                	add	s1,s1,a3
    802060a0:	092d                	addi	s2,s2,11
            if (d->sne.name[j] == ' ') { break; }
    802060a2:	02000693          	li	a3,32
    802060a6:	0007c703          	lbu	a4,0(a5)
    802060aa:	00d70863          	beq	a4,a3,802060ba <read_entry_name+0x94>
            buffer[i] = d->sne.name[j];
    802060ae:	00e48023          	sb	a4,0(s1)
        for (int j = 8; j < CHAR_SHORT_NAME; j++, i++) {
    802060b2:	0785                	addi	a5,a5,1
    802060b4:	0485                	addi	s1,s1,1
    802060b6:	ff2798e3          	bne	a5,s2,802060a6 <read_entry_name+0x80>
        }
    }
}
    802060ba:	70e2                	ld	ra,56(sp)
    802060bc:	7442                	ld	s0,48(sp)
    802060be:	74a2                	ld	s1,40(sp)
    802060c0:	7902                	ld	s2,32(sp)
    802060c2:	69e2                	ld	s3,24(sp)
    802060c4:	6121                	addi	sp,sp,64
    802060c6:	8082                	ret
        memmove(temp, d->lne.name1, sizeof(temp));
    802060c8:	4629                	li	a2,10
    802060ca:	0585                	addi	a1,a1,1
    802060cc:	fc040993          	addi	s3,s0,-64
    802060d0:	854e                	mv	a0,s3
    802060d2:	ffffa097          	auipc	ra,0xffffa
    802060d6:	71a080e7          	jalr	1818(ra) # 802007ec <memmove>
        snstr(buffer, temp, NELEM(d->lne.name1));
    802060da:	4615                	li	a2,5
    802060dc:	85ce                	mv	a1,s3
    802060de:	8526                	mv	a0,s1
    802060e0:	ffffb097          	auipc	ra,0xffffb
    802060e4:	890080e7          	jalr	-1904(ra) # 80200970 <snstr>
        snstr(buffer, d->lne.name2, NELEM(d->lne.name2));
    802060e8:	4619                	li	a2,6
    802060ea:	00e90593          	addi	a1,s2,14
    802060ee:	00548513          	addi	a0,s1,5
    802060f2:	ffffb097          	auipc	ra,0xffffb
    802060f6:	87e080e7          	jalr	-1922(ra) # 80200970 <snstr>
        snstr(buffer, d->lne.name3, NELEM(d->lne.name3));
    802060fa:	4609                	li	a2,2
    802060fc:	01c90593          	addi	a1,s2,28
    80206100:	00b48513          	addi	a0,s1,11
    80206104:	ffffb097          	auipc	ra,0xffffb
    80206108:	86c080e7          	jalr	-1940(ra) # 80200970 <snstr>
    8020610c:	b77d                	j	802060ba <read_entry_name+0x94>
        for (i = 0; d->sne.name[i] != ' ' && i < 8; i++) {
    8020610e:	4681                	li	a3,0
    80206110:	bf85                	j	80206080 <read_entry_name+0x5a>

0000000080206112 <fat32_init>:
{
    80206112:	7139                	addi	sp,sp,-64
    80206114:	fc06                	sd	ra,56(sp)
    80206116:	f822                	sd	s0,48(sp)
    80206118:	f426                	sd	s1,40(sp)
    8020611a:	f04a                	sd	s2,32(sp)
    8020611c:	ec4e                	sd	s3,24(sp)
    8020611e:	e852                	sd	s4,16(sp)
    80206120:	e456                	sd	s5,8(sp)
    80206122:	0080                	addi	s0,sp,64
    printf("[fat32_init] enter!\n");
    80206124:	00004517          	auipc	a0,0x4
    80206128:	cec50513          	addi	a0,a0,-788 # 80209e10 <digits+0xa90>
    8020612c:	ffffa097          	auipc	ra,0xffffa
    80206130:	062080e7          	jalr	98(ra) # 8020018e <printf>
    struct buf *b = bread(0, 0);
    80206134:	4581                	li	a1,0
    80206136:	4501                	li	a0,0
    80206138:	ffffe097          	auipc	ra,0xffffe
    8020613c:	800080e7          	jalr	-2048(ra) # 80203938 <bread>
    80206140:	892a                	mv	s2,a0
    if (strncmp((char const*)(b->data + 82), "FAT32", 5))
    80206142:	4615                	li	a2,5
    80206144:	00004597          	auipc	a1,0x4
    80206148:	ce458593          	addi	a1,a1,-796 # 80209e28 <digits+0xaa8>
    8020614c:	0aa50513          	addi	a0,a0,170
    80206150:	ffffa097          	auipc	ra,0xffffa
    80206154:	718080e7          	jalr	1816(ra) # 80200868 <strncmp>
    80206158:	1e051163          	bnez	a0,8020633a <fat32_init+0x228>
    memmove(&fat.bpb.byts_per_sec, b->data + 11, 2);            // avoid misaligned load on k210
    8020615c:	0004a497          	auipc	s1,0x4a
    80206160:	54c48493          	addi	s1,s1,1356 # 802506a8 <fat>
    80206164:	4609                	li	a2,2
    80206166:	06390593          	addi	a1,s2,99
    8020616a:	0004a517          	auipc	a0,0x4a
    8020616e:	54e50513          	addi	a0,a0,1358 # 802506b8 <fat+0x10>
    80206172:	ffffa097          	auipc	ra,0xffffa
    80206176:	67a080e7          	jalr	1658(ra) # 802007ec <memmove>
    fat.bpb.sec_per_clus = *(b->data + 13);
    8020617a:	06594683          	lbu	a3,101(s2)
    8020617e:	00d48923          	sb	a3,18(s1)
    fat.bpb.rsvd_sec_cnt = *(uint16 *)(b->data + 14);
    80206182:	06695603          	lhu	a2,102(s2)
    80206186:	00c49a23          	sh	a2,20(s1)
    fat.bpb.fat_cnt = *(b->data + 16);
    8020618a:	06894703          	lbu	a4,104(s2)
    8020618e:	00e48b23          	sb	a4,22(s1)
    fat.bpb.hidd_sec = *(uint32 *)(b->data + 28);
    80206192:	07492783          	lw	a5,116(s2)
    80206196:	cc9c                	sw	a5,24(s1)
    fat.bpb.tot_sec = *(uint32 *)(b->data + 32);
    80206198:	07892783          	lw	a5,120(s2)
    8020619c:	ccdc                	sw	a5,28(s1)
    fat.bpb.fat_sz = *(uint32 *)(b->data + 36);
    8020619e:	07c92583          	lw	a1,124(s2)
    802061a2:	d08c                	sw	a1,32(s1)
    fat.bpb.root_clus = *(uint32 *)(b->data + 44);
    802061a4:	08492503          	lw	a0,132(s2)
    802061a8:	d0c8                	sw	a0,36(s1)
    fat.first_data_sec = fat.bpb.rsvd_sec_cnt + fat.bpb.fat_cnt * fat.bpb.fat_sz;
    802061aa:	02b7073b          	mulw	a4,a4,a1
    802061ae:	9f31                	addw	a4,a4,a2
    802061b0:	c098                	sw	a4,0(s1)
    fat.data_sec_cnt = fat.bpb.tot_sec - fat.first_data_sec;
    802061b2:	9f99                	subw	a5,a5,a4
    802061b4:	c0dc                	sw	a5,4(s1)
    fat.data_clus_cnt = fat.data_sec_cnt / fat.bpb.sec_per_clus;
    802061b6:	02d7d7bb          	divuw	a5,a5,a3
    802061ba:	c49c                	sw	a5,8(s1)
    fat.byts_per_clus = fat.bpb.sec_per_clus * fat.bpb.byts_per_sec;
    802061bc:	0104d783          	lhu	a5,16(s1)
    802061c0:	02d787bb          	mulw	a5,a5,a3
    802061c4:	c4dc                	sw	a5,12(s1)
    brelse(b);
    802061c6:	854a                	mv	a0,s2
    802061c8:	ffffe097          	auipc	ra,0xffffe
    802061cc:	89c080e7          	jalr	-1892(ra) # 80203a64 <brelse>
    printf("[FAT32 init]byts_per_sec: %d\n", fat.bpb.byts_per_sec);
    802061d0:	0104d583          	lhu	a1,16(s1)
    802061d4:	00004517          	auipc	a0,0x4
    802061d8:	c7450513          	addi	a0,a0,-908 # 80209e48 <digits+0xac8>
    802061dc:	ffffa097          	auipc	ra,0xffffa
    802061e0:	fb2080e7          	jalr	-78(ra) # 8020018e <printf>
    printf("[FAT32 init]root_clus: %d\n", fat.bpb.root_clus);
    802061e4:	50cc                	lw	a1,36(s1)
    802061e6:	00004517          	auipc	a0,0x4
    802061ea:	c8250513          	addi	a0,a0,-894 # 80209e68 <digits+0xae8>
    802061ee:	ffffa097          	auipc	ra,0xffffa
    802061f2:	fa0080e7          	jalr	-96(ra) # 8020018e <printf>
    printf("[FAT32 init]sec_per_clus: %d\n", fat.bpb.sec_per_clus);
    802061f6:	0124c583          	lbu	a1,18(s1)
    802061fa:	00004517          	auipc	a0,0x4
    802061fe:	c8e50513          	addi	a0,a0,-882 # 80209e88 <digits+0xb08>
    80206202:	ffffa097          	auipc	ra,0xffffa
    80206206:	f8c080e7          	jalr	-116(ra) # 8020018e <printf>
    printf("[FAT32 init]fat_cnt: %d\n", fat.bpb.fat_cnt);
    8020620a:	0164c583          	lbu	a1,22(s1)
    8020620e:	00004517          	auipc	a0,0x4
    80206212:	c9a50513          	addi	a0,a0,-870 # 80209ea8 <digits+0xb28>
    80206216:	ffffa097          	auipc	ra,0xffffa
    8020621a:	f78080e7          	jalr	-136(ra) # 8020018e <printf>
    printf("[FAT32 init]fat_sz: %d\n", fat.bpb.fat_sz);
    8020621e:	508c                	lw	a1,32(s1)
    80206220:	00004517          	auipc	a0,0x4
    80206224:	ca850513          	addi	a0,a0,-856 # 80209ec8 <digits+0xb48>
    80206228:	ffffa097          	auipc	ra,0xffffa
    8020622c:	f66080e7          	jalr	-154(ra) # 8020018e <printf>
    printf("[FAT32 init]first_data_sec: %d\n", fat.first_data_sec);
    80206230:	408c                	lw	a1,0(s1)
    80206232:	00004517          	auipc	a0,0x4
    80206236:	cae50513          	addi	a0,a0,-850 # 80209ee0 <digits+0xb60>
    8020623a:	ffffa097          	auipc	ra,0xffffa
    8020623e:	f54080e7          	jalr	-172(ra) # 8020018e <printf>
    if (BSIZE != fat.bpb.byts_per_sec) 
    80206242:	0104d703          	lhu	a4,16(s1)
    80206246:	20000793          	li	a5,512
    8020624a:	10f71063          	bne	a4,a5,8020634a <fat32_init+0x238>
    initlock(&ecache.lock, "ecache");
    8020624e:	00004597          	auipc	a1,0x4
    80206252:	cca58593          	addi	a1,a1,-822 # 80209f18 <digits+0xb98>
    80206256:	0004a517          	auipc	a0,0x4a
    8020625a:	5e250513          	addi	a0,a0,1506 # 80250838 <ecache>
    8020625e:	ffffa097          	auipc	ra,0xffffa
    80206262:	452080e7          	jalr	1106(ra) # 802006b0 <initlock>
    memset(&root, 0, sizeof(root));
    80206266:	0004a497          	auipc	s1,0x4a
    8020626a:	44248493          	addi	s1,s1,1090 # 802506a8 <fat>
    8020626e:	0004a917          	auipc	s2,0x4a
    80206272:	46290913          	addi	s2,s2,1122 # 802506d0 <root>
    80206276:	16800613          	li	a2,360
    8020627a:	4581                	li	a1,0
    8020627c:	854a                	mv	a0,s2
    8020627e:	ffffa097          	auipc	ra,0xffffa
    80206282:	512080e7          	jalr	1298(ra) # 80200790 <memset>
    initsleeplock(&root.lock, "entry");
    80206286:	00004597          	auipc	a1,0x4
    8020628a:	c9a58593          	addi	a1,a1,-870 # 80209f20 <digits+0xba0>
    8020628e:	0004a517          	auipc	a0,0x4a
    80206292:	57a50513          	addi	a0,a0,1402 # 80250808 <root+0x138>
    80206296:	ffffe097          	auipc	ra,0xffffe
    8020629a:	8e4080e7          	jalr	-1820(ra) # 80203b7a <initsleeplock>
    root.attribute = (ATTR_DIRECTORY | ATTR_SYSTEM);
    8020629e:	47d1                	li	a5,20
    802062a0:	12f48423          	sb	a5,296(s1)
    root.first_clus = root.cur_clus = fat.bpb.root_clus;
    802062a4:	50dc                	lw	a5,36(s1)
    802062a6:	12f4aa23          	sw	a5,308(s1)
    802062aa:	12f4a623          	sw	a5,300(s1)
    root.valid = 1;
    802062ae:	4785                	li	a5,1
    802062b0:	12f49f23          	sh	a5,318(s1)
    root.prev = &root;
    802062b4:	1524bc23          	sd	s2,344(s1)
    root.next = &root;
    802062b8:	1524b823          	sd	s2,336(s1)
    for(struct dirent *de = ecache.entries; de < ecache.entries + ENTRY_CACHE_NUM; de++) {
    802062bc:	0004a497          	auipc	s1,0x4a
    802062c0:	59448493          	addi	s1,s1,1428 # 80250850 <ecache+0x18>
        de->next = root.next;
    802062c4:	0004a917          	auipc	s2,0x4a
    802062c8:	3e490913          	addi	s2,s2,996 # 802506a8 <fat>
        de->prev = &root;
    802062cc:	0004aa97          	auipc	s5,0x4a
    802062d0:	404a8a93          	addi	s5,s5,1028 # 802506d0 <root>
        initsleeplock(&de->lock, "entry");
    802062d4:	00004a17          	auipc	s4,0x4
    802062d8:	c4ca0a13          	addi	s4,s4,-948 # 80209f20 <digits+0xba0>
    for(struct dirent *de = ecache.entries; de < ecache.entries + ENTRY_CACHE_NUM; de++) {
    802062dc:	0004f997          	auipc	s3,0x4f
    802062e0:	bc498993          	addi	s3,s3,-1084 # 80254ea0 <cons>
        de->dev = 0;
    802062e4:	10048a23          	sb	zero,276(s1)
        de->valid = 0;
    802062e8:	10049b23          	sh	zero,278(s1)
        de->ref = 0;
    802062ec:	1004ac23          	sw	zero,280(s1)
        de->dirty = 0;
    802062f0:	10048aa3          	sb	zero,277(s1)
        de->parent = 0;
    802062f4:	1204b023          	sd	zero,288(s1)
        de->next = root.next;
    802062f8:	15093783          	ld	a5,336(s2)
    802062fc:	12f4b423          	sd	a5,296(s1)
        de->prev = &root;
    80206300:	1354b823          	sd	s5,304(s1)
        initsleeplock(&de->lock, "entry");
    80206304:	85d2                	mv	a1,s4
    80206306:	13848513          	addi	a0,s1,312
    8020630a:	ffffe097          	auipc	ra,0xffffe
    8020630e:	870080e7          	jalr	-1936(ra) # 80203b7a <initsleeplock>
        root.next->prev = de;
    80206312:	15093783          	ld	a5,336(s2)
    80206316:	1297b823          	sd	s1,304(a5)
        root.next = de;
    8020631a:	14993823          	sd	s1,336(s2)
    for(struct dirent *de = ecache.entries; de < ecache.entries + ENTRY_CACHE_NUM; de++) {
    8020631e:	16848493          	addi	s1,s1,360
    80206322:	fd3491e3          	bne	s1,s3,802062e4 <fat32_init+0x1d2>
}
    80206326:	4501                	li	a0,0
    80206328:	70e2                	ld	ra,56(sp)
    8020632a:	7442                	ld	s0,48(sp)
    8020632c:	74a2                	ld	s1,40(sp)
    8020632e:	7902                	ld	s2,32(sp)
    80206330:	69e2                	ld	s3,24(sp)
    80206332:	6a42                	ld	s4,16(sp)
    80206334:	6aa2                	ld	s5,8(sp)
    80206336:	6121                	addi	sp,sp,64
    80206338:	8082                	ret
        panic("not FAT32 volume");
    8020633a:	00004517          	auipc	a0,0x4
    8020633e:	af650513          	addi	a0,a0,-1290 # 80209e30 <digits+0xab0>
    80206342:	ffffa097          	auipc	ra,0xffffa
    80206346:	e02080e7          	jalr	-510(ra) # 80200144 <panic>
        panic("byts_per_sec != BSIZE");
    8020634a:	00004517          	auipc	a0,0x4
    8020634e:	bb650513          	addi	a0,a0,-1098 # 80209f00 <digits+0xb80>
    80206352:	ffffa097          	auipc	ra,0xffffa
    80206356:	df2080e7          	jalr	-526(ra) # 80200144 <panic>

000000008020635a <eread>:
    if (off > entry->file_size || off + n < off || (entry->attribute & ATTR_DIRECTORY)) {
    8020635a:	10852783          	lw	a5,264(a0)
    8020635e:	0ed7e863          	bltu	a5,a3,8020644e <eread+0xf4>
{
    80206362:	711d                	addi	sp,sp,-96
    80206364:	ec86                	sd	ra,88(sp)
    80206366:	e8a2                	sd	s0,80(sp)
    80206368:	e4a6                	sd	s1,72(sp)
    8020636a:	e0ca                	sd	s2,64(sp)
    8020636c:	fc4e                	sd	s3,56(sp)
    8020636e:	f852                	sd	s4,48(sp)
    80206370:	f456                	sd	s5,40(sp)
    80206372:	f05a                	sd	s6,32(sp)
    80206374:	ec5e                	sd	s7,24(sp)
    80206376:	e862                	sd	s8,16(sp)
    80206378:	e466                	sd	s9,8(sp)
    8020637a:	e06a                	sd	s10,0(sp)
    8020637c:	1080                	addi	s0,sp,96
    8020637e:	8a2a                	mv	s4,a0
    80206380:	8bae                	mv	s7,a1
    80206382:	8ab2                	mv	s5,a2
    80206384:	8936                	mv	s2,a3
    80206386:	8b3a                	mv	s6,a4
    if (off > entry->file_size || off + n < off || (entry->attribute & ATTR_DIRECTORY)) {
    80206388:	9eb9                	addw	a3,a3,a4
        return 0;
    8020638a:	4501                	li	a0,0
    if (off > entry->file_size || off + n < off || (entry->attribute & ATTR_DIRECTORY)) {
    8020638c:	0b26e163          	bltu	a3,s2,8020642e <eread+0xd4>
    80206390:	100a4703          	lbu	a4,256(s4)
    80206394:	8b41                	andi	a4,a4,16
    80206396:	ef41                	bnez	a4,8020642e <eread+0xd4>
    if (off + n > entry->file_size) {
    80206398:	00d7f463          	bgeu	a5,a3,802063a0 <eread+0x46>
        n = entry->file_size - off;
    8020639c:	41278b3b          	subw	s6,a5,s2
    for (tot = 0; entry->cur_clus < FAT32_EOC && tot < n; tot += m, off += m, dst += m) {
    802063a0:	10ca2703          	lw	a4,268(s4)
    802063a4:	100007b7          	lui	a5,0x10000
    802063a8:	17dd                	addi	a5,a5,-9 # ffffff7 <_entry-0x70200009>
    802063aa:	06e7ef63          	bltu	a5,a4,80206428 <eread+0xce>
    802063ae:	080b0e63          	beqz	s6,8020644a <eread+0xf0>
    802063b2:	4981                	li	s3,0
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    802063b4:	0004ac97          	auipc	s9,0x4a
    802063b8:	2f4c8c93          	addi	s9,s9,756 # 802506a8 <fat>
    for (tot = 0; entry->cur_clus < FAT32_EOC && tot < n; tot += m, off += m, dst += m) {
    802063bc:	8c3e                	mv	s8,a5
    802063be:	a82d                	j	802063f8 <eread+0x9e>
    802063c0:	00048d1b          	sext.w	s10,s1
        if (rw_clus(entry->cur_clus, 0, user_dst, dst, off % fat.byts_per_clus, m) != m) {
    802063c4:	87ea                	mv	a5,s10
    802063c6:	86d6                	mv	a3,s5
    802063c8:	865e                	mv	a2,s7
    802063ca:	4581                	li	a1,0
    802063cc:	10ca2503          	lw	a0,268(s4)
    802063d0:	00000097          	auipc	ra,0x0
    802063d4:	9fc080e7          	jalr	-1540(ra) # 80205dcc <rw_clus>
    802063d8:	2501                	sext.w	a0,a0
    802063da:	04ad1863          	bne	s10,a0,8020642a <eread+0xd0>
    for (tot = 0; entry->cur_clus < FAT32_EOC && tot < n; tot += m, off += m, dst += m) {
    802063de:	013489bb          	addw	s3,s1,s3
    802063e2:	0124893b          	addw	s2,s1,s2
    802063e6:	1482                	slli	s1,s1,0x20
    802063e8:	9081                	srli	s1,s1,0x20
    802063ea:	9aa6                	add	s5,s5,s1
    802063ec:	10ca2783          	lw	a5,268(s4)
    802063f0:	02fc6d63          	bltu	s8,a5,8020642a <eread+0xd0>
    802063f4:	0369fb63          	bgeu	s3,s6,8020642a <eread+0xd0>
        reloc_clus(entry, off, 0);
    802063f8:	4601                	li	a2,0
    802063fa:	85ca                	mv	a1,s2
    802063fc:	8552                	mv	a0,s4
    802063fe:	00000097          	auipc	ra,0x0
    80206402:	8c6080e7          	jalr	-1850(ra) # 80205cc4 <reloc_clus>
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    80206406:	00cca683          	lw	a3,12(s9)
    8020640a:	02d9763b          	remuw	a2,s2,a3
    8020640e:	0006071b          	sext.w	a4,a2
        if (n - tot < m) {
    80206412:	413b07bb          	subw	a5,s6,s3
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    80206416:	9e91                	subw	a3,a3,a2
    80206418:	84be                	mv	s1,a5
    8020641a:	2781                	sext.w	a5,a5
    8020641c:	0006861b          	sext.w	a2,a3
    80206420:	faf670e3          	bgeu	a2,a5,802063c0 <eread+0x66>
    80206424:	84b6                	mv	s1,a3
    80206426:	bf69                	j	802063c0 <eread+0x66>
    for (tot = 0; entry->cur_clus < FAT32_EOC && tot < n; tot += m, off += m, dst += m) {
    80206428:	4981                	li	s3,0
    return tot;
    8020642a:	0009851b          	sext.w	a0,s3
}
    8020642e:	60e6                	ld	ra,88(sp)
    80206430:	6446                	ld	s0,80(sp)
    80206432:	64a6                	ld	s1,72(sp)
    80206434:	6906                	ld	s2,64(sp)
    80206436:	79e2                	ld	s3,56(sp)
    80206438:	7a42                	ld	s4,48(sp)
    8020643a:	7aa2                	ld	s5,40(sp)
    8020643c:	7b02                	ld	s6,32(sp)
    8020643e:	6be2                	ld	s7,24(sp)
    80206440:	6c42                	ld	s8,16(sp)
    80206442:	6ca2                	ld	s9,8(sp)
    80206444:	6d02                	ld	s10,0(sp)
    80206446:	6125                	addi	sp,sp,96
    80206448:	8082                	ret
    for (tot = 0; entry->cur_clus < FAT32_EOC && tot < n; tot += m, off += m, dst += m) {
    8020644a:	89da                	mv	s3,s6
    8020644c:	bff9                	j	8020642a <eread+0xd0>
        return 0;
    8020644e:	4501                	li	a0,0
}
    80206450:	8082                	ret

0000000080206452 <ewrite>:
    if (off > entry->file_size || off + n < off || (uint64)off + n > 0xffffffff
    80206452:	10852783          	lw	a5,264(a0)
    80206456:	0ed7e463          	bltu	a5,a3,8020653e <ewrite+0xec>
{
    8020645a:	711d                	addi	sp,sp,-96
    8020645c:	ec86                	sd	ra,88(sp)
    8020645e:	e8a2                	sd	s0,80(sp)
    80206460:	e4a6                	sd	s1,72(sp)
    80206462:	e0ca                	sd	s2,64(sp)
    80206464:	fc4e                	sd	s3,56(sp)
    80206466:	f852                	sd	s4,48(sp)
    80206468:	f456                	sd	s5,40(sp)
    8020646a:	f05a                	sd	s6,32(sp)
    8020646c:	ec5e                	sd	s7,24(sp)
    8020646e:	e862                	sd	s8,16(sp)
    80206470:	e466                	sd	s9,8(sp)
    80206472:	1080                	addi	s0,sp,96
    80206474:	8aaa                	mv	s5,a0
    80206476:	8bae                	mv	s7,a1
    80206478:	8a32                	mv	s4,a2
    8020647a:	8936                	mv	s2,a3
    8020647c:	8b3a                	mv	s6,a4
    if (off > entry->file_size || off + n < off || (uint64)off + n > 0xffffffff
    8020647e:	00e687bb          	addw	a5,a3,a4
    80206482:	0cd7e063          	bltu	a5,a3,80206542 <ewrite+0xf0>
    80206486:	02069793          	slli	a5,a3,0x20
    8020648a:	9381                	srli	a5,a5,0x20
    8020648c:	1702                	slli	a4,a4,0x20
    8020648e:	9301                	srli	a4,a4,0x20
    80206490:	97ba                	add	a5,a5,a4
    80206492:	577d                	li	a4,-1
    80206494:	9301                	srli	a4,a4,0x20
    80206496:	0af76863          	bltu	a4,a5,80206546 <ewrite+0xf4>
        || (entry->attribute & ATTR_READ_ONLY)) {
    8020649a:	10054783          	lbu	a5,256(a0)
    8020649e:	8b85                	andi	a5,a5,1
    802064a0:	e7cd                	bnez	a5,8020654a <ewrite+0xf8>
    if (entry->first_clus == 0) {   // so file_size if 0 too, which requests off == 0
    802064a2:	10452783          	lw	a5,260(a0)
    802064a6:	cb89                	beqz	a5,802064b8 <ewrite+0x66>
    for (tot = 0; tot < n; tot += m, off += m, src += m) {
    802064a8:	080b0963          	beqz	s6,8020653a <ewrite+0xe8>
    802064ac:	4981                	li	s3,0
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    802064ae:	0004ac17          	auipc	s8,0x4a
    802064b2:	1fac0c13          	addi	s8,s8,506 # 802506a8 <fat>
    802064b6:	a891                	j	8020650a <ewrite+0xb8>
        entry->cur_clus = entry->first_clus = alloc_clus(entry->dev);
    802064b8:	11454503          	lbu	a0,276(a0)
    802064bc:	fffff097          	auipc	ra,0xfffff
    802064c0:	646080e7          	jalr	1606(ra) # 80205b02 <alloc_clus>
    802064c4:	2501                	sext.w	a0,a0
    802064c6:	10aaa223          	sw	a0,260(s5)
    802064ca:	10aaa623          	sw	a0,268(s5)
        entry->clus_cnt = 0;
    802064ce:	100aa823          	sw	zero,272(s5)
        entry->dirty = 1;
    802064d2:	4785                	li	a5,1
    802064d4:	10fa8aa3          	sb	a5,277(s5)
    802064d8:	bfc1                	j	802064a8 <ewrite+0x56>
    802064da:	00048c9b          	sext.w	s9,s1
        if (rw_clus(entry->cur_clus, 1, user_src, src, off % fat.byts_per_clus, m) != m) {
    802064de:	87e6                	mv	a5,s9
    802064e0:	86d2                	mv	a3,s4
    802064e2:	865e                	mv	a2,s7
    802064e4:	4585                	li	a1,1
    802064e6:	10caa503          	lw	a0,268(s5)
    802064ea:	00000097          	auipc	ra,0x0
    802064ee:	8e2080e7          	jalr	-1822(ra) # 80205dcc <rw_clus>
    802064f2:	2501                	sext.w	a0,a0
    802064f4:	04ac9d63          	bne	s9,a0,8020654e <ewrite+0xfc>
    for (tot = 0; tot < n; tot += m, off += m, src += m) {
    802064f8:	013489bb          	addw	s3,s1,s3
    802064fc:	0124893b          	addw	s2,s1,s2
    80206500:	1482                	slli	s1,s1,0x20
    80206502:	9081                	srli	s1,s1,0x20
    80206504:	9a26                	add	s4,s4,s1
    80206506:	0569f463          	bgeu	s3,s6,8020654e <ewrite+0xfc>
        reloc_clus(entry, off, 1);
    8020650a:	4605                	li	a2,1
    8020650c:	85ca                	mv	a1,s2
    8020650e:	8556                	mv	a0,s5
    80206510:	fffff097          	auipc	ra,0xfffff
    80206514:	7b4080e7          	jalr	1972(ra) # 80205cc4 <reloc_clus>
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    80206518:	00cc2683          	lw	a3,12(s8)
    8020651c:	02d9763b          	remuw	a2,s2,a3
    80206520:	0006071b          	sext.w	a4,a2
        if (n - tot < m) {
    80206524:	413b07bb          	subw	a5,s6,s3
        m = fat.byts_per_clus - off % fat.byts_per_clus;
    80206528:	9e91                	subw	a3,a3,a2
    8020652a:	84be                	mv	s1,a5
    8020652c:	2781                	sext.w	a5,a5
    8020652e:	0006861b          	sext.w	a2,a3
    80206532:	faf674e3          	bgeu	a2,a5,802064da <ewrite+0x88>
    80206536:	84b6                	mv	s1,a3
    80206538:	b74d                	j	802064da <ewrite+0x88>
    for (tot = 0; tot < n; tot += m, off += m, src += m) {
    8020653a:	89da                	mv	s3,s6
    8020653c:	a015                	j	80206560 <ewrite+0x10e>
        return -1;
    8020653e:	557d                	li	a0,-1
}
    80206540:	8082                	ret
        return -1;
    80206542:	557d                	li	a0,-1
    80206544:	a005                	j	80206564 <ewrite+0x112>
    80206546:	557d                	li	a0,-1
    80206548:	a831                	j	80206564 <ewrite+0x112>
    8020654a:	557d                	li	a0,-1
    8020654c:	a821                	j	80206564 <ewrite+0x112>
        if(off > entry->file_size) {
    8020654e:	108aa783          	lw	a5,264(s5)
    80206552:	0127f763          	bgeu	a5,s2,80206560 <ewrite+0x10e>
            entry->file_size = off;
    80206556:	112aa423          	sw	s2,264(s5)
            entry->dirty = 1;
    8020655a:	4785                	li	a5,1
    8020655c:	10fa8aa3          	sb	a5,277(s5)
    return tot;
    80206560:	0009851b          	sext.w	a0,s3
}
    80206564:	60e6                	ld	ra,88(sp)
    80206566:	6446                	ld	s0,80(sp)
    80206568:	64a6                	ld	s1,72(sp)
    8020656a:	6906                	ld	s2,64(sp)
    8020656c:	79e2                	ld	s3,56(sp)
    8020656e:	7a42                	ld	s4,48(sp)
    80206570:	7aa2                	ld	s5,40(sp)
    80206572:	7b02                	ld	s6,32(sp)
    80206574:	6be2                	ld	s7,24(sp)
    80206576:	6c42                	ld	s8,16(sp)
    80206578:	6ca2                	ld	s9,8(sp)
    8020657a:	6125                	addi	sp,sp,96
    8020657c:	8082                	ret

000000008020657e <formatname>:
{
    8020657e:	7179                	addi	sp,sp,-48
    80206580:	f406                	sd	ra,40(sp)
    80206582:	f022                	sd	s0,32(sp)
    80206584:	ec26                	sd	s1,24(sp)
    80206586:	e84a                	sd	s2,16(sp)
    80206588:	e44e                	sd	s3,8(sp)
    8020658a:	e052                	sd	s4,0(sp)
    8020658c:	1800                	addi	s0,sp,48
    8020658e:	84aa                	mv	s1,a0
    while (*name == ' ' || *name == '.') { name++; }
    80206590:	02000793          	li	a5,32
    80206594:	02e00713          	li	a4,46
    80206598:	a011                	j	8020659c <formatname+0x1e>
    8020659a:	0485                	addi	s1,s1,1
    8020659c:	0004c583          	lbu	a1,0(s1)
    802065a0:	fef58de3          	beq	a1,a5,8020659a <formatname+0x1c>
    802065a4:	fee58be3          	beq	a1,a4,8020659a <formatname+0x1c>
    for (p = name; *p; p++) {
    802065a8:	c1b9                	beqz	a1,802065ee <formatname+0x70>
    802065aa:	8926                	mv	s2,s1
        if (c < 0x20 || strchr(illegal, c)) {
    802065ac:	49fd                	li	s3,31
    802065ae:	0002da17          	auipc	s4,0x2d
    802065b2:	d12a0a13          	addi	s4,s4,-750 # 802332c0 <illegal.1>
    802065b6:	02b9fe63          	bgeu	s3,a1,802065f2 <formatname+0x74>
    802065ba:	8552                	mv	a0,s4
    802065bc:	ffffa097          	auipc	ra,0xffffa
    802065c0:	432080e7          	jalr	1074(ra) # 802009ee <strchr>
    802065c4:	e121                	bnez	a0,80206604 <formatname+0x86>
    for (p = name; *p; p++) {
    802065c6:	0905                	addi	s2,s2,1
    802065c8:	00094583          	lbu	a1,0(s2)
    802065cc:	f5ed                	bnez	a1,802065b6 <formatname+0x38>
        if (*p != ' ') {
    802065ce:	02000693          	li	a3,32
    802065d2:	874a                	mv	a4,s2
    while (p-- > name) {
    802065d4:	0124fb63          	bgeu	s1,s2,802065ea <formatname+0x6c>
        if (*p != ' ') {
    802065d8:	197d                	addi	s2,s2,-1
    802065da:	00094783          	lbu	a5,0(s2)
    802065de:	fed78ae3          	beq	a5,a3,802065d2 <formatname+0x54>
            p[1] = '\0';
    802065e2:	00070023          	sb	zero,0(a4)
            break;
    802065e6:	8526                	mv	a0,s1
    802065e8:	a031                	j	802065f4 <formatname+0x76>
    802065ea:	8526                	mv	a0,s1
    802065ec:	a021                	j	802065f4 <formatname+0x76>
    for (p = name; *p; p++) {
    802065ee:	8526                	mv	a0,s1
    802065f0:	a011                	j	802065f4 <formatname+0x76>
            return 0;
    802065f2:	4501                	li	a0,0
}
    802065f4:	70a2                	ld	ra,40(sp)
    802065f6:	7402                	ld	s0,32(sp)
    802065f8:	64e2                	ld	s1,24(sp)
    802065fa:	6942                	ld	s2,16(sp)
    802065fc:	69a2                	ld	s3,8(sp)
    802065fe:	6a02                	ld	s4,0(sp)
    80206600:	6145                	addi	sp,sp,48
    80206602:	8082                	ret
            return 0;
    80206604:	4501                	li	a0,0
    80206606:	b7fd                	j	802065f4 <formatname+0x76>

0000000080206608 <cal_checksum>:
{
    80206608:	1141                	addi	sp,sp,-16
    8020660a:	e422                	sd	s0,8(sp)
    8020660c:	0800                	addi	s0,sp,16
    8020660e:	87aa                	mv	a5,a0
    for (int i = CHAR_SHORT_NAME; i != 0; i--) {
    80206610:	00b50613          	addi	a2,a0,11
    uint8 sum = 0;
    80206614:	4501                	li	a0,0
        sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *shortname++;
    80206616:	0075171b          	slliw	a4,a0,0x7
    8020661a:	0785                	addi	a5,a5,1
    8020661c:	0015551b          	srliw	a0,a0,0x1
    80206620:	fff7c683          	lbu	a3,-1(a5)
    80206624:	9d35                	addw	a0,a0,a3
    80206626:	953a                	add	a0,a0,a4
    80206628:	0ff57513          	zext.b	a0,a0
    for (int i = CHAR_SHORT_NAME; i != 0; i--) {
    8020662c:	fef615e3          	bne	a2,a5,80206616 <cal_checksum+0xe>
}
    80206630:	6422                	ld	s0,8(sp)
    80206632:	0141                	addi	sp,sp,16
    80206634:	8082                	ret

0000000080206636 <emake>:
{
    80206636:	7131                	addi	sp,sp,-192
    80206638:	fd06                	sd	ra,184(sp)
    8020663a:	f922                	sd	s0,176(sp)
    8020663c:	f526                	sd	s1,168(sp)
    8020663e:	f14a                	sd	s2,160(sp)
    80206640:	ed4e                	sd	s3,152(sp)
    80206642:	e952                	sd	s4,144(sp)
    80206644:	e556                	sd	s5,136(sp)
    80206646:	e15a                	sd	s6,128(sp)
    80206648:	fcde                	sd	s7,120(sp)
    8020664a:	f8e2                	sd	s8,112(sp)
    8020664c:	f4e6                	sd	s9,104(sp)
    8020664e:	f0ea                	sd	s10,96(sp)
    80206650:	ecee                	sd	s11,88(sp)
    80206652:	0180                	addi	s0,sp,192
    if (!(dp->attribute & ATTR_DIRECTORY))
    80206654:	10054783          	lbu	a5,256(a0)
    80206658:	8bc1                	andi	a5,a5,16
    8020665a:	c3d5                	beqz	a5,802066fe <emake+0xc8>
    8020665c:	8b2a                	mv	s6,a0
    8020665e:	8d2e                	mv	s10,a1
    80206660:	8cb2                	mv	s9,a2
    if (off % sizeof(union dentry))
    80206662:	01f67793          	andi	a5,a2,31
    80206666:	e7c5                	bnez	a5,8020670e <emake+0xd8>
    memset(&de, 0, sizeof(de));
    80206668:	02000613          	li	a2,32
    8020666c:	4581                	li	a1,0
    8020666e:	f7040513          	addi	a0,s0,-144
    80206672:	ffffa097          	auipc	ra,0xffffa
    80206676:	11e080e7          	jalr	286(ra) # 80200790 <memset>
    if (off <= 32) {
    8020667a:	02000793          	li	a5,32
    8020667e:	0b97ec63          	bltu	a5,s9,80206736 <emake+0x100>
        if (off == 0) {
    80206682:	080c9e63          	bnez	s9,8020671e <emake+0xe8>
            strncpy(de.sne.name, ".          ", sizeof(de.sne.name));
    80206686:	462d                	li	a2,11
    80206688:	00004597          	auipc	a1,0x4
    8020668c:	8c858593          	addi	a1,a1,-1848 # 80209f50 <digits+0xbd0>
    80206690:	f7040513          	addi	a0,s0,-144
    80206694:	ffffa097          	auipc	ra,0xffffa
    80206698:	210080e7          	jalr	528(ra) # 802008a4 <strncpy>
        de.sne.attr = ATTR_DIRECTORY;
    8020669c:	47c1                	li	a5,16
    8020669e:	f6f40da3          	sb	a5,-133(s0)
        de.sne.fst_clus_hi = (uint16)(ep->first_clus >> 16);        // first clus high 16 bits
    802066a2:	104d2783          	lw	a5,260(s10) # fffffffffffff104 <ebss_clear+0xffffffff7fda7104>
    802066a6:	0107d71b          	srliw	a4,a5,0x10
    802066aa:	f8e41223          	sh	a4,-124(s0)
        de.sne.fst_clus_lo = (uint16)(ep->first_clus & 0xffff);       // low 16 bits
    802066ae:	f8f41523          	sh	a5,-118(s0)
        de.sne.file_size = 0;                                       // filesize is updated in eupdate()
    802066b2:	f8042623          	sw	zero,-116(s0)
        off = reloc_clus(dp, off, 1);
    802066b6:	4605                	li	a2,1
    802066b8:	85e6                	mv	a1,s9
    802066ba:	855a                	mv	a0,s6
    802066bc:	fffff097          	auipc	ra,0xfffff
    802066c0:	608080e7          	jalr	1544(ra) # 80205cc4 <reloc_clus>
        rw_clus(dp->cur_clus, 1, 0, (uint64)&de, off, sizeof(de));
    802066c4:	02000793          	li	a5,32
    802066c8:	0005071b          	sext.w	a4,a0
    802066cc:	f7040693          	addi	a3,s0,-144
    802066d0:	4601                	li	a2,0
    802066d2:	4585                	li	a1,1
    802066d4:	10cb2503          	lw	a0,268(s6)
    802066d8:	fffff097          	auipc	ra,0xfffff
    802066dc:	6f4080e7          	jalr	1780(ra) # 80205dcc <rw_clus>
}
    802066e0:	70ea                	ld	ra,184(sp)
    802066e2:	744a                	ld	s0,176(sp)
    802066e4:	74aa                	ld	s1,168(sp)
    802066e6:	790a                	ld	s2,160(sp)
    802066e8:	69ea                	ld	s3,152(sp)
    802066ea:	6a4a                	ld	s4,144(sp)
    802066ec:	6aaa                	ld	s5,136(sp)
    802066ee:	6b0a                	ld	s6,128(sp)
    802066f0:	7be6                	ld	s7,120(sp)
    802066f2:	7c46                	ld	s8,112(sp)
    802066f4:	7ca6                	ld	s9,104(sp)
    802066f6:	7d06                	ld	s10,96(sp)
    802066f8:	6de6                	ld	s11,88(sp)
    802066fa:	6129                	addi	sp,sp,192
    802066fc:	8082                	ret
        panic("emake: not dir");
    802066fe:	00004517          	auipc	a0,0x4
    80206702:	82a50513          	addi	a0,a0,-2006 # 80209f28 <digits+0xba8>
    80206706:	ffffa097          	auipc	ra,0xffffa
    8020670a:	a3e080e7          	jalr	-1474(ra) # 80200144 <panic>
        panic("emake: not aligned");
    8020670e:	00004517          	auipc	a0,0x4
    80206712:	82a50513          	addi	a0,a0,-2006 # 80209f38 <digits+0xbb8>
    80206716:	ffffa097          	auipc	ra,0xffffa
    8020671a:	a2e080e7          	jalr	-1490(ra) # 80200144 <panic>
            strncpy(de.sne.name, "..         ", sizeof(de.sne.name));
    8020671e:	462d                	li	a2,11
    80206720:	00004597          	auipc	a1,0x4
    80206724:	84058593          	addi	a1,a1,-1984 # 80209f60 <digits+0xbe0>
    80206728:	f7040513          	addi	a0,s0,-144
    8020672c:	ffffa097          	auipc	ra,0xffffa
    80206730:	178080e7          	jalr	376(ra) # 802008a4 <strncpy>
    80206734:	b7a5                	j	8020669c <emake+0x66>
        int entcnt = (strlen(ep->filename) + CHAR_LONG_NAME - 1) / CHAR_LONG_NAME;   // count of l-n-entries, rounds up
    80206736:	896a                	mv	s2,s10
    80206738:	856a                	mv	a0,s10
    8020673a:	ffffa097          	auipc	ra,0xffffa
    8020673e:	1da080e7          	jalr	474(ra) # 80200914 <strlen>
    80206742:	f4a43423          	sd	a0,-184(s0)
    80206746:	00c5071b          	addiw	a4,a0,12
    8020674a:	47b5                	li	a5,13
    8020674c:	02f747bb          	divw	a5,a4,a5
    80206750:	f4f42c23          	sw	a5,-168(s0)
    80206754:	00078d9b          	sext.w	s11,a5
        memset(shortname, 0, sizeof(shortname));
    80206758:	4631                	li	a2,12
    8020675a:	4581                	li	a1,0
    8020675c:	f6040513          	addi	a0,s0,-160
    80206760:	ffffa097          	auipc	ra,0xffffa
    80206764:	030080e7          	jalr	48(ra) # 80200790 <memset>
    for (int j = strlen(name) - 1; j >= 0; j--) {
    80206768:	856a                	mv	a0,s10
    8020676a:	ffffa097          	auipc	ra,0xffffa
    8020676e:	1aa080e7          	jalr	426(ra) # 80200914 <strlen>
    80206772:	fff5079b          	addiw	a5,a0,-1
    80206776:	0207cf63          	bltz	a5,802067b4 <emake+0x17e>
    8020677a:	97ea                	add	a5,a5,s10
    8020677c:	ffed0693          	addi	a3,s10,-2
    80206780:	96aa                	add	a3,a3,a0
    80206782:	fff5071b          	addiw	a4,a0,-1
    80206786:	1702                	slli	a4,a4,0x20
    80206788:	9301                	srli	a4,a4,0x20
    8020678a:	8e99                	sub	a3,a3,a4
        if (name[j] == '.') {
    8020678c:	02e00613          	li	a2,46
    80206790:	89be                	mv	s3,a5
    80206792:	0007c703          	lbu	a4,0(a5)
    80206796:	00c70663          	beq	a4,a2,802067a2 <emake+0x16c>
    for (int j = strlen(name) - 1; j >= 0; j--) {
    8020679a:	17fd                	addi	a5,a5,-1
    8020679c:	fed79ae3          	bne	a5,a3,80206790 <emake+0x15a>
    char c, *p = name;
    802067a0:	89ea                	mv	s3,s10
        shortname[i++] = c;
    802067a2:	4481                	li	s1,0
        if (i == 8 && p) {
    802067a4:	4aa1                	li	s5,8
    while (i < CHAR_SHORT_NAME && (c = *name++)) {
    802067a6:	4ba9                	li	s7,10
        if (c >= 'a' && c <= 'z') {
    802067a8:	4c65                	li	s8,25
                memset(shortname + i, ' ', 8 - i);
    802067aa:	f6040793          	addi	a5,s0,-160
    802067ae:	f4f43823          	sd	a5,-176(s0)
    802067b2:	a8d9                	j	80206888 <emake+0x252>
    char c, *p = name;
    802067b4:	89ea                	mv	s3,s10
    802067b6:	b7f5                	j	802067a2 <emake+0x16c>
        if (i == 8 && p) {
    802067b8:	22098b63          	beqz	s3,802069ee <emake+0x3b8>
            if (p + 1 < name) { break; }            // no '.'
    802067bc:	0985                	addi	s3,s3,1
    802067be:	0949fc63          	bgeu	s3,s4,80206856 <emake+0x220>
    802067c2:	f6040793          	addi	a5,s0,-160
    802067c6:	97a6                	add	a5,a5,s1
        shortname[i++] = ' ';
    802067c8:	02000693          	li	a3,32
    while (i < CHAR_SHORT_NAME) {
    802067cc:	4729                	li	a4,10
        shortname[i++] = ' ';
    802067ce:	2485                	addiw	s1,s1,1
    802067d0:	00d78023          	sb	a3,0(a5)
    while (i < CHAR_SHORT_NAME) {
    802067d4:	0785                	addi	a5,a5,1
    802067d6:	fe975ce3          	bge	a4,s1,802067ce <emake+0x198>
        de.lne.checksum = cal_checksum((uchar *)shortname);
    802067da:	f6040513          	addi	a0,s0,-160
    802067de:	00000097          	auipc	ra,0x0
    802067e2:	e2a080e7          	jalr	-470(ra) # 80206608 <cal_checksum>
    802067e6:	f6a40ea3          	sb	a0,-131(s0)
        de.lne.attr = ATTR_LONG_NAME;
    802067ea:	47bd                	li	a5,15
    802067ec:	f6f40da3          	sb	a5,-133(s0)
        for (int i = entcnt; i > 0; i--) {
    802067f0:	f4843783          	ld	a5,-184(s0)
    802067f4:	14f05c63          	blez	a5,8020694c <emake+0x316>
    802067f8:	f5842783          	lw	a5,-168(s0)
    802067fc:	37fd                	addiw	a5,a5,-1
    802067fe:	f4f42223          	sw	a5,-188(s0)
    80206802:	00179b9b          	slliw	s7,a5,0x1
    80206806:	00fb8bbb          	addw	s7,s7,a5
    8020680a:	002b9b9b          	slliw	s7,s7,0x2
    8020680e:	00fb8bbb          	addw	s7,s7,a5
    80206812:	9bea                	add	s7,s7,s10
    80206814:	8aee                	mv	s5,s11
    80206816:	8c66                	mv	s8,s9
            int end = 0;
    80206818:	4981                	li	s3,0
            uint8 *w = (uint8 *)de.lne.name1;
    8020681a:	f7140793          	addi	a5,s0,-143
    8020681e:	f4f43823          	sd	a5,-176(s0)
    80206822:	0ff00913          	li	s2,255
                    case 5:     w = (uint8 *)de.lne.name2; break;
    80206826:	f7e40793          	addi	a5,s0,-130
    8020682a:	f4f43c23          	sd	a5,-168(s0)
                switch (j) {
    8020682e:	44ad                	li	s1,11
    80206830:	f8c40a13          	addi	s4,s0,-116
    80206834:	a0c5                	j	80206914 <emake+0x2de>
            if (name > p) {                    // last '.'
    80206836:	0549f663          	bgeu	s3,s4,80206882 <emake+0x24c>
                memset(shortname + i, ' ', 8 - i);
    8020683a:	47a1                	li	a5,8
    8020683c:	4097863b          	subw	a2,a5,s1
    80206840:	02000593          	li	a1,32
    80206844:	f5043783          	ld	a5,-176(s0)
    80206848:	00978533          	add	a0,a5,s1
    8020684c:	ffffa097          	auipc	ra,0xffffa
    80206850:	f44080e7          	jalr	-188(ra) # 80200790 <memset>
    while (i < CHAR_SHORT_NAME && (c = *name++)) {
    80206854:	89d2                	mv	s3,s4
        shortname[i++] = c;
    80206856:	8a4e                	mv	s4,s3
    80206858:	84d6                	mv	s1,s5
    8020685a:	4981                	li	s3,0
    8020685c:	a02d                	j	80206886 <emake+0x250>
            if (strchr(illegal, c) != NULL) {
    8020685e:	85ca                	mv	a1,s2
    80206860:	0002d517          	auipc	a0,0x2d
    80206864:	a7050513          	addi	a0,a0,-1424 # 802332d0 <illegal.0>
    80206868:	ffffa097          	auipc	ra,0xffffa
    8020686c:	186080e7          	jalr	390(ra) # 802009ee <strchr>
    80206870:	c119                	beqz	a0,80206876 <emake+0x240>
                c = '_';
    80206872:	05f00913          	li	s2,95
        shortname[i++] = c;
    80206876:	f9048793          	addi	a5,s1,-112
    8020687a:	97a2                	add	a5,a5,s0
    8020687c:	fd278823          	sb	s2,-48(a5)
    80206880:	2485                	addiw	s1,s1,1
    while (i < CHAR_SHORT_NAME && (c = *name++)) {
    80206882:	f49bcce3          	blt	s7,s1,802067da <emake+0x1a4>
        shortname[i++] = c;
    80206886:	8952                	mv	s2,s4
    while (i < CHAR_SHORT_NAME && (c = *name++)) {
    80206888:	00190a13          	addi	s4,s2,1
    8020688c:	00094903          	lbu	s2,0(s2)
    80206890:	02090663          	beqz	s2,802068bc <emake+0x286>
        if (i == 8 && p) {
    80206894:	f35482e3          	beq	s1,s5,802067b8 <emake+0x182>
        if (c == ' ') { continue; }
    80206898:	02000793          	li	a5,32
    8020689c:	fef903e3          	beq	s2,a5,80206882 <emake+0x24c>
        if (c == '.') {
    802068a0:	02e00793          	li	a5,46
    802068a4:	f8f909e3          	beq	s2,a5,80206836 <emake+0x200>
        if (c >= 'a' && c <= 'z') {
    802068a8:	f9f9079b          	addiw	a5,s2,-97
    802068ac:	0ff7f793          	zext.b	a5,a5
    802068b0:	fafc67e3          	bltu	s8,a5,8020685e <emake+0x228>
            c += 'A' - 'a';
    802068b4:	3901                	addiw	s2,s2,-32
    802068b6:	0ff97913          	zext.b	s2,s2
    802068ba:	bf75                	j	80206876 <emake+0x240>
    while (i < CHAR_SHORT_NAME) {
    802068bc:	47a9                	li	a5,10
    802068be:	f097d2e3          	bge	a5,s1,802067c2 <emake+0x18c>
    802068c2:	bf21                	j	802067da <emake+0x1a4>
                de.lne.order |= LAST_LONG_ENTRY;
    802068c4:	0407e793          	ori	a5,a5,64
    802068c8:	f6f40823          	sb	a5,-144(s0)
    802068cc:	a891                	j	80206920 <emake+0x2ea>
                    if ((*w++ = *p++) == 0) {
    802068ce:	00084603          	lbu	a2,0(a6)
                        end = 1;
    802068d2:	00163513          	seqz	a0,a2
                    *w++ = 0;
    802068d6:	00278893          	addi	a7,a5,2
                    if ((*w++ = *p++) == 0) {
    802068da:	0805                	addi	a6,a6,1
                    *w++ = 0;
    802068dc:	85ce                	mv	a1,s3
    802068de:	a8d5                	j	802069d2 <emake+0x39c>
            uint off2 = reloc_clus(dp, off, 1);
    802068e0:	4605                	li	a2,1
    802068e2:	85e2                	mv	a1,s8
    802068e4:	855a                	mv	a0,s6
    802068e6:	fffff097          	auipc	ra,0xfffff
    802068ea:	3de080e7          	jalr	990(ra) # 80205cc4 <reloc_clus>
            rw_clus(dp->cur_clus, 1, 0, (uint64)&de, off2, sizeof(de));
    802068ee:	02000793          	li	a5,32
    802068f2:	0005071b          	sext.w	a4,a0
    802068f6:	f7040693          	addi	a3,s0,-144
    802068fa:	864e                	mv	a2,s3
    802068fc:	4585                	li	a1,1
    802068fe:	10cb2503          	lw	a0,268(s6)
    80206902:	fffff097          	auipc	ra,0xfffff
    80206906:	4ca080e7          	jalr	1226(ra) # 80205dcc <rw_clus>
            off += sizeof(de);
    8020690a:	020c0c1b          	addiw	s8,s8,32
        for (int i = entcnt; i > 0; i--) {
    8020690e:	1bcd                	addi	s7,s7,-13
    80206910:	03505163          	blez	s5,80206932 <emake+0x2fc>
            if ((de.lne.order = i) == entcnt) {
    80206914:	0ffaf793          	zext.b	a5,s5
    80206918:	fafd86e3          	beq	s11,a5,802068c4 <emake+0x28e>
    8020691c:	f6f40823          	sb	a5,-144(s0)
            char *p = ep->filename + (i - 1) * CHAR_LONG_NAME;
    80206920:	3afd                	addiw	s5,s5,-1
    80206922:	885e                	mv	a6,s7
    80206924:	4709                	li	a4,2
            int end = 0;
    80206926:	854e                	mv	a0,s3
            uint8 *w = (uint8 *)de.lne.name1;
    80206928:	f5043783          	ld	a5,-176(s0)
                switch (j) {
    8020692c:	4315                	li	t1,5
            for (int j = 1; j <= CHAR_LONG_NAME; j++) {
    8020692e:	4e35                	li	t3,13
    80206930:	a851                	j	802069c4 <emake+0x38e>
    80206932:	020c8c9b          	addiw	s9,s9,32
            off += sizeof(de);
    80206936:	4781                	li	a5,0
    80206938:	f4843703          	ld	a4,-184(s0)
    8020693c:	00e05663          	blez	a4,80206948 <emake+0x312>
    80206940:	f4442783          	lw	a5,-188(s0)
    80206944:	0057979b          	slliw	a5,a5,0x5
    80206948:	01978cbb          	addw	s9,a5,s9
        memset(&de, 0, sizeof(de));
    8020694c:	02000613          	li	a2,32
    80206950:	4581                	li	a1,0
    80206952:	f7040493          	addi	s1,s0,-144
    80206956:	8526                	mv	a0,s1
    80206958:	ffffa097          	auipc	ra,0xffffa
    8020695c:	e38080e7          	jalr	-456(ra) # 80200790 <memset>
        strncpy(de.sne.name, shortname, sizeof(de.sne.name));
    80206960:	462d                	li	a2,11
    80206962:	f6040593          	addi	a1,s0,-160
    80206966:	8526                	mv	a0,s1
    80206968:	ffffa097          	auipc	ra,0xffffa
    8020696c:	f3c080e7          	jalr	-196(ra) # 802008a4 <strncpy>
        de.sne.attr = ep->attribute;
    80206970:	100d4783          	lbu	a5,256(s10)
    80206974:	f6f40da3          	sb	a5,-133(s0)
        de.sne.fst_clus_hi = (uint16)(ep->first_clus >> 16);      // first clus high 16 bits
    80206978:	104d2783          	lw	a5,260(s10)
    8020697c:	0107d71b          	srliw	a4,a5,0x10
    80206980:	f8e41223          	sh	a4,-124(s0)
        de.sne.fst_clus_lo = (uint16)(ep->first_clus & 0xffff);     // low 16 bits
    80206984:	f8f41523          	sh	a5,-118(s0)
        de.sne.file_size = ep->file_size;                         // filesize is updated in eupdate()
    80206988:	108d2783          	lw	a5,264(s10)
    8020698c:	f8f42623          	sw	a5,-116(s0)
        off = reloc_clus(dp, off, 1);
    80206990:	4605                	li	a2,1
    80206992:	85e6                	mv	a1,s9
    80206994:	855a                	mv	a0,s6
    80206996:	fffff097          	auipc	ra,0xfffff
    8020699a:	32e080e7          	jalr	814(ra) # 80205cc4 <reloc_clus>
        rw_clus(dp->cur_clus, 1, 0, (uint64)&de, off, sizeof(de));
    8020699e:	02000793          	li	a5,32
    802069a2:	0005071b          	sext.w	a4,a0
    802069a6:	86a6                	mv	a3,s1
    802069a8:	4601                	li	a2,0
    802069aa:	4585                	li	a1,1
    802069ac:	10cb2503          	lw	a0,268(s6)
    802069b0:	fffff097          	auipc	ra,0xfffff
    802069b4:	41c080e7          	jalr	1052(ra) # 80205dcc <rw_clus>
}
    802069b8:	b325                	j	802066e0 <emake+0xaa>
                    case 5:     w = (uint8 *)de.lne.name2; break;
    802069ba:	f5843783          	ld	a5,-168(s0)
    802069be:	a011                	j	802069c2 <emake+0x38c>
                switch (j) {
    802069c0:	87d2                	mv	a5,s4
            for (int j = 1; j <= CHAR_LONG_NAME; j++) {
    802069c2:	2705                	addiw	a4,a4,1
    802069c4:	fff7069b          	addiw	a3,a4,-1
                if (end) {
    802069c8:	d119                	beqz	a0,802068ce <emake+0x298>
                    *w++ = 0xff;
    802069ca:	00278893          	addi	a7,a5,2
    802069ce:	864a                	mv	a2,s2
    802069d0:	85ca                	mv	a1,s2
                    *w++ = 0xff;            // on k210, unaligned reading is illegal
    802069d2:	00c78023          	sb	a2,0(a5)
                    *w++ = 0xff;
    802069d6:	00b780a3          	sb	a1,1(a5)
                switch (j) {
    802069da:	fe6680e3          	beq	a3,t1,802069ba <emake+0x384>
    802069de:	fe9681e3          	beq	a3,s1,802069c0 <emake+0x38a>
            for (int j = 1; j <= CHAR_LONG_NAME; j++) {
    802069e2:	0007079b          	sext.w	a5,a4
    802069e6:	eefe4de3          	blt	t3,a5,802068e0 <emake+0x2aa>
    802069ea:	87c6                	mv	a5,a7
    802069ec:	bfd9                	j	802069c2 <emake+0x38c>
        if (c == ' ') { continue; }
    802069ee:	02000793          	li	a5,32
    802069f2:	e8f90ae3          	beq	s2,a5,80206886 <emake+0x250>
        if (c == '.') {
    802069f6:	02e00793          	li	a5,46
    802069fa:	eaf917e3          	bne	s2,a5,802068a8 <emake+0x272>
    802069fe:	bd35                	j	8020683a <emake+0x204>

0000000080206a00 <edup>:
{
    80206a00:	1101                	addi	sp,sp,-32
    80206a02:	ec06                	sd	ra,24(sp)
    80206a04:	e822                	sd	s0,16(sp)
    80206a06:	e426                	sd	s1,8(sp)
    80206a08:	1000                	addi	s0,sp,32
    80206a0a:	84aa                	mv	s1,a0
    if (entry != 0) {
    80206a0c:	c515                	beqz	a0,80206a38 <edup+0x38>
        acquire(&ecache.lock);
    80206a0e:	0004a517          	auipc	a0,0x4a
    80206a12:	e2a50513          	addi	a0,a0,-470 # 80250838 <ecache>
    80206a16:	ffffa097          	auipc	ra,0xffffa
    80206a1a:	cde080e7          	jalr	-802(ra) # 802006f4 <acquire>
        entry->ref++;
    80206a1e:	1184a783          	lw	a5,280(s1)
    80206a22:	2785                	addiw	a5,a5,1
    80206a24:	10f4ac23          	sw	a5,280(s1)
        release(&ecache.lock);
    80206a28:	0004a517          	auipc	a0,0x4a
    80206a2c:	e1050513          	addi	a0,a0,-496 # 80250838 <ecache>
    80206a30:	ffffa097          	auipc	ra,0xffffa
    80206a34:	d18080e7          	jalr	-744(ra) # 80200748 <release>
}
    80206a38:	8526                	mv	a0,s1
    80206a3a:	60e2                	ld	ra,24(sp)
    80206a3c:	6442                	ld	s0,16(sp)
    80206a3e:	64a2                	ld	s1,8(sp)
    80206a40:	6105                	addi	sp,sp,32
    80206a42:	8082                	ret

0000000080206a44 <eupdate>:
    if (!entry->dirty || entry->valid != 1) { return; }
    80206a44:	11554783          	lbu	a5,277(a0)
    80206a48:	c3e5                	beqz	a5,80206b28 <eupdate+0xe4>
{
    80206a4a:	711d                	addi	sp,sp,-96
    80206a4c:	ec86                	sd	ra,88(sp)
    80206a4e:	e8a2                	sd	s0,80(sp)
    80206a50:	e4a6                	sd	s1,72(sp)
    80206a52:	e0ca                	sd	s2,64(sp)
    80206a54:	fc4e                	sd	s3,56(sp)
    80206a56:	1080                	addi	s0,sp,96
    80206a58:	84aa                	mv	s1,a0
    if (!entry->dirty || entry->valid != 1) { return; }
    80206a5a:	11651703          	lh	a4,278(a0)
    80206a5e:	4785                	li	a5,1
    80206a60:	00f70963          	beq	a4,a5,80206a72 <eupdate+0x2e>
}
    80206a64:	60e6                	ld	ra,88(sp)
    80206a66:	6446                	ld	s0,80(sp)
    80206a68:	64a6                	ld	s1,72(sp)
    80206a6a:	6906                	ld	s2,64(sp)
    80206a6c:	79e2                	ld	s3,56(sp)
    80206a6e:	6125                	addi	sp,sp,96
    80206a70:	8082                	ret
    uint entcnt = 0;
    80206a72:	fc042623          	sw	zero,-52(s0)
    uint32 off = reloc_clus(entry->parent, entry->off, 0);
    80206a76:	4601                	li	a2,0
    80206a78:	11c52583          	lw	a1,284(a0)
    80206a7c:	12053503          	ld	a0,288(a0)
    80206a80:	fffff097          	auipc	ra,0xfffff
    80206a84:	244080e7          	jalr	580(ra) # 80205cc4 <reloc_clus>
    rw_clus(entry->parent->cur_clus, 0, 0, (uint64) &entcnt, off, 1);
    80206a88:	1204b803          	ld	a6,288(s1)
    80206a8c:	4785                	li	a5,1
    80206a8e:	0005071b          	sext.w	a4,a0
    80206a92:	fcc40693          	addi	a3,s0,-52
    80206a96:	4601                	li	a2,0
    80206a98:	4581                	li	a1,0
    80206a9a:	10c82503          	lw	a0,268(a6)
    80206a9e:	fffff097          	auipc	ra,0xfffff
    80206aa2:	32e080e7          	jalr	814(ra) # 80205dcc <rw_clus>
    entcnt &= ~LAST_LONG_ENTRY;
    80206aa6:	fcc42583          	lw	a1,-52(s0)
    80206aaa:	fbf5f593          	andi	a1,a1,-65
    80206aae:	fcb42623          	sw	a1,-52(s0)
    off = reloc_clus(entry->parent, entry->off + (entcnt << 5), 0);
    80206ab2:	0055959b          	slliw	a1,a1,0x5
    80206ab6:	11c4a783          	lw	a5,284(s1)
    80206aba:	4601                	li	a2,0
    80206abc:	9dbd                	addw	a1,a1,a5
    80206abe:	1204b503          	ld	a0,288(s1)
    80206ac2:	fffff097          	auipc	ra,0xfffff
    80206ac6:	202080e7          	jalr	514(ra) # 80205cc4 <reloc_clus>
    80206aca:	0005099b          	sext.w	s3,a0
    rw_clus(entry->parent->cur_clus, 0, 0, (uint64)&de, off, sizeof(de));
    80206ace:	1204b503          	ld	a0,288(s1)
    80206ad2:	02000793          	li	a5,32
    80206ad6:	874e                	mv	a4,s3
    80206ad8:	fa840913          	addi	s2,s0,-88
    80206adc:	86ca                	mv	a3,s2
    80206ade:	4601                	li	a2,0
    80206ae0:	4581                	li	a1,0
    80206ae2:	10c52503          	lw	a0,268(a0)
    80206ae6:	fffff097          	auipc	ra,0xfffff
    80206aea:	2e6080e7          	jalr	742(ra) # 80205dcc <rw_clus>
    de.sne.fst_clus_hi = (uint16)(entry->first_clus >> 16);
    80206aee:	1044a783          	lw	a5,260(s1)
    80206af2:	0107d71b          	srliw	a4,a5,0x10
    80206af6:	fae41e23          	sh	a4,-68(s0)
    de.sne.fst_clus_lo = (uint16)(entry->first_clus & 0xffff);
    80206afa:	fcf41123          	sh	a5,-62(s0)
    de.sne.file_size = entry->file_size;
    80206afe:	1084a783          	lw	a5,264(s1)
    80206b02:	fcf42223          	sw	a5,-60(s0)
    rw_clus(entry->parent->cur_clus, 1, 0, (uint64)&de, off, sizeof(de));
    80206b06:	1204b503          	ld	a0,288(s1)
    80206b0a:	02000793          	li	a5,32
    80206b0e:	874e                	mv	a4,s3
    80206b10:	86ca                	mv	a3,s2
    80206b12:	4601                	li	a2,0
    80206b14:	4585                	li	a1,1
    80206b16:	10c52503          	lw	a0,268(a0)
    80206b1a:	fffff097          	auipc	ra,0xfffff
    80206b1e:	2b2080e7          	jalr	690(ra) # 80205dcc <rw_clus>
    entry->dirty = 0;
    80206b22:	10048aa3          	sb	zero,277(s1)
    80206b26:	bf3d                	j	80206a64 <eupdate+0x20>
    80206b28:	8082                	ret

0000000080206b2a <eremove>:
    if (entry->valid != 1) { return; }
    80206b2a:	11651703          	lh	a4,278(a0)
    80206b2e:	4785                	li	a5,1
    80206b30:	00f70363          	beq	a4,a5,80206b36 <eremove+0xc>
    80206b34:	8082                	ret
{
    80206b36:	715d                	addi	sp,sp,-80
    80206b38:	e486                	sd	ra,72(sp)
    80206b3a:	e0a2                	sd	s0,64(sp)
    80206b3c:	fc26                	sd	s1,56(sp)
    80206b3e:	f84a                	sd	s2,48(sp)
    80206b40:	f44e                	sd	s3,40(sp)
    80206b42:	f052                	sd	s4,32(sp)
    80206b44:	ec56                	sd	s5,24(sp)
    80206b46:	0880                	addi	s0,sp,80
    80206b48:	89aa                	mv	s3,a0
    uint entcnt = 0;
    80206b4a:	fa042e23          	sw	zero,-68(s0)
    uint32 off = entry->off;
    80206b4e:	11c52a03          	lw	s4,284(a0)
    uint32 off2 = reloc_clus(entry->parent, off, 0);
    80206b52:	4601                	li	a2,0
    80206b54:	85d2                	mv	a1,s4
    80206b56:	12053503          	ld	a0,288(a0)
    80206b5a:	fffff097          	auipc	ra,0xfffff
    80206b5e:	16a080e7          	jalr	362(ra) # 80205cc4 <reloc_clus>
    80206b62:	0005049b          	sext.w	s1,a0
    rw_clus(entry->parent->cur_clus, 0, 0, (uint64) &entcnt, off2, 1);
    80206b66:	1209b503          	ld	a0,288(s3)
    80206b6a:	4785                	li	a5,1
    80206b6c:	8726                	mv	a4,s1
    80206b6e:	fbc40693          	addi	a3,s0,-68
    80206b72:	4601                	li	a2,0
    80206b74:	4581                	li	a1,0
    80206b76:	10c52503          	lw	a0,268(a0)
    80206b7a:	fffff097          	auipc	ra,0xfffff
    80206b7e:	252080e7          	jalr	594(ra) # 80205dcc <rw_clus>
    entcnt &= ~LAST_LONG_ENTRY;
    80206b82:	fbc42783          	lw	a5,-68(s0)
    80206b86:	fbf7f793          	andi	a5,a5,-65
    80206b8a:	faf42e23          	sw	a5,-68(s0)
    uint8 flag = EMPTY_ENTRY;
    80206b8e:	5795                	li	a5,-27
    80206b90:	faf40da3          	sb	a5,-69(s0)
    for (int i = 0; i <= entcnt; i++) {
    80206b94:	4901                	li	s2,0
        rw_clus(entry->parent->cur_clus, 1, 0, (uint64) &flag, off2, 1);
    80206b96:	fbb40a93          	addi	s5,s0,-69
    80206b9a:	1209b503          	ld	a0,288(s3)
    80206b9e:	4785                	li	a5,1
    80206ba0:	8726                	mv	a4,s1
    80206ba2:	86d6                	mv	a3,s5
    80206ba4:	4601                	li	a2,0
    80206ba6:	4585                	li	a1,1
    80206ba8:	10c52503          	lw	a0,268(a0)
    80206bac:	fffff097          	auipc	ra,0xfffff
    80206bb0:	220080e7          	jalr	544(ra) # 80205dcc <rw_clus>
        off += 32;
    80206bb4:	020a0a1b          	addiw	s4,s4,32
        off2 = reloc_clus(entry->parent, off, 0);
    80206bb8:	4601                	li	a2,0
    80206bba:	85d2                	mv	a1,s4
    80206bbc:	1209b503          	ld	a0,288(s3)
    80206bc0:	fffff097          	auipc	ra,0xfffff
    80206bc4:	104080e7          	jalr	260(ra) # 80205cc4 <reloc_clus>
    80206bc8:	0005049b          	sext.w	s1,a0
    for (int i = 0; i <= entcnt; i++) {
    80206bcc:	0019079b          	addiw	a5,s2,1
    80206bd0:	0007891b          	sext.w	s2,a5
    80206bd4:	fbc42703          	lw	a4,-68(s0)
    80206bd8:	fd2771e3          	bgeu	a4,s2,80206b9a <eremove+0x70>
    entry->valid = -1;
    80206bdc:	57fd                	li	a5,-1
    80206bde:	10f99b23          	sh	a5,278(s3)
}
    80206be2:	60a6                	ld	ra,72(sp)
    80206be4:	6406                	ld	s0,64(sp)
    80206be6:	74e2                	ld	s1,56(sp)
    80206be8:	7942                	ld	s2,48(sp)
    80206bea:	79a2                	ld	s3,40(sp)
    80206bec:	7a02                	ld	s4,32(sp)
    80206bee:	6ae2                	ld	s5,24(sp)
    80206bf0:	6161                	addi	sp,sp,80
    80206bf2:	8082                	ret

0000000080206bf4 <etrunc>:
{
    80206bf4:	7179                	addi	sp,sp,-48
    80206bf6:	f406                	sd	ra,40(sp)
    80206bf8:	f022                	sd	s0,32(sp)
    80206bfa:	ec26                	sd	s1,24(sp)
    80206bfc:	e84a                	sd	s2,16(sp)
    80206bfe:	e44e                	sd	s3,8(sp)
    80206c00:	e052                	sd	s4,0(sp)
    80206c02:	1800                	addi	s0,sp,48
    80206c04:	8a2a                	mv	s4,a0
    for (uint32 clus = entry->first_clus; clus >= 2 && clus < FAT32_EOC; ) {
    80206c06:	10452483          	lw	s1,260(a0)
    80206c0a:	ffe4871b          	addiw	a4,s1,-2
    80206c0e:	100007b7          	lui	a5,0x10000
    80206c12:	17d5                	addi	a5,a5,-11 # ffffff5 <_entry-0x7020000b>
    80206c14:	02e7e663          	bltu	a5,a4,80206c40 <etrunc+0x4c>
    80206c18:	89be                	mv	s3,a5
        uint32 next = read_fat(clus);
    80206c1a:	0004891b          	sext.w	s2,s1
    80206c1e:	8526                	mv	a0,s1
    80206c20:	fffff097          	auipc	ra,0xfffff
    80206c24:	e6c080e7          	jalr	-404(ra) # 80205a8c <read_fat>
    80206c28:	0005049b          	sext.w	s1,a0
    write_fat(cluster, 0);
    80206c2c:	4581                	li	a1,0
    80206c2e:	854a                	mv	a0,s2
    80206c30:	fffff097          	auipc	ra,0xfffff
    80206c34:	014080e7          	jalr	20(ra) # 80205c44 <write_fat>
    for (uint32 clus = entry->first_clus; clus >= 2 && clus < FAT32_EOC; ) {
    80206c38:	ffe4879b          	addiw	a5,s1,-2
    80206c3c:	fcf9ffe3          	bgeu	s3,a5,80206c1a <etrunc+0x26>
    entry->file_size = 0;
    80206c40:	100a2423          	sw	zero,264(s4)
    entry->first_clus = 0;
    80206c44:	100a2223          	sw	zero,260(s4)
    entry->dirty = 1;
    80206c48:	4785                	li	a5,1
    80206c4a:	10fa0aa3          	sb	a5,277(s4)
}
    80206c4e:	70a2                	ld	ra,40(sp)
    80206c50:	7402                	ld	s0,32(sp)
    80206c52:	64e2                	ld	s1,24(sp)
    80206c54:	6942                	ld	s2,16(sp)
    80206c56:	69a2                	ld	s3,8(sp)
    80206c58:	6a02                	ld	s4,0(sp)
    80206c5a:	6145                	addi	sp,sp,48
    80206c5c:	8082                	ret

0000000080206c5e <elock>:
{
    80206c5e:	1141                	addi	sp,sp,-16
    80206c60:	e406                	sd	ra,8(sp)
    80206c62:	e022                	sd	s0,0(sp)
    80206c64:	0800                	addi	s0,sp,16
    if (entry == 0 || entry->ref < 1)
    80206c66:	cd19                	beqz	a0,80206c84 <elock+0x26>
    80206c68:	11852783          	lw	a5,280(a0)
    80206c6c:	00f05c63          	blez	a5,80206c84 <elock+0x26>
    acquiresleep(&entry->lock);
    80206c70:	13850513          	addi	a0,a0,312
    80206c74:	ffffd097          	auipc	ra,0xffffd
    80206c78:	f40080e7          	jalr	-192(ra) # 80203bb4 <acquiresleep>
}
    80206c7c:	60a2                	ld	ra,8(sp)
    80206c7e:	6402                	ld	s0,0(sp)
    80206c80:	0141                	addi	sp,sp,16
    80206c82:	8082                	ret
        panic("elock");
    80206c84:	00003517          	auipc	a0,0x3
    80206c88:	2ec50513          	addi	a0,a0,748 # 80209f70 <digits+0xbf0>
    80206c8c:	ffff9097          	auipc	ra,0xffff9
    80206c90:	4b8080e7          	jalr	1208(ra) # 80200144 <panic>

0000000080206c94 <eunlock>:
{
    80206c94:	1101                	addi	sp,sp,-32
    80206c96:	ec06                	sd	ra,24(sp)
    80206c98:	e822                	sd	s0,16(sp)
    80206c9a:	e426                	sd	s1,8(sp)
    80206c9c:	e04a                	sd	s2,0(sp)
    80206c9e:	1000                	addi	s0,sp,32
    if (entry == 0 || !holdingsleep(&entry->lock) || entry->ref < 1)
    80206ca0:	c90d                	beqz	a0,80206cd2 <eunlock+0x3e>
    80206ca2:	84aa                	mv	s1,a0
    80206ca4:	13850913          	addi	s2,a0,312
    80206ca8:	854a                	mv	a0,s2
    80206caa:	ffffd097          	auipc	ra,0xffffd
    80206cae:	fa4080e7          	jalr	-92(ra) # 80203c4e <holdingsleep>
    80206cb2:	c105                	beqz	a0,80206cd2 <eunlock+0x3e>
    80206cb4:	1184a783          	lw	a5,280(s1)
    80206cb8:	00f05d63          	blez	a5,80206cd2 <eunlock+0x3e>
    releasesleep(&entry->lock);
    80206cbc:	854a                	mv	a0,s2
    80206cbe:	ffffd097          	auipc	ra,0xffffd
    80206cc2:	f4c080e7          	jalr	-180(ra) # 80203c0a <releasesleep>
}
    80206cc6:	60e2                	ld	ra,24(sp)
    80206cc8:	6442                	ld	s0,16(sp)
    80206cca:	64a2                	ld	s1,8(sp)
    80206ccc:	6902                	ld	s2,0(sp)
    80206cce:	6105                	addi	sp,sp,32
    80206cd0:	8082                	ret
        panic("eunlock");
    80206cd2:	00003517          	auipc	a0,0x3
    80206cd6:	2a650513          	addi	a0,a0,678 # 80209f78 <digits+0xbf8>
    80206cda:	ffff9097          	auipc	ra,0xffff9
    80206cde:	46a080e7          	jalr	1130(ra) # 80200144 <panic>

0000000080206ce2 <eput>:
{
    80206ce2:	1101                	addi	sp,sp,-32
    80206ce4:	ec06                	sd	ra,24(sp)
    80206ce6:	e822                	sd	s0,16(sp)
    80206ce8:	e426                	sd	s1,8(sp)
    80206cea:	e04a                	sd	s2,0(sp)
    80206cec:	1000                	addi	s0,sp,32
    80206cee:	84aa                	mv	s1,a0
    acquire(&ecache.lock);
    80206cf0:	0004a517          	auipc	a0,0x4a
    80206cf4:	b4850513          	addi	a0,a0,-1208 # 80250838 <ecache>
    80206cf8:	ffffa097          	auipc	ra,0xffffa
    80206cfc:	9fc080e7          	jalr	-1540(ra) # 802006f4 <acquire>
    if (entry != &root && entry->valid != 0 && entry->ref == 1) {
    80206d00:	0004a797          	auipc	a5,0x4a
    80206d04:	9d078793          	addi	a5,a5,-1584 # 802506d0 <root>
    80206d08:	00f48a63          	beq	s1,a5,80206d1c <eput+0x3a>
    80206d0c:	11649783          	lh	a5,278(s1)
    80206d10:	c791                	beqz	a5,80206d1c <eput+0x3a>
    80206d12:	1184a703          	lw	a4,280(s1)
    80206d16:	4785                	li	a5,1
    80206d18:	02f70563          	beq	a4,a5,80206d42 <eput+0x60>
    entry->ref--;
    80206d1c:	1184a783          	lw	a5,280(s1)
    80206d20:	37fd                	addiw	a5,a5,-1
    80206d22:	10f4ac23          	sw	a5,280(s1)
    release(&ecache.lock);
    80206d26:	0004a517          	auipc	a0,0x4a
    80206d2a:	b1250513          	addi	a0,a0,-1262 # 80250838 <ecache>
    80206d2e:	ffffa097          	auipc	ra,0xffffa
    80206d32:	a1a080e7          	jalr	-1510(ra) # 80200748 <release>
}
    80206d36:	60e2                	ld	ra,24(sp)
    80206d38:	6442                	ld	s0,16(sp)
    80206d3a:	64a2                	ld	s1,8(sp)
    80206d3c:	6902                	ld	s2,0(sp)
    80206d3e:	6105                	addi	sp,sp,32
    80206d40:	8082                	ret
        acquiresleep(&entry->lock);
    80206d42:	13848913          	addi	s2,s1,312
    80206d46:	854a                	mv	a0,s2
    80206d48:	ffffd097          	auipc	ra,0xffffd
    80206d4c:	e6c080e7          	jalr	-404(ra) # 80203bb4 <acquiresleep>
        entry->next->prev = entry->prev;
    80206d50:	1284b703          	ld	a4,296(s1)
    80206d54:	1304b783          	ld	a5,304(s1)
    80206d58:	12f73823          	sd	a5,304(a4)
        entry->prev->next = entry->next;
    80206d5c:	1284b703          	ld	a4,296(s1)
    80206d60:	12e7b423          	sd	a4,296(a5)
        entry->next = root.next;
    80206d64:	0004a797          	auipc	a5,0x4a
    80206d68:	94478793          	addi	a5,a5,-1724 # 802506a8 <fat>
    80206d6c:	1507b703          	ld	a4,336(a5)
    80206d70:	12e4b423          	sd	a4,296(s1)
        entry->prev = &root;
    80206d74:	0004a697          	auipc	a3,0x4a
    80206d78:	95c68693          	addi	a3,a3,-1700 # 802506d0 <root>
    80206d7c:	12d4b823          	sd	a3,304(s1)
        root.next->prev = entry;
    80206d80:	12973823          	sd	s1,304(a4)
        root.next = entry;
    80206d84:	1497b823          	sd	s1,336(a5)
        release(&ecache.lock);
    80206d88:	0004a517          	auipc	a0,0x4a
    80206d8c:	ab050513          	addi	a0,a0,-1360 # 80250838 <ecache>
    80206d90:	ffffa097          	auipc	ra,0xffffa
    80206d94:	9b8080e7          	jalr	-1608(ra) # 80200748 <release>
        if (entry->valid == -1) {       // this means some one has called eremove()
    80206d98:	11649703          	lh	a4,278(s1)
    80206d9c:	57fd                	li	a5,-1
    80206d9e:	06f70863          	beq	a4,a5,80206e0e <eput+0x12c>
            elock(entry->parent);
    80206da2:	1204b503          	ld	a0,288(s1)
    80206da6:	00000097          	auipc	ra,0x0
    80206daa:	eb8080e7          	jalr	-328(ra) # 80206c5e <elock>
            eupdate(entry);
    80206dae:	8526                	mv	a0,s1
    80206db0:	00000097          	auipc	ra,0x0
    80206db4:	c94080e7          	jalr	-876(ra) # 80206a44 <eupdate>
            eunlock(entry->parent);
    80206db8:	1204b503          	ld	a0,288(s1)
    80206dbc:	00000097          	auipc	ra,0x0
    80206dc0:	ed8080e7          	jalr	-296(ra) # 80206c94 <eunlock>
        releasesleep(&entry->lock);
    80206dc4:	854a                	mv	a0,s2
    80206dc6:	ffffd097          	auipc	ra,0xffffd
    80206dca:	e44080e7          	jalr	-444(ra) # 80203c0a <releasesleep>
        struct dirent *eparent = entry->parent;
    80206dce:	1204b903          	ld	s2,288(s1)
        acquire(&ecache.lock);
    80206dd2:	0004a517          	auipc	a0,0x4a
    80206dd6:	a6650513          	addi	a0,a0,-1434 # 80250838 <ecache>
    80206dda:	ffffa097          	auipc	ra,0xffffa
    80206dde:	91a080e7          	jalr	-1766(ra) # 802006f4 <acquire>
        entry->ref--;
    80206de2:	1184a783          	lw	a5,280(s1)
    80206de6:	37fd                	addiw	a5,a5,-1
    80206de8:	10f4ac23          	sw	a5,280(s1)
        release(&ecache.lock);
    80206dec:	0004a517          	auipc	a0,0x4a
    80206df0:	a4c50513          	addi	a0,a0,-1460 # 80250838 <ecache>
    80206df4:	ffffa097          	auipc	ra,0xffffa
    80206df8:	954080e7          	jalr	-1708(ra) # 80200748 <release>
        if (entry->ref == 0) {
    80206dfc:	1184a783          	lw	a5,280(s1)
    80206e00:	fb9d                	bnez	a5,80206d36 <eput+0x54>
            eput(eparent);
    80206e02:	854a                	mv	a0,s2
    80206e04:	00000097          	auipc	ra,0x0
    80206e08:	ede080e7          	jalr	-290(ra) # 80206ce2 <eput>
    80206e0c:	b72d                	j	80206d36 <eput+0x54>
            etrunc(entry);
    80206e0e:	8526                	mv	a0,s1
    80206e10:	00000097          	auipc	ra,0x0
    80206e14:	de4080e7          	jalr	-540(ra) # 80206bf4 <etrunc>
    80206e18:	b775                	j	80206dc4 <eput+0xe2>

0000000080206e1a <estat>:
{
    80206e1a:	1101                	addi	sp,sp,-32
    80206e1c:	ec06                	sd	ra,24(sp)
    80206e1e:	e822                	sd	s0,16(sp)
    80206e20:	e426                	sd	s1,8(sp)
    80206e22:	e04a                	sd	s2,0(sp)
    80206e24:	1000                	addi	s0,sp,32
    80206e26:	892a                	mv	s2,a0
    80206e28:	84ae                	mv	s1,a1
    strncpy(st->name, de->filename, STAT_MAX_NAME);
    80206e2a:	02000613          	li	a2,32
    80206e2e:	85aa                	mv	a1,a0
    80206e30:	8526                	mv	a0,s1
    80206e32:	ffffa097          	auipc	ra,0xffffa
    80206e36:	a72080e7          	jalr	-1422(ra) # 802008a4 <strncpy>
    st->type = (de->attribute & ATTR_DIRECTORY) ? T_DIR : T_FILE;
    80206e3a:	10094783          	lbu	a5,256(s2)
    80206e3e:	8bc1                	andi	a5,a5,16
    80206e40:	0017b793          	seqz	a5,a5
    80206e44:	0785                	addi	a5,a5,1
    80206e46:	02f49423          	sh	a5,40(s1)
    st->dev = de->dev;
    80206e4a:	11494783          	lbu	a5,276(s2)
    80206e4e:	d0dc                	sw	a5,36(s1)
    st->size = de->file_size;
    80206e50:	10896783          	lwu	a5,264(s2)
    80206e54:	f89c                	sd	a5,48(s1)
}
    80206e56:	60e2                	ld	ra,24(sp)
    80206e58:	6442                	ld	s0,16(sp)
    80206e5a:	64a2                	ld	s1,8(sp)
    80206e5c:	6902                	ld	s2,0(sp)
    80206e5e:	6105                	addi	sp,sp,32
    80206e60:	8082                	ret

0000000080206e62 <enext>:
 * @return  -1      meet the end of dir
 *          0       find empty slots
 *          1       find a file with all its entries
 */
int enext(struct dirent *dp, struct dirent *ep, uint off, int *count)
{
    80206e62:	7119                	addi	sp,sp,-128
    80206e64:	fc86                	sd	ra,120(sp)
    80206e66:	f8a2                	sd	s0,112(sp)
    80206e68:	f4a6                	sd	s1,104(sp)
    80206e6a:	f0ca                	sd	s2,96(sp)
    80206e6c:	ecce                	sd	s3,88(sp)
    80206e6e:	e8d2                	sd	s4,80(sp)
    80206e70:	e4d6                	sd	s5,72(sp)
    80206e72:	e0da                	sd	s6,64(sp)
    80206e74:	fc5e                	sd	s7,56(sp)
    80206e76:	f862                	sd	s8,48(sp)
    80206e78:	f466                	sd	s9,40(sp)
    80206e7a:	0100                	addi	s0,sp,128
    if (!(dp->attribute & ATTR_DIRECTORY))
    80206e7c:	10054783          	lbu	a5,256(a0)
    80206e80:	8bc1                	andi	a5,a5,16
    80206e82:	cf95                	beqz	a5,80206ebe <enext+0x5c>
    80206e84:	892a                	mv	s2,a0
    80206e86:	89ae                	mv	s3,a1
    80206e88:	84b2                	mv	s1,a2
    80206e8a:	8ab6                	mv	s5,a3
        panic("enext not dir");
    if (ep->valid)
    80206e8c:	11659783          	lh	a5,278(a1)
    80206e90:	ef9d                	bnez	a5,80206ece <enext+0x6c>
        panic("enext ep valid");
    if (off % 32)
    80206e92:	01f67793          	andi	a5,a2,31
    80206e96:	e7a1                	bnez	a5,80206ede <enext+0x7c>
        panic("enext not align");
    if (dp->valid != 1) { return -1; }
    80206e98:	11651703          	lh	a4,278(a0)
    80206e9c:	4785                	li	a5,1
    80206e9e:	557d                	li	a0,-1
    80206ea0:	04f70763          	beq	a4,a5,80206eee <enext+0x8c>
            read_entry_info(ep, &de);
            return 1;
        }
    }
    return -1;
}
    80206ea4:	70e6                	ld	ra,120(sp)
    80206ea6:	7446                	ld	s0,112(sp)
    80206ea8:	74a6                	ld	s1,104(sp)
    80206eaa:	7906                	ld	s2,96(sp)
    80206eac:	69e6                	ld	s3,88(sp)
    80206eae:	6a46                	ld	s4,80(sp)
    80206eb0:	6aa6                	ld	s5,72(sp)
    80206eb2:	6b06                	ld	s6,64(sp)
    80206eb4:	7be2                	ld	s7,56(sp)
    80206eb6:	7c42                	ld	s8,48(sp)
    80206eb8:	7ca2                	ld	s9,40(sp)
    80206eba:	6109                	addi	sp,sp,128
    80206ebc:	8082                	ret
        panic("enext not dir");
    80206ebe:	00003517          	auipc	a0,0x3
    80206ec2:	0c250513          	addi	a0,a0,194 # 80209f80 <digits+0xc00>
    80206ec6:	ffff9097          	auipc	ra,0xffff9
    80206eca:	27e080e7          	jalr	638(ra) # 80200144 <panic>
        panic("enext ep valid");
    80206ece:	00003517          	auipc	a0,0x3
    80206ed2:	0c250513          	addi	a0,a0,194 # 80209f90 <digits+0xc10>
    80206ed6:	ffff9097          	auipc	ra,0xffff9
    80206eda:	26e080e7          	jalr	622(ra) # 80200144 <panic>
        panic("enext not align");
    80206ede:	00003517          	auipc	a0,0x3
    80206ee2:	0c250513          	addi	a0,a0,194 # 80209fa0 <digits+0xc20>
    80206ee6:	ffff9097          	auipc	ra,0xffff9
    80206eea:	25e080e7          	jalr	606(ra) # 80200144 <panic>
    memset(ep->filename, 0, FAT32_MAX_FILENAME + 1);
    80206eee:	10000613          	li	a2,256
    80206ef2:	4581                	li	a1,0
    80206ef4:	854e                	mv	a0,s3
    80206ef6:	ffffa097          	auipc	ra,0xffffa
    80206efa:	89a080e7          	jalr	-1894(ra) # 80200790 <memset>
    int cnt = 0;
    80206efe:	4b01                	li	s6,0
    for (int off2; (off2 = reloc_clus(dp, off, 0)) != -1; off += 32) {
    80206f00:	5bfd                	li	s7,-1
        if (rw_clus(dp->cur_clus, 0, 0, (uint64)&de, off2, 32) != 32 || de.lne.order == END_OF_ENTRY) {
    80206f02:	f8040a13          	addi	s4,s0,-128
        if (de.lne.order == EMPTY_ENTRY) {
    80206f06:	0e500c13          	li	s8,229
        if (de.lne.attr == ATTR_LONG_NAME) {
    80206f0a:	4cbd                	li	s9,15
    for (int off2; (off2 = reloc_clus(dp, off, 0)) != -1; off += 32) {
    80206f0c:	a03d                	j	80206f3a <enext+0xd8>
            cnt++;
    80206f0e:	2b05                	addiw	s6,s6,1
            continue;
    80206f10:	a01d                	j	80206f36 <enext+0xd4>
            *count = cnt;
    80206f12:	016aa023          	sw	s6,0(s5)
            return 0;
    80206f16:	4501                	li	a0,0
    80206f18:	b771                	j	80206ea4 <enext+0x42>
            read_entry_name(ep->filename + (lcnt - 1) * CHAR_LONG_NAME, &de);
    80206f1a:	fff7079b          	addiw	a5,a4,-1
    80206f1e:	0017951b          	slliw	a0,a5,0x1
    80206f22:	9d3d                	addw	a0,a0,a5
    80206f24:	0025151b          	slliw	a0,a0,0x2
    80206f28:	9d3d                	addw	a0,a0,a5
    80206f2a:	85d2                	mv	a1,s4
    80206f2c:	954e                	add	a0,a0,s3
    80206f2e:	fffff097          	auipc	ra,0xfffff
    80206f32:	0f8080e7          	jalr	248(ra) # 80206026 <read_entry_name>
    for (int off2; (off2 = reloc_clus(dp, off, 0)) != -1; off += 32) {
    80206f36:	0204849b          	addiw	s1,s1,32
    80206f3a:	4601                	li	a2,0
    80206f3c:	85a6                	mv	a1,s1
    80206f3e:	854a                	mv	a0,s2
    80206f40:	fffff097          	auipc	ra,0xfffff
    80206f44:	d84080e7          	jalr	-636(ra) # 80205cc4 <reloc_clus>
    80206f48:	f5750ee3          	beq	a0,s7,80206ea4 <enext+0x42>
        if (rw_clus(dp->cur_clus, 0, 0, (uint64)&de, off2, 32) != 32 || de.lne.order == END_OF_ENTRY) {
    80206f4c:	02000793          	li	a5,32
    80206f50:	0005071b          	sext.w	a4,a0
    80206f54:	86d2                	mv	a3,s4
    80206f56:	4601                	li	a2,0
    80206f58:	4581                	li	a1,0
    80206f5a:	10c92503          	lw	a0,268(s2)
    80206f5e:	fffff097          	auipc	ra,0xfffff
    80206f62:	e6e080e7          	jalr	-402(ra) # 80205dcc <rw_clus>
    80206f66:	2501                	sext.w	a0,a0
    80206f68:	02000793          	li	a5,32
    80206f6c:	06f51c63          	bne	a0,a5,80206fe4 <enext+0x182>
    80206f70:	f8044783          	lbu	a5,-128(s0)
    80206f74:	cbb5                	beqz	a5,80206fe8 <enext+0x186>
        if (de.lne.order == EMPTY_ENTRY) {
    80206f76:	f9878ce3          	beq	a5,s8,80206f0e <enext+0xac>
        } else if (cnt) {
    80206f7a:	f80b1ce3          	bnez	s6,80206f12 <enext+0xb0>
        if (de.lne.attr == ATTR_LONG_NAME) {
    80206f7e:	f8b44703          	lbu	a4,-117(s0)
    80206f82:	01971d63          	bne	a4,s9,80206f9c <enext+0x13a>
            int lcnt = de.lne.order & ~LAST_LONG_ENTRY;
    80206f86:	0bf7f713          	andi	a4,a5,191
            if (de.lne.order & LAST_LONG_ENTRY) {
    80206f8a:	0407f793          	andi	a5,a5,64
    80206f8e:	d7d1                	beqz	a5,80206f1a <enext+0xb8>
                *count = lcnt + 1;                              // plus the s-n-e;
    80206f90:	0017079b          	addiw	a5,a4,1
    80206f94:	00faa023          	sw	a5,0(s5)
                count = 0;
    80206f98:	4a81                	li	s5,0
    80206f9a:	b741                	j	80206f1a <enext+0xb8>
            if (count) {
    80206f9c:	000a8c63          	beqz	s5,80206fb4 <enext+0x152>
                *count = 1;
    80206fa0:	4785                	li	a5,1
    80206fa2:	00faa023          	sw	a5,0(s5)
                read_entry_name(ep->filename, &de);
    80206fa6:	f8040593          	addi	a1,s0,-128
    80206faa:	854e                	mv	a0,s3
    80206fac:	fffff097          	auipc	ra,0xfffff
    80206fb0:	07a080e7          	jalr	122(ra) # 80206026 <read_entry_name>
    entry->attribute = d->sne.attr;
    80206fb4:	f8b44783          	lbu	a5,-117(s0)
    80206fb8:	10f98023          	sb	a5,256(s3)
    entry->first_clus = ((uint32)d->sne.fst_clus_hi << 16) | d->sne.fst_clus_lo;
    80206fbc:	f9445783          	lhu	a5,-108(s0)
    80206fc0:	0107979b          	slliw	a5,a5,0x10
    80206fc4:	f9a45703          	lhu	a4,-102(s0)
    80206fc8:	8fd9                	or	a5,a5,a4
    80206fca:	2781                	sext.w	a5,a5
    80206fcc:	10f9a223          	sw	a5,260(s3)
    entry->file_size = d->sne.file_size;
    80206fd0:	f9c42703          	lw	a4,-100(s0)
    80206fd4:	10e9a423          	sw	a4,264(s3)
    entry->cur_clus = entry->first_clus;
    80206fd8:	10f9a623          	sw	a5,268(s3)
    entry->clus_cnt = 0;
    80206fdc:	1009a823          	sw	zero,272(s3)
            return 1;
    80206fe0:	4505                	li	a0,1
}
    80206fe2:	b5c9                	j	80206ea4 <enext+0x42>
            return -1;
    80206fe4:	557d                	li	a0,-1
    80206fe6:	bd7d                	j	80206ea4 <enext+0x42>
    80206fe8:	557d                	li	a0,-1
    80206fea:	bd6d                	j	80206ea4 <enext+0x42>

0000000080206fec <dirlookup>:
 * @param   dp          entry of a directory file
 * @param   filename    target filename
 * @param   poff        offset of proper empty entry slots from the beginning of the dir
 */
struct dirent *dirlookup(struct dirent *dp, char *filename, uint *poff)
{
    80206fec:	711d                	addi	sp,sp,-96
    80206fee:	ec86                	sd	ra,88(sp)
    80206ff0:	e8a2                	sd	s0,80(sp)
    80206ff2:	e4a6                	sd	s1,72(sp)
    80206ff4:	e0ca                	sd	s2,64(sp)
    80206ff6:	fc4e                	sd	s3,56(sp)
    80206ff8:	f852                	sd	s4,48(sp)
    80206ffa:	f456                	sd	s5,40(sp)
    80206ffc:	f05a                	sd	s6,32(sp)
    80206ffe:	ec5e                	sd	s7,24(sp)
    80207000:	1080                	addi	s0,sp,96
    if (!(dp->attribute & ATTR_DIRECTORY))
    80207002:	10054783          	lbu	a5,256(a0)
    80207006:	8bc1                	andi	a5,a5,16
    80207008:	cbb1                	beqz	a5,8020705c <dirlookup+0x70>
    8020700a:	84aa                	mv	s1,a0
    8020700c:	89ae                	mv	s3,a1
    8020700e:	8ab2                	mv	s5,a2
        panic("dirlookup not DIR");
    if (strncmp(filename, ".", FAT32_MAX_FILENAME) == 0) {
    80207010:	0ff00613          	li	a2,255
    80207014:	00003597          	auipc	a1,0x3
    80207018:	fb458593          	addi	a1,a1,-76 # 80209fc8 <digits+0xc48>
    8020701c:	854e                	mv	a0,s3
    8020701e:	ffffa097          	auipc	ra,0xffffa
    80207022:	84a080e7          	jalr	-1974(ra) # 80200868 <strncmp>
    80207026:	c139                	beqz	a0,8020706c <dirlookup+0x80>
        return edup(dp);
    } else if (strncmp(filename, "..", FAT32_MAX_FILENAME) == 0) {
    80207028:	0ff00613          	li	a2,255
    8020702c:	00003597          	auipc	a1,0x3
    80207030:	fa458593          	addi	a1,a1,-92 # 80209fd0 <digits+0xc50>
    80207034:	854e                	mv	a0,s3
    80207036:	ffffa097          	auipc	ra,0xffffa
    8020703a:	832080e7          	jalr	-1998(ra) # 80200868 <strncmp>
    8020703e:	e125                	bnez	a0,8020709e <dirlookup+0xb2>
        if (dp == &root) {
    80207040:	00049797          	auipc	a5,0x49
    80207044:	69078793          	addi	a5,a5,1680 # 802506d0 <root>
    80207048:	04f48463          	beq	s1,a5,80207090 <dirlookup+0xa4>
            return edup(&root);
        }
        return edup(dp->parent);
    8020704c:	1204b503          	ld	a0,288(s1)
    80207050:	00000097          	auipc	ra,0x0
    80207054:	9b0080e7          	jalr	-1616(ra) # 80206a00 <edup>
    80207058:	892a                	mv	s2,a0
    8020705a:	a839                	j	80207078 <dirlookup+0x8c>
        panic("dirlookup not DIR");
    8020705c:	00003517          	auipc	a0,0x3
    80207060:	f5450513          	addi	a0,a0,-172 # 80209fb0 <digits+0xc30>
    80207064:	ffff9097          	auipc	ra,0xffff9
    80207068:	0e0080e7          	jalr	224(ra) # 80200144 <panic>
        return edup(dp);
    8020706c:	8526                	mv	a0,s1
    8020706e:	00000097          	auipc	ra,0x0
    80207072:	992080e7          	jalr	-1646(ra) # 80206a00 <edup>
    80207076:	892a                	mv	s2,a0
        *poff = off;
    }
    eput(ep);
    // printf("dirlookup end\n");
    return NULL;
}
    80207078:	854a                	mv	a0,s2
    8020707a:	60e6                	ld	ra,88(sp)
    8020707c:	6446                	ld	s0,80(sp)
    8020707e:	64a6                	ld	s1,72(sp)
    80207080:	6906                	ld	s2,64(sp)
    80207082:	79e2                	ld	s3,56(sp)
    80207084:	7a42                	ld	s4,48(sp)
    80207086:	7aa2                	ld	s5,40(sp)
    80207088:	7b02                	ld	s6,32(sp)
    8020708a:	6be2                	ld	s7,24(sp)
    8020708c:	6125                	addi	sp,sp,96
    8020708e:	8082                	ret
            return edup(&root);
    80207090:	853e                	mv	a0,a5
    80207092:	00000097          	auipc	ra,0x0
    80207096:	96e080e7          	jalr	-1682(ra) # 80206a00 <edup>
    8020709a:	892a                	mv	s2,a0
    8020709c:	bff1                	j	80207078 <dirlookup+0x8c>
    if (dp->valid != 1) {
    8020709e:	11649703          	lh	a4,278(s1)
    802070a2:	4785                	li	a5,1
        return NULL;
    802070a4:	4901                	li	s2,0
    if (dp->valid != 1) {
    802070a6:	fcf719e3          	bne	a4,a5,80207078 <dirlookup+0x8c>
    struct dirent *ep = eget(dp, filename);
    802070aa:	85ce                	mv	a1,s3
    802070ac:	8526                	mv	a0,s1
    802070ae:	fffff097          	auipc	ra,0xfffff
    802070b2:	e6a080e7          	jalr	-406(ra) # 80205f18 <eget>
    802070b6:	892a                	mv	s2,a0
    if (ep->valid == 1) { return ep; }                               // ecache hits
    802070b8:	11651703          	lh	a4,278(a0)
    802070bc:	4785                	li	a5,1
    802070be:	faf70de3          	beq	a4,a5,80207078 <dirlookup+0x8c>
    int len = strlen(filename);
    802070c2:	854e                	mv	a0,s3
    802070c4:	ffffa097          	auipc	ra,0xffffa
    802070c8:	850080e7          	jalr	-1968(ra) # 80200914 <strlen>
    int count = 0;
    802070cc:	fa042623          	sw	zero,-84(s0)
    reloc_clus(dp, 0, 0);
    802070d0:	4601                	li	a2,0
    802070d2:	4581                	li	a1,0
    802070d4:	8526                	mv	a0,s1
    802070d6:	fffff097          	auipc	ra,0xfffff
    802070da:	bee080e7          	jalr	-1042(ra) # 80205cc4 <reloc_clus>
    uint off = 0;
    802070de:	4a01                	li	s4,0
    while ((type = enext(dp, ep, off, &count) != -1)) {
    802070e0:	fac40b93          	addi	s7,s0,-84
    802070e4:	5b7d                	li	s6,-1
    802070e6:	86de                	mv	a3,s7
    802070e8:	8652                	mv	a2,s4
    802070ea:	85ca                	mv	a1,s2
    802070ec:	8526                	mv	a0,s1
    802070ee:	00000097          	auipc	ra,0x0
    802070f2:	d74080e7          	jalr	-652(ra) # 80206e62 <enext>
    802070f6:	03650f63          	beq	a0,s6,80207134 <dirlookup+0x148>
        } else if (strncmp(filename, ep->filename, FAT32_MAX_FILENAME) == 0) {
    802070fa:	0ff00613          	li	a2,255
    802070fe:	85ca                	mv	a1,s2
    80207100:	854e                	mv	a0,s3
    80207102:	ffff9097          	auipc	ra,0xffff9
    80207106:	766080e7          	jalr	1894(ra) # 80200868 <strncmp>
    8020710a:	c901                	beqz	a0,8020711a <dirlookup+0x12e>
        off += count << 5;
    8020710c:	fac42783          	lw	a5,-84(s0)
    80207110:	0057979b          	slliw	a5,a5,0x5
    80207114:	01478a3b          	addw	s4,a5,s4
    80207118:	b7f9                	j	802070e6 <dirlookup+0xfa>
            ep->parent = edup(dp);
    8020711a:	8526                	mv	a0,s1
    8020711c:	00000097          	auipc	ra,0x0
    80207120:	8e4080e7          	jalr	-1820(ra) # 80206a00 <edup>
    80207124:	12a93023          	sd	a0,288(s2)
            ep->off = off;
    80207128:	11492e23          	sw	s4,284(s2)
            ep->valid = 1;
    8020712c:	4785                	li	a5,1
    8020712e:	10f91b23          	sh	a5,278(s2)
            return ep;
    80207132:	b799                	j	80207078 <dirlookup+0x8c>
    if (poff) {
    80207134:	000a8463          	beqz	s5,8020713c <dirlookup+0x150>
        *poff = off;
    80207138:	014aa023          	sw	s4,0(s5)
    eput(ep);
    8020713c:	854a                	mv	a0,s2
    8020713e:	00000097          	auipc	ra,0x0
    80207142:	ba4080e7          	jalr	-1116(ra) # 80206ce2 <eput>
    return NULL;
    80207146:	4901                	li	s2,0
    80207148:	bf05                	j	80207078 <dirlookup+0x8c>

000000008020714a <ealloc>:
{
    8020714a:	715d                	addi	sp,sp,-80
    8020714c:	e486                	sd	ra,72(sp)
    8020714e:	e0a2                	sd	s0,64(sp)
    80207150:	fc26                	sd	s1,56(sp)
    80207152:	f84a                	sd	s2,48(sp)
    80207154:	f44e                	sd	s3,40(sp)
    80207156:	f052                	sd	s4,32(sp)
    80207158:	ec56                	sd	s5,24(sp)
    8020715a:	0880                	addi	s0,sp,80
    8020715c:	892a                	mv	s2,a0
    if (!(dp->attribute & ATTR_DIRECTORY)) {
    8020715e:	10054783          	lbu	a5,256(a0)
    80207162:	8bc1                	andi	a5,a5,16
    80207164:	c7b1                	beqz	a5,802071b0 <ealloc+0x66>
    80207166:	852e                	mv	a0,a1
    80207168:	8a32                	mv	s4,a2
    if (dp->valid != 1 || !(name = formatname(name))) {        // detect illegal character
    8020716a:	11691703          	lh	a4,278(s2)
    8020716e:	4785                	li	a5,1
        return NULL;
    80207170:	4481                	li	s1,0
    if (dp->valid != 1 || !(name = formatname(name))) {        // detect illegal character
    80207172:	02f71563          	bne	a4,a5,8020719c <ealloc+0x52>
    80207176:	fffff097          	auipc	ra,0xfffff
    8020717a:	408080e7          	jalr	1032(ra) # 8020657e <formatname>
    8020717e:	89aa                	mv	s3,a0
    80207180:	10050663          	beqz	a0,8020728c <ealloc+0x142>
    uint off = 0;
    80207184:	fa042e23          	sw	zero,-68(s0)
    if ((ep = dirlookup(dp, name, &off)) != 0) {      // entry exists
    80207188:	fbc40613          	addi	a2,s0,-68
    8020718c:	85aa                	mv	a1,a0
    8020718e:	854a                	mv	a0,s2
    80207190:	00000097          	auipc	ra,0x0
    80207194:	e5c080e7          	jalr	-420(ra) # 80206fec <dirlookup>
    80207198:	84aa                	mv	s1,a0
    8020719a:	c11d                	beqz	a0,802071c0 <ealloc+0x76>
}
    8020719c:	8526                	mv	a0,s1
    8020719e:	60a6                	ld	ra,72(sp)
    802071a0:	6406                	ld	s0,64(sp)
    802071a2:	74e2                	ld	s1,56(sp)
    802071a4:	7942                	ld	s2,48(sp)
    802071a6:	79a2                	ld	s3,40(sp)
    802071a8:	7a02                	ld	s4,32(sp)
    802071aa:	6ae2                	ld	s5,24(sp)
    802071ac:	6161                	addi	sp,sp,80
    802071ae:	8082                	ret
        panic("ealloc not dir");
    802071b0:	00003517          	auipc	a0,0x3
    802071b4:	e2850513          	addi	a0,a0,-472 # 80209fd8 <digits+0xc58>
    802071b8:	ffff9097          	auipc	ra,0xffff9
    802071bc:	f8c080e7          	jalr	-116(ra) # 80200144 <panic>
    ep = eget(dp, name);
    802071c0:	85ce                	mv	a1,s3
    802071c2:	854a                	mv	a0,s2
    802071c4:	fffff097          	auipc	ra,0xfffff
    802071c8:	d54080e7          	jalr	-684(ra) # 80205f18 <eget>
    802071cc:	84aa                	mv	s1,a0
    elock(ep);
    802071ce:	00000097          	auipc	ra,0x0
    802071d2:	a90080e7          	jalr	-1392(ra) # 80206c5e <elock>
    ep->attribute = attr;
    802071d6:	11448023          	sb	s4,256(s1)
    ep->file_size = 0;
    802071da:	1004a423          	sw	zero,264(s1)
    ep->first_clus = 0;
    802071de:	1004a223          	sw	zero,260(s1)
    ep->parent = edup(dp);
    802071e2:	854a                	mv	a0,s2
    802071e4:	00000097          	auipc	ra,0x0
    802071e8:	81c080e7          	jalr	-2020(ra) # 80206a00 <edup>
    802071ec:	12a4b023          	sd	a0,288(s1)
    ep->off = off;
    802071f0:	fbc42a83          	lw	s5,-68(s0)
    802071f4:	1154ae23          	sw	s5,284(s1)
    ep->clus_cnt = 0;
    802071f8:	1004a823          	sw	zero,272(s1)
    ep->cur_clus = 0;
    802071fc:	1004a623          	sw	zero,268(s1)
    ep->dirty = 0;
    80207200:	10048aa3          	sb	zero,277(s1)
    strncpy(ep->filename, name, FAT32_MAX_FILENAME);
    80207204:	0ff00613          	li	a2,255
    80207208:	85ce                	mv	a1,s3
    8020720a:	8526                	mv	a0,s1
    8020720c:	ffff9097          	auipc	ra,0xffff9
    80207210:	698080e7          	jalr	1688(ra) # 802008a4 <strncpy>
    ep->filename[FAT32_MAX_FILENAME] = '\0';
    80207214:	0e048fa3          	sb	zero,255(s1)
    if (attr == ATTR_DIRECTORY) {    // generate "." and ".." for ep
    80207218:	47c1                	li	a5,16
    8020721a:	02fa0863          	beq	s4,a5,8020724a <ealloc+0x100>
        ep->attribute |= ATTR_ARCHIVE;
    8020721e:	1004c783          	lbu	a5,256(s1)
    80207222:	0207e793          	ori	a5,a5,32
    80207226:	10f48023          	sb	a5,256(s1)
    emake(dp, ep, off);
    8020722a:	8656                	mv	a2,s5
    8020722c:	85a6                	mv	a1,s1
    8020722e:	854a                	mv	a0,s2
    80207230:	fffff097          	auipc	ra,0xfffff
    80207234:	406080e7          	jalr	1030(ra) # 80206636 <emake>
    ep->valid = 1;
    80207238:	4785                	li	a5,1
    8020723a:	10f49b23          	sh	a5,278(s1)
    eunlock(ep);
    8020723e:	8526                	mv	a0,s1
    80207240:	00000097          	auipc	ra,0x0
    80207244:	a54080e7          	jalr	-1452(ra) # 80206c94 <eunlock>
    return ep;
    80207248:	bf91                	j	8020719c <ealloc+0x52>
        ep->attribute |= ATTR_DIRECTORY;
    8020724a:	1004c783          	lbu	a5,256(s1)
    8020724e:	0107e793          	ori	a5,a5,16
    80207252:	10f48023          	sb	a5,256(s1)
        ep->cur_clus = ep->first_clus = alloc_clus(dp->dev);
    80207256:	11494503          	lbu	a0,276(s2)
    8020725a:	fffff097          	auipc	ra,0xfffff
    8020725e:	8a8080e7          	jalr	-1880(ra) # 80205b02 <alloc_clus>
    80207262:	2501                	sext.w	a0,a0
    80207264:	10a4a223          	sw	a0,260(s1)
    80207268:	10a4a623          	sw	a0,268(s1)
        emake(ep, ep, 0);
    8020726c:	4601                	li	a2,0
    8020726e:	85a6                	mv	a1,s1
    80207270:	8526                	mv	a0,s1
    80207272:	fffff097          	auipc	ra,0xfffff
    80207276:	3c4080e7          	jalr	964(ra) # 80206636 <emake>
        emake(ep, dp, 32);
    8020727a:	02000613          	li	a2,32
    8020727e:	85ca                	mv	a1,s2
    80207280:	8526                	mv	a0,s1
    80207282:	fffff097          	auipc	ra,0xfffff
    80207286:	3b4080e7          	jalr	948(ra) # 80206636 <emake>
    8020728a:	b745                	j	8020722a <ealloc+0xe0>
        return NULL;
    8020728c:	84aa                	mv	s1,a0
    8020728e:	b739                	j	8020719c <ealloc+0x52>

0000000080207290 <lookup_path>:
    return path;
}

// FAT32 version of namex in xv6's original file system.
static struct dirent *lookup_path(char *path, int parent, char *name)
{   
    80207290:	715d                	addi	sp,sp,-80
    80207292:	e486                	sd	ra,72(sp)
    80207294:	e0a2                	sd	s0,64(sp)
    80207296:	fc26                	sd	s1,56(sp)
    80207298:	f84a                	sd	s2,48(sp)
    8020729a:	f44e                	sd	s3,40(sp)
    8020729c:	f052                	sd	s4,32(sp)
    8020729e:	ec56                	sd	s5,24(sp)
    802072a0:	e85a                	sd	s6,16(sp)
    802072a2:	e45e                	sd	s7,8(sp)
    802072a4:	e062                	sd	s8,0(sp)
    802072a6:	0880                	addi	s0,sp,80
    802072a8:	892a                	mv	s2,a0
    802072aa:	8b2e                	mv	s6,a1
    802072ac:	8ab2                	mv	s5,a2
    // printf("path: %s\n", path);
    struct dirent *entry, *next;
    if (*path == '/') { // 绝对路径
    802072ae:	00054783          	lbu	a5,0(a0)
    802072b2:	02f00713          	li	a4,47
    802072b6:	02e78663          	beq	a5,a4,802072e2 <lookup_path+0x52>
        entry = edup(&root);
    } else if (*path != '\0') { // 相对路径
        entry = edup(myproc()->cwd);
    } else {
        return NULL;
    802072ba:	4a01                	li	s4,0
    } else if (*path != '\0') { // 相对路径
    802072bc:	cba1                	beqz	a5,8020730c <lookup_path+0x7c>
        entry = edup(myproc()->cwd);
    802072be:	ffffb097          	auipc	ra,0xffffb
    802072c2:	81a080e7          	jalr	-2022(ra) # 80201ad8 <myproc>
    802072c6:	44853503          	ld	a0,1096(a0)
    802072ca:	fffff097          	auipc	ra,0xfffff
    802072ce:	736080e7          	jalr	1846(ra) # 80206a00 <edup>
    802072d2:	8a2a                	mv	s4,a0
    while (*path == '/') {
    802072d4:	02f00993          	li	s3,47
    802072d8:	0ff00b93          	li	s7,255
    802072dc:	0ff00c13          	li	s8,255
    802072e0:	a0e5                	j	802073c8 <lookup_path+0x138>
        entry = edup(&root);
    802072e2:	00049517          	auipc	a0,0x49
    802072e6:	3ee50513          	addi	a0,a0,1006 # 802506d0 <root>
    802072ea:	fffff097          	auipc	ra,0xfffff
    802072ee:	716080e7          	jalr	1814(ra) # 80206a00 <edup>
    802072f2:	8a2a                	mv	s4,a0
    802072f4:	b7c5                	j	802072d4 <lookup_path+0x44>
    }
    while ((path = skipelem(path, name)) != 0) {    // 路径解析和文件搜索
        // printf("name: %s\n", name);
        elock(entry);
        if (!(entry->attribute & ATTR_DIRECTORY)) {
            eunlock(entry);
    802072f6:	8552                	mv	a0,s4
    802072f8:	00000097          	auipc	ra,0x0
    802072fc:	99c080e7          	jalr	-1636(ra) # 80206c94 <eunlock>
            eput(entry);
    80207300:	8552                	mv	a0,s4
    80207302:	00000097          	auipc	ra,0x0
    80207306:	9e0080e7          	jalr	-1568(ra) # 80206ce2 <eput>
            return NULL;
    8020730a:	4a01                	li	s4,0
        eput(entry);
        return NULL;
    }

    return entry;
}
    8020730c:	8552                	mv	a0,s4
    8020730e:	60a6                	ld	ra,72(sp)
    80207310:	6406                	ld	s0,64(sp)
    80207312:	74e2                	ld	s1,56(sp)
    80207314:	7942                	ld	s2,48(sp)
    80207316:	79a2                	ld	s3,40(sp)
    80207318:	7a02                	ld	s4,32(sp)
    8020731a:	6ae2                	ld	s5,24(sp)
    8020731c:	6b42                	ld	s6,16(sp)
    8020731e:	6ba2                	ld	s7,8(sp)
    80207320:	6c02                	ld	s8,0(sp)
    80207322:	6161                	addi	sp,sp,80
    80207324:	8082                	ret
            eunlock(entry);
    80207326:	8552                	mv	a0,s4
    80207328:	00000097          	auipc	ra,0x0
    8020732c:	96c080e7          	jalr	-1684(ra) # 80206c94 <eunlock>
            return entry;
    80207330:	bff1                	j	8020730c <lookup_path+0x7c>
            eunlock(entry);
    80207332:	8552                	mv	a0,s4
    80207334:	00000097          	auipc	ra,0x0
    80207338:	960080e7          	jalr	-1696(ra) # 80206c94 <eunlock>
            eput(entry);
    8020733c:	8552                	mv	a0,s4
    8020733e:	00000097          	auipc	ra,0x0
    80207342:	9a4080e7          	jalr	-1628(ra) # 80206ce2 <eput>
            return NULL;
    80207346:	8a4a                	mv	s4,s2
    80207348:	b7d1                	j	8020730c <lookup_path+0x7c>
    int len = path - s;
    8020734a:	412487b3          	sub	a5,s1,s2
    8020734e:	863e                	mv	a2,a5
    80207350:	2781                	sext.w	a5,a5
    80207352:	00fbd363          	bge	s7,a5,80207358 <lookup_path+0xc8>
    80207356:	8662                	mv	a2,s8
    80207358:	0006079b          	sext.w	a5,a2
    name[len] = 0;
    8020735c:	97d6                	add	a5,a5,s5
    8020735e:	00078023          	sb	zero,0(a5)
    memmove(name, s, len);
    80207362:	2601                	sext.w	a2,a2
    80207364:	85ca                	mv	a1,s2
    80207366:	8556                	mv	a0,s5
    80207368:	ffff9097          	auipc	ra,0xffff9
    8020736c:	484080e7          	jalr	1156(ra) # 802007ec <memmove>
    while (*path == '/') {
    80207370:	0004c783          	lbu	a5,0(s1)
    80207374:	01379763          	bne	a5,s3,80207382 <lookup_path+0xf2>
        path++;
    80207378:	0485                	addi	s1,s1,1
    while (*path == '/') {
    8020737a:	0004c783          	lbu	a5,0(s1)
    8020737e:	ff378de3          	beq	a5,s3,80207378 <lookup_path+0xe8>
        elock(entry);
    80207382:	8552                	mv	a0,s4
    80207384:	00000097          	auipc	ra,0x0
    80207388:	8da080e7          	jalr	-1830(ra) # 80206c5e <elock>
        if (!(entry->attribute & ATTR_DIRECTORY)) {
    8020738c:	100a4783          	lbu	a5,256(s4)
    80207390:	8bc1                	andi	a5,a5,16
    80207392:	d3b5                	beqz	a5,802072f6 <lookup_path+0x66>
        if (parent && *path == '\0') {
    80207394:	000b0563          	beqz	s6,8020739e <lookup_path+0x10e>
    80207398:	0004c783          	lbu	a5,0(s1)
    8020739c:	d7c9                	beqz	a5,80207326 <lookup_path+0x96>
        if ((next = dirlookup(entry, name, 0)) == 0) {  // buggy
    8020739e:	4601                	li	a2,0
    802073a0:	85d6                	mv	a1,s5
    802073a2:	8552                	mv	a0,s4
    802073a4:	00000097          	auipc	ra,0x0
    802073a8:	c48080e7          	jalr	-952(ra) # 80206fec <dirlookup>
    802073ac:	892a                	mv	s2,a0
    802073ae:	d151                	beqz	a0,80207332 <lookup_path+0xa2>
        eunlock(entry);
    802073b0:	8552                	mv	a0,s4
    802073b2:	00000097          	auipc	ra,0x0
    802073b6:	8e2080e7          	jalr	-1822(ra) # 80206c94 <eunlock>
        eput(entry);
    802073ba:	8552                	mv	a0,s4
    802073bc:	00000097          	auipc	ra,0x0
    802073c0:	926080e7          	jalr	-1754(ra) # 80206ce2 <eput>
        entry = next;
    802073c4:	8a4a                	mv	s4,s2
        eput(entry);
    802073c6:	8926                	mv	s2,s1
    while (*path == '/') {
    802073c8:	00094783          	lbu	a5,0(s2)
    802073cc:	03379363          	bne	a5,s3,802073f2 <lookup_path+0x162>
        path++;
    802073d0:	0905                	addi	s2,s2,1
    while (*path == '/') {
    802073d2:	00094783          	lbu	a5,0(s2)
    802073d6:	ff378de3          	beq	a5,s3,802073d0 <lookup_path+0x140>
    if (*path == 0) { return NULL; }
    802073da:	cf89                	beqz	a5,802073f4 <lookup_path+0x164>
        path++;
    802073dc:	84ca                	mv	s1,s2
    while (*path != '/' && *path != 0) {
    802073de:	f73786e3          	beq	a5,s3,8020734a <lookup_path+0xba>
        eput(entry);
    802073e2:	84ca                	mv	s1,s2
    while (*path != '/' && *path != 0) {
    802073e4:	d3bd                	beqz	a5,8020734a <lookup_path+0xba>
        path++;
    802073e6:	0485                	addi	s1,s1,1
    while (*path != '/' && *path != 0) {
    802073e8:	0004c783          	lbu	a5,0(s1)
    802073ec:	ff379ce3          	bne	a5,s3,802073e4 <lookup_path+0x154>
    802073f0:	bfa9                	j	8020734a <lookup_path+0xba>
    if (*path == 0) { return NULL; }
    802073f2:	fbe5                	bnez	a5,802073e2 <lookup_path+0x152>
    if (parent) {
    802073f4:	f00b0ce3          	beqz	s6,8020730c <lookup_path+0x7c>
        eput(entry);
    802073f8:	8552                	mv	a0,s4
    802073fa:	00000097          	auipc	ra,0x0
    802073fe:	8e8080e7          	jalr	-1816(ra) # 80206ce2 <eput>
        return NULL;
    80207402:	4a01                	li	s4,0
    80207404:	b721                	j	8020730c <lookup_path+0x7c>

0000000080207406 <ename>:

struct dirent *ename(char *path)
{
    80207406:	716d                	addi	sp,sp,-272
    80207408:	e606                	sd	ra,264(sp)
    8020740a:	e222                	sd	s0,256(sp)
    8020740c:	0a00                	addi	s0,sp,272
    char name[FAT32_MAX_FILENAME + 1];
    return lookup_path(path, 0, name);
    8020740e:	ef040613          	addi	a2,s0,-272
    80207412:	4581                	li	a1,0
    80207414:	00000097          	auipc	ra,0x0
    80207418:	e7c080e7          	jalr	-388(ra) # 80207290 <lookup_path>
}
    8020741c:	60b2                	ld	ra,264(sp)
    8020741e:	6412                	ld	s0,256(sp)
    80207420:	6151                	addi	sp,sp,272
    80207422:	8082                	ret

0000000080207424 <enameparent>:

struct dirent *enameparent(char *path, char *name)
{
    80207424:	1141                	addi	sp,sp,-16
    80207426:	e406                	sd	ra,8(sp)
    80207428:	e022                	sd	s0,0(sp)
    8020742a:	0800                	addi	s0,sp,16
    8020742c:	862e                	mv	a2,a1
    return lookup_path(path, 1, name);
    8020742e:	4585                	li	a1,1
    80207430:	00000097          	auipc	ra,0x0
    80207434:	e60080e7          	jalr	-416(ra) # 80207290 <lookup_path>
}
    80207438:	60a2                	ld	ra,8(sp)
    8020743a:	6402                	ld	s0,0(sp)
    8020743c:	0141                	addi	sp,sp,16
    8020743e:	8082                	ret

0000000080207440 <plicinit>:

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

void plicinit(void) {
    80207440:	1141                	addi	sp,sp,-16
    80207442:	e406                	sd	ra,8(sp)
    80207444:	e022                	sd	s0,0(sp)
    80207446:	0800                	addi	s0,sp,16
	writed(1, PLIC_V + DISK_IRQ * sizeof(uint32));
    80207448:	00fc37b7          	lui	a5,0xfc3
    8020744c:	07ba                	slli	a5,a5,0xe
    8020744e:	4705                	li	a4,1
    80207450:	c3d8                	sw	a4,4(a5)
	writed(1, PLIC_V + UART_IRQ * sizeof(uint32));
    80207452:	d798                	sw	a4,40(a5)

	#ifdef DEBUG 
	printf("plicinit\n");
    80207454:	00003517          	auipc	a0,0x3
    80207458:	b9450513          	addi	a0,a0,-1132 # 80209fe8 <digits+0xc68>
    8020745c:	ffff9097          	auipc	ra,0xffff9
    80207460:	d32080e7          	jalr	-718(ra) # 8020018e <printf>
	#endif 
}
    80207464:	60a2                	ld	ra,8(sp)
    80207466:	6402                	ld	s0,0(sp)
    80207468:	0141                	addi	sp,sp,16
    8020746a:	8082                	ret

000000008020746c <plicinithart>:

void
plicinithart(void)
{
    8020746c:	1141                	addi	sp,sp,-16
    8020746e:	e406                	sd	ra,8(sp)
    80207470:	e022                	sd	s0,0(sp)
    80207472:	0800                	addi	s0,sp,16
  int hart = cpuid();
    80207474:	ffffa097          	auipc	ra,0xffffa
    80207478:	638080e7          	jalr	1592(ra) # 80201aac <cpuid>
  #ifdef QEMU
  // set uart's enable bit for this hart's S-mode. 
  *(uint32*)PLIC_SENABLE(hart)= (1 << UART_IRQ) | (1 << DISK_IRQ);
    8020747c:	0085171b          	slliw	a4,a0,0x8
    80207480:	01f867b7          	lui	a5,0x1f86
    80207484:	0785                	addi	a5,a5,1 # 1f86001 <_entry-0x7e279fff>
    80207486:	07b6                	slli	a5,a5,0xd
    80207488:	97ba                	add	a5,a5,a4
    8020748a:	40200713          	li	a4,1026
    8020748e:	08e7a023          	sw	a4,128(a5)
  // set this hart's S-mode priority threshold to 0.
  *(uint32*)PLIC_SPRIORITY(hart) = 0;
    80207492:	00d5151b          	slliw	a0,a0,0xd
    80207496:	03f0c7b7          	lui	a5,0x3f0c
    8020749a:	20178793          	addi	a5,a5,513 # 3f0c201 <_entry-0x7c2f3dff>
    8020749e:	07b2                	slli	a5,a5,0xc
    802074a0:	97aa                	add	a5,a5,a0
    802074a2:	0007a023          	sw	zero,0(a5)
  *(hart_m_enable) = readd(hart_m_enable) | (1 << DISK_IRQ);
  uint32 *hart0_m_int_enable_hi = hart_m_enable + 1;
  *(hart0_m_int_enable_hi) = readd(hart0_m_int_enable_hi) | (1 << (UART_IRQ % 32));
  #endif
  #ifdef DEBUG
  printf("plicinithart\n");
    802074a6:	00003517          	auipc	a0,0x3
    802074aa:	b5250513          	addi	a0,a0,-1198 # 80209ff8 <digits+0xc78>
    802074ae:	ffff9097          	auipc	ra,0xffff9
    802074b2:	ce0080e7          	jalr	-800(ra) # 8020018e <printf>
  #endif
}
    802074b6:	60a2                	ld	ra,8(sp)
    802074b8:	6402                	ld	s0,0(sp)
    802074ba:	0141                	addi	sp,sp,16
    802074bc:	8082                	ret

00000000802074be <plic_claim>:

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
    802074be:	1141                	addi	sp,sp,-16
    802074c0:	e406                	sd	ra,8(sp)
    802074c2:	e022                	sd	s0,0(sp)
    802074c4:	0800                	addi	s0,sp,16
  int hart = cpuid();
    802074c6:	ffffa097          	auipc	ra,0xffffa
    802074ca:	5e6080e7          	jalr	1510(ra) # 80201aac <cpuid>
  int irq;
  #ifndef QEMU
  irq = *(uint32*)PLIC_MCLAIM(hart);
  #else
  irq = *(uint32*)PLIC_SCLAIM(hart);
    802074ce:	00d5151b          	slliw	a0,a0,0xd
    802074d2:	03f0c7b7          	lui	a5,0x3f0c
    802074d6:	20178793          	addi	a5,a5,513 # 3f0c201 <_entry-0x7c2f3dff>
    802074da:	07b2                	slli	a5,a5,0xc
    802074dc:	97aa                	add	a5,a5,a0
  #endif
  return irq;
}
    802074de:	43c8                	lw	a0,4(a5)
    802074e0:	60a2                	ld	ra,8(sp)
    802074e2:	6402                	ld	s0,0(sp)
    802074e4:	0141                	addi	sp,sp,16
    802074e6:	8082                	ret

00000000802074e8 <plic_complete>:

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
    802074e8:	1101                	addi	sp,sp,-32
    802074ea:	ec06                	sd	ra,24(sp)
    802074ec:	e822                	sd	s0,16(sp)
    802074ee:	e426                	sd	s1,8(sp)
    802074f0:	1000                	addi	s0,sp,32
    802074f2:	84aa                	mv	s1,a0
  int hart = cpuid();
    802074f4:	ffffa097          	auipc	ra,0xffffa
    802074f8:	5b8080e7          	jalr	1464(ra) # 80201aac <cpuid>
  #ifndef QEMU
  *(uint32*)PLIC_MCLAIM(hart) = irq;
  #else
  *(uint32*)PLIC_SCLAIM(hart) = irq;
    802074fc:	00d5151b          	slliw	a0,a0,0xd
    80207500:	03f0c7b7          	lui	a5,0x3f0c
    80207504:	20178793          	addi	a5,a5,513 # 3f0c201 <_entry-0x7c2f3dff>
    80207508:	07b2                	slli	a5,a5,0xc
    8020750a:	97aa                	add	a5,a5,a0
    8020750c:	c3c4                	sw	s1,4(a5)
  #endif
}
    8020750e:	60e2                	ld	ra,24(sp)
    80207510:	6442                	ld	s0,16(sp)
    80207512:	64a2                	ld	s1,8(sp)
    80207514:	6105                	addi	sp,sp,32
    80207516:	8082                	ret

0000000080207518 <consolewrite>:
//
// user write()s to the console go here.
//
int
consolewrite(int user_src, uint64 src, int n)
{
    80207518:	715d                	addi	sp,sp,-80
    8020751a:	e486                	sd	ra,72(sp)
    8020751c:	e0a2                	sd	s0,64(sp)
    8020751e:	fc26                	sd	s1,56(sp)
    80207520:	f84a                	sd	s2,48(sp)
    80207522:	f44e                	sd	s3,40(sp)
    80207524:	f052                	sd	s4,32(sp)
    80207526:	ec56                	sd	s5,24(sp)
    80207528:	e85a                	sd	s6,16(sp)
    8020752a:	0880                	addi	s0,sp,80
    8020752c:	8a2a                	mv	s4,a0
    8020752e:	84ae                	mv	s1,a1
    80207530:	89b2                	mv	s3,a2
  int i;

  acquire(&cons.lock);
    80207532:	0004e517          	auipc	a0,0x4e
    80207536:	96e50513          	addi	a0,a0,-1682 # 80254ea0 <cons>
    8020753a:	ffff9097          	auipc	ra,0xffff9
    8020753e:	1ba080e7          	jalr	442(ra) # 802006f4 <acquire>
  for(i = 0; i < n; i++){
    80207542:	07305063          	blez	s3,802075a2 <consolewrite+0x8a>
    80207546:	4901                	li	s2,0
    char c;
    if(either_copyin(&c, user_src, src+i, 1) == -1)
    80207548:	fbf40b13          	addi	s6,s0,-65
    8020754c:	5afd                	li	s5,-1
    8020754e:	4685                	li	a3,1
    80207550:	8626                	mv	a2,s1
    80207552:	85d2                	mv	a1,s4
    80207554:	855a                	mv	a0,s6
    80207556:	ffffb097          	auipc	ra,0xffffb
    8020755a:	1f2080e7          	jalr	498(ra) # 80202748 <either_copyin>
    8020755e:	01550f63          	beq	a0,s5,8020757c <consolewrite+0x64>
	SBI_CALL_1(SBI_CONSOLE_PUTCHAR, ch);
    80207562:	fbf44503          	lbu	a0,-65(s0)
    80207566:	4581                	li	a1,0
    80207568:	4601                	li	a2,0
    8020756a:	4681                	li	a3,0
    8020756c:	4885                	li	a7,1
    8020756e:	00000073          	ecall
  for(i = 0; i < n; i++){
    80207572:	2905                	addiw	s2,s2,1
    80207574:	0485                	addi	s1,s1,1
    80207576:	fd299ce3          	bne	s3,s2,8020754e <consolewrite+0x36>
    8020757a:	894e                	mv	s2,s3
      break;
    sbi_console_putchar(c);
  }
  release(&cons.lock);
    8020757c:	0004e517          	auipc	a0,0x4e
    80207580:	92450513          	addi	a0,a0,-1756 # 80254ea0 <cons>
    80207584:	ffff9097          	auipc	ra,0xffff9
    80207588:	1c4080e7          	jalr	452(ra) # 80200748 <release>

  return i;
}
    8020758c:	854a                	mv	a0,s2
    8020758e:	60a6                	ld	ra,72(sp)
    80207590:	6406                	ld	s0,64(sp)
    80207592:	74e2                	ld	s1,56(sp)
    80207594:	7942                	ld	s2,48(sp)
    80207596:	79a2                	ld	s3,40(sp)
    80207598:	7a02                	ld	s4,32(sp)
    8020759a:	6ae2                	ld	s5,24(sp)
    8020759c:	6b42                	ld	s6,16(sp)
    8020759e:	6161                	addi	sp,sp,80
    802075a0:	8082                	ret
  for(i = 0; i < n; i++){
    802075a2:	4901                	li	s2,0
    802075a4:	bfe1                	j	8020757c <consolewrite+0x64>

00000000802075a6 <consoleread>:
// user_dist indicates whether dst is a user
// or kernel address.
//
int
consoleread(int user_dst, uint64 dst, int n)
{
    802075a6:	7119                	addi	sp,sp,-128
    802075a8:	fc86                	sd	ra,120(sp)
    802075aa:	f8a2                	sd	s0,112(sp)
    802075ac:	f4a6                	sd	s1,104(sp)
    802075ae:	f0ca                	sd	s2,96(sp)
    802075b0:	ecce                	sd	s3,88(sp)
    802075b2:	e8d2                	sd	s4,80(sp)
    802075b4:	e4d6                	sd	s5,72(sp)
    802075b6:	e0da                	sd	s6,64(sp)
    802075b8:	fc5e                	sd	s7,56(sp)
    802075ba:	f862                	sd	s8,48(sp)
    802075bc:	f466                	sd	s9,40(sp)
    802075be:	f06a                	sd	s10,32(sp)
    802075c0:	ec6e                	sd	s11,24(sp)
    802075c2:	0100                	addi	s0,sp,128
    802075c4:	8aaa                	mv	s5,a0
    802075c6:	8a2e                	mv	s4,a1
    802075c8:	89b2                	mv	s3,a2
  uint target;
  int c;
  char cbuf;

  target = n;
    802075ca:	00060b1b          	sext.w	s6,a2
  acquire(&cons.lock);
    802075ce:	0004e517          	auipc	a0,0x4e
    802075d2:	8d250513          	addi	a0,a0,-1838 # 80254ea0 <cons>
    802075d6:	ffff9097          	auipc	ra,0xffff9
    802075da:	11e080e7          	jalr	286(ra) # 802006f4 <acquire>
  while(n > 0){
    // wait until interrupt handler has put some
    // input into cons.buffer.
    while(cons.r == cons.w){
    802075de:	0004e497          	auipc	s1,0x4e
    802075e2:	8c248493          	addi	s1,s1,-1854 # 80254ea0 <cons>
      if(myproc()->killed){
        release(&cons.lock);
        return -1;
      }
      sleep(&cons.r, &cons.lock);
    802075e6:	0004e917          	auipc	s2,0x4e
    802075ea:	95290913          	addi	s2,s2,-1710 # 80254f38 <cons+0x98>
    }

    c = cons.buf[cons.r++ % INPUT_BUF];

    if(c == C('D')){  // end-of-file
    802075ee:	4c11                	li	s8,4
      break;
    }

    // copy the input byte to the user-space buffer.
    cbuf = c;
    if(either_copyout(user_dst, dst, &cbuf, 1) == -1)
    802075f0:	f8f40d13          	addi	s10,s0,-113
    802075f4:	5cfd                	li	s9,-1
      break;

    dst++;
    --n;

    if(c == '\n'){
    802075f6:	4da9                	li	s11,10
  while(n > 0){
    802075f8:	07305763          	blez	s3,80207666 <consoleread+0xc0>
    while(cons.r == cons.w){
    802075fc:	0984a783          	lw	a5,152(s1)
    80207600:	09c4a703          	lw	a4,156(s1)
    80207604:	02f71463          	bne	a4,a5,8020762c <consoleread+0x86>
      if(myproc()->killed){
    80207608:	ffffa097          	auipc	ra,0xffffa
    8020760c:	4d0080e7          	jalr	1232(ra) # 80201ad8 <myproc>
    80207610:	591c                	lw	a5,48(a0)
    80207612:	e7ad                	bnez	a5,8020767c <consoleread+0xd6>
      sleep(&cons.r, &cons.lock);
    80207614:	85a6                	mv	a1,s1
    80207616:	854a                	mv	a0,s2
    80207618:	ffffb097          	auipc	ra,0xffffb
    8020761c:	e88080e7          	jalr	-376(ra) # 802024a0 <sleep>
    while(cons.r == cons.w){
    80207620:	0984a783          	lw	a5,152(s1)
    80207624:	09c4a703          	lw	a4,156(s1)
    80207628:	fef700e3          	beq	a4,a5,80207608 <consoleread+0x62>
    c = cons.buf[cons.r++ % INPUT_BUF];
    8020762c:	0017871b          	addiw	a4,a5,1
    80207630:	08e4ac23          	sw	a4,152(s1)
    80207634:	07f7f713          	andi	a4,a5,127
    80207638:	9726                	add	a4,a4,s1
    8020763a:	01874703          	lbu	a4,24(a4)
    8020763e:	00070b9b          	sext.w	s7,a4
    if(c == C('D')){  // end-of-file
    80207642:	078b8563          	beq	s7,s8,802076ac <consoleread+0x106>
    cbuf = c;
    80207646:	f8e407a3          	sb	a4,-113(s0)
    if(either_copyout(user_dst, dst, &cbuf, 1) == -1)
    8020764a:	4685                	li	a3,1
    8020764c:	866a                	mv	a2,s10
    8020764e:	85d2                	mv	a1,s4
    80207650:	8556                	mv	a0,s5
    80207652:	ffffb097          	auipc	ra,0xffffb
    80207656:	0c0080e7          	jalr	192(ra) # 80202712 <either_copyout>
    8020765a:	01950663          	beq	a0,s9,80207666 <consoleread+0xc0>
    dst++;
    8020765e:	0a05                	addi	s4,s4,1
    --n;
    80207660:	39fd                	addiw	s3,s3,-1
    if(c == '\n'){
    80207662:	f9bb9be3          	bne	s7,s11,802075f8 <consoleread+0x52>
      // a whole line has arrived, return to
      // the user-level read().
      break;
    }
  }
  release(&cons.lock);
    80207666:	0004e517          	auipc	a0,0x4e
    8020766a:	83a50513          	addi	a0,a0,-1990 # 80254ea0 <cons>
    8020766e:	ffff9097          	auipc	ra,0xffff9
    80207672:	0da080e7          	jalr	218(ra) # 80200748 <release>

  return target - n;
    80207676:	413b053b          	subw	a0,s6,s3
    8020767a:	a811                	j	8020768e <consoleread+0xe8>
        release(&cons.lock);
    8020767c:	0004e517          	auipc	a0,0x4e
    80207680:	82450513          	addi	a0,a0,-2012 # 80254ea0 <cons>
    80207684:	ffff9097          	auipc	ra,0xffff9
    80207688:	0c4080e7          	jalr	196(ra) # 80200748 <release>
        return -1;
    8020768c:	557d                	li	a0,-1
}
    8020768e:	70e6                	ld	ra,120(sp)
    80207690:	7446                	ld	s0,112(sp)
    80207692:	74a6                	ld	s1,104(sp)
    80207694:	7906                	ld	s2,96(sp)
    80207696:	69e6                	ld	s3,88(sp)
    80207698:	6a46                	ld	s4,80(sp)
    8020769a:	6aa6                	ld	s5,72(sp)
    8020769c:	6b06                	ld	s6,64(sp)
    8020769e:	7be2                	ld	s7,56(sp)
    802076a0:	7c42                	ld	s8,48(sp)
    802076a2:	7ca2                	ld	s9,40(sp)
    802076a4:	7d02                	ld	s10,32(sp)
    802076a6:	6de2                	ld	s11,24(sp)
    802076a8:	6109                	addi	sp,sp,128
    802076aa:	8082                	ret
      if(n < target){
    802076ac:	0009871b          	sext.w	a4,s3
    802076b0:	fb677be3          	bgeu	a4,s6,80207666 <consoleread+0xc0>
        cons.r--;
    802076b4:	0004e717          	auipc	a4,0x4e
    802076b8:	88f72223          	sw	a5,-1916(a4) # 80254f38 <cons+0x98>
    802076bc:	b76d                	j	80207666 <consoleread+0xc0>

00000000802076be <consputc>:
void consputc(int c) {
    802076be:	1141                	addi	sp,sp,-16
    802076c0:	e422                	sd	s0,8(sp)
    802076c2:	0800                	addi	s0,sp,16
  if(c == BACKSPACE){
    802076c4:	10000793          	li	a5,256
    802076c8:	00f50b63          	beq	a0,a5,802076de <consputc+0x20>
    802076cc:	4581                	li	a1,0
    802076ce:	4601                	li	a2,0
    802076d0:	4681                	li	a3,0
    802076d2:	4885                	li	a7,1
    802076d4:	00000073          	ecall
}
    802076d8:	6422                	ld	s0,8(sp)
    802076da:	0141                	addi	sp,sp,16
    802076dc:	8082                	ret
    802076de:	4521                	li	a0,8
    802076e0:	4581                	li	a1,0
    802076e2:	4601                	li	a2,0
    802076e4:	4681                	li	a3,0
    802076e6:	4885                	li	a7,1
    802076e8:	00000073          	ecall
    802076ec:	02000513          	li	a0,32
    802076f0:	00000073          	ecall
    802076f4:	4521                	li	a0,8
    802076f6:	00000073          	ecall
}
    802076fa:	bff9                	j	802076d8 <consputc+0x1a>

00000000802076fc <consoleintr>:
// do erase/kill processing, append to cons.buf,
// wake up consoleread() if a whole line has arrived.
//
void
consoleintr(int c)
{
    802076fc:	1101                	addi	sp,sp,-32
    802076fe:	ec06                	sd	ra,24(sp)
    80207700:	e822                	sd	s0,16(sp)
    80207702:	e426                	sd	s1,8(sp)
    80207704:	e04a                	sd	s2,0(sp)
    80207706:	1000                	addi	s0,sp,32
    80207708:	84aa                	mv	s1,a0
  acquire(&cons.lock);
    8020770a:	0004d517          	auipc	a0,0x4d
    8020770e:	79650513          	addi	a0,a0,1942 # 80254ea0 <cons>
    80207712:	ffff9097          	auipc	ra,0xffff9
    80207716:	fe2080e7          	jalr	-30(ra) # 802006f4 <acquire>

  switch(c){
    8020771a:	47d5                	li	a5,21
    8020771c:	0af48663          	beq	s1,a5,802077c8 <consoleintr+0xcc>
    80207720:	0297ca63          	blt	a5,s1,80207754 <consoleintr+0x58>
    80207724:	47a1                	li	a5,8
    80207726:	0ef48763          	beq	s1,a5,80207814 <consoleintr+0x118>
    8020772a:	47c1                	li	a5,16
    8020772c:	10f49a63          	bne	s1,a5,80207840 <consoleintr+0x144>
  case C('P'):  // Print process list.
    procdump();
    80207730:	ffffb097          	auipc	ra,0xffffb
    80207734:	04c080e7          	jalr	76(ra) # 8020277c <procdump>
      }
    }
    break;
  }
  
  release(&cons.lock);
    80207738:	0004d517          	auipc	a0,0x4d
    8020773c:	76850513          	addi	a0,a0,1896 # 80254ea0 <cons>
    80207740:	ffff9097          	auipc	ra,0xffff9
    80207744:	008080e7          	jalr	8(ra) # 80200748 <release>
}
    80207748:	60e2                	ld	ra,24(sp)
    8020774a:	6442                	ld	s0,16(sp)
    8020774c:	64a2                	ld	s1,8(sp)
    8020774e:	6902                	ld	s2,0(sp)
    80207750:	6105                	addi	sp,sp,32
    80207752:	8082                	ret
  switch(c){
    80207754:	07f00793          	li	a5,127
    80207758:	0af48e63          	beq	s1,a5,80207814 <consoleintr+0x118>
    if(c != 0 && cons.e-cons.r < INPUT_BUF){
    8020775c:	0004d717          	auipc	a4,0x4d
    80207760:	74470713          	addi	a4,a4,1860 # 80254ea0 <cons>
    80207764:	0a072783          	lw	a5,160(a4)
    80207768:	09872703          	lw	a4,152(a4)
    8020776c:	9f99                	subw	a5,a5,a4
    8020776e:	07f00713          	li	a4,127
    80207772:	fcf763e3          	bltu	a4,a5,80207738 <consoleintr+0x3c>
      c = (c == '\r') ? '\n' : c;
    80207776:	47b5                	li	a5,13
    80207778:	0cf48763          	beq	s1,a5,80207846 <consoleintr+0x14a>
      consputc(c);
    8020777c:	8526                	mv	a0,s1
    8020777e:	00000097          	auipc	ra,0x0
    80207782:	f40080e7          	jalr	-192(ra) # 802076be <consputc>
      cons.buf[cons.e++ % INPUT_BUF] = c;
    80207786:	0004d797          	auipc	a5,0x4d
    8020778a:	71a78793          	addi	a5,a5,1818 # 80254ea0 <cons>
    8020778e:	0a07a703          	lw	a4,160(a5)
    80207792:	0017069b          	addiw	a3,a4,1
    80207796:	0006861b          	sext.w	a2,a3
    8020779a:	0ad7a023          	sw	a3,160(a5)
    8020779e:	07f77713          	andi	a4,a4,127
    802077a2:	97ba                	add	a5,a5,a4
    802077a4:	00978c23          	sb	s1,24(a5)
      if(c == '\n' || c == C('D') || cons.e == cons.r+INPUT_BUF){
    802077a8:	47a9                	li	a5,10
    802077aa:	0cf48563          	beq	s1,a5,80207874 <consoleintr+0x178>
    802077ae:	4791                	li	a5,4
    802077b0:	0cf48263          	beq	s1,a5,80207874 <consoleintr+0x178>
    802077b4:	0004d797          	auipc	a5,0x4d
    802077b8:	7847a783          	lw	a5,1924(a5) # 80254f38 <cons+0x98>
    802077bc:	0807879b          	addiw	a5,a5,128
    802077c0:	f6f61ce3          	bne	a2,a5,80207738 <consoleintr+0x3c>
      cons.buf[cons.e++ % INPUT_BUF] = c;
    802077c4:	863e                	mv	a2,a5
    802077c6:	a07d                	j	80207874 <consoleintr+0x178>
    while(cons.e != cons.w &&
    802077c8:	0004d717          	auipc	a4,0x4d
    802077cc:	6d870713          	addi	a4,a4,1752 # 80254ea0 <cons>
    802077d0:	0a072783          	lw	a5,160(a4)
    802077d4:	09c72703          	lw	a4,156(a4)
          cons.buf[(cons.e-1) % INPUT_BUF] != '\n'){
    802077d8:	0004d497          	auipc	s1,0x4d
    802077dc:	6c848493          	addi	s1,s1,1736 # 80254ea0 <cons>
    while(cons.e != cons.w &&
    802077e0:	4929                	li	s2,10
    802077e2:	f4f70be3          	beq	a4,a5,80207738 <consoleintr+0x3c>
          cons.buf[(cons.e-1) % INPUT_BUF] != '\n'){
    802077e6:	37fd                	addiw	a5,a5,-1
    802077e8:	07f7f713          	andi	a4,a5,127
    802077ec:	9726                	add	a4,a4,s1
    while(cons.e != cons.w &&
    802077ee:	01874703          	lbu	a4,24(a4)
    802077f2:	f52703e3          	beq	a4,s2,80207738 <consoleintr+0x3c>
      cons.e--;
    802077f6:	0af4a023          	sw	a5,160(s1)
      consputc(BACKSPACE);
    802077fa:	10000513          	li	a0,256
    802077fe:	00000097          	auipc	ra,0x0
    80207802:	ec0080e7          	jalr	-320(ra) # 802076be <consputc>
    while(cons.e != cons.w &&
    80207806:	0a04a783          	lw	a5,160(s1)
    8020780a:	09c4a703          	lw	a4,156(s1)
    8020780e:	fcf71ce3          	bne	a4,a5,802077e6 <consoleintr+0xea>
    80207812:	b71d                	j	80207738 <consoleintr+0x3c>
    if(cons.e != cons.w){
    80207814:	0004d717          	auipc	a4,0x4d
    80207818:	68c70713          	addi	a4,a4,1676 # 80254ea0 <cons>
    8020781c:	0a072783          	lw	a5,160(a4)
    80207820:	09c72703          	lw	a4,156(a4)
    80207824:	f0f70ae3          	beq	a4,a5,80207738 <consoleintr+0x3c>
      cons.e--;
    80207828:	37fd                	addiw	a5,a5,-1
    8020782a:	0004d717          	auipc	a4,0x4d
    8020782e:	70f72b23          	sw	a5,1814(a4) # 80254f40 <cons+0xa0>
      consputc(BACKSPACE);
    80207832:	10000513          	li	a0,256
    80207836:	00000097          	auipc	ra,0x0
    8020783a:	e88080e7          	jalr	-376(ra) # 802076be <consputc>
    8020783e:	bded                	j	80207738 <consoleintr+0x3c>
    if(c != 0 && cons.e-cons.r < INPUT_BUF){
    80207840:	ee048ce3          	beqz	s1,80207738 <consoleintr+0x3c>
    80207844:	bf21                	j	8020775c <consoleintr+0x60>
      consputc(c);
    80207846:	4529                	li	a0,10
    80207848:	00000097          	auipc	ra,0x0
    8020784c:	e76080e7          	jalr	-394(ra) # 802076be <consputc>
      cons.buf[cons.e++ % INPUT_BUF] = c;
    80207850:	0004d797          	auipc	a5,0x4d
    80207854:	65078793          	addi	a5,a5,1616 # 80254ea0 <cons>
    80207858:	0a07a703          	lw	a4,160(a5)
    8020785c:	0017069b          	addiw	a3,a4,1
    80207860:	0006861b          	sext.w	a2,a3
    80207864:	0ad7a023          	sw	a3,160(a5)
    80207868:	07f77713          	andi	a4,a4,127
    8020786c:	97ba                	add	a5,a5,a4
    8020786e:	4729                	li	a4,10
    80207870:	00e78c23          	sb	a4,24(a5)
        cons.w = cons.e;
    80207874:	0004d797          	auipc	a5,0x4d
    80207878:	6cc7a423          	sw	a2,1736(a5) # 80254f3c <cons+0x9c>
        wakeup(&cons.r);
    8020787c:	0004d517          	auipc	a0,0x4d
    80207880:	6bc50513          	addi	a0,a0,1724 # 80254f38 <cons+0x98>
    80207884:	ffffb097          	auipc	ra,0xffffb
    80207888:	db4080e7          	jalr	-588(ra) # 80202638 <wakeup>
    8020788c:	b575                	j	80207738 <consoleintr+0x3c>

000000008020788e <consoleinit>:

void
consoleinit(void)
{
    8020788e:	1101                	addi	sp,sp,-32
    80207890:	ec06                	sd	ra,24(sp)
    80207892:	e822                	sd	s0,16(sp)
    80207894:	e426                	sd	s1,8(sp)
    80207896:	1000                	addi	s0,sp,32
  initlock(&cons.lock, "cons");
    80207898:	0004d497          	auipc	s1,0x4d
    8020789c:	60848493          	addi	s1,s1,1544 # 80254ea0 <cons>
    802078a0:	00002597          	auipc	a1,0x2
    802078a4:	76858593          	addi	a1,a1,1896 # 8020a008 <digits+0xc88>
    802078a8:	8526                	mv	a0,s1
    802078aa:	ffff9097          	auipc	ra,0xffff9
    802078ae:	e06080e7          	jalr	-506(ra) # 802006b0 <initlock>

  cons.e = cons.w = cons.r = 0;
    802078b2:	0804ac23          	sw	zero,152(s1)
    802078b6:	0804ae23          	sw	zero,156(s1)
    802078ba:	0a04a023          	sw	zero,160(s1)
  
  // connect read and write system calls
  // to consoleread and consolewrite.
  devsw[CONSOLE].read = consoleread;
    802078be:	0002c797          	auipc	a5,0x2c
    802078c2:	a727b783          	ld	a5,-1422(a5) # 80233330 <_GLOBAL_OFFSET_TABLE_+0x58>
    802078c6:	00000717          	auipc	a4,0x0
    802078ca:	ce070713          	addi	a4,a4,-800 # 802075a6 <consoleread>
    802078ce:	eb98                	sd	a4,16(a5)
  devsw[CONSOLE].write = consolewrite;
    802078d0:	00000717          	auipc	a4,0x0
    802078d4:	c4870713          	addi	a4,a4,-952 # 80207518 <consolewrite>
    802078d8:	ef98                	sd	a4,24(a5)
}
    802078da:	60e2                	ld	ra,24(sp)
    802078dc:	6442                	ld	s0,16(sp)
    802078de:	64a2                	ld	s1,8(sp)
    802078e0:	6105                	addi	sp,sp,32
    802078e2:	8082                	ret

00000000802078e4 <free_desc>:
}

// mark a descriptor as free.
static void
free_desc(int i)
{
    802078e4:	1141                	addi	sp,sp,-16
    802078e6:	e406                	sd	ra,8(sp)
    802078e8:	e022                	sd	s0,0(sp)
    802078ea:	0800                	addi	s0,sp,16
  if(i >= NUM)
    802078ec:	479d                	li	a5,7
    802078ee:	04a7cb63          	blt	a5,a0,80207944 <free_desc+0x60>
    panic("virtio_disk_intr 1");
  if(disk.free[i])
    802078f2:	0004d717          	auipc	a4,0x4d
    802078f6:	70e70713          	addi	a4,a4,1806 # 80255000 <disk>
    802078fa:	972a                	add	a4,a4,a0
    802078fc:	6789                	lui	a5,0x2
    802078fe:	97ba                	add	a5,a5,a4
    80207900:	0187c783          	lbu	a5,24(a5) # 2018 <_entry-0x801fdfe8>
    80207904:	eba1                	bnez	a5,80207954 <free_desc+0x70>
    panic("virtio_disk_intr 2");
  disk.desc[i].addr = 0;
    80207906:	00451713          	slli	a4,a0,0x4
    8020790a:	0004f797          	auipc	a5,0x4f
    8020790e:	6f67b783          	ld	a5,1782(a5) # 80257000 <disk+0x2000>
    80207912:	97ba                	add	a5,a5,a4
    80207914:	0007b023          	sd	zero,0(a5)
  disk.free[i] = 1;
    80207918:	0004d717          	auipc	a4,0x4d
    8020791c:	6e870713          	addi	a4,a4,1768 # 80255000 <disk>
    80207920:	972a                	add	a4,a4,a0
    80207922:	6789                	lui	a5,0x2
    80207924:	97ba                	add	a5,a5,a4
    80207926:	4705                	li	a4,1
    80207928:	00e78c23          	sb	a4,24(a5) # 2018 <_entry-0x801fdfe8>
  wakeup(&disk.free[0]);
    8020792c:	0004f517          	auipc	a0,0x4f
    80207930:	6ec50513          	addi	a0,a0,1772 # 80257018 <disk+0x2018>
    80207934:	ffffb097          	auipc	ra,0xffffb
    80207938:	d04080e7          	jalr	-764(ra) # 80202638 <wakeup>
}
    8020793c:	60a2                	ld	ra,8(sp)
    8020793e:	6402                	ld	s0,0(sp)
    80207940:	0141                	addi	sp,sp,16
    80207942:	8082                	ret
    panic("virtio_disk_intr 1");
    80207944:	00002517          	auipc	a0,0x2
    80207948:	6cc50513          	addi	a0,a0,1740 # 8020a010 <digits+0xc90>
    8020794c:	ffff8097          	auipc	ra,0xffff8
    80207950:	7f8080e7          	jalr	2040(ra) # 80200144 <panic>
    panic("virtio_disk_intr 2");
    80207954:	00002517          	auipc	a0,0x2
    80207958:	6d450513          	addi	a0,a0,1748 # 8020a028 <digits+0xca8>
    8020795c:	ffff8097          	auipc	ra,0xffff8
    80207960:	7e8080e7          	jalr	2024(ra) # 80200144 <panic>

0000000080207964 <virtio_disk_init>:
{
    80207964:	1141                	addi	sp,sp,-16
    80207966:	e406                	sd	ra,8(sp)
    80207968:	e022                	sd	s0,0(sp)
    8020796a:	0800                	addi	s0,sp,16
  initlock(&disk.vdisk_lock, "virtio_disk");
    8020796c:	00002597          	auipc	a1,0x2
    80207970:	6d458593          	addi	a1,a1,1748 # 8020a040 <digits+0xcc0>
    80207974:	0004f517          	auipc	a0,0x4f
    80207978:	73450513          	addi	a0,a0,1844 # 802570a8 <disk+0x20a8>
    8020797c:	ffff9097          	auipc	ra,0xffff9
    80207980:	d34080e7          	jalr	-716(ra) # 802006b0 <initlock>
  if(*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
    80207984:	03f107b7          	lui	a5,0x3f10
    80207988:	0785                	addi	a5,a5,1 # 3f10001 <_entry-0x7c2effff>
    8020798a:	07b2                	slli	a5,a5,0xc
    8020798c:	4398                	lw	a4,0(a5)
    8020798e:	2701                	sext.w	a4,a4
    80207990:	747277b7          	lui	a5,0x74727
    80207994:	97678793          	addi	a5,a5,-1674 # 74726976 <_entry-0xbad968a>
    80207998:	12f71963          	bne	a4,a5,80207aca <virtio_disk_init+0x166>
     *R(VIRTIO_MMIO_VERSION) != 1 ||
    8020799c:	00002797          	auipc	a5,0x2
    802079a0:	7447b783          	ld	a5,1860(a5) # 8020a0e0 <digits+0xd60>
    802079a4:	439c                	lw	a5,0(a5)
    802079a6:	2781                	sext.w	a5,a5
  if(*R(VIRTIO_MMIO_MAGIC_VALUE) != 0x74726976 ||
    802079a8:	4705                	li	a4,1
    802079aa:	12e79063          	bne	a5,a4,80207aca <virtio_disk_init+0x166>
     *R(VIRTIO_MMIO_DEVICE_ID) != 2 ||
    802079ae:	00002797          	auipc	a5,0x2
    802079b2:	73a7b783          	ld	a5,1850(a5) # 8020a0e8 <digits+0xd68>
    802079b6:	439c                	lw	a5,0(a5)
    802079b8:	2781                	sext.w	a5,a5
     *R(VIRTIO_MMIO_VERSION) != 1 ||
    802079ba:	4709                	li	a4,2
    802079bc:	10e79763          	bne	a5,a4,80207aca <virtio_disk_init+0x166>
     *R(VIRTIO_MMIO_VENDOR_ID) != 0x554d4551){
    802079c0:	00002797          	auipc	a5,0x2
    802079c4:	7307b783          	ld	a5,1840(a5) # 8020a0f0 <digits+0xd70>
    802079c8:	4398                	lw	a4,0(a5)
    802079ca:	2701                	sext.w	a4,a4
     *R(VIRTIO_MMIO_DEVICE_ID) != 2 ||
    802079cc:	554d47b7          	lui	a5,0x554d4
    802079d0:	55178793          	addi	a5,a5,1361 # 554d4551 <_entry-0x2ad2baaf>
    802079d4:	0ef71b63          	bne	a4,a5,80207aca <virtio_disk_init+0x166>
  *R(VIRTIO_MMIO_STATUS) = status;
    802079d8:	00002797          	auipc	a5,0x2
    802079dc:	7207b783          	ld	a5,1824(a5) # 8020a0f8 <digits+0xd78>
    802079e0:	4705                	li	a4,1
    802079e2:	c398                	sw	a4,0(a5)
  *R(VIRTIO_MMIO_STATUS) = status;
    802079e4:	470d                	li	a4,3
    802079e6:	c398                	sw	a4,0(a5)
  uint64 features = *R(VIRTIO_MMIO_DEVICE_FEATURES);
    802079e8:	00002717          	auipc	a4,0x2
    802079ec:	71873703          	ld	a4,1816(a4) # 8020a100 <digits+0xd80>
    802079f0:	4318                	lw	a4,0(a4)
  *R(VIRTIO_MMIO_DRIVER_FEATURES) = features;
    802079f2:	c7ffe6b7          	lui	a3,0xc7ffe
    802079f6:	75f68693          	addi	a3,a3,1887 # ffffffffc7ffe75f <ebss_clear+0xffffffff47da675f>
    802079fa:	8f75                	and	a4,a4,a3
    802079fc:	00002697          	auipc	a3,0x2
    80207a00:	70c6b683          	ld	a3,1804(a3) # 8020a108 <digits+0xd88>
    80207a04:	c298                	sw	a4,0(a3)
  *R(VIRTIO_MMIO_STATUS) = status;
    80207a06:	472d                	li	a4,11
    80207a08:	c398                	sw	a4,0(a5)
  *R(VIRTIO_MMIO_STATUS) = status;
    80207a0a:	473d                	li	a4,15
    80207a0c:	c398                	sw	a4,0(a5)
  *R(VIRTIO_MMIO_GUEST_PAGE_SIZE) = PGSIZE;
    80207a0e:	00002797          	auipc	a5,0x2
    80207a12:	7027b783          	ld	a5,1794(a5) # 8020a110 <digits+0xd90>
    80207a16:	6705                	lui	a4,0x1
    80207a18:	c398                	sw	a4,0(a5)
  *R(VIRTIO_MMIO_QUEUE_SEL) = 0;
    80207a1a:	00002797          	auipc	a5,0x2
    80207a1e:	6fe7b783          	ld	a5,1790(a5) # 8020a118 <digits+0xd98>
    80207a22:	0007a023          	sw	zero,0(a5)
  uint32 max = *R(VIRTIO_MMIO_QUEUE_NUM_MAX);
    80207a26:	00002797          	auipc	a5,0x2
    80207a2a:	6fa7b783          	ld	a5,1786(a5) # 8020a120 <digits+0xda0>
    80207a2e:	439c                	lw	a5,0(a5)
    80207a30:	2781                	sext.w	a5,a5
  if(max == 0)
    80207a32:	c7c5                	beqz	a5,80207ada <virtio_disk_init+0x176>
  if(max < NUM)
    80207a34:	471d                	li	a4,7
    80207a36:	0af77a63          	bgeu	a4,a5,80207aea <virtio_disk_init+0x186>
  *R(VIRTIO_MMIO_QUEUE_NUM) = NUM;
    80207a3a:	00002797          	auipc	a5,0x2
    80207a3e:	6ee7b783          	ld	a5,1774(a5) # 8020a128 <digits+0xda8>
    80207a42:	4721                	li	a4,8
    80207a44:	c398                	sw	a4,0(a5)
  memset(disk.pages, 0, sizeof(disk.pages));
    80207a46:	6609                	lui	a2,0x2
    80207a48:	4581                	li	a1,0
    80207a4a:	0004d517          	auipc	a0,0x4d
    80207a4e:	5b650513          	addi	a0,a0,1462 # 80255000 <disk>
    80207a52:	ffff9097          	auipc	ra,0xffff9
    80207a56:	d3e080e7          	jalr	-706(ra) # 80200790 <memset>
  *R(VIRTIO_MMIO_QUEUE_PFN) = ((uint64)disk.pages) >> PGSHIFT;
    80207a5a:	0004d717          	auipc	a4,0x4d
    80207a5e:	5a670713          	addi	a4,a4,1446 # 80255000 <disk>
    80207a62:	00c75793          	srli	a5,a4,0xc
    80207a66:	2781                	sext.w	a5,a5
    80207a68:	00002697          	auipc	a3,0x2
    80207a6c:	6c86b683          	ld	a3,1736(a3) # 8020a130 <digits+0xdb0>
    80207a70:	c29c                	sw	a5,0(a3)
  disk.desc = (struct VRingDesc *) disk.pages;
    80207a72:	0004f797          	auipc	a5,0x4f
    80207a76:	58e78793          	addi	a5,a5,1422 # 80257000 <disk+0x2000>
    80207a7a:	e398                	sd	a4,0(a5)
  disk.avail = (uint16*)(((char*)disk.desc) + NUM*sizeof(struct VRingDesc));
    80207a7c:	0004d717          	auipc	a4,0x4d
    80207a80:	60470713          	addi	a4,a4,1540 # 80255080 <disk+0x80>
    80207a84:	e798                	sd	a4,8(a5)
  disk.used = (struct UsedArea *) (disk.pages + PGSIZE);
    80207a86:	0004e717          	auipc	a4,0x4e
    80207a8a:	57a70713          	addi	a4,a4,1402 # 80256000 <disk+0x1000>
    80207a8e:	eb98                	sd	a4,16(a5)
    disk.free[i] = 1;
    80207a90:	4705                	li	a4,1
    80207a92:	00e78c23          	sb	a4,24(a5)
    80207a96:	00e78ca3          	sb	a4,25(a5)
    80207a9a:	00e78d23          	sb	a4,26(a5)
    80207a9e:	00e78da3          	sb	a4,27(a5)
    80207aa2:	00e78e23          	sb	a4,28(a5)
    80207aa6:	00e78ea3          	sb	a4,29(a5)
    80207aaa:	00e78f23          	sb	a4,30(a5)
    80207aae:	00e78fa3          	sb	a4,31(a5)
  printf("virtio_disk_init\n");
    80207ab2:	00002517          	auipc	a0,0x2
    80207ab6:	5fe50513          	addi	a0,a0,1534 # 8020a0b0 <digits+0xd30>
    80207aba:	ffff8097          	auipc	ra,0xffff8
    80207abe:	6d4080e7          	jalr	1748(ra) # 8020018e <printf>
}
    80207ac2:	60a2                	ld	ra,8(sp)
    80207ac4:	6402                	ld	s0,0(sp)
    80207ac6:	0141                	addi	sp,sp,16
    80207ac8:	8082                	ret
    panic("could not find virtio disk");
    80207aca:	00002517          	auipc	a0,0x2
    80207ace:	58650513          	addi	a0,a0,1414 # 8020a050 <digits+0xcd0>
    80207ad2:	ffff8097          	auipc	ra,0xffff8
    80207ad6:	672080e7          	jalr	1650(ra) # 80200144 <panic>
    panic("virtio disk has no queue 0");
    80207ada:	00002517          	auipc	a0,0x2
    80207ade:	59650513          	addi	a0,a0,1430 # 8020a070 <digits+0xcf0>
    80207ae2:	ffff8097          	auipc	ra,0xffff8
    80207ae6:	662080e7          	jalr	1634(ra) # 80200144 <panic>
    panic("virtio disk max queue too short");
    80207aea:	00002517          	auipc	a0,0x2
    80207aee:	5a650513          	addi	a0,a0,1446 # 8020a090 <digits+0xd10>
    80207af2:	ffff8097          	auipc	ra,0xffff8
    80207af6:	652080e7          	jalr	1618(ra) # 80200144 <panic>

0000000080207afa <virtio_disk_rw>:
  return 0;
}

void
virtio_disk_rw(struct buf *b, int write)
{
    80207afa:	7175                	addi	sp,sp,-144
    80207afc:	e506                	sd	ra,136(sp)
    80207afe:	e122                	sd	s0,128(sp)
    80207b00:	fca6                	sd	s1,120(sp)
    80207b02:	f8ca                	sd	s2,112(sp)
    80207b04:	f4ce                	sd	s3,104(sp)
    80207b06:	f0d2                	sd	s4,96(sp)
    80207b08:	ecd6                	sd	s5,88(sp)
    80207b0a:	e8da                	sd	s6,80(sp)
    80207b0c:	e4de                	sd	s7,72(sp)
    80207b0e:	e0e2                	sd	s8,64(sp)
    80207b10:	fc66                	sd	s9,56(sp)
    80207b12:	f86a                	sd	s10,48(sp)
    80207b14:	f46e                	sd	s11,40(sp)
    80207b16:	0900                	addi	s0,sp,144
    80207b18:	8aaa                	mv	s5,a0
    80207b1a:	8cae                	mv	s9,a1
  uint64 sector = b->sectorno;
    80207b1c:	00c56d03          	lwu	s10,12(a0)

  acquire(&disk.vdisk_lock);
    80207b20:	0004f517          	auipc	a0,0x4f
    80207b24:	58850513          	addi	a0,a0,1416 # 802570a8 <disk+0x20a8>
    80207b28:	ffff9097          	auipc	ra,0xffff9
    80207b2c:	bcc080e7          	jalr	-1076(ra) # 802006f4 <acquire>
  for(int i = 0; i < 3; i++){
    80207b30:	4981                	li	s3,0
  for(int i = 0; i < NUM; i++){
    80207b32:	44a1                	li	s1,8
      disk.free[i] = 0;
    80207b34:	0004dc17          	auipc	s8,0x4d
    80207b38:	4ccc0c13          	addi	s8,s8,1228 # 80255000 <disk>
    80207b3c:	6b89                	lui	s7,0x2
  for(int i = 0; i < 3; i++){
    80207b3e:	4b0d                	li	s6,3
    80207b40:	a0ad                	j	80207baa <virtio_disk_rw+0xb0>
      disk.free[i] = 0;
    80207b42:	00fc0733          	add	a4,s8,a5
    80207b46:	975e                	add	a4,a4,s7
    80207b48:	00070c23          	sb	zero,24(a4)
    idx[i] = alloc_desc();
    80207b4c:	c19c                	sw	a5,0(a1)
    if(idx[i] < 0){
    80207b4e:	0207c563          	bltz	a5,80207b78 <virtio_disk_rw+0x7e>
  for(int i = 0; i < 3; i++){
    80207b52:	2905                	addiw	s2,s2,1
    80207b54:	0611                	addi	a2,a2,4 # 2004 <_entry-0x801fdffc>
    80207b56:	19690e63          	beq	s2,s6,80207cf2 <virtio_disk_rw+0x1f8>
    idx[i] = alloc_desc();
    80207b5a:	85b2                	mv	a1,a2
  for(int i = 0; i < NUM; i++){
    80207b5c:	0004f717          	auipc	a4,0x4f
    80207b60:	4bc70713          	addi	a4,a4,1212 # 80257018 <disk+0x2018>
    80207b64:	87ce                	mv	a5,s3
    if(disk.free[i]){
    80207b66:	00074683          	lbu	a3,0(a4)
    80207b6a:	fee1                	bnez	a3,80207b42 <virtio_disk_rw+0x48>
  for(int i = 0; i < NUM; i++){
    80207b6c:	2785                	addiw	a5,a5,1
    80207b6e:	0705                	addi	a4,a4,1
    80207b70:	fe979be3          	bne	a5,s1,80207b66 <virtio_disk_rw+0x6c>
    idx[i] = alloc_desc();
    80207b74:	57fd                	li	a5,-1
    80207b76:	c19c                	sw	a5,0(a1)
      for(int j = 0; j < i; j++)
    80207b78:	01205d63          	blez	s2,80207b92 <virtio_disk_rw+0x98>
    80207b7c:	8dce                	mv	s11,s3
        free_desc(idx[j]);
    80207b7e:	000a2503          	lw	a0,0(s4)
    80207b82:	00000097          	auipc	ra,0x0
    80207b86:	d62080e7          	jalr	-670(ra) # 802078e4 <free_desc>
      for(int j = 0; j < i; j++)
    80207b8a:	2d85                	addiw	s11,s11,1
    80207b8c:	0a11                	addi	s4,s4,4
    80207b8e:	ff2d98e3          	bne	s11,s2,80207b7e <virtio_disk_rw+0x84>
  int idx[3];
  while(1){
    if(alloc3_desc(idx) == 0) {
      break;
    }
    sleep(&disk.free[0], &disk.vdisk_lock);
    80207b92:	0004f597          	auipc	a1,0x4f
    80207b96:	51658593          	addi	a1,a1,1302 # 802570a8 <disk+0x20a8>
    80207b9a:	0004f517          	auipc	a0,0x4f
    80207b9e:	47e50513          	addi	a0,a0,1150 # 80257018 <disk+0x2018>
    80207ba2:	ffffb097          	auipc	ra,0xffffb
    80207ba6:	8fe080e7          	jalr	-1794(ra) # 802024a0 <sleep>
  for(int i = 0; i < 3; i++){
    80207baa:	f8040a13          	addi	s4,s0,-128
{
    80207bae:	8652                	mv	a2,s4
  for(int i = 0; i < 3; i++){
    80207bb0:	894e                	mv	s2,s3
    80207bb2:	b765                	j	80207b5a <virtio_disk_rw+0x60>
  disk.desc[idx[0]].next = idx[1];

  disk.desc[idx[1]].addr = (uint64) b->data;
  disk.desc[idx[1]].len = BSIZE;
  if(write)
    disk.desc[idx[1]].flags = 0; // device reads b->data
    80207bb4:	0004f717          	auipc	a4,0x4f
    80207bb8:	44c73703          	ld	a4,1100(a4) # 80257000 <disk+0x2000>
    80207bbc:	973e                	add	a4,a4,a5
    80207bbe:	00071623          	sh	zero,12(a4)
  else
    disk.desc[idx[1]].flags = VRING_DESC_F_WRITE; // device writes b->data
  disk.desc[idx[1]].flags |= VRING_DESC_F_NEXT;
    80207bc2:	0004d517          	auipc	a0,0x4d
    80207bc6:	43e50513          	addi	a0,a0,1086 # 80255000 <disk>
    80207bca:	0004f717          	auipc	a4,0x4f
    80207bce:	43670713          	addi	a4,a4,1078 # 80257000 <disk+0x2000>
    80207bd2:	6314                	ld	a3,0(a4)
    80207bd4:	96be                	add	a3,a3,a5
    80207bd6:	00c6d603          	lhu	a2,12(a3)
    80207bda:	00166613          	ori	a2,a2,1
    80207bde:	00c69623          	sh	a2,12(a3)
  disk.desc[idx[1]].next = idx[2];
    80207be2:	f8842683          	lw	a3,-120(s0)
    80207be6:	6310                	ld	a2,0(a4)
    80207be8:	97b2                	add	a5,a5,a2
    80207bea:	00d79723          	sh	a3,14(a5)

  disk.info[idx[0]].status = 0;
    80207bee:	20048613          	addi	a2,s1,512
    80207bf2:	0612                	slli	a2,a2,0x4
    80207bf4:	962a                	add	a2,a2,a0
    80207bf6:	02060823          	sb	zero,48(a2)
  disk.desc[idx[2]].addr = (uint64) &disk.info[idx[0]].status;
    80207bfa:	00469793          	slli	a5,a3,0x4
    80207bfe:	630c                	ld	a1,0(a4)
    80207c00:	95be                	add	a1,a1,a5
    80207c02:	6689                	lui	a3,0x2
    80207c04:	03068693          	addi	a3,a3,48 # 2030 <_entry-0x801fdfd0>
    80207c08:	96ca                	add	a3,a3,s2
    80207c0a:	96aa                	add	a3,a3,a0
    80207c0c:	e194                	sd	a3,0(a1)
  disk.desc[idx[2]].len = 1;
    80207c0e:	6314                	ld	a3,0(a4)
    80207c10:	96be                	add	a3,a3,a5
    80207c12:	4585                	li	a1,1
    80207c14:	c68c                	sw	a1,8(a3)
  disk.desc[idx[2]].flags = VRING_DESC_F_WRITE; // device writes the status
    80207c16:	6314                	ld	a3,0(a4)
    80207c18:	96be                	add	a3,a3,a5
    80207c1a:	4509                	li	a0,2
    80207c1c:	00a69623          	sh	a0,12(a3)
  disk.desc[idx[2]].next = 0;
    80207c20:	6314                	ld	a3,0(a4)
    80207c22:	97b6                	add	a5,a5,a3
    80207c24:	00079723          	sh	zero,14(a5)

  // record struct buf for virtio_disk_intr().
  b->disk = 1;
    80207c28:	00baa223          	sw	a1,4(s5)
  disk.info[idx[0]].b = b;
    80207c2c:	03563423          	sd	s5,40(a2)

  // avail[0] is flags
  // avail[1] tells the device how far to look in avail[2...].
  // avail[2...] are desc[] indices the device should process.
  // we only tell device the first index in our chain of descriptors.
  disk.avail[2 + (disk.avail[1] % NUM)] = idx[0];
    80207c30:	6714                	ld	a3,8(a4)
    80207c32:	0026d783          	lhu	a5,2(a3)
    80207c36:	8b9d                	andi	a5,a5,7
    80207c38:	0789                	addi	a5,a5,2
    80207c3a:	0786                	slli	a5,a5,0x1
    80207c3c:	96be                	add	a3,a3,a5
    80207c3e:	00969023          	sh	s1,0(a3)
  __sync_synchronize();
    80207c42:	0ff0000f          	fence
  disk.avail[1] = disk.avail[1] + 1;
    80207c46:	6718                	ld	a4,8(a4)
    80207c48:	00275783          	lhu	a5,2(a4)
    80207c4c:	2785                	addiw	a5,a5,1
    80207c4e:	00f71123          	sh	a5,2(a4)

  *R(VIRTIO_MMIO_QUEUE_NOTIFY) = 0; // value is queue number
    80207c52:	00002797          	auipc	a5,0x2
    80207c56:	4e67b783          	ld	a5,1254(a5) # 8020a138 <digits+0xdb8>
    80207c5a:	0007a023          	sw	zero,0(a5)

  // Wait for virtio_disk_intr() to say request has finished.
  while(b->disk == 1) {
    80207c5e:	004aa783          	lw	a5,4(s5)
    80207c62:	02b79163          	bne	a5,a1,80207c84 <virtio_disk_rw+0x18a>
    sleep(b, &disk.vdisk_lock);
    80207c66:	0004f917          	auipc	s2,0x4f
    80207c6a:	44290913          	addi	s2,s2,1090 # 802570a8 <disk+0x20a8>
  while(b->disk == 1) {
    80207c6e:	4485                	li	s1,1
    sleep(b, &disk.vdisk_lock);
    80207c70:	85ca                	mv	a1,s2
    80207c72:	8556                	mv	a0,s5
    80207c74:	ffffb097          	auipc	ra,0xffffb
    80207c78:	82c080e7          	jalr	-2004(ra) # 802024a0 <sleep>
  while(b->disk == 1) {
    80207c7c:	004aa783          	lw	a5,4(s5)
    80207c80:	fe9788e3          	beq	a5,s1,80207c70 <virtio_disk_rw+0x176>
  }

  disk.info[idx[0]].b = 0;
    80207c84:	f8042483          	lw	s1,-128(s0)
    80207c88:	20048713          	addi	a4,s1,512
    80207c8c:	0712                	slli	a4,a4,0x4
    80207c8e:	0004d797          	auipc	a5,0x4d
    80207c92:	37278793          	addi	a5,a5,882 # 80255000 <disk>
    80207c96:	97ba                	add	a5,a5,a4
    80207c98:	0207b423          	sd	zero,40(a5)
    if(disk.desc[i].flags & VRING_DESC_F_NEXT)
    80207c9c:	0004f917          	auipc	s2,0x4f
    80207ca0:	36490913          	addi	s2,s2,868 # 80257000 <disk+0x2000>
    80207ca4:	a019                	j	80207caa <virtio_disk_rw+0x1b0>
      i = disk.desc[i].next;
    80207ca6:	00e7d483          	lhu	s1,14(a5)
    free_desc(i);
    80207caa:	8526                	mv	a0,s1
    80207cac:	00000097          	auipc	ra,0x0
    80207cb0:	c38080e7          	jalr	-968(ra) # 802078e4 <free_desc>
    if(disk.desc[i].flags & VRING_DESC_F_NEXT)
    80207cb4:	0492                	slli	s1,s1,0x4
    80207cb6:	00093783          	ld	a5,0(s2)
    80207cba:	97a6                	add	a5,a5,s1
    80207cbc:	00c7d703          	lhu	a4,12(a5)
    80207cc0:	8b05                	andi	a4,a4,1
    80207cc2:	f375                	bnez	a4,80207ca6 <virtio_disk_rw+0x1ac>
  free_chain(idx[0]);

  release(&disk.vdisk_lock);
    80207cc4:	0004f517          	auipc	a0,0x4f
    80207cc8:	3e450513          	addi	a0,a0,996 # 802570a8 <disk+0x20a8>
    80207ccc:	ffff9097          	auipc	ra,0xffff9
    80207cd0:	a7c080e7          	jalr	-1412(ra) # 80200748 <release>
}
    80207cd4:	60aa                	ld	ra,136(sp)
    80207cd6:	640a                	ld	s0,128(sp)
    80207cd8:	74e6                	ld	s1,120(sp)
    80207cda:	7946                	ld	s2,112(sp)
    80207cdc:	79a6                	ld	s3,104(sp)
    80207cde:	7a06                	ld	s4,96(sp)
    80207ce0:	6ae6                	ld	s5,88(sp)
    80207ce2:	6b46                	ld	s6,80(sp)
    80207ce4:	6ba6                	ld	s7,72(sp)
    80207ce6:	6c06                	ld	s8,64(sp)
    80207ce8:	7ce2                	ld	s9,56(sp)
    80207cea:	7d42                	ld	s10,48(sp)
    80207cec:	7da2                	ld	s11,40(sp)
    80207cee:	6149                	addi	sp,sp,144
    80207cf0:	8082                	ret
  if(write)
    80207cf2:	019037b3          	snez	a5,s9
    80207cf6:	f6f42823          	sw	a5,-144(s0)
  buf0.reserved = 0;
    80207cfa:	f6042a23          	sw	zero,-140(s0)
  buf0.sector = sector;
    80207cfe:	f7a43c23          	sd	s10,-136(s0)
  disk.desc[idx[0]].addr = (uint64) kwalkaddr(myproc()->kpagetable, (uint64) &buf0);
    80207d02:	ffffa097          	auipc	ra,0xffffa
    80207d06:	dd6080e7          	jalr	-554(ra) # 80201ad8 <myproc>
    80207d0a:	f8042483          	lw	s1,-128(s0)
    80207d0e:	00449913          	slli	s2,s1,0x4
    80207d12:	0004f997          	auipc	s3,0x4f
    80207d16:	2ee98993          	addi	s3,s3,750 # 80257000 <disk+0x2000>
    80207d1a:	0009ba03          	ld	s4,0(s3)
    80207d1e:	9a4a                	add	s4,s4,s2
    80207d20:	f7040593          	addi	a1,s0,-144
    80207d24:	6d28                	ld	a0,88(a0)
    80207d26:	ffff9097          	auipc	ra,0xffff9
    80207d2a:	f30080e7          	jalr	-208(ra) # 80200c56 <kwalkaddr>
    80207d2e:	00aa3023          	sd	a0,0(s4)
  disk.desc[idx[0]].len = sizeof(buf0);
    80207d32:	0009b783          	ld	a5,0(s3)
    80207d36:	97ca                	add	a5,a5,s2
    80207d38:	4741                	li	a4,16
    80207d3a:	c798                	sw	a4,8(a5)
  disk.desc[idx[0]].flags = VRING_DESC_F_NEXT;
    80207d3c:	0009b783          	ld	a5,0(s3)
    80207d40:	97ca                	add	a5,a5,s2
    80207d42:	4705                	li	a4,1
    80207d44:	00e79623          	sh	a4,12(a5)
  disk.desc[idx[0]].next = idx[1];
    80207d48:	f8442783          	lw	a5,-124(s0)
    80207d4c:	0009b703          	ld	a4,0(s3)
    80207d50:	974a                	add	a4,a4,s2
    80207d52:	00f71723          	sh	a5,14(a4)
  disk.desc[idx[1]].addr = (uint64) b->data;
    80207d56:	0792                	slli	a5,a5,0x4
    80207d58:	0009b703          	ld	a4,0(s3)
    80207d5c:	973e                	add	a4,a4,a5
    80207d5e:	058a8693          	addi	a3,s5,88
    80207d62:	e314                	sd	a3,0(a4)
  disk.desc[idx[1]].len = BSIZE;
    80207d64:	0009b703          	ld	a4,0(s3)
    80207d68:	973e                	add	a4,a4,a5
    80207d6a:	20000693          	li	a3,512
    80207d6e:	c714                	sw	a3,8(a4)
  if(write)
    80207d70:	e40c92e3          	bnez	s9,80207bb4 <virtio_disk_rw+0xba>
    disk.desc[idx[1]].flags = VRING_DESC_F_WRITE; // device writes b->data
    80207d74:	0004f717          	auipc	a4,0x4f
    80207d78:	28c73703          	ld	a4,652(a4) # 80257000 <disk+0x2000>
    80207d7c:	973e                	add	a4,a4,a5
    80207d7e:	4689                	li	a3,2
    80207d80:	00d71623          	sh	a3,12(a4)
    80207d84:	bd3d                	j	80207bc2 <virtio_disk_rw+0xc8>

0000000080207d86 <virtio_disk_intr>:

void
virtio_disk_intr()
{
    80207d86:	1101                	addi	sp,sp,-32
    80207d88:	ec06                	sd	ra,24(sp)
    80207d8a:	e822                	sd	s0,16(sp)
    80207d8c:	e426                	sd	s1,8(sp)
    80207d8e:	e04a                	sd	s2,0(sp)
    80207d90:	1000                	addi	s0,sp,32
  acquire(&disk.vdisk_lock);
    80207d92:	0004f517          	auipc	a0,0x4f
    80207d96:	31650513          	addi	a0,a0,790 # 802570a8 <disk+0x20a8>
    80207d9a:	ffff9097          	auipc	ra,0xffff9
    80207d9e:	95a080e7          	jalr	-1702(ra) # 802006f4 <acquire>

  while((disk.used_idx % NUM) != (disk.used->id % NUM)){
    80207da2:	0004f717          	auipc	a4,0x4f
    80207da6:	25e70713          	addi	a4,a4,606 # 80257000 <disk+0x2000>
    80207daa:	02075783          	lhu	a5,32(a4)
    80207dae:	6b18                	ld	a4,16(a4)
    80207db0:	00275683          	lhu	a3,2(a4)
    80207db4:	8ebd                	xor	a3,a3,a5
    80207db6:	8a9d                	andi	a3,a3,7
    80207db8:	cab9                	beqz	a3,80207e0e <virtio_disk_intr+0x88>
    int id = disk.used->elems[disk.used_idx].id;

    if(disk.info[id].status != 0)
    80207dba:	0004d917          	auipc	s2,0x4d
    80207dbe:	24690913          	addi	s2,s2,582 # 80255000 <disk>
      panic("virtio_disk_intr status");
    
    disk.info[id].b->disk = 0;   // disk is done with buf
    wakeup(disk.info[id].b);

    disk.used_idx = (disk.used_idx + 1) % NUM;
    80207dc2:	0004f497          	auipc	s1,0x4f
    80207dc6:	23e48493          	addi	s1,s1,574 # 80257000 <disk+0x2000>
    int id = disk.used->elems[disk.used_idx].id;
    80207dca:	078e                	slli	a5,a5,0x3
    80207dcc:	973e                	add	a4,a4,a5
    80207dce:	435c                	lw	a5,4(a4)
    if(disk.info[id].status != 0)
    80207dd0:	20078713          	addi	a4,a5,512
    80207dd4:	0712                	slli	a4,a4,0x4
    80207dd6:	974a                	add	a4,a4,s2
    80207dd8:	03074703          	lbu	a4,48(a4)
    80207ddc:	e335                	bnez	a4,80207e40 <virtio_disk_intr+0xba>
    disk.info[id].b->disk = 0;   // disk is done with buf
    80207dde:	20078793          	addi	a5,a5,512
    80207de2:	0792                	slli	a5,a5,0x4
    80207de4:	97ca                	add	a5,a5,s2
    80207de6:	7798                	ld	a4,40(a5)
    80207de8:	00072223          	sw	zero,4(a4)
    wakeup(disk.info[id].b);
    80207dec:	7788                	ld	a0,40(a5)
    80207dee:	ffffb097          	auipc	ra,0xffffb
    80207df2:	84a080e7          	jalr	-1974(ra) # 80202638 <wakeup>
    disk.used_idx = (disk.used_idx + 1) % NUM;
    80207df6:	0204d783          	lhu	a5,32(s1)
    80207dfa:	2785                	addiw	a5,a5,1
    80207dfc:	8b9d                	andi	a5,a5,7
    80207dfe:	02f49023          	sh	a5,32(s1)
  while((disk.used_idx % NUM) != (disk.used->id % NUM)){
    80207e02:	6898                	ld	a4,16(s1)
    80207e04:	00275683          	lhu	a3,2(a4)
    80207e08:	8a9d                	andi	a3,a3,7
    80207e0a:	fcf690e3          	bne	a3,a5,80207dca <virtio_disk_intr+0x44>
  }
  *R(VIRTIO_MMIO_INTERRUPT_ACK) = *R(VIRTIO_MMIO_INTERRUPT_STATUS) & 0x3;
    80207e0e:	00002797          	auipc	a5,0x2
    80207e12:	3327b783          	ld	a5,818(a5) # 8020a140 <digits+0xdc0>
    80207e16:	439c                	lw	a5,0(a5)
    80207e18:	8b8d                	andi	a5,a5,3
    80207e1a:	00002717          	auipc	a4,0x2
    80207e1e:	32e73703          	ld	a4,814(a4) # 8020a148 <digits+0xdc8>
    80207e22:	c31c                	sw	a5,0(a4)

  release(&disk.vdisk_lock);
    80207e24:	0004f517          	auipc	a0,0x4f
    80207e28:	28450513          	addi	a0,a0,644 # 802570a8 <disk+0x20a8>
    80207e2c:	ffff9097          	auipc	ra,0xffff9
    80207e30:	91c080e7          	jalr	-1764(ra) # 80200748 <release>
}
    80207e34:	60e2                	ld	ra,24(sp)
    80207e36:	6442                	ld	s0,16(sp)
    80207e38:	64a2                	ld	s1,8(sp)
    80207e3a:	6902                	ld	s2,0(sp)
    80207e3c:	6105                	addi	sp,sp,32
    80207e3e:	8082                	ret
      panic("virtio_disk_intr status");
    80207e40:	00002517          	auipc	a0,0x2
    80207e44:	28850513          	addi	a0,a0,648 # 8020a0c8 <digits+0xd48>
    80207e48:	ffff8097          	auipc	ra,0xffff8
    80207e4c:	2fc080e7          	jalr	764(ra) # 80200144 <panic>
	...

0000000080208000 <_trampoline>:
    80208000:	14051573          	csrrw	a0,sscratch,a0
    80208004:	02153423          	sd	ra,40(a0)
    80208008:	02253823          	sd	sp,48(a0)
    8020800c:	02353c23          	sd	gp,56(a0)
    80208010:	04453023          	sd	tp,64(a0)
    80208014:	04553423          	sd	t0,72(a0)
    80208018:	04653823          	sd	t1,80(a0)
    8020801c:	04753c23          	sd	t2,88(a0)
    80208020:	f120                	sd	s0,96(a0)
    80208022:	f524                	sd	s1,104(a0)
    80208024:	fd2c                	sd	a1,120(a0)
    80208026:	e150                	sd	a2,128(a0)
    80208028:	e554                	sd	a3,136(a0)
    8020802a:	e958                	sd	a4,144(a0)
    8020802c:	ed5c                	sd	a5,152(a0)
    8020802e:	0b053023          	sd	a6,160(a0)
    80208032:	0b153423          	sd	a7,168(a0)
    80208036:	0b253823          	sd	s2,176(a0)
    8020803a:	0b353c23          	sd	s3,184(a0)
    8020803e:	0d453023          	sd	s4,192(a0)
    80208042:	0d553423          	sd	s5,200(a0)
    80208046:	0d653823          	sd	s6,208(a0)
    8020804a:	0d753c23          	sd	s7,216(a0)
    8020804e:	0f853023          	sd	s8,224(a0)
    80208052:	0f953423          	sd	s9,232(a0)
    80208056:	0fa53823          	sd	s10,240(a0)
    8020805a:	0fb53c23          	sd	s11,248(a0)
    8020805e:	11c53023          	sd	t3,256(a0)
    80208062:	11d53423          	sd	t4,264(a0)
    80208066:	11e53823          	sd	t5,272(a0)
    8020806a:	11f53c23          	sd	t6,280(a0)
    8020806e:	140022f3          	csrr	t0,sscratch
    80208072:	06553823          	sd	t0,112(a0)
    80208076:	00853103          	ld	sp,8(a0)
    8020807a:	02053203          	ld	tp,32(a0)
    8020807e:	01053283          	ld	t0,16(a0)
    80208082:	00053303          	ld	t1,0(a0)
    80208086:	18031073          	csrw	satp,t1
    8020808a:	12000073          	sfence.vma
    8020808e:	8282                	jr	t0

0000000080208090 <userret>:
    80208090:	18059073          	csrw	satp,a1
    80208094:	12000073          	sfence.vma
    80208098:	07053283          	ld	t0,112(a0)
    8020809c:	14029073          	csrw	sscratch,t0
    802080a0:	02853083          	ld	ra,40(a0)
    802080a4:	03053103          	ld	sp,48(a0)
    802080a8:	03853183          	ld	gp,56(a0)
    802080ac:	04053203          	ld	tp,64(a0)
    802080b0:	04853283          	ld	t0,72(a0)
    802080b4:	05053303          	ld	t1,80(a0)
    802080b8:	05853383          	ld	t2,88(a0)
    802080bc:	7120                	ld	s0,96(a0)
    802080be:	7524                	ld	s1,104(a0)
    802080c0:	7d2c                	ld	a1,120(a0)
    802080c2:	6150                	ld	a2,128(a0)
    802080c4:	6554                	ld	a3,136(a0)
    802080c6:	6958                	ld	a4,144(a0)
    802080c8:	6d5c                	ld	a5,152(a0)
    802080ca:	0a053803          	ld	a6,160(a0)
    802080ce:	0a853883          	ld	a7,168(a0)
    802080d2:	0b053903          	ld	s2,176(a0)
    802080d6:	0b853983          	ld	s3,184(a0)
    802080da:	0c053a03          	ld	s4,192(a0)
    802080de:	0c853a83          	ld	s5,200(a0)
    802080e2:	0d053b03          	ld	s6,208(a0)
    802080e6:	0d853b83          	ld	s7,216(a0)
    802080ea:	0e053c03          	ld	s8,224(a0)
    802080ee:	0e853c83          	ld	s9,232(a0)
    802080f2:	0f053d03          	ld	s10,240(a0)
    802080f6:	0f853d83          	ld	s11,248(a0)
    802080fa:	10053e03          	ld	t3,256(a0)
    802080fe:	10853e83          	ld	t4,264(a0)
    80208102:	11053f03          	ld	t5,272(a0)
    80208106:	11853f83          	ld	t6,280(a0)
    8020810a:	14051573          	csrrw	a0,sscratch,a0
    8020810e:	10200073          	sret
	...
