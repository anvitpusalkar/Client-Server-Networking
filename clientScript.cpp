#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(){
	WSADATA wsaData;
	SOCKET clientSocket = INVALID_SOCKET;
    sockaddr_in serverAddr;
    int iResult;
    char buffer[512];
    int bytesReceived;
    std::string message = "UEFA Euros have started!";
    
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout<<"WSAStartup Failed."<<std::endl;
        exit(EXIT_FAILURE);
        return 1;
    }
    
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cout<<"Error while creating socket."<<std::endl;
        WSACleanup();
        return 1;
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    iResult = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout<<"Unable to connect to server."<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    iResult = send(clientSocket, message.c_str(), message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout<<"Send failed."<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout<<"Sent to server: "<<message<<std::endl;

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout<<"Response received from server: "<<buffer<<std::endl;
    } else if (bytesReceived == 0) {
        std::cout<<"Connection closed by server."<<std::endl;
    } else {
        std::cout<<"Receive failed."<<std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
