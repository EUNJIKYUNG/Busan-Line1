#ifndef DEFINE_LED
#define DEFINE_LED
#include <stdint.h>
#include "../defineCommon.h"

enum
{
	LED_FDI = 1,
	LED_PII1 = 3,
	LED_PII2 = 4,
};
/*
typedef struct _FDISDISENDHEAD
{
	uint8_t uTxCounter;
	uint8_t uDevCode;
	uint8_t uDevID;

	uint8_t bReset : 1;
	uint8_t : 3;
	uint8_t bScrOff : 1;
	uint8_t : 2;
	uint8_t bPtnTest : 1;

	uint8_t bRetryUpdate : 1;
	uint8_t : 3;
	uint8_t bTextSend : 1;
	uint8_t : 2;
	uint8_t bUpdate : 1;

	uint8_t uCRCValue[36];
	uint8_t bSyncByCNCS : 1;
	uint8_t : 7;
	uint8_t uPageCnt;
	uint8_t eImageRetainigTime;

	uint8_t uStartAddr[2];
	uint8_t uEndAddr[2];
}FDISDISENDHEAD;


typedef struct _LEDRECV
{
	uint8_t uTxCounter;

	uint8_t bSelfTest : 1;
	uint8_t : 2;
	uint8_t bOK : 1;
	uint8_t bNG : 1;
	uint8_t : 2;

	uint8_t bStart : 1;
	uint8_t bOngoing : 1;
	uint8_t bCompleted : 1;
	uint8_t : 1;
	uint8_t bAllBlockOk : 1;
	uint8_t bAllBlockNG : 1;
	uint8_t : 2;
	uint8_t uStartAddr[2];
	uint8_t uEndAddr[2];

	uint8_t bUpdate1 : 1;
	uint8_t bUpdate2 : 1;
	uint8_t bUpdate3 : 1;
	uint8_t bUpdate4 : 1;
	uint8_t bUpdate5 : 1;
	uint8_t bUpdate6 : 1;
	uint8_t bUpdate7 : 1;
	uint8_t bUpdate8 : 1;
	uint8_t bUpdate9 : 1;
	uint8_t bUpdate10 : 1;
	uint8_t bUpdate11 : 1;
	uint8_t bUpdate12 : 1;
	uint8_t bUpdate13 : 1;
	uint8_t bUpdate14 : 1;
	uint8_t bUpdate15 : 1;
	uint8_t bUpdate16 : 1;
	uint8_t bUpdate17 : 1;
	uint8_t bUpdate18 : 1;
	uint8_t : 6;

	uint8_t bReset : 1;
	uint8_t : 3;
	uint8_t bScrOff : 1;
	uint8_t : 2;
	uint8_t bPtnTest : 1;
	VERSIONINFO tVerInfo;
}LEDRECV;*/





typedef struct _FDISDISENDHEAD
{
	uint8_t uTxCounter;
	uint8_t uDevCode;
	uint8_t uDevID;

	uint8_t bReset : 1;
	uint8_t : 3;
	uint8_t bScrOff : 1;
	uint8_t : 2;
	uint8_t bPtnTest : 1;

	uint8_t bRetryUpdate : 1;
	uint8_t : 3;
	uint8_t bTextSend : 1;
	uint8_t : 2;
	uint8_t bUpdate : 1;

	uint8_t uCRCValue[72];
	uint8_t bSyncByCNCS : 1;
	uint8_t : 7;
	uint8_t uPageCnt;
	uint8_t eImageRetainigTime;

	uint8_t uStartAddr[2];
	uint8_t uEndAddr[2];
}FDISDISENDHEAD;


