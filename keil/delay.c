
void delay(unsigned int xms)
{
    while(xms--)
    {
        unsigned char i, j;

        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}

void delayus(unsigned int xus)
{
    while(xus--);
}