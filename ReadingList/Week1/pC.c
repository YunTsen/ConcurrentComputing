#include  <stdio.h>

int  main(void)
{
     int   i, LIMIT = 100;
     char  input[100];

     // now we use a better way: fgets()
     // keep reading until EOF 
     while (fgets(input, LIMIT, stdin) != NULL)
          printf("          From C: %s", input);
}

