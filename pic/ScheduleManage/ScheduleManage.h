#ifndef SCHEDULE_MANAGE
#define SCHEDULE_MANAGE
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>
#include <map>
#ifdef _WIN32
    #include <windows.h>
#endif
#include "Schedule.h"
#include "TimeRelated.h"

#include "PrivateMutex.h"
#include <string.h>
//#include "../CLogRecord.h"




class CConstantSchedule
	:public CSchedule
{
public:
	CConstantSchedule(int nPriority, int nContentsIndex);
	virtual ~CConstantSchedule();
	int initDuration(int nStartFromNow, CContentsManage *pCM);
};


class CEventSchedule
	:public CSchedule
{
public:
	CEventSchedule(int nPriority, int nContentsIndex);
	virtual ~CEventSchedule();
	int initDuration(int nStartFromNow, CContentsManage *pCM);
};


class CCustomEventSchedule
	:public CSchedule
{
public:
	CCustomEventSchedule(int nPriority, int nContentsIndex);
	virtual ~CCustomEventSchedule();
	int initDuration(int nStartFromNow, CContentsManage *pCM);
};



enum ePriority
{
	E_PRIORITY_OUTBOUND=100,
	E_PRIORITY_MAIN=200,
	E_PRIORITY_EVENT=300,
	E_PRIORITY_EMERGENCY=400,
};



struct findContentsByName : public std::unary_function<std::pair<int,std::shared_ptr<CContents>>, bool>
{
	findContentsByName(XCHAR *pName)
	{
		STRCPY(szName, pName);
	}
	bool operator ()(std::pair<int, std::shared_ptr<CContents>> pSI)
	{
		return STRCMP(szName, pSI.second->getFileName())?false:true;
	}
private:
	int m_nIndex;
	XCHAR szName[256];
};




class CContentsManage
{
public:
	CContentsManage()
	{
		//P7_REGISTER_MODULE(TM("ContentsManage"),&l_pModule,&l_pModuleConsole)
	}
	virtual ~CContentsManage()
	{
	}

	void *l_pModule;
	void *l_pModuleConsole;

	void addToOutboundScheuleDirect(XCHAR *pFileName, int nDuration, char *pStart, char *pEnd,int nFileType=0)
	{
		auto it=find_if(m_mContents.begin(), m_mContents.end(), findContentsByName(pFileName));
		if (it != m_mContents.end())
		{
			addToOutboundSchedule(it->first,false);
		}
		else
		{
			int nIndex = 10000000;
			if(m_mContents.size())
				nIndex = m_mContents.rbegin()->first + 1;
			addContents(nIndex, pFileName, nDuration, pStart, pEnd,nFileType);
			addToOutboundSchedule(nIndex,false);
			getValidSchedule();
		}
	}


	int addToEventScheduleDirect(XCHAR *pFileName, int nDuration, char *pStart, char *pEnd,int nTimeFromNow,int nFileType=0)
	{
		auto it = find_if(m_mContents.begin(), m_mContents.end(), findContentsByName(pFileName));
		if (it != m_mContents.end())
		{
			return addEventToSchedule(it->first, nTimeFromNow);
		}
		else
		{
			int nIndex = 1;
			if (m_mContents.size())
				nIndex = m_mContents.rbegin()->first + 1;
			addContents(nIndex, pFileName, nDuration, pStart, pEnd, nFileType);
			return addEventToSchedule(nIndex, nTimeFromNow,false);
		}
	}


	int findContents(XCHAR *pFileName)
	{
		auto it = find_if(m_mContents.begin(), m_mContents.end(), findContentsByName(pFileName));
		if (it != m_mContents.end())
		{
			return it->first;
		}
		return 0;
	}





	int addToEmergencyScheduleDirect(XCHAR *pFileName, int nDuration, char *pStart, char *pEnd, int nTimeFromNow, int nFileType=0)
	{
		auto it = find_if(m_mContents.begin(), m_mContents.end(), findContentsByName(pFileName));
		if (it != m_mContents.end())
		{
			return addToEmergencySchedule(it->first, nTimeFromNow);
		}
		else
		{
			int nIndex = m_mContents.rbegin()->first + 1;
			addContents(nIndex, pFileName, nDuration, pStart, pEnd, nFileType);
			return addToEmergencySchedule(nIndex, nTimeFromNow,false);
		}
	}


	void addContents(int nIndex, XCHAR *pFileName, int nDuration, char *pStart, char *pEnd,int nFileType=0)
	{
		auto it = m_mContents.find(nIndex);
		if(it==m_mContents.end())
			m_mContents[nIndex] = std::shared_ptr<CContents>(new CContents(nIndex, pFileName, nDuration, pStart, pEnd, nFileType));
	}

	void addCustomContents(int nIndex, int nDuration, char *pStart, char *pEnd, int nFileType)
	{
		m_mCustomContents[nIndex] = std::shared_ptr<CContents>(new CContents(nIndex, nDuration, pStart, pEnd, nFileType));
	}

