/*
 * INCLUDES
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

#include"database.h"

/*
 * CONSTANTS AND DEFINITIONS
 */
// how many pending connections queue will hold
#define BACKLOG 5

// max number of bytes we can get at once
#define MAXDATASIZE 100


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
 * I am a websocket TCP server.
 */
int main (int argc, char *argv[]) {
    // listener socket (socket file descriptor)
    int sockfd;

    // new connection socket
    int new_fd;

    // server address variables
    struct addrinfo hints, *servinfo, *p;

    // connector's address information
    struct sockaddr_storage their_addr;

    // verify error code of function calls
    int check;

    // variable to store address size
    socklen_t sin_size;

    // struct to reap dead processes
    struct sigaction sa;

    // flag for socket options
    int yes=1;

    // string with maximum address length
    char s[INET6_ADDRSTRLEN];

    // store server get address info error code
    int rv;

    // buffer to read requests from and buffer to write response
    char buf[MAXDATASIZE], response[MAXDATASIZE];

    // received bytes from request
    int numbytes;

    // request parameter
    char param[150];

    // time of th day literals
    struct timeval tv1, tv2;

    if (argc != 2) {
        fprintf(stderr,"Usage: ./server PORT\n");
        exit(1);
    }

    // set hints data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    hints.ai_flags = AI_PASSIVE; // use my IP

    // get server address info
    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // create socket and verify if it was created correctly
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            printf("Server error on creating socket\n");
            continue;
        }

        // bind socket to a port and verify if it was binded correctly
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("Server error on bind\n");
            continue;
        }

        // websocket created correctly: break
        break;
    }

    // clear server info as it will no longer be used
    freeaddrinfo(servinfo);

    // none of the server addresses were able to create a websocker: abort
    if (p == NULL) {
        printf("Server error: none addresses could be binded\n");
        exit(1);
    }

    /*
    DATABASE CALLS TESTS
    */
    test_database();
    char dbres[1048576];

    printf("%d\n", list_person_info_by_email("carlos_silva@gmail.com", dbres));
    printf("%s\n", dbres);
    /*
    END DATABASE CALLS TESTS
    */

    printf("Server: waiting rcvfrom...\n");

    // accept and deal with incoming requests
    while(1) {
        // store size of conector's address
        sin_size = sizeof their_addr;

        // receive datagrams
        numbytes = recvfrom(
            sockfd,
            buf,
            MAXDATASIZE - 1,
            0,
            (struct sockaddr *)&their_addr,
            &sin_size
        );

        if (numbytes  == -1) {
            printf("recvfrom");
            exit(1);
        }

        // convert IP adress to a string and print it
        inet_ntop(
            their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s,
            sizeof s
        );
        printf("Server: got packet from %s\n", s);

        printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buf);

        // get request parameters
        if (strlen(buf) >= 3)
            strcpy(param, buf + 2);

        // get time of the day
        gettimeofday(&tv1, NULL);

        if (buf[0] == '1') {
            list_person_info_by_email(param, dbres);
            printf("%s\n", dbres);
        } else {
            strcpy(dbres, "Invalid Code. Send a valid code (1)");
        }

        // get time at the end of the db operation
        gettimeofday(&tv2, NULL);

        // calculate time spent
        int microseconds = (tv2.tv_sec - tv1.tv_sec) * 1000000 + ((int)tv2.tv_usec - (int)tv1.tv_usec);
        int milliseconds = microseconds/1000;

        printf("Took %d us to execute \n", microseconds);

        // return the response from the command received
        numbytes = sendto(
                        sockfd,
                        dbres,
                        strlen(dbres),
                        0,
                        (struct sockaddr *)&their_addr,
                        sin_size
                    );
        if (numbytes == -1) {
            printf("Error while sending the response");
            exit(1);
        }
        if (numbytes == -1)
            printf("Error while sending response\n");

        // close(sockfd);
    }

    return 0;

}
