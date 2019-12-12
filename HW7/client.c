// TCP Client program 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <errno.h> 
#include <signal.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <unistd.h> 
#define MAXLINE 1024 
int STATUS = 0;
/*
STATUS = 0 -> No account signed in
STATUS = 1 -> Already have account sign in
STATUS = 2 -> Account had log out
*/
char USERNAME[30];
char PASSWORD[30];
#define SUCCESS "Success"
#define FAILURE "Failure"
#define BLOCKING "Blocking"

void saveAccount(char tmp_username[30], char tmp_password[30]){
	int k=0; int h=0;
	for (k=0; k< strlen(tmp_username); k++){
		USERNAME[k] = tmp_username[k];
	}
	for (h=0; h< strlen(tmp_password); h++){
		PASSWORD[h] = tmp_password[h];
	}
	STATUS = 1;
}
int main(int argc, char *argv[]) 
{ 
	if (argc != 3){
		printf("Please input IP address and port number\n");		
		return 0;
	}
	char *ip_address = argv[1];
	int PORT = atoi(argv[2]);
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr; 

	int n, len; 

	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr(ip_address); 
	if(inet_pton(AF_INET, ip_address, &servaddr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) { 
		printf("\n Error : Connect Failed \n"); 
	} 
	while(1){
		printf("---------------------------------------\n");
		printf("1. Log in\n");
		printf("2. Log out\n");
		printf("3. Quit\n");
		printf("Your choice : ");
		int choice;
		scanf("%d", &choice);
		if (choice == 1){
			if (STATUS == 1 ){
				printf("You have already log in\n");
			}
			if(STATUS == 2){
				printf("This window can not log in more.\n");				
			}
			if(STATUS == 0){
				while ((getchar()) != '\n');
				char result[1024] = {0};
				char tmp_username[30], tmp_password[30];
				printf("Username:");
				scanf("%s", tmp_username);
				send(sockfd, tmp_username, strlen(tmp_username), 0);
				recv(sockfd, result, sizeof(result), 0);
				if (strcmp(result,SUCCESS)==0) { 
					while ((getchar()) != '\n');
					printf("Password: ");
					scanf("%s", tmp_password);
					send(sockfd, tmp_password, strlen(tmp_password), 0);
					char result[1024] = {0};
					recv(sockfd, result, sizeof(result), 0);
					if (strcmp(result, SUCCESS)==0){
						printf("Log in successfully\n");
						saveAccount(tmp_username,tmp_password);
					}
					if (strcmp(result, FAILURE)==0)
					{
						while ((getchar()) != '\n');
						printf("Password incorrect please try again\nPassword: ");
						scanf("%s", tmp_password);
						send(sockfd, tmp_password, strlen(tmp_password), 0);
						char result[1024] = {0};
						recv(sockfd, result, sizeof(result), 0);
						if (strcmp(result, SUCCESS)==0){
							printf("Log in successfully\n");
							saveAccount(tmp_username,tmp_password);
						}
						if (strcmp(result, FAILURE)==0){
							while ((getchar()) != '\n');
							printf("Password incorrect please try again\nPassword: ");
							scanf("%s", tmp_password);
							send(sockfd, tmp_password, strlen(tmp_password), 0);
							char result[1024] = {0};
							recv(sockfd, result, sizeof(result), 0);
							if (strcmp(result, SUCCESS)==0){
								printf("Log in successfully\n");
								saveAccount(tmp_username,tmp_password);
							}
							if (strcmp(result, FAILURE)==0){
								printf("Password in correct 3 times. Block account\n");
							}
						}
					}
				}
				if (strcmp(result,FAILURE)==0){
					printf("Account is not existed\n");
				}
				if (strcmp(result,BLOCKING)==0)	{
					printf("Account is blocked, can't log in\n");
				}			
			}
		}
		else if (choice == 2){
			if (STATUS == 0){
				printf("Haven't sign in please log in first\n");
			}
			if (STATUS == 2){
				printf("Have already log out\n");
			}
			if (STATUS == 1){
				printf("Sign out username %s successfully\n", USERNAME);
				STATUS = 2;
			}							
		}
		else if (choice == 3){
			return 0;
		}
		else{
			printf("WRONG INPUT, CHOOSE AGAIN!\n");
		}
	}		
	return 0;
} 