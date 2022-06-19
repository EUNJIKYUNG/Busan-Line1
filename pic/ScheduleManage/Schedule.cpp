#include "ScheduleManage.h"
#include "TimeRelated.h"
#include <string.h>


uint64_t CSchedule::g_uInsertCount = 0;

CSchedule::CSchedule(int nPriority,int nContentsIndex)
	:m_nPriority(nPriority), m_nContentsIndex(nContentsIndex), m_uPosition(0)
{
}

CSchedule::~CSchedule()
{

}

int CSchedule::getPriority()
{
	return m_nPriority;
}

int CSchedule::getIndex()
{
	return m_nContentsIndex;
}

bool CSchedule::operator ==(CSchedule &T)
{
	return ((getPriority() == T.getPriority() && (m_uInsertCount == T.m_uInsertCount)));
}

bool CSchedule::operator !=(CSchedule &T)
{
	return !(*this == T);
}

bool CSchedule::operator >(CSchedule &T)
{
	if (getPriority() > T.getPriority())
	{
		return true;
	}
	else if (getPriority() < T.getPriority())
	{
		return false;
	}
	else if (m_uInsertCount < T.m_uInsertCount)
	{
		return true;
	}
	return false;
}

bool CSchedule::operator >=(CSchedule &T)
{
	return (*this == T || *this > T);
}

bool CSchedule::operator <(CSchedule &T)
{
	return !(*this >= T);
}

bool CSchedule::operator <=(CSchedule &T)
{
	return !(*this > T);
}

void CSchedule::setPosition(unsigned int uPosition)
{
	m_uPosition = uPosition;
}

unsigned int CSchedule::getPosition()
{
	return m_uPosition;
}

int CSchedule::isInPeriod()
{
	return m_pPeriod->isInPeriod(0);
}

void CSchedule::setCurrentTimeStamp()
{
	m_uInsertCount = g_uInsertCount;
	g_uInsertCount = g_uInsertCount + 1;
	//m_uInsertCount = g_uInsertCount;
}

const XCHAR* CSchedule::getContentsName()
{
	if (m_pContents)
		return m_pContents->getFileName();
	return NULL;
}
const int CSchedule::getDuration()
{
	if (m_pContents)
		return m_pContents->getDuration();
	return 0;
}

const int CSchedule::getType()
{
	if (m_pContents)
		return m_pContents->getType();
	return 0;
}


CContents::CContents(int nIndex, XCHAR *pFileName, int nDuration, char *pStart, char *pEnd,int nType)
	:m_nIndex(nIndex), m_nDuration(nDuration),m_nType(nType)
{
	STRCAT(chFileName, pFileName);
	m_pPeriod = std::shared_ptr<CPeriod>(new CPeriod(pStart, pEnd));
    //printf("%s %s %s %s\n",__FUNCTIONW__,pFileName,pStart,pEnd);
}

CContents::CContents(int nIndex, int nDuration, char *pStart, char *pEnd, int nType)
	:m_nIndex(nIndex), m_nDuration(nDuration), m_nType(nType)
{
	memset(chFileName, 0,sizeof(chFileName));
	m_pPeriod = std::shared_ptr<CPeriod>(new CPeriod(pStart, pEnd));
    //printf("%s %s %s %s\n",__FUNCTIONW__,chFileName,pStart,pEnd);
}

CContents::~CContents()
{

}

const int CContents::getDuration() const
{
	return m_nDuration;
}
const XCHAR* CContents::getFileName() const
{
	return chFileName;
}

const int CContents::getType() const
{
	return m_nType;
}

const int CContents::getIndex() const
{
	return m_nIndex;
}
const std::shared_ptr<CPeriod>& CContents::getPeriod() const
{
	return m_pPeriod;
}

