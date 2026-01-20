
xv6-user/_kill:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <main>:
#include "kernel/include/stat.h"
#include "xv6-user/user.h"

int
main(int argc, char **argv)
{
   0:	1101                	addi	sp,sp,-32
   2:	ec06                	sd	ra,24(sp)
   4:	e822                	sd	s0,16(sp)
   6:	e426                	sd	s1,8(sp)
   8:	e04a                	sd	s2,0(sp)
   a:	1000                	addi	s0,sp,32
  int i;

  if(argc < 2){
   c:	4785                	li	a5,1
   e:	02a7dd63          	bge	a5,a0,48 <main+0x48>
  12:	00858493          	addi	s1,a1,8
  16:	ffe5091b          	addiw	s2,a0,-2
  1a:	02091793          	slli	a5,s2,0x20
  1e:	01d7d913          	srli	s2,a5,0x1d
  22:	05c1                	addi	a1,a1,16
  24:	992e                	add	s2,s2,a1
    fprintf(2, "usage: kill pid...\n");
    exit(1);
  }
  for(i=1; i<argc; i++)
    kill(atoi(argv[i]));
  26:	6088                	ld	a0,0(s1)
  28:	00000097          	auipc	ra,0x0
  2c:	1e2080e7          	jalr	482(ra) # 20a <atoi>
  30:	00000097          	auipc	ra,0x0
  34:	328080e7          	jalr	808(ra) # 358 <kill>
  for(i=1; i<argc; i++)
  38:	04a1                	addi	s1,s1,8
  3a:	ff2496e3          	bne	s1,s2,26 <main+0x26>
  exit(0);
  3e:	4501                	li	a0,0
  40:	00000097          	auipc	ra,0x0
  44:	2dc080e7          	jalr	732(ra) # 31c <exit>
    fprintf(2, "usage: kill pid...\n");
  48:	00001597          	auipc	a1,0x1
  4c:	83858593          	addi	a1,a1,-1992 # 880 <malloc+0xea>
  50:	4509                	li	a0,2
  52:	00000097          	auipc	ra,0x0
  56:	65e080e7          	jalr	1630(ra) # 6b0 <fprintf>
    exit(1);
  5a:	4505                	li	a0,1
  5c:	00000097          	auipc	ra,0x0
  60:	2c0080e7          	jalr	704(ra) # 31c <exit>

0000000000000064 <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
  64:	1141                	addi	sp,sp,-16
  66:	e422                	sd	s0,8(sp)
  68:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
  6a:	87aa                	mv	a5,a0
  6c:	0585                	addi	a1,a1,1
  6e:	0785                	addi	a5,a5,1
  70:	fff5c703          	lbu	a4,-1(a1)
  74:	fee78fa3          	sb	a4,-1(a5)
  78:	fb75                	bnez	a4,6c <strcpy+0x8>
    ;
  return os;
}
  7a:	6422                	ld	s0,8(sp)
  7c:	0141                	addi	sp,sp,16
  7e:	8082                	ret

0000000000000080 <strcat>:

char*
strcat(char *s, const char *t)
{
  80:	1141                	addi	sp,sp,-16
  82:	e422                	sd	s0,8(sp)
  84:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
  86:	00054783          	lbu	a5,0(a0)
  8a:	c385                	beqz	a5,aa <strcat+0x2a>
  8c:	87aa                	mv	a5,a0
    s++;
  8e:	0785                	addi	a5,a5,1
  while(*s)
  90:	0007c703          	lbu	a4,0(a5)
  94:	ff6d                	bnez	a4,8e <strcat+0xe>
  while((*s++ = *t++))
  96:	0585                	addi	a1,a1,1
  98:	0785                	addi	a5,a5,1
  9a:	fff5c703          	lbu	a4,-1(a1)
  9e:	fee78fa3          	sb	a4,-1(a5)
  a2:	fb75                	bnez	a4,96 <strcat+0x16>
    ;
  return os;
}
  a4:	6422                	ld	s0,8(sp)
  a6:	0141                	addi	sp,sp,16
  a8:	8082                	ret
  while(*s)
  aa:	87aa                	mv	a5,a0
  ac:	b7ed                	j	96 <strcat+0x16>

00000000000000ae <strcmp>:


int
strcmp(const char *p, const char *q)
{
  ae:	1141                	addi	sp,sp,-16
  b0:	e422                	sd	s0,8(sp)
  b2:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
  b4:	00054783          	lbu	a5,0(a0)
  b8:	cb91                	beqz	a5,cc <strcmp+0x1e>
  ba:	0005c703          	lbu	a4,0(a1)
  be:	00f71763          	bne	a4,a5,cc <strcmp+0x1e>
    p++, q++;
  c2:	0505                	addi	a0,a0,1
  c4:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
  c6:	00054783          	lbu	a5,0(a0)
  ca:	fbe5                	bnez	a5,ba <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
  cc:	0005c503          	lbu	a0,0(a1)
}
  d0:	40a7853b          	subw	a0,a5,a0
  d4:	6422                	ld	s0,8(sp)
  d6:	0141                	addi	sp,sp,16
  d8:	8082                	ret

00000000000000da <strlen>:

