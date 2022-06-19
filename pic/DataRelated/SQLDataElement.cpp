#include "SQLDataElement.h"

#include <string>
#include <algorithm>
#ifdef _WIN32
	#include <Windows.h>
	#include <sqltypes.h>
	#include <sqlext.h>
	#include <tchar.h>
#endif
#include <sqlite3.h>



std::string UnicodeToUTF8(unsigned short *pWS)
{

	std::string s;
	int nCount = 0;
	char uBuffer[7] = { 0 };
	//char nTempBuffer[32] = { 0 };
	bool bExpended = false;
	while (pWS[nCount])
	{
		unsigned short wc = pWS[nCount];
		if (0 <= wc && wc <= 0x7f)
		{
			/*if (bExpended)
			{
				s.append("\"\"");
			}
			s += (char)wc;*/
			s += (char)wc;
			bExpended = false;
		}
		else if (0x80 <= wc && wc <= 0x7ff)
		{
			uBuffer[0] = (0xc0 | (wc >> 6));
			uBuffer[1] = (0x80 | (wc & 0x3f));
			uBuffer[2] = 0;
			//sprintf(nTempBuffer, "\\x%02x\\x%02x", uBuffer[0], uBuffer[1]);
			//s.append(nTempBuffer);
			s.append(uBuffer);
			bExpended = true;
		}
		else if (0x800 <= wc && wc <= 0xffff)
		{
			uBuffer[0] = (0xe0 | (wc >> 12));
			uBuffer[1] = (0x80 | ((wc >> 6) & 0x3f));
			uBuffer[2] = (0x80 | (wc & 0x3f));
			uBuffer[3] = 0;
			//sprintf(nTempBuffer, "\\x%02x\\x%02x\\x%02x", uBuffer[0], uBuffer[1], uBuffer[2]);
			//s.append(nTempBuffer);
			s.append(uBuffer);
			bExpended = true;

		}
		else if (0x10000 <= wc && wc <= 0x1fffff)
		{
			uBuffer[0] = (0xf0 | (wc >> 18));
			uBuffer[1] = (0x80 | ((wc >> 12) & 0x3f));
			uBuffer[2] = (0x80 | ((wc >> 6) & 0x3f));
			uBuffer[3] = (0x80 | (wc & 0x3f));
			uBuffer[4] = 0;
			//sprintf(nTempBuffer, "\\x%02x\\x%02x\\x%02x\\x%02x", uBuffer[0], uBuffer[1], uBuffer[2], uBuffer[3]);
			//s.append(nTempBuffer);
			s.append(uBuffer);
			bExpended = true;
		}
		else if (0x200000 <= wc && wc <= 0x3ffffff)
		{
			uBuffer[0] = (0xf8 | (wc >> 24));
			uBuffer[1] = (0x80 | ((wc >> 18) & 0x3f));
			uBuffer[2] = (0x80 | ((wc >> 12) & 0x3f));
			uBuffer[3] = (0x80 | ((wc >> 6) & 0x3f));
			uBuffer[4] = (0x80 | (wc & 0x3f));
			uBuffer[5] = 0;
			//sprintf(nTempBuffer, "\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x", uBuffer[0], uBuffer[1], uBuffer[2], uBuffer[3], uBuffer[4]);
			//s.append(nTempBuffer);
			s.append(uBuffer);
			bExpended = true;
		}
		else if (0x4000000 <= wc && wc <= 0x7fffffff)
		{
			uBuffer[0] = (0xfc | (wc >> 30));
			uBuffer[1] = (0x80 | ((wc >> 24) & 0x3f));
			uBuffer[2] = (0x80 | ((wc >> 18) & 0x3f));
			uBuffer[3] = (0x80 | ((wc >> 12) & 0x3f));
			uBuffer[4] = (0x80 | ((wc >> 6) & 0x3f));
			uBuffer[5] = (0x80 | (wc & 0x3f));
			uBuffer[6] = 0;
			//sprintf(nTempBuffer, "\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x", uBuffer[0], uBuffer[1], uBuffer[2], uBuffer[3], uBuffer[4], uBuffer[5]);
			//s.append(nTempBuffer);
			s.append(uBuffer);
			bExpended = true;
		}
		nCount++;
	}


	return s;

}


SQLDataElement::SQLDataElement()
	:m_bModified(false),m_uMaxSize(0),m_uSize(0)
{
}


SQLDataElement::~SQLDataElement()
{
}


