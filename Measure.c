#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "172.17.17.23"
#define SIZE ((1024*1024)-1)
int count=1;

void write_file(int sockfd){
 // clock_t time = clock();
  int n;
  char buffer[SIZE];
  printf("Downloading...");

  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if(n <= 0){
      if (n == -1)
      {
        printf("Error reciving file...\n");
      }
      else
      {
         printf("Recived file #%d\n",count++);
      }
      break;
    }
    bzero(buffer, SIZE);
  }
    //time = clock()-time;
    //double time_taken = ((double)time)/CLOCKS_PER_SEC; 
 //   printf("File sent successfully. time: %lf\n",time_taken);
  return;
}

int main(){
    char buffer[SIZE] = {0};
    int sock, client_socket;
    struct sockaddr_in serv_addr, clientAddress;
    socklen_t clientAddressLen;
    int i=1;
    int Cubic,Reno;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Error in socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        printf("Error in bind\n");
        exit(0);
    }
    printf("Bind() success\n");
    listen(sock, 50);
    printf("Waiting for incoming TCP-connections...\n");

    clientAddressLen = sizeof(clientAddress);
    struct timeval t1, t2;
    double time1;
  while(1)
    {
      memset(&clientAddress, 0, sizeof(clientAddress));
      clientAddressLen = sizeof(clientAddress);    
      client_socket = accept(sock, (struct sockaddr*)&clientAddress, &clientAddressLen);
       if(client_socket == -1 ){
         printf("send() failed with error code : %d \n",errno);
         break;
         }
         
      gettimeofday(&t1, NULL);
      write_file(client_socket);
      gettimeofday(&t2, NULL);
      time1 = (t2.tv_sec - t1.tv_sec) * 1000.0;   
      time1 += (t2.tv_usec - t1.tv_usec) / 1000.0;   
      if(i<6)
        Cubic+=time1;
      else
        Reno+=Cubic-time1;  
            
        i++; 

      if(i==6)
        printf("cubic avg : %lf ms\n", (Cubic/5.0));
            
      if(i==11) {
          printf("Reno avg:  %lf ms \n", (Reno/5.0));
          break;
      }
        


    }

    close(sock);
    
    return 0;
}
