#ifndef __MAIN_H
#define __MAIN_H
#include <mcs51/8051.h>
#include <stdint.h>

__sfr __at (0x91) P1M1;
__sfr __at (0x92) P1M0;
__sfr __at (0x93) P0M1;
__sfr __at (0x94) P0M0;
__sfr __at (0x95) P2M1;
__sfr __at (0x96) P2M0;
__sfr __at (0xB1) P3M1;
__sfr __at (0xB2) P3M0;
__sfr __at (0xB3) P4M1;
__sfr __at (0xB4) P4M0;

__sfr __at (0x8E) AUXR;

__sfr __at (0xD6) T2H;
__sfr __at (0xD7) T2L;

__sbit __at (0xC1) P4_1;
__sbit __at (0xC2) P4_2;
__sbit __at (0xC4) P4_4;

#define nop() __asm nop __endasm
#define _nop_() nop()

#define halt_ for (;;) {}
#define haltif(cmp, msg) if (cmp) { print(msg "\r\n"); halt_ }

#endif