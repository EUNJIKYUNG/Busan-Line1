#ifndef DEFINE_STRUCT
#define DEFINE_STRUCT
#include <stdint.h>
#include "defineCommon.h"

// 220630 KEJ TCMS 프로토콜 수정
typedef struct _INPUT
{
	uint8_t bHCR1 : 1;
	uint8_t bHCR0 : 1;
    uint8_t bDCW  : 1;
    uint8_t bDOW  : 1;
	uint8_t bDDN_R : 1;
	uint8_t bDDN_L : 1;
	uint8_t bDDN_S : 1;
	uint8_t bPISMOD: 1;
}INPUT,*LPINPUT;
// typedef struct _INPUT
// {
// 	uint8_t bHCR1 : 1;
// 	uint8_t bHCR0 : 1;
// 	uint8_t bDDN_R : 1;
// 	uint8_t bDDN_L : 1;
// 	uint8_t bDDN_S : 1;
// 	uint8_t bSLEEP : 1;
// 	uint8_t : 1;
// 	uint8_t bSTEST : 1;;
// }INPUT,*LPINPUT;

typedef struct _OPERINFO
{
    uint8_t uTN0X00 : 4;    // 열차번호
	uint8_t uTNX000 : 4;
	uint8_t uTN000X : 4;
	uint8_t uTN00X0 : 4;

    uint8_t uSTARTCODE;     // 출발역
	uint8_t uCURCODE;       // 현재역
	uint8_t uNEXCODE;       // 다음역
    uint8_t uDSTCODE;       // 종착역
	
}opERINFO,*LPOPERINFO;

// 220701 KEJ TCMS 프로토콜 수정 
typedef struct _TCMS2PIS
{
	uint8_t uStx;           // STX
	uint8_t uAddr[2];       // ADDRESS
	uint8_t uDataType;      // DATA TYPE
	_TIMEINFO tInfo;        // DATE&TIME   
	_OPERINFO OPERINFO;     // 열차번호 및 역코드
    _INPUT INPUT;           // 0/다음역출입문/DOOR-L/DOOR-R/DOW/DCW/HCR8/HCR1
    uint8_t uAc;            // AC 투입
    uint8_t uDIST[2];       // 역간 이동거리
    uint8_t uSpeed;         // Speed
	uint8_t uAsp[16];        // AS 압력(1호차~8호차)
    int8_t  uTempInt[8];    // 객실온도(1호차~8호차)
    uint8_t uDust[16];      // 미세먼지농도(1호차~8호차)
    uint8_t uAircon[8];     // 냉난방 상태(1호차~8호차)
    uint8_t uCo2[16];       // 이산화탄소 농도(1호차~8호차)
    uint8_t uHumidity[8];   // 객실습도(1호차~8호차)
    int8_t  uTempExt;       // 실외온도
    uint8_t uCarVolt;       // 차량 전압
	uint8_t uSpare[8];      // spare
	uint8_t uETX;
	uint8_t uBCC[2];
}TCMS2PIS,*LPTCMSPIS;
// typedef struct _TCMS2PIS
// {
// 	uint8_t uSTX;
// 	uint8_t uAddr[2];
// 	uint8_t uDataType;
// 	_TIMEINFO tInfo;
// 	_INPUT INPUT;
// 	_OPERINFO OPERINFO;
// 	uint8_t uAsp[8];
// 	uint8_t uSIGLAN;
// 	uint8_t : 8;
// 	uint8_t : 8;
// 	uint8_t uETX;
// 	uint8_t uBCC[2];
// }TCMS2PIS,*LPTCMSPIS;

struct _PIS2TCMS
{
	uint8_t uDataType;
	_INPUT INPUT;
	_OPERINFO OPERINFO;
	struct tagPISState
	{
		uint8_t bOP_PISCF : 1;
		uint8_t bMASTER : 1;
		uint8_t : 1;
		uint8_t bPISCBKUP : 1;
		uint8_t : 4;
	}STATE;

	struct tagFAULTS
	{
		uint8_t bFTNDD1F : 1;
		uint8_t : 6;
		uint8_t bFTNDD0F : 1;

		uint8_t : 1;
		uint8_t bPII1_2F : 1;
		uint8_t bPII1_3F : 1;
		uint8_t bPII1_4F : 1;
		uint8_t bPII1_5F : 1;
		uint8_t bPII1_6F : 1;
		uint8_t bPII1_7F : 1;
		uint8_t : 1;

		uint8_t bPII2_1F : 1;
		uint8_t bPII2_2F : 1;
		uint8_t bPII2_3F : 1;
		uint8_t bPII2_4F : 1;
		uint8_t bPII2_5F : 1;
		uint8_t bPII2_6F : 1;
		uint8_t bPII2_7F : 1;
		uint8_t bPII2_8F : 1;

		uint8_t bIDD1R1_1F : 1;
		uint8_t bIDD1R1_2F : 1;
		uint8_t bIDD1R1_3F : 1;
		uint8_t bIDD1R1_4F : 1;
		uint8_t bIDD1R1_5F : 1;
		uint8_t bIDD1R1_6F : 1;
		uint8_t bIDD1R1_7F : 1;
		uint8_t bIDD1R1_8F : 1;

		uint8_t bIDD1R2_1F : 1;
		uint8_t bIDD1R2_2F : 1;
		uint8_t bIDD1R2_3F : 1;
		uint8_t bIDD1R2_4F : 1;
		uint8_t bIDD1R2_5F : 1;
		uint8_t bIDD1R2_6F : 1;
		uint8_t bIDD1R2_7F : 1;
		uint8_t bIDD1R2_8F : 1;

