#include "TableManage.h"
#include "DefineTables.h"
#ifdef LINEMAP_USED
	#include "DefinePIDDRMTables.h"
#endif
#include <functional>
#include <algorithm>
#ifdef USE_MDB
#include "DataRelated/MDBQueryHandler.h"
#endif
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

#ifdef SEPTA_EDITOR
template <int N, bool B>
struct StationBroadcastComparator : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	StationBroadcastComparator()
	{

	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		StationAnnouncement *pSA = (StationAnnouncement*)pSI.get();
		bool bTerminus = pSA->uTerminus ? true : false;
		return ((bTerminus == B) && ((pSA->uTriggerPoint % 10) == N));
	}
};
#endif

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

#ifdef USE_MDB
ODBC::MDBConnection2* CTableManage::m_pConnection;
#endif

sqlite3 *CTableManage::m_dbSource = 0;
std::shared_ptr<DBQueryHandler> CTableManage::m_pQueryHandler;
std::shared_ptr<DBEditor> CTableManage::m_pEditor;


sqlite3 *CTableManage::m_dbOperDB = 0;

std::shared_ptr<DBQueryHandler> CTableManage::m_pOperDB;
std::shared_ptr<DBEditor> CTableManage::m_pOperDBEditor;


sqlite3 *CTableManage::m_dbPIDDB = 0;

std::shared_ptr<DBQueryHandler> CTableManage::m_pPIDDB;
std::shared_ptr<DBEditor> CTableManage::m_pPIDDBEditor;



CTableManage::CTableManage(void)
{
}

CTableManage::~CTableManage(void)
{
}



CTableManage *CTableManage::GetInstance()
{
	/*CTableManage tManage;
	return &tManage;*/
	static std::shared_ptr<CTableManage> pManage;
	if (!pManage)
	{
		pManage = std::shared_ptr<CTableManage>(new CTableManage());
	}
	return pManage.get();
}

