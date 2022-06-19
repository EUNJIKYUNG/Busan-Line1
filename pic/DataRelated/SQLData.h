#ifndef SQLDATA_HEADER
#define SQLDATA_HEADER
#include <vector>
#include <memory>
#include <cstring>
#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <cstdio>
#include <map>
#include "DBQueryHandler.h"
#ifdef _WIN32
#include <Windows.h>
#endif


#ifdef UNDER_CE
#include <loki/smartptr.h>
	#define SHARED_PTRC(ARG) Loki::SmartPtr<ARG,Loki::RefCounted,Loki::AllowConversion,Loki::AssertCheck,Loki::DefaultSPStorage>
	#define SHARED_PTRM(ARG) Loki::SmartPtr<ARG,Loki::RefCounted,Loki::AllowConversion,Loki::AssertCheck,Loki::HeapStorage>
	#define MALLOCFORSP(TYPEOFIT,SIZEOFIT) (TYPEOFIT*)malloc(SIZEOFIT)
	#define GET_POINTEROF(A) A
	#define GET_PTFROMSP(A) (&(*A))
	#define GET_PTFROMIT(A) (&(**A))
#else
	#if defined(_MSC_VER)
		#if (_MSC_VER==1500)
			#define	SHARED_PTRC(ARG) std::tr1::shared_ptr<ARG>
			#define SHARED_PTRM(ARG) SHARED_PTRC(ARG)
		#else 
			#define	SHARED_PTRC(ARG) std::shared_ptr<ARG>
			#define SHARED_PTRM(ARG) SHARED_PTRC(ARG)
		#endif
	#else
		#define	SHARED_PTRC(ARG) std::shared_ptr<ARG>
		#define SHARED_PTRM(ARG) SHARED_PTRC(ARG)
	#endif
	#define GET_POINTEROF(A) A.get()
	#define GET_PTFROMSP(A) A.get()
	#define GET_PTFROMIT(A) A->get()
	#define MALLOCFORSP(TYPEOFIT,SIZEOFIT) (TYPEOFIT*)malloc(SIZEOFIT),free
#endif



#ifdef PAC_EDITOR
	#pragma message("PAC_EDITOR")
	#include <windows.h>
	#define SQLITE3_COLUMN_TEXT sqlite3_column_text16
	#define SQLITE3_PREPARE		sqlite3_prepare16_v2
	#define SQLITE3_OPEN		sqlite3_open16
	#define SQLITE3_ERRMSG		sqlite3_errmsg16
	#define STRCAT				wcscat
	#define SPRINTF				swprintf
	#define STRCPY              wcscpy
	#define STRLEN              wcslen
	#define STRCMP              wcscmp
	#define STRNCPY             wcsncpy
	#define TYC wchar_t
	#define MAKE_T(A)			L##A
	#define TAIL_CASTING		(const void**)
	#define TSTRING				std::wstring
#else
	#ifndef UNDER_CE
		#define SQLITE3_COLUMN_TEXT sqlite3_column_text
		#define SQLITE3_PREPARE		sqlite3_prepare_v2
		#define SQLITE3_OPEN		sqlite3_open
		#define SQLITE3_ERRMSG		sqlite3_errmsg
		#define STRCAT				strcat
		#if defined(_MSC_VER)
			#if (_MSC_VER==1500)
				#define SPRINTF				sprintf_s
			#else 
				#define SPRINTF				snprintf
			#endif
		#else
			#define SPRINTF				snprintf
		#endif
		#define STRCPY              strcpy
		#define STRLEN              strlen
		#define STRCMP              strcmp
		#define STRNCPY             strncpy
		#define TYC char
		#define MAKE_T
		#define TAIL_CASTING		(const char**)
		#define TSTRING				std::string
    #else
		#define SQLITE3_COLUMN_TEXT sqlite3_column_text16
		#define SQLITE3_PREPARE		sqlite3_prepare16
		#define SQLITE3_OPEN		sqlite3_open16
		#define SQLITE3_ERRMSG		sqlite3_errmsg16
		#define STRCAT				wcscat
		#define SPRINTF				swprintf_s
		#define STRCPY              wcscpy
		#define STRLEN              wcslen
		#define STRCMP              wcscmp
		#define STRNCPY             wcsncpy
		#define TYC wchar_t
		#define MAKE_T(A)			L##A
		#define TAIL_CASTING		(const void**)
		#define TSTRING				std::wstring
	#endif
#endif

#define BCDTODEC(A)  (A/16)*10+(A%16)



class CSQLData;
class CEditSQLData;
class SQLDataElement;
class DBQueryHandler;
class DBEditor;

