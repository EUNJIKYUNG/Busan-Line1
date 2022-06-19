#ifndef PA_OPERATION
#define PA_OPERATION

#include <pthread.h>
#include <atomic>
#include "OperRelated/OperManage.h"
#include "defineStruct.h"



class COperation
{
public:
	COperation();
	~COperation();
	static COperation *GetInstance();
	void RunLoop();
	bool SetOperationNextStationCode(int32_t nStationCode, OPERATION_MODE eMode);
	bool SetOperationNextStationIndex(int32_t nStopPatternIndex, OPERATION_MODE eMode);
	bool SetOperationEventIndex(int32_t uIndex, OPERATION_MODE eMode);
	bool SetStopPatternByIndex(int32_t uIndex);
	bool SetDistance(int32_t uDistance, OPERATION_MODE eMode);
	void SetSimulationMode(bool bSim);
	bool GetSimulationMode();
	void SetOperationMode(int32_t nOperationMode);
	void SetSimulationOffset(int32_t nDistance);
	int32_t GetOperationMode();
	bool SetDRMIndex(int nIndex);
	void SetPIITextIndex(int nIndex);
	std::function<void(const char*)> pushLineType;
	std::function<void(const char*)> pushNextStation;
	std::function<void(const char*)> pushThisStation;
	std::function<void(const char*)> pushManualOper;
	std::function<void(const char*)> pushDestSelect;
	std::function<void(const char*)> pushTrainNumber;
	std::function<void(void)> updateMenu;
	std::function<bool(int, std::string&)> m_tSimulation;

	bool SetTrainNumber(const char *pTrainNumber);
	void SetIDD1Information(OPERATION_MODE eMode);
	bool GetIDD1Information(_PISC2IDD1 *pPISC);
	_PISC2IDD1 *GetIDD1();
	void SetFTNDD(int nIndex);

	void setStopPtnRouteString(std::string *pString);
private:
	pthread_mutex_t m_tOperation;


	std::atomic<uint32_t> m_nOperMode;
	std::atomic<uint32_t> m_nEventIndex;
	std::atomic<int32_t> m_nOperationMode;
	std::atomic<bool> m_bUsingNow;
	std::atomic<bool> m_bSimulation;

	std::atomic<int32_t> m_uPIDIndex;
	std::atomic<int32_t> m_uPIDCount;

	std::atomic<int32_t> m_nSimDistance;

	std::string *m_pStrPtnRoutes;
	bool m_bIntSIM;

	_PISC2IDD1 m_tPISC2IDD;
};

#endif
