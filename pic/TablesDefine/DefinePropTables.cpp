#include "DefinePropTables.h"


// CLASS_NAME,BLOEXIST,TABLENAME,PARENT_INDEX_COLUMN,ORDER_COLUMN,INDEX_COLUMN
IMPLEMENT_STATIC_VARIABLES(PropSection, PROP_SECTION, -1, 1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(PropSection, 4) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100},
#ifndef _WIN32
	{TYPE_WTEXT,"SECTION_TITLE",64,NULL,false,false,false, "Section Title", 100},
	{TYPE_WTEXT,"SECTION_TITLE",128,NULL,false,false,false, "Section Description", 100},
#else
	{ TYPE_WTEXT,"SECTION_TITLE",64,NULL,false,false,false, "Section Title", 200 },
	{ TYPE_WTEXT,"SECTION_TITLE",128,NULL,false,false,false, "Section Description", 200 },
#endif
};

IMPLEMENT_STATIC_VARIABLES(PropKeys, PROP_KEYS, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(PropKeys, 6) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100},
#ifndef _WIN32
	{TYPE_TEXT,"KEY_TITLE",64,NULL,false,false,false, "Key Title", 100},
	{TYPE_TEXT,"KEY_VALUE",128,NULL,false,false,false, "Key Value", 100},
#else
	{ TYPE_WTEXT,"KEY_TITLE",64,NULL,false,false,false, "Key Title", 200 },
	{ TYPE_WTEXT,"KEY_VALUE",128,NULL,false,false,false, "Key Value", 200 },
#endif
	{TYPE_INTEGER,"KEY_TYPE",0,NULL,false,false,false, "Key Type", 60},
};


