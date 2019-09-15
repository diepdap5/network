// THIS IS A DEPRECATED METHOD OF GETTING HOST NAMES
// use getaddrinfo() instead!

#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int i;
    struct hostent *he;
    //struct in_addr **addr_list;

    // if (argc != 2) {
    //     fprintf(stderr,"usage: ghbn hostname\n");
    //     return 1;
    // }

    // if ((he = gethostbyname(argv[1])) == NULL) {  // get the host info
    //     herror("gethostbyname");
    //     return 2;
    // }

    // // print information about this host:
    // printf("Official name is: %s\n", he->h_name);
    // printf("    IP addresses: ");
    // addr_list = (struct in_addr **)he->h_addr_list;
    // for(i = 0; addr_list[i] != NULL; i++) {
    //     printf("%s ", inet_ntoa(*addr_list[i]));
    // }
    // printf("\n");

    //struct hostent *he;
    struct in_addr ipv4addr;
    struct in6_addr ipv6addr;

    inet_pton(AF_INET, "192.0.2.34", &ipv4addr);
    he = getnameinfo(&ipv4addr, sizeof ipv4addr, AF_INET);
    printf("Host name: %s\n", he->h_name);


    return 0;
}