uint
strlen(const char *s)
{
  da:	1141                	addi	sp,sp,-16
  dc:	e422                	sd	s0,8(sp)
  de:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
  e0:	00054783          	lbu	a5,0(a0)
  e4:	cf91                	beqz	a5,100 <strlen+0x26>
  e6:	0505                	addi	a0,a0,1
  e8:	87aa                	mv	a5,a0
  ea:	4685                	li	a3,1
  ec:	9e89                	subw	a3,a3,a0
  ee:	00f6853b          	addw	a0,a3,a5
  f2:	0785                	addi	a5,a5,1
  f4:	fff7c703          	lbu	a4,-1(a5)
  f8:	fb7d                	bnez	a4,ee <strlen+0x14>
    ;
  return n;
}
  fa:	6422                	ld	s0,8(sp)
  fc:	0141                	addi	sp,sp,16
  fe:	8082                	ret
  for(n = 0; s[n]; n++)
 100:	4501                	li	a0,0
 102:	bfe5                	j	fa <strlen+0x20>

0000000000000104 <memset>:

void*
memset(void *dst, int c, uint n)
{
 104:	1141                	addi	sp,sp,-16
 106:	e422                	sd	s0,8(sp)
 108:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 10a:	ca19                	beqz	a2,120 <memset+0x1c>
 10c:	87aa                	mv	a5,a0
 10e:	1602                	slli	a2,a2,0x20
 110:	9201                	srli	a2,a2,0x20
 112:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 116:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 11a:	0785                	addi	a5,a5,1
 11c:	fee79de3          	bne	a5,a4,116 <memset+0x12>
  }
  return dst;
}
 120:	6422                	ld	s0,8(sp)
 122:	0141                	addi	sp,sp,16
 124:	8082                	ret

0000000000000126 <strchr>:

char*
strchr(const char *s, char c)
{
 126:	1141                	addi	sp,sp,-16
 128:	e422                	sd	s0,8(sp)
 12a:	0800                	addi	s0,sp,16
  for(; *s; s++)
 12c:	00054783          	lbu	a5,0(a0)
 130:	cb99                	beqz	a5,146 <strchr+0x20>
    if(*s == c)
 132:	00f58763          	beq	a1,a5,140 <strchr+0x1a>
  for(; *s; s++)
 136:	0505                	addi	a0,a0,1
 138:	00054783          	lbu	a5,0(a0)
 13c:	fbfd                	bnez	a5,132 <strchr+0xc>
      return (char*)s;
  return 0;
 13e:	4501                	li	a0,0
}
 140:	6422                	ld	s0,8(sp)
 142:	0141                	addi	sp,sp,16
 144:	8082                	ret
  return 0;
 146:	4501                	li	a0,0
 148:	bfe5                	j	140 <strchr+0x1a>

000000000000014a <gets>:

