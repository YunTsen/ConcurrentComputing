#include  <stdio.h>
#include <stdlib.h>//atoi 

int  main(void)
{
     int   i, LIMIT;
     char  input[100];
   
     fgets(input,99,stdin);   // read a complete input line
     LIMIT = atoi(input); // convert to integer
     for (i = 1; i <= LIMIT; i++) {  // repeat
          fgets(input,99,stdin);    // read a complete input line
          printf("     From B: %s\n", input);
     }
}