#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];

    // Send greeting
    send(client_socket, "220 SMTP Server Ready\n", 22, 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Client: %s", buffer);

        if (strncmp(buffer, "HELO", 4) == 0) {
            send(client_socket, "250 Hello\n", 10, 0);
        }

        else if (strncmp(buffer, "MAIL FROM", 9) == 0) {
            send(client_socket, "250 OK\n", 7, 0);
        }

        else if (strncmp(buffer, "RCPT TO", 7) == 0) {
            send(client_socket, "250 OK\n", 7, 0);
        }

        else if (strncmp(buffer, "DATA", 4) == 0) {
            send(client_socket, "354 End data with <CRLF>.<CRLF>\n", 32, 0);

            printf("Email message:\n");

            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                recv(client_socket, buffer, BUFFER_SIZE, 0);

                if (strcmp(buffer, ".\n") == 0)
                    break;

                printf("%s", buffer);
            }

            send(client_socket, "250 Message accepted\n", 21, 0);
        }

        else if (strncmp(buffer, "QUIT", 4) == 0) {
            send(client_socket, "221 Bye\n", 8, 0);
            break;
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("SMTP Server running on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        printf("Client connected\n");
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}