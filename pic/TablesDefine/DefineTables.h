#ifndef DEFINE_TABLES
#define DEFINE_TABLES
#include "../DataRelated/SQLData.h"
#include "../DataRelated/SQLDataElement.h"


class CDrawImageIndex;


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(TrainHeaders, uTrainNumber = uTrainType = uOrder \
	= uDaysOp = uDestStationCode = uDestCodePP = uOriginStationCode = uOriginCodePP \
	= uBeforeWholeTrain = uAfterWholeTrain = uActiveBroadcast[0] = uActiveBroadcast[1] = 0;, , 1);
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(13)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex)
		MM(1, &uOrder)
		MM(2, &uTrainNumber)
		MM(3, &uTrainType)
		MM(4, &uDaysOp)
		MM(5, &uDestStationCode)
		MM(6, &uDestCodePP)
		MM(7, &uOriginStationCode)
		MM(8, &uOriginCodePP)
		MM(9, &uBeforeWholeTrain)
		MM(10, &uAfterWholeTrain)
		MM(11, &uActiveBroadcast[0])
		MM(12, &uActiveBroadcast[1])
	END_MAPPING_MEMBERS
	unsigned int uTrainNumber;
	unsigned int uOrder;
	unsigned int uTrainType;
	unsigned int uDaysOp;
	unsigned int uDestStationCode;
	unsigned int uDestCodePP;
	unsigned int uOriginStationCode;
	unsigned int uOriginCodePP;
	unsigned int uBeforeWholeTrain;
	unsigned int uAfterWholeTrain;
	unsigned int uActiveBroadcast[2];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(TrainHeaders)


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(StationInformation,
	memset(szStationName, 0, sizeof(szStationName));
	dLatitude = dLongitude = 0.0; nOrder = 0;
	nTotalWidth = 0;
	nTotalHeight = 0;
	nStationCode = 0;
	nStationType = 0; nProp[0] = 0; nProp[1] = 0; , , 1);
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(17)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nStationCode);
		MM(2,szStationName[0]);
		MM(3,szStationName[1]);
		MM(4,szStationName[2]);
		MM(5,szStationName[3]);
		MM(6,szStationName[4]);
		MM(7,szStationName[5]);
		MM(8,szStationName[6]);
		MM(9,szStationName[7]);
		MM(10, &dLatitude);
		MM(11, &dLongitude);
		MM(12, &nStationType);
		MM(13, &nTotalWidth);
		MM(14, &nTotalHeight);
		MM(15, &nProp[0]);
		MM(16, &nProp[1]);
	END_MAPPING_MEMBERS
	unsigned short szStationName[8][96];
	int nStationCode;
	int nStationType;
	double dLatitude;
	double dLongitude;
	int nTotalWidth;
	int nTotalHeight;
	int nProp[2];
	int nOrder;
	int nPosX;
	int nPosY;
	int nState;
	std::shared_ptr<CDrawImageIndex> pStationIndex;
	std::shared_ptr<CDrawImageIndex> pSpotIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(StationInformation)




BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(DestinationInformation,
	memset(szDestinationName, 0, sizeof(szDestinationName));dLatitude = dLongitude = 0.0;
	nTotalWidth = 0;
	nTotalHeight = 0;
	nDestinationCode = 0;
	nDestinationType = 0; nProp[0] = 0; nProp[1] = 0;, , 1);
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(17)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nDestinationCode);
		MM(2,szDestinationName[0]);
		MM(3, szDestinationName[1]);
		MM(4, szDestinationName[2]);
		MM(5,szDestinationName[3]);
		MM(6, szDestinationName[4]);
		MM(7, szDestinationName[5]);
		MM(8,szDestinationName[6]);
		MM(9, szDestinationName[7]);
		MM(10, &dLatitude);
		MM(11, &dLongitude);
		MM(12, &nDestinationType);
		MM(13, &nTotalWidth);
		MM(14, &nTotalHeight);
		MM(15, &nProp[0]);
		MM(16, &nProp[1]);
	END_MAPPING_MEMBERS
	unsigned short szDestinationName[8][96];
	int nDestinationCode;
	int nDestinationType;
	double dLatitude;
	double dLongitude;
	int nTotalWidth;
	int nTotalHeight;
	int nProp[2];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(DestinationInformation)


