#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 10000000
#define filename "randomArray10000000.txt"



void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
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
  quickSort(arr, 0, n - 1);

  clock_gettime(CLOCK_REALTIME,&end);\
  long unsigned exeTime = 1000000000*(end.tv_sec - start.tv_sec);
  exeTime += end.tv_nsec - start.tv_nsec;
  printf("The serial implementation of quicksort has execution time: %f ms\n",(float)exeTime/1000000);

  // Validate
  qsort(valArr,n,sizeof(int),cmpfunc);
  int difs = validateSort(arr,valArr,n,0);
  printf("Errors: %d \n",difs);
  return 0;
}