
#include "SQLData.h"
#include <algorithm>
#include "SQLDataElement.h"


#define	MIN(X,Y) ((X)<(Y)?(X):(Y))
#define	MAX(X,Y) ((X)>(Y)?(X):(Y))

#define SQL_CREATE_STRING		"CREATE TABLE "
#define SQL_SELECT_STRING		"SELECT "
#define SQL_UPDATE_STRING		"UPDATE "
#define SQL_INSERT_STRING		"INSERT INTO "
#define SQL_DELETE_STRING		"DELETE FROM "
#define SQL_ALTER_STRING		"ALTER TABLE "

#define SQL_MAX			"MAX("
#define SQL_MAXEND		") FROM "
#define SQL_INSERT		"INSERT INTO "






CSQLData::CSQLData(EDITSTATE eState, int nChildCount)
	:m_eRecordState(eState), m_nChildCount(nChildCount)
{
	m_nTableIndex = 0;
}

CSQLData::~CSQLData(void)
{

}

void CSQLData::Initialize()
{
	for (int i = 0; i < m_nChildCount; i++)
	{
		CHILDITEM item;
		CEditSQLData *pEdit = this->GetEditor(i);
		m_vChildItem.push_back(item);
		if (pEdit)
		{
			pEdit->SetVectors(&m_vChildItem.at(i).vSQLData, &m_vChildItem.at(i).vSQLDataDelItems);
		}
	}
}


bool CSQLData::SetRecordState(EDITSTATE eStatus)
{
	bool bRet = false;
	switch (eStatus)
	{
	case EDIT_NOT_MODIFIED:
	case EDIT_UPDATED:
	case EDIT_INSERTED:
	case EDIT_DELETED:
	case EDIT_TO_BE_IGNORED:
		m_eRecordState = eStatus;
		bRet = true;
		break;
	}
	return bRet;
}

int CSQLData::GetRecordState()
{
	return m_eRecordState;
}

void CSQLData::SetIndex(int nIndex)
{
	m_nTableIndex = nIndex;
}

int CSQLData::GetIndex()
{
	return m_nTableIndex;
}

void CSQLData::GetSelectQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember)
{
	int nOrder = -1;
	memset((void*)lpStr, 0, nCount);
	strcat(lpStr, SQL_SELECT_STRING);
	for (int i = 0; i<nMember; i++)
	{
		switch (pSettings[i].nTYPE)
		{
		case TYPE_INTEGER:
		case TYPE_TEXT:
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_DATA:
		case TYPE_WTEXT:
			strcat(lpStr, pSettings[i].szFIELDNAME);
			break;
		}
		if (pSettings[i].bORDER)
		{
			nOrder = i;
		}

		if (i == (nMember - 1))
		{
			strcat(lpStr, " FROM ");
			strcat(lpStr, pszTableName);
		}
		else
		{
			strcat(lpStr, ",");
		}
	}
	if (nOrder != -1)
	{
		strcat(lpStr, (" ORDER BY "));
		strcat(lpStr, pSettings[nOrder].szFIELDNAME);
	}
}

void CSQLData::GetInsertQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember,std::shared_ptr<SQLDataElement> *pElement)
{
	memset((void*)lpStr, 0, nCount);
	strcat(lpStr, SQL_INSERT_STRING);
	strcat(lpStr, pszTableName);
	strcat(lpStr, (" "));
	int nInsertedColumn = 0;
	for (int i = 0; i < nMember; i++)
	{
		if (!i)
		{
			strcat(lpStr, ("("));
		}
		switch (pSettings[i].nTYPE)
		{
		case TYPE_INTEGER:
		case TYPE_TEXT:
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_DATA:
		case TYPE_WTEXT:
			nInsertedColumn++;
			strcat(lpStr, "[");
			strcat(lpStr, pSettings[i].szFIELDNAME);
			strcat(lpStr, "]");
			break;
		}
		if (i == (nMember - 1))
		{
			strcat(lpStr, (")"));
		}
		else
		{
			strcat(lpStr, (","));
		}
	}

	strcat(lpStr, (" VALUES "));
	for (int i = 0; i < nMember; i++)
	{
		if (!i)
		{
			strcat(lpStr, ("("));
		}
		if (i == (nMember - 1))
		{
			strcat(lpStr, ("?)"));
		}
		else
		{
			strcat(lpStr, ("?,"));
		}
	}
}

