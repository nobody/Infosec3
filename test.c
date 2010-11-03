//This program is not compilable.
// By Edwin Sha

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void copy1( const char *a ){
	char buf[320];
        int i, j, k;
        printf("Inside COPY\n"); 
	strcpy(buf,a);
}

void Doing_nothing() {
  int i, a[200];
  printf("Inside Doing_nothing\n");
  for (i=0; i < 100; i++)
      a[i] =2;
}

void main( int argc, char *argv[] ) {
        
        printf("\n *********************************\n");
        printf("This is a newly developed WEB server. \n");
        printf(" ****************************************\n") ;
        printf(" ******The web server is executing*******\n") ;
        printf(" ****************************************\n") ;
	if (argc >=2 ) {
	      Doing_nothing();
	      copy1( argv[1] );
	}      
}
