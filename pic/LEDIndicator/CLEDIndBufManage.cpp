#include "CLEDIndBufManage.h"
#include <string>
#include <chrono>
#include <algorithm>
#include <string.h>
#include "uni2ksc.h"

#define GETBITMAPOFFSET(WIDTH,BPP) ((((((unsigned int)WIDTH) *((unsigned int)BPP)) + 31) & ~31) >> 3)
#define GETBITMAPBUFSIZE(WIDTH,BPP) ((((((unsigned int)WIDTH) *((unsigned int)BPP)) + 7) & ~7) >> 3)


unsigned short CLEDIndBufManage::m_uCRC16tab[256] = {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};



CLEDIndBufManage::CLEDIndBufManage(unsigned int uWidth, unsigned int uHeight, unsigned int uBit, unsigned int uBlockSize, unsigned int uWholeSize)
	:m_uSocBitOffset(GETBITMAPOFFSET(uWidth, uBit)), m_uBlockSize(uBlockSize), m_uPageSize(uWidth*uHeight), m_uWholeSize(uWholeSize), m_uHeight(uHeight), m_uWidth(uWidth),
	m_uDesBitOffset(GETBITMAPBUFSIZE(uWidth, uBit)), m_uDelayPerPage(1), m_uCurPageCounter(0), m_uTextCRCBlock(0), m_uPIITextLength(0)
{
	m_bInUse = false;
	m_bUpdated = false;
	m_pBuffer = (unsigned char*)calloc(uWholeSize,1);
	m_pCRCBlock = (unsigned short*)calloc(uWholeSize / uBlockSize,sizeof(unsigned short));
	m_tElapsedTime = std::chrono::system_clock::now(); 
}


CLEDIndBufManage::~CLEDIndBufManage()
{
	if (m_pCRCBlock)
		free(m_pCRCBlock);
	if (m_pBuffer)
		free(m_pBuffer);
}


void CLEDIndBufManage::SetPageFromBuffer(int nPage, const unsigned char *pSoc)
{
	unsigned char *pDesBuf = m_pBuffer + (nPage* m_uPageSize);
	const unsigned char *pSocBuf = pSoc;
	for (unsigned int i=0; i < m_uHeight; i++)
	{
		memcpy(pDesBuf, pSocBuf, m_uDesBitOffset);
		pDesBuf += m_uDesBitOffset;
		pSocBuf += m_uSocBitOffset;
	}
	SetCRCPage(nPage);
	pushValidPage(nPage);
}


void CLEDIndBufManage::SetPageFromBuffer(int nPage, int nSocWidth, int nSocHeight,unsigned char *pSoc, int nDesLeft, int nDesTop)
{
	unsigned char *pDesBuf = m_pBuffer + (nPage* m_uPageSize)+ (m_uDesBitOffset*nDesTop)+nDesLeft;
	const unsigned char *pSocBuf = (pSoc);
	for(int i=0;i<nSocHeight;i++)
	{
		memcpy(pDesBuf, pSocBuf, nSocWidth);
		pSocBuf += nSocWidth;
		pDesBuf += m_uDesBitOffset;
	}
}

void CLEDIndBufManage::SetCalcCRC(int nPage)
{
	SetCRCPage(nPage);
	pushValidPage(nPage);
}

unsigned char* CLEDIndBufManage::GetBlock(unsigned int uBlock)
{
	return (unsigned char*)(m_pBuffer + (m_uBlockSize*uBlock));
}

unsigned int CLEDIndBufManage::GetBlockSize()
{
	return m_uBlockSize;
}

bool CLEDIndBufManage::ClearPage(int nPage)
{
	bool bChanged=true;
	unsigned char *pDesBuf = m_pBuffer + (nPage* m_uPageSize);
	memset(pDesBuf, 0, m_uPageSize);
	bChanged=clearCRCPage(nPage);
	clearValidPage(nPage);
	return bChanged;
	
}

bool CLEDIndBufManage::clearCRCPage(int nPage)
{
	bool bChanged=false;
	unsigned int uCurBlock = (nPage* m_uPageSize) / m_uBlockSize;
	unsigned int uBlockCount = m_uPageSize / m_uBlockSize;
	for (unsigned int i = 0; i < uBlockCount; i++)
	{
		if(m_pCRCBlock[uCurBlock])
			bChanged=true;
			printf("%d %08x\n",uCurBlock,m_pCRCBlock[uCurBlock]);
		m_pCRCBlock[uCurBlock] = 0;
		uCurBlock++;
	}
	return bChanged;
}