char*
gets(char *buf, int max)
{
 14a:	711d                	addi	sp,sp,-96
 14c:	ec86                	sd	ra,88(sp)
 14e:	e8a2                	sd	s0,80(sp)
 150:	e4a6                	sd	s1,72(sp)
 152:	e0ca                	sd	s2,64(sp)
 154:	fc4e                	sd	s3,56(sp)
 156:	f852                	sd	s4,48(sp)
 158:	f456                	sd	s5,40(sp)
 15a:	f05a                	sd	s6,32(sp)
 15c:	ec5e                	sd	s7,24(sp)
 15e:	e862                	sd	s8,16(sp)
 160:	1080                	addi	s0,sp,96
 162:	8baa                	mv	s7,a0
 164:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 166:	892a                	mv	s2,a0
 168:	4481                	li	s1,0
    cc = read(0, &c, 1);
 16a:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 16e:	4b29                	li	s6,10
 170:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 172:	89a6                	mv	s3,s1
 174:	2485                	addiw	s1,s1,1
 176:	0344d763          	bge	s1,s4,1a4 <gets+0x5a>
    cc = read(0, &c, 1);
 17a:	4605                	li	a2,1
 17c:	85d6                	mv	a1,s5
 17e:	4501                	li	a0,0
 180:	00000097          	auipc	ra,0x0
 184:	1ba080e7          	jalr	442(ra) # 33a <read>
    if(cc < 1)
 188:	00a05e63          	blez	a0,1a4 <gets+0x5a>
    buf[i++] = c;
 18c:	faf44783          	lbu	a5,-81(s0)
 190:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 194:	01678763          	beq	a5,s6,1a2 <gets+0x58>
 198:	0905                	addi	s2,s2,1
 19a:	fd879ce3          	bne	a5,s8,172 <gets+0x28>
  for(i=0; i+1 < max; ){
 19e:	89a6                	mv	s3,s1
 1a0:	a011                	j	1a4 <gets+0x5a>
 1a2:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 1a4:	99de                	add	s3,s3,s7
 1a6:	00098023          	sb	zero,0(s3)
  return buf;
}
 1aa:	855e                	mv	a0,s7
 1ac:	60e6                	ld	ra,88(sp)
 1ae:	6446                	ld	s0,80(sp)
 1b0:	64a6                	ld	s1,72(sp)
 1b2:	6906                	ld	s2,64(sp)
 1b4:	79e2                	ld	s3,56(sp)
 1b6:	7a42                	ld	s4,48(sp)
 1b8:	7aa2                	ld	s5,40(sp)
 1ba:	7b02                	ld	s6,32(sp)
 1bc:	6be2                	ld	s7,24(sp)
 1be:	6c42                	ld	s8,16(sp)
 1c0:	6125                	addi	sp,sp,96
 1c2:	8082                	ret

00000000000001c4 <stat>:

int
stat(const char *n, struct stat *st)
{
 1c4:	1101                	addi	sp,sp,-32
 1c6:	ec06                	sd	ra,24(sp)
 1c8:	e822                	sd	s0,16(sp)
 1ca:	e426                	sd	s1,8(sp)
 1cc:	e04a                	sd	s2,0(sp)
 1ce:	1000                	addi	s0,sp,32
 1d0:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 1d2:	4581                	li	a1,0
 1d4:	00000097          	auipc	ra,0x0
 1d8:	196080e7          	jalr	406(ra) # 36a <open>
  if(fd < 0)
 1dc:	02054563          	bltz	a0,206 <stat+0x42>
 1e0:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 1e2:	85ca                	mv	a1,s2
 1e4:	00000097          	auipc	ra,0x0
 1e8:	190080e7          	jalr	400(ra) # 374 <fstat>
 1ec:	892a                	mv	s2,a0
  close(fd);
 1ee:	8526                	mv	a0,s1
 1f0:	00000097          	auipc	ra,0x0
 1f4:	15e080e7          	jalr	350(ra) # 34e <close>
  return r;
}
 1f8:	854a                	mv	a0,s2
 1fa:	60e2                	ld	ra,24(sp)
 1fc:	6442                	ld	s0,16(sp)
 1fe:	64a2                	ld	s1,8(sp)
 200:	6902                	ld	s2,0(sp)
 202:	6105                	addi	sp,sp,32
 204:	8082                	ret
    return -1;
 206:	597d                	li	s2,-1
 208:	bfc5                	j	1f8 <stat+0x34>

000000000000020a <atoi>:

int
atoi(const char *s)
{
 20a:	1141                	addi	sp,sp,-16
 20c:	e422                	sd	s0,8(sp)
 20e:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 210:	00054703          	lbu	a4,0(a0)
 214:	02d00793          	li	a5,45
  int neg = 1;
 218:	4585                	li	a1,1
  if (*s == '-') {
 21a:	04f70363          	beq	a4,a5,260 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 21e:	00054703          	lbu	a4,0(a0)
 222:	fd07079b          	addiw	a5,a4,-48
 226:	0ff7f793          	zext.b	a5,a5
 22a:	46a5                	li	a3,9
 22c:	02f6ed63          	bltu	a3,a5,266 <atoi+0x5c>
  n = 0;
 230:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 232:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 234:	0505                	addi	a0,a0,1
 236:	0026979b          	slliw	a5,a3,0x2
 23a:	9fb5                	addw	a5,a5,a3
 23c:	0017979b          	slliw	a5,a5,0x1
 240:	9fb9                	addw	a5,a5,a4
 242:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 246:	00054703          	lbu	a4,0(a0)
 24a:	fd07079b          	addiw	a5,a4,-48
 24e:	0ff7f793          	zext.b	a5,a5
 252:	fef671e3          	bgeu	a2,a5,234 <atoi+0x2a>
  return n * neg;
}
 256:	02d5853b          	mulw	a0,a1,a3
 25a:	6422                	ld	s0,8(sp)
 25c:	0141                	addi	sp,sp,16
 25e:	8082                	ret
    s++;
 260:	0505                	addi	a0,a0,1
    neg = -1;
 262:	55fd                	li	a1,-1
 264:	bf6d                	j	21e <atoi+0x14>
  n = 0;
 266:	4681                	li	a3,0
 268:	b7fd                	j	256 <atoi+0x4c>

000000000000026a <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 26a:	1141                	addi	sp,sp,-16
 26c:	e422                	sd	s0,8(sp)
 26e:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 270:	02b57463          	bgeu	a0,a1,298 <memmove+0x2e>
    while(n-- > 0)
 274:	00c05f63          	blez	a2,292 <memmove+0x28>
 278:	1602                	slli	a2,a2,0x20
 27a:	9201                	srli	a2,a2,0x20
 27c:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 280:	872a                	mv	a4,a0
      *dst++ = *src++;
 282:	0585                	addi	a1,a1,1
 284:	0705                	addi	a4,a4,1
 286:	fff5c683          	lbu	a3,-1(a1)
 28a:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 28e:	fee79ae3          	bne	a5,a4,282 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 292:	6422                	ld	s0,8(sp)
 294:	0141                	addi	sp,sp,16
 296:	8082                	ret
    dst += n;
 298:	00c50733          	add	a4,a0,a2
    src += n;
 29c:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 29e:	fec05ae3          	blez	a2,292 <memmove+0x28>
 2a2:	fff6079b          	addiw	a5,a2,-1
 2a6:	1782                	slli	a5,a5,0x20
 2a8:	9381                	srli	a5,a5,0x20
 2aa:	fff7c793          	not	a5,a5
 2ae:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 2b0:	15fd                	addi	a1,a1,-1
 2b2:	177d                	addi	a4,a4,-1
 2b4:	0005c683          	lbu	a3,0(a1)
 2b8:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 2bc:	fee79ae3          	bne	a5,a4,2b0 <memmove+0x46>
 2c0:	bfc9                	j	292 <memmove+0x28>

00000000000002c2 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 2c2:	1141                	addi	sp,sp,-16
 2c4:	e422                	sd	s0,8(sp)
 2c6:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 2c8:	ca05                	beqz	a2,2f8 <memcmp+0x36>
 2ca:	fff6069b          	addiw	a3,a2,-1
 2ce:	1682                	slli	a3,a3,0x20
 2d0:	9281                	srli	a3,a3,0x20
 2d2:	0685                	addi	a3,a3,1
 2d4:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 2d6:	00054783          	lbu	a5,0(a0)
 2da:	0005c703          	lbu	a4,0(a1)
 2de:	00e79863          	bne	a5,a4,2ee <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 2e2:	0505                	addi	a0,a0,1
    p2++;
 2e4:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 2e6:	fed518e3          	bne	a0,a3,2d6 <memcmp+0x14>
  }
  return 0;
 2ea:	4501                	li	a0,0
 2ec:	a019                	j	2f2 <memcmp+0x30>
      return *p1 - *p2;
 2ee:	40e7853b          	subw	a0,a5,a4
}
 2f2:	6422                	ld	s0,8(sp)
 2f4:	0141                	addi	sp,sp,16
 2f6:	8082                	ret
  return 0;
 2f8:	4501                	li	a0,0
 2fa:	bfe5                	j	2f2 <memcmp+0x30>