int CSQLData::GetChildCount()
{
	return m_nChildCount;
}

void CSQLData::GetUpdateQuery(char *lpStr, int nCount, char *pszTableName,  TYPESETTINGS *pSettings, int nMember, std::shared_ptr<SQLDataElement> *pElement)
{
	int nModifiedField = 0;
	char szTemp[256];
	memset((void*)lpStr, 0, nCount);
	strcat(lpStr, SQL_UPDATE_STRING);
	strcat(lpStr, "[");
	strcat(lpStr, pszTableName);
	strcat(lpStr, "]");
	strcat(lpStr, (" SET "));
	for (int i = 0; i < nMember; i++)
	{
		switch (pSettings[i].nTYPE)
		{
		case TYPE_INTEGER:
		case TYPE_TEXT:
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_DATA:
		case TYPE_WTEXT:
			if ((!pSettings[i].bPRIMARYKEY)&&pElement[i]->IsModified())
			{
				if(nModifiedField)
					strcat(lpStr, (","));
				strcat(lpStr, "[");
				strcat(lpStr, pSettings[i].szFIELDNAME);
				strcat(lpStr, "]");
				strcat(lpStr, ("= ? "));
				nModifiedField++;
			}
			break;
		}
		/*if (i == (nMember - 1))
		{
			strcat(lpStr,(")");
		}
		else
		{
			strcat(lpStr, (","));
		}*/
	}

	strcat(lpStr, (" WHERE "));
	for (int i = 0; i < nMember; i++)
	{
		if (pSettings[i].bPRIMARYKEY)
		{
			switch (pSettings[i].nTYPE)
			{
			case TYPE_INTEGER:
				strcat(lpStr, pSettings[i].szFIELDNAME);
				strcat(lpStr, ("= ? "));
				break;
			case TYPE_TEXT:
				strcat(lpStr, pSettings[i].szFIELDNAME);
				strcat(lpStr, ("= ? "));
				break;
			}
		}
	}
}

void CSQLData::GetDeleteQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember)
{
	char szTemp[256];
	memset((void*)lpStr, 0, nCount);
	strcat(lpStr, SQL_DELETE_STRING);
	strcat(lpStr, "[");
	strcat(lpStr, pszTableName);
	strcat(lpStr, "]");
	strcat(lpStr, (" WHERE "));
	for (int i = 0; i < nMember; i++)
	{
		if (pSettings[i].bPRIMARYKEY)
		{
			switch (pSettings[i].nTYPE)
			{
			case TYPE_INTEGER:
				strcat(lpStr, "[");
				strcat(lpStr, pSettings[i].szFIELDNAME);
				strcat(lpStr, "]");
				strcat(lpStr, ("= ? "));
				break;
			case TYPE_TEXT:
				strcat(lpStr, "[");
				strcat(lpStr, pSettings[i].szFIELDNAME);
				strcat(lpStr, "]");
				strcat(lpStr, ("= ? "));
				break;
			}
		}
	}
}
void CSQLData::GetMaxIndexQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember)
{
	memset(lpStr, 0, nCount);
	strcat(lpStr, SQL_SELECT_STRING);
	strcat(lpStr, SQL_MAX);
	for (int i = 0; i<nMember; i++)
	{
		if (pSettings[i].bPRIMARYKEY)
		{
			strcat(lpStr, pSettings[i].szFIELDNAME);
		}
	}
	strcat(lpStr, SQL_MAXEND);
	strcat(lpStr, pszTableName);
}

