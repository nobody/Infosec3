/******************************************************************************
*
*  File Name........: listen.c  For Dr. Edwin Sha's security course
*  Usage: listen <port-number>
*  Example: login apache.utdallas.edu, and type
*           listen 7000
*
*  Description......:
*      Creates a program that establishes a passive socket.  The socket
*  accepts connection from the speak.cc program.  While the connection is
*  open, listen will print the characters that come down the socket.
*
*  Listen takes a single argument, the port number of the socket.  Choose
*  a number that isn't assigned.  Invoke the speak.c program with the
*  same port number.
*
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

#define N 2        /* maximum connections to the socket */
#define MAXL 2048   /* max string length */

int socket_dp;  /* s: socket descriptor */
int conn_dp;  /* p: connection descriptor (similar to file descriptor)
  		        which you will use to send and receive data through the connection*/
char ack_msg[]="Got Your Message.";
	    /* Acknowledeg message sent to the client*/

void process(char * a )
{
	char buf[320];
	char *p=buf;
	int i;

	strcpy(buf, a);

// You may uncoment it when you test your prorgam :) -- Edwin Sha
//	printf("\nHint: The buffer addr - %p.\n", buf); 
	printf("I Got it\n"); 
}

void process1(char * buf)
{
        char foo[300];
	int i;
	for (i=0; i < 100; i++);

        process(buf);
}

int main (int argc, char *argv[])
{
  char buf[MAXL];
  char host[64];

  /*socket structure variables */
  struct hostent *hp, *ihp;
  struct sockaddr_in sin, incoming;
  int port; 

  int i, result, len; 

  /*********************************************************************/
  /*                       command line parsing                        */
  /*********************************************************************/
  
  if ( argc != 2 ) 
  { 
  	fprintf(stderr, "Usage: %s <port-number> \n", argv[0]);
    exit(1);
  }
  
  /* get port number */
  port = atoi((char*)argv[1]);
    
  /* get my host name and fill in hostent struct */
  gethostname(host, sizeof host);
  hp = gethostbyname(host);
  if ( hp == NULL ) 
  { 
  	fprintf(stderr, "%s: host not found (%s)\n", argv[0], host);
    exit(1);
  }

  /***************************************************
     open a socket for listening
     4 steps:
   	1. create socket
   	2. bind it to an address/port
	3. listen
   	4. accept a connection
  *****************************************************/

  /* create passive socket:
     use address family INET and STREAMing sockets (TCP) */
  socket_dp = socket(AF_INET, SOCK_STREAM, 0);
  if ( socket_dp < 0 ) 
  { 
  	perror("socket:");
    exit(socket_dp);
  }

  /* set up the address and port */
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);
  
  /* bind socket s to address sin */
  result = bind(socket_dp, (struct sockaddr *)&sin, sizeof(sin));
  if ( result < 0 ) 
  { 
  	perror("bind:");
    exit(result);
  }

  /* listen to the network for connections */
  result = listen(socket_dp, N);
  if ( result < 0 ) 
  {
    perror("listen:");
    exit(result);
  }
  
  /*********************************************************************/
  /*                       accept connections                          */
  /*********************************************************************/
  
  len = sizeof(struct sockaddr_in);
  /* wait to accept a connection */ 
  conn_dp = accept(socket_dp, (struct sockaddr *)&incoming, (socklen_t *) &len); 
  if ( conn_dp < 0 ) 
  { 
  	perror("accept:");
    exit(result);
  }
  ihp = gethostbyaddr((char *)&incoming.sin_addr,sizeof(struct in_addr), AF_INET);
  printf(">> Connected to %s\n", ihp->h_name);
 
  /* read and print strings sent over the connection */
  while ( 1 ) 
	{ 	
    	len = recv(conn_dp, buf, MAXL, 0);
    	if ( len < 0 ) 
		{ 
	 		perror("recv");
			exit(1);
		}
      
    	if ( strcmp("CLOSE", buf)==0 )
			break;

		process1(buf);
	
    	/* Send ACK string (including the last char '\0') */
    	/* through the connection                         */
      
		len = send(conn_dp, ack_msg, strlen(ack_msg)+1, 0);

    	if ( len != strlen(ack_msg)+1 ) 
		{ 
	  		perror("send");
	  		exit(1);
		}	
  	}
    
  	close(conn_dp);
  	printf(">> Connection closed\n");
  	close(socket_dp);
}


