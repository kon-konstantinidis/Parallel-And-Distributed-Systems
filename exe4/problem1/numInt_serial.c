#include <stdio.h>
#include <time.h>

/**
 * @brief The function f(R)->R to integrate
 * 
 * @param num Input
 * @return float Output
 */
float f(float num){
    float value = num*num*num; // change this to change the function
    return value;
}


float numericalInt_serial(float(*f)(float), int lower, int upper, int intervals ){
    float h = (float)(upper - lower)/(float)intervals;
    float integral = 0;
    integral = f(lower) + f(upper);
    float k;
    for(int i=1; i <= (intervals-1); i++)
    {
        k = lower + i*h;
        integral += 2 * f(k);
    }
    integral = integral * (h/2);
    return integral;
}


int main(){
    int reps = 1000;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME,&start);

    /*code to time here, repeated enough times and averaged */
    float integral;
    for (int i=0; i<reps; i++){
        integral = numericalInt_serial(f,0,50000,1000000);
    }
    
    clock_gettime(CLOCK_REALTIME,&end);\
    long unsigned exeTime = 1000000000*(end.tv_sec - start.tv_sec);
    exeTime += end.tv_nsec - start.tv_nsec;
    exeTime /= reps; // average execution time of all reps

    printf("Integral calculated as: %f \n",integral);
    printf("The serial implementation of numerical integration has execution time: %f ms\n",(float)exeTime/1000000);
    return 0;
}