bool CLEDIndBufManage::getSync()
{
	return m_bSync;
}

void CLEDIndBufManage::toggleSync()
{
	m_bSync = !m_bSync;
}

void CLEDIndBufManage::setSync(bool bSync)
{
	m_bSync = bSync;
}

void CLEDIndBufManage::SetCRCPage(int nPage)
{
	unsigned int uCurBlock = (nPage* m_uPageSize) / m_uBlockSize;
	unsigned int uBlockCount = m_uPageSize / m_uBlockSize;
	for (unsigned int i = 0; i < uBlockCount; i++)
	{
		m_pCRCBlock[uCurBlock] = SetBlockCRC(uCurBlock);
		uCurBlock++;
	}
}

unsigned short CLEDIndBufManage::SetBlockCRC(int nBlock)
{
	unsigned char *pDesBuf = m_pBuffer + (nBlock* m_uBlockSize);
	return crc16_ccitt(pDesBuf, m_uBlockSize);
}

unsigned short CLEDIndBufManage::GetBlockCRC(int nBlock)
{
	return m_pCRCBlock[nBlock];
}

unsigned short CLEDIndBufManage::crc16_ccitt(const void *buf, int len)
{
	register int counter;
	register unsigned short crc = 0;
	unsigned char *pBuf = (unsigned char *)buf;
	for (counter = 0; counter < len; counter++)
	{
		crc = (crc << 8) ^ m_uCRC16tab[((crc >> 8) ^ *(pBuf++)) & 0x00FF];
	}
	return crc;
}

void CLEDIndBufManage::pushValidPage(uint32_t nPage)
{
	
	std::unique_lock<std::mutex> lock(m_uMutexPageCounter, std::defer_lock);
	lock.lock();
	auto it=find(m_vValidPages.begin(), m_vValidPages.end(), nPage);
	if (it == m_vValidPages.end())
	{
		m_vValidPages.push_back(nPage);
	}
	lock.unlock();
}
void CLEDIndBufManage::clearValidPage(uint32_t nPage)
{
	std::unique_lock<std::mutex> lock(m_uMutexPageCounter, std::defer_lock);
	lock.lock();
	auto it = find(m_vValidPages.begin(), m_vValidPages.end(), nPage);
	if (it != m_vValidPages.end())
	{
		it=m_vValidPages.erase(it);
	}
	if (m_vValidPages.size())
	{
		if (m_uCurPageCounter >= m_vValidPages.size())
			m_uCurPageCounter = (m_vValidPages.size() - 1);
	}
	else
		m_uCurPageCounter = 0;
	lock.unlock();
}

uint32_t CLEDIndBufManage::getCurrentPage()
{
	std::unique_lock<std::mutex> lock(m_uMutexPageCounter, std::defer_lock);
	lock.lock();
	if (std::chrono::system_clock::now() > m_tElapsedTime)
	{
		m_tElapsedTime = std::chrono::system_clock::now() +std::chrono::seconds(m_uDelayPerPage);
		m_uCurPageCounter++;
		if (m_vValidPages.size() <= m_uCurPageCounter)
		{
			m_uCurPageCounter=0;
		}
	}
	if (m_vValidPages.size())
	{
		if (m_vValidPages.size() <= m_uCurPageCounter)
		{
			m_uCurPageCounter = 0;
		}
		lock.unlock();
		return m_vValidPages.at(m_uCurPageCounter);
	}
	lock.unlock();
	return 0;
}


void CLEDIndBufManage::pushText(char *pChar, uint32_t uLen)
{
	m_qString.push(std::string(pChar, pChar+ uLen));
}

uint32_t CLEDIndBufManage::getPIIText(char *pText, uint32_t nBuffLen)
{
	if (m_qString.size())
	{
		uint32_t uLen = m_qString.front().size();
		memcpy(pText, m_qString.front().c_str(), std::min(uLen,nBuffLen));
		m_qString.pop();
		return uLen;
	}
	else
		return 0;
}

