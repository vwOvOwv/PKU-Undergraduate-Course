
xv6-user/_cat:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <cat>:

char buf[512];

void
cat(int fd)
{
   0:	7179                	addi	sp,sp,-48
   2:	f406                	sd	ra,40(sp)
   4:	f022                	sd	s0,32(sp)
   6:	ec26                	sd	s1,24(sp)
   8:	e84a                	sd	s2,16(sp)
   a:	e44e                	sd	s3,8(sp)
   c:	1800                	addi	s0,sp,48
   e:	89aa                	mv	s3,a0
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
  10:	00001917          	auipc	s2,0x1
  14:	9f090913          	addi	s2,s2,-1552 # a00 <buf>
  18:	20000613          	li	a2,512
  1c:	85ca                	mv	a1,s2
  1e:	854e                	mv	a0,s3
  20:	00000097          	auipc	ra,0x0
  24:	3d4080e7          	jalr	980(ra) # 3f4 <read>
  28:	84aa                	mv	s1,a0
  2a:	02a05963          	blez	a0,5c <cat+0x5c>
    if (write(1, buf, n) != n) {
  2e:	8626                	mv	a2,s1
  30:	85ca                	mv	a1,s2
  32:	4505                	li	a0,1
  34:	00000097          	auipc	ra,0x0
  38:	3ca080e7          	jalr	970(ra) # 3fe <write>
  3c:	fc950ee3          	beq	a0,s1,18 <cat+0x18>
      fprintf(2, "cat: write error\n");
  40:	00001597          	auipc	a1,0x1
  44:	90058593          	addi	a1,a1,-1792 # 940 <malloc+0xf0>
  48:	4509                	li	a0,2
  4a:	00000097          	auipc	ra,0x0
  4e:	720080e7          	jalr	1824(ra) # 76a <fprintf>
      exit(1);
  52:	4505                	li	a0,1
  54:	00000097          	auipc	ra,0x0
  58:	382080e7          	jalr	898(ra) # 3d6 <exit>
    }
  }
  if(n < 0){
  5c:	00054963          	bltz	a0,6e <cat+0x6e>
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}
  60:	70a2                	ld	ra,40(sp)
  62:	7402                	ld	s0,32(sp)
  64:	64e2                	ld	s1,24(sp)
  66:	6942                	ld	s2,16(sp)
  68:	69a2                	ld	s3,8(sp)
  6a:	6145                	addi	sp,sp,48
  6c:	8082                	ret
    fprintf(2, "cat: read error\n");
  6e:	00001597          	auipc	a1,0x1
  72:	8ea58593          	addi	a1,a1,-1814 # 958 <malloc+0x108>
  76:	4509                	li	a0,2
  78:	00000097          	auipc	ra,0x0
  7c:	6f2080e7          	jalr	1778(ra) # 76a <fprintf>
    exit(1);
  80:	4505                	li	a0,1
  82:	00000097          	auipc	ra,0x0
  86:	354080e7          	jalr	852(ra) # 3d6 <exit>

000000000000008a <main>:

int
main(int argc, char *argv[])
{
  8a:	7179                	addi	sp,sp,-48
  8c:	f406                	sd	ra,40(sp)
  8e:	f022                	sd	s0,32(sp)
  90:	ec26                	sd	s1,24(sp)
  92:	e84a                	sd	s2,16(sp)
  94:	e44e                	sd	s3,8(sp)
  96:	e052                	sd	s4,0(sp)
  98:	1800                	addi	s0,sp,48
  int fd, i;

  if(argc <= 1){
  9a:	4785                	li	a5,1
  9c:	04a7d763          	bge	a5,a0,ea <main+0x60>
  a0:	00858913          	addi	s2,a1,8
  a4:	ffe5099b          	addiw	s3,a0,-2
  a8:	02099793          	slli	a5,s3,0x20
  ac:	01d7d993          	srli	s3,a5,0x1d
  b0:	05c1                	addi	a1,a1,16
  b2:	99ae                	add	s3,s3,a1
    cat(0);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
  b4:	4581                	li	a1,0
  b6:	00093503          	ld	a0,0(s2)
  ba:	00000097          	auipc	ra,0x0
  be:	36a080e7          	jalr	874(ra) # 424 <open>
  c2:	84aa                	mv	s1,a0
  c4:	02054d63          	bltz	a0,fe <main+0x74>
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    cat(fd);
  c8:	00000097          	auipc	ra,0x0
  cc:	f38080e7          	jalr	-200(ra) # 0 <cat>
    close(fd);
  d0:	8526                	mv	a0,s1
  d2:	00000097          	auipc	ra,0x0
  d6:	336080e7          	jalr	822(ra) # 408 <close>
  for(i = 1; i < argc; i++){
  da:	0921                	addi	s2,s2,8
  dc:	fd391ce3          	bne	s2,s3,b4 <main+0x2a>
  }
  exit(0);
  e0:	4501                	li	a0,0
  e2:	00000097          	auipc	ra,0x0
  e6:	2f4080e7          	jalr	756(ra) # 3d6 <exit>
    cat(0);
  ea:	4501                	li	a0,0
  ec:	00000097          	auipc	ra,0x0
  f0:	f14080e7          	jalr	-236(ra) # 0 <cat>
    exit(0);
  f4:	4501                	li	a0,0
  f6:	00000097          	auipc	ra,0x0
  fa:	2e0080e7          	jalr	736(ra) # 3d6 <exit>
      fprintf(2, "cat: cannot open %s\n", argv[i]);
  fe:	00093603          	ld	a2,0(s2)
 102:	00001597          	auipc	a1,0x1
 106:	86e58593          	addi	a1,a1,-1938 # 970 <malloc+0x120>
 10a:	4509                	li	a0,2
 10c:	00000097          	auipc	ra,0x0
 110:	65e080e7          	jalr	1630(ra) # 76a <fprintf>
      exit(1);
 114:	4505                	li	a0,1
 116:	00000097          	auipc	ra,0x0
 11a:	2c0080e7          	jalr	704(ra) # 3d6 <exit>

000000000000011e <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
 11e:	1141                	addi	sp,sp,-16
 120:	e422                	sd	s0,8(sp)
 122:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 124:	87aa                	mv	a5,a0
 126:	0585                	addi	a1,a1,1
 128:	0785                	addi	a5,a5,1
 12a:	fff5c703          	lbu	a4,-1(a1)
 12e:	fee78fa3          	sb	a4,-1(a5)
 132:	fb75                	bnez	a4,126 <strcpy+0x8>
    ;
  return os;
}
 134:	6422                	ld	s0,8(sp)
 136:	0141                	addi	sp,sp,16
 138:	8082                	ret

000000000000013a <strcat>:

char*
strcat(char *s, const char *t)
{
 13a:	1141                	addi	sp,sp,-16
 13c:	e422                	sd	s0,8(sp)
 13e:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 140:	00054783          	lbu	a5,0(a0)
 144:	c385                	beqz	a5,164 <strcat+0x2a>
 146:	87aa                	mv	a5,a0
    s++;
 148:	0785                	addi	a5,a5,1
  while(*s)
 14a:	0007c703          	lbu	a4,0(a5)
 14e:	ff6d                	bnez	a4,148 <strcat+0xe>
  while((*s++ = *t++))
 150:	0585                	addi	a1,a1,1
 152:	0785                	addi	a5,a5,1
 154:	fff5c703          	lbu	a4,-1(a1)
 158:	fee78fa3          	sb	a4,-1(a5)
 15c:	fb75                	bnez	a4,150 <strcat+0x16>
    ;
  return os;
}
 15e:	6422                	ld	s0,8(sp)
 160:	0141                	addi	sp,sp,16
 162:	8082                	ret
  while(*s)
 164:	87aa                	mv	a5,a0
 166:	b7ed                	j	150 <strcat+0x16>

0000000000000168 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 168:	1141                	addi	sp,sp,-16
 16a:	e422                	sd	s0,8(sp)
 16c:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 16e:	00054783          	lbu	a5,0(a0)
 172:	cb91                	beqz	a5,186 <strcmp+0x1e>
 174:	0005c703          	lbu	a4,0(a1)
 178:	00f71763          	bne	a4,a5,186 <strcmp+0x1e>
    p++, q++;
 17c:	0505                	addi	a0,a0,1
 17e:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 180:	00054783          	lbu	a5,0(a0)
 184:	fbe5                	bnez	a5,174 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 186:	0005c503          	lbu	a0,0(a1)
}
 18a:	40a7853b          	subw	a0,a5,a0
 18e:	6422                	ld	s0,8(sp)
 190:	0141                	addi	sp,sp,16
 192:	8082                	ret

