#ifndef LEDINDBUFMANAGE
#define LEDINDBUFMANAGE
#include <memory>
#include <atomic>
#include <vector>
#include <mutex>
#include <queue>
#include <string>


class CLEDIndBufManage
{
public:
	CLEDIndBufManage(unsigned int uWidth,unsigned int uHeight,unsigned int uBit,unsigned int uBlockSize,unsigned int uwholeSize);
	~CLEDIndBufManage();
	void SetPageFromBuffer(int nPage, const unsigned char *pSoc);
	void SetPageFromBuffer(int nPage, int nSocWidth, int nSocHeight, unsigned char *pSoc, int nDesLeft, int nDesTop);
	void SetCalcCRC(int nPage);
	bool ClearPage(int nPage);
	unsigned char* GetBlock(unsigned int uBlock);
	unsigned int GetBlockSize();
	unsigned short GetBlockCRC(int nBlock);
	unsigned short SetBlockCRC(int nBlock);
	uint32_t getPIIKSCConvText(char *pBuf, uint32_t uLen);
	unsigned int getPIIText(char *pText, uint32_t nLen);
	unsigned int getPIIWText(unsigned short *pShort, uint32_t uLen);
	void pushText(char *pChar, uint32_t nLen);
	void pushWText(unsigned short *pShort, uint32_t uLen);
	void toggleSync();
	bool getSync();
	void setSync(bool bSync);
	void pushValidPage(uint32_t nPage);
	void clearValidPage(uint32_t nPage);
	std::atomic_bool m_bInUse;
	std::atomic_bool m_bUpdated;
	std::atomic_bool m_bSync;
	std::mutex m_uMutexPageCounter;
	uint32_t getCurrentPage();
	uint32_t preparePIIText();
	uint16_t GetTextBlockCRC();
	uint32_t getTextBlockLength();
private:
	static unsigned short m_uCRC16tab[256];
	bool clearCRCPage(int nPage);
	void SetCRCPage(int nPage);
	unsigned short crc16_ccitt(const void *buf, int len);
	const int m_uHeight;
	const int m_uWidth;
	const int m_uSocBitOffset;
	const int m_uDesBitOffset;
	const int m_uBlockSize;
	const int m_uPageSize;
	const int m_uWholeSize;
	unsigned char *m_pBuffer;
	unsigned short *m_pCRCBlock;
	std::vector<uint32_t> m_vValidPages;
	std::queue<std::string> m_qString;
	std::queue<std::shared_ptr<unsigned short>> m_qWString;
	uint32_t m_uCurPageCounter;
	std::chrono::system_clock::time_point m_tElapsedTime;
	uint32_t m_uDelayPerPage;
	uint32_t m_uPIITextLength;
	unsigned short m_uTextCRCBlock;
};


class CFDIIndBufManage
{
public:
	static void SetInstance(CLEDIndBufManage *pManage);
	static CLEDIndBufManage* GetInstance();
private:
	static std::shared_ptr<CLEDIndBufManage> m_pManage;
};

class CPII1IndBufManage
{
public:
	static void SetInstance(CLEDIndBufManage *pManage);
	static CLEDIndBufManage* GetInstance();
private:
	static std::shared_ptr<CLEDIndBufManage> m_pManage;
};

class CPII2IndBufManage
{
public:
	static void SetInstance(CLEDIndBufManage *pManage);
	static CLEDIndBufManage* GetInstance();
private:
	static std::shared_ptr<CLEDIndBufManage> m_pManage;
};

#endif