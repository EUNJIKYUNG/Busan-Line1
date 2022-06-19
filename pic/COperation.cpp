#include "COperation.h"
#include "OperRelated/OperManage.h"
#include <pthread.h>
#include "CDevIdentify.h"
#include "CDataManage.h"



COperation::COperation()
	:m_bIntSIM(false)
{
	m_bSimulation = false;
	m_bUsingNow=false;
	m_nOperationMode = 0;
	pthread_mutex_init(&m_tOperation, NULL);
}

COperation *COperation::GetInstance()
{
	static COperation tOper;
	return &tOper;
}


COperation::~COperation()
{
	pthread_mutex_destroy(&m_tOperation);
}



bool COperation::SetOperationNextStationIndex(int32_t nOperationIndex, OPERATION_MODE eMode)
{
	pthread_mutex_lock(&m_tOperation);
	COperManage *pOM = COperManage::GetInstance();
	if (pOM->SetStationByDistanceIndex(nOperationIndex,false, false, eMode))
	{
		int nStationCode = pOM->GetStationCodeByOrder(0, eMode);
		int nLimitDistance = pOM->GetLimitDistance(false, eMode);
		int nProceededDistance = pOM->GetProceededDistance(false, eMode);
		pthread_mutex_unlock(&m_tOperation);
		return true;
	}
	pthread_mutex_unlock(&m_tOperation);
	return false;
}

bool COperation::SetOperationEventIndex(int32_t uIndex, OPERATION_MODE eMode)
{
	pthread_mutex_lock(&m_tOperation);
	COperManage *pOM = COperManage::GetInstance();
	if (pOM->GoToStationBySelectedEvent(uIndex, false, eMode))
	{
		int nStationCode = pOM->GetStationCodeByOrder(0, eMode);
		int nLimitDistance = pOM->GetLimitDistance(false, eMode);
		int nProceededDistance = pOM->GetProceededDistance(false, eMode);
		pthread_mutex_unlock(&m_tOperation);
		return true;
	}
	pthread_mutex_unlock(&m_tOperation);
	return false;
}


bool COperation::SetOperationNextStationCode(int32_t nStationCode, OPERATION_MODE eMode)
{
	bool bRet = false;
	COperManage *pOM = COperManage::GetInstance();
	pthread_mutex_lock(&m_tOperation);
	if (pOM->GetStationCodeByOrder(0, eMode) != nStationCode)
	{
		bRet = pOM->GoToStation(nStationCode, false, false, eMode);
	}
	else
		bRet = true;
	pthread_mutex_unlock(&m_tOperation);
	return bRet;
}


bool COperation::SetTrainNumber(const char *pTrainNumber)
{
	COperManage *pOM = COperManage::GetInstance();
	CDataManage *pDM = CDataManage::GetInstance();
	pDM->setTrainNumber(0, pTrainNumber);
	//pDM->setUpdateFlagFDIImage();
#ifdef _WIN32
	pDM->UpdateBitmap();
#endif

	pthread_mutex_lock(&m_tOperation);
	if (pOM->SetTrainNumber(pTrainNumber, false))
	{
		int nStopPtnIndex=pOM->GetStopPatternIndex();
		if(!m_bSimulation)
		{
			m_tSimulation(nStopPtnIndex, (*m_pStrPtnRoutes));
		}
		pthread_mutex_unlock(&m_tOperation);
		return true;
	}
	pthread_mutex_unlock(&m_tOperation);
}




bool COperation::SetStopPatternByIndex(int32_t uIndex)
{
	COperManage *pOM = COperManage::GetInstance();
	if (pOM->GetStopPatternIndex() != uIndex)
	{
		pthread_mutex_lock(&m_tOperation);
		if (pOM->SetStopPatternByIndex(uIndex))
		{

			pthread_mutex_unlock(&m_tOperation);
			return true;
		}
		pthread_mutex_unlock(&m_tOperation);
		return false;
	}
	return true;	
}

bool COperation::SetDistance(int32_t uDistance, OPERATION_MODE eMode)
{
	COperManage *pOM = COperManage::GetInstance();
	pthread_mutex_lock(&m_tOperation);
	bool bReset = (m_bSimulation ? true : (!m_nOperationMode) ? false : true);
	bool bRet = pOM->SetDistance(uDistance, bReset, false, eMode);
	pthread_mutex_unlock(&m_tOperation);
	return bRet;
}


void COperation::RunLoop()
{
	if (m_bSimulation)
	{
		pthread_mutex_lock(&m_tOperation);
		COperManage *pOM = COperManage::GetInstance();
		int nDistance=pOM->GetProceededDistance(false, OM_SIMULATION);
		if (!pOM->SetDistance(nDistance + m_nSimDistance, true, false, OM_SIMULATION))
		{
			SetPIITextIndex(0);
			pOM->GoToNextStation(false, OM_SIMULATION);
			int nStationCode = pOM->GetStationCodeByOrder(0, OM_SIMULATION);
			int nLimitDistance = pOM->GetLimitDistance(false, OM_SIMULATION);
			int nProceededDistance = pOM->GetProceededDistance(false, OM_SIMULATION);
		}
		SetIDD1Information(OM_SIMULATION);
		pthread_mutex_unlock(&m_tOperation);
	}
	else
	{
		//if (m_nOperationMode)
		{
			pthread_mutex_lock(&m_tOperation);
			SetIDD1Information(OM_NORMAL);
			pthread_mutex_unlock(&m_tOperation);
		}
	}
	COperManage *pOM = COperManage::GetInstance();

	if (pushLineType)
	{
		std::string strDest = pOM->GetDestinationNameUTF8(OM_NORMAL);
		pushLineType(strDest.c_str());
	}

	if (pushNextStation)
	{
		std::string strNext = pOM->GetNextStationNameUTF8(OM_NORMAL);
		pushNextStation(strNext.c_str());
	}
	if (pushThisStation)
	{
		std::string strThis = pOM->GetCurrentStationNameUTF8(OM_NORMAL);
		pushThisStation(strThis.c_str());
	}

	CDevIdentify *pIdent=CDevIdentify::GetInstance();
	pIdent->IsAllDeviceWorking();
	
}

