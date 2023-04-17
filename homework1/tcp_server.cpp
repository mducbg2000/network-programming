#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <arpa/inet.h>
#include <string>
#include <string.h>

int main(int argc, char const *argv[])
{

    // load greeting from file
    std::ifstream temp(argv[1]);
    std::string greeting;
    getline(temp, greeting);
    temp.close();
    auto msg = greeting.c_str();

    // open file to write
    std::ofstream receiver(argv[2]);

    // Init socket
    auto port = atoi(argv[0]);

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0)
    {
        std::cout << "Failed to create socket.\n";
        return -1;
    }

    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // bind address to socket
    if (bind(server, (struct sockaddr *)&addr, sizeof(addr)))
    {
        std::cout << "bind() failed.\n";
        return -1;
    }

    // listen clients
    if (listen(server, 5))
    {
        std::cout << "listen() failed.\n";
        return -1;
    }
    std::cout << "Waiting for a new client ...\n";

    // accept connect
    sockaddr_in clientAddr;
    unsigned int clientAddrLen = sizeof(clientAddr);
    int client = accept(server, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        std::cout << "accept() failed.\n";
        return -1;
    }
    // Send greeting message to client
    send(client, msg, strlen(msg), 0);
    std::cout << ("New client connected: %d\n", client);

    // receive client message
    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        std::cout << "recv() failed.\n";
        return -1;
    }
    // Ghi vao file
    receiver << buf;
    receiver.close();

    // Dong ket noi
    close(client);
    close(server);
    return 0;
}
