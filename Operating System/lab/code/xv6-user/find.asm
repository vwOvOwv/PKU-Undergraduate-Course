
xv6-user/_find:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <find>:
#include "xv6-user/user.h"

static char path[512];

void find(char *filename)
{
   0:	7175                	addi	sp,sp,-144
   2:	e506                	sd	ra,136(sp)
   4:	e122                	sd	s0,128(sp)
   6:	fca6                	sd	s1,120(sp)
   8:	f8ca                	sd	s2,112(sp)
   a:	f4ce                	sd	s3,104(sp)
   c:	f0d2                	sd	s4,96(sp)
   e:	ecd6                	sd	s5,88(sp)
  10:	e8da                	sd	s6,80(sp)
  12:	e4de                	sd	s7,72(sp)
  14:	e0e2                	sd	s8,64(sp)
  16:	0900                	addi	s0,sp,144
  18:	8a2a                	mv	s4,a0
    int fd;
    struct stat st;
    if ((fd = open(path, O_RDONLY)) < 0) {
  1a:	4581                	li	a1,0
  1c:	00001517          	auipc	a0,0x1
  20:	b0450513          	addi	a0,a0,-1276 # b20 <path>
  24:	00000097          	auipc	ra,0x0
  28:	4e8080e7          	jalr	1256(ra) # 50c <open>
  2c:	04054163          	bltz	a0,6e <find+0x6e>
  30:	84aa                	mv	s1,a0
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
  32:	f7840593          	addi	a1,s0,-136
  36:	00000097          	auipc	ra,0x0
  3a:	4e0080e7          	jalr	1248(ra) # 516 <fstat>
  3e:	04054663          	bltz	a0,8a <find+0x8a>
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (st.type != T_DIR) {
  42:	fa041703          	lh	a4,-96(s0)
  46:	4785                	li	a5,1
  48:	06f70463          	beq	a4,a5,b0 <find+0xb0>
        close(fd);
  4c:	8526                	mv	a0,s1
  4e:	00000097          	auipc	ra,0x0
  52:	4a2080e7          	jalr	1186(ra) # 4f0 <close>
        }
        find(filename);
    }
    close(fd);
    return;
}
  56:	60aa                	ld	ra,136(sp)
  58:	640a                	ld	s0,128(sp)
  5a:	74e6                	ld	s1,120(sp)
  5c:	7946                	ld	s2,112(sp)
  5e:	79a6                	ld	s3,104(sp)
  60:	7a06                	ld	s4,96(sp)
  62:	6ae6                	ld	s5,88(sp)
  64:	6b46                	ld	s6,80(sp)
  66:	6ba6                	ld	s7,72(sp)
  68:	6c06                	ld	s8,64(sp)
  6a:	6149                	addi	sp,sp,144
  6c:	8082                	ret
        fprintf(2, "find: cannot open %s\n", path);
  6e:	00001617          	auipc	a2,0x1
  72:	ab260613          	addi	a2,a2,-1358 # b20 <path>
  76:	00001597          	auipc	a1,0x1
  7a:	9b258593          	addi	a1,a1,-1614 # a28 <malloc+0xf0>
  7e:	4509                	li	a0,2
  80:	00000097          	auipc	ra,0x0
  84:	7d2080e7          	jalr	2002(ra) # 852 <fprintf>
        return;
  88:	b7f9                	j	56 <find+0x56>
        fprintf(2, "find: cannot stat %s\n", path);
  8a:	00001617          	auipc	a2,0x1
  8e:	a9660613          	addi	a2,a2,-1386 # b20 <path>
  92:	00001597          	auipc	a1,0x1
  96:	9ae58593          	addi	a1,a1,-1618 # a40 <malloc+0x108>
  9a:	4509                	li	a0,2
  9c:	00000097          	auipc	ra,0x0
  a0:	7b6080e7          	jalr	1974(ra) # 852 <fprintf>
        close(fd);
  a4:	8526                	mv	a0,s1
  a6:	00000097          	auipc	ra,0x0
  aa:	44a080e7          	jalr	1098(ra) # 4f0 <close>
        return;
  ae:	b765                	j	56 <find+0x56>
    if (strlen(path) + 255 + 2 > sizeof(path)) {
  b0:	00001517          	auipc	a0,0x1
  b4:	a7050513          	addi	a0,a0,-1424 # b20 <path>
  b8:	00000097          	auipc	ra,0x0
  bc:	1c4080e7          	jalr	452(ra) # 27c <strlen>
  c0:	1015051b          	addiw	a0,a0,257
  c4:	20000793          	li	a5,512
  c8:	0aa7e863          	bltu	a5,a0,178 <find+0x178>
    char *p = path + strlen(path) - 1;
  cc:	00001917          	auipc	s2,0x1
  d0:	a5490913          	addi	s2,s2,-1452 # b20 <path>
  d4:	854a                	mv	a0,s2
  d6:	00000097          	auipc	ra,0x0
  da:	1a6080e7          	jalr	422(ra) # 27c <strlen>
  de:	1502                	slli	a0,a0,0x20
  e0:	9101                	srli	a0,a0,0x20
  e2:	fff50793          	addi	a5,a0,-1
  e6:	993e                	add	s2,s2,a5
    if (*p != '/') {
  e8:	00094703          	lbu	a4,0(s2)
  ec:	02f00793          	li	a5,47
  f0:	00f70963          	beq	a4,a5,102 <find+0x102>
        *++p = '/';
  f4:	00f900a3          	sb	a5,1(s2)
  f8:	00001917          	auipc	s2,0x1
  fc:	a2890913          	addi	s2,s2,-1496 # b20 <path>
 100:	992a                	add	s2,s2,a0
    p++;
 102:	0905                	addi	s2,s2,1
    while (readdir(fd, &st)) {
 104:	f7840993          	addi	s3,s0,-136
        if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
 108:	00001a97          	auipc	s5,0x1
 10c:	968a8a93          	addi	s5,s5,-1688 # a70 <malloc+0x138>
 110:	00001b17          	auipc	s6,0x1
 114:	968b0b13          	addi	s6,s6,-1688 # a78 <malloc+0x140>
            fprintf(1, "%s\n", path);
 118:	00001c17          	auipc	s8,0x1
 11c:	a08c0c13          	addi	s8,s8,-1528 # b20 <path>
 120:	00001b97          	auipc	s7,0x1
 124:	960b8b93          	addi	s7,s7,-1696 # a80 <malloc+0x148>
    while (readdir(fd, &st)) {
 128:	85ce                	mv	a1,s3
 12a:	8526                	mv	a0,s1
 12c:	00000097          	auipc	ra,0x0
 130:	44a080e7          	jalr	1098(ra) # 576 <readdir>
 134:	c92d                	beqz	a0,1a6 <find+0x1a6>
        strcpy(p, st.name);
 136:	85ce                	mv	a1,s3
 138:	854a                	mv	a0,s2
 13a:	00000097          	auipc	ra,0x0
 13e:	0cc080e7          	jalr	204(ra) # 206 <strcpy>
        if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
 142:	85d6                	mv	a1,s5
 144:	854a                	mv	a0,s2
 146:	00000097          	auipc	ra,0x0
 14a:	10a080e7          	jalr	266(ra) # 250 <strcmp>
 14e:	dd69                	beqz	a0,128 <find+0x128>
 150:	85da                	mv	a1,s6
 152:	854a                	mv	a0,s2
 154:	00000097          	auipc	ra,0x0
 158:	0fc080e7          	jalr	252(ra) # 250 <strcmp>
 15c:	d571                	beqz	a0,128 <find+0x128>
        if (strcmp(p, filename) == 0) {
 15e:	85d2                	mv	a1,s4
 160:	854a                	mv	a0,s2
 162:	00000097          	auipc	ra,0x0
 166:	0ee080e7          	jalr	238(ra) # 250 <strcmp>
 16a:	c515                	beqz	a0,196 <find+0x196>
        find(filename);
 16c:	8552                	mv	a0,s4
 16e:	00000097          	auipc	ra,0x0
 172:	e92080e7          	jalr	-366(ra) # 0 <find>
 176:	bf4d                	j	128 <find+0x128>
        fprintf(2, "find: path too long\n");
 178:	00001597          	auipc	a1,0x1
 17c:	8e058593          	addi	a1,a1,-1824 # a58 <malloc+0x120>
 180:	4509                	li	a0,2
 182:	00000097          	auipc	ra,0x0
 186:	6d0080e7          	jalr	1744(ra) # 852 <fprintf>
        close(fd);
 18a:	8526                	mv	a0,s1
 18c:	00000097          	auipc	ra,0x0
 190:	364080e7          	jalr	868(ra) # 4f0 <close>
        return;
 194:	b5c9                	j	56 <find+0x56>
            fprintf(1, "%s\n", path);
 196:	8662                	mv	a2,s8
 198:	85de                	mv	a1,s7
 19a:	4505                	li	a0,1
 19c:	00000097          	auipc	ra,0x0
 1a0:	6b6080e7          	jalr	1718(ra) # 852 <fprintf>
 1a4:	b7e1                	j	16c <find+0x16c>
    close(fd);
 1a6:	8526                	mv	a0,s1
 1a8:	00000097          	auipc	ra,0x0
 1ac:	348080e7          	jalr	840(ra) # 4f0 <close>
    return;
 1b0:	b55d                	j	56 <find+0x56>

00000000000001b2 <main>:


int main(int argc, char *argv[])
{
 1b2:	1101                	addi	sp,sp,-32
 1b4:	ec06                	sd	ra,24(sp)
 1b6:	e822                	sd	s0,16(sp)
 1b8:	e426                	sd	s1,8(sp)
 1ba:	1000                	addi	s0,sp,32
    if (argc < 3) {
 1bc:	4789                	li	a5,2
 1be:	02a7c063          	blt	a5,a0,1de <main+0x2c>
        fprintf(2, "Usage: find DIR FILENAME\n");
 1c2:	00001597          	auipc	a1,0x1
 1c6:	8c658593          	addi	a1,a1,-1850 # a88 <malloc+0x150>
 1ca:	4509                	li	a0,2
 1cc:	00000097          	auipc	ra,0x0
 1d0:	686080e7          	jalr	1670(ra) # 852 <fprintf>
        exit(0);
 1d4:	4501                	li	a0,0
 1d6:	00000097          	auipc	ra,0x0
 1da:	2e8080e7          	jalr	744(ra) # 4be <exit>
 1de:	84ae                	mv	s1,a1
    } else {
        strcpy(path, argv[1]);
 1e0:	658c                	ld	a1,8(a1)
 1e2:	00001517          	auipc	a0,0x1
 1e6:	93e50513          	addi	a0,a0,-1730 # b20 <path>
 1ea:	00000097          	auipc	ra,0x0
 1ee:	01c080e7          	jalr	28(ra) # 206 <strcpy>
        find(argv[2]);
 1f2:	6888                	ld	a0,16(s1)
 1f4:	00000097          	auipc	ra,0x0
 1f8:	e0c080e7          	jalr	-500(ra) # 0 <find>
    }
    exit(0);
 1fc:	4501                	li	a0,0
 1fe:	00000097          	auipc	ra,0x0
 202:	2c0080e7          	jalr	704(ra) # 4be <exit>

0000000000000206 <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
 206:	1141                	addi	sp,sp,-16
 208:	e422                	sd	s0,8(sp)
 20a:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 20c:	87aa                	mv	a5,a0
 20e:	0585                	addi	a1,a1,1
 210:	0785                	addi	a5,a5,1
 212:	fff5c703          	lbu	a4,-1(a1)
 216:	fee78fa3          	sb	a4,-1(a5)
 21a:	fb75                	bnez	a4,20e <strcpy+0x8>
    ;
  return os;
}
 21c:	6422                	ld	s0,8(sp)
 21e:	0141                	addi	sp,sp,16
 220:	8082                	ret

0000000000000222 <strcat>:

char*
strcat(char *s, const char *t)
{
 222:	1141                	addi	sp,sp,-16
 224:	e422                	sd	s0,8(sp)
 226:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 228:	00054783          	lbu	a5,0(a0)
 22c:	c385                	beqz	a5,24c <strcat+0x2a>
 22e:	87aa                	mv	a5,a0
    s++;
 230:	0785                	addi	a5,a5,1
  while(*s)
 232:	0007c703          	lbu	a4,0(a5)
 236:	ff6d                	bnez	a4,230 <strcat+0xe>
  while((*s++ = *t++))
 238:	0585                	addi	a1,a1,1
 23a:	0785                	addi	a5,a5,1
 23c:	fff5c703          	lbu	a4,-1(a1)
 240:	fee78fa3          	sb	a4,-1(a5)
 244:	fb75                	bnez	a4,238 <strcat+0x16>
    ;
  return os;
}
 246:	6422                	ld	s0,8(sp)
 248:	0141                	addi	sp,sp,16
 24a:	8082                	ret
  while(*s)
 24c:	87aa                	mv	a5,a0
 24e:	b7ed                	j	238 <strcat+0x16>

0000000000000250 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 250:	1141                	addi	sp,sp,-16
 252:	e422                	sd	s0,8(sp)
 254:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 256:	00054783          	lbu	a5,0(a0)
 25a:	cb91                	beqz	a5,26e <strcmp+0x1e>
 25c:	0005c703          	lbu	a4,0(a1)
 260:	00f71763          	bne	a4,a5,26e <strcmp+0x1e>
    p++, q++;
 264:	0505                	addi	a0,a0,1
 266:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 268:	00054783          	lbu	a5,0(a0)
 26c:	fbe5                	bnez	a5,25c <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 26e:	0005c503          	lbu	a0,0(a1)
}
 272:	40a7853b          	subw	a0,a5,a0
 276:	6422                	ld	s0,8(sp)
 278:	0141                	addi	sp,sp,16
 27a:	8082                	ret

000000000000027c <strlen>:

uint
strlen(const char *s)
{
 27c:	1141                	addi	sp,sp,-16
 27e:	e422                	sd	s0,8(sp)
 280:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 282:	00054783          	lbu	a5,0(a0)
 286:	cf91                	beqz	a5,2a2 <strlen+0x26>
 288:	0505                	addi	a0,a0,1
 28a:	87aa                	mv	a5,a0
 28c:	4685                	li	a3,1
 28e:	9e89                	subw	a3,a3,a0
 290:	00f6853b          	addw	a0,a3,a5
 294:	0785                	addi	a5,a5,1
 296:	fff7c703          	lbu	a4,-1(a5)
 29a:	fb7d                	bnez	a4,290 <strlen+0x14>
    ;
  return n;
}
 29c:	6422                	ld	s0,8(sp)
 29e:	0141                	addi	sp,sp,16
 2a0:	8082                	ret
  for(n = 0; s[n]; n++)
 2a2:	4501                	li	a0,0
 2a4:	bfe5                	j	29c <strlen+0x20>

00000000000002a6 <memset>:

void*
memset(void *dst, int c, uint n)
{
 2a6:	1141                	addi	sp,sp,-16
 2a8:	e422                	sd	s0,8(sp)
 2aa:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 2ac:	ca19                	beqz	a2,2c2 <memset+0x1c>
 2ae:	87aa                	mv	a5,a0
 2b0:	1602                	slli	a2,a2,0x20
 2b2:	9201                	srli	a2,a2,0x20
 2b4:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 2b8:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 2bc:	0785                	addi	a5,a5,1
 2be:	fee79de3          	bne	a5,a4,2b8 <memset+0x12>
  }
  return dst;
}
 2c2:	6422                	ld	s0,8(sp)
 2c4:	0141                	addi	sp,sp,16
 2c6:	8082                	ret

00000000000002c8 <strchr>:

char*
strchr(const char *s, char c)
{
 2c8:	1141                	addi	sp,sp,-16
 2ca:	e422                	sd	s0,8(sp)
 2cc:	0800                	addi	s0,sp,16
  for(; *s; s++)
 2ce:	00054783          	lbu	a5,0(a0)
 2d2:	cb99                	beqz	a5,2e8 <strchr+0x20>
    if(*s == c)
 2d4:	00f58763          	beq	a1,a5,2e2 <strchr+0x1a>
  for(; *s; s++)
 2d8:	0505                	addi	a0,a0,1
 2da:	00054783          	lbu	a5,0(a0)
 2de:	fbfd                	bnez	a5,2d4 <strchr+0xc>
      return (char*)s;
  return 0;
 2e0:	4501                	li	a0,0
}
 2e2:	6422                	ld	s0,8(sp)
 2e4:	0141                	addi	sp,sp,16
 2e6:	8082                	ret
  return 0;
 2e8:	4501                	li	a0,0
 2ea:	bfe5                	j	2e2 <strchr+0x1a>

00000000000002ec <gets>:

char*
gets(char *buf, int max)
{
 2ec:	711d                	addi	sp,sp,-96
 2ee:	ec86                	sd	ra,88(sp)
 2f0:	e8a2                	sd	s0,80(sp)
 2f2:	e4a6                	sd	s1,72(sp)
 2f4:	e0ca                	sd	s2,64(sp)
 2f6:	fc4e                	sd	s3,56(sp)
 2f8:	f852                	sd	s4,48(sp)
 2fa:	f456                	sd	s5,40(sp)
 2fc:	f05a                	sd	s6,32(sp)
 2fe:	ec5e                	sd	s7,24(sp)
 300:	e862                	sd	s8,16(sp)
 302:	1080                	addi	s0,sp,96
 304:	8baa                	mv	s7,a0
 306:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 308:	892a                	mv	s2,a0
 30a:	4481                	li	s1,0
    cc = read(0, &c, 1);
 30c:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 310:	4b29                	li	s6,10
 312:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 314:	89a6                	mv	s3,s1
 316:	2485                	addiw	s1,s1,1
 318:	0344d763          	bge	s1,s4,346 <gets+0x5a>
    cc = read(0, &c, 1);
 31c:	4605                	li	a2,1
 31e:	85d6                	mv	a1,s5
 320:	4501                	li	a0,0
 322:	00000097          	auipc	ra,0x0
 326:	1ba080e7          	jalr	442(ra) # 4dc <read>
    if(cc < 1)
 32a:	00a05e63          	blez	a0,346 <gets+0x5a>
    buf[i++] = c;
 32e:	faf44783          	lbu	a5,-81(s0)
 332:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 336:	01678763          	beq	a5,s6,344 <gets+0x58>
 33a:	0905                	addi	s2,s2,1
 33c:	fd879ce3          	bne	a5,s8,314 <gets+0x28>
  for(i=0; i+1 < max; ){
 340:	89a6                	mv	s3,s1
 342:	a011                	j	346 <gets+0x5a>
 344:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 346:	99de                	add	s3,s3,s7
 348:	00098023          	sb	zero,0(s3)
  return buf;
}
 34c:	855e                	mv	a0,s7
 34e:	60e6                	ld	ra,88(sp)
 350:	6446                	ld	s0,80(sp)
 352:	64a6                	ld	s1,72(sp)
 354:	6906                	ld	s2,64(sp)
 356:	79e2                	ld	s3,56(sp)
 358:	7a42                	ld	s4,48(sp)
 35a:	7aa2                	ld	s5,40(sp)
 35c:	7b02                	ld	s6,32(sp)
 35e:	6be2                	ld	s7,24(sp)
 360:	6c42                	ld	s8,16(sp)
 362:	6125                	addi	sp,sp,96
 364:	8082                	ret

0000000000000366 <stat>:

int
stat(const char *n, struct stat *st)
{
 366:	1101                	addi	sp,sp,-32
 368:	ec06                	sd	ra,24(sp)
 36a:	e822                	sd	s0,16(sp)
 36c:	e426                	sd	s1,8(sp)
 36e:	e04a                	sd	s2,0(sp)
 370:	1000                	addi	s0,sp,32
 372:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 374:	4581                	li	a1,0
 376:	00000097          	auipc	ra,0x0
 37a:	196080e7          	jalr	406(ra) # 50c <open>
  if(fd < 0)
 37e:	02054563          	bltz	a0,3a8 <stat+0x42>
 382:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 384:	85ca                	mv	a1,s2
 386:	00000097          	auipc	ra,0x0
 38a:	190080e7          	jalr	400(ra) # 516 <fstat>
 38e:	892a                	mv	s2,a0
  close(fd);
 390:	8526                	mv	a0,s1
 392:	00000097          	auipc	ra,0x0
 396:	15e080e7          	jalr	350(ra) # 4f0 <close>
  return r;
}
 39a:	854a                	mv	a0,s2
 39c:	60e2                	ld	ra,24(sp)
 39e:	6442                	ld	s0,16(sp)
 3a0:	64a2                	ld	s1,8(sp)
 3a2:	6902                	ld	s2,0(sp)
 3a4:	6105                	addi	sp,sp,32
 3a6:	8082                	ret
    return -1;
 3a8:	597d                	li	s2,-1
 3aa:	bfc5                	j	39a <stat+0x34>

00000000000003ac <atoi>:

int
atoi(const char *s)
{
 3ac:	1141                	addi	sp,sp,-16
 3ae:	e422                	sd	s0,8(sp)
 3b0:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 3b2:	00054703          	lbu	a4,0(a0)
 3b6:	02d00793          	li	a5,45
  int neg = 1;
 3ba:	4585                	li	a1,1
  if (*s == '-') {
 3bc:	04f70363          	beq	a4,a5,402 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 3c0:	00054703          	lbu	a4,0(a0)
 3c4:	fd07079b          	addiw	a5,a4,-48
 3c8:	0ff7f793          	zext.b	a5,a5
 3cc:	46a5                	li	a3,9
 3ce:	02f6ed63          	bltu	a3,a5,408 <atoi+0x5c>
  n = 0;
 3d2:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 3d4:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 3d6:	0505                	addi	a0,a0,1
 3d8:	0026979b          	slliw	a5,a3,0x2
 3dc:	9fb5                	addw	a5,a5,a3
 3de:	0017979b          	slliw	a5,a5,0x1
 3e2:	9fb9                	addw	a5,a5,a4
 3e4:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 3e8:	00054703          	lbu	a4,0(a0)
 3ec:	fd07079b          	addiw	a5,a4,-48
 3f0:	0ff7f793          	zext.b	a5,a5
 3f4:	fef671e3          	bgeu	a2,a5,3d6 <atoi+0x2a>
  return n * neg;
}
 3f8:	02d5853b          	mulw	a0,a1,a3
 3fc:	6422                	ld	s0,8(sp)
 3fe:	0141                	addi	sp,sp,16
 400:	8082                	ret
    s++;
 402:	0505                	addi	a0,a0,1
    neg = -1;
 404:	55fd                	li	a1,-1
 406:	bf6d                	j	3c0 <atoi+0x14>
  n = 0;
 408:	4681                	li	a3,0
 40a:	b7fd                	j	3f8 <atoi+0x4c>

000000000000040c <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 40c:	1141                	addi	sp,sp,-16
 40e:	e422                	sd	s0,8(sp)
 410:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 412:	02b57463          	bgeu	a0,a1,43a <memmove+0x2e>
    while(n-- > 0)
 416:	00c05f63          	blez	a2,434 <memmove+0x28>
 41a:	1602                	slli	a2,a2,0x20
 41c:	9201                	srli	a2,a2,0x20
 41e:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 422:	872a                	mv	a4,a0
      *dst++ = *src++;
 424:	0585                	addi	a1,a1,1
 426:	0705                	addi	a4,a4,1
 428:	fff5c683          	lbu	a3,-1(a1)
 42c:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 430:	fee79ae3          	bne	a5,a4,424 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 434:	6422                	ld	s0,8(sp)
 436:	0141                	addi	sp,sp,16
 438:	8082                	ret
    dst += n;
 43a:	00c50733          	add	a4,a0,a2
    src += n;
 43e:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 440:	fec05ae3          	blez	a2,434 <memmove+0x28>
 444:	fff6079b          	addiw	a5,a2,-1
 448:	1782                	slli	a5,a5,0x20
 44a:	9381                	srli	a5,a5,0x20
 44c:	fff7c793          	not	a5,a5
 450:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 452:	15fd                	addi	a1,a1,-1
 454:	177d                	addi	a4,a4,-1
 456:	0005c683          	lbu	a3,0(a1)
 45a:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 45e:	fee79ae3          	bne	a5,a4,452 <memmove+0x46>
 462:	bfc9                	j	434 <memmove+0x28>

0000000000000464 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 464:	1141                	addi	sp,sp,-16
 466:	e422                	sd	s0,8(sp)
 468:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 46a:	ca05                	beqz	a2,49a <memcmp+0x36>
 46c:	fff6069b          	addiw	a3,a2,-1
 470:	1682                	slli	a3,a3,0x20
 472:	9281                	srli	a3,a3,0x20
 474:	0685                	addi	a3,a3,1
 476:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 478:	00054783          	lbu	a5,0(a0)
 47c:	0005c703          	lbu	a4,0(a1)
 480:	00e79863          	bne	a5,a4,490 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 484:	0505                	addi	a0,a0,1
    p2++;
 486:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 488:	fed518e3          	bne	a0,a3,478 <memcmp+0x14>
  }
  return 0;
 48c:	4501                	li	a0,0
 48e:	a019                	j	494 <memcmp+0x30>
      return *p1 - *p2;
 490:	40e7853b          	subw	a0,a5,a4
}
 494:	6422                	ld	s0,8(sp)
 496:	0141                	addi	sp,sp,16
 498:	8082                	ret
  return 0;
 49a:	4501                	li	a0,0
 49c:	bfe5                	j	494 <memcmp+0x30>

000000000000049e <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 49e:	1141                	addi	sp,sp,-16
 4a0:	e406                	sd	ra,8(sp)
 4a2:	e022                	sd	s0,0(sp)
 4a4:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 4a6:	00000097          	auipc	ra,0x0
 4aa:	f66080e7          	jalr	-154(ra) # 40c <memmove>
}
 4ae:	60a2                	ld	ra,8(sp)
 4b0:	6402                	ld	s0,0(sp)
 4b2:	0141                	addi	sp,sp,16
 4b4:	8082                	ret

00000000000004b6 <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 4b6:	4885                	li	a7,1
 ecall
 4b8:	00000073          	ecall
 ret
 4bc:	8082                	ret

00000000000004be <exit>:
.global exit
exit:
 li a7, SYS_exit
 4be:	05d00893          	li	a7,93
 ecall
 4c2:	00000073          	ecall
 ret
 4c6:	8082                	ret

00000000000004c8 <wait>:
.global wait
wait:
 li a7, SYS_wait
 4c8:	4bf00893          	li	a7,1215
 ecall
 4cc:	00000073          	ecall
 ret
 4d0:	8082                	ret

00000000000004d2 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 4d2:	03b00893          	li	a7,59
 ecall
 4d6:	00000073          	ecall
 ret
 4da:	8082                	ret

00000000000004dc <read>:
.global read
read:
 li a7, SYS_read
 4dc:	03f00893          	li	a7,63
 ecall
 4e0:	00000073          	ecall
 ret
 4e4:	8082                	ret

00000000000004e6 <write>:
.global write
write:
 li a7, SYS_write
 4e6:	04000893          	li	a7,64
 ecall
 4ea:	00000073          	ecall
 ret
 4ee:	8082                	ret

00000000000004f0 <close>:
.global close
close:
 li a7, SYS_close
 4f0:	03900893          	li	a7,57
 ecall
 4f4:	00000073          	ecall
 ret
 4f8:	8082                	ret

00000000000004fa <kill>:
.global kill
kill:
 li a7, SYS_kill
 4fa:	4899                	li	a7,6
 ecall
 4fc:	00000073          	ecall
 ret
 500:	8082                	ret

0000000000000502 <exec>:
.global exec
exec:
 li a7, SYS_exec
 502:	0dd00893          	li	a7,221
 ecall
 506:	00000073          	ecall
 ret
 50a:	8082                	ret

000000000000050c <open>:
.global open
open:
 li a7, SYS_open
 50c:	03800893          	li	a7,56
 ecall
 510:	00000073          	ecall
 ret
 514:	8082                	ret

0000000000000516 <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 516:	05000893          	li	a7,80
 ecall
 51a:	00000073          	ecall
 ret
 51e:	8082                	ret

0000000000000520 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 520:	02200893          	li	a7,34
 ecall
 524:	00000073          	ecall
 ret
 528:	8082                	ret

000000000000052a <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 52a:	03100893          	li	a7,49
 ecall
 52e:	00000073          	ecall
 ret
 532:	8082                	ret

0000000000000534 <dup>:
.global dup
dup:
 li a7, SYS_dup
 534:	48dd                	li	a7,23
 ecall
 536:	00000073          	ecall
 ret
 53a:	8082                	ret

000000000000053c <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 53c:	0ac00893          	li	a7,172
 ecall
 540:	00000073          	ecall
 ret
 544:	8082                	ret

0000000000000546 <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 546:	0d600893          	li	a7,214
 ecall
 54a:	00000073          	ecall
 ret
 54e:	8082                	ret

0000000000000550 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 550:	48b5                	li	a7,13
 ecall
 552:	00000073          	ecall
 ret
 556:	8082                	ret

0000000000000558 <times>:
.global times
times:
 li a7, SYS_times
 558:	09900893          	li	a7,153
 ecall
 55c:	00000073          	ecall
 ret
 560:	8082                	ret

0000000000000562 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 562:	48d9                	li	a7,22
 ecall
 564:	00000073          	ecall
 ret
 568:	8082                	ret

000000000000056a <dev>:
.global dev
dev:
 li a7, SYS_dev
 56a:	6889                	lui	a7,0x2
 56c:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x144d>
 ecall
 570:	00000073          	ecall
 ret
 574:	8082                	ret

0000000000000576 <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 576:	7d200893          	li	a7,2002
 ecall
 57a:	00000073          	ecall
 ret
 57e:	8082                	ret

0000000000000580 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 580:	48c5                	li	a7,17
 ecall
 582:	00000073          	ecall
 ret
 586:	8082                	ret

0000000000000588 <remove>:
.global remove
remove:
 li a7, SYS_remove
 588:	48e5                	li	a7,25
 ecall
 58a:	00000073          	ecall
 ret
 58e:	8082                	ret

0000000000000590 <trace>:
.global trace
trace:
 li a7, SYS_trace
 590:	48c9                	li	a7,18
 ecall
 592:	00000073          	ecall
 ret
 596:	8082                	ret

0000000000000598 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 598:	48cd                	li	a7,19
 ecall
 59a:	00000073          	ecall
 ret
 59e:	8082                	ret

00000000000005a0 <rename>:
.global rename
rename:
 li a7, SYS_rename
 5a0:	48e9                	li	a7,26
 ecall
 5a2:	00000073          	ecall
 ret
 5a6:	8082                	ret

00000000000005a8 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 5a8:	1101                	addi	sp,sp,-32
 5aa:	ec06                	sd	ra,24(sp)
 5ac:	e822                	sd	s0,16(sp)
 5ae:	1000                	addi	s0,sp,32
 5b0:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 5b4:	4605                	li	a2,1
 5b6:	fef40593          	addi	a1,s0,-17
 5ba:	00000097          	auipc	ra,0x0
 5be:	f2c080e7          	jalr	-212(ra) # 4e6 <write>
}
 5c2:	60e2                	ld	ra,24(sp)
 5c4:	6442                	ld	s0,16(sp)
 5c6:	6105                	addi	sp,sp,32
 5c8:	8082                	ret

00000000000005ca <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 5ca:	7139                	addi	sp,sp,-64
 5cc:	fc06                	sd	ra,56(sp)
 5ce:	f822                	sd	s0,48(sp)
 5d0:	f426                	sd	s1,40(sp)
 5d2:	f04a                	sd	s2,32(sp)
 5d4:	ec4e                	sd	s3,24(sp)
 5d6:	0080                	addi	s0,sp,64
 5d8:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 5da:	c299                	beqz	a3,5e0 <printint+0x16>
 5dc:	0805c863          	bltz	a1,66c <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 5e0:	2581                	sext.w	a1,a1
  neg = 0;
 5e2:	4881                	li	a7,0
  }

  i = 0;
 5e4:	fc040993          	addi	s3,s0,-64
  neg = 0;
 5e8:	86ce                	mv	a3,s3
  i = 0;
 5ea:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 5ec:	2601                	sext.w	a2,a2
 5ee:	00000517          	auipc	a0,0x0
 5f2:	51a50513          	addi	a0,a0,1306 # b08 <digits>
 5f6:	883a                	mv	a6,a4
 5f8:	2705                	addiw	a4,a4,1
 5fa:	02c5f7bb          	remuw	a5,a1,a2
 5fe:	1782                	slli	a5,a5,0x20
 600:	9381                	srli	a5,a5,0x20
 602:	97aa                	add	a5,a5,a0
 604:	0007c783          	lbu	a5,0(a5)
 608:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 60c:	0005879b          	sext.w	a5,a1
 610:	02c5d5bb          	divuw	a1,a1,a2
 614:	0685                	addi	a3,a3,1
 616:	fec7f0e3          	bgeu	a5,a2,5f6 <printint+0x2c>
  if(neg)
 61a:	00088c63          	beqz	a7,632 <printint+0x68>
    buf[i++] = '-';
 61e:	fd070793          	addi	a5,a4,-48
 622:	00878733          	add	a4,a5,s0
 626:	02d00793          	li	a5,45
 62a:	fef70823          	sb	a5,-16(a4)
 62e:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 632:	02e05663          	blez	a4,65e <printint+0x94>
 636:	fc040913          	addi	s2,s0,-64
 63a:	993a                	add	s2,s2,a4
 63c:	19fd                	addi	s3,s3,-1
 63e:	99ba                	add	s3,s3,a4
 640:	377d                	addiw	a4,a4,-1
 642:	1702                	slli	a4,a4,0x20
 644:	9301                	srli	a4,a4,0x20
 646:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 64a:	fff94583          	lbu	a1,-1(s2)
 64e:	8526                	mv	a0,s1
 650:	00000097          	auipc	ra,0x0
 654:	f58080e7          	jalr	-168(ra) # 5a8 <putc>
  while(--i >= 0)
 658:	197d                	addi	s2,s2,-1
 65a:	ff3918e3          	bne	s2,s3,64a <printint+0x80>
}
 65e:	70e2                	ld	ra,56(sp)
 660:	7442                	ld	s0,48(sp)
 662:	74a2                	ld	s1,40(sp)
 664:	7902                	ld	s2,32(sp)
 666:	69e2                	ld	s3,24(sp)
 668:	6121                	addi	sp,sp,64
 66a:	8082                	ret
    x = -xx;
 66c:	40b005bb          	negw	a1,a1
    neg = 1;
 670:	4885                	li	a7,1
    x = -xx;
 672:	bf8d                	j	5e4 <printint+0x1a>

