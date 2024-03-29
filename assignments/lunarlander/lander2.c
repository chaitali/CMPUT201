

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
#define TIME 0.05

double xA, yA;
int intersection;
double seg1y, seg2y;
double old_yV, new_yV, old_xV, new_xV;
//old_yV = 0;
//new_yV = 0;
Lander lander;
void thrust();
FILE * executable;
void startSignals();
void handle_timeout(int signal);
void startLander();
void rotateRight();
sigset_t block_mask_g;
//int crashed;
double angle;
//angle = PI/2; 
void find_center();
int checkIntersection(double Ax, double Ay, double Bx, double By, 
                       double Cx, double Cy, double Dx, double Dy);
int landPoints;
drawLand points;
double shipThrust;
double gravity;
//double yA;

int main( int argc, char * argv[] ){
   
  int i;
  FILE *landscape_file;
  if( argc != 7) {
    printf("need flags -g gravity, -f landscape.txt, -t thrust\n");
    exit( EXIT_SUCCESS);
  }
  for( i=0; i< argc; i++ ){
    
    if( strncmp( argv[i], "-f", 5) == 0){
      landscape_file = fopen(argv[i+1], "r");
    }
    if( strncmp( argv[i], "-g", 5) == 0){
      gravity = atoi(argv[i+1]);
    }
    if( strncmp( argv[i], "-t", 5) == 0){
      shipThrust = atoi(argv[i+1]);
    }
  }
  
  //drawLand points;
  //long pt_x[25];
  memset(points.pt_x, 0, 25);
  //long pt_y[25];
  memset(points.pt_y, 0, 25);
  start_curses();
  landscape( landscape_file, points.pt_x, points.pt_y, points.size );
//  startSignals();
  executable = popen(exec_name, "w"); /* open pipe to sketchpad */
////////////////
//  fprintf( executable, "drawSegment -5 -10 5 -10\n");
  //fprintf( executable, "drawSegment -5 -10 -10 10\n");
//  fprintf( executable, "drawSegment -10 10 10 10\n");
 // fprintf( executable, "drawSegment 10 10 5 -10\n");
 ///////////////
 // fflush(executable);
 // fprintf( executable, "drawSegment 300 10 310 10\n");
 // fprintf( executable, "drawSegment 300 10 295 30\n");
 // fprintf( executable, "drawSegment 310 10 315 30\n");
  //fprintf( executable, "drawSegment 295 30 315 30\n");
  //fprintf( executable, "pause 10\n");
  //fprintf( executable, "end\n");
 // fflush(executable);
//  draw_landscape( executable, points.pt_x, points.pt_y, points.size);
  startLander();
//  drawShip(lander.x, lander.y, executable);
  getts(executable);
//  pclose(executable);
 // end_curses();


return 0;

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
  angle = (PI/2);
  old_yV = 0;  
  new_yV = 0;
  old_xV = 0;
  new_xV = 0;
  xA = 0;
  yA = 0;

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

  /* start timer */
  startSignals();

  /* get keyboard input */
  c = getch(); 

  while(1){

    if(c != ERR){

      if(c == KEY_LEFT){
        /* BLOCK THE SIGNAL */
        sigset_t old_mask;
        if(sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
          exit(EXIT_FAILURE);
        }

        printw("left key pressed");  //TAKE OTU
	find_center();
	eraseShip( lander.x, lander.y, executable);
        rotateLeft();
  	drawShip( lander.x, lander.y, executable);
        find_center();
        // UNBLOCK SIGNAL
        if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
          exit(EXIT_FAILURE);
        }
      }
      else if (c == KEY_RIGHT) {
        printw("right key pressed"); //OUT
	find_center();
        eraseShip( lander.x, lander.y, executable);
	rotateRight();
	drawShip( lander.x, lander.y, executable);
        find_center();
      } 
      else if (c == ' '){
        printw("space pressed");
	find_center();
        eraseShip( lander.x, lander.y, executable);
        thrust();
	drawShip( lander.x, lander.y, executable);
	find_center();
      }
      else if (c == 'q'){
        break;
      }

      refresh();

    }

    /* get next character */
    c = getch();

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
  fflush( executable );
}