0000000000000194 <strlen>:

uint
strlen(const char *s)
{
 194:	1141                	addi	sp,sp,-16
 196:	e422                	sd	s0,8(sp)
 198:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 19a:	00054783          	lbu	a5,0(a0)
 19e:	cf91                	beqz	a5,1ba <strlen+0x26>
 1a0:	0505                	addi	a0,a0,1
 1a2:	87aa                	mv	a5,a0
 1a4:	4685                	li	a3,1
 1a6:	9e89                	subw	a3,a3,a0
 1a8:	00f6853b          	addw	a0,a3,a5
 1ac:	0785                	addi	a5,a5,1
 1ae:	fff7c703          	lbu	a4,-1(a5)
 1b2:	fb7d                	bnez	a4,1a8 <strlen+0x14>
    ;
  return n;
}
 1b4:	6422                	ld	s0,8(sp)
 1b6:	0141                	addi	sp,sp,16
 1b8:	8082                	ret
  for(n = 0; s[n]; n++)
 1ba:	4501                	li	a0,0
 1bc:	bfe5                	j	1b4 <strlen+0x20>

00000000000001be <memset>:

void*
memset(void *dst, int c, uint n)
{
 1be:	1141                	addi	sp,sp,-16
 1c0:	e422                	sd	s0,8(sp)
 1c2:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 1c4:	ca19                	beqz	a2,1da <memset+0x1c>
 1c6:	87aa                	mv	a5,a0
 1c8:	1602                	slli	a2,a2,0x20
 1ca:	9201                	srli	a2,a2,0x20
 1cc:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 1d0:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 1d4:	0785                	addi	a5,a5,1
 1d6:	fee79de3          	bne	a5,a4,1d0 <memset+0x12>
  }
  return dst;
}
 1da:	6422                	ld	s0,8(sp)
 1dc:	0141                	addi	sp,sp,16
 1de:	8082                	ret

00000000000001e0 <strchr>:

char*
strchr(const char *s, char c)
{
 1e0:	1141                	addi	sp,sp,-16
 1e2:	e422                	sd	s0,8(sp)
 1e4:	0800                	addi	s0,sp,16
  for(; *s; s++)
 1e6:	00054783          	lbu	a5,0(a0)
 1ea:	cb99                	beqz	a5,200 <strchr+0x20>
    if(*s == c)
 1ec:	00f58763          	beq	a1,a5,1fa <strchr+0x1a>
  for(; *s; s++)
 1f0:	0505                	addi	a0,a0,1
 1f2:	00054783          	lbu	a5,0(a0)
 1f6:	fbfd                	bnez	a5,1ec <strchr+0xc>
      return (char*)s;
  return 0;
 1f8:	4501                	li	a0,0
}
 1fa:	6422                	ld	s0,8(sp)
 1fc:	0141                	addi	sp,sp,16
 1fe:	8082                	ret
  return 0;
 200:	4501                	li	a0,0
 202:	bfe5                	j	1fa <strchr+0x1a>

0000000000000204 <gets>:

