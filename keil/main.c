#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "status.h"

int el = 0, er = 0, up = 0, down = 0;

typedef struct Elevator
{
    unsigned int index;
    unsigned int floor;
    unsigned int status;
} Elev;

Elev leftElev, rightElev;

void init();
void showElevStatus();
void onButtonClick();
void two2FourDecode(unsigned char flagEl, unsigned char flagEr, unsigned char flagUp, unsigned char flagDown);
void showIndicators();

void main()
{
    unsigned int k = 0;
    init();
    while (1)
    {

        onButtonClick();
        showIndicators();

        el = 0;
        er = 0;
        up = 0;
        down = 0;

        for (k = 0; k < 128; k++)
        {
            onButtonClick();
            showIndicators();
        }
    }
}

void init()
{
    // 初始化LCD显示屏
    LCD_Init();

    // 初始化左边电梯
    leftElev.index = LEFT;
    leftElev.floor = 1;
    leftElev.status = STILL;
    // 初始化右边电梯
    rightElev.index = RIGHT;
    rightElev.floor = 1;
    rightElev.status = STILL;
    showElevStatus();
}

void showElevStatus()
{
    showStatus(LEFT, leftElev.status, leftElev.floor);
    showStatus(RIGHT, rightElev.status, rightElev.floor);
}

void onButtonClick()
{
    if (P1_7)
    {
        el |= (1 << ((P1_6 * 4) | (P1_5 * 2) | P1_4));
    }

    if (P3_7)
    {
        er |= (1 << ((P3_6 * 4) | (P3_5 * 2) | P3_4));
    }

    if (P1_3)
    {
        up |= (1 << ((P1_2 * 4) | (P1_1 * 2) | P1_0));
    }

    if (P3_3)
    {
        down |= (1 << ((P3_2 * 4) | (P3_1 * 2) | P3_0));
    }
}

void two2FourDecode(unsigned char flagEl, unsigned char flagEr, unsigned char flagUp, unsigned char flagDown)
{
    if (flagEl)
    {
        P2_3 = 0;
        P2_4 = 0;
        P2_5 = 0;
        delay(1);
    }
    if (flagEr)
    {
        P2_3 = 1;
        P2_4 = 0;
        P2_5 = 0;
        delay(1);
    }
    if (flagUp)
    {
        P2_3 = 0;
        P2_4 = 1;
        P2_5 = 0;
        delay(1);
    }
    if (flagDown)
    {
        P2_3 = 1;
        P2_4 = 1;
        P2_5 = 0;
        delay(1);
    }
    P2_5 = 1;
}

void showIndicators()
{
    unsigned char i;
    unsigned char flagEl, flagEr, flagUp, flagDown;
    for (i = 0; i < 8; i++)
    {
        P2_0 = i & 0x01;
        P2_1 = i & 0x02;
        P2_2 = i & 0x04;
        flagEl = (el >> i) & 1 == 1;
        flagEr = (er >> i) & 1 == 1;
        flagUp = (up >> i) & 1 == 1;
        flagDown = (down >> i) & 1 == 1;
        two2FourDecode(flagEl, flagEr, flagUp, flagDown);
        delayus(50);
    }
}
