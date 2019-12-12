// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<stdlib.h> 
#include <ctype.h>
// check if have type .txt
int check_valid_file(char* file_name){
	if (file_name[strlen(file_name)-1] == 't' && file_name[strlen(file_name)-2] == 'x' && 
		file_name[strlen(file_name)-3] == 't' && file_name[strlen(file_name)-4] == '.'){
		return 1;
	}
	return 0;
}
// split string into only_string and only_number
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
int main(int argc, char *argv[]) 
{
	// catch wrong input
	if(argc==1){
		printf("Please input port number\n");
		return 0;
	}
	char *port_number = argv[1];
	int port = atoi(port_number);
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( port ); 
	
	// Forcefully attaching socket to the port 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	while(1)
	{
		int valread = 0;
		char buffer[1024] = {0};
		char only_string[100];
		char only_number[100];
		valread = read( new_socket , buffer, 1024);
		// check if have type filename.txt 
		// means: do case 2
		// else do case 1
		if (check_valid_file(buffer) == 1){
			FILE *f;
			int words = 0;
			char c;
			// catch un-exist file
			f = fopen(buffer, "r");
			if (f == NULL) {
        		perror("Error in file open\n");
    		}
    		// count the line
			char buffer2[100];
			int count = 0;
			for (c = getc(f); c != EOF; c = getc(f)){
				if (c == '\n'){
					count = count + 1;
				}
			}
			fclose(f);
			// send number of line in this file
			write(new_socket, &count, sizeof(int));
			f = fopen(buffer, "r");
			// send data of file
			for(int i=0;i< count + 1;i++){
				fscanf(f,"%[^\n]\n",buffer2);
				write(new_socket,buffer2,100);
			}
			fclose(f);
			break;
		} 
		// do case 1

		printf("%s\n",buffer ); 
		// split string
		if (split(buffer,only_number,only_string)==1)
		{
			//puts("Success");
		}
		else {
			//puts("Error");
			strcpy(only_number,"Error");
			strcpy(only_string,"");

		}
		if (strlen(only_number) == 0) strcpy(only_number,"   ");
		if (strlen(only_string) == 0) strcpy(only_string,"   ");
		// send string after split
		send(new_socket , only_number , strlen(only_number) , 0 ); 
		send(new_socket , only_string , strlen(only_string) , 0 ); 
	}
	return 0; 
} 
