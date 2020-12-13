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
#include <vector>
#include <arpa/inet.h>

#define PORT 6818
#define UDP_A 21918
#define UDP_B 22018
#define UDP_C 22118
#define UDP_1 22218
#define UDP_2 22318
#define UDP_3 22418
#define UDP_4 22518
#define UDP_5 22618
#define student 5
#define hospital 3

std::vector<std::string> split(const std::string &s, const std::string &seperator)
{
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    string_size i = 0;

    while (i != s.size())
    {
        int flag = 0;
        while (i != s.size() && flag == 0)
        {
            flag = 1;
            string_size x = 0;
            for (; x < seperator.size(); x++)
            {
                if (s[i] == seperator[x])
                {
                    i++;
                    flag = 0;
                    break;
                }
            }

            flag = 0;
            string_size j = i;
            while (j != s.size() && flag == 0)
            {
                for (string_size x = 0; x < seperator.size(); x++)
                    if (s[j] == seperator[x])
                    {
                        flag = 1;
                        break;
                    }
                if (flag == 0)
                    j++;
            }
            if (i != j)
            {
                result.push_back(s.substr(i, j - i));
                i = j;
            }
        }
    }
    return result;
}

//analyze the data from hospital
std::vector<std::string> get_data_from_hospital()
{
    std::vector<std::string> res;
    std::vector<std::string> format;
    for (int j = 0; j < 3; j++)
    {
        std::string name = "Data/Hospital";
        char hospital_list[] = "ABC";
        name += hospital_list[j];
        name.append(".txt");
        std::ifstream file(name);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                int j = 0;
                std::vector<std::string> x = split(line, "Hospital#:"); //split the message so that we can get the data
                for (int i = 0; i < x.size(); i += 3)
                {
                    res.push_back(x[i + 1]);
                    res.push_back(x[i + 2]);
                }
            }
            file.close();
        }
    }
    return res;
}
//analyze the data from student
std::vector<std::string> get_data_from_student()
{
    std::vector<std::string> res;
    for (int j = 0; j < 5; j++)
    {
        std::string name = "Data/Student";
        char student_list[] = "12345";
        name += student_list[j];
        name.append(".txt");
        std::ifstream file(name);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                int j = 0;
                std::vector<std::string> x = split(line, ":"); //split the message so that we can get the data
                std::string buf;
                for (int i = 0; i < 3; i++)
                {
                    if ('0' <= x[2][i] && x[2][i] <= '9')
                    {
                        buf += x[2][i];
                    }
                }
                res.push_back(buf);
                for (int i = 3; i < x.size(); i++)
                {
                    std::string buf;
                    buf += x[i][0];
                    buf += x[i][1];
                    res.push_back(buf);
                }
            }
            file.close();
        }
    }
    return res;
}

std::vector<std::string> allocate_list(std::vector<std::string> hospital_list, std::vector<std::string> student_list)
{
    std::vector<std::string> res;
    int name = 0;
    for (int i = 0; i < student_list.size(); i++)
    {
        if ('0' <= student_list[i][0] && student_list[i][0] <= '9')
        {
            name++;
            int symptom = std::stoi(student_list[i]);
            int j;
            for (j = 0; j < hospital_list.size(); j++)
            {
                if (!student_list[i + 1].compare(hospital_list[j]))
                {
                    if (hospital_list[j + 1][0] - '0' <= symptom && (symptom <= hospital_list[j + 1][2] - '0' || hospital_list[j + 1][3] == '0'))
                    {
                        std::string people = std::to_string(name);
                        res.push_back(people);
                        res.push_back(hospital_list[j]);
                        break;
                    }
                }
                if (!student_list[i + 2].compare(hospital_list[j]))
                {
                    if (hospital_list[j + 1][0] - '0' <= symptom && (symptom <= hospital_list[j + 1][2] - '0' || hospital_list[j + 1][3] == '0'))
                    {
                        std::string people = std::to_string(name);
                        res.push_back(people);
                        res.push_back(hospital_list[j]);
                        break;
                    }
                }
                if (!student_list[i + 3].compare(hospital_list[j]))
                {
                    if (hospital_list[j + 1][0] - '0' <= symptom && (symptom <= hospital_list[j + 1][2] - '0' || hospital_list[j + 1][3] == '0'))
                    {
                        std::string people = std::to_string(name);
                        res.push_back(people);
                        res.push_back(hospital_list[j]);
                        break;
                    }
                }
            }
            if (j == hospital_list.size())
            {
                std::string people = std::to_string(name);
                res.push_back(people);
                res.push_back("NULL");
            }
        }
    }
    return res;
}