BEGIN_CLASS_FROM_SQLDATA(StationDistance, nDepartStn = nArrivalStn = nDistance = 0; memset(szDesc, 0, sizeof(szDesc));, )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(5)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nDepartStn);
		MM(2, &nArrivalStn);
		MM(3, &nDistance);
		MM(4, szDesc);
	END_MAPPING_MEMBERS
	int nDepartStn;
	int nArrivalStn;
	int nDistance;
	unsigned short szDesc[96];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(StationDistance)



BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(StopPtnHeader, nDepartStnIndex = nArrivalStnIndex = 0; memset(szDesc, 0, sizeof(szDesc)); , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(5)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nDepartStnIndex);
		MM(2, &nArrivalStnIndex);
		MM(3, szDesc);
		MM(4, &nRelatedLineMapIndex);
	END_MAPPING_MEMBERS
	int nDepartStnIndex;
	int nArrivalStnIndex;
	unsigned short szDesc[96];
	int nRelatedLineMapIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(StopPtnHeader);




BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(StopPtnRoutes, nDistance = nRouteDistance = nDestIndex2 = nParentIndex = nOrder = nDistIndex = nDestIndex = nDoorDirection = nDCVoiceIndex = nDCVideoIndex = nDCDRMIndex = nDOVoiceIndex = nDOVideoIndex = nDODRMIndex = 0; bDepSkipped = bArrSkipped = false; , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(13)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParentIndex);
		MM(2, &nOrder);
		MM(3, &nDistIndex);//°Å¸®ÀÎµ¦½º
		MM(4, &nDestIndex);//Çà¼±ÀÎµ¦½º
		MM(5, &nDoorDirection);
		MM(6, &nDestIndex2);
		MM(7, &nDCVoiceIndex);
		MM(8, &nDCVideoIndex);
		MM(9, &nDCDRMIndex);
		MM(10, &nDOVoiceIndex);
		MM(11, &nDOVideoIndex);
		MM(12, &nDODRMIndex);
	END_MAPPING_MEMBERS
	int nParentIndex;
	int nOrder;
	int nDistIndex;
	int nDestIndex;
	int nDestIndex2;
	int nDoorDirection;
	int nDistance;
	int nRouteDistance;
	int nBaseDistance;
	int nArrStnIndex;
	int nDepStnIndex;
	int nDepStnCode;
	int nDepStnCodeAlt;
	int nArrStnCode;
	int nArrStnCodeAlt;
	int nDCVoiceIndex;
	int nDCVideoIndex;
	int nDCDRMIndex;
	int nDOVoiceIndex;
	int nDOVideoIndex;
	int nDODRMIndex;
	bool bDepSkipped;       //Is departure station skipped?
	bool bArrSkipped;       //Is arrival station skipped?
	int nRealOrder;

	virtual void SetAdditionalMembers(CSQLData *pData)
	{
		StopPtnRoutes *pSPR = (StopPtnRoutes*)pData;
		pSPR->nDistance = nDistance;
		pSPR->nRouteDistance = nRouteDistance;
		pSPR->nBaseDistance = nBaseDistance;
		pSPR->nArrStnIndex = nArrStnIndex;
		pSPR->nDepStnIndex = nDepStnIndex;
		pSPR->nDepStnCode = nDepStnCode;
		pSPR->nArrStnCode = nArrStnCode;
		pSPR->nDepStnCodeAlt = nDepStnCodeAlt;
		pSPR->nArrStnCodeAlt = nArrStnCodeAlt;
		pSPR->nRealOrder = nRealOrder;
	}

	std::shared_ptr<CSQLData> pAltRoutes;


END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(StopPtnRoutes);









BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(EventLists, nPIITextIndex=nDistanceType = nAppRouteDistance = nParentIndex = nDistance = nVoiceIndex = nVideoIndex = nDRMIndex = nEventType = nRouteDistance = 0; bRemoved = false; memset(szDesc, 0, sizeof(szDesc)); , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(11)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParentIndex);
		MM(2, &nDistance);
		MM(3, &nVoiceIndex);
		MM(4, &nVideoIndex);
		MM(5, &nOrder);
		MM(6, szDesc);
		MM(7, &nDRMIndex);
		MM(8, &nEventType);
		MM(9, &nDistanceType);
		MM(10, &nPIITextIndex);
	END_MAPPING_MEMBERS
	int nParentIndex;
	int nDistance;
	int nVoiceIndex;
	int nVideoIndex;
	int nDRMIndex;
	int nOrder;
	int nEventType;
	int nDistanceType;
	int nPIITextIndex;
	unsigned short szDesc[128];
	int nRouteDistance;
	bool bRemoved;
	int nAppRouteDistance;

	virtual void SetAdditionalMembers(CSQLData *pData)
	{
		EventLists *pSPR = (EventLists*)pData;
		pSPR->nDistance = nDistance;
		pSPR->nRouteDistance = nRouteDistance;
		pSPR->nAppRouteDistance = nAppRouteDistance;
	}

