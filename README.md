# Network File Sharing Project

This project implements a simple network file sharing system using TCP sockets.

## Features
- Server listens for client file requests.
- Client can request and download files.
- Works on Windows (Winsock) and Linux.

## How to Build and Run

### Windows
- Compile using MinGW:
g++ server.cpp -o server.exe -mconsole -lws2_32
g++ client.cpp -o client.exe -mconsole -lws2_32
- Run the server: `./server.exe`
- Run the client: `./client.exe`


### Linux
- Compile using g++:
g++ server.cpp -o server
g++ client.cpp -o client
- Run the server: `./server`
- Run the client: `./client`

## Usage
- Make sure server is running before client.
- Enter the filename on client to download.


