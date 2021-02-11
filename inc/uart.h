#ifndef __UART_H
#define __UART_H
#include "main.h"

void UartInit(void);
void print(const char *str);
void print_8x(uint8_t data);
void print_8d(uint8_t data);

#endif