void CTableManage::LoadDatabase()
{
	DECLARE_TEMP_VECTOR_CLASS(StopPtnRoutes)
	DECLARE_TEMP_VECTOR_CLASS(EventLists)
	DECLARE_TEMP_VECTOR_CLASS(VideoIndexList)
	DECLARE_TEMP_VECTOR_CLASS(AudioIndexList)
	DECLARE_TEMP_VECTOR_CLASS(PIDIndexList)
	DECLARE_TEMP_VECTOR_CLASS(UpdateFile)
	DECLARE_TEMP_VECTOR_CLASS(EDDIndexList)
	DECLARE_TEMP_VECTOR_CLASS(UpdateTarget)

	CREATE_EDITOR_CLASS_FOR_CLASS(StationInformation)
	CREATE_EDITOR_CLASS_FOR_CLASS(DestinationInformation)

	CREATE_EDITOR_CLASS_FOR_CLASS(StationDistance)
	CREATE_EDITOR_CLASS_FOR_CLASS(StopPtnHeader)
	CREATE_EDITOR_CLASS_FOR_CLASS(StopPtnRoutes)
	CREATE_EDITOR_CLASS_FOR_CLASS(EventLists)
	CREATE_EDITOR_CLASS_FOR_CLASS(AudioFilePool)
	CREATE_EDITOR_CLASS_FOR_CLASS(VideoFilePool)
	CREATE_EDITOR_CLASS_FOR_CLASS(VideoContents)
	CREATE_EDITOR_CLASS_FOR_CLASS(AudioContents)
	CREATE_EDITOR_CLASS_FOR_CLASS(PIDContents)
	CREATE_EDITOR_CLASS_FOR_CLASS(VideoIndexList)
	CREATE_EDITOR_CLASS_FOR_CLASS(AudioIndexList)
	CREATE_EDITOR_CLASS_FOR_CLASS(PIDIndexList)
	CREATE_EDITOR_CLASS_FOR_CLASS(UpdateFileTypeList)
	CREATE_EDITOR_CLASS_FOR_CLASS(UpdateFile)
	CREATE_EDITOR_CLASS_FOR_CLASS(FontPool)
	CREATE_EDITOR_CLASS_FOR_CLASS(TrainNumber)
	CREATE_EDITOR_CLASS_FOR_CLASS(EDDIndex)
	CREATE_EDITOR_CLASS_FOR_CLASS(EDDIndexList)
	CREATE_EDITOR_CLASS_FOR_CLASS(EDDIndicator)
	CREATE_EDITOR_CLASS_FOR_CLASS(ETNDIndicator)
	CREATE_EDITOR_CLASS_FOR_CLASS(EditorTagTable)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPool)
	CREATE_EDITOR_CLASS_FOR_CLASS(UpdateTarget)
	CREATE_EDITOR_CLASS_FOR_CLASS(TargetDevices)
	CREATE_EDITOR_CLASS_FOR_CLASS(UpdateMarking)
	CREATE_EDITOR_CLASS_FOR_CLASS(PIIText)
	//Parent;
	
	INIT_EDITORCLASS(StationInformation, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(DestinationInformation, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(StationDistance, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(StopPtnHeader, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(AudioFilePool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(VideoFilePool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(AudioContents, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(VideoContents, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(PIDContents, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(UpdateFileTypeList, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(FontPool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(TrainNumber, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(EDDIndex, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(EditorTagTable, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(DisplayItemPool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(EDDIndicator, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(ETNDIndicator, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(TargetDevices, m_pQueryHandler, m_pEditor)
	//INIT_EDITORCLASS(PIIText, m_pQueryHandler, m_pEditor)


	INIT_EDITORCLASS_WITH_TEMPVECTORS(StopPtnRoutes, &TEMP_VECTOR_CLASS(StopPtnRoutes), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(EventLists, &TEMP_VECTOR_CLASS(EventLists), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(VideoIndexList, &TEMP_VECTOR_CLASS(VideoIndexList), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(AudioIndexList, &TEMP_VECTOR_CLASS(AudioIndexList), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(PIDIndexList, &TEMP_VECTOR_CLASS(PIDIndexList), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(UpdateFile, &TEMP_VECTOR_CLASS(UpdateFile), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(EDDIndexList, &TEMP_VECTOR_CLASS(EDDIndexList), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(UpdateTarget, &TEMP_VECTOR_CLASS(UpdateTarget), NULL, m_pQueryHandler, m_pEditor)

	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(StopPtnHeader), 0, &TEMP_VECTOR_CLASS(StopPtnRoutes));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(StopPtnRoutes), 0, &TEMP_VECTOR_CLASS(EventLists));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(VideoContents), 0, &TEMP_VECTOR_CLASS(VideoIndexList));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(AudioContents), 0, &TEMP_VECTOR_CLASS(AudioIndexList));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(PIDContents), 0, &TEMP_VECTOR_CLASS(PIDIndexList));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(UpdateFileTypeList), 0, &TEMP_VECTOR_CLASS(UpdateFile));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(UpdateFileTypeList), 1, &TEMP_VECTOR_CLASS(UpdateTarget));		//�θ� �ڽ� ����
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(EDDIndex), 0, &TEMP_VECTOR_CLASS(EDDIndexList));		//�θ� �ڽ� ����


#ifdef LINEMAP_USED
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapSpot)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapPath)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapTile)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPool)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropPos)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropRotation)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropScale)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropColor)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropZOrder)
	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapDisplayItemPropVisible)


	DECLARE_TEMP_VECTOR_CLASS(LineMapSpot)
	DECLARE_TEMP_VECTOR_CLASS(LineMapPath)
	DECLARE_TEMP_VECTOR_CLASS(LineMapTile)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPool)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropPos)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropRotation)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropScale)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropColor)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropZOrder)
	DECLARE_TEMP_VECTOR_CLASS(LineMapDisplayItemPropVisible)

	CREATE_EDITOR_CLASS_FOR_CLASS(LineMapPool)


	INIT_EDITORCLASS(LineMapPool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapSpot, &TEMP_VECTOR_CLASS(LineMapSpot), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapPath, &TEMP_VECTOR_CLASS(LineMapPath), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapTile, &TEMP_VECTOR_CLASS(LineMapTile), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPool, &TEMP_VECTOR_CLASS(LineMapDisplayItemPool), NULL, m_pQueryHandler, m_pEditor)

	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropPos, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropPos), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropRotation, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropRotation), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropScale, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropScale), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropColor, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropColor), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropZOrder, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropZOrder), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(LineMapDisplayItemPropVisible, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropVisible), NULL, m_pQueryHandler, m_pEditor)

	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 0, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropPos));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 1, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropRotation));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 2, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropScale));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 3, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropColor));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 4, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropZOrder));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(LineMapDisplayItemPool), 5, &TEMP_VECTOR_CLASS(LineMapDisplayItemPropVisible));



	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(LineMapPool), 0, &TEMP_VECTOR_CLASS(LineMapTile));
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(LineMapPool), 1, &TEMP_VECTOR_CLASS(LineMapPath));
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(LineMapPool), 2, &TEMP_VECTOR_CLASS(LineMapSpot));
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(LineMapPool), 3, &TEMP_VECTOR_CLASS(LineMapDisplayItemPool));



	DECLARE_TEMP_VECTOR_CLASS(ImageIndex)

	CREATE_EDITOR_CLASS_FOR_CLASS(ImageIndex)
	CREATE_EDITOR_CLASS_FOR_CLASS(BitmapImagePool)
	CREATE_EDITOR_CLASS_FOR_CLASS(ImageIndexList)
	CREATE_EDITOR_CLASS_FOR_CLASS(StringPool)

	INIT_EDITORCLASS(BitmapImagePool, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(ImageIndexList, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS(StringPool, m_pQueryHandler, m_pEditor)

	INIT_EDITORCLASS_WITH_TEMPVECTORS(ImageIndex, &TEMP_VECTOR_CLASS(ImageIndex), NULL, m_pQueryHandler, m_pEditor)
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(ImageIndexList), 0, &TEMP_VECTOR_CLASS(ImageIndex));

	DECLARE_TEMP_VECTOR_CLASS(DisplayItem)

	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropPos)
	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropRotation)
	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropScale)
	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropColor)
	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropZOrder)
	DECLARE_TEMP_VECTOR_CLASS(DisplayItemPropVisible)

	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItem)

	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropPos)
	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropRotation)
	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropScale)
	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropColor)
	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropZOrder)
	DECLARE_TEMP_VECTOR_CLASS(DisplayMetaItemPropVisible)



	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItem)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayMetaItem)

	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropPos)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropRotation)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropScale)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropColor)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropZOrder)
	CREATE_EDITOR_CLASS_FOR_CLASS(DisplayItemPropVisible)










	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItem, &TEMP_VECTOR_CLASS(DisplayItem), NULL, m_pQueryHandler, m_pEditor)

	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropPos, &TEMP_VECTOR_CLASS(DisplayItemPropPos), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropRotation, &TEMP_VECTOR_CLASS(DisplayItemPropRotation), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropScale, &TEMP_VECTOR_CLASS(DisplayItemPropScale), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropColor, &TEMP_VECTOR_CLASS(DisplayItemPropColor), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropZOrder, &TEMP_VECTOR_CLASS(DisplayItemPropZOrder), NULL, m_pQueryHandler, m_pEditor)
	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayItemPropVisible, &TEMP_VECTOR_CLASS(DisplayItemPropVisible), NULL, m_pQueryHandler, m_pEditor)

	INIT_EDITORCLASS_WITH_TEMPVECTORS(DisplayMetaItem, &TEMP_VECTOR_CLASS(DisplayMetaItem), NULL, m_pQueryHandler, m_pEditor)




	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(DisplayItemPool), 0, &TEMP_VECTOR_CLASS(DisplayItem));
	MakeRelationShip<DefaultComparator>(&VECTOR_CLASS(DisplayItemPool), 1, &TEMP_VECTOR_CLASS(DisplayMetaItem));

	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 0, &TEMP_VECTOR_CLASS(DisplayItemPropPos));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 1, &TEMP_VECTOR_CLASS(DisplayItemPropRotation));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 2, &TEMP_VECTOR_CLASS(DisplayItemPropScale));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 3, &TEMP_VECTOR_CLASS(DisplayItemPropColor));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 4, &TEMP_VECTOR_CLASS(DisplayItemPropZOrder));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayItem), 5, &TEMP_VECTOR_CLASS(DisplayItemPropVisible));

	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 0, &TEMP_VECTOR_CLASS(DisplayItemPropPos));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 1, &TEMP_VECTOR_CLASS(DisplayItemPropRotation));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 2, &TEMP_VECTOR_CLASS(DisplayItemPropScale));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 3, &TEMP_VECTOR_CLASS(DisplayItemPropColor));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 4, &TEMP_VECTOR_CLASS(DisplayItemPropZOrder));
	MakeRelationShip<DefaultComparator>(&TEMP_VECTOR_CLASS(DisplayMetaItem), 5, &TEMP_VECTOR_CLASS(DisplayItemPropVisible));