END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(EventLists);



BEGIN_CLASS_FROM_SQLDATA(AudioFilePool, uDuration = uSize = nCode = nSpareCode = 0;  memset(szExt, 0, sizeof(szExt)); memset(szConvFileName, 0, sizeof(szConvFileName)); memset(szMD5Hash, 0, sizeof(szMD5Hash)); memset(szFileName, 0, sizeof(szFileName));, )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(8)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nCode);
		MM(2, &nSpareCode);
		MM(3, szFileName);
		MM(4, &uDuration);
		MM(5, &uSize);
		MM(6, szMD5Hash);
		MM(7, szExt);
	END_MAPPING_MEMBERS
	int nCode;
	int nSpareCode;
	char szFileName[128];
	unsigned int	uDuration;
	unsigned int	uSize;
	char szExt[16];
	char szMD5Hash[64];
#ifdef _WIN32
	wchar_t szConvFileName[256];
#else
	char szConvFileName[256];
#endif
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(AudioFilePool);



BEGIN_CLASS_FROM_SQLDATA(VideoFilePool, uDuration = uSize = nCode = nSpareCode = 0;  memset(szExt, 0, sizeof(szExt)); memset(szMD5Hash, 0, sizeof(szMD5Hash)); memset(szFileName, 0, sizeof(szFileName)); , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(8)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nCode);
		MM(2, &nSpareCode);
		MM(3, szFileName);
		MM(4, &uDuration);
		MM(5, &uSize);
		MM(6, szMD5Hash);
		MM(7, szExt);
	END_MAPPING_MEMBERS
	int nCode;
	int nSpareCode;
	unsigned short szFileName[256];
	unsigned int	uDuration;
	unsigned int	uSize;
	char szExt[16];
	char szMD5Hash[64];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(VideoFilePool);


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(VideoContents, nOrder = nMsgID = nType = nStationIndex = nDestIndex = 0; memset(szDesc, 0, sizeof(szDesc)); , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(7)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, &nMsgID);
		MM(3, &nType);
		MM(4, &nStationIndex);
		MM(5, &nDestIndex);
		MM(6, &nOrder);
	END_MAPPING_MEMBERS
	int nMsgID;
	int nType;
	int nStationIndex;
	int nDestIndex;
	int nOrder;
	char  szDesc[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(VideoContents);

BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(AudioContents, nOrder = nMessageTypeID = nInternalIndex = nMsgID = nType = nStationIndex = nDestIndex = 0; memset(szDesc, 0, sizeof(szDesc)); , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(9)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, &nMsgID);
		MM(3, &nType);
		MM(4, &nStationIndex);
		MM(5, &nDestIndex);
		MM(6, &nMessageTypeID);
		MM(7, &nInternalIndex);
		MM(8, &nOrder);
	END_MAPPING_MEMBERS
	int nMsgID;
	int nType;
	int nStationIndex;
	int nDestIndex;
	int nInternalIndex;
	int nMessageTypeID;
	int nOrder;
	char szDesc[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(AudioContents);



BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(PIDContents, nOrder = nMessageTypeID = nInternalIndex = nMsgID = nType = nStationIndex = nDestIndex = 0; memset(szDesc, 0, sizeof(szDesc));, , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(10)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, &nMsgID);
		MM(3, &nType);
		MM(4, &nStationIndex);
		MM(5, &nDestIndex);
		MM(6, &nMessageTypeID);
		MM(7, &nInternalIndex);
		MM(8, &nOrder);
		MM(9, &nDevType);
	END_MAPPING_MEMBERS
	int nMsgID;
	int nType;
	int nStationIndex;
	int nDestIndex;
	int nInternalIndex;
	int nMessageTypeID;
	int nOrder;
	int nDevType;
	char szDesc[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(PIDContents);






BEGIN_CLASS_FROM_SQLDATA(VideoIndexList, nParents = nOrder = nContentsPoolIndex = 0; , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParents);
		MM(2, &nOrder);
		MM(3, &nContentsPoolIndex);
	END_MAPPING_MEMBERS

	int nParents;
	int nOrder;
	int nContentsPoolIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(VideoIndexList);

BEGIN_CLASS_FROM_SQLDATA(AudioIndexList, nParents = nOrder = nContentsPoolIndex = 0; , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParents);
		MM(2, &nOrder);
		MM(3, &nContentsPoolIndex);
	END_MAPPING_MEMBERS
	int nParents;
	int nOrder;
	int nContentsPoolIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(AudioIndexList);



BEGIN_CLASS_FROM_SQLDATA(PIDIndexList, nParents = nOrder = nContentsPoolIndex = 0;, )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParents);
		MM(2, &nOrder);
		MM(3, &nContentsPoolIndex);
	END_MAPPING_MEMBERS
	int nParents;
	int nOrder;
	int nContentsPoolIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(PIDIndexList);

BEGIN_CLASS_FROM_SQLDATA(TargetDevices, nTypeCode = 0; memset(szAddress, 0, sizeof(szAddress));  memset(szDesc, 0, sizeof(szDesc));,)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(3)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szAddress);
		MM(2, szDesc);
	END_MAPPING_MEMBERS
	int nTypeCode;
	char szAddress[128];
	char szDesc[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(TargetDevices);


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(UpdateFileTypeList, nExcludeFromZip = 1; nTypeCode = 0; memset(szRealPath, 0, sizeof(szRealPath)); memset(szID, 0, sizeof(szID)); memset(szPW, 0, sizeof(szPW)); memset(szDesc, 0, sizeof(szDesc));, , 2)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(7)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nTypeCode);
		MM(2, szRealPath);
		MM(3, szDesc);
		MM(4, szID);
		MM(5, szPW);
		MM(6, &nExcludeFromZip);
	END_MAPPING_MEMBERS
	int nTypeCode;
	int nExcludeFromZip;
	char szRealPath[128];
	char szDesc[128];
	char szID[64];
	char szPW[64];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(UpdateFileTypeList);



BEGIN_CLASS_FROM_SQLDATA(UpdateFile, nParent = nTypeCode = nFileSize = nFileCRC = 0; memset(szFileName, 0, sizeof(szFileName)); , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(6)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParent);
		MM(2, &nTypeCode);
		MM(3, &nFileSize);
		MM(4, &nFileCRC);
		MM(5,szFileName);
	END_MAPPING_MEMBERS
	int nParent;
	int nTypeCode;
	int nFileSize;
	int nFileCRC;
	char szFileName[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(UpdateFile);


BEGIN_CLASS_FROM_SQLDATA(UpdateTarget, nTarget=nOrder=nParent = 0;  ,)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS	
		MM(0, &m_nTableIndex);
		MM(1, &nParent);
		MM(2, &nOrder);
		MM(3, &nTarget);
	END_MAPPING_MEMBERS
	int nOrder;
	int nParent;
	int nTarget;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(UpdateTarget);




BEGIN_CLASS_FROM_SQLDATA(FontPool, nCode = nSpareCode = 0; memset(szFileName, 0, sizeof(szFileName)); memset(szFontFace, 0, sizeof(szFontFace)); , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(5)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nCode);
		MM(2, &nSpareCode);
		MM(3, szFileName);
		MM(4, szFontFace);
	END_MAPPING_MEMBERS
	int nCode;
	int nSpareCode;
	char szFileName[128];
	char szFontFace[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(FontPool);



BEGIN_CLASS_FROM_SQLDATA(TrainNumber, nUpDnDirection = nTrainNumberIndex = nDestinationIndex = nStopPatternIndex = 0; memset(szTrainNumber, 0, sizeof(szTrainNumber)); memset(szTrainNumberPrefix, 0, sizeof(szTrainNumberPrefix));, )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(7)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nTrainNumberIndex);
		MM(2, &nDestinationIndex);
		MM(3, &nStopPatternIndex);
		MM(4, szTrainNumber);
		MM(5, szTrainNumberPrefix);
		MM(6, &nUpDnDirection);
	END_MAPPING_MEMBERS
	int nTrainNumberIndex;
	int nDestinationIndex;
	int nStopPatternIndex;
	int nUpDnDirection;
	char szTrainNumber[128];
	char szTrainNumberPrefix[64];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(TrainNumber);





BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(EDDIndex, nCode = 0; memset(szDesc, 0, sizeof(szDesc)); , , 1)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(3)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, &nCode);
	END_MAPPING_MEMBERS
	unsigned short szDesc[128];
	int nCode;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(EDDIndex);



BEGIN_CLASS_FROM_SQLDATA(EDDIndexList, nParent = nOrder = nEDDIndex = 0;, )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nParent);
		MM(2, &nOrder);
		MM(3, &nEDDIndex);
	END_MAPPING_MEMBERS
	int nParent;
	int nOrder;
	int nEDDIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(EDDIndexList);



