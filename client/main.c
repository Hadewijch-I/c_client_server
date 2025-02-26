#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> 

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
const char *server_name = "c_server";

void send_user_data(const char *csv_file, const char *log_file) {
    FILE *file = fopen(csv_file, "r");
    if (!file) {
        perror("Failed to open CSV file");
        return;
    }

    FILE *log = fopen(log_file, "a");
    if (!log) {
        perror("Failed to open log file");
        fclose(file);
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        fclose(file);
        fclose(log);
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    struct hostent *host = gethostbyname(server_name);
    if (host == NULL) {
        perror("gethostbyname");
        fclose(file);
        fclose(log);
        close(sock);
        return;
    }

    memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        fclose(file);
        fclose(log);
        close(sock);
        return;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("sending data: %s\n", buffer);
        send(sock, buffer, strlen(buffer), 0);
        recv(sock, buffer, sizeof(buffer), 0);
        fprintf(log, "Acknowledged: %s ", buffer);
    }

    fclose(file);
    fclose(log);
    close(sock);
}

int main() {
    send_user_data("users.csv", "log.txt");
    return 0;
}