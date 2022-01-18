#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>


std::vector<std::string> v_messages = {"dump", "restore", "else", "exit"};

int main(int argc, char const* argv[])
{
    int PORT = atoi(argv[1]);
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    for (;;)
    {
        std::string s;
        std::cin >> s;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            std::cout << "\n Socket creation error \n";
            return -1;
        }
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            std::cout << "\nConnection Failed \n";
            return -1;
        }
        std::cout << "sent : " << s
                  << "\n sented length : " << send(sock, s.c_str(), s.size(), 0)
                  << std::endl;
        valread = read(sock, buffer, 1024);
        std::cout << "message received : " << std::string(buffer, valread)
                  << std::endl;
        close(sock);
        if(s=="exit") break;
    }
    return 0;
}