enum EDITSTATE
{
	EDIT_NOT_MODIFIED = 0,
	EDIT_UPDATED,
	EDIT_INSERTED,
	EDIT_DELETED,
	EDIT_TO_BE_IGNORED,
};

enum TYPEDEFINE
{
	TYPE_INTEGER,
	TYPE_TEXT,
	TYPE_DOUBLE,
	TYPE_FLOAT,
	TYPE_DATA,
	TYPE_WTEXT,
};


#define dim(x) (sizeof(x)/sizeof(x[0]))
#define MM(ORDER,B) MAPPING_MEMBERS(ORDER,B)
#define MAKEUINT(a, b) ((unsigned int)(((unsigned short)(a)) | ((unsigned int)((unsigned short)(b))) << 16))
#define MAKEUSHORT(low, high) ((unsigned short)((((unsigned short)(high)) << 8) | ((unsigned char)(low))))







#define DECLARE_EDITOR_CLASS(CLASS_NAME) \
				class CLASS_NAME##Editor \
					: public CEditSQLData \
				{\
					public :\
					char* GetTableName()\
					{\
						return CLASS_NAME::szTableName;\
					}\
					TYPESETTINGS* GetTypeSettingsTable()\
					{\
						return CLASS_NAME::m_tSettings;\
					}\
					void SetMaxIndex(int nMaxIndex)\
					{\
						if(CLASS_NAME::m_nTableMaxIdx<nMaxIndex)\
							CLASS_NAME::m_nTableMaxIdx=nMaxIndex;\
					}\
					CSQLData *FeedInserted()\
					{\
						return new CLASS_NAME(EDIT_INSERTED);\
					}\
					CSQLData *FeedNotModified()\
					{\
						return new CLASS_NAME(EDIT_NOT_MODIFIED);\
					}\
					int GetTypeSettingsCount()\
					{\
						return sizeof(CLASS_NAME::m_tSettings)/sizeof(CLASS_NAME::m_tSettings[0]);\
					}\
					DBEditor* GetDBEditor()\
					{\
						return CLASS_NAME##Editor::m_pEditor.get();\
					}\
					static bool SetDBEditor(std::shared_ptr<DBEditor> &pEditor)\
					{\
						if(pEditor){m_pEditor=pEditor;return true;}return false;\
					}\
					static std::shared_ptr<DBEditor> m_pEditor;\
				};

#define DECLARE_TYPESETTINGS(SETTINGS_COUNT)\
					static TYPESETTINGS m_tSettings[SETTINGS_COUNT];\
					std::shared_ptr<SQLDataElement> m_tDataSet[SETTINGS_COUNT];

#define DECLARE_COMMON_FUNCTIONS bool GetTypeSettingParam(TYPESETTINGS **pSettings,int *pParam){if(pSettings&&pParam){*pSettings=m_tSettings;*pParam=sizeof(m_tSettings)/sizeof(m_tSettings[0]);return true;}return false;}\
					char* GetTableName(){return szTableName;}int GetInsertIndex() { return ++m_nTableMaxIdx; };int GetParentIndexCol(){return m_nParentIndexCol;}int GetOrderCol(){return m_nOrderCol;}int GetIndexCol(){return m_nIndexCol;}\
					void SetTableName(char *pTable){strcpy(szTableName, pTable);};

#define DECLARE_ADDITIONAL(CLASS_NAME) void SetAdditionalMember(CSQLData *pData){};

#define DECLARE_CREATE_NEW(CLASS_NAME)  \
					CSQLData* CreateNew()\
					{\
						CLASS_NAME *pClone=new CLASS_NAME(EDIT_INSERTED);\
						pClone->SetIndex(pClone->GetInsertIndex());\
						return pClone;\
					}\

#define IMPLEMENT_STATIC_TYPESETTINGS(CLASS_NAME,COUNT) TYPESETTINGS CLASS_NAME::m_tSettings[COUNT]

