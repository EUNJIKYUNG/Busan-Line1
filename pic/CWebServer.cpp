#include "CWebServer.h"
#include "OperRelated/OperManage.h"
#include "TablesDefine/TableManage.h"
#include "CDataManage.h"

#include "CDevIdentify.h"

#include "webPages/DestinationHandler.h"
#include "webPages/IndexHandler.h"
#include "webPages/MenuHandler.h"
#include "webPages/DetailedDiagnosisHandler.h"
#include "webPages/SimulationHandler.h"
#include "webPages/SelfDiagnosisHandler.h"
#include "webPages/TrainNumberHandler.h"
#include "webPages/ManualOperationHandler.h"
#include "webPages/OperationModeSelectionHandler.h"

#include "webPages/WebSocketDestinationHandler.hpp"
#include "webPages/WebSocketIndexHandler.hpp"
#include "webPages/WebSocketSimulationHandler.hpp"
#include "webPages/WebSocketTrainNumberHandler.hpp"
#include "webPages/WebSocketDetailedDiagnosisHandler.hpp"
#include "webPages/WebSocketMenuHandler.hpp"
#include "webPages/WebSocketDiagnosisHandler.hpp"
#include "webPages/WebSocketManualOperationHandler.hpp"
#include "webPages/WebSocketOperaitonModeSelectionHandler.hpp"
#include "COperation.h"


#define DOCUMENT_ROOT "."

#define PORT "8081"

#define EXAMPLE_URI "/example"

#define EXIT_URI "/exit"


CWebServer::CWebServer()
{
}


CWebServer::~CWebServer()
{
}

CWebServer* CWebServer::GetInstance()
{
	static CWebServer tServer;
	return &tServer;
}


