/* ----------------------------------------------------------- */
/* NAME : 羅芸岑                         User ID: xxxxxxxx */
/* DUE DATE : mm/dd/yyyy                                       */
/* PROGRAM ASSIGNMENT #                                        */
/* FILE NAME : xxxx.yyyy.zzzz (your unix file name)            */
/* PROGRAM PURPOSE :                                           */
/*    A couple of lines describing your program briefly        */
/* ----------------------------------------------------------- */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

long fibonacci(int);
double buffonsNeedle(int);
int ellipseArea(int, int, int);
void pinball(int, int);

int main(int argc, char* argv[]) {
    int status;
    pid_t pid;
    char buf[100];
    sprintf(buf, "Main Process Started\n");
    write(1, buf, strlen(buf));
    sprintf(buf, "Fibonacci Input            = %s\n", argv[1]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Buffon's Needle Iterations = %s\n", argv[2]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Total random Number Pairs  = %s\n", argv[3]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Semi-Major Axis Length     = %s\n", argv[4]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Semi-Minor Axis Length     = %s\n", argv[5]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Number of Bins             = %s\n", argv[6]);
    write(1, buf, strlen(buf));
    sprintf(buf, "Number of Ball Droppings   = %s\n", argv[7]);
    write(1, buf, strlen(buf));
    //try to fork the first child process
    pid = fork();
    if (pid < 0) {  //failed to fork child process1
        sprintf(buf, buf, "fork() failed\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) {  //fork succeed, childProcess1 runs: Fibonacci
        sprintf(buf, "Fibonacci Process Created\n");
        write(1, buf, strlen(buf));
        sprintf(buf, "   Fibonacci Process Started\n");
        write(1, buf, strlen(buf));
        sprintf(buf, "   Input Number 10%d\n", atoi(argv[1]));
        write(1, buf, strlen(buf));
        sprintf(buf, "   Fibonacci Number f(10) is %ld\n", fibonacci(atoi(argv[1])));
        write(1, buf, strlen(buf));
        sprintf(buf, "   Fibonacci Process Exits\n");
        write(1, buf, strlen(buf));
    } else {
        //try to fork the second child process
        pid = fork();
        if (pid < 0) {  //failed to fork child process2
            sprintf(buf, "fork() failed\n");
            write(1, buf, strlen(buf));
        } else if (pid == 0) {  //fork succeed, childProcess2 runs: Buffon's Needle
            sprintf(buf, "Buffon's Needle Process Created\n");
            write(1, buf, strlen(buf));
            sprintf(buf, "      Buffon's Needle Process Started\n");
            write(1, buf, strlen(buf));
            sprintf(buf, "      Input Number 10%d\n", atoi(argv[2]));
            write(1, buf, strlen(buf));
            sprintf(buf, "      Estimated Probability is %.5f\n", buffonsNeedle(atoi(argv[2])));
            write(1, buf, strlen(buf));
            sprintf(buf, "      Buffon's Needle Process Exits\n");
            write(1, buf, strlen(buf));
        } else {
            //try to fork the third child process
            pid = fork();
            if (pid < 0) {  //failed to fork child process3
                sprintf(buf, "fork() failed\n");
                write(1, buf, strlen(buf));
            } else if (pid == 0) {  //fork succeed, childProcess3 runs: Ellipse Area
                sprintf(buf, "Ellipse Area Process Created\n");
                write(1, buf, strlen(buf));
                int a, b, s;
                a = atoi(argv[3]);
                b = atoi(argv[4]);
                s = atoi(argv[5]);
                sprintf(buf, "         Ellipse Area Process Started\n");
                write(1, buf, strlen(buf));
                sprintf(buf, "         Total random Number Pairs %d\n", s);
                write(1, buf, strlen(buf));
                sprintf(buf, "         Semi-Major Axis Length %d\n", a);
                write(1, buf, strlen(buf));
                sprintf(buf, "         Semi-Minor Axis Length %d\n", b);
                write(1, buf, strlen(buf));
                sprintf(buf, "         Total Hits %d\n", ellipseArea(6, 2, 200000));
                write(1, buf, strlen(buf));
                sprintf(buf, "         Estimated Area is %.5f\n", ((double)(ellipseArea(a, b, s)) / s) * a * b * 4);
                write(1, buf, strlen(buf));
                sprintf(buf, "         Actual Area is %.5f\n", (double)acos(-1.0) * a * b);
                write(1, buf, strlen(buf));
                sprintf(buf, "         Ellipse Area Process Exits\n");
                write(1, buf, strlen(buf));
            } else {
                //try to fork the forth child process
                pid = fork();
                if (pid < 0) {  //failed to fork child process4
                    sprintf(buf, "fork() failed\n");
                    write(1, buf, strlen(buf));
                } else if (pid == 0) {  //fork succeed, childProcess4 runs: Pinball Game
                    sprintf(buf, "Pinball Process Created\n");
                    write(1, buf, strlen(buf));
                    sprintf(buf, "Simple Pinball Process Started\n");
                    write(1, buf, strlen(buf));
                    sprintf(buf, "Number of Bins %s\n", argv[6]);
                    write(1, buf, strlen(buf));
                    sprintf(buf, "Number of Ball Droppings %s\n", argv[7]);
                    write(1, buf, strlen(buf));
                    pinball(atoi(argv[6]), atoi(argv[7]));
                    sprintf(buf, "Simple Pinball Process Exits\n");
                    write(1, buf, strlen(buf));
                } else {
                    //wait for all 4 children done
                    sprintf(buf, "Main Process Waits\n");
                    write(1, buf, strlen(buf));
                    for (int i = 0; i < 4; i++) {
                        wait(&status);
                    }
                    sprintf(buf, "Main Process Exits\n");
                    write(1, buf, strlen(buf));
                }
            }
        }
    }
    return 0;
}

long fibonacci(int n) {
    long f1 = 1, f2 = 1;
    if (n == 0)
        return 0;
    else if (n == 1 | n == 2)
        return 1;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

double buffonsNeedle(int r) {
    double PI = acos(-1.0);
    double d, a, G, L;
    G = L = 1;
    int count = 0;
    // initialize random seed
    srand(time(NULL));

    for (int i = 0; i < r; i++) {
        //generate two random value, d & a
        d = rand() / (RAND_MAX + 1.0);                     //[0,1)
        a = (2 * PI - 0) * rand() / (RAND_MAX + 1.0) + 0;  //[0,2*π)
        //see if the needle crosses a dividing line
        if ((d + L * sin(a) < 0) | (d + L * sin(a) > G))
            count++;
    }

    return count / (double)r;
}

int ellipseArea(int a, int b, int s) {
    double x, y;
    int count = 0;
    // initialize random seed
    srand(time(NULL));

    for (int i = 0; i < s; i++) {
        //generate two random value, x & y
        x = ((double)a - 0) * rand() / (RAND_MAX + 1.0) + 0;  //x, 0 ≤ x < a
        y = ((double)b - 0) * rand() / (RAND_MAX + 1.0) + 0;  //y, 0 ≤ y < b
        //see if (x,y) is in the given ellipse
        if (pow(x, 2) / pow(a, 2) + pow(y, 2) / pow(b, 2) <= 1)
            count++;
    }
    return count;
}

void pinball(int x, int y) {
    float odds;
    int temPinNo;
    //int pin[x]
    int* pin;
    char buf[100];
    pin = (int*)malloc(x * sizeof(int));

    if (pin == NULL) {  //malloc() failed
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    } else {  //malloc() succeed, initialize: pin[x]={0}
        for (int i = 0; i < x; i++) {
            pin[i] = 0;
        }
    }
    //initialize random seed
    srand(time(NULL));

    for (int i = 0; i < y; i++) {
        temPinNo = 0;
        for (int j = 0; j < x - 1; j++) {
            //generate a random value, odd
            odds = rand() / (RAND_MAX + 1.0);  //odds, [0,1)
            if (odds < 0.5) {                  //the ball goes left

            } else if (odds >= 0.5) {  //the ball goes right
                temPinNo += 1;
            }
        }
        *(pin + temPinNo) += 1;
    }
    for (int i = 0; i < x; i++) {
        sprintf(buf, "%3d-(%7d)-(%5.2f)|", i + 1, *(pin + i), (*(pin + i) / (float)y) * 100);
        write(1, buf, strlen(buf));
        for (float j = 0; j < (*(pin + i) / (float)y) * 100; j++) {
            sprintf(buf, "*");
            write(1, buf, strlen(buf));
        }
        sprintf(buf, "\n");
        write(1, buf, strlen(buf));
    }
    free(pin);
    return;
}