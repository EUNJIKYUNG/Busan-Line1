#include "CDatagram.h"
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <ws2ipdef.h>
#include <ws2tcpip.h>

#else
#include <sys/fcntl.h>
#endif



typedef void * (*THREADFUNCPTR)(void *);

struct event_base *CDatagram::m_pBase = 0;

pthread_t  CDatagram::m_tThreadDispatch;

int CDatagram::m_nThreadResult;

bool CDatagram::initEvent()
{
#ifdef _WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	(void)WSAStartup(wVersionRequested, &wsaData);
#endif

	m_pBase = event_base_new();
	if (!m_pBase)
		return false;
	return true;
}
void CDatagram::deInitEvent()
{

	if(m_pBase)
	{
		event_base_loopbreak(m_pBase);
		pthread_join(m_tThreadDispatch, (void **)&m_nThreadResult);
	}


	if(m_pBase)
	{
		event_base_free(m_pBase);
		m_pBase=NULL;
	}

	//event_free(&m_tEvent);
	
}

CDatagram::CDatagram(int nPeriod)
	:m_nSockfd(0), m_wPort(0), m_nPeriod(nPeriod)
{
	
	memset((char *)&m_tServerAddr, 0, sizeof(m_tServerAddr));
	printf("%s %d\n",__FUNCTION__,__LINE__);
	/*if(m_nPeriod)
	{
		setTimeoutEvent(m_nPeriod,(void*)this);
	}*/
}




CDatagram::~CDatagram()
{
	deInitEvent();
}


bool CDatagram::set_blocking_mode(const int &socket, bool is_blocking)
{
	bool ret = true;
#ifdef WIN32
	/// @note windows sockets are created in blocking mode by default
	// currently on windows, there is no easy way to obtain the socket's current blocking mode since WSAIsBlocking was deprecated
	u_long flags = is_blocking ? 0 : 1;
	ret = NO_ERROR == ioctlsocket(socket, FIONBIO, &flags);
#else
	const int flags = fcntl(socket, F_GETFL, 0);
	if ((flags & O_NONBLOCK) && !is_blocking) { printf("set_blocking_mode(): socket was already in non-blocking mode"); return ret; }
	if (!(flags & O_NONBLOCK) && is_blocking) { printf("set_blocking_mode(): socket was already in blocking mode"); return ret; }
	ret = 0 == fcntl(socket, F_SETFL, is_blocking ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
#endif

	return ret;
}


bool CDatagram::createSocket()
{
	if ((m_nSockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("%s %u ERROR - unable to create socket", __FILE__, __LINE__);
		return false;
	}

	//To Make Non Blocking Mode
	if (!set_blocking_mode(m_nSockfd, false))
	{
		printf("%s %u Set blocking mode failed!", __FILE__, __LINE__);
		return false;
	}

	int nOptVal = 1;
	int nRetVal = setsockopt(m_nSockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&nOptVal, sizeof(nOptVal));
	if (nRetVal)
	{
		printf("%s %u  ERROR - socketOptions: Error at Setsockopt %d", __FILE__, __LINE__, nRetVal);
		return false;
	}
	return true;
}

bool CDatagram::bindSocket()
{
	m_tServerAddr.sin_family = AF_INET;
	m_tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_tServerAddr.sin_port = htons(m_wPort);
	int addrlen = sizeof(m_tServerAddr);
	if (bind(getSocket(), (struct sockaddr *) &m_tServerAddr, sizeof(m_tServerAddr)) < 0)
	{
		printf("%s %u binding address failed!", __FILE__, __LINE__);
		return false;
	}
	return true;
}

void CDatagram::setEvent()
{
	int nSocket = getSocket();
	event_assign(&m_tEvent, m_pBase, getSocket(), EV_READ | EV_PERSIST, &CDatagram::readEventCB, this);
	event_add(&m_tEvent, NULL);
}

void CDatagram::setTimeoutEvent(unsigned int uTimeOut,void *pArgs)
{

	struct timeval stTv;
	stTv.tv_sec = (uTimeOut/1000);
	stTv.tv_usec = (uTimeOut%1000) * 1000;
	event_assign(&m_tTimer, m_pBase, -1, EV_TIMEOUT | EV_PERSIST, &CDatagram::timerCB, pArgs);
	event_add(&m_tTimer, &stTv);
}

event_base *CDatagram::getEventBase()
{
	return m_pBase;
}

int CDatagram::getSocket()
{
	return m_nSockfd;
}

void CDatagram::send(const char *pBuffer, int nSize, sockaddr *pAddr, int nAddrSize)
{
	sendto(getSocket(), pBuffer, nSize, 0, pAddr, nAddrSize);
}


void CDatagram::readEventCB(int socket, short int type, void *pargs)
{
	CDatagram *pDG = (CDatagram*)pargs;
	int unFromAddrLen = 0;
	int nByte = 0;
	char aReqBuffer[2048];
	char aAddrBuf[128];
	if ((nByte = recvfrom(socket, aReqBuffer, sizeof(aReqBuffer), 0, (sockaddr*)aAddrBuf, (socklen_t*)&pDG->m_nAddrLen)) == -1)
	{
	
		printf("error occured while receiving\n");
	}
	else
	{
		pDG->processReceived(aReqBuffer, nByte, (unsigned char*)aAddrBuf, unFromAddrLen);
	}
}

void CDatagram::timerCB(int socket, short int type, void *pargs)
{
	CDatagram *pDG = (CDatagram*)pargs;
	pDG->handleToBeSent();
}

void CDatagram::startDispatch()
{
	pthread_create(&m_tThreadDispatch, NULL,(THREADFUNCPTR)&CDatagram::dispatchThread,0);
}

void CDatagram::dispatchThread(void *pArg)
{
	event_base_dispatch(m_pBase);
}


void CDatagram::handleToBeSent()
{

}
void CDatagram::processReceived(char *pBuf, int nSize, unsigned char *pAdddr, int nAddrSize)
{

}