	void addToOutboundSchedule(int nContentsIndex, bool bMain)
	{
		std::shared_ptr<CSchedule> schedule = std::shared_ptr<CSchedule>(new CConstantSchedule(bMain ? E_PRIORITY_MAIN : E_PRIORITY_OUTBOUND, nContentsIndex));
		schedule->initDuration(0, this);
		schedule->setCurrentTimeStamp();
		m_qSchedule.push(schedule);
	}

	int addEventToSchedule(int nContentsIndex,int nTimeFromNow,bool bRemove=true)
	{
		std::shared_ptr<CSchedule> schedule = std::shared_ptr<CSchedule>(new CEventSchedule(E_PRIORITY_EVENT, nContentsIndex));
		int nRetDuration=schedule->initDuration(nTimeFromNow, this);
		if (bRemove)
		{
			removeEventPriority();
		}
		schedule->setCurrentTimeStamp();
		m_qSchedule.push(schedule);
		return nRetDuration;
	}


	int addCustomEventToSchedule(int nContentsIndex, int nTimeFromNow, bool bRemove = true)
	{
		std::shared_ptr<CSchedule> schedule = std::shared_ptr<CSchedule>(new CCustomEventSchedule(E_PRIORITY_EVENT, nContentsIndex));
		int nRetDuration = schedule->initDuration(nTimeFromNow, this);
		if (bRemove)
		{
			removeEventPriority();
		}
		schedule->setCurrentTimeStamp();
		m_qSchedule.push(schedule);
		return nRetDuration;
	}



	int addToEmergencySchedule(int nContentsIndex, int nTimeFromNow, bool bRemove = true)
	{
		std::shared_ptr<CSchedule> schedule = std::shared_ptr<CSchedule>(new CConstantSchedule(E_PRIORITY_EMERGENCY, nContentsIndex));
		int nRetDuration = schedule->initDuration(nTimeFromNow,this);
		if (bRemove)
		{
			removeEmergencyPriority();
		}
		schedule->setCurrentTimeStamp();
		m_qSchedule.push(schedule);
		return nRetDuration;
	}

	void removeEventPriority()
	{
		while ((!m_qSchedule.empty()) && m_qSchedule.top()->getPriority() == E_PRIORITY_EVENT)
			m_qSchedule.pop();
	}

	void removeEmergencyPriority()
	{
		while ((!m_qSchedule.empty()) && (m_qSchedule.top()->getPriority() >= E_PRIORITY_EVENT) || (m_qSchedule.top()->getPriority() <= E_PRIORITY_EMERGENCY))
			m_qSchedule.pop();
	}

	void pushEvent(unsigned int nIndex)
	{
		m_tMutex.lock();
		m_qEvent.push(nIndex);
		m_tMutex.unlock();
	}

	unsigned int popEvent()
	{
		unsigned int uEvent = 0;
		m_tMutex.lock();
		while (m_qEvent.size())
		{
			uEvent = m_qEvent.front();
			m_qEvent.pop();
		}
		m_tMutex.unlock();
		return uEvent;
	}

	bool goToNextSchedule()
	{
		if (m_qSchedule.size())
		{
			//TRACE_P7(l_pModule,l_pModuleConsole, TM("Schedule queue : %d %u %d %u\n"), m_qSchedule.size(), m_qSchedule.top()->getIndex(), m_pTopSchedule->isInPeriod(), m_pTopSchedule->getIndex());
			//printf("Schedule queue : %d %u %d %u\n", m_qSchedule.size(), m_qSchedule.top()->getIndex(), m_pTopSchedule->isInPeriod(),m_pTopSchedule->getIndex());
			m_qSchedule.pop();
			int nRet = m_pTopSchedule->isInPeriod();
			if (nRet > 0)
			{
				m_pTopSchedule->setCurrentTimeStamp();
				m_qSchedule.push(m_pTopSchedule);
			}
			else if (nRet <=0)
			{
				m_pTopSchedule.reset();
				if (!m_qSchedule.size())
					return false;
			}
			return getValidSchedule();
		}
		else
			m_pTopSchedule.reset();
		return false;
	}

