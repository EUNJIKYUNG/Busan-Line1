#include "PropTableManage.h"
#include "DefinePropTables.h"
#include <functional>
#include <algorithm>
#include "../DataRelated/SQL3QueryHandler.h"
#include "sqlite3.h"


struct SQLDataCompare : public std::binary_function<std::shared_ptr<CSQLData>, std::shared_ptr<CSQLData>, bool>
{
	SQLDataCompare(int nCol, bool bAscendingOrDescending)
		:m_nCol(nCol), bAscDes(bAscendingOrDescending)
	{

	}
	bool operator ()(std::shared_ptr<CSQLData> pSIA, std::shared_ptr<CSQLData> pSIB)
	{
		if (bAscDes)
			return (*pSIA->m_pDataElem[m_nCol].get()) > (*pSIB->m_pDataElem[m_nCol].get());
		else
			return (*pSIA->m_pDataElem[m_nCol].get()) < (*pSIB->m_pDataElem[m_nCol].get());
	}
private:
	int m_nCol;
	bool bAscDes;
};


struct DefaultComparator : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	DefaultComparator()
	{

	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		return true;
	}
};



sqlite3 *CPropTableManage::m_dbSource = 0;
std::shared_ptr<DBQueryHandler> CPropTableManage::m_pQueryHandler;
std::shared_ptr<DBEditor> CPropTableManage::m_pEditor;


CPropTableManage::CPropTableManage(void)
{
}

CPropTableManage::~CPropTableManage(void)
{
}



CPropTableManage *CPropTableManage::GetInstance()
{
	static std::shared_ptr<CPropTableManage> pManage;
	if (!pManage)
	{
		pManage = std::shared_ptr<CPropTableManage>(new CPropTableManage());
	}
	return pManage.get();
}

void CPropTableManage::LoadDatabase()
{
	DECLARE_TEMP_VECTOR_CLASS(PropKeys)

	CREATE_EDITOR_CLASS_FOR_CLASS(PropKeys)
	CREATE_EDITOR_CLASS_FOR_CLASS(PropSection)
	//Parent;
	INIT_EDITORCLASS(PropSection, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(PropKeys, &TEMP_VECTOR_CLASS(PropKeys), NULL, m_pQueryHandler, m_pEditor)

	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(PropSection), 0, &TEMP_VECTOR_CLASS(PropKeys));		//부모 자식 관계
}

void CPropTableManage::SaveModified()
{
	SAVEDATA_FOR_CLASS(PropSection);
}





bool CPropTableManage::InitDatabase(const char *pszPath)
{
	int nRetVal = sqlite3_open(pszPath, &m_dbSource);
	if (nRetVal != SQLITE_OK)
	{
		return false;
	}

	SQL3QueryHandler *pHandler = new SQL3QueryHandler();
	SQL3Editor *pEditor = new SQL3Editor();
	pHandler->SetDatabase(m_dbSource);
	pEditor->SetDatabase(m_dbSource);
	m_pQueryHandler = std::shared_ptr<DBQueryHandler>(pHandler);
	m_pEditor = std::shared_ptr<DBEditor>(pEditor);
	return true;
}


bool CPropTableManage::CloseDatabase()
{
	if (m_dbSource && (SQLITE_OK == sqlite3_close(m_dbSource)))
	{
		m_dbSource = NULL;
		return true;
	}
	return false;
}

