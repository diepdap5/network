// server program for udp connection 
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#define MAXLINE 1000 
int split(char * buffer, char * only_number,char * only_string){
	
	// Only number in buffer converts to string only_number 
	strcpy(only_string,buffer);
	int k=0;
	strcpy(only_number,buffer);
	int j=0;

	// if number, copy to only_number
	// if character, copy to only_string
	int m = 0;
	for(int i=0; i<100 ;i++){
		char ch = only_number[i];
		if (ch == '\0') break;
		if(ch >= '0' && ch <= '9'){
    	    only_number[j] = ch;
    	   	j++;
   		}
   		else if ((ch >= 'a' && ch <= 'z')||(ch == ' ')){
   			only_string[k] = ch;
   			k++;
   		}
   		else{
   			return 0;
   		}
   	}
   	only_number[j] = '\0'; 
	only_string[k] = '\0'; 
	return 1;

}

// Driver code 
int main(int argc, char *argv[]) 
{ 
	// catch wrong input
	if(argc==1){
		printf("Please input port number\n");
		return 0;
	}
	// port : port get from argv
	// buffer : data get from client
	// only_string : message send to client ( string only )
	// only_number : message send to client ( number only )
	char *port_number = argv[1];
	int port = atoi(port_number);
	char buffer[100];
	char only_string[100];
	char only_number[100];
	int listenfd, len; 
	struct sockaddr_in servaddr, cliaddr; 
	bzero(&servaddr, sizeof(servaddr)); 
	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	while(1){
	//receive the datagram 
	len = sizeof(cliaddr); 
	int n = recvfrom(listenfd, buffer, sizeof(buffer), 
			0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
	buffer[n] = '\0';

	// split data to only string and only number
	if (split(buffer,only_number,only_string)==1)
	{
		//puts("Success");
	}
	else {
		//puts("Error");
		strcpy(only_number,"Error");
		strcpy(only_string,"");

	}

	// send the response to client
	sendto(listenfd, only_number, MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
	sendto(listenfd, only_string, MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
}
} 