typedef struct _LEDRECV
{
	uint8_t uTxCounter;

	uint8_t bSelfTest : 1;
	uint8_t : 2;
	uint8_t bOK : 1;
	uint8_t bNG : 1;
	uint8_t : 2;

	uint8_t bStart : 1;
	uint8_t bOngoing : 1;
	uint8_t bCompleted : 1;
	uint8_t : 1;
	uint8_t bAllBlockOk : 1;
	uint8_t bAllBlockNG : 1;
	uint8_t : 2;
	uint8_t uStartAddr[2];
	uint8_t uEndAddr[2];

	uint8_t bUpdate1 : 1;
	uint8_t bUpdate2 : 1;
	uint8_t bUpdate3 : 1;
	uint8_t bUpdate4 : 1;
	uint8_t bUpdate5 : 1;
	uint8_t bUpdate6 : 1;
	uint8_t bUpdate7 : 1;
	uint8_t bUpdate8 : 1;

	uint8_t bUpdate9 : 1;
	uint8_t bUpdate10 : 1;
	uint8_t bUpdate11 : 1;
	uint8_t bUpdate12 : 1;
	uint8_t bUpdate13 : 1;
	uint8_t bUpdate14 : 1;
	uint8_t bUpdate15 : 1;
	uint8_t bUpdate16 : 1;
	
	uint8_t bUpdate17 : 1;
	uint8_t bUpdate18 : 1;
	uint8_t bUpdate19 : 1;
	uint8_t bUpdate20 : 1;
	uint8_t bUpdate21 : 1;
	uint8_t bUpdate22 : 1;
	uint8_t bUpdate23 : 1;
	uint8_t bUpdate24 : 1;
	
	uint8_t bUpdate25 : 1;
	uint8_t bUpdate26 : 1;
	uint8_t bUpdate27 : 1;
	uint8_t bUpdate28 : 1;
	uint8_t bUpdate29 : 1;
	uint8_t bUpdate30 : 1;
	uint8_t bUpdate31 : 1;
	uint8_t bUpdate32 : 1;
	
	uint8_t bUpdate33 : 1;
	uint8_t bUpdate34 : 1;
	uint8_t bUpdate35 : 1;
	uint8_t bUpdate36 : 1;
	uint8_t : 4;

	uint8_t bReset : 1;
	uint8_t : 3;
	uint8_t bScrOff : 1;
	uint8_t : 2;
	uint8_t bPtnTest : 1;
	VERSIONINFO tVerInfo;
}LEDRECV;


typedef struct _FDISDISEND
{
	FDISDISENDHEAD tHead;
	uint8_t uBinBuf[1312];
}FDISDISEND;



#define ETND_WIDTH	16
#define ETND_HEIGHT	32
#define FDI_WIDTH	192
#define FDI_HEIGHT	32
#define SDI_WIDTH	160
#define SDI_HEIGHT	16
#define PII_WIDTH	160
#define PII_HEIGHT	16

#define FDI_DEV_CNT	2
#define PII1_DEV_CNT 6
#define PII2_DEV_CNT 8

#define DEV_INC(A,B) A++;if(A>=B){A=0;}

#define DEV_INC_PII1(A) DEV_INC(A,PII1_DEV_CNT)
#define DEV_INC_PII2(A) DEV_INC(A,PII2_DEV_CNT)

#define DEV_INC_FDI(A) DEV_INC(A,FDI_DEV_CNT)
#define DEV_INC_SDI(A) DEV_INC(A,SDI_DEV_CNT)

#define FDI_PAGE_BYTE_COUNT (FDI_WIDTH*FDI_HEIGHT)
#define SDI_PAGE_BYTE_COUNT (SDI_WIDTH*SDI_HEIGHT)
#define PII_PAGE_BYTE_COUNT (PII_WIDTH*PII_HEIGHT)

#define FDI_BLOCK_BYTE_COUNT FDI_PAGE_BYTE_COUNT/6
#define SDI_BLOCK_BYTE_COUNT SDI_PAGE_BYTE_COUNT/4
#define PII_BLOCK_BYTE_COUNT PII_PAGE_BYTE_COUNT/4



#endif
