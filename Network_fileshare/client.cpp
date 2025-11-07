#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }
   
    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // Use inet_addr instead of inet_pton

    // Connect to server
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server\n";

    // Get filename from user
    std::string filename;
    std::cout << "Enter filename to download: ";
    std::getline(std::cin, filename);

    // Send filename to server
    send(sock, filename.c_str(), (int)filename.size(), 0);

    // Open file to write received data
    std::ofstream outfile("received_" + filename, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to create file\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive file data
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        outfile.write(buffer, bytes_received);
        if (bytes_received < BUFFER_SIZE) break;
    }
    outfile.close();

    std::cout << "File received and saved as: received_" << filename << std::endl;

    closesocket(sock);
    WSACleanup();
    return 0;
}
