
xv6-user/_strace:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <main>:
#include "kernel/include/stat.h"
#include "xv6-user/user.h"

int
main(int argc, char *argv[])
{
   0:	712d                	addi	sp,sp,-288
   2:	ee06                	sd	ra,280(sp)
   4:	ea22                	sd	s0,272(sp)
   6:	e626                	sd	s1,264(sp)
   8:	e24a                	sd	s2,256(sp)
   a:	1200                	addi	s0,sp,288
   c:	892e                	mv	s2,a1
  int i;
  char *nargv[MAXARG];

  if(argc < 3 /*|| (argv[1][0] < '0' || argv[1][0] > '9')*/){
   e:	4789                	li	a5,2
  10:	02a7c163          	blt	a5,a0,32 <main+0x32>
    fprintf(2, "usage: %s MASK COMMAND\n", argv[0]);
  14:	6190                	ld	a2,0(a1)
  16:	00001597          	auipc	a1,0x1
  1a:	8ca58593          	addi	a1,a1,-1846 # 8e0 <malloc+0xec>
  1e:	4509                	li	a0,2
  20:	00000097          	auipc	ra,0x0
  24:	6ee080e7          	jalr	1774(ra) # 70e <fprintf>
    exit(1);
  28:	4505                	li	a0,1
  2a:	00000097          	auipc	ra,0x0
  2e:	350080e7          	jalr	848(ra) # 37a <exit>
  32:	84aa                	mv	s1,a0
  }

  if (trace(atoi(argv[1])) < 0) {
  34:	6588                	ld	a0,8(a1)
  36:	00000097          	auipc	ra,0x0
  3a:	232080e7          	jalr	562(ra) # 268 <atoi>
  3e:	00000097          	auipc	ra,0x0
  42:	40e080e7          	jalr	1038(ra) # 44c <trace>
  46:	04054e63          	bltz	a0,a2 <main+0xa2>
  4a:	01090793          	addi	a5,s2,16
  4e:	ee040713          	addi	a4,s0,-288
  52:	ffd4869b          	addiw	a3,s1,-3
  56:	02069613          	slli	a2,a3,0x20
  5a:	01d65693          	srli	a3,a2,0x1d
  5e:	96be                	add	a3,a3,a5
  60:	10090593          	addi	a1,s2,256
    fprintf(2, "%s: strace failed\n", argv[0]);
    exit(1);
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  64:	6390                	ld	a2,0(a5)
  66:	e310                	sd	a2,0(a4)
  for(i = 2; i < argc && i < MAXARG; i++){
  68:	00d78663          	beq	a5,a3,74 <main+0x74>
  6c:	07a1                	addi	a5,a5,8
  6e:	0721                	addi	a4,a4,8
  70:	feb79ae3          	bne	a5,a1,64 <main+0x64>
  }
  exec(nargv[0], nargv);  
  74:	ee040593          	addi	a1,s0,-288
  78:	ee043503          	ld	a0,-288(s0)
  7c:	00000097          	auipc	ra,0x0
  80:	342080e7          	jalr	834(ra) # 3be <exec>
  printf("strace: exec %s fail\n", nargv[0]);
  84:	ee043583          	ld	a1,-288(s0)
  88:	00001517          	auipc	a0,0x1
  8c:	88850513          	addi	a0,a0,-1912 # 910 <malloc+0x11c>
  90:	00000097          	auipc	ra,0x0
  94:	6ac080e7          	jalr	1708(ra) # 73c <printf>
  exit(0);
  98:	4501                	li	a0,0
  9a:	00000097          	auipc	ra,0x0
  9e:	2e0080e7          	jalr	736(ra) # 37a <exit>
    fprintf(2, "%s: strace failed\n", argv[0]);
  a2:	00093603          	ld	a2,0(s2)
  a6:	00001597          	auipc	a1,0x1
  aa:	85258593          	addi	a1,a1,-1966 # 8f8 <malloc+0x104>
  ae:	4509                	li	a0,2
  b0:	00000097          	auipc	ra,0x0
  b4:	65e080e7          	jalr	1630(ra) # 70e <fprintf>
    exit(1);
  b8:	4505                	li	a0,1
  ba:	00000097          	auipc	ra,0x0
  be:	2c0080e7          	jalr	704(ra) # 37a <exit>

00000000000000c2 <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
  c2:	1141                	addi	sp,sp,-16
  c4:	e422                	sd	s0,8(sp)
  c6:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
  c8:	87aa                	mv	a5,a0
  ca:	0585                	addi	a1,a1,1
  cc:	0785                	addi	a5,a5,1
  ce:	fff5c703          	lbu	a4,-1(a1)
  d2:	fee78fa3          	sb	a4,-1(a5)
  d6:	fb75                	bnez	a4,ca <strcpy+0x8>
    ;
  return os;
}
  d8:	6422                	ld	s0,8(sp)
  da:	0141                	addi	sp,sp,16
  dc:	8082                	ret

00000000000000de <strcat>:

char*
strcat(char *s, const char *t)
{
  de:	1141                	addi	sp,sp,-16
  e0:	e422                	sd	s0,8(sp)
  e2:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
  e4:	00054783          	lbu	a5,0(a0)
  e8:	c385                	beqz	a5,108 <strcat+0x2a>
  ea:	87aa                	mv	a5,a0
    s++;
  ec:	0785                	addi	a5,a5,1
  while(*s)
  ee:	0007c703          	lbu	a4,0(a5)
  f2:	ff6d                	bnez	a4,ec <strcat+0xe>
  while((*s++ = *t++))
  f4:	0585                	addi	a1,a1,1
  f6:	0785                	addi	a5,a5,1
  f8:	fff5c703          	lbu	a4,-1(a1)
  fc:	fee78fa3          	sb	a4,-1(a5)
 100:	fb75                	bnez	a4,f4 <strcat+0x16>
    ;
  return os;
}
 102:	6422                	ld	s0,8(sp)
 104:	0141                	addi	sp,sp,16
 106:	8082                	ret
  while(*s)
 108:	87aa                	mv	a5,a0
 10a:	b7ed                	j	f4 <strcat+0x16>

000000000000010c <strcmp>:


int
strcmp(const char *p, const char *q)
{
 10c:	1141                	addi	sp,sp,-16
 10e:	e422                	sd	s0,8(sp)
 110:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 112:	00054783          	lbu	a5,0(a0)
 116:	cb91                	beqz	a5,12a <strcmp+0x1e>
 118:	0005c703          	lbu	a4,0(a1)
 11c:	00f71763          	bne	a4,a5,12a <strcmp+0x1e>
    p++, q++;
 120:	0505                	addi	a0,a0,1
 122:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 124:	00054783          	lbu	a5,0(a0)
 128:	fbe5                	bnez	a5,118 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 12a:	0005c503          	lbu	a0,0(a1)
}
 12e:	40a7853b          	subw	a0,a5,a0
 132:	6422                	ld	s0,8(sp)
 134:	0141                	addi	sp,sp,16
 136:	8082                	ret

0000000000000138 <strlen>:

uint
strlen(const char *s)
{
 138:	1141                	addi	sp,sp,-16
 13a:	e422                	sd	s0,8(sp)
 13c:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 13e:	00054783          	lbu	a5,0(a0)
 142:	cf91                	beqz	a5,15e <strlen+0x26>
 144:	0505                	addi	a0,a0,1
 146:	87aa                	mv	a5,a0
 148:	4685                	li	a3,1
 14a:	9e89                	subw	a3,a3,a0
 14c:	00f6853b          	addw	a0,a3,a5
 150:	0785                	addi	a5,a5,1
 152:	fff7c703          	lbu	a4,-1(a5)
 156:	fb7d                	bnez	a4,14c <strlen+0x14>
    ;
  return n;
}
 158:	6422                	ld	s0,8(sp)
 15a:	0141                	addi	sp,sp,16
 15c:	8082                	ret
  for(n = 0; s[n]; n++)
 15e:	4501                	li	a0,0
 160:	bfe5                	j	158 <strlen+0x20>

0000000000000162 <memset>:

void*
memset(void *dst, int c, uint n)
{
 162:	1141                	addi	sp,sp,-16
 164:	e422                	sd	s0,8(sp)
 166:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 168:	ca19                	beqz	a2,17e <memset+0x1c>
 16a:	87aa                	mv	a5,a0
 16c:	1602                	slli	a2,a2,0x20
 16e:	9201                	srli	a2,a2,0x20
 170:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 174:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 178:	0785                	addi	a5,a5,1
 17a:	fee79de3          	bne	a5,a4,174 <memset+0x12>
  }
  return dst;
}
 17e:	6422                	ld	s0,8(sp)
 180:	0141                	addi	sp,sp,16
 182:	8082                	ret

