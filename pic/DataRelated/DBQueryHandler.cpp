#include "DBQueryHandler.h"
#include "SQLData.h"



DBQueryHandler::DBQueryHandler()
{
}


DBQueryHandler::~DBQueryHandler()
{
}


bool DBQueryHandler::InsertExec(CSQLData *pData)
{
	char szQuery[2048];
	TYPESETTINGS *pSettings;
	int nCount, rc;
	pData->GetTypeSettingParam(&pSettings, &nCount);
	char *pTableName = pData->GetTableName();
	if (pTableName)
	{
		pData->GetInsertQuery(szQuery, dim(szQuery), pTableName, pSettings, nCount);
		return InsertExecQuery(szQuery,pData,nCount);
	}
	return false;
}
bool DBQueryHandler::DeleteExec(CSQLData *pData)
{
	char szQuery[2048];
	TYPESETTINGS *pSettings;
	int nCount, rc;
	pData->GetTypeSettingParam(&pSettings, &nCount);
	char *pTableName = pData->GetTableName();
	if (pTableName)
	{
		pData->GetDeleteQuery(szQuery, dim(szQuery), pTableName, pSettings, nCount);
		int nPrimaryCol = -1;
		for (int i = 0; i < nCount; i++)
		{
			if (pSettings[i].bPRIMARYKEY)
			{
				nPrimaryCol = i;
				break;
			}
		}
		if (nPrimaryCol != -1)
		{
			return DeleteExecQuery(szQuery,pData,nPrimaryCol);
		}
	}
	return false;
}
bool DBQueryHandler::UpdateExec(CSQLData *pData)
{
	char szQuery[2048];
	TYPESETTINGS *pSettings;
	int nCount, rc;
	int nPrimaryCol = -1;
	pData->GetTypeSettingParam(&pSettings, &nCount);
	char *pTableName = pData->GetTableName();
	if (pTableName)
	{
		pData->GetUpdateQuery(szQuery, dim(szQuery), pTableName, pSettings, nCount, pData->m_pDataElem);
		return UpdateExecQuery(szQuery, pData, pSettings, nCount);
	}
	return false;
}





DBEditor::DBEditor()
{

}

DBEditor::~DBEditor()
{

}

bool DBEditor::InitTable(CEditSQLData *pEditor)
{
	char szQuery[2048];
	int nColumns = pEditor->GetTypeSettingsCount();
	CSQLData::GetSelectQuery(szQuery, dim(szQuery), pEditor->GetTableName(), pEditor->GetTypeSettingsTable(), nColumns);
	return ExecInitTableQuery(pEditor, szQuery, nColumns);
}
bool DBEditor::GetMaxIndex(CEditSQLData *pEditor)
{
	char szQuery[2048];
	CSQLData::GetMaxIndexQuery(szQuery, dim(szQuery), pEditor->GetTableName(), pEditor->GetTypeSettingsTable(), pEditor->GetTypeSettingsCount());
	return ExecGetMaxIndexQuery(pEditor, szQuery);
}
