#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct 
{
	char user_name[20];
	char password[20];
	int status;
}elementtype;
#include "linkedlist.h"
/*
Các hàm bổ sung
displaySingleList : Display all list
getData : get data from file txt, push it to list
searchData : check if the username is existed or not, return 1 for existed
WriteDataToFile : write(update) user to file txt
checkAccount : check if the username and password is correct or not, return 1 for correct
blockData : change this username's status to 0 (blocked)
checkBlock : check if the username is blocked or not, return 0 for blocked
checkSignIn : check if the username had been sign in or not, return 1 for had sign in
Signout : Sign out this username

user_name_had_sign_in: an array of username which had sign in
number_of_sign_in : the length of user_name_had_sign_in
*/

void displaySingleList(singleList list);
void getDatatoList(singleList *list,elementtype element);
int searchData(singleList list, char username[30]);  
void WriteDataToFile(char ten[30], char mk[30]);
int checkAccount(singleList list, char ten[30], char mk[30]);
void blockData(singleList list,char ten[30]);
int checkBlock(singleList list, char ten[30]);
int checkSignIn(char ten[30]);
void Signout(char ten[30]);
char user_name_had_sign_in[10][30];
int number_of_sign_in = 0;



/*
Chi tiết các hàm 
*/
void displaySingleList(singleList list)
{
  if (list.root == NULL) return;
  printf("----------------DS--------------\n");
  list.cur = list.root;
  while(list.cur != NULL)
    {
      printf("%s %s %d \n",list.cur->element.user_name,list.cur->element.password,list.cur->element.status);
      list.cur = list.cur->next;
    }
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
int searchData(singleList list, char username[30])  
{  
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
void writeDataToFile(char ten[30], char mk[30]){
	// write username, password, status to the end of the file
	FILE *fp;
	fp = fopen("account.txt","a");
	fprintf(fp,"\n%s %s 1",ten, mk);
	fclose(fp);
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
int checkSignIn(char ten[30]){
	// check if this username is in the array of user_name_had_sign_in or not
	// return 1 for yes
	// else return 0 for no
	for(int k=0; k<number_of_sign_in;k++){
		if (strcmp(user_name_had_sign_in[k],ten)==0){
			return 1;
		}
	}
	return 0;
}
void Signout(char ten[30]){
	// find the position of this username in the array user_name_had_sign_in (k)
	int k = 0;
	for(k=0; k<number_of_sign_in;k++){
		if (strcmp(user_name_had_sign_in[k],ten)==0){
			break;
		}
	}
	// if username in the head of the array
	if(k == 0){
		if(number_of_sign_in >1){
			strcpy(user_name_had_sign_in[k],user_name_had_sign_in[number_of_sign_in -1]); 
			number_of_sign_in --;
		}
		else{
			number_of_sign_in--;
		}
	}
	// if not the head
	else{
		number_of_sign_in--;
	}
}



/*
*
*
MAIN
*
*
*/
int main(){
	FILE *fp;
	fp = fopen("account.txt","r");
	if (fp == NULL){
		printf("No file input\n");
		return 0;
	}
	singleList list;
	createSingleList(&list);
	elementtype element;
	/*
	*
	MENU
	*
	*/
	do{
		printf("\n \n---------------------------------------\n");
		printf("USER MANAGEMENT PROGRAM\n");
		printf("---------------------------------------\n");
		printf("1. Register\n");
		printf("2. Sign in\n");
		printf("3. Search\n");
		printf("4. Sign out\n");
		printf("Your choice (1-4), other to quit: \n");
		int choice = 0;
		scanf("%d",&choice);
		switch(choice){
			case 1: {
				// Read from keyboard the username
				while ((getchar()) != '\n'); 
				char ten_dang_nhap[30], mat_khau[30]; 
				getDatatoList(&list,element);
				printf("\n---------Register---------\n");
				printf("Username : ");
				scanf("%s", ten_dang_nhap);
				// check if the username is existed or not
				// If yes, print out fault
				// If not, registration
				if (searchData(list,ten_dang_nhap) == 1){
					printf("\nAccount existed\n");
				}
				else{
					while ((getchar()) != '\n');
					printf("Password : ");
					scanf("%s", mat_khau);
					writeDataToFile(ten_dang_nhap, mat_khau);
					printf("Successful registration\n");
				}
				break;	
			}
			case 2:{
				// Read from keyboard the username
				char ten_dang_nhap[30], mat_khau[30]; 
				while ((getchar()) != '\n'); 
				getDatatoList(&list,element);
				printf("\n---------Sign In---------\n");
				printf("Username : ");
				scanf("%s", ten_dang_nhap);
				// check if the username is existed or not
				// if not, print out fault
				// if yes, countinous
				if (searchData(list,ten_dang_nhap) == 1){
					// check if the username is blocked or not
					// if blocked, print out fault
					// if not, countinous
					if (checkBlock(list,ten_dang_nhap) == 1){
						// Read pass from keyboard
						// if more than 3 times wrong, block username
						int wrong_pass_count = 0;
						do{
							while ((getchar()) != '\n');
							printf("Password : ");
							scanf("%s", mat_khau);
							if (checkAccount(list,ten_dang_nhap,mat_khau) == 1){
								printf("Hello %s\n", ten_dang_nhap);
								strcpy(user_name_had_sign_in[number_of_sign_in],ten_dang_nhap);
								number_of_sign_in = number_of_sign_in +1;
								break;
							}
							else{
								printf("Password is incorrect\n");
								wrong_pass_count++;
							}
						}while(wrong_pass_count < 3);
						if (wrong_pass_count == 3){
							blockData(list,ten_dang_nhap);
							printf("Password is incorrect. Account is blocked\n");
						}
					}
					else{
						printf("Account is blocked\n");
					}
				}
				else{
					printf("Cannot find account\n");
				}
				break;
			}
			case 3: {
				// Read username from keyboard
				char ten_dang_nhap[30];
				while ((getchar()) != '\n'); 
				getDatatoList(&list,element);
				printf("\n---------Search---------\n");
				printf("Username : ");
				scanf("%s", ten_dang_nhap);
				// Check if username is existed and had sign in or not
				if (searchData(list,ten_dang_nhap) == 1) {
					if(number_of_sign_in==0){
						printf("Account is not sign in \n");
					}
					else{
						if (checkBlock(list, ten_dang_nhap)==1){
							printf("Account is active\n");
						}
						else{
							printf("Account is blocked\n");
						}
					}
				}
				else{
					printf("Cannot find account\n");
				}
				break;
			}
			case 4:{
				// Read username from keyboard
				char ten_dang_nhap[30];
				while ((getchar()) != '\n'); 
				getDatatoList(&list,element);
				printf("\n---------Search---------\n");
				printf("Username : ");
				scanf("%s", ten_dang_nhap);
				// check if username if existed and had sign in or not
				if (searchData(list,ten_dang_nhap)==1){
					if(checkSignIn(ten_dang_nhap)==1){
						printf("Goodbye %s\n",ten_dang_nhap );
						Signout(ten_dang_nhap);
					}
					else{
						printf("Account is not sign in\n");
					}
				}
				else{
					printf("Cannot find account\n");
				}
				break;
			}
			default: return 0;
		}
	}while(1);
return 0;
}