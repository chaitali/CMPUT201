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

const char* const exec_name = "java -jar Sketchpad.jar ";

int main( int argc, char * argv[] ){
  int i;
  FILE *fp, *executable; 
  struct aLine *theLines;
  struct image *eachImage;
  
  /* initialize all global variables */
  imgNum = 0;
  currentLine = 0;
  initSize = INIT_IMAGE;
  bufferSize = MAX_BUF;

  /* initialize both structures */
  theLines = calloc( bufferSize, sizeof(struct aLine));
  eachImage = calloc( initSize, sizeof(struct image));
  
  /* try opening file */
  for( i = 1; i < argc; i++ ){
    fp = fopen( argv[ i ], "r" );

    /* if invalid, free allocated memory and exit*/
    if( fp == NULL || argc > 2){
      fprintf(stderr, "draw2: %d, error.\n", 0 );
      free( eachImage );
      free( theLines );
      exit( EXIT_FAILURE );
    }
    else{
      /* print date and input file */
      printf( "%s started on ", argv[ 0 ] );
      fflush( NULL );
      system( "date" );
      printf( "Input file: %s\n", argv[ 1 ] ); 
      
      executable = popen(exec_name, "w"); /* open pipe to sketchpad */
      bufferFile( fp, theLines, eachImage, executable );
      pclose(executable);
      fclose( fp );
      exit(EXIT_SUCCESS);
    }

  } /* end for */
  return( 0 );
} /* main */


void cleanup( FILE*  executable, struct image * eachImage, FILE* fp, struct aLine * theLines  ) {
  int j;
  fprintf(stderr, "draw2: %d, error.\n", ( currentLine + 1) );
  /* free each images lineTo allocated memory */
  for( j = 0; j < imgNum; j++){
        free( eachImage[ j ].x);
        free( eachImage[ j ].y);
      } 
  /* if image left incompleted free memory */
  if( imgNum == 0){
    free( eachImage[ 0 ].x); 
    free( eachImage[ 0 ].y);
  }
  /* free struct memory */
  free( eachImage );
  free( theLines );
  /* send end cmd to sketchpad */
  fprintf( executable, "end\n" );
  /* close pipe to sketchpad and input file */
  pclose( executable );
  fclose( fp );
  exit( EXIT_FAILURE );

} /* cleanup */
