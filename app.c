#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "handle_client.h"

int main(){
  
  char server_message[256] = "You have reached the server!";
  //Create the struct to hold the server address
  struct sockaddr_in server_address;
  //Create a struct to hold the client address
  struct sockaddr_in client_addr;
  int client_socket;

  //Create a socket
  /*The first argument is the domain of the socket (IPv4) AF_LOCAL is for local sockets
   * The second argument is the type of the socket (TCP) SOCK_DGRAM is for UDP
   * The third argument is the protocol (0 means use the default protocol for the type of socket)
  */
  int socket_descriptor;  //This is the file descriptor for the socket
  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  
  //Check if the socket was created successfully
  if (socket < 0){
    printf("Error creating socket\n");
    return 1;
  }

  //Set the server address
  // INADDR_ANY is used when you don't need to bind a socket to a specific IP. It will bind the socket to all available interfaces
  server_address.sin_family = AF_INET; //Define the family of the address (IPv4)
  server_address.sin_port = htons(9002); //Define the port and then convert it to network byte order (htons)
  server_address.sin_addr.s_addr = INADDR_ANY; //Define the address for the socket
  
  //Bind the socket to the IP and port
  /*The first argument is the file descruptor returned by the socket function
   *The second argument is a pointer to the address of the server
   * The third argument is the size in bytes of the address of the server, sockadrr_in is casted to sockaddr
   * */

  int bind_status = bind(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));

  //Check if the bind was successful
  if(bind_status < 0){
    perror("Error binding socket to IP and port");
    EXIT_FAILURE;
  }

  //Listen for connections on the socket
  //The second argument is the number of connections that can be waiting while the server is handling a particular connection
  int listen_status = listen(socket_descriptor, 3); 
   
  //Check if the listen was successful
  if(listen_status < 0){
    perror("Error listening on socket");
    EXIT_FAILURE;
  }
  
  //Client socket information
  /* the "socklen_t" type is a parameter used to store the size of the address of the client (struct sockaddr_in)
  * */
  socklen_t client_address_size = sizeof(client_addr);

  while(1) {
    //Accept a connection on the socket.
    /*The second and third arguments are pointers to the address of the client and the length of the address of the client.
    */
    client_socket = accept(socket_descriptor, (struct sockaddr *) &client_addr, &client_address_size); 
    
    //Create a child process to handle the client
    if (client_socket >= 0) {
      //Cast the IP client to a string
      /*The first argument is the address family (IPv4)
      * The second argument is the address of the client 
      * The third argument is the buffer to store the IP
      * and the fourth argument is the size of the buffer
      * */
      char client_ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        
        //The ntohs function converts the port to host byte order
        //Get the port of the client
      
      printf("Connection from: %s:%d\n", client_ip, ntohs(client_addr.sin_port));
      //Create a child process
      if (fork() == 0) { //This is the child process
        //Handle the client
        handle_client(client_socket);
        exit(0);
      } else {
        //Close the client socket in the parent process
        close(client_socket);
      }
    }
    
}

  //Send data to the client
  /*The first argument is the file descriptor of the socket
   *  The second argument is the variable wich stores the message
   *  The third argument is the size of the message
   *  The fourth argument is the flags (0 means no flags)
   */
  send(client_socket, server_message, sizeof(server_message), 0);

  //Close the socket
  close(socket_descriptor);
  return 0;
}
