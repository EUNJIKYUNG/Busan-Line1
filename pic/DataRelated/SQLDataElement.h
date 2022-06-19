#ifndef SQLDATAELEMENT
#define SQLDATAELEMENT
#include <string.h>
#include <string>

#define IS_SQL_OK(res) (res==SQL_SUCCESS_WITH_INFO || res==SQL_SUCCESS)

struct sqlite3_stmt;
typedef void* HSTMT;

class SQLDataElement
{
public:
	SQLDataElement();
	~SQLDataElement();
	virtual void SetData(void *pData,unsigned int nSize=0) = 0;
	virtual bool GetData(void *pData, unsigned int nSize = 0) = 0;
	
	virtual unsigned int GetSize() const;
	virtual unsigned int GetMaxSize() const;
	virtual bool IsModified();
	virtual void ResetModified();
	virtual void SetModified();
#ifdef _WIN32
	static void ODBCSTMTErrorReport(HSTMT hStmt);
	virtual bool ODBCBindParam(HSTMT hStmt, int nCol) = 0;
	unsigned long ODBCGetColumnSize(HSTMT hStmt, unsigned short Column);
	virtual bool ODBCGetData(HSTMT hStmt, int nCol) = 0;
#endif
	virtual bool SQL3BindParam(sqlite3_stmt *state, int nCol) = 0;
	virtual bool SQL3GetData(sqlite3_stmt *state, int nCol) = 0;
	virtual bool GetJSONData(std::string &strData)=0;

	virtual bool operator ==(const SQLDataElement &R) = 0;

	virtual bool operator !=(const SQLDataElement &R)
	{
		return !((*this) == R);
	}

	virtual bool operator >(const SQLDataElement &R) = 0;

	virtual bool operator >=(const SQLDataElement &R)
	{
		return ((*this) == R) || ((*this) > R);
	}
	virtual bool operator <(const SQLDataElement &R)
	{
		return !((*this) >= R);
	}
	virtual bool operator <=(const SQLDataElement &R)
	{
		return !((*this) > R);
	}
	virtual SQLDataElement &operator =(const SQLDataElement &R) = 0;

	
protected:
	bool m_bModified;
	unsigned int m_uMaxSize;
	unsigned int m_uSize;
	long cbCol;

	
};

class SQLInt
	:public SQLDataElement
{
public:
	SQLInt(unsigned int uValue);
	~SQLInt();
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32
	bool ODBCBindParam(HSTMT hStmt, int nCol);
	bool ODBCGetData(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLInt *p=(SQLInt*)&R;
		return (*m_pValue) == (*(p->m_pValue));
	}

	bool operator >(const SQLDataElement &R)
	{
		SQLInt *p = (SQLInt*)&R;
		return (*m_pValue) > (*(p->m_pValue));
	}

	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLInt *p = (SQLInt*)&R;
		(*m_pValue)=(*p->m_pValue);
		m_uSize=p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}
	

private:
	unsigned int *m_pValue;
};

class SQLText
	:public SQLDataElement
{
public:
	SQLText(unsigned int uValue, int nMaxSize = 0);
	~SQLText();
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32
	bool ODBCGetData(HSTMT hStmt, int nCol);
	bool ODBCBindParam(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLText *p = (SQLText*)&R;
		return  !strcmp(m_pValue,p->m_pValue);
	}
	
	bool operator >(const SQLDataElement &R)
	{
		SQLText *p = (SQLText*)&R;
		return (strcmp(m_pValue,p->m_pValue)>0);
	}

	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLText *p = (SQLText*)&R;
		strcpy(m_pValue, p->m_pValue);
		m_uSize = p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}
	

private:
	char *m_pValue;
};

