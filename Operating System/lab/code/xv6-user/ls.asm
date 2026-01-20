
xv6-user/_ls:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <fmtname>:
#include "kernel/include/stat.h"
#include "xv6-user/user.h"

char*
fmtname(char *name)
{
   0:	7179                	addi	sp,sp,-48
   2:	f406                	sd	ra,40(sp)
   4:	f022                	sd	s0,32(sp)
   6:	ec26                	sd	s1,24(sp)
   8:	e84a                	sd	s2,16(sp)
   a:	e44e                	sd	s3,8(sp)
   c:	1800                	addi	s0,sp,48
   e:	892a                	mv	s2,a0
  static char buf[STAT_MAX_NAME+1];
  int len = strlen(name);
  10:	00000097          	auipc	ra,0x0
  14:	24c080e7          	jalr	588(ra) # 25c <strlen>
  18:	0005049b          	sext.w	s1,a0

  // Return blank-padded name.
  if(len >= STAT_MAX_NAME)
  1c:	47fd                	li	a5,31
  1e:	0097da63          	bge	a5,s1,32 <fmtname+0x32>
    return name;
  memmove(buf, name, len);
  memset(buf + len, ' ', STAT_MAX_NAME - len);
  buf[STAT_MAX_NAME] = '\0';
  return buf;
}
  22:	854a                	mv	a0,s2
  24:	70a2                	ld	ra,40(sp)
  26:	7402                	ld	s0,32(sp)
  28:	64e2                	ld	s1,24(sp)
  2a:	6942                	ld	s2,16(sp)
  2c:	69a2                	ld	s3,8(sp)
  2e:	6145                	addi	sp,sp,48
  30:	8082                	ret
  memmove(buf, name, len);
  32:	00001997          	auipc	s3,0x1
  36:	ab698993          	addi	s3,s3,-1354 # ae8 <buf.0>
  3a:	8626                	mv	a2,s1
  3c:	85ca                	mv	a1,s2
  3e:	854e                	mv	a0,s3
  40:	00000097          	auipc	ra,0x0
  44:	3ac080e7          	jalr	940(ra) # 3ec <memmove>
  memset(buf + len, ' ', STAT_MAX_NAME - len);
  48:	02000613          	li	a2,32
  4c:	9e05                	subw	a2,a2,s1
  4e:	02000593          	li	a1,32
  52:	00998533          	add	a0,s3,s1
  56:	00000097          	auipc	ra,0x0
  5a:	230080e7          	jalr	560(ra) # 286 <memset>
  buf[STAT_MAX_NAME] = '\0';
  5e:	02098023          	sb	zero,32(s3)
  return buf;
  62:	894e                	mv	s2,s3
  64:	bf7d                	j	22 <fmtname+0x22>

0000000000000066 <ls>:

