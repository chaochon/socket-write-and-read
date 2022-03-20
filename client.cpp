#include<stddef.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>

#define SERVER_PORT 12345
#define SERVER_ADDR "127.0.0.1"
#define MESSAGE_SIZE 102400

void send_data(int sockfd);

int main(int argc, char** argv)
{
    int clientfd;
    struct sockaddr_in server_addr;

    clientfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
    
    connect(clientfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    
    send_data(clientfd);
    return 0;
}

void send_data(int sockfd) {
    char *query;
    query = (char *)malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    long remaining = (long) strlen(query);
    while (remaining) {
        long n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written < 0) {
            perror("send");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }
}