#endif

}

void CTableManage::SaveModified()
{
	SAVEDATA_FOR_CLASS(StationInformation);
	SAVEDATA_FOR_CLASS(DestinationInformation);
	SAVEDATA_FOR_CLASS(StationDistance);
	SAVEDATA_FOR_CLASS(StopPtnHeader);
	SAVEDATA_FOR_CLASS(AudioFilePool);
	SAVEDATA_FOR_CLASS(VideoFilePool);
	SAVEDATA_FOR_CLASS(AudioContents);
	SAVEDATA_FOR_CLASS(VideoContents);
	SAVEDATA_FOR_CLASS(PIDContents);
	SAVEDATA_FOR_CLASS(UpdateFileTypeList);
	SAVEDATA_FOR_CLASS(FontPool);
	SAVEDATA_FOR_CLASS(TrainNumber);
	SAVEDATA_FOR_CLASS(DisplayItemPool);
	SAVEDATA_FOR_CLASS(TargetDevices);
	SAVEDATA_FOR_CLASS(PIIText);
#ifdef LINEMAP_USED
	SAVEDATA_FOR_CLASS(LineMapPool);
	SAVEDATA_FOR_CLASS(StringPool);
	SAVEDATA_FOR_CLASS(BitmapImagePool);
	SAVEDATA_FOR_CLASS(ImageIndexList);
#endif
	SAVEDATA_FOR_CLASS(EDDIndicator);
	SAVEDATA_FOR_CLASS(ETNDIndicator);
	SAVEDATA_FOR_CLASS(EDDIndex);
	SAVEDATA_FOR_CLASS(EditorTagTable);
}





