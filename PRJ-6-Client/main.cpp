//#include <ncurses.h>
//#include <cstdlib> // rand(), srand(), exit()
//#include <ctime> // time()
//#include <string>
//#include <cstring> // std::memcpy()

#include <iostream> // cout
#include <unistd.h> // usleep()
#include <vector>
#include <string>

#include "connector.h"
#include "ncScreen.h"
#include "playerController.h"




int main(int args, char *argv[])
{

    int command = 0;
    int myid;
    int gameMode = 1;

    CreateData createData;
    createData.skin = 'A';
    createData.name = "Name";
    // int fakeInput = 0;
    // int fakeInputCounter = 0;


    NcScreen screen;
    Connector connect;
    PlayerController controller;

    //screen.refreshCount = 0;


    screen.initNcScreen();

    command = screen.mainMenu(createData, gameMode);

    if(command != 9)
        if(connect.connectServer(gameMode) == 1) // Выход при потере соединения
            command = 9;

    if(command != 9)
        if(controller.createPlayer(connect, myid, createData) == 1) // Выход при потере соединения
            command = 9;



    while (command != 9)
    {
        usleep(10000);

        command = screen.getInput();

        controller.setCommand(command, myid, connect.sendBuff, connect.sendSize);

        if(connect.syncData() == 1)
            break;

        controller.recvBuffHandler(connect.recvBuff, screen);

        screen.printScreen(myid, connect.pbCount);

    }


    screen.exitNcScreen();
    connect.end();
    //std::cout << initPlanetSkin << std::endl;
    return 0;
}







//fakeInput = setFakeInput(*argv[2], fakeInputCounter, command);

// int setFakeInput(char mode, int &counter, int trueInput)
// {
//     if(mode == '1')
//     {
//         counter ++;

//         if(counter == 100)
//             counter = 0;

//         if(counter < 50)
//             return 3;

//         if(counter > 50)
//             return 4;
//     }
//     else
//         return trueInput;
// }