void CSQLData::GetCreateQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember)
{
	char szTemp[256];
	memset(lpStr, 0, nCount);
	strcat(lpStr, SQL_CREATE_STRING);
	strcat(lpStr, ("["));
	strcat(lpStr, pszTableName);
	strcat(lpStr, ("] "));
	for (int i = 0; i<nMember; i++)
	{
		if (!i)
		{
			strcat(lpStr, ("("));
		}
		switch (pSettings[i].nTYPE)
		{
		case TYPE_INTEGER:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
			strcat(lpStr, ("] INTEGER"));
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		case TYPE_TEXT:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
#ifdef _WIN32
			_snprintf(szTemp, 256, ("] NVARCHAR(%d)"), pSettings[i].nLENGTH);
#else
			snprintf(szTemp, 256, ("] NVARCHAR(%d)"), pSettings[i].nLENGTH);
#endif
			strcat(lpStr, szTemp);
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		case TYPE_WTEXT:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
#ifdef _WIN32
			_snprintf(szTemp, 256, ("] NVARCHAR(%d)"), pSettings[i].nLENGTH);
#else
			snprintf(szTemp, 256, ("] NVARCHAR(%d)"), pSettings[i].nLENGTH);
#endif
			strcat(lpStr, szTemp);
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		case TYPE_FLOAT:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
			strcat(lpStr, ("] FLOAT"));
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		case TYPE_DOUBLE:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
			strcat(lpStr, ("] DOUBLE"));
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		case TYPE_DATA:
			strcat(lpStr, ("["));
			strcat(lpStr, pSettings[i].szFIELDNAME);
			strcat(lpStr, ("] LONGBINARY"));
			if (pSettings[i].bPRIMARYKEY)
			{
				strcat(lpStr, (" PRIMARY KEY"));
			}
			break;
		}

		if (i == (nMember - 1))
		{
			strcat(lpStr, (")"));
		}
		else
		{
			strcat(lpStr, (","));
		}
	}
}

void CSQLData::GetAlterTableAddColumn(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember)
{
	char szTemp[256];
	memset(lpStr, 0, sizeof(char)*nCount);
	strcat(lpStr, SQL_ALTER_STRING);
	strcat(lpStr, pszTableName);
	strcat(lpStr, (" ADD "));
	switch (pSettings[nMember].nTYPE)
	{
	case TYPE_INTEGER:
		strcat(lpStr, (" "));
		strcat(lpStr, pSettings[nMember].szFIELDNAME);
		strcat(lpStr, (" INTEGER "));
		if (pSettings[nMember].bPRIMARYKEY)
		{
			strcat(lpStr, (" PRIMARYKEY "));
		}
		break;
	case TYPE_TEXT:
		strcat(lpStr, (" "));
		strcat(lpStr, pSettings[nMember].szFIELDNAME);
#ifdef _WIN32
		_snprintf(szTemp, 256, (" VARCHAR(%d) "), pSettings[nMember].nLENGTH);
#else
		snprintf(szTemp, 256, (" VARCHAR(%d) "), pSettings[nMember].nLENGTH);
#endif
		strcat(lpStr, szTemp);
		if (pSettings[nMember].bPRIMARYKEY)
		{
			strcat(lpStr, (" PRIMARYKEY "));
		}
		break;
	case TYPE_WTEXT:
		strcat(lpStr, (" "));
		strcat(lpStr, pSettings[nMember].szFIELDNAME);
#ifdef _WIN32
		_snprintf(szTemp, 256, (" NVARCHAR(%d) "), pSettings[nMember].nLENGTH);
#else
		snprintf(szTemp, 256, (" NVARCHAR(%d) "), pSettings[nMember].nLENGTH);
#endif
		strcat(lpStr, szTemp);
		if (pSettings[nMember].bPRIMARYKEY)
		{
			strcat(lpStr, (" PRIMARYKEY "));
		}
		break;
	case TYPE_FLOAT:
	case TYPE_DOUBLE:
		strcat(lpStr, (" "));
		strcat(lpStr, pSettings[nMember].szFIELDNAME);
		strcat(lpStr, (" FLOAT "));
		if (pSettings[nMember].bPRIMARYKEY)
		{
			strcat(lpStr, (" PRIMARYKEY "));
		}
		break;
	case TYPE_DATA:
		strcat(lpStr, (" "));
		strcat(lpStr, pSettings[nMember].szFIELDNAME);
		strcat(lpStr, (" LONGBINARY "));
		if (pSettings[nMember].bPRIMARYKEY)
		{
			strcat(lpStr, (" PRIMARYKEY "));
		}
		break;
	}
}



