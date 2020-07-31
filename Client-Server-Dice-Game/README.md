# Client-Server-Dice-Game
* A client-server application on which multiple clients can roll dice and server respond to the client according to their scores.
* The server process and the client process will run on two different machines and the communication between the two processes is achieved using Sockets.
* Once any client receives 100 score server declares that client as winner and stop that game.
# Instructions
1. The server must start running before any client, and goes into an infinite loop to wait for other clients.
2. Run server.c file using gcc server.c command. Call Model for server.c is {Port}
3. Run client.c file using gcc client.c command. Call Model for client.c is {Enter player name} {IP} {Port}
4. Server waits for another client. When the server gets the other client (now two clients), it forks and, lets the child process take care of these two clients (players) in a separate function, called servicePlayers, while the parent process goes back to wait for the next two clients (players).
5. The server’s child process will be a referee for the two clients. If one or both accumulated scores (totals) reaches 100, the referee sends "Game over: you won the game" to the winner and sends "Game over: you lost the game" to the player who lost and closes both sockets.