void drawShip( double x[], double y[], FILE * executable ) {

  //sigset_t old_mask;
  //if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    //exit(EXIT_FAILURE);

  fprintf( executable, "drawSegment %ld %ld %ld %ld\n",lround(x[0]),lround( y[0]), lround(x[1]), lround(y[1]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n", lround(x[1]),lround( y[1]),lround( x[3]), lround(y[3]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n",lround( x[2]),lround( y[2]),lround( x[3]),lround( y[3]) );
  fprintf( executable, "drawSegment %ld %ld %ld %ld\n", lround(x[0]),lround( y[0]),lround( x[2]),lround( y[2]) );
  fflush(executable);
  //if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    //exit(EXIT_FAILURE);

}

void reset( double x[], double y[] ) {


}


void landscape( FILE* fp, long pt_x[], long pt_y[], int *size){
 
  long pt1, pt2;
  int i;
  i = 0;
  while ( fscanf(fp, "%ld %ld", &pt1, &pt2) == 2){
     pt_x[i] = pt1;
     pt_y[i] = pt2;
     i++; 
  }
  fclose(fp);
  size[0] = i;
  landPoints = i;
  //printf("%d", *size);
}

void draw_landscape( FILE *executable, long pt_x[], long pt_y[], int *size ){
  int i;
  fprintf(executable, "setColor 154 152 150\n");
  //printf("%d", size[0]);
  for( i=0; i < ( size[0] - 1); i++){
    fprintf( executable, "drawSegment %ld %ld %ld %ld\n",
             lround(pt_x[i]),lround( pt_y[i]),
	     lround(pt_x[i+1]),lround( pt_y[i +1]));
    printf("%ld", pt_x[i]);

  }
  fflush(executable);
  fprintf(executable, "setColor 0 0 0\n");
  fflush(executable);
}
void rotateLeft(){
 

  //eraseShip( lander.x, lander.y, executable); 
  angle -= A_R;
  double rotateX, rotateY;
  for( int i = 0; i < 4; i++) {
   // moveX[i] = lander.x[i] - lander.center_x;
   // moveY[i] = lander.y[i] - lander.center_y;
   // rotateX = moveX[i];
    //rotateY = moveY[i];
   /// moveX[i] = rotateX * cos(-A_R) - rotateY * sin(-A_R);
    //moveY[i] = rotateX * sin(-A_R) + rotateY * cos(-A_R);
    //lander.x[i] = moveX[i] + lander.center_x;
    //lander.y[i] = moveY[i] + lander.center_y;
     rotateX = lander.x[i] - lander.center_x;
    rotateY = lander.y[i] - lander.center_y;
    //rotateX = moveX[i];
    //rotateY = moveY[i];
    lander.x[i] = rotateX * cos(-A_R) - rotateY * sin(-A_R);
    lander.y[i] = rotateX * sin(-A_R) + rotateY * cos(-A_R);
    lander.x[i] += lander.center_x;
    lander.y[i] += lander.center_y;  
 

   }
  //drawShip( lander.x, lander.y, executable); 
  //find_center();
}

void rotateRight(){
  
  //find_center();
  //eraseShip( lander.x, lander.y, executable);  
  /* increment the ship angle by 10 degrees */
  angle += A_R;
  double rotateX, rotateY;
  for(int i = 0; i < 4; i++) {
    rotateX = lander.x[i] - lander.center_x;
    rotateY = lander.y[i] - lander.center_y;
    lander.x[i] = rotateX * cos(A_R) - rotateY * sin(A_R);
    lander.y[i] = rotateX * sin(A_R) + rotateY * cos(A_R);
    lander.x[i] += lander.center_x;
    lander.y[i] += lander.center_y;
  }
  //drawShip( lander.x, lander.y, executable);
  //find_center();
}


void find_center(){
  
  for( int i = 0; i < 4; i++){
    
    if( lander.x[i] < lander.min_x ){
      lander.min_x = lander.x[i];
    }
    else if( lander.x[i] > lander.max_x ){
      lander.max_x = lander.x[i];
    }
    if( lander.y[i] < lander.min_y ){
      lander.min_y = lander.y[i];
    }
    else if( lander.y[i] > lander.max_y ){
      lander.max_y = lander.y[i];
    }
  }
  lander.center_x = ( lander.max_x + lander.min_x )/2;
  lander.center_y = ( lander.max_y + lander.min_y )/2;
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
    find_center();
    eraseShip(lander.x, lander.y, executable);
    fall(lander.x, lander.y, executable);
    //find_center();
    drawShip(lander.x, lander.y, executable);  
    find_center();

//    yA = gravity;
  //  xA = 0;


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
            if ((angle == (PI/2)) && (new_yV < 20) && (seg1y == seg2y)) {
                move(6, 10);
                printw("LANDED!!!");
	}
           else {
                move(6, 10);
                printw("CRASHED!!!");
	    }

//	crashed = 0;
  timer.it_interval.tv_usec = 0;
            timer.it_value.tv_usec = 0;
  }

  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
      exit(EXIT_FAILURE);
 }
}

void fall( double x[], double y[], FILE *executable){

  for( int i = 0; i < 4; i++) {
    lander.y[i] = lander.y[i] + ( old_yV * TIME ) + ( 1/2 * gravity * TIME * TIME );
    //lander.x[i] = lander.x[i] + ( old_xV * TIME ) + ( 1/2 * gravity * TIME * TIME );
    //printf("%f", lander.y[i]); 
  //  new_velocity = old_velocity + ( gravity * t )
  }
  new_yV = old_yV + gravity * TIME;
  old_yV = new_yV;
} 

void thrust(){

  //sigset_t old_mask;
  //if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
   // exit(EXIT_FAILURE);
   //}
   
  // double xA, yA;
  //static double yA;
  //static double old_xV = 0;
  //static double new_xV = 0;
  //static double old_yV = 0;
  //static double old_xV = 0;
 //int i;
  //gravity = 0.5;
  //double t;//thrust;
  //thrust = -10;
  //ravity = 0.5
  //t = 0.05;
  xA = shipThrust * cos(angle);
  yA = gravity + shipThrust * sin(angle);
  //double new_y, y_velocity, t, new_velocity, x_velocity;
  for(int i = 0; i < 4; i++) {
    lander.y[i] = lander.y[i] + ( old_yV * TIME ) + ( 1/2 * yA * TIME * TIME );
    lander.x[i] = lander.x[i] + ( old_xV * TIME ) + ( 1/2 * xA * TIME * TIME );
  }

  new_xV = old_xV + xA * TIME;
  new_yV = old_yV + yA * TIME;
  old_xV = new_xV;
  old_yV = new_yV;
  //gravity = yA;
 // if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
   // exit(EXIT_FAILURE);
 // }


}

//double seg1y, seg2y;

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





/* u  need to make one fucntion for erasing the lander, one for 
drawing it, one for rotation left, one for rotation right,  */

//corresponding left arrow or right arrow key is clicked, rotate the ship by 10 //degrees each key click