BEGIN_CLASS_FROM_SQLDATA(EditorTagTable, memset(szDesc, 0, sizeof(szDesc)); nSpare = nVariable = 0; pRealValue = NULL; , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(4)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex)
		MM(1, szDesc)
		MM(2, &nVariable)
		MM(3, &nSpare)
	END_MAPPING_MEMBERS
	char szDesc[128];
	int nVariable;
	int nSpare;
	int *pRealValue;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(EditorTagTable);


BEGIN_CLASS_FROM_SQLDATA_WITH_CHILDS(DisplayItemPool, memset(szDesc, 0, sizeof(szDesc)); nWidth = nHeight = nDuration = uBackColor = 0;, , 2)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(6)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, &nWidth);
		MM(3, &nHeight);
		MM(4, &nDuration);
		MM(5, &uBackColor);
	END_MAPPING_MEMBERS
	char szDesc[128];
	int nWidth;
	int nHeight;
	int nDuration;
	unsigned int uBackColor;
	std::shared_ptr<CDrawImageIndex> pImageIndex;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(DisplayItemPool);




BEGIN_CLASS_FROM_SQLDATA(ETNDIndicator, nCode = nMode = 0; nWidth = 0; nHeight = 0; nPage = 1; nBit = 4;  pPixelData = nullptr;,)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(8)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nCode);
		MM(2, &nWidth);
		MM(3, &nHeight);
		MM(4, &nPage);
		MM(5, &nMode);
		MM(6, &nBit);
		MM(7, &pPixelData);
	END_MAPPING_MEMBERS
	int nCode;
	int nWidth;
	int nHeight;
	int nBit;
	int nPage;
	int nMode;
	unsigned char *pPixelData;
	std::shared_ptr<unsigned char> pConvData;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(ETNDIndicator);