unsigned int SQLDataElement::GetSize() const 
{
	return m_uSize;
}
unsigned int SQLDataElement::GetMaxSize() const 
{
	return m_uMaxSize;
}
bool SQLDataElement::IsModified()
{
	return m_bModified;
}
void SQLDataElement::SetModified()
{
	m_bModified = true;
}

void SQLDataElement::ResetModified()
{
	m_bModified = false;
}

#ifdef _WIN32
void SQLDataElement::ODBCSTMTErrorReport(HSTMT hStmt)
{
	SQLINTEGER error_name_length = 0;
	SQLSMALLINT error_description_length = 0;
	SQLTCHAR error_name[32] = { 0 }, error_message[512] = { 0 };
	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, error_name,
		&error_name_length, error_message, 512, &error_description_length);

	OutputDebugString(error_name);
	//OutputDebugString(_T("\t"));
	OutputDebugString(error_message);
	//OutputDebugString(_T("\n"));
	//MessageBox(NULL, error_message, error_name, MB_OK);
}

unsigned long SQLDataElement::ODBCGetColumnSize(HSTMT hStmt,unsigned short Column)
{
	int nType = SQL_C_DEFAULT;
	SQLTCHAR svColName[256] = L"";
	SWORD swCol = 0, swType = 0, swScale = 0, swNull = 0;
	unsigned long pcbColDef = 0;
	SQLDescribeCol(hStmt,            // Statement handle
		Column,             // ColumnNumber
		svColName,          // ColumnName
		sizeof(svColName), // BufferLength
		&swCol,             // NameLengthPtr
		&swType,            // DataTypePtr
		&pcbColDef,         // ColumnSizePtr
		&swScale,           // DecimalDigitsPtr
		&swNull);          // NullablePtr
	return pcbColDef;
}
#endif





SQLInt::SQLInt(unsigned int uValue)
	:m_pValue((unsigned int*)uValue)
{
	m_uMaxSize=m_uSize = sizeof(unsigned int);
}

SQLInt::~SQLInt() 
{
}

void SQLInt::SetData(void *pData, unsigned int nSize)
{
	unsigned int *pValue = (unsigned int*)pData;
	if (*pValue != (*m_pValue))
	{
		*m_pValue = *pValue;
		m_bModified = true;
	}
}

bool SQLInt::GetData(void *pData, unsigned int nSize)
{
	unsigned int *pInt = (unsigned int *)pData;
	*pInt = *m_pValue;
	return true;
}
#ifdef _WIN32
bool SQLInt::ODBCBindParam(HSTMT hStmt, int nCol) 
{
	cbCol = sizeof(SQLINTEGER);
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,
		0, 0, m_pValue, sizeof(SQLINTEGER), &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}

bool SQLInt::ODBCGetData(HSTMT hStmt, int nCol)
{
	//return true;
	cbCol = 0;
	SQLRETURN rc = SQLGetData(hStmt, nCol, SQL_C_LONG, m_pValue, 0, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}
#endif


bool SQLInt::SQL3BindParam(sqlite3_stmt *state, int nCol) 
{
	int rc = sqlite3_bind_int(state, nCol,(*m_pValue));
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}

bool SQLInt::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	(*m_pValue) = sqlite3_column_int(state, nCol);
	return true;
}

bool SQLInt::GetJSONData(std::string &strData)
{
	std::string strAppend = std::to_string(*m_pValue);
	strData.append(strAppend);
	return true;
}









SQLText::SQLText(unsigned int uValue, int nMaxSize)
	:m_pValue((char*)uValue)
{
	m_uMaxSize = nMaxSize;
	m_uSize = 0;
}
SQLText::~SQLText()
{

}
void SQLText::SetData(void *pData, unsigned int nSize)
{
	char *pChar = (char*)pData;
	if (strncmp(pChar, m_pValue, m_uMaxSize))
	{
		if (strlen(pChar) < m_uMaxSize)
			strcpy(m_pValue, pChar);
		else
			strncpy(m_pValue, pChar, m_uMaxSize - 1);
		m_uSize = strlen(m_pValue);
		m_bModified = true;
	}
}
bool SQLText::GetData(void *pData, unsigned int nSize)
{
	char *pChar = (char *)pData;
	if (nSize <= m_uSize)
	{
		strcpy(pChar, m_pValue);
		return true;
	}
	return false;
}
#ifdef _WIN32
bool SQLText::ODBCBindParam(HSTMT hStmt, int nCol) 
{
	cbCol = strlen(m_pValue);
	/*if (!cbCol)
		cbCol = SQL_NULL_DATA;*/
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
		(SQLINTEGER)(cbCol ? cbCol : 1), 0, m_pValue, cbCol, 0);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}

