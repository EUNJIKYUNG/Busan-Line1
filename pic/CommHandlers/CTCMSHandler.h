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

    // 220629 KEJ TCMS 통신 처리
    void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDevIdentify *pIdent = CDevIdentify::GetInstance();

        printf("CTCMSHandler::processReceived(%s)\n", pBuf);

		// if ((GETCLASSA(pAddr) == 192) &&
		// 	(GETCLASSB(pAddr) == 9))
		{
            printf("TCMS -----> %d/%dbytes\n", nSize, sizeof(_TCMS2PIS));
			if (nSize == sizeof(_TCMS2PIS))
			{
				_TCMS2PIS *pPIS = (_TCMS2PIS*)pBuf;
				COperation *pO = COperation::GetInstance();
                COperation *pOM = COperation::GetInstance();

                // 날짜 및 시간
                pOM->GetIDD1()->tInfo = pPIS->tInfo;

                // door, dcw, hcr...
                pOM->GetIDD1()->INPUT = pPIS->INPUT;

                // 자동모드인 경우
				if ((!pO->GetOperationMode())&&(!pO->GetSimulationMode()))
				{	
                    // 열차번호
					char szTrainNumber[5] = { 0 };
					szTrainNumber[0] = pPIS->OPERINFO.uTNX000 + 0x30;
					szTrainNumber[1] = pPIS->OPERINFO.uTN0X00 + 0x30;
					szTrainNumber[2] = pPIS->OPERINFO.uTN00X0 + 0x30;
					szTrainNumber[3] = pPIS->OPERINFO.uTN000X + 0x30;
                    pOM->GetIDD1()->OPERINFO.uTNX000 = pPIS->OPERINFO.uTNX000;
                    pOM->GetIDD1()->OPERINFO.uTN0X00 = pPIS->OPERINFO.uTN0X00;
                    pOM->GetIDD1()->OPERINFO.uTN00X0 = pPIS->OPERINFO.uTN00X0;
                    pOM->GetIDD1()->OPERINFO.uTN000X = pPIS->OPERINFO.uTN000X;
                    

					if (strcmp(m_szTrainNumber, szTrainNumber))
					{
						pO->SetTrainNumber(szTrainNumber);
					}
					strcpy(m_szTrainNumber,szTrainNumber);

                    // 출발역, 현재역, 다음역, 종착역
					uint16_t uDistance = ((pPIS->uDIST[0] << 8) & 0xFF00) | (pPIS->uDIST[1] & 0x00FF);
					if (pPIS->OPERINFO.uNEXCODE != m_tPIS.OPERINFO.uNEXCODE)
					{
						// pO->SetPIITextIndex(0);
						printf("Nex Code %u\n",pPIS->OPERINFO.uNEXCODE);
						pO->SetOperationNextStationCode(pPIS->OPERINFO.uNEXCODE, OM_NORMAL);
					}

                    pOM->GetIDD1()->OPERINFO.uCURCODE = pPIS->OPERINFO.uCURCODE; // 현재역
                    pOM->GetIDD1()->OPERINFO.uDSTCODE = pPIS->OPERINFO.uDSTCODE; // 종착역
                    pOM->GetIDD1()->OPERINFO.uNEXCODE = pPIS->OPERINFO.uNEXCODE; // 다음역
                    pOM->GetIDD1()->OPERINFO.uSTARTCODE = pPIS->OPERINFO.uSTARTCODE; // 출발역

                    // 역간 이동거리
                    pOM->GetIDD1()->uDIST[0] = pPIS->uDIST[0];
                    pOM->GetIDD1()->uDIST[1] = pPIS->uDIST[1];
                    
					pO->SetDistance(uDistance, OM_NORMAL);
					pO->SetIDD1Information(OM_NORMAL);

                    // 속도
                    pOM->GetIDD1()->uSpeed = pPIS->uSpeed;

                    // AS 압력
                    memcpy(pOM->GetIDD1()->uAsp, pPIS->uAsp, sizeof(pPIS->uAsp));

                    // 미세먼지 농도
                    memcpy(pOM->GetIDD1()->uDust, pPIS->uDust, sizeof(pPIS->uDust));

                    // 냉난방 상태
                    memcpy(pOM->GetIDD1()->uAircon, pPIS->uAircon, sizeof(pPIS->uAircon));

                    // 이산화탄소 농도
                    memcpy(pOM->GetIDD1()->uCo2, pPIS->uCo2, sizeof(pPIS->uCo2));

                    // 객실 습도
                    memcpy(pOM->GetIDD1()->uHumidity, pPIS->uCo2, sizeof(pPIS->uHumidity));

                    // 실외온도
                    pOM->GetIDD1()->uTempExt = pPIS->uTempExt;

                    // 차량전압
                    pOM->GetIDD1()->uCarVolt = pPIS->uCarVolt;
					
					// if (pPIS->INPUT.bHCR0)
					// 	pOM->GetIDD1()->uCabDir = 1;
					// if (pPIS->INPUT.bHCR1)
					// 	pOM->GetIDD1()->uCabDir = 2;
					// if (pPIS->INPUT.bDDN_L)
					// 	pOM->GetIDD1()->uDoorDir = 1;
					// if (pPIS->INPUT.bDDN_R)
					// 	pOM->GetIDD1()->uDoorDir = 2;
					// pOM->GetIDD1()->uCarCongestion[0] = pPIS->uAsp[0];
					// pOM->GetIDD1()->uCarCongestion[1] = pPIS->uAsp[1];
					// pOM->GetIDD1()->uCarCongestion[2] = pPIS->uAsp[2];
					// pOM->GetIDD1()->uCarCongestion[3] = pPIS->uAsp[3];
					// pOM->GetIDD1()->uCarCongestion[4] = pPIS->uAsp[4];
					// pOM->GetIDD1()->uCarCongestion[5] = pPIS->uAsp[5];
					// pOM->GetIDD1()->uCarCongestion[6] = pPIS->uAsp[6];
					// pOM->GetIDD1()->uCarCongestion[7] = pPIS->uAsp[7];
					
				}

                memcpy(&m_tPIS, pPIS, sizeof(_TCMS2PIS));

#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(1003);
#endif
			}
					
		}
	}