bool CWebServer::InitWebServer()
{
	COperManage *pOM = COperManage::GetInstance();
	pOM->InitStopPtnHeader();


	bool bRet=false;
	const char *options[] = {"document_root", DOCUMENT_ROOT, "listening_ports", PORT, 0 };



	std::vector<std::string> cpp_options;

	for (int i = 0; i < (sizeof(options) / sizeof(options[0]) - 1); i++) {

		cpp_options.push_back(options[i]);

	}

	//std::string strJSON;
	//SetStopStopPtnHeaderByType(0,strJSON, "stopPtnList");



	m_pCivetServer = std::shared_ptr<CivetServer>(new CivetServer(cpp_options));
	m_pIndexHandle=std::shared_ptr<CIndexHandle>(new CIndexHandle());
	m_pCSSHandle=std::shared_ptr<CCSSHandle>(new CCSSHandle());
	m_pImageHandle=std::shared_ptr<CImageHandle>(new CImageHandle());
	m_pMenuHandle = std::shared_ptr<CMenuHandle>(new CMenuHandle());
	m_pDestinationHandle = std::shared_ptr<CDestinationHandle>(new CDestinationHandle());
	m_pSimulationHandle = std::shared_ptr<CSimulationHandle>(new CSimulationHandle());
	m_pSelfDiagnosisHandle = std::shared_ptr<CSelfDiagnosisHandle>(new CSelfDiagnosisHandle());
	m_pDetailedDiagnosisHandle = std::shared_ptr<CDetailedDiagnosisHandle>(new CDetailedDiagnosisHandle());
	m_pTrainNumberHandle = std::shared_ptr<CTrainNumberHandle>(new CTrainNumberHandle());
	m_pManualOperationHandle = std::shared_ptr<CManualOperationHandler>(new CManualOperationHandler());
	m_pOperationModeSelectionHandle=std::shared_ptr<COperationModeSelectionHandle>(new COperationModeSelectionHandle());
	m_pCivetServer->addHandler("/menu", (*m_pMenuHandle.get()));
	m_pCivetServer->addHandler("/index", (*m_pIndexHandle.get()));
	m_pCivetServer->addHandler("/destination", (*m_pDestinationHandle.get()));
	m_pCivetServer->addHandler("/image", (*m_pImageHandle.get()));
	m_pCivetServer->addHandler("/detailedDiagnosis", (*m_pDetailedDiagnosisHandle.get()));
	m_pCivetServer->addHandler("/simulation", (*m_pSimulationHandle.get()));
	m_pCivetServer->addHandler("/selfDiagnosis", (*m_pSelfDiagnosisHandle.get()));
	m_pCivetServer->addHandler("/trainNumber", (*m_pTrainNumberHandle.get()));
	m_pCivetServer->addHandler("/manualOperation", (*m_pManualOperationHandle.get()));
	m_pCivetServer->addHandler("/operationModeSelect", (*m_pOperationModeSelectionHandle.get()));

#ifdef USE_WEBSOCKET
	m_pWSSimulationHandle = std::shared_ptr<CWSSimulationHandle>(new CWSSimulationHandle());
	m_pCivetServer->addWebSocketHandler("/wsSimulation",m_pWSSimulationHandle.get());

	m_pWSDestinationHandle = std::shared_ptr<CWSDestinationHandle>(new CWSDestinationHandle());
	m_pCivetServer->addWebSocketHandler("/wsDestination", m_pWSDestinationHandle.get());

	m_pWSTrainNumberHandle = std::shared_ptr<CWSTrainNumberHandle>(new CWSTrainNumberHandle());
	m_pCivetServer->addWebSocketHandler("/wsTrainNumber", m_pWSTrainNumberHandle.get());

	m_pWSDetailedDiagnosisHandle = std::shared_ptr<CWSDetailedDiagnosisHandle>(new CWSDetailedDiagnosisHandle());
	m_pCivetServer->addWebSocketHandler("/wsDD", m_pWSDetailedDiagnosisHandle.get());

	m_pWSIndexHandle = std::shared_ptr<CWSIndexHandle>(new CWSIndexHandle());
	m_pCivetServer->addWebSocketHandler("/wsIndex", m_pWSIndexHandle.get());

	m_pWSMenuHandle = std::shared_ptr<CWSMenuHandle>(new CWSMenuHandle());
	m_pCivetServer->addWebSocketHandler("/wsMenu", m_pWSMenuHandle.get());

	m_pWSSelfDiagnosisHandle= std::shared_ptr<CWSSelfDiagnosisHandle>(new CWSSelfDiagnosisHandle());
	m_pCivetServer->addWebSocketHandler("/wsSelfDiagnosis", m_pWSSelfDiagnosisHandle.get());

	m_pWSTrainNumberHandle = std::shared_ptr<CWSTrainNumberHandle>(new CWSTrainNumberHandle());
	m_pCivetServer->addWebSocketHandler("/wsTrainNumber", m_pWSTrainNumberHandle.get());

	m_pWSManualOperationHandle = std::shared_ptr<CWSManualOperationHandle>(new CWSManualOperationHandle());
	m_pCivetServer->addWebSocketHandler("/wsManualOperation", m_pWSManualOperationHandle.get());

	m_pWSOperationModeSelectionHandle = std::shared_ptr<CWSOperationModeSelectionHandle>(new CWSOperationModeSelectionHandle());
	m_pCivetServer->addWebSocketHandler("/wsOperationModeSelect", m_pWSOperationModeSelectionHandle.get());

	COperation *pO=COperation::GetInstance();

	m_pWSOperationModeSelectionHandle->m_tSetOperMode = std::bind(&COperation::SetOperationMode, COperation::GetInstance(), std::placeholders::_1);
	m_pWSOperationModeSelectionHandle->m_tGetOperMode= std::bind(&COperation::GetOperationMode, COperation::GetInstance());
    
	m_pWSDestinationHandle->setStopPtnHeaderString(&m_strStopHeader);
    m_pWSDestinationHandle->setStopPtnRouteString(&m_strStopRoutes);
	m_pWSDestinationHandle->selectStopPtnRouteString(&m_strStopSelRoutes);
	m_pWSDestinationHandle->setStopPtnRouteStringForSimulation(&m_StrStopRoutesSimulation);

	m_pWSSimulationHandle->setStopPtnRouteString(&m_strStopRoutes);
	m_pWSSimulationHandle->setEventListString(&m_strEventLists);


	m_pWSManualOperationHandle->setStopPtnRouteString(&m_strStopRoutes);
	m_pWSManualOperationHandle->setEventListString(&m_strEventLists);

	SetStopStopPtnHeaderByType(0, m_strStopHeader, "stopList");

	m_pWSDestinationHandle->setStopPtnDirection(0);
    m_pWSDestinationHandle->m_tSelRoutes = std::bind(&CWebServer::SelectedStopPtnList, this, std::placeholders::_1, std::placeholders::_2, "routeList", OM_NORMAL); // 220706 KEJ 노선선택 이벤트
	m_pWSDestinationHandle->m_tRoutes = std::bind(&CWebServer::SetSelectedStopPtnList, this, std::placeholders::_1, std::placeholders::_2, "routeList", OM_NORMAL);
	m_pWSDestinationHandle->m_tDirection = std::bind(&CWebServer::SetStopStopPtnHeaderByType, this, std::placeholders::_1, std::placeholders::_2, "stopList");
	m_pWSDestinationHandle->m_tConfirmStopPtn = std::bind(&COperation::SetStopPatternByIndex, COperation::GetInstance(), std::placeholders::_1);
	m_pWSDestinationHandle->m_tSimulation = std::bind(&CWebServer::SetSelectedStopPtnList, this, std::placeholders::_1, std::placeholders::_2, "routeList", OM_SIMULATION);
	
	m_pWSSimulationHandle->m_tEvent = std::bind(&CWebServer::SetSelectedEventList, this, std::placeholders::_1, std::placeholders::_2, "eventList", OM_SIMULATION);
	m_pWSTrainNumberHandle->m_tConfirmTrainNumber = std::bind(&COperation::SetTrainNumber, COperation::GetInstance(), std::placeholders::_1);
	m_pWSSimulationHandle->m_tConfirmEventList = std::bind(&COperation::SetOperationEventIndex, COperation::GetInstance(), std::placeholders::_1,OM_SIMULATION);
	m_pWSSimulationHandle->m_tGoToStation = std::bind(&COperation::SetOperationNextStationIndex, COperation::GetInstance(), std::placeholders::_1, OM_SIMULATION);
	
	m_pWSSimulationHandle->m_tSetSimulationMode = std::bind(&COperation::SetSimulationMode, COperation::GetInstance(), std::placeholders::_1);
	m_pWSSimulationHandle->m_tGetSimulationMode = std::bind(&COperation::GetSimulationMode, COperation::GetInstance());
	m_pWSSimulationHandle->m_tSetDistance=std::bind(&COperation::SetSimulationOffset,COperation::GetInstance(),std::placeholders::_1);
	
	m_pWSManualOperationHandle->m_tGoToStation = std::bind(&COperation::SetOperationNextStationIndex, COperation::GetInstance(), std::placeholders::_1, OM_NORMAL);
	m_pWSManualOperationHandle->m_tConfirmEventList = std::bind(&COperation::SetOperationEventIndex, COperation::GetInstance(), std::placeholders::_1, OM_NORMAL);
	m_pWSManualOperationHandle->m_tEvent = std::bind(&CWebServer::SetSelectedEventList, this, std::placeholders::_1, std::placeholders::_2, "eventList", OM_SIMULATION);

	
	pOM->m_tEventIndex = std::bind(&CWSSimulationHandle::pushNumberData, m_pWSSimulationHandle.get(), "slteventList", std::placeholders::_1,true);
	pOM->m_tRouteIndex = std::bind(&CWSSimulationHandle::pushNumberData, m_pWSSimulationHandle.get(), "sltrouteList", std::placeholders::_1,true);
	pOM->m_tDestInd = std::bind(&COperation::SetFTNDD, COperation::GetInstance(), std::placeholders::_1);
	pOM->m_tFuncDRM = std::bind(&COperation::SetDRMIndex, COperation::GetInstance(), std::placeholders::_1);
	pOM->m_tPIIText = std::bind(&COperation::SetPIITextIndex,COperation::GetInstance(), std::placeholders::_1);

    pO->chgOperMode = std::bind(&CWSIndexHandle::pushData, m_pWSIndexHandle.get(), "txtOperModeStat", std::placeholders::_1,true);      // 운행상태

	pO->pushLineType = std::bind(&CWSIndexHandle::pushData, m_pWSIndexHandle.get(), "txtLineType", std::placeholders::_1,true);         // 행선
	pO->pushDestStation =std::bind(&CWSIndexHandle::pushData, m_pWSIndexHandle.get(), "txtDestination", std::placeholders::_1, true);   // 종착역
	pO->pushNextStation =std::bind(&CWSIndexHandle::pushData, m_pWSIndexHandle.get(), "txtNextStation", std::placeholders::_1, true);   // 이번역

    // 220706 KEJ 열차번호 설정
    pO->chgTrainNum =std::bind(&CWSIndexHandle::pushData, m_pWSIndexHandle.get(), "txtTrainNum", std::placeholders::_1, true);   // 열차번호

    // 수동운행이면 노선설정, 열차번호설정, 수동운행 버튼 활성화 됨
	pO->pushManualOper=std::bind(&CWSMenuHandle::pushData,m_pWSMenuHandle.get(),"disbtnMO",std::placeholders::_1);
	pO->pushDestSelect=std::bind(&CWSMenuHandle::pushData,m_pWSMenuHandle.get(),"disbtnDS",std::placeholders::_1);
	pO->pushTrainNumber=std::bind(&CWSMenuHandle::pushData,m_pWSMenuHandle.get(),"disbtnTN",std::placeholders::_1);
    pO->pushSimulation=std::bind(&CWSMenuHandle::pushData,m_pWSMenuHandle.get(),"disbtnSIM",std::placeholders::_1);
    
    
	pO->updateMenu=std::bind(&CWSMenuHandle::update,m_pWSMenuHandle.get());
	pO->setStopPtnRouteString(&m_strStopRoutes);
	pO->m_tSimulation=std::bind(&CWebServer::SetSelectedStopPtnList, this, std::placeholders::_1, std::placeholders::_2, "routeList", OM_SIMULATION);
	//Default: automatic operation by TCMS
	m_pWSMenuHandle->pushData("disbtnMO","true");
	m_pWSMenuHandle->pushData("disbtnDS","true");
	m_pWSMenuHandle->pushData("disbtnTN","true");
    m_pWSMenuHandle->pushData("disbtnSIM","true");
    
#endif
	return bRet;
}

