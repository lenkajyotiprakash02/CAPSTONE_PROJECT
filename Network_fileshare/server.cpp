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
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept client connection
    client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Client connected\n";

    // Receive filename from client
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        std::cerr << "Failed to receive filename\n";
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    buffer[bytes_received] = '\0';
    std::string filename(buffer);
    std::cout << "Filename requested: " << filename << std::endl;

    // Open file and send content
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::string error = "ERROR: File not found\n";
        send(client_socket, error.c_str(), (int)error.size(), 0);
    } else {
        while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
            send(client_socket, buffer, (int)file.gcount(), 0);
        }
        file.close();
        std::cout << "File transfer complete\n";
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
