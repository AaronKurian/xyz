#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int newsock)
{
    char buffer[BUFFER_SIZE] = {0};
    char filename[BUFFER_SIZE] = {0};

    int bytes_received = recv(newsock, filename, BUFFER_SIZE, 0);

    if (bytes_received < 0)
    {
        perror("Error receiving filename");
        close(newsock);
        return;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        char *error_message = "File not found";
        send(newsock, error_message, strlen(error_message), 0);
        perror("File not found");
    }
    else
    {
        while (fgets(buffer, BUFFER_SIZE, file) != NULL)
        {
            send(newsock, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
        }

        printf("File sent to client\n");
        fclose(file);
    }

    close(newsock);
}

int main()
{
    int sockfd, newsock;

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1)
    {
        newsock = accept(sockfd, (struct sockaddr *)&client, &len);

        if (newsock < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        handle_client(newsock);
    }

    close(sockfd);

    return 0;
}