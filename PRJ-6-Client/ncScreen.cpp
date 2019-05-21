#include <ncurses.h>
#include <cstdlib> // rand(), srand(), exit()
#include <vector>

#include "ncScreen.h"




NcScreen::NcScreen()
{
    clientVersion = 0.2;
}


void NcScreen::initNcScreen()
{
    if (!initscr())
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(1);
    }
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear();
    refresh();

}



int NcScreen::mainMenu(CreateData &createData, int &gameMode)
{
    int key;
    char input[10];
    int cursPos = 0;

    nodelay(stdscr, FALSE);


    while(true)
    {
        getmaxyx(stdscr, row, col);

        clear();
        mvaddstr(row / 2 - 2, col / 4, " Start");
        mvaddstr(row / 2 - 1, col / 4, " Skin = ");
        addch(createData.skin);
        mvaddstr(row / 2 - 0, col / 4, " Name = ");
        addstr(createData.name.c_str());
        mvaddstr(row / 2 + 1, col / 4, " Game mode = ");
        printw("%d", gameMode);
        mvaddstr(row / 2 + 2, col / 4, " Exit");
        mvaddch(row / 2 - 2 + cursPos, col / 4 - 2, '>');

        mvprintw(0, 0, "PotaynoyLaz %.2f", clientVersion);
        mvaddstr(row - 3, 0, "In game controls:");

        mvaddstr(row - 1, 0, "q = exit");

        key = getch();

        if(key == KEY_DOWN) cursPos ++;
        if(key == KEY_UP) cursPos --;

        if(cursPos < 0) cursPos = 4;
        if(cursPos > 4) cursPos = 0;

        if(key == '\n' && cursPos == 0)
        {
            nodelay(stdscr, TRUE);
            noecho();
            return 0;
        }

        if(key == '\n' && cursPos == 1)
        {
            clear();
            mvaddstr(row / 2 - 2 + cursPos, col / 4 + 2, "Enter your skin: ");
            getnstr (input, 1);
            createData.skin = input[0];
        }

        if(key == '\n' && cursPos == 2)
        {
            clear();
            mvaddstr(row / 2 - 2 + cursPos, col / 4 + 2, "Enter your name: ");
            getnstr (input, 20);
            createData.name = input;
        }

        if(key == '\n' && cursPos == 3)
        {
            while(true)
            {
                clear();
                mvaddstr(row / 2 - 2, col / 4, "- Select game mode -");
                mvaddstr(row / 2 - 0, col / 4, " Mode 1");
                mvaddstr(row / 2 + 1, col / 4, " Mode 2");
                mvaddstr(row / 2 + 2, col / 4, " Back");
                mvaddch(row / 2 - 2 + cursPos, col / 4 - 2, '>');

                key = getch();

                if(key == KEY_DOWN) cursPos ++;
                if(key == KEY_UP) cursPos --;

                if(cursPos < 2) cursPos = 4;
                if(cursPos > 4) cursPos = 2;

                if(key == '\n' && cursPos == 2)
                {
                    gameMode = 1;
                    key = 0;
                    break;
                }

                if(key == '\n' && cursPos == 3)
                {
                    gameMode = 2;
                    key = 0;
                    break;
                }

                if(key == '\n' && cursPos == 4)
                {
                    key = 0;
                    break;
                }
                if(key == 'q')
                {
                    key = 0;
                    break;
                }
            }

        }

        if(key == '\n' && cursPos == 4)
            return 9;
        if(key == 'q')
            return 9;
    }
}



int NcScreen::getInput()
{
    int key = getch();

    switch(key)
    {
        case KEY_DOWN:
            return 1;
            break;
        case KEY_UP:
            return 2;
            break;
        case KEY_RIGHT:
            return 3;
            break;
        case KEY_LEFT:
            return 4;
            break;
        case 't':
            return 5;
            break;
        case 'q':
            return 9;
            break;
        default:
            return 0;
    }

}



void NcScreen::printScreen(int &myid, int &pbCount)
{

    if(updScreen)
    {
        updScreen = false;
        // refreshCount ++;

        setCam(myid);

        clear();

        for (int i = 0; i < printStars.size(); ++i)
        {
            mvaddch(printStars[i].x + camX, printStars[i].y + camY, printStars[i].skin);
        }

        for (int i = 0; i < printPwrPoints.size(); ++i)
        {
            mvaddch(printPwrPoints[i].x + camX, printPwrPoints[i].y + camY, printPwrPoints[i].skin);
        }

        for (int i = 0; i < printUnits.size(); ++i)
        {
            mvaddch(printUnits[i].x + camX, printUnits[i].y + camY, printUnits[i].skin);
        }

        mvprintw(0, 0, "PWR = %d", printStatus.pwr);
        // mvprintw(1, 1, "Package build count = %d", pbCount);
        // mvprintw(1, 1, "Refresh count = %d", refreshCount);
        //refresh();
    }

}



void NcScreen::setCam(int &myid)
{
    getmaxyx(stdscr, row, col);
    centerY = col / 2;
    centerX = row / 2;

    for(int i = 0; i < printUnits.size(); ++i)
    {
        if(printUnits[i].id == myid)
        {
            camX = centerX - printUnits[i].x;
            camY = centerY - printUnits[i].y;
            break;
        }
    }
}



void NcScreen::exitNcScreen()
{
    endwin();
}
