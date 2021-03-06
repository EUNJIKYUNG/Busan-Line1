#include "ColorPalette.h"

unsigned char g1BppColorTable[2][3] =
{
	{0x00,0x00,0x00},
	{0xFF,0xC3,0x00},
};

unsigned char g2BppColorTable[16][3] =
{
	{0x00,0x00,0x00},
	{0xFF,0x00,0x00},
	{0xC3,0xFF,0x00},
	{0xFF,0xC3,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
	{0x00,0x00,0x00},
};


unsigned char g4BppColorTable[16][3] =
{
	{0x00,0x00,0x00},
	{0x80,0x00,0x00},
	{0x00,0x80,0x00},
	{0x80,0x80,0x00},
	{0x00,0x00,0x80},
	{0x80,0x00,0x80},
	{0x00,0x80,0x80},
	{0xc0,0xc0,0xc0},
	{0x80,0x80,0x80},
	{0xff,0x00,0x00},
	{0x00,0xff,0x00},
	{0xff,0xff,0x00},
	{0x00,0x00,0xff},
	{0xff,0x00,0xff},
	{0x00,0xff,0xff},
	{0xff,0xff,0xff},
};


unsigned char g8BppColorTable[256][3] = {
	{0x00,0x00,0x00},
	{0x80,0x00,0x00},
	{0x00,0x80,0x00},
	{0x80,0x80,0x00},
	{0x00,0x00,0x80},
	{0x80,0x00,0x80},
	{0x00,0x80,0x80},
	{0xc0,0xc0,0xc0},
	{0x80,0x80,0x80},
	{0xff,0x00,0x00},
	{0x00,0xff,0x00},
	{0xff,0xff,0x00},
	{0x00,0x00,0xff},
	{0xff,0x00,0xff},
	{0x00,0xff,0xff},
	{0xff,0xff,0xff},
	{0x00,0x00,0x00},
	{0x00,0x00,0x5f},
	{0x00,0x00,0x87},
	{0x00,0x00,0xaf},
	{0x00,0x00,0xd7},
	{0x00,0x00,0xff},
	{0x00,0x5f,0x00},
	{0x00,0x5f,0x5f},
	{0x00,0x5f,0x87},
	{0x00,0x5f,0xaf},
	{0x00,0x5f,0xd7},
	{0x00,0x5f,0xff},
	{0x00,0x87,0x00},
	{0x00,0x87,0x5f},
	{0x00,0x87,0x87},
	{0x00,0x87,0xaf},
	{0x00,0x87,0xd7},
	{0x00,0x87,0xff},
	{0x00,0xaf,0x00},
	{0x00,0xaf,0x5f},
	{0x00,0xaf,0x87},
	{0x00,0xaf,0xaf},
	{0x00,0xaf,0xd7},
	{0x00,0xaf,0xff},
	{0x00,0xd7,0x00},
	{0x00,0xd7,0x5f},
	{0x00,0xd7,0x87},
	{0x00,0xd7,0xaf},
	{0x00,0xd7,0xd7},
	{0x00,0xd7,0xff},
	{0x00,0xff,0x00},
	{0x00,0xff,0x5f},
	{0x00,0xff,0x87},
	{0x00,0xff,0xaf},
	{0x00,0xff,0xd7},
	{0x00,0xff,0xff},
	{0x5f,0x00,0x00},
	{0x5f,0x00,0x5f},
	{0x5f,0x00,0x87},
	{0x5f,0x00,0xaf},
	{0x5f,0x00,0xd7},
	{0x5f,0x00,0xff},
	{0x5f,0x5f,0x00},
	{0x5f,0x5f,0x5f},
	{0x5f,0x5f,0x87},
	{0x5f,0x5f,0xaf},
	{0x5f,0x5f,0xd7},
	{0x5f,0x5f,0xff},
	{0x5f,0x87,0x00},
	{0x5f,0x87,0x5f},
	{0x5f,0x87,0x87},
	{0x5f,0x87,0xaf},
	{0x5f,0x87,0xd7},
	{0x5f,0x87,0xff},
	{0x5f,0xaf,0x00},
	{0x5f,0xaf,0x5f},
	{0x5f,0xaf,0x87},
	{0x5f,0xaf,0xaf},
	{0x5f,0xaf,0xd7},
	{0x5f,0xaf,0xff},
	{0x5f,0xd7,0x00},
	{0x5f,0xd7,0x5f},
	{0x5f,0xd7,0x87},
	{0x5f,0xd7,0xaf},
	{0x5f,0xd7,0xd7},
	{0x5f,0xd7,0xff},
	{0x5f,0xff,0x00},
	{0x5f,0xff,0x5f},
	{0x5f,0xff,0x87},
	{0x5f,0xff,0xaf},
	{0x5f,0xff,0xd7},
	{0x5f,0xff,0xff},
	{0x87,0x00,0x00},
	{0x87,0x00,0x5f},
	{0x87,0x00,0x87},
	{0x87,0x00,0xaf},
	{0x87,0x00,0xd7},
	{0x87,0x00,0xff},
	{0x87,0x5f,0x00},
	{0x87,0x5f,0x5f},
	{0x87,0x5f,0x87},
	{0x87,0x5f,0xaf},
	{0x87,0x5f,0xd7},
	{0x87,0x5f,0xff},
	{0x87,0x87,0x00},
	{0x87,0x87,0x5f},
	{0x87,0x87,0x87},
	{0x87,0x87,0xaf},
	{0x87,0x87,0xd7},
	{0x87,0x87,0xff},
	{0x87,0xaf,0x00},
	{0x87,0xaf,0x5f},
	{0x87,0xaf,0x87},
	{0x87,0xaf,0xaf},
	{0x87,0xaf,0xd7},
	{0x87,0xaf,0xff},
	{0x87,0xd7,0x00},
	{0x87,0xd7,0x5f},
	{0x87,0xd7,0x87},
	{0x87,0xd7,0xaf},
	{0x87,0xd7,0xd7},
	{0x87,0xd7,0xff},
	{0x87,0xff,0x00},
	{0x87,0xff,0x5f},
	{0x87,0xff,0x87},
	{0x87,0xff,0xaf},
	{0x87,0xff,0xd7},
	{0x87,0xff,0xff},
	{0xaf,0x00,0x00},
	{0xaf,0x00,0x5f},
	{0xaf,0x00,0x87},
	{0xaf,0x00,0xaf},
	{0xaf,0x00,0xd7},
	{0xaf,0x00,0xff},
	{0xaf,0x5f,0x00},
	{0xaf,0x5f,0x5f},
	{0xaf,0x5f,0x87},
	{0xaf,0x5f,0xaf},
	{0xaf,0x5f,0xd7},
	{0xaf,0x5f,0xff},
	{0xaf,0x87,0x00},
	{0xaf,0x87,0x5f},
	{0xaf,0x87,0x87},
	{0xaf,0x87,0xaf},
	{0xaf,0x87,0xd7},
	{0xaf,0x87,0xff},
	{0xaf,0xaf,0x00},
	{0xaf,0xaf,0x5f},
	{0xaf,0xaf,0x87},
	{0xaf,0xaf,0xaf},
	{0xaf,0xaf,0xd7},
	{0xaf,0xaf,0xff},
	{0xaf,0xd7,0x00},
	{0xaf,0xd7,0x5f},
	{0xaf,0xd7,0x87},
	{0xaf,0xd7,0xaf},
	{0xaf,0xd7,0xd7},
	{0xaf,0xd7,0xff},
	{0xaf,0xff,0x00},
	{0xaf,0xff,0x5f},
	{0xaf,0xff,0x87},
	{0xaf,0xff,0xaf},
	{0xaf,0xff,0xd7},
	{0xaf,0xff,0xff},
	{0xd7,0x00,0x00},
	{0xd7,0x00,0x5f},
	{0xd7,0x00,0x87},
	{0xd7,0x00,0xaf},
	{0xd7,0x00,0xd7},
	{0xd7,0x00,0xff},
	{0xd7,0x5f,0x00},
	{0xd7,0x5f,0x5f},
	{0xd7,0x5f,0x87},
	{0xd7,0x5f,0xaf},
	{0xd7,0x5f,0xd7},
	{0xd7,0x5f,0xff},
	{0xd7,0x87,0x00},
	{0xd7,0x87,0x5f},
	{0xd7,0x87,0x87},
	{0xd7,0x87,0xaf},
	{0xd7,0x87,0xd7},
	{0xd7,0x87,0xff},
	{0xd7,0xaf,0x00},
	{0xd7,0xaf,0x5f},
	{0xd7,0xaf,0x87},
	{0xd7,0xaf,0xaf},
	{0xd7,0xaf,0xd7},
	{0xd7,0xaf,0xff},
	{0xd7,0xd7,0x00},
	{0xd7,0xd7,0x5f},
	{0xd7,0xd7,0x87},
	{0xd7,0xd7,0xaf},
	{0xd7,0xd7,0xd7},
	{0xd7,0xd7,0xff},
	{0xd7,0xff,0x00},
	{0xd7,0xff,0x5f},
	{0xd7,0xff,0x87},
	{0xd7,0xff,0xaf},
	{0xd7,0xff,0xd7},
	{0xd7,0xff,0xff},
	{0xff,0x00,0x00},
	{0xff,0x00,0x5f},
	{0xff,0x00,0x87},
	{0xff,0x00,0xaf},
	{0xff,0x00,0xd7},
	{0xff,0x00,0xff},
	{0xff,0x5f,0x00},
	{0xff,0x5f,0x5f},
	{0xff,0x5f,0x87},
	{0xff,0x5f,0xaf},
	{0xff,0x5f,0xd7},
	{0xff,0x5f,0xff},
	{0xff,0x87,0x00},
	{0xff,0x87,0x5f},
	{0xff,0x87,0x87},
	{0xff,0x87,0xaf},
	{0xff,0x87,0xd7},
	{0xff,0x87,0xff},
	{0xff,0xaf,0x00},
	{0xff,0xaf,0x5f},
	{0xff,0xaf,0x87},
	{0xff,0xaf,0xaf},
	{0xff,0xaf,0xd7},
	{0xff,0xaf,0xff},
	{0xff,0xd7,0x00},
	{0xff,0xd7,0x5f},
	{0xff,0xd7,0x87},
	{0xff,0xd7,0xaf},
	{0xff,0xd7,0xd7},
	{0xff,0xd7,0xff},
	{0xff,0xff,0x00},
	{0xff,0xff,0x5f},
	{0xff,0xff,0x87},
	{0xff,0xff,0xaf},
	{0xff,0xff,0xd7},
	{0xff,0xff,0xff},
	{0x08,0x08,0x08},
	{0x12,0x12,0x12},
	{0x1c,0x1c,0x1c},
	{0x26,0x26,0x26},
	{0x30,0x30,0x30},
	{0x3a,0x3a,0x3a},
	{0x44,0x44,0x44},
	{0x4e,0x4e,0x4e},
	{0x58,0x58,0x58},
	{0x62,0x62,0x62},
	{0x6c,0x6c,0x6c},
	{0x76,0x76,0x76},
	{0x80,0x80,0x80},
	{0x8a,0x8a,0x8a},
	{0x94,0x94,0x94},
	{0x9e,0x9e,0x9e},
	{0xa8,0xa8,0xa8},
	{0xb2,0xb2,0xb2},
	{0xbc,0xbc,0xbc},
	{0xc6,0xc6,0xc6},
	{0xd0,0xd0,0xd0},
	{0xda,0xda,0xda},
	{0xe4,0xe4,0xe4},
	{0xee,0xee,0xee},
};

