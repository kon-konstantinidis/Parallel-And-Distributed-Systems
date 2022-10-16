#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define LENGTH 10000000
#define filename "randomArray10000000.txt"


struct thread_data
{
  int * arr;
  int low;
  int high;
  int level;
};

void swap(int lyst[], int i, int j)
{
  double temp = lyst[i];
  lyst[i] = lyst[j];
  lyst[j] = temp;
}

int partition(int lyst[], int lo, int hi)
{
  int b = lo;
  int r = (int)(lo + (hi - lo + 1) * (1.0 * rand() / RAND_MAX));
  int pivot = lyst[r];
  swap(lyst, r, hi);
  for (int i = lo; i < hi; i++)
  {
    if (lyst[i] < pivot)
    {
      swap(lyst, i, b);
      b++;
    }
  }
  swap(lyst, hi, b);
  return b;
}

void quicksortHelper(int lyst[], int lo, int hi)
{
  if (lo >= hi)
    return;
  int b = partition(lyst, lo, hi);
  quicksortHelper(lyst, lo, b - 1);
  quicksortHelper(lyst, b + 1, hi);
}

void *parallelQuicksortHelper(void *threadarg)
{
  int mid, t, rc;
  void *status;

  struct thread_data *my_data;
  my_data = (struct thread_data *)threadarg;

  if (my_data->level <= 0 || my_data->low == my_data->high)
  {
    // We have plenty of threads, finish with sequential.
    quicksortHelper(my_data->arr, my_data->low, my_data->high);
    pthread_exit(NULL);
  }

  // Now we partition our part of the lyst.
  mid = partition(my_data->arr, my_data->low, my_data->high);

  // At this point, we will create threads for the
  // left and right sides.  Must create their data args.
  struct thread_data thread_data_array[2];

  for (t = 0; t < 2; t++)
  {
    thread_data_array[t].arr = my_data->arr;
    thread_data_array[t].level = my_data->level - 1;
  }
  thread_data_array[0].low = my_data->low;
  thread_data_array[0].high = mid - 1;
  thread_data_array[1].low = mid + 1;
  thread_data_array[1].high = my_data->high;

  // Now, instantiate the threads.
  // In quicksort of course, due to the transitive property,
  // no elements in the left and right sides of mid will have
  // to be compared again.
  pthread_t threads[2];
  for (t = 0; t < 2; t++)
  {
    rc = pthread_create(&threads[t], NULL, parallelQuicksortHelper,(void *)&thread_data_array[t]);
    if (rc)
    {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  // Now, join the left and right sides to finish.
  for (t = 0; t < 2; t++)
  {
    rc = pthread_join(threads[t], &status);
    if (rc)
    {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }
  pthread_exit(NULL);
}

void readInts(const char *file_name, int *arr)
{
  FILE *file = fopen(file_name, "r");
  int i = 0;
  while (!feof(file))
  {
    int ret = fscanf(file, "%d\n", &arr[i]);
    i++;
  }
  fclose(file);
}

int cmpfunc(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

int validateSort(int *arr1, int *arr2, int n, int verbose)
{
  int difs = 0;
  for (int i = 0; i < n; i++)
  {
    if (arr1[i] != arr2[i])
    {
      difs++;
      if (verbose)
        printf("Different elements at index %d \n", i);
    }
  }
  return difs;
}

void parallelQuicksort(int lyst[], int size, int tlevel)
{
  int rc;
  void *status;

  // pthread function can take only one argument, so struct.
  struct thread_data td;
  td.arr = lyst;
  td.low = 0;
  td.high = size - 1;
  td.level = tlevel;

  // The top-level thread.
  pthread_t theThread;
  rc = pthread_create(&theThread, NULL, parallelQuicksortHelper,(void *)&td);
  if (rc)
  {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }

  // Now join the thread (wait, as joining blocks) and quit.
  rc = pthread_join(theThread, &status);
  if (rc)
  {
    printf("ERROR; return code from pthread_join() is %d\n", rc);
    exit(-1);
  }
}

int main()
{
  int n = LENGTH;

  int *arr = malloc(n * sizeof(int));
  readInts(filename, arr);
  int *valArr = malloc(n * sizeof(int));
  readInts(filename, valArr);

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  /*code to time here */
  
  parallelQuicksort(arr, n, 2);

  clock_gettime(CLOCK_REALTIME, &end);
  long unsigned exeTime = 1000000000 * (end.tv_sec - start.tv_sec);
  exeTime += end.tv_nsec - start.tv_nsec;
  printf("The pthreads implementation of quicksort has execution time: %f ms\n", (float)exeTime / 1000000);

  // Validate
  qsort(valArr, n, sizeof(int), cmpfunc);
  int difs = validateSort(arr, valArr, n, 0);
  printf("Errors: %d \n", difs);
  return 0;
}