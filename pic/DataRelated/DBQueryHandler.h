#ifndef DB_QUERY_HANDLER
#define DB_QUERY_HANDLER

class CSQLData;
class CEditSQLData;

struct _TYPESETTINGS;

class DBQueryHandler
{
public:
	DBQueryHandler();
	virtual ~DBQueryHandler();
	bool InsertExec(CSQLData *pData);
	bool DeleteExec(CSQLData *pData);
	bool UpdateExec(CSQLData *pData);

protected:
	virtual bool InsertExecQuery(char *pszQuery, CSQLData *pData, int nCount)=0;
	virtual bool DeleteExecQuery(char *pszQuery, CSQLData *pData, int nPrimaryCol)=0;
	virtual bool UpdateExecQuery(char *pszQuery, CSQLData *pData, _TYPESETTINGS *pSettings, int nCount)=0;
};


class DBEditor
{
public:
	DBEditor();
	virtual ~DBEditor();
	virtual bool QueryExec(const char *lpQuery) = 0;

	bool InitTable(CEditSQLData *pEditor);
	bool GetMaxIndex(CEditSQLData *pEditor);
protected:
	virtual bool ExecGetMaxIndexQuery(CEditSQLData *pEditor, char *pszQuery) = 0;
	virtual bool ExecInitTableQuery(CEditSQLData *pEditor, char *pszQuery, int nColumns) = 0;
};




#endif