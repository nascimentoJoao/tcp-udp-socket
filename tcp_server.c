#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;

    while(1){
        bzero(buff,MAX);

        //Le a mensagem do cliente e copia em um buffer
        read(sockfd, buff, sizeof(buff));
        printf("From client: %s\n\n To client: ", buff);
        bzero(buff, MAX);
        n = 0;

        while((buff[n++] = getchar()) != '\n')
        ;

        write(sockfd, buff, sizeof(buff));
        
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }

    }
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket created!\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0){
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded!\n");
    
    if((listen(sockfd, 5)) != 0){
        printf("Listen failed...\n");
    }
    else
        printf("Server is listening!");
    
    len = sizeof(cli);

    connfd = accept(sockfd, (SA*)&cli, &len);
    if(connfd < 0) {
        printf("Server acceptance failed..\n");
        exit(0);
    }
    else
        printf("Server accepted the client!\n");

    func(connfd);

    close(sockfd);
}