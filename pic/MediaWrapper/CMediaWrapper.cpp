#include "CMediaWrapper.h"
#include <string.h>
//#include <plog/Log.h>

CMediaWrapper::CMediaWrapper()
	:m_bPlaying(false), m_tState(ST_STOPPED)
{
}


CMediaWrapper::~CMediaWrapper()
{
}

void CMediaWrapper::SetMediaState(mediaState eState)
{
	m_tState = eState;
}

mediaState CMediaWrapper::GetMediaState()
{
	return m_tState;
}

void CVIDM::PlayFile(const char *pFile, unsigned int uPosition)
{
    DBG_LINE
	if((GetMediaState()==ST_STOPPED)||(GetMediaState()==ST_IDLE))
	{
		strcpy((char*)m_pCNCS2MTX->uVideoFileName, pFile);
		m_pCNCS2MTX->uVideoPlayPosition[0] = uPosition & 0xFF;
		m_pCNCS2MTX->uVideoPlayPosition[1] = (uPosition >> 8) & 0xFF;
		m_pCNCS2MTX->uVideoPlayPosition[2] = (uPosition >> 16) & 0xFF;
		m_pCNCS2MTX->uVideoPlayPosition[3] = (uPosition >> 24) & 0xFF;
		m_pCNCS2MTX->bVideoStop = false;
		m_pCNCS2MTX->bVideoPlay = true;
		strcpy(chFileName, pFile);
		SetMediaState(ST_WAITING_FOR_PLAY);
        printf("ST_WAITING_FOR_PLAY FOR %s\n",m_pCNCS2MTX->uVideoFileName);
	}

}
void CVIDM::Stop()
{
	switch(GetMediaState())
	{
	case ST_PLAYING:
	case ST_FINISHED:
		m_pCNCS2MTX->bVideoPlay = false;
		m_pCNCS2MTX->bVideoStop = true;
		SetMediaState(ST_WAITING_FOR_STOP);
        printf("ST_WAITING_FOR_STOP\n");
		break;
	case ST_WAITING_FOR_PLAY:
		m_pCNCS2MTX->bVideoPlay = false;
		m_pCNCS2MTX->bVideoStop = true;
		SetMediaState(ST_STOPPED);
         printf("ST_STOPPED\n");
		break;
	}
}

void CVIDM::GetState()
{
    if ((GetMediaState()!=ST_WAITING_FOR_PLAY)&&m_pMTX2CNCS->bVideoStandBy)
	{
		m_pCNCS2MTX->bVideoStop = false;
		SetMediaState(ST_STOPPED);
         printf("ST_STOPPED\n");
	}
	if ((GetMediaState() != ST_WAITING_FOR_STOP) && m_pMTX2CNCS->bVideoFinished)
	{
		m_pCNCS2MTX->bVideoStop = false;
		SetMediaState(ST_FINISHED);
        printf("ST_FINISHED\n");
	}
	if(m_pMTX2CNCS->bVideoPlaying)
	{
        if ((GetMediaState() == ST_WAITING_FOR_PLAY)&&(!strcmp((char*)m_pCNCS2MTX->uVideoFileName,(char*)m_pMTX2CNCS->uVideoFileName)))
		{
			SetMediaState(ST_PLAYING);
			memset(&m_pCNCS2MTX->uVideoPlayPosition, 0, sizeof(m_pCNCS2MTX->uVideoPlayPosition));
			m_pCNCS2MTX->bVideoPlay = 0;
            printf("ST_PLAYING\n");
		}
	}
	if (m_pMTX2CNCS->bVideoError)
	{
		if (GetMediaState()!=ST_STOPPED)
		{
			SetMediaState(ST_STOPPED);
			m_pCNCS2MTX->bVideoPlay = 0;
             printf("ST_STOPPED\n");
		}
    }
}


bool CVIDM::HasError()
{
	return m_pMTX2CNCS->bVideoError;
}

long CVIDM::GetPosition()
{
	unsigned int *pPosition =(unsigned int*)m_pMTX2CNCS->uVideoPlayPosition;
	return (long)(*pPosition);
}



void CVIDM::SetBuffer(void *pBuffer)
{
	m_pMTX2CNCS = (MTX_2_CNCS*)pBuffer;
}

void CVIDM::SetCmdBuffer(void *pBuffer)
{
	m_pCNCS2MTX = (CNCS_2_MTX*)pBuffer;
}




