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

void init()
{
    LCD_Init();

    leftElev.index=LEFT;
    leftElev.floor=1;
    leftElev.status=STILL;
    showStatus(leftElev);

    rightElev.index=RIGHT;
    rightElev.floor=1;
    rightElev.status=STILL;
    showStatus(rightElev);
    
}

void showIndicators()
{
    unsigned int i;
    for(i=0;i<8;i++)
    {
        
    }
}

void showStatus(Elev elev)
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
    init();
    while (1)
    {
        showIndicators();
    }
    
}