bool CTableManage::InitDatabase(const char *pszPath)
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


bool CTableManage::CloseDatabase()
{
	if (m_dbSource && (SQLITE_OK == sqlite3_close(m_dbSource)))
	{
		m_dbSource = NULL;
		return true;
	}
	return false;
}



bool CTableManage::OpenOperDB(const char *pszPath)
{
	int nRetVal = sqlite3_open(pszPath, &m_dbOperDB);
	if (nRetVal != SQLITE_OK)
	{
		return false;
	}

	char *err_msg;
	nRetVal = sqlite3_exec(m_dbOperDB, "PRAGMA journal_mode=OFF", 0, 0, &err_msg);
	nRetVal = sqlite3_exec(m_dbOperDB, "PRAGMA synchronous=OFF", 0, 0, &err_msg);
	nRetVal = sqlite3_exec(m_dbOperDB, "PRAGMA locking_mode = EXCLUSIVE", 0, 0, &err_msg);


	SQL3QueryHandler *pHandler = new SQL3QueryHandler();
	SQL3Editor *pEditor = new SQL3Editor();
	pHandler->SetDatabase(m_dbOperDB);
	pEditor->SetDatabase(m_dbOperDB);
	m_pOperDB = std::shared_ptr<DBQueryHandler>(pHandler);
	m_pOperDBEditor = std::shared_ptr<DBEditor>(pEditor);
	return true;
}