0000000000000184 <strchr>:

char*
strchr(const char *s, char c)
{
 184:	1141                	addi	sp,sp,-16
 186:	e422                	sd	s0,8(sp)
 188:	0800                	addi	s0,sp,16
  for(; *s; s++)
 18a:	00054783          	lbu	a5,0(a0)
 18e:	cb99                	beqz	a5,1a4 <strchr+0x20>
    if(*s == c)
 190:	00f58763          	beq	a1,a5,19e <strchr+0x1a>
  for(; *s; s++)
 194:	0505                	addi	a0,a0,1
 196:	00054783          	lbu	a5,0(a0)
 19a:	fbfd                	bnez	a5,190 <strchr+0xc>
      return (char*)s;
  return 0;
 19c:	4501                	li	a0,0
}
 19e:	6422                	ld	s0,8(sp)
 1a0:	0141                	addi	sp,sp,16
 1a2:	8082                	ret
  return 0;
 1a4:	4501                	li	a0,0
 1a6:	bfe5                	j	19e <strchr+0x1a>

00000000000001a8 <gets>:

char*
gets(char *buf, int max)
{
 1a8:	711d                	addi	sp,sp,-96
 1aa:	ec86                	sd	ra,88(sp)
 1ac:	e8a2                	sd	s0,80(sp)
 1ae:	e4a6                	sd	s1,72(sp)
 1b0:	e0ca                	sd	s2,64(sp)
 1b2:	fc4e                	sd	s3,56(sp)
 1b4:	f852                	sd	s4,48(sp)
 1b6:	f456                	sd	s5,40(sp)
 1b8:	f05a                	sd	s6,32(sp)
 1ba:	ec5e                	sd	s7,24(sp)
 1bc:	e862                	sd	s8,16(sp)
 1be:	1080                	addi	s0,sp,96
 1c0:	8baa                	mv	s7,a0
 1c2:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 1c4:	892a                	mv	s2,a0
 1c6:	4481                	li	s1,0
    cc = read(0, &c, 1);
 1c8:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 1cc:	4b29                	li	s6,10
 1ce:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 1d0:	89a6                	mv	s3,s1
 1d2:	2485                	addiw	s1,s1,1
 1d4:	0344d763          	bge	s1,s4,202 <gets+0x5a>
    cc = read(0, &c, 1);
 1d8:	4605                	li	a2,1
 1da:	85d6                	mv	a1,s5
 1dc:	4501                	li	a0,0
 1de:	00000097          	auipc	ra,0x0
 1e2:	1ba080e7          	jalr	442(ra) # 398 <read>
    if(cc < 1)
 1e6:	00a05e63          	blez	a0,202 <gets+0x5a>
    buf[i++] = c;
 1ea:	faf44783          	lbu	a5,-81(s0)
 1ee:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 1f2:	01678763          	beq	a5,s6,200 <gets+0x58>
 1f6:	0905                	addi	s2,s2,1
 1f8:	fd879ce3          	bne	a5,s8,1d0 <gets+0x28>
  for(i=0; i+1 < max; ){
 1fc:	89a6                	mv	s3,s1
 1fe:	a011                	j	202 <gets+0x5a>
 200:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 202:	99de                	add	s3,s3,s7
 204:	00098023          	sb	zero,0(s3)
  return buf;
}
 208:	855e                	mv	a0,s7
 20a:	60e6                	ld	ra,88(sp)
 20c:	6446                	ld	s0,80(sp)
 20e:	64a6                	ld	s1,72(sp)
 210:	6906                	ld	s2,64(sp)
 212:	79e2                	ld	s3,56(sp)
 214:	7a42                	ld	s4,48(sp)
 216:	7aa2                	ld	s5,40(sp)
 218:	7b02                	ld	s6,32(sp)
 21a:	6be2                	ld	s7,24(sp)
 21c:	6c42                	ld	s8,16(sp)
 21e:	6125                	addi	sp,sp,96
 220:	8082                	ret

0000000000000222 <stat>:

int
stat(const char *n, struct stat *st)
{
 222:	1101                	addi	sp,sp,-32
 224:	ec06                	sd	ra,24(sp)
 226:	e822                	sd	s0,16(sp)
 228:	e426                	sd	s1,8(sp)
 22a:	e04a                	sd	s2,0(sp)
 22c:	1000                	addi	s0,sp,32
 22e:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 230:	4581                	li	a1,0
 232:	00000097          	auipc	ra,0x0
 236:	196080e7          	jalr	406(ra) # 3c8 <open>
  if(fd < 0)
 23a:	02054563          	bltz	a0,264 <stat+0x42>
 23e:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 240:	85ca                	mv	a1,s2
 242:	00000097          	auipc	ra,0x0
 246:	190080e7          	jalr	400(ra) # 3d2 <fstat>
 24a:	892a                	mv	s2,a0
  close(fd);
 24c:	8526                	mv	a0,s1
 24e:	00000097          	auipc	ra,0x0
 252:	15e080e7          	jalr	350(ra) # 3ac <close>
  return r;
}
 256:	854a                	mv	a0,s2
 258:	60e2                	ld	ra,24(sp)
 25a:	6442                	ld	s0,16(sp)
 25c:	64a2                	ld	s1,8(sp)
 25e:	6902                	ld	s2,0(sp)
 260:	6105                	addi	sp,sp,32
 262:	8082                	ret
    return -1;
 264:	597d                	li	s2,-1
 266:	bfc5                	j	256 <stat+0x34>

0000000000000268 <atoi>:

int
atoi(const char *s)
{
 268:	1141                	addi	sp,sp,-16
 26a:	e422                	sd	s0,8(sp)
 26c:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 26e:	00054703          	lbu	a4,0(a0)
 272:	02d00793          	li	a5,45
  int neg = 1;
 276:	4585                	li	a1,1
  if (*s == '-') {
 278:	04f70363          	beq	a4,a5,2be <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 27c:	00054703          	lbu	a4,0(a0)
 280:	fd07079b          	addiw	a5,a4,-48
 284:	0ff7f793          	zext.b	a5,a5
 288:	46a5                	li	a3,9
 28a:	02f6ed63          	bltu	a3,a5,2c4 <atoi+0x5c>
  n = 0;
 28e:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 290:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 292:	0505                	addi	a0,a0,1
 294:	0026979b          	slliw	a5,a3,0x2
 298:	9fb5                	addw	a5,a5,a3
 29a:	0017979b          	slliw	a5,a5,0x1
 29e:	9fb9                	addw	a5,a5,a4
 2a0:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 2a4:	00054703          	lbu	a4,0(a0)
 2a8:	fd07079b          	addiw	a5,a4,-48
 2ac:	0ff7f793          	zext.b	a5,a5
 2b0:	fef671e3          	bgeu	a2,a5,292 <atoi+0x2a>
  return n * neg;
}
 2b4:	02d5853b          	mulw	a0,a1,a3
 2b8:	6422                	ld	s0,8(sp)
 2ba:	0141                	addi	sp,sp,16
 2bc:	8082                	ret
    s++;
 2be:	0505                	addi	a0,a0,1
    neg = -1;
 2c0:	55fd                	li	a1,-1
 2c2:	bf6d                	j	27c <atoi+0x14>
  n = 0;
 2c4:	4681                	li	a3,0
 2c6:	b7fd                	j	2b4 <atoi+0x4c>

00000000000002c8 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 2c8:	1141                	addi	sp,sp,-16
 2ca:	e422                	sd	s0,8(sp)
 2cc:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 2ce:	02b57463          	bgeu	a0,a1,2f6 <memmove+0x2e>
    while(n-- > 0)
 2d2:	00c05f63          	blez	a2,2f0 <memmove+0x28>
 2d6:	1602                	slli	a2,a2,0x20
 2d8:	9201                	srli	a2,a2,0x20
 2da:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 2de:	872a                	mv	a4,a0
      *dst++ = *src++;
 2e0:	0585                	addi	a1,a1,1
 2e2:	0705                	addi	a4,a4,1
 2e4:	fff5c683          	lbu	a3,-1(a1)
 2e8:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 2ec:	fee79ae3          	bne	a5,a4,2e0 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 2f0:	6422                	ld	s0,8(sp)
 2f2:	0141                	addi	sp,sp,16
 2f4:	8082                	ret
    dst += n;
 2f6:	00c50733          	add	a4,a0,a2
    src += n;
 2fa:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 2fc:	fec05ae3          	blez	a2,2f0 <memmove+0x28>
 300:	fff6079b          	addiw	a5,a2,-1
 304:	1782                	slli	a5,a5,0x20
 306:	9381                	srli	a5,a5,0x20
 308:	fff7c793          	not	a5,a5
 30c:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 30e:	15fd                	addi	a1,a1,-1
 310:	177d                	addi	a4,a4,-1
 312:	0005c683          	lbu	a3,0(a1)
 316:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 31a:	fee79ae3          	bne	a5,a4,30e <memmove+0x46>
 31e:	bfc9                	j	2f0 <memmove+0x28>

0000000000000320 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 320:	1141                	addi	sp,sp,-16
 322:	e422                	sd	s0,8(sp)
 324:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 326:	ca05                	beqz	a2,356 <memcmp+0x36>
 328:	fff6069b          	addiw	a3,a2,-1
 32c:	1682                	slli	a3,a3,0x20
 32e:	9281                	srli	a3,a3,0x20
 330:	0685                	addi	a3,a3,1
 332:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 334:	00054783          	lbu	a5,0(a0)
 338:	0005c703          	lbu	a4,0(a1)
 33c:	00e79863          	bne	a5,a4,34c <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 340:	0505                	addi	a0,a0,1
    p2++;
 342:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 344:	fed518e3          	bne	a0,a3,334 <memcmp+0x14>
  }
  return 0;
 348:	4501                	li	a0,0
 34a:	a019                	j	350 <memcmp+0x30>
      return *p1 - *p2;
 34c:	40e7853b          	subw	a0,a5,a4
}
 350:	6422                	ld	s0,8(sp)
 352:	0141                	addi	sp,sp,16
 354:	8082                	ret
  return 0;
 356:	4501                	li	a0,0
 358:	bfe5                	j	350 <memcmp+0x30>

