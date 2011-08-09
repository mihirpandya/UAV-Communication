/*
 *
mypthread.c acts as a server with 2 independent threads dedicated to sending and receiving alone. mypthread.c should accept connection from mypthread2.c (client) following which there should be communication possible between the 2.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define PORT_NUMBER 5576
#define BACKLOG 5
#define IP_ADDR "127.0.0.1"

char send_buf[128];
char recv_buf[128];

int sockfd, newfd;

void *send_func( void *arg )
{
	while(1) {
	fgets(send_buf, sizeof(send_buf), stdin);
	send(newfd, send_buf, sizeof(send_buf), 0);
	}
}

void *recv_func( void *arg )
{
//	char recvMessage;
//float testArray[3];

	while(1) {
	if(recv(newfd, recv_buf, sizeof(recv_buf), 0) == 0) {
		printf("Connection terminated.\n");
		exit(EXIT_FAILURE);
		}
	fputs(recv_buf, stdout);
//printf("%f",testArray[0]);
	}
//	printf("%i\n", recvMessage);
}

int main (int argc, char *argv[])
{
	struct sockaddr_in serverTest;
	memset(&serverTest, 0, sizeof(serverTest));
	serverTest.sin_family = AF_INET;
	serverTest.sin_port = htons(PORT_NUMBER);
	serverTest.sin_addr.s_addr = inet_addr(IP_ADDR);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		printf("Failed to create socket.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Successfully created socket\n");
	
	int yes = 1;
	printf("Setting socket properties...\n");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		printf("Failed to set socket properties.\n");
		exit(EXIT_FAILURE);
	}
	
	if(bind(sockfd, (struct sockaddr *)&serverTest, sizeof(serverTest)) == -1) {
		printf("Failed to bind socket.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Successfully bound socket\n");
	printf("Listening for connections...\n");
	
	if(listen(sockfd, BACKLOG) == -1) {
		printf("Could not listen for connections.\n");
		exit(EXIT_FAILURE);
	}
	
		socklen_t sin_size;
		sin_size = sizeof(serverTest);
	
	newfd = accept(sockfd, (struct sockaddr *)&serverTest, &sin_size);
		if(newfd == -1) {
		printf("Failed to accept connection.\n");
		exit(EXIT_FAILURE);
		}

//	while(1) {
		
	int ret;
	pthread_t sendThread, recvThread;
	ret = pthread_create(&sendThread, NULL, send_func, NULL);
	
 	if(ret != 0) {
		printf("Failed to create sendThread\n");
		exit(EXIT_FAILURE);
		}
		
	ret = pthread_create(&recvThread, NULL, recv_func, NULL);
	
	if(ret != 0) {
		printf("Failed to create recvThread\n");
		exit(EXIT_FAILURE);
		}
	
	if(pthread_join(recvThread, NULL) != 0) {
		printf("Failed to join recvThread\n");
		exit(EXIT_FAILURE);
		}
			
//	}
return 0;
}
