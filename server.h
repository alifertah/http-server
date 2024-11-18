#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char *read_file(char* filename);
int create_server_socket();
void bind_server_socket(int server_fd, struct sockaddr_in *address);
void handle_client_request(int client_socket);
void send_response(int client_socket, const char *path);


#endif