00000000000002fc <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 2fc:	1141                	addi	sp,sp,-16
 2fe:	e406                	sd	ra,8(sp)
 300:	e022                	sd	s0,0(sp)
 302:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 304:	00000097          	auipc	ra,0x0
 308:	f66080e7          	jalr	-154(ra) # 26a <memmove>
}
 30c:	60a2                	ld	ra,8(sp)
 30e:	6402                	ld	s0,0(sp)
 310:	0141                	addi	sp,sp,16
 312:	8082                	ret

0000000000000314 <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 314:	4885                	li	a7,1
 ecall
 316:	00000073          	ecall
 ret
 31a:	8082                	ret

000000000000031c <exit>:
.global exit
exit:
 li a7, SYS_exit
 31c:	05d00893          	li	a7,93
 ecall
 320:	00000073          	ecall
 ret
 324:	8082                	ret

0000000000000326 <wait>:
.global wait
wait:
 li a7, SYS_wait
 326:	4bf00893          	li	a7,1215
 ecall
 32a:	00000073          	ecall
 ret
 32e:	8082                	ret

0000000000000330 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 330:	03b00893          	li	a7,59
 ecall
 334:	00000073          	ecall
 ret
 338:	8082                	ret

000000000000033a <read>:
.global read
read:
 li a7, SYS_read
 33a:	03f00893          	li	a7,63
 ecall
 33e:	00000073          	ecall
 ret
 342:	8082                	ret

0000000000000344 <write>:
.global write
write:
 li a7, SYS_write
 344:	04000893          	li	a7,64
 ecall
 348:	00000073          	ecall
 ret
 34c:	8082                	ret

000000000000034e <close>:
.global close
close:
 li a7, SYS_close
 34e:	03900893          	li	a7,57
 ecall
 352:	00000073          	ecall
 ret
 356:	8082                	ret

0000000000000358 <kill>:
.global kill
kill:
 li a7, SYS_kill
 358:	4899                	li	a7,6
 ecall
 35a:	00000073          	ecall
 ret
 35e:	8082                	ret

0000000000000360 <exec>:
.global exec
exec:
 li a7, SYS_exec
 360:	0dd00893          	li	a7,221
 ecall
 364:	00000073          	ecall
 ret
 368:	8082                	ret

000000000000036a <open>:
.global open
open:
 li a7, SYS_open
 36a:	03800893          	li	a7,56
 ecall
 36e:	00000073          	ecall
 ret
 372:	8082                	ret

0000000000000374 <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 374:	05000893          	li	a7,80
 ecall
 378:	00000073          	ecall
 ret
 37c:	8082                	ret

000000000000037e <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 37e:	02200893          	li	a7,34
 ecall
 382:	00000073          	ecall
 ret
 386:	8082                	ret

0000000000000388 <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 388:	03100893          	li	a7,49
 ecall
 38c:	00000073          	ecall
 ret
 390:	8082                	ret

0000000000000392 <dup>:
.global dup
dup:
 li a7, SYS_dup
 392:	48dd                	li	a7,23
 ecall
 394:	00000073          	ecall
 ret
 398:	8082                	ret

000000000000039a <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 39a:	0ac00893          	li	a7,172
 ecall
 39e:	00000073          	ecall
 ret
 3a2:	8082                	ret

00000000000003a4 <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 3a4:	0d600893          	li	a7,214
 ecall
 3a8:	00000073          	ecall
 ret
 3ac:	8082                	ret

00000000000003ae <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 3ae:	48b5                	li	a7,13
 ecall
 3b0:	00000073          	ecall
 ret
 3b4:	8082                	ret

00000000000003b6 <times>:
.global times
times:
 li a7, SYS_times
 3b6:	09900893          	li	a7,153
 ecall
 3ba:	00000073          	ecall
 ret
 3be:	8082                	ret

00000000000003c0 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 3c0:	48d9                	li	a7,22
 ecall
 3c2:	00000073          	ecall
 ret
 3c6:	8082                	ret

00000000000003c8 <dev>:
.global dev
dev:
 li a7, SYS_dev
 3c8:	6889                	lui	a7,0x2
 3ca:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x165d>
 ecall
 3ce:	00000073          	ecall
 ret
 3d2:	8082                	ret

00000000000003d4 <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 3d4:	7d200893          	li	a7,2002
 ecall
 3d8:	00000073          	ecall
 ret
 3dc:	8082                	ret

00000000000003de <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 3de:	48c5                	li	a7,17
 ecall
 3e0:	00000073          	ecall
 ret
 3e4:	8082                	ret

00000000000003e6 <remove>:
.global remove
remove:
 li a7, SYS_remove
 3e6:	48e5                	li	a7,25
 ecall
 3e8:	00000073          	ecall
 ret
 3ec:	8082                	ret

00000000000003ee <trace>:
.global trace
trace:
 li a7, SYS_trace
 3ee:	48c9                	li	a7,18
 ecall
 3f0:	00000073          	ecall
 ret
 3f4:	8082                	ret

00000000000003f6 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 3f6:	48cd                	li	a7,19
 ecall
 3f8:	00000073          	ecall
 ret
 3fc:	8082                	ret

00000000000003fe <rename>:
.global rename
rename:
 li a7, SYS_rename
 3fe:	48e9                	li	a7,26
 ecall
 400:	00000073          	ecall
 ret
 404:	8082                	ret