000000000000035a <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 35a:	1141                	addi	sp,sp,-16
 35c:	e406                	sd	ra,8(sp)
 35e:	e022                	sd	s0,0(sp)
 360:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 362:	00000097          	auipc	ra,0x0
 366:	f66080e7          	jalr	-154(ra) # 2c8 <memmove>
}
 36a:	60a2                	ld	ra,8(sp)
 36c:	6402                	ld	s0,0(sp)
 36e:	0141                	addi	sp,sp,16
 370:	8082                	ret

0000000000000372 <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 372:	4885                	li	a7,1
 ecall
 374:	00000073          	ecall
 ret
 378:	8082                	ret

000000000000037a <exit>:
.global exit
exit:
 li a7, SYS_exit
 37a:	05d00893          	li	a7,93
 ecall
 37e:	00000073          	ecall
 ret
 382:	8082                	ret

0000000000000384 <wait>:
.global wait
wait:
 li a7, SYS_wait
 384:	4bf00893          	li	a7,1215
 ecall
 388:	00000073          	ecall
 ret
 38c:	8082                	ret

000000000000038e <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 38e:	03b00893          	li	a7,59
 ecall
 392:	00000073          	ecall
 ret
 396:	8082                	ret

0000000000000398 <read>:
.global read
read:
 li a7, SYS_read
 398:	03f00893          	li	a7,63
 ecall
 39c:	00000073          	ecall
 ret
 3a0:	8082                	ret

