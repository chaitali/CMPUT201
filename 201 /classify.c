/* ONE Card number:    1404270
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

void classifyLine( struct aLine * theLines, struct image * eachImage, FILE* executable, FILE* fp ){

  char firstWord[ MAX_BUF ], child[ MAX_BUF ], imgName[ MAX_NAME ];
  char command, currentImg, *Img, secondWord[ MAX_BUF ];
  double xCoor, yCoor, xCoor1, yCoor1, angle;
  double move_x, move_y;
  int rval, i, j, valid, ifExist, pause;
  static int place = 0; 
  static int saveNum = 0; 
  static int inImage =  0; /* 0/1 for out/in image defintion */
  static int coorNum = INIT_COOR; 
  ifExist = 0;


  /* Initialize arrays imgName, child and firstWord */
  memset( child, 0, MAX_BUF );
  memset( firstWord, 0, MAX_BUF );
  memset( secondWord, 0, MAX_BUF );
  memset( imgName, 0, MAX_NAME );

  /* Scan in the first word */
  rval = sscanf( theLines[ currentLine ].line, "%s", firstWord );
  /* if line is empty */
  if( rval != 1 ){
    return;
  }

  /* check for comments */
  if( strncmp( firstWord, "#", MAX_BUF ) == 0 ){
    printf( "%s", theLines[ currentLine ].line );
    return;
  }

  /* Add each new image to an array */
  if( strncmp( firstWord, "Image", MAX_BUF ) == 0 ){
    sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
            &command, imgName, &xCoor, &yCoor);
    inImage = 1; /* indicates within image def now */
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

    /* ensures extra memory isn't allocated for redefined image */
    if( saveNum == 0) {
    eachImage[ imgNum ].x = calloc(coorNum, sizeof(double));
    eachImage[ imgNum ].y = calloc(coorNum, sizeof(double));
    }
    eachImage[ imgNum ].x[ place ] = xCoor;
    eachImage[ imgNum ].y[ place ] = yCoor;
    place++;
    return;
  } /* end image */

  /* Add other coordinates to the array */
  if( strncmp( firstWord, "lineTo", MAX_BUF ) == 0 ){
    valid =  sscanf( theLines[ currentLine ].line, "%s %lf %lf", &command, &xCoor1, &yCoor1);
    if( (valid != 3) || (inImage != 1)) {
      cleanup( executable, eachImage, fp, theLines );
    }
    /* if x, y coor filled, realloc more memory */
    if( coorNum == place ) {
      coorNum = coorNum * 2;
      eachImage[imgNum].x = realloc(eachImage[imgNum].x, coorNum  * sizeof(double));
      eachImage[imgNum].y = realloc(eachImage[imgNum].y, coorNum * sizeof(double));
    }
    xCoor = xCoor + xCoor1;
    yCoor = yCoor + yCoor1;
    eachImage[ imgNum ].x[ place ] = xCoor;
    eachImage[ imgNum ].y[ place ] = yCoor;
    place++;
    return;
  } /* lineTo */

  /* Reset coordinate place to 0, move to next image */
  if( strncmp( firstWord, "End", MAX_BUF ) == 0 ){
    sscanf( theLines[ currentLine ].line, "%s Image  %s", &command, imgName);
    inImage = 0;
    if( strncmp( eachImage[ imgNum ].name, imgName, MAX_NAME) == 0) {
      eachImage[ imgNum ].size = place;
      if( imgNum < saveNum ) {
        imgNum = saveNum;
        saveNum = 0;
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
  } /* End */

  /* Print the coordinates of select image */
  if( strncmp( firstWord, "print", MAX_BUF ) == 0 ) {
    valid = sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
    if( (valid != 2) || (inImage == 1) ){
      cleanup( executable, eachImage, fp, theLines );
    }
    ifExist = print( theLines, eachImage );
    if( ifExist == 1){
      cleanup( executable, eachImage, fp, theLines );
    }
    return;
  } /* end print */

  /* Draw image in SketchPad */
  if( strncmp( firstWord, "draw", MAX_BUF ) == 0 ) {
    valid = sscanf( theLines[ currentLine ].line, "%s %s", &command, &currentImg );
    if( (valid != 2) || (inImage == 1) ){
      cleanup( executable, eachImage, fp, theLines );
    }
    ifExist = drawImg( theLines, eachImage, executable );
    if( ifExist == 1){
      cleanup( executable, eachImage, fp, theLines );
    }
    return;
  } /* end draw */

  /* send child commands to SketchPad */
  if ( strncmp( firstWord, "child", MAX_BUF ) == 0 ) {
    if( inImage == 1) {
       cleanup( executable, eachImage, fp, theLines );
    }
    strncpy(child, theLines[ currentLine ].line  + 5,  MAX_BUF );
    fprintf( executable, "%s", child );
    sscanf( theLines[ currentLine ].line, "%s %s %d", &command, secondWord, &pause );
    if( strncmp( secondWord, "end", MAX_BUF ) == 0) {
      for( j = 0; j < imgNum; j++){
        free( eachImage[ j ].x);
        free( eachImage[ j ].y);
      }
      free( eachImage );
      free( theLines );
   }
   return;
  } /* end child */


  /* translate (x,y) coordinates by shifts */
  if ( strncmp( firstWord, "translate", MAX_BUF ) == 0 ) {
    valid = sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
    &command, &currentImg, &move_x, &move_y);
    if( (valid != 4)|| (inImage == 1)){
      cleanup( executable, eachImage, fp, theLines );
    }
    ifExist = translate( theLines, eachImage );
    if( ifExist == 1){
      cleanup( executable, eachImage, fp, theLines );
    }
    return;
  } /* end translate */

  if( strncmp( firstWord, "rotate", MAX_BUF ) == 0 ){
    valid = sscanf( theLines[ currentLine ].line, "%s %s %lf", &command, &currentImg, &angle);
    if( (valid != 3) || (inImage == 1)){
      cleanup( executable, eachImage, fp, theLines );
    }
    ifExist = rotate( theLines, eachImage );
    if( ifExist == 1){
      cleanup( executable, eachImage, fp, theLines );
    } 
    return;
  } /* end rotate */
 
  /* invalid first token */
  else {
    cleanup( executable, eachImage, fp, theLines );
  }
} /* classifyLine */
