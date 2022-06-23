#include "OperManage.h"
#include <algorithm>
#include "../DataRelated/ValueMonitor.h"
#include "../DataRelated/SQLData.h"
#include <string>
//#include <QDebug>


COperManage *COperManage::GetInstance()
{
	static std::shared_ptr<COperManage> pManage;
	if (!pManage)
	{
		pManage = std::shared_ptr<COperManage>(new COperManage());
	}
	return pManage.get();
}

COperManage::COperManage()
	:m_nTolerance(-70), m_bImmediateEvent(false), m_bArrivedAtNextStation(false)
{
    memset(m_szTrainNumber,0,sizeof(m_szTrainNumber));
	memset(m_pCurStn,0,sizeof(m_pCurStn));
	memset(m_pNexStn,0,sizeof(m_pNexStn));
	memset(m_szDestinationUTF8,0,sizeof(m_szDestinationUTF8));
	memset(m_szCurStnUTF8,0,sizeof(m_szCurStnUTF8));
	memset(m_szNexStnUTF8,0,sizeof(m_szNexStnUTF8));
    memset(m_szDstStnUTF8,0,sizeof(m_szDstStnUTF8));
    memset(m_szDepStnUTF8,0,sizeof(m_szDepStnUTF8));
    
    m_pDestination=NULL;
    m_pTrainNumber=NULL;
	m_eOperMode = OM_NORMAL;


    m_nDepStnCode=0;
    m_nDstStnCode=0;
    m_nCurStnCode=0;
    m_nNexStnCode=0;
    m_nDstIndIdx=0;
    m_nStopPtnIdx=0;

    m_nAudioEventIdx=0;
    m_nDRMEventIdx=0;
    m_nPIDEventIdx=0;
	m_nDoorDirection=0;
	memset(m_pDistanceList, 0, sizeof(m_pDistanceList));
	memset(m_pTargetDistanceList, 0, sizeof(m_pTargetDistanceList));
#ifdef OPERMANAGE_CALLBACK_FUNC
	m_tFuncPID=std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncDRM=std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncVoice=std::bind(&COperManage::dummyCallback,this, std::placeholders::_1);
	m_tFuncDoorDir=std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncOpenAudio=std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncOpenPIDDRM=std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncCloseAudio = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncCloseVideo = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tFuncClosePIDDRM = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tDestInd = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tEventIndex = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
	m_tRouteIndex = std::bind(&COperManage::dummyCallback, this, std::placeholders::_1);
#endif
}

COperManage::~COperManage(void)
{
	for (int i = OM_NORMAL; i < OM_NULL; i++)
	{
		if (m_pDistanceList[i])
		{
			free(m_pDistanceList[i]);
			m_pDistanceList[i] = NULL;
		}
		if (m_pTargetDistanceList[i])
		{
			free(m_pTargetDistanceList[i]);
			m_pTargetDistanceList[i] = NULL;
		}
	}
}

void COperManage::InitStopPtnHeader()
{
	int nWholeDist;
	int nDepStnCode=0, nDepStnCodeAlt=0;
	int nArrStnCode=0, nArrStnCodeAlt=0;
    CTableManage *pTM=CTableManage::GetInstance();
    std::vector<SHARED_PTRC(CSQLData)>::iterator nit,nnit,findit,evtit,redunit;
    for(nit=pTM->m_vStopPtnHeader.begin();nit!=pTM->m_vStopPtnHeader.end();nit++)
    {
        StopPtnHeader *pSPH=(StopPtnHeader*)GET_PTFROMIT(nit);
		nWholeDist = 0;
        for(nnit=pSPH->m_vChildItem.at(0).vSQLData.begin();nnit!=pSPH->m_vChildItem.at(0).vSQLData.end();nnit++)
        {
            StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMIT(nnit);
			StationDistance *pSD = 0;
            findit=find_if(pTM->m_vStationDistance.begin(),pTM->m_vStationDistance.end(),findSQLData(pSPR->nDistIndex));
            if(findit!=pTM->m_vStationDistance.end())
            {
                 pSD=(StationDistance*)GET_PTFROMIT(findit);
                 pSPR->nDistance=pSD->nDistance;
                 pSPR->nArrStnIndex=pSD->nArrivalStn;
                 pSPR->nDepStnIndex=pSD->nDepartStn;
            }
           
			findit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPR->nDepStnIndex));
			if (findit != pTM->m_vStationInformation.end())
			{
				StationInformation *pSI = (StationInformation*)GET_PTFROMIT(findit);
				nDepStnCode = pSI->nStationCode;
				nDepStnCodeAlt = pSI->nProp[0];
			}
			findit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPR->nArrStnIndex));
			if (findit != pTM->m_vStationInformation.end())
			{
				StationInformation *pSI = (StationInformation*)GET_PTFROMIT(findit);
				nArrStnCode = pSI->nStationCode;
				nArrStnCodeAlt = pSI->nProp[0];
			}



			redunit=find_if(pSPH->m_vChildItem.at(0).vSQLData.begin(), pSPH->m_vChildItem.at(0).vSQLData.end(), findRedundentStopPtnRoutesByStationCode(nDepStnCode, nArrStnCode));
			if (redunit != pSPH->m_vChildItem.at(0).vSQLData.end())
			{
				StopPtnRoutes *pSPRRedun = (StopPtnRoutes*)GET_PTFROMIT(redunit);
				pSPR->nBaseDistance = pSPRRedun->nBaseDistance;
			}
			else
			{
				pSPR->nBaseDistance = nWholeDist;
			}


			pSPR->nDepStnCode = nDepStnCode;
			pSPR->nDepStnCodeAlt = nDepStnCodeAlt;
			pSPR->nArrStnCode = nArrStnCode;
			pSPR->nArrStnCodeAlt = nArrStnCodeAlt;
			


			pSPR->nRouteDistance = pSPR->nBaseDistance + pSPR->nDistance;

			for (evtit = pSPR->m_vChildItem[0].vSQLData.begin(); evtit != pSPR->m_vChildItem[0].vSQLData.end(); evtit++)
			{
				EventLists *pEL = (EventLists*)GET_PTFROMIT(evtit);
				switch (pEL->nDistanceType)
				{
				case 0:
					pEL->nRouteDistance = pEL->nDistance + pSPR->nBaseDistance;
					pEL->nAppRouteDistance = pEL->nRouteDistance;
					//printf("Dep %d Arr %d,nAppRouteDistance %d\n", pSPR->nDepStnCode, pSPR->nArrStnCode, pEL->nAppRouteDistance);
					break;
				case 1:
					pEL->nRouteDistance = (pSPR->nDistance - pEL->nDistance) + pSPR->nBaseDistance;
					pEL->nAppRouteDistance = pEL->nRouteDistance;
					//printf("Dep %d Arr %d,nAppRouteDistance %d\n", pSPR->nDepStnCode, pSPR->nArrStnCode, pEL->nAppRouteDistance);
					break;
				}    
				//qDebug("%s %d Route Dist %d Dist %d\n",pSPH->szDesc,std::distance(pSPR->m_vChildItem[0].vSQLData.begin(),evtit),pEL->nRouteDistance,pEL->nDistance);
			}
			nWholeDist = pSPR->nBaseDistance + pSPR->nDistance;
			printf("Dep %d(%d) Arr %d(%d),nWholeDist %d\n", pSPR->nDepStnCode, pSPR->nDepStnCodeAlt, pSPR->nArrStnCode, pSPR->nArrStnCodeAlt,nWholeDist);
			//nWholeDist += pSPR->nDistance;
        }
    }
}

bool COperManage::SetRouteByDepArrCode(int nDepartueStationCode,int nArrivalStationCode,bool bForce)
{
	int nDepartureStationIndex=0,nArrivalStationIndex=0;
	CTableManage *pTM=CTableManage::GetInstance();
	std::vector<SHARED_PTRC(CSQLData)>::iterator depit=find_if(pTM->m_vStationInformation.begin(),pTM->m_vStationInformation.end(),findStationInformationByCode(nDepartueStationCode));
	
    printf("COperManage::SetRouteByDepArrCode(%d,%d, %d)\n", nDepartueStationCode, nArrivalStationCode, bForce);
    
    if(depit!=pTM->m_vStationInformation.end())
	{
		nDepartureStationIndex=(*depit)->GetIndex();
	}

	std::vector<SHARED_PTRC(CSQLData)>::iterator arrit=find_if(pTM->m_vStationInformation.begin(),pTM->m_vStationInformation.end(),findStationInformationByCode(nArrivalStationCode));
	if(arrit!=pTM->m_vStationInformation.end())
	{
		nArrivalStationIndex=(*arrit)->GetIndex();
	}
	
	std::vector<SHARED_PTRC(CSQLData)>::iterator spit=find_if(pTM->m_vStopPtnHeader.begin(),pTM->m_vStopPtnHeader.end(),findStopPtnRoutesByDepartureAndArrivalStationIndex(nDepartureStationIndex,nArrivalStationIndex));
	if(spit!=pTM->m_vStopPtnHeader.end())
	{
		return SetStopPatternByStopPtnHeader((*spit));
	}
	return false;
}