char*
gets(char *buf, int max)
{
 204:	711d                	addi	sp,sp,-96
 206:	ec86                	sd	ra,88(sp)
 208:	e8a2                	sd	s0,80(sp)
 20a:	e4a6                	sd	s1,72(sp)
 20c:	e0ca                	sd	s2,64(sp)
 20e:	fc4e                	sd	s3,56(sp)
 210:	f852                	sd	s4,48(sp)
 212:	f456                	sd	s5,40(sp)
 214:	f05a                	sd	s6,32(sp)
 216:	ec5e                	sd	s7,24(sp)
 218:	e862                	sd	s8,16(sp)
 21a:	1080                	addi	s0,sp,96
 21c:	8baa                	mv	s7,a0
 21e:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 220:	892a                	mv	s2,a0
 222:	4481                	li	s1,0
    cc = read(0, &c, 1);
 224:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 228:	4b29                	li	s6,10
 22a:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 22c:	89a6                	mv	s3,s1
 22e:	2485                	addiw	s1,s1,1
 230:	0344d763          	bge	s1,s4,25e <gets+0x5a>
    cc = read(0, &c, 1);
 234:	4605                	li	a2,1
 236:	85d6                	mv	a1,s5
 238:	4501                	li	a0,0
 23a:	00000097          	auipc	ra,0x0
 23e:	1ba080e7          	jalr	442(ra) # 3f4 <read>
    if(cc < 1)
 242:	00a05e63          	blez	a0,25e <gets+0x5a>
    buf[i++] = c;
 246:	faf44783          	lbu	a5,-81(s0)
 24a:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 24e:	01678763          	beq	a5,s6,25c <gets+0x58>
 252:	0905                	addi	s2,s2,1
 254:	fd879ce3          	bne	a5,s8,22c <gets+0x28>
  for(i=0; i+1 < max; ){
 258:	89a6                	mv	s3,s1
 25a:	a011                	j	25e <gets+0x5a>
 25c:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 25e:	99de                	add	s3,s3,s7
 260:	00098023          	sb	zero,0(s3)
  return buf;
}
 264:	855e                	mv	a0,s7
 266:	60e6                	ld	ra,88(sp)
 268:	6446                	ld	s0,80(sp)
 26a:	64a6                	ld	s1,72(sp)
 26c:	6906                	ld	s2,64(sp)
 26e:	79e2                	ld	s3,56(sp)
 270:	7a42                	ld	s4,48(sp)
 272:	7aa2                	ld	s5,40(sp)
 274:	7b02                	ld	s6,32(sp)
 276:	6be2                	ld	s7,24(sp)
 278:	6c42                	ld	s8,16(sp)
 27a:	6125                	addi	sp,sp,96
 27c:	8082                	ret

000000000000027e <stat>:

int
stat(const char *n, struct stat *st)
{
 27e:	1101                	addi	sp,sp,-32
 280:	ec06                	sd	ra,24(sp)
 282:	e822                	sd	s0,16(sp)
 284:	e426                	sd	s1,8(sp)
 286:	e04a                	sd	s2,0(sp)
 288:	1000                	addi	s0,sp,32
 28a:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 28c:	4581                	li	a1,0
 28e:	00000097          	auipc	ra,0x0
 292:	196080e7          	jalr	406(ra) # 424 <open>
  if(fd < 0)
 296:	02054563          	bltz	a0,2c0 <stat+0x42>
 29a:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 29c:	85ca                	mv	a1,s2
 29e:	00000097          	auipc	ra,0x0
 2a2:	190080e7          	jalr	400(ra) # 42e <fstat>
 2a6:	892a                	mv	s2,a0
  close(fd);
 2a8:	8526                	mv	a0,s1
 2aa:	00000097          	auipc	ra,0x0
 2ae:	15e080e7          	jalr	350(ra) # 408 <close>
  return r;
}
 2b2:	854a                	mv	a0,s2
 2b4:	60e2                	ld	ra,24(sp)
 2b6:	6442                	ld	s0,16(sp)
 2b8:	64a2                	ld	s1,8(sp)
 2ba:	6902                	ld	s2,0(sp)
 2bc:	6105                	addi	sp,sp,32
 2be:	8082                	ret
    return -1;
 2c0:	597d                	li	s2,-1
 2c2:	bfc5                	j	2b2 <stat+0x34>

00000000000002c4 <atoi>:

int
atoi(const char *s)
{
 2c4:	1141                	addi	sp,sp,-16
 2c6:	e422                	sd	s0,8(sp)
 2c8:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 2ca:	00054703          	lbu	a4,0(a0)
 2ce:	02d00793          	li	a5,45
  int neg = 1;
 2d2:	4585                	li	a1,1
  if (*s == '-') {
 2d4:	04f70363          	beq	a4,a5,31a <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 2d8:	00054703          	lbu	a4,0(a0)
 2dc:	fd07079b          	addiw	a5,a4,-48
 2e0:	0ff7f793          	zext.b	a5,a5
 2e4:	46a5                	li	a3,9
 2e6:	02f6ed63          	bltu	a3,a5,320 <atoi+0x5c>
  n = 0;
 2ea:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 2ec:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 2ee:	0505                	addi	a0,a0,1
 2f0:	0026979b          	slliw	a5,a3,0x2
 2f4:	9fb5                	addw	a5,a5,a3
 2f6:	0017979b          	slliw	a5,a5,0x1
 2fa:	9fb9                	addw	a5,a5,a4
 2fc:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 300:	00054703          	lbu	a4,0(a0)
 304:	fd07079b          	addiw	a5,a4,-48
 308:	0ff7f793          	zext.b	a5,a5
 30c:	fef671e3          	bgeu	a2,a5,2ee <atoi+0x2a>
  return n * neg;
}
 310:	02d5853b          	mulw	a0,a1,a3
 314:	6422                	ld	s0,8(sp)
 316:	0141                	addi	sp,sp,16
 318:	8082                	ret
    s++;
 31a:	0505                	addi	a0,a0,1
    neg = -1;
 31c:	55fd                	li	a1,-1
 31e:	bf6d                	j	2d8 <atoi+0x14>
  n = 0;
 320:	4681                	li	a3,0
 322:	b7fd                	j	310 <atoi+0x4c>

0000000000000324 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 324:	1141                	addi	sp,sp,-16
 326:	e422                	sd	s0,8(sp)
 328:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 32a:	02b57463          	bgeu	a0,a1,352 <memmove+0x2e>
    while(n-- > 0)
 32e:	00c05f63          	blez	a2,34c <memmove+0x28>
 332:	1602                	slli	a2,a2,0x20
 334:	9201                	srli	a2,a2,0x20
 336:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 33a:	872a                	mv	a4,a0
      *dst++ = *src++;
 33c:	0585                	addi	a1,a1,1
 33e:	0705                	addi	a4,a4,1
 340:	fff5c683          	lbu	a3,-1(a1)
 344:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 348:	fee79ae3          	bne	a5,a4,33c <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 34c:	6422                	ld	s0,8(sp)
 34e:	0141                	addi	sp,sp,16
 350:	8082                	ret
    dst += n;
 352:	00c50733          	add	a4,a0,a2
    src += n;
 356:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 358:	fec05ae3          	blez	a2,34c <memmove+0x28>
 35c:	fff6079b          	addiw	a5,a2,-1
 360:	1782                	slli	a5,a5,0x20
 362:	9381                	srli	a5,a5,0x20
 364:	fff7c793          	not	a5,a5
 368:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 36a:	15fd                	addi	a1,a1,-1
 36c:	177d                	addi	a4,a4,-1
 36e:	0005c683          	lbu	a3,0(a1)
 372:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 376:	fee79ae3          	bne	a5,a4,36a <memmove+0x46>
 37a:	bfc9                	j	34c <memmove+0x28>

000000000000037c <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 37c:	1141                	addi	sp,sp,-16
 37e:	e422                	sd	s0,8(sp)
 380:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 382:	ca05                	beqz	a2,3b2 <memcmp+0x36>
 384:	fff6069b          	addiw	a3,a2,-1
 388:	1682                	slli	a3,a3,0x20
 38a:	9281                	srli	a3,a3,0x20
 38c:	0685                	addi	a3,a3,1
 38e:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 390:	00054783          	lbu	a5,0(a0)
 394:	0005c703          	lbu	a4,0(a1)
 398:	00e79863          	bne	a5,a4,3a8 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 39c:	0505                	addi	a0,a0,1
    p2++;
 39e:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 3a0:	fed518e3          	bne	a0,a3,390 <memcmp+0x14>
  }
  return 0;
 3a4:	4501                	li	a0,0
 3a6:	a019                	j	3ac <memcmp+0x30>
      return *p1 - *p2;
 3a8:	40e7853b          	subw	a0,a5,a4
}
 3ac:	6422                	ld	s0,8(sp)
 3ae:	0141                	addi	sp,sp,16
 3b0:	8082                	ret
  return 0;
 3b2:	4501                	li	a0,0
 3b4:	bfe5                	j	3ac <memcmp+0x30>

