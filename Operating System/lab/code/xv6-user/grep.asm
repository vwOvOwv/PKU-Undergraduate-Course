
xv6-user/_grep:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <matchstar>:
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
   0:	7179                	addi	sp,sp,-48
   2:	f406                	sd	ra,40(sp)
   4:	f022                	sd	s0,32(sp)
   6:	ec26                	sd	s1,24(sp)
   8:	e84a                	sd	s2,16(sp)
   a:	e44e                	sd	s3,8(sp)
   c:	e052                	sd	s4,0(sp)
   e:	1800                	addi	s0,sp,48
  10:	892a                	mv	s2,a0
  12:	89ae                	mv	s3,a1
  14:	84b2                	mv	s1,a2
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  16:	02e00a13          	li	s4,46
    if(matchhere(re, text))
  1a:	85a6                	mv	a1,s1
  1c:	854e                	mv	a0,s3
  1e:	00000097          	auipc	ra,0x0
  22:	030080e7          	jalr	48(ra) # 4e <matchhere>
  26:	e919                	bnez	a0,3c <matchstar+0x3c>
  }while(*text!='\0' && (*text++==c || c=='.'));
  28:	0004c783          	lbu	a5,0(s1)
  2c:	cb89                	beqz	a5,3e <matchstar+0x3e>
  2e:	0485                	addi	s1,s1,1
  30:	2781                	sext.w	a5,a5
  32:	ff2784e3          	beq	a5,s2,1a <matchstar+0x1a>
  36:	ff4902e3          	beq	s2,s4,1a <matchstar+0x1a>
  3a:	a011                	j	3e <matchstar+0x3e>
      return 1;
  3c:	4505                	li	a0,1
  return 0;
}
  3e:	70a2                	ld	ra,40(sp)
  40:	7402                	ld	s0,32(sp)
  42:	64e2                	ld	s1,24(sp)
  44:	6942                	ld	s2,16(sp)
  46:	69a2                	ld	s3,8(sp)
  48:	6a02                	ld	s4,0(sp)
  4a:	6145                	addi	sp,sp,48
  4c:	8082                	ret

000000000000004e <matchhere>:
  if(re[0] == '\0')
  4e:	00054703          	lbu	a4,0(a0)
  52:	cb3d                	beqz	a4,c8 <matchhere+0x7a>
{
  54:	1141                	addi	sp,sp,-16
  56:	e406                	sd	ra,8(sp)
  58:	e022                	sd	s0,0(sp)
  5a:	0800                	addi	s0,sp,16
  5c:	87aa                	mv	a5,a0
  if(re[1] == '*')
  5e:	00154683          	lbu	a3,1(a0)
  62:	02a00613          	li	a2,42
  66:	02c68563          	beq	a3,a2,90 <matchhere+0x42>
  if(re[0] == '$' && re[1] == '\0')
  6a:	02400613          	li	a2,36
  6e:	02c70a63          	beq	a4,a2,a2 <matchhere+0x54>
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
  72:	0005c683          	lbu	a3,0(a1)
  return 0;
  76:	4501                	li	a0,0
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
  78:	ca81                	beqz	a3,88 <matchhere+0x3a>
  7a:	02e00613          	li	a2,46
  7e:	02c70d63          	beq	a4,a2,b8 <matchhere+0x6a>
  return 0;
  82:	4501                	li	a0,0
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
  84:	02d70a63          	beq	a4,a3,b8 <matchhere+0x6a>
}
  88:	60a2                	ld	ra,8(sp)
  8a:	6402                	ld	s0,0(sp)
  8c:	0141                	addi	sp,sp,16
  8e:	8082                	ret
    return matchstar(re[0], re+2, text);
  90:	862e                	mv	a2,a1
  92:	00250593          	addi	a1,a0,2
  96:	853a                	mv	a0,a4
  98:	00000097          	auipc	ra,0x0
  9c:	f68080e7          	jalr	-152(ra) # 0 <matchstar>
  a0:	b7e5                	j	88 <matchhere+0x3a>
  if(re[0] == '$' && re[1] == '\0')
  a2:	c691                	beqz	a3,ae <matchhere+0x60>
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
  a4:	0005c683          	lbu	a3,0(a1)
  a8:	fee9                	bnez	a3,82 <matchhere+0x34>
  return 0;
  aa:	4501                	li	a0,0
  ac:	bff1                	j	88 <matchhere+0x3a>
    return *text == '\0';
  ae:	0005c503          	lbu	a0,0(a1)
  b2:	00153513          	seqz	a0,a0
  b6:	bfc9                	j	88 <matchhere+0x3a>
    return matchhere(re+1, text+1);
  b8:	0585                	addi	a1,a1,1
  ba:	00178513          	addi	a0,a5,1
  be:	00000097          	auipc	ra,0x0
  c2:	f90080e7          	jalr	-112(ra) # 4e <matchhere>
  c6:	b7c9                	j	88 <matchhere+0x3a>
    return 1;
  c8:	4505                	li	a0,1
}
  ca:	8082                	ret

00000000000000cc <match>:
{
  cc:	1101                	addi	sp,sp,-32
  ce:	ec06                	sd	ra,24(sp)
  d0:	e822                	sd	s0,16(sp)
  d2:	e426                	sd	s1,8(sp)
  d4:	e04a                	sd	s2,0(sp)
  d6:	1000                	addi	s0,sp,32
  d8:	892a                	mv	s2,a0
  da:	84ae                	mv	s1,a1
  if(re[0] == '^')
  dc:	00054703          	lbu	a4,0(a0)
  e0:	05e00793          	li	a5,94
  e4:	00f70e63          	beq	a4,a5,100 <match+0x34>
    if(matchhere(re, text))
  e8:	85a6                	mv	a1,s1
  ea:	854a                	mv	a0,s2
  ec:	00000097          	auipc	ra,0x0
  f0:	f62080e7          	jalr	-158(ra) # 4e <matchhere>
  f4:	ed01                	bnez	a0,10c <match+0x40>
  }while(*text++ != '\0');
  f6:	0485                	addi	s1,s1,1
  f8:	fff4c783          	lbu	a5,-1(s1)
  fc:	f7f5                	bnez	a5,e8 <match+0x1c>
  fe:	a801                	j	10e <match+0x42>
    return matchhere(re+1, text);
 100:	0505                	addi	a0,a0,1
 102:	00000097          	auipc	ra,0x0
 106:	f4c080e7          	jalr	-180(ra) # 4e <matchhere>
 10a:	a011                	j	10e <match+0x42>
      return 1;
 10c:	4505                	li	a0,1
}
 10e:	60e2                	ld	ra,24(sp)
 110:	6442                	ld	s0,16(sp)
 112:	64a2                	ld	s1,8(sp)
 114:	6902                	ld	s2,0(sp)
 116:	6105                	addi	sp,sp,32
 118:	8082                	ret

