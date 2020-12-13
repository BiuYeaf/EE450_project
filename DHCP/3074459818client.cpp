// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sstream>
#include <string>
#define PORT 4818
using namespace std;

string toString(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

string CreateTranID()
{
    string TranID;
    int r = rand() % 256;
    TranID = toString(r);
    return TranID;
}

string GetIP(string buffer)
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

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    srand((unsigned)time(NULL));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    std::string dhcpIP = std::string(argv[1]);
    if (strcmp(argv[1], "localhost") == 0)
    {
        dhcpIP = "127.0.0.1";
    }
    if (inet_pton(AF_INET, dhcpIP.c_str(), &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    //Build the connection
    printf("client: connecting to 127.0.0.1\n");

    //Send the first request
    string first = "53";
    send(sock, first.c_str(), strlen(first.c_str()), 0);
    printf("Sending the following Transaction ID to server: %s\n", first.c_str());

    //Get the first response
    char buffer1[1024] = {0};
    valread = read(sock, buffer1, 1024);
    string FirstGet = buffer1;
    printf("Received the following:\n");
    string IP = GetIP(FirstGet);
    printf("IP address: %s\n", IP.c_str());
    string TranID = GetID(FirstGet);
    printf("Transsaction ID: %s\n", TranID.c_str());

    //send the request
    printf("Formally requesting the following server:\n");
    string secondID = CreateTranID();
    string second = IP + " " + secondID;
    send(sock, second.c_str(), strlen(second.c_str()), 0);
    printf("IP address: %s\n", IP.c_str());
    printf("Transaction ID: %s\n", secondID.c_str());

    //get the IP
    char buffer2[1024] = {0};
    valread = read(sock, buffer2, 1024);
    string SecondGet = buffer2;
    IP = GetIP(SecondGet);
    TranID = GetID(SecondGet);
    printf("Officially connected to IP Address: %s\n", IP.c_str());
    printf("client: received '%s'\n", TranID.c_str());

    return 0;
}