00000000000003b6 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 3b6:	1141                	addi	sp,sp,-16
 3b8:	e406                	sd	ra,8(sp)
 3ba:	e022                	sd	s0,0(sp)
 3bc:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 3be:	00000097          	auipc	ra,0x0
 3c2:	f66080e7          	jalr	-154(ra) # 324 <memmove>
}
 3c6:	60a2                	ld	ra,8(sp)
 3c8:	6402                	ld	s0,0(sp)
 3ca:	0141                	addi	sp,sp,16
 3cc:	8082                	ret

00000000000003ce <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 3ce:	4885                	li	a7,1
 ecall
 3d0:	00000073          	ecall
 ret
 3d4:	8082                	ret

00000000000003d6 <exit>:
.global exit
exit:
 li a7, SYS_exit
 3d6:	05d00893          	li	a7,93
 ecall
 3da:	00000073          	ecall
 ret
 3de:	8082                	ret

00000000000003e0 <wait>:
.global wait
wait:
 li a7, SYS_wait
 3e0:	4bf00893          	li	a7,1215
 ecall
 3e4:	00000073          	ecall
 ret
 3e8:	8082                	ret

00000000000003ea <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 3ea:	03b00893          	li	a7,59
 ecall
 3ee:	00000073          	ecall
 ret
 3f2:	8082                	ret

00000000000003f4 <read>:
.global read
read:
 li a7, SYS_read
 3f4:	03f00893          	li	a7,63
 ecall
 3f8:	00000073          	ecall
 ret
 3fc:	8082                	ret

00000000000003fe <write>:
.global write
write:
 li a7, SYS_write
 3fe:	04000893          	li	a7,64
 ecall
 402:	00000073          	ecall
 ret
 406:	8082                	ret

0000000000000408 <close>:
.global close
close:
 li a7, SYS_close
 408:	03900893          	li	a7,57
 ecall
 40c:	00000073          	ecall
 ret
 410:	8082                	ret

0000000000000412 <kill>:
.global kill
kill:
 li a7, SYS_kill
 412:	4899                	li	a7,6
 ecall
 414:	00000073          	ecall
 ret
 418:	8082                	ret

000000000000041a <exec>:
.global exec
exec:
 li a7, SYS_exec
 41a:	0dd00893          	li	a7,221
 ecall
 41e:	00000073          	ecall
 ret
 422:	8082                	ret

0000000000000424 <open>:
.global open
open:
 li a7, SYS_open
 424:	03800893          	li	a7,56
 ecall
 428:	00000073          	ecall
 ret
 42c:	8082                	ret

000000000000042e <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 42e:	05000893          	li	a7,80
 ecall
 432:	00000073          	ecall
 ret
 436:	8082                	ret

0000000000000438 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 438:	02200893          	li	a7,34
 ecall
 43c:	00000073          	ecall
 ret
 440:	8082                	ret

0000000000000442 <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 442:	03100893          	li	a7,49
 ecall
 446:	00000073          	ecall
 ret
 44a:	8082                	ret

000000000000044c <dup>:
.global dup
dup:
 li a7, SYS_dup
 44c:	48dd                	li	a7,23
 ecall
 44e:	00000073          	ecall
 ret
 452:	8082                	ret

0000000000000454 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 454:	0ac00893          	li	a7,172
 ecall
 458:	00000073          	ecall
 ret
 45c:	8082                	ret

000000000000045e <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 45e:	0d600893          	li	a7,214
 ecall
 462:	00000073          	ecall
 ret
 466:	8082                	ret

0000000000000468 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 468:	48b5                	li	a7,13
 ecall
 46a:	00000073          	ecall
 ret
 46e:	8082                	ret

