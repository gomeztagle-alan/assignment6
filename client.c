/* Alan Gomez-Tagle, Kelsey Erb
 * client.c
 * =============================
 * [NOTE FOR GRADER]  Needs xml_request.txt in the local directory to work
 * - Also, needs doubles in commandline args
 */
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include <time.h>

/*
 * function: parse_response
 * ========================
 * Searches through RPC-XML response from server for return values
 * Modifies double array passed in by reference with values
 * Return type void 
 */
void parse_response(const char *response, double *values) {
	const char *start = strstr(response, "<double>");
	if (start) {
		// First occurence of float between <double></double> tags
		sscanf(start, "<double>%lf</double>", &values[0]);
		start = strstr(start + 1, "<double>");
		if (start) {
		// Second occurence of float between <double></double> tags
			sscanf(start, "<double>%lf</double>", &values[1]);
		}
	}
}

int main(int argc, char **argv) 
{

	/* === VARS === */

    int clientfd; /* File descriptor for socket */
    char *num1, *num2; /* Commandline arguements */
    char *host, *port; /* Host info */
	char src[MAXLINE]; /* Buffer for reading from xml_request.txt */
	char dest[MAXLINE]; /* Buffer for writting (RIO) through socket - client request */
	char buf[MAXLINE]; /* Buffer for reading (RIO) through socket - server reply*/
	char buff_write[MAXLINE]; /* Buffer for modifying arguement line with values */
	char reply[MAXLINE]; /* Buffer for parsing reply from server */
	double numbers[2] = {0.0, 0.0}; /* Array for float return values */
	rio_t rio; /*  RIO buffer for reading socket robustly */


	/* === SETUP === */

	// Open xml_request to read
	FILE *file = fopen("xml_request.txt", "r");
	if (file == NULL) { perror("Error opening file: xml_request.txt"); return 1; }

	// Commandline args
    if (argc != 3) {
        fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
        exit(0);
    }
    num1 = argv[1];
    num2 = argv[2];

	// Define host info
    host = "localhost";
    port = "8080";

	// Open socket connection
    clientfd = Open_clientfd(host, port);
	// Setup read buffer through socket
	Rio_readinitb(&rio, clientfd);

	/* === GENERATE HEADERS === */
	int n = 1;
	// Read each line of xml_request & construct request
	while (Fgets(src, MAXLINE - 1, file) != NULL) {
		size_t len = strlen(src);
		// Headers get \r\n
		if (src[0] != '<') {
			src[len - 1] = '\r';
			src[len] = '\n';
			src[len + 1] = '\0';
		}

		// Values modified and written to dest buffer when line 13 & 16 read
		if (n == 13) {
			snprintf(buff_write, sizeof(char) * ( 34+strlen(num1) ),"<value><double>%s</double></value>\n", num1);
			strncat(dest, buff_write, strlen(buff_write));
			n++;
			continue; // Skip iteration
		
		}
		if (n == 16) {
			snprintf(buff_write, sizeof(char) * ( 34+strlen(num2) ), "<value><double>%s</double></value>\n", num2);
			strncat(dest, buff_write, strlen(buff_write));
			n++;
			continue; // Skip iteration
		}

		// Write headers & body to dest buffer
		strncat(dest, src, strlen(src));
		n++;
	} 

	/* === SEND REQUEST TO SERVER === */
	// Write dest buffer to socket
	Rio_writen(clientfd, dest, strlen(dest));
	

	/* === PROCESS REPLY === */

	int len; // Length of line read from rio buffer
	while( (len = Rio_readlineb(&rio, buf, MAXLINE)) != 0 ) {
		// Construct reply buffer to be parsed
		strncat(reply, buf, len);
	}

	// Retrieve numbers from reply
	parse_response(reply, numbers);

	// Return values to stdout
	printf("\nresult: %.1lf %.1lf\n", numbers[0], numbers[1]);
    
	/* === CLOSE FILES == */
	// Close read file
	fclose(file);
	// Close connection with server
    Close(clientfd);
    exit(0);
}
