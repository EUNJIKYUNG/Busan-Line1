#ifndef DEFINE_TABLES
#define DEFINE_TABLES
#include "../DataRelated/SQLData.h"
#include "../DataRelated/SQLDataElement.h"


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(PropSection, nOrder = 0; memset(szChTitle, 0, sizeof(szChTitle)); memset(szDescription, 0, sizeof(szDescription));, , 1);
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex)
		MM(1, &nOrder)
		MM(2, szChTitle)
		MM(3, szDescription)
	END_MAPPING_MEMBERS
		int nOrder;
#ifndef _WIN32
		char szChTitle[64];
		char szDescription[128];
#else
		wchar_t szChTitle[64];
		wchar_t szDescription[128];
#endif
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(PropSection)


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(PropKeys, nParent = nOrder = nType = 0; memset(szKeyTitle, 0, sizeof(szKeyTitle)); memset(szKeyValue, 0, sizeof(szKeyValue)); , , 1);
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(6)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex)
		MM(1, &nParent)
		MM(2, &nOrder)
		MM(3, szKeyTitle)
		MM(4, szKeyValue)
		MM(5, &nType)
	END_MAPPING_MEMBERS
	int nParent;
	int nOrder;
	int nType;
#ifndef _WIN32
	char szKeyTitle[64];
	char szKeyValue[128];
#else
	wchar_t szKeyTitle[64];
	wchar_t szKeyValue[128];
#endif
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(PropKeys)



#endif