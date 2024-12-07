/*
 * client.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include <time.h>

int main(int argc, char **argv) 
{
    int clientfd;
    char *num1, *num2;
    char *host, *port;
	char src[MAXLINE - 2];
	char dest[MAXLINE];
	char buf[MAXLINE];
	rio_t rio;

	/* Read file */
	FILE *file = fopen("xml_request.txt", "r");
	if (file == NULL) { perror("Error opening file"); return 1; }

    if (argc != 3) {
        fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
        exit(0);
    }

    num1 = argv[1];
    num2 = argv[2];

    host = "localhost";
    port = "8080";

    clientfd = Open_clientfd(host, port);
	Rio_readinitb(&rio, clientfd);

    /* 
	Write your code here.
	Recommend to use the Robust I/O package.
    */
	int i = 1;
	while (Fgets(src, MAXLINE-2, file) != NULL) {
		sprintf(dest, "%s\r", src);
		Rio_writen(clientfd, dest, strlen(dest));
		Rio_readlineb(&rio, buf, MAXLINE);
//		Fputs("%d: ", i++, stdout);
		Fputs(buf, stdout);
		//sleep(5);
	}
	//Rio_writen(clientfd, "\r\n", strlen("\r\n"));


    
    Close(clientfd);
    exit(0);
}
