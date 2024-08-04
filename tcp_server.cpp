#include "tcp_server.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

const int BUFFER = 512;

namespace http
{
    TCPServer::TCPServer(std::string ip_address, int port): s_ip_address(ip_address), s_port(port), s_socket(), new_socket(),
                                                             s_incoming_message(),
                                                             s_socket_address(), s_message_len(sizeof(s_socket_address)), s_wsaData()
	{
    	s_socket_address.sin_family = AF_INET;
		s_socket_address.sin_port = htons(s_port);
		s_socket_address.sin_addr.s_addr = inet_addr(s_ip_address.c_str());
		
		if (startServer() != 0)
        {
            std::cout<<"Failed to start server with PORT: "<<ntohs(s_socket_address.sin_port)<<std::endl;
            exit(EXIT_FAILURE);
        }
	}

    TCPServer::~TCPServer(){
    	closeServer();
	}
    
    int TCPServer::startServer(){
		if (WSAStartup(MAKEWORD(2, 0), &s_wsaData) != 0){
    		std::cout<<"WSA Startup failed."<<std::endl;
    		exit(EXIT_FAILURE);
    		return 1;
		}
		
		s_socket = socket(AF_INET,SOCK_STREAM,0);
		if (s_socket <0){
			std::cout<<"Error while creating socket."<<std::endl;
			exit(EXIT_FAILURE);
			return 1;
		}
		
		if (bind(s_socket, (sockaddr *)&s_socket_address, s_message_len) < 0)
        {
            std::cout<<"Cannot connect socket to address"<<std::endl;
            exit(EXIT_FAILURE);
            return 1;
        }
		
		return 0;
	}
	
	void TCPServer::closeServer(){
		closesocket(s_socket);
		closesocket(new_socket);
		WSACleanup();
		exit(0);
	}
	
	void TCPServer::start_listen()
    {
    	int ret_val = listen(s_socket, 20);
        if (ret_val < 0)
        { 
            std::cout<<"Socket listen failed"<<std::endl;
            exit(EXIT_FAILURE);
        }
        
        else{
        	std::cout<<"Listening on Address: "<<inet_ntoa(s_socket_address.sin_addr)<<" PORT: "<<ntohs(s_socket_address.sin_port)<<std::endl;
		}
		
		int bytesReceived = 0;
			
		char buffer[BUFFER] = {0};
		
		while (true){
			acceptConnection(new_socket);
			
            bytesReceived = recv(new_socket, buffer, BUFFER, 0);
            if (bytesReceived < 0)
            {
            	std::cout<<"Failed to receive bytes from client socket connection"<<std::endl;
            	exit(EXIT_FAILURE);
            }
            
            std::cout<<"Bytes Received: "<<bytesReceived<<std::endl;
        	std::cout<<"Buffer: "<<buffer<<std::endl;
			    
			send_response();
			
			closesocket(new_socket);
		}
	}
	
	void TCPServer::acceptConnection(SOCKET &new_socket){
		new_socket = accept(s_socket, (sockaddr *)&s_socket_address, &s_message_len);
        if (new_socket < 0)
        {
            std::cout<<"Failed to accept connection on address: "<<inet_ntoa(s_socket_address.sin_addr)<<" PORT: "<<ntohs(s_socket_address.sin_port)<<std::endl;
            exit(EXIT_FAILURE);
        }
	}
	
	void TCPServer::send_response(){
		int bytesSent, totalBytes;
		
		while (totalBytes < s_server_message.size()){
			bytesSent = send(new_socket, s_server_message.c_str(), s_server_message.size(), 0);
            if (bytesSent < 0)
            {
                break;
            }
            totalBytes += bytesSent;
        }

        if (totalBytes == s_server_message.size())
        {
            std::cout<<"Bytes Sent: "<<bytesSent<<std::endl;
        }
    }

}
