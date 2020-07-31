#include <sys/socket.h> // Generic address structure is defined in this library
#include <sys/types.h>
#include <netinet/in.h> //sockaddr_in is defined in this library.
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int point,socket_descriptor, port_number;
	char server_message[255], s_point[255] ;
	struct sockaddr_in server_address; //IPv4 socket declaration
	if(argc!=4)
		{
			printf("Call Model is <Enter player name> <IP> <Port>\n");
			exit(0);
		}
	if((socket_descriptor=socket(AF_INET,SOCK_STREAM,0))<0) //create a socket, 0 is given as a protocol
		{
			perror("\n Socket creation failed.\n");
			exit(1);
		}
	server_address.sin_family = AF_INET;//Assigning IP and Port
	sscanf(argv[3],"%d", &port_number);
	server_address.sin_port = htons((uint16_t)port_number);
	if(inet_pton(AF_INET,argv[2],&server_address.sin_addr) < 0)//converts IPV4 address from presentation to network format.
		{
			fprintf(stderr, "inet_pton() has failed\n");
			exit(1);
		}
	if(connect(socket_descriptor,(struct sockaddr *) &server_address, sizeof(server_address))!=0)//Establish connection to server
		{
			fprintf(stderr, "connect() failed, exiting\n");
			exit(1);
		}
	printf("You are sucessfully connected to the server.\n");
	write(socket_descriptor,argv[1],strlen(argv[1])+1);//sending name to server.
	while(1)
	{
		if(read(socket_descriptor, server_message, 255) < 0)//reading message from server
			{
				fprintf(stderr, "read() error\n");
				exit(1);
			}
		if((strcmp(server_message,"You can now play") == 0))
			{
				printf("\n----------------------------\n");
				printf("%s",server_message);
				point = (int)time(NULL) % 10 + 1;//Generate random numbers by using time
				sprintf(s_point, "%d", point);//getting formatted output and store it in s_point as a string
				printf("\nObtained score of %s is : %s",argv[1],s_point);
				printf("\n----------------------------\n\n");
				strcpy(server_message,s_point);
				write(socket_descriptor, server_message, strlen(server_message)+1);
			}
		if((strcmp(server_message,"Game over: you won the game") == 0))//check if won game
			{
				printf("\n\n\n==================\n");
				printf("| I won the game |\n");
				printf("==================\n\n\n\n");
				close(socket_descriptor);
				exit(0);
			}
		if((strcmp(server_message,"Game over: you lost the game") == 0))//check if lost game
			{
				printf("\n\n\n===================\n");
				printf("| I lost the game |\n");
				printf("===================\n\n\n\n");
				close(socket_descriptor);
				exit(0);
			}
	}
	return 0;
}