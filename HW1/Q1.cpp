#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
     int i, n = atoi(argv[1]);
    printf("0:Process %ld with parent %ld\n", (long)getpid(), (long)getppid());
     for (i = 1; i < n; i++)
          if (fork()){
              printf("1:Process %ld with parent %ld\n", (long)getpid(), (long)getppid());
               break;
          }
          else{
              printf("2:Process %ld with parent %ld\n", (long)getpid(), (long)getppid());
          }
     printf("3:Process %ld with parent %ld\n", (long)getpid(), (long)getppid());
     sleep(2);
}