bool CSQLData::SetData(int nField, void *pValue, int nSize)
{
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);
	if (nField < nCount)
	{
		m_pDataElem[nField]->SetData(pValue, nSize);
		if (m_pDataElem[nField]->IsModified())
		{
			if (GetRecordState() == EDIT_NOT_MODIFIED)
				SetRecordState(EDIT_UPDATED);
		}
	}
	/*switch (pSettings[nField].nTYPE)
	{
	case TYPE_INTEGER:
	{
		int nValue = (*((int*)pValue));
		if (nValue != (*((int*)pSettings[nField].POINTER)))
		{
			(*((int*)pSettings[nField].POINTER)) = nValue;
			if (GetRecordState() == EDIT_NOT_MODIFIED)
				SetRecordState(EDIT_UPDATED);
		}
	}
	break;
	case TYPE_TEXT:
	{
        if (strcmp((char *)pValue, (char *)pSettings[nField].POINTER))
		{
            strncpy((char *)pSettings[nField].POINTER, (char *)pValue, pSettings[nField].nLENGTH - 1);
			if (GetRecordState() == EDIT_NOT_MODIFIED)
				SetRecordState(EDIT_UPDATED);
		}
	}
	break;
	case TYPE_DOUBLE:
	{
		double dValue = (*((double*)pValue));
		if (dValue != (*((double*)pSettings[nField].POINTER)))
		{
			(*((double*)pSettings[nField].POINTER)) = dValue;
			if (GetRecordState() == EDIT_NOT_MODIFIED)
				SetRecordState(EDIT_UPDATED);
		}
	}
	break;
	case TYPE_FLOAT:
	{
		float fValue = (*((float*)pValue));
		if (fValue != (*((float*)pSettings[nField].POINTER)))
		{
			(*((float*)pSettings[nField].POINTER)) = fValue;
			if (GetRecordState() == EDIT_NOT_MODIFIED)
				SetRecordState(EDIT_UPDATED);
		}
	}
	break;
	case TYPE_DATA:
	{
		SHARED_PTRM(unsigned char) *pByte = (SHARED_PTRM(unsigned char)*)pSettings[nField].POINTER;
		memcpy(GET_POINTEROF((*pByte)), (unsigned char*)pValue, *((int*)pSettings[nField - 1].POINTER));
		if (GetRecordState() == EDIT_NOT_MODIFIED)
			SetRecordState(EDIT_UPDATED);
	}
	break;
	}*/
	return false;
}


int CSQLData::GetJSONData(int nField,std::string &strJSON)
{
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);

	std::string strTemp;
	if (m_pDataElem[nField]->GetJSONData(strTemp))
	{
		strJSON.append("\"");
		strJSON.append(pSettings[nField].szJSONName);
		strJSON.append("\":");
		strJSON.append(strTemp);
		return pSettings[nField].nTYPE;
	}
	return -1;
}

char* CSQLData::GetJSONFieldName(int nField)
{
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);
	return pSettings[nField].szJSONName;
}

int CSQLData::GetFieldData(int nField, std::string &strData)
{
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);

	std::string strTemp;
	if (m_pDataElem[nField]->GetJSONData(strTemp))
	{
		strData.append(strTemp);
		return pSettings[nField].nTYPE;
	}
	return -1;
}

int CSQLData::GetData(int nField, void *pValue)
{
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);
	if (m_pDataElem[nField]->GetData(pValue))
	{
		return pSettings[nField].nTYPE;
	}
	return -1;
	/*switch (pSettings[nField].nTYPE)
	{
	case TYPE_INTEGER:
	{
		int *pInt = (int*)pValue;
		(*pInt) = (int)(*((int*)(pSettings[nField].POINTER)));
		return TYPE_INTEGER;
	}
	break;
	case TYPE_TEXT:
	{
        STRCPY((TYC *)pValue, (TYC *)pSettings[nField].POINTER);
		return TYPE_TEXT;
	}
	break;
	case TYPE_FLOAT:
	{
		float *pDouble = (float*)pValue;
		*pDouble = (float)(*(float*)pSettings[nField].POINTER);
		return TYPE_FLOAT;
	}
	break;
	case TYPE_DOUBLE:
	{
		double *pDouble = (double*)pValue;
		*pDouble = (double)(*(double*)pSettings[nField].POINTER);
		return TYPE_DOUBLE;
	}
	break;
	case TYPE_DATA:
	{
		unsigned int **pUChar = (unsigned int**)pValue;
		SHARED_PTRM(unsigned char) *pByte = (SHARED_PTRM(unsigned char)*)pSettings[nField].POINTER;
		(*pUChar) = (unsigned int*)(*(GET_POINTEROF((*pByte))));
		return TYPE_DATA;
	}
	break;
	}
	return -1;*/
}



