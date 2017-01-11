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

int initSize; 

int main( int argc, char * argv[] ){
  int i; //initSize;
  initSize = 5; 
  FILE *fp, *executable; 
  struct aLine theLines[ MAX_LINES ]; /* An array of structs */
  struct image *eachImage;
  //struct image eachImage;
  eachImage = malloc( initSize * sizeof(struct image));
  printf("%d", eachImage);
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
      bufferFile( fp, theLines, eachImage, executable );
      pclose(executable);
      fclose( fp );
    }
  } /* end for */
  return( 0 );
} /* main */

void bufferFile( FILE *fp, struct aLine *theLines, struct image *eachImage, FILE *executable ){
  
  char * rbuf;

  /* Line-oriented */
  while( 1 ) {
    /* check if the end of file has been reached */
    if ( feof ( fp ) ) {
      break;
    }
    rbuf = fgets( theLines[ currentLine ].line, MAX_BUF, fp );
    if( rbuf == NULL ){
      printf("cool");
      break;
    }
    if( ( imgNum ) == (initSize ) ) {
      //printf("XX%dXX", eachImage);
      //b = biggerArray( &eachImage);
      printf("%d \n", currentLine);
      initSize = initSize * 2;
      eachImage = realloc( eachImage, initSize  * sizeof( struct image ) );
      //currentLine--;
      printf("%d \n", currentLine);
     //printf("//A%d b%d//", initSize, imgNum);
     //*eachImage = *eachImage;
      //printf("//A%d b%d//", initSize, eachImage);
    }
    classifyLine( theLines, eachImage, executable );
    printf( "n: %s %d \n", eachImage[ 10 ].name, imgNum);
    fflush( stdout );
    currentLine++;
  } /* while */

} /* bufferFile */