bool SQLText::ODBCGetData(HSTMT hStmt, int nCol)
{
	//return true;
	cbCol = 0;
	SQLRETURN rc = SQLGetData(hStmt, nCol, SQL_C_CHAR, m_pValue, m_uMaxSize, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}
#endif

bool SQLText::SQL3BindParam(sqlite3_stmt *state, int nCol) 
{
	int rc = sqlite3_bind_text(state, nCol,m_pValue,strlen(m_pValue),SQLITE_STATIC);
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}

bool SQLText::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	const unsigned char *lpStrRet = sqlite3_column_text(state, nCol);
	if (lpStrRet)
	{
		strcpy(m_pValue, (const char*)lpStrRet);
		return true;
	}
	return false;
}

bool SQLText::GetJSONData(std::string &strData)
{
	strData.append(m_pValue);
	return true;
}

/*bool SQLText::ODBCBindData(HSTMT hStmt, int nCol)
{
	cbCol = 0;
	SQLRETURN rc= SQLBindCol(hStmt, nCol, SQL_C_CHAR,m_pValue, m_uMaxSize, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}*/



SQLFloat::SQLFloat(unsigned int uValue)
	:m_pValue((float*)uValue)
{
	
}
SQLFloat::~SQLFloat()
{

}
void SQLFloat::SetData(void *pData, unsigned int nSize)
{
	float *pValue = (float*)pData;
	if (*pValue != (*m_pValue))
	{
		(*m_pValue) = (*pValue);
		m_bModified = true;
	}
}
bool SQLFloat::GetData(void *pData, unsigned int nSize)
{
	float *pfloat = (float *)pData;
	*pfloat = *m_pValue;
	return true;
}

#ifdef _WIN32
bool SQLFloat::ODBCBindParam(HSTMT hStmt, int nCol) 
{
	cbCol = sizeof(SQLREAL);
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL,
		0, 0, m_pValue, sizeof(SQLREAL), &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}

bool SQLFloat::ODBCGetData(HSTMT hStmt, int nCol)
{
	//return true;
	cbCol = 0;
	SQLRETURN rc = SQLGetData(hStmt, nCol, SQL_C_FLOAT, m_pValue, 0, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}
#endif

bool SQLFloat::SQL3BindParam(sqlite3_stmt *state, int nCol) 
{
	int rc=sqlite3_bind_double(state, nCol, (*m_pValue));
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}


bool SQLFloat::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	(*m_pValue) = sqlite3_column_double(state, nCol);
	return true;
}

bool SQLFloat::GetJSONData(std::string &strData)
{
	std::string strAppend = std::to_string(*m_pValue);
	strData.append(strAppend);
	return true;
}



/*bool SQLFloat::ODBCBindData(HSTMT hStmt, int nCol)
{
	cbCol = 0;
	SQLRETURN rc = SQLBindCol(hStmt, nCol, SQL_C_FLOAT, m_pValue, 0, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}*/






SQLDouble::SQLDouble(unsigned int uValue)
	:m_pValue((double*)uValue)
{
}
SQLDouble::~SQLDouble()
{
}
void SQLDouble::SetData(void *pData, unsigned int nSize)
{
	double *pValue = (double*)pData;
	if (*pValue != (*m_pValue))
	{
		(*m_pValue) = (*pValue);
		m_bModified = true;
	}
}
bool SQLDouble::GetData(void *pData, unsigned int nSize)
{
	double *pDouble = (double *)pData;
	*pDouble = *m_pValue;
	return true;
}
#ifdef _WIN32
bool SQLDouble::ODBCBindParam(HSTMT hStmt, int nCol) 
{
	cbCol = sizeof(SQLDOUBLE);
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
		0, 0, m_pValue, sizeof(SQLDOUBLE), &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}

bool SQLDouble::ODBCGetData(HSTMT hStmt, int nCol)
{
	//return true;
	cbCol = 0;
	SQLRETURN rc = SQLGetData(hStmt, nCol, SQL_C_DOUBLE, m_pValue, 0, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}
#endif

bool SQLDouble::SQL3BindParam(sqlite3_stmt *state, int nCol) 
{
	int rc=sqlite3_bind_double(state, nCol, (*m_pValue));
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}

bool SQLDouble::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	(*m_pValue) = sqlite3_column_double(state, nCol);
	return true;
}

