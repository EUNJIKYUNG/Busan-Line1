#include "CUDPMulticast.h"
#include <string.h>
/*#ifdef _WIN32
	#include <ws2ipdef.h>
	#include <winsock.h>
#include <windows.h>
#endif*/


CUDPMulticast::CUDPMulticast(unsigned short wPort, const char *pIPAddrMulticast, const char *pIPAddrInterface, int nPeriod,unsigned char uTTL)
	:CDatagram(nPeriod),m_uMultiTTL(uTTL)
{
	
	m_nAddrLen = sizeof(sockaddr_in);
	m_wPort = wPort;
	printf("%s %d\n",__FUNCTION__,__LINE__);
	strcpy(m_chIPMultiAddr, pIPAddrMulticast);
	printf("%s %d\n",__FUNCTION__,__LINE__);
	strcpy(m_chIPAddr, pIPAddrInterface);
	printf("%s %d\n",__FUNCTION__,__LINE__);
	createSocket();
	printf("%s %d\n",__FUNCTION__,__LINE__);
	setEvent();
}


CUDPMulticast::~CUDPMulticast()
{
	
}

bool CUDPMulticast::createSocket()
{
	bool bRet=CDatagram::createSocket();
	if (bRet)
	{
		if (!setsockopt(getSocket(), IPPROTO_IP, IP_MULTICAST_TTL, (char *)&m_uMultiTTL, sizeof(m_uMultiTTL)) < 0)
		{
			return false;
		}
		if (!bindSocket())
		{
			return false;
		}
		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(m_chIPMultiAddr);
		mreq.imr_interface.s_addr = inet_addr(m_chIPAddr);
		if (setsockopt(getSocket(), IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
		{
			return false;
		}
	}
	return bRet;
}

void CUDPMulticast::processReceived(char *pBuf, int nSize, unsigned char *pAddr, int nAddrSize)
{
	processReceived(pBuf, nSize, (sockaddr_in*)pAddr);
}