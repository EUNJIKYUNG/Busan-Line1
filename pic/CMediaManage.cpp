#include "CMediaManage.h"
#include "ScheduleManage/ScheduleManage.h"
#include "TablesDefine/TableManage.h"
#include "CDataManage.h"
#include <ctime>
#include <cctype>




#define DEFAULT_TIME		_T("00000000000000")
#define DEFAULT_SCHEDULE	_T("00000000")
#define PLAYLIST_NUMBER		_T("NumberOfSchedule")
#define PLAYLIST_COUNT		_T("Count")
#define PLAYLIST_ORDER		_T("Order")
#define PLAYLIST_SCHEDULEID	_T("ScheduleID")
#define PLAYLIST_CONTENTSID _T("ContentsID")
#define PLAYLIST_BEGIN		_T("Begin")
#define PLAYLIST_END		_T("End")
#define PLAYLIST_PRIORITY	_T("Priority")
#define PLAYLIST_STARTTIME	_T("Start Time")
#define PLAYLIST_END_TIME	_T("End Time")
#define PLAYLIST_PATH		_T("Path")
#define PLAYLIST_VOLUME		_T("Volume")


#define BEGININFINITY		_T("19800101T000000")
#define ENDINFINITY			_T("20361230T232323")

#define BEGININFINITYA		"19800101T000000"
#define ENDINFINITYA		"20361230T232323"


#define CONTENTS_FILE		_T("Contents.ini")
#define SCHEDULE_FILE		_T("PlayList.ini")









CMediaManage::CMediaManage()
	:m_lSameCount(0), m_lPrevProgress(0)
{
	m_pWrapper = (CMediaWrapper*)(&m_tVIDM);;
	/*m_bVideoScheduleShifted = false;
	m_lPrevProgress=0;
	m_lSameCount=0;*/
}


CMediaManage::~CMediaManage()
{

}


CMediaManage *CMediaManage::GetInstance()
{
	static CMediaManage tManage;
	return &tManage;
}






void CMediaManage::PushPPSchedule(char *pPPSchedule)
{

	m_tMutexPP.lock();
	m_qPP.push(pPPSchedule);
	m_tMutexPP.unlock();
}

bool CMediaManage::PopPPSchedule(char *pPPSchedule,int nLen)
{
	m_tMutexPP.lock();
	if (m_qPP.size())
	{
		strcpy(pPPSchedule,m_qPP.front().c_str());
		m_qPP.pop();
		m_tMutexPP.unlock();
		return true;
	}
	m_tMutexPP.unlock();
	return false;
}



void CMediaManage::PushAudioSchedule(char *pPPSchedule)
{

	m_tMutexAudio.lock();
	m_qAudio.push(pPPSchedule);
	m_tMutexAudio.unlock();
}

bool CMediaManage::PopAudioSchedule(char *pPPSchedule, int nLen)
{
	m_tMutexAudio.lock();
	if (m_qAudio.size())
	{
		strcpy(pPPSchedule, m_qAudio.front().c_str());
		m_qAudio.pop();
		m_tMutexAudio.unlock();
		return true;
	}
	m_tMutexAudio.unlock();
	return false;
}




bool CMediaManage::LoadVideoEventContents()
{
	char chPPSchedule[256];
	std::vector< PPContents> vPP;
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	if (PopPPSchedule(chPPSchedule, 256))
	{
		pSM->m_tVideo.removeEventPriority();
		pSM->m_tVideo.getValidSchedule();
		printf("\n%s\n", chPPSchedule);
		SplitVideoSchedule(chPPSchedule, &vPP);
		int nTempDuration = 0;
		for (auto it = vPP.begin(); it != vPP.end(); it++)
		{
			if (it->nContentsType >= TYPE_SEPTA_PP)
			{
				pSM->m_tVideo.addCustomContents(it->nIndex,it->nPPDuration*1000,(char*) BEGININFINITYA, (char*)ENDINFINITYA,it->nContentsType);
				printf("Type %d Duration %d From Now \n", it->nContentsType, nTempDuration*1000);
				nTempDuration=pSM->m_tVideo.addCustomEventToSchedule(it->nIndex, nTempDuration*1000, false);
				pSM->m_tVideo.getValidSchedule();
				pSM->m_tVideo.getIndex();
			}
			else
			{	
				//
				nTempDuration = pSM->m_tVideo.addEventToSchedule(it->nIndex, nTempDuration, false);
				pSM->m_tVideo.getValidSchedule();
			}
		}
		return true;
	}
	else
		return false;
}