bool COperManage::SetRouteByArrCode(int nArrivalStationCode, bool bForce)
{
	int nArrivalStationIndex = 0;
	CTableManage *pTM = CTableManage::GetInstance();
	std::vector<SHARED_PTRC(CSQLData)>::iterator arrit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findStationInformationByCode(nArrivalStationCode));
	if (arrit != pTM->m_vStationInformation.end())
	{
		nArrivalStationIndex = (*arrit)->GetIndex();
	}


	std::vector<SHARED_PTRC(CSQLData)>::iterator spit = find_if(pTM->m_vStopPtnHeader.begin(), pTM->m_vStopPtnHeader.end(), findStopPtnRoutesByArrivalStationIndex(nArrivalStationIndex));
	if (spit != pTM->m_vStopPtnHeader.end())
	{
		return SetStopPatternByStopPtnHeader((*spit));
	}
	return false;
}

bool COperManage::SetRouteByDestinationCode(int nDestinationCode)
{
	CTableManage *pTM = CTableManage::GetInstance();
	
    printf("COperManage::SetRouteByDestinationCode(%d)\n", nDestinationCode);
    
    std::vector<SHARED_PTRC(CSQLData)>::iterator it=find_if(pTM->m_vDestinationInformation.begin(), pTM->m_vDestinationInformation.end(), findDestinationInformationByCode(nDestinationCode));
	if (it != pTM->m_vDestinationInformation.end())
	{
		int nDestinationIndex=(*it)->GetIndex();
		auto itTrainNum=find_if(pTM->m_vTrainNumber.begin(), pTM->m_vTrainNumber.end(), findTrainNumberByDestinationIndex(nDestinationIndex));
		if (itTrainNum != pTM->m_vTrainNumber.end())
		{
			TrainNumber *pTN = (TrainNumber*)itTrainNum->get();
			return SetStopPatternByIndex(pTN->nStopPatternIndex);
		}
	}
    
	return false;
}


int COperManage::IsRouteExistDestAndNextCode(int nDestinationCode, int nNextStnCode)
{
	CTableManage *pTM = CTableManage::GetInstance();
	std::vector<SHARED_PTRC(CSQLData)>::iterator it = find_if(pTM->m_vDestinationInformation.begin(), pTM->m_vDestinationInformation.end(), findDestinationInformationByCode(nDestinationCode));
	if (it != pTM->m_vDestinationInformation.end())
	{
		int nDestinationIndex = (*it)->GetIndex();
		auto itTrainNum = pTM->m_vTrainNumber.begin();
		while (itTrainNum != pTM->m_vTrainNumber.end())
		{
			itTrainNum = find_if(itTrainNum, pTM->m_vTrainNumber.end(), findTrainNumberByDestinationIndex(nDestinationIndex));
			if (itTrainNum != pTM->m_vTrainNumber.end())
			{
				TrainNumber *pTN = (TrainNumber*)itTrainNum->get();
				std::vector<SHARED_PTRC(CSQLData)>::iterator spit = find_if(pTM->m_vStopPtnHeader.begin(), pTM->m_vStopPtnHeader.end(), findSQLData(pTN->nStopPatternIndex));
				if (spit != pTM->m_vStopPtnHeader.end())
				{

					std::vector<SHARED_PTRC(CSQLData)>::iterator auit = find_if(spit->get()->m_vChildItem[0].vSQLData.begin(), spit->get()->m_vChildItem[0].vSQLData.end(), findStopPtnRoutesByCode(nNextStnCode, false));
					if (auit != spit->get()->m_vChildItem[0].vSQLData.end())
					{
						return spit->get()->GetIndex();
					}
				}
				itTrainNum++;
			}
		}
	}
	return 0;
}

bool COperManage::SetStopPatternByStopPtnHeader(SHARED_PTRC(CSQLData) &pData)
{
	bool bRet = false;
	int nDepartureStationCode = 0;
	CTableManage *pTM = CTableManage::GetInstance();
	return SetStopPatternByIndex(pData->GetIndex());
}



bool COperManage::SetStopPattern(SHARED_PTRC(CSQLData) &pData)
{
    bool bRet=false;
    TrainNumber *pTN=(TrainNumber*)GET_PTFROMSP(pData);
    int nDepartureStationCode=0;
    CTableManage *pTM=CTableManage::GetInstance();

    printf("COperManage::SetStopPattern\n");

	return SetStopPatternByIndex(pTN->nStopPatternIndex);
}


bool COperManage::SetStopPatternByIndex(int nIndex)
{
	bool bRet = false;
	int nDepartureStationCode = 0;
	CTableManage *pTM = CTableManage::GetInstance();

    printf("COperManage::SetStopPatternByIndex(%d)\n", nIndex);

	std::vector<SHARED_PTRC(CSQLData)>::iterator spit = find_if(pTM->m_vStopPtnHeader.begin(), pTM->m_vStopPtnHeader.end(), findSQLData(nIndex));
	if (spit != pTM->m_vStopPtnHeader.end())
	{
		bRet = true;
		CloneStopPtnRoutes(*spit);
		StopPtnHeader *pSPH = (StopPtnHeader*)GET_PTFROMIT(spit);

		m_nStopPtnIdx = nIndex;
		m_nLineMapIdx = pSPH->nRelatedLineMapIndex;

        // 종착역
		std::vector<SHARED_PTRC(CSQLData)>::iterator snit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPH->nArrivalStnIndex));
		if (snit != pTM->m_vStationInformation.end())
		{
			StationInformation *pSI = (StationInformation*)GET_PTFROMIT(snit);
            std::string str = UnicodeToUTF8(pSI->szStationName[0]);
			m_nDstStnCode = pSI->nStationCode;
            strcpy(m_szDstStnUTF8, str.c_str());
		}
		else
		{
			m_nDstStnCode = 0;
            memset(m_szDstStnUTF8, 0, sizeof(m_szDstStnUTF8));
		}

        // 출발역
		snit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPH->nDepartStnIndex));
		if (snit != pTM->m_vStationInformation.end())
		{
			StationInformation *pSI = (StationInformation*)GET_PTFROMIT(snit);
            std::string str = UnicodeToUTF8(pSI->szStationName[0]);
			nDepartureStationCode = pSI->nStationCode;
            strcpy(m_szDepStnUTF8, str.c_str());
            
		}
		else
		{
            m_nDepStnCode = 0;
            memset(m_szDepStnUTF8, 0, sizeof(m_szDepStnUTF8));
			/*if(GET_PTFROMIT(spit)->m_vChildItem[0].vSQLData.size())
			{
				StopPtnRoutes *pRoutes=(StopPtnRoutes*)GET_PTFROMIT(GET_PTFROMIT(spit)->m_vChildItem[0].vSQLData.begin());
				nDepartureStationCode=pRoutes->nDepStnCode;
			}*/
		}
		while (!m_vSkippedStation[OM_NORMAL].empty())
			m_vSkippedStation[OM_NORMAL].clear();
		while (!m_vSkippedStation[OM_SIMULATION].empty())
			m_vSkippedStation[OM_SIMULATION].clear();
		while (!m_vSkippedStation[OM_CORRECTION].empty())
			m_vSkippedStation[OM_CORRECTION].clear();

		GoToStation(nDepartureStationCode, true, false, OM_CORRECTION);
		GoToStation(nDepartureStationCode, true, false, OM_SIMULATION);
		GoToStation(nDepartureStationCode, true, false, OM_NORMAL);

		for (int iMode = 0; iMode < OM_NULL; iMode++)
		{
			SetOperationMode((OPERATION_MODE)iMode);
		}
		SetOperationMode(OM_NORMAL);

        // 출발역 
        m_nDepStnCode = nDepartureStationCode;
        printf("nDepartureStationCode:%d, m_nDstStnCode:%d\n", nDepartureStationCode, m_nDstStnCode);
	}

    

	return bRet;
}

bool COperManage::SetTrainNumber(const TYC *pszTrainNumPrefix,const TYC *pszTrainNum,bool bForce)
{
    bool bRet=false;
    CTableManage *pTM=CTableManage::GetInstance();
    std::vector<SHARED_PTRC(CSQLData)>::iterator vit=find_if(pTM->m_vTrainNumber.begin(),pTM->m_vTrainNumber.end(),findTrainNumberByPrefix(pszTrainNumPrefix,pszTrainNum));

    printf("COperManage::SetTrainNumber(%s, %s, %d)\n", pszTrainNumPrefix, pszTrainNum, bForce);        

    if(vit!=pTM->m_vTrainNumber.end())
    {
        TrainNumber *pTN=(TrainNumber*)GET_PTFROMIT(vit);
        std::vector<SHARED_PTRC(CSQLData)>::iterator deit=find_if(pTM->m_vDestinationInformation.begin(),pTM->m_vDestinationInformation.end(),findSQLData(pTN->nDestinationIndex));
        if(deit!=pTM->m_vDestinationInformation.end())
        {
            DestinationInformation *pDI=(DestinationInformation*)GET_PTFROMIT(deit);
            m_pDestination=pDI->szDestinationName[0];
			
			
			std::string strTemp= UnicodeToUTF8(m_pDestination);
			printf("Destination %s",strTemp.c_str());
			strcpy(m_szDestinationUTF8, strTemp.c_str());
        }
        else
		{
			memset(m_szDestinationUTF8,0,sizeof(m_szDestinationUTF8));
            m_pDestination=NULL;
		}
        STRCPY(m_szTrainNumber,pszTrainNum);

        bRet=SetStopPattern((*vit));
        if(bRet)
        {
            STRCPY(m_szTrainNumberPrefix,pszTrainNumPrefix);
            STRCPY(m_szTrainNumber,pszTrainNum);
        }
    }
    return bRet;
}