bool CWebServer::SetTrainNumber(const char *pTrainNumber)
{

	CDataManage *pDM = CDataManage::GetInstance();
	if (pDM->setTrainNumber(0, pTrainNumber))
	{
		// pDM->setFDISync(true);
		// pDM->setUpdateFlagFDIImage();
#ifdef _WIN32
		pDM->UpdateBitmap();
#endif
		return true;
	}
	//printf("\nTrainNumber : %s", pTrainNumber);
	return false;
}

bool CWebServer::SetStopStopPtnHeaderByType(int nType, std::string &strJSON, const char *chTarget)
{
	bool bRet = false;
	std::string strTemp;
	CTableManage *pTM = CTableManage::GetInstance();
	COperManage *pOM = COperManage::GetInstance();
	pOM->GetStopPtnHeaderTypeList(nType);
	pTM->m_pStopPtnHeaderEditor->SetVectors(&pOM->m_vStopPatternHeader[OM_NORMAL], 0);
	pTM->m_pStopPtnHeaderEditor->ResetJSONColumn();
	pTM->m_pStopPtnHeaderEditor->AddJSONColumn(0);
	pTM->m_pStopPtnHeaderEditor->AddJSONColumn(3);
	int nCount = pTM->m_pStopPtnHeaderEditor->GetAllJSONColumn(strTemp);
	if (nCount)
	{
		MakeJSONListString(nCount, strJSON, strTemp, chTarget);
		bRet = true;
	}
	return bRet;
}