bool SQLDouble::GetJSONData(std::string &strData)
{
	std::string strAppend = std::to_string(*m_pValue);
	strData.append(strAppend);
	return true;
}



/*bool SQLDouble::ODBCBindData(HSTMT hStmt, int nCol)
{
	cbCol = 0;
	SQLRETURN rc = SQLBindCol(hStmt, nCol, SQL_C_DOUBLE, m_pValue, 0, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}*/



SQLBLOB::SQLBLOB(unsigned int uValue, unsigned int uMaxSize)
	:m_pValue((unsigned char**)uValue)
{
	(*m_pValue) = 0;
	m_uMaxSize=2048*2048;
}
SQLBLOB::~SQLBLOB()
{
	if ((*m_pValue))
	{
		free((*m_pValue));
		(*m_pValue) = 0;
	}
}
void SQLBLOB::SetData(void *pData, unsigned int uSize)
{
	bool bChanged = false;
	if ((!m_uSize)&&uSize)
	{
		(*m_pValue)=(unsigned char*)malloc(uSize);
		m_uSize = uSize;
		bChanged = true;
	}
	else if (uSize&&m_uSize)
	{
		if (m_uSize != uSize)
		{
			(*m_pValue) = (unsigned char*)realloc((*m_pValue), uSize);
			m_uSize = uSize;
			bChanged = true;
		}
		else
		{
			bChanged = memcmp((*m_pValue), pData, m_uSize) ? true : false;
		}
	}
	else if(!uSize)
	{
		free(*m_pValue);
		(*m_pValue) = 0;
		m_uSize = 0;
	}
	if (bChanged)
	{
		memcpy((*m_pValue), pData, m_uSize);
		m_bModified = true;
	}
}

bool SQLBLOB::GetData(void *pData, unsigned int uSize)
{
	if (uSize <= m_uSize)
	{
		pData = (*m_pValue);
		//memcpy(pData, *m_pValue, uSize);
		return true;
	}
	return false;
}

void SQLBLOB::AllocData(unsigned int uSize)
{
	if ((!m_uSize) && uSize)
	{
		(*m_pValue) = (unsigned char*)malloc(uSize);
		m_uSize = uSize;
	}
	else if (uSize&&m_uSize)
	{
		if (m_uSize != uSize)
		{
			(*m_pValue) = (unsigned char*)realloc((*m_pValue), uSize);
			m_uSize = uSize;
		}
	}
	else if (!uSize)
	{
		free(*m_pValue);
		(*m_pValue) = 0;
		m_uSize = 0;
	}
}
#ifdef _WIN32
bool SQLBLOB::ODBCBindParam(HSTMT hStmt, int nCol)
{
	cbCol = m_uSize;
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
		m_uSize, 0, (*m_pValue), m_uSize, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}


bool SQLBLOB::ODBCGetData(HSTMT hStmt, int nCol)
{
	cbCol = 0;
	SQLCHAR       BinaryPtr[5000];
	SQLINTEGER    BinaryLenOrInd, NumBytes;
	SQLRETURN     rc;
	int nTotalSize=0,nMemoryPosition=0;
	while ((rc = SQLGetData(hStmt, nCol, SQL_C_BINARY, BinaryPtr, sizeof(BinaryPtr),&cbCol)) != SQL_NO_DATA)
	{
		if (cbCol > 0)
		{
			NumBytes = (cbCol > 5000) || (cbCol == SQL_NO_TOTAL) ? 5000 : cbCol;
			nTotalSize += NumBytes;
			AllocData(nTotalSize);
			unsigned char *pBuffer = (*m_pValue) + nMemoryPosition;
			nMemoryPosition += NumBytes;
			memcpy(pBuffer, BinaryPtr, NumBytes);
		}
	}
	return IS_SQL_OK(rc);
}
#endif

bool SQLBLOB::SQL3BindParam(sqlite3_stmt *state, int nCol)
{
	int rc = sqlite3_bind_blob(state, nCol, (*m_pValue), m_uSize, SQLITE_STATIC);
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}

bool SQLBLOB::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	unsigned char *pByte;
	int nBytes = sqlite3_column_bytes(state, nCol);
	if (nBytes)
	{
		AllocData(nBytes);
		memcpy((*m_pValue), sqlite3_column_blob(state, nCol), nBytes);
		return true;
	}
	return false;
}