0000000000000406 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 406:	1101                	addi	sp,sp,-32
 408:	ec06                	sd	ra,24(sp)
 40a:	e822                	sd	s0,16(sp)
 40c:	1000                	addi	s0,sp,32
 40e:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 412:	4605                	li	a2,1
 414:	fef40593          	addi	a1,s0,-17
 418:	00000097          	auipc	ra,0x0
 41c:	f2c080e7          	jalr	-212(ra) # 344 <write>
}
 420:	60e2                	ld	ra,24(sp)
 422:	6442                	ld	s0,16(sp)
 424:	6105                	addi	sp,sp,32
 426:	8082                	ret

0000000000000428 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 428:	7139                	addi	sp,sp,-64
 42a:	fc06                	sd	ra,56(sp)
 42c:	f822                	sd	s0,48(sp)
 42e:	f426                	sd	s1,40(sp)
 430:	f04a                	sd	s2,32(sp)
 432:	ec4e                	sd	s3,24(sp)
 434:	0080                	addi	s0,sp,64
 436:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 438:	c299                	beqz	a3,43e <printint+0x16>
 43a:	0805c863          	bltz	a1,4ca <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 43e:	2581                	sext.w	a1,a1
  neg = 0;
 440:	4881                	li	a7,0
  }

  i = 0;
 442:	fc040993          	addi	s3,s0,-64
  neg = 0;
 446:	86ce                	mv	a3,s3
  i = 0;
 448:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 44a:	2601                	sext.w	a2,a2
 44c:	00000517          	auipc	a0,0x0
 450:	4ac50513          	addi	a0,a0,1196 # 8f8 <digits>
 454:	883a                	mv	a6,a4
 456:	2705                	addiw	a4,a4,1
 458:	02c5f7bb          	remuw	a5,a1,a2
 45c:	1782                	slli	a5,a5,0x20
 45e:	9381                	srli	a5,a5,0x20
 460:	97aa                	add	a5,a5,a0
 462:	0007c783          	lbu	a5,0(a5)
 466:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 46a:	0005879b          	sext.w	a5,a1
 46e:	02c5d5bb          	divuw	a1,a1,a2
 472:	0685                	addi	a3,a3,1
 474:	fec7f0e3          	bgeu	a5,a2,454 <printint+0x2c>
  if(neg)
 478:	00088c63          	beqz	a7,490 <printint+0x68>
    buf[i++] = '-';
 47c:	fd070793          	addi	a5,a4,-48
 480:	00878733          	add	a4,a5,s0
 484:	02d00793          	li	a5,45
 488:	fef70823          	sb	a5,-16(a4)
 48c:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 490:	02e05663          	blez	a4,4bc <printint+0x94>
 494:	fc040913          	addi	s2,s0,-64
 498:	993a                	add	s2,s2,a4
 49a:	19fd                	addi	s3,s3,-1
 49c:	99ba                	add	s3,s3,a4
 49e:	377d                	addiw	a4,a4,-1
 4a0:	1702                	slli	a4,a4,0x20
 4a2:	9301                	srli	a4,a4,0x20
 4a4:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 4a8:	fff94583          	lbu	a1,-1(s2)
 4ac:	8526                	mv	a0,s1
 4ae:	00000097          	auipc	ra,0x0
 4b2:	f58080e7          	jalr	-168(ra) # 406 <putc>
  while(--i >= 0)
 4b6:	197d                	addi	s2,s2,-1
 4b8:	ff3918e3          	bne	s2,s3,4a8 <printint+0x80>
}
 4bc:	70e2                	ld	ra,56(sp)
 4be:	7442                	ld	s0,48(sp)
 4c0:	74a2                	ld	s1,40(sp)
 4c2:	7902                	ld	s2,32(sp)
 4c4:	69e2                	ld	s3,24(sp)
 4c6:	6121                	addi	sp,sp,64
 4c8:	8082                	ret
    x = -xx;
 4ca:	40b005bb          	negw	a1,a1
    neg = 1;
 4ce:	4885                	li	a7,1
    x = -xx;
 4d0:	bf8d                	j	442 <printint+0x1a>

