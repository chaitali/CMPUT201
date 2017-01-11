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
#include "draw1b.h"


int main( int argc, char * argv[] ){
  int i;
  imgArray = 10; 
  FILE *fp, *executable; 
  struct aLine theLines[ MAX_LINES ]; /* An array of structs */
  //struct image eachImage[20];
  struct image *eachImage =  malloc( 20 * sizeof( *eachImage ) );
  /* initialize global variables */
  imgNum = 0;
  place = 0;
  currentLine = 0;
  saveNum = 0;

  /* initialize both structures */
  memset( eachImage, 0, sizeof( eachImage ));
  memset( theLines, 0, sizeof( theLines ) ); 

  /* try opening file */
  for( i = 1; i < argc; i++ ){
    fp = fopen( argv[ i ], "r" );
    if( fp == NULL ){
      printf( "Could not open file %s\n", argv[ i ] );
    } 
    else{
      /* print date and input file */
      printf( "%s started on ", argv[ 0 ] );
      fflush( NULL );
      system( "date" );
      printf( "Input file: %s\n", argv[ 1 ] ); 
      
      executable = popen(exec_name, "w"); /* open pipe to sketchpad */
      bufferFile( fp, theLines, *eachImage, executable );
      pclose(executable);
      fclose( fp );
    }
  } /* end for */
  return( 0 );
} /* main */

void bufferFile( FILE *fp, struct aLine *theLines, struct image eachImage, FILE *executable ){
  
  char * rbuf;

  /* Line-oriented */
  while( 1 ) {
    /* check if the end of file has been reached */
    if ( feof ( fp ) ) {
      break;
    }
    rbuf = fgets( theLines[ currentLine ].line, MAX_BUF, fp );
    if( rbuf == NULL ){
      break;
    }
    classifyLine( theLines, eachImage, executable );
    fflush( stdout );
    currentLine++;
  } /* while */

} /* bufferFile */


