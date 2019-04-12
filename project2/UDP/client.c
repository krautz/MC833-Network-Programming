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
#define PARTIALBUFFERSIZE 63001


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
    int sockfd;

    // buffer to read response from
    char buf[MAXDATASIZE];

    // buffer to read each datagram response from
    char buf_partial[PARTIALBUFFERSIZE];

    // server address variables
    struct addrinfo hints, *servinfo, *p;

    // store server get address info error code
    int rv;

    // received bytes from request
    int numbytes;

    // string with maximum address length
    char s[INET6_ADDRSTRLEN];

    // string to store request
    char request[150];

    // time of th day literals
    struct timeval tv, tv1, tv2;

    if (argc != 5) {
        fprintf(stderr,"Usage: ./client HOSTNAME PORT REQ_CODE [PARAMS]\n");
        exit(1);
    }

    // set the request
    strcpy(request, argv[3]);
    strcat(request, " ");
    strcat(request, argv[4]);
    strcat(request, "\0");
    printf("Request: %s\n", request);

    // set hints data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // get server address info
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and creat the socket to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // create socket and verify if it was created correctly
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            printf("Client error on creating socket\n");
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
    printf("client: will send to %s\n", s);

    // clear server info as it will no longer be used
    freeaddrinfo(servinfo);

    // send request to server and get the time
    gettimeofday(&tv1, NULL);

    // send request
    numbytes = sendto (
        sockfd,
        request,
        strlen(request),
        0,
        p->ai_addr,
        p->ai_addrlen
    );
    if (numbytes == -1) {
        printf("Error while sending the request");
        exit(1);
    }

    // temp file descriptor list for select()
    fd_set read_fds;

    // clear our set
    FD_ZERO(&read_fds);

    // add the the socked descriptor to the set
    FD_SET(sockfd, &read_fds);

    // set the timeout to 3 seconds
    tv.tv_usec = 3000000;

    // initial timeout
    if (select(sockfd + 1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("select");
        exit(4);
    }

    // clear final buffer
    strcpy(buf, "");

    // store total number of bytes
    int total = 0;

    // store size of conector's address
    int sin_size = sizeof p;

    // receive the packetes
    while (FD_ISSET(sockfd, &read_fds)) {

        // receive response from server
        numbytes = recvfrom(
            sockfd,
            buf_partial,
            PARTIALBUFFERSIZE - 1,
            0,
            (struct sockaddr *)&p,
            &sin_size
        );

        // incremental total number of bytes
        total += numbytes;

        // check if response is ok
        if (numbytes == -1) {
            printf("recv error");
            exit(1);
        }

        // append this datagram to the final response buffer
        strcat(buf, buf_partial);

        printf("listener:  received packet is %d bytes long\n", numbytes);
        if (select(sockfd + 1, &read_fds, NULL, NULL, &tv) == -1) {
            perror("select");
            exit(4);
        }
    }

    // set the final character of the final response
    buf[total] = '\0';

    // get time at the end of the request
    gettimeofday(&tv2, NULL);

    // calculate time spent
    int microseconds = (tv2.tv_sec - tv1.tv_sec) * 1000000 + ((int)tv2.tv_usec - (int)tv1.tv_usec);
    int milliseconds = microseconds/1000;

    printf("client: received '%s'\n",buf);
    printf("Took %d us to execute \n", microseconds);

    // close the socket
    close(sockfd);

    return 0;
}