#define IMPLEMENT_STATIC_VARIABLES(CLASS_NAME,TABLENAME,PARENT_INDEX_COL,ORDER_COL,INDEX_COL)\
					char CLASS_NAME::szTableName[128]={#TABLENAME};\
					int CLASS_NAME::m_nTableMaxIdx=0;\
					int CLASS_NAME::m_nParentIndexCol=PARENT_INDEX_COL;\
					int CLASS_NAME::m_nOrderCol=ORDER_COL;\
					int CLASS_NAME::m_nIndexCol=INDEX_COL;\
					std::shared_ptr<DBQueryHandler> CLASS_NAME::m_pHandler;\
					std::shared_ptr<DBEditor> CLASS_NAME##Editor::m_pEditor;


#define IMPLEMENT_STATIC_VARIABLES_WITH_VALUE(CLASS_NAME,TABLENAME,PARENT_INDEX_COL,ORDER_COL,INDEX_COL,INIT_VALUE)\
					char CLASS_NAME::szTableName[128]={#TABLENAME};\
					int CLASS_NAME::m_nTableMaxIdx=INIT_VALUE;\
					int CLASS_NAME::m_nParentIndexCol=PARENT_INDEX_COL;\
					int CLASS_NAME::m_nOrderCol=ORDER_COL;\
					int CLASS_NAME::m_nIndexCol=INDEX_COL;\
					std::shared_ptr<DBQueryHandler> CLASS_NAME::m_pHandler;\
					std::shared_ptr<DBEditor> CLASS_NAME##Editor::m_pEditor;


#define INIT_DATA_ELEMENTS SQLClassInit(m_tSettings, m_tDataSet, dim(m_tSettings))





#define BEGIN_CLASS_FROM_SQLDATA(CLASS_NAME,INITIALIZER,DESTRUCTOR)\
					class CLASS_NAME :public CSQLData { public: CLASS_NAME(EDITSTATE eState):CSQLData(eState) {INIT_DATA_ELEMENTS;INITIALIZER;} DECLARE_CREATE_NEW(CLASS_NAME) virtual ~CLASS_NAME(void){DESTRUCTOR} DBQueryHandler* GetDBHandler(){return m_pHandler.get();} static bool SetDBHandler(std::shared_ptr<DBQueryHandler> &pHandler){if(pHandler){m_pHandler=pHandler;return true;}return false;}
#define BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(CLASS_NAME,INITIALIZER,DESTRUCTOR,CHILDCOUNT)\
					class CLASS_NAME :public CSQLData { public: CLASS_NAME(EDITSTATE eState):CSQLData(eState,CHILDCOUNT){Initialize();INIT_DATA_ELEMENTS;INITIALIZER} DECLARE_CREATE_NEW(CLASS_NAME) virtual ~CLASS_NAME(void){DESTRUCTOR} DBQueryHandler* GetDBHandler(){return m_pHandler.get();} static bool SetDBHandler(std::shared_ptr<DBQueryHandler> &pHandler){if(pHandler){m_pHandler=pHandler;return true;}return false;}


#define BEGIN_CLASS_FROM_SQLDATA_FROM_COMMON_CLASS(CLASS_NAME,COMMON_CLASS,INITIALIZER,DESTRUCTOR)\
					class CLASS_NAME :public CSQLData ,public COMMON_CLASS { public: CLASS_NAME(EDITSTATE eState):CSQLData(eState) {INIT_DATA_ELEMENTS;INITIALIZER;} DECLARE_CREATE_NEW(CLASS_NAME) virtual ~CLASS_NAME(void){DESTRUCTOR} DBQueryHandler* GetDBHandler(){return m_pHandler.get();} static bool SetDBHandler(std::shared_ptr<DBQueryHandler> &pHandler){if(pHandler){m_pHandler=pHandler;return true;}return false;}
#define BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS_FROM_COMMON_CLASS(CLASS_NAME,COMMON_CLASS,INITIALIZER,DESTRUCTOR,CHILDCOUNT)\
					class CLASS_NAME :public CSQLData ,public COMMON_CLASS{ public: CLASS_NAME(EDITSTATE eState):CSQLData(eState,CHILDCOUNT){Initialize();INIT_DATA_ELEMENTS;INITIALIZER} DECLARE_CREATE_NEW(CLASS_NAME) virtual ~CLASS_NAME(void){DESTRUCTOR} DBQueryHandler* GetDBHandler(){return m_pHandler.get();} static bool SetDBHandler(std::shared_ptr<DBQueryHandler> &pHandler){if(pHandler){m_pHandler=pHandler;return true;}return false;}



#define END_CLASS_FROM_SQLDATA	};

#define BEGIN_MAPPING_MEMBERS	void MappingMembers(){

#define END_MAPPING_MEMBERS		}

#define OVERRIDE_DUMMY_EDITOR_FUNC		CEditSQLData* GetEditor(int nRow){return NULL;};


#define COMMON_STATIC_VARIABLES_FOR_SQLDATA static int m_nParentIndexCol;static int m_nOrderCol;static int m_nIndexCol;static int m_nTableMaxIdx;static char szTableName[128];static std::shared_ptr<DBQueryHandler> m_pHandler;

#define MAPPING_MEMBERS(ORDER,B) m_tSettings[ORDER].POINTER=(unsigned int)B;


#define VECTOR_CLASS(CLASS_NAME)	m_v##CLASS_NAME
#define VECTOR_DELETE_CLASS(CLASS_NAME) m_v##CLASS_NAME##DelItems
#define EDITOR_POINTER(CLASS_NAME)	m_p##CLASS_NAME##Editor
#define DECLARE_VECTOR_FOR_CLASS(CLASS_NAME)			std::vector<std::shared_ptr<CSQLData>> VECTOR_CLASS(CLASS_NAME);std::vector<std::shared_ptr<CSQLData>> VECTOR_DELETE_CLASS(CLASS_NAME);
#define DECLARE_EDITOR_POINTER_FOR_CLASS(CLASS_NAME)		std::shared_ptr<CEditSQLData> EDITOR_POINTER(CLASS_NAME);
#define CREATE_EDITOR_CLASS_FOR_CLASS(CLASS_NAME)		EDITOR_POINTER(CLASS_NAME)=std::shared_ptr<CEditSQLData>(new CLASS_NAME##Editor());

#define TEMP_VECTOR_CLASS(CLASS_NAME)	vTemp##CLASS_NAME
#define DECLARE_TEMP_VECTOR_CLASS(CLASS_NAME)	std::vector<std::shared_ptr<CSQLData>> TEMP_VECTOR_CLASS(CLASS_NAME);

#define SAVEDATA_FOR_CLASS(CLASS_NAME)	EDITOR_POINTER(CLASS_NAME)->SaveData();

#define	INIT_EDITORCLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		{CEditSQLData *pBC=(CEditSQLData*)EDITOR_POINTER(CLASS_NAME).get();\
		CLASS_NAME::SetDBHandler(DBHANDLER);CLASS_NAME##Editor::SetDBEditor(DBEDITOR);\
		pBC->SetVectors(&VECTOR_CLASS(CLASS_NAME),&VECTOR_DELETE_CLASS(CLASS_NAME));\
		pBC->AlterTable();pBC->CreateTable();pBC->InitTable();pBC->GetMaxIndex();};	

#define INIT_EDITORCLASS_WITH_TEMPVECTORS(CLASS_NAME,VECTOR,VECTORDEL,DBHANDLER,DBEDITOR)\
		{CEditSQLData *pBC=(CEditSQLData*)EDITOR_POINTER(CLASS_NAME).get();\
		CLASS_NAME::SetDBHandler(DBHANDLER);CLASS_NAME##Editor::SetDBEditor(DBEDITOR);\
		pBC->SetVectors(VECTOR,VECTORDEL);pBC->AlterTable(); pBC->CreateTable();pBC->InitTable();pBC->GetMaxIndex();};


#define SETDBHANDLEREDITOR_CLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		CLASS_NAME::SetDBHandler(DBHANDLER);CLASS_NAME##Editor::SetDBEditor(DBEDITOR);

#define VECTOR_CLASSSAVE(CLASS_NAME)	m_v##CLASS_NAME##Save
#define DECLARE_VECTOR_FOR_CLASSSAVE(CLASS_NAME)			std::vector<std::shared_ptr<CSQLData>> VECTOR_CLASSSAVE(CLASS_NAME);
#define	TEMP_INIT_EDITORCLASSSAVENSAVE(CLASS_NAME,DBHANDLER,DBEDITOR)\
		{CEditSQLData *pBC=(CEditSQLData*)EDITOR_POINTER(CLASS_NAME).get();\
		SETDBHANDLEREDITOR_CLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		pBC->AlterTable();pBC->CreateTable();\
		pBC->SetVectors(&VECTOR_CLASSSAVE(CLASS_NAME),0);\
		pBC->SaveData();}	

#define	TEMP_INIT_EDITORCLASSSAVE(CLASS_NAME,DBHANDLER,DBEDITOR)\
		{SETDBHANDLEREDITOR_CLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		CEditSQLData *pBC=(CEditSQLData*)EDITOR_POINTER(CLASS_NAME).get();\
		pBC->AlterTable();pBC->CreateTable();}\

#define REVERT_BACK_EDITORCLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		{SETDBHANDLEREDITOR_CLASS(CLASS_NAME,DBHANDLER,DBEDITOR)\
		CEditSQLData *pBC=(CEditSQLData*)EDITOR_POINTER(CLASS_NAME).get();\
		pBC->SetVectors(&VECTOR_CLASS(CLASS_NAME),&VECTOR_DELETE_CLASS(CLASS_NAME));}

#define SET_SAVEVECTOR(CLASS_NAME)\
	for (std::vector<std::shared_ptr<CSQLData>>::iterator it = VECTOR_CLASS(CLASS_NAME).begin(); it!=VECTOR_CLASS(CLASS_NAME).end(); it++)\
	{\
		VECTOR_CLASSSAVE(CLASS_NAME).push_back(std::shared_ptr<CSQLData>(it->get()->Clone()));\
	}

#define CLEAR_SAVEVECTOR(CLASS_NAME)\
		VECTOR_CLASSSAVE(CLASS_NAME).clear();




typedef struct _TYPESETTINGS
{
	TYPEDEFINE nTYPE;
	char szFIELDNAME[256];
	int nLENGTH;
	unsigned int POINTER;
	bool bPRIMARYKEY;
	bool bORDER;
	bool bPARENTINDEX;
	char szCOLUMNNAME[128];
	int nColWidth;
	bool bUTF8Convert;
	char szJSONName[128];
}TYPESETTINGS,*LPTYPESETTINGS;







typedef struct _CHILDITEM
{
	std::vector<SHARED_PTRC(CSQLData)> vSQLData;
	std::vector<SHARED_PTRC(CSQLData)> vSQLDataDelItems;
}CHILDITEM,*LPCHILDITEM;






class CSQLData
{
public:
	CSQLData(EDITSTATE eState,int nChildCount=0);
	virtual ~CSQLData(void);

private:
	EDITSTATE m_eRecordState;	//��ϻ���
	int m_nChildCount;
public:
	int m_nParentIndex;
	int m_nTableIndex;
public:
	void Initialize();
	bool SetRecordState(EDITSTATE nStatus);
	int GetRecordState();
	void SetIndex(int nIndex);
	int GetIndex();
	
	static void GetSelectQuery(char *lpStr,int nCount, char *pszTableName,TYPESETTINGS *pSettings,int nMember);
	static void GetCreateQuery(char *lpStr,int nCount, char *pszTableName,TYPESETTINGS *pSettings,int nMember);
	static void GetInsertQuery(char *lpStr,int nCount, char *pszTableName,TYPESETTINGS *pSettings,int nMember, std::shared_ptr<SQLDataElement> *pElement=0);
	static void GetUpdateQuery(char *lpStr,int nCount, char *pszTableName,TYPESETTINGS *pSettings,int nMember, std::shared_ptr<SQLDataElement> *pElement=0);
	static void GetDeleteQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember);
	static void GetMaxIndexQuery(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember);
	static void GetAlterTableAddColumn(char *lpStr, int nCount, char *pszTableName, TYPESETTINGS *pSettings, int nMember);

	bool CommitData(int nOrder);
	bool SetData(int nField,void *pValue,int nSize=0);
	int GetData(int nField,void *pValue);
	int GetJSONData(int nField, std::string &strTemp);
	char *GetJSONFieldName(int nField);
	int GetFieldData(int nField, std::string &strData);
	int GetSize(int nField);

	int GetChildCount();


	virtual void MappingMembers()=0;
	virtual bool GetTypeSettingParam(TYPESETTINGS **pSettings,int *pParam)=0;
	virtual char *GetTableName()=0;
	virtual CEditSQLData* GetEditor(int nRow) = 0;
	
	
	virtual int GetIndexCol() = 0;
	virtual int GetInsertIndex()=0;
	virtual int GetOrderCol() = 0;
	virtual int GetParentIndexCol()=0;
	virtual void SetTableName(char *pTable) = 0;

	

	virtual bool InsertExec();
	virtual bool DeleteExec();
	virtual bool UpdateExec();
	virtual DBQueryHandler* GetDBHandler() = 0;
	std::shared_ptr<SQLDataElement> *m_pDataElem;

	CSQLData* CloneCopied();
	CSQLData* Clone();
	virtual void SetAdditionalMembers(CSQLData *pData);
	virtual CSQLData* CreateNew()=0;

	void RemoveData(int nIndex, int nRow);
	std::vector<CHILDITEM> m_vChildItem;
protected:
	void SQLClassInit(TYPESETTINGS *pSettings, std::shared_ptr<SQLDataElement> *pSQLElem, int nMember);

};


struct findSQLData : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findSQLData(int nIndex)
		:m_nIndex(nIndex)
	{

	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		return (pSI->GetIndex() == m_nIndex);
	}
private:
	int m_nIndex;
};

class CEditSQLData
{
public:
	CEditSQLData()
		:m_pvSQLData(NULL), m_pvSQLDataDelItems(NULL)
	{

	}
	virtual ~CEditSQLData()
	{
	}

	int m_nCurIndex;


	void SetCurParentIndex(int nIndex)
	{
		m_nCurIndex = nIndex;
	}
	int GetCurParentIndex()
	{
		return m_nCurIndex;
	}
	void InsertData(int nRow, SHARED_PTRC(CSQLData) &pData)
	{

		pData->SetRecordState(EDIT_INSERTED);
		pData->SetIndex(pData->GetInsertIndex());
		if (nRow == -1)
			m_pvSQLData->push_back(pData);
		else
			m_pvSQLData->insert(m_pvSQLData->begin() + nRow, 1, pData);
	}

	bool RemoveData(int nRow)
	{
		int nChildCount, nChildCounts;
		if ((*m_pvSQLData).size() > nRow)
		{
			switch ((*m_pvSQLData)[nRow]->GetRecordState())
			{
			case EDIT_UPDATED:
				(*m_pvSQLData)[nRow]->SetRecordState(EDIT_DELETED);
				nChildCount = (*m_pvSQLData)[nRow]->GetChildCount();
				for (int i = 0; i < nChildCount; i++)
				{
					while ((*m_pvSQLData)[nRow]->m_vChildItem[i].vSQLData.size())
					{
						(*m_pvSQLData)[nRow]->RemoveData(i, 0);
					}
				}
				break;
			case EDIT_NOT_MODIFIED:
				(*m_pvSQLData)[nRow]->SetRecordState(EDIT_DELETED);
				nChildCount = (*m_pvSQLData)[nRow]->GetChildCount();
				for (int i = 0; i < nChildCount; i++)
				{
					while ((*m_pvSQLData)[nRow]->m_vChildItem[i].vSQLData.size())
					{
						(*m_pvSQLData)[nRow]->RemoveData(i, 0);
					}
				}
				break;
			default:
				(*m_pvSQLData)[nRow]->SetRecordState(EDIT_TO_BE_IGNORED);//Inserted �̸�
				break;
			}
			(*m_pvSQLDataDelItems).insert((*m_pvSQLDataDelItems).end(), (*m_pvSQLData).begin() + nRow, (*m_pvSQLData).begin() + nRow + 1);
			(*m_pvSQLData).erase((*m_pvSQLData).begin() + nRow);
			return true;
		}
		return false;
	}

	bool SaveData()
	{
		std::vector<SHARED_PTRC(CSQLData)>::iterator nit;
		if (m_pvSQLData&&m_pvSQLDataDelItems)
		{
			for (nit = m_pvSQLData->begin(); nit != m_pvSQLData->end(); nit++)
			{
				int nOrder = distance(m_pvSQLData->begin(), nit) + 1;
				(*nit)->CommitData(nOrder);
			}
			for (nit = m_pvSQLDataDelItems->begin(); nit != m_pvSQLDataDelItems->end(); nit++)
			{
				int nOrder = distance(m_pvSQLDataDelItems->begin(), nit) + 1;
				(*nit)->CommitData(nOrder);
			}
			return true;
		}
		return false;
	}


	virtual void Insert(int nRow)
	{
		SHARED_PTRC(CSQLData) pData = SHARED_PTRC(CSQLData)(FeedInserted());
		InsertData(nRow, pData);	// 
	}

	virtual void Remove(int nRow)
	{
		RemoveData(nRow);
	}

	void SetVectors(std::vector<SHARED_PTRC(CSQLData)> *pSQL, std::vector<SHARED_PTRC(CSQLData)> *pSQLDel)
	{
		if (pSQL)
			m_pvSQLData = pSQL;
		if (pSQLDel)
			m_pvSQLDataDelItems = pSQLDel;
	}

	void AddJSONColumn(int nColumn)
	{
		m_vJSONColumn.push_back(nColumn);
	}
	void ResetJSONColumn()
	{
		m_vJSONColumn.clear();
#ifdef JSONCOLUMNFUNC
		m_mFunc.clear();
#endif
	}

#ifdef JSONCOLUMNFUNC

	bool ConvertFunction(std::vector<SHARED_PTRC(CSQLData)> &V, int nColumnTarget,int nIndex, std::string &strTemp)
	{
		std::vector<SHARED_PTRC(CSQLData)>::iterator vit=find_if(V.begin(), V.end(), findSQLData(nIndex));
		if (vit != V.end())
		{
			(*vit)->GetJSONData(nColumnTarget, strTemp);
			return true;
		}
		return false;
	}

	bool ConvertFunctionFieldData(std::vector<SHARED_PTRC(CSQLData)> &V, int nColumnTarget, int nIndex, std::string &strTemp)
	{
		std::vector<SHARED_PTRC(CSQLData)>::iterator vit = find_if(V.begin(), V.end(), findSQLData(nIndex));
		if (vit != V.end())
		{
			(*vit)->GetFieldData(nColumnTarget, strTemp);
			return true;
		}
		return false;
	}


	std::map<int,std::function<bool(int,std::string&)>> m_mFunc;
	void AddJSONColumnToFunction(int nColumn, std::function<bool(int, std::string&)> tFunc)
	{
		m_vJSONColumn.push_back(nColumn);
		m_mFunc[nColumn] = tFunc;
	}
#endif

	int GetUpdatedJSONColumn(std::string &strJSON)
	{
		int nCounted = 0;
		bool bFirst = false;
		int nSize = 0, nDataSize = 0;
		int nCount = 0, nDataCount = 0;
		std::vector<SHARED_PTRC(CSQLData)>::iterator vit;
		std::vector<int>::iterator xit;
		nSize = m_vJSONColumn.size();
		nDataSize = m_pvSQLData->size();
		if (nDataSize)
		{
			//strJSON.append("[");
			
			for (vit = m_pvSQLData->begin(), nDataCount=0; vit != m_pvSQLData->end(); vit++, nDataCount++)
			{
				switch ((*vit)->GetRecordState())
				{
				case EDIT_UPDATED:
					if (!bFirst)
					{
						bFirst = true;
						strJSON.append("[");
					}
					else
						strJSON.append(",");
					strJSON.append("{");
					for (xit = m_vJSONColumn.begin(), nCount = 0; xit != m_vJSONColumn.end(); xit++, nCount++)
					{
#ifdef JSONCOLUMNFUNC
						std::map<int, std::function<bool(int, std::string&)>>::iterator fit=m_mFunc.find((*xit));
						if (fit == m_mFunc.end())
						{
							(*vit)->GetJSONData((*xit), strJSON);
							if ((nCount + 1) != nSize)
							{
								strJSON.append(",");
							}
						}
						else
						{
							int nIndex = 0;
							if ((*vit)->GetData((*xit), &nIndex) != -1)
							{
								std::string strFieldData;
								char *pFieldName=(*vit)->GetJSONFieldName((*xit));
								if (fit->second(nIndex, strFieldData))
								{
									if (pFieldName)
									{
										strJSON.append("\"");
										strJSON.append(pFieldName);
										strJSON.append("\":");
										strJSON.append(strFieldData);
										if ((nCount + 1) != nSize)
										{
											strJSON.append(",");
										}
									}
								}
							}
						}
#else
						(*vit)->GetJSONData((*xit), strJSON);
						if ((nCount + 1) != nSize)
						{
							strJSON.append(",");
						}
				
#endif
					}
					nCounted++;
					strJSON.append("}");
					break;
				}
			}
			if (bFirst)
				strJSON.append("]");
			//strJSON.append("]");
		}
		return nCounted;
	}

	int GetAllJSONColumn(std::string &strJSON)
	{
		int nSize = 0, nDataSize = 0;
		int nCount = 0, nDataCount = 0;
		std::vector<SHARED_PTRC(CSQLData)>::iterator vit;
		std::vector<int>::iterator xit;
		nSize = m_vJSONColumn.size();
		nDataSize = m_pvSQLData->size();
		if (nDataSize)
		{
			strJSON.append("[");
			for (vit = m_pvSQLData->begin(), nDataCount = 0; vit != m_pvSQLData->end(); vit++, nDataCount++)
			{
				strJSON.append("{");
				for (xit = m_vJSONColumn.begin(), nCount = 0; xit != m_vJSONColumn.end(); xit++, nCount++)
				{
#ifdef JSONCOLUMNFUNC
					std::map<int, std::function<bool(int, std::string&)>>::iterator fit = m_mFunc.find((*xit));
					if (fit == m_mFunc.end())
					{
						(*vit)->GetJSONData((*xit), strJSON);
						if ((nCount + 1) != nSize)
						{
							strJSON.append(",");
						}
					}
					else
					{
						int nIndex = 0;
						if ((*vit)->GetData((*xit), &nIndex) != -1)
						{
							int nIndex = 0;
							if ((*vit)->GetData((*xit), &nIndex) != -1)
							{
								std::string strFieldData;
								char *pFieldName = (*vit)->GetJSONFieldName((*xit));
								if (fit->second(nIndex, strFieldData))
								{
									if (pFieldName)
									{
										strJSON.append("\"");
										strJSON.append(pFieldName);
										strJSON.append("\":");
										strJSON.append(strFieldData);
										if ((nCount + 1) != nSize)
										{
											strJSON.append(",");
										}
									}
								}
							}
						}
					}
#else
					(*vit)->GetJSONData((*xit), strJSON);
					if ((nCount + 1) != nSize)
					{
						strJSON.append(",");
					}
#endif
				}
				strJSON.append("}");
				if ((nDataCount + 1) != nDataSize)
				{
					strJSON.append(",");
				}
			}
			strJSON.append("]");
		}
		return nDataSize;
	}

	void GetDeletedJSONColumn(std::string &strJSON)
	{

	}

	int GetInsertedJSONColumn(std::string &strJSON)
	{
		int nCounted = 0;
		bool bFirst = false;
		int nSize = 0, nDataSize = 0;
		int nCount = 0, nDataCount = 0;
		std::vector<SHARED_PTRC(CSQLData)>::iterator vit;
		std::vector<int>::iterator xit;
		nSize = m_vJSONColumn.size();
		nDataSize = m_pvSQLData->size();
		if (nDataSize)
		{
			//strJSON.append("[");

			for (vit = m_pvSQLData->begin(), nDataCount = 0; vit != m_pvSQLData->end(); vit++, nDataCount++)
			{
				switch ((*vit)->GetRecordState())
				{
				case EDIT_INSERTED:
					if (!bFirst)
					{
						bFirst = true;
						strJSON.append("[");
					}
					else
						strJSON.append(",");
					strJSON.append("{");
					for (xit = m_vJSONColumn.begin(), nCount = 0; xit != m_vJSONColumn.end(); xit++, nCount++)
					{
#ifdef JSONCOLUMNFUNC
						std::map<int, std::function<bool(int, std::string&)>>::iterator fit = m_mFunc.find((*xit));
						if (fit == m_mFunc.end())
						{
							(*vit)->GetJSONData((*xit), strJSON);
							if ((nCount + 1) != nSize)
							{
								strJSON.append(",");
							}
						}
						else
						{
							int nIndex = 0;
							if ((*vit)->GetData((*xit), &nIndex) != -1)
							{
								int nIndex = 0;
								if ((*vit)->GetData((*xit), &nIndex) != -1)
								{
									std::string strFieldData;
									char *pFieldName = (*vit)->GetJSONFieldName((*xit));
									if (fit->second(nIndex, strFieldData))
									{
										if (pFieldName)
										{
											strJSON.append("\"");
											strJSON.append(pFieldName);
											strJSON.append("\":");
											strJSON.append(strFieldData);
											if ((nCount + 1) != nSize)
											{
												strJSON.append(",");
											}
										}
									}
								}
							}
						}
#else
						(*vit)->GetJSONData((*xit), strJSON);
						if ((nCount + 1) != nSize)
						{
							strJSON.append(",");
						}
#endif
					}
					strJSON.append("}");
					break;
				}
				nCounted++;
			}
			if (bFirst)
				strJSON.append("]");
			//strJSON.append("]");
		}
		return nCounted;
	}


public:
	virtual char *GetTableName() = 0;
	virtual CSQLData *FeedInserted() = 0;
	virtual CSQLData *FeedNotModified() = 0;
public:
	virtual TYPESETTINGS* GetTypeSettingsTable() = 0;
	virtual int GetTypeSettingsCount() = 0;
	virtual void SetMaxIndex(int nMaxIndex) = 0;

	std::vector<SHARED_PTRC(CSQLData)> *m_pvSQLData;
	std::vector<SHARED_PTRC(CSQLData)> *m_pvSQLDataDelItems;
	std::vector<int> m_vJSONColumn;
public:
	bool QueryExec(const char *lpQuery)
	{
		DBEditor *pEditor = GetDBEditor();
		if (pEditor)
			return pEditor->QueryExec(lpQuery);
		return false;
	}
	bool CreateTable();
	bool AlterTable();
	bool InitTable()
	{
		DBEditor *pEditor = GetDBEditor();
		if(pEditor)
			return pEditor->InitTable(this);
		return false;
	}
	bool GetMaxIndex()
	{
		DBEditor *pEditor = GetDBEditor();
		if (pEditor)
			return pEditor->GetMaxIndex(this);
		return false;
	}
	virtual DBEditor* GetDBEditor() = 0;
};



#include "SQLData.hpp"

template <typename T>
void MakeRelationShip(std::vector<SHARED_PTRC(CSQLData)> *pParent, int nChildIndex, std::vector<SHARED_PTRC(CSQLData)> *pChild);

/*
struct findSQLData : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findSQLData(int nIndex)
		:m_nIndex(nIndex)
	{

	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		return (pSI->GetIndex() == m_nIndex);
	}
private:
	int m_nIndex;
};*/


#endif

