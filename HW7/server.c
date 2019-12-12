#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>

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
int main(int argc, char const *argv[])
{
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
	int listenfd, connfd, udpfd, nready, maxfdp1; 
	char buffer[100]; 
	fd_set rset; 
	ssize_t n; 
	socklen_t len;
	struct sockaddr_in cliaddr, servaddr; 
	char* message = "Hello Client";
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0))==0){
		perror("Socket failed");
		exit(EXIT_FAILURE);
	} 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		perror("\nError: ");
		return 0;
	} 
	if (listen(listenfd, 10) < 0){
		perror("\nError: ");
		return 0;
	}
	FD_ZERO(&rset); 
	FD_SET(listenfd, &rset);
	maxfdp1 = listenfd;
	while(1){
		nready = select(maxfdp1+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)){
			if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len))<0) { 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			}
			printf("You got a connection from %s\n", inet_ntoa(cliaddr.sin_addr));
			char confirm[100];
			int i=0;
			for (i = 0;i<10;i++){
				getDatatoList(&list,element);
				char result_username[1024] = {0};
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
		}
	}
	return 0;
}