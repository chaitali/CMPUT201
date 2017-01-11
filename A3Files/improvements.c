/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * contains the improvements: fuel gauge, crashing 
 * lander and screen wrapping
 *
 */

#include <stdio.h>
#include "lander.h"

void fuelGauge(){

  /* draw the fuel bar outline */
  fprintf(executable,"drawSegment 605 20 625 20\n");
  fprintf(executable,"drawSegment 605 20 605 90\n");
  fprintf(executable,"drawSegment 625 20 625 90\n");
  fprintf(executable,"drawSegment 605 90 625 90\n");
  fflush(executable);

  /* fill the fuel bar */
  for( int i = 19; i < 90; i++) {
    fprintf(executable, "drawSegment 605 %d 625 %d\n", i, i);
  } /* end for */

  fflush(executable);

} /* fuelGauge */

void decreaseFuel(){

  fuelUp++;
  if( fuelUp < 90 ){
    fprintf(executable, "eraseSegment 605 %d 625 %d\n", fuelUp, fuelUp);
  }
} /* decreaseFuel */ 

void wrap_around(){

  /* if the lander moves too far left */
  if(lander.max_x <= 0){
    for( int i = 0; i < 4; i++){
      lander.x[i] += 640;
    } /* end for */
    thrust_x += 640;
  }
  /* if the lander moves too far right */
  else if( lander.min_x >= 640){
    for( int i = 0; i < 4; i++){
      lander.x[i] -= 640;
    } /* end for */
    thrust_x -= 640;
  }
} /* wrap around */

void crashShip(){

   eraseShip(lander.x, lander.y, executable);
   for( int i = 0; i < 4; i++) {
     fprintf(executable, "drawSegment %ld %ld %ld %ld\n",
	     lround(lander.x[i] - 10), lround(lander.y[i] + 5),
             lround(lander.x[i+1] + 4), lround(lander.y[i+1] + 13));
             //   (int)shipPoints[i].x + 10, (int)shipPoints[i].y - 3,
              //  (int)shipPoints[i+1].x - 7, (int)shipPoints[i+1].y - 12);
  } /* end for */
}