bool SQLBLOB::GetJSONData(std::string &strData)
{
	return false;
}


int WCSCMP(const unsigned short *s1, const unsigned short *s2)
{
	unsigned short c1, c2;
	do
	{
		c1 = *s1++;
		c2 = *s2++;
		if (c2 == L'\0')
			return c1 - c2;
	} while (c1 == c2);
	return c1 < c2 ? -1 : 1;
}

unsigned short* WCSCPY(unsigned short *d, const unsigned short *s)
{
	unsigned short *a = d;
	while ((*d++ = *s++));
	return a;
}

int WCSNCMP(const unsigned short *s1, const unsigned short *s2, register size_t n)
{
	register unsigned short u1, u2;
	while (n-- > 0)
	{
		u1 = (unsigned short)*s1++;
		u2 = (unsigned short)*s2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}
	return 0;
}

unsigned short *WCSNCPY(unsigned short *dest, const unsigned short *src, size_t n)
{
	unsigned short *ret = dest;
	do {
		if (!n--)
			return ret;
	} while (*dest++ = *src++);
	while (n--)
		*dest++ = 0;
	return ret;
}


size_t WCSLEN(const unsigned short *str)
{
	size_t i = 0;
	while (str[i++]);
	return i;
}


SQLWText::SQLWText(unsigned int uValue, int nMaxSize)
	:m_pValue((unsigned short*)uValue)
{
	m_uMaxSize = nMaxSize;
	m_uSize = 0;
}
SQLWText::~SQLWText()
{

}
void SQLWText::SetData(void *pData, unsigned int nSize)
{
	unsigned short *pChar = (unsigned short *)pData;
	if (WCSNCMP(pChar, m_pValue, m_uMaxSize))
	{
		if (WCSLEN(pChar) < m_uMaxSize)
			WCSCPY(m_pValue, pChar);
		else
			WCSNCPY(m_pValue, pChar, m_uMaxSize - 1);
		m_uSize = WCSLEN(m_pValue);
		m_bModified = true;
	}
}
bool SQLWText::GetData(void *pData, unsigned int nSize)
{
	unsigned short  *pChar = (unsigned short  *)pData;
	if (nSize <= m_uSize)
	{
		WCSCPY(pChar, m_pValue);
		return true;
	}
	return false;
}

bool SQLWText::GetJSONData(std::string &strData)
{
	std::string strTemp=UnicodeToUTF8(m_pValue);
	if (strTemp.length())
	{
		strData.append("\"");
		strData.append(strTemp);
		strData.append("\"");
		return true;
	}

	/*char chBuffer[256];
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)m_pValue, -1, chBuffer, 256, 0, 0);
	std::string strTemp = std::string(chBuffer);
	if (strTemp.length())
	{
		strData.append("\"");
		strData.append(strTemp);
		strData.append("\"");
		return true;
	}*/

	return false;
}

#ifdef _WIN32
bool SQLWText::ODBCBindParam(HSTMT hStmt, int nCol)
{
	return true;
	cbCol = WCSLEN(m_pValue);
	/*if (!cbCol)
		cbCol = SQL_NULL_DATA;*/
	SQLRETURN rc = SQLBindParameter(hStmt, nCol, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
		(SQLINTEGER)(cbCol ? cbCol : 1), 0, m_pValue, cbCol, 0);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);
}

bool SQLWText::ODBCGetData(HSTMT hStmt, int nCol)
{
	return true;
	/*cbCol = 0;
	SQLRETURN rc = SQLGetData(hStmt, nCol, SQL_C_CHAR, m_pValue, m_uMaxSize, &cbCol);
	if (rc == -1)
		ODBCSTMTErrorReport(hStmt);
	return IS_SQL_OK(rc);*/
}
#endif

bool SQLWText::SQL3BindParam(sqlite3_stmt *state, int nCol)
{
	int rc = sqlite3_bind_text16(state, nCol, m_pValue, WCSLEN(m_pValue)*sizeof(unsigned short), SQLITE_STATIC);
	if (rc != SQLITE_ERROR)
		return false;
	return true;
}

bool SQLWText::SQL3GetData(sqlite3_stmt *state, int nCol)
{
	const unsigned short *lpStrRet = (unsigned short*)sqlite3_column_text16(state, nCol);
	if (lpStrRet)
	{
		WCSCPY(m_pValue, (const unsigned short*)lpStrRet);
		return true;
	}
	return false;
}
