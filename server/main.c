#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void create_table(MYSQL *conn) {
    const char *create_table_query = 
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id INT PRIMARY KEY, "
        "first_name VARCHAR(50) NOT NULL, "
        "last_name VARCHAR(50) NOT NULL, "
        "email VARCHAR(100) NOT NULL UNIQUE, "
        "city VARCHAR(50) NOT NULL);";

    if (mysql_query(conn, create_table_query)) {
        fprintf(stderr, "MySQL query failed: %s\n", mysql_error(conn));
    } else {
        printf("Table 'users' created successfully.\n");
    }
}

void store_user_data(char *data) {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "MySQL initialization failed\n");
        return;
    }
    
    if (!mysql_real_connect(conn, "db-mysql", "myuser", "user_password", "mydatabase", 3306, NULL, 0)) {
        fprintf(stderr, "MySQL connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }
    static int called = 0;
    if(!called) {
        printf("setting up table\n");
        create_table(conn);
        called = 1;
    }

    char first_name[50], last_name[50], email[100], city[50];
    int user_id;

    sscanf(data, "%d,%49[^,],%49[^,],%99[^,],%49[^,]", &user_id, first_name, last_name, email, city);

    char query[BUFFER_SIZE];
    snprintf(query, sizeof(query), "INSERT INTO users (user_id, first_name, last_name, email, city) VALUES (%d, '%s', '%s', '%s', '%s')",
             user_id, first_name, last_name, email, city);
    

    if (mysql_query(conn, query)) {
        fprintf(stderr, "MySQL query failed: %s\n", mysql_error(conn));
    }
    
    mysql_close(conn);
}

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
            store_user_data(buffer);
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
