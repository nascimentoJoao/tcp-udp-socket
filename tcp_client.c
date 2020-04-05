#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd, char* filename)
{
    char buff[MAX];
    int n, b;

    while(1) {
        bzero(buff, sizeof(buff));
        printf("Enter file name: ");
        FILE *fp = fopen(filename, "rb");

        if(fp == NULL) {
            perror("File");
            return ;
        }

        while( (b = fread(buff, 1, sizeof(buff), fp)) > 0) {
            send(sockfd, buff, b, 0);
        }

        read(sockfd, buff, sizeof(buff));
        printf("From server: %s\n\n", buff);
            if((strncmp(buff, "exit", 4)) == 0) {
                printf("Client Exit...\n");
                break;
            }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("argv: %s\n\n", argv[1]);

    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created!\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);


    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("Connected with the server.\n");

    func(sockfd, argv[1]);

    close(sockfd);
}
