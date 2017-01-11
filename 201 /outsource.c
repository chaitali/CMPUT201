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


int print( struct aLine * theLines, struct image * eachImage ) {

  char currentImg[ MAX_NAME ], command;
  int i, j, len;
  char Img[ MAX_NAME ];

  sscanf( theLines[ currentLine ].line, "%s %s", &command, currentImg );
  for( i = 0; i < imgNum; i++ ) {
    memcpy( Img, eachImage[ i ].name, MAX_NAME);
    if( strncmp( Img, currentImg, MAX_NAME ) == 0 ) {
      len = eachImage[ i ].size;
      if( len == 1 ){          /* if there is no lineTo commands */
        printf( "Print Image %s =\n", Img );
        printf( "End Image %s\n", Img );
        break;
      }
      printf( "Print Image %s =\n", Img );
      for( j = 0; j < len ; j++ ) {
        printf( "%g %g\n", eachImage[ i ].x[ j ] , eachImage[ i ].y[ j ] ) ;
      } /* end inner for*/
      printf( "End Image %s\n", Img );
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

int rotate( struct aLine * theLines, struct image * eachImage ){
  
  double angle_radians, angle_degrees, x, y;
  char currentImg[ MAX_NAME ], command;
  int i, j, len;
  char Img[ MAX_NAME ];

  sscanf( theLines[ currentLine ].line, "%s %s %lf", &command, currentImg, &angle_degrees);
  angle_radians = angle_degrees * PI / 180.0;
    for( i = 0; i < imgNum; i++ ){
      memcpy( Img, eachImage[ i ].name, MAX_NAME);  
      if(  strncmp( Img, currentImg, MAX_NAME ) == 0 ) {
        len = eachImage[ i ].size;
        for(j  = 0; j < len ; j++ ) {
          x = eachImage[ i ].x[ j ];
          y = eachImage[ i ].y[ j ];
          eachImage[ i ].x[ j ] = x * cos(angle_radians) - y * sin(angle_radians);
          eachImage[ i ].y[ j ] = x * sin( angle_radians) + y * cos(angle_radians);
        } /* end inner for */
        break;
      }
      if( ( strncmp( Img, currentImg, MAX_NAME ) != 0 ) && ( i == (imgNum - 1))){
        return 1;
      }
    } /* end outer for */
  return 0;
} /* rotate */

int translate( struct aLine * theLines, struct image * eachImage ){

  char currentImg[ MAX_NAME ], command;
  int i, j, len;
  char Img[ MAX_NAME ];
  double move_x, move_y;

  sscanf( theLines[ currentLine ].line, "%s %s %lf %lf",
          &command, currentImg, &move_x, &move_y);

  for( i = 0; i < imgNum; i++ ) {
    memcpy( Img, eachImage[ i ].name, MAX_NAME);
    if(  strncmp( Img, currentImg, MAX_NAME ) == 0 ) {
      len = eachImage[ i ].size;
      for(j  = 0; j < len ; j++ ) {
        eachImage[ i ].x[ j ] = eachImage[ i ].x[ j ] + move_x;
        eachImage[ i ].y[ j ] = eachImage[ i ].y[ j ] + move_y;
      } /* end inner for */
    break;
    }
    if( ( strncmp( Img, currentImg, MAX_NAME ) != 0 ) && ( (i + 1)  == (imgNum))){
      return 1;
    }
  } /* end outer for */
  return 0;
} /* translate */


