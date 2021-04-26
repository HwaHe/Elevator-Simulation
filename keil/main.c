#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "status.h"


void main()
{
	LCD_Init();
    showStatus(LEFT, UP, 3);
    delay(3000);
    showStatus(RIGHT, DOWN, 4);
    delay(3000);
    showStatus(RIGHT, STILL, 4);
    while(1);
}