int CSQLData::GetSize(int nField)
{
	return m_pDataElem[nField]->GetSize();
}




void CSQLData::SQLClassInit(TYPESETTINGS *pSettings, std::shared_ptr<SQLDataElement> *pSQLElem, int nMember)
{
	MappingMembers();
	m_pDataElem = pSQLElem;
	for (int i = 0; i < nMember; i++)
	{
		pSettings[i].nLENGTH;
		switch (pSettings[i].nTYPE)
		{
		case TYPE_INTEGER:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLInt(pSettings[i].POINTER));
			break;
		case TYPE_TEXT:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLText(pSettings[i].POINTER, pSettings[i].nLENGTH));
			break;
		case TYPE_FLOAT:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLFloat(pSettings[i].POINTER));
			break;
		case TYPE_DOUBLE:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLDouble(pSettings[i].POINTER));
			break;
		case TYPE_DATA:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLBLOB(pSettings[i].POINTER, 2048 * 2048 * 4));
			break;
		case TYPE_WTEXT:
			pSQLElem[i] = std::shared_ptr<SQLDataElement>(new SQLWText(pSettings[i].POINTER, pSettings[i].nLENGTH));
		}
	}
}


bool CSQLData::InsertExec()
{
	DBQueryHandler *pHandler=GetDBHandler();
	if (pHandler)
		return pHandler->InsertExec(this);
	return false;
}
bool CSQLData::DeleteExec()
{
	DBQueryHandler *pHandler = GetDBHandler();
	if (pHandler)
		return pHandler->DeleteExec(this);
	return false;
}
bool CSQLData::UpdateExec()
{
	DBQueryHandler *pHandler = GetDBHandler();
	if (pHandler)
		return pHandler->UpdateExec(this);
	return false;
}




/*ALTER TABLE ���̺���
ADD �߰��� �÷���  ������ ����;*/








