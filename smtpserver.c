#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

void handle_client(int newsock) {
    char buffer[BUFFER_SIZE];

    // Send greeting
    send(newsock, "220 SMTP Server Ready\n", 22, 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(newsock, buffer, BUFFER_SIZE, 0);
        printf("Client: %s", buffer);

        if ((strncmp(buffer, "HELO", 4) == 0) || (strncmp(buffer, "EHLO", 4) == 0)) {
            send(newsock, "250 Hello\n", 10, 0);
        }

        else if (strncmp(buffer, "MAIL FROM", 9) == 0) {
            send(newsock, "250 OK\n", 7, 0);
        }

        else if (strncmp(buffer, "RCPT TO", 7) == 0) {
            send(newsock, "250 OK\n", 7, 0);
        }

        else if (strncmp(buffer, "DATA", 4) == 0) {
            send(newsock, "354 End data with <CRLF>.<CRLF>\n", 32, 0);

            printf("Email message:\n");

            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                recv(newsock, buffer, BUFFER_SIZE, 0);

                if (strcmp(buffer, ".\n") == 0)
                    break;

                printf("%s", buffer);
            }

            send(newsock, "250 Message accepted\n", 21, 0);
        }

        else if (strncmp(buffer, "QUIT", 4) == 0) {
            send(newsock, "221 Bye\n", 8, 0);
            break;
        }
    }

    close(newsock);
}

int main() {
    int sockfd, newsock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 3);

    printf("SMTP Server running on port %d\n", PORT);

    while (1) {
        newsock = accept(sockfd, (struct sockaddr *)&client, &len);
        printf("Client connected\n");
        handle_client(newsock);
    }

    close(sockfd);
    return 0;
}