	bool getValidSchedule()
	{
		std::shared_ptr<CSchedule> pSchedule;
		int nScheduleCount = m_qSchedule.size();
		int nLoopCount = 0;
        //printf("%s %s %d \nSchedule Count %d\n",__FILE__,__FUNCTIONW__,__LINE__,nScheduleCount);
		//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
		while ((!pSchedule.get()) && !m_qSchedule.empty())
		{
            nLoopCount++; //printf("%s %s %d %d\n",__FILE__,__FUNCTIONW__,__LINE__,m_qSchedule.size());
            int nRetVal = m_qSchedule.top()->isInPeriod();
			//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
            //printf("%s %s %d %d\n",__FILE__,__FUNCTIONW__,__LINE__,nRetVal);
			switch (nRetVal)
			{
			case -1:
            case 0:
				
				//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"),__FILE__,__FUNCTIONW__,__LINE__);
                //printf("expired %s %u\n", m_qSchedule.top()->getContentsName(),m_qSchedule.top()->getIndex());


				//LOGI << "Expired! " << m_qSchedule.top()->getContentsName();
				m_qSchedule.pop();
				break;
            case 1:
				//
				//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
				//printf("%s %s %d \n",__FILE__,__FUNCTIONW__,__LINE__);
				pSchedule = m_qSchedule.top();
				break;
            case 2:
				
				//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
               //printf("%s %s %d \n",__FILE__,__FUNCTIONW__,__LINE__);
				if (nScheduleCount < nLoopCount)
				{
					//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
					//printf("%s %s %d \n", __FILE__, __FUNCTIONW__, __LINE__);
					//printf("not expired but removed %s %u\n", m_qSchedule.top()->getContentsName(), m_qSchedule.top()->getIndex());
                    //LOGI << "Not Expired but removed! " << m_qSchedule.top()->getContentsName();
					m_qSchedule.pop();
				}
				else
				{
					//TRACE_P7(l_pModule, l_pModuleConsole, TM("%s %s %d \n"), __FILE__, __FUNCTIONW__, __LINE__);
					//printf("%s %s %d \nSchedule Queue %d\n", __FILE__, __FUNCTIONW__, __LINE__, m_qSchedule.size());
					//printf("%08x",(unsigned int)m_qSchedule.top().get());
					std::shared_ptr<CSchedule> pSch = m_qSchedule.top();
					pSch->setCurrentTimeStamp();
					//m_qSchedule.top()->setCurrentTimeStamp();
					
					m_qSchedule.pop();
					m_qSchedule.push(m_qSchedule.top());
				}
				break;
			}
        }
		//printf("%s %s %d \n",__FILE__,__FUNCTIONW__,__LINE__);
	
		if (m_qSchedule.size())
		{
			std::shared_ptr<CSchedule> pSchedule = m_qSchedule.top();
			if (pSchedule)
            { 
				//printf("%s %s %d Schedule Queue %d\n",__FILE__,__FUNCTIONW__,__LINE__,m_qSchedule.size());
				m_pTopSchedule = pSchedule;
				return true;
			}
			m_pTopSchedule.reset();
        }
      // printf("%s %s %d \n",__FILE__,__FUNCTIONW__,__LINE__);
		return false;
	}

	void dumpScheduleQueue()
	{
		/*std::vector<std::shared_ptr<CSchedule>> vSchedule;
		while (m_qSchedule.size())
		{
			vSchedule.push_back(m_qSchedule.top());
			//printf("Name%S Index %d Priority %u\n", (wchar_t*)m_qSchedule.top()->getContentsName(), m_qSchedule.top()->getIndex(), m_qSchedule.top()->getPriority());
			m_qSchedule.pop();
		}

	


		for (auto it = vSchedule.begin(); it != vSchedule.end(); it++)
		{
			m_qSchedule.push(*it);
		}*/
	}

	void setCurrentPosition(unsigned int uTimePassed)
	{
		if (m_pTopSchedule.get())
		{
			m_pTopSchedule->getPosition();
			m_pTopSchedule->setPosition(uTimePassed);
		}
	}

	unsigned int getCurrentPosition()
	{
		if (m_pTopSchedule.get())
		{
			return m_pTopSchedule->getPosition();
		}
		return 0;
	}

	const XCHAR *getFileName()
	{
		if (m_pTopSchedule.get())
		{
			return m_pTopSchedule->getContentsName();
		}
		return NULL;
	}
	int getIndex()
	{
		if (m_pTopSchedule.get())
		{
			return m_pTopSchedule->getIndex();
		}
		return 0;
	}


	const int getType()
	{
		if (m_pTopSchedule.get())
		{
			return m_pTopSchedule->getType();
		}
		return 0;
	}

	const int getDuration()
	{
		if (m_pTopSchedule.get())
		{
			return m_pTopSchedule->getDuration();
		}
		return 0;
	}



	bool GetContents(int nIndex, std::shared_ptr<CContents> &pContents)
	{
		auto it = m_mContents.find(nIndex);
		if (it != m_mContents.end())
		{
			pContents = it->second;
			return true;
		}
		return false;
	}

	bool GetCustomContents(int nIndex, std::shared_ptr<CContents> &pContents)
	{
		auto it = m_mCustomContents.find(nIndex);
		if (it != m_mCustomContents.end())
		{
			pContents = it->second;
			return true;
		}
		return false;
	}

public:

	//std::mutex m_tMutex;
	Mutex m_tMutex;
	std::queue<unsigned int> m_qEvent;
	std::shared_ptr<CSchedule> m_pTopSchedule;
	std::map<int, std::shared_ptr<CContents>> m_mContents;
	std::map<int, std::shared_ptr<CContents>> m_mCustomContents;
    std::priority_queue < std::shared_ptr<CSchedule>, std::vector<std::shared_ptr<CSchedule>>, ScheduleCompare> m_qSchedule;
};



class CScheduleManage
{
public:

	
	CScheduleManage()
	{
	}
	virtual ~CScheduleManage()
	{

	}
	static CScheduleManage* GetInstance();

	CContentsManage m_tVideo;
	CContentsManage m_tAudio;
};



#endif
