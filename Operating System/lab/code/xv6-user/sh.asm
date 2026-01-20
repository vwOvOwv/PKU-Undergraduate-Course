
xv6-user/_sh:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <checkenvname>:

char mycwd[128];

int
checkenvname(char* s)
{
       0:	1141                	addi	sp,sp,-16
       2:	e422                	sd	s0,8(sp)
       4:	0800                	addi	s0,sp,16
       6:	88aa                	mv	a7,a0
  if((*s >= 'A' && *s <= 'Z') ||
       8:	00054703          	lbu	a4,0(a0)
       c:	fdf77793          	andi	a5,a4,-33
      10:	fbf7879b          	addiw	a5,a5,-65
      14:	0ff7f793          	zext.b	a5,a5
      18:	46e5                	li	a3,25
      1a:	00f6f763          	bgeu	a3,a5,28 <checkenvname+0x28>
     (*s >= 'a' && *s <= 'z') ||
      1e:	05f00793          	li	a5,95
      *s == '_')
    ;
  else
    return 0;
      22:	4501                	li	a0,0
     (*s >= 'a' && *s <= 'z') ||
      24:	02f71e63          	bne	a4,a5,60 <checkenvname+0x60>
  char *tmp = s + 1;
      28:	00188713          	addi	a4,a7,1
  while((*tmp >= 'A' && *tmp <= 'Z') ||
        (*tmp >= 'a' && *tmp <= 'z') ||
        (*tmp >= '0' && *tmp <= '9') ||
      2c:	4665                	li	a2,25
        (*tmp >= 'a' && *tmp <= 'z') ||
      2e:	45a5                	li	a1,9
        (*tmp >= '0' && *tmp <= '9') ||
      30:	05f00813          	li	a6,95
  while((*tmp >= 'A' && *tmp <= 'Z') ||
      34:	a011                	j	38 <checkenvname+0x38>
         *tmp == '_')
    tmp++;
      36:	0705                	addi	a4,a4,1
  while((*tmp >= 'A' && *tmp <= 'Z') ||
      38:	00074683          	lbu	a3,0(a4)
        (*tmp >= '0' && *tmp <= '9') ||
      3c:	fdf6f793          	andi	a5,a3,-33
      40:	fbf7879b          	addiw	a5,a5,-65
      44:	0ff7f793          	zext.b	a5,a5
      48:	fef677e3          	bgeu	a2,a5,36 <checkenvname+0x36>
      4c:	fd06879b          	addiw	a5,a3,-48
        (*tmp >= 'a' && *tmp <= 'z') ||
      50:	0ff7f793          	zext.b	a5,a5
      54:	fef5f1e3          	bgeu	a1,a5,36 <checkenvname+0x36>
        (*tmp >= '0' && *tmp <= '9') ||
      58:	fd068fe3          	beq	a3,a6,36 <checkenvname+0x36>
  return (int)(tmp - s);
      5c:	4117053b          	subw	a0,a4,a7
}
      60:	6422                	ld	s0,8(sp)
      62:	0141                	addi	sp,sp,16
      64:	8082                	ret

0000000000000066 <export>:

int
export(char *argv[])
{
      66:	7131                	addi	sp,sp,-192
      68:	fd06                	sd	ra,184(sp)
      6a:	f922                	sd	s0,176(sp)
      6c:	f526                	sd	s1,168(sp)
      6e:	f14a                	sd	s2,160(sp)
      70:	ed4e                	sd	s3,152(sp)
      72:	e952                	sd	s4,144(sp)
      74:	e556                	sd	s5,136(sp)
      76:	e15a                	sd	s6,128(sp)
      78:	0180                	addi	s0,sp,192
      7a:	84aa                	mv	s1,a0
  if(!strcmp(argv[1], "-p"))
      7c:	00001597          	auipc	a1,0x1
      80:	7dc58593          	addi	a1,a1,2012 # 1858 <malloc+0xea>
      84:	6508                	ld	a0,8(a0)
      86:	00001097          	auipc	ra,0x1
      8a:	000080e7          	jalr	ra # 1086 <strcmp>
      8e:	e12d                	bnez	a0,f0 <export+0x8a>
      90:	89aa                	mv	s3,a0
  { // print all the env vars
    if(!nenv)
      92:	00002b17          	auipc	s6,0x2
      96:	a2eb2b03          	lw	s6,-1490(s6) # 1ac0 <nenv>
      9a:	040b0163          	beqz	s6,dc <export+0x76>
    {
      printf("NO env var exported\n");
      return 0;
    }
    for(int i=0; i<nenv; i++)
      9e:	00002497          	auipc	s1,0x2
      a2:	b1248493          	addi	s1,s1,-1262 # 1bb0 <envs>
      a6:	892a                	mv	s2,a0
      printf("export %s=%s\n", envs[i].name, envs[i].value);
      a8:	00001a97          	auipc	s5,0x1
      ac:	7d0a8a93          	addi	s5,s5,2000 # 1878 <malloc+0x10a>
    for(int i=0; i<nenv; i++)
      b0:	00002a17          	auipc	s4,0x2
      b4:	a10a0a13          	addi	s4,s4,-1520 # 1ac0 <nenv>
      b8:	0f605c63          	blez	s6,1b0 <export+0x14a>
      printf("export %s=%s\n", envs[i].name, envs[i].value);
      bc:	02048613          	addi	a2,s1,32
      c0:	85a6                	mv	a1,s1
      c2:	8556                	mv	a0,s5
      c4:	00001097          	auipc	ra,0x1
      c8:	5f2080e7          	jalr	1522(ra) # 16b6 <printf>
    for(int i=0; i<nenv; i++)
      cc:	2905                	addiw	s2,s2,1
      ce:	08048493          	addi	s1,s1,128
      d2:	000a2783          	lw	a5,0(s4)
      d6:	fef943e3          	blt	s2,a5,bc <export+0x56>
      da:	a8d9                	j	1b0 <export+0x14a>
      printf("NO env var exported\n");
      dc:	00001517          	auipc	a0,0x1
      e0:	78450513          	addi	a0,a0,1924 # 1860 <malloc+0xf2>
      e4:	00001097          	auipc	ra,0x1
      e8:	5d2080e7          	jalr	1490(ra) # 16b6 <printf>
      return 0;
      ec:	89da                	mv	s3,s6
      ee:	a0c9                	j	1b0 <export+0x14a>
    return 0;
  }
  else if(nenv == NENVS)
      f0:	00002997          	auipc	s3,0x2
      f4:	9d09a983          	lw	s3,-1584(s3) # 1ac0 <nenv>
      f8:	47c1                	li	a5,16
      fa:	0cf98663          	beq	s3,a5,1c6 <export+0x160>
  {
    fprintf(2, "too many env vars\n");
    return -1;
  }
  char name[32], value[96];
  char *s = argv[1], *t = name;
      fe:	0084b903          	ld	s2,8(s1)

  for(s=argv[1], t=name; (*t=*s++)!='='; t++)
     102:	00190493          	addi	s1,s2,1
     106:	00094783          	lbu	a5,0(s2)
     10a:	faf40023          	sb	a5,-96(s0)
     10e:	03d00713          	li	a4,61
     112:	0ce78563          	beq	a5,a4,1dc <export+0x176>
     116:	fa040793          	addi	a5,s0,-96
     11a:	03d00693          	li	a3,61
     11e:	0785                	addi	a5,a5,1
     120:	0485                	addi	s1,s1,1
     122:	fff4c703          	lbu	a4,-1(s1)
     126:	00e78023          	sb	a4,0(a5)
     12a:	fed71ae3          	bne	a4,a3,11e <export+0xb8>
    ;
  *t = 0;
     12e:	00078023          	sb	zero,0(a5)

  if(checkenvname(name) != ((s - argv[1]) - 1))
     132:	fa040513          	addi	a0,s0,-96
     136:	00000097          	auipc	ra,0x0
     13a:	eca080e7          	jalr	-310(ra) # 0 <checkenvname>
     13e:	41248933          	sub	s2,s1,s2
     142:	197d                	addi	s2,s2,-1
     144:	09251f63          	bne	a0,s2,1e2 <export+0x17c>
  {
    fprintf(2, "Invalid NAME!\n");
    return -1;
  }
  for(t=value; (*t=*s); s++, t++)
     148:	0004c703          	lbu	a4,0(s1)
     14c:	f4e40023          	sb	a4,-192(s0)
     150:	f4040793          	addi	a5,s0,-192
     154:	cb01                	beqz	a4,164 <export+0xfe>
     156:	0485                	addi	s1,s1,1
     158:	0785                	addi	a5,a5,1
     15a:	0004c703          	lbu	a4,0(s1)
     15e:	00e78023          	sb	a4,0(a5)
     162:	fb75                	bnez	a4,156 <export+0xf0>
    ;
  if(*--t == '/')
     164:	fff7c683          	lbu	a3,-1(a5)
     168:	02f00713          	li	a4,47
     16c:	08e68663          	beq	a3,a4,1f8 <export+0x192>
    *t = 0;
  
  strcpy(envs[nenv].name, name);
     170:	00799513          	slli	a0,s3,0x7
     174:	00002917          	auipc	s2,0x2
     178:	a3c90913          	addi	s2,s2,-1476 # 1bb0 <envs>
     17c:	fa040593          	addi	a1,s0,-96
     180:	954a                	add	a0,a0,s2
     182:	00001097          	auipc	ra,0x1
     186:	eba080e7          	jalr	-326(ra) # 103c <strcpy>
  strcpy(envs[nenv].value, value);
     18a:	00002497          	auipc	s1,0x2
     18e:	93648493          	addi	s1,s1,-1738 # 1ac0 <nenv>
     192:	4088                	lw	a0,0(s1)
     194:	051e                	slli	a0,a0,0x7
     196:	02050513          	addi	a0,a0,32
     19a:	f4040593          	addi	a1,s0,-192
     19e:	954a                	add	a0,a0,s2
     1a0:	00001097          	auipc	ra,0x1
     1a4:	e9c080e7          	jalr	-356(ra) # 103c <strcpy>
  nenv++;
     1a8:	409c                	lw	a5,0(s1)
     1aa:	2785                	addiw	a5,a5,1
     1ac:	c09c                	sw	a5,0(s1)
  return 0;
     1ae:	4981                	li	s3,0
}
     1b0:	854e                	mv	a0,s3
     1b2:	70ea                	ld	ra,184(sp)
     1b4:	744a                	ld	s0,176(sp)
     1b6:	74aa                	ld	s1,168(sp)
     1b8:	790a                	ld	s2,160(sp)
     1ba:	69ea                	ld	s3,152(sp)
     1bc:	6a4a                	ld	s4,144(sp)
     1be:	6aaa                	ld	s5,136(sp)
     1c0:	6b0a                	ld	s6,128(sp)
     1c2:	6129                	addi	sp,sp,192
     1c4:	8082                	ret
    fprintf(2, "too many env vars\n");
     1c6:	00001597          	auipc	a1,0x1
     1ca:	6c258593          	addi	a1,a1,1730 # 1888 <malloc+0x11a>
     1ce:	4509                	li	a0,2
     1d0:	00001097          	auipc	ra,0x1
     1d4:	4b8080e7          	jalr	1208(ra) # 1688 <fprintf>
    return -1;
     1d8:	59fd                	li	s3,-1
     1da:	bfd9                	j	1b0 <export+0x14a>
  for(s=argv[1], t=name; (*t=*s++)!='='; t++)
     1dc:	fa040793          	addi	a5,s0,-96
     1e0:	b7b9                	j	12e <export+0xc8>
    fprintf(2, "Invalid NAME!\n");
     1e2:	00001597          	auipc	a1,0x1
     1e6:	6be58593          	addi	a1,a1,1726 # 18a0 <malloc+0x132>
     1ea:	4509                	li	a0,2
     1ec:	00001097          	auipc	ra,0x1
     1f0:	49c080e7          	jalr	1180(ra) # 1688 <fprintf>
    return -1;
     1f4:	59fd                	li	s3,-1
     1f6:	bf6d                	j	1b0 <export+0x14a>
    *t = 0;
     1f8:	fe078fa3          	sb	zero,-1(a5)
     1fc:	bf95                	j	170 <export+0x10a>

00000000000001fe <replace>:

int
replace(char *buf)
{
     1fe:	7111                	addi	sp,sp,-256
     200:	fd86                	sd	ra,248(sp)
     202:	f9a2                	sd	s0,240(sp)
     204:	f5a6                	sd	s1,232(sp)
     206:	f1ca                	sd	s2,224(sp)
     208:	edce                	sd	s3,216(sp)
     20a:	e9d2                	sd	s4,208(sp)
     20c:	e5d6                	sd	s5,200(sp)
     20e:	e1da                	sd	s6,192(sp)
     210:	fd5e                	sd	s7,184(sp)
     212:	f962                	sd	s8,176(sp)
     214:	f566                	sd	s9,168(sp)
     216:	f16a                	sd	s10,160(sp)
     218:	ed6e                	sd	s11,152(sp)
     21a:	0200                	addi	s0,sp,256
     21c:	89aa                	mv	s3,a0
  char raw[100], name[32], *s, *t, *tmp;
  int n = 0;
  strcpy(raw, buf);
     21e:	85aa                	mv	a1,a0
     220:	f2840513          	addi	a0,s0,-216
     224:	00001097          	auipc	ra,0x1
     228:	e18080e7          	jalr	-488(ra) # 103c <strcpy>
  for(s=raw, t=buf; (*t=*s); t++)
     22c:	f2844783          	lbu	a5,-216(s0)
     230:	00f98023          	sb	a5,0(s3)
     234:	c7f5                	beqz	a5,320 <replace+0x122>
  int n = 0;
     236:	4c81                	li	s9,0
  for(s=raw, t=buf; (*t=*s); t++)
     238:	f2840713          	addi	a4,s0,-216
  {
    if(*s++ == '$'){
     23c:	02400c13          	li	s8,36
      tmp = name;
      if((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || *s == '_')
     240:	4965                	li	s2,25
      {
        *tmp++ = *s++;
        while((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_')
     242:	4a25                	li	s4,9
     244:	05f00a93          	li	s5,95
          *tmp++ = *s++;
        *tmp = 0;
        for(int i=0; i<nenv; i++)
     248:	00002b17          	auipc	s6,0x2
     24c:	878b0b13          	addi	s6,s6,-1928 # 1ac0 <nenv>
          if(!strcmp(name, envs[i].name))
     250:	f0840b93          	addi	s7,s0,-248
     254:	a03d                	j	282 <replace+0x84>
      if((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || *s == '_')
     256:	00174683          	lbu	a3,1(a4)
     25a:	fdf6f793          	andi	a5,a3,-33
     25e:	fbf7879b          	addiw	a5,a5,-65
     262:	0ff7f793          	zext.b	a5,a5
     266:	02f97563          	bgeu	s2,a5,290 <replace+0x92>
     26a:	03568363          	beq	a3,s5,290 <replace+0x92>
            for(tmp=envs[i].value; (*t=*tmp); t++, tmp++)
              ;
        t--;
      }
      n++;
     26e:	2c85                	addiw	s9,s9,1
     270:	86ce                	mv	a3,s3
     272:	8726                	mv	a4,s1
  for(s=raw, t=buf; (*t=*s); t++)
     274:	00168993          	addi	s3,a3,1
     278:	00074783          	lbu	a5,0(a4)
     27c:	00f680a3          	sb	a5,1(a3)
     280:	c3cd                	beqz	a5,322 <replace+0x124>
    if(*s++ == '$'){
     282:	00170493          	addi	s1,a4,1
     286:	fd8788e3          	beq	a5,s8,256 <replace+0x58>
     28a:	86ce                	mv	a3,s3
     28c:	8726                	mv	a4,s1
     28e:	b7dd                	j	274 <replace+0x76>
        *tmp++ = *s++;
     290:	00270493          	addi	s1,a4,2
     294:	f0d40423          	sb	a3,-248(s0)
     298:	f0940693          	addi	a3,s0,-247
        while((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_')
     29c:	a029                	j	2a6 <replace+0xa8>
          *tmp++ = *s++;
     29e:	0485                	addi	s1,s1,1
     2a0:	0685                	addi	a3,a3,1
     2a2:	fee68fa3          	sb	a4,-1(a3)
        while((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_')
     2a6:	0004c703          	lbu	a4,0(s1)
     2aa:	fdf77793          	andi	a5,a4,-33
     2ae:	fbf7879b          	addiw	a5,a5,-65
     2b2:	0ff7f793          	zext.b	a5,a5
     2b6:	fef974e3          	bgeu	s2,a5,29e <replace+0xa0>
     2ba:	fd07079b          	addiw	a5,a4,-48
     2be:	0ff7f793          	zext.b	a5,a5
     2c2:	fcfa7ee3          	bgeu	s4,a5,29e <replace+0xa0>
     2c6:	fd570ce3          	beq	a4,s5,29e <replace+0xa0>
        *tmp = 0;
     2ca:	00068023          	sb	zero,0(a3)
        for(int i=0; i<nenv; i++)
     2ce:	000b2783          	lw	a5,0(s6)
     2d2:	04f05563          	blez	a5,31c <replace+0x11e>
     2d6:	00002d17          	auipc	s10,0x2
     2da:	8fad0d13          	addi	s10,s10,-1798 # 1bd0 <envs+0x20>
     2de:	4d81                	li	s11,0
     2e0:	a801                	j	2f0 <replace+0xf2>
     2e2:	2d85                	addiw	s11,s11,1
     2e4:	080d0d13          	addi	s10,s10,128
     2e8:	000b2783          	lw	a5,0(s6)
     2ec:	02fdd863          	bge	s11,a5,31c <replace+0x11e>
          if(!strcmp(name, envs[i].name))
     2f0:	fe0d0593          	addi	a1,s10,-32
     2f4:	855e                	mv	a0,s7
     2f6:	00001097          	auipc	ra,0x1
     2fa:	d90080e7          	jalr	-624(ra) # 1086 <strcmp>
     2fe:	f175                	bnez	a0,2e2 <replace+0xe4>
            for(tmp=envs[i].value; (*t=*tmp); t++, tmp++)
     300:	87ea                	mv	a5,s10
     302:	000d4703          	lbu	a4,0(s10)
     306:	00e98023          	sb	a4,0(s3)
     30a:	df61                	beqz	a4,2e2 <replace+0xe4>
     30c:	0985                	addi	s3,s3,1
     30e:	0785                	addi	a5,a5,1
     310:	0007c703          	lbu	a4,0(a5)
     314:	00e98023          	sb	a4,0(s3)
     318:	fb75                	bnez	a4,30c <replace+0x10e>
     31a:	b7e1                	j	2e2 <replace+0xe4>
        t--;
     31c:	19fd                	addi	s3,s3,-1
     31e:	bf81                	j	26e <replace+0x70>
  int n = 0;
     320:	4c81                	li	s9,0
    }
  }
  return n;
}
     322:	8566                	mv	a0,s9
     324:	70ee                	ld	ra,248(sp)
     326:	744e                	ld	s0,240(sp)
     328:	74ae                	ld	s1,232(sp)
     32a:	790e                	ld	s2,224(sp)
     32c:	69ee                	ld	s3,216(sp)
     32e:	6a4e                	ld	s4,208(sp)
     330:	6aae                	ld	s5,200(sp)
     332:	6b0e                	ld	s6,192(sp)
     334:	7bea                	ld	s7,184(sp)
     336:	7c4a                	ld	s8,176(sp)
     338:	7caa                	ld	s9,168(sp)
     33a:	7d0a                	ld	s10,160(sp)
     33c:	6dea                	ld	s11,152(sp)
     33e:	6111                	addi	sp,sp,256
     340:	8082                	ret

0000000000000342 <getcmd>:
  exit(0);
}

int
getcmd(char *buf, int nbuf)
{
     342:	1101                	addi	sp,sp,-32
     344:	ec06                	sd	ra,24(sp)
     346:	e822                	sd	s0,16(sp)
     348:	e426                	sd	s1,8(sp)
     34a:	e04a                	sd	s2,0(sp)
     34c:	1000                	addi	s0,sp,32
     34e:	84aa                	mv	s1,a0
     350:	892e                	mv	s2,a1
  fprintf(2, "-> %s $ ", mycwd);
     352:	00001617          	auipc	a2,0x1
     356:	77660613          	addi	a2,a2,1910 # 1ac8 <mycwd>
     35a:	00001597          	auipc	a1,0x1
     35e:	55658593          	addi	a1,a1,1366 # 18b0 <malloc+0x142>
     362:	4509                	li	a0,2
     364:	00001097          	auipc	ra,0x1
     368:	324080e7          	jalr	804(ra) # 1688 <fprintf>
  memset(buf, 0, nbuf);
     36c:	864a                	mv	a2,s2
     36e:	4581                	li	a1,0
     370:	8526                	mv	a0,s1
     372:	00001097          	auipc	ra,0x1
     376:	d6a080e7          	jalr	-662(ra) # 10dc <memset>
  gets(buf, nbuf);
     37a:	85ca                	mv	a1,s2
     37c:	8526                	mv	a0,s1
     37e:	00001097          	auipc	ra,0x1
     382:	da4080e7          	jalr	-604(ra) # 1122 <gets>
  if(buf[0] == 0) // EOF
     386:	0004c503          	lbu	a0,0(s1)
     38a:	00153513          	seqz	a0,a0
    return -1;
  return 0;
}
     38e:	40a00533          	neg	a0,a0
     392:	60e2                	ld	ra,24(sp)
     394:	6442                	ld	s0,16(sp)
     396:	64a2                	ld	s1,8(sp)
     398:	6902                	ld	s2,0(sp)
     39a:	6105                	addi	sp,sp,32
     39c:	8082                	ret

000000000000039e <panic>:
  exit(0);
}

void
panic(char *s)
{
     39e:	1141                	addi	sp,sp,-16
     3a0:	e406                	sd	ra,8(sp)
     3a2:	e022                	sd	s0,0(sp)
     3a4:	0800                	addi	s0,sp,16
     3a6:	862a                	mv	a2,a0
  fprintf(2, "%s\n", s);
     3a8:	00001597          	auipc	a1,0x1
     3ac:	51858593          	addi	a1,a1,1304 # 18c0 <malloc+0x152>
     3b0:	4509                	li	a0,2
     3b2:	00001097          	auipc	ra,0x1
     3b6:	2d6080e7          	jalr	726(ra) # 1688 <fprintf>
  exit(1);
     3ba:	4505                	li	a0,1
     3bc:	00001097          	auipc	ra,0x1
     3c0:	f38080e7          	jalr	-200(ra) # 12f4 <exit>

00000000000003c4 <fork1>:
}

int
fork1(void)
{
     3c4:	1141                	addi	sp,sp,-16
     3c6:	e406                	sd	ra,8(sp)
     3c8:	e022                	sd	s0,0(sp)
     3ca:	0800                	addi	s0,sp,16
  int pid;

  pid = fork();
     3cc:	00001097          	auipc	ra,0x1
     3d0:	f20080e7          	jalr	-224(ra) # 12ec <fork>
  if(pid == -1)
     3d4:	57fd                	li	a5,-1
     3d6:	00f50663          	beq	a0,a5,3e2 <fork1+0x1e>
    panic("fork");
  return pid;
}
     3da:	60a2                	ld	ra,8(sp)
     3dc:	6402                	ld	s0,0(sp)
     3de:	0141                	addi	sp,sp,16
     3e0:	8082                	ret
    panic("fork");
     3e2:	00001517          	auipc	a0,0x1
     3e6:	4e650513          	addi	a0,a0,1254 # 18c8 <malloc+0x15a>
     3ea:	00000097          	auipc	ra,0x0
     3ee:	fb4080e7          	jalr	-76(ra) # 39e <panic>

00000000000003f2 <runcmd>:
{
     3f2:	7135                	addi	sp,sp,-160
     3f4:	ed06                	sd	ra,152(sp)
     3f6:	e922                	sd	s0,144(sp)
     3f8:	e526                	sd	s1,136(sp)
     3fa:	e14a                	sd	s2,128(sp)
     3fc:	fcce                	sd	s3,120(sp)
     3fe:	f8d2                	sd	s4,112(sp)
     400:	f4d6                	sd	s5,104(sp)
     402:	f0da                	sd	s6,96(sp)
     404:	ecde                	sd	s7,88(sp)
     406:	1100                	addi	s0,sp,160
  if(cmd == 0)
     408:	c10d                	beqz	a0,42a <runcmd+0x38>
     40a:	84aa                	mv	s1,a0
  switch(cmd->type){
     40c:	4118                	lw	a4,0(a0)
     40e:	4795                	li	a5,5
     410:	02e7e263          	bltu	a5,a4,434 <runcmd+0x42>
     414:	00056783          	lwu	a5,0(a0)
     418:	078a                	slli	a5,a5,0x2
     41a:	00001717          	auipc	a4,0x1
     41e:	5ee70713          	addi	a4,a4,1518 # 1a08 <malloc+0x29a>
     422:	97ba                	add	a5,a5,a4
     424:	439c                	lw	a5,0(a5)
     426:	97ba                	add	a5,a5,a4
     428:	8782                	jr	a5
    exit(1);
     42a:	4505                	li	a0,1
     42c:	00001097          	auipc	ra,0x1
     430:	ec8080e7          	jalr	-312(ra) # 12f4 <exit>
    panic("runcmd");
     434:	00001517          	auipc	a0,0x1
     438:	49c50513          	addi	a0,a0,1180 # 18d0 <malloc+0x162>
     43c:	00000097          	auipc	ra,0x0
     440:	f62080e7          	jalr	-158(ra) # 39e <panic>
    if(ecmd->argv[0] == 0)
     444:	6508                	ld	a0,8(a0)
     446:	c155                	beqz	a0,4ea <runcmd+0xf8>
    exec(ecmd->argv[0], ecmd->argv);
     448:	00848a13          	addi	s4,s1,8
     44c:	85d2                	mv	a1,s4
     44e:	00001097          	auipc	ra,0x1
     452:	eea080e7          	jalr	-278(ra) # 1338 <exec>
    for(i=0; i<nenv; i++)
     456:	00001797          	auipc	a5,0x1
     45a:	66a7a783          	lw	a5,1642(a5) # 1ac0 <nenv>
     45e:	06f05763          	blez	a5,4cc <runcmd+0xda>
     462:	00001917          	auipc	s2,0x1
     466:	76f90913          	addi	s2,s2,1903 # 1bd1 <envs+0x21>
     46a:	4981                	li	s3,0
      *s_tmp++ = '/';
     46c:	02f00b93          	li	s7,47
      exec(env_cmd, ecmd->argv);
     470:	f6840b13          	addi	s6,s0,-152
    for(i=0; i<nenv; i++)
     474:	00001a97          	auipc	s5,0x1
     478:	64ca8a93          	addi	s5,s5,1612 # 1ac0 <nenv>
      while((*s_tmp = *d_tmp++))
     47c:	874a                	mv	a4,s2
     47e:	fff94783          	lbu	a5,-1(s2)
     482:	f6f40423          	sb	a5,-152(s0)
     486:	c7bd                	beqz	a5,4f4 <runcmd+0x102>
      char *s_tmp = env_cmd;
     488:	f6840793          	addi	a5,s0,-152
        s_tmp++;
     48c:	0785                	addi	a5,a5,1
      while((*s_tmp = *d_tmp++))
     48e:	0705                	addi	a4,a4,1
     490:	fff74683          	lbu	a3,-1(a4)
     494:	00d78023          	sb	a3,0(a5)
     498:	faf5                	bnez	a3,48c <runcmd+0x9a>
      *s_tmp++ = '/';
     49a:	00178713          	addi	a4,a5,1
     49e:	01778023          	sb	s7,0(a5)
      d_tmp = ecmd->argv[0];
     4a2:	649c                	ld	a5,8(s1)
      while((*s_tmp++ = *d_tmp++))
     4a4:	0785                	addi	a5,a5,1
     4a6:	0705                	addi	a4,a4,1
     4a8:	fff7c683          	lbu	a3,-1(a5)
     4ac:	fed70fa3          	sb	a3,-1(a4)
     4b0:	faf5                	bnez	a3,4a4 <runcmd+0xb2>
      exec(env_cmd, ecmd->argv);
     4b2:	85d2                	mv	a1,s4
     4b4:	855a                	mv	a0,s6
     4b6:	00001097          	auipc	ra,0x1
     4ba:	e82080e7          	jalr	-382(ra) # 1338 <exec>
    for(i=0; i<nenv; i++)
     4be:	2985                	addiw	s3,s3,1
     4c0:	08090913          	addi	s2,s2,128
     4c4:	000aa783          	lw	a5,0(s5)
     4c8:	faf9cae3          	blt	s3,a5,47c <runcmd+0x8a>
    fprintf(2, "exec %s failed\n", ecmd->argv[0]);
     4cc:	6490                	ld	a2,8(s1)
     4ce:	00001597          	auipc	a1,0x1
     4d2:	40a58593          	addi	a1,a1,1034 # 18d8 <malloc+0x16a>
     4d6:	4509                	li	a0,2
     4d8:	00001097          	auipc	ra,0x1
     4dc:	1b0080e7          	jalr	432(ra) # 1688 <fprintf>
  exit(0);
     4e0:	4501                	li	a0,0
     4e2:	00001097          	auipc	ra,0x1
     4e6:	e12080e7          	jalr	-494(ra) # 12f4 <exit>
      exit(1);
     4ea:	4505                	li	a0,1
     4ec:	00001097          	auipc	ra,0x1
     4f0:	e08080e7          	jalr	-504(ra) # 12f4 <exit>
      char *s_tmp = env_cmd;
     4f4:	f6840793          	addi	a5,s0,-152
     4f8:	b74d                	j	49a <runcmd+0xa8>
    close(rcmd->fd);
     4fa:	5148                	lw	a0,36(a0)
     4fc:	00001097          	auipc	ra,0x1
     500:	e2a080e7          	jalr	-470(ra) # 1326 <close>
    if(open(rcmd->file, rcmd->mode) < 0){
     504:	508c                	lw	a1,32(s1)
     506:	6888                	ld	a0,16(s1)
     508:	00001097          	auipc	ra,0x1
     50c:	e3a080e7          	jalr	-454(ra) # 1342 <open>
     510:	00054763          	bltz	a0,51e <runcmd+0x12c>
    runcmd(rcmd->cmd);
     514:	6488                	ld	a0,8(s1)
     516:	00000097          	auipc	ra,0x0
     51a:	edc080e7          	jalr	-292(ra) # 3f2 <runcmd>
      fprintf(2, "open %s failed\n", rcmd->file);
     51e:	6890                	ld	a2,16(s1)
     520:	00001597          	auipc	a1,0x1
     524:	3c858593          	addi	a1,a1,968 # 18e8 <malloc+0x17a>
     528:	4509                	li	a0,2
     52a:	00001097          	auipc	ra,0x1
     52e:	15e080e7          	jalr	350(ra) # 1688 <fprintf>
      exit(1);
     532:	4505                	li	a0,1
     534:	00001097          	auipc	ra,0x1
     538:	dc0080e7          	jalr	-576(ra) # 12f4 <exit>
    if(fork1() == 0)
     53c:	00000097          	auipc	ra,0x0
     540:	e88080e7          	jalr	-376(ra) # 3c4 <fork1>
     544:	c919                	beqz	a0,55a <runcmd+0x168>
    wait(0);
     546:	4501                	li	a0,0
     548:	00001097          	auipc	ra,0x1
     54c:	db6080e7          	jalr	-586(ra) # 12fe <wait>
    runcmd(lcmd->right);
     550:	6888                	ld	a0,16(s1)
     552:	00000097          	auipc	ra,0x0
     556:	ea0080e7          	jalr	-352(ra) # 3f2 <runcmd>
      runcmd(lcmd->left);
     55a:	6488                	ld	a0,8(s1)
     55c:	00000097          	auipc	ra,0x0
     560:	e96080e7          	jalr	-362(ra) # 3f2 <runcmd>
    if(pipe(p) < 0)
     564:	fa840513          	addi	a0,s0,-88
     568:	00001097          	auipc	ra,0x1
     56c:	da0080e7          	jalr	-608(ra) # 1308 <pipe>
     570:	04054363          	bltz	a0,5b6 <runcmd+0x1c4>
    if(fork1() == 0){
     574:	00000097          	auipc	ra,0x0
     578:	e50080e7          	jalr	-432(ra) # 3c4 <fork1>
     57c:	c529                	beqz	a0,5c6 <runcmd+0x1d4>
    if(fork1() == 0){
     57e:	00000097          	auipc	ra,0x0
     582:	e46080e7          	jalr	-442(ra) # 3c4 <fork1>
     586:	cd25                	beqz	a0,5fe <runcmd+0x20c>
    close(p[0]);
     588:	fa842503          	lw	a0,-88(s0)
     58c:	00001097          	auipc	ra,0x1
     590:	d9a080e7          	jalr	-614(ra) # 1326 <close>
    close(p[1]);
     594:	fac42503          	lw	a0,-84(s0)
     598:	00001097          	auipc	ra,0x1
     59c:	d8e080e7          	jalr	-626(ra) # 1326 <close>
    wait(0);
     5a0:	4501                	li	a0,0
     5a2:	00001097          	auipc	ra,0x1
     5a6:	d5c080e7          	jalr	-676(ra) # 12fe <wait>
    wait(0);
     5aa:	4501                	li	a0,0
     5ac:	00001097          	auipc	ra,0x1
     5b0:	d52080e7          	jalr	-686(ra) # 12fe <wait>
    break;
     5b4:	b735                	j	4e0 <runcmd+0xee>
      panic("pipe");
     5b6:	00001517          	auipc	a0,0x1
     5ba:	34250513          	addi	a0,a0,834 # 18f8 <malloc+0x18a>
     5be:	00000097          	auipc	ra,0x0
     5c2:	de0080e7          	jalr	-544(ra) # 39e <panic>
      close(1);
     5c6:	4505                	li	a0,1
     5c8:	00001097          	auipc	ra,0x1
     5cc:	d5e080e7          	jalr	-674(ra) # 1326 <close>
      dup(p[1]);
     5d0:	fac42503          	lw	a0,-84(s0)
     5d4:	00001097          	auipc	ra,0x1
     5d8:	d96080e7          	jalr	-618(ra) # 136a <dup>
      close(p[0]);
     5dc:	fa842503          	lw	a0,-88(s0)
     5e0:	00001097          	auipc	ra,0x1
     5e4:	d46080e7          	jalr	-698(ra) # 1326 <close>
      close(p[1]);
     5e8:	fac42503          	lw	a0,-84(s0)
     5ec:	00001097          	auipc	ra,0x1
     5f0:	d3a080e7          	jalr	-710(ra) # 1326 <close>
      runcmd(pcmd->left);
     5f4:	6488                	ld	a0,8(s1)
     5f6:	00000097          	auipc	ra,0x0
     5fa:	dfc080e7          	jalr	-516(ra) # 3f2 <runcmd>
      close(0);
     5fe:	00001097          	auipc	ra,0x1
     602:	d28080e7          	jalr	-728(ra) # 1326 <close>
      dup(p[0]);
     606:	fa842503          	lw	a0,-88(s0)
     60a:	00001097          	auipc	ra,0x1
     60e:	d60080e7          	jalr	-672(ra) # 136a <dup>
      close(p[0]);
     612:	fa842503          	lw	a0,-88(s0)
     616:	00001097          	auipc	ra,0x1
     61a:	d10080e7          	jalr	-752(ra) # 1326 <close>
      close(p[1]);
     61e:	fac42503          	lw	a0,-84(s0)
     622:	00001097          	auipc	ra,0x1
     626:	d04080e7          	jalr	-764(ra) # 1326 <close>
      runcmd(pcmd->right);
     62a:	6888                	ld	a0,16(s1)
     62c:	00000097          	auipc	ra,0x0
     630:	dc6080e7          	jalr	-570(ra) # 3f2 <runcmd>
    if(fork1() == 0)
     634:	00000097          	auipc	ra,0x0
     638:	d90080e7          	jalr	-624(ra) # 3c4 <fork1>
     63c:	ea0512e3          	bnez	a0,4e0 <runcmd+0xee>
      runcmd(bcmd->cmd);
     640:	6488                	ld	a0,8(s1)
     642:	00000097          	auipc	ra,0x0
     646:	db0080e7          	jalr	-592(ra) # 3f2 <runcmd>

000000000000064a <execcmd>:
//PAGEBREAK!
// Constructors

struct cmd*
execcmd(void)
{
     64a:	1101                	addi	sp,sp,-32
     64c:	ec06                	sd	ra,24(sp)
     64e:	e822                	sd	s0,16(sp)
     650:	e426                	sd	s1,8(sp)
     652:	1000                	addi	s0,sp,32
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
     654:	0a800513          	li	a0,168
     658:	00001097          	auipc	ra,0x1
     65c:	116080e7          	jalr	278(ra) # 176e <malloc>
     660:	84aa                	mv	s1,a0
  memset(cmd, 0, sizeof(*cmd));
     662:	0a800613          	li	a2,168
     666:	4581                	li	a1,0
     668:	00001097          	auipc	ra,0x1
     66c:	a74080e7          	jalr	-1420(ra) # 10dc <memset>
  cmd->type = EXEC;
     670:	4785                	li	a5,1
     672:	c09c                	sw	a5,0(s1)
  return (struct cmd*)cmd;
}
     674:	8526                	mv	a0,s1
     676:	60e2                	ld	ra,24(sp)
     678:	6442                	ld	s0,16(sp)
     67a:	64a2                	ld	s1,8(sp)
     67c:	6105                	addi	sp,sp,32
     67e:	8082                	ret

0000000000000680 <redircmd>:

struct cmd*
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
     680:	7139                	addi	sp,sp,-64
     682:	fc06                	sd	ra,56(sp)
     684:	f822                	sd	s0,48(sp)
     686:	f426                	sd	s1,40(sp)
     688:	f04a                	sd	s2,32(sp)
     68a:	ec4e                	sd	s3,24(sp)
     68c:	e852                	sd	s4,16(sp)
     68e:	e456                	sd	s5,8(sp)
     690:	e05a                	sd	s6,0(sp)
     692:	0080                	addi	s0,sp,64
     694:	8b2a                	mv	s6,a0
     696:	8aae                	mv	s5,a1
     698:	8a32                	mv	s4,a2
     69a:	89b6                	mv	s3,a3
     69c:	893a                	mv	s2,a4
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
     69e:	02800513          	li	a0,40
     6a2:	00001097          	auipc	ra,0x1
     6a6:	0cc080e7          	jalr	204(ra) # 176e <malloc>
     6aa:	84aa                	mv	s1,a0
  memset(cmd, 0, sizeof(*cmd));
     6ac:	02800613          	li	a2,40
     6b0:	4581                	li	a1,0
     6b2:	00001097          	auipc	ra,0x1
     6b6:	a2a080e7          	jalr	-1494(ra) # 10dc <memset>
  cmd->type = REDIR;
     6ba:	4789                	li	a5,2
     6bc:	c09c                	sw	a5,0(s1)
  cmd->cmd = subcmd;
     6be:	0164b423          	sd	s6,8(s1)
  cmd->file = file;
     6c2:	0154b823          	sd	s5,16(s1)
  cmd->efile = efile;
     6c6:	0144bc23          	sd	s4,24(s1)
  cmd->mode = mode;
     6ca:	0334a023          	sw	s3,32(s1)
  cmd->fd = fd;
     6ce:	0324a223          	sw	s2,36(s1)
  return (struct cmd*)cmd;
}
     6d2:	8526                	mv	a0,s1
     6d4:	70e2                	ld	ra,56(sp)
     6d6:	7442                	ld	s0,48(sp)
     6d8:	74a2                	ld	s1,40(sp)
     6da:	7902                	ld	s2,32(sp)
     6dc:	69e2                	ld	s3,24(sp)
     6de:	6a42                	ld	s4,16(sp)
     6e0:	6aa2                	ld	s5,8(sp)
     6e2:	6b02                	ld	s6,0(sp)
     6e4:	6121                	addi	sp,sp,64
     6e6:	8082                	ret

00000000000006e8 <pipecmd>:

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
     6e8:	7179                	addi	sp,sp,-48
     6ea:	f406                	sd	ra,40(sp)
     6ec:	f022                	sd	s0,32(sp)
     6ee:	ec26                	sd	s1,24(sp)
     6f0:	e84a                	sd	s2,16(sp)
     6f2:	e44e                	sd	s3,8(sp)
     6f4:	1800                	addi	s0,sp,48
     6f6:	89aa                	mv	s3,a0
     6f8:	892e                	mv	s2,a1
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
     6fa:	4561                	li	a0,24
     6fc:	00001097          	auipc	ra,0x1
     700:	072080e7          	jalr	114(ra) # 176e <malloc>
     704:	84aa                	mv	s1,a0
  memset(cmd, 0, sizeof(*cmd));
     706:	4661                	li	a2,24
     708:	4581                	li	a1,0
     70a:	00001097          	auipc	ra,0x1
     70e:	9d2080e7          	jalr	-1582(ra) # 10dc <memset>
  cmd->type = PIPE;
     712:	478d                	li	a5,3
     714:	c09c                	sw	a5,0(s1)
  cmd->left = left;
     716:	0134b423          	sd	s3,8(s1)
  cmd->right = right;
     71a:	0124b823          	sd	s2,16(s1)
  return (struct cmd*)cmd;
}
     71e:	8526                	mv	a0,s1
     720:	70a2                	ld	ra,40(sp)
     722:	7402                	ld	s0,32(sp)
     724:	64e2                	ld	s1,24(sp)
     726:	6942                	ld	s2,16(sp)
     728:	69a2                	ld	s3,8(sp)
     72a:	6145                	addi	sp,sp,48
     72c:	8082                	ret

000000000000072e <listcmd>:

struct cmd*
listcmd(struct cmd *left, struct cmd *right)
{
     72e:	7179                	addi	sp,sp,-48
     730:	f406                	sd	ra,40(sp)
     732:	f022                	sd	s0,32(sp)
     734:	ec26                	sd	s1,24(sp)
     736:	e84a                	sd	s2,16(sp)
     738:	e44e                	sd	s3,8(sp)
     73a:	1800                	addi	s0,sp,48
     73c:	89aa                	mv	s3,a0
     73e:	892e                	mv	s2,a1
  struct listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
     740:	4561                	li	a0,24
     742:	00001097          	auipc	ra,0x1
     746:	02c080e7          	jalr	44(ra) # 176e <malloc>
     74a:	84aa                	mv	s1,a0
  memset(cmd, 0, sizeof(*cmd));
     74c:	4661                	li	a2,24
     74e:	4581                	li	a1,0
     750:	00001097          	auipc	ra,0x1
     754:	98c080e7          	jalr	-1652(ra) # 10dc <memset>
  cmd->type = LIST;
     758:	4791                	li	a5,4
     75a:	c09c                	sw	a5,0(s1)
  cmd->left = left;
     75c:	0134b423          	sd	s3,8(s1)
  cmd->right = right;
     760:	0124b823          	sd	s2,16(s1)
  return (struct cmd*)cmd;
}
     764:	8526                	mv	a0,s1
     766:	70a2                	ld	ra,40(sp)
     768:	7402                	ld	s0,32(sp)
     76a:	64e2                	ld	s1,24(sp)
     76c:	6942                	ld	s2,16(sp)
     76e:	69a2                	ld	s3,8(sp)
     770:	6145                	addi	sp,sp,48
     772:	8082                	ret

0000000000000774 <backcmd>:

struct cmd*
backcmd(struct cmd *subcmd)
{
     774:	1101                	addi	sp,sp,-32
     776:	ec06                	sd	ra,24(sp)
     778:	e822                	sd	s0,16(sp)
     77a:	e426                	sd	s1,8(sp)
     77c:	e04a                	sd	s2,0(sp)
     77e:	1000                	addi	s0,sp,32
     780:	892a                	mv	s2,a0
  struct backcmd *cmd;

  cmd = malloc(sizeof(*cmd));
     782:	4541                	li	a0,16
     784:	00001097          	auipc	ra,0x1
     788:	fea080e7          	jalr	-22(ra) # 176e <malloc>
     78c:	84aa                	mv	s1,a0
  memset(cmd, 0, sizeof(*cmd));
     78e:	4641                	li	a2,16
     790:	4581                	li	a1,0
     792:	00001097          	auipc	ra,0x1
     796:	94a080e7          	jalr	-1718(ra) # 10dc <memset>
  cmd->type = BACK;
     79a:	4795                	li	a5,5
     79c:	c09c                	sw	a5,0(s1)
  cmd->cmd = subcmd;
     79e:	0124b423          	sd	s2,8(s1)
  return (struct cmd*)cmd;
}
     7a2:	8526                	mv	a0,s1
     7a4:	60e2                	ld	ra,24(sp)
     7a6:	6442                	ld	s0,16(sp)
     7a8:	64a2                	ld	s1,8(sp)
     7aa:	6902                	ld	s2,0(sp)
     7ac:	6105                	addi	sp,sp,32
     7ae:	8082                	ret

00000000000007b0 <gettoken>:
char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int
gettoken(char **ps, char *es, char **q, char **eq)
{
     7b0:	7139                	addi	sp,sp,-64
     7b2:	fc06                	sd	ra,56(sp)
     7b4:	f822                	sd	s0,48(sp)
     7b6:	f426                	sd	s1,40(sp)
     7b8:	f04a                	sd	s2,32(sp)
     7ba:	ec4e                	sd	s3,24(sp)
     7bc:	e852                	sd	s4,16(sp)
     7be:	e456                	sd	s5,8(sp)
     7c0:	e05a                	sd	s6,0(sp)
     7c2:	0080                	addi	s0,sp,64
     7c4:	8a2a                	mv	s4,a0
     7c6:	892e                	mv	s2,a1
     7c8:	8ab2                	mv	s5,a2
     7ca:	8b36                	mv	s6,a3
  char *s;
  int ret;

  s = *ps;
     7cc:	6104                	ld	s1,0(a0)
  while(s < es && strchr(whitespace, *s))
     7ce:	00001997          	auipc	s3,0x1
     7d2:	2e298993          	addi	s3,s3,738 # 1ab0 <whitespace>
     7d6:	00b4fe63          	bgeu	s1,a1,7f2 <gettoken+0x42>
     7da:	0004c583          	lbu	a1,0(s1)
     7de:	854e                	mv	a0,s3
     7e0:	00001097          	auipc	ra,0x1
     7e4:	91e080e7          	jalr	-1762(ra) # 10fe <strchr>
     7e8:	c509                	beqz	a0,7f2 <gettoken+0x42>
    s++;
     7ea:	0485                	addi	s1,s1,1
  while(s < es && strchr(whitespace, *s))
     7ec:	fe9917e3          	bne	s2,s1,7da <gettoken+0x2a>
    s++;
     7f0:	84ca                	mv	s1,s2
  if(q)
     7f2:	000a8463          	beqz	s5,7fa <gettoken+0x4a>
    *q = s;
     7f6:	009ab023          	sd	s1,0(s5)
  ret = *s;
     7fa:	0004c783          	lbu	a5,0(s1)
     7fe:	00078a9b          	sext.w	s5,a5
  switch(*s){
     802:	03c00713          	li	a4,60
     806:	06f76663          	bltu	a4,a5,872 <gettoken+0xc2>
     80a:	03a00713          	li	a4,58
     80e:	00f76e63          	bltu	a4,a5,82a <gettoken+0x7a>
     812:	cf89                	beqz	a5,82c <gettoken+0x7c>
     814:	02600713          	li	a4,38
     818:	00e78963          	beq	a5,a4,82a <gettoken+0x7a>
     81c:	fd87879b          	addiw	a5,a5,-40
     820:	0ff7f793          	zext.b	a5,a5
     824:	4705                	li	a4,1
     826:	06f76d63          	bltu	a4,a5,8a0 <gettoken+0xf0>
  case '(':
  case ')':
  case ';':
  case '&':
  case '<':
    s++;
     82a:	0485                	addi	s1,s1,1
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if(eq)
     82c:	000b0463          	beqz	s6,834 <gettoken+0x84>
    *eq = s;
     830:	009b3023          	sd	s1,0(s6)

  while(s < es && strchr(whitespace, *s))
     834:	00001997          	auipc	s3,0x1
     838:	27c98993          	addi	s3,s3,636 # 1ab0 <whitespace>
     83c:	0124fe63          	bgeu	s1,s2,858 <gettoken+0xa8>
     840:	0004c583          	lbu	a1,0(s1)
     844:	854e                	mv	a0,s3
     846:	00001097          	auipc	ra,0x1
     84a:	8b8080e7          	jalr	-1864(ra) # 10fe <strchr>
     84e:	c509                	beqz	a0,858 <gettoken+0xa8>
    s++;
     850:	0485                	addi	s1,s1,1
  while(s < es && strchr(whitespace, *s))
     852:	fe9917e3          	bne	s2,s1,840 <gettoken+0x90>
    s++;
     856:	84ca                	mv	s1,s2
  *ps = s;
     858:	009a3023          	sd	s1,0(s4)
  return ret;
}
     85c:	8556                	mv	a0,s5
     85e:	70e2                	ld	ra,56(sp)
     860:	7442                	ld	s0,48(sp)
     862:	74a2                	ld	s1,40(sp)
     864:	7902                	ld	s2,32(sp)
     866:	69e2                	ld	s3,24(sp)
     868:	6a42                	ld	s4,16(sp)
     86a:	6aa2                	ld	s5,8(sp)
     86c:	6b02                	ld	s6,0(sp)
     86e:	6121                	addi	sp,sp,64
     870:	8082                	ret
  switch(*s){
     872:	03e00713          	li	a4,62
     876:	02e79163          	bne	a5,a4,898 <gettoken+0xe8>
    s++;
     87a:	00148693          	addi	a3,s1,1
    if(*s == '>'){
     87e:	0014c703          	lbu	a4,1(s1)
     882:	03e00793          	li	a5,62
      s++;
     886:	0489                	addi	s1,s1,2
      ret = '+';
     888:	02b00a93          	li	s5,43
    if(*s == '>'){
     88c:	faf700e3          	beq	a4,a5,82c <gettoken+0x7c>
    s++;
     890:	84b6                	mv	s1,a3
  ret = *s;
     892:	03e00a93          	li	s5,62
     896:	bf59                	j	82c <gettoken+0x7c>
  switch(*s){
     898:	07c00713          	li	a4,124
     89c:	f8e787e3          	beq	a5,a4,82a <gettoken+0x7a>
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
     8a0:	00001997          	auipc	s3,0x1
     8a4:	21098993          	addi	s3,s3,528 # 1ab0 <whitespace>
     8a8:	00001a97          	auipc	s5,0x1
     8ac:	210a8a93          	addi	s5,s5,528 # 1ab8 <symbols>
     8b0:	0324f663          	bgeu	s1,s2,8dc <gettoken+0x12c>
     8b4:	0004c583          	lbu	a1,0(s1)
     8b8:	854e                	mv	a0,s3
     8ba:	00001097          	auipc	ra,0x1
     8be:	844080e7          	jalr	-1980(ra) # 10fe <strchr>
     8c2:	e50d                	bnez	a0,8ec <gettoken+0x13c>
     8c4:	0004c583          	lbu	a1,0(s1)
     8c8:	8556                	mv	a0,s5
     8ca:	00001097          	auipc	ra,0x1
     8ce:	834080e7          	jalr	-1996(ra) # 10fe <strchr>
     8d2:	e911                	bnez	a0,8e6 <gettoken+0x136>
      s++;
     8d4:	0485                	addi	s1,s1,1
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
     8d6:	fc991fe3          	bne	s2,s1,8b4 <gettoken+0x104>
      s++;
     8da:	84ca                	mv	s1,s2
  if(eq)
     8dc:	06100a93          	li	s5,97
     8e0:	f40b18e3          	bnez	s6,830 <gettoken+0x80>
     8e4:	bf95                	j	858 <gettoken+0xa8>
    ret = 'a';
     8e6:	06100a93          	li	s5,97
     8ea:	b789                	j	82c <gettoken+0x7c>
     8ec:	06100a93          	li	s5,97
     8f0:	bf35                	j	82c <gettoken+0x7c>

00000000000008f2 <peek>:

int
peek(char **ps, char *es, char *toks)
{
     8f2:	7139                	addi	sp,sp,-64
     8f4:	fc06                	sd	ra,56(sp)
     8f6:	f822                	sd	s0,48(sp)
     8f8:	f426                	sd	s1,40(sp)
     8fa:	f04a                	sd	s2,32(sp)
     8fc:	ec4e                	sd	s3,24(sp)
     8fe:	e852                	sd	s4,16(sp)
     900:	e456                	sd	s5,8(sp)
     902:	0080                	addi	s0,sp,64
     904:	8a2a                	mv	s4,a0
     906:	892e                	mv	s2,a1
     908:	8ab2                	mv	s5,a2
  char *s;

  s = *ps;
     90a:	6104                	ld	s1,0(a0)
  while(s < es && strchr(whitespace, *s))
     90c:	00001997          	auipc	s3,0x1
     910:	1a498993          	addi	s3,s3,420 # 1ab0 <whitespace>
     914:	00b4fe63          	bgeu	s1,a1,930 <peek+0x3e>
     918:	0004c583          	lbu	a1,0(s1)
     91c:	854e                	mv	a0,s3
     91e:	00000097          	auipc	ra,0x0
     922:	7e0080e7          	jalr	2016(ra) # 10fe <strchr>
     926:	c509                	beqz	a0,930 <peek+0x3e>
    s++;
     928:	0485                	addi	s1,s1,1
  while(s < es && strchr(whitespace, *s))
     92a:	fe9917e3          	bne	s2,s1,918 <peek+0x26>
    s++;
     92e:	84ca                	mv	s1,s2
  *ps = s;
     930:	009a3023          	sd	s1,0(s4)
  return *s && strchr(toks, *s);
     934:	0004c583          	lbu	a1,0(s1)
     938:	4501                	li	a0,0
     93a:	e991                	bnez	a1,94e <peek+0x5c>
}
     93c:	70e2                	ld	ra,56(sp)
     93e:	7442                	ld	s0,48(sp)
     940:	74a2                	ld	s1,40(sp)
     942:	7902                	ld	s2,32(sp)
     944:	69e2                	ld	s3,24(sp)
     946:	6a42                	ld	s4,16(sp)
     948:	6aa2                	ld	s5,8(sp)
     94a:	6121                	addi	sp,sp,64
     94c:	8082                	ret
  return *s && strchr(toks, *s);
     94e:	8556                	mv	a0,s5
     950:	00000097          	auipc	ra,0x0
     954:	7ae080e7          	jalr	1966(ra) # 10fe <strchr>
     958:	00a03533          	snez	a0,a0
     95c:	b7c5                	j	93c <peek+0x4a>

000000000000095e <parseredirs>:
  return cmd;
}

struct cmd*
parseredirs(struct cmd *cmd, char **ps, char *es)
{
     95e:	7119                	addi	sp,sp,-128
     960:	fc86                	sd	ra,120(sp)
     962:	f8a2                	sd	s0,112(sp)
     964:	f4a6                	sd	s1,104(sp)
     966:	f0ca                	sd	s2,96(sp)
     968:	ecce                	sd	s3,88(sp)
     96a:	e8d2                	sd	s4,80(sp)
     96c:	e4d6                	sd	s5,72(sp)
     96e:	e0da                	sd	s6,64(sp)
     970:	fc5e                	sd	s7,56(sp)
     972:	f862                	sd	s8,48(sp)
     974:	f466                	sd	s9,40(sp)
     976:	f06a                	sd	s10,32(sp)
     978:	ec6e                	sd	s11,24(sp)
     97a:	0100                	addi	s0,sp,128
     97c:	8a2a                	mv	s4,a0
     97e:	89ae                	mv	s3,a1
     980:	8932                	mv	s2,a2
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
     982:	00001b97          	auipc	s7,0x1
     986:	f9eb8b93          	addi	s7,s7,-98 # 1920 <malloc+0x1b2>
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
     98a:	f8040d13          	addi	s10,s0,-128
     98e:	f8840c93          	addi	s9,s0,-120
     992:	06100c13          	li	s8,97
      panic("missing file for redirection");
    switch(tok){
     996:	03c00d93          	li	s11,60
  while(peek(ps, es, "<>")){
     99a:	a02d                	j	9c4 <parseredirs+0x66>
      panic("missing file for redirection");
     99c:	00001517          	auipc	a0,0x1
     9a0:	f6450513          	addi	a0,a0,-156 # 1900 <malloc+0x192>
     9a4:	00000097          	auipc	ra,0x0
     9a8:	9fa080e7          	jalr	-1542(ra) # 39e <panic>
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
     9ac:	4701                	li	a4,0
     9ae:	4681                	li	a3,0
     9b0:	f8043603          	ld	a2,-128(s0)
     9b4:	f8843583          	ld	a1,-120(s0)
     9b8:	8552                	mv	a0,s4
     9ba:	00000097          	auipc	ra,0x0
     9be:	cc6080e7          	jalr	-826(ra) # 680 <redircmd>
     9c2:	8a2a                	mv	s4,a0
    switch(tok){
     9c4:	03e00b13          	li	s6,62
     9c8:	02b00a93          	li	s5,43
  while(peek(ps, es, "<>")){
     9cc:	865e                	mv	a2,s7
     9ce:	85ca                	mv	a1,s2
     9d0:	854e                	mv	a0,s3
     9d2:	00000097          	auipc	ra,0x0
     9d6:	f20080e7          	jalr	-224(ra) # 8f2 <peek>
     9da:	c535                	beqz	a0,a46 <parseredirs+0xe8>
    tok = gettoken(ps, es, 0, 0);
     9dc:	4681                	li	a3,0
     9de:	4601                	li	a2,0
     9e0:	85ca                	mv	a1,s2
     9e2:	854e                	mv	a0,s3
     9e4:	00000097          	auipc	ra,0x0
     9e8:	dcc080e7          	jalr	-564(ra) # 7b0 <gettoken>
     9ec:	84aa                	mv	s1,a0
    if(gettoken(ps, es, &q, &eq) != 'a')
     9ee:	86ea                	mv	a3,s10
     9f0:	8666                	mv	a2,s9
     9f2:	85ca                	mv	a1,s2
     9f4:	854e                	mv	a0,s3
     9f6:	00000097          	auipc	ra,0x0
     9fa:	dba080e7          	jalr	-582(ra) # 7b0 <gettoken>
     9fe:	f9851fe3          	bne	a0,s8,99c <parseredirs+0x3e>
    switch(tok){
     a02:	fbb485e3          	beq	s1,s11,9ac <parseredirs+0x4e>
     a06:	03648263          	beq	s1,s6,a2a <parseredirs+0xcc>
     a0a:	fd5491e3          	bne	s1,s5,9cc <parseredirs+0x6e>
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE|O_TRUNC, 1);
      break;
    case '+':  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE|O_APPEND, 1);
     a0e:	4705                	li	a4,1
     a10:	04500693          	li	a3,69
     a14:	f8043603          	ld	a2,-128(s0)
     a18:	f8843583          	ld	a1,-120(s0)
     a1c:	8552                	mv	a0,s4
     a1e:	00000097          	auipc	ra,0x0
     a22:	c62080e7          	jalr	-926(ra) # 680 <redircmd>
     a26:	8a2a                	mv	s4,a0
      break;
     a28:	bf71                	j	9c4 <parseredirs+0x66>
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE|O_TRUNC, 1);
     a2a:	4705                	li	a4,1
     a2c:	44100693          	li	a3,1089
     a30:	f8043603          	ld	a2,-128(s0)
     a34:	f8843583          	ld	a1,-120(s0)
     a38:	8552                	mv	a0,s4
     a3a:	00000097          	auipc	ra,0x0
     a3e:	c46080e7          	jalr	-954(ra) # 680 <redircmd>
     a42:	8a2a                	mv	s4,a0
      break;
     a44:	b741                	j	9c4 <parseredirs+0x66>
    }
  }
  return cmd;
}
     a46:	8552                	mv	a0,s4
     a48:	70e6                	ld	ra,120(sp)
     a4a:	7446                	ld	s0,112(sp)
     a4c:	74a6                	ld	s1,104(sp)
     a4e:	7906                	ld	s2,96(sp)
     a50:	69e6                	ld	s3,88(sp)
     a52:	6a46                	ld	s4,80(sp)
     a54:	6aa6                	ld	s5,72(sp)
     a56:	6b06                	ld	s6,64(sp)
     a58:	7be2                	ld	s7,56(sp)
     a5a:	7c42                	ld	s8,48(sp)
     a5c:	7ca2                	ld	s9,40(sp)
     a5e:	7d02                	ld	s10,32(sp)
     a60:	6de2                	ld	s11,24(sp)
     a62:	6109                	addi	sp,sp,128
     a64:	8082                	ret

0000000000000a66 <parseexec>:
  return cmd;
}

struct cmd*
parseexec(char **ps, char *es)
{
     a66:	7119                	addi	sp,sp,-128
     a68:	fc86                	sd	ra,120(sp)
     a6a:	f8a2                	sd	s0,112(sp)
     a6c:	f4a6                	sd	s1,104(sp)
     a6e:	f0ca                	sd	s2,96(sp)
     a70:	ecce                	sd	s3,88(sp)
     a72:	e8d2                	sd	s4,80(sp)
     a74:	e4d6                	sd	s5,72(sp)
     a76:	e0da                	sd	s6,64(sp)
     a78:	fc5e                	sd	s7,56(sp)
     a7a:	f862                	sd	s8,48(sp)
     a7c:	f466                	sd	s9,40(sp)
     a7e:	f06a                	sd	s10,32(sp)
     a80:	ec6e                	sd	s11,24(sp)
     a82:	0100                	addi	s0,sp,128
     a84:	8a2a                	mv	s4,a0
     a86:	8aae                	mv	s5,a1
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  if(peek(ps, es, "("))
     a88:	00001617          	auipc	a2,0x1
     a8c:	ea060613          	addi	a2,a2,-352 # 1928 <malloc+0x1ba>
     a90:	00000097          	auipc	ra,0x0
     a94:	e62080e7          	jalr	-414(ra) # 8f2 <peek>
     a98:	ed05                	bnez	a0,ad0 <parseexec+0x6a>
     a9a:	89aa                	mv	s3,a0
    return parseblock(ps, es);

  ret = execcmd();
     a9c:	00000097          	auipc	ra,0x0
     aa0:	bae080e7          	jalr	-1106(ra) # 64a <execcmd>
     aa4:	8daa                	mv	s11,a0
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
     aa6:	8656                	mv	a2,s5
     aa8:	85d2                	mv	a1,s4
     aaa:	00000097          	auipc	ra,0x0
     aae:	eb4080e7          	jalr	-332(ra) # 95e <parseredirs>
     ab2:	84aa                	mv	s1,a0
  while(!peek(ps, es, "|)&;")){
     ab4:	008d8913          	addi	s2,s11,8
     ab8:	00001b17          	auipc	s6,0x1
     abc:	e90b0b13          	addi	s6,s6,-368 # 1948 <malloc+0x1da>
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
     ac0:	f8040c13          	addi	s8,s0,-128
     ac4:	f8840b93          	addi	s7,s0,-120
      break;
    if(tok != 'a')
     ac8:	06100d13          	li	s10,97
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
     acc:	4ca9                	li	s9,10
  while(!peek(ps, es, "|)&;")){
     ace:	a881                	j	b1e <parseexec+0xb8>
    return parseblock(ps, es);
     ad0:	85d6                	mv	a1,s5
     ad2:	8552                	mv	a0,s4
     ad4:	00000097          	auipc	ra,0x0
     ad8:	1bc080e7          	jalr	444(ra) # c90 <parseblock>
     adc:	84aa                	mv	s1,a0
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}
     ade:	8526                	mv	a0,s1
     ae0:	70e6                	ld	ra,120(sp)
     ae2:	7446                	ld	s0,112(sp)
     ae4:	74a6                	ld	s1,104(sp)
     ae6:	7906                	ld	s2,96(sp)
     ae8:	69e6                	ld	s3,88(sp)
     aea:	6a46                	ld	s4,80(sp)
     aec:	6aa6                	ld	s5,72(sp)
     aee:	6b06                	ld	s6,64(sp)
     af0:	7be2                	ld	s7,56(sp)
     af2:	7c42                	ld	s8,48(sp)
     af4:	7ca2                	ld	s9,40(sp)
     af6:	7d02                	ld	s10,32(sp)
     af8:	6de2                	ld	s11,24(sp)
     afa:	6109                	addi	sp,sp,128
     afc:	8082                	ret
      panic("syntax");
     afe:	00001517          	auipc	a0,0x1
     b02:	e3250513          	addi	a0,a0,-462 # 1930 <malloc+0x1c2>
     b06:	00000097          	auipc	ra,0x0
     b0a:	898080e7          	jalr	-1896(ra) # 39e <panic>
    ret = parseredirs(ret, ps, es);
     b0e:	8656                	mv	a2,s5
     b10:	85d2                	mv	a1,s4
     b12:	8526                	mv	a0,s1
     b14:	00000097          	auipc	ra,0x0
     b18:	e4a080e7          	jalr	-438(ra) # 95e <parseredirs>
     b1c:	84aa                	mv	s1,a0
  while(!peek(ps, es, "|)&;")){
     b1e:	865a                	mv	a2,s6
     b20:	85d6                	mv	a1,s5
     b22:	8552                	mv	a0,s4
     b24:	00000097          	auipc	ra,0x0
     b28:	dce080e7          	jalr	-562(ra) # 8f2 <peek>
     b2c:	e121                	bnez	a0,b6c <parseexec+0x106>
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
     b2e:	86e2                	mv	a3,s8
     b30:	865e                	mv	a2,s7
     b32:	85d6                	mv	a1,s5
     b34:	8552                	mv	a0,s4
     b36:	00000097          	auipc	ra,0x0
     b3a:	c7a080e7          	jalr	-902(ra) # 7b0 <gettoken>
     b3e:	c51d                	beqz	a0,b6c <parseexec+0x106>
    if(tok != 'a')
     b40:	fba51fe3          	bne	a0,s10,afe <parseexec+0x98>
    cmd->argv[argc] = q;
     b44:	f8843783          	ld	a5,-120(s0)
     b48:	00f93023          	sd	a5,0(s2)
    cmd->eargv[argc] = eq;
     b4c:	f8043783          	ld	a5,-128(s0)
     b50:	04f93823          	sd	a5,80(s2)
    argc++;
     b54:	2985                	addiw	s3,s3,1
    if(argc >= MAXARGS)
     b56:	0921                	addi	s2,s2,8
     b58:	fb999be3          	bne	s3,s9,b0e <parseexec+0xa8>
      panic("too many args");
     b5c:	00001517          	auipc	a0,0x1
     b60:	ddc50513          	addi	a0,a0,-548 # 1938 <malloc+0x1ca>
     b64:	00000097          	auipc	ra,0x0
     b68:	83a080e7          	jalr	-1990(ra) # 39e <panic>
  cmd->argv[argc] = 0;
     b6c:	098e                	slli	s3,s3,0x3
     b6e:	9dce                	add	s11,s11,s3
     b70:	000db423          	sd	zero,8(s11)
  cmd->eargv[argc] = 0;
     b74:	040dbc23          	sd	zero,88(s11)
  return ret;
     b78:	b79d                	j	ade <parseexec+0x78>

0000000000000b7a <parsepipe>:
{
     b7a:	7179                	addi	sp,sp,-48
     b7c:	f406                	sd	ra,40(sp)
     b7e:	f022                	sd	s0,32(sp)
     b80:	ec26                	sd	s1,24(sp)
     b82:	e84a                	sd	s2,16(sp)
     b84:	e44e                	sd	s3,8(sp)
     b86:	1800                	addi	s0,sp,48
     b88:	892a                	mv	s2,a0
     b8a:	89ae                	mv	s3,a1
  cmd = parseexec(ps, es);
     b8c:	00000097          	auipc	ra,0x0
     b90:	eda080e7          	jalr	-294(ra) # a66 <parseexec>
     b94:	84aa                	mv	s1,a0
  if(peek(ps, es, "|")){
     b96:	00001617          	auipc	a2,0x1
     b9a:	dba60613          	addi	a2,a2,-582 # 1950 <malloc+0x1e2>
     b9e:	85ce                	mv	a1,s3
     ba0:	854a                	mv	a0,s2
     ba2:	00000097          	auipc	ra,0x0
     ba6:	d50080e7          	jalr	-688(ra) # 8f2 <peek>
     baa:	e909                	bnez	a0,bbc <parsepipe+0x42>
}
     bac:	8526                	mv	a0,s1
     bae:	70a2                	ld	ra,40(sp)
     bb0:	7402                	ld	s0,32(sp)
     bb2:	64e2                	ld	s1,24(sp)
     bb4:	6942                	ld	s2,16(sp)
     bb6:	69a2                	ld	s3,8(sp)
     bb8:	6145                	addi	sp,sp,48
     bba:	8082                	ret
    gettoken(ps, es, 0, 0);
     bbc:	4681                	li	a3,0
     bbe:	4601                	li	a2,0
     bc0:	85ce                	mv	a1,s3
     bc2:	854a                	mv	a0,s2
     bc4:	00000097          	auipc	ra,0x0
     bc8:	bec080e7          	jalr	-1044(ra) # 7b0 <gettoken>
    cmd = pipecmd(cmd, parsepipe(ps, es));
     bcc:	85ce                	mv	a1,s3
     bce:	854a                	mv	a0,s2
     bd0:	00000097          	auipc	ra,0x0
     bd4:	faa080e7          	jalr	-86(ra) # b7a <parsepipe>
     bd8:	85aa                	mv	a1,a0
     bda:	8526                	mv	a0,s1
     bdc:	00000097          	auipc	ra,0x0
     be0:	b0c080e7          	jalr	-1268(ra) # 6e8 <pipecmd>
     be4:	84aa                	mv	s1,a0
  return cmd;
     be6:	b7d9                	j	bac <parsepipe+0x32>

0000000000000be8 <parseline>:
{
     be8:	7179                	addi	sp,sp,-48
     bea:	f406                	sd	ra,40(sp)
     bec:	f022                	sd	s0,32(sp)
     bee:	ec26                	sd	s1,24(sp)
     bf0:	e84a                	sd	s2,16(sp)
     bf2:	e44e                	sd	s3,8(sp)
     bf4:	e052                	sd	s4,0(sp)
     bf6:	1800                	addi	s0,sp,48
     bf8:	892a                	mv	s2,a0
     bfa:	89ae                	mv	s3,a1
  cmd = parsepipe(ps, es);
     bfc:	00000097          	auipc	ra,0x0
     c00:	f7e080e7          	jalr	-130(ra) # b7a <parsepipe>
     c04:	84aa                	mv	s1,a0
  while(peek(ps, es, "&")){
     c06:	00001a17          	auipc	s4,0x1
     c0a:	d52a0a13          	addi	s4,s4,-686 # 1958 <malloc+0x1ea>
     c0e:	a839                	j	c2c <parseline+0x44>
    gettoken(ps, es, 0, 0);
     c10:	4681                	li	a3,0
     c12:	4601                	li	a2,0
     c14:	85ce                	mv	a1,s3
     c16:	854a                	mv	a0,s2
     c18:	00000097          	auipc	ra,0x0
     c1c:	b98080e7          	jalr	-1128(ra) # 7b0 <gettoken>
    cmd = backcmd(cmd);
     c20:	8526                	mv	a0,s1
     c22:	00000097          	auipc	ra,0x0
     c26:	b52080e7          	jalr	-1198(ra) # 774 <backcmd>
     c2a:	84aa                	mv	s1,a0
  while(peek(ps, es, "&")){
     c2c:	8652                	mv	a2,s4
     c2e:	85ce                	mv	a1,s3
     c30:	854a                	mv	a0,s2
     c32:	00000097          	auipc	ra,0x0
     c36:	cc0080e7          	jalr	-832(ra) # 8f2 <peek>
     c3a:	f979                	bnez	a0,c10 <parseline+0x28>
  if(peek(ps, es, ";")){
     c3c:	00001617          	auipc	a2,0x1
     c40:	d2460613          	addi	a2,a2,-732 # 1960 <malloc+0x1f2>
     c44:	85ce                	mv	a1,s3
     c46:	854a                	mv	a0,s2
     c48:	00000097          	auipc	ra,0x0
     c4c:	caa080e7          	jalr	-854(ra) # 8f2 <peek>
     c50:	e911                	bnez	a0,c64 <parseline+0x7c>
}
     c52:	8526                	mv	a0,s1
     c54:	70a2                	ld	ra,40(sp)
     c56:	7402                	ld	s0,32(sp)
     c58:	64e2                	ld	s1,24(sp)
     c5a:	6942                	ld	s2,16(sp)
     c5c:	69a2                	ld	s3,8(sp)
     c5e:	6a02                	ld	s4,0(sp)
     c60:	6145                	addi	sp,sp,48
     c62:	8082                	ret
    gettoken(ps, es, 0, 0);
     c64:	4681                	li	a3,0
     c66:	4601                	li	a2,0
     c68:	85ce                	mv	a1,s3
     c6a:	854a                	mv	a0,s2
     c6c:	00000097          	auipc	ra,0x0
     c70:	b44080e7          	jalr	-1212(ra) # 7b0 <gettoken>
    cmd = listcmd(cmd, parseline(ps, es));
     c74:	85ce                	mv	a1,s3
     c76:	854a                	mv	a0,s2
     c78:	00000097          	auipc	ra,0x0
     c7c:	f70080e7          	jalr	-144(ra) # be8 <parseline>
     c80:	85aa                	mv	a1,a0
     c82:	8526                	mv	a0,s1
     c84:	00000097          	auipc	ra,0x0
     c88:	aaa080e7          	jalr	-1366(ra) # 72e <listcmd>
     c8c:	84aa                	mv	s1,a0
  return cmd;
     c8e:	b7d1                	j	c52 <parseline+0x6a>

0000000000000c90 <parseblock>:
{
     c90:	7179                	addi	sp,sp,-48
     c92:	f406                	sd	ra,40(sp)
     c94:	f022                	sd	s0,32(sp)
     c96:	ec26                	sd	s1,24(sp)
     c98:	e84a                	sd	s2,16(sp)
     c9a:	e44e                	sd	s3,8(sp)
     c9c:	1800                	addi	s0,sp,48
     c9e:	84aa                	mv	s1,a0
     ca0:	892e                	mv	s2,a1
  if(!peek(ps, es, "("))
     ca2:	00001617          	auipc	a2,0x1
     ca6:	c8660613          	addi	a2,a2,-890 # 1928 <malloc+0x1ba>
     caa:	00000097          	auipc	ra,0x0
     cae:	c48080e7          	jalr	-952(ra) # 8f2 <peek>
     cb2:	c12d                	beqz	a0,d14 <parseblock+0x84>
  gettoken(ps, es, 0, 0);
     cb4:	4681                	li	a3,0
     cb6:	4601                	li	a2,0
     cb8:	85ca                	mv	a1,s2
     cba:	8526                	mv	a0,s1
     cbc:	00000097          	auipc	ra,0x0
     cc0:	af4080e7          	jalr	-1292(ra) # 7b0 <gettoken>
  cmd = parseline(ps, es);
     cc4:	85ca                	mv	a1,s2
     cc6:	8526                	mv	a0,s1
     cc8:	00000097          	auipc	ra,0x0
     ccc:	f20080e7          	jalr	-224(ra) # be8 <parseline>
     cd0:	89aa                	mv	s3,a0
  if(!peek(ps, es, ")"))
     cd2:	00001617          	auipc	a2,0x1
     cd6:	ca660613          	addi	a2,a2,-858 # 1978 <malloc+0x20a>
     cda:	85ca                	mv	a1,s2
     cdc:	8526                	mv	a0,s1
     cde:	00000097          	auipc	ra,0x0
     ce2:	c14080e7          	jalr	-1004(ra) # 8f2 <peek>
     ce6:	cd1d                	beqz	a0,d24 <parseblock+0x94>
  gettoken(ps, es, 0, 0);
     ce8:	4681                	li	a3,0
     cea:	4601                	li	a2,0
     cec:	85ca                	mv	a1,s2
     cee:	8526                	mv	a0,s1
     cf0:	00000097          	auipc	ra,0x0
     cf4:	ac0080e7          	jalr	-1344(ra) # 7b0 <gettoken>
  cmd = parseredirs(cmd, ps, es);
     cf8:	864a                	mv	a2,s2
     cfa:	85a6                	mv	a1,s1
     cfc:	854e                	mv	a0,s3
     cfe:	00000097          	auipc	ra,0x0
     d02:	c60080e7          	jalr	-928(ra) # 95e <parseredirs>
}
     d06:	70a2                	ld	ra,40(sp)
     d08:	7402                	ld	s0,32(sp)
     d0a:	64e2                	ld	s1,24(sp)
     d0c:	6942                	ld	s2,16(sp)
     d0e:	69a2                	ld	s3,8(sp)
     d10:	6145                	addi	sp,sp,48
     d12:	8082                	ret
    panic("parseblock");
     d14:	00001517          	auipc	a0,0x1
     d18:	c5450513          	addi	a0,a0,-940 # 1968 <malloc+0x1fa>
     d1c:	fffff097          	auipc	ra,0xfffff
     d20:	682080e7          	jalr	1666(ra) # 39e <panic>
    panic("syntax - missing )");
     d24:	00001517          	auipc	a0,0x1
     d28:	c5c50513          	addi	a0,a0,-932 # 1980 <malloc+0x212>
     d2c:	fffff097          	auipc	ra,0xfffff
     d30:	672080e7          	jalr	1650(ra) # 39e <panic>

0000000000000d34 <nulterminate>:

// NUL-terminate all the counted strings.
struct cmd*
nulterminate(struct cmd *cmd)
{
     d34:	1101                	addi	sp,sp,-32
     d36:	ec06                	sd	ra,24(sp)
     d38:	e822                	sd	s0,16(sp)
     d3a:	e426                	sd	s1,8(sp)
     d3c:	1000                	addi	s0,sp,32
     d3e:	84aa                	mv	s1,a0
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
     d40:	c521                	beqz	a0,d88 <nulterminate+0x54>
    return 0;

  switch(cmd->type){
     d42:	4118                	lw	a4,0(a0)
     d44:	4795                	li	a5,5
     d46:	04e7e163          	bltu	a5,a4,d88 <nulterminate+0x54>
     d4a:	00056783          	lwu	a5,0(a0)
     d4e:	078a                	slli	a5,a5,0x2
     d50:	00001717          	auipc	a4,0x1
     d54:	cd070713          	addi	a4,a4,-816 # 1a20 <malloc+0x2b2>
     d58:	97ba                	add	a5,a5,a4
     d5a:	439c                	lw	a5,0(a5)
     d5c:	97ba                	add	a5,a5,a4
     d5e:	8782                	jr	a5
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
     d60:	651c                	ld	a5,8(a0)
     d62:	c39d                	beqz	a5,d88 <nulterminate+0x54>
     d64:	01050793          	addi	a5,a0,16
      *ecmd->eargv[i] = 0;
     d68:	67b8                	ld	a4,72(a5)
     d6a:	00070023          	sb	zero,0(a4)
    for(i=0; ecmd->argv[i]; i++)
     d6e:	07a1                	addi	a5,a5,8
     d70:	ff87b703          	ld	a4,-8(a5)
     d74:	fb75                	bnez	a4,d68 <nulterminate+0x34>
     d76:	a809                	j	d88 <nulterminate+0x54>
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
     d78:	6508                	ld	a0,8(a0)
     d7a:	00000097          	auipc	ra,0x0
     d7e:	fba080e7          	jalr	-70(ra) # d34 <nulterminate>
    *rcmd->efile = 0;
     d82:	6c9c                	ld	a5,24(s1)
     d84:	00078023          	sb	zero,0(a5)
    bcmd = (struct backcmd*)cmd;
    nulterminate(bcmd->cmd);
    break;
  }
  return cmd;
     d88:	8526                	mv	a0,s1
     d8a:	60e2                	ld	ra,24(sp)
     d8c:	6442                	ld	s0,16(sp)
     d8e:	64a2                	ld	s1,8(sp)
     d90:	6105                	addi	sp,sp,32
     d92:	8082                	ret
    nulterminate(pcmd->left);
     d94:	6508                	ld	a0,8(a0)
     d96:	00000097          	auipc	ra,0x0
     d9a:	f9e080e7          	jalr	-98(ra) # d34 <nulterminate>
    nulterminate(pcmd->right);
     d9e:	6888                	ld	a0,16(s1)
     da0:	00000097          	auipc	ra,0x0
     da4:	f94080e7          	jalr	-108(ra) # d34 <nulterminate>
    break;
     da8:	b7c5                	j	d88 <nulterminate+0x54>
    nulterminate(lcmd->left);
     daa:	6508                	ld	a0,8(a0)
     dac:	00000097          	auipc	ra,0x0
     db0:	f88080e7          	jalr	-120(ra) # d34 <nulterminate>
    nulterminate(lcmd->right);
     db4:	6888                	ld	a0,16(s1)
     db6:	00000097          	auipc	ra,0x0
     dba:	f7e080e7          	jalr	-130(ra) # d34 <nulterminate>
    break;
     dbe:	b7e9                	j	d88 <nulterminate+0x54>
    nulterminate(bcmd->cmd);
     dc0:	6508                	ld	a0,8(a0)
     dc2:	00000097          	auipc	ra,0x0
     dc6:	f72080e7          	jalr	-142(ra) # d34 <nulterminate>
    break;
     dca:	bf7d                	j	d88 <nulterminate+0x54>

0000000000000dcc <parsecmd>:
{
     dcc:	7139                	addi	sp,sp,-64
     dce:	fc06                	sd	ra,56(sp)
     dd0:	f822                	sd	s0,48(sp)
     dd2:	f426                	sd	s1,40(sp)
     dd4:	f04a                	sd	s2,32(sp)
     dd6:	ec4e                	sd	s3,24(sp)
     dd8:	0080                	addi	s0,sp,64
     dda:	fca43423          	sd	a0,-56(s0)
  es = s + strlen(s);
     dde:	84aa                	mv	s1,a0
     de0:	00000097          	auipc	ra,0x0
     de4:	2d2080e7          	jalr	722(ra) # 10b2 <strlen>
     de8:	1502                	slli	a0,a0,0x20
     dea:	9101                	srli	a0,a0,0x20
     dec:	94aa                	add	s1,s1,a0
  cmd = parseline(&s, es);
     dee:	85a6                	mv	a1,s1
     df0:	fc840913          	addi	s2,s0,-56
     df4:	854a                	mv	a0,s2
     df6:	00000097          	auipc	ra,0x0
     dfa:	df2080e7          	jalr	-526(ra) # be8 <parseline>
     dfe:	89aa                	mv	s3,a0
  peek(&s, es, "");
     e00:	00001617          	auipc	a2,0x1
     e04:	bf060613          	addi	a2,a2,-1040 # 19f0 <malloc+0x282>
     e08:	85a6                	mv	a1,s1
     e0a:	854a                	mv	a0,s2
     e0c:	00000097          	auipc	ra,0x0
     e10:	ae6080e7          	jalr	-1306(ra) # 8f2 <peek>
  if(s != es){
     e14:	fc843603          	ld	a2,-56(s0)
     e18:	00961f63          	bne	a2,s1,e36 <parsecmd+0x6a>
  nulterminate(cmd);
     e1c:	854e                	mv	a0,s3
     e1e:	00000097          	auipc	ra,0x0
     e22:	f16080e7          	jalr	-234(ra) # d34 <nulterminate>
}
     e26:	854e                	mv	a0,s3
     e28:	70e2                	ld	ra,56(sp)
     e2a:	7442                	ld	s0,48(sp)
     e2c:	74a2                	ld	s1,40(sp)
     e2e:	7902                	ld	s2,32(sp)
     e30:	69e2                	ld	s3,24(sp)
     e32:	6121                	addi	sp,sp,64
     e34:	8082                	ret
    fprintf(2, "leftovers: %s\n", s);
     e36:	00001597          	auipc	a1,0x1
     e3a:	b6258593          	addi	a1,a1,-1182 # 1998 <malloc+0x22a>
     e3e:	4509                	li	a0,2
     e40:	00001097          	auipc	ra,0x1
     e44:	848080e7          	jalr	-1976(ra) # 1688 <fprintf>
    panic("syntax");
     e48:	00001517          	auipc	a0,0x1
     e4c:	ae850513          	addi	a0,a0,-1304 # 1930 <malloc+0x1c2>
     e50:	fffff097          	auipc	ra,0xfffff
     e54:	54e080e7          	jalr	1358(ra) # 39e <panic>

0000000000000e58 <main>:
{
     e58:	711d                	addi	sp,sp,-96
     e5a:	ec86                	sd	ra,88(sp)
     e5c:	e8a2                	sd	s0,80(sp)
     e5e:	e4a6                	sd	s1,72(sp)
     e60:	e0ca                	sd	s2,64(sp)
     e62:	fc4e                	sd	s3,56(sp)
     e64:	f852                	sd	s4,48(sp)
     e66:	f456                	sd	s5,40(sp)
     e68:	f05a                	sd	s6,32(sp)
     e6a:	ec5e                	sd	s7,24(sp)
     e6c:	e862                	sd	s8,16(sp)
     e6e:	e466                	sd	s9,8(sp)
     e70:	1080                	addi	s0,sp,96
  while((fd = dev(O_RDWR, 1, 0)) >= 0){
     e72:	4601                	li	a2,0
     e74:	4585                	li	a1,1
     e76:	4509                	li	a0,2
     e78:	00000097          	auipc	ra,0x0
     e7c:	528080e7          	jalr	1320(ra) # 13a0 <dev>
     e80:	00054963          	bltz	a0,e92 <main+0x3a>
    if(fd >= 3){
     e84:	4789                	li	a5,2
     e86:	fea7d6e3          	bge	a5,a0,e72 <main+0x1a>
      close(fd);
     e8a:	00000097          	auipc	ra,0x0
     e8e:	49c080e7          	jalr	1180(ra) # 1326 <close>
  strcpy(envs[nenv].name, "SHELL");
     e92:	00001497          	auipc	s1,0x1
     e96:	c2e48493          	addi	s1,s1,-978 # 1ac0 <nenv>
     e9a:	4088                	lw	a0,0(s1)
     e9c:	051e                	slli	a0,a0,0x7
     e9e:	00001917          	auipc	s2,0x1
     ea2:	d1290913          	addi	s2,s2,-750 # 1bb0 <envs>
     ea6:	00001597          	auipc	a1,0x1
     eaa:	b0258593          	addi	a1,a1,-1278 # 19a8 <malloc+0x23a>
     eae:	954a                	add	a0,a0,s2
     eb0:	00000097          	auipc	ra,0x0
     eb4:	18c080e7          	jalr	396(ra) # 103c <strcpy>
  strcpy(envs[nenv].value, "/bin");
     eb8:	4088                	lw	a0,0(s1)
     eba:	051e                	slli	a0,a0,0x7
     ebc:	02050513          	addi	a0,a0,32
     ec0:	00001597          	auipc	a1,0x1
     ec4:	af058593          	addi	a1,a1,-1296 # 19b0 <malloc+0x242>
     ec8:	954a                	add	a0,a0,s2
     eca:	00000097          	auipc	ra,0x0
     ece:	172080e7          	jalr	370(ra) # 103c <strcpy>
  nenv++;
     ed2:	409c                	lw	a5,0(s1)
     ed4:	2785                	addiw	a5,a5,1
     ed6:	c09c                	sw	a5,0(s1)
  getcwd(mycwd,0);
     ed8:	4581                	li	a1,0
     eda:	00001517          	auipc	a0,0x1
     ede:	bee50513          	addi	a0,a0,-1042 # 1ac8 <mycwd>
     ee2:	00000097          	auipc	ra,0x0
     ee6:	4d4080e7          	jalr	1236(ra) # 13b6 <getcwd>
  while(getcmd(buf, sizeof(buf)) >= 0){
     eea:	00001917          	auipc	s2,0x1
     eee:	c5e90913          	addi	s2,s2,-930 # 1b48 <buf.0>
    replace(buf);
     ef2:	89a6                	mv	s3,s1
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
     ef4:	06300a13          	li	s4,99
      else if(!strcmp(ecmd->argv[0], "export"))
     ef8:	00001a97          	auipc	s5,0x1
     efc:	ad0a8a93          	addi	s5,s5,-1328 # 19c8 <malloc+0x25a>
          fprintf(2, "export failed\n");
     f00:	00001b97          	auipc	s7,0x1
     f04:	af8b8b93          	addi	s7,s7,-1288 # 19f8 <malloc+0x28a>
          fprintf(2, "Usage: export [-p] [NAME=VALUE]\n");
     f08:	00001b17          	auipc	s6,0x1
     f0c:	ac8b0b13          	addi	s6,s6,-1336 # 19d0 <malloc+0x262>
      if(chdir(buf+3) < 0)
     f10:	00001c17          	auipc	s8,0x1
     f14:	c3bc0c13          	addi	s8,s8,-965 # 1b4b <buf.0+0x3>
      getcwd(mycwd,0);
     f18:	00001c97          	auipc	s9,0x1
     f1c:	bb0c8c93          	addi	s9,s9,-1104 # 1ac8 <mycwd>
  while(getcmd(buf, sizeof(buf)) >= 0){
     f20:	a8ad                	j	f9a <main+0x142>
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
     f22:	08a9c703          	lbu	a4,138(s3)
     f26:	02000793          	li	a5,32
     f2a:	0af71063          	bne	a4,a5,fca <main+0x172>
      buf[strlen(buf)-1] = 0;  // chop \n
     f2e:	854a                	mv	a0,s2
     f30:	00000097          	auipc	ra,0x0
     f34:	182080e7          	jalr	386(ra) # 10b2 <strlen>
     f38:	fff5079b          	addiw	a5,a0,-1
     f3c:	1782                	slli	a5,a5,0x20
     f3e:	9381                	srli	a5,a5,0x20
     f40:	97ce                	add	a5,a5,s3
     f42:	08078423          	sb	zero,136(a5)
      if(chdir(buf+3) < 0)
     f46:	8562                	mv	a0,s8
     f48:	00000097          	auipc	ra,0x0
     f4c:	418080e7          	jalr	1048(ra) # 1360 <chdir>
     f50:	00054963          	bltz	a0,f62 <main+0x10a>
      getcwd(mycwd,0);
     f54:	4581                	li	a1,0
     f56:	8566                	mv	a0,s9
     f58:	00000097          	auipc	ra,0x0
     f5c:	45e080e7          	jalr	1118(ra) # 13b6 <getcwd>
     f60:	a82d                	j	f9a <main+0x142>
        fprintf(2, "cannot cd %s\n", buf+3);
     f62:	8662                	mv	a2,s8
     f64:	00001597          	auipc	a1,0x1
     f68:	a5458593          	addi	a1,a1,-1452 # 19b8 <malloc+0x24a>
     f6c:	4509                	li	a0,2
     f6e:	00000097          	auipc	ra,0x0
     f72:	71a080e7          	jalr	1818(ra) # 1688 <fprintf>
     f76:	bff9                	j	f54 <main+0xfc>
        free(cmd);
     f78:	8526                	mv	a0,s1
     f7a:	00000097          	auipc	ra,0x0
     f7e:	772080e7          	jalr	1906(ra) # 16ec <free>
        continue;
     f82:	a821                	j	f9a <main+0x142>
          fprintf(2, "Usage: export [-p] [NAME=VALUE]\n");
     f84:	85da                	mv	a1,s6
     f86:	4509                	li	a0,2
     f88:	00000097          	auipc	ra,0x0
     f8c:	700080e7          	jalr	1792(ra) # 1688 <fprintf>
        free(cmd);
     f90:	8526                	mv	a0,s1
     f92:	00000097          	auipc	ra,0x0
     f96:	75a080e7          	jalr	1882(ra) # 16ec <free>
  while(getcmd(buf, sizeof(buf)) >= 0){
     f9a:	06400593          	li	a1,100
     f9e:	854a                	mv	a0,s2
     fa0:	fffff097          	auipc	ra,0xfffff
     fa4:	3a2080e7          	jalr	930(ra) # 342 <getcmd>
     fa8:	08054563          	bltz	a0,1032 <main+0x1da>
    replace(buf);
     fac:	854a                	mv	a0,s2
     fae:	fffff097          	auipc	ra,0xfffff
     fb2:	250080e7          	jalr	592(ra) # 1fe <replace>
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
     fb6:	0889c783          	lbu	a5,136(s3)
     fba:	01479863          	bne	a5,s4,fca <main+0x172>
     fbe:	0899c703          	lbu	a4,137(s3)
     fc2:	06400793          	li	a5,100
     fc6:	f4f70ee3          	beq	a4,a5,f22 <main+0xca>
      struct cmd *cmd = parsecmd(buf);
     fca:	854a                	mv	a0,s2
     fcc:	00000097          	auipc	ra,0x0
     fd0:	e00080e7          	jalr	-512(ra) # dcc <parsecmd>
     fd4:	84aa                	mv	s1,a0
      if(ecmd->argv[0] == 0) {
     fd6:	6508                	ld	a0,8(a0)
     fd8:	d145                	beqz	a0,f78 <main+0x120>
      else if(!strcmp(ecmd->argv[0], "export"))
     fda:	85d6                	mv	a1,s5
     fdc:	00000097          	auipc	ra,0x0
     fe0:	0aa080e7          	jalr	170(ra) # 1086 <strcmp>
     fe4:	e115                	bnez	a0,1008 <main+0x1b0>
        if(ecmd->argv[1] == NULL)
     fe6:	689c                	ld	a5,16(s1)
     fe8:	dfd1                	beqz	a5,f84 <main+0x12c>
        else if(export(ecmd->argv) < 0)
     fea:	00848513          	addi	a0,s1,8
     fee:	fffff097          	auipc	ra,0xfffff
     ff2:	078080e7          	jalr	120(ra) # 66 <export>
     ff6:	f8055de3          	bgez	a0,f90 <main+0x138>
          fprintf(2, "export failed\n");
     ffa:	85de                	mv	a1,s7
     ffc:	4509                	li	a0,2
     ffe:	00000097          	auipc	ra,0x0
    1002:	68a080e7          	jalr	1674(ra) # 1688 <fprintf>
    1006:	b769                	j	f90 <main+0x138>
      else if(fork1() == 0) 
    1008:	fffff097          	auipc	ra,0xfffff
    100c:	3bc080e7          	jalr	956(ra) # 3c4 <fork1>
    1010:	cd01                	beqz	a0,1028 <main+0x1d0>
      wait(0);
    1012:	4501                	li	a0,0
    1014:	00000097          	auipc	ra,0x0
    1018:	2ea080e7          	jalr	746(ra) # 12fe <wait>
      free(cmd);
    101c:	8526                	mv	a0,s1
    101e:	00000097          	auipc	ra,0x0
    1022:	6ce080e7          	jalr	1742(ra) # 16ec <free>
    1026:	bf95                	j	f9a <main+0x142>
        runcmd(cmd);
    1028:	8526                	mv	a0,s1
    102a:	fffff097          	auipc	ra,0xfffff
    102e:	3c8080e7          	jalr	968(ra) # 3f2 <runcmd>
  exit(0);
    1032:	4501                	li	a0,0
    1034:	00000097          	auipc	ra,0x0
    1038:	2c0080e7          	jalr	704(ra) # 12f4 <exit>

000000000000103c <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
    103c:	1141                	addi	sp,sp,-16
    103e:	e422                	sd	s0,8(sp)
    1040:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    1042:	87aa                	mv	a5,a0
    1044:	0585                	addi	a1,a1,1
    1046:	0785                	addi	a5,a5,1
    1048:	fff5c703          	lbu	a4,-1(a1)
    104c:	fee78fa3          	sb	a4,-1(a5)
    1050:	fb75                	bnez	a4,1044 <strcpy+0x8>
    ;
  return os;
}
    1052:	6422                	ld	s0,8(sp)
    1054:	0141                	addi	sp,sp,16
    1056:	8082                	ret

0000000000001058 <strcat>:

char*
strcat(char *s, const char *t)
{
    1058:	1141                	addi	sp,sp,-16
    105a:	e422                	sd	s0,8(sp)
    105c:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
    105e:	00054783          	lbu	a5,0(a0)
    1062:	c385                	beqz	a5,1082 <strcat+0x2a>
    1064:	87aa                	mv	a5,a0
    s++;
    1066:	0785                	addi	a5,a5,1
  while(*s)
    1068:	0007c703          	lbu	a4,0(a5)
    106c:	ff6d                	bnez	a4,1066 <strcat+0xe>
  while((*s++ = *t++))
    106e:	0585                	addi	a1,a1,1
    1070:	0785                	addi	a5,a5,1
    1072:	fff5c703          	lbu	a4,-1(a1)
    1076:	fee78fa3          	sb	a4,-1(a5)
    107a:	fb75                	bnez	a4,106e <strcat+0x16>
    ;
  return os;
}
    107c:	6422                	ld	s0,8(sp)
    107e:	0141                	addi	sp,sp,16
    1080:	8082                	ret
  while(*s)
    1082:	87aa                	mv	a5,a0
    1084:	b7ed                	j	106e <strcat+0x16>

0000000000001086 <strcmp>:


int
strcmp(const char *p, const char *q)
{
    1086:	1141                	addi	sp,sp,-16
    1088:	e422                	sd	s0,8(sp)
    108a:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
    108c:	00054783          	lbu	a5,0(a0)
    1090:	cb91                	beqz	a5,10a4 <strcmp+0x1e>
    1092:	0005c703          	lbu	a4,0(a1)
    1096:	00f71763          	bne	a4,a5,10a4 <strcmp+0x1e>
    p++, q++;
    109a:	0505                	addi	a0,a0,1
    109c:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
    109e:	00054783          	lbu	a5,0(a0)
    10a2:	fbe5                	bnez	a5,1092 <strcmp+0xc>
  return (uchar)*p - (uchar)*q;
    10a4:	0005c503          	lbu	a0,0(a1)
}
    10a8:	40a7853b          	subw	a0,a5,a0
    10ac:	6422                	ld	s0,8(sp)
    10ae:	0141                	addi	sp,sp,16
    10b0:	8082                	ret

00000000000010b2 <strlen>:

uint
strlen(const char *s)
{
    10b2:	1141                	addi	sp,sp,-16
    10b4:	e422                	sd	s0,8(sp)
    10b6:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
    10b8:	00054783          	lbu	a5,0(a0)
    10bc:	cf91                	beqz	a5,10d8 <strlen+0x26>
    10be:	0505                	addi	a0,a0,1
    10c0:	87aa                	mv	a5,a0
    10c2:	4685                	li	a3,1
    10c4:	9e89                	subw	a3,a3,a0
    10c6:	00f6853b          	addw	a0,a3,a5
    10ca:	0785                	addi	a5,a5,1
    10cc:	fff7c703          	lbu	a4,-1(a5)
    10d0:	fb7d                	bnez	a4,10c6 <strlen+0x14>
    ;
  return n;
}
    10d2:	6422                	ld	s0,8(sp)
    10d4:	0141                	addi	sp,sp,16
    10d6:	8082                	ret
  for(n = 0; s[n]; n++)
    10d8:	4501                	li	a0,0
    10da:	bfe5                	j	10d2 <strlen+0x20>

00000000000010dc <memset>:

void*
memset(void *dst, int c, uint n)
{
    10dc:	1141                	addi	sp,sp,-16
    10de:	e422                	sd	s0,8(sp)
    10e0:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    10e2:	ca19                	beqz	a2,10f8 <memset+0x1c>
    10e4:	87aa                	mv	a5,a0
    10e6:	1602                	slli	a2,a2,0x20
    10e8:	9201                	srli	a2,a2,0x20
    10ea:	00a60733          	add	a4,a2,a0
    cdst[i] = c;
    10ee:	00b78023          	sb	a1,0(a5)
  for(i = 0; i < n; i++){
    10f2:	0785                	addi	a5,a5,1
    10f4:	fee79de3          	bne	a5,a4,10ee <memset+0x12>
  }
  return dst;
}
    10f8:	6422                	ld	s0,8(sp)
    10fa:	0141                	addi	sp,sp,16
    10fc:	8082                	ret

00000000000010fe <strchr>:

char*
strchr(const char *s, char c)
{
    10fe:	1141                	addi	sp,sp,-16
    1100:	e422                	sd	s0,8(sp)
    1102:	0800                	addi	s0,sp,16
  for(; *s; s++)
    1104:	00054783          	lbu	a5,0(a0)
    1108:	cb99                	beqz	a5,111e <strchr+0x20>
    if(*s == c)
    110a:	00f58763          	beq	a1,a5,1118 <strchr+0x1a>
  for(; *s; s++)
    110e:	0505                	addi	a0,a0,1
    1110:	00054783          	lbu	a5,0(a0)
    1114:	fbfd                	bnez	a5,110a <strchr+0xc>
      return (char*)s;
  return 0;
    1116:	4501                	li	a0,0
}
    1118:	6422                	ld	s0,8(sp)
    111a:	0141                	addi	sp,sp,16
    111c:	8082                	ret
  return 0;
    111e:	4501                	li	a0,0
    1120:	bfe5                	j	1118 <strchr+0x1a>

0000000000001122 <gets>:

char*
gets(char *buf, int max)
{
    1122:	711d                	addi	sp,sp,-96
    1124:	ec86                	sd	ra,88(sp)
    1126:	e8a2                	sd	s0,80(sp)
    1128:	e4a6                	sd	s1,72(sp)
    112a:	e0ca                	sd	s2,64(sp)
    112c:	fc4e                	sd	s3,56(sp)
    112e:	f852                	sd	s4,48(sp)
    1130:	f456                	sd	s5,40(sp)
    1132:	f05a                	sd	s6,32(sp)
    1134:	ec5e                	sd	s7,24(sp)
    1136:	e862                	sd	s8,16(sp)
    1138:	1080                	addi	s0,sp,96
    113a:	8baa                	mv	s7,a0
    113c:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    113e:	892a                	mv	s2,a0
    1140:	4481                	li	s1,0
    cc = read(0, &c, 1);
    1142:	faf40a93          	addi	s5,s0,-81
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
    1146:	4b29                	li	s6,10
    1148:	4c35                	li	s8,13
  for(i=0; i+1 < max; ){
    114a:	89a6                	mv	s3,s1
    114c:	2485                	addiw	s1,s1,1
    114e:	0344d763          	bge	s1,s4,117c <gets+0x5a>
    cc = read(0, &c, 1);
    1152:	4605                	li	a2,1
    1154:	85d6                	mv	a1,s5
    1156:	4501                	li	a0,0
    1158:	00000097          	auipc	ra,0x0
    115c:	1ba080e7          	jalr	442(ra) # 1312 <read>
    if(cc < 1)
    1160:	00a05e63          	blez	a0,117c <gets+0x5a>
    buf[i++] = c;
    1164:	faf44783          	lbu	a5,-81(s0)
    1168:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
    116c:	01678763          	beq	a5,s6,117a <gets+0x58>
    1170:	0905                	addi	s2,s2,1
    1172:	fd879ce3          	bne	a5,s8,114a <gets+0x28>
  for(i=0; i+1 < max; ){
    1176:	89a6                	mv	s3,s1
    1178:	a011                	j	117c <gets+0x5a>
    117a:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
    117c:	99de                	add	s3,s3,s7
    117e:	00098023          	sb	zero,0(s3)
  return buf;
}
    1182:	855e                	mv	a0,s7
    1184:	60e6                	ld	ra,88(sp)
    1186:	6446                	ld	s0,80(sp)
    1188:	64a6                	ld	s1,72(sp)
    118a:	6906                	ld	s2,64(sp)
    118c:	79e2                	ld	s3,56(sp)
    118e:	7a42                	ld	s4,48(sp)
    1190:	7aa2                	ld	s5,40(sp)
    1192:	7b02                	ld	s6,32(sp)
    1194:	6be2                	ld	s7,24(sp)
    1196:	6c42                	ld	s8,16(sp)
    1198:	6125                	addi	sp,sp,96
    119a:	8082                	ret

000000000000119c <stat>:

int
stat(const char *n, struct stat *st)
{
    119c:	1101                	addi	sp,sp,-32
    119e:	ec06                	sd	ra,24(sp)
    11a0:	e822                	sd	s0,16(sp)
    11a2:	e426                	sd	s1,8(sp)
    11a4:	e04a                	sd	s2,0(sp)
    11a6:	1000                	addi	s0,sp,32
    11a8:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = open(n, O_RDONLY);
    11aa:	4581                	li	a1,0
    11ac:	00000097          	auipc	ra,0x0
    11b0:	196080e7          	jalr	406(ra) # 1342 <open>
  if(fd < 0)
    11b4:	02054563          	bltz	a0,11de <stat+0x42>
    11b8:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
    11ba:	85ca                	mv	a1,s2
    11bc:	00000097          	auipc	ra,0x0
    11c0:	190080e7          	jalr	400(ra) # 134c <fstat>
    11c4:	892a                	mv	s2,a0
  close(fd);
    11c6:	8526                	mv	a0,s1
    11c8:	00000097          	auipc	ra,0x0
    11cc:	15e080e7          	jalr	350(ra) # 1326 <close>
  return r;
}
    11d0:	854a                	mv	a0,s2
    11d2:	60e2                	ld	ra,24(sp)
    11d4:	6442                	ld	s0,16(sp)
    11d6:	64a2                	ld	s1,8(sp)
    11d8:	6902                	ld	s2,0(sp)
    11da:	6105                	addi	sp,sp,32
    11dc:	8082                	ret
    return -1;
    11de:	597d                	li	s2,-1
    11e0:	bfc5                	j	11d0 <stat+0x34>

00000000000011e2 <atoi>:

int
atoi(const char *s)
{
    11e2:	1141                	addi	sp,sp,-16
    11e4:	e422                	sd	s0,8(sp)
    11e6:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
    11e8:	00054703          	lbu	a4,0(a0)
    11ec:	02d00793          	li	a5,45
  int neg = 1;
    11f0:	4585                	li	a1,1
  if (*s == '-') {
    11f2:	04f70363          	beq	a4,a5,1238 <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
    11f6:	00054703          	lbu	a4,0(a0)
    11fa:	fd07079b          	addiw	a5,a4,-48
    11fe:	0ff7f793          	zext.b	a5,a5
    1202:	46a5                	li	a3,9
    1204:	02f6ed63          	bltu	a3,a5,123e <atoi+0x5c>
  n = 0;
    1208:	4681                	li	a3,0
  while('0' <= *s && *s <= '9')
    120a:	4625                	li	a2,9
    n = n*10 + *s++ - '0';
    120c:	0505                	addi	a0,a0,1
    120e:	0026979b          	slliw	a5,a3,0x2
    1212:	9fb5                	addw	a5,a5,a3
    1214:	0017979b          	slliw	a5,a5,0x1
    1218:	9fb9                	addw	a5,a5,a4
    121a:	fd07869b          	addiw	a3,a5,-48
  while('0' <= *s && *s <= '9')
    121e:	00054703          	lbu	a4,0(a0)
    1222:	fd07079b          	addiw	a5,a4,-48
    1226:	0ff7f793          	zext.b	a5,a5
    122a:	fef671e3          	bgeu	a2,a5,120c <atoi+0x2a>
  return n * neg;
}
    122e:	02d5853b          	mulw	a0,a1,a3
    1232:	6422                	ld	s0,8(sp)
    1234:	0141                	addi	sp,sp,16
    1236:	8082                	ret
    s++;
    1238:	0505                	addi	a0,a0,1
    neg = -1;
    123a:	55fd                	li	a1,-1
    123c:	bf6d                	j	11f6 <atoi+0x14>
  n = 0;
    123e:	4681                	li	a3,0
    1240:	b7fd                	j	122e <atoi+0x4c>

0000000000001242 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
    1242:	1141                	addi	sp,sp,-16
    1244:	e422                	sd	s0,8(sp)
    1246:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    1248:	02b57463          	bgeu	a0,a1,1270 <memmove+0x2e>
    while(n-- > 0)
    124c:	00c05f63          	blez	a2,126a <memmove+0x28>
    1250:	1602                	slli	a2,a2,0x20
    1252:	9201                	srli	a2,a2,0x20
    1254:	00c507b3          	add	a5,a0,a2
  dst = vdst;
    1258:	872a                	mv	a4,a0
      *dst++ = *src++;
    125a:	0585                	addi	a1,a1,1
    125c:	0705                	addi	a4,a4,1
    125e:	fff5c683          	lbu	a3,-1(a1)
    1262:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
    1266:	fee79ae3          	bne	a5,a4,125a <memmove+0x18>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
    126a:	6422                	ld	s0,8(sp)
    126c:	0141                	addi	sp,sp,16
    126e:	8082                	ret
    dst += n;
    1270:	00c50733          	add	a4,a0,a2
    src += n;
    1274:	95b2                	add	a1,a1,a2
    while(n-- > 0)
    1276:	fec05ae3          	blez	a2,126a <memmove+0x28>
    127a:	fff6079b          	addiw	a5,a2,-1
    127e:	1782                	slli	a5,a5,0x20
    1280:	9381                	srli	a5,a5,0x20
    1282:	fff7c793          	not	a5,a5
    1286:	97ba                	add	a5,a5,a4
      *--dst = *--src;
    1288:	15fd                	addi	a1,a1,-1
    128a:	177d                	addi	a4,a4,-1
    128c:	0005c683          	lbu	a3,0(a1)
    1290:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
    1294:	fee79ae3          	bne	a5,a4,1288 <memmove+0x46>
    1298:	bfc9                	j	126a <memmove+0x28>

000000000000129a <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
    129a:	1141                	addi	sp,sp,-16
    129c:	e422                	sd	s0,8(sp)
    129e:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    12a0:	ca05                	beqz	a2,12d0 <memcmp+0x36>
    12a2:	fff6069b          	addiw	a3,a2,-1
    12a6:	1682                	slli	a3,a3,0x20
    12a8:	9281                	srli	a3,a3,0x20
    12aa:	0685                	addi	a3,a3,1
    12ac:	96aa                	add	a3,a3,a0
    if (*p1 != *p2) {
    12ae:	00054783          	lbu	a5,0(a0)
    12b2:	0005c703          	lbu	a4,0(a1)
    12b6:	00e79863          	bne	a5,a4,12c6 <memcmp+0x2c>
      return *p1 - *p2;
    }
    p1++;
    12ba:	0505                	addi	a0,a0,1
    p2++;
    12bc:	0585                	addi	a1,a1,1
  while (n-- > 0) {
    12be:	fed518e3          	bne	a0,a3,12ae <memcmp+0x14>
  }
  return 0;
    12c2:	4501                	li	a0,0
    12c4:	a019                	j	12ca <memcmp+0x30>
      return *p1 - *p2;
    12c6:	40e7853b          	subw	a0,a5,a4
}
    12ca:	6422                	ld	s0,8(sp)
    12cc:	0141                	addi	sp,sp,16
    12ce:	8082                	ret
  return 0;
    12d0:	4501                	li	a0,0
    12d2:	bfe5                	j	12ca <memcmp+0x30>

00000000000012d4 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
    12d4:	1141                	addi	sp,sp,-16
    12d6:	e406                	sd	ra,8(sp)
    12d8:	e022                	sd	s0,0(sp)
    12da:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
    12dc:	00000097          	auipc	ra,0x0
    12e0:	f66080e7          	jalr	-154(ra) # 1242 <memmove>
}
    12e4:	60a2                	ld	ra,8(sp)
    12e6:	6402                	ld	s0,0(sp)
    12e8:	0141                	addi	sp,sp,16
    12ea:	8082                	ret

00000000000012ec <fork>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global fork
fork:
 li a7, SYS_fork
    12ec:	4885                	li	a7,1
 ecall
    12ee:	00000073          	ecall
 ret
    12f2:	8082                	ret

00000000000012f4 <exit>:
.global exit
exit:
 li a7, SYS_exit
    12f4:	05d00893          	li	a7,93
 ecall
    12f8:	00000073          	ecall
 ret
    12fc:	8082                	ret

00000000000012fe <wait>:
.global wait
wait:
 li a7, SYS_wait
    12fe:	4bf00893          	li	a7,1215
 ecall
    1302:	00000073          	ecall
 ret
    1306:	8082                	ret

0000000000001308 <pipe>:
.global pipe
pipe:
 li a7, SYS_pipe
    1308:	03b00893          	li	a7,59
 ecall
    130c:	00000073          	ecall
 ret
    1310:	8082                	ret

0000000000001312 <read>:
.global read
read:
 li a7, SYS_read
    1312:	03f00893          	li	a7,63
 ecall
    1316:	00000073          	ecall
 ret
    131a:	8082                	ret

000000000000131c <write>:
.global write
write:
 li a7, SYS_write
    131c:	04000893          	li	a7,64
 ecall
    1320:	00000073          	ecall
 ret
    1324:	8082                	ret

0000000000001326 <close>:
.global close
close:
 li a7, SYS_close
    1326:	03900893          	li	a7,57
 ecall
    132a:	00000073          	ecall
 ret
    132e:	8082                	ret

0000000000001330 <kill>:
.global kill
kill:
 li a7, SYS_kill
    1330:	4899                	li	a7,6
 ecall
    1332:	00000073          	ecall
 ret
    1336:	8082                	ret

0000000000001338 <exec>:
.global exec
exec:
 li a7, SYS_exec
    1338:	0dd00893          	li	a7,221
 ecall
    133c:	00000073          	ecall
 ret
    1340:	8082                	ret

0000000000001342 <open>:
.global open
open:
 li a7, SYS_open
    1342:	03800893          	li	a7,56
 ecall
    1346:	00000073          	ecall
 ret
    134a:	8082                	ret

000000000000134c <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
    134c:	05000893          	li	a7,80
 ecall
    1350:	00000073          	ecall
 ret
    1354:	8082                	ret

0000000000001356 <mkdir>:
.global mkdir
mkdir:
 li a7, SYS_mkdir
    1356:	02200893          	li	a7,34
 ecall
    135a:	00000073          	ecall
 ret
    135e:	8082                	ret

0000000000001360 <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
    1360:	03100893          	li	a7,49
 ecall
    1364:	00000073          	ecall
 ret
    1368:	8082                	ret

000000000000136a <dup>:
.global dup
dup:
 li a7, SYS_dup
    136a:	48dd                	li	a7,23
 ecall
    136c:	00000073          	ecall
 ret
    1370:	8082                	ret

0000000000001372 <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
    1372:	0ac00893          	li	a7,172
 ecall
    1376:	00000073          	ecall
 ret
    137a:	8082                	ret

000000000000137c <sbrk>:
.global sbrk
sbrk:
 li a7, SYS_sbrk
    137c:	0d600893          	li	a7,214
 ecall
    1380:	00000073          	ecall
 ret
    1384:	8082                	ret

0000000000001386 <sleep>:
.global sleep
sleep:
 li a7, SYS_sleep
    1386:	48b5                	li	a7,13
 ecall
    1388:	00000073          	ecall
 ret
    138c:	8082                	ret

000000000000138e <times>:
.global times
times:
 li a7, SYS_times
    138e:	09900893          	li	a7,153
 ecall
    1392:	00000073          	ecall
 ret
    1396:	8082                	ret

0000000000001398 <test_proc>:
.global test_proc
test_proc:
 li a7, SYS_test_proc
    1398:	48d9                	li	a7,22
 ecall
    139a:	00000073          	ecall
 ret
    139e:	8082                	ret

00000000000013a0 <dev>:
.global dev
dev:
 li a7, SYS_dev
    13a0:	6889                	lui	a7,0x2
    13a2:	7668889b          	addiw	a7,a7,1894 # 2766 <__BSS_END__+0x39e>
 ecall
    13a6:	00000073          	ecall
 ret
    13aa:	8082                	ret

00000000000013ac <readdir>:
.global readdir
readdir:
 li a7, SYS_readdir
    13ac:	7d200893          	li	a7,2002
 ecall
    13b0:	00000073          	ecall
 ret
    13b4:	8082                	ret

00000000000013b6 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
    13b6:	48c5                	li	a7,17
 ecall
    13b8:	00000073          	ecall
 ret
    13bc:	8082                	ret

00000000000013be <remove>:
.global remove
remove:
 li a7, SYS_remove
    13be:	48e5                	li	a7,25
 ecall
    13c0:	00000073          	ecall
 ret
    13c4:	8082                	ret

00000000000013c6 <trace>:
.global trace
trace:
 li a7, SYS_trace
    13c6:	48c9                	li	a7,18
 ecall
    13c8:	00000073          	ecall
 ret
    13cc:	8082                	ret

00000000000013ce <sysinfo>:
.global sysinfo
sysinfo:
 li a7, SYS_sysinfo
    13ce:	48cd                	li	a7,19
 ecall
    13d0:	00000073          	ecall
 ret
    13d4:	8082                	ret

00000000000013d6 <rename>:
.global rename
rename:
 li a7, SYS_rename
    13d6:	48e9                	li	a7,26
 ecall
    13d8:	00000073          	ecall
 ret
    13dc:	8082                	ret

00000000000013de <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
    13de:	1101                	addi	sp,sp,-32
    13e0:	ec06                	sd	ra,24(sp)
    13e2:	e822                	sd	s0,16(sp)
    13e4:	1000                	addi	s0,sp,32
    13e6:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
    13ea:	4605                	li	a2,1
    13ec:	fef40593          	addi	a1,s0,-17
    13f0:	00000097          	auipc	ra,0x0
    13f4:	f2c080e7          	jalr	-212(ra) # 131c <write>
}
    13f8:	60e2                	ld	ra,24(sp)
    13fa:	6442                	ld	s0,16(sp)
    13fc:	6105                	addi	sp,sp,32
    13fe:	8082                	ret

0000000000001400 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
    1400:	7139                	addi	sp,sp,-64
    1402:	fc06                	sd	ra,56(sp)
    1404:	f822                	sd	s0,48(sp)
    1406:	f426                	sd	s1,40(sp)
    1408:	f04a                	sd	s2,32(sp)
    140a:	ec4e                	sd	s3,24(sp)
    140c:	0080                	addi	s0,sp,64
    140e:	84aa                	mv	s1,a0
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    1410:	c299                	beqz	a3,1416 <printint+0x16>
    1412:	0805c863          	bltz	a1,14a2 <printint+0xa2>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
    1416:	2581                	sext.w	a1,a1
  neg = 0;
    1418:	4881                	li	a7,0
  }

  i = 0;
    141a:	fc040993          	addi	s3,s0,-64
  neg = 0;
    141e:	86ce                	mv	a3,s3
  i = 0;
    1420:	4701                	li	a4,0
  do{
    buf[i++] = digits[x % base];
    1422:	2601                	sext.w	a2,a2
    1424:	00000517          	auipc	a0,0x0
    1428:	67450513          	addi	a0,a0,1652 # 1a98 <digits>
    142c:	883a                	mv	a6,a4
    142e:	2705                	addiw	a4,a4,1
    1430:	02c5f7bb          	remuw	a5,a1,a2
    1434:	1782                	slli	a5,a5,0x20
    1436:	9381                	srli	a5,a5,0x20
    1438:	97aa                	add	a5,a5,a0
    143a:	0007c783          	lbu	a5,0(a5)
    143e:	00f68023          	sb	a5,0(a3)
  }while((x /= base) != 0);
    1442:	0005879b          	sext.w	a5,a1
    1446:	02c5d5bb          	divuw	a1,a1,a2
    144a:	0685                	addi	a3,a3,1
    144c:	fec7f0e3          	bgeu	a5,a2,142c <printint+0x2c>
  if(neg)
    1450:	00088c63          	beqz	a7,1468 <printint+0x68>
    buf[i++] = '-';
    1454:	fd070793          	addi	a5,a4,-48
    1458:	00878733          	add	a4,a5,s0
    145c:	02d00793          	li	a5,45
    1460:	fef70823          	sb	a5,-16(a4)
    1464:	0028071b          	addiw	a4,a6,2

  while(--i >= 0)
    1468:	02e05663          	blez	a4,1494 <printint+0x94>
    146c:	fc040913          	addi	s2,s0,-64
    1470:	993a                	add	s2,s2,a4
    1472:	19fd                	addi	s3,s3,-1
    1474:	99ba                	add	s3,s3,a4
    1476:	377d                	addiw	a4,a4,-1
    1478:	1702                	slli	a4,a4,0x20
    147a:	9301                	srli	a4,a4,0x20
    147c:	40e989b3          	sub	s3,s3,a4
    putc(fd, buf[i]);
    1480:	fff94583          	lbu	a1,-1(s2)
    1484:	8526                	mv	a0,s1
    1486:	00000097          	auipc	ra,0x0
    148a:	f58080e7          	jalr	-168(ra) # 13de <putc>
  while(--i >= 0)
    148e:	197d                	addi	s2,s2,-1
    1490:	ff3918e3          	bne	s2,s3,1480 <printint+0x80>
}
    1494:	70e2                	ld	ra,56(sp)
    1496:	7442                	ld	s0,48(sp)
    1498:	74a2                	ld	s1,40(sp)
    149a:	7902                	ld	s2,32(sp)
    149c:	69e2                	ld	s3,24(sp)
    149e:	6121                	addi	sp,sp,64
    14a0:	8082                	ret
    x = -xx;
    14a2:	40b005bb          	negw	a1,a1
    neg = 1;
    14a6:	4885                	li	a7,1
    x = -xx;
    14a8:	bf8d                	j	141a <printint+0x1a>

00000000000014aa <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
    14aa:	7119                	addi	sp,sp,-128
    14ac:	fc86                	sd	ra,120(sp)
    14ae:	f8a2                	sd	s0,112(sp)
    14b0:	f4a6                	sd	s1,104(sp)
    14b2:	f0ca                	sd	s2,96(sp)
    14b4:	ecce                	sd	s3,88(sp)
    14b6:	e8d2                	sd	s4,80(sp)
    14b8:	e4d6                	sd	s5,72(sp)
    14ba:	e0da                	sd	s6,64(sp)
    14bc:	fc5e                	sd	s7,56(sp)
    14be:	f862                	sd	s8,48(sp)
    14c0:	f466                	sd	s9,40(sp)
    14c2:	f06a                	sd	s10,32(sp)
    14c4:	ec6e                	sd	s11,24(sp)
    14c6:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
    14c8:	0005c903          	lbu	s2,0(a1)
    14cc:	18090f63          	beqz	s2,166a <vprintf+0x1c0>
    14d0:	8aaa                	mv	s5,a0
    14d2:	8b32                	mv	s6,a2
    14d4:	00158493          	addi	s1,a1,1
  state = 0;
    14d8:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
    14da:	02500a13          	li	s4,37
    14de:	4c55                	li	s8,21
    14e0:	00000c97          	auipc	s9,0x0
    14e4:	560c8c93          	addi	s9,s9,1376 # 1a40 <malloc+0x2d2>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
    14e8:	02800d93          	li	s11,40
  putc(fd, 'x');
    14ec:	4d41                	li	s10,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
    14ee:	00000b97          	auipc	s7,0x0
    14f2:	5aab8b93          	addi	s7,s7,1450 # 1a98 <digits>
    14f6:	a839                	j	1514 <vprintf+0x6a>
        putc(fd, c);
    14f8:	85ca                	mv	a1,s2
    14fa:	8556                	mv	a0,s5
    14fc:	00000097          	auipc	ra,0x0
    1500:	ee2080e7          	jalr	-286(ra) # 13de <putc>
    1504:	a019                	j	150a <vprintf+0x60>
    } else if(state == '%'){
    1506:	01498d63          	beq	s3,s4,1520 <vprintf+0x76>
  for(i = 0; fmt[i]; i++){
    150a:	0485                	addi	s1,s1,1
    150c:	fff4c903          	lbu	s2,-1(s1)
    1510:	14090d63          	beqz	s2,166a <vprintf+0x1c0>
    if(state == 0){
    1514:	fe0999e3          	bnez	s3,1506 <vprintf+0x5c>
      if(c == '%'){
    1518:	ff4910e3          	bne	s2,s4,14f8 <vprintf+0x4e>
        state = '%';
    151c:	89d2                	mv	s3,s4
    151e:	b7f5                	j	150a <vprintf+0x60>
      if(c == 'd'){
    1520:	11490c63          	beq	s2,s4,1638 <vprintf+0x18e>
    1524:	f9d9079b          	addiw	a5,s2,-99
    1528:	0ff7f793          	zext.b	a5,a5
    152c:	10fc6e63          	bltu	s8,a5,1648 <vprintf+0x19e>
    1530:	f9d9079b          	addiw	a5,s2,-99
    1534:	0ff7f713          	zext.b	a4,a5
    1538:	10ec6863          	bltu	s8,a4,1648 <vprintf+0x19e>
    153c:	00271793          	slli	a5,a4,0x2
    1540:	97e6                	add	a5,a5,s9
    1542:	439c                	lw	a5,0(a5)
    1544:	97e6                	add	a5,a5,s9
    1546:	8782                	jr	a5
        printint(fd, va_arg(ap, int), 10, 1);
    1548:	008b0913          	addi	s2,s6,8
    154c:	4685                	li	a3,1
    154e:	4629                	li	a2,10
    1550:	000b2583          	lw	a1,0(s6)
    1554:	8556                	mv	a0,s5
    1556:	00000097          	auipc	ra,0x0
    155a:	eaa080e7          	jalr	-342(ra) # 1400 <printint>
    155e:	8b4a                	mv	s6,s2
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
    1560:	4981                	li	s3,0
    1562:	b765                	j	150a <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
    1564:	008b0913          	addi	s2,s6,8
    1568:	4681                	li	a3,0
    156a:	4629                	li	a2,10
    156c:	000b2583          	lw	a1,0(s6)
    1570:	8556                	mv	a0,s5
    1572:	00000097          	auipc	ra,0x0
    1576:	e8e080e7          	jalr	-370(ra) # 1400 <printint>
    157a:	8b4a                	mv	s6,s2
      state = 0;
    157c:	4981                	li	s3,0
    157e:	b771                	j	150a <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
    1580:	008b0913          	addi	s2,s6,8
    1584:	4681                	li	a3,0
    1586:	866a                	mv	a2,s10
    1588:	000b2583          	lw	a1,0(s6)
    158c:	8556                	mv	a0,s5
    158e:	00000097          	auipc	ra,0x0
    1592:	e72080e7          	jalr	-398(ra) # 1400 <printint>
    1596:	8b4a                	mv	s6,s2
      state = 0;
    1598:	4981                	li	s3,0
    159a:	bf85                	j	150a <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
    159c:	008b0793          	addi	a5,s6,8
    15a0:	f8f43423          	sd	a5,-120(s0)
    15a4:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
    15a8:	03000593          	li	a1,48
    15ac:	8556                	mv	a0,s5
    15ae:	00000097          	auipc	ra,0x0
    15b2:	e30080e7          	jalr	-464(ra) # 13de <putc>
  putc(fd, 'x');
    15b6:	07800593          	li	a1,120
    15ba:	8556                	mv	a0,s5
    15bc:	00000097          	auipc	ra,0x0
    15c0:	e22080e7          	jalr	-478(ra) # 13de <putc>
    15c4:	896a                	mv	s2,s10
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
    15c6:	03c9d793          	srli	a5,s3,0x3c
    15ca:	97de                	add	a5,a5,s7
    15cc:	0007c583          	lbu	a1,0(a5)
    15d0:	8556                	mv	a0,s5
    15d2:	00000097          	auipc	ra,0x0
    15d6:	e0c080e7          	jalr	-500(ra) # 13de <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    15da:	0992                	slli	s3,s3,0x4
    15dc:	397d                	addiw	s2,s2,-1
    15de:	fe0914e3          	bnez	s2,15c6 <vprintf+0x11c>
        printptr(fd, va_arg(ap, uint64));
    15e2:	f8843b03          	ld	s6,-120(s0)
      state = 0;
    15e6:	4981                	li	s3,0
    15e8:	b70d                	j	150a <vprintf+0x60>
        s = va_arg(ap, char*);
    15ea:	008b0913          	addi	s2,s6,8
    15ee:	000b3983          	ld	s3,0(s6)
        if(s == 0)
    15f2:	02098163          	beqz	s3,1614 <vprintf+0x16a>
        while(*s != 0){
    15f6:	0009c583          	lbu	a1,0(s3)
    15fa:	c5ad                	beqz	a1,1664 <vprintf+0x1ba>
          putc(fd, *s);
    15fc:	8556                	mv	a0,s5
    15fe:	00000097          	auipc	ra,0x0
    1602:	de0080e7          	jalr	-544(ra) # 13de <putc>
          s++;
    1606:	0985                	addi	s3,s3,1
        while(*s != 0){
    1608:	0009c583          	lbu	a1,0(s3)
    160c:	f9e5                	bnez	a1,15fc <vprintf+0x152>
        s = va_arg(ap, char*);
    160e:	8b4a                	mv	s6,s2
      state = 0;
    1610:	4981                	li	s3,0
    1612:	bde5                	j	150a <vprintf+0x60>
          s = "(null)";
    1614:	00000997          	auipc	s3,0x0
    1618:	42498993          	addi	s3,s3,1060 # 1a38 <malloc+0x2ca>
        while(*s != 0){
    161c:	85ee                	mv	a1,s11
    161e:	bff9                	j	15fc <vprintf+0x152>
        putc(fd, va_arg(ap, uint));
    1620:	008b0913          	addi	s2,s6,8
    1624:	000b4583          	lbu	a1,0(s6)
    1628:	8556                	mv	a0,s5
    162a:	00000097          	auipc	ra,0x0
    162e:	db4080e7          	jalr	-588(ra) # 13de <putc>
    1632:	8b4a                	mv	s6,s2
      state = 0;
    1634:	4981                	li	s3,0
    1636:	bdd1                	j	150a <vprintf+0x60>
        putc(fd, c);
    1638:	85d2                	mv	a1,s4
    163a:	8556                	mv	a0,s5
    163c:	00000097          	auipc	ra,0x0
    1640:	da2080e7          	jalr	-606(ra) # 13de <putc>
      state = 0;
    1644:	4981                	li	s3,0
    1646:	b5d1                	j	150a <vprintf+0x60>
        putc(fd, '%');
    1648:	85d2                	mv	a1,s4
    164a:	8556                	mv	a0,s5
    164c:	00000097          	auipc	ra,0x0
    1650:	d92080e7          	jalr	-622(ra) # 13de <putc>
        putc(fd, c);
    1654:	85ca                	mv	a1,s2
    1656:	8556                	mv	a0,s5
    1658:	00000097          	auipc	ra,0x0
    165c:	d86080e7          	jalr	-634(ra) # 13de <putc>
      state = 0;
    1660:	4981                	li	s3,0
    1662:	b565                	j	150a <vprintf+0x60>
        s = va_arg(ap, char*);
    1664:	8b4a                	mv	s6,s2
      state = 0;
    1666:	4981                	li	s3,0
    1668:	b54d                	j	150a <vprintf+0x60>
    }
  }
}
    166a:	70e6                	ld	ra,120(sp)
    166c:	7446                	ld	s0,112(sp)
    166e:	74a6                	ld	s1,104(sp)
    1670:	7906                	ld	s2,96(sp)
    1672:	69e6                	ld	s3,88(sp)
    1674:	6a46                	ld	s4,80(sp)
    1676:	6aa6                	ld	s5,72(sp)
    1678:	6b06                	ld	s6,64(sp)
    167a:	7be2                	ld	s7,56(sp)
    167c:	7c42                	ld	s8,48(sp)
    167e:	7ca2                	ld	s9,40(sp)
    1680:	7d02                	ld	s10,32(sp)
    1682:	6de2                	ld	s11,24(sp)
    1684:	6109                	addi	sp,sp,128
    1686:	8082                	ret

0000000000001688 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
    1688:	715d                	addi	sp,sp,-80
    168a:	ec06                	sd	ra,24(sp)
    168c:	e822                	sd	s0,16(sp)
    168e:	1000                	addi	s0,sp,32
    1690:	e010                	sd	a2,0(s0)
    1692:	e414                	sd	a3,8(s0)
    1694:	e818                	sd	a4,16(s0)
    1696:	ec1c                	sd	a5,24(s0)
    1698:	03043023          	sd	a6,32(s0)
    169c:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
    16a0:	8622                	mv	a2,s0
    16a2:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
    16a6:	00000097          	auipc	ra,0x0
    16aa:	e04080e7          	jalr	-508(ra) # 14aa <vprintf>
}
    16ae:	60e2                	ld	ra,24(sp)
    16b0:	6442                	ld	s0,16(sp)
    16b2:	6161                	addi	sp,sp,80
    16b4:	8082                	ret

00000000000016b6 <printf>:

void
printf(const char *fmt, ...)
{
    16b6:	711d                	addi	sp,sp,-96
    16b8:	ec06                	sd	ra,24(sp)
    16ba:	e822                	sd	s0,16(sp)
    16bc:	1000                	addi	s0,sp,32
    16be:	e40c                	sd	a1,8(s0)
    16c0:	e810                	sd	a2,16(s0)
    16c2:	ec14                	sd	a3,24(s0)
    16c4:	f018                	sd	a4,32(s0)
    16c6:	f41c                	sd	a5,40(s0)
    16c8:	03043823          	sd	a6,48(s0)
    16cc:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
    16d0:	00840613          	addi	a2,s0,8
    16d4:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
    16d8:	85aa                	mv	a1,a0
    16da:	4505                	li	a0,1
    16dc:	00000097          	auipc	ra,0x0
    16e0:	dce080e7          	jalr	-562(ra) # 14aa <vprintf>
}
    16e4:	60e2                	ld	ra,24(sp)
    16e6:	6442                	ld	s0,16(sp)
    16e8:	6125                	addi	sp,sp,96
    16ea:	8082                	ret

00000000000016ec <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
    16ec:	1141                	addi	sp,sp,-16
    16ee:	e422                	sd	s0,8(sp)
    16f0:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
    16f2:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    16f6:	00001797          	auipc	a5,0x1
    16fa:	cba7b783          	ld	a5,-838(a5) # 23b0 <freep>
    16fe:	a02d                	j	1728 <free+0x3c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    1700:	4618                	lw	a4,8(a2)
    1702:	9f2d                	addw	a4,a4,a1
    1704:	fee52c23          	sw	a4,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
    1708:	6398                	ld	a4,0(a5)
    170a:	6310                	ld	a2,0(a4)
    170c:	a83d                	j	174a <free+0x5e>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    170e:	ff852703          	lw	a4,-8(a0)
    1712:	9f31                	addw	a4,a4,a2
    1714:	c798                	sw	a4,8(a5)
    p->s.ptr = bp->s.ptr;
    1716:	ff053683          	ld	a3,-16(a0)
    171a:	a091                	j	175e <free+0x72>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
    171c:	6398                	ld	a4,0(a5)
    171e:	00e7e463          	bltu	a5,a4,1726 <free+0x3a>
    1722:	00e6ea63          	bltu	a3,a4,1736 <free+0x4a>
{
    1726:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    1728:	fed7fae3          	bgeu	a5,a3,171c <free+0x30>
    172c:	6398                	ld	a4,0(a5)
    172e:	00e6e463          	bltu	a3,a4,1736 <free+0x4a>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
    1732:	fee7eae3          	bltu	a5,a4,1726 <free+0x3a>
  if(bp + bp->s.size == p->s.ptr){
    1736:	ff852583          	lw	a1,-8(a0)
    173a:	6390                	ld	a2,0(a5)
    173c:	02059813          	slli	a6,a1,0x20
    1740:	01c85713          	srli	a4,a6,0x1c
    1744:	9736                	add	a4,a4,a3
    1746:	fae60de3          	beq	a2,a4,1700 <free+0x14>
    bp->s.ptr = p->s.ptr->s.ptr;
    174a:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
    174e:	4790                	lw	a2,8(a5)
    1750:	02061593          	slli	a1,a2,0x20
    1754:	01c5d713          	srli	a4,a1,0x1c
    1758:	973e                	add	a4,a4,a5
    175a:	fae68ae3          	beq	a3,a4,170e <free+0x22>
    p->s.ptr = bp->s.ptr;
    175e:	e394                	sd	a3,0(a5)
  } else
    p->s.ptr = bp;
  freep = p;
    1760:	00001717          	auipc	a4,0x1
    1764:	c4f73823          	sd	a5,-944(a4) # 23b0 <freep>
}
    1768:	6422                	ld	s0,8(sp)
    176a:	0141                	addi	sp,sp,16
    176c:	8082                	ret

000000000000176e <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
    176e:	7139                	addi	sp,sp,-64
    1770:	fc06                	sd	ra,56(sp)
    1772:	f822                	sd	s0,48(sp)
    1774:	f426                	sd	s1,40(sp)
    1776:	f04a                	sd	s2,32(sp)
    1778:	ec4e                	sd	s3,24(sp)
    177a:	e852                	sd	s4,16(sp)
    177c:	e456                	sd	s5,8(sp)
    177e:	e05a                	sd	s6,0(sp)
    1780:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
    1782:	02051493          	slli	s1,a0,0x20
    1786:	9081                	srli	s1,s1,0x20
    1788:	04bd                	addi	s1,s1,15
    178a:	8091                	srli	s1,s1,0x4
    178c:	00148a1b          	addiw	s4,s1,1
    1790:	0485                	addi	s1,s1,1
  if((prevp = freep) == 0){
    1792:	00001517          	auipc	a0,0x1
    1796:	c1e53503          	ld	a0,-994(a0) # 23b0 <freep>
    179a:	c515                	beqz	a0,17c6 <malloc+0x58>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    179c:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
    179e:	4798                	lw	a4,8(a5)
    17a0:	04977163          	bgeu	a4,s1,17e2 <malloc+0x74>
    17a4:	89d2                	mv	s3,s4
    17a6:	000a071b          	sext.w	a4,s4
    17aa:	6685                	lui	a3,0x1
    17ac:	00d77363          	bgeu	a4,a3,17b2 <malloc+0x44>
    17b0:	6985                	lui	s3,0x1
    17b2:	00098b1b          	sext.w	s6,s3
  p = sbrk(nu * sizeof(Header));
    17b6:	0049999b          	slliw	s3,s3,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
    17ba:	00001917          	auipc	s2,0x1
    17be:	bf690913          	addi	s2,s2,-1034 # 23b0 <freep>
  if(p == (char*)-1)
    17c2:	5afd                	li	s5,-1
    17c4:	a8a5                	j	183c <malloc+0xce>
    base.s.ptr = freep = prevp = &base;
    17c6:	00001797          	auipc	a5,0x1
    17ca:	bea78793          	addi	a5,a5,-1046 # 23b0 <freep>
    17ce:	00001717          	auipc	a4,0x1
    17d2:	bea70713          	addi	a4,a4,-1046 # 23b8 <base>
    17d6:	e398                	sd	a4,0(a5)
    17d8:	e798                	sd	a4,8(a5)
    base.s.size = 0;
    17da:	0007a823          	sw	zero,16(a5)
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    17de:	87ba                	mv	a5,a4
    17e0:	b7d1                	j	17a4 <malloc+0x36>
      if(p->s.size == nunits)
    17e2:	02e48c63          	beq	s1,a4,181a <malloc+0xac>
        p->s.size -= nunits;
    17e6:	4147073b          	subw	a4,a4,s4
    17ea:	c798                	sw	a4,8(a5)
        p += p->s.size;
    17ec:	02071693          	slli	a3,a4,0x20
    17f0:	01c6d713          	srli	a4,a3,0x1c
    17f4:	97ba                	add	a5,a5,a4
        p->s.size = nunits;
    17f6:	0147a423          	sw	s4,8(a5)
      freep = prevp;
    17fa:	00001717          	auipc	a4,0x1
    17fe:	baa73b23          	sd	a0,-1098(a4) # 23b0 <freep>
      return (void*)(p + 1);
    1802:	01078513          	addi	a0,a5,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
    1806:	70e2                	ld	ra,56(sp)
    1808:	7442                	ld	s0,48(sp)
    180a:	74a2                	ld	s1,40(sp)
    180c:	7902                	ld	s2,32(sp)
    180e:	69e2                	ld	s3,24(sp)
    1810:	6a42                	ld	s4,16(sp)
    1812:	6aa2                	ld	s5,8(sp)
    1814:	6b02                	ld	s6,0(sp)
    1816:	6121                	addi	sp,sp,64
    1818:	8082                	ret
        prevp->s.ptr = p->s.ptr;
    181a:	6398                	ld	a4,0(a5)
    181c:	e118                	sd	a4,0(a0)
    181e:	bff1                	j	17fa <malloc+0x8c>
  hp->s.size = nu;
    1820:	01652423          	sw	s6,8(a0)
  free((void*)(hp + 1));
    1824:	0541                	addi	a0,a0,16
    1826:	00000097          	auipc	ra,0x0
    182a:	ec6080e7          	jalr	-314(ra) # 16ec <free>
  return freep;
    182e:	00093503          	ld	a0,0(s2)
      if((p = morecore(nunits)) == 0)
    1832:	d971                	beqz	a0,1806 <malloc+0x98>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    1834:	611c                	ld	a5,0(a0)
    if(p->s.size >= nunits){
    1836:	4798                	lw	a4,8(a5)
    1838:	fa9775e3          	bgeu	a4,s1,17e2 <malloc+0x74>
    if(p == freep)
    183c:	00093703          	ld	a4,0(s2)
    1840:	853e                	mv	a0,a5
    1842:	fef719e3          	bne	a4,a5,1834 <malloc+0xc6>
  p = sbrk(nu * sizeof(Header));
    1846:	854e                	mv	a0,s3
    1848:	00000097          	auipc	ra,0x0
    184c:	b34080e7          	jalr	-1228(ra) # 137c <sbrk>
  if(p == (char*)-1)
    1850:	fd5518e3          	bne	a0,s5,1820 <malloc+0xb2>
        return 0;
    1854:	4501                	li	a0,0
    1856:	bf45                	j	1806 <malloc+0x98>
