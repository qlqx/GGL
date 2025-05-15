#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


void receiveLoop(int sock) {
    char buffer[1024];
    while (true) {
        int n = read(sock, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "\n" << buffer << "你: ";
            std::cout.flush();
        }
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    std::string ip;
    std::cout << "输入服务端 IP: ";
    std::cin >> ip;
    std::cin.ignore();

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5555);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "连接失败\n";
        return 1;
    }

    char buffer[1024];
    int n = read(sock, buffer, sizeof(buffer));
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << buffer;
    }

    std::string nickname;
    std::getline(std::cin, nickname);
    nickname += "\n";
    send(sock, nickname.c_str(), nickname.length(), 0);

    std::thread recv_thread(receiveLoop, sock);
    recv_thread.detach();

    while (true) {
        std::string msg;
        std::cout << "你: ";
        std::getline(std::cin, msg);
        msg += "\n";
        send(sock, msg.c_str(), msg.length(), 0);
    }

    return 0;
}