0000000000000674 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 674:	7119                	addi	sp,sp,-128
 676:	fc86                	sd	ra,120(sp)
 678:	f8a2                	sd	s0,112(sp)
 67a:	f4a6                	sd	s1,104(sp)
 67c:	f0ca                	sd	s2,96(sp)
 67e:	ecce                	sd	s3,88(sp)
 680:	e8d2                	sd	s4,80(sp)
 682:	e4d6                	sd	s5,72(sp)
 684:	e0da                	sd	s6,64(sp)
 686:	fc5e                	sd	s7,56(sp)
 688:	f862                	sd	s8,48(sp)
 68a:	f466                	sd	s9,40(sp)
 68c:	f06a                	sd	s10,32(sp)
 68e:	ec6e                	sd	s11,24(sp)
 690:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 692:	0005c903          	lbu	s2,0(a1)
 696:	18090f63          	beqz	s2,834 <vprintf+0x1c0>
 69a:	8aaa                	mv	s5,a0
 69c:	8b32                	mv	s6,a2
 69e:	00158493          	addi	s1,a1,1
  state = 0;
 6a2:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 6a4:	02500a13          	li	s4,37
 6a8:	4c55                	li	s8,21
 6aa:	00000c97          	auipc	s9,0x0
 6ae:	406c8c93          	addi	s9,s9,1030 # ab0 <malloc+0x178>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 6b2:	02800d93          	li	s11,40
  putc(fd, 'x');
 6b6:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 6b8:	00000b97          	auipc	s7,0x0
 6bc:	450b8b93          	addi	s7,s7,1104 # b08 <digits>
 6c0:	a839                	j	6de <vprintf+0x6a>
        putc(fd, c);
 6c2:	85ca                	mv	a1,s2
 6c4:	8556                	mv	a0,s5
 6c6:	00000097          	auipc	ra,0x0
 6ca:	ee2080e7          	jalr	-286(ra) # 5a8 <putc>
 6ce:	a019                	j	6d4 <vprintf+0x60>
    } else if(state == '%'){
 6d0:	01498d63          	beq	s3,s4,6ea <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 6d4:	0485                	addi	s1,s1,1
 6d6:	fff4c903          	lbu	s2,-1(s1)
 6da:	14090d63          	beqz	s2,834 <vprintf+0x1c0>
    if(state == 0){
 6de:	fe0999e3          	bnez	s3,6d0 <vprintf+0x5c>
      if(c == '%'){
 6e2:	ff4910e3          	bne	s2,s4,6c2 <vprintf+0x4e>
        state = '%';
 6e6:	89d2                	mv	s3,s4
 6e8:	b7f5                	j	6d4 <vprintf+0x60>
      if(c == 'd'){
 6ea:	11490c63          	beq	s2,s4,802 <vprintf+0x18e>
 6ee:	f9d9079b          	addiw	a5,s2,-99
 6f2:	0ff7f793          	zext.b	a5,a5
 6f6:	10fc6e63          	bltu	s8,a5,812 <vprintf+0x19e>
 6fa:	f9d9079b          	addiw	a5,s2,-99
 6fe:	0ff7f713          	zext.b	a4,a5
 702:	10ec6863          	bltu	s8,a4,812 <vprintf+0x19e>
 706:	00271793          	slli	a5,a4,0x2
 70a:	97e6                	add	a5,a5,s9
 70c:	439c                	lw	a5,0(a5)
 70e:	97e6                	add	a5,a5,s9
 710:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 712:	008b0913          	addi	s2,s6,8
 716:	4685                	li	a3,1
 718:	4629                	li	a2,10
 71a:	000b2583          	lw	a1,0(s6)
 71e:	8556                	mv	a0,s5
 720:	00000097          	auipc	ra,0x0
 724:	eaa080e7          	jalr	-342(ra) # 5ca <printint>
 728:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 72a:	4981                	li	s3,0
 72c:	b765                	j	6d4 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 72e:	008b0913          	addi	s2,s6,8
 732:	4681                	li	a3,0
 734:	4629                	li	a2,10
 736:	000b2583          	lw	a1,0(s6)
 73a:	8556                	mv	a0,s5
 73c:	00000097          	auipc	ra,0x0
 740:	e8e080e7          	jalr	-370(ra) # 5ca <printint>
 744:	8b4a                	mv	s6,s2
      state = 0;
 746:	4981                	li	s3,0
 748:	b771                	j	6d4 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 74a:	008b0913          	addi	s2,s6,8
 74e:	4681                	li	a3,0
 750:	866a                	mv	a2,s10
 752:	000b2583          	lw	a1,0(s6)
 756:	8556                	mv	a0,s5
 758:	00000097          	auipc	ra,0x0
 75c:	e72080e7          	jalr	-398(ra) # 5ca <printint>
 760:	8b4a                	mv	s6,s2
      state = 0;
 762:	4981                	li	s3,0
 764:	bf85                	j	6d4 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 766:	008b0793          	addi	a5,s6,8
 76a:	f8f43423          	sd	a5,-120(s0)
 76e:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 772:	03000593          	li	a1,48
 776:	8556                	mv	a0,s5
 778:	00000097          	auipc	ra,0x0
 77c:	e30080e7          	jalr	-464(ra) # 5a8 <putc>
  putc(fd, 'x');
 780:	07800593          	li	a1,120
 784:	8556                	mv	a0,s5
 786:	00000097          	auipc	ra,0x0
 78a:	e22080e7          	jalr	-478(ra) # 5a8 <putc>
 78e:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 790:	03c9d793          	srli	a5,s3,0x3c
 794:	97de                	add	a5,a5,s7
 796:	0007c583          	lbu	a1,0(a5)
 79a:	8556                	mv	a0,s5
 79c:	00000097          	auipc	ra,0x0
 7a0:	e0c080e7          	jalr	-500(ra) # 5a8 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 7a4:	0992                	slli	s3,s3,0x4
 7a6:	397d                	addiw	s2,s2,-1
 7a8:	fe0914e3          	bnez	s2,790 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 7ac:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 7b0:	4981                	li	s3,0
 7b2:	b70d                	j	6d4 <vprintf+0x60>
        s = va_arg(ap, char*);
 7b4:	008b0913          	addi	s2,s6,8
 7b8:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 7bc:	02098163          	beqz	s3,7de <vprintf+0x16a>
        while(*s != 0){
 7c0:	0009c583          	lbu	a1,0(s3)
 7c4:	c5ad                	beqz	a1,82e <vprintf+0x1ba>
          putc(fd, *s);
 7c6:	8556                	mv	a0,s5
 7c8:	00000097          	auipc	ra,0x0
 7cc:	de0080e7          	jalr	-544(ra) # 5a8 <putc>
          s++;
 7d0:	0985                	addi	s3,s3,1
        while(*s != 0){
 7d2:	0009c583          	lbu	a1,0(s3)
 7d6:	f9e5                	bnez	a1,7c6 <vprintf+0x152>
        s = va_arg(ap, char*);
 7d8:	8b4a                	mv	s6,s2
      state = 0;
 7da:	4981                	li	s3,0
 7dc:	bde5                	j	6d4 <vprintf+0x60>
          s = "(null)";
 7de:	00000997          	auipc	s3,0x0
 7e2:	2ca98993          	addi	s3,s3,714 # aa8 <malloc+0x170>
        while(*s != 0){
 7e6:	85ee                	mv	a1,s11
 7e8:	bff9                	j	7c6 <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 7ea:	008b0913          	addi	s2,s6,8
 7ee:	000b4583          	lbu	a1,0(s6)
 7f2:	8556                	mv	a0,s5
 7f4:	00000097          	auipc	ra,0x0
 7f8:	db4080e7          	jalr	-588(ra) # 5a8 <putc>
 7fc:	8b4a                	mv	s6,s2
      state = 0;
 7fe:	4981                	li	s3,0
 800:	bdd1                	j	6d4 <vprintf+0x60>
        putc(fd, c);
 802:	85d2                	mv	a1,s4
 804:	8556                	mv	a0,s5
 806:	00000097          	auipc	ra,0x0
 80a:	da2080e7          	jalr	-606(ra) # 5a8 <putc>
      state = 0;
 80e:	4981                	li	s3,0
 810:	b5d1                	j	6d4 <vprintf+0x60>
        putc(fd, '%');
 812:	85d2                	mv	a1,s4
 814:	8556                	mv	a0,s5
 816:	00000097          	auipc	ra,0x0
 81a:	d92080e7          	jalr	-622(ra) # 5a8 <putc>
        putc(fd, c);
 81e:	85ca                	mv	a1,s2
 820:	8556                	mv	a0,s5
 822:	00000097          	auipc	ra,0x0
 826:	d86080e7          	jalr	-634(ra) # 5a8 <putc>
      state = 0;
 82a:	4981                	li	s3,0
 82c:	b565                	j	6d4 <vprintf+0x60>
        s = va_arg(ap, char*);
 82e:	8b4a                	mv	s6,s2
      state = 0;
 830:	4981                	li	s3,0
 832:	b54d                	j	6d4 <vprintf+0x60>
    }
  }
}
 834:	70e6                	ld	ra,120(sp)
 836:	7446                	ld	s0,112(sp)
 838:	74a6                	ld	s1,104(sp)
 83a:	7906                	ld	s2,96(sp)
 83c:	69e6                	ld	s3,88(sp)
 83e:	6a46                	ld	s4,80(sp)
 840:	6aa6                	ld	s5,72(sp)
 842:	6b06                	ld	s6,64(sp)
 844:	7be2                	ld	s7,56(sp)
 846:	7c42                	ld	s8,48(sp)
 848:	7ca2                	ld	s9,40(sp)
 84a:	7d02                	ld	s10,32(sp)
 84c:	6de2                	ld	s11,24(sp)
 84e:	6109                	addi	sp,sp,128
 850:	8082                	ret

0000000000000852 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 852:	715d                	addi	sp,sp,-80
 854:	ec06                	sd	ra,24(sp)
 856:	e822                	sd	s0,16(sp)
 858:	1000                	addi	s0,sp,32
 85a:	e010                	sd	a2,0(s0)
 85c:	e414                	sd	a3,8(s0)
 85e:	e818                	sd	a4,16(s0)
 860:	ec1c                	sd	a5,24(s0)
 862:	03043023          	sd	a6,32(s0)
 866:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 86a:	8622                	mv	a2,s0
 86c:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 870:	00000097          	auipc	ra,0x0
 874:	e04080e7          	jalr	-508(ra) # 674 <vprintf>
}
 878:	60e2                	ld	ra,24(sp)
 87a:	6442                	ld	s0,16(sp)
 87c:	6161                	addi	sp,sp,80
 87e:	8082                	ret

0000000000000880 <printf>:

void
printf(const char *fmt, ...)
{
 880:	711d                	addi	sp,sp,-96
 882:	ec06                	sd	ra,24(sp)
 884:	e822                	sd	s0,16(sp)
 886:	1000                	addi	s0,sp,32
 888:	e40c                	sd	a1,8(s0)
 88a:	e810                	sd	a2,16(s0)
 88c:	ec14                	sd	a3,24(s0)
 88e:	f018                	sd	a4,32(s0)
 890:	f41c                	sd	a5,40(s0)
 892:	03043823          	sd	a6,48(s0)
 896:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 89a:	00840613          	addi	a2,s0,8
 89e:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 8a2:	85aa                	mv	a1,a0
 8a4:	4505                	li	a0,1
 8a6:	00000097          	auipc	ra,0x0
 8aa:	dce080e7          	jalr	-562(ra) # 674 <vprintf>
}
 8ae:	60e2                	ld	ra,24(sp)
 8b0:	6442                	ld	s0,16(sp)
 8b2:	6125                	addi	sp,sp,96
 8b4:	8082                	ret

00000000000008b6 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 8b6:	1141                	addi	sp,sp,-16
 8b8:	e422                	sd	s0,8(sp)
 8ba:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 8bc:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8c0:	00000797          	auipc	a5,0x0
 8c4:	4607b783          	ld	a5,1120(a5) # d20 <freep>
 8c8:	a02d                	j	8f2 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 8ca:	4618                	lw	a4,8(a2)
 8cc:	9f2d                	addw	a4,a4,a1
 8ce:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 8d2:	6398                	ld	a4,0(a5)
 8d4:	6310                	ld	a2,0(a4)
 8d6:	a83d                	j	914 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 8d8:	ff852703          	lw	a4,-8(a0)
 8dc:	9f31                	addw	a4,a4,a2
 8de:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 8e0:	ff053683          	ld	a3,-16(a0)
 8e4:	a091                	j	928 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8e6:	6398                	ld	a4,0(a5)
 8e8:	00e7e463          	bltu	a5,a4,8f0 <free+0x3a>
 8ec:	00e6ea63          	bltu	a3,a4,900 <free+0x4a>
{
 8f0:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8f2:	fed7fae3          	bgeu	a5,a3,8e6 <free+0x30>
 8f6:	6398                	ld	a4,0(a5)
 8f8:	00e6e463          	bltu	a3,a4,900 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8fc:	fee7eae3          	bltu	a5,a4,8f0 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 900:	ff852583          	lw	a1,-8(a0)
 904:	6390                	ld	a2,0(a5)
 906:	02059813          	slli	a6,a1,0x20
 90a:	01c85713          	srli	a4,a6,0x1c
 90e:	9736                	add	a4,a4,a3
 910:	fae60de3          	beq	a2,a4,8ca <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 914:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 918:	4790                	lw	a2,8(a5)
 91a:	02061593          	slli	a1,a2,0x20
 91e:	01c5d713          	srli	a4,a1,0x1c
 922:	973e                	add	a4,a4,a5
 924:	fae68ae3          	beq	a3,a4,8d8 <free+0x22>
    p->s.ptr = bp->s.ptr;
 928:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 92a:	00000717          	auipc	a4,0x0
 92e:	3ef73b23          	sd	a5,1014(a4) # d20 <freep>
}
 932:	6422                	ld	s0,8(sp)
 934:	0141                	addi	sp,sp,16
 936:	8082                	ret

0000000000000938 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 938:	7139                	addi	sp,sp,-64
 93a:	fc06                	sd	ra,56(sp)
 93c:	f822                	sd	s0,48(sp)
 93e:	f426                	sd	s1,40(sp)
 940:	f04a                	sd	s2,32(sp)
 942:	ec4e                	sd	s3,24(sp)
 944:	e852                	sd	s4,16(sp)
 946:	e456                	sd	s5,8(sp)
 948:	e05a                	sd	s6,0(sp)
 94a:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 94c:	02051493          	slli	s1,a0,0x20
 950:	9081                	srli	s1,s1,0x20
 952:	04bd                	addi	s1,s1,15
 954:	8091                	srli	s1,s1,0x4
 956:	00148a1b          	addiw	s4,s1,1
 95a:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 95c:	00000517          	auipc	a0,0x0
 960:	3c453503          	ld	a0,964(a0) # d20 <freep>
 964:	c515                	beqz	a0,990 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 966:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 968:	4798                	lw	a4,8(a5)
 96a:	04977163          	bgeu	a4,s1,9ac <malloc+0x74>
 96e:	89d2                	mv	s3,s4
 970:	000a071b          	sext.w	a4,s4
 974:	6685                	lui	a3,0x1
 976:	00d77363          	bgeu	a4,a3,97c <malloc+0x44>
 97a:	6985                	lui	s3,0x1
 97c:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 980:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 984:	00000917          	auipc	s2,0x0
 988:	39c90913          	addi	s2,s2,924 # d20 <freep>
  if(p == (char*)-1)
 98c:	5afd                	li	s5,-1
 98e:	a8a5                	j	a06 <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 990:	00000797          	auipc	a5,0x0
 994:	39078793          	addi	a5,a5,912 # d20 <freep>
 998:	00000717          	auipc	a4,0x0
 99c:	39070713          	addi	a4,a4,912 # d28 <base>
 9a0:	e398                	sd	a4,0(a5)
 9a2:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 9a4:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9a8:	87ba                	mv	a5,a4
 9aa:	b7d1                	j	96e <malloc+0x36>
      if(p->s.size == nunits)
 9ac:	02e48c63          	beq	s1,a4,9e4 <malloc+0xac>
        p->s.size -= nunits;
 9b0:	4147073b          	subw	a4,a4,s4
 9b4:	c798                	sw	a4,8(a5)
        p += p->s.size;
 9b6:	02071693          	slli	a3,a4,0x20
 9ba:	01c6d713          	srli	a4,a3,0x1c
 9be:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 9c0:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 9c4:	00000717          	auipc	a4,0x0
 9c8:	34a73e23          	sd	a0,860(a4) # d20 <freep>
      return (void*)(p + 1);
 9cc:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 9d0:	70e2                	ld	ra,56(sp)
 9d2:	7442                	ld	s0,48(sp)
 9d4:	74a2                	ld	s1,40(sp)
 9d6:	7902                	ld	s2,32(sp)
 9d8:	69e2                	ld	s3,24(sp)
 9da:	6a42                	ld	s4,16(sp)
 9dc:	6aa2                	ld	s5,8(sp)
 9de:	6b02                	ld	s6,0(sp)
 9e0:	6121                	addi	sp,sp,64
 9e2:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 9e4:	6398                	ld	a4,0(a5)
 9e6:	e118                	sd	a4,0(a0)
 9e8:	bff1                	j	9c4 <malloc+0x8c>
  hp->s.size = nu;
 9ea:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 9ee:	0541                	addi	a0,a0,16
 9f0:	00000097          	auipc	ra,0x0
 9f4:	ec6080e7          	jalr	-314(ra) # 8b6 <free>
  return freep;
 9f8:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 9fc:	d971                	beqz	a0,9d0 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9fe:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 a00:	4798                	lw	a4,8(a5)
 a02:	fa9775e3          	bgeu	a4,s1,9ac <malloc+0x74>
    if(p == freep)
 a06:	00093703          	ld	a4,0(s2)
 a0a:	853e                	mv	a0,a5
 a0c:	fef719e3          	bne	a4,a5,9fe <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 a10:	854e                	mv	a0,s3
 a12:	00000097          	auipc	ra,0x0
 a16:	b34080e7          	jalr	-1228(ra) # 546 <sbrk>
  if(p == (char*)-1)
 a1a:	fd5518e3          	bne	a0,s5,9ea <malloc+0xb2>
        return 0;
 a1e:	4501                	li	a0,0
 a20:	bf45                	j	9d0 <malloc+0x98>