bool COperManage::SetTrainNumber(const TYC *pszTrainNum,bool bForce,int *pTNIndex)
{
    bool bRet=false;
    CTableManage *pTM=CTableManage::GetInstance();	
    std::vector<SHARED_PTRC(CSQLData)>::iterator vit=find_if(pTM->m_vTrainNumber.begin(),pTM->m_vTrainNumber.end(),findTrainNumber(pszTrainNum));

    printf("COperManage::SetTrainNumber(%s, %d)\n", pszTrainNum, bForce);

    //220623 KEJ 열차번호 저장
    strcpy(m_szTrainNumber, pszTrainNum);
    // if(vit!=pTM->m_vTrainNumber.end())
    // {
    //     TrainNumber *pTN=(TrainNumber*)GET_PTFROMIT(vit);
	// 	if(pTNIndex)
	// 		(*pTNIndex)=pTN->GetIndex();

    //     std::vector<SHARED_PTRC(CSQLData)>::iterator deit=find_if(pTM->m_vDestinationInformation.begin(),pTM->m_vDestinationInformation.end(),findSQLData(pTN->nDestinationIndex));
    //     if(deit!=pTM->m_vDestinationInformation.end())
    //     {
    //         DestinationInformation *pDI=(DestinationInformation*)GET_PTFROMIT(deit);
    //         m_pDestination=pDI->szDestinationName[0];
    //     }
    //     else
    //         m_pDestination=NULL;
    //     STRCPY(m_szTrainNumber,pszTrainNum);
    //     bRet=SetStopPattern((*vit));
    //     if(bRet)
    //     {
    //         STRCPY(m_szTrainNumber,pszTrainNum);
    //     }
    // }

    printf("m_szTrainNumber:%s\n", m_szTrainNumber);
    return bRet;
}


void COperManage::SetVideoClearEvent()
{
    CTableManage *pTM=CTableManage::GetInstance();
    std::vector<SHARED_PTRC(CSQLData)>::iterator viit;
    viit=find_if(pTM->m_vPIDContents.begin(),pTM->m_vPIDContents.end(),findPIDContentsByMsgCode(1));
    if(viit!=pTM->m_vPIDContents.end())
    {
       m_nDRMEventIdx=GET_PTFROMIT(viit)->GetIndex();
       m_nPIDEventIdx=GET_PTFROMIT(viit)->GetIndex();
    }
}

void COperManage::SetAudioClearEvent()
{
    CTableManage *pTM=CTableManage::GetInstance();
    std::vector<SHARED_PTRC(CSQLData)>::iterator viit;
    viit=find_if(pTM->m_vAudioContents.begin(),pTM->m_vAudioContents.end(),findAudioContentsByMsgCode(1));
    if(viit!=pTM->m_vAudioContents.end())
    {
		m_nAudioEventIdx=GET_PTFROMIT(viit)->GetIndex();
    }
}



OPERATION_MODE COperManage::SetOperationMode(OPERATION_MODE eMode)
{
    if((eMode<OM_NULL)&&(eMode>=OM_NORMAL))
    {
        m_eOperMode=eMode;
    }
    return m_eOperMode;
}


OPERATION_MODE COperManage::GetOperationMode()
{
	return m_eOperMode;
}

void COperManage::CloneStopPtnRoutes(SHARED_PTRC(CSQLData) &pData)
{    
    std::vector<SHARED_PTRC(CSQLData)>::iterator sit,tit,fit;
	CTableManage *pTM = CTableManage::GetInstance();
	StopPtnHeader *pR=(StopPtnHeader*)pData.get();
    for(int iMode=OM_NORMAL;iMode<OM_NULL;iMode++)
    {
		int nPrevIndex = 0;
		int nOrder = 1;
		int nRealOrder = 1;
		m_vStationInformation[iMode].clear();
		m_vStationInformationSkipped[iMode].clear();
		m_vStopRoutes[iMode].clear();
        for(sit=GET_PTFROMSP(pData)->m_vChildItem[0].vSQLData.begin();sit!=GET_PTFROMSP(pData)->m_vChildItem[0].vSQLData.end();sit++)
        {
            StopPtnRoutes *pSPR=(StopPtnRoutes *)GET_PTFROMIT(sit)->Clone();
			fit = find_if(m_vStopRoutes[iMode].begin(), m_vStopRoutes[iMode].end(), findRedundentStopPtnRoutesByStationCode(pSPR->nDepStnCode,pSPR->nArrStnCode));
			if (fit == m_vStopRoutes[iMode].end())
			{
				tit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPR->nDepStnIndex));
				if (tit != pTM->m_vStationInformation.end())
				{
					if (nPrevIndex != GET_PTFROMIT(tit)->GetIndex())
					{
						StationInformation *pSI = (StationInformation*)GET_PTFROMIT(tit)->Clone();
						pSI->nOrder = nOrder;
						m_vStationInformation[iMode].push_back(SHARED_PTRC(CSQLData)(pSI));
						nPrevIndex = GET_PTFROMIT(tit)->GetIndex();
						nOrder++;
					}
				}

				tit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSPR->nArrStnIndex));
				if (tit != pTM->m_vStationInformation.end())
				{
					if (nPrevIndex != GET_PTFROMIT(tit)->GetIndex())
					{
						StationInformation *pSI = (StationInformation*)GET_PTFROMIT(tit)->Clone();
						pSI->nOrder = nOrder;
						m_vStationInformation[iMode].push_back(SHARED_PTRC(CSQLData)(pSI));
						nPrevIndex = GET_PTFROMIT(tit)->GetIndex();
						nOrder++;
					}
				}
				pSPR->nRealOrder = nRealOrder;
				m_vStopRoutes[iMode].push_back(SHARED_PTRC(CSQLData)(pSPR));
				nRealOrder++;
			}
			else
			{
				StopPtnRoutes *pOrigSPR=(StopPtnRoutes *)fit->get();
				pSPR->nRealOrder = pOrigSPR->nRealOrder;
				pOrigSPR->pAltRoutes = SHARED_PTRC(CSQLData)(pSPR);
			}
        }
        m_vEvents[iMode].clear();
		m_vAltEvents[iMode].clear();
        for(sit=m_vStopRoutes[iMode].begin();sit!=m_vStopRoutes[iMode].end();sit++)
        {
			StopPtnRoutes *pSPR = (StopPtnRoutes *)sit->get();
			m_vEvents[iMode].insert(m_vEvents[iMode].end(), (*sit)->m_vChildItem[0].vSQLData.begin(), (*sit)->m_vChildItem[0].vSQLData.end());

			if (pSPR->pAltRoutes)
			{
				m_vAltEvents[iMode].insert(m_vAltEvents[iMode].end(), pSPR->pAltRoutes->m_vChildItem[0].vSQLData.begin(), pSPR->pAltRoutes->m_vChildItem[0].vSQLData.end());
			}
			else
				m_vAltEvents[iMode].insert(m_vAltEvents[iMode].end(), (*sit)->m_vChildItem[0].vSQLData.begin(), (*sit)->m_vChildItem[0].vSQLData.end());
        }
		
        std::sort(m_vEvents[iMode].begin(),m_vEvents[iMode].end(),EventRouteDistanceCompare());

		std::sort(m_vAltEvents[iMode].begin(), m_vAltEvents[iMode].end(), EventRouteDistanceCompare());
    }
}

bool COperManage::GoToStationBySelectedEvent(int nIndex, bool bCurMode, OPERATION_MODE eOperMode)
{
	bool bStn = false, bRet = false;
	std::vector<SHARED_PTRC(CSQLData)>::iterator sit,zit;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	sit=find_if(m_vEvents[eTmpMode].begin(), m_vEvents[eTmpMode].end(), findSQLData(nIndex));
	if (sit != m_vEvents[eTmpMode].end())
	{
		EventLists *pEventList = (EventLists *)sit->get();	
		zit=find_if(m_vStopRoutes[eTmpMode].begin(), m_vStopRoutes[eTmpMode].end(), findSQLData(pEventList->nParentIndex));
		if (zit != m_vStopRoutes[eTmpMode].end())
		{
			StopPtnRoutes *pStopRoutes = (StopPtnRoutes*)zit->get();
			pStopRoutes->nArrStnCode;
			GoToStation(pStopRoutes->nArrStnCode,false,bCurMode,eOperMode);
			SetDistance(pEventList->nDistance+1, false, bCurMode, eOperMode);
		}
		return true;
	}
	return false;
}



