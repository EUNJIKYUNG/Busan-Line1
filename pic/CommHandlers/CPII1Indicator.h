#pragma once
#include "../LEDIndicator/defineLED.h"
#include "../commonsocket/CUDPMulticast.h"
#include "../CDataManage.h"
#ifdef VIEWER_SUPPORT
#include "../CViewerManage.h"
#endif
#include "../TablesDefine/TableManage.h"
#include "../LEDIndicator/CLEDIndBufManage.h"
#include "../CDevIdentify.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#define RESEND_PIISEQ(A) if(pBuffer->bUpdate##A) { lock.lock(); pushRetryBuffer(A-1); pManage->m_bInUse = true; lock.unlock(); printf("retry request block %d from Dev %d\n",A,uDevID);};


class CPII1Indicator :
	public CUDPMulticast
{
public:
	CPII1Indicator(unsigned short wPort, const char *pIPAddrMulticast, const char *pIPAddrInterface, int nPeriod, unsigned char uTTL)
		: CUDPMulticast(wPort, pIPAddrMulticast, pIPAddrInterface, nPeriod, uTTL), m_uID(0), m_bTextRetry(false), m_bMode(true)
	{
		m_uDevID[0] = 2;
		m_uDevID[1] = 3;
		m_uDevID[2] = 4;
		m_uDevID[3] = 5;
		m_uDevID[4] = 6;
		m_uDevID[5] = 7;
		m_uDevID[6] = 110;
		fillContents = &CPII1Indicator::fillText;
		handleReceived = &CPII1Indicator::handleReceivedText;
		memset(m_uCRC, 0, sizeof(m_uCRC));
		memset(&m_tPIISend, 0, sizeof(m_tPIISend));
	}

	virtual ~CPII1Indicator()
	{

	}



	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDevIdentify *pIdent = CDevIdentify::GetInstance();
		CDataManage *pDM = CDataManage::GetInstance();
		if ((GETCLASSA(pAddr) == 192) &&
			(GETCLASSB(pAddr) == 9) &&
			(GETCLASSC(pAddr) == 131))
		{
			LEDRECV *pRecvBuf = (LEDRECV*)pBuf;
			switch (GETCLASSD(pAddr))
			{
			case 2:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[0], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII4,&m_tPIIRecv[0].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2003);
#endif
				break;
			case 3:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[1], pBuf, sizeof(LEDRECV));
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2004);
#endif
				break;
			case 4:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[2], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII5,&m_tPIIRecv[2].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2005);
#endif
				break;
			case 5:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[3], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII2,&m_tPIIRecv[3].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2006);
#endif
				break;
			case 6:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[4], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII6,&m_tPIIRecv[4].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2007);
#endif
				break;
			case 7:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[5], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII3,&m_tPIIRecv[5].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2008);
#endif
				break;
			case 110:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				(this->*handleReceived)(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tPIIRecv[6], pBuf, sizeof(LEDRECV));
				//pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_PII3,&m_tPIIRecv[5].tVerInfo);