BEGIN_CLASS_FROM_SQLDATA(EDDIndicator, nCode = nMode = 0; nWidth = 0; nHeight = 0; nPage = 1; nBit = 4;  pPixelData = nullptr;,)
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(8)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, &nCode);
		MM(2, &nWidth);	
		MM(3, &nHeight);
		MM(4, &nPage);
		MM(5, &nMode);
		MM(6, &nBit);
		MM(7, &pPixelData);
	END_MAPPING_MEMBERS
	int nCode;
	int nWidth;
	int nHeight;
	int nBit;
	int nPage;
	int nMode;
	unsigned char *pPixelData;
	std::shared_ptr<unsigned char> pConvData;
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(EDDIndicator);


BEGIN_CLASS_FROM_SQLDATA(PIIText, memset(szDesc, 0, sizeof(szDesc));memset(szPIIText, 0, sizeof(szPIIText)); , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(3)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS
		MM(0, &m_nTableIndex);
		MM(1, szDesc);
		MM(2, szPIIText);
		END_MAPPING_MEMBERS
		unsigned short szDesc[64];
		unsigned short szPIIText[128];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(PIIText);




BEGIN_CLASS_FROM_SQLDATA(UpdateMarking, memset(szUpdateMarking, 0, sizeof(szUpdateMarking)); , )
	COMMON_STATIC_VARIABLES_FOR_SQLDATA
	DECLARE_TYPESETTINGS(2)
	DECLARE_COMMON_FUNCTIONS OVERRIDE_DUMMY_EDITOR_FUNC
	BEGIN_MAPPING_MEMBERS	
		MM(0, &m_nTableIndex);
		MM(1, szUpdateMarking);
	END_MAPPING_MEMBERS
	char szUpdateMarking[64];
END_CLASS_FROM_SQLDATA

DECLARE_EDITOR_CLASS(UpdateMarking);




#endif