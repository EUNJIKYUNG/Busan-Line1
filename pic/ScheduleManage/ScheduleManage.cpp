#include "ScheduleManage.h"
#include "TimeRelated.h"
#include "Schedule.h"



CScheduleManage *CScheduleManage::GetInstance()
{
	static CScheduleManage tManage;
	return &tManage;
}


CConstantSchedule::CConstantSchedule(int nPriority, int nContentsIndex)
	:CSchedule(nPriority, nContentsIndex)
{

}
CConstantSchedule::~CConstantSchedule()
{

}

int CConstantSchedule::initDuration(int nStartFromNow, CContentsManage *pCM)
{
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	std::shared_ptr<CContents> pContents;
	bool bRet = pCM->GetContents(m_nContentsIndex, pContents);
	if (bRet)
	{
		m_pContents = pContents;
		m_pPeriod = m_pContents->getPeriod();
	}
	return 0;
}




CEventSchedule::CEventSchedule(int nPriority, int nContentsIndex)
	:CSchedule(nPriority, nContentsIndex)
{

}
CEventSchedule::~CEventSchedule()
{

}

int CEventSchedule::initDuration(int nStartFromNow, CContentsManage *pCM)
{
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	std::shared_ptr<CContents> pContents;
	bool bRet= pCM->GetContents(m_nContentsIndex,pContents);
	if (bRet)
	{
		double  fDuration = (double)pContents->getDuration()*0.9f;
		int nRetStartFromNow = (nStartFromNow + pContents->getDuration()) / 1000;
		int nNewStartFromNow = (nStartFromNow + (int)fDuration) / 1000;
		m_pContents = pContents;
		m_pPeriod = std::shared_ptr<CPeriod>(new CPeriod(nNewStartFromNow));
		return nRetStartFromNow;
	}
	else
	{
		//printf("Invalid Duration\n");
	}
	return 0;
}



CCustomEventSchedule::CCustomEventSchedule(int nPriority, int nContentsIndex)
	:CSchedule(nPriority, nContentsIndex)//TemporarySchedule
{

}
CCustomEventSchedule::~CCustomEventSchedule()
{

}
int CCustomEventSchedule::initDuration(int nStartFromNow, CContentsManage *pCM)
{
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	std::shared_ptr<CContents> pContents;
	bool bRet = pCM->GetCustomContents(m_nContentsIndex, pContents);
	if (bRet)
	{
		double  fDuration = (double)pContents->getDuration()*0.9f;
		int nRetStartFromNow = (nStartFromNow + pContents->getDuration()) / 1000;
		int nNewStartFromNow = (nStartFromNow + (int)fDuration) / 1000;
		m_pContents = pContents;
		m_pPeriod = std::shared_ptr<CPeriod>(new CPeriod(nNewStartFromNow));
		return nRetStartFromNow;
	}
	return 0;
}
