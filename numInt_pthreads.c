#include <stdio.h>
#include <pthread.h>

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


typedef struct input_args {
    float(*f)(float);
    float lower;
    float upper;
    int intervals;
    float integral;
    int leftEdge; // whether this thread has the lower end of the integration space (the type changes)
    int rightEdge; // whether this thread has the upper end of the integration space (the type changes)
} input_args;

void * pthread_func(void *args_in){
    input_args* args = args_in;
    int lower = args->lower;
    int upper = args->upper;
    int intervals = args->intervals;
    float (*f)(float) = args->f;

    float h = (float)(upper - lower)/(float)intervals;
    float integral = 0;

    integral = 2*f(lower) + 2*f(upper);
    if (args->leftEdge==1){
        integral -= f(lower);
    }
    else if(args->rightEdge==1){
        integral -= f(upper);
    }
    float k;
    for(int i=1; i <= (intervals-1); i++)
    {
        k = lower + i*h;
        integral += 2 * f(k);
    }
    integral = integral * (h/2);
    args->integral = integral;
}


float numericalInt_pthreads(float(*funcToIntegrate)(float), int lower, int upper, int intervals, int numThreads){
    float integral = 0;
    pthread_t threads[numThreads];
    input_args all_thread_args[numThreads];

    float intervalPerThread = (upper-lower)/numThreads;
    float intervalsPerThread = intervals/numThreads;

    for (int i=0; i<numThreads; i++){
        all_thread_args[i].f = funcToIntegrate;
        all_thread_args[i].lower = lower + intervalPerThread*i;
        all_thread_args[i].upper = lower + intervalPerThread*(i+1);
        all_thread_args[i].intervals = intervalsPerThread;
        pthread_create(&threads[i],NULL,&pthread_func,&all_thread_args[i]);
    }
    for(int i=0; i<numThreads; i++){
        pthread_join(threads[i],NULL);
        integral += all_thread_args[i].integral;
    }
    return integral;
}


int main(){
    int reps = 1000;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME,&start);


    /*code to time here, repeated enough times and averaged */
    float integral;
    for (int i=0; i<reps; i++){
        integral = numericalInt_pthreads(f,0,50000,1000000,16);
    }

    clock_gettime(CLOCK_REALTIME,&end);
    long unsigned exeTime = 1000000000*(end.tv_sec - start.tv_sec);
    exeTime += end.tv_nsec - start.tv_nsec;
    exeTime /= reps; // average execution time of all reps

    printf("Integral calculated as: %f \n",integral);
    printf("The pthreads implementation of numerical integration has execution time: %f ms\n",(float)exeTime/1000000);
    return 0;
}