00000000000003a2 <write>:
.global write
write:
 li a7, SYS_write
 3a2:	04000893          	li	a7,64
 ecall
 3a6:	00000073          	ecall
 ret
 3aa:	8082                	ret

00000000000003ac <close>:
.global close
close:
 li a7, SYS_close
 3ac:	03900893          	li	a7,57
 ecall
 3b0:	00000073          	ecall
 ret
 3b4:	8082                	ret

00000000000003b6 <kill>:
.global kill
kill:
 li a7, SYS_kill
 3b6:	4899                	li	a7,6
 ecall
 3b8:	00000073          	ecall
 ret
 3bc:	8082                	ret

00000000000003be <exec>:
.global exec
exec:
 li a7, SYS_exec
 3be:	0dd00893          	li	a7,221
 ecall
 3c2:	00000073          	ecall
 ret
 3c6:	8082                	ret

00000000000003c8 <open>:
.global open
open:
 li a7, SYS_open
 3c8:	03800893          	li	a7,56
 ecall
 3cc:	00000073          	ecall
 ret
 3d0:	8082                	ret

00000000000003d2 <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 3d2:	05000893          	li	a7,80
 ecall
 3d6:	00000073          	ecall
 ret
 3da:	8082                	ret

00000000000003dc <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 3dc:	02200893          	li	a7,34
 ecall
 3e0:	00000073          	ecall
 ret
 3e4:	8082                	ret

00000000000003e6 <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 3e6:	03100893          	li	a7,49
 ecall
 3ea:	00000073          	ecall
 ret
 3ee:	8082                	ret

00000000000003f0 <dup>:
.global dup
dup:
 li a7, SYS_dup
 3f0:	48dd                	li	a7,23
 ecall
 3f2:	00000073          	ecall
 ret
 3f6:	8082                	ret

00000000000003f8 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 3f8:	0ac00893          	li	a7,172
 ecall
 3fc:	00000073          	ecall
 ret
 400:	8082                	ret

0000000000000402 <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 402:	0d600893          	li	a7,214
 ecall
 406:	00000073          	ecall
 ret
 40a:	8082                	ret

000000000000040c <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 40c:	48b5                	li	a7,13
 ecall
 40e:	00000073          	ecall
 ret
 412:	8082                	ret

0000000000000414 <times>:
.global times
times:
 li a7, SYS_times
 414:	09900893          	li	a7,153
 ecall
 418:	00000073          	ecall
 ret
 41c:	8082                	ret

000000000000041e <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 41e:	48d9                	li	a7,22
 ecall
 420:	00000073          	ecall
 ret
 424:	8082                	ret

0000000000000426 <dev>:
.global dev
dev:
 li a7, SYS_dev
 426:	6889                	lui	a7,0x2
 428:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x15cd>
 ecall
 42c:	00000073          	ecall
 ret
 430:	8082                	ret

0000000000000432 <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 432:	7d200893          	li	a7,2002
 ecall
 436:	00000073          	ecall
 ret
 43a:	8082                	ret

000000000000043c <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 43c:	48c5                	li	a7,17
 ecall
 43e:	00000073          	ecall
 ret
 442:	8082                	ret