bool COperManage::SkipNextStation(bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bStn=false,bRet=false;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	int *pProcededDist=&m_nProceededDistance[eTmpMode];
	int nNexStn=m_uNexStn[eTmpMode];
	bRet=SkipStation(nNexStn,true,bCurMode,eOperMode);
	if(bRet)
		bStn=SetDistance((*pProcededDist),true,bCurMode,eOperMode);

	return bRet&bStn; 
}

bool COperManage::UnSkipFromLatest(bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bStn=false,bRet=false;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	int nTotalProceededDist=m_nTotalProceededDistance[eTmpMode];
	int *pBaseDist=&m_nBaseDistance[eTmpMode];
	std::vector<unsigned short> *pSkipped=&m_vSkippedStation[eTmpMode];
	if(!pSkipped->empty())
	{
		int nLastestStn=pSkipped->back();
		bRet=SkipStation(nLastestStn,false,bCurMode,eOperMode);
		if(bRet)
			bStn=SetRouteDistance(nTotalProceededDist,bCurMode,eOperMode);
		return bRet&bStn; 
	}
	return false;
}

bool COperManage::SetStationByDistanceIndex(int nStationIndex, bool bDepArr, bool bCurMode, OPERATION_MODE eOperMode)
{
	bool bRet = false;
	CTableManage *pTM = CTableManage::GetInstance();
	std::vector<SHARED_PTRC(CSQLData)>::iterator snit;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes = &m_vStopRoutes[eTmpMode];
	snit = find_if(pRoutes->begin(), pRoutes->end(), findSQLData(nStationIndex));
	if (snit != pRoutes->end())
	{
		StopPtnRoutes *pSPR = (StopPtnRoutes*)snit->get();
		if (bDepArr)
			bRet=GoToStation(pSPR->nDepStnCode, bDepArr, bCurMode, eOperMode);
		else
			bRet=GoToStation(pSPR->nArrStnCode, bDepArr, bCurMode, eOperMode);
	}

    printf("COperManage::SetStationByDistanceIndex(%d,%d,%d)\n", nStationIndex, bDepArr, bCurMode);

	return bRet;
}



bool COperManage::GoToStation(int nStationCode,bool bDepArr,bool bCurMode,OPERATION_MODE eOperMode)
{
    bool bRet=false;
    int nDepStnIdx,nArrStnIdx;
    int nBeginPoint,nEndPoint,nCurPoint,nDoorDirection=0,nDestLEDIndex=0;
    CTableManage *pTM=CTableManage::GetInstance();
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pEvents=&m_vEvents[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent=&m_vTempDistEvent[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)> *pTempTargetDistEvent = &m_vTempTargetDistEvent[eTmpMode];
    int *pBaseDist=&m_nBaseDistance[eTmpMode];
	int *pLimitDist=&m_nLimitDistance[eTmpMode];
	int **ppDistList=&(m_pDistanceList[eTmpMode]);
	int **ppTargetDistList = &(m_pTargetDistanceList[eTmpMode]);
	int *pProcededDist=&m_nProceededDistance[eTmpMode];
	int *pTotalProceededDist=&m_nTotalProceededDistance[eTmpMode];
	int nCorrectedStationCode = 0;

	m_nDoorOpenAudio = 0;
	m_nDoorOpenVideo = 0;
	m_nDoorOpenPIDDRM = 0;

	m_nDoorCloseVideo = 0;
	m_nDoorCloseVideo = 0;
	m_nDoorClosePIDDRM = 0;

	std::vector<SHARED_PTRC(CSQLData)>::reverse_iterator srrit;
    std::vector<SHARED_PTRC(CSQLData)>::iterator snit,srit,diit,deit,altit;
	bool bAltRoutes = false;


    printf("COperManage::GoToStation(%d,%d,%d)\n", nStationCode, bDepArr, bCurMode);

	if(pRoutes->size())
    {

		//TRUE �̸� �ù߿� ����
		diit = find_if(pRoutes->begin(), pRoutes->end(), findStopPtnRoutesByCode(nStationCode, bDepArr));
		if (diit == pRoutes->end())
		{
			diit = find_if(pRoutes->begin(), pRoutes->end(), findAltStopPtnRoutesByCode(nStationCode, bDepArr));
			if (diit != pRoutes->end())
			{
				bAltRoutes = true;
				//��ü ��ΰ� �ִ°���� (��õ 2ȣ��)
				altit=find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findStationInformationByCode(nStationCode));
				if (altit != pTM->m_vStationInformation.end())
				{
					StationInformation *pSI = (StationInformation*)GET_PTFROMIT(altit);
					nCorrectedStationCode=pSI->nProp[0];
				}
				pEvents= &m_vAltEvents[eTmpMode];
			}
		}
		else
		{
			bAltRoutes = false;
			nCorrectedStationCode = nStationCode;
		}

        if(diit!=pRoutes->end())
		{

            StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMIT(diit);

#ifdef OPERMANAGE_CALLBACK_FUNC

			if (eOperMode == OM_SIMULATION)
			{
				if (m_tRouteIndex)
					m_tRouteIndex(pSPR->GetIndex());
			}
#endif
			StopPtnRoutes *pAltSPR = (StopPtnRoutes*)pSPR->pAltRoutes.get();
            if(pSPR->bDepSkipped)
            {
               srrit=find_if((std::vector<SHARED_PTRC(CSQLData)>::reverse_iterator)diit,pRoutes->rend(),findValidStopPtnRoutes(true));
               if(srrit!=pRoutes->rend())
               {
                   StopPtnRoutes *pDepSPR=(StopPtnRoutes*)GET_PTFROMIT(srrit);
                   nDepStnIdx=pDepSPR->nDepStnIndex;
                   nBeginPoint=pDepSPR->nRouteDistance-pDepSPR->nDistance;
               }
               else
               {
                   nDepStnIdx=pSPR->nDepStnIndex;
                   nBeginPoint=pSPR->nRouteDistance-pSPR->nDistance;  
               }
            }
            else
            {
				nDestLEDIndex=pSPR->nDestIndex;
                nDepStnIdx=pSPR->nDepStnIndex;
                nBeginPoint=pSPR->nRouteDistance-pSPR->nDistance;
            }
            
		

            snit=find_if(pTM->m_vStationInformation.begin(),pTM->m_vStationInformation.end(),findSQLData(nDepStnIdx));
            if(snit!=pTM->m_vStationInformation.end())
            {
                StationInformation *pSI=(StationInformation*)GET_PTFROMIT(snit);
            
				std::string str = UnicodeToUTF8(pSI->szStationName[0]);
				strcpy(m_szCurStnUTF8[eTmpMode],str.c_str());


                m_uCurStn[eTmpMode]=pSI->nStationCode;
                m_pCurStn[eTmpMode]=pSI->szStationName[0];
            }
			else
			{
				m_pCurStn[eTmpMode] = 0;
				memset(m_szCurStnUTF8[eTmpMode], 0, sizeof(m_szCurStnUTF8[eTmpMode]));
			}
            
            
            if(pSPR->bArrSkipped)
            {
               srit=find_if(diit,pRoutes->end(),findValidStopPtnRoutes(false));
               if(srit!=pRoutes->end())
               {
                   StopPtnRoutes *pArrSPR=(StopPtnRoutes*)GET_PTFROMIT(srit);
                   nArrStnIdx=pArrSPR->nArrStnIndex;
                   nEndPoint=pArrSPR->nRouteDistance;
				   nDoorDirection = pArrSPR->nDoorDirection;
               }
               else
               {
                   nArrStnIdx=pSPR->nArrStnIndex;
                   nEndPoint=pSPR->nRouteDistance;
				   nDoorDirection = pSPR->nDoorDirection;
               }
            }
            else
            {
				
                nArrStnIdx=pSPR->nArrStnIndex;
                nEndPoint=pSPR->nRouteDistance;
				
				if (!bAltRoutes)
				{
					nDoorDirection = pSPR->nDoorDirection;

					m_nDoorOpenAudio = pSPR->nDOVoiceIndex;
					m_nDoorOpenVideo = pSPR->nDOVideoIndex;
					m_nDoorOpenPIDDRM = pSPR->nDODRMIndex;

					m_nDoorCloseVideo = pSPR->nDCVoiceIndex;
					m_nDoorCloseVideo = pSPR->nDCVideoIndex;
					m_nDoorClosePIDDRM = pSPR->nDCDRMIndex;
				}
				else
				{
					nDoorDirection = pAltSPR->nDoorDirection;

					m_nDoorOpenAudio = pAltSPR->nDOVoiceIndex;
					m_nDoorOpenVideo = pAltSPR->nDOVideoIndex;
					m_nDoorOpenPIDDRM = pAltSPR->nDODRMIndex;

					m_nDoorCloseVideo = pAltSPR->nDCVoiceIndex;
					m_nDoorCloseVideo = pAltSPR->nDCVideoIndex;
					m_nDoorClosePIDDRM = pAltSPR->nDCDRMIndex;
				}
				/*
				/*pSPR->nDCDRMIndex;
				pSPR->nDCVoiceIndex;
				pSPR->nDCVideoIndex;

				pSPR->nDODRMIndex;
				pSPR->nDOVoiceIndex;
				pSPR->nDOVideoIndex;*/
            }
			m_bArrivedAtNextStation = false;
            printf("********************************nArrStnIdx:%d\n", nArrStnIdx);
            snit=find_if(pTM->m_vStationInformation.begin(),pTM->m_vStationInformation.end(),findSQLData(nArrStnIdx));
            if(snit!=pTM->m_vStationInformation.end())
            {
                StationInformation *pSI=(StationInformation*)GET_PTFROMIT(snit);
     
				std::string str = UnicodeToUTF8(pSI->szStationName[0]);
				strcpy(m_szNexStnUTF8[eTmpMode], str.c_str());

                m_uNexStn[eTmpMode]=pSI->nStationCode;
                m_pNexStn[eTmpMode]=pSI->szStationName[0];
            }
			else
			{
				m_pNexStn[eTmpMode] = 0;
				memset(m_szNexStnUTF8[eTmpMode], 0, sizeof(m_szNexStnUTF8[eTmpMode]));
			}

        }
		else
			return false;
		if(eOperMode!=OM_CORRECTION)
		{
			m_nNexStnCode=m_uNexStn[eTmpMode];
			m_nCurStnCode=m_uCurStn[eTmpMode];
			m_nDstIndIdx=nDestLEDIndex;
		}
        pTempDistEvent->clear();
		pTempTargetDistEvent->clear();
        for(deit=pEvents->begin();deit!=pEvents->end();deit++)
        {
            EventLists *pEL=(EventLists*)GET_PTFROMIT(deit);
			switch (pEL->nDistanceType)
			{
			case 0:
				if ((nBeginPoint <= pEL->nRouteDistance) && (nEndPoint > pEL->nRouteDistance) && (!pEL->bRemoved))
				{
					pTempDistEvent->push_back((*deit));
				}
				break;
			case 1:
				if ((nBeginPoint <= pEL->nRouteDistance) && (nEndPoint > pEL->nRouteDistance) && (!pEL->bRemoved))
				{
					pTempTargetDistEvent->push_back((*deit));
				}
				break;
			}
        }

#ifdef	OPERMANAGE_CALLBACK_FUNC
		
		if (m_tFuncDoorDir&&nDoorDirection)
			m_tFuncDoorDir(nDoorDirection);
		if (m_tDestInd&&nDestLEDIndex)
			m_tDestInd(nDestLEDIndex);
#endif
        if((*ppDistList))
            free(*ppDistList);
		if ((*ppTargetDistList))
			free(*ppTargetDistList);

        int nDistList=0;
        (*ppDistList)=(int*)malloc((pTempDistEvent->size()+1)*sizeof(int));
        for(snit=pTempDistEvent->begin();snit!=pTempDistEvent->end();snit++)
        {
            EventLists *pDE=(EventLists*)GET_PTFROMIT(snit);
            (*ppDistList)[nDistList++]=pDE->nAppRouteDistance-nBeginPoint;
        }

		int nTargetDistList = 0;
		(*ppTargetDistList) = (int*)malloc((pTempTargetDistEvent->size() + 1) * sizeof(int));
		for (snit = pTempTargetDistEvent->begin(); snit != pTempTargetDistEvent->end(); snit++)
		{
			EventLists *pDE = (EventLists*)GET_PTFROMIT(snit);
			(*ppTargetDistList)[nTargetDistList++] = pDE->nAppRouteDistance - nBeginPoint;
		}

	
        (*ppDistList)[nDistList]=-1;
		(*ppTargetDistList)[nTargetDistList] = -1;
        (*pBaseDist)=nBeginPoint;
        (*pProcededDist)=0;
        (*pLimitDist)=nEndPoint-nBeginPoint;
		(*pTotalProceededDist)=(*pBaseDist)+(*pProcededDist);
		//Corrected Station Code ���� Next Station Code �� ����
		int nOrder = SetStationOrder(m_nNexStnCode, eTmpMode);
		if (nOrder != -1)
		{
			m_nStationOrder[eOperMode] = nOrder;
		}
        bRet=true;
    }
    else
    {
        SetOperationMode(OM_NORMAL);
    }
    return bRet;
}


