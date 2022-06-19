#include "CDevIdentify.h"
#include "tickCount.h"
#include <string.h>


const char* CDevIdentify::chIPTable[CAR_INVALID][DEV_INVALID] = {
	"192.9.130.1"	,"192.9.130.101", ""			,"192.9.131.101", "192.9.132.1"	, "192.9.133.1"	, "192.9.134.1"	, "192.9.135.1"	, "192.9.136.1"	, "192.9.137.1"	, "192.9.138.1"	, "192.9.139.1",
	""				,""				, "192.9.131.2"	,"192.9.131.102", "192.9.132.2"	, "192.9.133.2"	, "192.9.134.2"	, "192.9.135.2"	, "192.9.136.2"	, "192.9.137.2"	, "192.9.138.2"	, "192.9.139.2",
	""				,""				, "192.9.131.3"	,"192.9.131.103", "192.9.132.3"	, "192.9.133.3"	, "192.9.134.3"	, "192.9.135.3"	, "192.9.136.3"	, "192.9.137.3"	, "192.9.138.3"	, "192.9.139.3",
	""				,""				, "192.9.131.4"	,"192.9.131.104", "192.9.132.4"	, "192.9.133.4"	, "192.9.134.4"	, "192.9.135.4"	, "192.9.136.4"	, "192.9.137.4"	, "192.9.138.4"	, "192.9.139.4",
	""				,""				, "192.9.131.5"	,"192.9.131.105", "192.9.132.5"	, "192.9.133.5"	, "192.9.134.5"	, "192.9.135.5"	, "192.9.136.5"	, "192.9.137.5"	, "192.9.138.5"	, "192.9.139.5",
	""				,""				, "192.9.131.6"	,"192.9.131.106", "192.9.132.6"	, "192.9.133.6"	, "192.9.134.6"	, "192.9.135.6"	, "192.9.136.6"	, "192.9.137.6"	, "192.9.138.6"	, "192.9.139.6",
	""				,""				, "192.9.131.7"	,"192.9.131.107", "192.9.132.7"	, "192.9.133.7"	, "192.9.134.7"	, "192.9.135.7"	, "192.9.136.7"	, "192.9.137.7"	, "192.9.138.7"	, "192.9.139.7",
	"192.9.130.10"	,"192.9.130.110", ""			,"192.9.131.110", "192.9.132.10", "192.9.133.10", "192.9.134.10", "192.9.135.10", "192.9.136.10", "192.9.137.10", "192.9.138.10", "192.9.139.10" };


const uint8_t CDevIdentify::uValueToEnumCar[16]=
{
	CAR_INVALID,CAR_1,CAR_2,CAR_3,CAR_4,CAR_5,CAR_6,CAR_7,CAR_INVALID,CAR_INVALID,CAR_0,CAR_INVALID,CAR_INVALID,CAR_INVALID,CAR_INVALID,CAR_INVALID
};


const uint8_t CDevIdentify::uValueToEnumDev[110]=
{
	D_PISC,D_PII_1,D_IDD1_R1,D_IDD1_L1,D_IDD1_R2,D_IDD1_L2,D_IDD2_R1,D_IDD2_L1,D_IDD2_R2,D_IDD2_L2,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,
	D_FTNDD,D_PII_2,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID,DEV_INVALID
};

in_addr CDevIdentify::uIPTable[CAR_INVALID][DEV_INVALID];
uint8_t CDevIdentify::uDevOrientationTable[MOVDIR_INVALID][CAR_INVALID][DEV_INVALID];
uint64_t CDevIdentify::lDevTick[CAR_INVALID][DEV_INVALID];
uint64_t CDevIdentify::lDevTimeOut[CAR_INVALID][DEV_INVALID] = 
{
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,
	5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000
};

int16_t CDevIdentify::uDevStatus[CAR_INVALID][DEV_INVALID]=
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
int16_t CDevIdentify::uPrevDevStatus[CAR_INVALID][DEV_INVALID]=
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

int16_t CDevIdentify::uCarStatus[CAR_INVALID] =
{
	-1,-1,-1,-1,-1,-1,-1,-1
};
int16_t CDevIdentify::uPrevCarStatus[CAR_INVALID]=
{
	-1, -1, -1, -1, -1, -1, -1, -1
};

