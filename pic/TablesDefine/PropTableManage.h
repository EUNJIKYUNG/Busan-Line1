#ifndef PROP_TABLE_MANAGE
#define PROP_TABLE_MANAGE
#include <memory>
#include <vector>
#include "../DataRelated/SQLData.h"
#ifdef USE_MDB
#include "ODBC/ODBCClass.h"
#endif

#ifdef USE_MDB
typedef void* HDBC;
#endif
struct sqlite3;
class DBQueryHandler;
class DBEditor;

class CPropTableManage
{
public:
	CPropTableManage(void);
	~CPropTableManage(void);
private:
	static sqlite3 *m_dbSource;

	static std::shared_ptr<DBQueryHandler> m_pQueryHandler;
	static std::shared_ptr<DBEditor> m_pEditor;
public:
	static CPropTableManage *GetInstance();


	DECLARE_VECTOR_FOR_CLASS(PropSection)
	DECLARE_EDITOR_POINTER_FOR_CLASS(PropSection)
	DECLARE_EDITOR_POINTER_FOR_CLASS(PropKeys)
	
	bool InitDatabase(const char *pszPath);
	bool CloseDatabase();
	void LoadDatabase();
	void SaveModified();

};


#endif
