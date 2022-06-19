#ifndef UDPUNICAST
#define UDPUNICAST
#include "CDatagram.h"

class CUDPUnicast
	:public CDatagram
{
public:
	CUDPUnicast(unsigned short wPort,int nPeriod = 0);
	~CUDPUnicast();
	bool createSocket();
	virtual void processReceived(char *pBuf, int nSize, sockaddr_in* pAddr) = 0;
private:
	void processReceived(char *pBuf, int nSize, unsigned char *pAdddr, int nAddrSize);
};

#endif