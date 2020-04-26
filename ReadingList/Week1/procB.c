#include <stdio.h>
#include <stdlib.h>
#include <time.h>//time

#define  LIMIT (20)

int main(void)
{
     int i, j, x, y;

     srand(time(NULL));
     for (j = 1; j <= LIMIT; j++) {
          x = rand()/30;
          for (i = 1; i <= x; i++)
               y = rand();
          printf("          Hi, B here!  Random number = %d\n", x);
     }
     printf("          B completes\n");
}

