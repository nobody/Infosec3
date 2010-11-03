// Written by Edwin Sha

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void Doing_nothing() {
  int i, a[400];
  for (i=0; i< 100; i++)
     a[i]++;
}

void copy2( const char *a ){
	char buf[360];
        int i, j, k;
        printf("* Inside COPY2", buf); 
	strcpy(buf,a);
        printf("* Leaving COPY2\n");
}

void copy1(const char *a){
       char foo[200];
       int i, j, k;
       printf("Inside COPY1\n", foo);
       copy2(a);
       Doing_nothing();
//       printf("Leaving COPY1\n");
}

void main( int argc, char *argv[] ) {
        
        printf("\n *********************************\n");
        printf("This is a WEB server. Entering the WEB server\n");
        printf(" **********************************\n") ;
	if (argc >=2 )copy1( argv[1] );
        printf("\n Good Bye! The WEB server is over. Exit. \n") ;
        printf(" **********************************\n") ;
}

