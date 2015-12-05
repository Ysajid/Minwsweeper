#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include <time.h>
#define MAX 30
using namespace std;

////////////////////    for windows       ////////////////
///////comment out this part if you are using ubuntu

//#include <windows.h>  //for windows only
//#include <conio.h>    //for windows only
//#define clearScreen system("cls") // for windows
//static int upArrow = -72,rightArrow = -77,leftArrow = -75, downArrow = -80, enterKey = -13;  //for windows

////////////////////    for windows       ////////////////



////////////////////    for ubuntu    ////////////////
///////comment out this part if you are using windows

#define clearScreen system("clear")
#include <termios.h>
static struct termios old, newT;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  newT = old; /* make newT settings same as old settings */
  newT.c_lflag &= ~ICANON; /* disable buffered i/o */
  newT.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newT); /* use these newT terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}
static int upArrow = -65,rightArrow = -67,leftArrow = -68, downArrow = -66, enterKey = -10;
////////////////////    for ubuntu only    ////////////////

typedef struct pos
{
    int x,y;
}pos;

pos cPos = {0,0};
static pos adjCells[] = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
static pos sideCells[] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
int bombs=0, flags=0;



int gridSize = 10;
char grid[MAX][MAX];
int mineGrid[MAX][MAX];

void setFlag(int , int);
void gameOver();
void youWon();
int main();

void changePos(int dir)
{
    switch(dir){
        case 1:
            if(!(cPos.x == gridSize - 1))cPos.x++; //Move Right
            break;
        case 2:
            if(!(cPos.x == 0))cPos.x--; //Move Left
            break;
        case 3:
            if(!(cPos.y == gridSize - 1))cPos.y++; //Move Up
            break;
        case 4:
            if(!(cPos.y == 0))cPos.y--; //move Down
            break;
    }
}


void bombCount(int x, int y)
{
    int adjX, adjY;
    for (int i=0;i<8 ;i++ ){
        adjX = x + adjCells[i].x;
        adjY = y + adjCells[i].y;
        if(adjX >= gridSize || adjX <0) continue;
        if(adjY >= gridSize || adjY <0) continue;
        if(mineGrid[adjY][adjX] == 1) {
            continue;
        }
        if(mineGrid[adjY][adjX] == 0){
            mineGrid[adjY][adjX] = 2;
        }
        else {
            mineGrid[adjY][adjX]++;
        }

        for (int y=gridSize - 1; y >= 0; y-- ){
            for (int x=0;x<gridSize ;x++ ){
            }
        }
    }
}

void clearField(int x, int y)
{
    int adjX, adjY;
    for (int i=0;i<8 ;i++ ){
        adjX = x + adjCells[i].x;
        adjY = y + adjCells[i].y;
        if(adjX >= gridSize || adjX <0) continue;
        if(adjY >= gridSize || adjY <0) continue;
        if(grid[adjY][adjX] != '*') continue;
        if(mineGrid[adjY][adjX] != 1){
            if(mineGrid[adjY][adjX] != 0){
                grid[adjY][adjX] = mineGrid[adjY][adjX] + 47 ;
                continue;
            }
            else grid[adjY][adjX] = ' ';
        }
        else continue;
        clearField(adjX, adjY);
    }
}


int getAction()
{
    char ch = -getch();

    if(ch == rightArrow){
        changePos(1);
    }
    else if(ch == leftArrow){
        changePos(2);
    }
    else if(ch == upArrow){
        changePos(3);
    }
    else if(ch == downArrow){
        changePos(4);
    }
    else if(ch == enterKey && grid[cPos.y][cPos.x] == '*'){
        if(mineGrid[cPos.y][cPos.x] == 1){
            clearScreen;
            gameOver();
            return 0;
        }
        else if(flags == bombs){
            youWon();
        }
        else {
            if(mineGrid[cPos.y][cPos.x] != 0)
                grid[cPos.y][cPos.x] = mineGrid[cPos.y][cPos.x] + 47 ;
            else grid[cPos.y][cPos.x] = ' ';
        }
        clearField(cPos.x,cPos.y);
    }
    else if(ch == -32){
        setFlag(cPos.x,cPos.y);
    }

    clearScreen;
    return 1;
}

void printUpdatedGrid()
{
    if(gridSize == 10) printf("\n\n\n\n\n\n                              ");
    else if(gridSize == 15) printf("\n\n\n                        ");
    else if(gridSize == 20) printf("                    ");
    for (int i=0;i<gridSize ;i++ ){
        if(i == gridSize - 1)printf("_");
        else printf("__");
    }
    printf("\n\n");
    for (int y = gridSize - 1; y >= 0;y-- ){
        if(gridSize == 10) printf("                            | ");
        else if(gridSize == 15) printf("                      | ");
        else if(gridSize == 20) printf("                  | ");
        for (int x=0;x<gridSize ;x++ ){
            printf("%c",grid[y][x]);
            if(cPos.x == x && cPos.y == y) printf("<");
            else printf(" ");
        }
        printf("|\n");
    }
    if(gridSize == 10) printf("                              ");
    else if(gridSize == 15) printf("                        ");
    else if(gridSize == 20) printf("                    ");

    for (int i=0;i<gridSize ;i++ ){
        if(i == gridSize - 1)printf("_");
        else printf("__");
    }
    printf("\n");
}

void setFlag(int x , int y)
{
    if(grid[y][x] == '*'){
        grid[y][x] = '#';
        if(mineGrid[y][x] == 1){
            flags++;
        }
    }
    else if(grid[y][x] == '#'){
        grid[y][x] = '*';
        if(mineGrid[y][x] == 1){
            flags--;
        }
    }
    if(flags == bombs) youWon();
}

void gameOver()
{
    printf("             _____ _____ _____ _____    _____ _____ _____ _____ \n");
    printf("            |   __|  _  |     |   __|  |     |  |  |   __| __  |\n");
    printf("            |  |  |     | | | |   __|  |  |  |  |  |   __|    -|\n");
    printf("            |_____|__|__|_|_|_|_____|  |_____| ___/|_____|__|__|\n");
    if(gridSize == 10) printf("\n                              ");
    else if(gridSize == 15) printf("\n                        ");
    else if(gridSize == 20) printf("                    ");
    for (int i=0;i<gridSize ;i++ ){
        if(i == gridSize - 1)printf("_");
        else printf("__");
    }
    printf("\n\n");
    for (int y = gridSize - 1; y >= 0;y-- ){
        if(gridSize == 10) printf("                            | ");
        else if(gridSize == 15) printf("                      | ");
        else if(gridSize == 20) printf("                  | ");
        for (int x=0;x<gridSize ;x++ ){
            if(mineGrid[y][x] == 1){
                printf("o");
                if(cPos.x == x && cPos.y == y) printf("<");
                else printf(" ");
            }
            else printf("  ");
        }
        printf("|\n");
    }
    if(gridSize == 10) printf("                              ");
    else if(gridSize == 15) printf("                        ");
    else if(gridSize == 20) printf("                    ");

    for (int i=0;i<gridSize ;i++ ){
        if(i == gridSize - 1)printf("_");
        else printf("__");
    }
    printf("\nWANT TO PLAY AGAIN? y/n\n");
    char ch = getch();
    switch(ch){
        case 'y' :
           clearScreen;
            main();
            break;
        case 'n' :
            exit(0);
    }
}

void youWon()
{
    clearScreen;
    printf("\n\n                    YOU ARE A POFESSIONAL MINESWEEPER\n");
    printf("            YOU HAVE SWAPED %d BOMBS OUT OF %d X %d GRID SPACE\n\n", flags, gridSize, gridSize);
    printf("                              CONGRATULATION\n\n", flags, gridSize, gridSize);
    printf("                  __ __ _____ _____    _ _ _ _____ _____ \n");
    printf("                 |  |  |     |  |  |  | | | |     |   | |\n");
    printf("                 |_   _|  |  |  |  |  | | | |  |  | | | |\n");
    printf("                   |_| |_____|_____|  |_____|_____|_|___|\n");
    printf("\nWANT TO PLAY AGAIN? y/n\n");
    char ch = getch();
    switch(ch){
        case 'y' :
            clearScreen;
            main();
            break;
        case 'n' :
            exit(0);
    }
}

void setupGrid()
{
    time_t sec;
    time(&sec);
    srand(sec);
    for (int y=gridSize - 1;y >= 0 ;y--){
        for (int x=0;x<gridSize ;x++ ){
            grid[y][x] = '*';
            if(rand() % 5 == 0){
                mineGrid[y][x] = 1;
                bombCount(x,y);
                bombs++;
            }
            else if(!mineGrid[y][x]) mineGrid[y][x] = 0;
        }
    }
}

void showInstruction()
{
    clearScreen;
    printf("\n\n\n\n\n");
    printf("            WANT TO BE A PROFESSIONAL MINESWEEPER?\n");
    printf("            THEN FOLLOW THESE INSTRUCIONS\n\n");
    printf("            1.MOVE USING THE CURSORS\n");
    printf("            2.USE 'ENTER' TO CLEAN AN AREA\n");
    printf("            3.USE 'SPACEBAR' TO FLAG A SPACE AS A MINE\n");
    printf("            4.FLAG EVERY MINE IN THE FIELD AND YOUR JOB IS DONE\n");
    getch();
}

void showHighScore()
{
    printf("highscore");
    getch();
}

int difficulty()
{
    char menu[3][50]  ;
    int temp, nowAt = 0;

    while(nowAt + 1){
        strcpy(menu[0], " EASY");
        strcpy(menu[1], " MEDIUM");
        strcpy(menu[2], " HARD");

        printf("\n\n\n\n\n\n\n\n");
        menu[nowAt][0] = '>';
        printf("                                %s\n                                %s\n                                %s\n",menu[0],menu[1],menu[2]);
        char ch = -getch();
        if(ch == downArrow){
            if(nowAt != 2) nowAt++;
        }
        else if(ch == upArrow){
            if(nowAt != 0) nowAt--;
        }
        else if(ch == enterKey){
            if(nowAt == 0){
                temp =  10;
                nowAt = -1;
            }
            else if(nowAt == 1){
                temp =  15;
                nowAt = -1;
            }
            else if( nowAt == 2){
                temp =  20;
                nowAt = -1;
            }
        }
        clearScreen;
    }
    return temp;
}

void welcomeScreen()
{


    char menu[3][50]  ;
    int nowAt = 0;

    while(nowAt + 1){
        strcpy(menu[0], " START GAME");
        strcpy(menu[1], " INSTRUCTIONS");

        printf("\n\n");
        printf("        _____ _____ _____ _____ _____ _ _ _ _____ _____ _____ _____ _____ \n");
        printf("       |     |     |   | |   __|   __| | | |   __|   __|  _  |   __| __  |\n");
        printf("       | | | |-   -| | | |   __|__   | | | |   __|   __|   __|   __|    -|\n");
        printf("       |_|_|_|_____|_|___|_____|_____|_____|_____|_____|__|  |_____|__|__|\n\n\n");
        menu[nowAt][0] = '>';
        printf("                                %s\n                                %s\n",menu[0],menu[1]);
        char ch = -getch();
        if(ch == downArrow){
            if(nowAt != 1) nowAt++;
        }
        else if(ch == upArrow){
            if(nowAt != 0) nowAt--;
        }
        else if(ch == enterKey){
            if(nowAt == 0){
                system("clear");
                gridSize = difficulty();
                nowAt = -1;
            }
            else if(nowAt == 1){
                showInstruction();
            }
            else if( nowAt == 2){
                showHighScore();
            }
        }
        clearScreen;
    }

}

int main()
{
    int check = 1;

    welcomeScreen();
    setupGrid();

    while(check){
        printUpdatedGrid();
        check = getAction();
    }
    return 0;
}
