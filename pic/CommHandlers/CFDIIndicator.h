#pragma once
#include "../LEDIndicator/defineLED.h"
#include "../commonsocket/CUDPMulticast.h"
//#include "CDataManage.h"
#include "../TablesDefine/TableManage.h"
#ifdef VIEWER_SUPPORT
#include "../CViewerManage.h"
#endif
#include "../LEDIndicator/CLEDIndBufManage.h"
#include "../CDevIdentify.h"
#ifndef _WIN32
#include <arpa/inet.h>
#endif

#define RESEND_FDISEQ(A) if(pBuffer->bUpdate##A) { lock.lock(); pushRetryBuffer(A-1); pManage->m_bInUse = true; lock.unlock(); printf("retry request block %d from Dev %d\n",A,uDevID);};


class CFDIIndicator :
	public CUDPMulticast
{
public:
	CFDIIndicator(unsigned short wPort, const char *pIPAddrMulticast, const char *pIPAddrInterface, int nPeriod, unsigned char uTTL)
        :CUDPMulticast(wPort, pIPAddrMulticast, pIPAddrInterface, nPeriod, uTTL),m_uID(0)
	{
		m_uDevID[0] = 101;
		m_uDevID[1] = 110;
		memset(m_uCRC, 0, sizeof(m_uCRC));
		memset(&m_tFDISend, 0, sizeof(m_tFDISend));
	}

	virtual ~CFDIIndicator()
	{

	}
	
	

	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDevIdentify *pIdent = CDevIdentify::GetInstance();
		if ((GETCLASSA(pAddr) == 192)&&
			(GETCLASSB(pAddr) == 9)&&
			(GETCLASSC(pAddr) == 130))
		{ 
			LEDRECV *pRecvBuf = (LEDRECV*)pBuf;
			switch (GETCLASSD(pAddr))
			{
			case 101:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				handleReceived(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tFDIRecv[0], pBuf, sizeof(LEDRECV));
                //pDM->SetVersionStringByVerInfo(DEV_LIST::DEV_FDI,&m_tFDIRecv.tVerInfo);
#ifdef VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2001);
#endif
				break;
			case 110:
				pIdent->SetCurrentTick(&pAddr->sin_addr);
				handleReceived(pRecvBuf, GETCLASSD(pAddr));
				memcpy(&m_tFDIRecv[1], pBuf, sizeof(LEDRECV));
#ifdef VIEWER_SUPPORT
				CViewerManage::UpdateViewer(2002);
#endif
				break;
			default:
				break;
			}
		}
	}

	void handleReceived(LEDRECV *pBuffer, uint32_t uDevID)
	{
		CLEDIndBufManage *pManage = CFDIIndBufManage::GetInstance();
		pManage->m_bInUse;
		if (!pManage->m_bInUse)
		{
			std::unique_lock<std::mutex> lock(m_tMutex, std::defer_lock);
			RESEND_FDISEQ(1);
			RESEND_FDISEQ(2);
			RESEND_FDISEQ(3);
			RESEND_FDISEQ(4);
			RESEND_FDISEQ(5);
			RESEND_FDISEQ(6);
			RESEND_FDISEQ(7);
			RESEND_FDISEQ(8);
			RESEND_FDISEQ(9);
			RESEND_FDISEQ(10);
			RESEND_FDISEQ(11);
			RESEND_FDISEQ(12);
			RESEND_FDISEQ(13);
			RESEND_FDISEQ(14);
			RESEND_FDISEQ(15);
			RESEND_FDISEQ(16);
			RESEND_FDISEQ(17);
			RESEND_FDISEQ(18);
			RESEND_FDISEQ(19);
			RESEND_FDISEQ(20);
			RESEND_FDISEQ(21);
			RESEND_FDISEQ(22);
			RESEND_FDISEQ(23);
			RESEND_FDISEQ(24);
			RESEND_FDISEQ(25);
			RESEND_FDISEQ(26);
			RESEND_FDISEQ(27);
			RESEND_FDISEQ(28);
			RESEND_FDISEQ(29);
			RESEND_FDISEQ(30);
			RESEND_FDISEQ(31);
			RESEND_FDISEQ(32);
			RESEND_FDISEQ(33);
			RESEND_FDISEQ(34);
			RESEND_FDISEQ(35);
			RESEND_FDISEQ(36);
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
		m_tFDISend.tHead.uTxCounter++;
		m_tFDISend.tHead.uDevCode = LED_FDI;
		m_tFDISend.tHead.uDevID = m_uDevID[m_uID];
		m_tFDISend.tHead.eImageRetainigTime = 20;
		CTableManage *pTM = CTableManage::GetInstance();
		int nSize = sizeof(FDISDISENDHEAD);
		unsigned int uAddSize = 0;
		CLEDIndBufManage *pManage = CFDIIndBufManage::GetInstance();
		if (pManage->m_bUpdated && (!m_tFDISend.tHead.bUpdate))
		{
			for (int i = 0; i < 36; i++)
			{
				uint16_t uCRC = pManage->GetBlockCRC(i);
				//printf("BlockCRC[%d][%04X]\n",i, uCRC);
				if (m_uCRC[i] != uCRC)
				{
					m_uCRC[i] = pManage->GetBlockCRC(i);
					m_tFDISend.tHead.uCRCValue[2 * i] = (m_uCRC[i] >> 8) & 0xFF;
					m_tFDISend.tHead.uCRCValue[2 * i + 1] = (m_uCRC[i]) & 0xFF;
					//printf("CRC [%d]%02x%02x\n", i,m_tFDISend.tHead.uCRCValue[2 * i], m_tFDISend.tHead.uCRCValue[2 * i + 1]);
					m_qUpdate.push(i);
					pManage->m_bInUse = true;
				}
			}
			pManage->m_bUpdated = false;
		}

		if ((!m_vRetry.size()) && (!m_qUpdate.size()))
		{
			uAddSize = 0;
			memset(m_tFDISend.tHead.uStartAddr, 0, sizeof(m_tFDISend.tHead.uStartAddr));
			memset(m_tFDISend.tHead.uEndAddr, 0, sizeof(m_tFDISend.tHead.uEndAddr));

			pManage->m_bInUse = false;
			m_tFDISend.tHead.bUpdate = false;
			m_tFDISend.tHead.bRetryUpdate = false;
		}


		if (m_qUpdate.size())
		{
			m_tFDISend.tHead.bUpdate = true;
			m_tFDISend.tHead.bRetryUpdate = false;
			auto uIndex = m_qUpdate.front();
			uAddSize = pManage->GetBlockSize();
			auto uStart = uIndex * pManage->GetBlockSize();
			auto uEnd = (uStart + pManage->GetBlockSize() - 1);
			m_tFDISend.tHead.uStartAddr[0] = (uStart >> 8) & 0xFF;
			m_tFDISend.tHead.uStartAddr[1] = (uStart) & 0xFF;
			m_tFDISend.tHead.uEndAddr[0] = (uEnd >> 8) & 0xFF;
			m_tFDISend.tHead.uEndAddr[1] = (uEnd) & 0xFF;
			memcpy(m_tFDISend.uBinBuf, pManage->GetBlock(uIndex), pManage->GetBlockSize());
			m_qUpdate.pop();
		}

		if (m_vRetry.size()&&(!uAddSize))
		{
			std::unique_lock<std::mutex> lock(m_tMutex, std::defer_lock);
			m_tFDISend.tHead.bUpdate = false;
			m_tFDISend.tHead.bRetryUpdate = true;
			auto uIndex = (*m_vRetry.begin());
			uAddSize = pManage->GetBlockSize();
			auto uStart = uIndex * pManage->GetBlockSize();
			auto uEnd = (uStart + pManage->GetBlockSize() - 1);
			m_tFDISend.tHead.uStartAddr[0] = (uStart >> 8) & 0xFF;
			m_tFDISend.tHead.uStartAddr[1] = (uStart) & 0xFF;
			m_tFDISend.tHead.uEndAddr[0] = (uEnd >> 8) & 0xFF;
			m_tFDISend.tHead.uEndAddr[1] = (uEnd) & 0xFF;
			memcpy(m_tFDISend.uBinBuf, pManage->GetBlock(uIndex), pManage->GetBlockSize());
			lock.lock();
			m_vRetry.erase(m_vRetry.begin());
			lock.unlock();
		}
		m_tFDISend.tHead.uPageCnt = pManage->getCurrentPage();
		m_tFDISend.tHead.bSyncByCNCS = pManage->getSync();
		DEV_INC_FDI(m_uID)
		int nSendSize = sizeof(FDISDISENDHEAD) + uAddSize;
		memcpy(m_uSendBuffer, &m_tFDISend, nSendSize);
		m_tSockAddr.sin_family = AF_INET;
		m_tSockAddr.sin_addr.s_addr = inet_addr(m_chIPMultiAddr);
		m_tSockAddr.sin_port = htons(m_wPort);
        send(m_uSendBuffer,nSendSize,(sockaddr*)&m_tSockAddr,sizeof(m_tSockAddr));
#ifdef		VIEWER_SUPPORT
		CViewerManage::UpdateViewer(1001);
#endif
	}

	static FDISDISEND m_tFDISend;
	static LEDRECV m_tFDIRecv[2];
	uint8_t m_uDevID[2];
	uint8_t m_uID;
	unsigned short m_uCRC[36];
	std::mutex m_tMutex;;
	std::queue<unsigned int> m_qUpdate;
	std::vector<uint32_t> m_vRetry;
	sockaddr_in m_tSockAddr;
	char m_uSendBuffer[2048];
};


