#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 50

int main()
{
    int sockfd;

    struct sockaddr_in server;

    int windowSize, totalFrames;
    int ack[MAX] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Enter total frames: ");
    scanf("%d", &totalFrames);

    printf("Enter window size: ");
    scanf("%d", &windowSize);

    int base = 0;

    while (base < totalFrames)
    {
        // Send frames in window
        for (int i = base; i < base + windowSize && i < totalFrames; i++)
        {
            if (ack[i] == 0)
            {
                printf("Sending Frame %d\n", i);
                send(sockfd, &i, sizeof(i), 0);
            }
        }

        // Receive ACKs
        for (int i = base; i < base + windowSize && i < totalFrames; i++)
        {
            if (ack[i] == 0)
            {
                int response;

                recv(sockfd, &response, sizeof(response), 0);

                if (response == i)
                {
                    printf("ACK received for %d\n", i);
                    ack[i] = 1;
                }
                else
                {
                    printf("Frame %d lost --> will resend\n", i);
                }
            }
        }

        // Slide window
        while (ack[base] == 1 && base < totalFrames)
        {
            base++;
        }
    }

    int end = -1;
    send(sockfd, &end, sizeof(end), 0);

    printf("All frames sent successfully!\n");

    close(sockfd);

    return 0;
}