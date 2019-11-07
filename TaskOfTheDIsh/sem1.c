#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

const int TABLE_LIMIT = 5;
char* NAME_FILE_TIME_WASH = "textWash.txt";//c const не работает
char* NAME_FILE_TIME_DRY = "textDry.txt";
char* NAME_FILE_DISHES = "textCommand.txt";

struct Data{
    int semid;
    struct sembuf mybuf[2];
    char* name;
    char* inputTime;
    char* inputCommand;
};

void Split(char *str, char *delimiters, char** words, int* count){
    words[*count] = strtok(str, delimiters);
    while(words[*count] != NULL){
	     //printf("%s\n", words[*count]);
        words[++(*count)] = strtok(NULL, delimiters);
    }
    return;
}

void GetData(char** dishes, int* k, int* times, char* name, char* buff){

  FILE* text = fopen(name, "r");
  char** words = (char**)calloc(2, sizeof(char*));

  while(fgets(buff, 1000, text) != 0){
    buff[strlen(buff)-1] = '\0';
    //printf("%s\n", buff);
    char delimiters = ':';
    int count = 0;
    Split(buff, &delimiters, words, &count);
    strcpy(dishes[*k], words[0]);
    times[*k] = atoi(words[1]);
    //printf("!!! %s %d &&& %s\n", dishes[*k], times[*k], dishes[2]);
    //printf("K IS %d\n", *k);
    //printf("%s:%s\n", words[0], words[1]);
    //printf("%s %d\n", dishes[*k], *times[*k]);
    *k = *k + 1;
    //printf("K1 IS %d\n", *k);
  }

  fclose(text);
  free(words);
  return;
}

void GetDish(char** Dish_Command, int* countCommand, int* counts, char* name, char* buff){
  FILE* text = fopen(name, "r");
  char** words = (char**)calloc(2, sizeof(char*));

  while(fgets(buff, 1000, text) != 0){
    buff[strlen(buff)-1] = '\0';
    //printf("%s\n", buff);
    char delimiters = ':';
    int count = 0;
    Split(buff, &delimiters, words, &count);
    strcpy(Dish_Command[*countCommand], words[0]);
    counts[*countCommand] = atoi(words[1]);
    //printf("%s :  %s\n", words[0], words[1]);
    *countCommand = *countCommand + 1;
  }

  fclose(text);
  free(words);
  return;
}

int ReadFifo(char* buff, char* name){
  int fd = open(name, O_RDONLY);
  if(fd < 0){
    return -1;
  }
  //printf("1\n");
  read(fd, buff, 100);
  //printf("2\n");
  //printf("%s", buff);
  //memset(buff, 0, 100);
  close(fd);
  return 0;
}

int WriteFifo(char* buff, char* name){
  //printf("%s\n", name);
  //printf("%s\n", buff);
  int fd = open("ccc.fifo", O_RDWR);//O_WRONLY не работает
  if(fd < 0){
    return -1;
  }
  //printf("%s\n", name);
  //fgets(buff, 100, stdin);//при включенном не сработает
  write(fd, buff, 100);
  close(fd);
  //printf("1%s\n", name);
  return 0;
}

void *mythread1(void *dummy){
    struct Data newData = *(struct Data*)dummy;

    int maxType = 10;
    char** dishes = (char**)calloc(maxType, sizeof(char*));
    for(int i = 0; i < maxType; i++){
      dishes[i] = (char*)calloc(30, sizeof(char));
    }
    int countType = 0;
    int* times = (int*)calloc(10, sizeof(int));
    char* buff = (char*)calloc(100, sizeof(char));

    GetData(dishes, &countType, times, newData.inputTime, buff);
    /*for(int i = 0; i < countType; i++){
      printf("&&&&& %s %d\n", dishes[i], times[i]);
    }*/
    int maxCommand = 10;
    char** Dish_Command = (char**)calloc(maxCommand, sizeof(char*));
    for(int i = 0; i < maxCommand; i++){
      Dish_Command[i] = (char*)calloc(30, sizeof(char));
    }
    int countCommand = 0;
    int* counts = (int*)calloc(10, sizeof(int));
    GetDish(Dish_Command, &countCommand, counts, newData.inputCommand, buff);
    //printf("%d COUNT\n", countCommand);
    for(int i = 0; i < TABLE_LIMIT; i++){
      semop(newData.semid, &newData.mybuf[0], 1);
    }
    for(int i = 0; i < countCommand; i++){
      //printf("1HERE7\n");
      int number = counts[i];
      //printf("NUMBER %d\n", number);
      int timeOperation = 0;
      for(int j = 0; j < countType; j++){
        if(strcmp(dishes[j], Dish_Command[i]) == 0){
          timeOperation = times[j];
          //printf("J IS %d\n", j);
          break;
        }
      }
      //printf("%s%s1\n", dishes[0], Dish_Command[0]);

      //printf("TIME IS %d\n", timeOperation);

      for(int b = number; b > 0; b--){
        //D(s1, -1);
        //printf("1HERE8\n");
        newData.mybuf[0].sem_op = -1;
        semop(newData.semid, &newData.mybuf[0], 1);

        printf("Thread 1, dry %s with %d time, count is %d\n", Dish_Command[i], timeOperation, b);
        //sleep(timeOperation);
        WriteFifo(Dish_Command[i], newData.name);

        //A(s2, 1);
        //printf("1HERE9\n");

        newData.mybuf[1].sem_op = 1;
        semop(newData.semid, &newData.mybuf[1], 1);
        //printf("1HERE67\n");
      }
      //printf("AGAIN1\n");
    }
    strcpy(buff, "END");
    //printf("BUFF IS %s\n", buff);
    WriteFifo(buff, newData.name);

    for(int i = 0; i < maxType; i++){
      free(dishes[i]);
    }
    free(dishes);
    for(int i = 0; i < maxCommand; i++){
      free(Dish_Command[i]);
    }
    free(Dish_Command);
    free(times);
    free(counts);
    free(buff);
    printf("OK1!\n");
    return NULL;
}

