#ifndef VALUEMONITOR
#define VALUEMONITOR

class CValueMonitor
{
public:
	CValueMonitor();
	~CValueMonitor();
	virtual bool isChanged()=0;
	virtual void saveLatest()=0;
	virtual void setValue(void *pValue)=0;
	virtual void getValue(void *pValue) = 0;
	virtual void getNonZeroValue(void *pValue)=0;
	virtual void* getRealPointer()=0;

private:
	unsigned char *m_pBuf;
};


class CUIntValueMonitor
	:public CValueMonitor
{
public:
	CUIntValueMonitor()
	{

	}
	~CUIntValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	

	void setValue(void *pValue)
	{
		m_pValue = (unsigned int *)pValue;
	}

	void getValue(void *pValue)
	{
		unsigned int *pTmpValue=(unsigned int*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		unsigned int *pTmpValue=(unsigned int*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}


	void* getRealPointer()
	{
		return (void*)m_pValue;
	}


private:
	unsigned int *m_pValue;
	unsigned int m_nPreValue;
};


class CUCharValueMonitor
	:public CValueMonitor
{
public:
	CUCharValueMonitor()
	{

	}
	~CUCharValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	void setValue(void *pValue)
	{
		m_pValue = (unsigned char *)pValue;
	}

	void getValue(void *pValue)
	{
		unsigned char *pTmpValue = (unsigned char*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		unsigned char *pTmpValue=(unsigned char*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}


	
	void* getRealPointer()
	{
		return (void*)m_pValue;
	}
private:
	unsigned char *m_pValue;
	unsigned char m_nPreValue;
};


class CUShortValueMonitor
	:public CValueMonitor
{
public:
	CUShortValueMonitor()
	{

	}
	~CUShortValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	void setValue(void *pValue)
	{
		m_pValue = (unsigned short *)pValue;
	}

	void getValue(void *pValue)
	{
		unsigned short *pTmpValue = (unsigned short*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		unsigned short *pTmpValue=(unsigned short*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}


	void* getRealPointer()
	{
		return (void*)m_pValue;
	}
private:
	unsigned short *m_pValue;
	unsigned short m_nPreValue;
};


class CIntValueMonitor
	:public CValueMonitor
{
public:
	CIntValueMonitor()
	{

	}
	~CIntValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	void setValue(void *pValue)
	{
		m_pValue = (int *)pValue;
	}

	void getValue(void *pValue)
	{
		int *pTmpValue = (int*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		int *pTmpValue=(int*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}



	void* getRealPointer()
	{
		return (void*)m_pValue;
	}
private:
	int *m_pValue;
	int m_nPreValue;
};


class CCharValueMonitor
	:public CValueMonitor
{
public:
	CCharValueMonitor()
	{

	}
	~CCharValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	void setValue(void *pValue)
	{
		m_pValue = (char *)pValue;
	}

	void getValue(void *pValue)
	{
		char *pTmpValue = (char*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		char *pTmpValue=(char*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}


	void* getRealPointer()
	{
		return (void*)m_pValue;
	}
private:
	char *m_pValue;
	char m_nPreValue;
};


class CShortValueMonitor
	:public CValueMonitor
{
public:
	CShortValueMonitor()
	{

	}
	~CShortValueMonitor()
	{

	}
	bool isChanged()
	{
		return (m_nPreValue != (*m_pValue));
	}

	void saveLatest()
	{
		m_nPreValue = (*m_pValue);
	}

	void setValue(void *pValue)
	{
		m_pValue = (short *)pValue;
	}

	void getValue(void *pValue)
	{
		short *pTmpValue = (short*)pValue;
		(*pTmpValue) = (*m_pValue);
	}

	void getNonZeroValue(void *pValue)
	{
		short *pTmpValue=(short*)pValue;
		if((*m_pValue))
			(*pTmpValue) = (*m_pValue);
		else
			(*pTmpValue) = m_nPreValue;
	}


	void* getRealPointer()
	{
		return (void*)m_pValue;
	}


private:
	short *m_pValue;
	short m_nPreValue;
};


#endif