void do_service(int new_socket)
{
    char recvbuf[1024];
    int valread;
    char buffer[1024] = {0};
    int flag = 0;
    valread = int(read(new_socket, buffer, 1024));
    // store the information into the txt documents
    if (buffer[0] == 'H') //this message is from hospital
    {
        std::string name = "Data/Hospital";
        name += buffer[8];
        name.append(".txt");
        std::ofstream file(name);
        if (file.is_open())
        {

            file << buffer << std::endl;
            printf("Received the department list from <Hospital %c>\n", buffer[8]);
            file.close();
        }
    }
    if (buffer[0] == 'S') //this message is from student
    {
        printf("health center receive the application from <Student %c>\n", buffer[7]);
        std::vector<std::string> hospital_list = get_data_from_hospital();
        std::vector<std::string> x = split(buffer, "I:");
        for (int i = 0; i < hospital_list.size(); i++)
        {
            for (int j = 0; j < x.size(); j++)
            {
                if (!x[j].compare(hospital_list[i]))
                {
                    flag = 1;
                }
            }
        }
        if (flag == 0)
        {
            send(new_socket, "0", 1, 0);
        }
        else
        {
            send(new_socket, "1", 1, 0);
            std::string name = "Data/Student";
            name += buffer[7];
            name.append(".txt");
            std::ofstream file(name);
            if (file.is_open())
            {
                file << buffer << std::endl;
                file.close();
            }
        }
    }
}

void accept_messages(int number, int new_socket, int server_fd, struct sockaddr_in address)
{
    int addrlen = sizeof(address);
    for (int i = 0; i < number; i++) //we define the number we can get from above so that we can start the udp phase
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
}

int Decide_port(char judge)
{
    if (judge == 'A')
        return UDP_A;
    else if (judge == 'B')
        return UDP_B;
    else if (judge == 'C')
        return UDP_C;
    else if (judge == '1')
        return UDP_1;
    else if (judge == '2')
        return UDP_2;
    else if (judge == '3')
        return UDP_3;
    else if (judge == '4')
        return UDP_4;
    else if (judge == '5')
        return UDP_5;
    else
        return 0;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
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

    //printf specific information of the healthcenter
    std::cout << "The health center has TCP port " << htons((address.sin_port)) << " and IP address " << inet_ntoa(address.sin_addr) << std::endl;

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // receive messages from hospitals
    accept_messages(hospital, new_socket, server_fd, address);
    sleep(1);
    printf("End of Phase 1 for the health center\n");
    std::cout << "The health center has TCP port " << htons((address.sin_port)) << " and IP address " << inet_ntoa(address.sin_addr) << std::endl;
    //receive messages from students
    accept_messages(student, new_socket, server_fd, address);
    sleep(1);

    //analyze the data after receiving the messages of hospitals;
    std::vector<std::string> hospital_list = get_data_from_hospital();
    //analyze the data after receiving the messages of students;
    std::vector<std::string> student_list = get_data_from_student();
    //allocate the application
    std::vector<std::string> final_list = allocate_list(hospital_list, student_list);

    close(server_fd);
    //Build UDP connection in phase 2

    int sockfd, n;
    struct sockaddr_in serveraddr;

    // clear servaddr
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(UDP_1);
    serveraddr.sin_family = AF_INET;

    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

    struct sockaddr_in current;
    socklen_t len = sizeof(current);
    getsockname(sockfd, (struct sockaddr *)&current, &len);
    std::cout << "The health center has UDP port " << ntohs(current.sin_port) << " and IP address " << inet_ntoa(current.sin_addr) << " for Phase 2" << std::endl;
    //set the udp datagram and prepare to send the messages
    for (int i = 0; i < final_list.size(); i = i + 2)
    {
        //request to send datagram to student1
        serveraddr.sin_port = htons(Decide_port(final_list[i][0]));
        if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        {
            printf("\n Error : Connect Failed \n");
            exit(0);
        }
        sendto(sockfd, final_list[i + 1].c_str(), final_list[i + 1].size(), 0, (struct sockaddr *)NULL, sizeof(serveraddr));
        printf("The health Center has send the application result to <Student %d>\n", i / 2 + 1);

        //send datagram to related student3
        int port = Decide_port(final_list[i + 1][0]);
        if (port)
        {
            serveraddr.sin_port = htons(port);
            if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
            {
                printf("\n Error : Connect Failed \n");
                exit(0);
            }
            sendto(sockfd, final_list[i].c_str(), final_list[i].size(), 0, (struct sockaddr *)NULL, sizeof(serveraddr));
            printf("The health Center has send one admitted student to <Hospital %c>\n", final_list[i + 1][0]);
        }
    }
    return 0;
}