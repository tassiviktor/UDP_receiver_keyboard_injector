#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket.h"

char buf[SBUFLEN];

struct sockaddr_in serveraddr; /* server's addr */
struct sockaddr_in clientaddr; /* client addr */

void error(char *msg) {
  perror(msg);
  exit(1);
}

int sock_initialize(int port) {

    int optval; /* flag value for setsockopt */
    struct sockaddr_in serveraddr; /* server's addr */
    /* 
     * socket: create the parent socket 
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */

    optval = 1;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    /*
     * build the server's Internet address
     */
  
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    /* 
     * bind: associate the parent socket with a port 
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        error("ERROR on binding");

    return -1;
}

int sock_listen(DatagramHandler d){

    int n; /* message byte size */
    int clientlen; /* byte size of client's address */
    struct hostent *hostp; /* client host info */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */

    while(1) {

        /*
         * recvfrom: receive a UDP datagram from a client
         */

        bzero(buf, SBUFLEN);

        n = recvfrom(sockfd, buf, SBUFLEN, 0, (struct sockaddr *) &clientaddr, &clientlen);

        if (n < 0)
            error("ERROR in recvfrom");


        /* 
         * gethostbyaddr: determine who sent the datagram
         */

        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);

        if (hostp == NULL)
            error("ERROR on gethostbyaddr");

        hostaddrp = inet_ntoa(clientaddr.sin_addr);

        if (hostaddrp == NULL)
            error("ERROR on inet_ntoa\n");

        printf("server received datagram from %s (%s)\n", hostp->h_name, hostaddrp);
        printf("server received %lu/%u", strlen(buf), n);
    
        /* 
         * sendto: echo the input back to the client 
         */
        // n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, clientlen);
        // if (n < 0) 
        // error("ERROR in sendto");
        d(buf);
    }
    
}