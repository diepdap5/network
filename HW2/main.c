#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <errno.h>
int valid_digit(char *ip_str) 
{ 
    while (*ip_str) { 
        if (*ip_str >= '0' && *ip_str <= '9') 
            ++ip_str; 
        else
            return 0; 
    } 
    return 1; 
} 
  
/* return 1 if IP string is valid, else return 0 */
int is_valid_ip(char *ip_str) 
{ 
    int i, num, dots = 0; 
    char *ptr; 
  
    if (ip_str == NULL) 
        return 0; 
  
    // See following link for strtok() 
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/strtok_r.html 
    ptr = strtok(ip_str, "."); 
  
    if (ptr == NULL) 
        return 0; 
  
    while (ptr) { 
  
        /* after parsing string, it must contain only digits */
        if (!valid_digit(ptr)) 
            return 0; 
  
        num = atoi(ptr); 
  
        /* check for valid IP */
        if (num >= 0 && num <= 255) { 
            /* parse remaining string */
            ptr = strtok(NULL, "."); 
            if (ptr != NULL) 
                ++dots; 
        } else
            return 0; 
    } 
  
    /* valid IP string must contain 3 dots */
    if (dots != 3) 
        return 0; 
    return 1; 
} 

int get_ip(char * hostname , char* ip) 
{  struct hostent *he;     
   struct in_addr **addr_list;     
   int i;     
   if ( (he = gethostbyname( hostname ) ) == NULL)     
   { herror("gethostbyname");         
     return 1;}     
   addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++)
    {   strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;}
    return 1;
}
int main(int argc, char *argv[]){
	if(argc==1){
		printf("Please input IP or host name\n");
		return 0;
	}
	else{
		char *input = argv[1];
		//printf("%s\n", input);
		char ip[100];
		//if (validate_ip(input) == 1) {printf("Valid\n");}
		//else {printf("Not Valid\n");}
		// if (is_valid_ip(input)==1) {
		// 	printf("Valid\n");		 
		// if (get_ip(input , ip)== 0){     
  // 		printf("%s resolved to %s" , input , ip);     
  // 		printf("\n");}
		// }
		// else { printf("Not valid\n"); }
	}
	return 0;

}