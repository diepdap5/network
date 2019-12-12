// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include<stdlib.h> 
void process_string();
int main(int argc, char *argv[]) 
{
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
	// ip_address of server
	// port number	
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int sock = 0; 
	struct sockaddr_in serv_addr; 

	// Try catch false when connecting
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 


	// MENU


	printf("MENU\n");
	printf("----------------------------\n");
	printf("1. Gửi xâu bất kỳ\n");
	printf("2. Gửi nội dung một file\n");
	printf("Khác để thoát\n Lựa chọn là: ");
	char choice ;
	scanf("%s",&choice);
	getchar();
	switch(choice){
		case '1': 
			{
				do{
					// send message to server
					// return message after split string
					int valread = 0;
					char buffer[1024] = {0}; 
					char buffer2[1024] ={0};
					int g=0;
					char message[100];
					// input message
					printf("INPUT: ");
					g = scanf("%[^\n]", message); 	
					if (g == 0) break;
					getchar();
					// send message to server
					send(sock , message , strlen(message) , 0 ); 

					// get message from server
					valread = read( sock , buffer, 1024); 
					printf("OUTPUT: \n%s\n",buffer ); 
					if (strcmp(buffer,buffer2)!=0) 	
					{
						valread = read( sock , buffer2, 1024); 
						printf("%s\n",buffer2 ); 
					}
					puts("-------------------------");		
				}while(1);
				break;
			}
		case '2': 
			{
				// send message (file name) to server
				// get contain of that file
				char buffer[100];
				int count;
				char message[100];
				int g=0, ch=0;
				// input file name
				printf("INPUT: "); 
				g = scanf("%[^\n]", message);
				if (g == 0) break;
				// change file name to txt type
				strcat(message, ".txt");
				// send file name to server
				send(sock , message , strlen(message) , 0 );	
				// get number line in that file			
				read(sock, &count, sizeof(int));
				// get contain of that file
				while(ch < count + 1){
					read(sock, buffer,100);
					ch++;
					printf("%s\n",buffer);		
				}
				printf("\n");
				break;
			}
		default: 	break;
	}
	return 0; 
} 