void classifyLine( struct aLine * theLines, struct image * eachImage, FILE* executable ){
 
  char firstWord[ MAX_BUF ], child[ MAX_BUF ], imgName[ MAX_NAME ]; 
  char command, currentImg, *Img, secondWord[ MAX_BUF];   
  double xCoor, yCoor, xCoor1, yCoor1;
  double move_x, move_y; 
  int rval, i, j, len; 
  printf( "hello: %s %d \n", eachImage[ 10 ].name, imgNum);  
  /* Initialize arrays imgName, child and firstWord */
  //memset( imgName, 0, sizeof( imgName ));
  memset( child, 0, sizeof( child ));
  memset( firstWord, 0, MAX_BUF );

  /* Check for comments */
  //if( theLines[ currentLine ].line[ 0 ] == '#' ){
    //printf( "%s", theLines[ currentLine ].line );
    //return;
  //}
  
  /* Scan in the first word */
  rval = sscanf( theLines[ currentLine ].line, "%s", firstWord );
  if( rval != 1 ) {
    printf( "Error in sscanf\n" );
  return;
  }

   /* check for comments */
  if( strncmp( firstWord, "#", MAX_BUF ) == 0 ){
    printf( "%s", theLines[ currentLine ].line );
    //return;
  }
  int b;
 
  /* Add each new image to an array */
  if( strncmp( firstWord, "Image", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
     &command, imgName, &xCoor, &yCoor) );  
    //printf("uwu %d %d uwu", imgNum, initSize);
   //if( imgNum == (initSize )) {
      //printf("XX%dXX", eachImage);
      //b = biggerArray( &eachImage);
     // printf("%d \n", currentLine);
      //initSize = initSize * 2;
      //eachImage = realloc( eachImage, initSize  * sizeof( struct image ) );
      //currentLine--;
     // printf("%d \n", currentLine); 
     //printf("//A%d b%d//", initSize, imgNum);
     //*eachImage = *eachImage;
      //printf("//A%d b%d//", initSize, eachImage);
    //}
   //printf("1#");
    if( imgNum > 0 ) {
//printf("who");
      
      for( i = 0; i < imgNum; i++ ) {
        //printf(">>%d", imgNum);
        Img = eachImage[ i ].name; 
        if( strncmp ( Img, imgName, MAX_NAME ) == 0) {
          saveNum = imgNum;
          imgNum = i;
          break; 
        }
      }
    }
    //printf("RR %d RR", imgNum);
    /* Copy each (x,y ) coordinate and image name */
    memcpy( eachImage[ imgNum ].name, imgName , sizeof( imgName ) );
    eachImage[ imgNum ].x[ place ] = xCoor;
    eachImage[ imgNum ].y[ place ] = yCoor;
    //printf( "oo%s %doo", eachImage[ 10 ].name, imgNum);
    place++;

  /*if( imgNum == (initSize - 1)) {
      printf("XX%dXX", imgNum);
      b = biggerArray( &eachImage);
      printf("//A%d b%d//", initSize, imgNum);
  }*/
   printf( "First: %s %d \n", eachImage[ 10 ].name, imgNum);
  //return;
}
 printf( "ugh: %s %d \n", eachImage[ 10 ].name, imgNum);

  /* Add other coordinates to the array */ 
  if( strncmp( firstWord, "lineTo", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s %lf %lf", &command, &xCoor1, &yCoor1) );
     printf( "lineTo: %s %d \n", eachImage[ 10 ].name, imgNum);
     xCoor = xCoor + xCoor1; 
     yCoor = yCoor + yCoor1;  
     eachImage[ imgNum ].x[ place ] = xCoor;
     eachImage[ imgNum ].y[ place ] = yCoor;
     place++;
     //printf("ccool"); 
  //return;
  }
  
  /* Reset coordinate place to 0, move to next image */
  if( strncmp( firstWord, "End", MAX_BUF ) == 0 ){
    
//if( ( imgNum ) == (initSize )) {
      //printf("XX%dXX", eachImage);
      //b = biggerArray( &eachImage);
  //    printf("%d \n", currentLine);
    //  initSize = initSize * 2;
     // eachImage = realloc( eachImage, initSize  * sizeof( struct image ) );
      //currentLine--;
     // printf("%d \n", currentLine);
     //printf("//A%d b%d//", initSize, imgNum);
     //*eachImage = *eachImage;
      //printf("//A%d b%d//", initSize, eachImage);
   // }

eachImage[ imgNum ].size = place;
    printf( "End: %s %d \n", eachImage[ 10 ].name, imgNum);    
//printf("YY%dYY", imgNum);
    if( imgNum < saveNum ) {
      imgNum = saveNum;
    }
    else {
      imgNum++;
    }
    place = 0;
    //printf("//A%d B%d//", initSize, imgNum);
    /*if( imgNum >= (initSize - 3)) {
      printf("XX%dXX", imgNum);
      biggerArray( &eachImage);
      printf("//A%d b%d//", initSize, imgNum); 

      printf("//A%d B%d//", initSize, imgNum);
      initSize = initSize *  2;
      printf("//C%d//", initSize);
      printf("//%p//", eachImage);
      eachImage = realloc( eachImage, initSize  * sizeof( *eachImage));
      printf("//%p//", eachImage);
      printf("%d", sizeof(struct image));
      //initSize = initSize + 10;
      }*/
  }

  /* Print the coordinates of select image */  
  if( strncmp( firstWord, "print", MAX_BUF ) == 0 ) {
    printf( "Print: %s %d \n", eachImage[ 10 ].name, imgNum);
    print( theLines, eachImage ); 
  }

  /* Draw image in SketchPad */
  if( strncmp( firstWord, "draw", MAX_BUF ) == 0 ) {
    drawImg( theLines, eachImage, executable ); 
  }
 
  /* send child commands to SketchPad */
  if ( strncmp( firstWord, "child", MAX_BUF ) == 0 ) {
    strncpy(child, theLines[ currentLine ].line  + 5,  MAX_BUF );    
    fprintf( executable, "%s", child );
    //sscanf( theLines[ currentLine ].line, "child %s", secondWord);
    //if( strncmp( secondWord, "end", MAX_BUF ) == 0) {
      //free( eachImage );
    }
  //}
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
printf( "Halp: %s %d \n", eachImage[ 10 ].name, imgNum);
} /* classifyLine */

void print( struct aLine * theLines, struct image * eachImage ) {
 
  char currentImg, command; 
  int i, j, len; 
  char *Img; 

  sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
    printf("vvvv %s %d", eachImage[10].name, imgNum);
  for( i = 0; i < imgNum; i++ ) {
	printf("TT%dTT", imgNum);
    Img = eachImage[ i ].name;
    printf("Name: %s %s", Img, &currentImg);
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
//return;
} /* end print */

void drawImg ( struct aLine * theLines, struct image * eachImage, FILE*  executable ) {
  
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
//return;
} /* end drawImg */

//int biggerArray( struct image **eachImage) {
  //initSize = initSize *  2;
  //printf("xx%d", initSize);
  //printf("gg%d^^", sizeof(**eachImage));
  //printf("gg%d", *eachImage);
  //struct image **tmp;
  //*eachImage = realloc(*eachImage, initSize  * sizeof( struct image ) );
  //if( tmp == NULL){
    //printf("cool");
  //}
  //else {
    //eachImage = tmp;
  
  //printf("ll%dcc", sizeof(**eachImage));
  //return(*eachImage);
  //printf("gg%d", *eachImage)
  //return 0;
  //} 
