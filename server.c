void copy( const char *a ){
	char buf[256];
        int i, j, k;
        printf("Inside COPY, &buf = %p\n", buf); 
	strcpy(buf,a);
}

void main( int argc, char *argv[] ) {
        
        printf("\n *********************************\n");
        printf("This is a newly developed WEB server. \n");
        printf(" ****************************************\n") ;
        printf(" ******The web server is executing*******\n") ;
	if (argc >=2 )copy( argv[1] );
}
