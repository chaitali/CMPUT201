/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 */

#ifndef DRAW1B_H
#define DRAW1B_H

#define MAX_BUF 256 /* Max line size */
#define MAX_LINES 256 /* Max number of lines */
#define MAX_NAME 128 /* Max figure name size */

/* Global Variables */
int imgNum, currentLine, place, saveNum ;

const char exec_name[] = "java -jar Sketchpad.jar ";

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
   char name[ 128 ];
   double x[ 256 ];
   double y[ 256 ];
   int size;
};

/* Function prototypes */
void bufferFile( FILE * fp, struct aLine * theLines, struct image * eachImage, FILE* executable ); 
void classifyLine( struct aLine * theLines, struct image * eachImage, FILE* executable ); 
void print( struct aLine * theLines, struct image * eachImage );
void drawImg( struct aLine * theLines, struct image * eachImage, FILE*  executable );

#endif
