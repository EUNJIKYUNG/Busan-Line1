#include "CDataManage.h"
#include "TablesDefine/DefineTables.h"
#include "TablesDefine/TableManage.h"
#include "LEDIndicator/CLEDIndBufManage.h"
#include "LEDIndicator/defineLED.h"
#include "LEDIndicator/ColorPalette.h"
#include "ScheduleManage/ScheduleManage.h"
#include "CMediaManage.h"
#include "define336.h"

#include <time.h> 
#include <ctime>

#define BEGININFINITY	"19800101T000000"
#define ENDINFINITY		"20371230T232323"


//find PID contents by message code
struct findETNDIndicatorByCode : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findETNDIndicatorByCode(int nCode)
		:m_nCode(nCode)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		ETNDIndicator *pETNDIndicator = (ETNDIndicator*)GET_PTFROMSP(pSI);
		return (pETNDIndicator->nCode == m_nCode);
	}
private:
	int m_nCode;
};



CDataManage::CDataManage()
{

}


CDataManage::~CDataManage()
{
}

CDataManage * CDataManage::GetInstance()
{
	static CDataManage tManage;
	return &tManage;
}

void CDataManage::InitDataManage()
{
	CTableManage *pTM = CTableManage::GetInstance();
	CFDIIndBufManage::SetInstance(new CLEDIndBufManage(FDI_WIDTH, FDI_HEIGHT, 8, FDI_BLOCK_BYTE_COUNT, FDI_PAGE_BYTE_COUNT*6));
	CPII1IndBufManage::SetInstance(new CLEDIndBufManage(PII_WIDTH, PII_HEIGHT, 8, PII_BLOCK_BYTE_COUNT, PII_PAGE_BYTE_COUNT*6));
	CPII2IndBufManage::SetInstance(new CLEDIndBufManage(PII_WIDTH, PII_HEIGHT, 8, PII_BLOCK_BYTE_COUNT, PII_PAGE_BYTE_COUNT * 6));
	CTickPoint::setTime();
	CScheduleManage *pSM = CScheduleManage::GetInstance();
	for (auto ait : pTM->m_vVideoFilePool)
	{
		printf("%s %d\n",__FUNCTION__,__LINE__);
		VideoFilePool *pVideoFile=(VideoFilePool*)ait.get();
		printf("%s %d\n",__FUNCTION__,__LINE__);
		pVideoFile->GetIndex();
		printf("%s %d\n",__FUNCTION__,__LINE__);
		char chValue[256] = { 0 };
		sprintf(chValue, "V_%010d.%s", pVideoFile->GetIndex(),"mp4");
		printf("%s %s %d\n",chValue,__FUNCTION__,__LINE__);
		pSM->m_tVideo.addContents(pVideoFile->GetIndex(), chValue,pVideoFile->uDuration, (char*)BEGININFINITY,(char*)ENDINFINITY,TYPE_VIDEO);
		printf("%s %d\n",__FUNCTION__,__LINE__);
	}
    printf("Video File %d\n",pTM->m_vVideoFilePool.size());

	/*for (auto ait : pTM->m_vAudioFile)
	{
		AudioFile *pAudioFile = (AudioFile*)ait.get();
		pSM->m_tAudio.addContents(pAudioFile->GetIndex(), pAudioFile->szFileName, pAudioFile->uDuration, (char*)BEGININFINITY, (char*)ENDINFINITY, TYPE_AUDIO);
	}
    printf("Audio File %d\n",pTM->m_vAudioFile.size());*/

	
	std::vector<std::shared_ptr<CSQLData>>::iterator  xit=find_if(pTM->m_vVideoContents.begin(), pTM->m_vVideoContents.end(), findSQLData(1));
	if (xit != pTM->m_vVideoContents.end())
	{
		VideoContents *pVC=(VideoContents*)xit->get();
		for (auto rit = pVC->m_vChildItem[0].vSQLData.begin(); rit != pVC->m_vChildItem[0].vSQLData.end(); rit++)
		{
			VideoIndexList *pVIL=(VideoIndexList*)rit->get();
			pSM->m_tVideo.addToOutboundSchedule(pVIL->nContentsPoolIndex,false);
		}
		pSM->m_tVideo.getValidSchedule();
	}




	/*CWebMainServer *pWMS = CWebMainServer::GetInstance();
	pWMS->initServer();
	if (pSM->m_tVideo.getValidSchedule())
	{

		CDatagram::initEvent();

        CMTXHandler2 *pMTXHandler = new CMTXHandler2(30001, 100);
        CSNTPHandler *pSNTPHandler = new CSNTPHandler(30083, 1000);
        CInstantMessageHandler *pInstMsgHandler=new CInstantMessageHandler(30084,0);

        CPIDHandler *pPIDHandler = new CPIDHandler(30043, "239.194.0.155", "10.0.0.112,", 100, 64);
        CFDIIndicator *pFDIHandler = new CFDIIndicator(30013, "239.194.0.155", "10.0.0.112", 100, 64);
        CSDIIndicator *pSDIHandler = new CSDIIndicator(30023, "239.194.0.155", "10.0.0.112", 100, 64);
        CPIIIndicator *pPIIHandler = new CPIIIndicator(30033, "239.194.0.155", "10.0.0.112", 100, 64);
        CSIHandler *pSIHandler = new CSIHandler(30003, "239.194.0.155", "10.0.0.112", 100);
        CCNCSHandler *pCNCSHandler = new CCNCSHandler(30093, "239.193.0.1","10.0.0.112",100,64);

       // CDatagram::startDispatch();
	}*/
}






