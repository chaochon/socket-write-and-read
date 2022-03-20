#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define SERVER_PORT 12345


void read_data(int connfd);

ssize_t readn(int connfd, void* vptr, ssize_t size);

int main(int argc, char**argv)
{
    int listenfd,connfd;
    socklen_t clilen;
    struct sockaddr_in client_addr, server_adder;

    // create socket object
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // fill the sokect address of server
    bzero(&server_adder, sizeof(server_adder));
    server_adder.sin_family = AF_INET;
    server_adder.sin_addr.s_addr = htons(INADDR_ANY);
    server_adder.sin_port = htons(SERVER_PORT);

    // bind between the socket object of server and the address of server
    bind(listenfd, (struct sockaddr*) &server_adder, sizeof(server_adder));

    // support simultaneously 1024 the connection of client
    listen(listenfd, 1024);

    for(;;)
    {
        clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr *)&client_addr, (socklen_t*)sizeof(client_addr));
        read_data(connfd);
        close(connfd);
    }  
    return 0;   
}

void read_data(int connfd)
{
    ssize_t time = 0;
    ssize_t n;
    char buff[1024];

    for(;;)
    {
        std::cout<<"block in read"<<std::endl;
        if((n = read(connfd, (void*)buff, (ssize_t)1024))==0)
            return;
        
        time++;

        std::cout<<"1K read for "<<time<<std::endl;
        usleep(1000);
    }
}

ssize_t readn(int connfd, void* vptr, ssize_t size)
{
    ssize_t nleft = 0;
    ssize_t nread = 0;
    char *ptr;

    ptr = static_cast<char*>(vptr);
    nleft = size;

    while(nleft > 0)
    {
        if((nread = read(connfd, ptr, nleft) < 0))
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }else if(nread == 0)
            break;

        nleft -= nread;
        ptr += nread;
    }
    return size-nleft;
}