#ifndef COMMMANAGE
#define COMMMANAGE
#ifdef _WIN32
	#include <Windows.h>
#endif
#include <memory>

#ifndef APP_MON_SERVER
	class CAppMonitorClientHandler;
#else
	class CAppMonitorServerHandler;
#endif


class CFDIIndicator;
class CPII1Indicator;
class CPII2Indicator;
class CMTXHandler2;
class CIDD1Handler;
class CTCMSHandler;


class CCommManage
{
public:
	CCommManage(void);
	virtual ~CCommManage(void);
#ifndef APP_MON_SERVER
	void deinitManage();
#ifndef _WIN32
	void initManage(unsigned short uPort);

#else
	void initManage(HWND hwndApp,unsigned short uPort);
#endif
#else
	void deinitManage();
	void initManage(unsigned short uPort);
#endif
	virtual void initHandlers();
	virtual void deinitHandlers();
public:
#ifndef APP_MON_SERVER
	std::shared_ptr<CAppMonitorClientHandler> m_pClient;
	std::shared_ptr<CFDIIndicator> m_pFDI;
	std::shared_ptr<CPII1Indicator> m_pPII1;
	std::shared_ptr<CPII2Indicator> m_pPII2;
	std::shared_ptr<CMTXHandler2> m_pMTX;
	std::shared_ptr<CIDD1Handler> m_pIDD1;
	std::shared_ptr<CTCMSHandler> m_pTCMS;
#else
	//CAppMonitorServerHandler *m_pServer;
	std::shared_ptr<CAppMonitorServerHandler> m_pServer;
#endif
};

#endif