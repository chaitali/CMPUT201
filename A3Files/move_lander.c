/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * all functions to move, draw and erase lander
 *
 */


#include <curses.h>
#include <stdio.h>
#include <string.h> /* For memset() */
#include <math.h>
#include <stdlib.h> /* For exit() */
#include "use_curses.h"
#include "lander.h"
#include <signal.h>
#include <sys/time.h>





/*********** THRUST *******************/

void eraseThrust(){

  fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[3]), lround(lander.y[3]),
          lround(thrust_x), lround(thrust_y));

  fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[2]), lround(lander.y[2]),
          lround(thrust_x), lround(thrust_y));

  fflush(executable);
}


void drawShip( double x[], double y[], FILE * executable ) {
  
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n",lround(x[0]),lround( y[0]), lround(x[1]), lround(y[1]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n", lround(x[1]),lround( y[1]),lround( x[3]), lround(y[3]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n",lround( x[2]),lround( y[2]),lround( x[3]),lround( y[3]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n", lround(x[0]),lround( y[0]),lround( x[2]),lround( y[2]) );
  fflush(executable);
} /* drawShip */



void eraseShip( double x[], double y[], FILE *executable) {

  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n",lround(x[0]),lround( y[0]), lround(x[1]), lround(y[1]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n", lround(x[1]),lround( y[1]),lround( x[3]), lround(y[3]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n",lround( x[2]),lround( y[2]),lround( x[3]),lround( y[3]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n", lround(x[0]),lround( y[0]),lround( x[2]),lround( y[2]) );

  /* erase thrust */
   fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[3]), lround(lander.y[3]),
          lround(thrust_x), lround(thrust_y));

   fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[2]), lround(lander.y[2]),
          lround(thrust_x), lround(thrust_y));


   fflush(executable);
} /* erase ship */


void rotateLeft(){

  find_center();
  eraseShip( lander.x, lander.y, executable);
  landerAngle -= DEG;
  radAngle = landerAngle * PI /180.0;

  double rotateX, rotateY;

  for( int i = 0; i < 4; i++) {
    rotateX = lander.x[i] - lander.center_x;
    rotateY = lander.y[i] - lander.center_y;

    lander.x[i] = rotateX * cos(rotLeft) - rotateY * sin(rotLeft);
    lander.y[i] = rotateX * sin(rotLeft) + rotateY * cos(rotLeft);

    lander.x[i] += lander.center_x;
    lander.y[i] += lander.center_y;
  }

  /* rotate thrust */
  rotateX = thrust_x - lander.center_x;
  rotateY = thrust_y - lander.center_y;
  thrust_x = rotateX * cos(rotLeft) - rotateY * sin(rotLeft);
  thrust_y = rotateX * sin(rotLeft) + rotateY * cos(rotLeft);
  thrust_x += lander.center_x;
  thrust_y += lander.center_y;

  drawShip( lander.x, lander.y, executable);

} /* rotateLeft */


void rotateRight(){

  find_center();
  eraseShip( lander.x, lander.y, executable);
  //find_center();
  /* increment the ship angle by 10 degrees */
  landerAngle += DEG;
  radAngle = landerAngle * PI /180.0;
  double rotateX, rotateY;
  for(int i = 0; i < 4; i++) {
    rotateX = lander.x[i] - lander.center_x;
    rotateY = lander.y[i] - lander.center_y;

    lander.x[i] = rotateX * cos(rotRight) - rotateY * sin(rotRight);
    lander.y[i] = rotateX * sin(rotRight) + rotateY * cos(rotRight);

    lander.x[i] += lander.center_x;
    lander.y[i] += lander.center_y;
  }

  /* rotate thrust */
  rotateX = thrust_x - lander.center_x;
  rotateY = thrust_y - lander.center_y;
  thrust_x = rotateX * cos(rotRight) - rotateY * sin(rotRight);
  thrust_y = rotateX * sin(rotRight) + rotateY * cos(rotRight);
  thrust_x += lander.center_x;
  thrust_y += lander.center_y;

  drawShip( lander.x, lander.y, executable);

} /* rotateRight */

void fall(){

  old_yV = new_yV;
  old_xV = new_xV;

  for( int i = 0; i < 4; i++) {
    lander.y[i] = lander.y[i] + ( old_yV * TIME ) + ( 1/2 * yA * TIME * TIME );
    lander.x[i] = lander.x[i] + ( old_xV * TIME ) + ( 1/2 * xA * TIME * TIME );

  }

  thrust_y = thrust_y + ( old_yV * TIME ) + ( 1/2 * yA * TIME * TIME );
  thrust_x = thrust_x + ( old_xV * TIME ) + ( 1/2 * xA * TIME * TIME );

  new_yV = old_yV + yA * TIME;
  new_xV = old_xV + xA * TIME;

}

void thrust(){

  xA = shipThrust * cos(radAngle);
  yA = gravity + shipThrust * sin(radAngle);


//draw thrust
   fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
          lround(lander.x[3]), lround(lander.y[3]),
          lround(thrust_x), lround(thrust_y));

   fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
          lround(lander.x[2]), lround(lander.y[2]),
          lround(thrust_x), lround(thrust_y));


   fflush(executable);



}
