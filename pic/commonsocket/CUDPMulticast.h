#ifndef UDPMULTICAST
#define UDPMULTICAST
#include "CDatagram.h"

class CUDPMulticast
	:public CDatagram
{
public:
	CUDPMulticast(unsigned short wPort,const char *pIPAddrMulticast,const char *pIPAddrInterface,int nPeriod=0,unsigned char uTTL=1);
	~CUDPMulticast();
	bool createSocket();
	unsigned char m_uMultiTTL;
	char m_chIPAddr[32];
	char m_chIPMultiAddr[32];
	virtual void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr)=0;
private:
	void processReceived(char *pBuf, int nSize, unsigned char *pAdddr, int nAddrSize);

};

#endif