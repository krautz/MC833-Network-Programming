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

#include"database.h"

/*
 * CONSTANTS AND DEFINITIONS
 */
// the port users will be connecting to
#define PORT "3490"

// how many pending connections queue will hold
#define BACKLOG 5

// max number of bytes we can get at once
#define MAXDATASIZE 100


/*
 * I save and restore errno
 */
void sigchld_handler(int s)
{
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}


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
int main (void) {
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

    // set hints data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE; // use my IP

    // get server address info
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
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

        // set socket options and verify if it was set correctly
        check = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (check == -1) {
            printf("Server error on setting socket options\n");
            exit(1);
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

    // listen to incoming requests and verify if it is listening correctly
    if (listen(sockfd, BACKLOG) == -1) {
        printf("Server erron on listen\n");
        exit(1);
    }

    // reap all dead processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        printf("sigaction\n");
        exit(1);
    }

    /*
    DATABASE CALLS TESTS
    */
    test_database();
    char dbres[1048576];
    

    printf("%d\n", list_user_by_education("Ciencia da computacao", dbres));
    printf("%s\n", dbres);
    printf("%d\n", get_user_by_email("carlos_silva@gmail.com", dbres));
    printf("%s\n", dbres);
    printf("%d\n", list_skills_by_city("Campinas", dbres));
    printf("%s\n", dbres);
    printf("%d\n", list_all(dbres));
    printf("%s\n", dbres);
    printf("%d\n", list_experience_by_email("carlos_silva@gmail.com", dbres));
    printf("%s\n", dbres);

    /*
    END DATABASE CALLS TESTS
    */

    printf("Server: waiting for connections...\n");

    // accept and deal with incoming requests
    while(1) {
        // store size of conector's address
        sin_size = sizeof their_addr;

        // new socket file descriptor of the incoming connection request
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        // failed to accept request: move to the next request
        if (new_fd == -1) {
            printf("Error while accepting connection\n");
            continue;
        }

        // convert IP adress to a string and print it
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("Server: got connection from %s\n", s);

        // create a child process to deal with the request on that connection
        if (!fork()) {

            // child doesn't need the listener
            close(sockfd);

            // receive request from client, if it's ok print it
            if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
                printf("recv error");
                exit(1);
            }
            buf[numbytes] = '\0';
            printf("server: received '%s'\n",buf);

            if (strcmp(buf, "1\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 1 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 1");
            }
            else if (strcmp(buf, "2\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 2 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 2");
            }
            else if (strcmp(buf, "3\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 3 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 3");
            }
            else if (strcmp(buf, "4\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 4 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 4");
            }
            else if (strcmp(buf, "5\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 5 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 5");
            }
            else if (strcmp(buf, "6\0") == 0) {
                // CALL FUNCTION TO DEAL WITH CODE 6 AND WRITE IT ON RESPONSE
                strcpy(response, "Received 6");
            }
            else {
                strcpy(response, "Invalid Code. Send a valid code (1 to 6)");
            }

            // return the response from the command received
            if (send(new_fd, response, strlen(response), 0) == -1)
                printf("Error while sending response\n");

            // close child new socket
            close(new_fd);
            exit(0);
        }

        // parent doesn't need this
        close(new_fd);
    }

    return 0;

}