std::function<void(const char*)> CDevIdentify::m_tDeviceStatusRecord[CAR_INVALID][DEV_INVALID];
std::function<void(const char*)> CDevIdentify::m_tCarStatusRecord[CAR_INVALID];


std::function<void(int)> CDevIdentify::m_tDevUpdate;
std::function<void()> CDevIdentify::m_tCarUpdate;
CDevIdentify *CDevIdentify::GetInstance()
{
	static CDevIdentify tManage;
	return &tManage;
}

CDevIdentify::CDevIdentify()
{
}


CDevIdentify::~CDevIdentify()
{
}


void CDevIdentify::InitDevTable()
{
	for (int i = 0; i < CAR_INVALID; i++)
	{
		for (int j = 0; j < DEV_INVALID; j++)
		{
			if (strlen(chIPTable[i][j]))
			{
				uIPTable[i][j].s_addr = inet_addr(chIPTable[i][j]);
			}
		}
	}

	uDevOrientationTable[MOVDIR_T1][CAR_1][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_1][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_2][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_2][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_3][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_3][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_4][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_4][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_6][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_6][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_7][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_7][D_IDD2_L2] = DIR_LEFT;

	uDevOrientationTable[MOVDIR_T1][CAR_0][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T1][CAR_0][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_1][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_1][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_2][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_2][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_3][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_3][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_4][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_4][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_6][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_6][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_7][D_PII_1] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_PII_2] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD1_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD1_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD1_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD1_L2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD2_R1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD2_L1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD2_R2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_7][D_IDD2_L2] = DIR_RIGHT;

	uDevOrientationTable[MOVDIR_T2][CAR_0][D_PII_1] = DIR_FORWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_PII_2] = DIR_BACKWARD;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD1_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD1_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD1_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD1_L2] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD2_R1] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD2_L1] = DIR_LEFT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD2_R2] = DIR_RIGHT;
	uDevOrientationTable[MOVDIR_T2][CAR_0][D_IDD2_L2] = DIR_LEFT;
}

uint32_t CDevIdentify::GetOrientationTable(int nMovDirection,int nCar, int nDev)
{
	return uDevOrientationTable[nMovDirection][nCar][nDev];
}

bool CDevIdentify::SetCurrentTick(in_addr *pAddr)
{

	int nDevType = GetDevType((pAddr->s_addr>>16)&0xFF,(pAddr->s_addr>>24)&0xFF);
	if (nDevType == DEV_INVALID)
	{
		return false;
	}
	int nCar = GetCarOrder((pAddr->s_addr>>24)&0xFF);
	if (nCar == CAR_INVALID)
	{
		return false;
	}
	lDevTick[nCar][nDevType] = getTickCount();
	//printf("%s of %s tick count is set\n", GetStringDEV_LIST((DEV_LIST)nDevType), GetStringCAR_NUMBER((CAR_NUMBER)nCar));
	return true;
}



bool CDevIdentify::SetDeviceTimeOut(int nCar,int nType,uint64_t uTimeOut)
{
	lDevTimeOut[nCar][nType] = uTimeOut;
	return false;
}

bool CDevIdentify::IsDeviceWorking(int nCar, int nType)
{
	int64_t lDelta = getTickCount() - lDevTick[nCar][nType];

	if (lDelta < lDevTimeOut[nCar][nType])
	{
		if ((uPrevDevStatus[nCar][nType] == -1) || (!uPrevDevStatus[nCar][nType]))
		{
			uPrevDevStatus[nCar][nType] = 1;
			m_tDeviceStatusRecord[nCar][nType]("background-color:rgb(0, 211, 28);");
			m_tDevUpdate(nCar);
		}
		return true;//Device Is Working
	}
	if ((uPrevDevStatus[nCar][nType] == -1) || (uPrevDevStatus[nCar][nType]==1))
	{
		uPrevDevStatus[nCar][nType] = 0;
		m_tDeviceStatusRecord[nCar][nType]("background-color:red;");
		m_tDevUpdate(nCar);
	}
	return false;
}

bool CDevIdentify::SetCarStatus(int nCar, bool bBool)
{
	if (bBool)
	{
		if ((uPrevCarStatus[nCar] == -1)||(!uPrevCarStatus[nCar]))
		{
			uPrevCarStatus[nCar] = 1;
			m_tCarStatusRecord[nCar]("background-color:rgb(0, 211, 28);");
			m_tCarUpdate();
		}
		return true;
	}
	
	if ((uPrevCarStatus[nCar] == -1) || (uPrevCarStatus[nCar]==1))
	{
		uPrevCarStatus[nCar] = 0;
		m_tCarStatusRecord[nCar]("background-color:red;");
		m_tCarUpdate();
	}
	return false;
}

