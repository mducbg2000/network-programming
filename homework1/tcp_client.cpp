#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{

    auto port = atoi(argv[2]);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client < 0)
    {
        std::cout << "Socket creation error";
        return -1;
    }

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "Connection Failed: " << errno << " - " << strerror(errno);
        return -1;
    }

    char buf[2048];
    int len = recv(client, buf, sizeof(buf), 0);
    buf[len] = 0;
    printf("Data received: %s\n", buf);

    std::cout << "Print message:";
    std::string message;
    std::cin >> message;
    auto msg = message.c_str();
    send(client, msg, strlen(msg), 0);
    std::cout << "Sended message!";

    close(client);
    return 0;
}
