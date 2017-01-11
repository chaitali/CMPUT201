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
#include "memwatch.h"

int initSize; 
int bufferSize;
int coorNum;
int main( int argc, char * argv[] ){
  int i, j;
  initSize = 10;
  coorNum = 10; 
  bufferSize = 256;
  FILE *fp, *executable; 
  //struct aLine theLines[ 8000 ];//MAX_LINES ]; /* An array of structs */
  struct aLine *theLines;
  theLines = calloc( bufferSize, sizeof(struct aLine));
  struct image *eachImage;
  eachImage = calloc( initSize, sizeof(struct image));
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
    if( fp == NULL || argc > 2){
      fprintf(stderr, "draw2: %d, error.\n", 0 );
      free( eachImage );
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

void bufferFile( FILE *fp, struct aLine *theLines, struct image *eachImage, FILE *executable ){
  
  char * rbuf;

  /* Line-oriented */
  while( 1 ) {
    /* check if the end of file has been reached */
    if ( feof ( fp ) ) {
      break;
    }
    rbuf = fgets( theLines[ currentLine ].line, MAX_NAME, fp );
    if( rbuf == NULL ){
//      printf("cool");
      break;
    }
    if( bufferSize == ( currentLine + 1 ) ){
       bufferSize = bufferSize * 2;
       theLines = realloc( theLines, bufferSize  * sizeof( struct aLine ) );
    }
    if( ( imgNum ) == (initSize ) ) {
      initSize = initSize * 2;
      eachImage = realloc( eachImage, initSize  * sizeof( struct image ) );
    }
    classifyLine( theLines, eachImage, executable, fp );
    //printf( "n: %s %d \n", eachImage[ 10 ].name, imgNum);
    fflush( stdout );
    currentLine++;
  } /* while */
 

} /* bufferFile */


void classifyLine( struct aLine * theLines, struct image * eachImage, FILE* executable, FILE* fp ){
 
  char firstWord[ MAX_BUF ], child[ MAX_BUF ], imgName[ MAX_NAME ]; 
  char command, currentImg, *Img, secondWord;   
  double xCoor, yCoor, xCoor1, yCoor1;
  double move_x, move_y; 
  int rval, i, j, len, valid, ifExist;
  static int inImage =  0; 
  //printf( "hello: %s %d \n", eachImage[ 10 ].name, imgNum);  
  /* Initialize arrays imgName, child and firstWord */
  //memset( imgName, 0, sizeof( imgName ));
  memset( child, 0, sizeof( child ));
  memset( firstWord, 0, MAX_BUF );
  ifExist = 0;
  /* Check for comments */
  //if( theLines[ currentLine ].line[ 0 ] == '#' ){
    //printf( "%s", theLines[ currentLine ].line );
    //return;
  //}
  
  /* Scan in the first word */
  rval = sscanf( theLines[ currentLine ].line, "%s", firstWord );
  if( rval != 1 ) {
 //  printf( "Error in sscanf\n" );
  return;
  }

   /* check for comments */
  if( strncmp( firstWord, "#", MAX_BUF ) == 0 ){
    printf( "%s", theLines[ currentLine ].line );
    return;
  }
  int b;
 
  /* Add each new image to an array */
  if( strncmp( firstWord, "Image", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
     &command, imgName, &xCoor, &yCoor) );  
    inImage = 1; 
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
    eachImage[imgNum].x = calloc(coorNum, sizeof(double));
    eachImage[imgNum].y = calloc(coorNum, sizeof(double));
    eachImage[ imgNum ].x[ place ] = xCoor;
    eachImage[ imgNum ].y[ place ] = yCoor;
    place++;

    return;
  }

  /* Add other coordinates to the array */ 
  if( strncmp( firstWord, "lineTo", MAX_BUF ) == 0 ){
    ( valid =  sscanf( theLines[ currentLine ].line, "%s %lf %lf", &command, &xCoor1, &yCoor1) );
     if( valid != 3) {
       cleanup( executable, eachImage, fp, theLines );
     } 
     if( inImage != 1) {
       cleanup( executable, eachImage, fp, theLines );
     }
     if( coorNum == (place) ) {
       
     coorNum = coorNum * 2;
     eachImage[imgNum].x = realloc(eachImage[imgNum].x, coorNum  * sizeof(double));
     eachImage[imgNum].y = realloc(eachImage[imgNum].y, coorNum * sizeof(double));
     }
     xCoor = xCoor + xCoor1;
     yCoor = yCoor + yCoor1;
     eachImage[ imgNum ].x[ place ] = xCoor;
     eachImage[ imgNum ].y[ place ] = yCoor;
     place++;
     //printf("%d", eachImage[imgNum].x); 
  return;
  }
  char imageName[ MAX_NAME ];
  
  /* Reset coordinate place to 0, move to next image */
  if( strncmp( firstWord, "End", MAX_BUF ) == 0 ){
    ( sscanf( theLines[ currentLine ].line, "%s Image  %s", &command, imageName) );
    inImage = 0; 
    if( strncmp( eachImage[ imgNum ].name, imageName, MAX_NAME) == 0) {
    eachImage[ imgNum ].size = place;
    if( imgNum < saveNum ) {
      imgNum = saveNum;
    }
    else {
      imgNum++;
    }
    place = 0;
    return; 
  }
   else {
     cleanup( executable, eachImage, fp, theLines );
   }
  return;
  }

  /* Print the coordinates of select image */  
  if( strncmp( firstWord, "print", MAX_BUF ) == 0 ) {
    
    valid = sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
  

    if( valid != 2 ){
      cleanup( executable, eachImage, fp, theLines );  
    }


  if( inImage == 1) {
   cleanup( executable, eachImage, fp, theLines );
     }
     
    ifExist = print( theLines, eachImage );
  if( ifExist == 1){
     cleanup( executable, eachImage, fp, theLines );
     } 
    return;
  }

  /* Draw image in SketchPad */
  if( strncmp( firstWord, "draw", MAX_BUF ) == 0 ) {
    valid = sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
    if( valid != 2 ){
     cleanup( executable, eachImage, fp, theLines );  
   }  
    if( inImage == 1) {
       cleanup( executable, eachImage, fp, theLines );
     } 
    ifExist = drawImg( theLines, eachImage, executable ); 
    if( ifExist == 1){
     cleanup( executable, eachImage, fp, theLines );
     }

    return;
  }
 int pause;
  /* send child commands to SketchPad */
  if ( strncmp( firstWord, "child", MAX_BUF ) == 0 ) {
    if( inImage == 1) {
       cleanup( executable, eachImage, fp, theLines );
     }
    strncpy(child, theLines[ currentLine ].line  + 5,  MAX_BUF );    
    fprintf( executable, "%s", child );
    sscanf( theLines[ currentLine ].line, "%s %s %d", &command, &secondWord, &pause );
    if( strncmp( &secondWord, "end", MAX_BUF ) == 0) {
      for( j=0; j < imgNum; j++){
        free( eachImage[ j ].x);
        free( eachImage[ j ].y);
      }
      free( eachImage );  
      free( theLines ); 
   } 
   return;
   
  }
  /* translate (x,y) coordinates by shifts */ 
  if ( strncmp( firstWord, "translate", MAX_BUF ) == 0 ) {
    valid = sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
    &command, &currentImg, &move_x, &move_y);
    if( valid != 4){
      cleanup( executable, eachImage, fp, theLines );
    }
    if( inImage == 1) {
       cleanup( executable, eachImage, fp, theLines );
     }
    for( i = 0; i < imgNum; i++ ) {
      Img = eachImage[ i ].name;
      if(  strncmp( Img, &currentImg, MAX_NAME ) == 0 ) {
        len = eachImage[ i ].size;
        for(j  = 0; j < len ; j++ ) {
          eachImage[ i ].x[ j ] = eachImage[ i ].x[ j ] + move_x;
          eachImage[ i ].y[ j ] = eachImage[ i ].y[ j ] + move_y;
        } /* end inner for */
      break; 
      }
      if( ( strncmp( Img, &currentImg, MAX_NAME ) != 0 ) && ( (i + 1)  == (imgNum))){
        cleanup( executable, eachImage, fp, theLines );
      }
    } /* end outer for */
  return;
  } /* end translate */
  //int valid; 
  double angle_radians, angle_degrees, x, y;
  if( strncmp( firstWord, "rotate", MAX_BUF ) == 0 ){
    ( valid = sscanf( theLines[ currentLine ].line, "%s %s %lf", &command, &currentImg, &angle_degrees) );
    if( valid != 3 ){
      cleanup( executable, eachImage, fp, theLines );
    }
    if( inImage == 1) {
       cleanup( executable, eachImage, fp, theLines );
     }
    angle_radians = angle_degrees * PI / 180.0;
     for( i = 0; i < imgNum; i++ ){
      Img = eachImage[ i ].name;
      if(  strncmp( Img, &currentImg, MAX_NAME ) == 0 ) {
        len = eachImage[ i ].size;
        for(j  = 0; j < len ; j++ ) {
          x = eachImage[ i ].x[ j ];
          y = eachImage[ i ].y[ j ];
          eachImage[ i ].x[ j ] = x * cos(angle_radians) - y * sin(angle_radians);
          eachImage[ i ].y[ j ] = x * sin( angle_radians) + y * cos(angle_radians);
        } /* end inner for */
      break;
      }
      if( ( strncmp( Img, &currentImg, MAX_NAME ) != 0 ) && ( i == (imgNum - 1))){
        cleanup( executable, eachImage, fp, theLines );
      }
     }/* end outer for */
    return;
  }
//printf( "Halp: %s %d \n", eachImage[ 10 ].name, imgNum);
   else {
    // fprintf(stderr, "draw2: %d, error.", ( currentLine + 1) );
     cleanup( executable, eachImage, fp, theLines );
     //exit( EXIT_FAILURE);
   }



} /* classifyLine */

