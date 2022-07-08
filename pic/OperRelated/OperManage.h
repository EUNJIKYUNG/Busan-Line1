#ifndef COPERMANAGE_H
#define COPERMANAGE_H
#include "../TablesDefine/TableManage.h"
#include "../TablesDefine/DefineTables.h"


#ifdef OPERMANAGE_CALLBACK_FUNC
	#include <functional>
#endif 

//#include "../TablesDefine/DefinePIDDRMTables.h"
#include <stack>
#include <map>

typedef enum _OPERATION_MODE
{
    OM_NORMAL=0,
    OM_SIMULATION,	
    OM_CORRECTION,
    OM_NULL,
}OPERATION_MODE;

typedef enum _EVENT_TYPE
{
    ET_NORMAL,		//not effected by skip station 
    ET_DEPARTURE,	//Departure Event -> Can be effected by skip station function
    ET_ARRIVAL,		//Arrival Event -> Can be effected by skip ststion function
    ET_SPECIAL,		//not effected by skip station
}EVENT_TYPE;


//find station information by station code - common
struct findStationInformationByCode : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findStationInformationByCode(int nCode)
        :m_nCode(nCode)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        StationInformation *p=(StationInformation*)GET_PTFROMSP(pSI);
		return (p->nStationCode==m_nCode);
    }
private:
    int m_nCode;
};


struct findDestinationInformationByCode : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findDestinationInformationByCode(int nCode)
		:m_nCode(nCode)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		DestinationInformation *p = (DestinationInformation*)GET_PTFROMSP(pSI);
		return (p->nDestinationCode == m_nCode);
	}
private:
	int m_nCode;
};


//find train number by train number string - project specific
struct findTrainNumber : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findTrainNumber(const TYC *pTrainNumber)
    {
        STRCPY(m_szTrainNumber,pTrainNumber);
    }
    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        TrainNumber *pTN=(TrainNumber*)GET_PTFROMSP(pSI);
        return !STRCMP(pTN->szTrainNumber,m_szTrainNumber);
    }
private:
    TYC m_szTrainNumber[64];
};

//find train number by Destination Index - project specific
struct findTrainNumberByDestinationIndex : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findTrainNumberByDestinationIndex(int nDestinationIndex)
		:m_nDestinationIndex(nDestinationIndex)
	{

	}
	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		TrainNumber *pTN = (TrainNumber*)GET_PTFROMSP(pSI);
		return (m_nDestinationIndex == pTN->nDestinationIndex);
	}
private:
	int m_nDestinationIndex;
};


//find train number by station number prefix - project specific
struct findTrainNumberByPrefix : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findTrainNumberByPrefix(const TYC *pTrainNumberPrefix,const TYC *pTrainNumber)
    {
        m_nTrainNumber=0;
        STRCPY(m_szTrainNumberPrefix,pTrainNumberPrefix);
        int nLen=STRLEN(pTrainNumber);
        if(nLen>0)
            m_nTrainNumber=(pTrainNumber[nLen-1]%2);
    }
    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        TrainNumber *pTN=(TrainNumber*)GET_PTFROMSP(pSI);
        return ((!STRCMP(pTN->szTrainNumberPrefix,m_szTrainNumberPrefix))&&(m_nTrainNumber==pTN->nUpDnDirection));
    }
private:
    TYC m_szTrainNumberPrefix[64];
    int m_nTrainNumber;
};

//find PID contents by message code
struct findPIDContentsByMsgCode : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findPIDContentsByMsgCode(int nCode)
        :m_nCode(nCode)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        PIDContents *pPIDContents=(PIDContents*)GET_PTFROMSP(pSI);
        return (pPIDContents->nMsgID==m_nCode);
    }
private:
    int m_nCode;
};

//find PID contents by message code and station index
struct findPIDContentsByMsgIDAndStationIndex : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findPIDContentsByMsgIDAndStationIndex(int nStnIndex, int nMsgID)
		:m_nStnIndex(nStnIndex), m_nMsgID(nMsgID)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		PIDContents *pPIDContents = (PIDContents*)GET_PTFROMSP(pSI);
		return (pPIDContents->nMsgID == m_nMsgID) && (pPIDContents->nStationIndex == m_nStnIndex);
	}
private:
	int m_nStnIndex;
	int m_nMsgID;
};

