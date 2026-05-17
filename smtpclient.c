#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // HELO
    strcpy(buffer, "HELO localhost\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // MAIL FROM
    strcpy(buffer, "MAIL FROM:<sender@example.com>\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // RCPT TO
    strcpy(buffer, "RCPT TO:<receiver@example.com>\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // DATA
    strcpy(buffer, "DATA\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // Message body
    printf("Enter email message (end with single '.' line):\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sockfd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, ".\n") == 0)
            break;
    }

    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // QUIT
    strcpy(buffer, "QUIT\n");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    close(sockfd);
    return 0;
}