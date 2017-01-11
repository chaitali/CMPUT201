/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 */

#ifndef DRAW2_H
#define DRAW2_H

#define INIT_IMAGE 10 /* initial number of images */
#define INIT_COOR 10 /* number of initial x,y coordinates */
#define MAX_BUF 256 /* Max line size */
#define MAX_NAME 128 /* Max figure name size */
#define PI acos(-1.0)

/* Global Variables */
int imgNum, currentLine;
int initSize, bufferSize; 

/* this is defined in draw2.c */
extern const char* const exec_name;

/* Non-C99 compliant function prototypes. */
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
long int lround(double x);

/* Structure definitions */
struct aLine
{
char line[ MAX_BUF ];
};

struct image /* A structure for each image */
{
   char name[ MAX_NAME ];
   double *x;
   double *y;
   int size;
};

/* Function prototypes */
void bufferFile( FILE * fp, struct aLine * theLines, struct image * eachImage, FILE* executable ); 
void classifyLine( struct aLine * theLines, struct image * eachImage, FILE* executable, FILE* fp ); 
int print( struct aLine * theLines, struct image * eachImage );
int drawImg( struct aLine * theLines, struct image * eachImage, FILE*  executable );
int rotate( struct aLine * theLines, struct image * eachImage );
void cleanup( FILE*  executable, struct image * eachImage, FILE* fp, struct aLine * theLines);
int translate( struct aLine * theLines, struct image * eachImage );
#endif
