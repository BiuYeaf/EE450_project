#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>

#define default_rate 2
#define bandidth 3

std::vector<std::vector<double> > slot;
std::vector<char> packet;
std::vector<char> priority;
std::vector<int> resultnum;
std::vector<std::string> resultpacket;
int slotnumber = 0;
int packetnumber = 0;
int average;
int number = 0;
int starttime = 0;

//calculate the number of the packets from the source
int packet_num(std::string buffer)
{
    int res = 0;
    for (int i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] == ',')
            res++;
    }
    res++;
    return res;
}
// judge that it is number or not
bool is_num(char a)
{
    if ('0' <= a && a <= '9')
    {
        return true;
    }
    else
        return false;
}
//extract the information of the txt documents
std::vector<double> Extract_Data_nums(std::string buffer)
{
    std::vector<double> res;
    if (buffer[0] == 'S' && buffer[1] == 'o')
    {
        for (int i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] == ',' || buffer[i] == ':')
            {
                std::string num1;
                int j = i + 1;
                while (buffer[j] == ' ')
                {
                    j++;
                }
                while (j)
                {
                    if (is_num(buffer[j]))
                        num1 += buffer[j];
                    else if (buffer[j] == ' ')
                        break;
                    j++;
                }
                res.push_back(stoi(num1));
                std::string num2;
                while (j)
                {
                    j++;
                    if (is_num(buffer[j]))
                        num2 += buffer[j];
                    else
                        break;
                }
                res.push_back(stoi(num2));
            }
        }
    }
    return res;
}

char Extract_Data_packets(std::string buffer)
{
    char res;
    if (buffer[0] == 'S' && buffer[1] == 'o')
    {
        for (int i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] == ':')
            {
                res = buffer[i - 1];
            }
        }
    }
    return res;
}

int allocate_address(char a)
{
    return a - 'A';
}
//use this function to sort the frame;
void sort_packets()
{
    for (int i = 0; i < priority.size(); i++)
    {
        for (int j = 0; j < packet.size(); j++)
        {
            if (packet[j] == priority[i])
            {
                for (int k = 0; k < slot[j].size(); k++)
                {
                    if (slot[j][k] == starttime && (k % 2 == 0))
                    {
                        std::string x;
                        x += packet[j];
                        x += std::to_string(k / 2 + 1);
                        resultpacket.push_back(x);
                        resultnum.push_back(slot[j][k]);
                        resultnum.push_back(slot[j][k + 1]);
                        number = number + slot[j][k + 1] - slot[j][k];
                    }
                }
            }
        }
    }
    starttime++;
    if (number < packetnumber)
        sort_packets();
}

void printf_frame()
{
    double time_slot = 1 / (double)average;
    double totaltime = 0;
    printf("SF\n");
    for (int i; i < resultnum.size(); i = i + 2)
    {
        printf("%d, %.1f %.1f ", allocate_address(resultpacket[i / 2][0]), totaltime, totaltime + time_slot * (resultnum[i + 1] - resultnum[i]));
        totaltime = totaltime + time_slot * (resultnum[i + 1] - resultnum[i]);
        std::cout << resultpacket[i / 2] << std::endl;
    }
    printf("EF\n");
}

int main(int argc, char const *argv[])
{
    std::ifstream file(argv[1]);
    if (file.is_open())
    {
        std::string buffer;
        while (std::getline(file, buffer))
        {
            slot.push_back(Extract_Data_nums(buffer));
            packet.push_back(Extract_Data_packets(buffer));
            if (!(buffer[0] == 'S' && buffer[1] == 'o'))
            {
                for (int i = 0; i < buffer.size(); i++)
                {
                    if (buffer[i] != ' ')
                        priority.push_back(buffer[i]);
                }
            }
        }
        file.close();
    }

    for (int i = 0; i < slot.size(); i++)
    {
        //calculate the number of time slot.
        for (int j = 0; j < slot[i].size(); j++)
        {
            if (slotnumber < slot[i][j])
                slotnumber = slot[i][j];
        }
        //calculate the number of packets
        for (int j = 0; j < slot[i].size(); j = j + 2)
        {
            packetnumber = packetnumber + slot[i][j + 1] - slot[i][j];
        }
    }
    average = packetnumber / slotnumber;
    if (average < 2)
        average = default_rate;

    sort_packets();
    printf_frame();

    return 0;
}