000000000000011a <grep>:
{
 11a:	715d                	addi	sp,sp,-80
 11c:	e486                	sd	ra,72(sp)
 11e:	e0a2                	sd	s0,64(sp)
 120:	fc26                	sd	s1,56(sp)
 122:	f84a                	sd	s2,48(sp)
 124:	f44e                	sd	s3,40(sp)
 126:	f052                	sd	s4,32(sp)
 128:	ec56                	sd	s5,24(sp)
 12a:	e85a                	sd	s6,16(sp)
 12c:	e45e                	sd	s7,8(sp)
 12e:	0880                	addi	s0,sp,80
 130:	89aa                	mv	s3,a0
 132:	8b2e                	mv	s6,a1
  m = 0;
 134:	4a01                	li	s4,0
  while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
 136:	3ff00b93          	li	s7,1023
 13a:	00001a97          	auipc	s5,0x1
 13e:	a36a8a93          	addi	s5,s5,-1482 # b70 <buf>
 142:	a0a1                	j	18a <grep+0x70>
      p = q+1;
 144:	00148913          	addi	s2,s1,1
    while((q = strchr(p, '\n')) != 0){
 148:	45a9                	li	a1,10
 14a:	854a                	mv	a0,s2
 14c:	00000097          	auipc	ra,0x0
 150:	214080e7          	jalr	532(ra) # 360 <strchr>
 154:	84aa                	mv	s1,a0
 156:	c905                	beqz	a0,186 <grep+0x6c>
      *q = 0;
 158:	00048023          	sb	zero,0(s1)
      if(match(pattern, p)){
 15c:	85ca                	mv	a1,s2
 15e:	854e                	mv	a0,s3
 160:	00000097          	auipc	ra,0x0
 164:	f6c080e7          	jalr	-148(ra) # cc <match>
 168:	dd71                	beqz	a0,144 <grep+0x2a>
        *q = '\n';
 16a:	47a9                	li	a5,10
 16c:	00f48023          	sb	a5,0(s1)
        write(1, p, q+1 - p);
 170:	00148613          	addi	a2,s1,1
 174:	4126063b          	subw	a2,a2,s2
 178:	85ca                	mv	a1,s2
 17a:	4505                	li	a0,1
 17c:	00000097          	auipc	ra,0x0
 180:	402080e7          	jalr	1026(ra) # 57e <write>
 184:	b7c1                	j	144 <grep+0x2a>
    if(m > 0){
 186:	03404563          	bgtz	s4,1b0 <grep+0x96>
  while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
 18a:	414b863b          	subw	a2,s7,s4
 18e:	014a85b3          	add	a1,s5,s4
 192:	855a                	mv	a0,s6
 194:	00000097          	auipc	ra,0x0
 198:	3e0080e7          	jalr	992(ra) # 574 <read>
 19c:	02a05663          	blez	a0,1c8 <grep+0xae>
    m += n;
 1a0:	00aa0a3b          	addw	s4,s4,a0
    buf[m] = '\0';
 1a4:	014a87b3          	add	a5,s5,s4
 1a8:	00078023          	sb	zero,0(a5)
    p = buf;
 1ac:	8956                	mv	s2,s5
    while((q = strchr(p, '\n')) != 0){
 1ae:	bf69                	j	148 <grep+0x2e>
      m -= p - buf;
 1b0:	415907b3          	sub	a5,s2,s5
 1b4:	40fa0a3b          	subw	s4,s4,a5
      memmove(buf, p, m);
 1b8:	8652                	mv	a2,s4
 1ba:	85ca                	mv	a1,s2
 1bc:	8556                	mv	a0,s5
 1be:	00000097          	auipc	ra,0x0
 1c2:	2e6080e7          	jalr	742(ra) # 4a4 <memmove>
 1c6:	b7d1                	j	18a <grep+0x70>
}
 1c8:	60a6                	ld	ra,72(sp)
 1ca:	6406                	ld	s0,64(sp)
 1cc:	74e2                	ld	s1,56(sp)
 1ce:	7942                	ld	s2,48(sp)
 1d0:	79a2                	ld	s3,40(sp)
 1d2:	7a02                	ld	s4,32(sp)
 1d4:	6ae2                	ld	s5,24(sp)
 1d6:	6b42                	ld	s6,16(sp)
 1d8:	6ba2                	ld	s7,8(sp)
 1da:	6161                	addi	sp,sp,80
 1dc:	8082                	ret

00000000000001de <main>:
{
 1de:	7139                	addi	sp,sp,-64
 1e0:	fc06                	sd	ra,56(sp)
 1e2:	f822                	sd	s0,48(sp)
 1e4:	f426                	sd	s1,40(sp)
 1e6:	f04a                	sd	s2,32(sp)
 1e8:	ec4e                	sd	s3,24(sp)
 1ea:	e852                	sd	s4,16(sp)
 1ec:	e456                	sd	s5,8(sp)
 1ee:	0080                	addi	s0,sp,64
  if(argc <= 1){
 1f0:	4785                	li	a5,1
 1f2:	04a7de63          	bge	a5,a0,24e <main+0x70>
  pattern = argv[1];
 1f6:	0085ba03          	ld	s4,8(a1)
  if(argc <= 2){
 1fa:	4789                	li	a5,2
 1fc:	06a7d763          	bge	a5,a0,26a <main+0x8c>
 200:	01058913          	addi	s2,a1,16
 204:	ffd5099b          	addiw	s3,a0,-3
 208:	02099793          	slli	a5,s3,0x20
 20c:	01d7d993          	srli	s3,a5,0x1d
 210:	05e1                	addi	a1,a1,24
 212:	99ae                	add	s3,s3,a1
    if((fd = open(argv[i], 0)) < 0){
 214:	4581                	li	a1,0
 216:	00093503          	ld	a0,0(s2)
 21a:	00000097          	auipc	ra,0x0
 21e:	38a080e7          	jalr	906(ra) # 5a4 <open>
 222:	84aa                	mv	s1,a0
 224:	04054e63          	bltz	a0,280 <main+0xa2>
    grep(pattern, fd);
 228:	85aa                	mv	a1,a0
 22a:	8552                	mv	a0,s4
 22c:	00000097          	auipc	ra,0x0
 230:	eee080e7          	jalr	-274(ra) # 11a <grep>
    close(fd);
 234:	8526                	mv	a0,s1
 236:	00000097          	auipc	ra,0x0
 23a:	352080e7          	jalr	850(ra) # 588 <close>
  for(i = 2; i < argc; i++){
 23e:	0921                	addi	s2,s2,8
 240:	fd391ae3          	bne	s2,s3,214 <main+0x36>
  exit(0);
 244:	4501                	li	a0,0
 246:	00000097          	auipc	ra,0x0
 24a:	310080e7          	jalr	784(ra) # 556 <exit>
    fprintf(2, "usage: grep pattern [file ...]\n");
 24e:	00001597          	auipc	a1,0x1
 252:	87258593          	addi	a1,a1,-1934 # ac0 <malloc+0xf0>
 256:	4509                	li	a0,2
 258:	00000097          	auipc	ra,0x0
 25c:	692080e7          	jalr	1682(ra) # 8ea <fprintf>
    exit(1);
 260:	4505                	li	a0,1
 262:	00000097          	auipc	ra,0x0
 266:	2f4080e7          	jalr	756(ra) # 556 <exit>
    grep(pattern, 0);
 26a:	4581                	li	a1,0
 26c:	8552                	mv	a0,s4
 26e:	00000097          	auipc	ra,0x0
 272:	eac080e7          	jalr	-340(ra) # 11a <grep>
    exit(0);
 276:	4501                	li	a0,0
 278:	00000097          	auipc	ra,0x0
 27c:	2de080e7          	jalr	734(ra) # 556 <exit>
      printf("grep: cannot open %s\n", argv[i]);
 280:	00093583          	ld	a1,0(s2)
 284:	00001517          	auipc	a0,0x1
 288:	85c50513          	addi	a0,a0,-1956 # ae0 <malloc+0x110>
 28c:	00000097          	auipc	ra,0x0
 290:	68c080e7          	jalr	1676(ra) # 918 <printf>
      exit(1);
 294:	4505                	li	a0,1
 296:	00000097          	auipc	ra,0x0
 29a:	2c0080e7          	jalr	704(ra) # 556 <exit>

000000000000029e <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
 29e:	1141                	addi	sp,sp,-16
 2a0:	e422                	sd	s0,8(sp)
 2a2:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 2a4:	87aa                	mv	a5,a0
 2a6:	0585                	addi	a1,a1,1
 2a8:	0785                	addi	a5,a5,1
 2aa:	fff5c703          	lbu	a4,-1(a1)
 2ae:	fee78fa3          	sb	a4,-1(a5)
 2b2:	fb75                	bnez	a4,2a6 <strcpy+0x8>
    ;
  return os;
}
 2b4:	6422                	ld	s0,8(sp)
 2b6:	0141                	addi	sp,sp,16
 2b8:	8082                	ret

00000000000002ba <strcat>:

char*
strcat(char *s, const char *t)
{
 2ba:	1141                	addi	sp,sp,-16
 2bc:	e422                	sd	s0,8(sp)
 2be:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
 2c0:	00054783          	lbu	a5,0(a0)
 2c4:	c385                	beqz	a5,2e4 <strcat+0x2a>
 2c6:	87aa                	mv	a5,a0
    s++;
 2c8:	0785                	addi	a5,a5,1
  while(*s)
 2ca:	0007c703          	lbu	a4,0(a5)
 2ce:	ff6d                	bnez	a4,2c8 <strcat+0xe>
  while((*s++ = *t++))
 2d0:	0585                	addi	a1,a1,1
 2d2:	0785                	addi	a5,a5,1
 2d4:	fff5c703          	lbu	a4,-1(a1)
 2d8:	fee78fa3          	sb	a4,-1(a5)
 2dc:	fb75                	bnez	a4,2d0 <strcat+0x16>
    ;
  return os;
}
 2de:	6422                	ld	s0,8(sp)
 2e0:	0141                	addi	sp,sp,16
 2e2:	8082                	ret
  while(*s)
 2e4:	87aa                	mv	a5,a0
 2e6:	b7ed                	j	2d0 <strcat+0x16>

00000000000002e8 <strcmp>:


int
strcmp(const char *p, const char *q)
{
 2e8:	1141                	addi	sp,sp,-16
 2ea:	e422                	sd	s0,8(sp)
 2ec:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
 2ee:	00054783          	lbu	a5,0(a0)
 2f2:	cb91                	beqz	a5,306 <strcmp+0x1e>
 2f4:	0005c703          	lbu	a4,0(a1)
 2f8:	00f71763          	bne	a4,a5,306 <strcmp+0x1e>
    p++, q++;
 2fc:	0505                	addi	a0,a0,1
 2fe:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
 300:	00054783          	lbu	a5,0(a0)
 304:	fbe5                	bnez	a5,2f4 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
 306:	0005c503          	lbu	a0,0(a1)
}
 30a:	40a7853b          	subw	a0,a5,a0
 30e:	6422                	ld	s0,8(sp)
 310:	0141                	addi	sp,sp,16
 312:	8082                	ret

0000000000000314 <strlen>:

uint
strlen(const char *s)
{
 314:	1141                	addi	sp,sp,-16
 316:	e422                	sd	s0,8(sp)
 318:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
 31a:	00054783          	lbu	a5,0(a0)
 31e:	cf91                	beqz	a5,33a <strlen+0x26>
 320:	0505                	addi	a0,a0,1
 322:	87aa                	mv	a5,a0
 324:	4685                	li	a3,1
 326:	9e89                	subw	a3,a3,a0
 328:	00f6853b          	addw	a0,a3,a5
 32c:	0785                	addi	a5,a5,1
 32e:	fff7c703          	lbu	a4,-1(a5)
 332:	fb7d                	bnez	a4,328 <strlen+0x14>
    ;
  return n;
}
 334:	6422                	ld	s0,8(sp)
 336:	0141                	addi	sp,sp,16
 338:	8082                	ret
  for(n = 0; s[n]; n++)
 33a:	4501                	li	a0,0
 33c:	bfe5                	j	334 <strlen+0x20>

000000000000033e <memset>:

void*
memset(void *dst, int c, uint n)
{
 33e:	1141                	addi	sp,sp,-16
 340:	e422                	sd	s0,8(sp)
 342:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 344:	ca19                	beqz	a2,35a <memset+0x1c>
 346:	87aa                	mv	a5,a0
 348:	1602                	slli	a2,a2,0x20
 34a:	9201                	srli	a2,a2,0x20
 34c:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
 350:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
 354:	0785                	addi	a5,a5,1
 356:	fee79de3          	bne	a5,a4,350 <memset+0x12>
  }
  return dst;
}
 35a:	6422                	ld	s0,8(sp)
 35c:	0141                	addi	sp,sp,16
 35e:	8082                	ret

0000000000000360 <strchr>:

char*
strchr(const char *s, char c)
{
 360:	1141                	addi	sp,sp,-16
 362:	e422                	sd	s0,8(sp)
 364:	0800                	addi	s0,sp,16
  for(; *s; s++)
 366:	00054783          	lbu	a5,0(a0)
 36a:	cb99                	beqz	a5,380 <strchr+0x20>
    if(*s == c)
 36c:	00f58763          	beq	a1,a5,37a <strchr+0x1a>
  for(; *s; s++)
 370:	0505                	addi	a0,a0,1
 372:	00054783          	lbu	a5,0(a0)
 376:	fbfd                	bnez	a5,36c <strchr+0xc>
      return (char*)s;
  return 0;
 378:	4501                	li	a0,0
}
 37a:	6422                	ld	s0,8(sp)
 37c:	0141                	addi	sp,sp,16
 37e:	8082                	ret
  return 0;
 380:	4501                	li	a0,0
 382:	bfe5                	j	37a <strchr+0x1a>

0000000000000384 <gets>:

char*
gets(char *buf, int max)
{
 384:	711d                	addi	sp,sp,-96
 386:	ec86                	sd	ra,88(sp)
 388:	e8a2                	sd	s0,80(sp)
 38a:	e4a6                	sd	s1,72(sp)
 38c:	e0ca                	sd	s2,64(sp)
 38e:	fc4e                	sd	s3,56(sp)
 390:	f852                	sd	s4,48(sp)
 392:	f456                	sd	s5,40(sp)
 394:	f05a                	sd	s6,32(sp)
 396:	ec5e                	sd	s7,24(sp)
 398:	e862                	sd	s8,16(sp)
 39a:	1080                	addi	s0,sp,96
 39c:	8baa                	mv	s7,a0
 39e:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 3a0:	892a                	mv	s2,a0
 3a2:	4481                	li	s1,0
    cc = read(0, &c, 1);
 3a4:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 3a8:	4b29                	li	s6,10
 3aa:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
 3ac:	89a6                	mv	s3,s1
 3ae:	2485                	addiw	s1,s1,1
 3b0:	0344d763          	bge	s1,s4,3de <gets+0x5a>
    cc = read(0, &c, 1);
 3b4:	4605                	li	a2,1
 3b6:	85d6                	mv	a1,s5
 3b8:	4501                	li	a0,0
 3ba:	00000097          	auipc	ra,0x0
 3be:	1ba080e7          	jalr	442(ra) # 574 <read>
    if(cc < 1)
 3c2:	00a05e63          	blez	a0,3de <gets+0x5a>
    buf[i++] = c;
 3c6:	faf44783          	lbu	a5,-81(s0)
 3ca:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 3ce:	01678763          	beq	a5,s6,3dc <gets+0x58>
 3d2:	0905                	addi	s2,s2,1
 3d4:	fd879ce3          	bne	a5,s8,3ac <gets+0x28>
  for(i=0; i+1 < max; ){
 3d8:	89a6                	mv	s3,s1
 3da:	a011                	j	3de <gets+0x5a>
 3dc:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 3de:	99de                	add	s3,s3,s7
 3e0:	00098023          	sb	zero,0(s3)
  return buf;
}
 3e4:	855e                	mv	a0,s7
 3e6:	60e6                	ld	ra,88(sp)
 3e8:	6446                	ld	s0,80(sp)
 3ea:	64a6                	ld	s1,72(sp)
 3ec:	6906                	ld	s2,64(sp)
 3ee:	79e2                	ld	s3,56(sp)
 3f0:	7a42                	ld	s4,48(sp)
 3f2:	7aa2                	ld	s5,40(sp)
 3f4:	7b02                	ld	s6,32(sp)
 3f6:	6be2                	ld	s7,24(sp)
 3f8:	6c42                	ld	s8,16(sp)
 3fa:	6125                	addi	sp,sp,96
 3fc:	8082                	ret

00000000000003fe <stat>:

int
stat(const char *n, struct stat *st)
{
 3fe:	1101                	addi	sp,sp,-32
 400:	ec06                	sd	ra,24(sp)
 402:	e822                	sd	s0,16(sp)
 404:	e426                	sd	s1,8(sp)
 406:	e04a                	sd	s2,0(sp)
 408:	1000                	addi	s0,sp,32
 40a:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 40c:	4581                	li	a1,0
 40e:	00000097          	auipc	ra,0x0
 412:	196080e7          	jalr	406(ra) # 5a4 <open>
  if(fd < 0)
 416:	02054563          	bltz	a0,440 <stat+0x42>
 41a:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 41c:	85ca                	mv	a1,s2
 41e:	00000097          	auipc	ra,0x0
 422:	190080e7          	jalr	400(ra) # 5ae <fstat>
 426:	892a                	mv	s2,a0
  close(fd);
 428:	8526                	mv	a0,s1
 42a:	00000097          	auipc	ra,0x0
 42e:	15e080e7          	jalr	350(ra) # 588 <close>
  return r;
}
 432:	854a                	mv	a0,s2
 434:	60e2                	ld	ra,24(sp)
 436:	6442                	ld	s0,16(sp)
 438:	64a2                	ld	s1,8(sp)
 43a:	6902                	ld	s2,0(sp)
 43c:	6105                	addi	sp,sp,32
 43e:	8082                	ret
    return -1;
 440:	597d                	li	s2,-1
 442:	bfc5                	j	432 <stat+0x34>

0000000000000444 <atoi>:

int
atoi(const char *s)
{
 444:	1141                	addi	sp,sp,-16
 446:	e422                	sd	s0,8(sp)
 448:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 44a:	00054703          	lbu	a4,0(a0)
 44e:	02d00793          	li	a5,45
  int neg = 1;
 452:	4585                	li	a1,1
  if (*s == '-') {
 454:	04f70363          	beq	a4,a5,49a <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 458:	00054703          	lbu	a4,0(a0)
 45c:	fd07079b          	addiw	a5,a4,-48
 460:	0ff7f793          	zext.b	a5,a5
 464:	46a5                	li	a3,9
 466:	02f6ed63          	bltu	a3,a5,4a0 <atoi+0x5c>
  n = 0;
 46a:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
 46c:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
 46e:	0505                	addi	a0,a0,1
 470:	0026979b          	slliw	a5,a3,0x2
 474:	9fb5                	addw	a5,a5,a3
 476:	0017979b          	slliw	a5,a5,0x1
 47a:	9fb9                	addw	a5,a5,a4
 47c:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
 480:	00054703          	lbu	a4,0(a0)
 484:	fd07079b          	addiw	a5,a4,-48
 488:	0ff7f793          	zext.b	a5,a5
 48c:	fef671e3          	bgeu	a2,a5,46e <atoi+0x2a>
  return n * neg;
}
 490:	02d5853b          	mulw	a0,a1,a3
 494:	6422                	ld	s0,8(sp)
 496:	0141                	addi	sp,sp,16
 498:	8082                	ret
    s++;
 49a:	0505                	addi	a0,a0,1
    neg = -1;
 49c:	55fd                	li	a1,-1
 49e:	bf6d                	j	458 <atoi+0x14>
  n = 0;
 4a0:	4681                	li	a3,0
 4a2:	b7fd                	j	490 <atoi+0x4c>

00000000000004a4 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 4a4:	1141                	addi	sp,sp,-16
 4a6:	e422                	sd	s0,8(sp)
 4a8:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 4aa:	02b57463          	bgeu	a0,a1,4d2 <memmove+0x2e>
    while(n-- > 0)
 4ae:	00c05f63          	blez	a2,4cc <memmove+0x28>
 4b2:	1602                	slli	a2,a2,0x20
 4b4:	9201                	srli	a2,a2,0x20
 4b6:	00c507b3          	add	a5,a0,a2
  dst = vdst;
 4ba:	872a                	mv	a4,a0
      *dst++ = *src++;
 4bc:	0585                	addi	a1,a1,1
 4be:	0705                	addi	a4,a4,1
 4c0:	fff5c683          	lbu	a3,-1(a1)
 4c4:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 4c8:	fee79ae3          	bne	a5,a4,4bc <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 4cc:	6422                	ld	s0,8(sp)
 4ce:	0141                	addi	sp,sp,16
 4d0:	8082                	ret
    dst += n;
 4d2:	00c50733          	add	a4,a0,a2
    src += n;
 4d6:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 4d8:	fec05ae3          	blez	a2,4cc <memmove+0x28>
 4dc:	fff6079b          	addiw	a5,a2,-1
 4e0:	1782                	slli	a5,a5,0x20
 4e2:	9381                	srli	a5,a5,0x20
 4e4:	fff7c793          	not	a5,a5
 4e8:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 4ea:	15fd                	addi	a1,a1,-1
 4ec:	177d                	addi	a4,a4,-1
 4ee:	0005c683          	lbu	a3,0(a1)
 4f2:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 4f6:	fee79ae3          	bne	a5,a4,4ea <memmove+0x46>
 4fa:	bfc9                	j	4cc <memmove+0x28>

00000000000004fc <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 4fc:	1141                	addi	sp,sp,-16
 4fe:	e422                	sd	s0,8(sp)
 500:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 502:	ca05                	beqz	a2,532 <memcmp+0x36>
 504:	fff6069b          	addiw	a3,a2,-1
 508:	1682                	slli	a3,a3,0x20
 50a:	9281                	srli	a3,a3,0x20
 50c:	0685                	addi	a3,a3,1
 50e:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
 510:	00054783          	lbu	a5,0(a0)
 514:	0005c703          	lbu	a4,0(a1)
 518:	00e79863          	bne	a5,a4,528 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
 51c:	0505                	addi	a0,a0,1
    p2++;
 51e:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 520:	fed518e3          	bne	a0,a3,510 <memcmp+0x14>
  }
  return 0;
 524:	4501                	li	a0,0
 526:	a019                	j	52c <memcmp+0x30>
      return *p1 - *p2;
 528:	40e7853b          	subw	a0,a5,a4
}
 52c:	6422                	ld	s0,8(sp)
 52e:	0141                	addi	sp,sp,16
 530:	8082                	ret
  return 0;
 532:	4501                	li	a0,0
 534:	bfe5                	j	52c <memcmp+0x30>

0000000000000536 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 536:	1141                	addi	sp,sp,-16
 538:	e406                	sd	ra,8(sp)
 53a:	e022                	sd	s0,0(sp)
 53c:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 53e:	00000097          	auipc	ra,0x0
 542:	f66080e7          	jalr	-154(ra) # 4a4 <memmove>
}
 546:	60a2                	ld	ra,8(sp)
 548:	6402                	ld	s0,0(sp)
 54a:	0141                	addi	sp,sp,16
 54c:	8082                	ret

000000000000054e <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
 54e:	4885                	li	a7,1
 ecall
 550:	00000073          	ecall
 ret
 554:	8082                	ret

0000000000000556 <exit>:
.global exit
exit:
 li a7, SYS_exit
 556:	05d00893          	li	a7,93
 ecall
 55a:	00000073          	ecall
 ret
 55e:	8082                	ret

0000000000000560 <wait>:
.global wait
wait:
 li a7, SYS_wait
 560:	4bf00893          	li	a7,1215
 ecall
 564:	00000073          	ecall
 ret
 568:	8082                	ret

000000000000056a <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
 56a:	03b00893          	li	a7,59
 ecall
 56e:	00000073          	ecall
 ret
 572:	8082                	ret

0000000000000574 <read>:
.global read
read:
 li a7, SYS_read
 574:	03f00893          	li	a7,63
 ecall
 578:	00000073          	ecall
 ret
 57c:	8082                	ret

000000000000057e <write>:
.global write
write:
 li a7, SYS_write
 57e:	04000893          	li	a7,64
 ecall
 582:	00000073          	ecall
 ret
 586:	8082                	ret

0000000000000588 <close>:
.global close
close:
 li a7, SYS_close
 588:	03900893          	li	a7,57
 ecall
 58c:	00000073          	ecall
 ret
 590:	8082                	ret

0000000000000592 <kill>:
.global kill
kill:
 li a7, SYS_kill
 592:	4899                	li	a7,6
 ecall
 594:	00000073          	ecall
 ret
 598:	8082                	ret

000000000000059a <exec>:
.global exec
exec:
 li a7, SYS_exec
 59a:	0dd00893          	li	a7,221
 ecall
 59e:	00000073          	ecall
 ret
 5a2:	8082                	ret

00000000000005a4 <open>:
.global open
open:
 li a7, SYS_open
 5a4:	03800893          	li	a7,56
 ecall
 5a8:	00000073          	ecall
 ret
 5ac:	8082                	ret

00000000000005ae <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 5ae:	05000893          	li	a7,80
 ecall
 5b2:	00000073          	ecall
 ret
 5b6:	8082                	ret

00000000000005b8 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
 5b8:	02200893          	li	a7,34
 ecall
 5bc:	00000073          	ecall
 ret
 5c0:	8082                	ret

00000000000005c2 <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 5c2:	03100893          	li	a7,49
 ecall
 5c6:	00000073          	ecall
 ret
 5ca:	8082                	ret

00000000000005cc <dup>:
.global dup
dup:
 li a7, SYS_dup
 5cc:	48dd                	li	a7,23
 ecall
 5ce:	00000073          	ecall
 ret
 5d2:	8082                	ret

00000000000005d4 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 5d4:	0ac00893          	li	a7,172
 ecall
 5d8:	00000073          	ecall
 ret
 5dc:	8082                	ret

00000000000005de <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
 5de:	0d600893          	li	a7,214
 ecall
 5e2:	00000073          	ecall
 ret
 5e6:	8082                	ret

00000000000005e8 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
 5e8:	48b5                	li	a7,13
 ecall
 5ea:	00000073          	ecall
 ret
 5ee:	8082                	ret

00000000000005f0 <times>:
.global times
times:
 li a7, SYS_times
 5f0:	09900893          	li	a7,153
 ecall
 5f4:	00000073          	ecall
 ret
 5f8:	8082                	ret

00000000000005fa <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
 5fa:	48d9                	li	a7,22
 ecall
 5fc:	00000073          	ecall
 ret
 600:	8082                	ret

0000000000000602 <dev>:
.global dev
dev:
 li a7, SYS_dev
 602:	6889                	lui	a7,0x2
 604:	7668889b          	addiw	a7,a7,1894 # 2766 <__global_pointer$+0x13fd>
 ecall
 608:	00000073          	ecall
 ret
 60c:	8082                	ret

000000000000060e <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
 60e:	7d200893          	li	a7,2002
 ecall
 612:	00000073          	ecall
 ret
 616:	8082                	ret

0000000000000618 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 618:	48c5                	li	a7,17
 ecall
 61a:	00000073          	ecall
 ret
 61e:	8082                	ret

0000000000000620 <remove>:
.global remove
remove:
 li a7, SYS_remove
 620:	48e5                	li	a7,25
 ecall
 622:	00000073          	ecall
 ret
 626:	8082                	ret

0000000000000628 <trace>:
.global trace
trace:
 li a7, SYS_trace
 628:	48c9                	li	a7,18
 ecall
 62a:	00000073          	ecall
 ret
 62e:	8082                	ret

0000000000000630 <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
 630:	48cd                	li	a7,19
 ecall
 632:	00000073          	ecall
 ret
 636:	8082                	ret

0000000000000638 <rename>:
.global rename
rename:
 li a7, SYS_rename
 638:	48e9                	li	a7,26
 ecall
 63a:	00000073          	ecall
 ret
 63e:	8082                	ret

0000000000000640 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 640:	1101                	addi	sp,sp,-32
 642:	ec06                	sd	ra,24(sp)
 644:	e822                	sd	s0,16(sp)
 646:	1000                	addi	s0,sp,32
 648:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 64c:	4605                	li	a2,1
 64e:	fef40593          	addi	a1,s0,-17
 652:	00000097          	auipc	ra,0x0
 656:	f2c080e7          	jalr	-212(ra) # 57e <write>
}
 65a:	60e2                	ld	ra,24(sp)
 65c:	6442                	ld	s0,16(sp)
 65e:	6105                	addi	sp,sp,32
 660:	8082                	ret

0000000000000662 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 662:	7139                	addi	sp,sp,-64
 664:	fc06                	sd	ra,56(sp)
 666:	f822                	sd	s0,48(sp)
 668:	f426                	sd	s1,40(sp)
 66a:	f04a                	sd	s2,32(sp)
 66c:	ec4e                	sd	s3,24(sp)
 66e:	0080                	addi	s0,sp,64
 670:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 672:	c299                	beqz	a3,678 <printint+0x16>
 674:	0805c863          	bltz	a1,704 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 678:	2581                	sext.w	a1,a1
  neg = 0;
 67a:	4881                	li	a7,0
  }

  i = 0;
 67c:	fc040993          	addi	s3,s0,-64
  neg = 0;
 680:	86ce                	mv	a3,s3
  i = 0;
 682:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
 684:	2601                	sext.w	a2,a2
 686:	00000517          	auipc	a0,0x0
 68a:	4d250513          	addi	a0,a0,1234 # b58 <digits>
 68e:	883a                	mv	a6,a4
 690:	2705                	addiw	a4,a4,1
 692:	02c5f7bb          	remuw	a5,a1,a2
 696:	1782                	slli	a5,a5,0x20
 698:	9381                	srli	a5,a5,0x20
 69a:	97aa                	add	a5,a5,a0
 69c:	0007c783          	lbu	a5,0(a5)
 6a0:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
 6a4:	0005879b          	sext.w	a5,a1
 6a8:	02c5d5bb          	divuw	a1,a1,a2
 6ac:	0685                	addi	a3,a3,1
 6ae:	fec7f0e3          	bgeu	a5,a2,68e <printint+0x2c>
  if(neg)
 6b2:	00088c63          	beqz	a7,6ca <printint+0x68>
    buf[i++] = '-';
 6b6:	fd070793          	addi	a5,a4,-48
 6ba:	00878733          	add	a4,a5,s0
 6be:	02d00793          	li	a5,45
 6c2:	fef70823          	sb	a5,-16(a4)
 6c6:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
 6ca:	02e05663          	blez	a4,6f6 <printint+0x94>
 6ce:	fc040913          	addi	s2,s0,-64
 6d2:	993a                	add	s2,s2,a4
 6d4:	19fd                	addi	s3,s3,-1
 6d6:	99ba                	add	s3,s3,a4
 6d8:	377d                	addiw	a4,a4,-1
 6da:	1702                	slli	a4,a4,0x20
 6dc:	9301                	srli	a4,a4,0x20
 6de:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
 6e2:	fff94583          	lbu	a1,-1(s2)
 6e6:	8526                	mv	a0,s1
 6e8:	00000097          	auipc	ra,0x0
 6ec:	f58080e7          	jalr	-168(ra) # 640 <putc>
  while(--i >= 0)
 6f0:	197d                	addi	s2,s2,-1
 6f2:	ff3918e3          	bne	s2,s3,6e2 <printint+0x80>
}
 6f6:	70e2                	ld	ra,56(sp)
 6f8:	7442                	ld	s0,48(sp)
 6fa:	74a2                	ld	s1,40(sp)
 6fc:	7902                	ld	s2,32(sp)
 6fe:	69e2                	ld	s3,24(sp)
 700:	6121                	addi	sp,sp,64
 702:	8082                	ret
    x = -xx;
 704:	40b005bb          	negw	a1,a1
    neg = 1;
 708:	4885                	li	a7,1
    x = -xx;
 70a:	bf8d                	j	67c <printint+0x1a>

000000000000070c <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 70c:	7119                	addi	sp,sp,-128
 70e:	fc86                	sd	ra,120(sp)
 710:	f8a2                	sd	s0,112(sp)
 712:	f4a6                	sd	s1,104(sp)
 714:	f0ca                	sd	s2,96(sp)
 716:	ecce                	sd	s3,88(sp)
 718:	e8d2                	sd	s4,80(sp)
 71a:	e4d6                	sd	s5,72(sp)
 71c:	e0da                	sd	s6,64(sp)
 71e:	fc5e                	sd	s7,56(sp)
 720:	f862                	sd	s8,48(sp)
 722:	f466                	sd	s9,40(sp)
 724:	f06a                	sd	s10,32(sp)
 726:	ec6e                	sd	s11,24(sp)
 728:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 72a:	0005c903          	lbu	s2,0(a1)
 72e:	18090f63          	beqz	s2,8cc <vprintf+0x1c0>
 732:	8aaa                	mv	s5,a0
 734:	8b32                	mv	s6,a2
 736:	00158493          	addi	s1,a1,1
  state = 0;
 73a:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 73c:	02500a13          	li	s4,37
 740:	4c55                	li	s8,21
 742:	00000c97          	auipc	s9,0x0
 746:	3bec8c93          	addi	s9,s9,958 # b00 <malloc+0x130>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 74a:	02800d93          	li	s11,40
  putc(fd, 'x');
 74e:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 750:	00000b97          	auipc	s7,0x0
 754:	408b8b93          	addi	s7,s7,1032 # b58 <digits>
 758:	a839                	j	776 <vprintf+0x6a>
        putc(fd, c);
 75a:	85ca                	mv	a1,s2
 75c:	8556                	mv	a0,s5
 75e:	00000097          	auipc	ra,0x0
 762:	ee2080e7          	jalr	-286(ra) # 640 <putc>
 766:	a019                	j	76c <vprintf+0x60>
    } else if(state == '%'){
 768:	01498d63          	beq	s3,s4,782 <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
 76c:	0485                	addi	s1,s1,1
 76e:	fff4c903          	lbu	s2,-1(s1)
 772:	14090d63          	beqz	s2,8cc <vprintf+0x1c0>
    if(state == 0){
 776:	fe0999e3          	bnez	s3,768 <vprintf+0x5c>
      if(c == '%'){
 77a:	ff4910e3          	bne	s2,s4,75a <vprintf+0x4e>
        state = '%';
 77e:	89d2                	mv	s3,s4
 780:	b7f5                	j	76c <vprintf+0x60>
      if(c == 'd'){
 782:	11490c63          	beq	s2,s4,89a <vprintf+0x18e>
 786:	f9d9079b          	addiw	a5,s2,-99
 78a:	0ff7f793          	zext.b	a5,a5
 78e:	10fc6e63          	bltu	s8,a5,8aa <vprintf+0x19e>
 792:	f9d9079b          	addiw	a5,s2,-99
 796:	0ff7f713          	zext.b	a4,a5
 79a:	10ec6863          	bltu	s8,a4,8aa <vprintf+0x19e>
 79e:	00271793          	slli	a5,a4,0x2
 7a2:	97e6                	add	a5,a5,s9
 7a4:	439c                	lw	a5,0(a5)
 7a6:	97e6                	add	a5,a5,s9
 7a8:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
 7aa:	008b0913          	addi	s2,s6,8
 7ae:	4685                	li	a3,1
 7b0:	4629                	li	a2,10
 7b2:	000b2583          	lw	a1,0(s6)
 7b6:	8556                	mv	a0,s5
 7b8:	00000097          	auipc	ra,0x0
 7bc:	eaa080e7          	jalr	-342(ra) # 662 <printint>
 7c0:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 7c2:	4981                	li	s3,0
 7c4:	b765                	j	76c <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 7c6:	008b0913          	addi	s2,s6,8
 7ca:	4681                	li	a3,0
 7cc:	4629                	li	a2,10
 7ce:	000b2583          	lw	a1,0(s6)
 7d2:	8556                	mv	a0,s5
 7d4:	00000097          	auipc	ra,0x0
 7d8:	e8e080e7          	jalr	-370(ra) # 662 <printint>
 7dc:	8b4a                	mv	s6,s2
      state = 0;
 7de:	4981                	li	s3,0
 7e0:	b771                	j	76c <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 7e2:	008b0913          	addi	s2,s6,8
 7e6:	4681                	li	a3,0
 7e8:	866a                	mv	a2,s10
 7ea:	000b2583          	lw	a1,0(s6)
 7ee:	8556                	mv	a0,s5
 7f0:	00000097          	auipc	ra,0x0
 7f4:	e72080e7          	jalr	-398(ra) # 662 <printint>
 7f8:	8b4a                	mv	s6,s2
      state = 0;
 7fa:	4981                	li	s3,0
 7fc:	bf85                	j	76c <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 7fe:	008b0793          	addi	a5,s6,8
 802:	f8f43423          	sd	a5,-120(s0)
 806:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 80a:	03000593          	li	a1,48
 80e:	8556                	mv	a0,s5
 810:	00000097          	auipc	ra,0x0
 814:	e30080e7          	jalr	-464(ra) # 640 <putc>
  putc(fd, 'x');
 818:	07800593          	li	a1,120
 81c:	8556                	mv	a0,s5
 81e:	00000097          	auipc	ra,0x0
 822:	e22080e7          	jalr	-478(ra) # 640 <putc>
 826:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 828:	03c9d793          	srli	a5,s3,0x3c
 82c:	97de                	add	a5,a5,s7
 82e:	0007c583          	lbu	a1,0(a5)
 832:	8556                	mv	a0,s5
 834:	00000097          	auipc	ra,0x0
 838:	e0c080e7          	jalr	-500(ra) # 640 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 83c:	0992                	slli	s3,s3,0x4
 83e:	397d                	addiw	s2,s2,-1
 840:	fe0914e3          	bnez	s2,828 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
 844:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 848:	4981                	li	s3,0
 84a:	b70d                	j	76c <vprintf+0x60>
        s = va_arg(ap, char*);
 84c:	008b0913          	addi	s2,s6,8
 850:	000b3983          	ld	s3,0(s6)
        if(s == 0)
 854:	02098163          	beqz	s3,876 <vprintf+0x16a>
        while(*s != 0){
 858:	0009c583          	lbu	a1,0(s3)
 85c:	c5ad                	beqz	a1,8c6 <vprintf+0x1ba>
          putc(fd, *s);
 85e:	8556                	mv	a0,s5
 860:	00000097          	auipc	ra,0x0
 864:	de0080e7          	jalr	-544(ra) # 640 <putc>
          s++;
 868:	0985                	addi	s3,s3,1
        while(*s != 0){
 86a:	0009c583          	lbu	a1,0(s3)
 86e:	f9e5                	bnez	a1,85e <vprintf+0x152>
        s = va_arg(ap, char*);
 870:	8b4a                	mv	s6,s2
      state = 0;
 872:	4981                	li	s3,0
 874:	bde5                	j	76c <vprintf+0x60>
          s = "(null)";
 876:	00000997          	auipc	s3,0x0
 87a:	28298993          	addi	s3,s3,642 # af8 <malloc+0x128>
        while(*s != 0){
 87e:	85ee                	mv	a1,s11
 880:	bff9                	j	85e <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
 882:	008b0913          	addi	s2,s6,8
 886:	000b4583          	lbu	a1,0(s6)
 88a:	8556                	mv	a0,s5
 88c:	00000097          	auipc	ra,0x0
 890:	db4080e7          	jalr	-588(ra) # 640 <putc>
 894:	8b4a                	mv	s6,s2
      state = 0;
 896:	4981                	li	s3,0
 898:	bdd1                	j	76c <vprintf+0x60>
        putc(fd, c);
 89a:	85d2                	mv	a1,s4
 89c:	8556                	mv	a0,s5
 89e:	00000097          	auipc	ra,0x0
 8a2:	da2080e7          	jalr	-606(ra) # 640 <putc>
      state = 0;
 8a6:	4981                	li	s3,0
 8a8:	b5d1                	j	76c <vprintf+0x60>
        putc(fd, '%');
 8aa:	85d2                	mv	a1,s4
 8ac:	8556                	mv	a0,s5
 8ae:	00000097          	auipc	ra,0x0
 8b2:	d92080e7          	jalr	-622(ra) # 640 <putc>
        putc(fd, c);
 8b6:	85ca                	mv	a1,s2
 8b8:	8556                	mv	a0,s5
 8ba:	00000097          	auipc	ra,0x0
 8be:	d86080e7          	jalr	-634(ra) # 640 <putc>
      state = 0;
 8c2:	4981                	li	s3,0
 8c4:	b565                	j	76c <vprintf+0x60>
        s = va_arg(ap, char*);
 8c6:	8b4a                	mv	s6,s2
      state = 0;
 8c8:	4981                	li	s3,0
 8ca:	b54d                	j	76c <vprintf+0x60>
    }
  }
}
 8cc:	70e6                	ld	ra,120(sp)
 8ce:	7446                	ld	s0,112(sp)
 8d0:	74a6                	ld	s1,104(sp)
 8d2:	7906                	ld	s2,96(sp)
 8d4:	69e6                	ld	s3,88(sp)
 8d6:	6a46                	ld	s4,80(sp)
 8d8:	6aa6                	ld	s5,72(sp)
 8da:	6b06                	ld	s6,64(sp)
 8dc:	7be2                	ld	s7,56(sp)
 8de:	7c42                	ld	s8,48(sp)
 8e0:	7ca2                	ld	s9,40(sp)
 8e2:	7d02                	ld	s10,32(sp)
 8e4:	6de2                	ld	s11,24(sp)
 8e6:	6109                	addi	sp,sp,128
 8e8:	8082                	ret

00000000000008ea <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 8ea:	715d                	addi	sp,sp,-80
 8ec:	ec06                	sd	ra,24(sp)
 8ee:	e822                	sd	s0,16(sp)
 8f0:	1000                	addi	s0,sp,32
 8f2:	e010                	sd	a2,0(s0)
 8f4:	e414                	sd	a3,8(s0)
 8f6:	e818                	sd	a4,16(s0)
 8f8:	ec1c                	sd	a5,24(s0)
 8fa:	03043023          	sd	a6,32(s0)
 8fe:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 902:	8622                	mv	a2,s0
 904:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 908:	00000097          	auipc	ra,0x0
 90c:	e04080e7          	jalr	-508(ra) # 70c <vprintf>
}
 910:	60e2                	ld	ra,24(sp)
 912:	6442                	ld	s0,16(sp)
 914:	6161                	addi	sp,sp,80
 916:	8082                	ret

0000000000000918 <printf>:

void
printf(const char *fmt, ...)
{
 918:	711d                	addi	sp,sp,-96
 91a:	ec06                	sd	ra,24(sp)
 91c:	e822                	sd	s0,16(sp)
 91e:	1000                	addi	s0,sp,32
 920:	e40c                	sd	a1,8(s0)
 922:	e810                	sd	a2,16(s0)
 924:	ec14                	sd	a3,24(s0)
 926:	f018                	sd	a4,32(s0)
 928:	f41c                	sd	a5,40(s0)
 92a:	03043823          	sd	a6,48(s0)
 92e:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 932:	00840613          	addi	a2,s0,8
 936:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 93a:	85aa                	mv	a1,a0
 93c:	4505                	li	a0,1
 93e:	00000097          	auipc	ra,0x0
 942:	dce080e7          	jalr	-562(ra) # 70c <vprintf>
}
 946:	60e2                	ld	ra,24(sp)
 948:	6442                	ld	s0,16(sp)
 94a:	6125                	addi	sp,sp,96
 94c:	8082                	ret

000000000000094e <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 94e:	1141                	addi	sp,sp,-16
 950:	e422                	sd	s0,8(sp)
 952:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 954:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 958:	00000797          	auipc	a5,0x0
 95c:	6187b783          	ld	a5,1560(a5) # f70 <freep>
 960:	a02d                	j	98a <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 962:	4618                	lw	a4,8(a2)
 964:	9f2d                	addw	a4,a4,a1
 966:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 96a:	6398                	ld	a4,0(a5)
 96c:	6310                	ld	a2,0(a4)
 96e:	a83d                	j	9ac <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 970:	ff852703          	lw	a4,-8(a0)
 974:	9f31                	addw	a4,a4,a2
 976:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
 978:	ff053683          	ld	a3,-16(a0)
 97c:	a091                	j	9c0 <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 97e:	6398                	ld	a4,0(a5)
 980:	00e7e463          	bltu	a5,a4,988 <free+0x3a>
 984:	00e6ea63          	bltu	a3,a4,998 <free+0x4a>
{
 988:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 98a:	fed7fae3          	bgeu	a5,a3,97e <free+0x30>
 98e:	6398                	ld	a4,0(a5)
 990:	00e6e463          	bltu	a3,a4,998 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 994:	fee7eae3          	bltu	a5,a4,988 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
 998:	ff852583          	lw	a1,-8(a0)
 99c:	6390                	ld	a2,0(a5)
 99e:	02059813          	slli	a6,a1,0x20
 9a2:	01c85713          	srli	a4,a6,0x1c
 9a6:	9736                	add	a4,a4,a3
 9a8:	fae60de3          	beq	a2,a4,962 <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
 9ac:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 9b0:	4790                	lw	a2,8(a5)
 9b2:	02061593          	slli	a1,a2,0x20
 9b6:	01c5d713          	srli	a4,a1,0x1c
 9ba:	973e                	add	a4,a4,a5
 9bc:	fae68ae3          	beq	a3,a4,970 <free+0x22>
    p->s.ptr = bp->s.ptr;
 9c0:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
 9c2:	00000717          	auipc	a4,0x0
 9c6:	5af73723          	sd	a5,1454(a4) # f70 <freep>
}
 9ca:	6422                	ld	s0,8(sp)
 9cc:	0141                	addi	sp,sp,16
 9ce:	8082                	ret

00000000000009d0 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 9d0:	7139                	addi	sp,sp,-64
 9d2:	fc06                	sd	ra,56(sp)
 9d4:	f822                	sd	s0,48(sp)
 9d6:	f426                	sd	s1,40(sp)
 9d8:	f04a                	sd	s2,32(sp)
 9da:	ec4e                	sd	s3,24(sp)
 9dc:	e852                	sd	s4,16(sp)
 9de:	e456                	sd	s5,8(sp)
 9e0:	e05a                	sd	s6,0(sp)
 9e2:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 9e4:	02051493          	slli	s1,a0,0x20
 9e8:	9081                	srli	s1,s1,0x20
 9ea:	04bd                	addi	s1,s1,15
 9ec:	8091                	srli	s1,s1,0x4
 9ee:	00148a1b          	addiw	s4,s1,1
 9f2:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
 9f4:	00000517          	auipc	a0,0x0
 9f8:	57c53503          	ld	a0,1404(a0) # f70 <freep>
 9fc:	c515                	beqz	a0,a28 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9fe:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 a00:	4798                	lw	a4,8(a5)
 a02:	04977163          	bgeu	a4,s1,a44 <malloc+0x74>
 a06:	89d2                	mv	s3,s4
 a08:	000a071b          	sext.w	a4,s4
 a0c:	6685                	lui	a3,0x1
 a0e:	00d77363          	bgeu	a4,a3,a14 <malloc+0x44>
 a12:	6985                	lui	s3,0x1
 a14:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
 a18:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 a1c:	00000917          	auipc	s2,0x0
 a20:	55490913          	addi	s2,s2,1364 # f70 <freep>
  if(p == (char*)-1)
 a24:	5afd                	li	s5,-1
 a26:	a8a5                	j	a9e <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
 a28:	00000797          	auipc	a5,0x0
 a2c:	54878793          	addi	a5,a5,1352 # f70 <freep>
 a30:	00000717          	auipc	a4,0x0
 a34:	54870713          	addi	a4,a4,1352 # f78 <base>
 a38:	e398                	sd	a4,0(a5)
 a3a:	e798                	sd	a4,8(a5)
    base.s.size = 0;
 a3c:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 a40:	87ba                	mv	a5,a4
 a42:	b7d1                	j	a06 <malloc+0x36>
      if(p->s.size == nunits)
 a44:	02e48c63          	beq	s1,a4,a7c <malloc+0xac>
        p->s.size -= nunits;
 a48:	4147073b          	subw	a4,a4,s4
 a4c:	c798                	sw	a4,8(a5)
        p += p->s.size;
 a4e:	02071693          	slli	a3,a4,0x20
 a52:	01c6d713          	srli	a4,a3,0x1c
 a56:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
 a58:	0147a423          	sw	s4,8(a5)
      freep = prevp;
 a5c:	00000717          	auipc	a4,0x0
 a60:	50a73a23          	sd	a0,1300(a4) # f70 <freep>
      return (void*)(p + 1);
 a64:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 a68:	70e2                	ld	ra,56(sp)
 a6a:	7442                	ld	s0,48(sp)
 a6c:	74a2                	ld	s1,40(sp)
 a6e:	7902                	ld	s2,32(sp)
 a70:	69e2                	ld	s3,24(sp)
 a72:	6a42                	ld	s4,16(sp)
 a74:	6aa2                	ld	s5,8(sp)
 a76:	6b02                	ld	s6,0(sp)
 a78:	6121                	addi	sp,sp,64
 a7a:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 a7c:	6398                	ld	a4,0(a5)
 a7e:	e118                	sd	a4,0(a0)
 a80:	bff1                	j	a5c <malloc+0x8c>
  hp->s.size = nu;
 a82:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
 a86:	0541                	addi	a0,a0,16
 a88:	00000097          	auipc	ra,0x0
 a8c:	ec6080e7          	jalr	-314(ra) # 94e <free>
  return freep;
 a90:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
 a94:	d971                	beqz	a0,a68 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 a96:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
 a98:	4798                	lw	a4,8(a5)
 a9a:	fa9775e3          	bgeu	a4,s1,a44 <malloc+0x74>
    if(p == freep)
 a9e:	00093703          	ld	a4,0(s2)
 aa2:	853e                	mv	a0,a5
 aa4:	fef719e3          	bne	a4,a5,a96 <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
 aa8:	854e                	mv	a0,s3
 aaa:	00000097          	auipc	ra,0x0
 aae:	b34080e7          	jalr	-1228(ra) # 5de <sbrk>
  if(p == (char*)-1)
 ab2:	fd5518e3          	bne	a0,s5,a82 <malloc+0xb2>
        return 0;
 ab6:	4501                	li	a0,0
 ab8:	bf45                	j	a68 <malloc+0x98>
