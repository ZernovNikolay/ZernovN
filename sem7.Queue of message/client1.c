#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 /* Тип сообщения для прекращения работы программы 2 */

int main(){
  int msqid; /* IPC дескриптор для очереди сообщений */
  long client = 1;
  char pathname[] = "08-1a.c"; /* Имя файла, использующееся для генерации ключа.
  Файл с таким именем должен существовать в текущей директории */

  key_t key; /* IPC ключ */

  int i,len; /* Cчетчик цикла и длина информативной части сообщения */
  int maxlen = 81;
  /* Ниже следует пользовательская структура для сообщения */

  /*typedef struct info{
    short sinfo;
    float finfo;
  } info;*/

  struct mymsgbuf
  {
    long mtype;
    int mtext;
  } mybuf;

  /* Генерируем IPC ключ из имени файла 09-1a.c в текущей директории
  и номера экземпляра очереди сообщений 0. */

  if((key = ftok(pathname,0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
  }

  /* Пытаемся получить доступ по ключу к очереди сообщений, если она существует,
  или создать ее, если она еще не существует, с правами доступа
  read & write для всех пользователей */

  if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
  }

mybuf.mtype = 1;

  /* Посылаем в цикле 5 сообщений с типом 1 в очередь сообщений, идентифицируемую msqid.*/
  while(1){
    /* Сначала заполняем структуру для нашего сообщения и определяем длину информативной части */
    len = sizeof(mybuf);
    if(mybuf.mtype == 1){
      mybuf.mtext = -1;
      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }

      if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0) < 0)){
        printf("Can\'t receive message from queue\n");
        exit(-1);
      }

      mybuf.mtype = mybuf.mtext;
    }else{
      int N;
      scanf("%d", &N);
      mybuf.mtext = N;
      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }

      if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0) < 0)){
        printf("Can\'t receive message from queue\n");
        exit(-1);
      }

        printf("FACTORIAL %d\n", mybuf.mtext);
      }
    }

  mybuf.mtype = LAST_MESSAGE;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}
