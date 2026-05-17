#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 50

int main()
{
    int sockfd, newsock;

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    int received[MAX] = {0};   // Track received frames
    int frame;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    newsock = accept(sockfd, (struct sockaddr *)&client, &len);

    while (1)
    {
        recv(newsock, &frame, sizeof(frame), 0);

        if (frame == -1)
            break;

        printf("Received Frame: %d\n", frame);

        // Simulate ACK loss
        int ack;

        printf("Send ACK? (1 = yes, 0 = lost): ");
        scanf("%d", &ack);

        if (ack == 1)
        {
            received[frame] = 1;

            // Send ACK frame number
            send(newsock, &frame, sizeof(frame), 0);
        }
        else
        {
            int nack = -1;

            // Send NACK
            send(newsock, &nack, sizeof(nack), 0);
        }
    }

    close(newsock);
    close(sockfd);

    return 0;
}