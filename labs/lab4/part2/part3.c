#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len; 



int main() {
  FILE* input; 
  long integer;
  int i;
  long *numbers;
  i = 1; 
  len = 10; 
  input = fopen("input3.txt", "r");
  //fscanf( input, "%ld", &first);
  numbers = (long *)malloc(sizeof(long)*len); //allocate first longs to 10
  memset( numbers, 0, len);

  while ( 1 ) {
    fscanf( input, "%ld", &integer);
    if ( feof ( input ))
      break;
    numbers[ i ] = integer;
    i++;
    if(i > len) {
      len = len * 2;
      numbers = ( long*)realloc(numbers, sizeof(long) * ( len )); 
      //printf("x %d %d", len, i); 
      
    }
  
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
  
  
