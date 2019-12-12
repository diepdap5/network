#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include<stdlib.h> 
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
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
	char *ip_address = argv[1];
	int PORT = atoi(argv[2]);
	struct sockaddr_in serv_addr; 
	int sock = 0;
	char buffer[1024] = {0};

	// Try catch false when connecting
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(ip_address); 
	
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
	while(1){
		printf("---------------------------------------\n");
		printf("1. Log in\n");
		printf("2. Log out\n");
		printf("3. Quit\n");
		printf("Your choice : ");
		char choice;
		scanf("%s", &choice);
		//switch(choice){
			//case '1': {
			if (choice == '1'){
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
					send(sock, tmp_username, strlen(tmp_username), 0);
					recv(sock, result, sizeof(result), 0);
					if (strcmp(result,SUCCESS)==0) { 
						while ((getchar()) != '\n');
						printf("Password: ");
						scanf("%s", tmp_password);
						send(sock, tmp_password, strlen(tmp_password), 0);
						char result[1024] = {0};
						recv(sock, result, sizeof(result), 0);
						if (strcmp(result, SUCCESS)==0){
							printf("Log in successfully\n");
							saveAccount(tmp_username,tmp_password);
							while ((getchar()) != '\n');
							char message_for_save[50];
							printf("----------------------------\n");
							printf("Enter what you want to save to file: \n");
							gets(message_for_save);
							send(sock, message_for_save, strlen(message_for_save), 0);
							printf("SENDED with message: %s\n", message_for_save);
						}
						if (strcmp(result, FAILURE)==0)
						{
							while ((getchar()) != '\n');
							printf("Password incorrect please try again\nPassword: ");
							scanf("%s", tmp_password);
							send(sock, tmp_password, strlen(tmp_password), 0);
							char result[1024] = {0};
							recv(sock, result, sizeof(result), 0);
							if (strcmp(result, SUCCESS)==0){
								printf("Log in successfully\n");
								saveAccount(tmp_username,tmp_password);
								while ((getchar()) != '\n');
								char message_for_save[50];
								printf("----------------------------\n");
								printf("Enter what you want to save to file: \n");
								gets(message_for_save);
								send(sock, message_for_save, strlen(message_for_save), 0);
								printf("SENDED with message: %s\n", message_for_save);
							}
							if (strcmp(result, FAILURE)==0){
								while ((getchar()) != '\n');
								printf("Password incorrect please try again\nPassword: ");
								scanf("%s", tmp_password);
								send(sock, tmp_password, strlen(tmp_password), 0);
								char result[1024] = {0};
								recv(sock, result, sizeof(result), 0);
								if (strcmp(result, SUCCESS)==0){
									printf("Log in successfully\n");
									saveAccount(tmp_username,tmp_password);
									while ((getchar()) != '\n');
									char message_for_save[50];
									printf("----------------------------\n");
									printf("Enter what you want to save to file: \n");
									gets(message_for_save);
									send(sock, message_for_save, strlen(message_for_save), 0);
									printf("SENDED with message: %s\n", message_for_save);
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
				//break;
			}
			//case '2': {
			else if (choice == '2'){
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
				//break;
			}
			//case '3':{
			else if (choice == '3'){
				return 0;
			}
			//default: printf("WRONG INPUT, CHOOSE AGAIN!\n"); break;
			else{
				printf("WRONG INPUT, CHOOSE AGAIN!\n");
			}
		}
	
	// recv(sock, buffer, sizeof(buffer), 0);
	// printf("OUTPUT: \n%s\n",buffer );
	return 0;
}