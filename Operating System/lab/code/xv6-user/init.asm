
xv6-user/_init:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <main>:
    "yield",
};

int
main(void)
{
   0:	7139                	addi	sp,sp,-64
   2:	fc06                	sd	ra,56(sp)
   4:	f822                	sd	s0,48(sp)
   6:	f426                	sd	s1,40(sp)
   8:	f04a                	sd	s2,32(sp)
   a:	ec4e                	sd	s3,24(sp)
   c:	e852                	sd	s4,16(sp)
   e:	e456                	sd	s5,8(sp)
  10:	e05a                	sd	s6,0(sp)
  12:	0080                	addi	s0,sp,64
    int pid, wpid;
    // int pid;
    dev(O_RDWR, CONSOLE, 0);
  14:	4601                	li	a2,0
  16:	4585                	li	a1,1
  18:	4509                	li	a0,2
  1a:	00000097          	auipc	ra,0x0
  1e:	444080e7          	jalr	1092(ra) # 45e <dev>
    dup(0);  // stdout
  22:	4501                	li	a0,0
  24:	00000097          	auipc	ra,0x0
  28:	404080e7          	jalr	1028(ra) # 428 <dup>
    dup(0);  // stderr
  2c:	4501                	li	a0,0
  2e:	00000097          	auipc	ra,0x0
  32:	3fa080e7          	jalr	1018(ra) # 428 <dup>

    printf("init: starting tests\n");
  36:	00001517          	auipc	a0,0x1
  3a:	8e250513          	addi	a0,a0,-1822 # 918 <malloc+0xec>
  3e:	00000097          	auipc	ra,0x0
  42:	736080e7          	jalr	1846(ra) # 774 <printf>

    for(;;){
        for(int i = 0; i < 33; i++){
  46:	4981                	li	s3,0
  48:	02100a13          	li	s4,33
            }
            else{
                wait((int*)0);
            }
        }
        (*(volatile uint32 *) 0x100000) = 0x5555; // write PHYSICAL address
  4c:	00100b37          	lui	s6,0x100
  50:	6795                	lui	a5,0x5
  52:	55578a93          	addi	s5,a5,1365 # 5555 <__global_pointer$+0x423d>
        for(int i = 0; i < 33; i++){
  56:	894e                	mv	s2,s3
            pid = fork();
  58:	00000097          	auipc	ra,0x0
  5c:	352080e7          	jalr	850(ra) # 3aa <fork>
  60:	84aa                	mv	s1,a0
            if(pid < 0){
  62:	04054363          	bltz	a0,a8 <main+0xa8>
            if(pid == 0){
  66:	cd31                	beqz	a0,c2 <main+0xc2>
                wait((int*)0);
  68:	854e                	mv	a0,s3
  6a:	00000097          	auipc	ra,0x0
  6e:	352080e7          	jalr	850(ra) # 3bc <wait>
        for(int i = 0; i < 33; i++){
  72:	2905                	addiw	s2,s2,1
  74:	ff4912e3          	bne	s2,s4,58 <main+0x58>
        (*(volatile uint32 *) 0x100000) = 0x5555; // write PHYSICAL address
  78:	015b2023          	sw	s5,0(s6) # 100000 <__global_pointer$+0xfece8>

        for(;;){
        // this call to wait() returns if the shell exits,
        // or if a parentless process exits.
        wpid = wait((int *) 0);
  7c:	854e                	mv	a0,s3
  7e:	00000097          	auipc	ra,0x0
  82:	33e080e7          	jalr	830(ra) # 3bc <wait>
        if(wpid == pid){
  86:	fc9508e3          	beq	a0,s1,56 <main+0x56>
            // the shell exited; restart it.
            break;
        } else if(wpid < 0){
  8a:	fe0559e3          	bgez	a0,7c <main+0x7c>
            printf("init: wait returned an error\n");
  8e:	00001517          	auipc	a0,0x1
  92:	8d250513          	addi	a0,a0,-1838 # 960 <malloc+0x134>
  96:	00000097          	auipc	ra,0x0
  9a:	6de080e7          	jalr	1758(ra) # 774 <printf>
            exit(1);
  9e:	4505                	li	a0,1
  a0:	00000097          	auipc	ra,0x0
  a4:	312080e7          	jalr	786(ra) # 3b2 <exit>
                printf("init: fork failed\n");
  a8:	00001517          	auipc	a0,0x1
  ac:	88850513          	addi	a0,a0,-1912 # 930 <malloc+0x104>
  b0:	00000097          	auipc	ra,0x0
  b4:	6c4080e7          	jalr	1732(ra) # 774 <printf>
                exit(1);
  b8:	4505                	li	a0,1
  ba:	00000097          	auipc	ra,0x0
  be:	2f8080e7          	jalr	760(ra) # 3b2 <exit>
                exec(tests[i], argv);
  c2:	090e                	slli	s2,s2,0x3
  c4:	00001797          	auipc	a5,0x1
  c8:	a5478793          	addi	a5,a5,-1452 # b18 <tests>
  cc:	97ca                	add	a5,a5,s2
  ce:	00001597          	auipc	a1,0x1
  d2:	b5258593          	addi	a1,a1,-1198 # c20 <argv>
  d6:	6388                	ld	a0,0(a5)
  d8:	00000097          	auipc	ra,0x0
  dc:	31e080e7          	jalr	798(ra) # 3f6 <exec>
                printf("init: exec test failed\n");
  e0:	00001517          	auipc	a0,0x1
  e4:	86850513          	addi	a0,a0,-1944 # 948 <malloc+0x11c>
  e8:	00000097          	auipc	ra,0x0
  ec:	68c080e7          	jalr	1676(ra) # 774 <printf>
                exit(1);
  f0:	4505                	li	a0,1
  f2:	00000097          	auipc	ra,0x0
  f6:	2c0080e7          	jalr	704(ra) # 3b2 <exit>

00000000000000fa <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
  fa:	1141                	addi	sp,sp,-16
  fc:	e422                	sd	s0,8(sp)
  fe:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 100:	87aa                	mv	a5,a0
 102:	0585                	addi	a1,a1,1
 104:	0785                	addi	a5,a5,1
 106:	fff5c703          	lbu	a4,-1(a1)
 10a:	fee78fa3          	sb	a4,-1(a5)
 10e:	fb75                	bnez	a4,102 <strcpy+0x8>
    ;
  return os;
}
 110:	6422                	ld	s0,8(sp)
 112:	0141                	addi	sp,sp,16
 114:	8082                	ret

0000000000000116 <strcat>:

char*
strcat(char *s, const char *t)
{
 116:	1141                	addi	sp,sp,-16
 118:	e422                	sd	s0,8(sp)
 11a:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 11c:	00054783          	lbu	a5,0(a0)
 120:	c385                	beqz	a5,140 <strcat+0x2a>
 122:	87aa                	mv	a5,a0
    s++;
 124:	0785                	addi	a5,a5,1
  while(*s)
 126:	0007c703          	lbu	a4,0(a5)
 12a:	ff6d                	bnez	a4,124 <strcat+0xe>
  while((*s++ = *t++))
 12c:	0585                	addi	a1,a1,1
 12e:	0785                	addi	a5,a5,1
 130:	fff5c703          	lbu	a4,-1(a1)
 134:	fee78fa3          	sb	a4,-1(a5)
 138:	fb75                	bnez	a4,12c <strcat+0x16>
    ;
  return os;
}
 13a:	6422                	ld	s0,8(sp)
 13c:	0141                	addi	sp,sp,16
 13e:	8082                	ret
  while(*s)
 140:	87aa                	mv	a5,a0
 142:	b7ed                	j	12c <strcat+0x16>

0000000000000144 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 144:	1141                	addi	sp,sp,-16
 146:	e422                	sd	s0,8(sp)
 148:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 14a:	00054783          	lbu	a5,0(a0)
 14e:	cb91                	beqz	a5,162 <strcmp+0x1e>
 150:	0005c703          	lbu	a4,0(a1)
 154:	00f71763          	bne	a4,a5,162 <strcmp+0x1e>
    p++, q++;
 158:	0505                	addi	a0,a0,1
 15a:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 15c:	00054783          	lbu	a5,0(a0)
 160:	fbe5                	bnez	a5,150 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 162:	0005c503          	lbu	a0,0(a1)
}
 166:	40a7853b          	subw	a0,a5,a0
 16a:	6422                	ld	s0,8(sp)
 16c:	0141                	addi	sp,sp,16
 16e:	8082                	ret

0000000000000170 <strlen>:

uint
strlen(const char *s)
{
 170:	1141                	addi	sp,sp,-16
 172:	e422                	sd	s0,8(sp)
 174:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 176:	00054783          	lbu	a5,0(a0)
 17a:	cf91                	beqz	a5,196 <strlen+0x26>
 17c:	0505                	addi	a0,a0,1
 17e:	87aa                	mv	a5,a0
 180:	4685                	li	a3,1
 182:	9e89                	subw	a3,a3,a0
 184:	00f6853b          	addw	a0,a3,a5
 188:	0785                	addi	a5,a5,1
 18a:	fff7c703          	lbu	a4,-1(a5)
 18e:	fb7d                	bnez	a4,184 <strlen+0x14>
    ;
  return n;
}
 190:	6422                	ld	s0,8(sp)
 192:	0141                	addi	sp,sp,16
 194:	8082                	ret
  for(n = 0; s[n]; n++)
 196:	4501                	li	a0,0
 198:	bfe5                	j	190 <strlen+0x20>

000000000000019a <memset>:

void*
memset(void *dst, int c, uint n)
{
 19a:	1141                	addi	sp,sp,-16
 19c:	e422                	sd	s0,8(sp)
 19e:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 1a0:	ca19                	beqz	a2,1b6 <memset+0x1c>
 1a2:	87aa                	mv	a5,a0
 1a4:	1602                	slli	a2,a2,0x20
 1a6:	9201                	srli	a2,a2,0x20
 1a8:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 1ac:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 1b0:	0785                	addi	a5,a5,1
 1b2:	fee79de3          	bne	a5,a4,1ac <memset+0x12>
  }
  return dst;
}
 1b6:	6422                	ld	s0,8(sp)
 1b8:	0141                	addi	sp,sp,16
 1ba:	8082                	ret

00000000000001bc <strchr>:

char*
strchr(const char *s, char c)
{
 1bc:	1141                	addi	sp,sp,-16
 1be:	e422                	sd	s0,8(sp)
 1c0:	0800                	addi	s0,sp,16
  for(; *s; s++)
 1c2:	00054783          	lbu	a5,0(a0)
 1c6:	cb99                	beqz	a5,1dc <strchr+0x20>
    if(*s == c)
 1c8:	00f58763          	beq	a1,a5,1d6 <strchr+0x1a>
  for(; *s; s++)
 1cc:	0505                	addi	a0,a0,1
 1ce:	00054783          	lbu	a5,0(a0)
 1d2:	fbfd                	bnez	a5,1c8 <strchr+0xc>
      return (char*)s;
  return 0;
 1d4:	4501                	li	a0,0
}
 1d6:	6422                	ld	s0,8(sp)
 1d8:	0141                	addi	sp,sp,16
 1da:	8082                	ret
  return 0;
 1dc:	4501                	li	a0,0
 1de:	bfe5                	j	1d6 <strchr+0x1a>

00000000000001e0 <gets>:

char*
gets(char *buf, int max)
{
 1e0:	711d                	addi	sp,sp,-96
 1e2:	ec86                	sd	ra,88(sp)
 1e4:	e8a2                	sd	s0,80(sp)
 1e6:	e4a6                	sd	s1,72(sp)
 1e8:	e0ca                	sd	s2,64(sp)
 1ea:	fc4e                	sd	s3,56(sp)
 1ec:	f852                	sd	s4,48(sp)
 1ee:	f456                	sd	s5,40(sp)
 1f0:	f05a                	sd	s6,32(sp)
 1f2:	ec5e                	sd	s7,24(sp)
 1f4:	e862                	sd	s8,16(sp)
 1f6:	1080                	addi	s0,sp,96
 1f8:	8baa                	mv	s7,a0
 1fa:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 1fc:	892a                	mv	s2,a0
 1fe:	4481                	li	s1,0
    cc = read(0, &c, 1);
 200:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 204:	4b29                	li	s6,10
 206:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 208:	89a6                	mv	s3,s1
 20a:	2485                	addiw	s1,s1,1
 20c:	0344d763          	bge	s1,s4,23a <gets+0x5a>
    cc = read(0, &c, 1);
 210:	4605                	li	a2,1
 212:	85d6                	mv	a1,s5
 214:	4501                	li	a0,0
 216:	00000097          	auipc	ra,0x0
 21a:	1ba080e7          	jalr	442(ra) # 3d0 <read>
    if(cc < 1)
 21e:	00a05e63          	blez	a0,23a <gets+0x5a>
    buf[i++] = c;
 222:	faf44783          	lbu	a5,-81(s0)
 226:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 22a:	01678763          	beq	a5,s6,238 <gets+0x58>
 22e:	0905                	addi	s2,s2,1
 230:	fd879ce3          	bne	a5,s8,208 <gets+0x28>
  for(i=0; i+1 < max; ){
 234:	89a6                	mv	s3,s1
 236:	a011                	j	23a <gets+0x5a>
 238:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 23a:	99de                	add	s3,s3,s7
 23c:	00098023          	sb	zero,0(s3)
  return buf;
}
 240:	855e                	mv	a0,s7
 242:	60e6                	ld	ra,88(sp)
 244:	6446                	ld	s0,80(sp)
 246:	64a6                	ld	s1,72(sp)
 248:	6906                	ld	s2,64(sp)
 24a:	79e2                	ld	s3,56(sp)
 24c:	7a42                	ld	s4,48(sp)
 24e:	7aa2                	ld	s5,40(sp)
 250:	7b02                	ld	s6,32(sp)
 252:	6be2                	ld	s7,24(sp)
 254:	6c42                	ld	s8,16(sp)
 256:	6125                	addi	sp,sp,96
 258:	8082                	ret

000000000000025a <stat>:

int
stat(const char *n, struct stat *st)
{
 25a:	1101                	addi	sp,sp,-32
 25c:	ec06                	sd	ra,24(sp)
 25e:	e822                	sd	s0,16(sp)
 260:	e426                	sd	s1,8(sp)
 262:	e04a                	sd	s2,0(sp)
 264:	1000                	addi	s0,sp,32
 266:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 268:	4581                	li	a1,0
 26a:	00000097          	auipc	ra,0x0
 26e:	196080e7          	jalr	406(ra) # 400 <open>
  if(fd < 0)
 272:	02054563          	bltz	a0,29c <stat+0x42>
 276:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 278:	85ca                	mv	a1,s2
 27a:	00000097          	auipc	ra,0x0
 27e:	190080e7          	jalr	400(ra) # 40a <fstat>
 282:	892a                	mv	s2,a0
  close(fd);
 284:	8526                	mv	a0,s1
 286:	00000097          	auipc	ra,0x0
 28a:	15e080e7          	jalr	350(ra) # 3e4 <close>
  return r;
}
 28e:	854a                	mv	a0,s2
 290:	60e2                	ld	ra,24(sp)
 292:	6442                	ld	s0,16(sp)
 294:	64a2                	ld	s1,8(sp)
 296:	6902                	ld	s2,0(sp)
 298:	6105                	addi	sp,sp,32
 29a:	8082                	ret
    return -1;
 29c:	597d                	li	s2,-1
 29e:	bfc5                	j	28e <stat+0x34>

00000000000002a0 <atoi>:

int
atoi(const char *s)
{
 2a0:	1141                	addi	sp,sp,-16
 2a2:	e422                	sd	s0,8(sp)
 2a4:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 2a6:	00054703          	lbu	a4,0(a0)
 2aa:	02d00793          	li	a5,45
  int neg = 1;
 2ae:	4585                	li	a1,1
  if (*s == '-') {
 2b0:	04f70363          	beq	a4,a5,2f6 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 2b4:	00054703          	lbu	a4,0(a0)
 2b8:	fd07079b          	addiw	a5,a4,-48
 2bc:	0ff7f793          	zext.b	a5,a5
 2c0:	46a5                	li	a3,9
 2c2:	02f6ed63          	bltu	a3,a5,2fc <atoi+0x5c>
  n = 0;
 2c6:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 2c8:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 2ca:	0505                	addi	a0,a0,1
 2cc:	0026979b          	slliw	a5,a3,0x2
 2d0:	9fb5                	addw	a5,a5,a3
 2d2:	0017979b          	slliw	a5,a5,0x1
 2d6:	9fb9                	addw	a5,a5,a4
 2d8:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 2dc:	00054703          	lbu	a4,0(a0)
 2e0:	fd07079b          	addiw	a5,a4,-48
 2e4:	0ff7f793          	zext.b	a5,a5
 2e8:	fef671e3          	bgeu	a2,a5,2ca <atoi+0x2a>
  return n * neg;
}
 2ec:	02d5853b          	mulw	a0,a1,a3
 2f0:	6422                	ld	s0,8(sp)
 2f2:	0141                	addi	sp,sp,16
 2f4:	8082                	ret
    s++;
 2f6:	0505                	addi	a0,a0,1
    neg = -1;
 2f8:	55fd                	li	a1,-1
 2fa:	bf6d                	j	2b4 <atoi+0x14>
  n = 0;
 2fc:	4681                	li	a3,0
 2fe:	b7fd                	j	2ec <atoi+0x4c>

0000000000000300 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 300:	1141                	addi	sp,sp,-16
 302:	e422                	sd	s0,8(sp)
 304:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 306:	02b57463          	bgeu	a0,a1,32e <memmove+0x2e>
    while(n-- > 0)
 30a:	00c05f63          	blez	a2,328 <memmove+0x28>
 30e:	1602                	slli	a2,a2,0x20
 310:	9201                	srli	a2,a2,0x20
 312:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 316:	872a                	mv	a4,a0
      *dst++ = *src++;
 318:	0585                	addi	a1,a1,1
 31a:	0705                	addi	a4,a4,1
 31c:	fff5c683          	lbu	a3,-1(a1)
 320:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 324:	fee79ae3          	bne	a5,a4,318 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 328:	6422                	ld	s0,8(sp)
 32a:	0141                	addi	sp,sp,16
 32c:	8082                	ret
    dst += n;
 32e:	00c50733          	add	a4,a0,a2
    src += n;
 332:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 334:	fec05ae3          	blez	a2,328 <memmove+0x28>
 338:	fff6079b          	addiw	a5,a2,-1
 33c:	1782                	slli	a5,a5,0x20
 33e:	9381                	srli	a5,a5,0x20
 340:	fff7c793          	not	a5,a5
 344:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 346:	15fd                	addi	a1,a1,-1
 348:	177d                	addi	a4,a4,-1
 34a:	0005c683          	lbu	a3,0(a1)
 34e:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 352:	fee79ae3          	bne	a5,a4,346 <memmove+0x46>
 356:	bfc9                	j	328 <memmove+0x28>

0000000000000358 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 358:	1141                	addi	sp,sp,-16
 35a:	e422                	sd	s0,8(sp)
 35c:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 35e:	ca05                	beqz	a2,38e <memcmp+0x36>
 360:	fff6069b          	addiw	a3,a2,-1
 364:	1682                	slli	a3,a3,0x20
 366:	9281                	srli	a3,a3,0x20
 368:	0685                	addi	a3,a3,1
 36a:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 36c:	00054783          	lbu	a5,0(a0)
 370:	0005c703          	lbu	a4,0(a1)
 374:	00e79863          	bne	a5,a4,384 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 378:	0505                	addi	a0,a0,1
    p2++;
 37a:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 37c:	fed518e3          	bne	a0,a3,36c <memcmp+0x14>
  }
  return 0;
 380:	4501                	li	a0,0
 382:	a019                	j	388 <memcmp+0x30>
      return *p1 - *p2;
 384:	40e7853b          	subw	a0,a5,a4
}
 388:	6422                	ld	s0,8(sp)
 38a:	0141                	addi	sp,sp,16
 38c:	8082                	ret
  return 0;
 38e:	4501                	li	a0,0
 390:	bfe5                	j	388 <memcmp+0x30>

0000000000000392 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 392:	1141                	addi	sp,sp,-16
 394:	e406                	sd	ra,8(sp)
 396:	e022                	sd	s0,0(sp)
 398:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 39a:	00000097          	auipc	ra,0x0
 39e:	f66080e7          	jalr	-154(ra) # 300 <memmove>
}
 3a2:	60a2                	ld	ra,8(sp)
 3a4:	6402                	ld	s0,0(sp)
 3a6:	0141                	addi	sp,sp,16
 3a8:	8082                	ret

00000000000003aa <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 3aa:	4885                	li	a7,1
 ecall
 3ac:	00000073          	ecall
 ret
 3b0:	8082                	ret

00000000000003b2 <exit>:
.global exit
exit:
 li a7, SYS_exit
 3b2:	05d00893          	li	a7,93
 ecall
 3b6:	00000073          	ecall
 ret
 3ba:	8082                	ret

00000000000003bc <wait>:
.global wait
wait:
 li a7, SYS_wait
 3bc:	4bf00893          	li	a7,1215
 ecall
 3c0:	00000073          	ecall
 ret
 3c4:	8082                	ret

00000000000003c6 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 3c6:	03b00893          	li	a7,59
 ecall
 3ca:	00000073          	ecall
 ret
 3ce:	8082                	ret

00000000000003d0 <read>:
.global read
read:
 li a7, SYS_read
 3d0:	03f00893          	li	a7,63
 ecall
 3d4:	00000073          	ecall
 ret
 3d8:	8082                	ret

00000000000003da <write>:
.global write
write:
 li a7, SYS_write
 3da:	04000893          	li	a7,64
 ecall
 3de:	00000073          	ecall
 ret
 3e2:	8082                	ret

00000000000003e4 <close>:
.global close
close:
 li a7, SYS_close
 3e4:	03900893          	li	a7,57
 ecall
 3e8:	00000073          	ecall
 ret
 3ec:	8082                	ret

00000000000003ee <kill>:
.global kill
kill:
 li a7, SYS_kill
 3ee:	4899                	li	a7,6
 ecall
 3f0:	00000073          	ecall
 ret
 3f4:	8082                	ret

00000000000003f6 <exec>:
.global exec
exec:
 li a7, SYS_exec
 3f6:	0dd00893          	li	a7,221
 ecall
 3fa:	00000073          	ecall
 ret
 3fe:	8082                	ret

0000000000000400 <open>:
.global open
open:
 li a7, SYS_open
 400:	03800893          	li	a7,56
 ecall
 404:	00000073          	ecall
 ret
 408:	8082                	ret

000000000000040a <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 40a:	05000893          	li	a7,80
 ecall
 40e:	00000073          	ecall
 ret
 412:	8082                	ret

0000000000000414 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 414:	02200893          	li	a7,34
 ecall
 418:	00000073          	ecall
 ret
 41c:	8082                	ret

000000000000041e <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 41e:	03100893          	li	a7,49
 ecall
 422:	00000073          	ecall
 ret
 426:	8082                	ret

0000000000000428 <dup>:
.global dup
dup:
 li a7, SYS_dup
 428:	48dd                	li	a7,23
 ecall
 42a:	00000073          	ecall
 ret
 42e:	8082                	ret

0000000000000430 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 430:	0ac00893          	li	a7,172
 ecall
 434:	00000073          	ecall
 ret
 438:	8082                	ret

000000000000043a <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 43a:	0d600893          	li	a7,214
 ecall
 43e:	00000073          	ecall
 ret
 442:	8082                	ret

0000000000000444 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 444:	48b5                	li	a7,13
 ecall
 446:	00000073          	ecall
 ret
 44a:	8082                	ret

000000000000044c <times>:
.global times
times:
 li a7, SYS_times
 44c:	09900893          	li	a7,153
 ecall
 450:	00000073          	ecall
 ret
 454:	8082                	ret

0000000000000456 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 456:	48d9                	li	a7,22
 ecall
 458:	00000073          	ecall
 ret
 45c:	8082                	ret

000000000000045e <dev>:
.global dev
dev:
 li a7, SYS_dev
 45e:	6889                	lui	a7,0x2
 460:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x144e>
 ecall
 464:	00000073          	ecall
 ret
 468:	8082                	ret

000000000000046a <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 46a:	7d200893          	li	a7,2002
 ecall
 46e:	00000073          	ecall
 ret
 472:	8082                	ret

0000000000000474 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 474:	48c5                	li	a7,17
 ecall
 476:	00000073          	ecall
 ret
 47a:	8082                	ret

000000000000047c <remove>:
.global remove
remove:
 li a7, SYS_remove
 47c:	48e5                	li	a7,25
 ecall
 47e:	00000073          	ecall
 ret
 482:	8082                	ret

0000000000000484 <trace>:
.global trace
trace:
 li a7, SYS_trace
 484:	48c9                	li	a7,18
 ecall
 486:	00000073          	ecall
 ret
 48a:	8082                	ret

000000000000048c <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 48c:	48cd                	li	a7,19
 ecall
 48e:	00000073          	ecall
 ret
 492:	8082                	ret

0000000000000494 <rename>:
.global rename
rename:
 li a7, SYS_rename
 494:	48e9                	li	a7,26
 ecall
 496:	00000073          	ecall
 ret
 49a:	8082                	ret

000000000000049c <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 49c:	1101                	addi	sp,sp,-32
 49e:	ec06                	sd	ra,24(sp)
 4a0:	e822                	sd	s0,16(sp)
 4a2:	1000                	addi	s0,sp,32
 4a4:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 4a8:	4605                	li	a2,1
 4aa:	fef40593          	addi	a1,s0,-17
 4ae:	00000097          	auipc	ra,0x0
 4b2:	f2c080e7          	jalr	-212(ra) # 3da <write>
}
 4b6:	60e2                	ld	ra,24(sp)
 4b8:	6442                	ld	s0,16(sp)
 4ba:	6105                	addi	sp,sp,32
 4bc:	8082                	ret

00000000000004be <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 4be:	7139                	addi	sp,sp,-64
 4c0:	fc06                	sd	ra,56(sp)
 4c2:	f822                	sd	s0,48(sp)
 4c4:	f426                	sd	s1,40(sp)
 4c6:	f04a                	sd	s2,32(sp)
 4c8:	ec4e                	sd	s3,24(sp)
 4ca:	0080                	addi	s0,sp,64
 4cc:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 4ce:	c299                	beqz	a3,4d4 <printint+0x16>
 4d0:	0805c863          	bltz	a1,560 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 4d4:	2581                	sext.w	a1,a1
  neg = 0;
 4d6:	4881                	li	a7,0
  }

  i = 0;
 4d8:	fc040993          	addi	s3,s0,-64
  neg = 0;
 4dc:	86ce                	mv	a3,s3
  i = 0;
 4de:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 4e0:	2601                	sext.w	a2,a2
 4e2:	00000517          	auipc	a0,0x0
 4e6:	61e50513          	addi	a0,a0,1566 # b00 <digits>
 4ea:	883a                	mv	a6,a4
 4ec:	2705                	addiw	a4,a4,1
 4ee:	02c5f7bb          	remuw	a5,a1,a2
 4f2:	1782                	slli	a5,a5,0x20
 4f4:	9381                	srli	a5,a5,0x20
 4f6:	97aa                	add	a5,a5,a0
 4f8:	0007c783          	lbu	a5,0(a5)
 4fc:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 500:	0005879b          	sext.w	a5,a1
 504:	02c5d5bb          	divuw	a1,a1,a2
 508:	0685                	addi	a3,a3,1
 50a:	fec7f0e3          	bgeu	a5,a2,4ea <printint+0x2c>
  if(neg)
 50e:	00088c63          	beqz	a7,526 <printint+0x68>
    buf[i++] = '-';
 512:	fd070793          	addi	a5,a4,-48
 516:	00878733          	add	a4,a5,s0
 51a:	02d00793          	li	a5,45
 51e:	fef70823          	sb	a5,-16(a4)
 522:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 526:	02e05663          	blez	a4,552 <printint+0x94>
 52a:	fc040913          	addi	s2,s0,-64
 52e:	993a                	add	s2,s2,a4
 530:	19fd                	addi	s3,s3,-1
 532:	99ba                	add	s3,s3,a4
 534:	377d                	addiw	a4,a4,-1
 536:	1702                	slli	a4,a4,0x20
 538:	9301                	srli	a4,a4,0x20
 53a:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 53e:	fff94583          	lbu	a1,-1(s2)
 542:	8526                	mv	a0,s1
 544:	00000097          	auipc	ra,0x0
 548:	f58080e7          	jalr	-168(ra) # 49c <putc>
  while(--i >= 0)
 54c:	197d                	addi	s2,s2,-1
 54e:	ff3918e3          	bne	s2,s3,53e <printint+0x80>
}
 552:	70e2                	ld	ra,56(sp)
 554:	7442                	ld	s0,48(sp)
 556:	74a2                	ld	s1,40(sp)
 558:	7902                	ld	s2,32(sp)
 55a:	69e2                	ld	s3,24(sp)
 55c:	6121                	addi	sp,sp,64
 55e:	8082                	ret
    x = -xx;
 560:	40b005bb          	negw	a1,a1
    neg = 1;
 564:	4885                	li	a7,1
    x = -xx;
 566:	bf8d                	j	4d8 <printint+0x1a>

