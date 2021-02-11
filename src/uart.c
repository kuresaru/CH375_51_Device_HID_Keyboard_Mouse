#include "uart.h"

void UartInit(void)		//115200bps@22.1184MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

static void uart_send(uint8_t data)
{
    SBUF = data;
    while (!TI)
    {
    }
    TI = 0;
}

void print(const char *str)
{
    char *s;
    for (s = (char *)str; *s; s++)
    {
        uart_send(*s);
    }
}

void print_8x(uint8_t data)
{
    uint8_t c;
    c = (data >> 4) & 0xF;
    c += 0x30;
    if (c > 0x39)
    {
        c += 7;
    }
    uart_send(c);
    c = data & 0xF;
    c += 0x30;
    if (c > 0x39)
    {
        c += 7;
    }
    uart_send(c);
}

void print_8d(uint8_t data)
{
    uart_send((data / 100) + 0x30);
    uart_send((data / 10 % 10) + 0x30);
    uart_send((data % 10) + 0x30);
}