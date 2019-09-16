#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <errno.h>
#include "../lib/domain_ip.h"

// Check number between 0 and 256
int check_number(char *ip_part) {
    int number = atoi(ip_part);

    if(number < 0 || number > 256)
        return 0;
    return 1;
}

// Check ip address 
// return 1 for true ip address
// return 0 for not
int ip_valid(char *ip) {
    char str[40];
    strcpy(str, ip);

    int count = 0;
    int number = 0;

  char *token = strtok(str, "."); 

  while (token != NULL) 
    { 
        if(!check_number(token))
            return 0;

        token = strtok(NULL, "."); 
        count++;
    }

    if(count != 4) 
        return 0;

    return 1;
}

// Input hostname, print out IP, alias IP

void get_ip(char * hostname) 
{   
    struct hostent *he;     
    struct in_addr **addr_list;     
    int i;     
    if ( (he = gethostbyname( hostname ) ) == NULL)     
    {
        // if not a hostname, print out fail
        //herror("gethostbyname");
        printf("Not found information\n");         
    }
    else{  
        // Print out all alias
        addr_list = (struct in_addr **) he->h_addr_list;
        for(i = 0; addr_list[i] != NULL; i++) {
            if (i==0) {
                printf("Official : %s \n", inet_ntoa(*addr_list[i]));
                printf("Alias : ");
            }
            else{
                
                printf("%s ", inet_ntoa(*addr_list[i]));
            }
            printf("\n");
        }
        printf("\n");
    }

}

// Input IP, print out hostname and alias


void get_info_ip_address(char *ipAddress) {
    struct in_addr addr;
    inet_aton(ipAddress, &addr);
    struct hostent *host = gethostbyaddr(&addr, sizeof(addr), AF_INET);

    if(host == NULL) {
        printf("Not found information\n");
    }
    else{
        printf("Official name: %s\n",host->h_name);
        printf("Alias: \n");
        if (host->h_aliases[0] == NULL){
            printf("NULL\n");
        }
        else{
            for(int i = 0; host->h_aliases[i] != NULL; i++) {
                printf("\t%s\n", host->h_aliases[i]);
            }
        }

    }   
}
