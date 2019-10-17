//compile with -O2
// N = 10000 everything breaks down

// N = 5000 and 3000 and 1500 programm works more then 4 minutes

/*  N = 2000
    if we work with 2 threads. Work faster than witn 1 thread
      real 87,15
      user 171,86
      sys  0,20

    if we work with 1 thread
      real 133,57
      user 133,04
      sys  0,20

      I can do normal function, that create specified nomber of thread, bu now... only 1 or 2
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//don't have to make more than 4 threads. Rewrite the programm

typedef struct DataMatrix{
  int** a;
  int** b;
  int** c;
  int string;
  int size;
  int count;
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
  int count = gh->count;
  //printf("SIZE %d\n", N);
  //printf("STRING %d\n", string);
  int sum = 0;
  for(int i = string; i < N; i = i+count){
    for(int j = 0; j < N; j++){
      for(int k = 0; k < N; k++){
        sum += (a[i][k]) * (b[k][j]);
      }
      c[i][j] = sum;
      sum = 0;
    }
    //printf("ALL RIGHT %d\n", i);
  }
  //printf("JOKER\n");
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
  int result = 0;
  pthread_t thid1, thid2;
  DataMatrix beta = *gh;
  beta.string++;

  if(gh->count == 2){
    result = pthread_create( &thid1, (pthread_attr_t *)NULL, mythread, gh);
  //printf("ISD %d\n", gh->string);
  //gh->string++; Doesn't work, I don't know why
    result = pthread_create( &thid2, (pthread_attr_t *)NULL, mythread, &beta);
    pthread_join(thid1, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
  }else if(gh->count == 1){
    result = pthread_create( &thid1, (pthread_attr_t *)NULL, mythread, gh);
    pthread_join(thid1, (void **)NULL);
  }
}

int main(int argc, char* argv[], char* envp[]){
  int N = 5000;
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
  }else{
    int kl = atoi(argv[1]);
    DataMatrix gh = {a, b, c, 0, N, kl};
    MatrixWithThread(&gh);
  }

  DestructMatrix(&N, a);
  DestructMatrix(&N, b);
  DestructMatrix(&N, c);
  return 0;
}
