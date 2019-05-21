//#include <iostream>
//#include <algorithm> // std::reverse()
#include <cstring> // std::memcpy()
#include "playerController.h"




PlayerController::PlayerController()
{
    unitsFrameNum = 0;
    pwrPointsFrameNum = 0;
}


int PlayerController::createPlayer(Connector &connect, int &myid, CreateData &createData)
{
    //std::reverse(createData.name.begin(), createData.name.end());

    connect.sendBuff[0] = 2; // тип пакета

    connect.sendBuff[1] = sizeof(char) * createData.name.size();

    connect.sendBuff[2] = createData.skin;

    std::memcpy(&connect.sendBuff[3], createData.name.data(), sizeof(char) * createData.name.size());
    connect.sendSize = sizeof(char) * createData.name.size() + 3;


    if(connect.syncData() == 1) // Выход при потере соединения
        return 1;


    if(connect.recvBuff[0] == 2)
    {
        myid = connect.recvBuff[1];
    }

    return 0;
}



void PlayerController::setCommand(int input, int &myid, char *sendBuff, int &sendSize)
{

    sendBuff[0] = 3;
    sendBuff[1] = myid;
    sendBuff[2] = input;
    sendBuff[3] = unitsFrameNum;
    sendBuff[4] = pwrPointsFrameNum;
    sendBuff[5] = 0; // резерв

    sendSize = 6;

}



void PlayerController::recvBuffHandler(char *recvBuff, NcScreen &screen)
{


    if(recvBuff[0] == 4)
    {
        screen.updScreen = true;

        int buffCarriage = 0;

        unitsFrameNum = recvBuff[1];
        pwrPointsFrameNum = recvBuff[2];


        std::memcpy(&printObjectsSize, &recvBuff[4], sizeof(int) * 3);
        buffCarriage = sizeof(int) * 3 + 4;

        if(printObjectsSize[0] != 0)
        {
            screen.printUnits.resize(printObjectsSize[0]);

            std::memcpy(screen.printUnits.data(), &recvBuff[buffCarriage], sizeof(PrintData) * printObjectsSize[0]);
            buffCarriage += sizeof(PrintData) * printObjectsSize[0];
        }


        if(printObjectsSize[1] != 0)
        {
            screen.printPwrPoints.resize(printObjectsSize[1]);

            std::memcpy(screen.printPwrPoints.data(), &recvBuff[buffCarriage], sizeof(PrintData) * printObjectsSize[1]);
            buffCarriage += sizeof(PrintData) * printObjectsSize[1];

            std::memcpy(&screen.printStatus, &recvBuff[buffCarriage], sizeof(PrintStatusData));
            buffCarriage += sizeof(PrintStatusData);
        }

        // printObjectsSize[2] != 0
    }
}

















// if(recvBuff[0] == 4)
//     {
//         int copyBuffCounter;

//         if(recvPrintSize != recvBuff[1])
//         {
//             recvPrintSize = recvBuff[1];
//             screen.printObjects.resize(recvPrintSize);
//         }


//         copyBuffCounter = sizeof(PrintObjectData) * recvPrintSize;
//         std::memcpy(screen.printObjects.data(), &recvBuff[3], copyBuffCounter);

//         std::memcpy(&screen.printStatus, &recvBuff[copyBuffCounter + 3], sizeof(PrintStatusData));

//         clientFrameNum = recvBuff[2];

//         // unitsFrameNum = recvBuff[2];
//         // pwrPointsFrameNum = recvBuff[3];
//         // starsFrameNum = recvBuff[4];

//         screen.updScreen = true;
//     }
