#include "CUDPUnicast.h"


CUDPUnicast::CUDPUnicast(unsigned short wPort, int nPeriod)
	:CDatagram(nPeriod)
{
	m_nAddrLen = sizeof(sockaddr_in);
	m_wPort = wPort;
	createSocket();
	setEvent();
	
}


CUDPUnicast::~CUDPUnicast()
{
}


bool CUDPUnicast::createSocket()
{
	bool bRet = CDatagram::createSocket();
	if (bRet)
	{
		if (!CDatagram::bindSocket())
		{
			return false;
		}
	}
	return bRet;
}

void CUDPUnicast::processReceived(char *pBuf, int nSize, unsigned char *pAddr, int nAddrSize)
{
	processReceived(pBuf, nSize, (sockaddr_in*)pAddr);
}