bool CMediaManage::LoadAudioEventContents()
{
	char chAudioSchedule[256];
	std::vector<int> vAudio;
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	if (PopAudioSchedule(chAudioSchedule, 256))
	{
		pSM->m_tAudio.removeEventPriority();
		pSM->m_tAudio.getValidSchedule();
		SplitAudioSchedule(chAudioSchedule, &vAudio);
		int nTempDuration = 0;
		for (auto it = vAudio.begin(); it != vAudio.end(); it++)
		{
			nTempDuration = pSM->m_tAudio.addEventToSchedule((*it), nTempDuration*1000, false);
			pSM->m_tAudio.getValidSchedule();
		}
		return true;
	}
	else
		return false;
}



void CMediaManage::SplitAudioSchedule(char *pSchedule, std::vector<int> *pVector)
{
	char *pToken = strtok(pSchedule, ";");
	while (pToken != NULL)
	{
		pVector->push_back(atoi(pToken));
		pToken = strtok(NULL, ";"); // C4996
	}
}




void CMediaManage::SplitVideoSchedule(char *pSchedule, std::vector<PPContents> *pVector)
{
	std::vector<char *> vTemp;
	char *pToken = strtok(pSchedule, ";");
	while (pToken != NULL)
	{
		vTemp.push_back(pToken);
		pToken = strtok(NULL, ";"); // C4996
	}
	CTableManage *pTM = CTableManage::GetInstance();
	for (auto pTemp : vTemp)
	{
		PPContents tPPContents = { 0 };
		pToken = strtok(pTemp, "=");
		while (pToken != NULL)
		{
			int i = 0;
			bool bIsString = false;
			while (pToken[i])
			{
				bIsString |= !std::isdigit((int)pToken[i]);
				i++;
			}
			bool bContentsExist = false;
			if (!bIsString)
			{
				tPPContents.nContentsType = TYPE_SEPTA_PP;
				tPPContents.nIndex = atoi(pToken);
				bContentsExist = true;
			}
			else if (!strcmp(pToken, "TIME"))
			{
				tPPContents.nContentsType = TYPE_SEPTA_PP_TIME;
				tPPContents.nIndex = -5;
				bContentsExist = true;
			}
			else
			{
				/*auto it = find_if(pTM->m_vVideoFilePool.begin(), pTM->m_vVideoFilePool.end(), findVideoContentsByText(pToken));
				if (it != pTM->m_vVideoFilePool.end())
				{
					tPPContents.nContentsType = TYPE_VIDEO;
					tPPContents.nIndex = (*it)->GetIndex();
					bContentsExist = true;
				}*/
			}
			pToken = strtok(NULL, "=");
			if (bContentsExist)
			{
				tPPContents.nPPDuration = atoi(pToken);
				pVector->push_back(tPPContents);
			}
			pToken = NULL;
		}
	}
}


void CMediaManage::SetCmdStatusBuffer(void *pCmd, void *pStatus)
{
	m_tVIDM.SetBuffer(pStatus);
	m_tVIDM.SetCmdBuffer(pCmd);

	m_tPPM.SetBuffer(pStatus);
	m_tPPM.SetCmdBuffer(pCmd);

	m_tAUDM.SetBuffer(pStatus);
	m_tAUDM.SetCmdBuffer(pCmd);
}

