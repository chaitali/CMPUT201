/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 */

#define _POSIX_C_SOURCE 200112L

#include <curses.h>
#include <stdio.h>
#include <string.h> /* For memset() */
#include <math.h>
#include <stdlib.h> /* For exit() */
#include "use_curses.h"
#include "lander.h"
#include <signal.h>
#include <sys/time.h>

sigset_t block_mask_g;
const char* const exec_name = "java -jar Sketchpad.jar ";

int main( int argc, char * argv[] ){
   
  FILE *landscape_file;
  /* check arguments */
  if( argc < 7) {
    printf("need flags -g gravity, -f landscape.txt, -t thrust\n");
    exit( EXIT_FAILURE );
  }
  
  for( int i = 0; i < argc; i++ ){
    
    if( strncmp( argv[i], "-f", 2) == 0){
      landscape_file = fopen(argv[i+1], "r");
    }
    if( strncmp( argv[i], "-g", 2) == 0){
      gravity = atoi(argv[i+1]);
    }
    if( strncmp( argv[i], "-t", 2) == 0){
      shipThrust = atoi(argv[i+1]);
    }
    if( strncmp( argv[i], "-i", 2) == 0){
      improve = 1;
    }
  }

  /* check inputs */
  if( shipThrust > 0 || shipThrust < -20){
    printf("thrust > 0, < -20 not allowed\n");
    exit( EXIT_FAILURE );
  } 
  if( gravity < 0 || gravity > 20){
    printf("gravity < 0, > 20 not allowed\n");
    exit( EXIT_FAILURE );
  }
  if( landscape_file == NULL ){
    printf("could not open landscape\n");
    exit( EXIT_FAILURE );
  }

  /* initialize points in landscape */
  memset(points.pt_x, 0, 25);
  memset(points.pt_y, 0, 25);

  start_curses();

  /* put points from landscape file into arrays */  
  landscape( landscape_file, points.pt_x, points.pt_y, points.size );

  /* open pipe to SketchPad */
  executable = popen(exec_name, "w");

  /* initialize points of lander */
  startLander();
  getts(executable);
  return 0;
}



void eraseThrust(){

  fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[3]), lround(lander.y[3]),
          lround(thrust_x), lround(thrust_y));

   fprintf(executable, "eraseSegment %ld %ld %ld %ld\n",
          lround(lander.x[2]), lround(lander.y[2]),
          lround(thrust_x), lround(thrust_y));


   fflush(executable);

}


void startLander(){
 
  lander.max_y = 30;
  lander.max_x = 315;
  lander.min_x = 295;
  lander.min_y = 10;
  double initialX[4] = {300, 310, 295, 315};
  double initialY[4] = { 10, 10, 30, 30};
  memcpy(lander.x, initialX, sizeof(lander.x));
  memcpy(lander.y, initialY, sizeof(lander.y));
  lander.center_x = ( lander.max_x + lander.min_x )/2;
  lander.center_y = ( lander.max_y + lander.min_y )/2;
  landerAngle = START_ANGLE;
  radAngle = landerAngle * ( PI /180.0);
  rotRight = DEG * PI /180.0;
  rotLeft = 0 - rotRight;
  old_yV = 0;  
  new_yV = 0;
  old_xV = 0;
  new_xV = 0;
  xA = 0;
  yA = gravity;
  thrust_x = 305;
  thrust_y = 33;
  fuelUp = 20;
}


void getts(FILE *executable){
  

  /* print start message */
  move(5, 10);
  printw("Press any key to start.");
  move(6, 10);
  printw("(Then press arrow keys to rotate, ");  
  printw("space for thrust, 'q' to quit.)");  

  /* get the first keyboard input, and start game */
  int c = getch();

  /* game message after initial key is entered */
  erase();
  move(5,0);
  printw("left arrow key rotates counter-clockwise, ");
  printw("right clockwise, space for thrust, q to quit.");

  /* draw landscape and lander */
  draw_landscape( executable, points.pt_x, points.pt_y, points.size);
  drawShip(lander.x, lander.y, executable);
  if( improve == 1){
    fuelGauge();
  }

  /* start timer */
  startSignals();

  /* get keyboard input */
  c = getch(); 

  while(1){
	
    if(c != ERR){

      if(c == KEY_LEFT){

        printw("left key pressed");  //TAKE OTU
        rotateLeft();

      }

      else if (c == KEY_RIGHT) {
        printw("right key pressed"); //OUT
	rotateRight();
      } 
      else if (c == ' '){
        printw("space pressed");
        thrust();
      }
      else if (c == 'q'){
        break;
      }

      refresh();

    }

    /* get next character */
    //eraseThrust();
    c = getch();
    eraseThrust();
  }

  /* restore terminal to normal state */
  end_curses();

  /* send end command to Sketchpad */
  fprintf(executable, "end\n");

  /* close pipe to Sketchpad */
  pclose(executable);

  exit(EXIT_SUCCESS);
} /* end enterChar */

void eraseShip( double x[], double y[], FILE *executable) {

  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n",lround(x[0]),lround( y[0]), lround(x[1]), lround(y[1]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n", lround(x[1]),lround( y[1]),lround( x[3]), lround(y[3]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n",lround( x[2]),lround( y[2]),lround( x[3]),lround( y[3]) );
  fprintf( executable, "eraseSegment %ld %ld %ld %ld\n", lround(x[0]),lround( y[0]),lround( x[2]),lround( y[2]) );
//  fflush( executable );
  
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
}

void rotateLeft(){
 
  find_center();
  eraseShip( lander.x, lander.y, executable);
  //find_center();
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

}

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

}