0000000000000568 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 568:	7119                	addi	sp,sp,-128
 56a:	fc86                	sd	ra,120(sp)
 56c:	f8a2                	sd	s0,112(sp)
 56e:	f4a6                	sd	s1,104(sp)
 570:	f0ca                	sd	s2,96(sp)
 572:	ecce                	sd	s3,88(sp)
 574:	e8d2                	sd	s4,80(sp)
 576:	e4d6                	sd	s5,72(sp)
 578:	e0da                	sd	s6,64(sp)
 57a:	fc5e                	sd	s7,56(sp)
 57c:	f862                	sd	s8,48(sp)
 57e:	f466                	sd	s9,40(sp)
 580:	f06a                	sd	s10,32(sp)
 582:	ec6e                	sd	s11,24(sp)
 584:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 586:	0005c903          	lbu	s2,0(a1)
 58a:	18090f63          	beqz	s2,728 <vprintf+0x1c0>
 58e:	8aaa                	mv	s5,a0
 590:	8b32                	mv	s6,a2
 592:	00158493          	addi	s1,a1,1
  state = 0;
 596:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 598:	02500a13          	li	s4,37
 59c:	4c55                	li	s8,21
 59e:	00000c97          	auipc	s9,0x0
 5a2:	50ac8c93          	addi	s9,s9,1290 # aa8 <malloc+0x27c>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 5a6:	02800d93          	li	s11,40
  putc(fd, 'x');
 5aa:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 5ac:	00000b97          	auipc	s7,0x0
 5b0:	554b8b93          	addi	s7,s7,1364 # b00 <digits>
 5b4:	a839                	j	5d2 <vprintf+0x6a>
        putc(fd, c);
 5b6:	85ca                	mv	a1,s2
 5b8:	8556                	mv	a0,s5
 5ba:	00000097          	auipc	ra,0x0
 5be:	ee2080e7          	jalr	-286(ra) # 49c <putc>
 5c2:	a019                	j	5c8 <vprintf+0x60>
    } else if(state == '%'){
 5c4:	01498d63          	beq	s3,s4,5de <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 5c8:	0485                	addi	s1,s1,1
 5ca:	fff4c903          	lbu	s2,-1(s1)
 5ce:	14090d63          	beqz	s2,728 <vprintf+0x1c0>
    if(state == 0){
 5d2:	fe0999e3          	bnez	s3,5c4 <vprintf+0x5c>
      if(c == '%'){
 5d6:	ff4910e3          	bne	s2,s4,5b6 <vprintf+0x4e>
        state = '%';
 5da:	89d2                	mv	s3,s4
 5dc:	b7f5                	j	5c8 <vprintf+0x60>
      if(c == 'd'){
 5de:	11490c63          	beq	s2,s4,6f6 <vprintf+0x18e>
 5e2:	f9d9079b          	addiw	a5,s2,-99
 5e6:	0ff7f793          	zext.b	a5,a5
 5ea:	10fc6e63          	bltu	s8,a5,706 <vprintf+0x19e>
 5ee:	f9d9079b          	addiw	a5,s2,-99
 5f2:	0ff7f713          	zext.b	a4,a5
 5f6:	10ec6863          	bltu	s8,a4,706 <vprintf+0x19e>
 5fa:	00271793          	slli	a5,a4,0x2
 5fe:	97e6                	add	a5,a5,s9
 600:	439c                	lw	a5,0(a5)
 602:	97e6                	add	a5,a5,s9
 604:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 606:	008b0913          	addi	s2,s6,8
 60a:	4685                	li	a3,1
 60c:	4629                	li	a2,10
 60e:	000b2583          	lw	a1,0(s6)
 612:	8556                	mv	a0,s5
 614:	00000097          	auipc	ra,0x0
 618:	eaa080e7          	jalr	-342(ra) # 4be <printint>
 61c:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 61e:	4981                	li	s3,0
 620:	b765                	j	5c8 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 622:	008b0913          	addi	s2,s6,8
 626:	4681                	li	a3,0
 628:	4629                	li	a2,10
 62a:	000b2583          	lw	a1,0(s6)
 62e:	8556                	mv	a0,s5
 630:	00000097          	auipc	ra,0x0
 634:	e8e080e7          	jalr	-370(ra) # 4be <printint>
 638:	8b4a                	mv	s6,s2
      state = 0;
 63a:	4981                	li	s3,0
 63c:	b771                	j	5c8 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 63e:	008b0913          	addi	s2,s6,8
 642:	4681                	li	a3,0
 644:	866a                	mv	a2,s10
 646:	000b2583          	lw	a1,0(s6)
 64a:	8556                	mv	a0,s5
 64c:	00000097          	auipc	ra,0x0
 650:	e72080e7          	jalr	-398(ra) # 4be <printint>
 654:	8b4a                	mv	s6,s2
      state = 0;
 656:	4981                	li	s3,0
 658:	bf85                	j	5c8 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 65a:	008b0793          	addi	a5,s6,8
 65e:	f8f43423          	sd	a5,-120(s0)
 662:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 666:	03000593          	li	a1,48
 66a:	8556                	mv	a0,s5
 66c:	00000097          	auipc	ra,0x0
 670:	e30080e7          	jalr	-464(ra) # 49c <putc>
  putc(fd, 'x');
 674:	07800593          	li	a1,120
 678:	8556                	mv	a0,s5
 67a:	00000097          	auipc	ra,0x0
 67e:	e22080e7          	jalr	-478(ra) # 49c <putc>
 682:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 684:	03c9d793          	srli	a5,s3,0x3c
 688:	97de                	add	a5,a5,s7
 68a:	0007c583          	lbu	a1,0(a5)
 68e:	8556                	mv	a0,s5
 690:	00000097          	auipc	ra,0x0
 694:	e0c080e7          	jalr	-500(ra) # 49c <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 698:	0992                	slli	s3,s3,0x4
 69a:	397d                	addiw	s2,s2,-1
 69c:	fe0914e3          	bnez	s2,684 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 6a0:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 6a4:	4981                	li	s3,0
 6a6:	b70d                	j	5c8 <vprintf+0x60>
        s = va_arg(ap, char*);
 6a8:	008b0913          	addi	s2,s6,8
 6ac:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 6b0:	02098163          	beqz	s3,6d2 <vprintf+0x16a>
        while(*s != 0){
 6b4:	0009c583          	lbu	a1,0(s3)
 6b8:	c5ad                	beqz	a1,722 <vprintf+0x1ba>
          putc(fd, *s);
 6ba:	8556                	mv	a0,s5
 6bc:	00000097          	auipc	ra,0x0
 6c0:	de0080e7          	jalr	-544(ra) # 49c <putc>
          s++;
 6c4:	0985                	addi	s3,s3,1
        while(*s != 0){
 6c6:	0009c583          	lbu	a1,0(s3)
 6ca:	f9e5                	bnez	a1,6ba <vprintf+0x152>
        s = va_arg(ap, char*);
 6cc:	8b4a                	mv	s6,s2
      state = 0;
 6ce:	4981                	li	s3,0
 6d0:	bde5                	j	5c8 <vprintf+0x60>
          s = "(null)";
 6d2:	00000997          	auipc	s3,0x0
 6d6:	3ce98993          	addi	s3,s3,974 # aa0 <malloc+0x274>
        while(*s != 0){
 6da:	85ee                	mv	a1,s11
 6dc:	bff9                	j	6ba <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 6de:	008b0913          	addi	s2,s6,8
 6e2:	000b4583          	lbu	a1,0(s6)
 6e6:	8556                	mv	a0,s5
 6e8:	00000097          	auipc	ra,0x0
 6ec:	db4080e7          	jalr	-588(ra) # 49c <putc>
 6f0:	8b4a                	mv	s6,s2
      state = 0;
 6f2:	4981                	li	s3,0
 6f4:	bdd1                	j	5c8 <vprintf+0x60>
        putc(fd, c);
 6f6:	85d2                	mv	a1,s4
 6f8:	8556                	mv	a0,s5
 6fa:	00000097          	auipc	ra,0x0
 6fe:	da2080e7          	jalr	-606(ra) # 49c <putc>
      state = 0;
 702:	4981                	li	s3,0
 704:	b5d1                	j	5c8 <vprintf+0x60>
        putc(fd, '%');
 706:	85d2                	mv	a1,s4
 708:	8556                	mv	a0,s5
 70a:	00000097          	auipc	ra,0x0
 70e:	d92080e7          	jalr	-622(ra) # 49c <putc>
        putc(fd, c);
 712:	85ca                	mv	a1,s2
 714:	8556                	mv	a0,s5
 716:	00000097          	auipc	ra,0x0
 71a:	d86080e7          	jalr	-634(ra) # 49c <putc>
      state = 0;
 71e:	4981                	li	s3,0
 720:	b565                	j	5c8 <vprintf+0x60>
        s = va_arg(ap, char*);
 722:	8b4a                	mv	s6,s2
      state = 0;
 724:	4981                	li	s3,0
 726:	b54d                	j	5c8 <vprintf+0x60>
    }
  }
}
 728:	70e6                	ld	ra,120(sp)
 72a:	7446                	ld	s0,112(sp)
 72c:	74a6                	ld	s1,104(sp)
 72e:	7906                	ld	s2,96(sp)
 730:	69e6                	ld	s3,88(sp)
 732:	6a46                	ld	s4,80(sp)
 734:	6aa6                	ld	s5,72(sp)
 736:	6b06                	ld	s6,64(sp)
 738:	7be2                	ld	s7,56(sp)
 73a:	7c42                	ld	s8,48(sp)
 73c:	7ca2                	ld	s9,40(sp)
 73e:	7d02                	ld	s10,32(sp)
 740:	6de2                	ld	s11,24(sp)
 742:	6109                	addi	sp,sp,128
 744:	8082                	ret

0000000000000746 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 746:	715d                	addi	sp,sp,-80
 748:	ec06                	sd	ra,24(sp)
 74a:	e822                	sd	s0,16(sp)
 74c:	1000                	addi	s0,sp,32
 74e:	e010                	sd	a2,0(s0)
 750:	e414                	sd	a3,8(s0)
 752:	e818                	sd	a4,16(s0)
 754:	ec1c                	sd	a5,24(s0)
 756:	03043023          	sd	a6,32(s0)
 75a:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 75e:	8622                	mv	a2,s0
 760:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 764:	00000097          	auipc	ra,0x0
 768:	e04080e7          	jalr	-508(ra) # 568 <vprintf>
}
 76c:	60e2                	ld	ra,24(sp)
 76e:	6442                	ld	s0,16(sp)
 770:	6161                	addi	sp,sp,80
 772:	8082                	ret

0000000000000774 <printf>:

void
printf(const char *fmt, ...)
{
 774:	711d                	addi	sp,sp,-96
 776:	ec06                	sd	ra,24(sp)
 778:	e822                	sd	s0,16(sp)
 77a:	1000                	addi	s0,sp,32
 77c:	e40c                	sd	a1,8(s0)
 77e:	e810                	sd	a2,16(s0)
 780:	ec14                	sd	a3,24(s0)
 782:	f018                	sd	a4,32(s0)
 784:	f41c                	sd	a5,40(s0)
 786:	03043823          	sd	a6,48(s0)
 78a:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 78e:	00840613          	addi	a2,s0,8
 792:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 796:	85aa                	mv	a1,a0
 798:	4505                	li	a0,1
 79a:	00000097          	auipc	ra,0x0
 79e:	dce080e7          	jalr	-562(ra) # 568 <vprintf>
}
 7a2:	60e2                	ld	ra,24(sp)
 7a4:	6442                	ld	s0,16(sp)
 7a6:	6125                	addi	sp,sp,96
 7a8:	8082                	ret

00000000000007aa <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 7aa:	1141                	addi	sp,sp,-16
 7ac:	e422                	sd	s0,8(sp)
 7ae:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 7b0:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 7b4:	00000797          	auipc	a5,0x0
 7b8:	4747b783          	ld	a5,1140(a5) # c28 <freep>
 7bc:	a02d                	j	7e6 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 7be:	4618                	lw	a4,8(a2)
 7c0:	9f2d                	addw	a4,a4,a1
 7c2:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 7c6:	6398                	ld	a4,0(a5)
 7c8:	6310                	ld	a2,0(a4)
 7ca:	a83d                	j	808 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 7cc:	ff852703          	lw	a4,-8(a0)
 7d0:	9f31                	addw	a4,a4,a2
 7d2:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 7d4:	ff053683          	ld	a3,-16(a0)
 7d8:	a091                	j	81c <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7da:	6398                	ld	a4,0(a5)
 7dc:	00e7e463          	bltu	a5,a4,7e4 <free+0x3a>
 7e0:	00e6ea63          	bltu	a3,a4,7f4 <free+0x4a>
{
 7e4:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 7e6:	fed7fae3          	bgeu	a5,a3,7da <free+0x30>
 7ea:	6398                	ld	a4,0(a5)
 7ec:	00e6e463          	bltu	a3,a4,7f4 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7f0:	fee7eae3          	bltu	a5,a4,7e4 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 7f4:	ff852583          	lw	a1,-8(a0)
 7f8:	6390                	ld	a2,0(a5)
 7fa:	02059813          	slli	a6,a1,0x20
 7fe:	01c85713          	srli	a4,a6,0x1c
 802:	9736                	add	a4,a4,a3
 804:	fae60de3          	beq	a2,a4,7be <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 808:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 80c:	4790                	lw	a2,8(a5)
 80e:	02061593          	slli	a1,a2,0x20
 812:	01c5d713          	srli	a4,a1,0x1c
 816:	973e                	add	a4,a4,a5
 818:	fae68ae3          	beq	a3,a4,7cc <free+0x22>
    p->s.ptr = bp->s.ptr;
 81c:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 81e:	00000717          	auipc	a4,0x0
 822:	40f73523          	sd	a5,1034(a4) # c28 <freep>
}
 826:	6422                	ld	s0,8(sp)
 828:	0141                	addi	sp,sp,16
 82a:	8082                	ret

000000000000082c <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 82c:	7139                	addi	sp,sp,-64
 82e:	fc06                	sd	ra,56(sp)
 830:	f822                	sd	s0,48(sp)
 832:	f426                	sd	s1,40(sp)
 834:	f04a                	sd	s2,32(sp)
 836:	ec4e                	sd	s3,24(sp)
 838:	e852                	sd	s4,16(sp)
 83a:	e456                	sd	s5,8(sp)
 83c:	e05a                	sd	s6,0(sp)
 83e:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 840:	02051493          	slli	s1,a0,0x20
 844:	9081                	srli	s1,s1,0x20
 846:	04bd                	addi	s1,s1,15
 848:	8091                	srli	s1,s1,0x4
 84a:	00148a1b          	addiw	s4,s1,1
 84e:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 850:	00000517          	auipc	a0,0x0
 854:	3d853503          	ld	a0,984(a0) # c28 <freep>
 858:	c515                	beqz	a0,884 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 85a:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 85c:	4798                	lw	a4,8(a5)
 85e:	04977163          	bgeu	a4,s1,8a0 <malloc+0x74>
 862:	89d2                	mv	s3,s4
 864:	000a071b          	sext.w	a4,s4
 868:	6685                	lui	a3,0x1
 86a:	00d77363          	bgeu	a4,a3,870 <malloc+0x44>
 86e:	6985                	lui	s3,0x1
 870:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 874:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 878:	00000917          	auipc	s2,0x0
 87c:	3b090913          	addi	s2,s2,944 # c28 <freep>
  if(p == (char*)-1)
 880:	5afd                	li	s5,-1
 882:	a8a5                	j	8fa <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 884:	00000797          	auipc	a5,0x0
 888:	3a478793          	addi	a5,a5,932 # c28 <freep>
 88c:	00000717          	auipc	a4,0x0
 890:	3a470713          	addi	a4,a4,932 # c30 <base>
 894:	e398                	sd	a4,0(a5)
 896:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 898:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 89c:	87ba                	mv	a5,a4
 89e:	b7d1                	j	862 <malloc+0x36>
      if(p->s.size == nunits)
 8a0:	02e48c63          	beq	s1,a4,8d8 <malloc+0xac>
        p->s.size -= nunits;
 8a4:	4147073b          	subw	a4,a4,s4
 8a8:	c798                	sw	a4,8(a5)
        p += p->s.size;
 8aa:	02071693          	slli	a3,a4,0x20
 8ae:	01c6d713          	srli	a4,a3,0x1c
 8b2:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 8b4:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 8b8:	00000717          	auipc	a4,0x0
 8bc:	36a73823          	sd	a0,880(a4) # c28 <freep>
      return (void*)(p + 1);
 8c0:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 8c4:	70e2                	ld	ra,56(sp)
 8c6:	7442                	ld	s0,48(sp)
 8c8:	74a2                	ld	s1,40(sp)
 8ca:	7902                	ld	s2,32(sp)
 8cc:	69e2                	ld	s3,24(sp)
 8ce:	6a42                	ld	s4,16(sp)
 8d0:	6aa2                	ld	s5,8(sp)
 8d2:	6b02                	ld	s6,0(sp)
 8d4:	6121                	addi	sp,sp,64
 8d6:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 8d8:	6398                	ld	a4,0(a5)
 8da:	e118                	sd	a4,0(a0)
 8dc:	bff1                	j	8b8 <malloc+0x8c>
  hp->s.size = nu;
 8de:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 8e2:	0541                	addi	a0,a0,16
 8e4:	00000097          	auipc	ra,0x0
 8e8:	ec6080e7          	jalr	-314(ra) # 7aa <free>
  return freep;
 8ec:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 8f0:	d971                	beqz	a0,8c4 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 8f2:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 8f4:	4798                	lw	a4,8(a5)
 8f6:	fa9775e3          	bgeu	a4,s1,8a0 <malloc+0x74>
    if(p == freep)
 8fa:	00093703          	ld	a4,0(s2)
 8fe:	853e                	mv	a0,a5
 900:	fef719e3          	bne	a4,a5,8f2 <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 904:	854e                	mv	a0,s3
 906:	00000097          	auipc	ra,0x0
 90a:	b34080e7          	jalr	-1228(ra) # 43a <sbrk>
  if(p == (char*)-1)
 90e:	fd5518e3          	bne	a0,s5,8de <malloc+0xb2>
        return 0;
 912:	4501                	li	a0,0
 914:	bf45                	j	8c4 <malloc+0x98>