0000000000000444 <remove>:
.global remove
remove:
 li a7, SYS_remove
 444:	48e5                	li	a7,25
 ecall
 446:	00000073          	ecall
 ret
 44a:	8082                	ret

000000000000044c <trace>:
.global trace
trace:
 li a7, SYS_trace
 44c:	48c9                	li	a7,18
 ecall
 44e:	00000073          	ecall
 ret
 452:	8082                	ret

0000000000000454 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 454:	48cd                	li	a7,19
 ecall
 456:	00000073          	ecall
 ret
 45a:	8082                	ret

000000000000045c <rename>:
.global rename
rename:
 li a7, SYS_rename
 45c:	48e9                	li	a7,26
 ecall
 45e:	00000073          	ecall
 ret
 462:	8082                	ret

0000000000000464 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 464:	1101                	addi	sp,sp,-32
 466:	ec06                	sd	ra,24(sp)
 468:	e822                	sd	s0,16(sp)
 46a:	1000                	addi	s0,sp,32
 46c:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 470:	4605                	li	a2,1
 472:	fef40593          	addi	a1,s0,-17
 476:	00000097          	auipc	ra,0x0
 47a:	f2c080e7          	jalr	-212(ra) # 3a2 <write>
}
 47e:	60e2                	ld	ra,24(sp)
 480:	6442                	ld	s0,16(sp)
 482:	6105                	addi	sp,sp,32
 484:	8082                	ret

0000000000000486 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 486:	7139                	addi	sp,sp,-64
 488:	fc06                	sd	ra,56(sp)
 48a:	f822                	sd	s0,48(sp)
 48c:	f426                	sd	s1,40(sp)
 48e:	f04a                	sd	s2,32(sp)
 490:	ec4e                	sd	s3,24(sp)
 492:	0080                	addi	s0,sp,64
 494:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 496:	c299                	beqz	a3,49c <printint+0x16>
 498:	0805c863          	bltz	a1,528 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 49c:	2581                	sext.w	a1,a1
  neg = 0;
 49e:	4881                	li	a7,0
  }

  i = 0;
 4a0:	fc040993          	addi	s3,s0,-64
  neg = 0;
 4a4:	86ce                	mv	a3,s3
  i = 0;
 4a6:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 4a8:	2601                	sext.w	a2,a2
 4aa:	00000517          	auipc	a0,0x0
 4ae:	4de50513          	addi	a0,a0,1246 # 988 <digits>
 4b2:	883a                	mv	a6,a4
 4b4:	2705                	addiw	a4,a4,1
 4b6:	02c5f7bb          	remuw	a5,a1,a2
 4ba:	1782                	slli	a5,a5,0x20
 4bc:	9381                	srli	a5,a5,0x20
 4be:	97aa                	add	a5,a5,a0
 4c0:	0007c783          	lbu	a5,0(a5)
 4c4:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 4c8:	0005879b          	sext.w	a5,a1
 4cc:	02c5d5bb          	divuw	a1,a1,a2
 4d0:	0685                	addi	a3,a3,1
 4d2:	fec7f0e3          	bgeu	a5,a2,4b2 <printint+0x2c>
  if(neg)
 4d6:	00088c63          	beqz	a7,4ee <printint+0x68>
    buf[i++] = '-';
 4da:	fd070793          	addi	a5,a4,-48
 4de:	00878733          	add	a4,a5,s0
 4e2:	02d00793          	li	a5,45
 4e6:	fef70823          	sb	a5,-16(a4)
 4ea:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 4ee:	02e05663          	blez	a4,51a <printint+0x94>
 4f2:	fc040913          	addi	s2,s0,-64
 4f6:	993a                	add	s2,s2,a4
 4f8:	19fd                	addi	s3,s3,-1
 4fa:	99ba                	add	s3,s3,a4
 4fc:	377d                	addiw	a4,a4,-1
 4fe:	1702                	slli	a4,a4,0x20
 500:	9301                	srli	a4,a4,0x20
 502:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 506:	fff94583          	lbu	a1,-1(s2)
 50a:	8526                	mv	a0,s1
 50c:	00000097          	auipc	ra,0x0
 510:	f58080e7          	jalr	-168(ra) # 464 <putc>
  while(--i >= 0)
 514:	197d                	addi	s2,s2,-1
 516:	ff3918e3          	bne	s2,s3,506 <printint+0x80>
}
 51a:	70e2                	ld	ra,56(sp)
 51c:	7442                	ld	s0,48(sp)
 51e:	74a2                	ld	s1,40(sp)
 520:	7902                	ld	s2,32(sp)
 522:	69e2                	ld	s3,24(sp)
 524:	6121                	addi	sp,sp,64
 526:	8082                	ret
    x = -xx;
 528:	40b005bb          	negw	a1,a1
    neg = 1;
 52c:	4885                	li	a7,1
    x = -xx;
 52e:	bf8d                	j	4a0 <printint+0x1a>