void find_center(){
  
  /* calculate minimum and maximum x */
  lander.min_x = min(lander.x[0], min(lander.x[1], 
                min(lander.x[2], lander.x[3])));
    lander.max_x = max(lander.x[0], max(lander.x[1], 
                max(lander.x[2], lander.x[3])));


  lander.min_y = min(lander.y[0], min(lander.y[1],          
                min(lander.y[2], lander.y[3])));
    lander.max_y = max(lander.y[0], max(lander.y[1],        
                max(lander.y[2], lander.y[3])));
  lander.center_x = ( lander.max_x + lander.min_x )/ 2.0;
  lander.center_y = ( lander.max_y + lander.min_y )/ 2.0;

}

double min(double n1, double n2)
{
    double minimum;
    minimum = n1;
    if (n2 < n1)
        minimum = n2;
    return minimum;
}

double max(double n1, double n2)
{
    double maximum;
    maximum = n1;
    if (n2 > n1)
        maximum = n2;
    return maximum;
}


void startSignals() {

  sigemptyset(&block_mask_g);
  sigaddset(&block_mask_g, SIGALRM);
  struct sigaction handler;
  handler.sa_handler = handle_timeout;
  sigemptyset(&handler.sa_mask);
  handler.sa_flags = 0;
  

  if (sigaction(SIGALRM, &handler, NULL) == -1){
    exit(EXIT_FAILURE);
  }

  /* set up timer */
  
  struct itimerval timer;
  struct timeval time_delay;

  time_delay.tv_sec = 0;
  time_delay.tv_usec = 50000;

  timer.it_interval = time_delay;
  
  struct timeval start;
  start.tv_sec = 0;
  start.tv_usec = 10;
  
  timer.it_value = start;

  if (setitimer(ITIMER_REAL, &timer, NULL) == -1){
    exit(EXIT_FAILURE);
  }
}


void handle_timeout(int signal)
{

  double Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
  
  /* called because of SIGALRM */
  if (signal == SIGALRM){
  
    /* gets timer, puts it into timer (man 2 getitimer) */
    struct itimerval timer;
    if(getitimer(ITIMER_REAL, &timer) == -1){
      exit(EXIT_FAILURE);
    } 
    /* BLOCK THE SIGNAL */
    sigset_t old_mask;
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
        exit(EXIT_FAILURE);
    }
    
    if( improve == 1){
      wrap_around();
    }
    static int useFuel = 0;

    if( improve == 1){
      useFuel++;
      /* every 0.50 sec decrease the fuel */
      if( useFuel % 15 == 0){
        decreaseFuel();
      }
    }

    if (improve == 1) {
      if (fuelUp == 100) {
        yA = 400;
      }
    }
    eraseShip(lander.x, lander.y, executable);
    fall();
    find_center();
    drawShip(lander.x, lander.y, executable);  
    
    yA = gravity;
    xA = 0;


    for (int i = 0; i < 4 - 1; i++) {
      Ax = lander.x[i];
            Ay = lander.y[i];
            Bx = lander.x[i+1];
            By = lander.y[i+1];

            for (int j = 0; j < landPoints - 1; j++) {
                Cx = points.pt_x[j];
                Cy = points.pt_y[j];
                Dx = points.pt_x[j+1];
                Dy = points.pt_y[j+1];
                intersection = checkIntersection(Ax,Ay, Bx,By, Cx,Cy, Dx,Dy);
                if (intersection) {
                    break;
                }
            }
            if (intersection) {
                break;
            }
        }

if (!intersection) {
            for (int k = 0; k < landPoints - 1; k++) {
                Ax = lander.x[0];
                Ay = lander.y[0];
                Bx = lander.x[3];
                By = lander.y[3];
                Cx = points.pt_x[k];
                Cy = points.pt_y[k];
                Dx = points.pt_x[k+1];
                Dy = points.pt_y[k+1];

                intersection = checkIntersection(Ax,Ay, Bx,By, Cx,Cy, Dx,Dy);
                if (intersection) {
                    break;
                }
            }
        }



  // UNBLOCK SIGNAL
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
    exit(EXIT_FAILURE);
}
  if (intersection) {
		
            /* if the ship is completely vertical (90 degrees), the velocity
               is less than 20, and the landscape where the intersection 
               ocurred is completely horizontal, then we have a landing */
            if ((landerAngle == (90)) && (new_yV < 20) && (seg1y == seg2y)) {
                move(6, 10);
                printw("LANDED!!!");
	}
           else {
                move(6, 10);
                printw("CRASHED!!!");
	    }

  timer.it_interval.tv_usec = 0;
            timer.it_value.tv_usec = 0;
  }

  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
      exit(EXIT_FAILURE);
 }
}

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


int checkIntersection(double Ax, double Ay, double Bx, double By, 
                       double Cx, double Cy, double Dx, double Dy){

 double distAB, theCos, theSin;

  seg1y = Cy; 
    seg2y = Dy;

  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) {
      return 0;
  }
  if (((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)) ||  
      ((Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy))){
      return 0; 
  }
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  /* calculate distance between A and B */
    distAB=sqrt(Bx*Bx+By*By);
  double newX, ABpos;
/* rotate system so point B is on the positive x-axis */
    theCos=Bx/distAB;
    theSin=By/distAB;
    newX=Cx*theCos+Cy*theSin;
    Cy  =Cy*theCos-Cx*theSin; Cx=newX;
    newX=Dx*theCos+Dy*theSin;
    Dy  =Dy*theCos-Dx*theSin; Dx=newX;
  
if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)) {
        return 0;
    }

/* calculate position of intersection along segment AB */
    ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

    /* fail if segment CD crosses AB outside line AB */
    if (ABpos<0. || ABpos>distAB) {
        return 0;
    }
    
    /* success: there is intersection */
    return 1;
}