void *mythread2(void *dummy){
    struct Data newData = *(struct Data*)dummy;
    sleep(10);
    int maxType = 10;
    char** dishes2 = (char**)calloc(maxType, sizeof(char*));
    for(int i = 0; i < maxType; i++){
      dishes2[i] = (char*)calloc(30, sizeof(char));
    }
    int countType = 0;
    int* times = (int*)calloc(10, sizeof(int));
    char* buff = (char*)calloc(100, sizeof(char));

    GetData(dishes2, &countType, times, newData.inputTime, buff);

    while(1){
      //D(s2, -1);
      //printf("2HERE89\n");
      newData.mybuf[1].sem_op = -1;
      semop(newData.semid, &newData.mybuf[1], 1);
      //printf("2HERE90\n");

      //A(s1, 1);

      newData.mybuf[0].sem_op = 1;
      semop(newData.semid, &newData.mybuf[0], 1);
      //printf("2HERE93\n");

      int g = ReadFifo(buff, newData.name);
      printf("TOOK FROM THE TABLE  %s\n", buff);
      if(strcmp("END", buff) == 0){
        printf("END\n");
        break;
      }

      if(g < 0){
        exit(-1);
      }
      int timeOperation = 0;
      //printf("2HERE94\n");
      for(int i = 0; i < countType; i++){
        if(strcmp(buff, dishes2[i]) == 0){
          timeOperation = times[i];
          break;
        }
      }
      printf("Thread 2, dry %s with %d time\n", buff, timeOperation);
      sleep(timeOperation);
      //printf("AGAIN2\n");
    }

    free(buff);
    free(times);
    for(int i = 0; i < maxType; i++){
      free(dishes2[i]);
    }
    free(dishes2);
    printf("OK2!\n");
    return NULL;
}

int main(int argc, char **argv){
    char name[] = "ccc.fifo";
    int semid;
    struct sembuf mybuf[2];

    key_t key;
    char pathname[]="08-1a.c";
    if((key = ftok(pathname,0)) < 0){
      printf("Can\'t generate key\n");
      exit(-1);
    }

    semid = semget(key, 2, 0666 | IPC_CREAT);

    //0 - количество свободного места
    //1 - количество занятого места

    mybuf[0].sem_flg = 0;
    mybuf[0].sem_num = 0;
    mybuf[0].sem_op = 1;
    mybuf[1].sem_flg = 0;
    mybuf[1].sem_num = 1;
    mybuf[1].sem_op = 1;
    //semop(semid, &mybuf[1], -1);

    struct Data myData1 = {semid, {mybuf[0], mybuf[1]}, (char*)calloc(30, sizeof(char)),
    (char*)calloc(30, sizeof(char)), (char*)calloc(30, sizeof(char))};
    struct Data myData2 = {semid, {mybuf[0], mybuf[1]}, (char*)calloc(30, sizeof(char)),
    (char*)calloc(30, sizeof(char)), (char*)calloc(30, sizeof(char))};
    // понятия не имею как мне очистить потом в нитях эту память
    myData1.name = name;
    myData1.inputTime = NAME_FILE_TIME_WASH;
    myData1.inputCommand = NAME_FILE_DISHES;
    myData2.name = name;
    myData2.inputTime = NAME_FILE_TIME_DRY;

    (void)umask(0);
    mknod(name, S_IFIFO|0666, 0);
    pthread_t thid, newthid;
    pthread_create(&thid, (pthread_attr_t *)NULL, mythread1, &myData1);
    pthread_create(&newthid, (pthread_attr_t *)NULL, mythread2, &myData2);

    pthread_join(thid, (void **)NULL);
    pthread_join(newthid, (void **)NULL);
}
