#ifndef SCHEDULE
#define SCHEDULE
#include <memory>
#include "defineString.h"


class CPeriod;
class CContents;
class CContentsManage;

class CSchedule
{
public:
	CSchedule(int nPriority, int nContentsIndex);
	virtual ~CSchedule();
	int getPriority();
	int getIndex();
	const XCHAR* getContentsName();
	const int getType();
	const int getDuration();
	void setPosition(unsigned int uPosition);
	int isInPeriod();
	void setCurrentTimeStamp();
	unsigned int getPosition();
	bool operator ==(CSchedule &T);
	bool operator !=(CSchedule &T);
	bool operator >(CSchedule &T);
	bool operator >=(CSchedule &T);
	bool operator <(CSchedule &T);
	bool operator <=(CSchedule &T);
	virtual int initDuration(int nStartFromNow, CContentsManage *pCM) = 0;
protected:
	int m_nContentsIndex;
	int m_nPriority;
	unsigned int m_uPosition;
	std::shared_ptr<CPeriod> m_pPeriod;
	std::shared_ptr<CContents> m_pContents;
	uint64_t m_uInsertCount;

	static uint64_t g_uInsertCount;

};

struct ScheduleCompare {
	bool operator()(std::shared_ptr<CSchedule> &t, std::shared_ptr<CSchedule> &u)
	{
		return (*t.get()) < (*u.get());
	}
};


class CContents
{
public:
	CContents(int nIndex, XCHAR *pFileName, int nDuration, char *pStart, char *pEnd,int nType);
	CContents(int nIndex, int nDuration, char *pStart, char *pEnd, int nType);
	virtual ~CContents();
	const int getIndex() const;
	const int getType() const;
	const int getDuration() const;
	const XCHAR *getFileName() const;
	const std::shared_ptr<CPeriod>& getPeriod() const;
private:
	int m_nIndex;
	int m_nDuration;
	int m_nType;
	XCHAR chFileName[256];
	std::shared_ptr<CPeriod> m_pPeriod;
};


#endif
