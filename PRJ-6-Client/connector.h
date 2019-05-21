#ifndef CONNECTOR_H
#define CONNECTOR_H


//#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <vector>
//#include <string>



class Connector
{
public:
        char sendBuff[1024];
    char recvBuff[102400];
    char recvPreBuff[102400];
    int sendSize;

    int pbCount;

        Connector();
        //~Connector();
        int connectServer(int gameMode);
        int syncData();
        void end();




private:
        int sock;
        struct sockaddr_in addr;
        int bytesRead;

        int targetRecvSize;
        int totalBytesRead;


};

#endif // CONNECTOR_H
