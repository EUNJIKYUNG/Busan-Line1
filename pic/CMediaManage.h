#ifndef MEDIA_MANAGE
#define MEDIA_MANAGE
#include <memory>
#include <mutex>
#include <queue>
#include "MediaWrapper/CMediaWrapper.h"
#include "TablesDefine/DefineTables.h"

typedef enum _GraphType
{
	NOT_INIT,
	TYPE_VIDEO = 100,
	TYPE_STILLIMAGE = 200,
	TYPE_SHOCKWAVE = 300,
	TYPE_SHOCKWAVE_EVT1 = 301,
	TYPE_SHOCKWAVE_EVT2 = 302,
	TYPE_SHOCKWAVE_EVT3 = 303,
	TYPE_SHOCKWAVE_EVT10 = 310,
	TYPE_SHOCKWAVE_EVT32 = 332,
	TYPE_AUDIO = 400,
	PASSENGER_TIME = 500,
	PASSENGER_MESSAGE,
	PASSENGER_BLANK,
	PASSENGER_VIDEO,
	PASSENGER_NEXTST,
	PASSENGER_THISST,
	PASSENGER_DOOR,
	TYPE_SEPTA_PP = 600,
	TYPE_SEPTA_PP_TIME = 601,
}GraphType;

typedef struct
{
	int nContentsType;
	int nIndex;
	int nPPDuration;
}PPContents;


/*
struct findVideoContentsByText : public  std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findVideoContentsByText(const char *pszText)
	{
		strcpy(chFileName, pszText);
	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		VideoFilePool *pVF = (VideoFilePool*)pSI.get();
		return !strcmp(pVF->szFileName, chFileName);
	}
private:
	char chFileName[256];
};*/


class CMediaManage
{
public:
	CMediaManage();
	~CMediaManage();
	static CMediaManage *GetInstance();
	void SetCmdStatusBuffer(void *pCmd,void *pStatus);
	void PushPPSchedule(char *pPPSchedule);
	bool PopPPSchedule(char *pPPSchedule, int nLen);
	void PushAudioSchedule(char *pPPSchedule);
	bool PopAudioSchedule(char *pPPSchedule, int nLen);
	void TaskRoutine();
private:
	bool LoadAudioEventContents();
	bool LoadVideoEventContents();
	void SplitVideoSchedule(char *pSchedule, std::vector<PPContents> *pVector);
	void SplitAudioSchedule(char *pSchedule, std::vector<int> *pVector);
	void PlayFile(int nIndex, int nType, unsigned int uCurPos,int nDuration);
	std::mutex m_tMutexPP;
	std::mutex m_tMutexAudio;

	std::queue<std::string> m_qPP;
	std::queue<std::string> m_qAudio;

	long m_lSameCount;
	long m_lPrevProgress;
	CMediaWrapper *m_pWrapper;
	CVIDM m_tVIDM;
	CPPM m_tPPM;
	CAUDM m_tAUDM;
};

#endif