void
ls(char *path)
{
  66:	7119                	addi	sp,sp,-128
  68:	fc86                	sd	ra,120(sp)
  6a:	f8a2                	sd	s0,112(sp)
  6c:	f4a6                	sd	s1,104(sp)
  6e:	f0ca                	sd	s2,96(sp)
  70:	ecce                	sd	s3,88(sp)
  72:	e8d2                	sd	s4,80(sp)
  74:	0100                	addi	s0,sp,128
  76:	892a                	mv	s2,a0
  int fd;
  struct stat st;
  char *types[] = {
  78:	f8043023          	sd	zero,-128(s0)
  7c:	00001797          	auipc	a5,0x1
  80:	98c78793          	addi	a5,a5,-1652 # a08 <malloc+0xf0>
  84:	f8f43423          	sd	a5,-120(s0)
  88:	00001797          	auipc	a5,0x1
  8c:	98878793          	addi	a5,a5,-1656 # a10 <malloc+0xf8>
  90:	f8f43823          	sd	a5,-112(s0)
    [T_DIR]   "DIR ",
    [T_FILE]  "FILE",
  };

  if((fd = open(path, 0)) < 0){
  94:	4581                	li	a1,0
  96:	00000097          	auipc	ra,0x0
  9a:	456080e7          	jalr	1110(ra) # 4ec <open>
  9e:	02054863          	bltz	a0,ce <ls+0x68>
  a2:	84aa                	mv	s1,a0
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
  a4:	f9840593          	addi	a1,s0,-104
  a8:	00000097          	auipc	ra,0x0
  ac:	44e080e7          	jalr	1102(ra) # 4f6 <fstat>
  b0:	02054a63          	bltz	a0,e4 <ls+0x7e>
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR){
  b4:	fc041703          	lh	a4,-64(s0)
  b8:	4785                	li	a5,1
  ba:	08f71f63          	bne	a4,a5,158 <ls+0xf2>
    while(readdir(fd, &st) == 1){
  be:	f9840913          	addi	s2,s0,-104
  c2:	4985                	li	s3,1
      printf("%s %s\t%d\n", fmtname(st.name), types[st.type], st.size);
  c4:	00001a17          	auipc	s4,0x1
  c8:	984a0a13          	addi	s4,s4,-1660 # a48 <malloc+0x130>
  cc:	a08d                	j	12e <ls+0xc8>
    fprintf(2, "ls: cannot open %s\n", path);
  ce:	864a                	mv	a2,s2
  d0:	00001597          	auipc	a1,0x1
  d4:	94858593          	addi	a1,a1,-1720 # a18 <malloc+0x100>
  d8:	4509                	li	a0,2
  da:	00000097          	auipc	ra,0x0
  de:	758080e7          	jalr	1880(ra) # 832 <fprintf>
    return;
  e2:	a09d                	j	148 <ls+0xe2>
    fprintf(2, "ls: cannot stat %s\n", path);
  e4:	864a                	mv	a2,s2
  e6:	00001597          	auipc	a1,0x1
  ea:	94a58593          	addi	a1,a1,-1718 # a30 <malloc+0x118>
  ee:	4509                	li	a0,2
  f0:	00000097          	auipc	ra,0x0
  f4:	742080e7          	jalr	1858(ra) # 832 <fprintf>
    close(fd);
  f8:	8526                	mv	a0,s1
  fa:	00000097          	auipc	ra,0x0
  fe:	3d6080e7          	jalr	982(ra) # 4d0 <close>
    return;
 102:	a099                	j	148 <ls+0xe2>
      printf("%s %s\t%d\n", fmtname(st.name), types[st.type], st.size);
 104:	854a                	mv	a0,s2
 106:	00000097          	auipc	ra,0x0
 10a:	efa080e7          	jalr	-262(ra) # 0 <fmtname>
 10e:	85aa                	mv	a1,a0
 110:	fc041783          	lh	a5,-64(s0)
 114:	078e                	slli	a5,a5,0x3
 116:	fd078793          	addi	a5,a5,-48
 11a:	97a2                	add	a5,a5,s0
 11c:	fc843683          	ld	a3,-56(s0)
 120:	fb07b603          	ld	a2,-80(a5)
 124:	8552                	mv	a0,s4
 126:	00000097          	auipc	ra,0x0
 12a:	73a080e7          	jalr	1850(ra) # 860 <printf>
    while(readdir(fd, &st) == 1){
 12e:	85ca                	mv	a1,s2
 130:	8526                	mv	a0,s1
 132:	00000097          	auipc	ra,0x0
 136:	424080e7          	jalr	1060(ra) # 556 <readdir>
 13a:	fd3505e3          	beq	a0,s3,104 <ls+0x9e>
    }
  } else {
    printf("%s %s\t%l\n", fmtname(st.name), types[st.type], st.size);
  }
  close(fd);
 13e:	8526                	mv	a0,s1
 140:	00000097          	auipc	ra,0x0
 144:	390080e7          	jalr	912(ra) # 4d0 <close>
}
 148:	70e6                	ld	ra,120(sp)
 14a:	7446                	ld	s0,112(sp)
 14c:	74a6                	ld	s1,104(sp)
 14e:	7906                	ld	s2,96(sp)
 150:	69e6                	ld	s3,88(sp)
 152:	6a46                	ld	s4,80(sp)
 154:	6109                	addi	sp,sp,128
 156:	8082                	ret
    printf("%s %s\t%l\n", fmtname(st.name), types[st.type], st.size);
 158:	f9840513          	addi	a0,s0,-104
 15c:	00000097          	auipc	ra,0x0
 160:	ea4080e7          	jalr	-348(ra) # 0 <fmtname>
 164:	85aa                	mv	a1,a0
 166:	fc041783          	lh	a5,-64(s0)
 16a:	078e                	slli	a5,a5,0x3
 16c:	fd078793          	addi	a5,a5,-48
 170:	97a2                	add	a5,a5,s0
 172:	fc843683          	ld	a3,-56(s0)
 176:	fb07b603          	ld	a2,-80(a5)
 17a:	00001517          	auipc	a0,0x1
 17e:	8de50513          	addi	a0,a0,-1826 # a58 <malloc+0x140>
 182:	00000097          	auipc	ra,0x0
 186:	6de080e7          	jalr	1758(ra) # 860 <printf>
 18a:	bf55                	j	13e <ls+0xd8>

000000000000018c <main>:

int
main(int argc, char *argv[])
{
 18c:	1101                	addi	sp,sp,-32
 18e:	ec06                	sd	ra,24(sp)
 190:	e822                	sd	s0,16(sp)
 192:	e426                	sd	s1,8(sp)
 194:	e04a                	sd	s2,0(sp)
 196:	1000                	addi	s0,sp,32
  int i;

  if(argc < 2){
 198:	4785                	li	a5,1
 19a:	02a7d963          	bge	a5,a0,1cc <main+0x40>
 19e:	00858493          	addi	s1,a1,8
 1a2:	ffe5091b          	addiw	s2,a0,-2
 1a6:	02091793          	slli	a5,s2,0x20
 1aa:	01d7d913          	srli	s2,a5,0x1d
 1ae:	05c1                	addi	a1,a1,16
 1b0:	992e                	add	s2,s2,a1
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
 1b2:	6088                	ld	a0,0(s1)
 1b4:	00000097          	auipc	ra,0x0
 1b8:	eb2080e7          	jalr	-334(ra) # 66 <ls>
  for(i=1; i<argc; i++)
 1bc:	04a1                	addi	s1,s1,8
 1be:	ff249ae3          	bne	s1,s2,1b2 <main+0x26>
  exit(0);
 1c2:	4501                	li	a0,0
 1c4:	00000097          	auipc	ra,0x0
 1c8:	2da080e7          	jalr	730(ra) # 49e <exit>
    ls(".");
 1cc:	00001517          	auipc	a0,0x1
 1d0:	89c50513          	addi	a0,a0,-1892 # a68 <malloc+0x150>
 1d4:	00000097          	auipc	ra,0x0
 1d8:	e92080e7          	jalr	-366(ra) # 66 <ls>
    exit(0);
 1dc:	4501                	li	a0,0
 1de:	00000097          	auipc	ra,0x0
 1e2:	2c0080e7          	jalr	704(ra) # 49e <exit>

00000000000001e6 <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
 1e6:	1141                	addi	sp,sp,-16
 1e8:	e422                	sd	s0,8(sp)
 1ea:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 1ec:	87aa                	mv	a5,a0
 1ee:	0585                	addi	a1,a1,1
 1f0:	0785                	addi	a5,a5,1
 1f2:	fff5c703          	lbu	a4,-1(a1)
 1f6:	fee78fa3          	sb	a4,-1(a5)
 1fa:	fb75                	bnez	a4,1ee <strcpy+0x8>
    ;
  return os;
}
 1fc:	6422                	ld	s0,8(sp)
 1fe:	0141                	addi	sp,sp,16
 200:	8082                	ret

0000000000000202 <strcat>:

char*
strcat(char *s, const char *t)
{
 202:	1141                	addi	sp,sp,-16
 204:	e422                	sd	s0,8(sp)
 206:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 208:	00054783          	lbu	a5,0(a0)
 20c:	c385                	beqz	a5,22c <strcat+0x2a>
 20e:	87aa                	mv	a5,a0
    s++;
 210:	0785                	addi	a5,a5,1
  while(*s)
 212:	0007c703          	lbu	a4,0(a5)
 216:	ff6d                	bnez	a4,210 <strcat+0xe>
  while((*s++ = *t++))
 218:	0585                	addi	a1,a1,1
 21a:	0785                	addi	a5,a5,1
 21c:	fff5c703          	lbu	a4,-1(a1)
 220:	fee78fa3          	sb	a4,-1(a5)
 224:	fb75                	bnez	a4,218 <strcat+0x16>
    ;
  return os;
}
 226:	6422                	ld	s0,8(sp)
 228:	0141                	addi	sp,sp,16
 22a:	8082                	ret
  while(*s)
 22c:	87aa                	mv	a5,a0
 22e:	b7ed                	j	218 <strcat+0x16>

0000000000000230 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 230:	1141                	addi	sp,sp,-16
 232:	e422                	sd	s0,8(sp)
 234:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 236:	00054783          	lbu	a5,0(a0)
 23a:	cb91                	beqz	a5,24e <strcmp+0x1e>
 23c:	0005c703          	lbu	a4,0(a1)
 240:	00f71763          	bne	a4,a5,24e <strcmp+0x1e>
    p++, q++;
 244:	0505                	addi	a0,a0,1
 246:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 248:	00054783          	lbu	a5,0(a0)
 24c:	fbe5                	bnez	a5,23c <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 24e:	0005c503          	lbu	a0,0(a1)
}
 252:	40a7853b          	subw	a0,a5,a0
 256:	6422                	ld	s0,8(sp)
 258:	0141                	addi	sp,sp,16
 25a:	8082                	ret

000000000000025c <strlen>:

uint
strlen(const char *s)
{
 25c:	1141                	addi	sp,sp,-16
 25e:	e422                	sd	s0,8(sp)
 260:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 262:	00054783          	lbu	a5,0(a0)
 266:	cf91                	beqz	a5,282 <strlen+0x26>
 268:	0505                	addi	a0,a0,1
 26a:	87aa                	mv	a5,a0
 26c:	4685                	li	a3,1
 26e:	9e89                	subw	a3,a3,a0
 270:	00f6853b          	addw	a0,a3,a5
 274:	0785                	addi	a5,a5,1
 276:	fff7c703          	lbu	a4,-1(a5)
 27a:	fb7d                	bnez	a4,270 <strlen+0x14>
    ;
  return n;
}
 27c:	6422                	ld	s0,8(sp)
 27e:	0141                	addi	sp,sp,16
 280:	8082                	ret
  for(n = 0; s[n]; n++)
 282:	4501                	li	a0,0
 284:	bfe5                	j	27c <strlen+0x20>

0000000000000286 <memset>:

void*
memset(void *dst, int c, uint n)
{
 286:	1141                	addi	sp,sp,-16
 288:	e422                	sd	s0,8(sp)
 28a:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 28c:	ca19                	beqz	a2,2a2 <memset+0x1c>
 28e:	87aa                	mv	a5,a0
 290:	1602                	slli	a2,a2,0x20
 292:	9201                	srli	a2,a2,0x20
 294:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 298:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 29c:	0785                	addi	a5,a5,1
 29e:	fee79de3          	bne	a5,a4,298 <memset+0x12>
  }
  return dst;
}
 2a2:	6422                	ld	s0,8(sp)
 2a4:	0141                	addi	sp,sp,16
 2a6:	8082                	ret

