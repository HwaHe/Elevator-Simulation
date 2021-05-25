#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "status.h"

#define uchar unsigned char
#define uint unsigned int

void init();

void showElevStatus();

void onButtonClick();

void two2FourDecode(uchar flagEl, uchar flagEr, uchar flagUp, uchar flagDown);

int hasUpRequest(int request, int floor);

int hasDownRequest(int request, int floor);

int clearBit(int num, int pos);

int setBit(int num, int pos);

void showIndicators();

void elevatorSche();

typedef struct Elevator {
    uint index;
    uint floor;
    uint status;
} Elev;

Elev leftElev, rightElev;
uchar el = 0, er = 0, up = 0, down = 0;

void main() {
    uint k;
    init();
    while (1) {
        onButtonClick();

        elevatorSche();

        for (k = 0; k < 128; k++) {
            onButtonClick();
        }
    }
}

void init() {
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

void showElevStatus() {
    // 显示左电梯状态和楼层
    LCD_SetCursor(1, 3);
    LCD_WriteData(leftElev.status);     // 显示自定义字符位置
    LCD_ShowNum(1, 4, leftElev.floor, 2);

    LCD_SetCursor(1, 12);
    LCD_WriteData(rightElev.status);
    LCD_ShowNum(1, 13, rightElev.floor, 2);
}

void onButtonClick() {
    // 如果P1_7高电平，通过8-3编码器得到按键数值
    if (P1_7) {
        el |= (1 << ((P1_6 * 4) | (P1_5 * 2) | P1_4));
    }

    if (P3_7) {
        er |= (1 << ((P3_6 * 4) | (P3_5 * 2) | P3_4));
    }

    if (P1_3) {
        up |= (1 << ((P1_2 * 4) | (P1_1 * 2) | P1_0));
    }

    if (P3_3) {
        down |= (1 << ((P3_2 * 4) | (P3_1 * 2) | P3_0));
    }

    // 点亮对应的指示灯
    showIndicators();
}

void showIndicators() {
    uchar i;
    for (i = 0; i < 8; i++) {
        // 给P2低三位赋按键位置值
        P2_0 = i & 0x01;
        P2_1 = i & 0x02;
        P2_2 = i & 0x04;

        // 通过2-4译码器使能对应的3-8译码器，点亮对应的指示灯
        two2FourDecode(
                (el >> i) & 0x01,
                (er >> i) & 0x01,
                (up >> i) & 0x01,
                (down >> i) & 0x01
        );
    }
}

void updateIndicators() {
    if (leftElev.status == UP) {
        up = clearBit(up, leftElev.floor);
        el = clearBit(el, leftElev.floor);

        // 是为了接到最高楼层的向下人员而处于上升的状态，而且当前楼层是向下人员中的最高层
        if (hasUpRequest(el, leftElev.floor) <= 0 && hasUpRequest(up, leftElev.floor) <= 0 &&
            hasUpRequest(down, leftElev.floor) <= 0) {
            down = clearBit(down, leftElev.floor);
        }
    } else if (leftElev.status == DOWN) {
        down = clearBit(down, leftElev.floor);
        el = clearBit(el, leftElev.floor);

        //为了接到最低楼层的向上人员，而且当前楼层是向上人员中的最底层
        if (hasDownRequest(el, leftElev.floor) <= 0 && hasDownRequest(down, leftElev.floor) <= 0 &&
            hasDownRequest(up, leftElev.floor) <= 0) {
            up = clearBit(up, leftElev.floor);
        }
    }

    if (rightElev.status == UP) {
        up = clearBit(up, rightElev.floor);
        er = clearBit(er, rightElev.floor);

        // 为了接到最高楼层的向上人员而处于向上的状态，而且当前楼层是向下人员中的最高层
        if (hasUpRequest(el, rightElev.floor) <= 0 && hasUpRequest(up, rightElev.floor) <= 0 &&
            hasUpRequest(down, rightElev.floor) <= 0) {
            down = clearBit(down, rightElev.floor);
        }
    } else if (rightElev.status == DOWN) {
        down = clearBit(down, rightElev.floor);
        er = clearBit(er, rightElev.floor);

        // 为了接到最低楼层的向上人员，而且当前楼层是向上人员中的最底层
        if (hasDownRequest(el, rightElev.floor) <= 0 && hasDownRequest(down, rightElev.floor) <= 0 &&
            hasDownRequest(up, rightElev.floor) <= 0) {
            up = clearBit(up, rightElev.floor);
        }
    }
}

void two2FourDecode(uchar flagEl, uchar flagEr, uchar flagUp, uchar flagDown) {
    if (flagEl) {
        P2_3 = 0;
        P2_4 = 0;
        P2_5 = 0;
        delay(1);
    }
    if (flagEr) {
        P2_3 = 1;
        P2_4 = 0;
        P2_5 = 0;
        delay(1);
    }
    if (flagUp) {
        P2_3 = 0;
        P2_4 = 1;
        P2_5 = 0;
        delay(1);
    }
    if (flagDown) {
        P2_3 = 1;
        P2_4 = 1;
        P2_5 = 0;
        delay(1);
    }
    P2_5 = 1;
}

int hasUpRequest(int request, int floor) {
    return request >> floor;
}

int hasDownRequest(int request, int floor) {
    return request & (~((request >> (floor - 1)) << (floor - 1)));
}

int most(int x) {
    // binary search
    int res;
    unsigned y;
    int n = 32;
    y = x >> 16;
    if (y != 0) {
        n = n - 16;
        x = y;
    }
    y = x >> 8;
    if (y != 0) {
        n = n - 8;
        x = y;
    }
    y = x >> 4;
    if (y != 0) {
        n = n - 4;
        x = y;
    }
    y = x >> 2;
    if (y != 0) {
        n = n - 2;
        x = y;
    }
    y = x >> 1;
    if (y != 0) res = n - 2;
    res = n - x;
    return 32 - res;
}

int least(int x) {
    // binary search
    int n;
    if (x == 0) return 0;
    n = 1;
    if ((x & 0x0000FFFF) == 0) {
        n = n + 16;
        x = x >> 16;
    }
    if ((x & 0x000000FF) == 0) {
        n = n + 8;
        x = x >> 8;
    }
    if ((x & 0x0000000F) == 0) {
        n = n + 4;
        x = x >> 4;
    }
    if ((x & 0x00000003) == 0) {
        n = n + 2;
        x = x >> 2;
    }
    return n - (x & 1) + 1;
}

int setBit(int num, int pos) {
    num |= (1 << (pos - 1));
    return num;
}

int clearBit(int num, int pos) {
    num &= ~(1 << (pos - 1));
    return num;
}

void elevatorSche() {
    if (leftElev.status == STILL) {
        if (rightElev.status == STILL) {
            // 静止，静止
            // 优先响应来自内部的请求
            if (clearBit(el, leftElev.floor) != 0) {       // 如果左边有内部请求
                if (hasUpRequest(el, leftElev.floor)) {
                    leftElev.status = UP;
                    leftElev.floor++;
                } else if (hasDownRequest(el, leftElev.floor)) {
                    leftElev.status = DOWN;
                    leftElev.floor--;
                }
            }
            if (clearBit(er, rightElev.floor != 0))       // 如果右边有内部请求
            {
                if (hasUpRequest(er, rightElev.floor)) {
                    rightElev.status = UP;
                    rightElev.floor++;
                } else if (hasDownRequest(er, rightElev.floor)) {
                    rightElev.status = DOWN;
                    rightElev.floor--;
                }
            }
            if (leftElev.status == STILL || rightElev.floor == STILL) {         // 如果两个没有都被dispatch，尝试响应up和down
                if (leftElev.status == STILL) {         // 如果是左边没有被dispatch
                    if (clearBit(up, leftElev.floor) != 0) {    // 尝试响应up
                        if (hasUpRequest(up, leftElev.floor)) {
                            leftElev.status = UP;
                            leftElev.floor++;
                        } else if (hasDownRequest(up, leftElev.floor)) {
                            leftElev.status = DOWN;
                            leftElev.floor--;
                        }
                    } else if (clearBit(down, leftElev.floor) != 0) {  // 尝试响应down
                        if (hasUpRequest(down, leftElev.floor)) {
                            leftElev.status = UP;
                            leftElev.floor++;
                        } else if (hasDownRequest(down, leftElev.floor)) {
                            leftElev.status = DOWN;
                            leftElev.floor--;
                        }
                    }
                } else {                              // 如果是右边没有被dispatch
                    if (clearBit(up, rightElev.floor) != 0) {   // 尝试响应up
                        if (hasUpRequest(up, rightElev.floor)) {
                            rightElev.status = UP;
                            rightElev.floor++;
                        } else if (hasDownRequest(up, rightElev.floor)) {
                            rightElev.status = DOWN;
                            rightElev.floor--;
                        }
                    } else if (clearBit(down, rightElev.floor) != 0) {   // 尝试响应down
                        if (hasUpRequest(down, rightElev.floor)) {
                            rightElev.status = UP;
                            rightElev.floor++;
                        } else if (hasDownRequest(down, rightElev.floor)) {
                            rightElev.status = DOWN;
                            rightElev.floor--;
                        }
                    }
                }
            }
        } else if (rightElev.status == UP) {
            // 静止，向上
            if (clearBit(el, leftElev.floor) != 0) {       // 如果左边的有内部请求，响应
                if (hasUpRequest(el, leftElev.floor)) {
                    leftElev.status = UP;
                    leftElev.floor++;
                } else if (hasDownRequest(el, leftElev.floor)) {
                    leftElev.status = DOWN;
                    leftElev.floor--;
                }
            } else if (clearBit(up, leftElev.floor) != 0 || clearBit(down, leftElev.floor) != 0) {   // 如果没有，尝试响应外部请求
                if (hasUpRequest(up, leftElev.floor) || hasUpRequest(down, leftElev.floor)) {   // 外部请求有向上的
                    leftElev.status = UP;
                    leftElev.floor++;
                } else {      // 外部请求有向下的
                    leftElev.status = DOWN;
                    leftElev.floor++;
                }
            }

            if (hasUpRequest(up, rightElev.floor) || hasUpRequest(er, rightElev.floor)) {   // 如果还存在上部的请求，继续上升
                rightElev.floor++;
            } else if (hasDownRequest(down, rightElev.floor) ||
                       hasDownRequest(er, rightElev.floor)) {   // 否则，检查是否存在下方请求
                rightElev.status = DOWN;
                rightElev.floor--;
            } else {  // 都没有，直接静置右边电梯
                rightElev.status = STILL;
            }
        } else {
            // 静止，向下
            if (clearBit(el, leftElev.floor) != 0) {       // 如果左边的有内部请求，响应
                if (hasUpRequest(el, leftElev.floor)) {
                    leftElev.status = UP;
                    leftElev.floor++;
                } else if (hasDownRequest(el, leftElev.floor)) {
                    leftElev.status = DOWN;
                    leftElev.floor--;
                }
            } else if (clearBit(up, leftElev.floor) != 0 || clearBit(down, leftElev.floor) != 0) {   // 如果没有，尝试响应外部请求
                if (hasUpRequest(up, leftElev.floor) || hasUpRequest(down, leftElev.floor)) {   // 外部请求有向上的
                    leftElev.status = UP;
                    leftElev.floor++;
                } else {      // 外部请求有向下的
                    leftElev.status = DOWN;
                    leftElev.floor++;
                }
            }

            if (hasDownRequest(down, rightElev.floor) || hasDownRequest(er, rightElev.floor)) {   // 如果还存在向下的请求，继续下降
                rightElev.floor--;
            } else if (hasUpRequest(up, rightElev.floor) || hasUpRequest(er, rightElev.floor)) {   // 否则，检查是否存在上方请求
                rightElev.status = UP;
                rightElev.floor++;
            } else {  // 都没有，静置右边电梯
                rightElev.status = STILL;
            }
        }
    } else if (leftElev.status == UP) {
        if (rightElev.status == STILL) {
            // 向上，静止
            if (clearBit(er, rightElev.floor) != 0) {  // 静止的优先响应内部请求
                if (hasUpRequest(er, rightElev.floor)) {
                    rightElev.status = UP;
                    rightElev.floor++;
                } else if (hasDownRequest(er, rightElev.floor)) {
                    rightElev.status = DOWN;
                    rightElev.floor--;
                }
            } else if (clearBit(up, rightElev.floor) != 0 || clearBit(down, rightElev.floor) != 0) {   // 内部无请求，响应外部请求
                if (hasUpRequest(up, rightElev.floor) || hasUpRequest(down, rightElev.floor)) {
                    rightElev.status = UP;
                    rightElev.floor++;
                } else {
                    rightElev.status = DOWN;
                    rightElev.floor--;
                }
            }

            if (hasUpRequest(up, leftElev.floor) || hasUpRequest(el, leftElev.floor)) {  // 如果向上的电梯在上方还有向上的请求，继续向上
                leftElev.floor++;
            } else if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) { // 如果没有，检查向下的请求
                leftElev.status = DOWN;
                leftElev.floor--;
            } else {  // 如果也没有，直接设置位静止
                leftElev.status = STILL;
            }
        } else if (rightElev.status == UP) {
            // 向上， 向上
            if (hasUpRequest(el, leftElev.floor) || hasUpRequest(up, leftElev.floor)) {
                leftElev.floor++;
            } else if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) {
                leftElev.status = DOWN;
                leftElev.floor--;
            } else {
                leftElev.status = STILL;
            }

            if (hasUpRequest(er, rightElev.floor) || hasUpRequest(up, rightElev.floor)) {
                rightElev.floor++;
            } else if (hasDownRequest(er, rightElev.floor) || hasDownRequest(down, rightElev.floor)) {
                rightElev.status = DOWN;
                rightElev.floor--;
            } else {
                rightElev.status = STILL;
            }
        } else {
            // 向上，向下
            if (hasUpRequest(el, leftElev.floor) || hasUpRequest(up, leftElev.floor)) {  // 如果左边电梯有向上请求，继续向上
                leftElev.floor++;
            } else if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) { // 如果没有，检查向下的请求
                leftElev.status = DOWN;
                leftElev.floor--;
            } else {  // 否则，静止左边电梯
                leftElev.status = STILL;
            }

            if (hasDownRequest(er, rightElev.floor) || hasDownRequest(down, rightElev.floor)) { // 如果右边电梯有向下请求，继续向下
                rightElev.floor--;
            } else if (hasUpRequest(up, rightElev.floor) || hasUpRequest(er, rightElev.floor)) { // 如果没有，检查向上的请求
                rightElev.status = UP;
                rightElev.floor++;
            } else {  // 否则，静止右边电梯
                rightElev.status = STILL;
            }
        }

    } else {
        if (rightElev.status == STILL) {
            // 向下，静止
            if (clearBit(er, rightElev.floor) != 0) {
                if (hasUpRequest(er, rightElev.floor)) {
                    rightElev.status = UP;
                    rightElev.floor++;
                } else if (hasDownRequest(er, rightElev.floor)) {
                    rightElev.status = DOWN;
                    rightElev.floor--;
                }
            } else if (clearBit(up, rightElev.floor) != 0 || clearBit(down, rightElev.floor) != 0) {
                if (hasUpRequest(up, rightElev.floor) || hasUpRequest(down, rightElev.floor)) {
                    rightElev.status = UP;
                    rightElev.floor++;
                } else {
                    rightElev.status = DOWN;
                    rightElev.floor--;
                }
            }

            if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) {
                leftElev.floor--;
            } else if (hasUpRequest(el, leftElev.floor) || hasUpRequest(up, leftElev.floor)) {
                leftElev.status = UP;
                leftElev.floor++;
            } else {
                leftElev.status = STILL;
            }
        } else if (rightElev.status == UP) {
            // 向下， 向上
            if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) {
                leftElev.floor--;
            } else if (hasUpRequest(el, leftElev.floor) || hasUpRequest(up, leftElev.floor)) {
                leftElev.status = UP;
                leftElev.floor++;
            } else {
                leftElev.floor = STILL;
            }

            if (hasUpRequest(er, rightElev.floor) || hasUpRequest(up, rightElev.floor)) {
                rightElev.floor++;
            } else if (hasDownRequest(er, rightElev.floor) || hasDownRequest(down, rightElev.floor)) {
                rightElev.status = DOWN;
                rightElev.floor--;
            } else {
                rightElev.status = STILL;
            }
        } else {
            // 向下，向下
            if (hasDownRequest(el, leftElev.floor) || hasDownRequest(down, leftElev.floor)) {
                leftElev.floor--;
            } else if (hasUpRequest(el, leftElev.floor) || hasUpRequest(up, leftElev.floor)) {
                leftElev.status = UP;
                leftElev.floor++;
            } else {
                leftElev.status = STILL;
            }

            if (hasDownRequest(er, rightElev.floor) || hasDownRequest(down, rightElev.floor)) {
                rightElev.floor--;
            } else if (hasUpRequest(er, rightElev.floor) || hasUpRequest(up, rightElev.floor)) {
                rightElev.status = UP;
                rightElev.floor++;
            } else {
                rightElev.status = STILL;
            }

        }
    }

    // 更新LCD显示器和指示灯
    updateIndicators();
    showElevStatus();
}