int COperManage::SetStationOrder(int nStationCode, OPERATION_MODE eOperMode)
{
	int nRet;
	std::vector<SHARED_PTRC(CSQLData)>::iterator snit;
	snit = find_if(m_vStationInformation[eOperMode].begin(), m_vStationInformation[eOperMode].end(), findStationInformationByCode(nStationCode));
	if (snit != m_vStationInformation[eOperMode].end())
	{
		nRet=std::distance(m_vStationInformation[eOperMode].begin(), snit);
		return nRet;
	}
	return -1;
}

int COperManage::GetStationCodeByOrder(int nOrder, OPERATION_MODE eOperMode)
{
	if ((m_nStationOrder[eOperMode]+nOrder < m_vStationInformation[eOperMode].size())&&
		((m_nStationOrder[eOperMode] + nOrder)>-1))
	{
		return ((StationInformation*)m_vStationInformation[eOperMode].at(m_nStationOrder[eOperMode] + nOrder).get())->nStationCode;
	}
	return 0;
}

bool COperManage::SetNextStation(int nNexStationCode, bool bCurMode, OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes = &m_vStopRoutes[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)>::iterator depit, diit, arrit;
	int nDepOrder = 0, nArrOrder = 0;
	int nCurStationCode = 0;
	arrit = find_if(pRoutes->begin(), pRoutes->end(), findStopPtnRoutesByCode(nNexStationCode, false));
	if (arrit != pRoutes->end())
	{
		nArrOrder = std::distance(pRoutes->begin(), arrit);
		StopPtnRoutes *pSPR=(StopPtnRoutes *)arrit->get();
		nCurStationCode=pSPR->nDepStnCode;
		return SetStations(nCurStationCode, nNexStationCode, bCurMode, eOperMode);
	}
	return false;
}