int CDevIdentify::GetDevType(uint8_t third,uint8_t fourth)
{
	int nOrder = (fourth / 100) * 100 + (third - 130);
	if (nOrder < sizeof(uValueToEnumDev))
		return uValueToEnumDev[nOrder];
	return DEV_INVALID;
}

int CDevIdentify::GetCarOrder(uint8_t fourth)
{
	int nOrder = fourth % 100;
	if (nOrder < sizeof(uValueToEnumCar))
		return uValueToEnumCar[nOrder];
	return CAR_INVALID;
}


void CDevIdentify::IsAllDeviceWorking()
{
	IsDeviceWorking(CAR_1, D_PISC);
	IsDeviceWorking(CAR_1, D_FTNDD);
	IsDeviceWorking(CAR_1, D_PII_2);
	IsDeviceWorking(CAR_1, D_IDD1_R1);
	IsDeviceWorking(CAR_1, D_IDD1_L1);
	IsDeviceWorking(CAR_1, D_IDD1_R2);
	IsDeviceWorking(CAR_1, D_IDD1_L2);
	IsDeviceWorking(CAR_1, D_IDD2_R1);
	IsDeviceWorking(CAR_1, D_IDD2_L1);
	IsDeviceWorking(CAR_1, D_IDD2_R2);
	IsDeviceWorking(CAR_1, D_IDD2_L2);

	SetCarStatus(CAR_1,
		uPrevDevStatus[CAR_1][D_PISC] &&
		uPrevDevStatus[CAR_1][D_FTNDD] &&
		uPrevDevStatus[CAR_1][D_PII_2] &&
		uPrevDevStatus[CAR_1][D_IDD1_R1] &&
		uPrevDevStatus[CAR_1][D_IDD1_L1] &&
		uPrevDevStatus[CAR_1][D_IDD1_R2] &&
		uPrevDevStatus[CAR_1][D_IDD1_L2] &&
		uPrevDevStatus[CAR_1][D_IDD2_R1] &&
		uPrevDevStatus[CAR_1][D_IDD2_L1] &&
		uPrevDevStatus[CAR_1][D_IDD2_R2] &&
		uPrevDevStatus[CAR_1][D_IDD2_L2]
	);
	

	IsDeviceWorking(CAR_2, D_PII_1);
	IsDeviceWorking(CAR_2, D_PII_2);
	IsDeviceWorking(CAR_2, D_IDD1_R1);
	IsDeviceWorking(CAR_2, D_IDD1_L1);
	IsDeviceWorking(CAR_2, D_IDD1_R2);
	IsDeviceWorking(CAR_2, D_IDD1_L2);
	IsDeviceWorking(CAR_2, D_IDD2_R1);
	IsDeviceWorking(CAR_2, D_IDD2_L1);
	IsDeviceWorking(CAR_2, D_IDD2_R2);
	IsDeviceWorking(CAR_2, D_IDD2_L2);


	SetCarStatus(CAR_2,
		uPrevDevStatus[CAR_2][D_PII_1] &&
		uPrevDevStatus[CAR_2][D_PII_2] &&
		uPrevDevStatus[CAR_2][D_IDD1_R1] &&
		uPrevDevStatus[CAR_2][D_IDD1_L1] &&
		uPrevDevStatus[CAR_2][D_IDD1_R2] &&
		uPrevDevStatus[CAR_2][D_IDD1_L2] &&
		uPrevDevStatus[CAR_2][D_IDD2_R1] &&
		uPrevDevStatus[CAR_2][D_IDD2_L1] &&
		uPrevDevStatus[CAR_2][D_IDD2_R2] &&
		uPrevDevStatus[CAR_2][D_IDD2_L2]
	);


	IsDeviceWorking(CAR_3, D_PII_1);
	IsDeviceWorking(CAR_3, D_PII_2);
	IsDeviceWorking(CAR_3, D_IDD1_R1);
	IsDeviceWorking(CAR_3, D_IDD1_L1);
	IsDeviceWorking(CAR_3, D_IDD1_R2);
	IsDeviceWorking(CAR_3, D_IDD1_L2);
	IsDeviceWorking(CAR_3, D_IDD2_R1);
	IsDeviceWorking(CAR_3, D_IDD2_L1);
	IsDeviceWorking(CAR_3, D_IDD2_R2);
	IsDeviceWorking(CAR_3, D_IDD2_L2);

	SetCarStatus(CAR_3,
		uPrevDevStatus[CAR_3][D_PII_1] &&
		uPrevDevStatus[CAR_3][D_PII_2] &&
		uPrevDevStatus[CAR_3][D_IDD1_R1] &&
		uPrevDevStatus[CAR_3][D_IDD1_L1] &&
		uPrevDevStatus[CAR_3][D_IDD1_R2] &&
		uPrevDevStatus[CAR_3][D_IDD1_L2] &&
		uPrevDevStatus[CAR_3][D_IDD2_R1] &&
		uPrevDevStatus[CAR_3][D_IDD2_L1] &&
		uPrevDevStatus[CAR_3][D_IDD2_R2] &&
		uPrevDevStatus[CAR_3][D_IDD2_L2]
	);

	IsDeviceWorking(CAR_4, D_PII_1);
	IsDeviceWorking(CAR_4, D_PII_2);
	IsDeviceWorking(CAR_4, D_IDD1_R1);
	IsDeviceWorking(CAR_4, D_IDD1_L1);
	IsDeviceWorking(CAR_4, D_IDD1_R2);
	IsDeviceWorking(CAR_4, D_IDD1_L2);
	IsDeviceWorking(CAR_4, D_IDD2_R1);
	IsDeviceWorking(CAR_4, D_IDD2_L1);
	IsDeviceWorking(CAR_4, D_IDD2_R2);
	IsDeviceWorking(CAR_4, D_IDD2_L2);

	SetCarStatus(CAR_4,
		uPrevDevStatus[CAR_4][D_PII_1] &&
		uPrevDevStatus[CAR_4][D_PII_2] &&
		uPrevDevStatus[CAR_4][D_IDD1_R1] &&
		uPrevDevStatus[CAR_4][D_IDD1_L1] &&
		uPrevDevStatus[CAR_4][D_IDD1_R2] &&
		uPrevDevStatus[CAR_4][D_IDD1_L2] &&
		uPrevDevStatus[CAR_4][D_IDD2_R1] &&
		uPrevDevStatus[CAR_4][D_IDD2_L1] &&
		uPrevDevStatus[CAR_4][D_IDD2_R2] &&
		uPrevDevStatus[CAR_4][D_IDD2_L2]
	);

	IsDeviceWorking(CAR_5, D_PII_1);
	IsDeviceWorking(CAR_5, D_PII_2);
	IsDeviceWorking(CAR_5, D_IDD1_R1);
	IsDeviceWorking(CAR_5, D_IDD1_L1);
	IsDeviceWorking(CAR_5, D_IDD1_R2);
	IsDeviceWorking(CAR_5, D_IDD1_L2);
	IsDeviceWorking(CAR_5, D_IDD2_R1);
	IsDeviceWorking(CAR_5, D_IDD2_L1);
	IsDeviceWorking(CAR_5, D_IDD2_R2);
	IsDeviceWorking(CAR_5, D_IDD2_L2);

	SetCarStatus(CAR_5,
		uPrevDevStatus[CAR_5][D_PII_1] &&
		uPrevDevStatus[CAR_5][D_PII_2] &&
		uPrevDevStatus[CAR_5][D_IDD1_R1] &&
		uPrevDevStatus[CAR_5][D_IDD1_L1] &&
		uPrevDevStatus[CAR_5][D_IDD1_R2] &&
		uPrevDevStatus[CAR_5][D_IDD1_L2] &&
		uPrevDevStatus[CAR_5][D_IDD2_R1] &&
		uPrevDevStatus[CAR_5][D_IDD2_L1] &&
		uPrevDevStatus[CAR_5][D_IDD2_R2] &&
		uPrevDevStatus[CAR_5][D_IDD2_L2]
	);

	IsDeviceWorking(CAR_6, D_PII_1);
	IsDeviceWorking(CAR_6, D_PII_2);
	IsDeviceWorking(CAR_6, D_IDD1_R1);
	IsDeviceWorking(CAR_6, D_IDD1_L1);
	IsDeviceWorking(CAR_6, D_IDD1_R2);
	IsDeviceWorking(CAR_6, D_IDD1_L2);
	IsDeviceWorking(CAR_6, D_IDD2_R1);
	IsDeviceWorking(CAR_6, D_IDD2_L1);
	IsDeviceWorking(CAR_6, D_IDD2_R2);
	IsDeviceWorking(CAR_6, D_IDD2_L2);

	SetCarStatus(CAR_6,
		uPrevDevStatus[CAR_6][D_PII_1] &&
		uPrevDevStatus[CAR_6][D_PII_2] &&
		uPrevDevStatus[CAR_6][D_IDD1_R1] &&
		uPrevDevStatus[CAR_6][D_IDD1_L1] &&
		uPrevDevStatus[CAR_6][D_IDD1_R2] &&
		uPrevDevStatus[CAR_6][D_IDD1_L2] &&
		uPrevDevStatus[CAR_6][D_IDD2_R1] &&
		uPrevDevStatus[CAR_6][D_IDD2_L1] &&
		uPrevDevStatus[CAR_6][D_IDD2_R2] &&
		uPrevDevStatus[CAR_6][D_IDD2_L2]
	);

	IsDeviceWorking(CAR_7, D_PII_1);
	IsDeviceWorking(CAR_7, D_PII_2);
	IsDeviceWorking(CAR_7, D_IDD1_R1);
	IsDeviceWorking(CAR_7, D_IDD1_L1);
	IsDeviceWorking(CAR_7, D_IDD1_R2);
	IsDeviceWorking(CAR_7, D_IDD1_L2);
	IsDeviceWorking(CAR_7, D_IDD2_R1);
	IsDeviceWorking(CAR_7, D_IDD2_L1);
	IsDeviceWorking(CAR_7, D_IDD2_R2);
	IsDeviceWorking(CAR_7, D_IDD2_L2);

	SetCarStatus(CAR_7,
		uPrevDevStatus[CAR_7][D_PII_1] &&
		uPrevDevStatus[CAR_7][D_PII_2] &&
		uPrevDevStatus[CAR_7][D_IDD1_R1] &&
		uPrevDevStatus[CAR_7][D_IDD1_L1] &&
		uPrevDevStatus[CAR_7][D_IDD1_R2] &&
		uPrevDevStatus[CAR_7][D_IDD1_L2] &&
		uPrevDevStatus[CAR_7][D_IDD2_R1] &&
		uPrevDevStatus[CAR_7][D_IDD2_L1] &&
		uPrevDevStatus[CAR_7][D_IDD2_R2] &&
		uPrevDevStatus[CAR_7][D_IDD2_L2]
	);

	IsDeviceWorking(CAR_0, D_PISC);
	IsDeviceWorking(CAR_0, D_FTNDD);
	IsDeviceWorking(CAR_0, D_PII_2);
	IsDeviceWorking(CAR_0, D_IDD1_R1);
	IsDeviceWorking(CAR_0, D_IDD1_L1);
	IsDeviceWorking(CAR_0, D_IDD1_R2);
	IsDeviceWorking(CAR_0, D_IDD1_L2);
	IsDeviceWorking(CAR_0, D_IDD2_R1);
	IsDeviceWorking(CAR_0, D_IDD2_L1);
	IsDeviceWorking(CAR_0, D_IDD2_R2);
	IsDeviceWorking(CAR_0, D_IDD2_L2);

	SetCarStatus(CAR_0,
		uPrevDevStatus[CAR_0][D_PISC] &&
		uPrevDevStatus[CAR_0][D_FTNDD] &&
		uPrevDevStatus[CAR_0][D_PII_2] &&
		uPrevDevStatus[CAR_0][D_IDD1_R1] &&
		uPrevDevStatus[CAR_0][D_IDD1_L1] &&
		uPrevDevStatus[CAR_0][D_IDD1_R2] &&
		uPrevDevStatus[CAR_0][D_IDD1_L2] &&
		uPrevDevStatus[CAR_0][D_IDD2_R1] &&
		uPrevDevStatus[CAR_0][D_IDD2_L1] &&
		uPrevDevStatus[CAR_0][D_IDD2_R2] &&
		uPrevDevStatus[CAR_0][D_IDD2_L2]
	);
}