class SQLFloat
	:public SQLDataElement
{
public:
	SQLFloat(unsigned int uValue);
	~SQLFloat();
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32
	bool ODBCGetData(HSTMT hStmt, int nCol);
	bool ODBCBindParam(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLFloat *p = (SQLFloat*)&R;
		return (*m_pValue) == (*(p->m_pValue));
	}

	bool operator >(const SQLDataElement &R)
	{
		SQLFloat *p = (SQLFloat*)&R;
		return (*m_pValue) > (*(p->m_pValue));
	}

	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLFloat *p = (SQLFloat*)&R;
		(*m_pValue) = (*p->m_pValue);
		m_uSize = p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}
private:
	float *m_pValue;
};

class SQLDouble
	:public SQLDataElement
{
public:
	SQLDouble(unsigned int uValue);
	~SQLDouble();
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32	
	bool ODBCGetData(HSTMT hStmt, int nCol);
	bool ODBCBindParam(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLDouble *p = (SQLDouble*)&R;
		return (*m_pValue) == (*(p->m_pValue));
	}

	bool operator >(const SQLDataElement &R)
	{
		SQLDouble *p = (SQLDouble*)&R;
		return (*m_pValue) > (*(p->m_pValue));
	}

	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLDouble *p = (SQLDouble*)&R;
		(*m_pValue) = (*p->m_pValue);
		m_uSize = p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}
private:
	double *m_pValue;
};

class SQLBLOB
	:public SQLDataElement
{
public:
	SQLBLOB(unsigned int uValue,unsigned int uMaxSize=0);
	~SQLBLOB();
	void AllocData(unsigned int uSize);
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32
	bool ODBCGetData(HSTMT hStmt, int nCol);
	bool ODBCBindParam(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLBLOB *p = (SQLBLOB*)&R;
		return (GetSize()) == (R.GetSize());
	}
	
	bool operator >(const SQLDataElement &R)
	{
		SQLBLOB *p = (SQLBLOB*)&R;
		return (GetSize() > R.GetSize());
	}
	
	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLBLOB *p = (SQLBLOB*)&R;
		AllocData(p->m_uSize);
		memcpy((*m_pValue), (*p->m_pValue), p->m_uSize);
		m_uSize = p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}

private:
	unsigned char **m_pValue;

};



int WCSCMP(const unsigned short *s1, const unsigned short *s2);
unsigned short* WCSCPY(unsigned short *d, const unsigned short *s);
int WCSNCMP(const unsigned short *s1, const unsigned short *s2, register size_t n);
unsigned short *WCSNCPY(unsigned short *dest, const unsigned short *src, size_t n);
size_t WCSLEN(const unsigned short *str);


class SQLWText
	:public SQLDataElement
{
public:
	SQLWText(unsigned int uValue, int nMaxSize = 0);
	~SQLWText();
	void SetData(void *pData, unsigned int nSize = 0);
	bool GetData(void *pData, unsigned int nSize = 0);

	bool SQL3BindParam(sqlite3_stmt *state, int nCol);
	bool SQL3GetData(sqlite3_stmt *state, int nCol);
#ifdef _WIN32
	bool ODBCGetData(HSTMT hStmt, int nCol);
	bool ODBCBindParam(HSTMT hStmt, int nCol);
#endif
	bool GetJSONData(std::string &strData);
	bool operator ==(const SQLDataElement &R)
	{
		SQLWText *p = (SQLWText*)&R;
		return  !WCSCMP(m_pValue, p->m_pValue);
	}

	bool operator >(const SQLDataElement &R)
	{
		SQLWText *p = (SQLWText*)&R;
		return (WCSCMP(m_pValue, p->m_pValue) > 0);
	}

	SQLDataElement &operator =(const SQLDataElement &R)
	{
		SQLWText *p = (SQLWText*)&R;
		WCSCPY(m_pValue, p->m_pValue);
		m_uSize = p->m_uSize;
		m_uMaxSize = p->m_uMaxSize;
		m_bModified = true;
		return (*this);
	}

private:
	unsigned short *m_pValue;
};

std::string UnicodeToUTF8(unsigned short *pWS);


#endif