/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * Based on the program finddw and buffer
 */


#include <stdio.h>
#include <string.h> /* For memset() */
#include <math.h>
#include <stdlib.h> /* For exit() */
#include "draw2.h"
#include "memwatch.h"


void bufferFile( FILE *fp, struct aLine *theLines, struct image *eachImage, FILE *executable ){

  char * rbuf;

  /* Line-oriented */
  while( 1 ) {

    /* check if the end of file has been reached */
    if ( feof (fp) ){
      break;
    }
    rbuf = fgets( theLines[ currentLine ].line, MAX_NAME, fp );
    /* if rbuf is NULL, break out of while */
    if( rbuf == NULL ){
      break;
    }
    /* if there are too many lines, allocate more memory */
    if( bufferSize == ( currentLine + 1 ) ){
       bufferSize = bufferSize * 2;
       theLines = realloc( theLines, bufferSize  * sizeof( struct aLine ) );
    }
    /* if there are too many images, allocate more memory */
    if( ( imgNum ) == (initSize ) ) {
      initSize = initSize * 2;
      eachImage = realloc( eachImage, initSize  * sizeof( struct image ) );
    }

    classifyLine( theLines, eachImage, executable, fp );
    fflush( stdout );
    currentLine++;
  } /* while */


} /* bufferFile */