0000000000000470 <times>:
.global times
times:
 li a7, SYS_times
 470:	09900893          	li	a7,153
 ecall
 474:	00000073          	ecall
 ret
 478:	8082                	ret

000000000000047a <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 47a:	48d9                	li	a7,22
 ecall
 47c:	00000073          	ecall
 ret
 480:	8082                	ret

0000000000000482 <dev>:
.global dev
dev:
 li a7, SYS_dev
 482:	6889                	lui	a7,0x2
 484:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x156d>
 ecall
 488:	00000073          	ecall
 ret
 48c:	8082                	ret

000000000000048e <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 48e:	7d200893          	li	a7,2002
 ecall
 492:	00000073          	ecall
 ret
 496:	8082                	ret

0000000000000498 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 498:	48c5                	li	a7,17
 ecall
 49a:	00000073          	ecall
 ret
 49e:	8082                	ret

00000000000004a0 <remove>:
.global remove
remove:
 li a7, SYS_remove
 4a0:	48e5                	li	a7,25
 ecall
 4a2:	00000073          	ecall
 ret
 4a6:	8082                	ret

00000000000004a8 <trace>:
.global trace
trace:
 li a7, SYS_trace
 4a8:	48c9                	li	a7,18
 ecall
 4aa:	00000073          	ecall
 ret
 4ae:	8082                	ret

00000000000004b0 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 4b0:	48cd                	li	a7,19
 ecall
 4b2:	00000073          	ecall
 ret
 4b6:	8082                	ret

00000000000004b8 <rename>:
.global rename
rename:
 li a7, SYS_rename
 4b8:	48e9                	li	a7,26
 ecall
 4ba:	00000073          	ecall
 ret
 4be:	8082                	ret

00000000000004c0 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 4c0:	1101                	addi	sp,sp,-32
 4c2:	ec06                	sd	ra,24(sp)
 4c4:	e822                	sd	s0,16(sp)
 4c6:	1000                	addi	s0,sp,32
 4c8:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 4cc:	4605                	li	a2,1
 4ce:	fef40593          	addi	a1,s0,-17
 4d2:	00000097          	auipc	ra,0x0
 4d6:	f2c080e7          	jalr	-212(ra) # 3fe <write>
}
 4da:	60e2                	ld	ra,24(sp)
 4dc:	6442                	ld	s0,16(sp)
 4de:	6105                	addi	sp,sp,32
 4e0:	8082                	ret

00000000000004e2 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 4e2:	7139                	addi	sp,sp,-64
 4e4:	fc06                	sd	ra,56(sp)
 4e6:	f822                	sd	s0,48(sp)
 4e8:	f426                	sd	s1,40(sp)
 4ea:	f04a                	sd	s2,32(sp)
 4ec:	ec4e                	sd	s3,24(sp)
 4ee:	0080                	addi	s0,sp,64
 4f0:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 4f2:	c299                	beqz	a3,4f8 <printint+0x16>
 4f4:	0805c863          	bltz	a1,584 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 4f8:	2581                	sext.w	a1,a1
  neg = 0;
 4fa:	4881                	li	a7,0
  }

  i = 0;
 4fc:	fc040993          	addi	s3,s0,-64
  neg = 0;
 500:	86ce                	mv	a3,s3
  i = 0;
 502:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 504:	2601                	sext.w	a2,a2
 506:	00000517          	auipc	a0,0x0
 50a:	4e250513          	addi	a0,a0,1250 # 9e8 <digits>
 50e:	883a                	mv	a6,a4
 510:	2705                	addiw	a4,a4,1
 512:	02c5f7bb          	remuw	a5,a1,a2
 516:	1782                	slli	a5,a5,0x20
 518:	9381                	srli	a5,a5,0x20
 51a:	97aa                	add	a5,a5,a0
 51c:	0007c783          	lbu	a5,0(a5)
 520:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 524:	0005879b          	sext.w	a5,a1
 528:	02c5d5bb          	divuw	a1,a1,a2
 52c:	0685                	addi	a3,a3,1
 52e:	fec7f0e3          	bgeu	a5,a2,50e <printint+0x2c>
  if(neg)
 532:	00088c63          	beqz	a7,54a <printint+0x68>
    buf[i++] = '-';
 536:	fd070793          	addi	a5,a4,-48
 53a:	00878733          	add	a4,a5,s0
 53e:	02d00793          	li	a5,45
 542:	fef70823          	sb	a5,-16(a4)
 546:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 54a:	02e05663          	blez	a4,576 <printint+0x94>
 54e:	fc040913          	addi	s2,s0,-64
 552:	993a                	add	s2,s2,a4
 554:	19fd                	addi	s3,s3,-1
 556:	99ba                	add	s3,s3,a4
 558:	377d                	addiw	a4,a4,-1
 55a:	1702                	slli	a4,a4,0x20
 55c:	9301                	srli	a4,a4,0x20
 55e:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 562:	fff94583          	lbu	a1,-1(s2)
 566:	8526                	mv	a0,s1
 568:	00000097          	auipc	ra,0x0
 56c:	f58080e7          	jalr	-168(ra) # 4c0 <putc>
  while(--i >= 0)
 570:	197d                	addi	s2,s2,-1
 572:	ff3918e3          	bne	s2,s3,562 <printint+0x80>
}
 576:	70e2                	ld	ra,56(sp)
 578:	7442                	ld	s0,48(sp)
 57a:	74a2                	ld	s1,40(sp)
 57c:	7902                	ld	s2,32(sp)
 57e:	69e2                	ld	s3,24(sp)
 580:	6121                	addi	sp,sp,64
 582:	8082                	ret
    x = -xx;
 584:	40b005bb          	negw	a1,a1
    neg = 1;
 588:	4885                	li	a7,1
    x = -xx;
 58a:	bf8d                	j	4fc <printint+0x1a>

