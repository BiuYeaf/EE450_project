#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 5000
#define MAXLINE 1000

// Driver code
int main()
{
    const char *be;
    int s, t;
    char b[5] = "3";
    int udp_sock;
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if ( udp_sock< 0)
        perror("udp socket");
    struct sockaddr_in udp_server;
    memset(&udp_server, 0, sizeof(udp_server));
    udp_server.sin_family = AF_INET;
    udp_server.sin_port = htons(21734);
    udp_server.sin_addr.s_addr = INADDR_ANY;
    // connect(udp_sock, (struct sockaddr *)&udp_server, sizeof(udp_server));
    //if(t==-1)
    //perror("Connect");
    sendto(udp_sock, "rtrt", 4, 0, (struct sockaddr *)&udp_server, sizeof(udp_server));
}