//find PID contents by message code and station index
struct findPIDContentsByMsgIDNStnIndexNDestIndex : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findPIDContentsByMsgIDNStnIndexNDestIndex(int nStnIndex,int nDestIndex, int nMsgID)
		:m_nStnIndex(nStnIndex), m_nDestIndex(nDestIndex), m_nMsgID(nMsgID)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		PIDContents *pPIDContents = (PIDContents*)GET_PTFROMSP(pSI);
		return (pPIDContents->nMsgID == m_nMsgID) && (pPIDContents->nStationIndex == m_nStnIndex) && (pPIDContents->nDestIndex == m_nDestIndex);
	}
private:
	int m_nStnIndex;
	int m_nDestIndex;
	int m_nMsgID;
};



//find Audio contents by message code
struct findAudioContentsByMsgCode : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findAudioContentsByMsgCode(int nCode)
        :m_nCode(nCode)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        AudioContents *pAudioContents=(AudioContents*)GET_PTFROMSP(pSI);
        return (pAudioContents->nMsgID==m_nCode);
    }
private:
    int m_nCode;
};


//find Audio contents by message code
struct findAudioContentsByMsgIDAndStationIndex : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findAudioContentsByMsgIDAndStationIndex(int nStnIndex,int nMsgID)
		:m_nStnIndex(nStnIndex), m_nMsgID(nMsgID)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		AudioContents *pAudioContents = (AudioContents*)GET_PTFROMSP(pSI);
		return (pAudioContents->nMsgID == m_nMsgID) && (pAudioContents->nStationIndex == m_nStnIndex);
	}
private:
	int m_nStnIndex;
	int m_nMsgID;
};


//find Stop pattern routes by station code (departure station and arrival station is selectable by arguments)
struct findStopPtnRoutesByCode : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findStopPtnRoutesByCode(int nCode,bool bDepArr)
        :m_nCode(nCode),m_bDepArr(bDepArr)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        int nDepArrCode=0;
        StopPtnRoutes *pStopPtnRoutes=(StopPtnRoutes*)GET_PTFROMSP(pSI);
        if(m_bDepArr)
            nDepArrCode=pStopPtnRoutes->nDepStnCode;
        else
            nDepArrCode=pStopPtnRoutes->nArrStnCode;
        return (nDepArrCode==m_nCode);
    }
private:
    int m_nCode;
    bool m_bDepArr;
};


//find Stop pattern routes by station code (departure station and arrival station is selectable by arguments)
struct findAltStopPtnRoutesByCode : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findAltStopPtnRoutesByCode(int nCode, bool bDepArr)
		:m_nCode(nCode), m_bDepArr(bDepArr)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		int nDepArrCode = 0;
		StopPtnRoutes *pStopPtnRoutes = (StopPtnRoutes*)GET_PTFROMSP(pSI);
		StopPtnRoutes *pAltStopPtnRoutes=(StopPtnRoutes*)pStopPtnRoutes->pAltRoutes.get();
		if (pAltStopPtnRoutes)
		{
			if (m_bDepArr)
				nDepArrCode = pAltStopPtnRoutes->nDepStnCode;
			else
				nDepArrCode = pAltStopPtnRoutes->nArrStnCode;
			return (nDepArrCode == m_nCode);
		}
		else
			return false;
	}
private:
	int m_nCode;
	bool m_bDepArr;
};

//find stop pattern route by departure and arrival station index
struct findStopPtnRoutesByDepartureAndArrivalStationIndex : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
	findStopPtnRoutesByDepartureAndArrivalStationIndex(int nDepartureIndex,int nArrivalIndex)
		:m_nDepartureIndex(nDepartureIndex),m_nArrivalIndex(nArrivalIndex)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        StopPtnHeader *pStopPtnHeader=(StopPtnHeader*)GET_PTFROMSP(pSI);
        return ((m_nDepartureIndex==pStopPtnHeader->nDepartStnIndex)&&(m_nArrivalIndex==pStopPtnHeader->nArrivalStnIndex));
    }
private:
	int m_nDepartureIndex;
	int m_nArrivalIndex;

};


//find stop pattern route by departure and arrival station index
struct findTrainNumberByDirection : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findTrainNumberByDirection(int nDirection)
		:m_nDirection(nDirection)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		TrainNumber *pTrainNumber = (TrainNumber*)GET_PTFROMSP(pSI);
		return (m_nDirection == pTrainNumber->nUpDnDirection);
	}
