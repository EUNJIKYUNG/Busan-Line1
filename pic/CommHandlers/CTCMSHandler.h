#pragma once
#include "../commonsocket/CUDPUnicast.h"
#include "../CDataManage.h"
#ifdef VIEWER_SUPPORT
#include "../CViewerManage.h"
#endif
#include "../TablesDefine/TableManage.h"
#include "../CDevIdentify.h"
#include "../defineStruct.h"
#include "../COperation.h"
#ifndef _WIN32
#include <arpa/inet.h>
#endif



class CTCMSHandler :
	public CUDPUnicast
{
public:
	CTCMSHandler(unsigned short wPort, int nPeriod)
		: CUDPUnicast(wPort, 0)
	{
		
	}

	virtual ~CTCMSHandler()
	{

	}



	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDevIdentify *pIdent = CDevIdentify::GetInstance();
		if ((GETCLASSA(pAddr) == 192) &&
			(GETCLASSB(pAddr) == 9))
		{
			if (nSize == sizeof(_TCMS2PIS))
			{
				_TCMS2PIS *pPIS = (_TCMS2PIS*)pBuf;
				COperation *pO = COperation::GetInstance();
				if ((!pO->GetOperationMode())&&(!pO->GetSimulationMode()))
				{	
					char szTrainNumber[5] = { 0 };
					szTrainNumber[0] = pPIS->OPERINFO.uTNX000 + 0x30;
					szTrainNumber[1] = pPIS->OPERINFO.uTN0X00 + 0x30;
					szTrainNumber[2] = pPIS->OPERINFO.uTN00X0 + 0x30;
					szTrainNumber[3] = pPIS->OPERINFO.uTN000X + 0x30;

					if (strcmp(m_szTrainNumber, szTrainNumber))
					{
						pO->SetTrainNumber(szTrainNumber);
					}
					strcpy(m_szTrainNumber,szTrainNumber);
					uint16_t uDistance = ((pPIS->OPERINFO.uDIST[0] << 8) & 0xFF00) | (pPIS->OPERINFO.uDIST[1] & 0x00FF);
					if (pPIS->OPERINFO.uNEXCODE != m_tPIS.OPERINFO.uNEXCODE)
					{
						pO->SetPIITextIndex(0);
						printf("Nex Code %u\n",pPIS->OPERINFO.uNEXCODE);
						pO->SetOperationNextStationCode(pPIS->OPERINFO.uNEXCODE, OM_NORMAL);
					}
					pO->SetDistance(uDistance, OM_NORMAL);
					pO->SetIDD1Information(OM_NORMAL);
					COperation *pOM = COperation::GetInstance();
					if (pPIS->INPUT.bHCR0)
						pOM->GetIDD1()->uCabDir = 1;
					if (pPIS->INPUT.bHCR1)
						pOM->GetIDD1()->uCabDir = 2;
					if (pPIS->INPUT.bDDN_L)
						pOM->GetIDD1()->uDoorDir = 1;
					if (pPIS->INPUT.bDDN_R)
						pOM->GetIDD1()->uDoorDir = 2;
					pOM->GetIDD1()->uCarCongestion[0] = pPIS->uASP[0];
					pOM->GetIDD1()->uCarCongestion[1] = pPIS->uASP[1];
					pOM->GetIDD1()->uCarCongestion[2] = pPIS->uASP[2];
					pOM->GetIDD1()->uCarCongestion[3] = pPIS->uASP[3];
					pOM->GetIDD1()->uCarCongestion[4] = pPIS->uASP[4];
					pOM->GetIDD1()->uCarCongestion[5] = pPIS->uASP[5];
					pOM->GetIDD1()->uCarCongestion[6] = pPIS->uASP[6];
					pOM->GetIDD1()->uCarCongestion[7] = pPIS->uASP[7];
					
					
					memcpy(&m_tPIS, pPIS, sizeof(_TCMS2PIS));
				}


				/*int nSendSize = sizeof(FDISDISENDHEAD) + uAddSize;
		memcpy(m_uSendBuffer, &m_tPIISend, nSendSize);
		m_tSockAddr.sin_family = AF_INET;
		m_tSockAddr.sin_addr.s_addr = inet_addr(m_chIPMultiAddr);
		m_tSockAddr.sin_port = htons(m_wPort);
		send(m_uSendBuffer, nSendSize, (sockaddr*)&m_tSockAddr, sizeof(m_tSockAddr));*/

#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(1003);
#endif
			}
			


		
		}
	}

	

	
	void handleToBeSent()	//Sending Data
	{

	}



	/*bool m_bMode;
	static FDISDISEND m_tPIISend;
	static LEDRECV m_tPIIRecv[7];

	uint8_t m_uDevID[7];
	uint8_t m_uID;
	std::mutex m_tMutex;;
	unsigned short m_uCRC[36];
	std::queue<uint32_t> m_qUpdate;
	std::vector<uint32_t> m_vRetry;
	std::atomic_bool m_bTextRetry;*/
	_TCMS2PIS m_tPIS;
	char m_szTrainNumber[5];
	sockaddr_in m_tSockAddr;
	char m_uSendBuffer[2048];
};





