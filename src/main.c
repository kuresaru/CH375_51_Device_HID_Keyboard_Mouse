#include "main.h"
#include "uart.h"
#include "joy.h"

#define DATA P2
#define A0 P3_7
#define CSn P4_1
#define RDn P4_2
#define WRn P4_4
#define LEDn P3_3

uint8_t __code DeviceDescriptor[18] = {
    18,         // bLength
    0x01,       // bDescriptorType
    0x00, 0x02, // bcdUSB
    0x00,       // bDeviceClass
    0x00,       // bDeviceSubClass
    0x00,       // bDeviceProtocol
    8,          // bMaxPacketSize0
    0x34, 0x12, // idVentor
    0x78, 0x56, // idProduct
    0x01, 0x00, // bcdDevice
    0,          // iManufacturer
    0,          // iProduct
    0,          // iSerialNumber
    1,          // bNumConfiguration
};

uint8_t __code ReportDescriptor_Mouse[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x09, 0x01, //   Usage (Pointer)
    0xA1, 0x00, //   Collection (Physical)
    0x05, 0x09, //     Usage Page (Buttons)
    0x19, 0x01, //     Usage Minimum (1)
    0x29, 0x05, //     Usage Maximum (5)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x95, 0x05, //     Report Count (5)
    0x75, 0x01, //     Report Size (1)
    0x81, 0x02, //     Input (Data, Variable, Absolute)
    0x95, 0x01, //     Report Count (1)
    0x75, 0x03, //     Report Size (3)
    0x81, 0x01, //     Input (Constant)
    0x05, 0x01, //     Usage Page (Generic Desktop)
    0x09, 0x30, //     Usage (X)
    0x09, 0x31, //     Usage (Y)
    0x09, 0x38, //     Usage (Wheel)
    0x15, 0x81, //     Logical Minimum (-127)
    0x25, 0x7F, //     Logical Maximum (127)
    0x75, 0x08, //     Report Size (8)
    0x95, 0x03, //     Report Count (3)
    0x81, 0x06, //     Input (Data, Variable, Relative)
    0xC0,       //   End Collection
    0xC0,       // End Collection
};

uint8_t __code ReportDescriptor_Keyboard[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    0x05, 0x07, //   Usage Page (Key Codes)
    0x19, 0xE0, //   Usage Minimum (224)
    0x29, 0xE7, //   Usage Maximum (231)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x75, 0x01, //   Report Size (1)
    0x95, 0x08, //   Report Count (8)
    0x81, 0x02, //   Input (Data, Variable, Absolute)  ; Modifier byte
    0x95, 0x01, //   Report Count (1)
    0x75, 0x08, //   Report Count (8)
    0x81, 0x01, //   Input (Constant)                  ; Reserved byte
    0x95, 0x05, //   Report Count (5)
    0x75, 0x01, //   Report Size (1)
    0x05, 0x08, //   Usage Page (Page# for LEDs)
    0x19, 0x01, //   Usage Minimum (1)
    0x29, 0x05, //   Usage Maximum (5)
    0x91, 0x02, //   Output (Data, Variable, Absolute) ; LED report
    0x95, 0x01, //   Report Count (1)
    0x75, 0x03, //   Report Size (3)
    0x91, 0x01, //   Output (Constant)                 ; LED report padding
    0x95, 0x06, //   Report Count (6)
    0x75, 0x08, //   Report Size (8)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x65, //   Logical Maximum (101)
    0x05, 0x07, //   Usage Page (Key Codes)
    0x19, 0x00, //   Usage Minimum (0)
    0x29, 0x65, //   Usage Maximum (101)
    0x81, 0x00, //   Input (Data, Array)               ; Key arrays (6 bytes)
    0xC0,       //  End Collection
};

#define bwVal(x) ((x)&0xFF), (((x) >> 8) & 0xFF)
#define CFGDES_SIZE (9 + 9 + 9 + 7 + 9 + 9 + 7 + 7)