private:
	int m_nDirection;

};



//find stop pattern route by departure and arrival station index
struct findStopPtnRoutesByArrivalStationIndex : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findStopPtnRoutesByArrivalStationIndex(int nArrivalIndex)
		:m_nArrivalIndex(nArrivalIndex)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		StopPtnHeader *pStopPtnHeader = (StopPtnHeader*)GET_PTFROMSP(pSI);
		return (m_nArrivalIndex == pStopPtnHeader->nArrivalStnIndex);
	}
private:
	int m_nArrivalIndex;

};



struct EventAppRouteDistanceCompare : public std::binary_function<SHARED_PTRC(CSQLData),SHARED_PTRC(CSQLData),bool>
{
    bool operator()(const SHARED_PTRC(CSQLData) &a,const SHARED_PTRC(CSQLData) &b)
    {
        EventLists *pA=(EventLists*)GET_PTFROMSP(a);
        EventLists *pB=(EventLists*)GET_PTFROMSP(b);
        return pA->nAppRouteDistance<pB->nAppRouteDistance;
    }
};


struct StationInformationOrderCompare : public std::binary_function<SHARED_PTRC(CSQLData), SHARED_PTRC(CSQLData), bool>
{
	bool operator()(const SHARED_PTRC(CSQLData) &a, const SHARED_PTRC(CSQLData) &b)
	{
		StationInformation *pA = (StationInformation*)GET_PTFROMSP(a);
		StationInformation *pB = (StationInformation*)GET_PTFROMSP(b);
		return pA->nOrder < pB->nOrder;
	}
};




struct EventRouteDistanceCompare : public std::binary_function<SHARED_PTRC(CSQLData),SHARED_PTRC(CSQLData),bool>
{
    bool operator()(const SHARED_PTRC(CSQLData) &a,const SHARED_PTRC(CSQLData) &b)
    {
        EventLists *pA=(EventLists*)GET_PTFROMSP(a);
        EventLists *pB=(EventLists*)GET_PTFROMSP(b);
        return pA->nRouteDistance<pB->nRouteDistance;
    }
};




struct findEventLists : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findEventLists(bool bDepArr)
        :m_bRemove(bDepArr)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        EventLists *pEL=(EventLists*)GET_PTFROMSP(pSI);
        return (pEL->bRemoved==m_bRemove);
    }
private:
    bool m_bRemove;
};


struct findValidStopPtnRoutes : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findValidStopPtnRoutes(bool bDepArr)
        :m_bDepArr(bDepArr)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        StopPtnRoutes *pSPR=(StopPtnRoutes*)GET_PTFROMSP(pSI);
        if(m_bDepArr)
          return !pSPR->bDepSkipped;
        else
          return !pSPR->bArrSkipped; 
    }
private:
    bool m_bDepArr;
};


struct findStopPtnRoutesByStationCode : public std::unary_function<SHARED_PTRC(CSQLData),bool>
{
    findStopPtnRoutesByStationCode(int nCode)
        :m_nCode(nCode)
    {

    }

    bool operator ()(SHARED_PTRC(CSQLData) &pSI)
    {
        StopPtnRoutes *pStopPtnRoutes=(StopPtnRoutes*)GET_PTFROMSP(pSI);
        return (m_nCode==pStopPtnRoutes->nDepStnCode)||(m_nCode==pStopPtnRoutes->nArrStnCode);
    }
private:
	int m_nCode;
};


struct findRedundentStopPtnRoutesByStationCode : public std::unary_function<SHARED_PTRC(CSQLData), bool>
{
	findRedundentStopPtnRoutesByStationCode(int nCode1,int nCode2)
		:m_nCode1(nCode1),m_nCode2(nCode2)
	{

	}

	bool operator ()(SHARED_PTRC(CSQLData) &pSI)
	{
		StopPtnRoutes *pStopPtnRoutes = (StopPtnRoutes*)GET_PTFROMSP(pSI);

		return ((m_nCode1 == pStopPtnRoutes->nDepStnCode) || (m_nCode1 == pStopPtnRoutes->nDepStnCodeAlt))&&
			((m_nCode2 == pStopPtnRoutes->nArrStnCode) || (m_nCode2 == pStopPtnRoutes->nArrStnCodeAlt));
	}
private:
	int m_nCode1,m_nCode2;
};






