#ifndef TIME_MANAGE
#define TIME_MANAGE

#include <ctime>
#include <stdint.h>

#define ONE_SECOND	1000
#define ONE_MINUTE  ONE_SECOND*60
#define ONE_HOUR	ONE_MINUTE*60
#define ONE_DAY		ONE_HOUR*24



typedef enum
{
	D_NOTVALID = -1,
	D_EXPIRED = 0,
	D_IN,
	D_YET_TO_COME,
}D_STATE;




class CTickPoint
{
public:
	CTickPoint();
	CTickPoint(int nSecondFromNow);
	CTickPoint(const CTickPoint &Other);
	CTickPoint(const char *pTime);
	virtual ~CTickPoint();
	bool isValid();
	bool fromISOString(const char *pString);
	const char *toISOString();

	std::time_t getTime() const;

	bool operator ==(CTickPoint &T);
	bool operator !=(CTickPoint &T);
	bool operator >(CTickPoint &T);
	bool operator >=(CTickPoint &T);
	bool operator <(CTickPoint &T);
	bool operator <=(CTickPoint &T);
	CTickPoint &operator =(const CTickPoint &Other);
public:
	bool bValid;
	static void setTime();
private:
	int64_t getTick() const;
	int64_t m_nTickCount;
	static std::time_t m_tBaseTime;
	static int64_t m_nBaseTick;
	char chTimePoint[16];
	char m_chTimePoint[16];
	
};

class CPeriod
{
public:
	CPeriod(const char *pStart, const char *pEnd);
	CPeriod(int nSeconds);
	virtual ~CPeriod();
	bool setDurationFromNow(int nSeconds);
	bool setDurationFromISOString(const char *pStart, const char *pEnd);
	D_STATE isInPeriod(const char *pPoint);
private:
	CTickPoint m_tStart;
	CTickPoint m_tEnd;
	bool bValid;
	void *l_pModule;
	void *l_pModuleConsole;
};





#endif
