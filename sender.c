#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 

#include<string.h>
#include <time.h>

#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "172.17.17.23"
#define SIZE ((1024*1024)-1)

void send_file(FILE *fp, int sockfd){
  int n;
  char buffer[SIZE] = {0};

  while(fgets(buffer, SIZE, fp) != NULL) {
    n=send(sockfd, buffer, sizeof(buffer), 0);
    if ( n < 0 ) {
      printf("Error in sending file. ");
      exit(0);
    }
   
   bzero(buffer, SIZE);
  }
  printf("sent %d bytes\n",n);
}

int main(){
  int sockfd;
  struct sockaddr_in serv_addr;
  char buffer[SIZE];
  FILE *fp;
  char *filename = "1mb.txt";

 for (size_t i = 0; i < 10; i++)
  {  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("Error in socket ");
        exit(0);
    }
    printf("Server socket created successfully.\n");

      if (i>4)
    {
      strcpy(buffer, "reno"); 
      socklen_t  len = strlen(buffer);
      if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buffer, len) != 0) {
        perror("setsockopt"); 
        return -1;
      }
    }

    serv_addr.sin_family= AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

       //connect
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed! ");
    }

  printf("connected to server\n");

    //send 
    fp = fopen(filename, "r");
    if (fp == NULL) {
         printf("Error in reading file.");
         exit(0);
    }
  
    send_file(fp, sockfd);
    

    printf("File sent successfully.\n");
    close(sockfd);

  }

    return 0;
}
