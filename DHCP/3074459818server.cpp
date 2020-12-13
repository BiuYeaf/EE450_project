// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <time.h>
#include <sstream>
#define PORT 4818
using namespace std;

string toString(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

string GetRequestIP(string buffer)
{
    size_t position = buffer.find(" ");
    string IP = buffer.substr(0, position);
    return IP;
}

string GetID(string buffer)
{
    size_t position = buffer.find(" ");
    string ID = buffer.substr(position + 1, buffer.size());
    return ID;
}

char *GetIP(int TranId)
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(TranId, (struct sockaddr *)&addr, &addr_size);
    char *clientip = new char[20];
    strcpy(clientip, inet_ntoa(addr.sin_addr));
    return clientip;
}

string CreateIP(int TranId)
{
    string IP;
    for (int i = 0; i < 3; i++)
    {
        int r = rand() % 255;
        IP = IP + toString(r) + '.';
    }
    IP = IP + toString(TranId);
    return IP;
}

string CreateTranID()
{
    string TranID;
    int r = rand() % 256;
    TranID = toString(r);
    return TranID;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    srand((unsigned)time(NULL));

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    //                &opt, sizeof(opt)))
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("server: waiting for connections...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //The first time server get the request
    char *ip = GetIP(new_socket);
    char buffer1[1024]={0};
    printf("server: got connection form %s\n", ip);
    valread = read(new_socket, buffer1, 1024);
    string TranID = buffer1;
    printf("Received the following Transaction ID from client: %s\n", TranID.c_str());

    //Server send the offered IP
    string IP = CreateIP(stoi(TranID));
    TranID = CreateTranID();
    string first = IP+' '+TranID;
    printf("Sending the following to Client:");
    send(new_socket, first.c_str(), strlen(first.c_str()), 0);
    printf("%s\n", IP.c_str());
    printf("Transaction ID: %s\n",TranID.c_str());

    //Server get the request
    char buffer2[1024]={0};
    valread = read(new_socket, buffer2, 1024);
    printf("Received the following request: \n");
    string Request=buffer2;
    printf("Transaction ID: %s\n", GetID(Request).c_str());

    //Server send the ACK
    printf("Sending following acknowledgment: \n");
    string RequestIP=GetRequestIP(Request);
    TranID = CreateTranID();
    string acknowledgment = RequestIP+" "+TranID;
    send(new_socket, acknowledgment.c_str(), strlen(acknowledgment.c_str()), 0);
    printf("IP address: %s\n",RequestIP.c_str());
    printf("Transaction ID: %s\n",TranID.c_str());

    return 0;
}