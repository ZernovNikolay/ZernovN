#include <dirent.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  DIR* here = opendir(argv[1]);
  struct dirent* gh;
  struct stat buf;
  char* name = (char*)calloc(1, sizeof(char));
  while((gh = readdir(here)) != NULL){
    //printf("HERE %s\n", argv[1]);
    strcpy(name, "../");
    strcat(name ,gh->d_name);
    //printf("%s\n", name);
    stat(name, &buf);
    printf("Type is %d; Size is %ld; Name is %s\n", S_ISREG(buf.st_mode), buf.st_size, gh->d_name);
  }
  closedir(here);
  return 0;
}