int32_t COperation::GetOperationMode()
{
	return m_nOperationMode;
}

void COperation::SetOperationMode(int32_t nOperationMode)
{
	m_nOperationMode = nOperationMode;
	if(pushManualOper)
		pushManualOper(m_nOperationMode?"false":"true");
	if(pushDestSelect)
		pushDestSelect(m_nOperationMode?"false":"true");
	if(pushTrainNumber)
		pushTrainNumber(m_nOperationMode?"false":"true");
	if(updateMenu)
		updateMenu();
}


void COperation::SetSimulationMode(bool bSim)
{
	m_bSimulation = bSim;
}

bool COperation::GetSimulationMode()
{
	return m_bSimulation;
}

void COperation::SetSimulationOffset(int32_t nDistance)
{
	m_nSimDistance=nDistance;
}

void COperation::SetIDD1Information(OPERATION_MODE eMode)
{
	COperManage *pOM = COperManage::GetInstance();
	m_tPISC2IDD.uCurStnCode = pOM->GetStationCodeByOrder(-1, eMode);
	m_tPISC2IDD.uNexStnCode = pOM->GetStationCodeByOrder(0, eMode);
	m_tPISC2IDD.uDesStnCode = pOM->GetDestStationCode();
	m_tPISC2IDD.uDistance = pOM->GetProceededDistance(false, eMode);
	m_tPISC2IDD.uTotalDistance = pOM->GetLimitDistance(false, eMode);
	m_tPISC2IDD.uStopPtnIndex = pOM->GetStopPatternIndex();

	printf("Cur:%u Nex:%u Des:%u StpPtn:%u\n",m_tPISC2IDD.uCurStnCode,m_tPISC2IDD.uNexStnCode,m_tPISC2IDD.uDesStnCode,m_tPISC2IDD.uStopPtnIndex);
	if (m_uPIDCount)
		m_tPISC2IDD.uEventIndex = m_uPIDIndex;
	else
		m_tPISC2IDD.uEventIndex = 0;
	if (m_uPIDCount)
		m_uPIDCount--;

}

bool COperation::GetIDD1Information( _PISC2IDD1 *pPISC)
{
	pthread_mutex_lock(&m_tOperation);
	memcpy(pPISC, &m_tPISC2IDD, sizeof(_PISC2IDD1));
	pthread_mutex_unlock(&m_tOperation);
	return true;
}

_PISC2IDD1 *COperation::GetIDD1()
{
	return &m_tPISC2IDD;
}

void COperation::SetFTNDD(int nIndex)
{
	CDataManage *pDM = CDataManage::GetInstance();
	pDM->setDestinationImage(1, nIndex);
	pDM->setFDISync(true);
	pDM->setUpdateFlagFDIImage();
#ifdef _WIN32
	pDM->UpdateBitmap();
#endif
}


bool COperation::SetDRMIndex(int nIndex)
{
	CTableManage *pTM = CTableManage::GetInstance();
	auto it=find_if(pTM->m_vPIDContents.begin(), pTM->m_vPIDContents.end(), findSQLData(nIndex));
	if (it != pTM->m_vPIDContents.end())
	{
		PIDContents *pContent = (PIDContents*)it->get();
		if (pContent->m_vChildItem[0].vSQLData.size())
		{
			PIDIndexList *pIL=(PIDIndexList*)(pContent->m_vChildItem[0].vSQLData.begin()->get());
			m_uPIDIndex = pIL->nContentsPoolIndex;
			m_uPIDCount = 3;
			return true;
		}
	}
	return false;
}

void COperation::SetPIITextIndex(int nIndex)
{
	CDataManage *pDM = CDataManage::GetInstance();
	CTableManage *pTM = CTableManage::GetInstance();
	auto it=find_if(pTM->m_vPIIText.begin(), pTM->m_vPIIText.end(), findSQLData(nIndex));
	if (it != pTM->m_vPIIText.end())
	{
		
		PIIText *pText=(PIIText*)it->get();
		int nCount = 0;
		while (pText->szPIIText[nCount++]);
		pDM->setPII1Text((unsigned short*)pText->szPIIText, nCount);
		pDM->setUpdateFlagPII1Image();
		pDM->setPII2Text((unsigned short*)pText->szPIIText, nCount);
		pDM->setUpdateFlagPII2Image();
	}
	else
	{
		unsigned short uTempBuf[128]=
		{
			0x20,0x00
		};
		pDM->setPII1Text((unsigned short*)uTempBuf, 1);
		pDM->setUpdateFlagPII1Image();
		pDM->setPII2Text((unsigned short*)uTempBuf, 1);
		pDM->setUpdateFlagPII2Image();
	}
}




void COperation::setStopPtnRouteString(std::string *pString)
{
	m_pStrPtnRoutes = pString;
}