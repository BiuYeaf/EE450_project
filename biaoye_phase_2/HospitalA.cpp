// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include <fstream>

#define HEALTH_CENTER_PORT 6818
#define UDP 21918

std::vector<std::string> readInfo()
{
    std::vector<std::string> res;

    std::ifstream file("input/HospitalA.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            // store the information
            res.push_back(line);
        }
        file.close();
    }

    return res;
}

int main(int argc, char const *argv[])
{

    int sock = 0, valread;
    struct sockaddr_in clientA_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    clientA_addr.sin_family = AF_INET;
    clientA_addr.sin_port = htons(HEALTH_CENTER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &clientA_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&clientA_addr, sizeof(clientA_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    //get the current port of the client
    struct sockaddr_in current;
    socklen_t len = sizeof(current);
    getsockname(sock, (struct sockaddr *)&current, &len);
    std::cout << "<Hospital A> has TCP port " << ntohs(current.sin_port) << " and IP address " << inet_ntoa(current.sin_addr) << " for Phase 1" << std::endl;

    //connected
    printf("<Hospital A> is now connected to the health center\n");

    //read information and send it
    std::vector<std::string> hospitalInfo = readInfo();
    for (int i = 0; i < hospitalInfo.size(); i++)
    {
        std::string x;
        x.append("HospitalA:");
        x.append(hospitalInfo[i]);
        printf("<Hospital A> has sent <%c%c> to the health center\n", hospitalInfo[i][0], hospitalInfo[i][1]);
        send(sock, x.c_str(), strlen(x.c_str()), 0);
    }
    //connected end
    printf("Updating the health center is done for <Hospital A>\n");
    printf("End of Phase 1 for <Hospital A>\n");
    close(sock);

    //phase2
    char buffer[100];
    int listenfd, len1;
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = hton cs(UDP);
    servaddr.sin_family = AF_INET;

    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    std::cout << "<Hospital A> has UDP port " << htons((servaddr.sin_port)) << " and IP address " << inet_ntoa(servaddr.sin_addr) << " for Phase2"<<std::endl;
    //receive the datagram
    len1 = sizeof(cliaddr);
    int n = recvfrom(listenfd, buffer, sizeof(buffer),
                     0, (struct sockaddr *)&cliaddr, (socklen_t *)&len1); //receive message from server
    printf("<Student %c> has been admitted to <Hospital A>\n",buffer[0]);

    return 0;
}
