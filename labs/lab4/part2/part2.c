#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE* input; 
  long integer, first;
  int i;
  long *numbers;
  i = 1; 
  
  input = fopen("input2.txt", "r");
  fscanf( input, "%ld", &first);
  numbers = (long *)malloc(sizeof(long)*first); //allocate first longs
  memset( numbers, 0, first);
  while ( 1 ) {
    fscanf( input, "%ld", &integer);
    if ( feof ( input ))
      break;
    numbers[ i ] = integer;
    i++;
  }
  i = i -1; 
  //printf("%ld\n", numbers[0]);//
  //printf("%d\n", i);//
  while (i >  0){
    printf("%ld\n", numbers[i]);
    i--;
  }
  free(numbers); 
}
  
  