void CAUDM::PlayFile(const char *pFile, unsigned int uPosition)
{
	if ((GetMediaState() == ST_STOPPED) || (GetMediaState() == ST_IDLE))
	{
		strcpy((char*)m_pCNCS2MTX->uAudioFileName, pFile);
		m_pCNCS2MTX->uAudioPlayPosition[0] = uPosition & 0xFF;
		m_pCNCS2MTX->uAudioPlayPosition[1] = (uPosition >> 8) & 0xFF;
		m_pCNCS2MTX->uAudioPlayPosition[2] = (uPosition >> 16) & 0xFF;
		m_pCNCS2MTX->uAudioPlayPosition[3] = (uPosition >> 24) & 0xFF;
		m_pCNCS2MTX->bAudioPlay = true;
		m_pCNCS2MTX->bAudioStop = false;
		strcpy(chFileName, pFile);
		SetMediaState(ST_WAITING_FOR_PLAY);
	}

}
void CAUDM::Stop()
{
	printf("StopCalled!\n");
	if (GetMediaState() == ST_PLAYING)
	{
		printf("WAITING_FOR_STOP!\n");
		m_pCNCS2MTX->bAudioStop = true;
		m_pCNCS2MTX->bAudioPlay = false;
		SetMediaState(ST_WAITING_FOR_STOP);
		printf("140 MediaState %d\n", GetMediaState());
	}
	printf("140 MediaState %d\n", GetMediaState());
}


void CAUDM::GetState()
{
	/*if ((GetMediaState() != ST_WAITING_FOR_STOP) && m_pMTX2CNCS->bVideoFinished)
	{
		m_pCNCS2MTX->bVideoStop = false;
		SetMediaState(ST_FINISHED);
	}*/

	if ((GetMediaState() != ST_WAITING_FOR_PLAY) && m_pMTX2CNCS->bAudioStopped)
	{
		m_pCNCS2MTX->bAudioStop = false;
		SetMediaState(ST_STOPPED);
	}
	
	if (m_pMTX2CNCS->bAudioPlaying)
	{
		if (GetMediaState() == ST_WAITING_FOR_PLAY)
		{
			SetMediaState(ST_PLAYING);
			memset(&m_pCNCS2MTX->uAudioPlayPosition, 0, sizeof(m_pCNCS2MTX->uAudioPlayPosition));
			m_pCNCS2MTX->bAudioPlay = 0;
		}
	}
	if (m_pMTX2CNCS->bAudioError)
	{
		if (GetMediaState() != ST_STOPPED)
		{
			SetMediaState(ST_STOPPED);
			m_pCNCS2MTX->bAudioPlay = 0;
		}
	}
}


bool CAUDM::HasError()
{
	return m_pMTX2CNCS->bAudioError;
}

long CAUDM::GetPosition()
{
	unsigned int *pPosition = (unsigned int*)m_pMTX2CNCS->uAudioPlayPosition;
	return (long)(*pPosition);
}

void CAUDM::SetBuffer(void *pBuffer)
{
	m_pMTX2CNCS = (MTX_2_CNCS*)pBuffer;
}

void CAUDM::SetCmdBuffer(void *pBuffer)
{
	m_pCNCS2MTX = (CNCS_2_MTX*)pBuffer;
}




#include "../CDataManage.h"



void CPPM::PlayFile(const char *pFile, unsigned int uPosition)
{
	if ((GetMediaState() == ST_STOPPED) || (GetMediaState() == ST_IDLE))
	{
		int nIndex = atoi(pFile);
		CDataManage *pDM = CDataManage::GetInstance();
		//pDM->PushPPMidText(nIndex);
		printf("%d\n", nIndex);
		//m_uInitTick = CDataManage::get_tick_count();
		SetMediaState(ST_PLAYING);
		//printf("STATE to PLAYING\n");
	}
}
void CPPM::Stop()
{
	switch(GetMediaState())
	{
	case ST_PLAYING:
		{	
			printf("STATE from PLAYING to STOP\n");
			CDataManage *pDM = CDataManage::GetInstance();
			SetMediaState(ST_WAITING_FOR_STOP);
			m_uDuration = 0;
		}
		break;
	case ST_FINISHED:
		printf("STATE from FINISHED to STOP\n");
		SetMediaState(ST_STOPPED);
		break;
	}
}

void CPPM::GetState()
{
	/*if (CDataManage::get_tick_count() > (m_uInitTick + m_uDuration))
	{
		if((GetMediaState()== ST_PLAYING)||(GetMediaState()!=ST_STOPPED))
			SetMediaState(ST_FINISHED);
	}

	if (CDataManage::get_tick_count() < (m_uInitTick + m_uDuration))
	{
        printf("P");
		SetMediaState(ST_PLAYING);
	}*/
}

long CPPM::GetPosition()
{
	unsigned int uDuration =0; // CDataManage::get_tick_count() - m_uInitTick;
	return (uDuration < m_uDuration) ? uDuration : m_uDuration;
}

bool CPPM::HasError()
{
	return false;
}

void CPPM::SetBuffer(void *pBuffer)
{
	m_pMTX2CNCS = (MTX_2_CNCS*)pBuffer;
}

void CPPM::SetCmdBuffer(void *pBuffer)
{
	m_pCNCS2MTX = (CNCS_2_MTX*)pBuffer;
}

void CPPM::SetDuration(long uDuration)
{
	m_uDuration = uDuration;
}
