#include "TimeRelated.h"
#include <stdio.h>
#include <memory>
#include "../tickCount/tickCount.h"
//#include "../CLogRecord.h"



#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

std::time_t CTickPoint::m_tBaseTime = 0;
int64_t CTickPoint::m_nBaseTick = 0;



void CTickPoint::setTime()
{
	m_tBaseTime = std::time(0);
	m_nBaseTick = getTickCount();
}




CTickPoint::CTickPoint()
{
	m_nTickCount = getTickCount();
    bValid = true;
	
}

CTickPoint::CTickPoint(int nSecondFromNow)
{
	m_nTickCount = (int64_t)getTickCount() + (int64_t)(nSecondFromNow * ONE_SECOND);
    bValid = true;
}

CTickPoint::CTickPoint(const CTickPoint &Other)
{
	m_nTickCount = Other.getTick();
    bValid = true;
}

CTickPoint::CTickPoint(const char *pTime)
{
	fromISOString(pTime);
}

CTickPoint::~CTickPoint()
{

}

bool CTickPoint::isValid()
{
	return bValid;
}




std::time_t CTickPoint::getTime() const
{
	return (std::time_t)(m_tBaseTime + ((int64_t)m_nTickCount - (int64_t)m_nBaseTick) / (int64_t)ONE_SECOND);
}

bool CTickPoint::operator ==(CTickPoint &T)
{
	return (getTick() == T.getTick());
}
bool CTickPoint::operator !=(CTickPoint &T)
{
	return !(*this == T);
}
bool CTickPoint::operator >(CTickPoint &T)
{
	if (getTick() > T.getTick()) return 1;
	return 0;
}
bool CTickPoint::operator >=(CTickPoint &T)
{
	return (*this == T || *this > T);
}
bool CTickPoint::operator <(CTickPoint &T)
{
	return !(*this >= T);
}
bool CTickPoint::operator <=(CTickPoint &T)
{
	return !(*this > T);
}

CTickPoint& CTickPoint::operator =(const CTickPoint &Other)
{
	if (this != &Other)
	{
		m_nTickCount = Other.getTick();
	}
	return *this;
};


int64_t CTickPoint::getTick() const
{
	return m_nTickCount;
}



bool CTickPoint::fromISOString(const char *pString)
{
	std::tm tTm = { 0 };
	if ((pString[8] == 'T') && (pString[15] == 0))
	{
		tTm.tm_sec = (((pString[13] - 0x30) * 10) + (pString[14] - 0x30));
		tTm.tm_min = (((pString[11] - 0x30) * 10) + (pString[12] - 0x30));
		tTm.tm_hour = (((pString[9] - 0x30) * 10) + (pString[10] - 0x30));
		tTm.tm_mday = (((pString[6] - 0x30) * 10) + (pString[7] - 0x30));
		tTm.tm_mon = (((pString[4] - 0x30) * 10) + (pString[5] - 0x30)) - 1;
		tTm.tm_year = (((pString[0] - 0x30) * 1000) +
			((pString[1] - 0x30) * 100) +
			((pString[2] - 0x30) * 10) +
			(pString[3] - 0x30)) - 1900;
		tTm.tm_isdst = -1;

		std::time_t tTmpTimePoint = mktime(&tTm);
		if (tTmpTimePoint != -1)
		{
			int64_t nTick = (int64_t)getTickCount();
			int64_t nDiff = ((int64_t)tTmpTimePoint - (int64_t)m_tBaseTime) * 1000;
			m_nTickCount = nTick + nDiff;
            bValid = true;
			return toISOString() ? true : false;
		}
		return false;
	}
    bValid = false;
	return false;
}

const char *CTickPoint::toISOString()
{
	std::time_t tTimePt = getTime();
	std::tm *pTime = std::localtime(&tTimePt);
	if (pTime)
	{
		sprintf(chTimePoint, "%04d%02d%02dT%02d%02d%02d",
			pTime->tm_year + 1900,
			pTime->tm_mon + 1,
			pTime->tm_mday,
			pTime->tm_hour,
			pTime->tm_min,
			pTime->tm_sec);
		return chTimePoint;
	}
	else
		return NULL;
}



CPeriod::CPeriod(const char *pStart, const char *pEnd)
{
	setDurationFromISOString(pStart, pEnd);
	//P7_REGISTER_MODULE(TM("Period"),&l_pModule,&l_pModuleConsole)
}

CPeriod::CPeriod(int nSeconds)
{
	setDurationFromNow(nSeconds);
}

CPeriod::~CPeriod()
{

}


bool CPeriod::setDurationFromNow(int nSeconds)
{
	char chBuffer[256];
	CTickPoint tStart;
	CTickPoint tEnd = CTickPoint(nSeconds);
	if (tStart.isValid() && tEnd.isValid())
	{
		m_tStart = tStart;
		m_tEnd = tEnd;
		//printf("Period %s~%s\n",m_tStart.toISOString(),m_tEnd.toISOString());
		bValid = true;
		return true;
	}
	bValid = false;
	return false;
}

bool CPeriod::setDurationFromISOString(const char *pStart, const char *pEnd)
{
	CTickPoint tPoint1(pStart);
	CTickPoint tPoint2(pEnd);

	if (tPoint1.isValid()&&tPoint2.isValid()&&(tPoint1 < tPoint2))
	{
		m_tStart = tPoint1;
		m_tEnd = tPoint2;
		bValid = true;
		return true;
	}
	bValid = false;
	return false;
}

D_STATE CPeriod::isInPeriod(const char *pPoint)
{
	char chBuffer[256];
	std::shared_ptr<CTickPoint> pTmp;
	if (pPoint)
		pTmp = std::shared_ptr<CTickPoint>(new CTickPoint(pPoint));
	else
    {
		pTmp = std::shared_ptr<CTickPoint>(new CTickPoint(0));
    }
	if (pTmp->isValid() && m_tStart.isValid() && m_tEnd.isValid())
	{
		if ((m_tStart <= (*pTmp.get())) && (m_tEnd >= (*pTmp.get())))
		{
			//TRACE_P7(l_pModule, l_pModuleConsole, TM("InDuration S%S C%S E%S"),m_tStart.toISOString(),pTmp->toISOString(),m_tEnd.toISOString());
			return D_IN;//InDuration
		}
		if (m_tStart > (*pTmp.get()))
		{
			printf("%s %s %s\n", m_tStart.toISOString(), pTmp->toISOString(), m_tEnd.toISOString());
			//TRACE_P7(l_pModule, l_pModuleConsole, TM("YetToCome S%S C%S E%S"), m_tStart.toISOString(), pTmp->toISOString(), m_tEnd.toISOString());
			return D_YET_TO_COME;//Future
		}
		if (m_tEnd < (*pTmp.get()))
		{
			//TRACE_P7(l_pModule, l_pModuleConsole, TM("Expired S%S C%S E%S"), m_tStart.toISOString(), pTmp->toISOString(), m_tEnd.toISOString());
			return D_EXPIRED;//Expired
		}
	}
	return D_NOTVALID;//Failed
}
