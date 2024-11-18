#include "server.h"

int main() {
    int server_fd = create_server_socket();

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind_server_socket(server_fd, &address);

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    int addrlen = sizeof(address);
    while (1) {
        printf("Waiting for a new connection...\n");
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connection established!\n");
        handle_client_request(client_socket);

        close(client_socket);
        printf("Connection closed.\n");
    }

    close(server_fd);
    return 0;
}