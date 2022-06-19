#include "CommManage.h"
#include "commonsocket/CDatagram.h"

/*#ifndef APP_MON_SERVER
	#include "AppMonitorClient.h"
#else
	#include "AppMonitorServer.h"
#endif*/

#include "CommHandlers/CFDIIndicator.h"
#include "CommHandlers/CPII1Indicator.h"
#include "CommHandlers/CPII2Indicator.h"
#include "CommHandlers/CMTXHandler2.h"
#include "CommHandlers/CIDD1Handler.h"
#include "CommHandlers/CTCMSHandler.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


CCommManage::CCommManage(void)
{

}

CCommManage::~CCommManage(void)
{

}

#ifndef APP_MON_SERVER
#ifndef _WIN32
void CCommManage::initManage(unsigned short uPort)
{
	CDatagram::initEvent();
	initHandlers();
	CDatagram::startDispatch();
}
#else
void CCommManage::initManage(HWND hwndApp,unsigned short uPort)
{
	CDatagram::initEvent();
	//m_pClient=std::shared_ptr<CAppMonitorClientHandler>(new CAppMonitorClientHandler(hwndApp,(unsigned short)uPort,1000));
	initHandlers();
	CDatagram::startDispatch();
}
#endif

void CCommManage::deinitManage()
{
	//close(m_pFDI->getSocket());
	deinitHandlers();
	CDatagram::deInitEvent();
}

void CCommManage::initHandlers()
{

		struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

char chMyip[256];

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name,"eth0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("\tInterface : <%s>\n",ifa->ifa_name );
            printf("\t  Address : <%s>\n", host); 
			strcpy(chMyip,host);
        }
    }

    freeifaddrs(ifaddr);





	m_pFDI = std::shared_ptr<CFDIIndicator>(new  CFDIIndicator(30013, "233.253.18.3", chMyip, 100, 64));
	m_pFDI->setTimeoutEvent(100,(void*)m_pFDI.get());
	m_pPII1 = std::shared_ptr<CPII1Indicator>(new  CPII1Indicator(30023, "233.253.18.3", chMyip, 100, 64));
	m_pPII1->setTimeoutEvent(100,(void*)m_pPII1.get());
	m_pPII2 = std::shared_ptr<CPII2Indicator>(new  CPII2Indicator(30033, "233.253.18.3", chMyip, 100, 64));
	m_pPII2->setTimeoutEvent(100,(void*)m_pPII2.get());
	m_pMTX = std::shared_ptr<CMTXHandler2>(new CMTXHandler2(30004, 100));
	m_pMTX->setTimeoutEvent(100,(void*)m_pMTX.get());
	m_pIDD1 = std::shared_ptr<CIDD1Handler>(new CIDD1Handler(30043, "233.253.18.1", chMyip, 100, 64));
	m_pIDD1->setTimeoutEvent(100,(void*)m_pIDD1.get());
	m_pTCMS = std::shared_ptr<CTCMSHandler>(new CTCMSHandler(1001, 0));
}

void CCommManage::deinitHandlers()
{
	close(m_pFDI->getSocket());
	close(m_pPII1->getSocket());
	close(m_pPII2->getSocket());

}

#else
void CCommManage::initManage(unsigned short uPort)
{
	CDatagram::initEvent();
	m_pServer= std::shared_ptr<CAppMonitorServerHandler>(new CAppMonitorServerHandler((unsigned short)uPort,1000));
	initHandlers();
	CDatagram::startDispatch();
}

void CCommManage::deinitManage()
{
	CDatagram::deInitEvent();
	deinitHandlers();
}
#endif