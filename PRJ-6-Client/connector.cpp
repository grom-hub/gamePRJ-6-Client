#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr()

#include <unistd.h>
#include <iostream>
#include <cstring> // std::memcpy()
//#include <cstdlib>
//#include <cstdio>
#include <vector>
#include <cstring> // std::memcpy()
#include <unistd.h> // usleep()

#include "connector.h"



Connector::Connector()
{
    pbCount = 10;
}



int Connector::connectServer(int gameMode)
{

    sock = socket(AF_INET, SOCK_STREAM, 0); // Префикс AF означает "address family" - "семейство адресов".
                                            // SOCK_STREAM - (TCP) способ передачи данных по сети.
    if(sock < 0)
    {
        // perror("socket");
        // exit(1);
        return 1;
    }

    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // INADDR_LOOPBACK - петля типа 127.0.0.1 (127.0.0.0/8).
    if(gameMode == 1)
    {
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        // addr.sin_addr.s_addr = inet_addr("192.168.0.78");
        addr.sin_port = htons(3425);
    }

    if(gameMode == 2)
    {
        addr.sin_addr.s_addr = inet_addr("192.168.1.69");
        addr.sin_port = htons(3425);
    }


    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        // perror("connect");
        // exit(2);
        return 1;
    }
    return 0;
}


int Connector::syncData()
{

// Отправка

    send(sock, sendBuff, sendSize, 0);


// Получение

    bytesRead = recv(sock, recvPreBuff, 102400, 0);

    if(bytesRead <= 0) // Выход при потере соединения
        return 1;

    if(bytesRead != 1) // Неоднозначное условие (доработать)
    {
        std::memcpy(&targetRecvSize, &recvPreBuff, sizeof(int));

        std::memcpy(&recvBuff, &recvPreBuff[sizeof(int)], bytesRead - sizeof(int));

        totalBytesRead = bytesRead - sizeof(int);


        while(totalBytesRead != targetRecvSize)
        {
            usleep(1000);

            pbCount ++;

            bytesRead = recv(sock, recvPreBuff, 102400, 0);

            if(bytesRead <= 0) // Выход при потере соединения
                return 1;

            std::memcpy(&recvBuff[totalBytesRead], &recvPreBuff, bytesRead);

            totalBytesRead += bytesRead;
        }
    }
    else
        recvBuff[0] = 0;

    return 0;
}



void Connector::end()
{
    if(close(sock) == 0)
        std::cout << "Disconnect. Socket close - ok." << std::endl;
}