void CLEDIndBufManage::pushWText(unsigned short *pShort, uint32_t uLen)
{
	std::shared_ptr<unsigned short> pSentence = std::shared_ptr<unsigned short>((unsigned short*)malloc((uLen+1) * sizeof(unsigned short)), free);
	memset(pSentence.get(), 0, (uLen + 1) * sizeof(unsigned short));
	memcpy(pSentence.get(), pShort, uLen*sizeof(unsigned short));
	m_qWString.push(pSentence);
}

uint32_t CLEDIndBufManage::getPIIKSCConvText(char *pBuf, uint32_t uLen)
{
	int nCnt = 0;
	if (m_qWString.size())
	{
		conv_uni2ksc(m_qWString.front().get(),(uint8_t*)pBuf);
		while (pBuf[nCnt++]);
		m_qWString.pop();
		return nCnt * sizeof(uint8_t);
	}
	return 0;
}

uint32_t CLEDIndBufManage::getPIIWText(unsigned short *pShort, uint32_t uLen)
{
	/*int nCnt = 0;
	if (m_qWString.size())
	{
		
		conv_uni2ksc(m_qWString.front().get(), );
		while (m_qWString.front().get()[nCnt])
		{
			//pShort[nCnt] = ((m_qWString.front().get()[nCnt] >> 8) & 0x00FF) | ((m_qWString.front().get()[nCnt] << 8) & 0xFF00);
			conv_uni2ksc(m_qWString.front().get(), );
			
			unsigned int uUniCode = m_qWString.front().get()[nCnt];
			if (uUniCode < 128)
				pShort[nCnt] = uUniCode;
			else
			{
				int ksc = bsearch_unitab(uUniCode, uni2ksc, n_uni2ksc);
				if (ksc >= 0)
				{
					pShort[nCnt]=ksc;
				}
			}
			
			printf("%04x \n", pShort[nCnt]);
			nCnt++;
		}


		


		CHAR szBuffer[256];
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pShort, -1, szBuffer, 256, 0, 0);
		//memcpy(pShort, m_qWString.front().get(), nCnt);
		m_qWString.pop();
		return nCnt*sizeof(unsigned short);
	}*/
	return 0;
}

uint32_t CLEDIndBufManage::preparePIIText()
{
	uint32_t uRetLen = 0;
	uint32_t uTotLen = 0;
	do
	{
		uRetLen = getPIIKSCConvText((char*)m_pBuffer, m_uPageSize - uTotLen);
		uTotLen += uRetLen;
	} while (uRetLen);
	if (uTotLen)
	{
		m_uPIITextLength = uTotLen;
		m_uTextCRCBlock = crc16_ccitt(m_pBuffer, m_uPIITextLength);
		return uTotLen;
	}
	else	
	{ 
		memset(m_pBuffer, 0, m_uPageSize);
		m_uPIITextLength = 0;
		m_uTextCRCBlock = 0;
		return 0;
	}
}

uint16_t CLEDIndBufManage::GetTextBlockCRC()
{
	return m_uTextCRCBlock;
}

uint32_t CLEDIndBufManage::getTextBlockLength()
{
	return m_uPIITextLength;
}




std::shared_ptr<CLEDIndBufManage> CFDIIndBufManage::m_pManage;

void CFDIIndBufManage::SetInstance(CLEDIndBufManage *pManage)
{
	m_pManage = std::shared_ptr<CLEDIndBufManage>(pManage);
}
CLEDIndBufManage* CFDIIndBufManage::GetInstance()
{
	return m_pManage.get();
}


std::shared_ptr<CLEDIndBufManage> CPII1IndBufManage::m_pManage;

void CPII1IndBufManage::SetInstance(CLEDIndBufManage *pManage)
{
	m_pManage = std::shared_ptr<CLEDIndBufManage>(pManage);
}
CLEDIndBufManage* CPII1IndBufManage::GetInstance()
{
	return m_pManage.get();
}


std::shared_ptr<CLEDIndBufManage> CPII2IndBufManage::m_pManage;

void CPII2IndBufManage::SetInstance(CLEDIndBufManage *pManage)
{
	m_pManage = std::shared_ptr<CLEDIndBufManage>(pManage);
}
CLEDIndBufManage* CPII2IndBufManage::GetInstance()
{
	return m_pManage.get();
}
