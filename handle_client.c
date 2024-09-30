#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "handle_client.h"

//Function to handle the client

void handle_client(int client_socket){
  char buffer[256];
  char server_message[256] = "You have reached the server";
  int bytes_reiceved;

  //Send data to the client
  send(client_socket, server_message, sizeof(server_message), 0);

  //Receive data from the client
  while((bytes_reiceved = recv(client_socket, buffer, sizeof(buffer), 0)) > 0){
    //Print the data received from the client
    buffer[bytes_reiceved] = '\0';
    printf("Client: %s\n", buffer);

    //Send the data back to the client
    send(client_socket, buffer, bytes_reiceved, 0);
  }

  //Close the socket
  close(client_socket);
}