bool CDataManage::setTrainNumber(int nPage,const char *pTrainNumber)
{
	bool bRet = false;
	CTableManage *pTM = CTableManage::GetInstance();
	std::vector<std::shared_ptr<CSQLData>>::iterator xit,nit;
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();

    printf("CDataManage::setTrainNumber(%d,%s)\n", nPage, pTrainNumber);

	if ((!pFDI->m_bInUse) && (!pFDI->m_bUpdated))
	{
		int nLen=strlen(pTrainNumber);
		int nLeft=(FDI_WIDTH - (ETND_WIDTH*nLen))/2;
		printf("%s %d %u %u %s\n",__FUNCTION__,__LINE__,pFDI->m_bInUse.load(),pFDI->m_bUpdated.load(),pTrainNumber);
		for (int i = 0; i < nLen; i++)
		{
			nit=find_if(pTM->m_vETNDIndicator.begin(), pTM->m_vETNDIndicator.end(), findETNDIndicatorByCode(pTrainNumber[i]));
			if (nit != pTM->m_vETNDIndicator.end())
			{
				ETNDIndicator *pETND=(ETNDIndicator*)nit->get();
				pFDI->SetPageFromBuffer(nPage, pETND->nWidth, pETND->nHeight, pETND->pPixelData, nLeft, 0);
				nLeft += ETND_WIDTH;
				bRet = true;
			}
		}
		pFDI->SetCalcCRC(nPage);
	}
	printf("%s %d %u\n",__FUNCTION__,__LINE__,bRet);
	return bRet;
}


int CDataManage::setDestinationImage(int nStartPage, int nIndex)
{
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	std::vector<std::shared_ptr<CSQLData>>::iterator xit,zit,pit;
	if ((!pFDI->m_bInUse) && (!pFDI->m_bUpdated))
	{
		if (nIndex)
		{
			xit=find_if(pTM->m_vEDDIndex.begin(), pTM->m_vEDDIndex.end(), findSQLData(nIndex));
			if (xit != pTM->m_vEDDIndex.end())
			{
				for (zit = xit->get()->m_vChildItem[0].vSQLData.begin(); zit != xit->get()->m_vChildItem[0].vSQLData.end(); zit++)
				{
					EDDIndexList *pList = (EDDIndexList*)zit->get();
					if (!setFDIImage(nStartPage, pList->nEDDIndex))
					{
						clearFDIImage(nStartPage);
					}
					pFDI->SetCalcCRC(nStartPage);
					nStartPage++;
				}
				return nStartPage;
			}
		}
	}
	return 0;
}

bool CDataManage::setFDIImage(int nPage, int nIndex)
{
	bool bRet = false;
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	std::vector<std::shared_ptr<CSQLData>>::iterator pit;
	pit=find_if(pTM->m_vEDDIndicator.begin(),pTM->m_vEDDIndicator.end(),findSQLData(nIndex));
	if (pit != pTM->m_vEDDIndicator.end())
	{
		EDDIndicator *pEDDInd = (EDDIndicator*)pit->get();
		pFDI->SetPageFromBuffer(nPage,pEDDInd->pPixelData);
		bRet = true;
	}
	return bRet;
}

void CDataManage::clearFDIImage(int nPage)
{
	CLEDIndBufManage *pSDI = CFDIIndBufManage::GetInstance();
	pSDI->ClearPage(nPage);
}

