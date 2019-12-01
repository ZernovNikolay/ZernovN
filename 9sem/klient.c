/* Простой пример UDP клиента для сервиса echo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* mythread(void *dummy)
{
  //printf("HERE1\n");
  int* sockfd = (int*)dummy;
  char recvline[100];
  int n;
  memset(recvline, 0, 100);
  pthread_t mythid;
  mythid = pthread_self();
  while(1){
    if((n = recvfrom(*sockfd, recvline, 1000, 0,
      (struct sockaddr *) NULL, NULL)) < 0){
      perror(NULL);
      close(*sockfd);
      exit(1);
    }
    printf("%s\n", recvline);
  }
  return NULL;
}

int main(int argc, char **argv){
  int sockfd;
  pthread_t thid;
 /* Дескриптор сокета */
  int len;
 /* Переменные для различных длин и количества символов */
 char* name = (char*)calloc(100, sizeof(char));
 char* sendline = (char*)calloc(1000, sizeof(char));
 /* Массивы для отсылаемой и принятой строки */
 struct sockaddr_in servaddr, cliaddr;
/* Структуры для адресов сервера и клиента */
/* Сначала проверяем наличие второго аргумента вкомандной строке. При его отсутствии ругаемся ипрекращаем работу */
if(argc != 2){
  printf("Usage: a.out <IP address>\n");
  exit(1);
}/* Создаем UDP сокет */
if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
  perror(NULL);
   /* Печатаем сообщение об ошибке */
   exit(1);
}/* Заполняем структуру для адреса клиента: семействопротоколов TCP/IP, сетевой интерфейс – любой, номерпорта по усмотрению операционной системы. Посколькув структуре содержится дополнительное не нужное намполе, которое должно быть нулевым, перед заполнениемобнуляем ее всю */
bzero(&cliaddr, sizeof(cliaddr));
cliaddr.sin_family = AF_INET;
cliaddr.sin_port = htons(0);
cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 /* Настраиваем адрес сокета */
if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
   perror(NULL);close(sockfd);
   /* По окончании работы закрываемдескриптор сокета */
   exit(1);
}
/* Заполняем структуру для адреса сервера:
семейство протоколов TCP/IP, сетевой интерфейс –из аргумента командной строки,
номер порта 7.Поскольку в структуре содержится дополнительноене нужное нам поле,
 которое должно быть нулевым,перед заполнением обнуляем ее всю*/
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(51000);
 if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
   printf("Invalid IP address\n");
   close(sockfd);
   /* По окончании работы закрываем дескриптор сокета */
   exit(1);
 }/* Вводим строку, которую отошлем серверу */
 //printf("String => ");
 //fgets(sendline, 1000, stdin);
 //printf("HUI %s\n", sendline);
 //printf("FD %d\n", sockfd);
 /* Отсылаем датаграмму */


 strcpy(sendline, "I am new klient\n");
 if(sendto(sockfd, sendline, strlen(sendline)+1, 0,
 (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
   perror(NULL);
   close(sockfd);
   exit(1);
 }

 if(recvfrom(sockfd, sendline, 1000, 0,
   (struct sockaddr *) NULL, NULL) < 0){
   perror(NULL);
   close(sockfd);
   exit(1);
 }
 printf("%s\n", sendline);

 printf("Please, enter the name of BOSS OF THIS GYM\n");

 fgets(name, 1000, stdin);

 if(sendto(sockfd, name, strlen(sendline)+1, 0,
 (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
   perror(NULL);
   close(sockfd);
   exit(1);
 }

//printf("HERE2\n");
int result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, &sockfd);
 if(result != 0){
   printf ("Error on thread create, return value = %d\n", result);
   return -1;
 }
//printf("HERE2\n");
char* Nikname = (char*)calloc(100, sizeof(char));
strncpy(Nikname, name, strlen(name)-1);
strcat(Nikname, ": ");
 while(1){
   memset(sendline, 0, 100);
   char* message = (char*)calloc(1101, sizeof(char));
   fgets(sendline, 1000, stdin);
   printf("\n");
   /*printf("NAME\n");
   printf("%s", name);
   memset(recvline, 0, 100);
   printf("Nikname\n");
   printf("%s", Nikname);*/
   strcpy(name, Nikname);
   //printf("AGAIN\n");
   //printf("NAME\n");
   //printf("%s", name);
   /*printf("Nikname\n");
   printf("%s", Nikname);
   printf("SENDLINE\n");
   printf("%s", sendline);*/
   strcpy(message, name);
   strcat(message, sendline);
   if(sendto(sockfd, message, strlen(message)+1, 0,
   (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
     perror(NULL);
     close(sockfd);
     exit(1);
   }
   /*printf("STRING\n");
   printf("%s", message);*/
   free(message);
 }
 pthread_join(thid, (void **)NULL);
 close(sockfd);
 return 0;
}