bool COperManage::SetStations(int nCurStationCode,int nNexStationCode,bool bCurMode,OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pEvents=&m_vEvents[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent=&m_vTempDistEvent[eTmpMode];

	int nDepOrder=0,nArrOrder=0;
	bool bDep=false,bArr=false;
	std::vector<SHARED_PTRC(CSQLData)>::iterator depit,diit,arrit;
	depit=find_if(pRoutes->begin(),pRoutes->end(),findStopPtnRoutesByCode(nCurStationCode,true));
	if(depit!=pRoutes->end())
	{
		bDep=true;
		nDepOrder=std::distance(pRoutes->begin(),depit);
	}
	arrit=find_if(pRoutes->begin(),pRoutes->end(),findStopPtnRoutesByCode(nNexStationCode,false));
	if(arrit!=pRoutes->end())
	{
		bArr=true;
		nArrOrder=std::distance(pRoutes->begin(),arrit);
	}

	bool bDepFind=false,bArrFind=false;

	if((bDep&&bArr)&&(nDepOrder<=nArrOrder))
	{
		RemoveFromSkipStations(nCurStationCode,bCurMode,eOperMode);
		RemoveFromSkipStations(nNexStationCode,bCurMode,eOperMode);

		for(diit=depit;diit!=pRoutes->end();diit++)
		{
			StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMIT(diit);
			if(pSPR->nDepStnCode==nCurStationCode)
			{
				bDepFind=true;
			}
			else if(bDepFind&&(!bArrFind))
			{
				AddToSkipStations(pSPR->nDepStnCode,bCurMode,eOperMode);
			}

			if(pSPR->nArrStnCode==nNexStationCode)
			{
				bArrFind=true;
				break;
			}
			else if(bDepFind&&(!bArrFind))
			{
				AddToSkipStations(pSPR->nArrStnCode,bCurMode,eOperMode);
			}
		}
		SetSkippedStations(bCurMode,false,eTmpMode);
		return GoToStation(nCurStationCode,true,bCurMode,eOperMode);
	}
	else
		return false;
}


bool COperManage::GoToNextStation(bool bCurMode,OPERATION_MODE eOperMode)
{
	printf("%s\n", __FUNCTION__);
	bool bRet=false;
	int nDepStnCode=0;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
	if(m_uNexStn[eTmpMode]==0)
	{
		StopPtnRoutes *pSR=(StopPtnRoutes*)GET_PTFROMIT(pRoutes->begin());
		nDepStnCode=pSR->nArrStnCode;
	}
	else
	{
		nDepStnCode=m_uNexStn[eTmpMode];
	}
	
	bRet=GoToStation(nDepStnCode,true,bCurMode,eOperMode);
	return bRet;

}

bool COperManage::GoToPrevStation(bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bRet=false;
	int nDepStnCode=0;
	bool bDepArr=false;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
	if(m_uCurStn[eTmpMode]==0)
	{
		StopPtnRoutes *pSR=(StopPtnRoutes*)GET_PTFROMIT(pRoutes->begin());
		nDepStnCode=pSR->nDepStnCode;
		bDepArr=true;
	}
	else
	{
		nDepStnCode=m_uCurStn[eTmpMode];
		bDepArr=false;
	}
	bRet=GoToStation(nDepStnCode,bDepArr,bCurMode,eOperMode);
	return bRet;
}


void COperManage::UpdateDistance(int nDistance,bool bCurMode,OPERATION_MODE eOperMode)
{
    int nDiff=0;
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
    int *pReceivedDistance=&m_nReceivedDistance[eTmpMode];
    if(nDistance)
    {
        nDiff=nDistance-(*pReceivedDistance);
        if(nDiff>0)           //Positive values accepted!
            AddDistance(nDiff);
    }
    (*pReceivedDistance)=nDistance;
}

bool COperManage::SetRouteDistance(int nDistance,bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bRet=false;
	bool bFirst=false;
	bool bFoundFirst=false;
	unsigned short uDepCode=0;
	int nLastRouteDistance=0;
	int nPartialDistance=0;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<SHARED_PTRC(CSQLData)>::iterator xit;
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent=&m_vTempDistEvent[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)> *pEvents=&m_vEvents[eTmpMode];
    int *pProcededDist=&m_nProceededDistance[eTmpMode];
    int *pLimitDist=&m_nLimitDistance[eTmpMode];
    int *pBaseDist=&m_nBaseDistance[eTmpMode];
	int *pTotalProceededDist=&m_nTotalProceededDistance[eTmpMode];
	for(xit=pRoutes->begin();xit!=pRoutes->end();xit++)
	{
		StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMIT(xit);
		if(!pSPR->bDepSkipped)
		{
			uDepCode=pSPR->nDepStnCode;
		}
		if((nDistance<pSPR->nRouteDistance)&&(!bFoundFirst))
		{
			bFoundFirst=true;
			break;
		}

		if(!pSPR->bArrSkipped)
			nLastRouteDistance=pSPR->nRouteDistance;
	}
	nPartialDistance=nDistance-nLastRouteDistance;
	bRet=GoToStation(uDepCode,true,bCurMode,eOperMode);
	if(bRet)
	{
		bRet=SetDistance(nPartialDistance,true,bCurMode,eOperMode);
		return bRet;
	}
	return false;
}


bool COperManage::SetDistance(int nDistance,bool bReset,bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bRet=false;
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	int nLimitDistance=GetLimitDistance(false, eOperMode);
	if (nLimitDistance > nDistance)
	{
		if (abs(nLimitDistance - nDistance) < abs(m_nTolerance*2))
			m_bArrivedAtNextStation = true;
		else
			m_bArrivedAtNextStation = false;

		if (bReset)
			bRet = GoToStation(m_uCurStn[eTmpMode], true, bCurMode, eOperMode);
		//printf("Current Station %d\n", m_uCurStn[eTmpMode]);
		m_nProceededDistance[eTmpMode] = nDistance;
		m_nTotalProceededDistance[eTmpMode] = m_nBaseDistance[eTmpMode] + m_nProceededDistance[eTmpMode];
		SetEventByDistance(bCurMode, eTmpMode);
		return bRet;
	}
	else
		return false;
}

bool COperManage::SetTargetDistance(int nTargetDistance, bool bReset, bool bCurMode, OPERATION_MODE eOperMode)
{
	bool bRet = false;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	if (bReset)
		bRet = GoToStation(m_uCurStn[eTmpMode], true, bCurMode, eOperMode);
	m_nProceededTargetDistance[eTmpMode] = nTargetDistance;
	SetEventByTargetDistance(bCurMode, eTmpMode);
	return bRet;

}


bool COperManage::SetDistance(float fDistance,bool bReset,bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bRet=false;
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	int nDistance=(int)((float)m_nLimitDistance[eTmpMode]*fDistance);
	if(bReset)
		bRet=GoToStation(m_uCurStn[eTmpMode],true,bCurMode,eOperMode);
	m_nProceededDistance[eTmpMode]=nDistance;
	m_nTotalProceededDistance[eTmpMode]=m_nBaseDistance[eTmpMode]+m_nProceededDistance[eTmpMode];
    SetEventByDistance(bCurMode,eTmpMode);
	return bRet;
}

int COperManage::GetLimitDistance(bool bCurMode,OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	return m_nLimitDistance[eTmpMode];
}

int COperManage::GetProceededDistance(bool bCurMode, OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	return m_nProceededDistance[eTmpMode];
}


void COperManage::AddDistance(int nDiff,bool bCurMode,OPERATION_MODE eOperMode)
{
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
    int *pLimitDistance=&m_nLimitDistance[eTmpMode];
    int *pProceededDistance=&m_nProceededDistance[eTmpMode];
	int *pTotalProceededDistance=&m_nTotalProceededDistance[eTmpMode];
	int *pBaseDistance=&m_nBaseDistance[eTmpMode];
    (*pProceededDistance)+=nDiff;
	(*pTotalProceededDistance)=(*pProceededDistance)+(*pBaseDistance);
    if((*pLimitDistance)<(*pProceededDistance))
    {
        if(m_eOperMode==OM_SIMULATION)
        {
            if(!GoToNextStation(bCurMode,eOperMode))
            {
                (*pProceededDistance)=(*pLimitDistance);
				(*pTotalProceededDistance)=(*pProceededDistance)+(*pBaseDistance);
            }
        }
        else
        {
            (*pProceededDistance)=(*pLimitDistance);
			(*pTotalProceededDistance)=(*pProceededDistance)+(*pBaseDistance);
        }
    }
    else
    {
        SetEventByDistance();
    }
}
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

void COperManage::GetDistanceList(bool bCurMode, OPERATION_MODE eOperMode, std::vector<unsigned int> &vDistList)
{
	//TCHAR szBuffer[256];
	int nDistOffset = 0;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent = &m_vTempDistEvent[eTmpMode];
	for (std::vector<SHARED_PTRC(CSQLData)>::iterator it =pTempDistEvent->begin();it!=pTempDistEvent->end();it++)
	{
		EventLists *pDE = (EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
		vDistList.push_back(pDE->nDistance);
	}
}

void COperManage::GetTargetDistanceList(bool bCurMode, OPERATION_MODE eOperMode, std::vector<unsigned int> &vDistList)
{
	//TCHAR szBuffer[256];
	int nDistOffset = 0;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent = &m_vTempTargetDistEvent[eTmpMode];
	for (std::vector<SHARED_PTRC(CSQLData)>::iterator it = pTempDistEvent->begin(); it != pTempDistEvent->end(); it++)
	{
		EventLists *pDE = (EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
		vDistList.push_back(pDE->nDistance);
	}
}

void COperManage::SetEventByDistance(bool bCurMode,OPERATION_MODE eOperMode)
{
	//TCHAR szBuffer[256];
    int nDistOffset=0;
    OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
    std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent=&m_vTempDistEvent[eTmpMode];
    int *pProceededDistance=&m_nProceededDistance[eTmpMode];
    int *pBaseDistance=&m_nBaseDistance[eTmpMode];
    if(pTempDistEvent->size())
    {
        int nCurDistance=(*pBaseDistance)+(*pProceededDistance);
        EventLists *pDE=(EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
		//_stprintf(szBuffer, _T("Distance %d %d\n"), pDE->nAppRouteDistance,nCurDistance);
		//OutputDebugString(szBuffer);
        while(pDE->nAppRouteDistance<nCurDistance)
        {
            nDistOffset=pDE->nAppRouteDistance-nCurDistance;
            if(nDistOffset>=m_nTolerance)
            {
               m_nAudioEventIdx=pDE->nVoiceIndex;
               m_nPIDEventIdx=pDE->nVideoIndex;
               m_nDRMEventIdx=pDE->nDRMIndex;

#ifdef	OPERMANAGE_CALLBACK_FUNC
			   if (eOperMode == OM_SIMULATION)
			   {
				   if (m_tEventIndex)
					   m_tEventIndex(pDE->GetIndex());
			   }
			   if(m_tFuncDRM)
				   m_tFuncDRM(pDE->nDRMIndex);
			   if(m_tFuncPID)
				   m_tFuncPID(pDE->nVideoIndex);
			   if(m_tFuncVoice)
				   m_tFuncVoice(pDE->nVoiceIndex);
			   if (m_tPIIText)
				   m_tPIIText(pDE->nPIITextIndex);
#endif	
            }

            pTempDistEvent->erase(pTempDistEvent->begin());
            if(pTempDistEvent->size())
            {
                pDE=(EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
            }
            else
                break;
        }
    }
}



void COperManage::SetEventByTargetDistance(bool bCurMode, OPERATION_MODE eOperMode)
{
	int nDistOffset = 0;
	OPERATION_MODE eTmpMode = bCurMode ? m_eOperMode : eOperMode;
	std::vector<SHARED_PTRC(CSQLData)> *pTempDistEvent = &m_vTempTargetDistEvent[eTmpMode];
	int *pProceededDistance = &m_nProceededTargetDistance[eTmpMode];
	if (pTempDistEvent->size())
	{
		//qDebug()<<"TargetDistEvent "<<pTempDistEvent->size();
		int nCurDistance = (*pProceededDistance);
		EventLists *pDE = (EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
		while (pDE->nDistance > nCurDistance)
		{
			nDistOffset = pDE->nDistance - nCurDistance;
			if (nDistOffset < -m_nTolerance)
			{
				m_nAudioEventIdx = pDE->nVoiceIndex;
				m_nPIDEventIdx = pDE->nVideoIndex;
				m_nDRMEventIdx = pDE->nDRMIndex;
				//qDebug()<<"Temp TargetDist EventPopped";

#ifdef	OPERMANAGE_CALLBACK_FUNC
				m_tFuncDRM(pDE->nDRMIndex);
				m_tFuncPID(pDE->nVideoIndex);
				m_tFuncVoice(pDE->nVoiceIndex);
#endif

			}

			pTempDistEvent->erase(pTempDistEvent->begin());
			//qDebug()<<"Target Dist Event Deleted "<<pTempDistEvent->size();
			if (pTempDistEvent->size())
			{
				pDE = (EventLists*)GET_PTFROMIT(pTempDistEvent->begin());
			}
			else
				break;
		}
	}
}

void COperManage::SetImmediateEvent(bool bEvent)
{
	m_bImmediateEvent = bEvent;
}

bool COperManage::GetImmediateEvent()
{
	return m_bImmediateEvent;
}



bool COperManage::SkipStation(int nStationCode,bool bInsert,bool bCurMode,OPERATION_MODE eOperMode)
{
	bool bRet=false;
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	if(bInsert)
		bRet=AddToSkipStations(nStationCode,bCurMode,eOperMode);
	else
		bRet=RemoveFromSkipStations(nStationCode,bCurMode,eOperMode);
	if(!bRet)
		return false;

	SetSkippedStations(nStationCode,bCurMode,eTmpMode);

	if (m_bImmediateEvent)
	{
		while (m_vSkippedEvents[eTmpMode].size())
		{
			EventLists *pDE = (EventLists*)GET_PTFROMIT(m_vSkippedEvents[eTmpMode].begin());
			m_nAudioEventIdx = pDE->nVoiceIndex;
			m_nPIDEventIdx = pDE->nVideoIndex;
			m_nDRMEventIdx = pDE->nDRMIndex;
#ifdef	OPERMANAGE_CALLBACK_FUNC
			if (m_tFuncDRM)
				m_tFuncDRM(pDE->nDRMIndex);
			if (m_tFuncPID)
				m_tFuncPID(pDE->nVideoIndex);
			if (m_tFuncVoice)
				m_tFuncVoice(pDE->nVoiceIndex);
#endif
			m_vSkippedEvents[eTmpMode].erase(m_vSkippedEvents[eTmpMode].begin());
		}
	}
	return true;
}

void COperManage::SetSkippedStations(int nStationCode, bool bCurMode,OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	int nGap=0,nRouteDistance=0;
    int nAdjustedDistance=0;
	int nLatestStnCode=0;
	bool bAdjustGapApplying=false;
    std::vector<SHARED_PTRC(CSQLData)> *pEvents=&m_vEvents[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)> *pSkippedEvents = &m_vSkippedEvents[eTmpMode];

	std::vector<SHARED_PTRC(CSQLData)> *pAltEvents = &m_vAltEvents[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)> *pAltSkippedEvents = &m_vAltSkippedEvents[eTmpMode];

    std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
	std::vector<unsigned short> *pSkipped=&m_vSkippedStation[eTmpMode];
    std::vector<SHARED_PTRC(CSQLData)>::iterator kit,xit,fit,ssit;
	std::vector<SHARED_PTRC(CSQLData)>::reverse_iterator rit;
	std::vector<unsigned short>::iterator skit;
	pEvents->clear();
	pAltEvents->clear();
	pSkippedEvents->clear();
	pAltSkippedEvents->clear();
	//���� Alt������ ���� �Ϸ� �ʿ�!!!(20-04-10)
	int nBaseDistance=0;
	bool bDepSkippedFocusedStation = false;
	for(kit=pRoutes->begin();kit!=pRoutes->end();kit++)
	{
		StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMIT(kit);
		skit=std::find(pSkipped->begin(),pSkipped->end(),(unsigned short)pSPR->nArrStnCode);
		if(skit!=pSkipped->end())
			pSPR->bArrSkipped=true;
		else
			pSPR->bArrSkipped=false;

		skit=std::find(pSkipped->begin(),pSkipped->end(),(unsigned short)pSPR->nDepStnCode);
		if (skit != pSkipped->end())
			pSPR->bDepSkipped = true;
		else
			pSPR->bDepSkipped=false;
	
		if ((nStationCode == pSPR->nDepStnCode) && (pSPR->bDepSkipped))
			bDepSkippedFocusedStation = true;
		else
			bDepSkippedFocusedStation = false;


		nAdjustedDistance=0;
		for(xit=(*kit)->m_vChildItem[0].vSQLData.begin();xit!=(*kit)->m_vChildItem[0].vSQLData.end();xit++)
		{
			EventLists *pEL=(EventLists*)GET_PTFROMIT(xit);
			switch(pEL->nEventType)
			{
			case ET_DEPARTURE:
				pEL->nAppRouteDistance=pEL->nRouteDistance;
				if(!pSPR->bDepSkipped)
				{
					pEL->bRemoved=pSPR->bArrSkipped;
					nBaseDistance=pEL->nRouteDistance;	//Base Distance ��� 
				}
				else if(pSPR->bDepSkipped&&(!pSPR->bArrSkipped))	//Departure Skip only
				{
					if(!nAdjustedDistance)
					{
						nAdjustedDistance=pEL->nRouteDistance;
						nGap=0;
					}
					else
						nGap=pEL->nRouteDistance-nAdjustedDistance;
					pEL->nAppRouteDistance=nBaseDistance+nGap;	//Base Distance �� ������ġ
					if (bDepSkippedFocusedStation)
						pSkippedEvents->push_back(*xit);	//���
					pEL->bRemoved=false;
				}
				else if(pSPR->bDepSkipped&&pSPR->bArrSkipped)		//All Skip
				{
					pEL->bRemoved=true;
				}
			case ET_ARRIVAL:
				pEL->bRemoved=pSPR->bArrSkipped;
				break;
			}
			if (!pEL->bRemoved)
				pEvents->push_back(*xit);
		}
		nLatestStnCode=pSPR->nArrStnCode;
	}
	m_vStationInformation[eTmpMode].insert(m_vStationInformation[eTmpMode].end(),
		m_vStationInformationSkipped[eTmpMode].begin(), m_vStationInformationSkipped[eTmpMode].end());
	for (skit = pSkipped->begin(); skit != pSkipped->end(); skit++)
	{
		ssit=find_if(m_vStationInformation[eTmpMode].begin(), m_vStationInformation[eTmpMode].end(), findStationInformationByCode((*skit)));
		if (ssit != m_vStationInformation[eTmpMode].end())
		{
			m_vStationInformationSkipped[eTmpMode].push_back(*ssit);
			m_vStationInformation[eTmpMode].erase(ssit);
		}
	}

	std::sort(m_vStationInformation[eTmpMode].begin(), m_vStationInformation[eTmpMode].end(), StationInformationOrderCompare());
	std::sort(pEvents->begin(),pEvents->end(),EventAppRouteDistanceCompare());
}

bool COperManage::AddToSkipStations(int nStationCode,bool bCurMode,OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<unsigned short> *pSkipped=&m_vSkippedStation[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)>::reverse_iterator rit;
	std::vector<unsigned short>::iterator skit=std::find(pSkipped->begin(),pSkipped->end(),nStationCode);
	int nCount=std::count_if(pRoutes->begin(),pRoutes->end(),findStopPtnRoutesByStationCode(nStationCode));
	if(nCount<2)
		return false;
	if(skit==pSkipped->end())
	{
		pSkipped->push_back(nStationCode);
		return true;
	}
	return true;
}

bool COperManage::RemoveFromSkipStations(int nStationCode,bool bCurMode,OPERATION_MODE eOperMode)
{
	OPERATION_MODE eTmpMode=bCurMode?m_eOperMode:eOperMode;
	std::vector<unsigned short> *pSkipped=&m_vSkippedStation[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)> *pRoutes=&m_vStopRoutes[eTmpMode];
	std::vector<SHARED_PTRC(CSQLData)>::reverse_iterator rit;
	std::vector<unsigned short>::iterator skit=std::find(pSkipped->begin(),pSkipped->end(),nStationCode);
	int nCount=std::count_if(pRoutes->begin(),pRoutes->end(),findStopPtnRoutesByStationCode(nStationCode));
	if(nCount<2)
		return false;
	if(skit!=pSkipped->end())
	{
		pSkipped->erase(skit);
		return true;
	}
	return true;
}


void COperManage::InitMappedValue()
{
	m_mMappedValue[TSTRING(MAKE_T("DEST_STN_CODE"))]	= &m_nDstStnCode;
	m_mMappedValue[TSTRING(MAKE_T("CUR_STN_CODE"))]		= &m_nCurStnCode;
	m_mMappedValue[TSTRING(MAKE_T("NEX_STN_CODE"))]		= &m_nNexStnCode;
	m_mMappedValue[TSTRING(MAKE_T("DST_IND_IDX"))]		= &m_nDstIndIdx;
	m_mMappedValue[TSTRING(MAKE_T("AUDIO_EVENT_IDX"))]	= &m_nAudioEventIdx;
	m_mMappedValue[TSTRING(MAKE_T("PID_EVENT_IDX"))]	= &m_nPIDEventIdx;
	m_mMappedValue[TSTRING(MAKE_T("DRM_EVENT_IDX"))]	= &m_nDRMEventIdx;
	m_mMappedValue[TSTRING(MAKE_T("DOOR_DIRECTION"))]	= &m_nDoorDirection;
}


bool COperManage::GetMappedValue(const TYC *pTagName, SHARED_PTRC(CValueMonitor) *pVM)
{
	std::map<TSTRING, int *>::iterator it = m_mMappedValue.find(pTagName);
	if (it != m_mMappedValue.end())
	{
		(*pVM) = SHARED_PTRC(CValueMonitor)(new CIntValueMonitor());
		(*pVM)->setValue(it->second);
		return true;
	}
	return false;
}

void COperManage::SetTolerance(int nTolerance)
{
	m_nTolerance=nTolerance;
}

// Get Departure Station Code
int COperManage::GetDepatStationCode()
{
    return m_nDepStnCode;                           //Departure station code

}

//Get Current Station Code;
int COperManage::GetThisStationCode()
{
	return m_nCurStnCode;							//current station code
}
//Get Next Staiton Code
int COperManage::GetNextStationCode()
{
    return m_nNexStnCode;							//next station code
}


int COperManage::GetDestStationCode()
{
	return m_nDstStnCode;
}

int COperManage::GetStopPatternIndex()
{
	return m_nStopPtnIdx;
}

int COperManage::GetLineMapIndex()
{
	return m_nLineMapIdx;
}

#ifdef _WIN32
void COperManage::SaveStopPatternAsCSV()
{
	CTableManage *pTM = CTableManage::GetInstance();
	FILE *fp=_wfopen(L"StopPtn.csv", L"w");
	if (fp != NULL)
	{
		for (auto it = pTM->m_vStopPtnHeader.begin(); it != pTM->m_vStopPtnHeader.end(); it++)
		{
			StopPtnHeader *p = (StopPtnHeader*)it->get();
			fwprintf(fp, (wchar_t*)p->szDesc);
			fwprintf(fp, L",,\n");
			for (auto nit = p->m_vChildItem[0].vSQLData.begin(); nit != p->m_vChildItem[0].vSQLData.end(); nit++)
			{
				StopPtnRoutes *r = (StopPtnRoutes*)nit->get();
				auto xxit = find_if(pTM->m_vStationDistance.begin(), pTM->m_vStationDistance.end(), findSQLData(r->nDistIndex));
				if (xxit != pTM->m_vStationDistance.end())
				{
					int nArrCode=0, nDepCode = 0;
					StationDistance *pSD = (StationDistance*)xxit->get();
					auto xxxit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSD->nArrivalStn));
					if (xxxit != pTM->m_vStationInformation.end())
					{
						StationInformation *pSTN=(StationInformation*)xxxit->get();
						nArrCode = pSTN->nStationCode;
					}
					auto xxyit = find_if(pTM->m_vStationInformation.begin(), pTM->m_vStationInformation.end(), findSQLData(pSD->nDepartStn));
					if (xxyit != pTM->m_vStationInformation.end())
					{
						StationInformation *pSTN = (StationInformation*)xxyit->get();
						nDepCode = pSTN->nStationCode;
					}
					fwprintf(fp, L",[%d][%d] %s", nDepCode, nArrCode,(wchar_t*)pSD->szDesc);
					fwprintf(fp, L",\n");
				}
				for (auto nnit = r->m_vChildItem[0].vSQLData.begin(); nnit != r->m_vChildItem[0].vSQLData.end(); nnit++)
				{
					EventLists *pEV = (EventLists*)nnit->get();
					fwprintf(fp, L",,");
					fwprintf(fp, L"%d\n",pEV->nDistance);
				}
			}
		}
		fclose(fp);
	}
}
#endif

#ifdef	OPERMANAGE_CALLBACK_FUNC

void COperManage::setPIDCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncPID = tFunc;
}

void COperManage::setDRMCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncDRM = tFunc;
}

void COperManage::setVoiceCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncVoice = tFunc;
}

void COperManage::setDoorDirectionCallBack(std::function<bool(int)> &tFunc)
{
	m_tFuncDoorDir = tFunc;
}

void COperManage::setDoorOpenAudioCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncOpenAudio = tFunc;
}

