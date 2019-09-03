#include "UdpServer.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>

UdpServer::UdpServer(char* toAddress, int port){
    // Setting up UDP client
    memset(&m_myaddr, 0, sizeof(m_myaddr));
    m_myaddr.sin_family = AF_INET;
    m_myaddr.sin_port = htons(port);
    m_myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET,toAddress,&m_myaddr.sin_addr.s_addr);
}

int UdpServer::setUp(){
    if((m_sock=socket(AF_INET, SOCK_DGRAM, 0))<0){
        return -1;
    }
    else{
        return 1;
    }
}

void UdpServer::sendMessage(char *message, int len){
    sendto(m_sock, message, len, 0, (struct sockaddr *)&m_myaddr, sizeof(m_myaddr));
}

void UdpServer::closeServer(){
    close(m_sock);
}

UdpServer::~UdpServer(){
}
