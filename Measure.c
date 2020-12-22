#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <time.h>
#include<string.h>
#include <errno.h>

#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "172.17.17.23"
#define SIZE ((1024*1024)-1)
int count=1;

void write_file(int sockfd){
  clock_t time = clock();
  int n;
  char buffer[SIZE];

  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n == SIZE){
     printf("Recived file #%d\n",count++);
      break;
    }
    if (SIZE > n)
		{
		   printf("sent only %d bytes from the required %d.\n",n, SIZE);
		}
  	if (n==-1)
		{
			printf("send() failed with error code : %d" ,errno);
		}
    if (0 == n)
     {
	      printf("peer has closed the TCP connection prior to send().\n");
        break;
     }

    bzero(buffer, SIZE);
    
  }
    time = clock()-time;
    double time_taken = ((double)time)/CLOCKS_PER_SEC; 
    printf("File sent successfully. time: %lf\n",time_taken);
  return;
}

int main(){
    char buffer[SIZE] = {0};
    int sock, client_socket;
    struct sockaddr_in serv_addr, clientAddress;
    socklen_t clientAddressLen;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Error in socket");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        printf("Error in bind");
        exit(0);
    }
    listen(sock, 50);

    clientAddressLen = sizeof(clientAddress);
  while(1)
    {
      memset(&clientAddress, 0, sizeof(clientAddress));
      clientAddressLen = sizeof(clientAddress);    
      client_socket = accept(sock, (struct sockaddr*)&clientAddress, &clientAddressLen);
       if(client_socket == -1 ){
         printf("end\n");
         break;
         }
      write_file(client_socket);

      printf("load finish\n");
    }

    close(sock);
    
    return 0;
}