uint8_t __code ConfigurationDescriptor[CFGDES_SIZE] = {
    // configuration
    9,                  // bLength
    0x02,               // bDescriptorType
    bwVal(CFGDES_SIZE), // wTotalLength
    2,                  // bNumInterfaces
    1,                  // bConfigurationValue
    0,                  // iConfiguration
    0xE0,               // bmAttributes
    250,                // bMaxPower
    // interface mouse
    9,    // bLength
    0x04, // bDescriptorType
    0x00, // bInterfaceNumber
    0,    // bAlternateSetting
    1,    // bNumEndpoints
    0x03, // bInterfaceClass(HID)
    0x01, // bInterfaceSubClass(Boot Interface)
    0x02, // bInterfaceProtocol(Mouse)
    0,    // iInterface
    // hid
    9,                                     // bLength
    0x21,                                  // bDescryptorType
    0x00, 0x01,                            // bcdHID
    0x00,                                  // bContryCode
    1,                                     // bNumDescryptors
    0x22,                                  // bDescriptorType
    bwVal(sizeof(ReportDescriptor_Mouse)), // wDescriptorLength
    // endpoint
    7,          // bLength
    0x05,       // bDescriptorType
    0x81,       // bEndpointAddress(IN1)
    0x03,       // bmAttributes(Interrupt)
    0x04, 0x00, // wMaxPacketSize
    0x06,       // bInterval (2^(n-1)) (1-16)  2^(6-1)=32ms  31.25Hz
    // interface keyboard
    9,    // bLength
    0x04, // bDescriptorType
    0x01, // bInterfaceNumber
    0,    // bAlternateSetting
    2,    // bNumEndpoints
    0x03, // bInterfaceClass(HID)
    0x01, // bInterfaceSubClass(Boot Interface)
    0x01, // bInterfaceProtocol(Keyboard)
    0,    // iInterface
    // hid
    9,                                        // bLength
    0x21,                                     // bDescryptorType
    0x00, 0x01,                               // bcdHID
    0x00,                                     // bContryCode
    1,                                        // bNumDescryptors
    0x22,                                     // bDescriptorType
    bwVal(sizeof(ReportDescriptor_Keyboard)), // wDescriptorLength
    // endpoint
    7,          // bLength
    0x05,       // bDescriptorType
    0x82,       // bEndpointAddress(IN2)
    0x03,       // bmAttributes(Interrupt)
    0x08, 0x00, // wMaxPacketSize
    0x06,       // bInterval (2^(n-1)) (1-16)  2^(6-1)=32ms  31.25Hz
    // endpoint
    7,          // bLength
    0x05,       // bDescriptorType
    0x02,       // bEndpointAddress(OUT2)
    0x03,       // bmAttributes(Interrupt)
    0x01, 0x00, // wMaxPacketSize
    0x00,       // bInterval
};

uint8_t CurrentSetupRequest = 0;
const uint8_t *CurrentDescriptor;
uint16_t CurrentDescriptor_Sent = 0;
uint16_t CurrentDescriptor_Size = 0;
uint8_t DeviceAddress = 0;

__bit configured = 0;

void Delay30ms() //@22.1184MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 3;
    j = 134;
    k = 115;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay1us() //@22.1184MHz
{
    unsigned char i;

    i = 3;
    while (--i)
        ;
}

void wr_cmd(uint8_t cmd)
{
    P2M0 = 0xFF;
    A0 = 1;
    DATA = cmd;
    Delay1us();
    WRn = 0;
    Delay1us();
    WRn = 1;
    Delay1us();
    A0 = 0;
    DATA = 0xFF;
    Delay1us();
}

void wr_data(uint8_t data)
{
    P2M0 = 0xFF;
    A0 = 0;
    DATA = data;
    Delay1us();
    WRn = 0;
    Delay1us();
    WRn = 1;
    Delay1us();
    A0 = 0;
    DATA = 0xFF;
    Delay1us();
}

