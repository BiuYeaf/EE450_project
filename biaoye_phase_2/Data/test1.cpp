// server program for udp connection
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#define PORT 5000
#define MAXLINE 1000

// Driver code
int main()
{
    int sock1;
    sock1 = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock1 < 0)
        perror("udp socket");
    struct sockaddr_in clientudp;
    memset(&clientudp, 0, sizeof(clientudp));
    clientudp.sin_family = AF_INET;
    clientudp.sin_port = htons(21734);
    clientudp.sin_addr.s_addr = htons(INADDR_ANY);
    if (bind(sock1, (struct sockaddr *)&clientudp, sizeof(clientudp)) < 0)
        perror("udp bind");
    char rec[1024] = {0};
    struct sockaddr_in peer;
    socklen_t peerlen;
    int n;
    peerlen = sizeof(peer);
    recvfrom(sock1, rec, 1024, 0, (struct sockaddr *)&peer, &peerlen);
    std::cout << rec << std::endl;
}