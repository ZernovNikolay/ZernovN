#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

unsigned int arrayIP[1000];
unsigned short arrayPort[1000];
int count = 0;

int main(){
  printf("HOST PIDOR\n");
  char** arrayName = (char**)calloc(100, sizeof(char*));
  for(int i = 0; i < 100; i++){
    arrayName[i] = (char*)calloc(100, sizeof(char));
  }
  int sockfd;
  int clilen, n;
  char line[1000];
  struct sockaddr_in servaddr, cliaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(51000);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
    perror(NULL);
    exit(1);
  }
  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  clilen = sizeof(cliaddr);
  while(1) {
    //clilen = sizeof(cliaddr);
    if((n = recvfrom(sockfd, line, 999, 0,
      (struct sockaddr *) &cliaddr, &clilen)) < 0){
      perror(NULL);
      close(sockfd);
      exit(1);
    }
    //printf("BEGIN\n");
    printf("%s", line);
    //printf("END\n");
    if(strcmp(line, "I am new klient\n") == 0){
      arrayIP[count] = cliaddr.sin_addr.s_addr;
      arrayPort[count] = cliaddr.sin_port;
      //count++;

      strcpy(line, "Welcome to the club, buddy!");
      if(sendto(sockfd, line, strlen(line)+1, 0, (struct sockaddr *) &cliaddr, clilen) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
      }

      if((n = recvfrom(sockfd, line, 999, 0,
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
      }
      int length = strlen(line);
      strncpy(arrayName[count], line, length-1);

      count++;

      strcpy(line, "We are a decent chat, but three hundred dollars is three hundred dollars\n");
      if(sendto(sockfd, line, strlen(line)+1, 0, (struct sockaddr *) &cliaddr, clilen) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
      }

      for(int i = 0; i < count; i++){
        printf("USER %d : NAME %s, IP %u, Port %hu\n", i, arrayName[i], arrayIP[i], arrayPort[i]);
      }
    }else{
      //printf("IP %u, Port %hu\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);
      unsigned short senderPort = cliaddr.sin_port;
      unsigned int senderIP = cliaddr.sin_addr.s_addr;
      for(int i = 0; i < count; i++){
        if((arrayPort[i] != senderPort) || (arrayIP[i] != senderIP)){
          //printf("Port %hu, ASS %hu\n", cliaddr.sin_port, arrayPort[i]);
          //printf("HA!");
          cliaddr.sin_addr.s_addr = arrayIP[i];
          cliaddr.sin_port = arrayPort[i];
          if(sendto(sockfd, line, strlen(line)+1, 0, (struct sockaddr *) &cliaddr, clilen) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
          }
        }
      }
      /*if(sendto(sockfd, line, strlen(line)+1, 0, (struct sockaddr *) &cliaddr, clilen) < 0){
        perror(NULL);
        printf("ERROR\n");
        close(sockfd);
        exit(1);
      }*/
    }
    printf("AGAIN\n");
  }
  return 0;
}
