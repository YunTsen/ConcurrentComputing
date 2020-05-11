/* -------------------------------------------------------------- */
/* NAME : YunTsen Lo                           User ID: 108598056 */
/* DUE DATE : 04/20/2020                                          */
/* PROGRAM ASSIGNMENT #                                           */
/* FILE NAME : main.c                                             */
/* PROGRAM PURPOSE :                                              */
/*      A main program that reads input array and fork a child to */
/*    to do merge sort on that array. It uses shared memory to    */
/*    communicate with its child process.                         */
/* -------------------------------------------------------------- */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* --------------------------------------------------- */
/* FUNCTION : main                                     */
/*       Reads input array and creates a child process  */
/*     a child process to do merge sort.                */
/* PARAMETER USAGE :                                   */
/*    int argc: number of arguments                    */
/*    char *argv[]: arguments value                    */
/* FUNCTION CALLED : NONE                              */
/* --------------------------------------------------- */
int main(int argc, char *argv[]) {
    char buf[200]; /*for standard output*/
    int n, i, status;

    sprintf(buf, "*** MAIN: Merge Sort with Multiple Processes:\n");
    write(1, buf, strlen(buf));

    scanf("%d", &n);                         /*the number of elements of array a[ ]*/
    if(n==0){
        sprintf(buf,"*** MAIN: Input null array, process terminates.\n");
        write(1, buf, strlen(buf));
        return 0;
    }
    int *a = (int *)malloc(sizeof(int) * n); /*dynamic memory allocation for array a[]*/
    for (i = 0; i < n; i++) {
        scanf("%d", a + i); /*elements of array a[ ]*/
    }

    /*request a shared memory :shm*/
    key_t shmKey = ftok("./", 'e');
    int shmID;
    int *shmPtr;
    shmID = shmget(shmKey, sizeof(int) * n, IPC_CREAT | 0666);
    sprintf(buf, "*** MAIN: shared memory key = %ld\n", (long)shmKey);
    write(1, buf, strlen(buf));
    if (shmID == -1) { /*check if shmget succeed*/
        sprintf(buf, "*** MAIN:shmget() for shm failed. Errno:%d, %s.\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
        exit(-1);
    } else {
        sprintf(buf, "*** MAIN: shared memory created\n");
        write(1, buf, strlen(buf));
    }
    shmPtr = (int *)shmat(shmID, NULL, 0);
    if ((long)shmPtr < 0) { /*check if shmat() succeed*/
        sprintf(buf, "*** MAIN:shmat() failed\n");
        write(1, buf, strlen(buf));
    } else {
        sprintf(buf, "*** MAIN: shared memory attached and is ready to use\n");
        write(1, buf, strlen(buf));
    }
    /*initialize the value of share memory*/
    for (i = 0; i < n; i++) {
        *(shmPtr + i) = *(a + i);
    }

    sprintf(buf, "*** MAIN: Input array for mergesort has %d elements:\n",n);
    for (i = 0; i < n; i++) {
        sprintf(buf + strlen(buf), "%4d", *(shmPtr + i));
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    /*fork child process to run program merge.c*/
    pid_t pid;
    char *execvpArgv[4]; /* ./merge left right */
    char prog[] = "./merge";
    char left[] = "0";
    char tem[256];
    sprintf(tem, "%d", n);
    char *right = tem;
    execvpArgv[0] = prog;
    execvpArgv[1] = left;
    execvpArgv[2] = right;
    execvpArgv[3] = '\0'; /*NOTE:do not use "\0"*/

    sprintf(buf, "*** MAIN: about to spawn the merge sort process\n");
    write(1, buf, strlen(buf));

    if ((pid = fork()) < 0) { /*fork() failed*/
        sprintf(buf, "Create child process failed.\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) { /*fork() succeed, child process execute execvp(./merge left right)*/
        if (execvp("./merge", execvpArgv) < 0) {
            sprintf(buf, "execvp()failed.\n");
            write(1, buf, strlen(buf));
        }
        exit(0);
    }

    wait(&status);
    sprintf(buf, "*** MAIN: merged array:\n");
    for (i = 0; i < n; i++) {
        sprintf(buf + strlen(buf), "%4d", *(shmPtr + i));
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    /*free the dinamic allocation memory space*/
    free(a);
    /*detach and deallocate a shared memory*/
    if (shmdt((void *)shmPtr) == 0) { /*shmdt() succeed*/
        sprintf(buf, "*** MAIN: shared memory successfully detached\n");
        write(1, buf, strlen(buf));
    } else { /*shmdt() failed*/
        sprintf(buf, "*** MAIN: shared memory fail to detach. Errno:%d, %s.\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
    }
    if (shmctl(shmID, IPC_RMID, NULL) == 0) { /*shmctl() succeed*/
        sprintf(buf, "*** MAIN: shared memory successfully removed.\n");
        write(1, buf, strlen(buf));
    } else { /*shmctl() failed*/
        sprintf(buf, "*** MAIN: shared memory fail to deallocate. Errno:%d, %s.\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
    }
    return 0;
}
