#include "DefineTables.h"


// CLASS_NAME,BLOEXIST,TABLENAME,PARENT_INDEX_COLUMN,ORDER_COLUMN,INDEX_COLUMN
IMPLEMENT_STATIC_VARIABLES(StationInformation, STATION_INFORMATION, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(StationInformation, 17) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false,"id"},
	{TYPE_INTEGER,"STATION_CODE",0,NULL,false,false,false, "Station Code", 100,false},
	{TYPE_WTEXT,"STATION_NAME1",96,NULL,false,false,false, "Station Name1", 100,true,"name"},
	{TYPE_WTEXT,"STATION_NAME2",96,NULL,false,false,false, "Station Name2", 100,true},
	{TYPE_WTEXT,"STATION_NAME3",96,NULL,false,false,false, "Station Name3", 100,true},
	{TYPE_WTEXT,"STATION_NAME4",96,NULL,false,false,false, "Station Name4", 100,true},
	{TYPE_WTEXT,"STATION_NAME5",96,NULL,false,false,false, "Station Name5", 100,true},
	{TYPE_WTEXT,"STATION_NAME6",96,NULL,false,false,false, "Station Name6", 100,true},
	{TYPE_WTEXT,"STATION_NAME7",96,NULL,false,false,false, "Station Name7", 100,true},
	{TYPE_WTEXT,"STATION_NAME8",96,NULL,false,false,false, "Station Name8", 100,true},
	{TYPE_DOUBLE,"LATITUDE",0,NULL,false,false,false, "Latitude",100,false},
	{TYPE_DOUBLE,"LONGITUDE",0,NULL,false,false,false,"Longitude", 100,false},
	{TYPE_INTEGER,"STATION_TYPE",0,NULL,false,false,false, "Station Type", 100,false},
	{TYPE_INTEGER,"TOTALWIDTH",0,NULL,false,false,false, "Total Width", 100,false},
	{TYPE_INTEGER,"TOTALHEIGHT",0,NULL,false,false,false, "Total Height", 100,false},
	{TYPE_INTEGER,"ADD_PROP1",0,NULL,false,false,false, "Related Station", 100,false},
	{TYPE_INTEGER,"ADD_PROP2",0,NULL,false,false,false, "Add Prop2", 100,false},

};



IMPLEMENT_STATIC_VARIABLES(DestinationInformation, DESTINATION_INFORMATION, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(DestinationInformation, 17) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false,"id"},
	{TYPE_INTEGER,"DESTINATION_CODE",0,NULL,false,false,false, "Destination Code", 100,false},
	{TYPE_WTEXT,"DESTINATION_NAME1",96,NULL,false,false,false, "Destination Name1", 100,true,"name"},
	{TYPE_WTEXT,"DESTINATION_NAME2",96,NULL,false,false,false, "Destination Name2", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME3",96,NULL,false,false,false, "Destination Name3", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME4",96,NULL,false,false,false, "Destination Name4", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME5",96,NULL,false,false,false, "Destination Name5", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME6",96,NULL,false,false,false, "Destination Name6", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME7",96,NULL,false,false,false, "Destination Name7", 100,true},
	{TYPE_WTEXT,"DESTINATION_NAME8",96,NULL,false,false,false, "Destination Name8", 100,true},
	{TYPE_DOUBLE,"LATITUDE",0,NULL,false,false,false, "Latitude",100,false},
	{TYPE_DOUBLE,"LONGITUDE",0,NULL,false,false,false,"Longitude", 100,false},
	{TYPE_INTEGER,"DESTINATION_TYPE",0,NULL,false,false,false, "Destination Type", 100,false},
	{TYPE_INTEGER,"TOTALWIDTH",0,NULL,false,false,false, "Total Width", 100,false},
	{TYPE_INTEGER,"TOTALHEIGHT",0,NULL,false,false,false, "Total Height", 100,false},
	{TYPE_INTEGER,"ADD_PROP1",0,NULL,false,false,false, "Add Prop1", 100,false},
	{TYPE_INTEGER,"ADD_PROP2",0,NULL,false,false,false, "Add Prop2", 100,false},

};

IMPLEMENT_STATIC_VARIABLES(StationDistance, STATION_DISTANCE, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(StationDistance, 5) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"DEPARTURE_STATION",0,NULL,false,false,false, "Departure Station", 100,false},
	{TYPE_INTEGER,"ARRIVAL_STATION",0,NULL,false,false,false, "Arrival Station", 100,false},
	{TYPE_INTEGER,"DISTANCE",0,NULL,false,false,false, "Distance", 100,false},
	{TYPE_WTEXT,"DESC",96,NULL,false,false,false, "Desc", 100,true},
};

