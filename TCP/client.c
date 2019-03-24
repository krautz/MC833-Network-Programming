/*
 * INCLUDES
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>


/*
 * CONSTANTS AND DEFINITIONS
 */
// max number of bytes we can get at once
#define MAXDATASIZE 1048576


/*
 * I get the socket address according to IP version.
 */
void *get_in_addr(struct sockaddr *sa)
{
    // IPv4
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    // IPv6
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


/*
 * I am a websocket TCP client.
 */
int main(int argc, char *argv[]) {
    // sender socket (socket file descriptor)
    int sockfd, numbytes;

    // buffer to read response from
    char buf[MAXDATASIZE];

    // server address variables
    struct addrinfo hints, *servinfo, *p;

    // store server get address info error code
    int rv;

    // string with maximum address length
    char s[INET6_ADDRSTRLEN];

    // string to store request
    char request[150];

    if (argc != 5) {
        fprintf(stderr,"Usage: ./client HOSTNAME PORT REQ_CODE [PARAMS]\n");
        exit(1);
    }
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    // set the request
    strcpy(request, argv[3]);
    strcat(request, " ");
    strcat(request, argv[4]);
    printf("Request: %s\n", request);

    // set hints data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // get server address info
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // create socket and verify if it was created correctly
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            printf("Client error on creating socket\n");
            continue;
        }

        // connect socket to server and verify if it was connected correctly
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            printf("Client error on connecting to server\n");
            continue;
        }

        // socket created correctly: break
        break;
    }

    // none of the results could connect to the server
    if (p == NULL) {
        printf("Client failed to connect\n");
        return 2;
    }

    // convert IP adress to a string and print it
    inet_ntop(p->ai_family,
              get_in_addr((struct sockaddr *)p->ai_addr),
              s,
              sizeof s);
    printf("client: connecting to %s\n", s);

    // clear server info as it will no longer be used
    freeaddrinfo(servinfo);

    // send request to server
    if (send(sockfd, request, strlen(request), 0) == -1) {
        printf("Error while sending the request");
        exit(1);
    }

    // receive response from server, if it's ok print it
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        printf("recv error");
        exit(1);
    }
    buf[numbytes] = '\0';

    gettimeofday(&tv2, NULL);

    int microseconds = (tv2.tv_sec - tv1.tv_sec) * 1000000 + ((int)tv2.tv_usec - (int)tv1.tv_usec);
    int milliseconds = microseconds/1000;
    printf("Took %d ms to execute \n", milliseconds);
    printf("client: received '%s'\n",buf);

    // close the socket
    close(sockfd);

    return 0;
}
