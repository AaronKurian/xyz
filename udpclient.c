#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[1024] = "Hello from client";
    socklen_t len = sizeof(server);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address configuration
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address

    // Send message to server
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server, len);
    printf("Message sent to server\n");

    // Receive response from server
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);
    printf("Message from server: %s\n", buffer);

    close(sockfd);
    return 0;
}