00000000000004d2 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 4d2:	7119                	addi	sp,sp,-128
 4d4:	fc86                	sd	ra,120(sp)
 4d6:	f8a2                	sd	s0,112(sp)
 4d8:	f4a6                	sd	s1,104(sp)
 4da:	f0ca                	sd	s2,96(sp)
 4dc:	ecce                	sd	s3,88(sp)
 4de:	e8d2                	sd	s4,80(sp)
 4e0:	e4d6                	sd	s5,72(sp)
 4e2:	e0da                	sd	s6,64(sp)
 4e4:	fc5e                	sd	s7,56(sp)
 4e6:	f862                	sd	s8,48(sp)
 4e8:	f466                	sd	s9,40(sp)
 4ea:	f06a                	sd	s10,32(sp)
 4ec:	ec6e                	sd	s11,24(sp)
 4ee:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 4f0:	0005c903          	lbu	s2,0(a1)
 4f4:	18090f63          	beqz	s2,692 <vprintf+0x1c0>
 4f8:	8aaa                	mv	s5,a0
 4fa:	8b32                	mv	s6,a2
 4fc:	00158493          	addi	s1,a1,1
  state = 0;
 500:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 502:	02500a13          	li	s4,37
 506:	4c55                	li	s8,21
 508:	00000c97          	auipc	s9,0x0
 50c:	398c8c93          	addi	s9,s9,920 # 8a0 <malloc+0x10a>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 510:	02800d93          	li	s11,40
  putc(fd, 'x');
 514:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 516:	00000b97          	auipc	s7,0x0
 51a:	3e2b8b93          	addi	s7,s7,994 # 8f8 <digits>
 51e:	a839                	j	53c <vprintf+0x6a>
        putc(fd, c);
 520:	85ca                	mv	a1,s2
 522:	8556                	mv	a0,s5
 524:	00000097          	auipc	ra,0x0
 528:	ee2080e7          	jalr	-286(ra) # 406 <putc>
 52c:	a019                	j	532 <vprintf+0x60>
    } else if(state == '%'){
 52e:	01498d63          	beq	s3,s4,548 <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 532:	0485                	addi	s1,s1,1
 534:	fff4c903          	lbu	s2,-1(s1)
 538:	14090d63          	beqz	s2,692 <vprintf+0x1c0>
    if(state == 0){
 53c:	fe0999e3          	bnez	s3,52e <vprintf+0x5c>
      if(c == '%'){
 540:	ff4910e3          	bne	s2,s4,520 <vprintf+0x4e>
        state = '%';
 544:	89d2                	mv	s3,s4
 546:	b7f5                	j	532 <vprintf+0x60>
      if(c == 'd'){
 548:	11490c63          	beq	s2,s4,660 <vprintf+0x18e>
 54c:	f9d9079b          	addiw	a5,s2,-99
 550:	0ff7f793          	zext.b	a5,a5
 554:	10fc6e63          	bltu	s8,a5,670 <vprintf+0x19e>
 558:	f9d9079b          	addiw	a5,s2,-99
 55c:	0ff7f713          	zext.b	a4,a5
 560:	10ec6863          	bltu	s8,a4,670 <vprintf+0x19e>
 564:	00271793          	slli	a5,a4,0x2
 568:	97e6                	add	a5,a5,s9
 56a:	439c                	lw	a5,0(a5)
 56c:	97e6                	add	a5,a5,s9
 56e:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 570:	008b0913          	addi	s2,s6,8
 574:	4685                	li	a3,1
 576:	4629                	li	a2,10
 578:	000b2583          	lw	a1,0(s6)
 57c:	8556                	mv	a0,s5
 57e:	00000097          	auipc	ra,0x0
 582:	eaa080e7          	jalr	-342(ra) # 428 <printint>
 586:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 588:	4981                	li	s3,0
 58a:	b765                	j	532 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 58c:	008b0913          	addi	s2,s6,8
 590:	4681                	li	a3,0
 592:	4629                	li	a2,10
 594:	000b2583          	lw	a1,0(s6)
 598:	8556                	mv	a0,s5
 59a:	00000097          	auipc	ra,0x0
 59e:	e8e080e7          	jalr	-370(ra) # 428 <printint>
 5a2:	8b4a                	mv	s6,s2
      state = 0;
 5a4:	4981                	li	s3,0
 5a6:	b771                	j	532 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 5a8:	008b0913          	addi	s2,s6,8
 5ac:	4681                	li	a3,0
 5ae:	866a                	mv	a2,s10
 5b0:	000b2583          	lw	a1,0(s6)
 5b4:	8556                	mv	a0,s5
 5b6:	00000097          	auipc	ra,0x0
 5ba:	e72080e7          	jalr	-398(ra) # 428 <printint>
 5be:	8b4a                	mv	s6,s2
      state = 0;
 5c0:	4981                	li	s3,0
 5c2:	bf85                	j	532 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 5c4:	008b0793          	addi	a5,s6,8
 5c8:	f8f43423          	sd	a5,-120(s0)
 5cc:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 5d0:	03000593          	li	a1,48
 5d4:	8556                	mv	a0,s5
 5d6:	00000097          	auipc	ra,0x0
 5da:	e30080e7          	jalr	-464(ra) # 406 <putc>
  putc(fd, 'x');
 5de:	07800593          	li	a1,120
 5e2:	8556                	mv	a0,s5
 5e4:	00000097          	auipc	ra,0x0
 5e8:	e22080e7          	jalr	-478(ra) # 406 <putc>
 5ec:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 5ee:	03c9d793          	srli	a5,s3,0x3c
 5f2:	97de                	add	a5,a5,s7
 5f4:	0007c583          	lbu	a1,0(a5)
 5f8:	8556                	mv	a0,s5
 5fa:	00000097          	auipc	ra,0x0
 5fe:	e0c080e7          	jalr	-500(ra) # 406 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 602:	0992                	slli	s3,s3,0x4
 604:	397d                	addiw	s2,s2,-1
 606:	fe0914e3          	bnez	s2,5ee <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 60a:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 60e:	4981                	li	s3,0
 610:	b70d                	j	532 <vprintf+0x60>
        s = va_arg(ap, char*);
 612:	008b0913          	addi	s2,s6,8
 616:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 61a:	02098163          	beqz	s3,63c <vprintf+0x16a>
        while(*s != 0){
 61e:	0009c583          	lbu	a1,0(s3)
 622:	c5ad                	beqz	a1,68c <vprintf+0x1ba>
          putc(fd, *s);
 624:	8556                	mv	a0,s5
 626:	00000097          	auipc	ra,0x0
 62a:	de0080e7          	jalr	-544(ra) # 406 <putc>
          s++;
 62e:	0985                	addi	s3,s3,1
        while(*s != 0){
 630:	0009c583          	lbu	a1,0(s3)
 634:	f9e5                	bnez	a1,624 <vprintf+0x152>
        s = va_arg(ap, char*);
 636:	8b4a                	mv	s6,s2
      state = 0;
 638:	4981                	li	s3,0
 63a:	bde5                	j	532 <vprintf+0x60>
          s = "(null)";
 63c:	00000997          	auipc	s3,0x0
 640:	25c98993          	addi	s3,s3,604 # 898 <malloc+0x102>
        while(*s != 0){
 644:	85ee                	mv	a1,s11
 646:	bff9                	j	624 <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 648:	008b0913          	addi	s2,s6,8
 64c:	000b4583          	lbu	a1,0(s6)
 650:	8556                	mv	a0,s5
 652:	00000097          	auipc	ra,0x0
 656:	db4080e7          	jalr	-588(ra) # 406 <putc>
 65a:	8b4a                	mv	s6,s2
      state = 0;
 65c:	4981                	li	s3,0
 65e:	bdd1                	j	532 <vprintf+0x60>
        putc(fd, c);
 660:	85d2                	mv	a1,s4
 662:	8556                	mv	a0,s5
 664:	00000097          	auipc	ra,0x0
 668:	da2080e7          	jalr	-606(ra) # 406 <putc>
      state = 0;
 66c:	4981                	li	s3,0
 66e:	b5d1                	j	532 <vprintf+0x60>
        putc(fd, '%');
 670:	85d2                	mv	a1,s4
 672:	8556                	mv	a0,s5
 674:	00000097          	auipc	ra,0x0
 678:	d92080e7          	jalr	-622(ra) # 406 <putc>
        putc(fd, c);
 67c:	85ca                	mv	a1,s2
 67e:	8556                	mv	a0,s5
 680:	00000097          	auipc	ra,0x0
 684:	d86080e7          	jalr	-634(ra) # 406 <putc>
      state = 0;
 688:	4981                	li	s3,0
 68a:	b565                	j	532 <vprintf+0x60>
        s = va_arg(ap, char*);
 68c:	8b4a                	mv	s6,s2
      state = 0;
 68e:	4981                	li	s3,0
 690:	b54d                	j	532 <vprintf+0x60>
    }
  }
}
 692:	70e6                	ld	ra,120(sp)
 694:	7446                	ld	s0,112(sp)
 696:	74a6                	ld	s1,104(sp)
 698:	7906                	ld	s2,96(sp)
 69a:	69e6                	ld	s3,88(sp)
 69c:	6a46                	ld	s4,80(sp)
 69e:	6aa6                	ld	s5,72(sp)
 6a0:	6b06                	ld	s6,64(sp)
 6a2:	7be2                	ld	s7,56(sp)
 6a4:	7c42                	ld	s8,48(sp)
 6a6:	7ca2                	ld	s9,40(sp)
 6a8:	7d02                	ld	s10,32(sp)
 6aa:	6de2                	ld	s11,24(sp)
 6ac:	6109                	addi	sp,sp,128
 6ae:	8082                	ret

00000000000006b0 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 6b0:	715d                	addi	sp,sp,-80
 6b2:	ec06                	sd	ra,24(sp)
 6b4:	e822                	sd	s0,16(sp)
 6b6:	1000                	addi	s0,sp,32
 6b8:	e010                	sd	a2,0(s0)
 6ba:	e414                	sd	a3,8(s0)
 6bc:	e818                	sd	a4,16(s0)
 6be:	ec1c                	sd	a5,24(s0)
 6c0:	03043023          	sd	a6,32(s0)
 6c4:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 6c8:	8622                	mv	a2,s0
 6ca:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 6ce:	00000097          	auipc	ra,0x0
 6d2:	e04080e7          	jalr	-508(ra) # 4d2 <vprintf>
}
 6d6:	60e2                	ld	ra,24(sp)
 6d8:	6442                	ld	s0,16(sp)
 6da:	6161                	addi	sp,sp,80
 6dc:	8082                	ret

00000000000006de <printf>:

void
printf(const char *fmt, ...)
{
 6de:	711d                	addi	sp,sp,-96
 6e0:	ec06                	sd	ra,24(sp)
 6e2:	e822                	sd	s0,16(sp)
 6e4:	1000                	addi	s0,sp,32
 6e6:	e40c                	sd	a1,8(s0)
 6e8:	e810                	sd	a2,16(s0)
 6ea:	ec14                	sd	a3,24(s0)
 6ec:	f018                	sd	a4,32(s0)
 6ee:	f41c                	sd	a5,40(s0)
 6f0:	03043823          	sd	a6,48(s0)
 6f4:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 6f8:	00840613          	addi	a2,s0,8
 6fc:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 700:	85aa                	mv	a1,a0
 702:	4505                	li	a0,1
 704:	00000097          	auipc	ra,0x0
 708:	dce080e7          	jalr	-562(ra) # 4d2 <vprintf>
}
 70c:	60e2                	ld	ra,24(sp)
 70e:	6442                	ld	s0,16(sp)
 710:	6125                	addi	sp,sp,96
 712:	8082                	ret

0000000000000714 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 714:	1141                	addi	sp,sp,-16
 716:	e422                	sd	s0,8(sp)
 718:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 71a:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 71e:	00000797          	auipc	a5,0x0
 722:	1f27b783          	ld	a5,498(a5) # 910 <freep>
 726:	a02d                	j	750 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 728:	4618                	lw	a4,8(a2)
 72a:	9f2d                	addw	a4,a4,a1
 72c:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 730:	6398                	ld	a4,0(a5)
 732:	6310                	ld	a2,0(a4)
 734:	a83d                	j	772 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 736:	ff852703          	lw	a4,-8(a0)
 73a:	9f31                	addw	a4,a4,a2
 73c:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 73e:	ff053683          	ld	a3,-16(a0)
 742:	a091                	j	786 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 744:	6398                	ld	a4,0(a5)
 746:	00e7e463          	bltu	a5,a4,74e <free+0x3a>
 74a:	00e6ea63          	bltu	a3,a4,75e <free+0x4a>
{
 74e:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 750:	fed7fae3          	bgeu	a5,a3,744 <free+0x30>
 754:	6398                	ld	a4,0(a5)
 756:	00e6e463          	bltu	a3,a4,75e <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 75a:	fee7eae3          	bltu	a5,a4,74e <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 75e:	ff852583          	lw	a1,-8(a0)
 762:	6390                	ld	a2,0(a5)
 764:	02059813          	slli	a6,a1,0x20
 768:	01c85713          	srli	a4,a6,0x1c
 76c:	9736                	add	a4,a4,a3
 76e:	fae60de3          	beq	a2,a4,728 <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 772:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 776:	4790                	lw	a2,8(a5)
 778:	02061593          	slli	a1,a2,0x20
 77c:	01c5d713          	srli	a4,a1,0x1c
 780:	973e                	add	a4,a4,a5
 782:	fae68ae3          	beq	a3,a4,736 <free+0x22>
    p->s.ptr = bp->s.ptr;
 786:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 788:	00000717          	auipc	a4,0x0
 78c:	18f73423          	sd	a5,392(a4) # 910 <freep>
}
 790:	6422                	ld	s0,8(sp)
 792:	0141                	addi	sp,sp,16
 794:	8082                	ret

0000000000000796 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 796:	7139                	addi	sp,sp,-64
 798:	fc06                	sd	ra,56(sp)
 79a:	f822                	sd	s0,48(sp)
 79c:	f426                	sd	s1,40(sp)
 79e:	f04a                	sd	s2,32(sp)
 7a0:	ec4e                	sd	s3,24(sp)
 7a2:	e852                	sd	s4,16(sp)
 7a4:	e456                	sd	s5,8(sp)
 7a6:	e05a                	sd	s6,0(sp)
 7a8:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 7aa:	02051493          	slli	s1,a0,0x20
 7ae:	9081                	srli	s1,s1,0x20
 7b0:	04bd                	addi	s1,s1,15
 7b2:	8091                	srli	s1,s1,0x4
 7b4:	00148a1b          	addiw	s4,s1,1
 7b8:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 7ba:	00000517          	auipc	a0,0x0
 7be:	15653503          	ld	a0,342(a0) # 910 <freep>
 7c2:	c515                	beqz	a0,7ee <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 7c4:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 7c6:	4798                	lw	a4,8(a5)
 7c8:	04977163          	bgeu	a4,s1,80a <malloc+0x74>
 7cc:	89d2                	mv	s3,s4
 7ce:	000a071b          	sext.w	a4,s4
 7d2:	6685                	lui	a3,0x1
 7d4:	00d77363          	bgeu	a4,a3,7da <malloc+0x44>
 7d8:	6985                	lui	s3,0x1
 7da:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 7de:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 7e2:	00000917          	auipc	s2,0x0
 7e6:	12e90913          	addi	s2,s2,302 # 910 <freep>
  if(p == (char*)-1)
 7ea:	5afd                	li	s5,-1
 7ec:	a8a5                	j	864 <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 7ee:	00000797          	auipc	a5,0x0
 7f2:	12278793          	addi	a5,a5,290 # 910 <freep>
 7f6:	00000717          	auipc	a4,0x0
 7fa:	12270713          	addi	a4,a4,290 # 918 <base>
 7fe:	e398                	sd	a4,0(a5)
 800:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 802:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 806:	87ba                	mv	a5,a4
 808:	b7d1                	j	7cc <malloc+0x36>
      if(p->s.size == nunits)
 80a:	02e48c63          	beq	s1,a4,842 <malloc+0xac>
        p->s.size -= nunits;
 80e:	4147073b          	subw	a4,a4,s4
 812:	c798                	sw	a4,8(a5)
        p += p->s.size;
 814:	02071693          	slli	a3,a4,0x20
 818:	01c6d713          	srli	a4,a3,0x1c
 81c:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 81e:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 822:	00000717          	auipc	a4,0x0
 826:	0ea73723          	sd	a0,238(a4) # 910 <freep>
      return (void*)(p + 1);
 82a:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 82e:	70e2                	ld	ra,56(sp)
 830:	7442                	ld	s0,48(sp)
 832:	74a2                	ld	s1,40(sp)
 834:	7902                	ld	s2,32(sp)
 836:	69e2                	ld	s3,24(sp)
 838:	6a42                	ld	s4,16(sp)
 83a:	6aa2                	ld	s5,8(sp)
 83c:	6b02                	ld	s6,0(sp)
 83e:	6121                	addi	sp,sp,64
 840:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 842:	6398                	ld	a4,0(a5)
 844:	e118                	sd	a4,0(a0)
 846:	bff1                	j	822 <malloc+0x8c>
  hp->s.size = nu;
 848:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 84c:	0541                	addi	a0,a0,16
 84e:	00000097          	auipc	ra,0x0
 852:	ec6080e7          	jalr	-314(ra) # 714 <free>
  return freep;
 856:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 85a:	d971                	beqz	a0,82e <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 85c:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 85e:	4798                	lw	a4,8(a5)
 860:	fa9775e3          	bgeu	a4,s1,80a <malloc+0x74>
    if(p == freep)
 864:	00093703          	ld	a4,0(s2)
 868:	853e                	mv	a0,a5
 86a:	fef719e3          	bne	a4,a5,85c <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 86e:	854e                	mv	a0,s3
 870:	00000097          	auipc	ra,0x0
 874:	b34080e7          	jalr	-1228(ra) # 3a4 <sbrk>
  if(p == (char*)-1)
 878:	fd5518e3          	bne	a0,s5,848 <malloc+0xb2>
        return 0;
 87c:	4501                	li	a0,0
 87e:	bf45                	j	82e <malloc+0x98>
