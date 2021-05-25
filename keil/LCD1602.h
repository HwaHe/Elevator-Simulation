#ifndef __LCD1602_H__
#define __LCD1602_H__

// 初始化液晶显示器
void LCD_Init();

// 设置输出游标位置
void LCD_SetCursor(unsigned char Line, unsigned char Column);

// 向LCD1602写数据
void LCD_WriteData(unsigned char Data);

// 向LCD1602写命令
void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char);

// 在LCD1602指定行和列显示字符串
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String);

// 在LCD指定行和列显示数字
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);

// 在LCD指定行和列显示有符号数字
void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Number, unsigned char Length);

// 在LCD指定行和列显示十六进制数字
void LCD_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);

// 在LCD指定行和列显示二进制数字
void LCD_ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);

#endif
