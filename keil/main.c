#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "status.h"

int el=0, er=0, up=0, down=0;

typedef struct Elevator
{
    unsigned int index;
    unsigned int floor;
    unsigned int status;
}Elev;

Elev leftElev, rightElev;

void showElevStatus(Elev elev);

void init()
{
    LCD_Init();

    leftElev.index=LEFT;
    leftElev.floor=1;
    leftElev.status=STILL;
    showElevStatus(leftElev);

    rightElev.index=RIGHT;
    rightElev.floor=1;
    rightElev.status=STILL;
    showElevStatus(rightElev);
    
}

void showIndicators()
{
    unsigned int i;
    for(i=0;i<8;i++)
    {

    }
}

void showElevStatus(Elev elev)
{
    switch (elev.index)
    {
    case LEFT:
        showStatus(LEFT, elev.status, elev.floor);
        break;
    case RIGHT:
        showStatus(RIGHT, elev.status, elev.floor);
        break;
    }
}

void main()
{
    // init();
    // while (1)
    // {
    //     showIndicators();
    // }
    LCD_Init();
    delay(3000);
    showStatus(LEFT, UP, 4);
    delay(3000);
    showStatus(RIGHT, DOWN, 8);
    delay(3000);
    showStatus(LEFT, STILL, 1);
    while (1)
    {
        ;
    }
    
}