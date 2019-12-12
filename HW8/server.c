#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>

#define SUCCESS "Success"
#define FAILURE "Failure"
#define BLOCKING "Blocking"
typedef struct 
{
	char user_name[20];
	char password[20];
	int status;
}elementtype;
#include "linkedlist.h"

void sig_chld(int signo){
	pid_t pid;
	int stat;
	pid = waitpid(-1, &stat, WNOHANG);
	printf("child %d terminated\n", pid );
}
int searchData(singleList list, char username[30])  {  
    list.cur = list.root; // Initialize current  
    // search all list to find username
    // if existed , return 1
    // else return 0
    while (list.cur != NULL)  
    {  
        if (strcmp(list.cur->element.user_name, username) == 0)  
            return 1;  
        list.cur = list.cur->next;
    }  
    return 0;  
} 
void getDatatoList(singleList *list,elementtype element){
	// make null the list
	deleteSingleList(list);
	FILE *fp;
	fp = fopen("account.txt","r");
	// counting the length of this file
	char c;
	int count =0;
	for (c = getc(fp); c != EOF; c = getc(fp)) 
	{
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
	}
	fclose(fp);
	// read the data to list 
	fp = fopen("account.txt","r");
	for(int i=0;i< count + 1;i++){
		fscanf(fp, "%s %s %d", element.user_name, element.password, &element.status);
		insertEnd(list,element);
	}  
	fclose(fp);
}
int checkAccount(singleList list, char ten[30], char mk[30]){
	// search all list to check username, password is correct or not
	// if correct, return 1
	// else return 0
	list.cur = list.root;
	while(list.cur != NULL){
		if ((strcmp(list.cur->element.user_name, ten) == 0) && (strcmp(list.cur->element.password, mk) == 0)){
			return 1;
		}
		list.cur = list.cur->next;
	}
	return 0;
}
int checkBlock(singleList list, char ten[30]){
	// search all list to find out this username has status 0 or 1
	// if status = 1 return 1
	// else status = 0 return 0
	list.cur = list.root;
	while(list.cur!=NULL){
		if (strcmp(list.cur->element.user_name, ten) == 0){
			if(list.cur->element.status == 1){
				return 1;
			}
			else{
				return 0;
			}
		}
		list.cur = list.cur->next;
	}
}
void blockData(singleList list,char ten[30]){
	// Change the status of username in list to 0
	list.cur = list.root;
	while(list.cur != NULL){
		if (strcmp(list.cur->element.user_name, ten) == 0){
			list.cur->element.status = 0;
			break;
		}
		list.cur = list.cur->next;
	}
	list.cur = list.root;
	// rewrite all list to file
	FILE *fp;
	fp = fopen("account.txt","w");
	while(list.cur!=NULL){
		fprintf(fp,"%s %s %d\n",list.cur->element.user_name, list.cur->element.password, list.cur->element.status);
		list.cur = list.cur->next;
	}
	fclose(fp);
}
int main(int argc, char *argv[]){

	// catch wrong input
	if (argc<2)
	{
		printf("Invalid Parameter\n");
		return 0;
	}
	// open fife
	// check if file existed
	FILE *fp;
	fp = fopen("account.txt","r");
	if (fp == NULL){
		printf("No file input\n");
		return 0;
	}
	// create list of account
	singleList list;
	createSingleList(&list);
	elementtype element;

	// Create server - client
	int PORT = atoi(argv[1]);
	int listenfd, connfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	pid_t pid;
	int sin_size;
	if ((listenfd = socket(AF_INET, SOCK_STREAM,0 )) == 0){
		perror("Socket Failed");
		exit(EXIT_FAILURE);
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons( PORT ); 

	if(bind(listenfd, (struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, 3) < 0){  
		perror("\nError: ");
		return 0;
	}
	signal(SIGCHLD, sig_chld);
	while(1){
		sin_size = sizeof(struct sockaddr_in);
		if ((connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t*)&sin_size))<0) { 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}
		pid = fork();
		// char hello[100];
		// strcpy(hello, "Hello Dip");
		char confirm[100];
		if (pid == 0){
			int i=0;
			for (i = 0;i<10;i++){
				getDatatoList(&list,element);
				char result_username[1024] = {0};
				printf("You got a connection from %s\n", inet_ntoa(client.sin_addr));
				recv(connfd, result_username, sizeof(result_username), 0);
				printf("Server going to validate %s\n", result_username );
				if(searchData(list, result_username)==1){
					if (checkBlock(list,result_username) == 1){
						strcpy(confirm, SUCCESS);
						send(connfd, confirm, strlen(confirm), 0);
						char result_password[1024] = {0};
						recv(connfd, result_password, sizeof(result_password), 0);
						printf("Check pass: %s\n", result_password );
						if (checkAccount(list,result_username,result_password) == 1){
							strcpy(confirm, SUCCESS);
							send(connfd, confirm, strlen(confirm), 0);
							strcat(result_username, ".txt");
							FILE *fs;
							fs = fopen(result_username,"a+");	
							char message_for_save[1024] = {0};
							recv(connfd, message_for_save, sizeof(message_for_save), 0);
							printf("Saving %s\n",message_for_save );
							fprintf(fs,"%s", message_for_save);		
							fclose(fs);				
						}
						else{
							strcpy(confirm, FAILURE);
							send(connfd, confirm, strlen(confirm), 0);
							char result_password[1024] = {0};
							recv(connfd, result_password, sizeof(result_password), 0);
							printf("Check pass: %s\n", result_password );
							if (checkAccount(list,result_username,result_password) == 1){
								strcpy(confirm, SUCCESS);
								send(connfd, confirm, strlen(confirm), 0);
								strcat(result_username, ".txt");
								FILE *fs;
								fs = fopen(result_username,"a+");	
								char message_for_save[1024] = {0};
								recv(connfd, message_for_save, sizeof(message_for_save), 0);
								printf("Saving %s\n",message_for_save );
								fprintf(fs,"%s", message_for_save);		
								fclose(fs);
							}
							else{
								strcpy(confirm, FAILURE);
								send(connfd, confirm, strlen(confirm), 0);
								char result_password[1024] = {0};
								recv(connfd, result_password, sizeof(result_password), 0);
								printf("Check pass: %s\n", result_password );
								if (checkAccount(list,result_username,result_password) == 1){
									strcpy(confirm, SUCCESS);
									send(connfd, confirm, strlen(confirm), 0);
									strcat(result_username, ".txt");
									FILE *fs;
									fs = fopen(result_username,"a+");	
									char message_for_save[1024] = {0};
									recv(connfd, message_for_save, sizeof(message_for_save), 0);
									printf("Saving %s\n",message_for_save );
									fprintf(fs,"%s", message_for_save);		
									fclose(fs);
								}
								else{
									strcpy(confirm, FAILURE);
									send(connfd, confirm, strlen(confirm), 0);
									blockData(list,result_username);
								}
							}
						}
					}
					else{
						strcpy(confirm, BLOCKING);
						send(connfd, confirm, strlen(confirm), 0);
					}
				}
				else{
					strcpy(confirm, FAILURE);
					send(connfd, confirm, strlen(confirm), 0);
				}
			}
			close(connfd);
			//exit(0);
		}
		close(connfd); 
	}
	//close(listenfd);	
	return 0;
}