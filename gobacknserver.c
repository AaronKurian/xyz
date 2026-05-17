#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in server, client;
    socklen_t addr_size;

    int frame, expected_frame = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    listen(sockfd, 5);

    printf("Waiting for connection...\n");

    addr_size = sizeof(client);

    newsockfd = accept(sockfd, (struct sockaddr *)&client, &addr_size);

    while (1)
    {
        recv(newsockfd, &frame, sizeof(frame), 0);

        if (frame == -1)
            break;

        printf("Received Frame: %d\n", frame);

        // Simulate ACK loss
        int send_ack;

        printf("Send ACK for frame %d ? (1=yes,0=lost): ", frame);
        scanf("%d", &send_ack);

        if (frame == expected_frame && send_ack == 1)
        {
            printf("ACK Sent for frame %d\n", frame);

            send(newsockfd, &frame, sizeof(frame), 0);

            expected_frame++;
        }
        else
        {
            int nack = -1;

            printf("ACK Lost for frame %d\n", frame);

            send(newsockfd, &nack, sizeof(nack), 0);
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}