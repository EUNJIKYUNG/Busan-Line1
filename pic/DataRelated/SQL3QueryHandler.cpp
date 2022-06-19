#include "SQL3QueryHandler.h"
#include "SQLData.h"
#include "SQLDataElement.h"
#include <sqlite3.h>


SQL3QueryHandler::SQL3QueryHandler()
{
}

SQL3QueryHandler::~SQL3QueryHandler()
{

}

bool SQL3QueryHandler::InsertExecQuery(char *pszQuery, CSQLData *pData, int nCount)
{
	sqlite3_stmt* state;
	const char *tail;
	int rc = sqlite3_prepare_v2(GetDatabase(), pszQuery, strlen(pszQuery) * sizeof(char), &state, (const char**)&tail);
	if (rc != SQLITE_OK)
	{
		return false;
	}
	for (int i = 0; i < nCount; i++)
	{
		pData->m_pDataElem[i]->SQL3BindParam(state, i + 1);
	}
	sqlite3_step(state);
	sqlite3_reset(state);
	sqlite3_finalize(state);
	return true;
}

bool SQL3QueryHandler::DeleteExecQuery(char *pszQuery, CSQLData *pData, int nPrimaryCol)
{
	sqlite3_stmt* state;
	const char *tail;
	int rc = sqlite3_prepare_v2(GetDatabase(), pszQuery, strlen(pszQuery) * sizeof(char), &state, (const char**)&tail);
	if (rc != SQLITE_OK)
	{
		return false;
	}
	if (nPrimaryCol != -1)
	{
		pData->m_pDataElem[nPrimaryCol]->SQL3BindParam(state, 1);
	}
	sqlite3_step(state);
	sqlite3_reset(state);
	sqlite3_finalize(state);
	return true;
}
bool SQL3QueryHandler::UpdateExecQuery(char *pszQuery, CSQLData *pData, _TYPESETTINGS *pSettings, int nCount)
{
	int nPrimaryCol = -1;
	sqlite3_stmt* state;
	const char *tail;
	int rc = sqlite3_prepare_v2(GetDatabase(), pszQuery, strlen(pszQuery) * sizeof(char), &state, (const char**)&tail);
	if (rc != SQLITE_OK)
	{
		return false;
	}
	int nParameterCount = 1;
	for (int i = 0; i < nCount; i++)
	{
		if (pSettings[i].bPRIMARYKEY)
			nPrimaryCol = i;
		else if (pData->m_pDataElem[i]->IsModified())
			pData->m_pDataElem[i]->SQL3BindParam(state, nParameterCount++);
	}
	if (nPrimaryCol != -1)
	{
		pData->m_pDataElem[nPrimaryCol]->SQL3BindParam(state, nParameterCount);
	}
	sqlite3_step(state);
	sqlite3_reset(state);
	sqlite3_finalize(state);
	return true;
}

sqlite3* SQL3QueryHandler::GetDatabase()
{
	return m_db;
}
void SQL3QueryHandler::SetDatabase(sqlite3 *pDB)
{
	m_db = pDB;
}



SQL3Editor::SQL3Editor()
{

}

SQL3Editor::~SQL3Editor()
{

}


bool SQL3Editor::QueryExec(const char *lpQuery)
{
	char *errmsg;
	int rc = sqlite3_exec(GetDatabase(), lpQuery, 0, 0, &errmsg);
	if (SQLITE_OK != rc)
	{
		printf("SQL error: %s\n", errmsg);
		sqlite3_free(errmsg);
		return false;
	}
	return true;
}

bool SQL3Editor::ExecGetMaxIndexQuery(CEditSQLData *pEditor, char *pszQuery)
{
	char *tail;
	sqlite3_stmt *state;
	int rc = sqlite3_prepare_v2(GetDatabase(), pszQuery, strlen(pszQuery) * sizeof(char), &state, (const char**)&tail);
	if (SQLITE_OK != rc)
	{
		return false;
	}
	rc = sqlite3_step(state);
	while (rc == SQLITE_ROW)
	{
		int nMaxIndex = sqlite3_column_int(state, 0);
		pEditor->SetMaxIndex(nMaxIndex);
		rc = sqlite3_step(state);
	}
	sqlite3_finalize(state);

}
bool SQL3Editor::ExecInitTableQuery(CEditSQLData *pEditor, char *pszQuery, int nColumns)
{
	char *tail;
	sqlite3_stmt *state;
	int rc = sqlite3_prepare_v2(GetDatabase(), pszQuery, strlen(pszQuery) * sizeof(char), &state, (const char**)&tail);
	if (SQLITE_OK != rc)
	{
		return false;
	}
	rc = sqlite3_step(state);
	while (rc == SQLITE_ROW)
	{
		SHARED_PTRC(CSQLData) pSQL = SHARED_PTRC(CSQLData)(pEditor->FeedNotModified());
		for (int i = 0; i < nColumns; i++)
		{
			pSQL->m_pDataElem[i]->SQL3GetData(state, i);
		}
		pEditor->m_pvSQLData->push_back(pSQL);
		rc = sqlite3_step(state);
	}
	sqlite3_finalize(state);
	return true;
}





sqlite3* SQL3Editor::GetDatabase()
{
	return m_db;
}

void SQL3Editor::SetDatabase(sqlite3 *pDB)
{
	m_db = pDB;
}
