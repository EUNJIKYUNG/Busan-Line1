#ifndef SQL3_QUERY_HANDLER
#define SQL3_QUERY_HANDLER

#include "DBQueryHandler.h"

struct sqlite3;
struct _TYPESETTINGS;

class SQL3QueryHandler
	:public DBQueryHandler
{
public:
	SQL3QueryHandler();
	virtual ~SQL3QueryHandler();
	bool InsertExecQuery(char *pszQuery, CSQLData *pData, int nCount);
	bool DeleteExecQuery(char *pszQuery, CSQLData *pData, int nPrimaryCol);
	bool UpdateExecQuery(char *pszQuery, CSQLData *pData, _TYPESETTINGS *pSettings, int nCount);
	sqlite3* GetDatabase();
	void SetDatabase(sqlite3 *pDB);
protected:
	sqlite3* m_db;
};


class SQL3Editor
	:public DBEditor
{
public:
	SQL3Editor();
	virtual ~SQL3Editor();
	bool QueryExec(const char *lpQuery);
	sqlite3* GetDatabase();
	void SetDatabase(sqlite3 *pDB);
protected:
	virtual bool ExecGetMaxIndexQuery(CEditSQLData *pEditor, char *pszQuery);
	virtual bool ExecInitTableQuery(CEditSQLData *pEditor, char *pszQuery, int nColumns);
protected:
	sqlite3 *m_db;
};




#endif