/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * Based on the program finddw
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* For strncmp(), etc. */

void parseFile( FILE * fp, char * fname );

int main( int argc, char * argv[] )
{
   int i;
   FILE * fp;

   /* open file and check if it opens */
   for( i = 1; i < argc; i++ ){
     fp = fopen( argv[ i ], "r" );
     if( fp == NULL ){
       printf( "Could not open file %s\n", argv[ i ] );
       exit( -1 );
     }
    /* print the file name*/
     else{
       printf( "%s started on ",argv[ 0 ]);
       parseFile( fp, argv[ i ] );
       fclose( fp );
     }
   }  
   return( 0 );
} /* main */

void parseFile( FILE * fp, char * fname )
{
   /* flush stdout and print date and input file */
   fflush(NULL);
   system("date");
   printf("Input file: %s\n",fname);

   char word[10];
   int pound = 0,
       lineTo = 0,
       End = 0,
       Image = 0,
       print = 0,
       draw = 0,
       translate = 0,
       child = 0,
       rval;

   /* Read first word */
   rval = fscanf(fp, "%s", word );
   if( rval != 1 ){
     printf( "Failed to read first word\n" );
    exit( -1 );
   }
   /* finish checking if valid*/
   rewind(fp);
   /*until end of file*/
   while( 1 ){
     
     fscanf ( fp, "%s%*[^\n]" , word );
     /* check if the end of file has been reached */
     if ( feof ( fp ) ) {
       break;
     }
     /* count the occurences of each first word */
     if ( strncmp ( word, "#" , 10 ) == 0 ) {
	pound++;
     }
     if ( strncmp ( word, "Image" , 10 ) == 0 ) {
        Image++;
     }
     if ( strncmp ( word, "lineTo" , 10 ) == 0 ) {
        lineTo++;
     }
     if ( strncmp ( word, "End" , 10 ) == 0 ) {
        End++;
     }
     if ( strncmp ( word, "print" , 10 ) == 0 ) {
        print++;
     }
     if ( strncmp ( word, "translate" , 10 ) == 0 ) {
        translate++;
     }
     if ( strncmp ( word, "draw" , 10 ) == 0 ) {
        draw++;
     }
     if ( strncmp ( word, "child" , 10 ) == 0 ) {
        child++;
     }
   } /* while */
   
   /* print to stdout */
   printf( "%d Image definitions(s)\n", Image );
   printf( "%d lineTo command(s) within Figures\n", lineTo );
   printf( "%d End command(s)\n", End );
   printf( "%d print command(s)\n", print );
   printf( "%d draw command(s)\n", draw );
   printf( "%d translate command(s)\n" , translate );
   printf( "%d child commands(s)\n", child );
   printf( "%d comments(s)\n", pound );
}  /* parseFile */
