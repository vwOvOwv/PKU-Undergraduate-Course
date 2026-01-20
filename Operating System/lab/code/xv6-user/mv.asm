
xv6-user/_mv:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <main>:
#include "kernel/include/fcntl.h"
#include "kernel/include/param.h"
#include "xv6-user/user.h"

int main(int argc, char *argv[])
{
   0:	d9010113          	addi	sp,sp,-624
   4:	26113423          	sd	ra,616(sp)
   8:	26813023          	sd	s0,608(sp)
   c:	24913c23          	sd	s1,600(sp)
  10:	25213823          	sd	s2,592(sp)
  14:	1c80                	addi	s0,sp,624
    if (argc < 3) {
  16:	4789                	li	a5,2
  18:	02a7c063          	blt	a5,a0,38 <main+0x38>
        fprintf(2, "Usage: mv old_name new_name\n");
  1c:	00001597          	auipc	a1,0x1
  20:	9ac58593          	addi	a1,a1,-1620 # 9c8 <malloc+0xec>
  24:	4509                	li	a0,2
  26:	00000097          	auipc	ra,0x0
  2a:	7d0080e7          	jalr	2000(ra) # 7f6 <fprintf>
        exit(1);
  2e:	4505                	li	a0,1
  30:	00000097          	auipc	ra,0x0
  34:	432080e7          	jalr	1074(ra) # 462 <exit>
  38:	84ae                	mv	s1,a1
    }

    char src[MAXPATH];
    char dst[MAXPATH];
    strcpy(src, argv[1]);
  3a:	658c                	ld	a1,8(a1)
  3c:	ed840513          	addi	a0,s0,-296
  40:	00000097          	auipc	ra,0x0
  44:	16a080e7          	jalr	362(ra) # 1aa <strcpy>
    strcpy(dst, argv[2]);
  48:	688c                	ld	a1,16(s1)
  4a:	dd040493          	addi	s1,s0,-560
  4e:	8526                	mv	a0,s1
  50:	00000097          	auipc	ra,0x0
  54:	15a080e7          	jalr	346(ra) # 1aa <strcpy>
    int fd = open(dst, O_RDONLY);
  58:	4581                	li	a1,0
  5a:	8526                	mv	a0,s1
  5c:	00000097          	auipc	ra,0x0
  60:	454080e7          	jalr	1108(ra) # 4b0 <open>
  64:	84aa                	mv	s1,a0
    if (fd >= 0) {
  66:	0e054763          	bltz	a0,154 <main+0x154>
        struct stat st;
        fstat(fd, &st);
  6a:	d9840593          	addi	a1,s0,-616
  6e:	00000097          	auipc	ra,0x0
  72:	44c080e7          	jalr	1100(ra) # 4ba <fstat>
        close(fd);
  76:	8526                	mv	a0,s1
  78:	00000097          	auipc	ra,0x0
  7c:	41c080e7          	jalr	1052(ra) # 494 <close>
        if (st.type == T_DIR) {
  80:	dc041703          	lh	a4,-576(s0)
  84:	4785                	li	a5,1
  86:	02f70263          	beq	a4,a5,aa <main+0xaa>
                    fprintf(2, "mv: fail! final dst path too long (exceed MAX=%d)!\n", MAXPATH);
                    exit(-1);
                }
            }
        } else {
            fprintf(2, "mv: fail! %s exists!\n", dst);
  8a:	dd040613          	addi	a2,s0,-560
  8e:	00001597          	auipc	a1,0x1
  92:	99258593          	addi	a1,a1,-1646 # a20 <malloc+0x144>
  96:	4509                	li	a0,2
  98:	00000097          	auipc	ra,0x0
  9c:	75e080e7          	jalr	1886(ra) # 7f6 <fprintf>
            exit(-1);
  a0:	557d                	li	a0,-1
  a2:	00000097          	auipc	ra,0x0
  a6:	3c0080e7          	jalr	960(ra) # 462 <exit>
            for (ps = src + strlen(src) - 1; ps >= src; ps--) { // trim '/' in tail
  aa:	ed840493          	addi	s1,s0,-296
  ae:	8526                	mv	a0,s1
  b0:	00000097          	auipc	ra,0x0
  b4:	170080e7          	jalr	368(ra) # 220 <strlen>
  b8:	02051793          	slli	a5,a0,0x20
  bc:	9381                	srli	a5,a5,0x20
  be:	17fd                	addi	a5,a5,-1
  c0:	97a6                	add	a5,a5,s1
  c2:	0297ec63          	bltu	a5,s1,fa <main+0xfa>
                if (*ps != '/') {
  c6:	02f00693          	li	a3,47
            for (ps = src + strlen(src) - 1; ps >= src; ps--) { // trim '/' in tail
  ca:	8626                	mv	a2,s1
                if (*ps != '/') {
  cc:	0007c703          	lbu	a4,0(a5)
  d0:	00d71663          	bne	a4,a3,dc <main+0xdc>
            for (ps = src + strlen(src) - 1; ps >= src; ps--) { // trim '/' in tail
  d4:	17fd                	addi	a5,a5,-1
  d6:	fec7fbe3          	bgeu	a5,a2,cc <main+0xcc>
  da:	a005                	j	fa <main+0xfa>
                    *(ps + 1) = '\0';
  dc:	000780a3          	sb	zero,1(a5)
            for (; ps >= src && *ps != '/'; ps--);
  e0:	02f00693          	li	a3,47
  e4:	ed840613          	addi	a2,s0,-296
  e8:	00c7e963          	bltu	a5,a2,fa <main+0xfa>
  ec:	0007c703          	lbu	a4,0(a5)
  f0:	00d70563          	beq	a4,a3,fa <main+0xfa>
  f4:	17fd                	addi	a5,a5,-1
  f6:	fec7fbe3          	bgeu	a5,a2,ec <main+0xec>
            ps++;
  fa:	00178493          	addi	s1,a5,1
            pd = dst + strlen(dst);
  fe:	dd040913          	addi	s2,s0,-560
 102:	854a                	mv	a0,s2
 104:	00000097          	auipc	ra,0x0
 108:	11c080e7          	jalr	284(ra) # 220 <strlen>
 10c:	1502                	slli	a0,a0,0x20
 10e:	9101                	srli	a0,a0,0x20
 110:	992a                	add	s2,s2,a0
            *pd++ = '/';
 112:	00190793          	addi	a5,s2,1
 116:	02f00713          	li	a4,47
 11a:	00e90023          	sb	a4,0(s2)
                if (pd >= dst + MAXPATH) {
 11e:	ed440693          	addi	a3,s0,-300
            while (*ps) {
 122:	0004c703          	lbu	a4,0(s1)
 126:	c71d                	beqz	a4,154 <main+0x154>
                *pd++ = *ps++;
 128:	0485                	addi	s1,s1,1
 12a:	0785                	addi	a5,a5,1
 12c:	fee78fa3          	sb	a4,-1(a5)
                if (pd >= dst + MAXPATH) {
 130:	fed7e9e3          	bltu	a5,a3,122 <main+0x122>
                    fprintf(2, "mv: fail! final dst path too long (exceed MAX=%d)!\n", MAXPATH);
 134:	10400613          	li	a2,260
 138:	00001597          	auipc	a1,0x1
 13c:	8b058593          	addi	a1,a1,-1872 # 9e8 <malloc+0x10c>
 140:	4509                	li	a0,2
 142:	00000097          	auipc	ra,0x0
 146:	6b4080e7          	jalr	1716(ra) # 7f6 <fprintf>
                    exit(-1);
 14a:	557d                	li	a0,-1
 14c:	00000097          	auipc	ra,0x0
 150:	316080e7          	jalr	790(ra) # 462 <exit>
        }
    }
    printf("moving [%s] to [%s]\n", src, dst);
 154:	dd040913          	addi	s2,s0,-560
 158:	864a                	mv	a2,s2
 15a:	ed840493          	addi	s1,s0,-296
 15e:	85a6                	mv	a1,s1
 160:	00001517          	auipc	a0,0x1
 164:	8d850513          	addi	a0,a0,-1832 # a38 <malloc+0x15c>
 168:	00000097          	auipc	ra,0x0
 16c:	6bc080e7          	jalr	1724(ra) # 824 <printf>
    if (rename(src, dst) < 0) {
 170:	85ca                	mv	a1,s2
 172:	8526                	mv	a0,s1
 174:	00000097          	auipc	ra,0x0
 178:	3d0080e7          	jalr	976(ra) # 544 <rename>
 17c:	00054763          	bltz	a0,18a <main+0x18a>
        fprintf(2, "mv: fail to rename %s to %s!\n", src, dst);
        exit(-1);
    }
    exit(0);
 180:	4501                	li	a0,0
 182:	00000097          	auipc	ra,0x0
 186:	2e0080e7          	jalr	736(ra) # 462 <exit>
        fprintf(2, "mv: fail to rename %s to %s!\n", src, dst);
 18a:	86ca                	mv	a3,s2
 18c:	8626                	mv	a2,s1
 18e:	00001597          	auipc	a1,0x1
 192:	8c258593          	addi	a1,a1,-1854 # a50 <malloc+0x174>
 196:	4509                	li	a0,2
 198:	00000097          	auipc	ra,0x0
 19c:	65e080e7          	jalr	1630(ra) # 7f6 <fprintf>
        exit(-1);
 1a0:	557d                	li	a0,-1
 1a2:	00000097          	auipc	ra,0x0
 1a6:	2c0080e7          	jalr	704(ra) # 462 <exit>

00000000000001aa <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
 1aa:	1141                	addi	sp,sp,-16
 1ac:	e422                	sd	s0,8(sp)
 1ae:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 1b0:	87aa                	mv	a5,a0
 1b2:	0585                	addi	a1,a1,1
 1b4:	0785                	addi	a5,a5,1
 1b6:	fff5c703          	lbu	a4,-1(a1)
 1ba:	fee78fa3          	sb	a4,-1(a5)
 1be:	fb75                	bnez	a4,1b2 <strcpy+0x8>
    ;
  return os;
}
 1c0:	6422                	ld	s0,8(sp)
 1c2:	0141                	addi	sp,sp,16
 1c4:	8082                	ret

00000000000001c6 <strcat>:

char*
strcat(char *s, const char *t)
{
 1c6:	1141                	addi	sp,sp,-16
 1c8:	e422                	sd	s0,8(sp)
 1ca:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 1cc:	00054783          	lbu	a5,0(a0)
 1d0:	c385                	beqz	a5,1f0 <strcat+0x2a>
 1d2:	87aa                	mv	a5,a0
    s++;
 1d4:	0785                	addi	a5,a5,1
  while(*s)
 1d6:	0007c703          	lbu	a4,0(a5)
 1da:	ff6d                	bnez	a4,1d4 <strcat+0xe>
  while((*s++ = *t++))
 1dc:	0585                	addi	a1,a1,1
 1de:	0785                	addi	a5,a5,1
 1e0:	fff5c703          	lbu	a4,-1(a1)
 1e4:	fee78fa3          	sb	a4,-1(a5)
 1e8:	fb75                	bnez	a4,1dc <strcat+0x16>
    ;
  return os;
}
 1ea:	6422                	ld	s0,8(sp)
 1ec:	0141                	addi	sp,sp,16
 1ee:	8082                	ret
  while(*s)
 1f0:	87aa                	mv	a5,a0
 1f2:	b7ed                	j	1dc <strcat+0x16>

00000000000001f4 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 1f4:	1141                	addi	sp,sp,-16
 1f6:	e422                	sd	s0,8(sp)
 1f8:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 1fa:	00054783          	lbu	a5,0(a0)
 1fe:	cb91                	beqz	a5,212 <strcmp+0x1e>
 200:	0005c703          	lbu	a4,0(a1)
 204:	00f71763          	bne	a4,a5,212 <strcmp+0x1e>
    p++, q++;
 208:	0505                	addi	a0,a0,1
 20a:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 20c:	00054783          	lbu	a5,0(a0)
 210:	fbe5                	bnez	a5,200 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 212:	0005c503          	lbu	a0,0(a1)
}
 216:	40a7853b          	subw	a0,a5,a0
 21a:	6422                	ld	s0,8(sp)
 21c:	0141                	addi	sp,sp,16
 21e:	8082                	ret

0000000000000220 <strlen>:

uint
strlen(const char *s)
{
 220:	1141                	addi	sp,sp,-16
 222:	e422                	sd	s0,8(sp)
 224:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 226:	00054783          	lbu	a5,0(a0)
 22a:	cf91                	beqz	a5,246 <strlen+0x26>
 22c:	0505                	addi	a0,a0,1
 22e:	87aa                	mv	a5,a0
 230:	4685                	li	a3,1
 232:	9e89                	subw	a3,a3,a0
 234:	00f6853b          	addw	a0,a3,a5
 238:	0785                	addi	a5,a5,1
 23a:	fff7c703          	lbu	a4,-1(a5)
 23e:	fb7d                	bnez	a4,234 <strlen+0x14>
    ;
  return n;
}
 240:	6422                	ld	s0,8(sp)
 242:	0141                	addi	sp,sp,16
 244:	8082                	ret
  for(n = 0; s[n]; n++)
 246:	4501                	li	a0,0
 248:	bfe5                	j	240 <strlen+0x20>

000000000000024a <memset>:

void*
memset(void *dst, int c, uint n)
{
 24a:	1141                	addi	sp,sp,-16
 24c:	e422                	sd	s0,8(sp)
 24e:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 250:	ca19                	beqz	a2,266 <memset+0x1c>
 252:	87aa                	mv	a5,a0
 254:	1602                	slli	a2,a2,0x20
 256:	9201                	srli	a2,a2,0x20
 258:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 25c:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 260:	0785                	addi	a5,a5,1
 262:	fee79de3          	bne	a5,a4,25c <memset+0x12>
  }
  return dst;
}
 266:	6422                	ld	s0,8(sp)
 268:	0141                	addi	sp,sp,16
 26a:	8082                	ret

000000000000026c <strchr>:

char*
strchr(const char *s, char c)
{
 26c:	1141                	addi	sp,sp,-16
 26e:	e422                	sd	s0,8(sp)
 270:	0800                	addi	s0,sp,16
  for(; *s; s++)
 272:	00054783          	lbu	a5,0(a0)
 276:	cb99                	beqz	a5,28c <strchr+0x20>
    if(*s == c)
 278:	00f58763          	beq	a1,a5,286 <strchr+0x1a>
  for(; *s; s++)
 27c:	0505                	addi	a0,a0,1
 27e:	00054783          	lbu	a5,0(a0)
 282:	fbfd                	bnez	a5,278 <strchr+0xc>
      return (char*)s;
  return 0;
 284:	4501                	li	a0,0
}
 286:	6422                	ld	s0,8(sp)
 288:	0141                	addi	sp,sp,16
 28a:	8082                	ret
  return 0;
 28c:	4501                	li	a0,0
 28e:	bfe5                	j	286 <strchr+0x1a>

0000000000000290 <gets>:

char*
gets(char *buf, int max)
{
 290:	711d                	addi	sp,sp,-96
 292:	ec86                	sd	ra,88(sp)
 294:	e8a2                	sd	s0,80(sp)
 296:	e4a6                	sd	s1,72(sp)
 298:	e0ca                	sd	s2,64(sp)
 29a:	fc4e                	sd	s3,56(sp)
 29c:	f852                	sd	s4,48(sp)
 29e:	f456                	sd	s5,40(sp)
 2a0:	f05a                	sd	s6,32(sp)
 2a2:	ec5e                	sd	s7,24(sp)
 2a4:	e862                	sd	s8,16(sp)
 2a6:	1080                	addi	s0,sp,96
 2a8:	8baa                	mv	s7,a0
 2aa:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 2ac:	892a                	mv	s2,a0
 2ae:	4481                	li	s1,0
    cc = read(0, &c, 1);
 2b0:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 2b4:	4b29                	li	s6,10
 2b6:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 2b8:	89a6                	mv	s3,s1
 2ba:	2485                	addiw	s1,s1,1
 2bc:	0344d763          	bge	s1,s4,2ea <gets+0x5a>
    cc = read(0, &c, 1);
 2c0:	4605                	li	a2,1
 2c2:	85d6                	mv	a1,s5
 2c4:	4501                	li	a0,0
 2c6:	00000097          	auipc	ra,0x0
 2ca:	1ba080e7          	jalr	442(ra) # 480 <read>
    if(cc < 1)
 2ce:	00a05e63          	blez	a0,2ea <gets+0x5a>
    buf[i++] = c;
 2d2:	faf44783          	lbu	a5,-81(s0)
 2d6:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 2da:	01678763          	beq	a5,s6,2e8 <gets+0x58>
 2de:	0905                	addi	s2,s2,1
 2e0:	fd879ce3          	bne	a5,s8,2b8 <gets+0x28>
  for(i=0; i+1 < max; ){
 2e4:	89a6                	mv	s3,s1
 2e6:	a011                	j	2ea <gets+0x5a>
 2e8:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 2ea:	99de                	add	s3,s3,s7
 2ec:	00098023          	sb	zero,0(s3)
  return buf;
}
 2f0:	855e                	mv	a0,s7
 2f2:	60e6                	ld	ra,88(sp)
 2f4:	6446                	ld	s0,80(sp)
 2f6:	64a6                	ld	s1,72(sp)
 2f8:	6906                	ld	s2,64(sp)
 2fa:	79e2                	ld	s3,56(sp)
 2fc:	7a42                	ld	s4,48(sp)
 2fe:	7aa2                	ld	s5,40(sp)
 300:	7b02                	ld	s6,32(sp)
 302:	6be2                	ld	s7,24(sp)
 304:	6c42                	ld	s8,16(sp)
 306:	6125                	addi	sp,sp,96
 308:	8082                	ret

000000000000030a <stat>:

int
stat(const char *n, struct stat *st)
{
 30a:	1101                	addi	sp,sp,-32
 30c:	ec06                	sd	ra,24(sp)
 30e:	e822                	sd	s0,16(sp)
 310:	e426                	sd	s1,8(sp)
 312:	e04a                	sd	s2,0(sp)
 314:	1000                	addi	s0,sp,32
 316:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 318:	4581                	li	a1,0
 31a:	00000097          	auipc	ra,0x0
 31e:	196080e7          	jalr	406(ra) # 4b0 <open>
  if(fd < 0)
 322:	02054563          	bltz	a0,34c <stat+0x42>
 326:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 328:	85ca                	mv	a1,s2
 32a:	00000097          	auipc	ra,0x0
 32e:	190080e7          	jalr	400(ra) # 4ba <fstat>
 332:	892a                	mv	s2,a0
  close(fd);
 334:	8526                	mv	a0,s1
 336:	00000097          	auipc	ra,0x0
 33a:	15e080e7          	jalr	350(ra) # 494 <close>
  return r;
}
 33e:	854a                	mv	a0,s2
 340:	60e2                	ld	ra,24(sp)
 342:	6442                	ld	s0,16(sp)
 344:	64a2                	ld	s1,8(sp)
 346:	6902                	ld	s2,0(sp)
 348:	6105                	addi	sp,sp,32
 34a:	8082                	ret
    return -1;
 34c:	597d                	li	s2,-1
 34e:	bfc5                	j	33e <stat+0x34>

0000000000000350 <atoi>:

int
atoi(const char *s)
{
 350:	1141                	addi	sp,sp,-16
 352:	e422                	sd	s0,8(sp)
 354:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 356:	00054703          	lbu	a4,0(a0)
 35a:	02d00793          	li	a5,45
  int neg = 1;
 35e:	4585                	li	a1,1
  if (*s == '-') {
 360:	04f70363          	beq	a4,a5,3a6 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 364:	00054703          	lbu	a4,0(a0)
 368:	fd07079b          	addiw	a5,a4,-48
 36c:	0ff7f793          	zext.b	a5,a5
 370:	46a5                	li	a3,9
 372:	02f6ed63          	bltu	a3,a5,3ac <atoi+0x5c>
  n = 0;
 376:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 378:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 37a:	0505                	addi	a0,a0,1
 37c:	0026979b          	slliw	a5,a3,0x2
 380:	9fb5                	addw	a5,a5,a3
 382:	0017979b          	slliw	a5,a5,0x1
 386:	9fb9                	addw	a5,a5,a4
 388:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 38c:	00054703          	lbu	a4,0(a0)
 390:	fd07079b          	addiw	a5,a4,-48
 394:	0ff7f793          	zext.b	a5,a5
 398:	fef671e3          	bgeu	a2,a5,37a <atoi+0x2a>
  return n * neg;
}
 39c:	02d5853b          	mulw	a0,a1,a3
 3a0:	6422                	ld	s0,8(sp)
 3a2:	0141                	addi	sp,sp,16
 3a4:	8082                	ret
    s++;
 3a6:	0505                	addi	a0,a0,1
    neg = -1;
 3a8:	55fd                	li	a1,-1
 3aa:	bf6d                	j	364 <atoi+0x14>
  n = 0;
 3ac:	4681                	li	a3,0
 3ae:	b7fd                	j	39c <atoi+0x4c>

00000000000003b0 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 3b0:	1141                	addi	sp,sp,-16
 3b2:	e422                	sd	s0,8(sp)
 3b4:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 3b6:	02b57463          	bgeu	a0,a1,3de <memmove+0x2e>
    while(n-- > 0)
 3ba:	00c05f63          	blez	a2,3d8 <memmove+0x28>
 3be:	1602                	slli	a2,a2,0x20
 3c0:	9201                	srli	a2,a2,0x20
 3c2:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 3c6:	872a                	mv	a4,a0
      *dst++ = *src++;
 3c8:	0585                	addi	a1,a1,1
 3ca:	0705                	addi	a4,a4,1
 3cc:	fff5c683          	lbu	a3,-1(a1)
 3d0:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 3d4:	fee79ae3          	bne	a5,a4,3c8 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 3d8:	6422                	ld	s0,8(sp)
 3da:	0141                	addi	sp,sp,16
 3dc:	8082                	ret
    dst += n;
 3de:	00c50733          	add	a4,a0,a2
    src += n;
 3e2:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 3e4:	fec05ae3          	blez	a2,3d8 <memmove+0x28>
 3e8:	fff6079b          	addiw	a5,a2,-1
 3ec:	1782                	slli	a5,a5,0x20
 3ee:	9381                	srli	a5,a5,0x20
 3f0:	fff7c793          	not	a5,a5
 3f4:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 3f6:	15fd                	addi	a1,a1,-1
 3f8:	177d                	addi	a4,a4,-1
 3fa:	0005c683          	lbu	a3,0(a1)
 3fe:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 402:	fee79ae3          	bne	a5,a4,3f6 <memmove+0x46>
 406:	bfc9                	j	3d8 <memmove+0x28>

0000000000000408 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 408:	1141                	addi	sp,sp,-16
 40a:	e422                	sd	s0,8(sp)
 40c:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 40e:	ca05                	beqz	a2,43e <memcmp+0x36>
 410:	fff6069b          	addiw	a3,a2,-1
 414:	1682                	slli	a3,a3,0x20
 416:	9281                	srli	a3,a3,0x20
 418:	0685                	addi	a3,a3,1
 41a:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 41c:	00054783          	lbu	a5,0(a0)
 420:	0005c703          	lbu	a4,0(a1)
 424:	00e79863          	bne	a5,a4,434 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 428:	0505                	addi	a0,a0,1
    p2++;
 42a:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 42c:	fed518e3          	bne	a0,a3,41c <memcmp+0x14>
  }
  return 0;
 430:	4501                	li	a0,0
 432:	a019                	j	438 <memcmp+0x30>
      return *p1 - *p2;
 434:	40e7853b          	subw	a0,a5,a4
}
 438:	6422                	ld	s0,8(sp)
 43a:	0141                	addi	sp,sp,16
 43c:	8082                	ret
  return 0;
 43e:	4501                	li	a0,0
 440:	bfe5                	j	438 <memcmp+0x30>

0000000000000442 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 442:	1141                	addi	sp,sp,-16
 444:	e406                	sd	ra,8(sp)
 446:	e022                	sd	s0,0(sp)
 448:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 44a:	00000097          	auipc	ra,0x0
 44e:	f66080e7          	jalr	-154(ra) # 3b0 <memmove>
}
 452:	60a2                	ld	ra,8(sp)
 454:	6402                	ld	s0,0(sp)
 456:	0141                	addi	sp,sp,16
 458:	8082                	ret

000000000000045a <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 45a:	4885                	li	a7,1
 ecall
 45c:	00000073          	ecall
 ret
 460:	8082                	ret

0000000000000462 <exit>:
.global exit
exit:
 li a7, SYS_exit
 462:	05d00893          	li	a7,93
 ecall
 466:	00000073          	ecall
 ret
 46a:	8082                	ret

000000000000046c <wait>:
.global wait
wait:
 li a7, SYS_wait
 46c:	4bf00893          	li	a7,1215
 ecall
 470:	00000073          	ecall
 ret
 474:	8082                	ret

0000000000000476 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 476:	03b00893          	li	a7,59
 ecall
 47a:	00000073          	ecall
 ret
 47e:	8082                	ret

0000000000000480 <read>:
.global read
read:
 li a7, SYS_read
 480:	03f00893          	li	a7,63
 ecall
 484:	00000073          	ecall
 ret
 488:	8082                	ret

000000000000048a <write>:
.global write
write:
 li a7, SYS_write
 48a:	04000893          	li	a7,64
 ecall
 48e:	00000073          	ecall
 ret
 492:	8082                	ret

0000000000000494 <close>:
.global close
close:
 li a7, SYS_close
 494:	03900893          	li	a7,57
 ecall
 498:	00000073          	ecall
 ret
 49c:	8082                	ret

000000000000049e <kill>:
.global kill
kill:
 li a7, SYS_kill
 49e:	4899                	li	a7,6
 ecall
 4a0:	00000073          	ecall
 ret
 4a4:	8082                	ret

00000000000004a6 <exec>:
.global exec
exec:
 li a7, SYS_exec
 4a6:	0dd00893          	li	a7,221
 ecall
 4aa:	00000073          	ecall
 ret
 4ae:	8082                	ret

00000000000004b0 <open>:
.global open
open:
 li a7, SYS_open
 4b0:	03800893          	li	a7,56
 ecall
 4b4:	00000073          	ecall
 ret
 4b8:	8082                	ret

00000000000004ba <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 4ba:	05000893          	li	a7,80
 ecall
 4be:	00000073          	ecall
 ret
 4c2:	8082                	ret

00000000000004c4 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 4c4:	02200893          	li	a7,34
 ecall
 4c8:	00000073          	ecall
 ret
 4cc:	8082                	ret

00000000000004ce <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 4ce:	03100893          	li	a7,49
 ecall
 4d2:	00000073          	ecall
 ret
 4d6:	8082                	ret

00000000000004d8 <dup>:
.global dup
dup:
 li a7, SYS_dup
 4d8:	48dd                	li	a7,23
 ecall
 4da:	00000073          	ecall
 ret
 4de:	8082                	ret

00000000000004e0 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 4e0:	0ac00893          	li	a7,172
 ecall
 4e4:	00000073          	ecall
 ret
 4e8:	8082                	ret

00000000000004ea <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 4ea:	0d600893          	li	a7,214
 ecall
 4ee:	00000073          	ecall
 ret
 4f2:	8082                	ret

00000000000004f4 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 4f4:	48b5                	li	a7,13
 ecall
 4f6:	00000073          	ecall
 ret
 4fa:	8082                	ret

00000000000004fc <times>:
.global times
times:
 li a7, SYS_times
 4fc:	09900893          	li	a7,153
 ecall
 500:	00000073          	ecall
 ret
 504:	8082                	ret

0000000000000506 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 506:	48d9                	li	a7,22
 ecall
 508:	00000073          	ecall
 ret
 50c:	8082                	ret

000000000000050e <dev>:
.global dev
dev:
 li a7, SYS_dev
 50e:	6889                	lui	a7,0x2
 510:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x1485>
 ecall
 514:	00000073          	ecall
 ret
 518:	8082                	ret

000000000000051a <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 51a:	7d200893          	li	a7,2002
 ecall
 51e:	00000073          	ecall
 ret
 522:	8082                	ret

0000000000000524 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 524:	48c5                	li	a7,17
 ecall
 526:	00000073          	ecall
 ret
 52a:	8082                	ret

000000000000052c <remove>:
.global remove
remove:
 li a7, SYS_remove
 52c:	48e5                	li	a7,25
 ecall
 52e:	00000073          	ecall
 ret
 532:	8082                	ret

0000000000000534 <trace>:
.global trace
trace:
 li a7, SYS_trace
 534:	48c9                	li	a7,18
 ecall
 536:	00000073          	ecall
 ret
 53a:	8082                	ret

000000000000053c <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 53c:	48cd                	li	a7,19
 ecall
 53e:	00000073          	ecall
 ret
 542:	8082                	ret

0000000000000544 <rename>:
.global rename
rename:
 li a7, SYS_rename
 544:	48e9                	li	a7,26
 ecall
 546:	00000073          	ecall
 ret
 54a:	8082                	ret

000000000000054c <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 54c:	1101                	addi	sp,sp,-32
 54e:	ec06                	sd	ra,24(sp)
 550:	e822                	sd	s0,16(sp)
 552:	1000                	addi	s0,sp,32
 554:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 558:	4605                	li	a2,1
 55a:	fef40593          	addi	a1,s0,-17
 55e:	00000097          	auipc	ra,0x0
 562:	f2c080e7          	jalr	-212(ra) # 48a <write>
}
 566:	60e2                	ld	ra,24(sp)
 568:	6442                	ld	s0,16(sp)
 56a:	6105                	addi	sp,sp,32
 56c:	8082                	ret

000000000000056e <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 56e:	7139                	addi	sp,sp,-64
 570:	fc06                	sd	ra,56(sp)
 572:	f822                	sd	s0,48(sp)
 574:	f426                	sd	s1,40(sp)
 576:	f04a                	sd	s2,32(sp)
 578:	ec4e                	sd	s3,24(sp)
 57a:	0080                	addi	s0,sp,64
 57c:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 57e:	c299                	beqz	a3,584 <printint+0x16>
 580:	0805c863          	bltz	a1,610 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 584:	2581                	sext.w	a1,a1
  neg = 0;
 586:	4881                	li	a7,0
  }

  i = 0;
 588:	fc040993          	addi	s3,s0,-64
  neg = 0;
 58c:	86ce                	mv	a3,s3
  i = 0;
 58e:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 590:	2601                	sext.w	a2,a2
 592:	00000517          	auipc	a0,0x0
 596:	53e50513          	addi	a0,a0,1342 # ad0 <digits>
 59a:	883a                	mv	a6,a4
 59c:	2705                	addiw	a4,a4,1
 59e:	02c5f7bb          	remuw	a5,a1,a2
 5a2:	1782                	slli	a5,a5,0x20
 5a4:	9381                	srli	a5,a5,0x20
 5a6:	97aa                	add	a5,a5,a0
 5a8:	0007c783          	lbu	a5,0(a5)
 5ac:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 5b0:	0005879b          	sext.w	a5,a1
 5b4:	02c5d5bb          	divuw	a1,a1,a2
 5b8:	0685                	addi	a3,a3,1
 5ba:	fec7f0e3          	bgeu	a5,a2,59a <printint+0x2c>
  if(neg)
 5be:	00088c63          	beqz	a7,5d6 <printint+0x68>
    buf[i++] = '-';
 5c2:	fd070793          	addi	a5,a4,-48
 5c6:	00878733          	add	a4,a5,s0
 5ca:	02d00793          	li	a5,45
 5ce:	fef70823          	sb	a5,-16(a4)
 5d2:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 5d6:	02e05663          	blez	a4,602 <printint+0x94>
 5da:	fc040913          	addi	s2,s0,-64
 5de:	993a                	add	s2,s2,a4
 5e0:	19fd                	addi	s3,s3,-1
 5e2:	99ba                	add	s3,s3,a4
 5e4:	377d                	addiw	a4,a4,-1
 5e6:	1702                	slli	a4,a4,0x20
 5e8:	9301                	srli	a4,a4,0x20
 5ea:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 5ee:	fff94583          	lbu	a1,-1(s2)
 5f2:	8526                	mv	a0,s1
 5f4:	00000097          	auipc	ra,0x0
 5f8:	f58080e7          	jalr	-168(ra) # 54c <putc>
  while(--i >= 0)
 5fc:	197d                	addi	s2,s2,-1
 5fe:	ff3918e3          	bne	s2,s3,5ee <printint+0x80>
}
 602:	70e2                	ld	ra,56(sp)
 604:	7442                	ld	s0,48(sp)
 606:	74a2                	ld	s1,40(sp)
 608:	7902                	ld	s2,32(sp)
 60a:	69e2                	ld	s3,24(sp)
 60c:	6121                	addi	sp,sp,64
 60e:	8082                	ret
    x = -xx;
 610:	40b005bb          	negw	a1,a1
    neg = 1;
 614:	4885                	li	a7,1
    x = -xx;
 616:	bf8d                	j	588 <printint+0x1a>

