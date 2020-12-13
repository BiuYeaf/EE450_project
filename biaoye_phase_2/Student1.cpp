// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include <fstream>

#define HEALTH_CENTER_PORT 6818
#define UDP 22218

std::vector<std::string> readInfo()
{
    std::vector<std::string> res;

    std::ifstream file("input/student1.txt");
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
    //connected
    struct sockaddr_in current;
    socklen_t len = sizeof(current);
    getsockname(sock, (struct sockaddr *)&current, &len);
    std::cout << "<Student 1> has TCP port " << ntohs(current.sin_port) << " and IP address " << inet_ntoa(current.sin_addr) << std::endl;

    //read information and send it
    std::vector<std::string> hospitalInfo = readInfo();
    std::string x;
    x.append("Student1:" + hospitalInfo[0]);
    send(sock, x.c_str(), strlen(x.c_str()), 0);
    for (int i = 1; i < hospitalInfo.size(); i++)
    {
        std::string x;
        x.append(hospitalInfo[i]);
        send(sock, x.c_str(), strlen(x.c_str()), 0);
    }
    //connected end
    printf("Completed sending application for <Student 1>\n");

    //receive the reply
    char buffer[1024] = {0};
    valread = read(sock, buffer, 1024);

    if (buffer[0] == '1')
    {
        printf("<Student 1> has received the reply from the health center\n");
        close(sock);

        //set the udp
        char buffer[100];
        int listenfd, len;
        struct sockaddr_in servaddr, cliaddr;
        bzero(&servaddr, sizeof(servaddr));

        // Create a UDP Socket
        listenfd = socket(AF_INET, SOCK_DGRAM, 0);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(UDP);
        servaddr.sin_family = AF_INET;

        // bind server address to socket descriptor
        bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        std::cout << "<Student 1> has UDP port " << htons((servaddr.sin_port)) << " and IP address " << inet_ntoa(servaddr.sin_addr) << std::endl;
        //receive the datagram
        len = sizeof(cliaddr);
        int n = recvfrom(listenfd, buffer, sizeof(buffer),
                         0, (struct sockaddr *)&cliaddr, (socklen_t *)&len); //receive message from server
        if (buffer[0] == 'N')
        {
            printf("<Student 1> has received the application result, the result is: rejected\n");
        }
        else
            printf("<Student 1> has received the application result, the result is: %s\n", buffer);
    }

    return 0;
}
