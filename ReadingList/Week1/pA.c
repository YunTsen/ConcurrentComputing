#include <stdio.h>
#include <stdlib.h>//atoi 


//修改
//int main(int argc, char **argv[])
int main(int argc, char *argv[])
{
     int   i, LIMIT;
     char  output[100];

     LIMIT = atoi(argv[1]);  // read command line argument
     printf("%d\n", LIMIT);  // print its value
     for (i = 1; i <= LIMIT; i++) {  // iterate 
          sprintf(output, "Printing %d from A", i);
          printf("%s\n", output);
     }
}

