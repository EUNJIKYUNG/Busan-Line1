#ifndef MTXHANDLER2
#define MTXHANDLER2
#include "../commonsocket/CUDPUnicast.h"
#include "../CDataManage.h"
#include "../TablesDefine/TableManage.h"
#include "../ScheduleManage/ScheduleManage.h"
#include "../CMediaManage.h"
#ifdef VIEWER_SUPPORT
#include "../CViewerManage.h"
#endif
#include "../defineMTX.h"
#include <atomic>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#define ADDR_DEVMTX		113

class CMTXHandler2:
	public CUDPUnicast
{
public:
	CMTXHandler2(unsigned short wPort, int nPeriod)
		:CUDPUnicast(wPort, nPeriod), m_bVideoScheduleShifted(false), m_bAudioScheduleShifted(false),nStopSteamingSendingCount(0)
	{
		memset(&m_tCNCS2MTX, 0, sizeof(m_tCNCS2MTX));
		memset(&m_tMTX2CNCS, 0, sizeof(m_tMTX2CNCS));
		CMediaManage *pMM = CMediaManage::GetInstance();
		pMM->SetCmdStatusBuffer(&m_tCNCS2MTX, &m_tMTX2CNCS);

	}
	virtual ~CMTXHandler2();

	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDataManage *pDM = CDataManage::GetInstance();

		/*if ((GETCLASSA(pAddr) == 10) &&
			(GETCLASSB(pAddr) == 0) &&
			(GETCLASSC(pAddr) == 0))
		{
			switch (GETCLASSD(pAddr))
			{
			case ADDR_DEVMTX:*/
				handleMTX((MTX_2_CNCS*)pBuf, nSize, pAddr);
				//pDM->SetCurrentTick(DEV_LIST::DEV_MTX);
               // pDM->SetVersionString(DEV_LIST::DEV_MTX,&(((MTX_2_CNCS*)pBuf)->tVersion));
				/*break;
			default:
				break;
			}
		}*/
	}


	void handleMTX(MTX_2_CNCS *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDataManage *pDM = CDataManage::GetInstance();
		CScheduleManage *pSM = CScheduleManage::GetInstance();
		if (nSize == sizeof(MTX_2_CNCS))
		{
        //Audio Part
			memcpy(&m_tMTX2CNCS, pBuf, nSize);
            //m_tMTX2CNCS.chSWBuildDate;
            //m_tMTX2CNCS.chSWVersion;
#ifdef VIEWER_SUPPORT
			CViewerManage::UpdateViewer(2017);
#endif
		}
		
	}


	void handleToBeSent()
	{
	
		m_tSockAddr.sin_port=htons(30003);
		m_tSockAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
		m_tSockAddr.sin_family=AF_INET;
		CScheduleManage *pSM = CScheduleManage::GetInstance();
		CTableManage *pTM = CTableManage::GetInstance();
		CMediaManage *pMM = CMediaManage::GetInstance();
		CDataManage *pDM = CDataManage::GetInstance();

		m_tCNCS2MTX.uTop[1] = (0 >> 8) & 0xFF;
		m_tCNCS2MTX.uTop[0] = (0) & 0xFF;
		m_tCNCS2MTX.uBottom[1] = (1080 >> 8) & 0xFF;
		m_tCNCS2MTX.uBottom[0] = (1080) & 0xFF;
		m_tCNCS2MTX.uRight[1] = (1920 >> 8) & 0xFF;
		m_tCNCS2MTX.uRight[0] = (1920) & 0xFF;
		m_tCNCS2MTX.uLeft[1] = (0 >> 8) & 0xFF;
		m_tCNCS2MTX.uLeft[0] = (0) & 0xFF;


		/*m_tCNCS2MTX.uTop[1] = (pDM->m_nTop >> 8) & 0xFF;
		m_tCNCS2MTX.uTop[0] = (pDM->m_nTop) & 0xFF;
		m_tCNCS2MTX.uBottom[1] = (pDM->m_nBottom >> 8) & 0xFF;
		m_tCNCS2MTX.uBottom[0] = (pDM->m_nBottom) & 0xFF;
		m_tCNCS2MTX.uRight[1] = (pDM->m_nRight >> 8) & 0xFF;
		m_tCNCS2MTX.uRight[0] = (pDM->m_nRight) & 0xFF;
		m_tCNCS2MTX.uLeft[1] = (pDM->m_nLeft >> 8) & 0xFF;
		m_tCNCS2MTX.uLeft[0] = (pDM->m_nLeft) & 0xFF;*/
//printf("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
        pMM->TaskRoutine();
//printf("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
		//Audio Part

		


		int *pLat = (int*)m_tCNCS2MTX.uGPSLatitude;
		int *pLon = (int*)m_tCNCS2MTX.uGPSLongitude;


		/*(*pLon) = pDM->m_nLongitude;
		(*pLat) = pDM->m_nLatitude;*/



		pDM->SetCurrentTime(&m_tCNCS2MTX.tInfo);

		memcpy(m_uSendBuffer, &m_tCNCS2MTX, sizeof(m_tCNCS2MTX));
		send(m_uSendBuffer, sizeof(m_tCNCS2MTX), (sockaddr*)&m_tSockAddr, sizeof(m_tSockAddr));

        /*if(!pDM->GetStopStreaming())
        {
			//printf("sendingstream\n");
			m_tCNCS2MTX.bStopStreaming=pDM->GetStopStreaming();
			memcpy(m_uSendBuffer, &m_tCNCS2MTX, sizeof(m_tCNCS2MTX));
            send(m_uSendBuffer, sizeof(m_tCNCS2MTX),(sockaddr*)&m_tSockAddr,sizeof(m_tSockAddr));
        }
        else
        {
			//printf("stopStreamingmode\n");
			memcpy(m_uSendBuffer, &m_tCNCS2MTX, sizeof(m_tCNCS2MTX));
            if(!m_tCNCS2MTX.bStopStreaming)
            {
				nStopSteamingSendingCount=5;
            }
			if(nStopSteamingSendingCount>0)
			{
				nStopSteamingSendingCount--;
				m_tCNCS2MTX.bStopStreaming=pDM->GetStopStreaming();
				printf("%s stopStreaming : on %u",__FUNCTION__,m_tCNCS2MTX.bStopStreaming);
                send(m_uSendBuffer, sizeof(m_tCNCS2MTX),(sockaddr*)&m_tSockAddr,sizeof(m_tSockAddr));
			}
        }*/
#ifdef VIEWER_SUPPORT
		CViewerManage::UpdateViewer(1004);
#endif
	}

	sockaddr_in m_tSockAddr;
	char m_uSendBuffer[2048];

	static CNCS_2_MTX	m_tCNCS2MTX;
	static MTX_2_CNCS	m_tMTX2CNCS;


	std::atomic_bool m_bVideoScheduleShifted;
	std::atomic_bool m_bAudioScheduleShifted;
	int nStopSteamingSendingCount;
};



#endif