bool CWebServer::SetStopPtnList(std::string &strJSON,const char *chTarget)
{
	bool bRet = false;
	std::string strTemp;
	CTableManage *pTM = CTableManage::GetInstance();
	pTM->m_pStopPtnHeaderEditor->ResetJSONColumn();
	pTM->m_pStopPtnHeaderEditor->AddJSONColumn(0);
	pTM->m_pStopPtnHeaderEditor->AddJSONColumn(3);
	int nCount=pTM->m_pStopPtnHeaderEditor->GetAllJSONColumn(strTemp);
	if (nCount)
	{
		MakeJSONListString(nCount, strJSON, strTemp, chTarget);
		bRet = true;
	}
	return bRet;
}

// 220706 KEJ 노선선택 이벤트
bool CWebServer::SelectedStopPtnList(int nIndex, std::string &strJSON, const char *chTarget,int nOperMode)
{
	bool bRet = false;
	std::string strTemp;

	CTableManage *pTM = CTableManage::GetInstance();
	COperManage *pOM = COperManage::GetInstance();
	if (pOM->SelectStopPatternByIndex(nIndex))
	{
		pTM->m_pStopPtnRoutesEditor->SetVectors(&pOM->m_vStopRoutes[nOperMode], NULL);
		pTM->m_pStopPtnRoutesEditor->ResetJSONColumn();
		pTM->m_pStopPtnRoutesEditor->AddJSONColumn(0);
		pTM->m_pStopPtnRoutesEditor->AddJSONColumnToFunction(3,
			std::move(std::bind(&CEditSQLData::ConvertFunctionFieldData, (CEditSQLData*)pTM->m_pStopPtnRoutesEditor.get(),
				pTM->m_vStationDistance, 4,
				std::placeholders::_1, std::placeholders::_2)));

		int nCount = pTM->m_pStopPtnRoutesEditor->GetAllJSONColumn(strTemp);
		if (nCount)
		{
			MakeJSONListString(nCount, strJSON, strTemp, chTarget);
			bRet = true;
		}
	}
	return bRet;
}


