/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * Takes in a .txt file as input, and draws landscape
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h> /* For exit() */
#include "lander.h"
//#include "use_curses.h"




void landscape( FILE* fp, long pt_x[], long pt_y[], int *size){

  long pt1, pt2;
  int i;

  i = 0;
  /* scan points into array */
  while ( fscanf(fp, "%ld %ld", &pt1, &pt2) == 2){
     pt_x[i] = pt1;
     pt_y[i] = pt2;
     i++;
  }
  /* close landscape file */
  fclose(fp);
  /* number of points */
  size[0] = i;
  landPoints = i;

} /* landscape */

void draw_landscape( FILE *executable, long pt_x[], long pt_y[], int *size ){
 
  /* set line colour to gray for landscape */
  fprintf(executable, "setColor 154 152 150\n");
  /* draw the landscape */
  for(int i=0; i < ( size[0] - 1); i++){
    fprintf( executable, "drawSegment %ld %ld %ld %ld\n",
             lround(pt_x[i]),lround( pt_y[i]),
             lround(pt_x[i+1]),lround( pt_y[i +1]));

  }
  /* set line colour back to black */
  fprintf(executable, "setColor 0 0 0\n");
  fflush(executable);

} /* draw_landscape */