bool CTableManage::CloseOperDB()
{
	if (m_dbOperDB && (SQLITE_OK == sqlite3_close(m_dbOperDB)))
	{
		m_dbOperDB = NULL;
		return true;
	}
	return false;
}


bool CTableManage::OpenPIDDB(const char *pszPath)
{
	int nRetVal = sqlite3_open(pszPath, &m_dbPIDDB);
	if (nRetVal != SQLITE_OK)
	{
		return false;
	}
	char *err_msg;
	nRetVal = sqlite3_exec(m_dbPIDDB, "PRAGMA schema.journal_mode=OFF", 0, 0, &err_msg);
	nRetVal = sqlite3_exec(m_dbPIDDB, "PRAGMA synchronous=OFF", 0, 0, &err_msg);
	nRetVal = sqlite3_exec(m_dbPIDDB, "PRAGMA locking_mode = EXCLUSIVE", 0, 0, &err_msg);

	SQL3QueryHandler *pHandler = new SQL3QueryHandler();
	SQL3Editor *pEditor = new SQL3Editor();
	pHandler->SetDatabase(m_dbPIDDB);
	pEditor->SetDatabase(m_dbPIDDB);
	m_pPIDDB = std::shared_ptr<DBQueryHandler>(pHandler);
	m_pPIDDBEditor = std::shared_ptr<DBEditor>(pEditor);
	return true;
}

bool CTableManage::ClosePIDDB()
{
	if (m_dbPIDDB && (SQLITE_OK == sqlite3_close(m_dbPIDDB)))
	{
		m_dbPIDDB = NULL;
		return true;
	}
	return false;
}