bool CWebServer::SetSelectedStopPtnList(int nIndex, std::string &strJSON, const char *chTarget,int nOperMode)
{
	bool bRet = false;
	std::string strTemp;

	CTableManage *pTM = CTableManage::GetInstance();
	COperManage *pOM = COperManage::GetInstance();
	if (pOM->SetStopPatternByIndex(nIndex))
	{
		pTM->m_pStopPtnRoutesEditor->SetVectors(&pOM->m_vStopRoutes[nOperMode], NULL);
		pTM->m_pStopPtnRoutesEditor->ResetJSONColumn();
		pTM->m_pStopPtnRoutesEditor->AddJSONColumn(0);
		pTM->m_pStopPtnRoutesEditor->AddJSONColumnToFunction(3,
			std::move(std::bind(&CEditSQLData::ConvertFunctionFieldData, (CEditSQLData*)pTM->m_pStopPtnRoutesEditor.get(),
				pTM->m_vStationDistance, 4,
				std::placeholders::_1, std::placeholders::_2)));

		int nCount = pTM->m_pStopPtnRoutesEditor->GetAllJSONColumn(strTemp);
		if (nCount)
		{
			MakeJSONListString(nCount, strJSON, strTemp, chTarget);
			bRet = true;
		}
	}
	return bRet;
}




bool CWebServer::SetSelectedEventList(int nRoutesIndex, std::string &strJSON, const char *chTarget,int nOperMode)
{
	bool bRet = false;
	std::string strTemp;
	CTableManage *pTM = CTableManage::GetInstance();
	COperManage *pOM = COperManage::GetInstance();
	printf("[SetSelectedEventList %d] %d %s \n",__LINE__,nRoutesIndex,strJSON.c_str());
	std::vector<std::shared_ptr<CSQLData>>::iterator xit=find_if(pOM->m_vStopRoutes[nOperMode].begin(), pOM->m_vStopRoutes[nOperMode].end(), findSQLData(nRoutesIndex));
	if (xit != pOM->m_vStopRoutes[nOperMode].end())
	{
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		//EventLists *pList=(EventLists*)xit->get()->m_vChildItem[0].vSQLData[0].get();
		//wchar_t* pDesc=(wchar_t*)pList->szDesc;
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		pTM->m_pEventListsEditor->ResetJSONColumn();
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		pTM->m_pEventListsEditor->SetVectors(&(xit->get()->m_vChildItem[0].vSQLData), NULL);
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		pTM->m_pEventListsEditor->AddJSONColumn(0);
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		pTM->m_pEventListsEditor->AddJSONColumn(6);
	}
	printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
	int nCount = pTM->m_pEventListsEditor->GetAllJSONColumn(strTemp);
	if (nCount)
	{
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		MakeJSONListString(nCount, strJSON, strTemp, chTarget);
		printf("[SetSelectedEventList %d]%s \n",__LINE__,strJSON.c_str());
		bRet = true;
	}
	return bRet;
}

void CWebServer::MakeJSONListString(int nCount,std::string &strJSON,std::string &strTemp,const char *chTarget)
{
	strJSON.clear();
	strJSON.append("{\"type\":\"updateall\",\"count\":");
	strJSON.append(std::to_string(nCount));
	strJSON.append(",\"target\":\"");
	strJSON.append(chTarget);
	strJSON.append("\",\"list\":");
	strJSON.append(strTemp);
	strJSON.append("}");
}

