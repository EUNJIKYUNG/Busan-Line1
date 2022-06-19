#ifndef DEV_IDENTIFY
#define DEV_IDENTIFY
#include "define336.h"
#include <functional>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

class CDevIdentify
{
public:
	CDevIdentify();
	~CDevIdentify();
	static CDevIdentify *GetInstance();
	void InitDevTable();
	bool SetCurrentTick(in_addr *pAddr);
	int GetDevType(uint8_t third, uint8_t fourth);
	int GetCarOrder(uint8_t fourth);
	bool SetDeviceTimeOut(int nCar, int nType, uint64_t uTimeOut);
	bool IsDeviceWorking(int nCar, int nType);
	bool SetCarStatus(int nCar, bool bBool);
	uint32_t GetOrientationTable(int nMovDirection, int nCar, int nDev);
	static std::function<void(int)> m_tDevUpdate;
	static std::function<void()> m_tCarUpdate;
	static std::function<void(const char*)> m_tDeviceStatusRecord[CAR_INVALID][DEV_INVALID];
	static std::function<void(const char*)> m_tCarStatusRecord[CAR_INVALID];
	void IsAllDeviceWorking();
private:
	static uint8_t uDevOrientationTable[MOVDIR_INVALID][CAR_INVALID][DEV_INVALID];
	static const char* chIPTable[CAR_INVALID][DEV_INVALID];
	static uint64_t lDevTick[CAR_INVALID][DEV_INVALID];
	static uint64_t lDevTimeOut[CAR_INVALID][DEV_INVALID];
	static in_addr uIPTable[CAR_INVALID][DEV_INVALID];

	static int16_t uDevStatus[CAR_INVALID][DEV_INVALID];
	static int16_t uPrevDevStatus[CAR_INVALID][DEV_INVALID];
	static int16_t uCarStatus[CAR_INVALID];
	static int16_t uPrevCarStatus[CAR_INVALID];
	static int16_t uStatus[90][2];
	

	static const uint8_t uValueToEnumCar[16];
	static const uint8_t uValueToEnumDev[110];
	//std::map<unsigned int, uint64_t> m_mDevTick;

};

#endif
