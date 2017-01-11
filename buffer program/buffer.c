#include <stdio.h>
#include <string.h> /* For memset() */
#include <stdlib.h> /* For exit() */
#define MAX_BUF 256 /* Max line size */
#define MAX_LINES 32 /* Max number of lines */

struct aLine /* A structure for heterogeneous data */
{
char line[ MAX_BUF ];
int type;
};

void bufferFile( FILE * fp, struct aLine * theLines,
int * currentLinePtr, int maxLines );
void classifyLine( struct aLine * theLines, int currentLine );
void printClass( struct aLine * theLines, int currentLine );

int main( int argc, char * argv[] )
{
int i;
FILE * fp;
/* Here, I avoid global variables */
struct aLine theLines[ MAX_LINES ]; /* An array of structs */
int currentLine;
currentLine = 0;
memset( theLines, 0, sizeof( theLines ) ); /* Initialize. Defensive. */
for( i = 1; i < argc; i++ )
{
fp = fopen( argv[ i ], "r" );
if( fp == NULL )
{
printf( "Could not open file %s\n", argv[ i ] );
}
else
{
/* currentLine is passed-by-reference/pointer */
bufferFile( fp, theLines, &currentLine, MAX_LINES );
fclose( fp );
}
}
return( 0 );
} /* main */

void bufferFile( FILE * fp, struct aLine * theLines, int * currentLinePtr,
int maxLines )
{
char * rbuf;
/* Line-oriented */
while( ( *currentLinePtr < maxLines ) && !feof( fp ) )
{
rbuf = fgets( theLines[ *currentLinePtr ].line, MAX_BUF, fp );
if( rbuf == NULL )
break;
printf( "Read (%2d): %s",
*currentLinePtr, theLines[ *currentLinePtr ].line );
classifyLine( theLines, *currentLinePtr );
printClass( theLines, *currentLinePtr );
fflush( stdout );
(*currentLinePtr)++;
} /* while */
/* Warn user if we exceed maximum number of lines buffered */
if( *currentLinePtr >= maxLines )
{
printf( "Warning: Exceeded %d lines of buffering.\n",
*currentLinePtr);
}
} /* bufferFile */


#define KEY_MATRIX "Image"
void classifyLine( struct aLine * theLines, int currentLine )
{
int rval, len;
char firstWord[ MAX_BUF ];
theLines[ currentLine ].type = -1; /* Default */
/* Check for comments */
if( theLines[ currentLine ].line[ 0 ] == '#' )
{
theLines[ currentLine ].type = 0;
return;
}
/* Check for matrix definitions */
memset( firstWord, 0, MAX_BUF ); /* Initialize. Defensive. */
rval = sscanf( theLines[ currentLine ].line, "%s", firstWord );
if( rval != 1 )
{
#if 0
printf( "Error in sscanf\n" );
#endif
return;
}
if( strncmp( firstWord, KEY_MATRIX, MAX_BUF ) == 0 )
{
theLines[ currentLine].type = 1;
}
} /* classifyLine */


void printClass( struct aLine * theLines, int currentLine )
{
switch( theLines[ currentLine ].type )
{
case -1:
printf( "\tUnknown\n" );
break;
case 0:
printf( "\tComment\n" );
break;
case 1:
printf( "\tMatrix Definition\n" );
break;
default:
printf( "\tUndefined\n" );
break;
}
} /* printClass */
