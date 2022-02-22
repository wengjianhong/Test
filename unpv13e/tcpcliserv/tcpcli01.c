#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define SA          struct sockaddr
#define SERV_PORT   8888
#define MAXLINE     4096   /* max text line length */
using namespace std;

void err_quit(const char *fmt, ...) {
    std::cout << fmt << endl;
    _exit(1);
}

void str_cli(FILE *fp, int sockfd) {
    char    sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        write(sockfd, sendline, strlen(sendline));
        if (read(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");

        fputs(recvline, stdout);
        memset(recvline, 0, sizeof(recvline));
    }
}

int main(int argc, char** argv){
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2){
        err_quit("usage: tcpcli <IP ADDRESS>");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    _exit(0);
}
