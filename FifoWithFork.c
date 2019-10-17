#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[], char* envp[]){
  int fd1, fd2, result;
  size_t size;
  char resstring[16];
  char name1[] = "aaa.fifo";
  char name2[] = "bbb.fifo";
  char* n0 = "0";
  char* n1 = "1";
  (void)umask(0);
  if(mknod(name1, S_IFIFO|0666, 0) < 0){
      printf("ERROR OF FIFO\n");
      return -1;
  }
  if(mknod(name2, S_IFIFO|0666, 0) < 0){
    printf("ERROR OF FIFO\n");
    return -1;
  }
  if(result = fork()< 0){
    printf("ERROR OF FORK\n");
    return -1;
  }
  if(result > 0){
    if(strcmp(argv[1], n0) == 0){
      char buff[100];
      memset(buff, 0, 100);
      while(1){
        int g = WriteFifo(buff, name1);
        if(g < 0){
          printf("ERROR OF OPEN FIFO %s\n", name1);
          return -1;
        }
      }
    }else if(strcmp(argv[1],n1) == 0){
      char buff[100];
      memset(buff, 0, 100);
      while(1){
        int g = WriteFifo(buff, name2);
        if(g < 0){
          printf("ERROR OF OPEN FIFO %s\n", name2);
          return -1;
        }
      }
    }
  }else{
    if(strcmp(argv[1],n0) == 0){
      char buff[100];
      memset(buff, 0, 100);
      while(1){
        int g = ReadFifo(buff, name2);
        if(g < 0){
          printf("ERROR OF OPEN FIFO %s\n", name2);
          return -1;
        }
      }
    }else if(strcmp(argv[1],n1) == 0){
      char buff[100];
      memset(buff, 0, 100);
      while(1){
        int g = ReadFifo(buff, name1);
        if(g < 0){
          printf("ERROR OF OPEN FIFO %s\n", name1);
          return -1;
        }
      }
    }
  }
  return 0;
}
