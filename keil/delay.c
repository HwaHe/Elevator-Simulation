/**
* @brief 晶振为12MHz时，延时时长为x ms
  */
void delay(unsigned int xms) {
    while (xms--) {
        unsigned char i, j;

        i = 2;
        j = 239;
        do {
            while (--j);
        } while (--i);
    }
}

/**
* @brief 如你所见，是一个不准确的微妙延时函数
  */
void delayus(unsigned int xus) {
    while (xus--);
}