int print( struct aLine * theLines, struct image * eachImage ) {
 
  char currentImg[ MAX_NAME ], command; 
  int i, j, len; 
  char Img[ MAX_NAME ];
  len = 0;
  j = 0; 
  i = 0;  

  sscanf( theLines[ currentLine ].line, "%s %s", &command, currentImg );
    //printf("vvvv %s %d", eachImage[10].name, imgNum);
  for( i = 0; i < imgNum; i++ ) {
	//printf("TT%dTT", imgNum);
    //Img = eachImage[ i ].name;
    memcpy( Img, eachImage[ i ].name, MAX_NAME);
    //printf("Name: %s// %s\n", Img, currentImg);
    if( strncmp( Img, currentImg, MAX_NAME ) == 0 ) {
      len = eachImage[ i ].size;
      if( len == 1 ){          /* if there is no lineTo commands */
        printf( "Print Image %s = \n", Img );
        printf( "End Image %s \n", Img ); 
        break; 
      }
      //printf( "Print Image %s = \n", Img );
      for( j = 0; j < len ; j++ ) {
        printf( "%ld %ld\n", lround( eachImage[ i ].x[ j ] ),  
        lround( eachImage[ i ].y[ j ] ) );
      } /* end inner for*/
      printf( "End Image %s \n", Img );
      break;
    }
    if( ( strncmp( Img, currentImg, MAX_NAME ) != 0 ) && ( i == (imgNum - 1))){
      return 1;
    }
  } /* end outer for */
 return 0;
} /* end print */

int drawImg ( struct aLine * theLines, struct image * eachImage, FILE*  executable ) {
  
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
     break;
     }
    if( ( strncmp( Img, &currentImg, MAX_NAME ) != 0 ) && ( i == (imgNum - 1))){
      return 1;
    }

  } /* end outer for */
 return 0;
} /* end drawImg */

void cleanup( FILE*  executable, struct image * eachImage, FILE* fp, struct aLine * theLines  ) {
  int j;
  fprintf(stderr, "draw2: %d, error.\n", ( currentLine + 1) );
  for( j=0; j < imgNum; j++){
        free( eachImage[ j ].x);
        free( eachImage[ j ].y);
      }  
  free( eachImage );
  free( theLines );
  fprintf( executable, "end\n" );
  pclose( executable );
  fclose( fp );
  exit( EXIT_FAILURE );
}