#ifdef		VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2009);
#endif
				break;
			default:
				break;
			}
		}
	}

	void handleReceivedLEDImage(LEDRECV *pBuffer, uint32_t uDevID)
	{
		CLEDIndBufManage *pManage = CPII1IndBufManage::GetInstance();
		if (!pManage->m_bInUse)
		{
			std::unique_lock<std::mutex> lock(m_tMutex, std::defer_lock);
			RESEND_PIISEQ(1);
			RESEND_PIISEQ(2);
			RESEND_PIISEQ(3);
			RESEND_PIISEQ(4);
			RESEND_PIISEQ(5);
			RESEND_PIISEQ(6);
			RESEND_PIISEQ(7);
			RESEND_PIISEQ(8);
			RESEND_PIISEQ(9);
			RESEND_PIISEQ(10);
			RESEND_PIISEQ(11);
			RESEND_PIISEQ(12);
			RESEND_PIISEQ(13);
			RESEND_PIISEQ(14);
			RESEND_PIISEQ(15);
			RESEND_PIISEQ(16);
			RESEND_PIISEQ(17);
			RESEND_PIISEQ(18);
			RESEND_PIISEQ(19);
			RESEND_PIISEQ(20);
			RESEND_PIISEQ(21);
			RESEND_PIISEQ(22);
			RESEND_PIISEQ(23);
			RESEND_PIISEQ(24);
			RESEND_PIISEQ(25);
			RESEND_PIISEQ(26);
			RESEND_PIISEQ(27);
			RESEND_PIISEQ(28);
			RESEND_PIISEQ(29);
			RESEND_PIISEQ(30);
			RESEND_PIISEQ(31);
			RESEND_PIISEQ(32);
			RESEND_PIISEQ(33);
			RESEND_PIISEQ(34);
			RESEND_PIISEQ(35);
			RESEND_PIISEQ(36);
		}
	}

	void handleReceivedText(LEDRECV *pBuffer, uint32_t uDevID)
	{
		CLEDIndBufManage *pManage = CPII1IndBufManage::GetInstance();
		if (!pManage->m_bInUse)
		{
			if (pBuffer->bUpdate1)
			{
				m_bTextRetry = true;
			}
		}
	}

	void pushRetryBuffer(uint32_t uBlock)
	{
		auto it = find(m_vRetry.begin(), m_vRetry.end(), uBlock);
		if (it == m_vRetry.end())
		{
			m_vRetry.push_back(uBlock);
		}
	}

	void handleToBeSent()	//Sending Data
	{
		m_tPIISend.tHead.uTxCounter++;
		m_tPIISend.tHead.uDevCode = 3;
		m_tPIISend.tHead.uDevID = m_uDevID[m_uID];
		m_tPIISend.tHead.eImageRetainigTime = 20;
		CDataManage *pDM = CDataManage::GetInstance();
		if (m_bMode != pDM->getPII1Mode())
		{
			if (pDM->getPII1Mode())
			{
				fillContents = &CPII1Indicator::fillText;
				handleReceived = &CPII1Indicator::handleReceivedText;
			}
			else
			{
				fillContents = &CPII1Indicator::fillLEDImage;
				handleReceived = &CPII1Indicator::handleReceivedLEDImage;
			}
			m_bMode = pDM->getPII1Mode();
		}
		m_tPIISend.tHead.bTextSend = m_bMode;
		CTableManage *pTM = CTableManage::GetInstance();
		DEV_INC_PII1(m_uID)
		int nSize = sizeof(FDISDISENDHEAD);
		unsigned int uAddSize = 0;
		(this->*fillContents)(&uAddSize);

		int nSendSize = sizeof(FDISDISENDHEAD) + uAddSize;
		memcpy(m_uSendBuffer, &m_tPIISend, nSendSize);
		m_tSockAddr.sin_family = AF_INET;
		m_tSockAddr.sin_addr.s_addr = inet_addr(m_chIPMultiAddr);
		m_tSockAddr.sin_port = htons(m_wPort); 
        send(m_uSendBuffer, nSendSize, (sockaddr*)&m_tSockAddr, sizeof(m_tSockAddr));

#ifdef		VIEWER_SUPPORT
		CViewerManage::UpdateViewer(1002);
#endif
	}


	void fillLEDImage(uint32_t *pAddSize)
	{
		CLEDIndBufManage *pManage = CPII1IndBufManage::GetInstance();
		if (pManage->m_bUpdated && (!m_tPIISend.tHead.bUpdate))
		{
			for (int i = 0; i < 18; i++)
			{
				uint16_t uCRC = pManage->GetBlockCRC(i);
				if (m_uCRC[i] != uCRC)
				{
					m_uCRC[i] = pManage->GetBlockCRC(i);
					m_tPIISend.tHead.uCRCValue[2 * i] = (m_uCRC[i] >> 8) & 0xFF;
					m_tPIISend.tHead.uCRCValue[2 * i + 1] = (m_uCRC[i]) & 0xFF;
					m_qUpdate.push(i);
					pManage->m_bInUse = true;
				}
			}
			pManage->m_bUpdated = false;
		}

		if ((!m_vRetry.size()) && (!m_qUpdate.size()))
		{
			(*pAddSize) = 0;
			memset(m_tPIISend.tHead.uStartAddr, 0, sizeof(m_tPIISend.tHead.uStartAddr));
			memset(m_tPIISend.tHead.uEndAddr, 0, sizeof(m_tPIISend.tHead.uEndAddr));

			pManage->m_bInUse = false;
			m_tPIISend.tHead.bUpdate = false;
			m_tPIISend.tHead.bRetryUpdate = false;
		}


		if (m_qUpdate.size())
		{
			m_tPIISend.tHead.bUpdate = true;
			m_tPIISend.tHead.bRetryUpdate = false;
			auto uIndex = m_qUpdate.front();
			(*pAddSize) = pManage->GetBlockSize();
			auto uStart = uIndex * pManage->GetBlockSize();
			auto uEnd = (uStart + pManage->GetBlockSize() - 1);
			m_tPIISend.tHead.uStartAddr[0] = (uStart >> 8) & 0xFF;
			m_tPIISend.tHead.uStartAddr[1] = (uStart) & 0xFF;
			m_tPIISend.tHead.uEndAddr[0] = (uEnd >> 8) & 0xFF;
			m_tPIISend.tHead.uEndAddr[1] = (uEnd) & 0xFF;
			memcpy(m_tPIISend.uBinBuf, pManage->GetBlock(uIndex), pManage->GetBlockSize());
			m_qUpdate.pop();
		}

		if (m_vRetry.size())
		{
			std::unique_lock<std::mutex> lock(m_tMutex, std::defer_lock);
			m_tPIISend.tHead.bUpdate = false;
			m_tPIISend.tHead.bRetryUpdate = true;
			auto uIndex = (*m_vRetry.begin());
			(*pAddSize) = pManage->GetBlockSize();
			auto uStart = uIndex * pManage->GetBlockSize();
			auto uEnd = (uStart + pManage->GetBlockSize() - 1);
			m_tPIISend.tHead.uStartAddr[0] = (uStart >> 8) & 0xFF;
			m_tPIISend.tHead.uStartAddr[1] = (uStart) & 0xFF;
			m_tPIISend.tHead.uEndAddr[0] = (uEnd >> 8) & 0xFF;
			m_tPIISend.tHead.uEndAddr[1] = (uEnd) & 0xFF;
			memcpy(m_tPIISend.uBinBuf, pManage->GetBlock(uIndex), pManage->GetBlockSize());
			lock.lock();
			m_vRetry.erase(m_vRetry.begin());
			lock.unlock();
		}
		m_tPIISend.tHead.uPageCnt = pManage->getCurrentPage();
		m_tPIISend.tHead.bSyncByCNCS = pManage->getSync();
	}

	void fillText(uint32_t *pDataSize)
	{
		(*pDataSize) = 0;
		m_tPIISend.tHead.bUpdate = false;
		m_tPIISend.tHead.bRetryUpdate = false;
		CLEDIndBufManage *pManage = CPII1IndBufManage::GetInstance();
		pManage->m_bInUse=false;
		if (pManage->m_bUpdated && (!m_tPIISend.tHead.bUpdate)&&(!m_tPIISend.tHead.bUpdate))
		{
			uint32_t uTotLen = pManage->preparePIIText();
			if (uTotLen)
			{
				pManage->m_bUpdated = false;
				pManage->m_bInUse = true;
				m_tPIISend.tHead.bUpdate = true;
				m_tPIISend.tHead.bRetryUpdate = false;
				fillPIITextBuffer(pDataSize);
			}
		}
		
		if(m_bTextRetry)
		{
			m_tPIISend.tHead.bUpdate = false;
			m_tPIISend.tHead.bRetryUpdate = true;
			fillPIITextBuffer(pDataSize);
			m_bTextRetry = false;
		}
		/*if ((!m_tPIISend.tHead.bUpdate) && (!m_tPIISend.tHead.bRetryUpdate))
		{
			m_tPIISend.tHead.uStartAddr[0] = 0;
			m_tPIISend.tHead.uStartAddr[1] = 0;
			m_tPIISend.tHead.uEndAddr[0] = 0;
			m_tPIISend.tHead.uEndAddr[1] = 0;
		}*/
		(*pDataSize) = pManage->getTextBlockLength();
	}

	void fillPIITextBuffer(uint32_t *pDataSize)
	{
		CLEDIndBufManage *pManage = CPII1IndBufManage::GetInstance();
		uint16_t uTextLength = pManage->getTextBlockLength();
		m_tPIISend.tHead.uStartAddr[0] = 0;
		m_tPIISend.tHead.uStartAddr[1] = 0;
		m_tPIISend.tHead.uEndAddr[0] = (uTextLength >> 8) & 0xFF;
		m_tPIISend.tHead.uEndAddr[1] = uTextLength & 0xFF;

		uint16_t uCRC = pManage->GetTextBlockCRC();
		memset(m_tPIISend.tHead.uCRCValue, 0, sizeof(m_tPIISend.tHead.uCRCValue));
		memset(m_uCRC, 0, sizeof(m_uCRC));
		m_tPIISend.tHead.uCRCValue[0] = (uCRC >> 8) & 0xFF;
		m_tPIISend.tHead.uCRCValue[1] = (uCRC) & 0xFF;
		memcpy(m_tPIISend.uBinBuf, pManage->GetBlock(0), pManage->getTextBlockLength());
		(*pDataSize) = pManage->getTextBlockLength();
	}


	void (CPII1Indicator::*fillContents)(uint32_t *uDataSize);
	void (CPII1Indicator::*handleReceived)(LEDRECV *pBuffer, uint32_t uDevID);

	

	bool m_bMode;
	static FDISDISEND m_tPIISend;
	static LEDRECV m_tPIIRecv[7];

	uint8_t m_uDevID[7];
	uint8_t m_uID;
	std::mutex m_tMutex;;
	unsigned short m_uCRC[36];
	std::queue<uint32_t> m_qUpdate;
	std::vector<uint32_t> m_vRetry;
	std::atomic_bool m_bTextRetry;

	sockaddr_in m_tSockAddr;
	char m_uSendBuffer[2048];
};