void classifyLine( struct aLine * theLines, struct image eachImage, FILE* executable ){
 
  char firstWord[ MAX_BUF ], child[ MAX_BUF ], imgName[ MAX_NAME ]; 
  char command, currentImg, *Img;   
  double xCoor, yCoor, xCoor1, yCoor1;
  double move_x, move_y; 
  int rval, i, j, len; 
  
  /* Initialize arrays imgName, child and firstWord */
  memset( imgName, 0, sizeof( imgName ));
  memset( child, 0, sizeof( child ));
  memset( firstWord, 0, MAX_BUF );

  /* Check for comments 
  if( theLines[ currentLine ].line[ 0 ] == '#' ){
    printf( "%s", theLines[ currentLine ].line );
    return;
  }*/
  
  /* Scan in the first word */
  rval = sscanf( theLines[ currentLine ].line, "%s", firstWord );
  
  /* If line is blank return */
  if( rval != 1 ) {
    return;
  }
  /* check for comments */
  if( strncmp( firstWord, "#", MAX_BUF ) == 0 ){
    printf( "%s", theLines[ currentLine ].line );
    return;
  }

 
  /* Add each new image to an array */
  if( strncmp( firstWord, "Image", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
     &command, imgName, &xCoor, &yCoor) );  
    if( imgNum > 0 ) {
      for( i = 0; i < imgNum; i++ ) {
        Img = eachImage[ i ].name; 
        if( strncmp ( Img, imgName, MAX_NAME ) == 0) {
          saveNum = imgNum;
          imgNum = i;
          break; 
        }
      } /* end for */
    }
    /* Copy each (x,y ) coordinate and image name */
    memcpy( eachImage[ imgNum ].name, imgName , sizeof( imgName ) );
    eachImage[ imgNum ].x[ place ] = xCoor;
    eachImage[ imgNum ].y[ place ] = yCoor;
    place++;
    
    /* allocate extra memory if necessary */
    //if( imgNum > imgArray) {
      //eachImage = realloc( eachImage, ADD_MEMORY * sizeof( struct image ) ); 
      //imgArray = imgArray + ADD_MEMORY;
    //}
  }


  /* Add other coordinates to the array */ 
  if( strncmp( firstWord, "lineTo", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s %lf %lf", &command, &xCoor1, &yCoor1) );
     xCoor = xCoor + xCoor1; 
     yCoor = yCoor + yCoor1;  
     eachImage[ imgNum ].x[ place ] = xCoor;
     eachImage[ imgNum ].y[ place ] = yCoor;
     place++; 
  }
  
  /* Reset coordinate place to 0, move to next image */
  if( strncmp( firstWord, "End", MAX_BUF ) == 0 ){
    eachImage[ imgNum ].size = place;
    if( imgNum < saveNum ) {
      imgNum = saveNum;
    }
    else {
      imgNum++;
    }
    place = 0;
     /* allocate extra memory if necessary 
    if( imgNum > imgArray) {
      eachImage = realloc( eachImage, ADD_MEMORY * sizeof( *eachImage ) );
      imgArray = imgArray + ADD_MEMORY;
    }*/

  }

  /* Print the coordinates of select image */  
  if( strncmp( firstWord, "print", MAX_BUF ) == 0 ) {
    print( theLines, *eachImage ); 
  }

  /* Draw image in SketchPad */
  if( strncmp( firstWord, "draw", MAX_BUF ) == 0 ) {
    drawImg( theLines, *eachImage, executable ); 
  }
 
  /* send child commands to SketchPad */
  if ( strncmp( firstWord, "child", MAX_BUF ) == 0 ) {
    strncpy(child, theLines[ currentLine ].line  + 5,  MAX_BUF );    
    fprintf( executable, "%s", child );
  }
  /* translate (x,y) coordinates by shifts */ 
  if ( strncmp( firstWord, "translate", MAX_BUF ) == 0 ) {
    sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
    &command, &currentImg, &move_x, &move_y);
    for( i = 0; i < imgNum; i++ ) {
      Img = eachImage[ i ].name;
      if(  strncmp( Img, &currentImg, MAX_NAME ) == 0 ) {
        len = eachImage[ i ].size;
        for(j  = 0; j < len ; j++ ) {
          eachImage[ i ].x[ j ] = eachImage[ i ].x[ j ] + move_x;
          eachImage[ i ].y[ j ] = eachImage[ i ].y[ j ] + move_y;
        } /* end inner for */
      }
    } /* end outer for */
  }

} /* classifyLine */

void print( struct aLine * theLines, struct image eachImage ) {
 
  char currentImg, command; 
  int i, j, len; 
  char *Img; 

  sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
    
  for( i = 0; i < imgNum; i++ ) {
    Img = eachImage[ i ].name;
    if( strncmp( Img, &currentImg, MAX_NAME ) == 0 ) {
      len = eachImage[ i ].size;
      if( len == 1 ){          /* if there is no lineTo commands */
        printf( "Print Image %s = \n", Img );
        printf( "End Image %s \n", Img ); 
        break; 
      }
      printf( "Print Image %s = \n", Img );
      for( j = 0; j < len ; j++ ) {
        printf( "%ld %ld\n", lround( eachImage[ i ].x[ j ] ), 
        lround( eachImage[ i ].y[ j ] ) );
      } /* end inner for*/
      printf( "End Image %s \n", Img );
    }
  } /* end outer for */

} /* end print */

void drawImg ( struct aLine * theLines, struct image eachImage, FILE*  executable ) {
  
  char command, currentImg, *Img;
  int i, j, len; 
 
  sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
  for( i = 0; i < imgNum; i++ ){
    Img = eachImage[ i ].name;
    if( strncmp( Img, &currentImg, MAX_NAME ) == 0 ) {
      len = eachImage[ i ].size;
      for(j = 0; j < (len - 1); j++) {
        fprintf( executable, "drawSegment %ld %ld %ld %ld\n",
        lround( eachImage[ i ].x[ j ] ), lround( eachImage[ i ].y[ j ] ),
        lround( eachImage[ i ].x[ j + 1 ] ), lround( eachImage[ i ].y[ j + 1 ] ) ); 
      }  /* end inner for */
    }
  } /* end outer for */

} /* end drawImg */