void CTableManage::CopyPIDDB()
{
#ifdef LINEMAP_EDITOR
#ifdef LINEMAP_USED
	TEMP_INIT_EDITORCLASSSAVE(StopPtnRoutes, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(EventLists, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(VideoIndexList, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(AudioIndexList, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(PIDIndexList, m_pPIDDB, m_pPIDDBEditor)

	TEMP_INIT_EDITORCLASSSAVE(LineMapSpot, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapPath, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapTile, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPool, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropPos, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropRotation, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropScale, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropColor, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropZOrder, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropVisible, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItem, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayMetaItem, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropPos, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropRotation, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropScale, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropColor, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropZOrder, m_pPIDDB, m_pPIDDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropVisible, m_pPIDDB, m_pPIDDBEditor)

	SetUpdateMarkingString();
	TEMP_INIT_EDITORCLASSSAVENSAVE(UpdateMarking, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(StationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationInformation, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(DestinationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DestinationInformation, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(StationDistance)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationDistance, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(StopPtnHeader)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StopPtnHeader, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(VideoFilePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoFilePool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(AudioContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(AudioContents, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(VideoContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoContents, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(PIDContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(PIDContents, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(FontPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(FontPool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(TrainNumber)
	TEMP_INIT_EDITORCLASSSAVENSAVE(TrainNumber, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(EditorTagTable)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EditorTagTable, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(DisplayItemPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DisplayItemPool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(LineMapPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(LineMapPool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(BitmapImagePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(BitmapImagePool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(ImageIndexList)
	TEMP_INIT_EDITORCLASSSAVENSAVE(ImageIndexList, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(StringPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StringPool, m_pPIDDB, m_pPIDDBEditor)




	SETDBHANDLEREDITOR_CLASS(LineMapSpot, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapPath, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapTile, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPool, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropPos, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropRotation, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropScale, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropColor, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropZOrder, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropVisible, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItem, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayMetaItem, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropPos, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropRotation, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropScale, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropColor, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropZOrder, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropVisible, m_pQueryHandler, m_pEditor)

	SETDBHANDLEREDITOR_CLASS(StopPtnRoutes, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(EventLists, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(VideoIndexList, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(AudioIndexList, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(PIDIndexList, m_pQueryHandler, m_pEditor)

	CLEAR_SAVEVECTOR(StationInformation)
	REVERT_BACK_EDITORCLASS(StationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DestinationInformation)
	REVERT_BACK_EDITORCLASS(DestinationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StationDistance)
	REVERT_BACK_EDITORCLASS(StationDistance, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StopPtnHeader)
	REVERT_BACK_EDITORCLASS(StopPtnHeader, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoFilePool)
	REVERT_BACK_EDITORCLASS(VideoFilePool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(AudioContents)
	REVERT_BACK_EDITORCLASS(AudioContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoContents)
	REVERT_BACK_EDITORCLASS(VideoContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(PIDContents)
	REVERT_BACK_EDITORCLASS(PIDContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(FontPool)
	REVERT_BACK_EDITORCLASS(FontPool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(TrainNumber)
	REVERT_BACK_EDITORCLASS(TrainNumber, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EditorTagTable)
	REVERT_BACK_EDITORCLASS(EditorTagTable, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DisplayItemPool)
	REVERT_BACK_EDITORCLASS(DisplayItemPool, m_pQueryHandler, m_pEditor)
#endif
#endif
}



void CTableManage::CopyOperDB()
{
#ifdef LINEMAP_EDITOR
	TEMP_INIT_EDITORCLASSSAVE(StopPtnRoutes, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(EventLists,m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(VideoIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(AudioIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(PIDIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(UpdateFile, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(UpdateTarget, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(EDDIndexList,m_pOperDB, m_pOperDBEditor);

	SET_SAVEVECTOR(StationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationInformation, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(DestinationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DestinationInformation, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(StationDistance)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationDistance, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(StopPtnHeader)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StopPtnHeader, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(AudioFilePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(AudioFilePool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(VideoFilePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoFilePool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(AudioContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(AudioContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(VideoContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(PIDContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(PIDContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(UpdateFileTypeList)
	TEMP_INIT_EDITORCLASSSAVENSAVE(UpdateFileTypeList, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(TargetDevices)
	TEMP_INIT_EDITORCLASSSAVENSAVE(TargetDevices, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(FontPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(FontPool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(TrainNumber)
	TEMP_INIT_EDITORCLASSSAVENSAVE(TrainNumber, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EDDIndex)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EDDIndex, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EditorTagTable)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EditorTagTable, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(DisplayItemPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DisplayItemPool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EDDIndicator)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EDDIndicator, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(ETNDIndicator)
	TEMP_INIT_EDITORCLASSSAVENSAVE(ETNDIndicator, m_pOperDB, m_pOperDBEditor)

	
	SETDBHANDLEREDITOR_CLASS(StopPtnRoutes, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(EventLists, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(VideoIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(AudioIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(PIDIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(UpdateFile, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(UpdateTarget, m_pOperDB, m_pOperDBEditor);
	SETDBHANDLEREDITOR_CLASS(EDDIndexList, m_pQueryHandler, m_pEditor);
	CLEAR_SAVEVECTOR(StationInformation)
	REVERT_BACK_EDITORCLASS(StationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DestinationInformation)
	REVERT_BACK_EDITORCLASS(DestinationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StationDistance)
	REVERT_BACK_EDITORCLASS(StationDistance, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StopPtnHeader)
	REVERT_BACK_EDITORCLASS(StopPtnHeader, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(AudioFilePool)
	REVERT_BACK_EDITORCLASS(AudioFilePool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoFilePool)
	REVERT_BACK_EDITORCLASS(VideoFilePool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(AudioContents)
	REVERT_BACK_EDITORCLASS(AudioContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoContents)
	REVERT_BACK_EDITORCLASS(VideoContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(PIDContents)
	REVERT_BACK_EDITORCLASS(PIDContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(UpdateFileTypeList)
	REVERT_BACK_EDITORCLASS(UpdateFileTypeList, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(TargetDevices)
	REVERT_BACK_EDITORCLASS(TargetDevices, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(FontPool)
	REVERT_BACK_EDITORCLASS(FontPool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(TrainNumber)
	REVERT_BACK_EDITORCLASS(TrainNumber, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EDDIndex)
	REVERT_BACK_EDITORCLASS(EDDIndex, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EditorTagTable)
	REVERT_BACK_EDITORCLASS(EditorTagTable, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DisplayItemPool)
	REVERT_BACK_EDITORCLASS(DisplayItemPool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EDDIndicator)
	REVERT_BACK_EDITORCLASS(EDDIndicator, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(ETNDIndicator)
	REVERT_BACK_EDITORCLASS(ETNDIndicator, m_pQueryHandler, m_pEditor)
	
#endif
}
void CTableManage::CopyCloneOperDB()
{
#ifdef LINEMAP_EDITOR
	TEMP_INIT_EDITORCLASSSAVE(StopPtnRoutes, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(EventLists, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(VideoIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(AudioIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(PIDIndexList, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(UpdateFile, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(UpdateTarget, m_pOperDB, m_pOperDBEditor);
	TEMP_INIT_EDITORCLASSSAVE(EDDIndexList, m_pOperDB, m_pOperDBEditor);
	
	TEMP_INIT_EDITORCLASSSAVE(LineMapSpot, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapPath, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapTile, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPool, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropPos, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropRotation, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropScale, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropColor, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropZOrder, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(LineMapDisplayItemPropVisible, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItem, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayMetaItem, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropPos, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropRotation, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropScale, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropColor, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropZOrder, m_pOperDB, m_pOperDBEditor)
	TEMP_INIT_EDITORCLASSSAVE(DisplayItemPropVisible, m_pOperDB, m_pOperDBEditor)
	
	SET_SAVEVECTOR(StationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationInformation, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(DestinationInformation)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DestinationInformation, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(StationDistance)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StationDistance, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(StopPtnHeader)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StopPtnHeader, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(AudioFilePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(AudioFilePool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(VideoFilePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoFilePool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(AudioContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(AudioContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(VideoContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(VideoContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(PIDContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(PIDContents, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(UpdateFileTypeList)
	TEMP_INIT_EDITORCLASSSAVENSAVE(UpdateFileTypeList, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(TargetDevices)
	TEMP_INIT_EDITORCLASSSAVENSAVE(TargetDevices, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(FontPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(FontPool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(TrainNumber)
	TEMP_INIT_EDITORCLASSSAVENSAVE(TrainNumber, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EDDIndex)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EDDIndex, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EditorTagTable)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EditorTagTable, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(DisplayItemPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(DisplayItemPool, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(EDDIndicator)
	TEMP_INIT_EDITORCLASSSAVENSAVE(EDDIndicator, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(ETNDIndicator)
	TEMP_INIT_EDITORCLASSSAVENSAVE(ETNDIndicator, m_pOperDB, m_pOperDBEditor)
	SET_SAVEVECTOR(PIDContents)
	TEMP_INIT_EDITORCLASSSAVENSAVE(PIDContents, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(FontPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(FontPool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(LineMapPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(LineMapPool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(BitmapImagePool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(BitmapImagePool, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(ImageIndexList)
	TEMP_INIT_EDITORCLASSSAVENSAVE(ImageIndexList, m_pPIDDB, m_pPIDDBEditor)
	SET_SAVEVECTOR(StringPool)
	TEMP_INIT_EDITORCLASSSAVENSAVE(StringPool, m_pPIDDB, m_pPIDDBEditor)

	SETDBHANDLEREDITOR_CLASS(StopPtnRoutes, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(EventLists, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(VideoIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(AudioIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(PIDIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(UpdateFile, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(UpdateTarget, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(EDDIndexList, m_pQueryHandler, m_pEditor);
	SETDBHANDLEREDITOR_CLASS(LineMapSpot, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapPath, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapTile, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPool, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropPos, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropRotation, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropScale, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropColor, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropZOrder, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(LineMapDisplayItemPropVisible, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItem, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayMetaItem, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropPos, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropRotation, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropScale, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropColor, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropZOrder, m_pQueryHandler, m_pEditor)
	SETDBHANDLEREDITOR_CLASS(DisplayItemPropVisible, m_pQueryHandler, m_pEditor)

	CLEAR_SAVEVECTOR(StationInformation)
	REVERT_BACK_EDITORCLASS(StationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DestinationInformation)
	REVERT_BACK_EDITORCLASS(DestinationInformation, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StationDistance)
	REVERT_BACK_EDITORCLASS(StationDistance, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(StopPtnHeader)
	REVERT_BACK_EDITORCLASS(StopPtnHeader, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(AudioFilePool)
	REVERT_BACK_EDITORCLASS(AudioFilePool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoFilePool)
	REVERT_BACK_EDITORCLASS(VideoFilePool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(AudioContents)
	REVERT_BACK_EDITORCLASS(AudioContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(VideoContents)
	REVERT_BACK_EDITORCLASS(VideoContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(PIDContents)
	REVERT_BACK_EDITORCLASS(PIDContents, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(UpdateFileTypeList)
	REVERT_BACK_EDITORCLASS(UpdateFileTypeList, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(TargetDevices)
	REVERT_BACK_EDITORCLASS(TargetDevices, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(FontPool)
	REVERT_BACK_EDITORCLASS(FontPool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(TrainNumber)
	REVERT_BACK_EDITORCLASS(TrainNumber, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EDDIndex)
	REVERT_BACK_EDITORCLASS(EDDIndex, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EditorTagTable)
	REVERT_BACK_EDITORCLASS(EditorTagTable, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(DisplayItemPool)
	REVERT_BACK_EDITORCLASS(DisplayItemPool, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(EDDIndicator)
	REVERT_BACK_EDITORCLASS(EDDIndicator, m_pQueryHandler, m_pEditor)
	CLEAR_SAVEVECTOR(ETNDIndicator)
	REVERT_BACK_EDITORCLASS(ETNDIndicator, m_pQueryHandler, m_pEditor)
#endif
}

#ifdef LINEMAP_EDITOR
#include <windows.h>
bool CTableManage::GenerateUpdateMarkingString()
{
	SYSTEMTIME tSysTime;
	GetLocalTime(&tSysTime);
	sprintf_s(m_szUpdateMarkingString, _countof(m_szUpdateMarkingString),"%04d%02d%02dT%02d%02d%02d",
		tSysTime.wYear, tSysTime.wMonth, tSysTime.wDay, tSysTime.wHour, tSysTime.wMinute, tSysTime.wSecond);
	return true;
}


char* CTableManage::GetUpdateMarkingString()
{
	return m_szUpdateMarkingString;
}

void CTableManage::SetUpdateMarkingString()
{
	UpdateMarking *pUM = new UpdateMarking(EDIT_INSERTED);
	strcpy(pUM->szUpdateMarking, m_szUpdateMarkingString);
	pUM->SetIndex(1);
	m_vUpdateMarkingSave.clear();
	m_pUpdateFileEditor->SetVectors(&m_vUpdateMarkingSave, 0);
	m_pUpdateFileEditor->InsertData(0, std::shared_ptr<CSQLData>(pUM));
}

#endif