void CDataManage::setUpdateFlagFDIImage()
{
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	if ((!pFDI->m_bInUse) && (!pFDI->m_bUpdated))
	{
		pFDI->m_bUpdated = true;
	}
}



void CDataManage::toggleFDISync()
{
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	pFDI->toggleSync();
}

void CDataManage::setFDISync(bool bSync)
{
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	pFDI->setSync(bSync);
}



void CDataManage::setPII1Image(int nPage, int nIndex)
{
	/*CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPIIIndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		if (nIndex)
		{
			auto it = find_if(pTM->m_vSDI_ML_INIT.begin(), pTM->m_vSDI_ML_INIT.end(), findSQLData(nIndex));
			if (it != pTM->m_vSDI_ML_INIT.end())
			{
				SDI_ML_INIT *pPIIFC = (SDI_ML_INIT*)it->get();
				pPII->SetPageFromBuffer(nPage, pPIIFC->pPixelData);
			}
		}
		else
			clearPIIImage(nPage);
	}*/
}

bool CDataManage::clearPII1Image(int nPage)
{
	CLEDIndBufManage *pSDI = CPII1IndBufManage::GetInstance();
	return pSDI->ClearPage(nPage);
}

void CDataManage::setUpdateFlagPII1Image()
{
	CLEDIndBufManage *pPII = CPII1IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->m_bUpdated = true;
	}
}

void CDataManage::togglePII1Sync()
{
	CLEDIndBufManage *pPII = CPII1IndBufManage::GetInstance();
	pPII->toggleSync();
}

void CDataManage::setPII1Mode(bool bText)		// True == Text False= Graphic
{
	m_bTextMode1 = bText;
}

void CDataManage::setPII1Text(char *pText,int nLen)
{
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPII1IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->pushText(pText, nLen);
	}
}

void CDataManage::setPII1Text(unsigned short *pText, int nLen)
{
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPII1IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->pushWText(pText, nLen);
	}
}

bool CDataManage::getPII1Mode()
{
	return m_bTextMode1;
}




void CDataManage::setPII2Image(int nPage, int nIndex)
{
	/*CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPIIIndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		if (nIndex)
		{
			auto it = find_if(pTM->m_vSDI_ML_INIT.begin(), pTM->m_vSDI_ML_INIT.end(), findSQLData(nIndex));
			if (it != pTM->m_vSDI_ML_INIT.end())
			{
				SDI_ML_INIT *pPIIFC = (SDI_ML_INIT*)it->get();
				pPII->SetPageFromBuffer(nPage, pPIIFC->pPixelData);
			}
		}
		else
			clearPIIImage(nPage);
	}*/
}

bool CDataManage::clearPII2Image(int nPage)
{
	CLEDIndBufManage *pSDI = CPII2IndBufManage::GetInstance();
	return pSDI->ClearPage(nPage);
}

void CDataManage::setUpdateFlagPII2Image()
{
	CLEDIndBufManage *pPII = CPII2IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->m_bUpdated = true;
	}
}

void CDataManage::togglePII2Sync()
{
	CLEDIndBufManage *pPII = CPII2IndBufManage::GetInstance();
	pPII->toggleSync();
}

void CDataManage::setPII2Mode(bool bText)		// True == Text False= Graphic
{
	m_bTextMode2 = bText;
}

void CDataManage::setPII2Text(char *pText, int nLen)
{
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPII2IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->pushText(pText, nLen);
	}
}

void CDataManage::setPII2Text(unsigned short *pText, int nLen)
{
	CTableManage *pTM = CTableManage::GetInstance();
	CLEDIndBufManage *pPII = CPII2IndBufManage::GetInstance();
	if ((!pPII->m_bInUse) && (!pPII->m_bUpdated))
	{
		pPII->pushWText(pText, nLen);
	}
}

bool CDataManage::getPII2Mode()
{
	return m_bTextMode2;
}


void CDataManage::SetCurrentTime(TIMEINFO *pInfo)
{
	std::time_t rawtime;
	struct tm * timeinfo;

	std::time(&rawtime);
	timeinfo = localtime(&rawtime);
	pInfo->uYY = timeinfo->tm_year - 100;
	pInfo->uMM = timeinfo->tm_mon + 1;
	pInfo->uDD = timeinfo->tm_mday;
	pInfo->uHH = timeinfo->tm_hour;
	pInfo->umm = timeinfo->tm_min;
	pInfo->uSS = timeinfo->tm_sec;
}