void rd_data(uint8_t *data)
{
    P2M0 = 0x00;
    DATA = 0xFF;
    A0 = 0;
    Delay1us();
    RDn = 0;
    Delay1us();
    *data = DATA;
    RDn = 1;
    Delay1us();
    A0 = 0;
    Delay1us();
}

void sysinit()
{
    P2M1 = 0b00000000;
    P2M0 = 0b00000000;
    P3M1 = 0b00000000;
    P3M0 = 0b10000000;
    P4M1 = 0b00000000;
    P4M0 = 0b00010110;

    EA = 1;
    UartInit();
    Joy_Init();
    //32毫秒@22.1184MHz
    AUXR &= 0x7F; //定时器时钟12T模式
    TMOD &= 0xF0; //设置定时器模式
    TL0 = 0x9A;   //设置定时初值
    TH0 = 0x19;   //设置定时初值
    TF0 = 0;      //清除TF0标志
    TR0 = 1;      //定时器0开始计时
    ET0 = 1;
    EX0 = 0;
    INT0 = 1;
    IT0 = 1; // 下降中断0

    CSn = 1;
    DATA = 0xFF;
    RDn = 1;
    WRn = 1;
    A0 = 0;

    print("start...\r\n");
    Delay30ms();
    Delay30ms();
    Delay30ms();
    Delay30ms();
}

void main()
{
    uint8_t i;

    sysinit();

    CSn = 0;
    wr_cmd(0x05);
    CSn = 1;
    Delay30ms();
    Delay30ms();

    CSn = 0;

    wr_cmd(0x06);
    wr_data(0x57);
    rd_data(&i);
    haltif(i != 0xA8, "check exist error");
    print("check ok\r\n");

    wr_cmd(0x15);
    wr_data(0x01);
    for (;;)
    {
        rd_data(&i);
        if (i == 0x51)
        {
            break;
        }
    }

    EX0 = 1;

    while (1)
    {
    }
}

void ExI0_Isr() __interrupt(0)
{
    uint8_t i, len;
    uint8_t buf[8];
    __bit setup_error;

    wr_cmd(0x22); // get status
    rd_data(&i);
    switch (i)
    {
    case 0x0C:        // ep0 setup
        wr_cmd(0x28); // rd usb data
        rd_data(&i);  // read length
        setup_error = 1;
        if (i == 8) // 数据长度一定是8
        {
            // 读8字节的数据
            for (i = 0; i < 8; i++)
            {
                rd_data(buf + i);
            }
            CurrentSetupRequest = buf[1]; // bRequest
            switch (CurrentSetupRequest)
            {
            case 0x06:      // get descriptor
                i = buf[3]; // descriptor type
                switch (i)
                {
                case 0x01: // device descriptor
                    CurrentDescriptor = DeviceDescriptor;
                    CurrentDescriptor_Size = 18;
                    break;
                case 0x02: // configuration descriptor
                    CurrentDescriptor = ConfigurationDescriptor;
                    CurrentDescriptor_Size = (buf[6] < CFGDES_SIZE) ? buf[6] : CFGDES_SIZE;
                    break;
                case 0x22: // hid report descriptor
                    if (buf[4] == 0)
                    {
                        CurrentDescriptor = ReportDescriptor_Mouse;
                        CurrentDescriptor_Size = sizeof(ReportDescriptor_Mouse);
                    }
                    else if (buf[4] == 1)
                    {
                        CurrentDescriptor = ReportDescriptor_Keyboard;
                        CurrentDescriptor_Size = sizeof(ReportDescriptor_Keyboard);
                    }
                    else
                    {
                        CurrentDescriptor = 0;
                    }
                    break;
                default:
                    CurrentDescriptor = 0;
                    break;
                }
                if (CurrentDescriptor)
                {
                    wr_cmd(0x29); // wr usb data3
                    wr_data(8);
                    for (i = 0; i < 8; i++)
                    {
                        wr_data(CurrentDescriptor[i]);
                    }
                    CurrentDescriptor_Sent = 8;
                    setup_error = 0;
                }
                wr_cmd(0x23); // unlock usb
                break;
            case 0x05: // set address
                DeviceAddress = buf[2];
                setup_error = 0;
                wr_cmd(0x29); // wr usb data3
                wr_data(0);
                wr_cmd(0x23); // unlock usb
                break;
            case 0x09: // set configuration
                setup_error = 0;
                wr_cmd(0x29); // wr usb data3
                wr_data(0);
                wr_cmd(0x23); // unlock usb
                configured = 1;
                break;
            }
        }
        if (setup_error)
        {
            wr_cmd(0x19);  // set endp 3
            wr_data(0x0F); // stall
            wr_cmd(0x23);  // unlock usb
        }
        break;
    case 0x08:        // ep0 in
        wr_cmd(0x23); // unlock usb
        if ((CurrentSetupRequest == 0x06) && (CurrentDescriptor))
        {
            len = CurrentDescriptor_Size - CurrentDescriptor_Sent;
            len = (len > 8) ? 8 : len;
            wr_cmd(0x29); // wr usb data3
            wr_data(len);
            for (i = 0; i < len; i++)
            {
                wr_data(CurrentDescriptor[CurrentDescriptor_Sent]);
                CurrentDescriptor_Sent++;
            }
        }
        else if (CurrentSetupRequest == 0x05)
        {
            wr_cmd(0x13);
            wr_data(DeviceAddress);
        }
        break;
    case 0x00:        // ep0 out
        wr_cmd(0x23); // unlock usb
        break;
    case 0x02:         // ep2 out
        wr_cmd(0x28);  // rd usb data
        rd_data(&len); // read length
        for (i = 0; i < len; i++)
        {
            rd_data(buf + i);
        }
        LEDn = !(buf[0] & 0x01);
        break;
    default:
        wr_cmd(0x23); // unlock usb
        rd_data(&i);  // dummy
        break;
    }
}

