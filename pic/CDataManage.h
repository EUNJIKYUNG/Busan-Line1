#ifndef DATAMANAGE
#define DATAMANAGE
#include <memory>
#include <atomic>
#include <queue>
#include <mutex>



#include "defineCommon.h"


class CDataManage
{
public:
	CDataManage();
	~CDataManage();
	static CDataManage *GetInstance();
	void InitDataManage();
	int setDestinationImage(int nStartPage, int nIndex);
	bool setFDIImage(int nPage, int nIndex);
	bool setTrainNumber(int nPage, const char *pTrainNumber);
	void setUpdateFlagFDIImage();
	void toggleFDISync();
	void setFDISync(bool bSync);
	void clearFDIImage(int nPage);

	void setPII1Image(int nPage, int nIndex);
	void setUpdateFlagPII1Image();
	void togglePII1Sync();
	bool clearPII1Image(int nPage);
	void setPII1Mode(bool bText);
	void setPII1Text(char *pText, int nLen);
	bool getPII1Mode();
	void setPII1Text(unsigned short *pText, int nLen);


	void setPII2Image(int nPage, int nIndex);
	void setUpdateFlagPII2Image();
	void togglePII2Sync();
	bool clearPII2Image(int nPage);
	void setPII2Mode(bool bText);
	void setPII2Text(char *pText, int nLen);
	bool getPII2Mode();
	void setPII2Text(unsigned short *pText, int nLen);

	void SetCurrentTime(TIMEINFO *pInfo);
public:

private:
	bool m_bTextMode1;
	bool m_bTextMode2;
#ifdef _WIN32
public:
	void SetHWND(HWND hWnd);
	HBITMAP initBitmap();
	void UpdateBitmap();
private:
	void *m_pBuffer;
	HBITMAP m_hBitmap;
	HWND m_hWnd;
	void setBinary(int nSize, int nBpp, int nHeight, int nWidth, LPVOID lpBuffer);
	HBITMAP CreateBitmapByCreateDIBSection(HDC hdc, int cx, int cy, int bpp, LPVOID *pBuf);
#endif



};


#endif
