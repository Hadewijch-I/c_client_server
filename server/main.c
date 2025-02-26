#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void start_server() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_sock);
        return;
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listening failed");
        close(server_sock);
        return;
    }

    printf("Listening on port %d...\n", SERVER_PORT);

    while ((client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size)) >= 0) {
        while (recv(client_sock, buffer, BUFFER_SIZE, 0) > 0) {
            printf("Received data: %s\n", buffer);
            send(client_sock, "ACK", strlen("ACK"), 0);
            memset(buffer, 0, BUFFER_SIZE);
        }
        close(client_sock);
    }

    close(server_sock);
}

int main() {
    start_server();
    return 0;
}
