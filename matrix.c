//compile with -O2
// N = 10000 everything breaks down

// N = 5000 and 3000 and 1500 programm works more then 4 minutes

/*  N = 1000
    if we work without threads
      real 18,87   16,81   16,84
      user 15,31   13,60   13,05
      sys 0,20     0,20    0,20

    if we work with threads
      real 16,77   19,68   18,98
      user 12,88   15,49   14,74
      sys 0,70     0,75    0,68

      Why so differents results? I worked on virtual box
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct DataMatrix{
  int** a;
  int** b;
  int** c;
  int string;
  int size;
}DataMatrix;

int** CreateMatrix(const int* size){
  int** a = (int**)calloc(*size, sizeof(int*));
  for(int i = 0; i < *size; i++){
    a[i] = (int*)calloc(*size, sizeof(int));
  }
  return a;
}

void FillMatrix(const int* size, int** a){
  for(int i = 0; i < *size; i++){
    for(int j = 0; j < *size; j++){
      a[i][j] = 1;
    }
  }
}

int** MulMatrix(const int* size, int** a, int** b){ //if I write: const int*** a, programm will not compilling. Why?
  int** c = CreateMatrix(size);
  int sum = 0;
  for(int i = 0; i < *size; i++){
    for(int j = 0; j < *size; j++){
      for(int k = 0; k < *size; k++){
        sum += (a[i][k]) * (b[k][j]);
      }
      c[i][j] = sum;
      sum = 0;
    }
  }
  return c;
}

void* mythread(void *dummy)
{
  DataMatrix* gh = (DataMatrix*)dummy;
  int** a = gh->a;
  int** b = gh->b;
  int** c = gh->c;
  int string = gh->string;
  int N = gh->size;
  //printf("SIZE %d\n", N);
  //printf("STRING %d\n", string);
  int sum = 0;
  for(int j = 0; j < N; j++){
    for(int k = 0; k < N; k++){
      sum += (a[string][k]) * (b[k][j]);
    }
    c[string][j] = sum;
    sum = 0;
  }
  //printf("JOKER\n");
  gh->string++;
  return NULL;
}

void PrintMatrix(const int* size, int** a){
  for(int i = 0; i < *size; i++){
    for(int j = 0; j < *size; j++){
      printf("%d ", a[i][j]);
    }
    printf("\n");
  }
}

void DestructMatrix(const int* size, int** a){
  for(int i = 0; i < *size; i++){
    free(a[i]);
  }
  free(a);
}

int MatrixWithThread(DataMatrix* gh){
  int N = gh->size;
  int result = 0;
  for(int i = 0; i < N; i++){
    pthread_t thid;
    //printf("ISD %d\n", gh->string);
    result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, gh);
    if(result != 0){
      printf ("Error on thread create, return value = %d\n", result);
      return -1;
    }
    pthread_join(thid, (void **)NULL);
  }
}

int main(int argc, char* argv[], char* envp[]){
  int N = 1000;
  int result;
  char* n0 = "0";
  char* n1 = "1";

  int** a = CreateMatrix(&N);
  int** b = CreateMatrix(&N);
  FillMatrix(&N, a);
  FillMatrix(&N, b);
  int** c = CreateMatrix(&N);

  if(argc < 2){
    printf("Joker's trap\n");
    return -1;
  }
  if(strcmp(argv[1],n0) == 0){
    DataMatrix gh = {a, b, c, 0, N};
    MatrixWithThread(&gh);
    PrintMatrix(&N, c);
  }else if(strcmp(argv[1],n1) == 0){
    c = MulMatrix(&N, a, b);
    PrintMatrix(&N, c);
  }else{
    printf("Joker's trap\n");
  }

  DestructMatrix(&N, a);
  DestructMatrix(&N, b);
  DestructMatrix(&N, c);
  return 0;
}