000000000000058c <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 58c:	7119                	addi	sp,sp,-128
 58e:	fc86                	sd	ra,120(sp)
 590:	f8a2                	sd	s0,112(sp)
 592:	f4a6                	sd	s1,104(sp)
 594:	f0ca                	sd	s2,96(sp)
 596:	ecce                	sd	s3,88(sp)
 598:	e8d2                	sd	s4,80(sp)
 59a:	e4d6                	sd	s5,72(sp)
 59c:	e0da                	sd	s6,64(sp)
 59e:	fc5e                	sd	s7,56(sp)
 5a0:	f862                	sd	s8,48(sp)
 5a2:	f466                	sd	s9,40(sp)
 5a4:	f06a                	sd	s10,32(sp)
 5a6:	ec6e                	sd	s11,24(sp)
 5a8:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 5aa:	0005c903          	lbu	s2,0(a1)
 5ae:	18090f63          	beqz	s2,74c <vprintf+0x1c0>
 5b2:	8aaa                	mv	s5,a0
 5b4:	8b32                	mv	s6,a2
 5b6:	00158493          	addi	s1,a1,1
  state = 0;
 5ba:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 5bc:	02500a13          	li	s4,37
 5c0:	4c55                	li	s8,21
 5c2:	00000c97          	auipc	s9,0x0
 5c6:	3cec8c93          	addi	s9,s9,974 # 990 <malloc+0x140>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 5ca:	02800d93          	li	s11,40
  putc(fd, 'x');
 5ce:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 5d0:	00000b97          	auipc	s7,0x0
 5d4:	418b8b93          	addi	s7,s7,1048 # 9e8 <digits>
 5d8:	a839                	j	5f6 <vprintf+0x6a>
        putc(fd, c);
 5da:	85ca                	mv	a1,s2
 5dc:	8556                	mv	a0,s5
 5de:	00000097          	auipc	ra,0x0
 5e2:	ee2080e7          	jalr	-286(ra) # 4c0 <putc>
 5e6:	a019                	j	5ec <vprintf+0x60>
    } else if(state == '%'){
 5e8:	01498d63          	beq	s3,s4,602 <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 5ec:	0485                	addi	s1,s1,1
 5ee:	fff4c903          	lbu	s2,-1(s1)
 5f2:	14090d63          	beqz	s2,74c <vprintf+0x1c0>
    if(state == 0){
 5f6:	fe0999e3          	bnez	s3,5e8 <vprintf+0x5c>
      if(c == '%'){
 5fa:	ff4910e3          	bne	s2,s4,5da <vprintf+0x4e>
        state = '%';
 5fe:	89d2                	mv	s3,s4
 600:	b7f5                	j	5ec <vprintf+0x60>
      if(c == 'd'){
 602:	11490c63          	beq	s2,s4,71a <vprintf+0x18e>
 606:	f9d9079b          	addiw	a5,s2,-99
 60a:	0ff7f793          	zext.b	a5,a5
 60e:	10fc6e63          	bltu	s8,a5,72a <vprintf+0x19e>
 612:	f9d9079b          	addiw	a5,s2,-99
 616:	0ff7f713          	zext.b	a4,a5
 61a:	10ec6863          	bltu	s8,a4,72a <vprintf+0x19e>
 61e:	00271793          	slli	a5,a4,0x2
 622:	97e6                	add	a5,a5,s9
 624:	439c                	lw	a5,0(a5)
 626:	97e6                	add	a5,a5,s9
 628:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 62a:	008b0913          	addi	s2,s6,8
 62e:	4685                	li	a3,1
 630:	4629                	li	a2,10
 632:	000b2583          	lw	a1,0(s6)
 636:	8556                	mv	a0,s5
 638:	00000097          	auipc	ra,0x0
 63c:	eaa080e7          	jalr	-342(ra) # 4e2 <printint>
 640:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 642:	4981                	li	s3,0
 644:	b765                	j	5ec <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 646:	008b0913          	addi	s2,s6,8
 64a:	4681                	li	a3,0
 64c:	4629                	li	a2,10
 64e:	000b2583          	lw	a1,0(s6)
 652:	8556                	mv	a0,s5
 654:	00000097          	auipc	ra,0x0
 658:	e8e080e7          	jalr	-370(ra) # 4e2 <printint>
 65c:	8b4a                	mv	s6,s2
      state = 0;
 65e:	4981                	li	s3,0
 660:	b771                	j	5ec <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 662:	008b0913          	addi	s2,s6,8
 666:	4681                	li	a3,0
 668:	866a                	mv	a2,s10
 66a:	000b2583          	lw	a1,0(s6)
 66e:	8556                	mv	a0,s5
 670:	00000097          	auipc	ra,0x0
 674:	e72080e7          	jalr	-398(ra) # 4e2 <printint>
 678:	8b4a                	mv	s6,s2
      state = 0;
 67a:	4981                	li	s3,0
 67c:	bf85                	j	5ec <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 67e:	008b0793          	addi	a5,s6,8
 682:	f8f43423          	sd	a5,-120(s0)
 686:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 68a:	03000593          	li	a1,48
 68e:	8556                	mv	a0,s5
 690:	00000097          	auipc	ra,0x0
 694:	e30080e7          	jalr	-464(ra) # 4c0 <putc>
  putc(fd, 'x');
 698:	07800593          	li	a1,120
 69c:	8556                	mv	a0,s5
 69e:	00000097          	auipc	ra,0x0
 6a2:	e22080e7          	jalr	-478(ra) # 4c0 <putc>
 6a6:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 6a8:	03c9d793          	srli	a5,s3,0x3c
 6ac:	97de                	add	a5,a5,s7
 6ae:	0007c583          	lbu	a1,0(a5)
 6b2:	8556                	mv	a0,s5
 6b4:	00000097          	auipc	ra,0x0
 6b8:	e0c080e7          	jalr	-500(ra) # 4c0 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 6bc:	0992                	slli	s3,s3,0x4
 6be:	397d                	addiw	s2,s2,-1
 6c0:	fe0914e3          	bnez	s2,6a8 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 6c4:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 6c8:	4981                	li	s3,0
 6ca:	b70d                	j	5ec <vprintf+0x60>
        s = va_arg(ap, char*);
 6cc:	008b0913          	addi	s2,s6,8
 6d0:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 6d4:	02098163          	beqz	s3,6f6 <vprintf+0x16a>
        while(*s != 0){
 6d8:	0009c583          	lbu	a1,0(s3)
 6dc:	c5ad                	beqz	a1,746 <vprintf+0x1ba>
          putc(fd, *s);
 6de:	8556                	mv	a0,s5
 6e0:	00000097          	auipc	ra,0x0
 6e4:	de0080e7          	jalr	-544(ra) # 4c0 <putc>
          s++;
 6e8:	0985                	addi	s3,s3,1
        while(*s != 0){
 6ea:	0009c583          	lbu	a1,0(s3)
 6ee:	f9e5                	bnez	a1,6de <vprintf+0x152>
        s = va_arg(ap, char*);
 6f0:	8b4a                	mv	s6,s2
      state = 0;
 6f2:	4981                	li	s3,0
 6f4:	bde5                	j	5ec <vprintf+0x60>
          s = "(null)";
 6f6:	00000997          	auipc	s3,0x0
 6fa:	29298993          	addi	s3,s3,658 # 988 <malloc+0x138>
        while(*s != 0){
 6fe:	85ee                	mv	a1,s11
 700:	bff9                	j	6de <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 702:	008b0913          	addi	s2,s6,8
 706:	000b4583          	lbu	a1,0(s6)
 70a:	8556                	mv	a0,s5
 70c:	00000097          	auipc	ra,0x0
 710:	db4080e7          	jalr	-588(ra) # 4c0 <putc>
 714:	8b4a                	mv	s6,s2
      state = 0;
 716:	4981                	li	s3,0
 718:	bdd1                	j	5ec <vprintf+0x60>
        putc(fd, c);
 71a:	85d2                	mv	a1,s4
 71c:	8556                	mv	a0,s5
 71e:	00000097          	auipc	ra,0x0
 722:	da2080e7          	jalr	-606(ra) # 4c0 <putc>
      state = 0;
 726:	4981                	li	s3,0
 728:	b5d1                	j	5ec <vprintf+0x60>
        putc(fd, '%');
 72a:	85d2                	mv	a1,s4
 72c:	8556                	mv	a0,s5
 72e:	00000097          	auipc	ra,0x0
 732:	d92080e7          	jalr	-622(ra) # 4c0 <putc>
        putc(fd, c);
 736:	85ca                	mv	a1,s2
 738:	8556                	mv	a0,s5
 73a:	00000097          	auipc	ra,0x0
 73e:	d86080e7          	jalr	-634(ra) # 4c0 <putc>
      state = 0;
 742:	4981                	li	s3,0
 744:	b565                	j	5ec <vprintf+0x60>
        s = va_arg(ap, char*);
 746:	8b4a                	mv	s6,s2
      state = 0;
 748:	4981                	li	s3,0
 74a:	b54d                	j	5ec <vprintf+0x60>
    }
  }
}
 74c:	70e6                	ld	ra,120(sp)
 74e:	7446                	ld	s0,112(sp)
 750:	74a6                	ld	s1,104(sp)
 752:	7906                	ld	s2,96(sp)
 754:	69e6                	ld	s3,88(sp)
 756:	6a46                	ld	s4,80(sp)
 758:	6aa6                	ld	s5,72(sp)
 75a:	6b06                	ld	s6,64(sp)
 75c:	7be2                	ld	s7,56(sp)
 75e:	7c42                	ld	s8,48(sp)
 760:	7ca2                	ld	s9,40(sp)
 762:	7d02                	ld	s10,32(sp)
 764:	6de2                	ld	s11,24(sp)
 766:	6109                	addi	sp,sp,128
 768:	8082                	ret

000000000000076a <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 76a:	715d                	addi	sp,sp,-80
 76c:	ec06                	sd	ra,24(sp)
 76e:	e822                	sd	s0,16(sp)
 770:	1000                	addi	s0,sp,32
 772:	e010                	sd	a2,0(s0)
 774:	e414                	sd	a3,8(s0)
 776:	e818                	sd	a4,16(s0)
 778:	ec1c                	sd	a5,24(s0)
 77a:	03043023          	sd	a6,32(s0)
 77e:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 782:	8622                	mv	a2,s0
 784:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 788:	00000097          	auipc	ra,0x0
 78c:	e04080e7          	jalr	-508(ra) # 58c <vprintf>
}
 790:	60e2                	ld	ra,24(sp)
 792:	6442                	ld	s0,16(sp)
 794:	6161                	addi	sp,sp,80
 796:	8082                	ret

0000000000000798 <printf>:

void
printf(const char *fmt, ...)
{
 798:	711d                	addi	sp,sp,-96
 79a:	ec06                	sd	ra,24(sp)
 79c:	e822                	sd	s0,16(sp)
 79e:	1000                	addi	s0,sp,32
 7a0:	e40c                	sd	a1,8(s0)
 7a2:	e810                	sd	a2,16(s0)
 7a4:	ec14                	sd	a3,24(s0)
 7a6:	f018                	sd	a4,32(s0)
 7a8:	f41c                	sd	a5,40(s0)
 7aa:	03043823          	sd	a6,48(s0)
 7ae:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 7b2:	00840613          	addi	a2,s0,8
 7b6:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 7ba:	85aa                	mv	a1,a0
 7bc:	4505                	li	a0,1
 7be:	00000097          	auipc	ra,0x0
 7c2:	dce080e7          	jalr	-562(ra) # 58c <vprintf>
}
 7c6:	60e2                	ld	ra,24(sp)
 7c8:	6442                	ld	s0,16(sp)
 7ca:	6125                	addi	sp,sp,96
 7cc:	8082                	ret

00000000000007ce <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 7ce:	1141                	addi	sp,sp,-16
 7d0:	e422                	sd	s0,8(sp)
 7d2:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 7d4:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 7d8:	00000797          	auipc	a5,0x0
 7dc:	4287b783          	ld	a5,1064(a5) # c00 <freep>
 7e0:	a02d                	j	80a <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 7e2:	4618                	lw	a4,8(a2)
 7e4:	9f2d                	addw	a4,a4,a1
 7e6:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 7ea:	6398                	ld	a4,0(a5)
 7ec:	6310                	ld	a2,0(a4)
 7ee:	a83d                	j	82c <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 7f0:	ff852703          	lw	a4,-8(a0)
 7f4:	9f31                	addw	a4,a4,a2
 7f6:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 7f8:	ff053683          	ld	a3,-16(a0)
 7fc:	a091                	j	840 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7fe:	6398                	ld	a4,0(a5)
 800:	00e7e463          	bltu	a5,a4,808 <free+0x3a>
 804:	00e6ea63          	bltu	a3,a4,818 <free+0x4a>
{
 808:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 80a:	fed7fae3          	bgeu	a5,a3,7fe <free+0x30>
 80e:	6398                	ld	a4,0(a5)
 810:	00e6e463          	bltu	a3,a4,818 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 814:	fee7eae3          	bltu	a5,a4,808 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 818:	ff852583          	lw	a1,-8(a0)
 81c:	6390                	ld	a2,0(a5)
 81e:	02059813          	slli	a6,a1,0x20
 822:	01c85713          	srli	a4,a6,0x1c
 826:	9736                	add	a4,a4,a3
 828:	fae60de3          	beq	a2,a4,7e2 <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 82c:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 830:	4790                	lw	a2,8(a5)
 832:	02061593          	slli	a1,a2,0x20
 836:	01c5d713          	srli	a4,a1,0x1c
 83a:	973e                	add	a4,a4,a5
 83c:	fae68ae3          	beq	a3,a4,7f0 <free+0x22>
    p->s.ptr = bp->s.ptr;
 840:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 842:	00000717          	auipc	a4,0x0
 846:	3af73f23          	sd	a5,958(a4) # c00 <freep>
}
 84a:	6422                	ld	s0,8(sp)
 84c:	0141                	addi	sp,sp,16
 84e:	8082                	ret

0000000000000850 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 850:	7139                	addi	sp,sp,-64
 852:	fc06                	sd	ra,56(sp)
 854:	f822                	sd	s0,48(sp)
 856:	f426                	sd	s1,40(sp)
 858:	f04a                	sd	s2,32(sp)
 85a:	ec4e                	sd	s3,24(sp)
 85c:	e852                	sd	s4,16(sp)
 85e:	e456                	sd	s5,8(sp)
 860:	e05a                	sd	s6,0(sp)
 862:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 864:	02051493          	slli	s1,a0,0x20
 868:	9081                	srli	s1,s1,0x20
 86a:	04bd                	addi	s1,s1,15
 86c:	8091                	srli	s1,s1,0x4
 86e:	00148a1b          	addiw	s4,s1,1
 872:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 874:	00000517          	auipc	a0,0x0
 878:	38c53503          	ld	a0,908(a0) # c00 <freep>
 87c:	c515                	beqz	a0,8a8 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 87e:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 880:	4798                	lw	a4,8(a5)
 882:	04977163          	bgeu	a4,s1,8c4 <malloc+0x74>
 886:	89d2                	mv	s3,s4
 888:	000a071b          	sext.w	a4,s4
 88c:	6685                	lui	a3,0x1
 88e:	00d77363          	bgeu	a4,a3,894 <malloc+0x44>
 892:	6985                	lui	s3,0x1
 894:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 898:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 89c:	00000917          	auipc	s2,0x0
 8a0:	36490913          	addi	s2,s2,868 # c00 <freep>
  if(p == (char*)-1)
 8a4:	5afd                	li	s5,-1
 8a6:	a8a5                	j	91e <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 8a8:	00000797          	auipc	a5,0x0
 8ac:	35878793          	addi	a5,a5,856 # c00 <freep>
 8b0:	00000717          	auipc	a4,0x0
 8b4:	35870713          	addi	a4,a4,856 # c08 <base>
 8b8:	e398                	sd	a4,0(a5)
 8ba:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 8bc:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 8c0:	87ba                	mv	a5,a4
 8c2:	b7d1                	j	886 <malloc+0x36>
      if(p->s.size == nunits)
 8c4:	02e48c63          	beq	s1,a4,8fc <malloc+0xac>
        p->s.size -= nunits;
 8c8:	4147073b          	subw	a4,a4,s4
 8cc:	c798                	sw	a4,8(a5)
        p += p->s.size;
 8ce:	02071693          	slli	a3,a4,0x20
 8d2:	01c6d713          	srli	a4,a3,0x1c
 8d6:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 8d8:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 8dc:	00000717          	auipc	a4,0x0
 8e0:	32a73223          	sd	a0,804(a4) # c00 <freep>
      return (void*)(p + 1);
 8e4:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 8e8:	70e2                	ld	ra,56(sp)
 8ea:	7442                	ld	s0,48(sp)
 8ec:	74a2                	ld	s1,40(sp)
 8ee:	7902                	ld	s2,32(sp)
 8f0:	69e2                	ld	s3,24(sp)
 8f2:	6a42                	ld	s4,16(sp)
 8f4:	6aa2                	ld	s5,8(sp)
 8f6:	6b02                	ld	s6,0(sp)
 8f8:	6121                	addi	sp,sp,64
 8fa:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 8fc:	6398                	ld	a4,0(a5)
 8fe:	e118                	sd	a4,0(a0)
 900:	bff1                	j	8dc <malloc+0x8c>
  hp->s.size = nu;
 902:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 906:	0541                	addi	a0,a0,16
 908:	00000097          	auipc	ra,0x0
 90c:	ec6080e7          	jalr	-314(ra) # 7ce <free>
  return freep;
 910:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 914:	d971                	beqz	a0,8e8 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 916:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 918:	4798                	lw	a4,8(a5)
 91a:	fa9775e3          	bgeu	a4,s1,8c4 <malloc+0x74>
    if(p == freep)
 91e:	00093703          	ld	a4,0(s2)
 922:	853e                	mv	a0,a5
 924:	fef719e3          	bne	a4,a5,916 <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 928:	854e                	mv	a0,s3
 92a:	00000097          	auipc	ra,0x0
 92e:	b34080e7          	jalr	-1228(ra) # 45e <sbrk>
  if(p == (char*)-1)
 932:	fd5518e3          	bne	a0,s5,902 <malloc+0xb2>
        return 0;
 936:	4501                	li	a0,0
 938:	bf45                	j	8e8 <malloc+0x98>
