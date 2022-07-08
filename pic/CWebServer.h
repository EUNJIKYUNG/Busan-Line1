#ifndef WEBSERVERMAIN
#define WEBSERVERMAIN
#include <memory>
#include <string>
#include <ctime>

class CivetServer;
class CIndexHandle;
class COperationModeSelectionHandle;
class CDestinationHandle;
class CSelfDiagnosisHandle;
class CDetailedDiagnosisHandle;
class CFormNumberHandle;
class CMenuHandle;
class CPowerSavingHandle;
class CManualOperationHandler;
class CSimulationHandle;
class CSystemInfoHandle;
class CTrainNumberHandle;
class CCSSHandle;
class CImageHandle;

class CWSIndexHandle;
class CWSDestinationHandle;
class CWSSelfDiagnosisHandle;
class CWSDetailedDiagnosisHandle;
class CWSFormNumberHandle;
class CWSMenuHandle;
class CWSPowerSavingHandle;
class CWSManualOperationHandle;
class CWSSimulationHandle;
class CWSSystemInfoHandle;
class CWSTrainNumberHandle;
class CWSOperationModeSelectionHandle;




class CWebServer
{
public:
	static CWebServer* GetInstance();
	bool InitWebServer();
	void SetDevStatus(int nCar, const char *pDev, bool bDevice);
	bool SetStopStopPtnHeaderType(int nType);
	bool SetStopPtnList(std::string &strJSON, const char *);
    bool SelectedStopPtnList(int nIndex, std::string &strJSON, const char *,int);
	bool SetSelectedStopPtnList(int nIndex, std::string &strJSON, const char *,int);
	bool SetSelectedEventList(int nIndex, std::string &strJSON, const char *,int);
	void MakeJSONListString(int nCount, std::string &strJSON, std::string &strTemp, const char *chTarget);
	void SelectSimulationIndex(const char *pTarget, const char *pValue);
	void updateTime();
	void InitCheckFunc();
	bool SetStopStopPtnHeaderByType(int nType, std::string &strJSON, const char *chTarget);
	bool SetTrainNumber(const char *pTrainNumber);
private:
	CWebServer();
	~CWebServer();


	std::string m_strStopHeader;
    std::string m_strStopSelRoutes;
	std::string m_strStopRoutes;
	std::string m_strEventLists;
	std::string m_StrStopRoutesSimulation;

	std::shared_ptr<CCSSHandle> m_pCSSHandle;
	std::shared_ptr<CImageHandle> m_pImageHandle;
	std::shared_ptr<CivetServer> m_pCivetServer;
	std::shared_ptr<CIndexHandle> m_pIndexHandle;
	std::shared_ptr<COperationModeSelectionHandle> m_pOperationModeSelectionHandle;
	std::shared_ptr<CDestinationHandle> m_pDestinationHandle;
	std::shared_ptr<CSelfDiagnosisHandle> m_pSelfDiagnosisHandle;
	std::shared_ptr<CDetailedDiagnosisHandle> m_pDetailedDiagnosisHandle;
	std::shared_ptr<CFormNumberHandle> m_pFormNumberHandle;
	std::shared_ptr<CMenuHandle> m_pMenuHandle;
	std::shared_ptr<CPowerSavingHandle> m_pPowerSavingHandle;
	std::shared_ptr<CManualOperationHandler> m_pManualOperationHandle;
	std::shared_ptr<CSimulationHandle> m_pSimulationHandle;
	std::shared_ptr<CSystemInfoHandle> m_pSystemInfoHandle;
	std::shared_ptr<CTrainNumberHandle> m_pTrainNumberHandle;


	std::shared_ptr<CWSIndexHandle> m_pWSIndexHandle;
	std::shared_ptr<CWSOperationModeSelectionHandle> m_pWSOperationModeSelectionHandle;
	std::shared_ptr<CWSDestinationHandle> m_pWSDestinationHandle;
	std::shared_ptr<CWSSelfDiagnosisHandle> m_pWSSelfDiagnosisHandle;
	std::shared_ptr<CWSDetailedDiagnosisHandle> m_pWSDetailedDiagnosisHandle;
	std::shared_ptr<CWSFormNumberHandle> m_pWSFormNumberHandle;
	std::shared_ptr<CWSMenuHandle> m_pWSMenuHandle;
	std::shared_ptr<CWSPowerSavingHandle> m_pWSPowerSavingHandle;
	std::shared_ptr<CWSManualOperationHandle> m_pWSManualOperationHandle;
	std::shared_ptr<CWSSimulationHandle> m_pWSSimulationHandle;
	std::shared_ptr<CWSSystemInfoHandle> m_pWSSystemInfoHandle;
	std::shared_ptr<CWSTrainNumberHandle> m_pWSTrainNumberHandle;
	std::string strDate;
	std::string strTime;
	std::tm preTM;
};


#endif