/*
bool CSQLData::InsertExec()
{
	int rc, nRetVal;
	TYC *tail;
	ODBC::ODBCConnection *pConn = GetDatabase();
	TYC szQueryBuffer[8192];
	LPTYPESETTINGS pSettings;
	int nCount;
	MappingMembers();
	GetTypeSettingParam(&pSettings, &nCount);
	TYC *pTableName = GetTableName();
	if (pTableName)
	{
		GetInsertQuery(szQueryBuffer, dim(szQueryBuffer), pTableName, pSettings, nCount);

		ODBC::ODBCStmt Stmt(*pConn);
		if (GetBLOBExist())
		{
			Stmt.Prepare(szQueryBuffer);

			for (int i = 0; i < nCount; i++)
			{
				if (pSettings[i].nTYPE == TYPE_DATALENGTH)
				{
				}
			}
		}
		else
		{
			rc=Stmt.Query(szQueryBuffer);

		}
		if(Stmt.Prepare(szQueryBuffer))
		{ 
	
        rc = SQLITE3_PREPARE(pDB, szQueryBuffer, STRLEN(szQueryBuffer) * sizeof(TYC), &state, TAIL_CASTING&tail);
		if (rc != SQLITE_OK)
		{
			return false;
		}
		if (GetBLOBExist())
		{
			for (int i = 0; i<nCount; i++)
			{
				if (pSettings[i].nTYPE == TYPE_DATALENGTH)
				{
					char chBuffer[128];
					SHARED_PTRM(unsigned char) *pByte = (SHARED_PTRM(unsigned char)*)pSettings[i + 1].POINTER;
#ifdef PAC_EDITOR
					snprintf(chBuffer, dim(chBuffer), ":%S", pSettings[i + 1].szFIELDNAME);
#else
	#ifndef UNDER_CE
					SPRINTF(chBuffer, dim(chBuffer), ":%s", pSettings[i + 1].szFIELDNAME);
	#else
					sprintf_s(chBuffer,dim(chBuffer),":%s",pSettings[i+1].szFIELDNAME);
	#endif
#endif
					sqlite3_bind_blob(state, sqlite3_bind_parameter_index(state, chBuffer), GET_POINTEROF((*pByte)), *((int*)pSettings[i].POINTER), SQLITE_TRANSIENT);
				}
			}
		}
		nRetVal = sqlite3_step(state);
		if (nRetVal == SQLITE_DONE)
		{
			nRetVal = sqlite3_finalize(state);
			if (nRetVal == SQLITE_OK)
				return true;
		}
	}
	return false;
}

bool CSQLData::UpdateExec()
{
	int rc, nRetVal;
	TYC *tail;
	sqlite3_stmt *state;
	sqlite3* pDB = GetDatabase();
	TYC szQueryBuffer[8192];
	LPTYPESETTINGS pSettings;
	int nCount;
	MappingMembers();
	GetTypeSettingParam(&pSettings, &nCount);
	TYC *pTableName = GetTableName();
	if (pTableName)
	{
		GetUpdateQuery(szQueryBuffer, dim(szQueryBuffer), pTableName, pSettings, nCount);
        rc = SQLITE3_PREPARE(pDB, szQueryBuffer, STRLEN(szQueryBuffer) * sizeof(TYC), &state, TAIL_CASTING&tail);
		if (rc != SQLITE_OK)
		{
#ifdef PAC_EDITOR
			OutputDebugStringW((TYC *)SQLITE3_ERRMSG(pDB));
			OutputDebugStringW(MAKE_T("\n"));
#endif
			return false;
		}
		if (GetBLOBExist())
		{
			for (int i = 0; i<nCount; i++)
			{
				if (pSettings[i].nTYPE == TYPE_DATALENGTH)
				{
					char chBuffer[128];
					SHARED_PTRM(unsigned char) *pByte = (SHARED_PTRM(unsigned char)*)pSettings[i + 1].POINTER;
#ifdef PAC_EDITOR
					snprintf(chBuffer, dim(chBuffer), ":%S", pSettings[i + 1].szFIELDNAME);
#else
	#ifndef UNDER_CE
					SPRINTF(chBuffer, dim(chBuffer), ":%s", pSettings[i + 1].szFIELDNAME);
	#else
					sprintf_s(chBuffer,dim(chBuffer),":%s",pSettings[i+1].szFIELDNAME);
	#endif
#endif
					int nSize = (*((int*)pSettings[i].POINTER));
					sqlite3_bind_blob(state, sqlite3_bind_parameter_index(state, chBuffer), GET_POINTEROF((*pByte)), *((int*)pSettings[i].POINTER), SQLITE_TRANSIENT);
				}
			}
		}
		nRetVal = sqlite3_step(state);
		if (nRetVal == SQLITE_DONE)
		{
			nRetVal = sqlite3_finalize(state);
			if (nRetVal == SQLITE_OK)
				return true;
		}
	}
	return false;
}


bool CSQLData::DeleteExec()
{
	int rc, nRetVal;
	sqlite3_stmt* state;
	const TYC *tail;
	sqlite3* pDB = GetDatabase();
	TYC szQueryBuffer[8192];
	TYC *pTableName;
	LPTYPESETTINGS pSettings;
	int nCount;
	GetTypeSettingParam(&pSettings, &nCount);
	pTableName = GetTableName();
	if (pTableName)
	{
		GetDeleteQuery(szQueryBuffer, dim(szQueryBuffer), pTableName, pSettings, nCount);
        rc = SQLITE3_PREPARE(pDB, szQueryBuffer, STRLEN(szQueryBuffer) * sizeof(TYC), &state, TAIL_CASTING&tail);
		if (SQLITE_OK != rc)
		{
#ifdef PAC_EDITOR
			OutputDebugStringW((TYC *)SQLITE3_ERRMSG(pDB));
			OutputDebugStringW(MAKE_T("\n"));
#endif
			return false;
		}
		int nColumns = sqlite3_column_count(state);
		nRetVal = sqlite3_step(state);
		sqlite3_finalize(state);
		if ((!nColumns) && (nRetVal == SQLITE_DONE))
		{
			return true;
		}
	}
	return false;
}*/



