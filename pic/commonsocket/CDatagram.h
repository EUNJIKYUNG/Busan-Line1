#ifndef DATAGRAM
#define DATAGRAM
#include <event.h>
#include <pthread.h>

#ifdef _WIN32
#define	GETCLASSA(A) A->sin_addr.S_un.S_un_b.s_b1
#define	GETCLASSB(B) B->sin_addr.S_un.S_un_b.s_b2
#define	GETCLASSC(C) C->sin_addr.S_un.S_un_b.s_b3
#define	GETCLASSD(D) D->sin_addr.S_un.S_un_b.s_b4
#else
#include <arpa/inet.h>
#define	GETCLASSD(D) (unsigned char)(((D->sin_addr.s_addr)>>24)&0xFF)
#define	GETCLASSC(C) (unsigned char)(((C->sin_addr.s_addr)>>16)&0xFF)
#define	GETCLASSB(B) (unsigned char)(((B->sin_addr.s_addr)>>8)&0xFF)
#define	GETCLASSA(A) (unsigned char)((A->sin_addr.s_addr)&0xFF)
#endif

class CDatagram
{
public:
	CDatagram(int nPeriod);
	~CDatagram();
	virtual bool createSocket();
	bool bindSocket();
	int getSocket();
	void setEvent();
	void setTimeoutEvent(unsigned int uTimeOut,void *pArgs=NULL);
	static event_base *getEventBase();
	static bool initEvent();
	static void deInitEvent();
	virtual void handleToBeSent();
	virtual void processReceived(char *pBuf, int nSize, unsigned char *pAdddr, int nAddrSize);
	static void startDispatch();
	void send(const char *pBuffer, int nSize, sockaddr *pAddr, int nAddrSize);
protected:
	unsigned short m_wPort;
	int m_nPeriod;
	int m_nAddrLen;
private:
	bool set_blocking_mode(const int &socket, bool is_blocking);
	int m_nSockfd;

	struct sockaddr_in m_tServerAddr;
	struct event m_tEvent;
	struct event m_tTimer;
	static struct event_base *m_pBase;
	static void readEventCB(int socket, short int type, void *pargs);
	static void timerCB(int socket, short int type, void *pargs);
	static pthread_t m_tThreadDispatch;
	static int m_nThreadResult;
	static void dispatchThread(void *pArg);

};


#endif
