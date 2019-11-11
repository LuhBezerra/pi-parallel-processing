#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

struct sockaddr_in remoto;

#define PORTA 2000
#define LEN 4096
int main(){
    int sockfd;
    int slen;
    int len = sizeof(remoto);
    char buffer[4096];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd== -1){
        perror("socket ");
        exit(1);
    } 
    else {
        printf("Socket criado com sucesso\n");
    }
    
    remoto.sin_family        = AF_INET;
    remoto.sin_port          = htons(PORTA);
    remoto.sin_addr.s_addr   = inet_addr("127.0.0.1");
    memset(remoto.sin_zero, 0x0, 8);

    if(connect(sockfd, (struct sockaddr*)&remoto,  len) == -1){
        perror("connect ");
        exit(1);
    } else {
        printf("conectado com o servidor");
    }
    
    while(1){
        if((slen = recv(sockfd, buffer, LEN, 0)) > 0){
            buffer[slen-1] = '\0';
            printf("Mensagem recebida: %s\n", buffer);
        }
        memset(buffer, 0x0, LEN);
        fgets(buffer, LEN, stdin);
        send(sockfd, buffer, strlen(buffer), 0);
    }

    close(sockfd);
    printf("Client/e  encerrado\n");
    return 0;
}