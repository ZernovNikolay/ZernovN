#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int WriteFifo(char* buff, char* name){
  int fd = open(name, O_WRONLY);
  if(fd < 0){
    return -1;
  }
  fgets(buff, 100, stdin);
  write(fd, buff, 100);
  memset(buff, 0, 100);
  close(fd);
  return 0;
}

int ReadFifo(char* buff, char* name){
  int fd = open(name, O_RDONLY);
  if(fd < 0){
    return -1;
  }
  read(fd, buff, 100);
  printf("%s", buff);
  memset(buff, 0, 100);
  close(fd);
  return 0;
}

void* mythread(void *dummy)
{
  char* name = (char*)dummy;
  char buff[100];
  memset(buff, 0, 100);
  pthread_t mythid;
  mythid = pthread_self();
  while(1){
    WriteFifo(buff, name);
  }
  return NULL;
}

int main(int argc, char* argv[], char* envp[]){
  int fd1, fd2, result;
  size_t size;
  char name1[] = "aaa.fifo";
  char name2[] = "bbb.fifo";
  char* n0 = "0";
  char* n1 = "1";
  char buff[100];
  (void)umask(0);
  mknod(name1, S_IFIFO|0666, 0);
  mknod(name2, S_IFIFO|0666, 0);
  pthread_t thid, mythid;
  if(strcmp(argv[1],n0) == 0){
    result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, name1);
    if(result != 0){
      printf ("Error on thread create, return value = %d\n", result);
      return -1;
    }

    while(1){
      int g = ReadFifo(buff, name2);
      if(g < 0){
        return -1;
      }
    }
    pthread_join(thid, (void **)NULL);
  }else if(strcmp(argv[1],n1) == 0){
    result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, name2);
    if(result != 0){
      printf ("Error on thread create, return value = %d\n", result);
      return -1;
    }

    while(1){
      int g = ReadFifo(buff, name1);
      if(g < 0){
        return -1;
      }
    }
    pthread_join(thid, (void **)NULL);
  }else{
    printf("JOKER\n");
  }
  return 0;
}