IMPLEMENT_STATIC_VARIABLES(StopPtnHeader, STOP_PATTERN_HEADER, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(StopPtnHeader, 5) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false,"id"},
	{TYPE_INTEGER,"DEPARTURE_STATION",0,NULL,false,false,false, "Departure Station", 100,false},
	{TYPE_INTEGER,"ARRIVAL_STATION",0,NULL,false,false,false, "Arrival Station", 100,false},
	{TYPE_WTEXT,"DESCRIPTION",96,NULL,false,false,false, "Description", 100,true,"name"},
	{TYPE_INTEGER,"LINE_MAP_INDEX",0,NULL,false,false,false, "Line Map Index", 100,false},
};

IMPLEMENT_STATIC_VARIABLES(StopPtnRoutes, STOP_PATTERN_ROUTES, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(StopPtnRoutes, 13) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false,"id"},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,true, "Table Order", 100,false},
	{TYPE_INTEGER,"DISTANCE_INDEX",0,NULL,false,false,false, "Distance Index", 100,false,"name"},
	{TYPE_INTEGER,"DESTINATION_INDEX",0,NULL,false,false,false, "Destination Index(LED)", 100,false},
	{TYPE_INTEGER,"DOOR_DIRECTION",0,NULL,false,false,false, "Door Direction", 100,false},
	{TYPE_INTEGER,"DESTINATION_INDEX2",0,NULL,false,false,false, "Destination Index(PIDDRM)", 100 ,false},
	{ TYPE_INTEGER,"DOORCLOSE_VOICEEVENT",0,NULL,false,false,false, "Door Close Voice Event", 100 ,false},
	{ TYPE_INTEGER,"DOORCLOSE_VIDEOEVENT",0,NULL,false,false,false, "Door Close Video Event", 100 ,false},
	{ TYPE_INTEGER,"DOORCLOSE_DRMEVENT",0,NULL,false,false,false, "Door Close DRM Event", 100 ,false},
	{ TYPE_INTEGER,"DOOROPEN_VOICEEVENT",0,NULL,false,false,false, "Door Open Voice Event", 100 ,false},
	{ TYPE_INTEGER,"DOOROPEN_VIDEOEVENT",0,NULL,false,false,false, "Door Open Video Event", 100 ,false},
	{ TYPE_INTEGER,"DOOROPEN_DRMEVENT",0,NULL,false,false,false, "Door Open DRM Event", 100 ,false},
};

IMPLEMENT_STATIC_VARIABLES(EventLists, EVENT_LISTS, 1, 5, 0)

IMPLEMENT_STATIC_TYPESETTINGS(EventLists, 11) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false,"id"},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"DISTANCE",0,NULL,false,false,false, "Distance", 100,false},
	{TYPE_INTEGER,"VOICE_EVENT_INDEX",0,NULL,false,false,false, "Voice Event Index", 100,false},
	{TYPE_INTEGER,"VIDEO_EVENT_INDEX",0,NULL,false,false,false, "Video Event Index", 100,false},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100,false},
	{TYPE_WTEXT,"DESCRIPTION",128,NULL,false,false,false, "Description", 100,false,"name"},
	{TYPE_INTEGER,"DRM_EVENT_INDEX",0,NULL,false,false,false, "DRM Event Index", 100 ,false},
	{TYPE_INTEGER,"EVENT_TYPES",0,NULL,false,false,false, "Event Type", 100 ,false},
	{TYPE_INTEGER,"DISTANCE_TYPES",0,NULL,false,false,false, "Distance Type", 100 ,false},
	{TYPE_INTEGER,"PII_TEXT_INDEX",0,NULL,false,false,false, "PII Text Index", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(AudioFilePool, AUDIO_FILE_POOL, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(AudioFilePool, 8) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"FILE_CODE",0,NULL,false,false,false, "File Code", 100,false},
	{TYPE_INTEGER,"SPARE_CODE",0,NULL,false,false,false, "Spare Code", 100,false},
	{TYPE_TEXT,"FILE_NAME",128,NULL,false,false,false, "File Name", 100,false},
	{TYPE_INTEGER,"FILE_CRC",0,NULL,false,false,false, "Duration", 100,false},
	{TYPE_INTEGER,"FILE_SIZE",0,NULL,false,false,false, "Size", 100,false},
	{TYPE_TEXT,"MD5HASH",64,NULL,false,false,false, "MD5Hash", 100,false},
	{TYPE_TEXT,"FILE_EXT",16,NULL,false,false,false, "File Ext", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(VideoFilePool, VIDEO_FILE_POOL, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(VideoFilePool, 8) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"FILE_CODE",0,NULL,false,false,false, "File Code", 100,false},
	{TYPE_INTEGER,"SPARE_CODE",0,NULL,false,false,false, "Spare Code", 100,false},
	{TYPE_WTEXT,"FILE_NAME",256,NULL,false,false,false, "File Name", 100,true},
	{TYPE_INTEGER,"FILE_CRC",0,NULL,false,false,false, "Duration", 100,false},
	{TYPE_INTEGER,"FILE_SIZE",0,NULL,false,false,false, "Size", 100,false},
	{TYPE_TEXT,"MD5HASH",64,NULL,false,false,false, "MD5Hash", 100,false},
	{TYPE_TEXT,"FILE_EXT",16,NULL,false,false,false, "File Ext", 100,false},
};



IMPLEMENT_STATIC_VARIABLES(VideoContents, VIDEO_CONTENTS, -1, 6, 0)

IMPLEMENT_STATIC_TYPESETTINGS(VideoContents, 7) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_TEXT,"FILE_DESCRIPTION",128,NULL,false,false,false, "File Description", 100,false},
	{TYPE_INTEGER,"MESSAGE_ID",0,NULL,false,false,false, "Message ID", 100,false},
	{TYPE_INTEGER,"MESSAGE_TYPE",0,NULL,false,false,false, "Message Type", 100,false},
	{TYPE_INTEGER,"STATION_INDEX",0,NULL,false,false,false, "Station Index", 100,false},
	{TYPE_INTEGER,"DESTINATION_INDEX",0,NULL,false,false,false, "Destination Index", 100,false},
	{ TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Order", 100 ,false},
};


