#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINES 100


int main() {
  FILE* input; 
  long numbers[ MAX_LINES ];
  long integer;
  int i;
  i = 0;
  
  memset( numbers, 0, MAX_LINES );
  input = fopen("input1.txt", "r");
  while ( 1 ) {
    fscanf( input, "%ld",&integer);
    if ( feof ( input ))
      break;
    numbers[ i ] = integer;
    i++; 
  }
  i = i -1; 
  //printf("%ld\n", numbers[0]);//
  //printf("%d\n", i);//
  while (i >=  0){
    printf("%ld\n", numbers[i]);
    i--;
  }
}
  
  