00000000000002a8 <strchr>:

char*
strchr(const char *s, char c)
{
 2a8:	1141                	addi	sp,sp,-16
 2aa:	e422                	sd	s0,8(sp)
 2ac:	0800                	addi	s0,sp,16
  for(; *s; s++)
 2ae:	00054783          	lbu	a5,0(a0)
 2b2:	cb99                	beqz	a5,2c8 <strchr+0x20>
    if(*s == c)
 2b4:	00f58763          	beq	a1,a5,2c2 <strchr+0x1a>
  for(; *s; s++)
 2b8:	0505                	addi	a0,a0,1
 2ba:	00054783          	lbu	a5,0(a0)
 2be:	fbfd                	bnez	a5,2b4 <strchr+0xc>
      return (char*)s;
  return 0;
 2c0:	4501                	li	a0,0
}
 2c2:	6422                	ld	s0,8(sp)
 2c4:	0141                	addi	sp,sp,16
 2c6:	8082                	ret
  return 0;
 2c8:	4501                	li	a0,0
 2ca:	bfe5                	j	2c2 <strchr+0x1a>

00000000000002cc <gets>:

char*
gets(char *buf, int max)
{
 2cc:	711d                	addi	sp,sp,-96
 2ce:	ec86                	sd	ra,88(sp)
 2d0:	e8a2                	sd	s0,80(sp)
 2d2:	e4a6                	sd	s1,72(sp)
 2d4:	e0ca                	sd	s2,64(sp)
 2d6:	fc4e                	sd	s3,56(sp)
 2d8:	f852                	sd	s4,48(sp)
 2da:	f456                	sd	s5,40(sp)
 2dc:	f05a                	sd	s6,32(sp)
 2de:	ec5e                	sd	s7,24(sp)
 2e0:	e862                	sd	s8,16(sp)
 2e2:	1080                	addi	s0,sp,96
 2e4:	8baa                	mv	s7,a0
 2e6:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 2e8:	892a                	mv	s2,a0
 2ea:	4481                	li	s1,0
    cc = read(0, &c, 1);
 2ec:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 2f0:	4b29                	li	s6,10
 2f2:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 2f4:	89a6                	mv	s3,s1
 2f6:	2485                	addiw	s1,s1,1
 2f8:	0344d763          	bge	s1,s4,326 <gets+0x5a>
    cc = read(0, &c, 1);
 2fc:	4605                	li	a2,1
 2fe:	85d6                	mv	a1,s5
 300:	4501                	li	a0,0
 302:	00000097          	auipc	ra,0x0
 306:	1ba080e7          	jalr	442(ra) # 4bc <read>
    if(cc < 1)
 30a:	00a05e63          	blez	a0,326 <gets+0x5a>
    buf[i++] = c;
 30e:	faf44783          	lbu	a5,-81(s0)
 312:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 316:	01678763          	beq	a5,s6,324 <gets+0x58>
 31a:	0905                	addi	s2,s2,1
 31c:	fd879ce3          	bne	a5,s8,2f4 <gets+0x28>
  for(i=0; i+1 < max; ){
 320:	89a6                	mv	s3,s1
 322:	a011                	j	326 <gets+0x5a>
 324:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 326:	99de                	add	s3,s3,s7
 328:	00098023          	sb	zero,0(s3)
  return buf;
}
 32c:	855e                	mv	a0,s7
 32e:	60e6                	ld	ra,88(sp)
 330:	6446                	ld	s0,80(sp)
 332:	64a6                	ld	s1,72(sp)
 334:	6906                	ld	s2,64(sp)
 336:	79e2                	ld	s3,56(sp)
 338:	7a42                	ld	s4,48(sp)
 33a:	7aa2                	ld	s5,40(sp)
 33c:	7b02                	ld	s6,32(sp)
 33e:	6be2                	ld	s7,24(sp)
 340:	6c42                	ld	s8,16(sp)
 342:	6125                	addi	sp,sp,96
 344:	8082                	ret

0000000000000346 <stat>:

int
stat(const char *n, struct stat *st)
{
 346:	1101                	addi	sp,sp,-32
 348:	ec06                	sd	ra,24(sp)
 34a:	e822                	sd	s0,16(sp)
 34c:	e426                	sd	s1,8(sp)
 34e:	e04a                	sd	s2,0(sp)
 350:	1000                	addi	s0,sp,32
 352:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 354:	4581                	li	a1,0
 356:	00000097          	auipc	ra,0x0
 35a:	196080e7          	jalr	406(ra) # 4ec <open>
  if(fd < 0)
 35e:	02054563          	bltz	a0,388 <stat+0x42>
 362:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 364:	85ca                	mv	a1,s2
 366:	00000097          	auipc	ra,0x0
 36a:	190080e7          	jalr	400(ra) # 4f6 <fstat>
 36e:	892a                	mv	s2,a0
  close(fd);
 370:	8526                	mv	a0,s1
 372:	00000097          	auipc	ra,0x0
 376:	15e080e7          	jalr	350(ra) # 4d0 <close>
  return r;
}
 37a:	854a                	mv	a0,s2
 37c:	60e2                	ld	ra,24(sp)
 37e:	6442                	ld	s0,16(sp)
 380:	64a2                	ld	s1,8(sp)
 382:	6902                	ld	s2,0(sp)
 384:	6105                	addi	sp,sp,32
 386:	8082                	ret
    return -1;
 388:	597d                	li	s2,-1
 38a:	bfc5                	j	37a <stat+0x34>

000000000000038c <atoi>:

int
atoi(const char *s)
{
 38c:	1141                	addi	sp,sp,-16
 38e:	e422                	sd	s0,8(sp)
 390:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 392:	00054703          	lbu	a4,0(a0)
 396:	02d00793          	li	a5,45
  int neg = 1;
 39a:	4585                	li	a1,1
  if (*s == '-') {
 39c:	04f70363          	beq	a4,a5,3e2 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 3a0:	00054703          	lbu	a4,0(a0)
 3a4:	fd07079b          	addiw	a5,a4,-48
 3a8:	0ff7f793          	zext.b	a5,a5
 3ac:	46a5                	li	a3,9
 3ae:	02f6ed63          	bltu	a3,a5,3e8 <atoi+0x5c>
  n = 0;
 3b2:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 3b4:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 3b6:	0505                	addi	a0,a0,1
 3b8:	0026979b          	slliw	a5,a3,0x2
 3bc:	9fb5                	addw	a5,a5,a3
 3be:	0017979b          	slliw	a5,a5,0x1
 3c2:	9fb9                	addw	a5,a5,a4
 3c4:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 3c8:	00054703          	lbu	a4,0(a0)
 3cc:	fd07079b          	addiw	a5,a4,-48
 3d0:	0ff7f793          	zext.b	a5,a5
 3d4:	fef671e3          	bgeu	a2,a5,3b6 <atoi+0x2a>
  return n * neg;
}
 3d8:	02d5853b          	mulw	a0,a1,a3
 3dc:	6422                	ld	s0,8(sp)
 3de:	0141                	addi	sp,sp,16
 3e0:	8082                	ret
    s++;
 3e2:	0505                	addi	a0,a0,1
    neg = -1;
 3e4:	55fd                	li	a1,-1
 3e6:	bf6d                	j	3a0 <atoi+0x14>
  n = 0;
 3e8:	4681                	li	a3,0
 3ea:	b7fd                	j	3d8 <atoi+0x4c>

00000000000003ec <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 3ec:	1141                	addi	sp,sp,-16
 3ee:	e422                	sd	s0,8(sp)
 3f0:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 3f2:	02b57463          	bgeu	a0,a1,41a <memmove+0x2e>
    while(n-- > 0)
 3f6:	00c05f63          	blez	a2,414 <memmove+0x28>
 3fa:	1602                	slli	a2,a2,0x20
 3fc:	9201                	srli	a2,a2,0x20
 3fe:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 402:	872a                	mv	a4,a0
      *dst++ = *src++;
 404:	0585                	addi	a1,a1,1
 406:	0705                	addi	a4,a4,1
 408:	fff5c683          	lbu	a3,-1(a1)
 40c:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 410:	fee79ae3          	bne	a5,a4,404 <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 414:	6422                	ld	s0,8(sp)
 416:	0141                	addi	sp,sp,16
 418:	8082                	ret
    dst += n;
 41a:	00c50733          	add	a4,a0,a2
    src += n;
 41e:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 420:	fec05ae3          	blez	a2,414 <memmove+0x28>
 424:	fff6079b          	addiw	a5,a2,-1
 428:	1782                	slli	a5,a5,0x20
 42a:	9381                	srli	a5,a5,0x20
 42c:	fff7c793          	not	a5,a5
 430:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 432:	15fd                	addi	a1,a1,-1
 434:	177d                	addi	a4,a4,-1
 436:	0005c683          	lbu	a3,0(a1)
 43a:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 43e:	fee79ae3          	bne	a5,a4,432 <memmove+0x46>
 442:	bfc9                	j	414 <memmove+0x28>

0000000000000444 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 444:	1141                	addi	sp,sp,-16
 446:	e422                	sd	s0,8(sp)
 448:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 44a:	ca05                	beqz	a2,47a <memcmp+0x36>
 44c:	fff6069b          	addiw	a3,a2,-1
 450:	1682                	slli	a3,a3,0x20
 452:	9281                	srli	a3,a3,0x20
 454:	0685                	addi	a3,a3,1
 456:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 458:	00054783          	lbu	a5,0(a0)
 45c:	0005c703          	lbu	a4,0(a1)
 460:	00e79863          	bne	a5,a4,470 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 464:	0505                	addi	a0,a0,1
    p2++;
 466:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 468:	fed518e3          	bne	a0,a3,458 <memcmp+0x14>
  }
  return 0;
 46c:	4501                	li	a0,0
 46e:	a019                	j	474 <memcmp+0x30>
      return *p1 - *p2;
 470:	40e7853b          	subw	a0,a5,a4
}
 474:	6422                	ld	s0,8(sp)
 476:	0141                	addi	sp,sp,16
 478:	8082                	ret
  return 0;
 47a:	4501                	li	a0,0
 47c:	bfe5                	j	474 <memcmp+0x30>