0000000000000618 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 618:	7119                	addi	sp,sp,-128
 61a:	fc86                	sd	ra,120(sp)
 61c:	f8a2                	sd	s0,112(sp)
 61e:	f4a6                	sd	s1,104(sp)
 620:	f0ca                	sd	s2,96(sp)
 622:	ecce                	sd	s3,88(sp)
 624:	e8d2                	sd	s4,80(sp)
 626:	e4d6                	sd	s5,72(sp)
 628:	e0da                	sd	s6,64(sp)
 62a:	fc5e                	sd	s7,56(sp)
 62c:	f862                	sd	s8,48(sp)
 62e:	f466                	sd	s9,40(sp)
 630:	f06a                	sd	s10,32(sp)
 632:	ec6e                	sd	s11,24(sp)
 634:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 636:	0005c903          	lbu	s2,0(a1)
 63a:	18090f63          	beqz	s2,7d8 <vprintf+0x1c0>
 63e:	8aaa                	mv	s5,a0
 640:	8b32                	mv	s6,a2
 642:	00158493          	addi	s1,a1,1
  state = 0;
 646:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 648:	02500a13          	li	s4,37
 64c:	4c55                	li	s8,21
 64e:	00000c97          	auipc	s9,0x0
 652:	42ac8c93          	addi	s9,s9,1066 # a78 <malloc+0x19c>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 656:	02800d93          	li	s11,40
  putc(fd, 'x');
 65a:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 65c:	00000b97          	auipc	s7,0x0
 660:	474b8b93          	addi	s7,s7,1140 # ad0 <digits>
 664:	a839                	j	682 <vprintf+0x6a>
        putc(fd, c);
 666:	85ca                	mv	a1,s2
 668:	8556                	mv	a0,s5
 66a:	00000097          	auipc	ra,0x0
 66e:	ee2080e7          	jalr	-286(ra) # 54c <putc>
 672:	a019                	j	678 <vprintf+0x60>
    } else if(state == '%'){
 674:	01498d63          	beq	s3,s4,68e <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 678:	0485                	addi	s1,s1,1
 67a:	fff4c903          	lbu	s2,-1(s1)
 67e:	14090d63          	beqz	s2,7d8 <vprintf+0x1c0>
    if(state == 0){
 682:	fe0999e3          	bnez	s3,674 <vprintf+0x5c>
      if(c == '%'){
 686:	ff4910e3          	bne	s2,s4,666 <vprintf+0x4e>
        state = '%';
 68a:	89d2                	mv	s3,s4
 68c:	b7f5                	j	678 <vprintf+0x60>
      if(c == 'd'){
 68e:	11490c63          	beq	s2,s4,7a6 <vprintf+0x18e>
 692:	f9d9079b          	addiw	a5,s2,-99
 696:	0ff7f793          	zext.b	a5,a5
 69a:	10fc6e63          	bltu	s8,a5,7b6 <vprintf+0x19e>
 69e:	f9d9079b          	addiw	a5,s2,-99
 6a2:	0ff7f713          	zext.b	a4,a5
 6a6:	10ec6863          	bltu	s8,a4,7b6 <vprintf+0x19e>
 6aa:	00271793          	slli	a5,a4,0x2
 6ae:	97e6                	add	a5,a5,s9
 6b0:	439c                	lw	a5,0(a5)
 6b2:	97e6                	add	a5,a5,s9
 6b4:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 6b6:	008b0913          	addi	s2,s6,8
 6ba:	4685                	li	a3,1
 6bc:	4629                	li	a2,10
 6be:	000b2583          	lw	a1,0(s6)
 6c2:	8556                	mv	a0,s5
 6c4:	00000097          	auipc	ra,0x0
 6c8:	eaa080e7          	jalr	-342(ra) # 56e <printint>
 6cc:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 6ce:	4981                	li	s3,0
 6d0:	b765                	j	678 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 6d2:	008b0913          	addi	s2,s6,8
 6d6:	4681                	li	a3,0
 6d8:	4629                	li	a2,10
 6da:	000b2583          	lw	a1,0(s6)
 6de:	8556                	mv	a0,s5
 6e0:	00000097          	auipc	ra,0x0
 6e4:	e8e080e7          	jalr	-370(ra) # 56e <printint>
 6e8:	8b4a                	mv	s6,s2
      state = 0;
 6ea:	4981                	li	s3,0
 6ec:	b771                	j	678 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 6ee:	008b0913          	addi	s2,s6,8
 6f2:	4681                	li	a3,0
 6f4:	866a                	mv	a2,s10
 6f6:	000b2583          	lw	a1,0(s6)
 6fa:	8556                	mv	a0,s5
 6fc:	00000097          	auipc	ra,0x0
 700:	e72080e7          	jalr	-398(ra) # 56e <printint>
 704:	8b4a                	mv	s6,s2
      state = 0;
 706:	4981                	li	s3,0
 708:	bf85                	j	678 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 70a:	008b0793          	addi	a5,s6,8
 70e:	f8f43423          	sd	a5,-120(s0)
 712:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 716:	03000593          	li	a1,48
 71a:	8556                	mv	a0,s5
 71c:	00000097          	auipc	ra,0x0
 720:	e30080e7          	jalr	-464(ra) # 54c <putc>
  putc(fd, 'x');
 724:	07800593          	li	a1,120
 728:	8556                	mv	a0,s5
 72a:	00000097          	auipc	ra,0x0
 72e:	e22080e7          	jalr	-478(ra) # 54c <putc>
 732:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 734:	03c9d793          	srli	a5,s3,0x3c
 738:	97de                	add	a5,a5,s7
 73a:	0007c583          	lbu	a1,0(a5)
 73e:	8556                	mv	a0,s5
 740:	00000097          	auipc	ra,0x0
 744:	e0c080e7          	jalr	-500(ra) # 54c <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 748:	0992                	slli	s3,s3,0x4
 74a:	397d                	addiw	s2,s2,-1
 74c:	fe0914e3          	bnez	s2,734 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 750:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 754:	4981                	li	s3,0
 756:	b70d                	j	678 <vprintf+0x60>
        s = va_arg(ap, char*);
 758:	008b0913          	addi	s2,s6,8
 75c:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 760:	02098163          	beqz	s3,782 <vprintf+0x16a>
        while(*s != 0){
 764:	0009c583          	lbu	a1,0(s3)
 768:	c5ad                	beqz	a1,7d2 <vprintf+0x1ba>
          putc(fd, *s);
 76a:	8556                	mv	a0,s5
 76c:	00000097          	auipc	ra,0x0
 770:	de0080e7          	jalr	-544(ra) # 54c <putc>
          s++;
 774:	0985                	addi	s3,s3,1
        while(*s != 0){
 776:	0009c583          	lbu	a1,0(s3)
 77a:	f9e5                	bnez	a1,76a <vprintf+0x152>
        s = va_arg(ap, char*);
 77c:	8b4a                	mv	s6,s2
      state = 0;
 77e:	4981                	li	s3,0
 780:	bde5                	j	678 <vprintf+0x60>
          s = "(null)";
 782:	00000997          	auipc	s3,0x0
 786:	2ee98993          	addi	s3,s3,750 # a70 <malloc+0x194>
        while(*s != 0){
 78a:	85ee                	mv	a1,s11
 78c:	bff9                	j	76a <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 78e:	008b0913          	addi	s2,s6,8
 792:	000b4583          	lbu	a1,0(s6)
 796:	8556                	mv	a0,s5
 798:	00000097          	auipc	ra,0x0
 79c:	db4080e7          	jalr	-588(ra) # 54c <putc>
 7a0:	8b4a                	mv	s6,s2
      state = 0;
 7a2:	4981                	li	s3,0
 7a4:	bdd1                	j	678 <vprintf+0x60>
        putc(fd, c);
 7a6:	85d2                	mv	a1,s4
 7a8:	8556                	mv	a0,s5
 7aa:	00000097          	auipc	ra,0x0
 7ae:	da2080e7          	jalr	-606(ra) # 54c <putc>
      state = 0;
 7b2:	4981                	li	s3,0
 7b4:	b5d1                	j	678 <vprintf+0x60>
        putc(fd, '%');
 7b6:	85d2                	mv	a1,s4
 7b8:	8556                	mv	a0,s5
 7ba:	00000097          	auipc	ra,0x0
 7be:	d92080e7          	jalr	-622(ra) # 54c <putc>
        putc(fd, c);
 7c2:	85ca                	mv	a1,s2
 7c4:	8556                	mv	a0,s5
 7c6:	00000097          	auipc	ra,0x0
 7ca:	d86080e7          	jalr	-634(ra) # 54c <putc>
      state = 0;
 7ce:	4981                	li	s3,0
 7d0:	b565                	j	678 <vprintf+0x60>
        s = va_arg(ap, char*);
 7d2:	8b4a                	mv	s6,s2
      state = 0;
 7d4:	4981                	li	s3,0
 7d6:	b54d                	j	678 <vprintf+0x60>
    }
  }
}
 7d8:	70e6                	ld	ra,120(sp)
 7da:	7446                	ld	s0,112(sp)
 7dc:	74a6                	ld	s1,104(sp)
 7de:	7906                	ld	s2,96(sp)
 7e0:	69e6                	ld	s3,88(sp)
 7e2:	6a46                	ld	s4,80(sp)
 7e4:	6aa6                	ld	s5,72(sp)
 7e6:	6b06                	ld	s6,64(sp)
 7e8:	7be2                	ld	s7,56(sp)
 7ea:	7c42                	ld	s8,48(sp)
 7ec:	7ca2                	ld	s9,40(sp)
 7ee:	7d02                	ld	s10,32(sp)
 7f0:	6de2                	ld	s11,24(sp)
 7f2:	6109                	addi	sp,sp,128
 7f4:	8082                	ret

00000000000007f6 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 7f6:	715d                	addi	sp,sp,-80
 7f8:	ec06                	sd	ra,24(sp)
 7fa:	e822                	sd	s0,16(sp)
 7fc:	1000                	addi	s0,sp,32
 7fe:	e010                	sd	a2,0(s0)
 800:	e414                	sd	a3,8(s0)
 802:	e818                	sd	a4,16(s0)
 804:	ec1c                	sd	a5,24(s0)
 806:	03043023          	sd	a6,32(s0)
 80a:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 80e:	8622                	mv	a2,s0
 810:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 814:	00000097          	auipc	ra,0x0
 818:	e04080e7          	jalr	-508(ra) # 618 <vprintf>
}
 81c:	60e2                	ld	ra,24(sp)
 81e:	6442                	ld	s0,16(sp)
 820:	6161                	addi	sp,sp,80
 822:	8082                	ret

0000000000000824 <printf>:

void
printf(const char *fmt, ...)
{
 824:	711d                	addi	sp,sp,-96
 826:	ec06                	sd	ra,24(sp)
 828:	e822                	sd	s0,16(sp)
 82a:	1000                	addi	s0,sp,32
 82c:	e40c                	sd	a1,8(s0)
 82e:	e810                	sd	a2,16(s0)
 830:	ec14                	sd	a3,24(s0)
 832:	f018                	sd	a4,32(s0)
 834:	f41c                	sd	a5,40(s0)
 836:	03043823          	sd	a6,48(s0)
 83a:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 83e:	00840613          	addi	a2,s0,8
 842:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 846:	85aa                	mv	a1,a0
 848:	4505                	li	a0,1
 84a:	00000097          	auipc	ra,0x0
 84e:	dce080e7          	jalr	-562(ra) # 618 <vprintf>
}
 852:	60e2                	ld	ra,24(sp)
 854:	6442                	ld	s0,16(sp)
 856:	6125                	addi	sp,sp,96
 858:	8082                	ret

000000000000085a <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 85a:	1141                	addi	sp,sp,-16
 85c:	e422                	sd	s0,8(sp)
 85e:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 860:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 864:	00000797          	auipc	a5,0x0
 868:	2847b783          	ld	a5,644(a5) # ae8 <freep>
 86c:	a02d                	j	896 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 86e:	4618                	lw	a4,8(a2)
 870:	9f2d                	addw	a4,a4,a1
 872:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 876:	6398                	ld	a4,0(a5)
 878:	6310                	ld	a2,0(a4)
 87a:	a83d                	j	8b8 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 87c:	ff852703          	lw	a4,-8(a0)
 880:	9f31                	addw	a4,a4,a2
 882:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 884:	ff053683          	ld	a3,-16(a0)
 888:	a091                	j	8cc <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 88a:	6398                	ld	a4,0(a5)
 88c:	00e7e463          	bltu	a5,a4,894 <free+0x3a>
 890:	00e6ea63          	bltu	a3,a4,8a4 <free+0x4a>
{
 894:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 896:	fed7fae3          	bgeu	a5,a3,88a <free+0x30>
 89a:	6398                	ld	a4,0(a5)
 89c:	00e6e463          	bltu	a3,a4,8a4 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8a0:	fee7eae3          	bltu	a5,a4,894 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 8a4:	ff852583          	lw	a1,-8(a0)
 8a8:	6390                	ld	a2,0(a5)
 8aa:	02059813          	slli	a6,a1,0x20
 8ae:	01c85713          	srli	a4,a6,0x1c
 8b2:	9736                	add	a4,a4,a3
 8b4:	fae60de3          	beq	a2,a4,86e <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 8b8:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 8bc:	4790                	lw	a2,8(a5)
 8be:	02061593          	slli	a1,a2,0x20
 8c2:	01c5d713          	srli	a4,a1,0x1c
 8c6:	973e                	add	a4,a4,a5
 8c8:	fae68ae3          	beq	a3,a4,87c <free+0x22>
    p->s.ptr = bp->s.ptr;
 8cc:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 8ce:	00000717          	auipc	a4,0x0
 8d2:	20f73d23          	sd	a5,538(a4) # ae8 <freep>
}
 8d6:	6422                	ld	s0,8(sp)
 8d8:	0141                	addi	sp,sp,16
 8da:	8082                	ret

00000000000008dc <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 8dc:	7139                	addi	sp,sp,-64
 8de:	fc06                	sd	ra,56(sp)
 8e0:	f822                	sd	s0,48(sp)
 8e2:	f426                	sd	s1,40(sp)
 8e4:	f04a                	sd	s2,32(sp)
 8e6:	ec4e                	sd	s3,24(sp)
 8e8:	e852                	sd	s4,16(sp)
 8ea:	e456                	sd	s5,8(sp)
 8ec:	e05a                	sd	s6,0(sp)
 8ee:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 8f0:	02051493          	slli	s1,a0,0x20
 8f4:	9081                	srli	s1,s1,0x20
 8f6:	04bd                	addi	s1,s1,15
 8f8:	8091                	srli	s1,s1,0x4
 8fa:	00148a1b          	addiw	s4,s1,1
 8fe:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 900:	00000517          	auipc	a0,0x0
 904:	1e853503          	ld	a0,488(a0) # ae8 <freep>
 908:	c515                	beqz	a0,934 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 90a:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 90c:	4798                	lw	a4,8(a5)
 90e:	04977163          	bgeu	a4,s1,950 <malloc+0x74>
 912:	89d2                	mv	s3,s4
 914:	000a071b          	sext.w	a4,s4
 918:	6685                	lui	a3,0x1
 91a:	00d77363          	bgeu	a4,a3,920 <malloc+0x44>
 91e:	6985                	lui	s3,0x1
 920:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 924:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 928:	00000917          	auipc	s2,0x0
 92c:	1c090913          	addi	s2,s2,448 # ae8 <freep>
  if(p == (char*)-1)
 930:	5afd                	li	s5,-1
 932:	a8a5                	j	9aa <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 934:	00000797          	auipc	a5,0x0
 938:	1b478793          	addi	a5,a5,436 # ae8 <freep>
 93c:	00000717          	auipc	a4,0x0
 940:	1b470713          	addi	a4,a4,436 # af0 <base>
 944:	e398                	sd	a4,0(a5)
 946:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 948:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 94c:	87ba                	mv	a5,a4
 94e:	b7d1                	j	912 <malloc+0x36>
      if(p->s.size == nunits)
 950:	02e48c63          	beq	s1,a4,988 <malloc+0xac>
        p->s.size -= nunits;
 954:	4147073b          	subw	a4,a4,s4
 958:	c798                	sw	a4,8(a5)
        p += p->s.size;
 95a:	02071693          	slli	a3,a4,0x20
 95e:	01c6d713          	srli	a4,a3,0x1c
 962:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 964:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 968:	00000717          	auipc	a4,0x0
 96c:	18a73023          	sd	a0,384(a4) # ae8 <freep>
      return (void*)(p + 1);
 970:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 974:	70e2                	ld	ra,56(sp)
 976:	7442                	ld	s0,48(sp)
 978:	74a2                	ld	s1,40(sp)
 97a:	7902                	ld	s2,32(sp)
 97c:	69e2                	ld	s3,24(sp)
 97e:	6a42                	ld	s4,16(sp)
 980:	6aa2                	ld	s5,8(sp)
 982:	6b02                	ld	s6,0(sp)
 984:	6121                	addi	sp,sp,64
 986:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 988:	6398                	ld	a4,0(a5)
 98a:	e118                	sd	a4,0(a0)
 98c:	bff1                	j	968 <malloc+0x8c>
  hp->s.size = nu;
 98e:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 992:	0541                	addi	a0,a0,16
 994:	00000097          	auipc	ra,0x0
 998:	ec6080e7          	jalr	-314(ra) # 85a <free>
  return freep;
 99c:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 9a0:	d971                	beqz	a0,974 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9a2:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 9a4:	4798                	lw	a4,8(a5)
 9a6:	fa9775e3          	bgeu	a4,s1,950 <malloc+0x74>
    if(p == freep)
 9aa:	00093703          	ld	a4,0(s2)
 9ae:	853e                	mv	a0,a5
 9b0:	fef719e3          	bne	a4,a5,9a2 <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 9b4:	854e                	mv	a0,s3
 9b6:	00000097          	auipc	ra,0x0
 9ba:	b34080e7          	jalr	-1228(ra) # 4ea <sbrk>
  if(p == (char*)-1)
 9be:	fd5518e3          	bne	a0,s5,98e <malloc+0xb2>
        return 0;
 9c2:	4501                	li	a0,0
 9c4:	bf45                	j	974 <malloc+0x98>
