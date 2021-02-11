#ifndef __JOY_H
#define __JOY_H
#include "main.h"
#include <stdint.h>

#define JOY_BTN1_LEFT (1 << 7)
#define JOY_BTN1_DOWN (1 << 6)
#define JOY_BTN1_RIGHT (1 << 5)
#define JOY_BTN1_UP (1 << 4)
#define JOY_BTN1_START (1 << 3)
#define JOY_BTN1_R3 (1 << 2)
#define JOY_BTN1_L3 (1 << 1)
#define JOY_BTN1_SELECT (1 << 0)
#define JOY_BTN2_SQUARE (1 << 7)
#define JOY_BTN2_X (1 << 6)
#define JOY_BTN2_CIRCLE (1 << 5)
#define JOY_BTN2_TRIANGLE (1 << 4)
#define JOY_BTN2_R1 (1 << 3)
#define JOY_BTN2_L1 (1 << 2)
#define JOY_BTN2_R2 (1 << 1)
#define JOY_BTN2_L2 (1 << 0)

typedef enum
{
    JOY_MODE_UNKNOWN = 0,
    JOY_MODE_OFF,
    JOY_MODE_ON,
} Joy_Mode_EnumDef;

extern uint8_t Joy_Mode; // 41灭 73亮
extern uint8_t Joy_Btn1; // BIT7> Left Down Right Up Start R3 L3 Select
extern uint8_t Joy_Btn2; // BIT7> 方 X 圆 三角 R1 L1 R2 L2
extern uint8_t Joy_RX;   // 右水平
extern uint8_t Joy_RY;   // 右垂直
extern uint8_t Joy_LX;   // 左水平
extern uint8_t Joy_LY;   // 左垂直
extern uint8_t Joy_VW;   // 右小震动
extern uint8_t Joy_VY;   // 左大震动

void Joy_Init();
void Joy_Sync();
Joy_Mode_EnumDef Joy_GetStatus();

#endif