bool CSQLData::CommitData(int nOrder)
{
	TYPESETTINGS *pSettings;
	int nCompCount;
	bool bRet = GetTypeSettingParam(&pSettings, &nCompCount);
	if (bRet)
	{
		MappingMembers();
		for(int i=0;i<nCompCount;i++)
		{
			if (pSettings[i].bORDER)
			{
				m_pDataElem[i]->SetData(&nOrder, 0);
				if(m_pDataElem[i]->IsModified())
				{
					int nRecordState=GetRecordState();
					switch (GetRecordState())
					{
					case EDIT_NOT_MODIFIED:
						SetRecordState(EDIT_UPDATED);
						break;
					case EDIT_DELETED:
					case EDIT_UPDATED:
					case EDIT_INSERTED:
						break;
					}
				}
			}
		}

		switch (GetRecordState())
		{
		case EDIT_NOT_MODIFIED:
			break;
		case EDIT_UPDATED:
			bRet = UpdateExec();
			if (bRet)
				SetRecordState(EDIT_NOT_MODIFIED);
			break;
		case EDIT_INSERTED:
			bRet = InsertExec();
			if (bRet)
				SetRecordState(EDIT_NOT_MODIFIED);
			break;
		case EDIT_DELETED:
			bRet = DeleteExec();
			break;
		case EDIT_TO_BE_IGNORED:
		default:
			bRet = false;
			break;
		}

		int nSubCount = GetChildCount();
		for (int i = 0; i<nSubCount; i++)
		{
			if (m_vChildItem[i].vSQLData.size())
			{
				int nParentIndexCol = 0;
				int nOrderCol = 0;
				int nIndexCol = 0;
				int nParentIndex = GetIndex();
				int nOrder = 1;
				for (int j = 0; j<m_vChildItem[i].vSQLData.size(); j++)
				{
					nParentIndexCol = m_vChildItem[i].vSQLData[j]->GetParentIndexCol();
					nOrderCol = m_vChildItem[i].vSQLData[j]->GetOrderCol();
					nIndexCol = m_vChildItem[i].vSQLData[j]->GetIndexCol();
					if (nParentIndexCol != -1)
						m_vChildItem[i].vSQLData[j]->SetData(nParentIndexCol, (void*)&nParentIndex);
					if (nOrderCol != -1)
						m_vChildItem[i].vSQLData[j]->SetData(nOrderCol, (void*)&nOrder);
					nOrder++;
					m_vChildItem[i].vSQLData[j]->CommitData(j+1);
				}
			}

			for (int j = 0; j<m_vChildItem[i].vSQLDataDelItems.size(); j++)
			{
				m_vChildItem[i].vSQLDataDelItems[j]->CommitData(j+1);
			}
			m_vChildItem[i].vSQLDataDelItems.clear();
		}
	}
	return bRet;
}


void CSQLData::RemoveData(int nIndex, int nRow)
{
	int nCount = GetChildCount();
	if (nIndex < nCount)
	{
		if (m_vChildItem[nIndex].vSQLData.size() > nRow)
		{
			switch (m_vChildItem[nIndex].vSQLData[nRow]->GetRecordState())
			{
			case EDIT_UPDATED:
				m_vChildItem[nIndex].vSQLData[nRow]->SetRecordState(EDIT_DELETED);
				break;
			case EDIT_NOT_MODIFIED:
				m_vChildItem[nIndex].vSQLData[nRow]->SetRecordState(EDIT_DELETED);
				break;
			default:
				m_vChildItem[nIndex].vSQLData[nRow]->SetRecordState(EDIT_TO_BE_IGNORED);//Inserted �̸�
				break;
			}

			int nChildCount = m_vChildItem[nIndex].vSQLData[nRow]->m_vChildItem.size();
			for (int i = 0; i < nChildCount; i++)
			{
				while (m_vChildItem[nIndex].vSQLData[nRow]->m_vChildItem[i].vSQLData.size())
				{
					m_vChildItem[nIndex].vSQLData[nRow]->RemoveData(i, 0);
				}
			}

			m_vChildItem[nIndex].vSQLDataDelItems.insert(m_vChildItem[nIndex].vSQLDataDelItems.end(), m_vChildItem[nIndex].vSQLData.begin() + nRow, m_vChildItem[nIndex].vSQLData.begin() + nRow + 1);
			m_vChildItem[nIndex].vSQLData.erase(m_vChildItem[nIndex].vSQLData.begin() + nRow);
		}
	}
}