void COperManage::setDoorOpenVideoCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncOpenVideo = tFunc;
}

void COperManage::setDoorOpenPIDDRMCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncOpenPIDDRM = tFunc;
}

void COperManage::setDoorCloseAudioCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncCloseAudio = tFunc;
}

void COperManage::setDoorCloseVideoCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncCloseVideo = tFunc;
}

void COperManage::setDoorClosePIDDRMCallback(std::function<bool(int)> &tFunc)
{
	m_tFuncClosePIDDRM = tFunc;
}


void COperManage::setDestIndCallback(std::function<bool(int)> &tFunc)
{
	m_tDestInd = tFunc;
}



void COperManage::setEventListIndexCallback(std::function<void(int)> &tFunc)
{
	m_tEventIndex = tFunc;
}

void COperManage::setRouteListIndexCallback(std::function<void(int)> &tFunc)
{
	m_tRouteIndex = tFunc;
}



bool COperManage::dummyCallback(int nIndex)
{
	return true;
}

void COperManage::setDoorOpen()
{
	if (m_bArrivedAtNextStation)
	{
		if (m_tFuncOpenAudio&&m_nDoorOpenAudio)
			m_tFuncOpenAudio(m_nDoorOpenAudio);
		if (m_tFuncOpenVideo&&m_nDoorOpenVideo)
			m_tFuncOpenVideo(m_nDoorOpenVideo);
		if (m_tFuncOpenPIDDRM&&m_nDoorOpenPIDDRM)
			m_tFuncOpenPIDDRM(m_nDoorOpenPIDDRM);
	}
}