		uint8_t bIDD1L1_1F : 1;
		uint8_t bIDD1L1_2F : 1;
		uint8_t bIDD1L1_3F : 1;
		uint8_t bIDD1L1_4F : 1;
		uint8_t bIDD1L1_5F : 1;
		uint8_t bIDD1L1_6F : 1;
		uint8_t bIDD1L1_7F : 1;
		uint8_t bIDD1L1_8F : 1;
		uint8_t bIDD1L2_1F : 1;
		uint8_t bIDD1L2_2F : 1;
		uint8_t bIDD1L2_3F : 1;
		uint8_t bIDD1L2_4F : 1;
		uint8_t bIDD1L2_5F : 1;
		uint8_t bIDD1L2_6F : 1;
		uint8_t bIDD1L2_7F : 1;
		uint8_t bIDD1L2_8F : 1;

		uint8_t bIDD2R1_1F : 1;
		uint8_t bIDD2R1_2F : 1;
		uint8_t bIDD2R1_3F : 1;
		uint8_t bIDD2R1_4F : 1;
		uint8_t bIDD2R1_5F : 1;
		uint8_t bIDD2R1_6F : 1;
		uint8_t bIDD2R1_7F : 1;
		uint8_t bIDD2R1_8F : 1;
		uint8_t bIDD2R2_1F : 1;
		uint8_t bIDD2R2_2F : 1;
		uint8_t bIDD2R2_3F : 1;
		uint8_t bIDD2R2_4F : 1;
		uint8_t bIDD2R2_5F : 1;
		uint8_t bIDD2R2_6F : 1;
		uint8_t bIDD2R2_7F : 1;
		uint8_t bIDD2R2_8F : 1;

		uint8_t bIDD2L1_1F : 1;
		uint8_t bIDD2L1_2F : 1;
		uint8_t bIDD2L1_3F : 1;
		uint8_t bIDD2L1_4F : 1;
		uint8_t bIDD2L1_5F : 1;
		uint8_t bIDD2L1_6F : 1;
		uint8_t bIDD2L1_7F : 1;
		uint8_t bIDD2L1_8F : 1;
		uint8_t bIDD2L2_1F : 1;
		uint8_t bIDD2L2_2F : 1;
		uint8_t bIDD2L2_3F : 1;
		uint8_t bIDD2L2_4F : 1;
		uint8_t bIDD2L2_5F : 1;
		uint8_t bIDD2L2_6F : 1;
		uint8_t bIDD2L2_7F : 1;
		uint8_t bIDD2L2_8F : 1;
	}FAULTS;

	struct tagCLSLVL
	{
		uint8_t bCLSN1 : 1;
		uint8_t bCLSN2 : 1;
		uint8_t bCLSN3 : 1;
		uint8_t bCLSN4 : 1;
		uint8_t bCLSN5 : 1;
		uint8_t bCLSN6 : 1;
		uint8_t bCLSN7 : 1;
		uint8_t bCLSN0 : 1;
		uint8_t bCLSC1 : 1;
		uint8_t bCLSC2 : 1;
		uint8_t bCLSC3 : 1;
		uint8_t bCLSC4 : 1;
		uint8_t bCLSC5 : 1;
		uint8_t bCLSC6 : 1;
		uint8_t bCLSC7 : 1;
		uint8_t bCLSC0 : 1;
	}CLS;
	uint8_t : 8;
	struct tagROMVER
	{
		uint8_t uVerMajor;
		uint8_t uVerMinor;
	}ROMVER;
};

// 220701 KEJ IDD1 프로토콜 수정
struct _PISC2IDD1
{
    uint8_t uStx;           // STX
	uint8_t uAddr[2];       // ADDRESS
	uint8_t uDataType;      // DATA TYPE
	_TIMEINFO tInfo;        // DATE&TIME   
	_OPERINFO OPERINFO;     // 열차번호 및 역코드
    _INPUT INPUT;           // 자동수동모드/다음역출입문/DOOR-L/DOOR-R/DOW/DCW/HCR8/HCR1
    uint8_t uEventIndex;    // 화면 이벤트
    uint8_t uDIST[2];       // 역간 이동거리
    uint8_t uSpeed;         // Speed
	uint8_t uAsp[16];        // AS 압력(1호차~8호차)
    int8_t  uTempInt[8];    // 객실온도(1호차~8호차)
    uint8_t uDust[16];      // 미세먼지농도(1호차~8호차)
    uint8_t uAircon[8];     // 냉난방 상태(1호차~8호차)
    uint8_t uCo2[16];       // 이산화탄소 농도(1호차~8호차)
    uint8_t uHumidity[8];   // 객실습도(1호차~8호차)
    int8_t  uTempExt;       // 실외온도
    uint8_t uCarVolt;       // 차량 전압
	uint8_t uSpare[8];      // spare
	uint8_t uETX;
	uint8_t uBCC[2];
};
// struct _PISC2IDD1
// {
// 	uint16_t uTgtDev;
// 	uint16_t uCabDir;
// 	uint16_t uCurStnCode;
// 	uint16_t uNexStnCode;
// 	uint16_t uDesStnCode;
// 	uint16_t uDistance;
// 	uint16_t uTotalDistance;
// 	uint16_t uDoorDir;
// 	uint16_t uStopPtnIndex;
// 	uint16_t uCarCongestion[8];
// 	uint32_t uEventIndex;
// };


#endif
