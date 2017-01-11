
#ifndef LANDER_H
#define LANDER_H

#define _POSIX_C_SOURCE 200112L
#define PI acos(-1.0)
#define DEG 10
#define START_ANGLE 90
#define TIME 0.05

/* Function Prototypes */

void landscape( FILE* landscape_file, long pt_x[], long pt_y[], int size[]);
void rotateLeft();
void getts(FILE *executable);
void eraseShip( double x[], double y[], FILE *executable);
void drawShip( double x[], double y[], FILE * executable);
void draw_landscape( FILE *executable, long pt_x[], long pt_y[], int size[] );
void fall();
void fuelGauge();
void wrap_around();
double min(double n1, double n2);
double max(double n1, double n2);
void decreaseFuel();
void rotateRight();
void thrust();
void startSignals();
void handle_timeout(int signal);
void startLander();
void find_center();
int checkIntersection(double Ax, double Ay, double Bx, double By,
                      double Cx, double Cy, double Dx, double Dy);
void eraseThrust();
void crashShip();


/* Non-C99 compliant function prototypes */
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

/* externally declared global variable */
extern const char* const exec_name;


/* struct for lander */
typedef struct{

  double x[4];
  double y[4];
  double max_y;
  double max_x;
  double min_x;
  double min_y;
  double center_x;
  double center_y;
}Lander;


/* struct for landscape points */
typedef struct{

  long pt_x[25];
  long pt_y[25];
  int size[1];
} drawLand;


/* global variables */
int intersection, landPoints, fuelUp, improve;
double xA, yA;
double line1, line2;
double old_yV, new_yV, old_xV, new_xV;
FILE * executable;
double thrust_y, thrust_x;
double rotLeft, rotRight;
double landerAngle, radAngle;
double shipThrust, gravity;

/* global structs */
drawLand points;
Lander lander;

#endif