void COperManage::setDoorClose()
{
	if (m_tFuncCloseAudio&&m_nDoorCloseAudio)
		m_tFuncCloseAudio(m_nDoorCloseAudio);
	if (m_tFuncCloseVideo&&m_nDoorCloseVideo)
		m_tFuncCloseVideo(m_nDoorCloseVideo);
	if (m_tFuncClosePIDDRM&&m_nDoorClosePIDDRM)
		m_tFuncClosePIDDRM(m_nDoorClosePIDDRM);

}
#endif


void COperManage::GetStopPtnHeaderTypeList(int nType)
{
	CTableManage *pTM = CTableManage::GetInstance();
	for (int iMode = 0; iMode < 3; iMode++)
	{
		m_vStopPatternHeader[iMode].clear();
		std::vector<SHARED_PTRC(CSQLData)>::iterator inner,innner,it = pTM->m_vTrainNumber.begin();
		while (it != pTM->m_vTrainNumber.end())
		{
			it = find_if(it, pTM->m_vTrainNumber.end(), findTrainNumberByDirection(nType));
			if (it != pTM->m_vTrainNumber.end())
			{
				TrainNumber *pTN = (TrainNumber*)it->get();
				inner=find_if(m_vStopPatternHeader[iMode].begin(), m_vStopPatternHeader[iMode].end(), findSQLData(pTN->nStopPatternIndex));
				if (inner == m_vStopPatternHeader[iMode].end())
				{
					innner=find_if(pTM->m_vStopPtnHeader.begin(), pTM->m_vStopPtnHeader.end(), findSQLData(pTN->nStopPatternIndex));
					if (innner != pTM->m_vStopPtnHeader.end())
					{
						m_vStopPatternHeader[iMode].push_back((*innner));
					}
				}
				it++;
			}
		}
	}
}


const char *COperManage::GetCurrentStationNameUTF8(OPERATION_MODE eOperMode)
{
	return (const char*)m_szDepStnUTF8;//;m_szCurStnUTF8[eOperMode];
}

const char *COperManage::GetDestinationNameUTF8(OPERATION_MODE eOperMode)
{
	return (const char*)m_szDstStnUTF8;//m_szDestinationUTF8;
}

const char *COperManage::GetNextStationNameUTF8(OPERATION_MODE eOperMode)
{
	return (const char*)m_szNexStnUTF8[eOperMode];
}

const char *COperManage::GetTrainNumber(int num)
{
    return (const char*)m_szTrainNumber;
}