IMPLEMENT_STATIC_VARIABLES(AudioContents, AUDIO_CONTENTS, -1, 8, 0)

IMPLEMENT_STATIC_TYPESETTINGS(AudioContents, 9) =
{
	{ TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{ TYPE_TEXT,"FILE_DESCRIPTION",128,NULL,false,false,false, "File Description", 100,false},
	{ TYPE_INTEGER,"MESSAGE_ID",0,NULL,false,false,false, "Message ID", 100,false},
	{ TYPE_INTEGER,"MESSAGE_TYPE",0,NULL,false,false,false, "Train Number", 100,false},
	{ TYPE_INTEGER,"STATION_INDEX",0,NULL,false,false,false, "Station Index", 100,false},
	{ TYPE_INTEGER,"DESTINATION_INDEX",0,NULL,false,false,false, "Destination Index", 100,false},
	{ TYPE_INTEGER,"MESSAGE_TYPE_ID",0,NULL,false,false,false, "Message TypeID", 100 ,false},
	{ TYPE_INTEGER,"INTERNAL_INDEX",0,NULL,false,false,false, "Internal Index", 100 ,false},
	{ TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Order", 100 ,false},
};


IMPLEMENT_STATIC_VARIABLES(PIDContents, PID_CONTENTS, -1, 8, 0)

IMPLEMENT_STATIC_TYPESETTINGS(PIDContents, 10) =
{
	{ TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100 ,false},
	{ TYPE_TEXT,"FILE_DESCRIPTION",128,NULL,false,false,false, "File Description", 100 ,false},
	{ TYPE_INTEGER,"MESSAGE_ID",0,NULL,false,false,false, "Message ID", 100 ,false},
	{ TYPE_INTEGER,"MESSAGE_TYPE",0,NULL,false,false,false, "Train Number", 100 ,false},
	{ TYPE_INTEGER,"STATION_INDEX",0,NULL,false,false,false, "Station Index", 100 ,false},
	{ TYPE_INTEGER,"DESTINATION_INDEX",0,NULL,false,false,false, "Destination Index", 100 ,false},
	{ TYPE_INTEGER,"MESSAGE_TYPE_ID",0,NULL,false,false,false, "Message TypeID", 100 ,false},
	{ TYPE_INTEGER,"INTERNAL_INDEX",0,NULL,false,false,false, "Internal Index", 100 ,false},
	{ TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Order", 100 ,false},
	{ TYPE_INTEGER,"DEV_TYPE",0,NULL,false,false,false, "Dev Type", 100 ,false},
};

IMPLEMENT_STATIC_VARIABLES(PIDIndexList, PID_INDEX_LIST, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(PIDIndexList, 4) =
{
	{ TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100 ,false},
	{ TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100 ,false},
	{ TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100 ,false},
	{ TYPE_INTEGER,"CONTENTS_POOL_INDEX",0,NULL,false,false,false, "Contents Pool Index", 100 ,false},
};



IMPLEMENT_STATIC_VARIABLES(VideoIndexList, VIDEO_INDEX_LIST, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(VideoIndexList, 4) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100,false},
	{TYPE_INTEGER,"CONTENTS_POOL_INDEX",0,NULL,false,false,false, "Contents Pool Index", 100,false},
};

IMPLEMENT_STATIC_VARIABLES(AudioIndexList, AUDIO_INDEX_LIST, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(AudioIndexList, 4) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Table Order", 100,false},
	{TYPE_INTEGER,"CONTENTS_POOL_INDEX",0,NULL,false,false,false, "Contents Pool Index", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(TargetDevices, TARGET_DEVICES, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(TargetDevices, 3) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_TEXT,"ADDRESS",128,NULL,false,false,false, "Address", 100,false},
	{TYPE_TEXT,"DESCRIPTION",128,NULL,false,false,false, "Description", 100,false},
};




IMPLEMENT_STATIC_VARIABLES(UpdateFileTypeList, UPDATE_FILE_TYPELIST, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(UpdateFileTypeList, 7) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"TYPE_CODE",0,NULL,false,false,false, "Type Code", 100,false},
	{TYPE_TEXT,"REAL_PATH",128,NULL,false,false,false, "Real Path", 100,false},
	{TYPE_TEXT,"DESCRIPTION",128,NULL,false,false,false, "Description", 100,false},
	{ TYPE_TEXT,"FTP_ID",128,NULL,false,false,false, "FTP_ID", 100 ,false},
	{ TYPE_TEXT,"FTP_PW",128,NULL,false,false,false, "FTP_PW", 100 ,false},
	{TYPE_INTEGER,"ZIP",0,NULL,false,false,false, "Type Code", 100,false},
};

IMPLEMENT_STATIC_VARIABLES(UpdateFile, UPDATE_FILE, 1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(UpdateFile, 6) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"TYPE_CODE",0,NULL,false,false,false, "Type Code", 100,false},
	{TYPE_INTEGER,"FILE_SIZE",0,NULL,false,false,false, "File Size", 100,false},
	{TYPE_INTEGER,"FILE_CRC",0,NULL,false,false,false, "File CRC", 100,false},
	{TYPE_TEXT,"FILE_NAME",128,NULL,false,false,false, "File Name", 100,false},
};



IMPLEMENT_STATIC_VARIABLES(UpdateTarget, UPDATE_TARGET, 1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(UpdateTarget, 4) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
	{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Order", 100,false},
	{TYPE_INTEGER,"TARGET",0,NULL,false,false,false, "Target Device", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(FontPool, FONT_POOL, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(FontPool, 5) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"FILE_CODE",0,NULL,false,false,false, "File Code", 100,false},
	{TYPE_INTEGER,"SPARE_CODE",0,NULL,false,false,false, "Spare Code", 100,false},
	{TYPE_TEXT,"FILE_NAME",128,NULL,false,false,false, "File Name", 100,false},
	{TYPE_TEXT,"FONT_FACE",128,NULL,false,false,false, "Font Face", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(TrainNumber, TRAIN_NUMBER, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(TrainNumber, 7) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_INTEGER,"TRAIN_NUMBER_INDEX",0,NULL,false,false,false, "Train Number Index", 100,false},
	{TYPE_INTEGER,"DESTINATION_INDEX",0,NULL,false,false,false, "Destination Index", 100,false},
	{TYPE_INTEGER,"STOP_PATTERN_INDEX",0,NULL,false,false,false, "Stop Pattern Index", 100,false},
	{TYPE_TEXT,"TRAIN_NUMBER",128,NULL,false,false,false, "Train Number", 100,false},
	{TYPE_TEXT,"TRAIN_NUMBER_PREFIX",64,NULL,false,false,false, "Train Number Prefix", 100 ,false},
	{TYPE_INTEGER,"UPDN_DIRECTION",0,NULL,false,false,false, "Direction", 100 ,false},
};

IMPLEMENT_STATIC_VARIABLES(EDDIndex, EDD_INDEX, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(EDDIndex, 3) =
{
	//PRIMARYKEY,ORDER,PARENTINDEX
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_WTEXT,"DESCRIPTION",128,NULL,false,false,false, "Description", 100,false},
	{TYPE_INTEGER,"CODE",128,NULL,false,false,false, "Code", 100 ,false},
};


IMPLEMENT_STATIC_VARIABLES(EDDIndexList, EDD_INDEXLIST, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(EDDIndexList, 4) =
{
	//PRIMARYKEY,ORDER,PARENTINDEX
{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
{TYPE_INTEGER,"PARENT_INDEX",0,NULL,false,false,true, "Parent Index", 100,false},
{TYPE_INTEGER,"TABLE_ORDER",0,NULL,false,true,false, "Order", 100,false},
{TYPE_INTEGER,"EDD_INDEX",0,NULL,false,false,false, "EDD Index", 100,false},
};


IMPLEMENT_STATIC_VARIABLES(EditorTagTable, EDITOR_TAG_TABLE, 1, 2, 0)

IMPLEMENT_STATIC_TYPESETTINGS(EditorTagTable, 4) =
{
	//PRIMARYKEY,ORDER,PARENTINDEX
	{ TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100 ,false},
	{ TYPE_TEXT,"DESCRIPTION",128,NULL,false,false,false, "Tag name", 400 ,false},
	{ TYPE_INTEGER,"VARIABLE",0,NULL,false,false,false, "Variable", 500 ,false},
	{ TYPE_INTEGER,"SPARE",0,NULL,false,false,false, "Spare", 100 ,false},
};


IMPLEMENT_STATIC_VARIABLES(DisplayItemPool, DISPLAY_ITEM_POOL, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(DisplayItemPool, 6) =
{
	{ TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100 ,false},
	{ TYPE_TEXT,"DESCRIPTION",128,NULL,false,false,false, "Description", 100 ,false},
	{ TYPE_INTEGER,"WIDTH",0,NULL,false,false,false, "Width", 100 ,false},
	{ TYPE_INTEGER,"HEIGHT",0,NULL,false,false,false, "Height", 100 ,false},
	{ TYPE_INTEGER,"DURATION",0,NULL,false,false,false, "Duration", 100 ,false},
	{ TYPE_INTEGER,"BACKCOLOR",0,NULL,false,false,false, "Color", 100 ,false},
};





IMPLEMENT_STATIC_VARIABLES(EDDIndicator, EDD_INDICATOR, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(EDDIndicator, 8) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false,"Table Index",100,false},
	{TYPE_INTEGER,"CODE",0,NULL,false,false,false,"Code",100,false},
	{TYPE_INTEGER,"IMG_WIDTH",0,NULL,false,false,false,"Width",100,false},
	{TYPE_INTEGER,"IMG_HEIGHT",0,NULL,false,false,false,"Height",100,false},
	{TYPE_INTEGER,"IMG_PAGE",0,NULL,false,false,false,"Page",100,false},
	{TYPE_INTEGER,"IMG_MODE",0,NULL,false,false,false,"Mode",100,false},
	{TYPE_INTEGER,"IMG_BIT",0,NULL,false,false,false,"Bit",100,false},
	{TYPE_DATA,"DATAPOINTER",0,NULL,false,false,false,"DataPointer",100,false},
};


IMPLEMENT_STATIC_VARIABLES(ETNDIndicator, ETND_INDICATOR, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(ETNDIndicator, 8) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false,"Table Index",100,false},
	{TYPE_INTEGER,"CODE",0,NULL,false,false,false,"Code",100,false},
	{TYPE_INTEGER,"IMG_WIDTH",0,NULL,false,false,false,"Width",100,false},
	{TYPE_INTEGER,"IMG_HEIGHT",0,NULL,false,false,false,"Height",100,false},
	{TYPE_INTEGER,"IMG_PAGE",0,NULL,false,false,false,"Page",100,false},
	{TYPE_INTEGER,"IMG_MODE",0,NULL,false,false,false,"Mode",100,false},
	{TYPE_INTEGER,"IMG_BIT",0,NULL,false,false,false,"Bit",100,false},
	{TYPE_DATA,"DATAPOINTER",0,NULL,false,false,false,"DataPointer",100,false},
};


IMPLEMENT_STATIC_VARIABLES(UpdateMarking, UPDATE_MARKING, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(UpdateMarking, 2) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_TEXT,"UPDATE_MARKING",64,NULL,false,false,false, "UpdateMarking", 100,false},
};

IMPLEMENT_STATIC_VARIABLES(PIIText, PIITEXT, -1, -1, 0)

IMPLEMENT_STATIC_TYPESETTINGS(PIIText, 3) =
{
	{TYPE_INTEGER,"TABLE_INDEX",0,NULL,true,false,false, "Table Index", 100,false},
	{TYPE_WTEXT,"DESCRIPTION",64,NULL,false,false,false, "Description", 100,false},
	{TYPE_WTEXT,"PIIText",128,NULL,false,false,false, "PII Text", 100,false},
};





