#pragma once
#include "../commonsocket/CUDPMulticast.h"
//#include "CDataManage.h"
#include "../defineStruct.h"
#include "../COperation.h"
#include "../TablesDefine/TableManage.h"
#ifdef VIEWER_SUPPORT
#include "../CViewerManage.h"
#endif
#include "../CDevIdentify.h"
#ifndef _WIN32
#include <arpa/inet.h>
#endif



class CIDD1Handler :
	public CUDPMulticast
{
public:
	CIDD1Handler(unsigned short wPort, const char *pIPAddrMulticast, const char *pIPAddrInterface, int nPeriod, unsigned char uTTL)
        :CUDPMulticast(wPort, pIPAddrMulticast, pIPAddrInterface, nPeriod, uTTL), m_uID(0)
	{

/*class CIDD1Handler :
	public CUDPUnicast
{
public:
	CIDD1Handler(unsigned short wPort,  int nPeriod)
		:CUDPUnicast(wPort, nPeriod), m_uID(0)
	{*/
		m_uDevID[0] = 1;
		m_uDevID[1] = 2;
		m_uDevID[2] = 3;
		m_uDevID[3] = 4;
		m_uDevID[4] = 5;
		m_uDevID[5] = 6;
		m_uDevID[6] = 7;
		m_uDevID[7] = 10;
	}

	virtual ~CIDD1Handler()
	{

	}
	
	

	void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)
	{
		CDevIdentify *pIdent = CDevIdentify::GetInstance();

        printf("CIDD1Handler::processReceived(%s)\n", pBuf);

		if ((GETCLASSA(pAddr) == 192)&&
			(GETCLASSB(pAddr) == 9))
		{ 
			if (GETCLASSC(pAddr) == 134)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 132)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 133)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 135)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}

			if (GETCLASSC(pAddr) == 136)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 137)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 138)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}
			if (GETCLASSC(pAddr) == 139)
			{
				pIdent->SetCurrentTick(&pAddr->sin_addr);
			}

		}

	}

	
	void handleToBeSent()	//Sending Data
	{
		m_uID++; 
		if (m_uID > sizeof(m_uDevID)) 
			m_uID = 0;
		
		COperation *pOM = COperation::GetInstance();
		pOM->GetIDD1Information(&m_tSEND);
		// m_tSEND.uTgtDev = m_uDevID[m_uID];

		memcpy(m_uSendBuffer, &m_tSEND, sizeof(m_tSEND));
		m_tSockAddr.sin_family = AF_INET;
		//m_tSockAddr
		m_tSockAddr.sin_addr.s_addr = inet_addr(m_chIPMultiAddr);
		m_tSockAddr.sin_port = htons(m_wPort);
        send(m_uSendBuffer, sizeof(m_tSEND),(sockaddr*)&m_tSockAddr,sizeof(m_tSockAddr));

        // printf("CIDD1Handler::handleToBeSent(%dbytes)\n", sizeof(m_tSEND));
       
#ifdef		VIEWER_SUPPORT
		CViewerManage::UpdateViewer(1001);
#endif
	}
	uint8_t m_uID;
	uint8_t m_uDevID[7];
	_PISC2IDD1 m_tSEND;
	sockaddr_in m_tSockAddr;
	char m_uSendBuffer[2048];
};


