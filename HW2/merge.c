/* --------------------------------------------------------------- */
/* NAME : YunTsen Lo                           User ID: 108598056  */
/* DUE DATE : 04/20/2020                                           */
/* PROGRAM ASSIGNMENT #                                            */
/* FILE NAME : merge.c                                             */
/* PROGRAM PURPOSE :                                               */
/*      A merge program that reads an input array with n elements  */
/*    from shared memory and then perform mergesort on that array. */
/*    It creates (2^k-1+2^k-2+...+1) child processes to split the  */
/*    array, plus n*(k-1) child processes to merge. k=log(n)       */
/*      It requires another shared memory to communicate with its  */
/*    child process(for temporary merging result in merge phase).  */
/* --------------------------------------------------------------- */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void mergeSort(int *, int, int, int *);
void swap(int *, int *);
void merge(int *, int, int, int, int *);
int modifiedBinarySearch(int, int *, int, int);

/* -------------------------------------------------------------- */
/* FUNCTION : main                                                */
/*      reads input array from shared memory and creates          */
/*    a child process to execute mergeSort(0,n-1)                 */
/*      It created another shared memory to communicate with its  */
/*    child process(for temporary merging result in merge phase). */
/* PARAMETER USAGE :                                              */
/*    int argc: number of arguments                               */
/*    char *argv[]: arguments value                               */
/* FUNCTION CALLED :                                              */
/*    void mergeSort(int *, int, int, int *)                      */
/* -------------------------------------------------------------- */
int main(int argc, char *argv[]) {
    /*int left,right;
    left=atoi(argv[1]);
    right=atoi(argv[2]);*/
    key_t shmKey = ftok("./", 'e');
    char buf[200]; /*for standard output*/
    int shmID, n, i;
    int *shmPtr;
    int status;
    pid_t pid;

    /*get the original array through shared memory*/
    n = atoi(argv[2]);
    shmID = shmget(shmKey, sizeof(int) * n, 0666);
    if (shmID == -1) {
        sprintf(buf, "   ***MERGE: a[] shmget() failed. errno:%d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
        exit(-1);
    }
    shmPtr = (int *)shmat(shmID, NULL, 0);
    if ((long)shmPtr < 0) { /*check if shmat() succeed*/
        sprintf(buf, "   ***MERGE: a[] shmat() failed. errno:%d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
        exit(-1);
    }

    /*request a shared memory to hold the temporary result while merging*/
    int temShmKey, temShmID, *temShmPtr;
    temShmKey = ftok("./", 't');
    sprintf(buf, "   ***MERGE: shared memory key = %ld\n", (long)temShmKey);
    write(1, buf, strlen(buf));
    temShmID = shmget(temShmKey, sizeof(int) * n, IPC_CREAT | 0666);
    if (temShmID == -1) { /*check if shmhet() success*/
        sprintf(buf, "   ***MERGE: tem_a[] shmget() failed. errno: %d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
        exit(-1);
    } else {
        sprintf(buf, "   ***MERGE: tem_a[] shared memory created\n");
        write(1, buf, strlen(buf));
    }
    temShmPtr = (int *)shmat(temShmID, NULL, 0);
    if ((long)temShmPtr < 0) { /*check if shmat() success*/
        sprintf(buf, "   ***MERGE: tem_a[] shmat() failed. errno: %d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
        exit(-1);
    } else {
        sprintf(buf, "   ***MERGE: tem_a[] shared memory attached and is ready to use for storing temporary merging results\n");
        write(1, buf, strlen(buf));
    }

    /*create child to do mergeSort*/
    if ((pid = fork()) < 0) {
        sprintf(buf, "Create child process failed.\n");
        write(1, buf, strlen(buf));
        exit(-1);
    } else if (pid == 0) {
        mergeSort(shmPtr, 0, n - 1, temShmPtr);
        exit(0);
    } else {
        wait(&status);
    }

    /*detach and deallocte a shared memory*/
    if (shmdt((void *)shmPtr) == -1) {
        sprintf(buf, "   *** MERGE: a[] shared memory shmdt() failed. errno: %d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
    }
    if (shmdt((void *)temShmPtr) == -1) {
        sprintf(buf, "   *** MERGE: tem_a[] shared memory shmdt() failed. errno: %d, %s\n", errno, strerror(errno));
        write(1, buf, strlen(buf));
    } else {
        sprintf(buf, "   *** MERGE: tem_a[] shared memory successfully detached\n");
        write(1, buf, strlen(buf));
    }
    shmctl(temShmID, IPC_RMID, NULL);
    sprintf(buf, "   *** MERGE: tem_a[] shared memory successfully removed\n");
    write(1, buf, strlen(buf));

    return 0;
}

/* ----------------------------------------------------------------- */
/* FUNCTION : mergeSort                                              */
/*      Given an array and its lower/upper_bound, sort it            */
/*    recursively using mergeSort.                                   */
/* PARAMETER USAGE :                                                 */
/*    int *a: array to be sorted                                     */
/*    int lower_bound: the lower_bound(index) of array a[]           */
/*    int upper_bound: the upper_bound(index) of array a[]           */
/*    int *tem_a: array to store the temporay results in merge phase */
/* FUNCTION CALLED :                                                 */
/*    void mergeSort(int *, int, int, int *)                         */
/*    void merge(int *, int, int, int, int *)                        */
/*    void swap(int *, int *);                                       */
/* ----------------------------------------------------------------- */
void mergeSort(int *a, int lower_bound, int upper_bound, int *tem_a) {
    char buf[200]; /*for standard output*/
    int middle, i;
    int status; /*for wait()*/
    pid_t pid;

    sprintf(buf, "   ### M-PROC(%ld) created by M-PROC(%ld): entering with a[%d..%d]\n   ",
            (long)getpid(), (long)getppid(), lower_bound, upper_bound);
    for (i = lower_bound; i <= upper_bound; i++) {
        sprintf(buf + strlen(buf), "%4d", a[i]);
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    if (upper_bound == lower_bound)
        return;
    else if (upper_bound - lower_bound == 1) {
        if (a[lower_bound] > a[upper_bound]){
            swap(&a[lower_bound], &a[upper_bound]);
        }
        sprintf(buf,"   ### M-PROC(%ld) created by M-PROC(%ld): entering with a[%d..%d] -- sorted\n   ",
        (long)getpid(),(long)getppid(),lower_bound,upper_bound);
        for(i=lower_bound;i<=upper_bound;i++){
            sprintf(buf+strlen(buf),"%4d",a[i]);
        }
        sprintf(buf+strlen(buf),"\n");
        write(1, buf, strlen(buf));
        return;
    }

    middle = (lower_bound + upper_bound) / 2;

    if ((pid = fork()) < 0) {
        sprintf(buf, "fork() failed");
        write(1, buf, strlen(buf));
        exit(-1);
    } else if (pid == 0) {
        mergeSort(a, lower_bound, middle, tem_a);
        exit(0);
    } else {
        if ((pid = fork()) < 0) {
            sprintf(buf, "fork() failed");
            write(1, buf, strlen(buf));
            exit(-1);
        } else if (pid == 0) {
            mergeSort(a, middle + 1, upper_bound, tem_a);
            exit(0);
        } else {
            wait(&status);
            wait(&status);
            sprintf(buf, "   ### M-PROC(%ld) created by M-PROC(%ld): both array section sorted. start merging\n",
                    (long)getpid(), (long)getppid());
            write(1, buf, strlen(buf));
            merge(a, lower_bound, middle, upper_bound, tem_a);
            sprintf(buf, "   ### M-PROC(%ld) created by M-PROC(%ld): merge sort a[%d..%d] completed:\n",
                    (long)getpid(), (long)getppid(), lower_bound, upper_bound);
            sprintf(buf + strlen(buf), "   ");
            for (i = lower_bound; i <= upper_bound; i++) {
                sprintf(buf + strlen(buf), "%4d", a[i]);
            }
            sprintf(buf + strlen(buf), "\n");
            write(1, buf, strlen(buf));
        }
    }
}

/* ------------------------------------------- */
/* FUNCTION : swap                             */
/*      exchange the value of two variables    */
/* PARAMETER USAGE :                           */
/*    int *a: a pointer to int                 */
/*    int *b: a pointer to int                 */
/* FUNCTION CALLED : NONE                      */
/* ------------------------------------------- */
void swap(int *a, int *b) {
    int tem;
    tem = *a;
    *a = *b;
    *b = tem;
    return;
}

/* -------------------------------------------------------------------- */
/* FUNCTION : merge                                                     */
/*      merge two sorterd array                                         */
/* PARAMETER USAGE :                                                    */
/*    int *a: array to be merged, it can be seperated into              */
/*      two subarrays x[lower_bound,middle], y[middle+1, upper_bound]   */
/*    int lower_bound: the lower_bound(index) of a[]                    */
/*    int middle: the middle(index) of a[]                              */
/*    int upper_bound: the upper_bound(index) of a[]                    */
/*    int *tem_a: array to store the temporary results while merging    */
/* FUNCTION CALLED :                                                    */
/*    int modifiedBinarySearch(int, int *, int, int);                   */
/* -------------------------------------------------------------------- */
void merge(int *a, int lower_bound, int middle, int upper_bound, int *tem_a) {
    int order, i, final_position, n, status;
    int x_left, x_right, y_left, y_right;
    char buf[200]; /*for standard output*/
    pid_t pid;
    x_left = lower_bound;
    x_right = middle;
    y_left = middle + 1;
    y_right = upper_bound;
    n = upper_bound - lower_bound + 1;

    int *tem_x, *tem_y;
    tem_x = (int *)malloc(sizeof(int) * (n / 2));
    tem_y = (int *)malloc(sizeof(int) * (n / 2));

    /*int tem_x[200], tem_y[200];*/

    for (i = x_left; i <= x_right; i++) {
        tem_x[i - x_left] = a[i];
    }
    for (i = y_left; i <= y_right; i++) {
        tem_y[i - y_left] = a[i];
    }

    /*find final position of entries in x_array*/
    for (i = x_left; i <= x_right; i++) {
        if ((pid = fork()) == 0) {
            sprintf(buf, "      $$$ B-PROC(%ld): created by M-PROC(%ld) for a[%d] = %d is created\n",
                    (long)getpid(), (long)getppid(), i, a[i]);
            write(1, buf, strlen(buf));
            order = modifiedBinarySearch(a[i], tem_y, 0, (n / 2) - 1);
            final_position = i + order;
            tem_a[final_position] = a[i];
            if (order == 0) {  //x[i] is smaller than every elements in y[]
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], y_left + order, a[y_left + order], final_position);
                write(1, buf, strlen(buf));
            } else if (order == n / 2) {  //x[i] is bigger than every elements in y[]
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is bigger than a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], y_left + order, a[y_left + order], final_position);
                write(1, buf, strlen(buf));
            } else {
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], y_left + order - 1, a[y_left + order - 1], y_left + order, a[y_left + order], final_position);
                write(1, buf, strlen(buf));
            }
            sprintf(buf, "      $$$ B-PROC(%ld): created by M-PROC(%ld) for a[%d] = %d is terminated\n",
                    (long)getpid(), (long)getppid(), i, a[i]);
            write(1, buf, strlen(buf));
            exit(0);
        }
    }

    /*find final position of entries in y_array*/
    for (i = y_left; i <= y_right; i++) {
        if ((pid = fork()) == 0) {
            order = modifiedBinarySearch(a[i], tem_x, 0, (n / 2) - 1);
            final_position = i - (n / 2) + order;
            tem_a[final_position] = a[i];
            if (order == 0) {  //y[i] is smaller than every elements in x[]
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], x_left + order, a[x_left + order], final_position);
                write(1, buf, strlen(buf));
            } else if (order == n / 2) {  //y[i] is bigger than every elements in x[]
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is bigger than a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], x_left + order, a[x_left + order], final_position);
                write(1, buf, strlen(buf));
            } else {
                sprintf(buf, "      $$$ B-PROC(%ld): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n",
                        (long)getpid(), i, a[i], x_left + order - 1, a[x_left + order - 1], x_left + order, a[x_left + order], final_position);
                write(1, buf, strlen(buf));
            }
            exit(0);
        }
    }
    for (i = lower_bound; i <= upper_bound; i++) {
        wait(&status);
    }

    /*merge*/
    for (i = lower_bound; i <= upper_bound; i++) {
        a[i] = tem_a[i];
    }

    free(tem_x);
    free(tem_y);
}

/* -------------------------------------------------------------------- */
/* FUNCTION : modifiedBinarySearch                                      */
/*      Given a target value and a sorted array,                        */
/*    use modiefiedBinarySearch to see the target value should be place */
/*    in which position(or the target is bigger than how many values in)*/
/*    the sorter array). Runs recursively.                              */
/* PARAMETER USAGE :                                                    */
/*    int target: the target value to be compared with values in list[] */
/*    int *list: sorted array                                           */
/*    int start: start(index) of list[]                                 */
/*    int end: end(index) of list[]                                     */
/* FUNCTION CALLED :                                                    */
/*    int modifiedBinarySearch(int, int *, int, int);                   */
/* -------------------------------------------------------------------- */
int modifiedBinarySearch(int target, int *list, int start, int end) {
    int middle = (start + end) / 2;
    int element = list[middle];
    if (start == end) {
        if (target < element)
            return middle;
        else
            return middle + 1;
    } else if (target < element) {
        return modifiedBinarySearch(target, list, start, middle);
    } else {
        return modifiedBinarySearch(target, list, middle + 1, end);
    }
}
