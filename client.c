/*
 * client.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include <time.h>

void parse_response(const char *response, double *values) {
	const char *start = strstr(response, "<double>");
	if (start) {
		sscanf(start, "<double>%lf</double>", &values[0]);
		start = strstr(start + 1, "<double>");
		if (start) {
			sscanf(start, "<double>%lf</double>", &values[1]);
		}
	}
}

int main(int argc, char **argv) 
{
    int clientfd;
    char *num1, *num2;
    char *host, *port;
	char src[MAXLINE];
	char dest[MAXLINE];
	char buf[MAXLINE];
	char reply[MAXLINE];
	char request[MAXLINE];
	char* ptr;
	double numbers[2] = {0.0, 0.0};
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
	int n = 1;
	char buff_write[MAXLINE];
	while (Fgets(src, MAXLINE - 1, file) != NULL) {
		size_t len = strlen(src);
		if (src[0] != '<') {
			src[len - 1] = '\r';
			src[len] = '\n';
			src[len + 1] = '\0';
		}
		if (n == 13) {
			snprintf(buff_write, sizeof(char) * ( 34+strlen(num1) ),"<value><double>%s</double></value>\n", num1);
			strncat(dest, buff_write, strlen(buff_write));
			n++;
			continue;
		
		}

		if (n == 16) {
			snprintf(buff_write, sizeof(char) * ( 34+strlen(num2) ), "<value><double>%s</double></value>\n", num2);
			strncat(dest, buff_write, strlen(buff_write));
			n++;
			continue;
		}
		strncat(dest, src, strlen(src));
		n++;
	} 
	printf("send: ");
	Fputs(request, stdout);
	printf("\nsend: ");
	Fputs(dest, stdout);
	Rio_writen(clientfd, dest, strlen(dest));
	
	int len;
	while( (len = Rio_readlineb(&rio, buf, MAXLINE)) != 0 ) {
	//while(n < 20) {
		//len = Rio_readlineb(&rio, buf, MAXLINE);
		strncat(reply, buf, len);
		//printf("%d %s",len ,buf);
	}

	printf("\nreply: ");
	Fputs(reply, stdout);

	parse_response(reply, numbers);

	printf("\nresult: %.2lf %.2lf\n", numbers[0], numbers[1]);
    
    Close(clientfd);
    exit(0);
}