000000000000047e <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 47e:	1141                	addi	sp,sp,-16
 480:	e406                	sd	ra,8(sp)
 482:	e022                	sd	s0,0(sp)
 484:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 486:	00000097          	auipc	ra,0x0
 48a:	f66080e7          	jalr	-154(ra) # 3ec <memmove>
}
 48e:	60a2                	ld	ra,8(sp)
 490:	6402                	ld	s0,0(sp)
 492:	0141                	addi	sp,sp,16
 494:	8082                	ret

0000000000000496 <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 496:	4885                	li	a7,1
 ecall
 498:	00000073          	ecall
 ret
 49c:	8082                	ret

000000000000049e <exit>:
.global exit
exit:
 li a7, SYS_exit
 49e:	05d00893          	li	a7,93
 ecall
 4a2:	00000073          	ecall
 ret
 4a6:	8082                	ret

00000000000004a8 <wait>:
.global wait
wait:
 li a7, SYS_wait
 4a8:	4bf00893          	li	a7,1215
 ecall
 4ac:	00000073          	ecall
 ret
 4b0:	8082                	ret

00000000000004b2 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 4b2:	03b00893          	li	a7,59
 ecall
 4b6:	00000073          	ecall
 ret
 4ba:	8082                	ret

00000000000004bc <read>:
.global read
read:
 li a7, SYS_read
 4bc:	03f00893          	li	a7,63
 ecall
 4c0:	00000073          	ecall
 ret
 4c4:	8082                	ret