0000000000000530 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 530:	7119                	addi	sp,sp,-128
 532:	fc86                	sd	ra,120(sp)
 534:	f8a2                	sd	s0,112(sp)
 536:	f4a6                	sd	s1,104(sp)
 538:	f0ca                	sd	s2,96(sp)
 53a:	ecce                	sd	s3,88(sp)
 53c:	e8d2                	sd	s4,80(sp)
 53e:	e4d6                	sd	s5,72(sp)
 540:	e0da                	sd	s6,64(sp)
 542:	fc5e                	sd	s7,56(sp)
 544:	f862                	sd	s8,48(sp)
 546:	f466                	sd	s9,40(sp)
 548:	f06a                	sd	s10,32(sp)
 54a:	ec6e                	sd	s11,24(sp)
 54c:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 54e:	0005c903          	lbu	s2,0(a1)
 552:	18090f63          	beqz	s2,6f0 <vprintf+0x1c0>
 556:	8aaa                	mv	s5,a0
 558:	8b32                	mv	s6,a2
 55a:	00158493          	addi	s1,a1,1
  state = 0;
 55e:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 560:	02500a13          	li	s4,37
 564:	4c55                	li	s8,21
 566:	00000c97          	auipc	s9,0x0
 56a:	3cac8c93          	addi	s9,s9,970 # 930 <malloc+0x13c>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 56e:	02800d93          	li	s11,40
  putc(fd, 'x');
 572:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 574:	00000b97          	auipc	s7,0x0
 578:	414b8b93          	addi	s7,s7,1044 # 988 <digits>
 57c:	a839                	j	59a <vprintf+0x6a>
        putc(fd, c);
 57e:	85ca                	mv	a1,s2
 580:	8556                	mv	a0,s5
 582:	00000097          	auipc	ra,0x0
 586:	ee2080e7          	jalr	-286(ra) # 464 <putc>
 58a:	a019                	j	590 <vprintf+0x60>
    } else if(state == '%'){
 58c:	01498d63          	beq	s3,s4,5a6 <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 590:	0485                	addi	s1,s1,1
 592:	fff4c903          	lbu	s2,-1(s1)
 596:	14090d63          	beqz	s2,6f0 <vprintf+0x1c0>
    if(state == 0){
 59a:	fe0999e3          	bnez	s3,58c <vprintf+0x5c>
      if(c == '%'){
 59e:	ff4910e3          	bne	s2,s4,57e <vprintf+0x4e>
        state = '%';
 5a2:	89d2                	mv	s3,s4
 5a4:	b7f5                	j	590 <vprintf+0x60>
      if(c == 'd'){
 5a6:	11490c63          	beq	s2,s4,6be <vprintf+0x18e>
 5aa:	f9d9079b          	addiw	a5,s2,-99
 5ae:	0ff7f793          	zext.b	a5,a5
 5b2:	10fc6e63          	bltu	s8,a5,6ce <vprintf+0x19e>
 5b6:	f9d9079b          	addiw	a5,s2,-99
 5ba:	0ff7f713          	zext.b	a4,a5
 5be:	10ec6863          	bltu	s8,a4,6ce <vprintf+0x19e>
 5c2:	00271793          	slli	a5,a4,0x2
 5c6:	97e6                	add	a5,a5,s9
 5c8:	439c                	lw	a5,0(a5)
 5ca:	97e6                	add	a5,a5,s9
 5cc:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 5ce:	008b0913          	addi	s2,s6,8
 5d2:	4685                	li	a3,1
 5d4:	4629                	li	a2,10
 5d6:	000b2583          	lw	a1,0(s6)
 5da:	8556                	mv	a0,s5
 5dc:	00000097          	auipc	ra,0x0
 5e0:	eaa080e7          	jalr	-342(ra) # 486 <printint>
 5e4:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 5e6:	4981                	li	s3,0
 5e8:	b765                	j	590 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 5ea:	008b0913          	addi	s2,s6,8
 5ee:	4681                	li	a3,0
 5f0:	4629                	li	a2,10
 5f2:	000b2583          	lw	a1,0(s6)
 5f6:	8556                	mv	a0,s5
 5f8:	00000097          	auipc	ra,0x0
 5fc:	e8e080e7          	jalr	-370(ra) # 486 <printint>
 600:	8b4a                	mv	s6,s2
      state = 0;
 602:	4981                	li	s3,0
 604:	b771                	j	590 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 606:	008b0913          	addi	s2,s6,8
 60a:	4681                	li	a3,0
 60c:	866a                	mv	a2,s10
 60e:	000b2583          	lw	a1,0(s6)
 612:	8556                	mv	a0,s5
 614:	00000097          	auipc	ra,0x0
 618:	e72080e7          	jalr	-398(ra) # 486 <printint>
 61c:	8b4a                	mv	s6,s2
      state = 0;
 61e:	4981                	li	s3,0
 620:	bf85                	j	590 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 622:	008b0793          	addi	a5,s6,8
 626:	f8f43423          	sd	a5,-120(s0)
 62a:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 62e:	03000593          	li	a1,48
 632:	8556                	mv	a0,s5
 634:	00000097          	auipc	ra,0x0
 638:	e30080e7          	jalr	-464(ra) # 464 <putc>
  putc(fd, 'x');
 63c:	07800593          	li	a1,120
 640:	8556                	mv	a0,s5
 642:	00000097          	auipc	ra,0x0
 646:	e22080e7          	jalr	-478(ra) # 464 <putc>
 64a:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 64c:	03c9d793          	srli	a5,s3,0x3c
 650:	97de                	add	a5,a5,s7
 652:	0007c583          	lbu	a1,0(a5)
 656:	8556                	mv	a0,s5
 658:	00000097          	auipc	ra,0x0
 65c:	e0c080e7          	jalr	-500(ra) # 464 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 660:	0992                	slli	s3,s3,0x4
 662:	397d                	addiw	s2,s2,-1
 664:	fe0914e3          	bnez	s2,64c <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 668:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 66c:	4981                	li	s3,0
 66e:	b70d                	j	590 <vprintf+0x60>
        s = va_arg(ap, char*);
 670:	008b0913          	addi	s2,s6,8
 674:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 678:	02098163          	beqz	s3,69a <vprintf+0x16a>
        while(*s != 0){
 67c:	0009c583          	lbu	a1,0(s3)
 680:	c5ad                	beqz	a1,6ea <vprintf+0x1ba>
          putc(fd, *s);
 682:	8556                	mv	a0,s5
 684:	00000097          	auipc	ra,0x0
 688:	de0080e7          	jalr	-544(ra) # 464 <putc>
          s++;
 68c:	0985                	addi	s3,s3,1
        while(*s != 0){
 68e:	0009c583          	lbu	a1,0(s3)
 692:	f9e5                	bnez	a1,682 <vprintf+0x152>
        s = va_arg(ap, char*);
 694:	8b4a                	mv	s6,s2
      state = 0;
 696:	4981                	li	s3,0
 698:	bde5                	j	590 <vprintf+0x60>
          s = "(null)";
 69a:	00000997          	auipc	s3,0x0
 69e:	28e98993          	addi	s3,s3,654 # 928 <malloc+0x134>
        while(*s != 0){
 6a2:	85ee                	mv	a1,s11
 6a4:	bff9                	j	682 <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 6a6:	008b0913          	addi	s2,s6,8
 6aa:	000b4583          	lbu	a1,0(s6)
 6ae:	8556                	mv	a0,s5
 6b0:	00000097          	auipc	ra,0x0
 6b4:	db4080e7          	jalr	-588(ra) # 464 <putc>
 6b8:	8b4a                	mv	s6,s2
      state = 0;
 6ba:	4981                	li	s3,0
 6bc:	bdd1                	j	590 <vprintf+0x60>
        putc(fd, c);
 6be:	85d2                	mv	a1,s4
 6c0:	8556                	mv	a0,s5
 6c2:	00000097          	auipc	ra,0x0
 6c6:	da2080e7          	jalr	-606(ra) # 464 <putc>
      state = 0;
 6ca:	4981                	li	s3,0
 6cc:	b5d1                	j	590 <vprintf+0x60>
        putc(fd, '%');
 6ce:	85d2                	mv	a1,s4
 6d0:	8556                	mv	a0,s5
 6d2:	00000097          	auipc	ra,0x0
 6d6:	d92080e7          	jalr	-622(ra) # 464 <putc>
        putc(fd, c);
 6da:	85ca                	mv	a1,s2
 6dc:	8556                	mv	a0,s5
 6de:	00000097          	auipc	ra,0x0
 6e2:	d86080e7          	jalr	-634(ra) # 464 <putc>
      state = 0;
 6e6:	4981                	li	s3,0
 6e8:	b565                	j	590 <vprintf+0x60>
        s = va_arg(ap, char*);
 6ea:	8b4a                	mv	s6,s2
      state = 0;
 6ec:	4981                	li	s3,0
 6ee:	b54d                	j	590 <vprintf+0x60>
    }
  }
}
 6f0:	70e6                	ld	ra,120(sp)
 6f2:	7446                	ld	s0,112(sp)
 6f4:	74a6                	ld	s1,104(sp)
 6f6:	7906                	ld	s2,96(sp)
 6f8:	69e6                	ld	s3,88(sp)
 6fa:	6a46                	ld	s4,80(sp)
 6fc:	6aa6                	ld	s5,72(sp)
 6fe:	6b06                	ld	s6,64(sp)
 700:	7be2                	ld	s7,56(sp)
 702:	7c42                	ld	s8,48(sp)
 704:	7ca2                	ld	s9,40(sp)
 706:	7d02                	ld	s10,32(sp)
 708:	6de2                	ld	s11,24(sp)
 70a:	6109                	addi	sp,sp,128
 70c:	8082                	ret

000000000000070e <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 70e:	715d                	addi	sp,sp,-80
 710:	ec06                	sd	ra,24(sp)
 712:	e822                	sd	s0,16(sp)
 714:	1000                	addi	s0,sp,32
 716:	e010                	sd	a2,0(s0)
 718:	e414                	sd	a3,8(s0)
 71a:	e818                	sd	a4,16(s0)
 71c:	ec1c                	sd	a5,24(s0)
 71e:	03043023          	sd	a6,32(s0)
 722:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 726:	8622                	mv	a2,s0
 728:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 72c:	00000097          	auipc	ra,0x0
 730:	e04080e7          	jalr	-508(ra) # 530 <vprintf>
}
 734:	60e2                	ld	ra,24(sp)
 736:	6442                	ld	s0,16(sp)
 738:	6161                	addi	sp,sp,80
 73a:	8082                	ret

000000000000073c <printf>:

void
printf(const char *fmt, ...)
{
 73c:	711d                	addi	sp,sp,-96
 73e:	ec06                	sd	ra,24(sp)
 740:	e822                	sd	s0,16(sp)
 742:	1000                	addi	s0,sp,32
 744:	e40c                	sd	a1,8(s0)
 746:	e810                	sd	a2,16(s0)
 748:	ec14                	sd	a3,24(s0)
 74a:	f018                	sd	a4,32(s0)
 74c:	f41c                	sd	a5,40(s0)
 74e:	03043823          	sd	a6,48(s0)
 752:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 756:	00840613          	addi	a2,s0,8
 75a:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 75e:	85aa                	mv	a1,a0
 760:	4505                	li	a0,1
 762:	00000097          	auipc	ra,0x0
 766:	dce080e7          	jalr	-562(ra) # 530 <vprintf>
}
 76a:	60e2                	ld	ra,24(sp)
 76c:	6442                	ld	s0,16(sp)
 76e:	6125                	addi	sp,sp,96
 770:	8082                	ret

0000000000000772 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 772:	1141                	addi	sp,sp,-16
 774:	e422                	sd	s0,8(sp)
 776:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 778:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 77c:	00000797          	auipc	a5,0x0
 780:	2247b783          	ld	a5,548(a5) # 9a0 <freep>
 784:	a02d                	j	7ae <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 786:	4618                	lw	a4,8(a2)
 788:	9f2d                	addw	a4,a4,a1
 78a:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 78e:	6398                	ld	a4,0(a5)
 790:	6310                	ld	a2,0(a4)
 792:	a83d                	j	7d0 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 794:	ff852703          	lw	a4,-8(a0)
 798:	9f31                	addw	a4,a4,a2
 79a:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 79c:	ff053683          	ld	a3,-16(a0)
 7a0:	a091                	j	7e4 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7a2:	6398                	ld	a4,0(a5)
 7a4:	00e7e463          	bltu	a5,a4,7ac <free+0x3a>
 7a8:	00e6ea63          	bltu	a3,a4,7bc <free+0x4a>
{
 7ac:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 7ae:	fed7fae3          	bgeu	a5,a3,7a2 <free+0x30>
 7b2:	6398                	ld	a4,0(a5)
 7b4:	00e6e463          	bltu	a3,a4,7bc <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7b8:	fee7eae3          	bltu	a5,a4,7ac <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 7bc:	ff852583          	lw	a1,-8(a0)
 7c0:	6390                	ld	a2,0(a5)
 7c2:	02059813          	slli	a6,a1,0x20
 7c6:	01c85713          	srli	a4,a6,0x1c
 7ca:	9736                	add	a4,a4,a3
 7cc:	fae60de3          	beq	a2,a4,786 <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 7d0:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 7d4:	4790                	lw	a2,8(a5)
 7d6:	02061593          	slli	a1,a2,0x20
 7da:	01c5d713          	srli	a4,a1,0x1c
 7de:	973e                	add	a4,a4,a5
 7e0:	fae68ae3          	beq	a3,a4,794 <free+0x22>
    p->s.ptr = bp->s.ptr;
 7e4:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 7e6:	00000717          	auipc	a4,0x0
 7ea:	1af73d23          	sd	a5,442(a4) # 9a0 <freep>
}
 7ee:	6422                	ld	s0,8(sp)
 7f0:	0141                	addi	sp,sp,16
 7f2:	8082                	ret

00000000000007f4 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 7f4:	7139                	addi	sp,sp,-64
 7f6:	fc06                	sd	ra,56(sp)
 7f8:	f822                	sd	s0,48(sp)
 7fa:	f426                	sd	s1,40(sp)
 7fc:	f04a                	sd	s2,32(sp)
 7fe:	ec4e                	sd	s3,24(sp)
 800:	e852                	sd	s4,16(sp)
 802:	e456                	sd	s5,8(sp)
 804:	e05a                	sd	s6,0(sp)
 806:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 808:	02051493          	slli	s1,a0,0x20
 80c:	9081                	srli	s1,s1,0x20
 80e:	04bd                	addi	s1,s1,15
 810:	8091                	srli	s1,s1,0x4
 812:	00148a1b          	addiw	s4,s1,1
 816:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 818:	00000517          	auipc	a0,0x0
 81c:	18853503          	ld	a0,392(a0) # 9a0 <freep>
 820:	c515                	beqz	a0,84c <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 822:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 824:	4798                	lw	a4,8(a5)
 826:	04977163          	bgeu	a4,s1,868 <malloc+0x74>
 82a:	89d2                	mv	s3,s4
 82c:	000a071b          	sext.w	a4,s4
 830:	6685                	lui	a3,0x1
 832:	00d77363          	bgeu	a4,a3,838 <malloc+0x44>
 836:	6985                	lui	s3,0x1
 838:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 83c:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 840:	00000917          	auipc	s2,0x0
 844:	16090913          	addi	s2,s2,352 # 9a0 <freep>
  if(p == (char*)-1)
 848:	5afd                	li	s5,-1
 84a:	a8a5                	j	8c2 <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 84c:	00000797          	auipc	a5,0x0
 850:	15478793          	addi	a5,a5,340 # 9a0 <freep>
 854:	00000717          	auipc	a4,0x0
 858:	15470713          	addi	a4,a4,340 # 9a8 <base>
 85c:	e398                	sd	a4,0(a5)
 85e:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 860:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 864:	87ba                	mv	a5,a4
 866:	b7d1                	j	82a <malloc+0x36>
      if(p->s.size == nunits)
 868:	02e48c63          	beq	s1,a4,8a0 <malloc+0xac>
        p->s.size -= nunits;
 86c:	4147073b          	subw	a4,a4,s4
 870:	c798                	sw	a4,8(a5)
        p += p->s.size;
 872:	02071693          	slli	a3,a4,0x20
 876:	01c6d713          	srli	a4,a3,0x1c
 87a:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 87c:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 880:	00000717          	auipc	a4,0x0
 884:	12a73023          	sd	a0,288(a4) # 9a0 <freep>
      return (void*)(p + 1);
 888:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 88c:	70e2                	ld	ra,56(sp)
 88e:	7442                	ld	s0,48(sp)
 890:	74a2                	ld	s1,40(sp)
 892:	7902                	ld	s2,32(sp)
 894:	69e2                	ld	s3,24(sp)
 896:	6a42                	ld	s4,16(sp)
 898:	6aa2                	ld	s5,8(sp)
 89a:	6b02                	ld	s6,0(sp)
 89c:	6121                	addi	sp,sp,64
 89e:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 8a0:	6398                	ld	a4,0(a5)
 8a2:	e118                	sd	a4,0(a0)
 8a4:	bff1                	j	880 <malloc+0x8c>
  hp->s.size = nu;
 8a6:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 8aa:	0541                	addi	a0,a0,16
 8ac:	00000097          	auipc	ra,0x0
 8b0:	ec6080e7          	jalr	-314(ra) # 772 <free>
  return freep;
 8b4:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 8b8:	d971                	beqz	a0,88c <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 8ba:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 8bc:	4798                	lw	a4,8(a5)
 8be:	fa9775e3          	bgeu	a4,s1,868 <malloc+0x74>
    if(p == freep)
 8c2:	00093703          	ld	a4,0(s2)
 8c6:	853e                	mv	a0,a5
 8c8:	fef719e3          	bne	a4,a5,8ba <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 8cc:	854e                	mv	a0,s3
 8ce:	00000097          	auipc	ra,0x0
 8d2:	b34080e7          	jalr	-1228(ra) # 402 <sbrk>
  if(p == (char*)-1)
 8d6:	fd5518e3          	bne	a0,s5,8a6 <malloc+0xb2>
        return 0;
 8da:	4501                	li	a0,0
 8dc:	bf45                	j	88c <malloc+0x98>
