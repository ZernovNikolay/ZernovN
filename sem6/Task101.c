#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  char pathname[] = "06-1a.c";
  //int new = 1;
  int shmid;
  key_t key;
  char* buff = (char*)calloc(100, sizeof(char));
  char name[] = "3.c";
  FILE *fd;

	if((fd = fopen(name, "r")) == NULL){
    printf("LOX\n");
    exit(-1);
  };

  fseek (fd, 0L, SEEK_END);
  int fileLength = ftell (fd);
  fseek (fd, 0L, SEEK_SET);
  char* originaltext = (char*) calloc (fileLength+1, sizeof (char));

  if((key = ftok(pathname,0)) < 0){
  printf("Can\'t generate key\n");
  exit(-1);
  }

  if((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){
    if(errno != EEXIST){
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if((shmid = shmget(key, 3*sizeof(int), 0)) < 0){
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      //new = 0;
    }
  }

  if((originaltext = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
  printf("Can't attach shared memory\n");
  exit(-1);
  }

  fread (originaltext, fileLength, 1, fd);
  //printf("\n%ld\n", k);

  if(shmdt(originaltext) < 0){
  printf("Can't detach shared memory\n");
  exit(-1);
  }

  fclose(fd);

  return 0;
}
