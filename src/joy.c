#include "joy.h"

#define Delay_2us Delay1us(); Delay1us()

#define JOY_CS   P0_2   // ATT
#define JOY_CLK  P0_3   // CLK
#define JOY_MISO P0_0   // DAT
#define JOY_MOSI P0_1   // CMD

void Delay1us();

uint8_t Joy_Mode;   // 41灭 73亮
uint8_t Joy_Btn1;   // Left Down Right Up Start R3 L3 Select
uint8_t Joy_Btn2;   // 方 X 圆 三角 R1 L1 R2 L2
uint8_t Joy_RX;     // 右水平
uint8_t Joy_RY;     // 右垂直
uint8_t Joy_LX;     // 左水平
uint8_t Joy_LY;     // 左垂直
uint8_t Joy_VW = 0; // 右小震动
uint8_t Joy_VY = 0; // 左大震动

uint8_t Joy_RW(uint8_t write)
{
    uint8_t read;
#define DL Delay_2us; Delay_2us; Delay1us()
    uint8_t i;
    read = 0;
    for (i = 0; i < 8; i++)
    {
        JOY_MOSI = write & 1 ? 1 : 0;
        DL;
        JOY_CLK = 0;
        DL;
        read >>= 1;
        read |= JOY_MISO & 1 ? 0x80 : 0;
        JOY_CLK = 1;
        write >>= 1;
    }
    return read;
}

//short poll
void Joy_ShortPoll(void)
{
	JOY_CS = 0;
    Delay_2us;
	Joy_RW(0x01);  
	Joy_RW(0x42);  
	Joy_RW(0X00);
	Joy_RW(0x00);
	Joy_RW(0x00);
	Delay_2us;
    JOY_CS = 1;
    Delay_2us;
}

//进入配置
void Joy_EnterConfing(void)
{
    JOY_CS = 0;
    Delay_2us;
	Joy_RW(0x01);  
	Joy_RW(0x43);  
	Joy_RW(0X00);
	Joy_RW(0x01);
	Joy_RW(0x00);
	// Joy_RW(0X00);
	// Joy_RW(0X00);
	// Joy_RW(0X00);
	// Joy_RW(0X00);
	Delay_2us;
    JOY_CS = 1;
    Delay_2us;
}

//发送模式设置
void Joy_TurnOnAnalogMode(void)
{
	JOY_CS = 0;
    Delay_2us;
	Joy_RW(0x01);  
	Joy_RW(0x44);  
	Joy_RW(0X00);
	Joy_RW(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	Joy_RW(0x03); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	Joy_RW(0X00);
	Joy_RW(0X00);
	Joy_RW(0X00);
	Joy_RW(0X00);
	JOY_CS = 1;
    Delay_2us;
}

//振动设置
void Joy_VibrationMode(void)
{
	JOY_CS = 0;
    Delay_2us;
    Joy_RW(0x01);
    Joy_RW(0x4D);
    Joy_RW(0x00);
    Joy_RW(0x00);
    Joy_RW(0x01);
    Delay_2us;
    JOY_CS = 1;
    Delay_2us;
}

//完成并保存配置
void Joy_ExitConfing(void)
{
    JOY_CS = 0;
    Delay_2us;
	Joy_RW(0x01);  
	Joy_RW(0x43);  
	Joy_RW(0X00);
	Joy_RW(0x00);
	Joy_RW(0x5A);
	Joy_RW(0x5A);
	Joy_RW(0x5A);
	Joy_RW(0x5A);
	Joy_RW(0x5A);
	Delay_2us;
    JOY_CS = 1;
    Delay_2us;
}

void Joy_Init()
{
    JOY_CS = 1;
    JOY_CLK = 1;
    JOY_MOSI = 0;
    JOY_MISO = 1;
	Joy_EnterConfing();		
	Joy_TurnOnAnalogMode();	
	Joy_VibrationMode();	
	Joy_ExitConfing();		
}

void Joy_Sync()
{
    JOY_CS = 0;
    Delay_2us;             // SI SO
    Joy_RW(0x01);              // 01 xx
    Joy_Mode = Joy_RW(0x42);   // 42 id
    Joy_RW(0x00);              // xx 5A
    Joy_Btn1 = Joy_RW(Joy_VW); // ww b1
    Joy_Btn2 = Joy_RW(Joy_VY); // yy b2
    Joy_RX = Joy_RW(0x00);     // xx rx
    Joy_RY = Joy_RW(0x00);     // xx ry
    Joy_LX = Joy_RW(0x00);     // xx lx
    Joy_LY = Joy_RW(0x00);     // xx ly
    Delay_2us;
    JOY_CS = 1;
    Delay_2us;
}

Joy_Mode_EnumDef Joy_GetStatus()
{
    if (Joy_Mode == 0x41)
        return JOY_MODE_OFF;
    else if (Joy_Mode == 0x73)
        return JOY_MODE_ON;
    else
        return JOY_MODE_UNKNOWN;
}
