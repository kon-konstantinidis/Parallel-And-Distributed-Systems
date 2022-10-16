#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/cilk_c11_threads.h>


#define LENGTH 10000000
#define filename "randomArray10000000.txt"


void quickSort(int arr[], int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2]; 
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      /* recursion  */
	  if (left < j)
            cilk_spawn quickSort(arr, left, j);
      if (i < right)
            cilk_spawn quickSort(arr, i, right);
}

void readInts (const char* file_name, int * arr)
{
  FILE* file = fopen (file_name, "r");
  int i = 0;
  while (!feof (file))
    {  
      int ret = fscanf (file, "%d\n", &arr[i]);
      i++;
    }
  fclose (file);        
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int validateSort(int * arr1, int * arr2, int n, int verbose){
  int difs = 0;
  for(int i=0; i<n; i++){
    if (arr1[i] != arr2[i]) {
      difs++;
      if (verbose) printf("Different elements at index %d \n",i);
  }
  }
  return difs;
}


int main() {
  int n = LENGTH;
  int * arr = malloc(n*sizeof(int));
  readInts(filename,arr);
  int * valArr = malloc(n*sizeof(int));
  readInts(filename,valArr);

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME,&start);

  /*code to time here */
  cilk_spawn quickSort(arr, 0, n - 1);
  cilk_sync;

  clock_gettime(CLOCK_REALTIME,&end);\
  long unsigned exeTime = 1000000000*(end.tv_sec - start.tv_sec);
  exeTime += end.tv_nsec - start.tv_nsec;
  printf("The multiCilk implementation of quicksort has execution time: %f ms\n",(float)exeTime/1000000);

  // Validate
  qsort(valArr,n,sizeof(int),cmpfunc);
  int difs = validateSort(arr,valArr,n,0);
  printf("Errors: %d \n",difs);
  return 0;
}