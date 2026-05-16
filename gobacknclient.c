#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;

    struct sockaddr_in serverAddr;

    int totalFrames, windowSize;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd,
            (struct sockaddr *)&serverAddr,
            sizeof(serverAddr));

    printf("Enter total number of frames: ");
    scanf("%d", &totalFrames);

    printf("Enter window size: ");
    scanf("%d", &windowSize);

    int base = 0;

    while (base < totalFrames)
    {
        printf("\nSending Window: ");

        // Send all frames in current window
        for (int i = base;
             i < base + windowSize && i < totalFrames;
             i++)
        {
            printf("%d ", i);

            write(sockfd, &i, sizeof(i));
        }

        printf("\n");

        int ack;
        int success = 1;

        // Receive ACKs
        for (int i = base;
             i < base + windowSize && i < totalFrames;
             i++)
        {
            read(sockfd, &ack, sizeof(ack));

            if (ack == i)
            {
                printf("ACK received for Frame %d\n", i);
            }
            else
            {
                printf("Timeout/Error at Frame %d\n", i);

                printf("Retransmitting from Frame %d\n", i);

                base = i;

                success = 0;

                break;
            }
        }

        // Slide window if all ACKs received
        if (success)
        {
            base += windowSize;
        }
    }

    int end = -1;

    write(sockfd, &end, sizeof(end));

    printf("\nAll Frames Sent Successfully\n");

    close(sockfd);

    return 0;
}