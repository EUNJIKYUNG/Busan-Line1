#ifndef MEDIA_WRAPPER
#define MEDIA_WRAPPER
#include <memory>
#include "../defineMTX.h"


typedef enum mediaState
{
	ST_IDLE = 0,
	ST_WAITING_FOR_PLAY = 1,
	ST_PLAYING = 2,
	ST_WAITING_FOR_STOP = 3,
	ST_STOPPED = 4,
	ST_FINISHED=5,
}mediaState;


class CMediaWrapper
{
public:
	CMediaWrapper();
	~CMediaWrapper();
	virtual void PlayFile(const char *pFile,unsigned int uPosition)=0;
	virtual void Stop()=0;
	virtual bool HasError() = 0;
	virtual long GetPosition() = 0;
	virtual void SetBuffer(void *pBuffer)=0;
	virtual void SetCmdBuffer(void *pBuffer) = 0;
	virtual void GetState()=0;
	void SetMediaState(mediaState eState);
	mediaState GetMediaState();
private:
	bool m_bPlaying;
	mediaState m_tState;
};



class CVIDM
	:public CMediaWrapper
{
public:
	void PlayFile(const char *pFile, unsigned int uPosition);
	void Stop();
	long GetPosition();
	bool HasError();
	void SetBuffer(void *pBuffer);
	void SetCmdBuffer(void *pBuffer);
	void GetState();
private:
	char chFileName[256];
	MTX_2_CNCS *m_pMTX2CNCS;
	CNCS_2_MTX *m_pCNCS2MTX;
	
};

class CAUDM
	:public CMediaWrapper
{
public:
	void PlayFile(const char *pFile, unsigned int uPosition);
	void Stop();
	long GetPosition();
	bool HasError();
	bool Stopped();
	void SetBuffer(void *pBuffer);
	void SetCmdBuffer(void *pBuffer);
	void GetState();
private:
	char chFileName[256];
	MTX_2_CNCS *m_pMTX2CNCS;
	CNCS_2_MTX *m_pCNCS2MTX;
};

class CPPM
	:public CMediaWrapper
{
public:
	void PlayFile(const char *pFile, unsigned int uPosition);
	void Stop();
	long GetPosition();
	bool HasError();
	bool Stopped();
	void SetBuffer(void *pBuffer);
	void SetCmdBuffer(void *pBuffer);
	void SetDuration(long uDuration);
	void GetState();
private:
	int nIndex;
	MTX_2_CNCS *m_pMTX2CNCS;
	CNCS_2_MTX *m_pCNCS2MTX;
	unsigned int m_uInitTick;
	unsigned int m_uDuration;
};


#endif
