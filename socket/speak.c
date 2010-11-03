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

int main (int argc, void *argv[])
{
  int s,port;
  char host[64];
  struct hostent *hp;  
  struct sockaddr_in sin;  
  
  int result, len;
  char str[MAXL],buf[MAXL];
  
  /* read host and port number from command line */
  if ( argc != 3 ) 
  { fprintf(stderr, 
     "Usage: %s <listen-machine-name> <port-number>\n", argv[0]);
    exit(1);
  }

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
  while ( gets(str) != NULL )  
    { // send a string (including the '\0' char at the end of the
      // string) through the connection to the listener
      len = send(s, str, strlen(str)+1, 0);
      if ( len != strlen(str)+1 ) 
	{ perror("send");
	  exit(1);
	}

      // get the ACK back from the listener
      len = recv(s, buf, 512, 0);
      if ( len < 0 ) 
      { perror("recv");
	exit(1);
      }
      printf("ACK I got: %s\n", buf);
  }

  
  /* when finished sending, tell host you are closing and close */
  printf(">> Send out message for disconnection.\n");
  send(s, "CLOSE\0", 6, 0);
  close(s);
  exit(0);
}