void CMediaManage::PlayFile(int nIndex, int nType, unsigned int uCurPos,int nDuration)
{
	CDataManage *pDM = CDataManage::GetInstance();
	CTableManage *pTM = CTableManage::GetInstance();
	char chBuffer[256];
	switch (nType)
	{
	case TYPE_VIDEO:
		{
			//pDM->PushPPMidText(0);
			m_pWrapper = &m_tVIDM;
			auto it=find_if(pTM->m_vVideoFilePool.begin(), pTM->m_vVideoFilePool.end(), findSQLData(nIndex));
			if (it != pTM->m_vVideoFilePool.end())
			{
				VideoFilePool *pVF = (VideoFilePool*)it->get();

				char chValue[256] = { 0 };
				sprintf(chValue, "V_%010d.mp4", pVF->GetIndex(),"mp4");
				m_pWrapper->PlayFile(chValue, uCurPos);
                //printf("Video File %s Type:%d From Position %u\n", pVF->szFileName, nType,uCurPos);
			}
		}
		break;
	case TYPE_SEPTA_PP:
		m_tPPM.SetDuration(nDuration);
		m_pWrapper = &m_tPPM;
		sprintf(chBuffer, "%d", nIndex);
		m_pWrapper->PlayFile(chBuffer, 0);
		break;
	case TYPE_SEPTA_PP_TIME:
		m_tPPM.SetDuration(nDuration);
		m_pWrapper = &m_tPPM;
		sprintf(chBuffer, "%d", -5);
		m_pWrapper->PlayFile(chBuffer, 0);
		break;
	}
}

void CMediaManage::TaskRoutine()
{
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	bool bRet = LoadVideoEventContents();
	if (bRet)
	{
		m_pWrapper->Stop();
    }
    m_pWrapper->GetState();

    switch (m_pWrapper->GetMediaState())
	{
	case ST_IDLE:
	case ST_STOPPED:
		{
			int nIndex = pSM->m_tVideo.getIndex();
			int nType = pSM->m_tVideo.getType();
			if (nIndex&&nType)
			{
				unsigned int uCurPos = pSM->m_tVideo.getCurrentPosition();
				PlayFile(nIndex, nType, uCurPos,pSM->m_tVideo.getDuration());
			}
			else
			{
				pSM->m_tVideo.setCurrentPosition(0);
				pSM->m_tVideo.goToNextSchedule();
			}
		}
		break;
	case ST_FINISHED:
		pSM->m_tVideo.setCurrentPosition(0);
		pSM->m_tVideo.goToNextSchedule();
		m_pWrapper->Stop();
		break;
	case ST_PLAYING:
		{
			long lPos = m_pWrapper->GetPosition();
			pSM->m_tVideo.setCurrentPosition(lPos);
			if (m_lPrevProgress == lPos)
				m_lSameCount++;
			else
				m_lSameCount = 0;
			if (m_lSameCount > 10)
			{
				pSM->m_tVideo.setCurrentPosition(0);
				pSM->m_tVideo.goToNextSchedule();
				m_pWrapper->Stop();
				m_lSameCount = 0;
			}
			m_lPrevProgress = lPos;
		}
		break;
	}
	bRet = LoadAudioEventContents();
	if (bRet)
    {
		m_tAUDM.Stop();
	}
	m_tAUDM.GetState();
	switch (m_tAUDM.GetMediaState())
	{
	case ST_IDLE:
	case ST_STOPPED:
	{
		pSM->m_tAudio.setCurrentPosition(0);
		pSM->m_tAudio.goToNextSchedule();
		int nIndex = pSM->m_tAudio.getIndex();
		int nType = pSM->m_tAudio.getType();
		if (nIndex&&nType)
		{
			unsigned int uCurPos = pSM->m_tAudio.getCurrentPosition();
			PlayFile(nIndex, nType, uCurPos, pSM->m_tAudio.getDuration());
		}
		else
		{
			pSM->m_tAudio.setCurrentPosition(0);
			pSM->m_tAudio.goToNextSchedule();
		}
	}
	break;

	case ST_PLAYING:
	{
		long lPos = m_tAUDM.GetPosition();
		pSM->m_tAudio.setCurrentPosition(lPos);
		if (m_lPrevProgress == lPos)
			m_lSameCount++;
		else
			m_lSameCount = 0;
		if (m_lSameCount > 10)
		{
			pSM->m_tAudio.setCurrentPosition(0);
			pSM->m_tAudio.goToNextSchedule();
			m_tAUDM.Stop();
			m_lSameCount = 0;
		}
		m_lPrevProgress = lPos;
	}
	break;
    }

}


