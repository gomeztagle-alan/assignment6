/*
 * Alan Gomez-Tagle & Kelsey Erb
 * client.c
 * ===============================
 * Usage: Requires modified version of echoserveri.c & xml_request.txt file
 * Launch server first!
 * ../echoserveri 8080 &
 * ./client.c <num1> <num2>
 */
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd; /* File descriptor where client connection exists */
    char *num1, *num2; /* Commandline arguemnts */
    char *host, *port, buf[MAXLINE]; /* Server IP & Port for clientfd */
	rio_t rio; /* Used later for Robust IO */

	/* xml_request.txt is where xml-RPC protocol is stored */
	/* Located in the same directory as client.c */
	FILE *file = fopen("xml_request.txt", "r"); /* Open xml_request in read mode */
	if (file == NULL) { perror("Error opening file"); return 1; } /* Raise error if unable to open */
	char buffer[256]; /* Buffer to store xml-RPC protocol messages with \r\n added */

	/* Ensure proper commandline usage */
    if (argc != 3) {
        fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
		fclose(file); /* Important to close files before exiting */
        exit(0);
    }

	/* Set commandline arguements */
    num1 = argv[1];
    num2 = argv[2];

	/* Set server ip & port */
    host = "localhost";
    port = "8080";

	/* Open client file descriptor socket*/
    clientfd = Open_clientfd(host, port);

    /* 
	Write your code here.
	Recommend to use the Robust I/O package.
    */
	while (fgets(buf, sizeof(buf), file) != NULL) { /* Reads each line from xml_request */
		// TO-DO: Fix the \r\n at the end of each line
		Rio_writen(clientfd, buf, strlen(buf)); /* Sends each line to server */
	}
	//Rio_writen(clientfd, "\r\n", strlen("\r\n"));
    
    Close(clientfd); /* Closes network socket before exiting */
    exit(0);
}