void CSQLData::SetAdditionalMembers(CSQLData *pData)
{

}


CSQLData* CSQLData::CloneCopied()
{
    CSQLData *pClone=CreateNew();
	LPTYPESETTINGS pSettingsSoc;
	int nCountSoc;
	GetTypeSettingParam(&pSettingsSoc, &nCountSoc);
	for (int i = 0; i < nCountSoc; i++)
	{
		if (!pSettingsSoc[i].bPRIMARYKEY)
			(*pClone->m_pDataElem[i].get()) = (*m_pDataElem[i].get());
	}

	int nParentIndex = pClone->GetIndex();
	SetAdditionalMembers(pClone);
    std::vector<SHARED_PTRC(CSQLData)>::iterator vit;
    for(int i=0;i<GetChildCount();i++)
    {
       for(vit=m_vChildItem[i].vSQLData.begin();vit!=m_vChildItem[i].vSQLData.end();vit++)
       {
		   int nDistance=std::distance(m_vChildItem[i].vSQLData.begin(),vit)+1;
           SHARED_PTRC(CSQLData) pValue=SHARED_PTRC(CSQLData)( (*vit)->CloneCopied());
		   int nParentCol=pValue->GetParentIndexCol();
		   if(nParentCol!=-1)
			   pValue->SetData(nParentCol,&nParentIndex);
		   int nOrderCol=pValue->GetOrderCol();
		   if(nOrderCol!=-1)
			   pValue->SetData(nOrderCol,&nDistance);
           pClone->m_vChildItem[i].vSQLData.push_back(pValue);
       }
    }
    return pClone;
}

CSQLData* CSQLData::Clone()
{
	CSQLData *pClone = CreateNew();
	LPTYPESETTINGS pSettingsSoc;
	int nCountSoc;
	GetTypeSettingParam(&pSettingsSoc, &nCountSoc);

	for (int i = 0; i < nCountSoc; i++)
	{
		(*pClone->m_pDataElem[i].get()) = (*m_pDataElem[i].get());
	}
	int nParentIndex = pClone->GetIndex();
	SetAdditionalMembers(pClone);
	std::vector<SHARED_PTRC(CSQLData)>::iterator vit;
	for (int i = 0; i < GetChildCount(); i++)
	{
		for (vit = m_vChildItem[i].vSQLData.begin(); vit != m_vChildItem[i].vSQLData.end(); vit++)
		{
			int nDistance = std::distance(m_vChildItem[i].vSQLData.begin(), vit) + 1;
			SHARED_PTRC(CSQLData) pValue = SHARED_PTRC(CSQLData)((*vit)->Clone());
			int nParentCol = pValue->GetParentIndexCol();
			if (nParentCol != -1)
				pValue->SetData(nParentCol, &nParentIndex);
			int nOrderCol = pValue->GetOrderCol();
			if (nOrderCol != -1)
				pValue->SetData(nOrderCol, &nDistance);
			pClone->m_vChildItem[i].vSQLData.push_back(pValue);
		}
	}
	return pClone;
}

bool CEditSQLData::CreateTable()
{
	char szQuery[4096] = { 0 };
	CSQLData::GetCreateQuery(szQuery, dim(szQuery), GetTableName(), GetTypeSettingsTable(), GetTypeSettingsCount());
	return QueryExec(szQuery);
}

bool CEditSQLData::AlterTable()
{
	bool bRet = false;
	char szQuery[512] = { 0 };
	for (int i = 0; i < GetTypeSettingsCount(); i++)
	{
		CSQLData::GetAlterTableAddColumn(szQuery, dim(szQuery), GetTableName(), GetTypeSettingsTable(), i);
		bRet |= QueryExec(szQuery);
	}
	return bRet;
}