class CValueMonitor;


/* OPERATION_MODE */


class COperManage
{
	friend class CWebServer;
public:
    COperManage();
    virtual ~COperManage(void);
    static COperManage *GetInstance();

    OPERATION_MODE SetOperationMode     (OPERATION_MODE eMode); // Set Current Operation Mode
	OPERATION_MODE GetOperationMode		();						// Get Current Operation Mode

	// Calls AddDistance( ) internally
    void UpdateDistance                 (int nDistance,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Set Current Distance
	bool SetDistance					(int nDistance,bool bReset=true,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Set Target Distance
	bool SetTargetDistance				(int nTargetDistance, bool bReset=true, bool bCurMode=true, OPERATION_MODE eOperMode=OM_NORMAL);

	int GetProceededDistance			(bool bCurMode, OPERATION_MODE eOperMode = OM_NORMAL);

	int GetLimitDistance				(bool bCurMode, OPERATION_MODE eOperMode = OM_NORMAL);
	// Set Route Distance (whole distance of this route)
    bool SetRouteDistance				(int nDistance,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Add Distance
	void AddDistance                    (int nDiff,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Set Distance by ratio
    bool SetDistance                    (float fRatio,bool bReset=true,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Reset Distance Events
    void ResetDistanceEvents            (bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// SetTrain Number by prefix and train number
    bool SetTrainNumber                 (const TYC *pszTrainNumPrefix,const TYC *pszTrainNum,bool bForce=false);
	// SetTrain Number by train number string
    bool SetTrainNumber                 (const TYC *pszTrainNum,bool bForce=false,int *pTN=0);
	// Set Route by departure and arrival station code
	bool SetRouteByDepArrCode			(int nDepartueStationCode,int nArrivalStationCode,bool bForce=false);
	// Set Route by departure code
	bool SetRouteByArrCode				(int nArrivalStationCode, bool bForce);

	bool SetRouteByDestinationCode		(int nDestinationCode);

	int IsRouteExistDestAndNextCode		(int nDestinationCode, int nNextStnCode);
	// Set Event By Distance
    void SetEventByDistance             (bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Set Event By Target Distance
	void SetEventByTargetDistance(bool bCurMode = true, OPERATION_MODE eOperMode = OM_NORMAL);
	// Go To Station ( Departure and Arrival station is seletable by boolean argument)
    bool GoToStation                    (int nStationCode,bool nDepArr=true,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);

	bool SetStationByDistanceIndex		(int nStationIndex, bool bDepArr, bool bCurMode, OPERATION_MODE eOperMode);
	// Go To Previous station
	bool GoToPrevStation                (bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Go To Next Station
    bool GoToNextStation                (bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Skip Next Station
	bool SkipNextStation				(bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Unskip station from the latest skipped station.
	bool UnSkipFromLatest				(bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Initialize all the stop pattern header
    void InitStopPtnHeader              ();
	// Clone Stop Pattern routes
    void CloneStopPtnRoutes             (SHARED_PTRC(CSQLData) &pData, int nDeparture, int nArrive);
	// Skip Station		(should be called internally)
    bool SkipStation                    (int nStationCode,bool bInsert,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Add station to skip station list
	bool AddToSkipStations				(int nStationCode,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// remove station from skip station list.
	bool RemoveFromSkipStations			(int nStationCode,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Go To Station ( put this and next station and it will skip all the imtermediate station(s) automatically if they are exist.)  
	bool SetStations					(int nCurStation,int nNexStation,bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);

	bool SetNextStation(int nNextStationCode, bool bCurMode = true, OPERATION_MODE eOperMode = OM_NORMAL);
	// Apply Skip function from skip station list
	void SetSkippedStations				(int nStationCode, bool bCurMode=true,OPERATION_MODE eOperMode=OM_NORMAL);
	// Set Tolerance
	void SetTolerance					(int nTolerance);
	// Set Video Clear event
    void SetAudioClearEvent();
	// Set Audio Clear event
    void SetVideoClearEvent();
	// Set Stop pattern
    bool SetStopPattern(SHARED_PTRC(CSQLData) &pData);
    // 220706 KEJ Select Stop Pattern by index 
    bool SelectStopPatternByIndex(int nIndex);
	// Set Stop pattern by index
	bool SetStopPatternByIndex(int nIndex);
	// Set Stop pattern by index
	bool SetStopPatternByStopPtnHeader(SHARED_PTRC(CSQLData) &pData);
	//initialize mapping value
	void InitMappedValue();
	//get mapping value
	bool GetMappedValue(const TYC *pTagName, SHARED_PTRC(CValueMonitor) *pVM);
    // Get Departure Station Code
    int GetDepatStationCode();
	//Get Current Station Code;
	int GetThisStationCode();
	//Get Next Staiton Code
	int GetNextStationCode();

	int GetDestStationCode();

	int GetStopPatternIndex();

	int GetLineMapIndex();

	void SetImmediateEvent(bool bEvent);

	bool GetImmediateEvent();

	int SetStationOrder(int nStationCode, OPERATION_MODE eOperMode = OM_NORMAL);

	int GetStationCodeByOrder(int nOrder, OPERATION_MODE eOperMode= OM_NORMAL);

	void setDoorOpen();
	
	void setDoorClose();

	void GetDistanceList(bool bCurMode, OPERATION_MODE eOperMode, std::vector<unsigned int> &vDistList);

	void GetTargetDistanceList(bool bCurMode, OPERATION_MODE eOperMode, std::vector<unsigned int> &vDistList);


	void GetStopPtnHeaderTypeList(int nType);

	bool GoToStationBySelectedEvent(int nIndex,bool bCurMode,OPERATION_MODE eOperMode);
public:
	void SaveStopPatternAsCSV();
private:

    std::vector<SHARED_PTRC(CSQLData)>	m_vEvents[OM_NULL];					//events list
	std::vector<SHARED_PTRC(CSQLData)>  m_vSkippedEvents[OM_NULL];			//skipped events list
    std::vector<SHARED_PTRC(CSQLData)>	m_vStopRoutes[OM_NULL];				//Stop Pattern routes

	std::vector<SHARED_PTRC(CSQLData)>	m_vAltEvents[OM_NULL];					//events list
	std::vector<SHARED_PTRC(CSQLData)>  m_vAltSkippedEvents[OM_NULL];			//skipped events list
	//std::vector<SHARED_PTRC(CSQLData)>  m_vStopAltRoutes[OM_NULL];			//Stop Alt routes	
    std::vector<SHARED_PTRC(CSQLData)>	m_vTempDistEvent[OM_NULL];			//Temporary Event
	std::vector<SHARED_PTRC(CSQLData)>  m_vTempTargetDistEvent[OM_NULL];	//Temporary Target Distance Event;
	std::vector<SHARED_PTRC(CSQLData)> m_vValidStopRoutes[OM_NULL];
	int m_nStationOrder[OM_NULL];
public:
	std::vector<SHARED_PTRC(CSQLData)>  m_vStationInformation[OM_NULL];	//Station Information;
	std::vector<SHARED_PTRC(CSQLData)>  m_vStationInformationSkipped[OM_NULL];	//Station Information;

public:
	std::vector<SHARED_PTRC(CSQLData)> m_vStopPatternHeader[OM_NULL];	//Stop Ptn Header Types;
private:
	std::vector<unsigned short>			m_vSkippedStation[OM_NULL];			//skip station list


    unsigned short m_uCurStn[OM_NULL];		// current station mode
    unsigned short m_uNexStn[OM_NULL];		// next station mode

	std::map<TSTRING, int *> m_mMappedValue;	//map value 

    int m_nDepStnCode;                          //departure station code
    int m_nDstStnCode;							//destination station code
    int m_nCurStnCode;							//current station code
    int m_nNexStnCode;							//next station code
    int m_nDstIndIdx;							//destination indicator index;
	int m_nStopPtnIdx;
	int m_nLineMapIdx;

    int m_nAudioEventIdx;						//audio event index
    int m_nDRMEventIdx;							//drm event index
    int m_nPIDEventIdx;							//PID event index
	
	int m_nDoorDirection;						//door direction -> project specific
	bool m_bImmediateEvent;
public:
    OPERATION_MODE m_eOperMode;					//operational mode
	int m_nTargetStationOrder[OM_NULL];			//TargetStationOrder;
    int m_nProceededDistance[OM_NULL];			//proceeded distance from departure station
	int m_nProceededTargetDistance[OM_NULL];	//proceeded target distance from departure station
	int m_nTotalProceededDistance[OM_NULL];		//total proceeded distance of this route
    int m_nBaseDistance[OM_NULL];				//base distance (whole route distance from departure station)
    int m_nReceivedDistance[OM_NULL];			//received distance from other device
    int m_nLimitDistance[OM_NULL];				//distance between station
    unsigned short *m_pCurStn[OM_NULL];					//station string pointer
	unsigned short *m_pNexStn[OM_NULL];					//station string pointer

	const char *GetCurrentStationNameUTF8(OPERATION_MODE eOperMode = OM_NORMAL);
	const char *GetNextStationNameUTF8(OPERATION_MODE eOperMode = OM_NORMAL);
	const char *GetDestinationNameUTF8(OPERATION_MODE eOperMode = OM_NORMAL);
    const char *GetTrainNumber(int num = 0);

private:
	char m_szCurStnUTF8[OM_NULL][128];		//current station string UTF-8 encoding;
	char m_szNexStnUTF8[OM_NULL][128];		//next station string UTF-8 encoding;
    char m_szDepStnUTF8[128];		//departure station string UTF-8 encoding;
	char m_szDstStnUTF8[128];		//destination station string UTF-8 encoding;

	char m_szDestinationUTF8[128];			//Destination UTF-8 encoding;
public:
    int *m_pDistanceList[OM_NULL];				//distance list for gui
	int *m_pTargetDistanceList[OM_NULL];				//distance list for gui
    unsigned short *m_pDestination;						//destination string pointer

    TYC m_szTrainNumber[16];					//current train number
private:
    int m_nPrevRouteIndex[OM_NULL];
    int m_nCurRouteIndex[OM_NULL];
    int m_nDestIndicatorIndex[OM_NULL];
    float	m_fProgRatio[OM_NULL];
    int m_nTolerance;

    TYC *m_pTrainNumber;
    TYC m_szTrainNumberPrefix[16];

	int m_nDoorOpenAudio;
	int m_nDoorOpenVideo;
	int m_nDoorOpenPIDDRM;

	int m_nDoorCloseAudio;
	int m_nDoorCloseVideo;
	int m_nDoorClosePIDDRM;
	bool m_bArrivedAtNextStation;

#ifdef	OPERMANAGE_CALLBACK_FUNC
public:

	bool dummyCallback(int nIndex);
	void setPIDCallback(std::function<bool(int)> &tFunc);
	void setDRMCallback(std::function<bool(int)> &tFunc);
	void setVoiceCallback(std::function<bool(int)> &tFunc);
	void setDoorDirectionCallBack(std::function<bool(int)> &tFunc);
	void setDoorOpenAudioCallback(std::function<bool(int)> &tFunc);
	void setDoorOpenVideoCallback(std::function<bool(int)> &tFunc);
	void setDoorOpenPIDDRMCallback(std::function<bool(int)> &tFunc);
	void setDoorCloseAudioCallback(std::function<bool(int)> &tFunc);
	void setDoorCloseVideoCallback(std::function<bool(int)> &tFunc);
	void setDoorClosePIDDRMCallback(std::function<bool(int)> &tFunc);
	void setDestIndCallback(std::function<bool(int)> &tFunc);

	void setEventListIndexCallback(std::function<void(int)> &tFunc);
	void setRouteListIndexCallback(std::function<void(int)> &tFunc);
public:
	std::function<bool(int)> m_tFuncPID;
	std::function<bool(int)> m_tFuncDRM;
	std::function<bool(int)> m_tFuncVoice;
	std::function<bool(int)> m_tFuncDoorDir;
	std::function<bool(int)> m_tFuncOpenAudio;
	std::function<bool(int)> m_tFuncOpenVideo;
	std::function<bool(int)> m_tFuncOpenPIDDRM;
	std::function<bool(int)> m_tFuncCloseAudio;
	std::function<bool(int)> m_tFuncCloseVideo;
	std::function<bool(int)> m_tFuncClosePIDDRM;
	std::function<void(int)> m_tDestInd;
	std::function<void(int)> m_tPIIText;
	std::function<void(int)> m_tEventIndex;
	std::function<void(int)> m_tRouteIndex;
#endif
};

#endif // COPERMANAGE_H