#ifdef _WIN32

HBITMAP CDataManage::CreateBitmapByCreateDIBSection(HDC hdc, int cx, int cy, int bpp, LPVOID *pBuf)
{
	int i;
	unsigned int cbPalette;
	RGBQUAD *pPalette;
	HPALETTE hPal;
	PALETTEENTRY pe[256];
	unsigned char pbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256] = { 0 };
	if (bpp == 0) {
		bpp = GetDeviceCaps(hdc, BITSPIXEL);
	}
	cbPalette = (bpp > 8 ? 0 : 1 << bpp) * sizeof(RGBQUAD);
	unsigned int cbImageSize = ((((cx * bpp) + 31) & ~31) >> 3) * cy;
	((BITMAPINFOHEADER *)pbi)->biSize = sizeof(BITMAPINFOHEADER);
	((BITMAPINFOHEADER *)pbi)->biWidth = cx;
	((BITMAPINFOHEADER *)pbi)->biHeight = -cy;
	((BITMAPINFOHEADER *)pbi)->biPlanes = 1;
	((BITMAPINFOHEADER *)pbi)->biBitCount = bpp;
	((BITMAPINFOHEADER *)pbi)->biCompression = BI_RGB;
	//((BITMAPINFOHEADER *)pbi)->biSizeImage = cbImageSize;
	pPalette = (RGBQUAD *)((unsigned char *)pbi + sizeof(BITMAPINFOHEADER));

	switch (bpp)
	{
	case 1:
		memset(pPalette, 0, sizeof(RGBQUAD) * 2);
		for (int i = 0; i < 2; i++)
		{
			pPalette[i].rgbRed = g1BppColorTable[i][0];
			pPalette[i].rgbGreen = g1BppColorTable[i][1];
			pPalette[i].rgbBlue = g1BppColorTable[i][2];
			pPalette[i].rgbReserved = 0;
		}
		break;
	case 2:
		memset(pPalette, 0, sizeof(RGBQUAD) * 16);
		for (int i = 0; i < 2; i++)
		{
			pPalette[i].rgbRed = g2BppColorTable[i][0];
			pPalette[i].rgbGreen = g2BppColorTable[i][1];
			pPalette[i].rgbBlue = g2BppColorTable[i][2];
			pPalette[i].rgbReserved = 0;
		}
		break;
	case 4:
		memset(pPalette, 0, sizeof(RGBQUAD) * 16);
		for (i = 0; i < 16; i++)
		{
			pPalette[i].rgbRed = g4BppColorTable[i][0];
			pPalette[i].rgbGreen = g4BppColorTable[i][1];
			pPalette[i].rgbBlue = g4BppColorTable[i][2];
			pPalette[i].rgbReserved = 0;
		}
		break;
	case 8:
		for (i = 0; i < 256; i++) {
			pPalette[i].rgbRed = g8BppColorTable[i][0];
			pPalette[i].rgbGreen = g8BppColorTable[i][1];
			pPalette[i].rgbBlue = g8BppColorTable[i][2];
			pPalette[i].rgbReserved = 0;
		}
		break;
	}
	return CreateDIBSection(hdc, (BITMAPINFO *)pbi, DIB_RGB_COLORS, (void**)(pBuf), NULL, 0);
}


void CDataManage::setBinary(int nSize, int nBpp, int nWidth, int nHeight, LPVOID lpBuffer)
{
	HDC hRefDC;
	HBITMAP hBitmap;
	unsigned int uBitmapSize = ((((nWidth *nBpp) + 31) & ~31) >> 3)*nHeight;
	if (lpBuffer && (nSize == uBitmapSize))
	{
		memcpy(m_pBuffer, lpBuffer, nSize);
		InvalidateRect(m_hWnd, 0, FALSE);
	}
}

HBITMAP CDataManage::initBitmap()
{
	HDC hRefDC = GetDC(NULL);
	m_hBitmap = CreateBitmapByCreateDIBSection(hRefDC, FDI_WIDTH, FDI_HEIGHT*6, 8, &m_pBuffer);
	ReleaseDC(NULL, hRefDC);
	return m_hBitmap;
}

void CDataManage::SetHWND(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CDataManage::UpdateBitmap()
{
	CLEDIndBufManage *pFDI = CFDIIndBufManage::GetInstance();
	setBinary(FDI_WIDTH*FDI_HEIGHT*6,8, FDI_WIDTH, FDI_HEIGHT*6, pFDI->GetBlock(0));
}
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
