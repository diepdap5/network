// udp client driver program 
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include<string.h>
#define MAXLINE 1000 

// Driver code 
int main(int argc, char *argv[]) 
{
	// catch wrong input
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
	// ip_address : get ip from argv
	// port : get port from argv
	// buffer : data get from server
	// message: data send to server
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);

	int sockfd, n; 
	struct sockaddr_in servaddr; 
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr(ip_address); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 
		
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	} 

	do {
		int g= 0;
		char buffer[100];
		char message[100];
		// get message
		printf("Input data: ");
		g = scanf("%[^\n]", message); 
		if (g == 0) break;
		getchar();
		// request to send datagram 
		// no need to specify server address in sendto 
		// connect stores the peers IP and port 
		sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	
		// waiting for response 
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
		puts(buffer); 
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
		puts(buffer);
		//while ((getchar()) != '\n');
		puts("-------------------------");
	}while(1);	
	// close the descriptor 
	close(sockfd); 
	return 0;
}