// 	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
// 	{
// 		CDevIdentify *pIdent = CDevIdentify::GetInstance();

//         printf("CTCMSHandler::processReceived(%s)\n", pBuf);

// 		if ((GETCLASSA(pAddr) == 192) &&
// 			(GETCLASSB(pAddr) == 9))
// 		{
// 			if (nSize == sizeof(_TCMS2PIS))
// 			{
// 				_TCMS2PIS *pPIS = (_TCMS2PIS*)pBuf;
// 				COperation *pO = COperation::GetInstance();
// 				if ((!pO->GetOperationMode())&&(!pO->GetSimulationMode()))
// 				{	
// 					char szTrainNumber[5] = { 0 };
// 					szTrainNumber[0] = pPIS->OPERINFO.uTNX000 + 0x30;
// 					szTrainNumber[1] = pPIS->OPERINFO.uTN0X00 + 0x30;
// 					szTrainNumber[2] = pPIS->OPERINFO.uTN00X0 + 0x30;
// 					szTrainNumber[3] = pPIS->OPERINFO.uTN000X + 0x30;

// 					if (strcmp(m_szTrainNumber, szTrainNumber))
// 					{
// 						pO->SetTrainNumber(szTrainNumber);
// 					}
// 					strcpy(m_szTrainNumber,szTrainNumber);
// 					uint16_t uDistance = ((pPIS->OPERINFO.uDIST[0] << 8) & 0xFF00) | (pPIS->OPERINFO.uDIST[1] & 0x00FF);
// 					if (pPIS->OPERINFO.uNEXCODE != m_tPIS.OPERINFO.uNEXCODE)
// 					{
// 						pO->SetPIITextIndex(0);
// 						printf("Nex Code %u\n",pPIS->OPERINFO.uNEXCODE);
// 						pO->SetOperationNextStationCode(pPIS->OPERINFO.uNEXCODE, OM_NORMAL);
// 					}
// 					pO->SetDistance(uDistance, OM_NORMAL);
// 					pO->SetIDD1Information(OM_NORMAL);
// 					COperation *pOM = COperation::GetInstance();
// 					if (pPIS->INPUT.bHCR0)
// 						pOM->GetIDD1()->uCabDir = 1;
// 					if (pPIS->INPUT.bHCR1)
// 						pOM->GetIDD1()->uCabDir = 2;
// 					if (pPIS->INPUT.bDDN_L)
// 						pOM->GetIDD1()->uDoorDir = 1;
// 					if (pPIS->INPUT.bDDN_R)
// 						pOM->GetIDD1()->uDoorDir = 2;
// 					pOM->GetIDD1()->uCarCongestion[0] = pPIS->uAsp[0];
// 					pOM->GetIDD1()->uCarCongestion[1] = pPIS->uAsp[1];
// 					pOM->GetIDD1()->uCarCongestion[2] = pPIS->uAsp[2];
// 					pOM->GetIDD1()->uCarCongestion[3] = pPIS->uAsp[3];
// 					pOM->GetIDD1()->uCarCongestion[4] = pPIS->uAsp[4];
// 					pOM->GetIDD1()->uCarCongestion[5] = pPIS->uAsp[5];
// 					pOM->GetIDD1()->uCarCongestion[6] = pPIS->uAsp[6];
// 					pOM->GetIDD1()->uCarCongestion[7] = pPIS->uAsp[7];
					
					
// 					memcpy(&m_tPIS, pPIS, sizeof(_TCMS2PIS));
// 				}


// 				/*int nSendSize = sizeof(FDISDISENDHEAD) + uAddSize;
// 		memcpy(m_uSendBuffer, &m_tPIISend, nSendSize);
// 		m_tSockAddr.sin_family = AF_INET;
// 		m_tSockAddr.sin_addr.s_addr = inet_addr(m_chIPMultiAddr);
// 		m_tSockAddr.sin_port = htons(m_wPort);
// 		send(m_uSendBuffer, nSendSize, (sockaddr*)&m_tSockAddr, sizeof(m_tSockAddr));*/

// #ifdef		VIEWER_SUPPORT
// 				CViewerManage::UpdateViewer(1003);
// #endif
// 			}
			


		
// 		}
// 	}

	

	
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