00000000000004c6 <write>:
.global write
write:
 li a7, SYS_write
 4c6:	04000893          	li	a7,64
 ecall
 4ca:	00000073          	ecall
 ret
 4ce:	8082                	ret

00000000000004d0 <close>:
.global close
close:
 li a7, SYS_close
 4d0:	03900893          	li	a7,57
 ecall
 4d4:	00000073          	ecall
 ret
 4d8:	8082                	ret

00000000000004da <kill>:
.global kill
kill:
 li a7, SYS_kill
 4da:	4899                	li	a7,6
 ecall
 4dc:	00000073          	ecall
 ret
 4e0:	8082                	ret

00000000000004e2 <exec>:
.global exec
exec:
 li a7, SYS_exec
 4e2:	0dd00893          	li	a7,221
 ecall
 4e6:	00000073          	ecall
 ret
 4ea:	8082                	ret

00000000000004ec <open>:
.global open
open:
 li a7, SYS_open
 4ec:	03800893          	li	a7,56
 ecall
 4f0:	00000073          	ecall
 ret
 4f4:	8082                	ret

00000000000004f6 <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 4f6:	05000893          	li	a7,80
 ecall
 4fa:	00000073          	ecall
 ret
 4fe:	8082                	ret

0000000000000500 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 500:	02200893          	li	a7,34
 ecall
 504:	00000073          	ecall
 ret
 508:	8082                	ret

000000000000050a <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 50a:	03100893          	li	a7,49
 ecall
 50e:	00000073          	ecall
 ret
 512:	8082                	ret

0000000000000514 <dup>:
.global dup
dup:
 li a7, SYS_dup
 514:	48dd                	li	a7,23
 ecall
 516:	00000073          	ecall
 ret
 51a:	8082                	ret

000000000000051c <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 51c:	0ac00893          	li	a7,172
 ecall
 520:	00000073          	ecall
 ret
 524:	8082                	ret

0000000000000526 <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 526:	0d600893          	li	a7,214
 ecall
 52a:	00000073          	ecall
 ret
 52e:	8082                	ret

0000000000000530 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 530:	48b5                	li	a7,13
 ecall
 532:	00000073          	ecall
 ret
 536:	8082                	ret

0000000000000538 <times>:
.global times
times:
 li a7, SYS_times
 538:	09900893          	li	a7,153
 ecall
 53c:	00000073          	ecall
 ret
 540:	8082                	ret

0000000000000542 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 542:	48d9                	li	a7,22
 ecall
 544:	00000073          	ecall
 ret
 548:	8082                	ret

000000000000054a <dev>:
.global dev
dev:
 li a7, SYS_dev
 54a:	6889                	lui	a7,0x2
 54c:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x1485>
 ecall
 550:	00000073          	ecall
 ret
 554:	8082                	ret

0000000000000556 <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 556:	7d200893          	li	a7,2002
 ecall
 55a:	00000073          	ecall
 ret
 55e:	8082                	ret

0000000000000560 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 560:	48c5                	li	a7,17
 ecall
 562:	00000073          	ecall
 ret
 566:	8082                	ret

0000000000000568 <remove>:
.global remove
remove:
 li a7, SYS_remove
 568:	48e5                	li	a7,25
 ecall
 56a:	00000073          	ecall
 ret
 56e:	8082                	ret

0000000000000570 <trace>:
.global trace
trace:
 li a7, SYS_trace
 570:	48c9                	li	a7,18
 ecall
 572:	00000073          	ecall
 ret
 576:	8082                	ret

0000000000000578 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 578:	48cd                	li	a7,19
 ecall
 57a:	00000073          	ecall
 ret
 57e:	8082                	ret

0000000000000580 <rename>:
.global rename
rename:
 li a7, SYS_rename
 580:	48e9                	li	a7,26
 ecall
 582:	00000073          	ecall
 ret
 586:	8082                	ret

0000000000000588 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 588:	1101                	addi	sp,sp,-32
 58a:	ec06                	sd	ra,24(sp)
 58c:	e822                	sd	s0,16(sp)
 58e:	1000                	addi	s0,sp,32
 590:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 594:	4605                	li	a2,1
 596:	fef40593          	addi	a1,s0,-17
 59a:	00000097          	auipc	ra,0x0
 59e:	f2c080e7          	jalr	-212(ra) # 4c6 <write>
}
 5a2:	60e2                	ld	ra,24(sp)
 5a4:	6442                	ld	s0,16(sp)
 5a6:	6105                	addi	sp,sp,32
 5a8:	8082                	ret

00000000000005aa <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 5aa:	7139                	addi	sp,sp,-64
 5ac:	fc06                	sd	ra,56(sp)
 5ae:	f822                	sd	s0,48(sp)
 5b0:	f426                	sd	s1,40(sp)
 5b2:	f04a                	sd	s2,32(sp)
 5b4:	ec4e                	sd	s3,24(sp)
 5b6:	0080                	addi	s0,sp,64
 5b8:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 5ba:	c299                	beqz	a3,5c0 <printint+0x16>
 5bc:	0805c863          	bltz	a1,64c <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 5c0:	2581                	sext.w	a1,a1
  neg = 0;
 5c2:	4881                	li	a7,0
  }

  i = 0;
 5c4:	fc040993          	addi	s3,s0,-64
  neg = 0;
 5c8:	86ce                	mv	a3,s3
  i = 0;
 5ca:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 5cc:	2601                	sext.w	a2,a2
 5ce:	00000517          	auipc	a0,0x0
 5d2:	50250513          	addi	a0,a0,1282 # ad0 <digits>
 5d6:	883a                	mv	a6,a4
 5d8:	2705                	addiw	a4,a4,1
 5da:	02c5f7bb          	remuw	a5,a1,a2
 5de:	1782                	slli	a5,a5,0x20
 5e0:	9381                	srli	a5,a5,0x20
 5e2:	97aa                	add	a5,a5,a0
 5e4:	0007c783          	lbu	a5,0(a5)
 5e8:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 5ec:	0005879b          	sext.w	a5,a1
 5f0:	02c5d5bb          	divuw	a1,a1,a2
 5f4:	0685                	addi	a3,a3,1
 5f6:	fec7f0e3          	bgeu	a5,a2,5d6 <printint+0x2c>
  if(neg)
 5fa:	00088c63          	beqz	a7,612 <printint+0x68>
    buf[i++] = '-';
 5fe:	fd070793          	addi	a5,a4,-48
 602:	00878733          	add	a4,a5,s0
 606:	02d00793          	li	a5,45
 60a:	fef70823          	sb	a5,-16(a4)
 60e:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 612:	02e05663          	blez	a4,63e <printint+0x94>
 616:	fc040913          	addi	s2,s0,-64
 61a:	993a                	add	s2,s2,a4
 61c:	19fd                	addi	s3,s3,-1
 61e:	99ba                	add	s3,s3,a4
 620:	377d                	addiw	a4,a4,-1
 622:	1702                	slli	a4,a4,0x20
 624:	9301                	srli	a4,a4,0x20
 626:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 62a:	fff94583          	lbu	a1,-1(s2)
 62e:	8526                	mv	a0,s1
 630:	00000097          	auipc	ra,0x0
 634:	f58080e7          	jalr	-168(ra) # 588 <putc>
  while(--i >= 0)
 638:	197d                	addi	s2,s2,-1
 63a:	ff3918e3          	bne	s2,s3,62a <printint+0x80>
}
 63e:	70e2                	ld	ra,56(sp)
 640:	7442                	ld	s0,48(sp)
 642:	74a2                	ld	s1,40(sp)
 644:	7902                	ld	s2,32(sp)
 646:	69e2                	ld	s3,24(sp)
 648:	6121                	addi	sp,sp,64
 64a:	8082                	ret
    x = -xx;
 64c:	40b005bb          	negw	a1,a1
    neg = 1;
 650:	4885                	li	a7,1
    x = -xx;
 652:	bf8d                	j	5c4 <printint+0x1a>

