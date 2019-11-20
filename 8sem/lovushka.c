#include <dirent.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* Search(char* target, DIR* here, char* current){
  struct dirent* gh;
  struct stat buf;
  char* name = (char*)calloc(17, sizeof(char));
  while((gh = readdir(here)) != NULL){
    //printf("HERE %s\n", argv[1]);
    strcpy(name, current);
    strcat(name, "/");
    strcat(name ,gh->d_name);
    //printf("%s\n", name);
    stat(name, &buf);
    if(S_ISREG(buf.st_mode) == 0 && strcmp(gh->d_name, ".") != 0 && strcmp(gh->d_name, "..") != 0){
      return Search(target, here, name);
    }
    if(strcmp(target, gh->d_name) == 0){
      //printf("HERE\n");
      return name;
    }
    //printf("Type is %d; Size is %ld; Name is %s\n", S_ISREG(buf.st_mode), buf.st_size, gh->d_name);
  }
}

int main(int argc, char* argv[]){
  DIR* here = opendir(".");
  char* distribute = (char*)calloc(17, sizeof(char));
  distribute = Search(argv[1], here, ".");
  printf("%s\n", distribute);
  closedir(here);
  return 0;
}
