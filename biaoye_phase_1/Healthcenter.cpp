// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>

#define PORT 6818
void do_service(int new_socket)
{
    char recvbuf[1024];
    int valread;
    char buffer[1024] = {0};
    int flag = 1;
    while (1)
    {
        valread = int(read(new_socket, buffer, 1024));
        if (valread == 0)
        {
            printf("Received the department list from <Hospital %c>\n", buffer[0]);
            // store the information into the txt documents
            if (buffer[0] == 'A')
            {
                std::ofstream file("A.txt");
                if (file.is_open())
                {
                    file << buffer << std::endl;
                    file.close();
                }
            }

            if (buffer[0] == 'B')
            {
                std::ofstream file("B.txt");
                if (file.is_open())
                {
                    file << buffer << std::endl;
                    file.close();
                }
            }

            if (buffer[0] == 'C')
            {
                std::ofstream file("C.txt");
                if (file.is_open())
                {
                    file << buffer << std::endl;
                    file.close();
                }
            }

            if (buffer[0] == 'C')
            {
                //sleep(5);
                printf("End of Phase 1 for the health center\n");
            }
            break;
        }
    }
}
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    std::string HA;
    std::string HB;
    std::string HC;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 6818
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in host;
    int len = sizeof(host);
    getsockname(opt, (struct sockaddr *)&host, (socklen_t *)sizeof(len));

    //printf specific information of the healthcenter
    std::cout << "The health center has TCP port " << htons((address.sin_port)) << " and IP address " << inet_ntoa(address.sin_addr) << std::endl;

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        //waiting for connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        int pid = fork();
        if (pid == -1)
            perror("fork error");
        if (pid == 0)
        {
            // child process
            close(server_fd);
            //handle the process
            do_service(new_socket);
            exit(EXIT_SUCCESS);
        }
        else
            close(new_socket); //parent process
    }
    return 0;
}
