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
	// buffer1,buffer2 : data get from client
	// only_string : message send to client ( string only )
	// only_number : message send to client ( number only )
	char *port_number = argv[1];
	int port = atoi(port_number);
	char buffer1[100];
	char buffer2[100];
	char only_string[100];
	char only_number[100];
	int listenfd, len1,len2; 
	struct sockaddr_in servaddr, cliaddr1, cliaddr2; 
	bzero(&servaddr, sizeof(servaddr)); 
	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	while(1){
	//receive the datagram from client 1 and 2
	len1 = sizeof(cliaddr1); 
	len2 = sizeof(cliaddr2); 
	int n1 = recvfrom(listenfd, buffer1, sizeof(buffer1), 0, (struct sockaddr*)&cliaddr1,&len1);
	int n2 = recvfrom(listenfd, buffer2, sizeof(buffer2), 0, (struct sockaddr*)&cliaddr2,&len2);  

	buffer1[n1] = '\0';
	buffer2[n2] = '\0';


	// split data from client1 to only string and only number
	if (split(buffer1,only_number,only_string)==1)
	{
		//puts("Success");
	}
	else {
		strcpy(only_number,"Error");
		strcpy(only_string,"");

	}
 
	// send the response to client 2
	sendto(listenfd, only_number, MAXLINE, 0, (struct sockaddr*)&cliaddr2, sizeof(cliaddr2)); 
	sendto(listenfd, only_string, MAXLINE, 0, (struct sockaddr*)&cliaddr2, sizeof(cliaddr2)); 

	// split data from client2 to only string and only number
	if (split(buffer2,only_number,only_string)==1)
	{
		//puts("Success");
	}
	else {
		strcpy(only_number,"Error");
		strcpy(only_string,"");

	}
	// send the response to client 1
	sendto(listenfd, only_number, MAXLINE, 0, (struct sockaddr*)&cliaddr1, sizeof(cliaddr1)); 
	sendto(listenfd, only_string, MAXLINE, 0, (struct sockaddr*)&cliaddr1, sizeof(cliaddr1)); 

}
} 
