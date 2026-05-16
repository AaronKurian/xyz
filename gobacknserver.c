#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    int frame, expected_frame = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    listen(sockfd, 5);

    printf("Waiting for connection...\n");

    addr_size = sizeof(clientAddr);

    newsockfd = accept(sockfd,
                       (struct sockaddr *)&clientAddr,
                       &addr_size);

    while (1)
    {
        read(newsockfd, &frame, sizeof(frame));

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

            write(newsockfd, &frame, sizeof(frame));

            expected_frame++;
        }
        else
        {
            int nack = -1;

            printf("ACK Lost for frame %d\n", frame);

            write(newsockfd, &nack, sizeof(nack));
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}