void CWebServer::updateTime()
{
	std::time_t t = std::time(nullptr);
	std::tm *pTime = std::localtime(&t);
	char szBuffer[64] = { 0 };

	if ((pTime->tm_year != preTM.tm_year) || (pTime->tm_mon != preTM.tm_mon) || (pTime->tm_mday != preTM.tm_mday))
	{
		sprintf(szBuffer, "%04d - %02d - %02d", pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday);
		m_pWSDestinationHandle->pushData("txttxtDate",szBuffer);
		m_pWSDestinationHandle->update();
		m_pWSSimulationHandle->pushData("txttxtDate", szBuffer);
		m_pWSSimulationHandle->update();
		m_pWSTrainNumberHandle->pushData("txttxtDate", szBuffer);
		m_pWSTrainNumberHandle->update();
	}
	if ((pTime->tm_hour != preTM.tm_hour) || (pTime->tm_min != preTM.tm_min) || (pTime->tm_sec != preTM.tm_sec))
	{
		int nHour = (pTime->tm_hour > 12) ? (pTime->tm_hour - 12) : (pTime->tm_hour) ? pTime->tm_hour : 12;
		sprintf(szBuffer, "%s %02d : %02d : %02d", (pTime->tm_hour > 12) ? ("PM") : pTime->tm_hour ? ("AM") : ("PM"), nHour, pTime->tm_min, pTime->tm_sec);
		m_pWSDestinationHandle->pushData("txttxtTime",szBuffer);
		m_pWSDestinationHandle->update();
		m_pWSSimulationHandle->pushData("txttxtTime", szBuffer);
		m_pWSSimulationHandle->update();
		m_pWSTrainNumberHandle->pushData("txttxtTime", szBuffer);
		m_pWSTrainNumberHandle->update();
	}
	memcpy(&preTM, pTime, sizeof(std::tm));
}

void CWebServer::SelectSimulationIndex(const char *pTarget, const char *pValue)
{
	m_pWSSimulationHandle->pushData(pTarget, pValue);
	m_pWSSimulationHandle->update();
}

#define TOSTRING(A,B) (#A#B)

void CWebServer::InitCheckFunc()
{
	CDevIdentify::m_tDevUpdate = std::bind(&CWSDetailedDiagnosisHandle::updateArray, m_pWSDetailedDiagnosisHandle.get(),std::placeholders::_1);
	CDevIdentify::m_tCarUpdate = std::bind(&CWSSelfDiagnosisHandle::update, m_pWSSelfDiagnosisHandle.get());

	CDevIdentify::m_tCarStatusRecord[CAR_1] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_1), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_2] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_2), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_3] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_3), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_4] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_4), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_5] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_5), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_6] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_6), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_7] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_7), std::placeholders::_1);
	CDevIdentify::m_tCarStatusRecord[CAR_0] = std::bind(&CWSSelfDiagnosisHandle::pushData, (CWSSelfDiagnosisHandle*)m_pWSSelfDiagnosisHandle.get(), TOSTRING(ind, CAR_0), std::placeholders::_1);

	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_PISC] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_PISC), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_FTNDD] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind, D_FTNDD), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_1][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_1, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);


	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_2][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_2, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);

	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_3][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_3, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);

	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_4][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_4, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);


	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_5][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_5, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);

	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_6][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_6, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);

	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_PII_1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_PII_1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_7][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_7, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);


	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_PISC] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_PISC), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_FTNDD] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_FTNDD), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_PII_2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_PII_2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD1_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD1_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD1_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD1_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD1_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD1_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD1_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD1_L2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD2_R1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD2_R1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD2_L1] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD2_L1), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD2_R2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD2_R2), std::placeholders::_1);
	CDevIdentify::m_tDeviceStatusRecord[CAR_0][D_IDD2_L2] = std::bind(&CWSDetailedDiagnosisHandle::pushArrayData, (CWSDetailedDiagnosisHandle*)m_pWSDetailedDiagnosisHandle.get(), CAR_0, TOSTRING(ind,D_IDD2_L2), std::placeholders::_1);
}
