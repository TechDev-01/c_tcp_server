#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

int main(){
  //Create the structure for the sever client
  struct sockaddr_in server_client;
  
  //Create the buffer to store the message
  char server_message[256];

  //Create the client socket
  int client_socket; //File descriptor returned from the socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  
  if (client_socket < 0){
    printf("Error creating the client socket\n");
    exit(1);
  }
  
  /*The first and second parameters are the family and the port where the client will connect
   * anf the third parameter is the IP address of the server_client
  */
  //Set the server_client structure
  server_client.sin_family = AF_INET;
  server_client.sin_port = htons(9002);
  server_client.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  //Connect to the server
  int connection_status = connect(client_socket, (struct sockaddr *) &server_client, sizeof(server_client));
  
  //Check for error with the connection
  if (connection_status == -1){
    printf("Error connecting to the remote socket\n");
    exit(1);
  }

  //Receive the data from the server
  recv(client_socket, &server_message, sizeof(server_message), 0);

  //Print the data received from the server
  printf("The server sent the data: %s\n", server_message);
  
  //Close the socket
  close(client_socket);
  return 0;
}