0000000000000654 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 654:	7119                	addi	sp,sp,-128
 656:	fc86                	sd	ra,120(sp)
 658:	f8a2                	sd	s0,112(sp)
 65a:	f4a6                	sd	s1,104(sp)
 65c:	f0ca                	sd	s2,96(sp)
 65e:	ecce                	sd	s3,88(sp)
 660:	e8d2                	sd	s4,80(sp)
 662:	e4d6                	sd	s5,72(sp)
 664:	e0da                	sd	s6,64(sp)
 666:	fc5e                	sd	s7,56(sp)
 668:	f862                	sd	s8,48(sp)
 66a:	f466                	sd	s9,40(sp)
 66c:	f06a                	sd	s10,32(sp)
 66e:	ec6e                	sd	s11,24(sp)
 670:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 672:	0005c903          	lbu	s2,0(a1)
 676:	18090f63          	beqz	s2,814 <vprintf+0x1c0>
 67a:	8aaa                	mv	s5,a0
 67c:	8b32                	mv	s6,a2
 67e:	00158493          	addi	s1,a1,1
  state = 0;
 682:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 684:	02500a13          	li	s4,37
 688:	4c55                	li	s8,21
 68a:	00000c97          	auipc	s9,0x0
 68e:	3eec8c93          	addi	s9,s9,1006 # a78 <malloc+0x160>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 692:	02800d93          	li	s11,40
  putc(fd, 'x');
 696:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 698:	00000b97          	auipc	s7,0x0
 69c:	438b8b93          	addi	s7,s7,1080 # ad0 <digits>
 6a0:	a839                	j	6be <vprintf+0x6a>
        putc(fd, c);
 6a2:	85ca                	mv	a1,s2
 6a4:	8556                	mv	a0,s5
 6a6:	00000097          	auipc	ra,0x0
 6aa:	ee2080e7          	jalr	-286(ra) # 588 <putc>
 6ae:	a019                	j	6b4 <vprintf+0x60>
    } else if(state == '%'){
 6b0:	01498d63          	beq	s3,s4,6ca <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 6b4:	0485                	addi	s1,s1,1
 6b6:	fff4c903          	lbu	s2,-1(s1)
 6ba:	14090d63          	beqz	s2,814 <vprintf+0x1c0>
    if(state == 0){
 6be:	fe0999e3          	bnez	s3,6b0 <vprintf+0x5c>
      if(c == '%'){
 6c2:	ff4910e3          	bne	s2,s4,6a2 <vprintf+0x4e>
        state = '%';
 6c6:	89d2                	mv	s3,s4
 6c8:	b7f5                	j	6b4 <vprintf+0x60>
      if(c == 'd'){
 6ca:	11490c63          	beq	s2,s4,7e2 <vprintf+0x18e>
 6ce:	f9d9079b          	addiw	a5,s2,-99
 6d2:	0ff7f793          	zext.b	a5,a5
 6d6:	10fc6e63          	bltu	s8,a5,7f2 <vprintf+0x19e>
 6da:	f9d9079b          	addiw	a5,s2,-99
 6de:	0ff7f713          	zext.b	a4,a5
 6e2:	10ec6863          	bltu	s8,a4,7f2 <vprintf+0x19e>
 6e6:	00271793          	slli	a5,a4,0x2
 6ea:	97e6                	add	a5,a5,s9
 6ec:	439c                	lw	a5,0(a5)
 6ee:	97e6                	add	a5,a5,s9
 6f0:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 6f2:	008b0913          	addi	s2,s6,8
 6f6:	4685                	li	a3,1
 6f8:	4629                	li	a2,10
 6fa:	000b2583          	lw	a1,0(s6)
 6fe:	8556                	mv	a0,s5
 700:	00000097          	auipc	ra,0x0
 704:	eaa080e7          	jalr	-342(ra) # 5aa <printint>
 708:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 70a:	4981                	li	s3,0
 70c:	b765                	j	6b4 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 70e:	008b0913          	addi	s2,s6,8
 712:	4681                	li	a3,0
 714:	4629                	li	a2,10
 716:	000b2583          	lw	a1,0(s6)
 71a:	8556                	mv	a0,s5
 71c:	00000097          	auipc	ra,0x0
 720:	e8e080e7          	jalr	-370(ra) # 5aa <printint>
 724:	8b4a                	mv	s6,s2
      state = 0;
 726:	4981                	li	s3,0
 728:	b771                	j	6b4 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 72a:	008b0913          	addi	s2,s6,8
 72e:	4681                	li	a3,0
 730:	866a                	mv	a2,s10
 732:	000b2583          	lw	a1,0(s6)
 736:	8556                	mv	a0,s5
 738:	00000097          	auipc	ra,0x0
 73c:	e72080e7          	jalr	-398(ra) # 5aa <printint>
 740:	8b4a                	mv	s6,s2
      state = 0;
 742:	4981                	li	s3,0
 744:	bf85                	j	6b4 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 746:	008b0793          	addi	a5,s6,8
 74a:	f8f43423          	sd	a5,-120(s0)
 74e:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 752:	03000593          	li	a1,48
 756:	8556                	mv	a0,s5
 758:	00000097          	auipc	ra,0x0
 75c:	e30080e7          	jalr	-464(ra) # 588 <putc>
  putc(fd, 'x');
 760:	07800593          	li	a1,120
 764:	8556                	mv	a0,s5
 766:	00000097          	auipc	ra,0x0
 76a:	e22080e7          	jalr	-478(ra) # 588 <putc>
 76e:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 770:	03c9d793          	srli	a5,s3,0x3c
 774:	97de                	add	a5,a5,s7
 776:	0007c583          	lbu	a1,0(a5)
 77a:	8556                	mv	a0,s5
 77c:	00000097          	auipc	ra,0x0
 780:	e0c080e7          	jalr	-500(ra) # 588 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 784:	0992                	slli	s3,s3,0x4
 786:	397d                	addiw	s2,s2,-1
 788:	fe0914e3          	bnez	s2,770 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 78c:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 790:	4981                	li	s3,0
 792:	b70d                	j	6b4 <vprintf+0x60>
        s = va_arg(ap, char*);
 794:	008b0913          	addi	s2,s6,8
 798:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 79c:	02098163          	beqz	s3,7be <vprintf+0x16a>
        while(*s != 0){
 7a0:	0009c583          	lbu	a1,0(s3)
 7a4:	c5ad                	beqz	a1,80e <vprintf+0x1ba>
          putc(fd, *s);
 7a6:	8556                	mv	a0,s5
 7a8:	00000097          	auipc	ra,0x0
 7ac:	de0080e7          	jalr	-544(ra) # 588 <putc>
          s++;
 7b0:	0985                	addi	s3,s3,1
        while(*s != 0){
 7b2:	0009c583          	lbu	a1,0(s3)
 7b6:	f9e5                	bnez	a1,7a6 <vprintf+0x152>
        s = va_arg(ap, char*);
 7b8:	8b4a                	mv	s6,s2
      state = 0;
 7ba:	4981                	li	s3,0
 7bc:	bde5                	j	6b4 <vprintf+0x60>
          s = "(null)";
 7be:	00000997          	auipc	s3,0x0
 7c2:	2b298993          	addi	s3,s3,690 # a70 <malloc+0x158>
        while(*s != 0){
 7c6:	85ee                	mv	a1,s11
 7c8:	bff9                	j	7a6 <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 7ca:	008b0913          	addi	s2,s6,8
 7ce:	000b4583          	lbu	a1,0(s6)
 7d2:	8556                	mv	a0,s5
 7d4:	00000097          	auipc	ra,0x0
 7d8:	db4080e7          	jalr	-588(ra) # 588 <putc>
 7dc:	8b4a                	mv	s6,s2
      state = 0;
 7de:	4981                	li	s3,0
 7e0:	bdd1                	j	6b4 <vprintf+0x60>
        putc(fd, c);
 7e2:	85d2                	mv	a1,s4
 7e4:	8556                	mv	a0,s5
 7e6:	00000097          	auipc	ra,0x0
 7ea:	da2080e7          	jalr	-606(ra) # 588 <putc>
      state = 0;
 7ee:	4981                	li	s3,0
 7f0:	b5d1                	j	6b4 <vprintf+0x60>
        putc(fd, '%');
 7f2:	85d2                	mv	a1,s4
 7f4:	8556                	mv	a0,s5
 7f6:	00000097          	auipc	ra,0x0
 7fa:	d92080e7          	jalr	-622(ra) # 588 <putc>
        putc(fd, c);
 7fe:	85ca                	mv	a1,s2
 800:	8556                	mv	a0,s5
 802:	00000097          	auipc	ra,0x0
 806:	d86080e7          	jalr	-634(ra) # 588 <putc>
      state = 0;
 80a:	4981                	li	s3,0
 80c:	b565                	j	6b4 <vprintf+0x60>
        s = va_arg(ap, char*);
 80e:	8b4a                	mv	s6,s2
      state = 0;
 810:	4981                	li	s3,0
 812:	b54d                	j	6b4 <vprintf+0x60>
    }
  }
}
 814:	70e6                	ld	ra,120(sp)
 816:	7446                	ld	s0,112(sp)
 818:	74a6                	ld	s1,104(sp)
 81a:	7906                	ld	s2,96(sp)
 81c:	69e6                	ld	s3,88(sp)
 81e:	6a46                	ld	s4,80(sp)
 820:	6aa6                	ld	s5,72(sp)
 822:	6b06                	ld	s6,64(sp)
 824:	7be2                	ld	s7,56(sp)
 826:	7c42                	ld	s8,48(sp)
 828:	7ca2                	ld	s9,40(sp)
 82a:	7d02                	ld	s10,32(sp)
 82c:	6de2                	ld	s11,24(sp)
 82e:	6109                	addi	sp,sp,128
 830:	8082                	ret

0000000000000832 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 832:	715d                	addi	sp,sp,-80
 834:	ec06                	sd	ra,24(sp)
 836:	e822                	sd	s0,16(sp)
 838:	1000                	addi	s0,sp,32
 83a:	e010                	sd	a2,0(s0)
 83c:	e414                	sd	a3,8(s0)
 83e:	e818                	sd	a4,16(s0)
 840:	ec1c                	sd	a5,24(s0)
 842:	03043023          	sd	a6,32(s0)
 846:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 84a:	8622                	mv	a2,s0
 84c:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 850:	00000097          	auipc	ra,0x0
 854:	e04080e7          	jalr	-508(ra) # 654 <vprintf>
}
 858:	60e2                	ld	ra,24(sp)
 85a:	6442                	ld	s0,16(sp)
 85c:	6161                	addi	sp,sp,80
 85e:	8082                	ret

0000000000000860 <printf>:

void
printf(const char *fmt, ...)
{
 860:	711d                	addi	sp,sp,-96
 862:	ec06                	sd	ra,24(sp)
 864:	e822                	sd	s0,16(sp)
 866:	1000                	addi	s0,sp,32
 868:	e40c                	sd	a1,8(s0)
 86a:	e810                	sd	a2,16(s0)
 86c:	ec14                	sd	a3,24(s0)
 86e:	f018                	sd	a4,32(s0)
 870:	f41c                	sd	a5,40(s0)
 872:	03043823          	sd	a6,48(s0)
 876:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 87a:	00840613          	addi	a2,s0,8
 87e:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 882:	85aa                	mv	a1,a0
 884:	4505                	li	a0,1
 886:	00000097          	auipc	ra,0x0
 88a:	dce080e7          	jalr	-562(ra) # 654 <vprintf>
}
 88e:	60e2                	ld	ra,24(sp)
 890:	6442                	ld	s0,16(sp)
 892:	6125                	addi	sp,sp,96
 894:	8082                	ret

0000000000000896 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 896:	1141                	addi	sp,sp,-16
 898:	e422                	sd	s0,8(sp)
 89a:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 89c:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8a0:	00000797          	auipc	a5,0x0
 8a4:	2707b783          	ld	a5,624(a5) # b10 <freep>
 8a8:	a02d                	j	8d2 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 8aa:	4618                	lw	a4,8(a2)
 8ac:	9f2d                	addw	a4,a4,a1
 8ae:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 8b2:	6398                	ld	a4,0(a5)
 8b4:	6310                	ld	a2,0(a4)
 8b6:	a83d                	j	8f4 <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 8b8:	ff852703          	lw	a4,-8(a0)
 8bc:	9f31                	addw	a4,a4,a2
 8be:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 8c0:	ff053683          	ld	a3,-16(a0)
 8c4:	a091                	j	908 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8c6:	6398                	ld	a4,0(a5)
 8c8:	00e7e463          	bltu	a5,a4,8d0 <free+0x3a>
 8cc:	00e6ea63          	bltu	a3,a4,8e0 <free+0x4a>
{
 8d0:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8d2:	fed7fae3          	bgeu	a5,a3,8c6 <free+0x30>
 8d6:	6398                	ld	a4,0(a5)
 8d8:	00e6e463          	bltu	a3,a4,8e0 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8dc:	fee7eae3          	bltu	a5,a4,8d0 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 8e0:	ff852583          	lw	a1,-8(a0)
 8e4:	6390                	ld	a2,0(a5)
 8e6:	02059813          	slli	a6,a1,0x20
 8ea:	01c85713          	srli	a4,a6,0x1c
 8ee:	9736                	add	a4,a4,a3
 8f0:	fae60de3          	beq	a2,a4,8aa <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 8f4:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 8f8:	4790                	lw	a2,8(a5)
 8fa:	02061593          	slli	a1,a2,0x20
 8fe:	01c5d713          	srli	a4,a1,0x1c
 902:	973e                	add	a4,a4,a5
 904:	fae68ae3          	beq	a3,a4,8b8 <free+0x22>
    p->s.ptr = bp->s.ptr;
 908:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 90a:	00000717          	auipc	a4,0x0
 90e:	20f73323          	sd	a5,518(a4) # b10 <freep>
}
 912:	6422                	ld	s0,8(sp)
 914:	0141                	addi	sp,sp,16
 916:	8082                	ret

0000000000000918 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 918:	7139                	addi	sp,sp,-64
 91a:	fc06                	sd	ra,56(sp)
 91c:	f822                	sd	s0,48(sp)
 91e:	f426                	sd	s1,40(sp)
 920:	f04a                	sd	s2,32(sp)
 922:	ec4e                	sd	s3,24(sp)
 924:	e852                	sd	s4,16(sp)
 926:	e456                	sd	s5,8(sp)
 928:	e05a                	sd	s6,0(sp)
 92a:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 92c:	02051493          	slli	s1,a0,0x20
 930:	9081                	srli	s1,s1,0x20
 932:	04bd                	addi	s1,s1,15
 934:	8091                	srli	s1,s1,0x4
 936:	00148a1b          	addiw	s4,s1,1
 93a:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 93c:	00000517          	auipc	a0,0x0
 940:	1d453503          	ld	a0,468(a0) # b10 <freep>
 944:	c515                	beqz	a0,970 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 946:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 948:	4798                	lw	a4,8(a5)
 94a:	04977163          	bgeu	a4,s1,98c <malloc+0x74>
 94e:	89d2                	mv	s3,s4
 950:	000a071b          	sext.w	a4,s4
 954:	6685                	lui	a3,0x1
 956:	00d77363          	bgeu	a4,a3,95c <malloc+0x44>
 95a:	6985                	lui	s3,0x1
 95c:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 960:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 964:	00000917          	auipc	s2,0x0
 968:	1ac90913          	addi	s2,s2,428 # b10 <freep>
  if(p == (char*)-1)
 96c:	5afd                	li	s5,-1
 96e:	a8a5                	j	9e6 <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 970:	00000797          	auipc	a5,0x0
 974:	1a078793          	addi	a5,a5,416 # b10 <freep>
 978:	00000717          	auipc	a4,0x0
 97c:	1a070713          	addi	a4,a4,416 # b18 <base>
 980:	e398                	sd	a4,0(a5)
 982:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 984:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 988:	87ba                	mv	a5,a4
 98a:	b7d1                	j	94e <malloc+0x36>
      if(p->s.size == nunits)
 98c:	02e48c63          	beq	s1,a4,9c4 <malloc+0xac>
        p->s.size -= nunits;
 990:	4147073b          	subw	a4,a4,s4
 994:	c798                	sw	a4,8(a5)
        p += p->s.size;
 996:	02071693          	slli	a3,a4,0x20
 99a:	01c6d713          	srli	a4,a3,0x1c
 99e:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 9a0:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 9a4:	00000717          	auipc	a4,0x0
 9a8:	16a73623          	sd	a0,364(a4) # b10 <freep>
      return (void*)(p + 1);
 9ac:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 9b0:	70e2                	ld	ra,56(sp)
 9b2:	7442                	ld	s0,48(sp)
 9b4:	74a2                	ld	s1,40(sp)
 9b6:	7902                	ld	s2,32(sp)
 9b8:	69e2                	ld	s3,24(sp)
 9ba:	6a42                	ld	s4,16(sp)
 9bc:	6aa2                	ld	s5,8(sp)
 9be:	6b02                	ld	s6,0(sp)
 9c0:	6121                	addi	sp,sp,64
 9c2:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 9c4:	6398                	ld	a4,0(a5)
 9c6:	e118                	sd	a4,0(a0)
 9c8:	bff1                	j	9a4 <malloc+0x8c>
  hp->s.size = nu;
 9ca:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 9ce:	0541                	addi	a0,a0,16
 9d0:	00000097          	auipc	ra,0x0
 9d4:	ec6080e7          	jalr	-314(ra) # 896 <free>
  return freep;
 9d8:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 9dc:	d971                	beqz	a0,9b0 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9de:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 9e0:	4798                	lw	a4,8(a5)
 9e2:	fa9775e3          	bgeu	a4,s1,98c <malloc+0x74>
    if(p == freep)
 9e6:	00093703          	ld	a4,0(s2)
 9ea:	853e                	mv	a0,a5
 9ec:	fef719e3          	bne	a4,a5,9de <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 9f0:	854e                	mv	a0,s3
 9f2:	00000097          	auipc	ra,0x0
 9f6:	b34080e7          	jalr	-1228(ra) # 526 <sbrk>
  if(p == (char*)-1)
 9fa:	fd5518e3          	bne	a0,s5,9ca <malloc+0xb2>
        return 0;
 9fe:	4501                	li	a0,0
 a00:	bf45                	j	9b0 <malloc+0x98>
