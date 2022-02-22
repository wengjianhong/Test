#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <wait.h>
using namespace std;

#define SERV_PORT 8888
#define LISTENQ 1024
#define	MAXLINE		4096	/* max text line length */
#define SA struct sockaddr
typedef struct args
{
    long arg1;
    long arg2;
}args;

typedef struct result
{
    long sum;
    long product;
    long difference;
    double quotient;
}result;

void err_quit(const char *fmt, ...) {
    std::cout << fmt << endl;
	_exit(1);
}

void str_echo(int sockfd) {
    args    d;
    result  res;
    ssize_t n;

again:
    while ((n = read(sockfd, &d, sizeof(args))) > 0){
        printf("arg1: %ld\n", d.arg1);
        printf("arg2: %ld\n", d.arg2);

        res.difference = d.arg1 - d.arg2;
        res.quotient = 1.0 * d.arg1 / d.arg2;
        res.product = d.arg1 * d.arg2;
        res.sum = d.arg1 + d.arg2;

        write(sockfd, &res, sizeof(result));
        memset(&res, 0, sizeof(result));
        memset(&d, 0, sizeof(args));
    }

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        err_quit("str_echo: read error");
    else
        cout <<"n: " << n << ", errno: " << errno << endl;
}

void sig_chld(int signo){
    pid_t pid;
    int   stat;
    while((pid=waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main(int argc, char **argv) { 
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    signal(SIGCHLD, sig_chld);

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *)&cliaddr, &clilen);

        if ((childpid = fork()) == 0) {
            cout << "connected..." << endl;
            close(listenfd);
            str_echo(connfd);
            _exit(0);
        }
        close(connfd);
    }
}