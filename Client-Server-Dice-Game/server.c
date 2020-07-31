#include <sys/socket.h> // Generic address structure is defined in this library
#include <sys/types.h>
#include <netinet/in.h> //sockaddr_in is defined in this library.
#include <stdio.h>
#include <netdb.h> //Definitions for network database operations
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void servicePlayers(int, int);
void servicePlayers(int player1, int player2)
{
	char name1[255], name2[255],client_message[255];
	int total_score[2]={0,0}, received_score, max=100;
	if(read(player1, name1, 255)<0)//Reading Player1 fd if fail then close connection and print error
		{
			close(player1);
			fprintf(stderr,"Client data not received\n");
			exit(0);
		}
	if(read(player2, name2, 255)<0)//Reading Player2 fd if fail then close connection and print error
		{
			close(player2);
			fprintf(stderr,"Client data not received\n");
			exit(0);
		}
	while(1)
	{
		strcpy(client_message,"You can now play");
		write(player1, client_message, strlen(client_message)+1);//Send client_message to Player1
		//Player1 Score
		if(read(player1, client_message, 255)<0)
			{
					close(player1);
					fprintf(stderr,"Client data not received\n");
					exit(0);
			}
		sscanf(client_message,"%d",&received_score);//getting formatted output converting client_message into int and storing in received_score
		total_score[0] = total_score[0] + received_score;
		printf("%s : %d",name1,total_score[0]);
		sleep(1);
		
		strcpy(client_message, "You can now play");
		write(player2, client_message, strlen(client_message)+1);//Send client_message to Player2
		//Player2 Score
		if(read(player2, client_message, 255)<0)
		{
    			close(player2);
    			fprintf(stderr,"Client data not received\n");
				exit(0);
		}
		sscanf(client_message,"%d",&received_score);//find number from client_message string and store in received_score
		total_score[1] = total_score[1] + received_score;
		printf(" | %s : %d\n",name2,total_score[1]);
		sleep(1);
		if(total_score[0]>= max)
		{
						strcpy(client_message, "Game over: you won the game");
						write(player1, client_message, strlen(client_message)+1);
						strcpy(client_message, "Game over: you lost the game");
						write(player2, client_message, strlen(client_message)+1);
						close(player1);
						close(player2);
						printf("\n================================================================\n");
						printf("%s won the game with %d points and %s lost the game by %d points.",name1,total_score[0],name2,(total_score[0]-total_score[1]));
						printf("\n================================================================\n\n");
						printf("Waiting for new players to play a new game....\n");
						exit(0);
		}
		if(total_score[1]>= max)
		{
					strcpy(client_message, "Game over: you won the game");
					write(player2, client_message, strlen(client_message)+1);
					strcpy(client_message, "Game over: you lost the game");
					write(player1, client_message, strlen(client_message)+1);
					close(player1);
					close(player2);
					printf("\n======================================================================\n");
					printf("%s won the game with %d points and %s lost the game by %d points.",name2,total_score[1],name1,(total_score[1]-total_score[0]));
					printf("\n======================================================================\n\n");
					printf("Waiting for new players to play a new game....\n");
					exit(0);
		}
	}
}
int main(int argc, char *argv[])
{
	int player1, player2, socket_descriptor, port_number, pid;
	struct sockaddr_in server_address;//IPv4 socket declaration
	if(argc!=2)
		{
			printf("Call Model is <Port>\n");
			exit(0);
		}
	if((socket_descriptor=socket(AF_INET,SOCK_STREAM,0))<0) //create a socket, 0 is given as a protocol
		{
			perror("Socket creation fail\n");
			exit(1);
		}
	server_address.sin_family = AF_INET;// Assigning family
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);// Assigning Address
	sscanf(argv[1], "%d", &port_number);
	server_address.sin_port = htons((uint16_t)port_number);// Assigning Port
	// Assigning local protocol address to a socket by using bind.
	if(bind(socket_descriptor,(struct sockaddr *) &server_address, sizeof(server_address))!=0)
		{
			perror("Bind fail\n");//print Bind fail when port is busy.
			exit(1);
		}
	listen(socket_descriptor, 3);//3 is maximum length of pending queue
	printf("\nServer is running\n");
	//Server is going in infinite loop
	while(1)
	{
		player1=accept(socket_descriptor,(struct sockaddr *)NULL,NULL); //Player 1 is connecting
		printf("New player is waiting for another player to play a game.\n");
		player2=accept(socket_descriptor,(struct sockaddr *)NULL,NULL); //Player 2 is connecting
		printf("Another player join the game and game started.......\n");
		//Creating Process by using fork
		pid = fork();
			if(pid == 0)
				{
					servicePlayers(player1,player2);
				}
	}
	return 0;
}
