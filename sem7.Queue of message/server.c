#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 /* Тип сообщения для прекращения работы */

int main()
{
  int msqid; /* IPC дескриптор для очереди сообщений */
  char pathname[] = "08-1a.c"; /* Имя файла, использующееся для генерации ключа.
  Файл с таким именем должен существовать в текущей директории */
  int NumberOfClient = 2;
  key_t key; /* IPC ключ */

  int len, maxlen; /* Реальная длина и максимальная длина информативной части сообщения */

  struct mymsgbuf
  {
    long mtype;
    int mtext;
  } mybuf;

  if((key = ftok(pathname,0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  while(1){

  /* В бесконечном цикле принимаем сообщения любого типа в порядке FIFO с максимальной длиной информативной части 81 символ до тех пор, пока не поступит сообщение с типом LAST_MESSAGE*/

  maxlen = 81;

    if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0) < 0)){
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    /* Если принятое сообщение имеет тип LAST_MESSAGE, прекращаем работу и удаляем очередь сообщений из системы.
    В противном случае печатаем текст принятого сообщения. */

    if (mybuf.mtype == LAST_MESSAGE){
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(0);
    }
    /*printf("%d\n", mybuf.mtext);
    printf("TYPE %ld\n", mybuf.mtype);*/
    if(mybuf.mtype == 1){
      mybuf.mtext = NumberOfClient;
      NumberOfClient++;
      len = sizeof(mybuf);
      printf("NEW CLIENT WITH ID %d\n", mybuf.mtext);
    }else{
      int sum = 1;
      for(int i = 1; i< mybuf.mtext+1; i++){
        sum = i* sum;
      }
      mybuf.mtext = sum;
      len = sizeof(mybuf);
    }

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    //printf("message type = %ld, sinfo = %hi, finfo = %f\n", mybuf.mtype, mybuf.mtext.sinfo, mybuf.mtext.finfo);
  }

  return 0; /* Исключительно для отсутствия warning'ов при компиляции. */
}
