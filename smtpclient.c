#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // HELO
    strcpy(buffer, "HELO localhost\n");
    send(client_socket, buffer, strlen(buffer), 0);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // MAIL FROM
    strcpy(buffer, "MAIL FROM:<sender@example.com>\n");
    send(client_socket, buffer, strlen(buffer), 0);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // RCPT TO
    strcpy(buffer, "RCPT TO:<receiver@example.com>\n");
    send(client_socket, buffer, strlen(buffer), 0);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // DATA
    strcpy(buffer, "DATA\n");
    send(client_socket, buffer, strlen(buffer), 0);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // Message body
    printf("Enter email message (end with single '.' line):\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, ".\n") == 0)
            break;
    }

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // QUIT
    strcpy(buffer, "QUIT\n");
    send(client_socket, buffer, strlen(buffer), 0);
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    close(client_socket);
    return 0;
}