__bit keyboard_down_flag = 0;
void T0_Isr() __interrupt(1)
{
    int8_t mouse_x, mouse_y, mouse_wheel;
    uint8_t mouse_btn;
    Joy_Sync();
    if (configured)
    {
        // mouse
        mouse_btn = ((!(Joy_Btn2 & JOY_BTN2_L1)) << 0) |
                    ((!(Joy_Btn2 & JOY_BTN2_R1)) << 1) |
                    ((!(Joy_Btn2 & JOY_BTN2_L2)) << 3) |
                    ((!(Joy_Btn2 & JOY_BTN2_R2)) << 4);
        mouse_x = (Joy_RX - 0x80) / 4;
        mouse_y = (Joy_RY - 0x7F) / 4;
        mouse_wheel = (!(Joy_Btn1 & JOY_BTN1_DOWN)) ? -1 : ((!(Joy_Btn1 & JOY_BTN1_UP)) ? 1 : 0);
        wr_cmd(0x2A); // wr usb data5 EP1_IN
        wr_data(4);
        wr_data(mouse_btn);
        wr_data(mouse_x);
        wr_data(mouse_y);
        wr_data(mouse_wheel);
        // keyboard
        if (!(Joy_Btn2 & JOY_BTN2_SQUARE))
        {
            keyboard_down_flag = 1;
            wr_cmd(0x2B); // wr usb data7 EP2_IN
            wr_data(8);
            wr_data(0b00000011);
            wr_data(0x00);
            wr_data(0x29);
            wr_data(0x00);
            wr_data(0x00);
            wr_data(0x00);
            wr_data(0x00);
            wr_data(0x00);
        }
        else if (keyboard_down_flag)
        {
            keyboard_down_flag = 0;
            wr_cmd(0x2B);
            wr_data(8);
            wr_data(0);
            wr_data(0);
            wr_data(0);
            wr_data(0);
            wr_data(0);
            wr_data(0);
            wr_data(0);
            wr_data(0);
        }
    }
}