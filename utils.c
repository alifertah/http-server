#include "server.h"

char *read_file(char* filename){
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("file open failed");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *content = malloc(file_size + 1);
    if(!content){
        perror("malloc failed allocation space!");
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);

    return content;
}


int create_server_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void bind_server_socket(int server_fd, struct sockaddr_in *address) {
    if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void handle_client_request(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Request:\n%s\n", buffer);

    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    send_response(client_socket, path);
}

void send_response(int client_socket, const char *path) {
    char file_path[BUFFER_SIZE];
    snprintf(file_path, BUFFER_SIZE, ".%s", path);

    char *content = read_file(file_path);
    if (!content) {
        const char *error_response =
            "HTTP/1.1 404 Not Found\n"
            "Content-Type: text/plain\n\n"
            "File not found.";
        send(client_socket, error_response, strlen(error_response), 0);
        return;
    }

    char header[BUFFER_SIZE];
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\n"
             "Content-Type: text/html\n"
             "Content-Length: %ld\n\n",
             strlen(content));
    send(client_socket, header, strlen(header), 0);
    send(client_socket, content, strlen(content), 0);

    free(content);
}
