#ifndef DEFINE_COMMON
#define DEFINE_COMMON
#include <stdint.h>
#define DBG_LINE printf("%s %s,%d\n",__FILE__,__FUNCTION__,__LINE__);

typedef struct _TIMEINFO
{
	uint8_t uYY;
	uint8_t uMM;
	uint8_t uDD;
	uint8_t uHH;
	uint8_t umm;
	uint8_t uSS;
}TIMEINFO;

typedef struct _VERSIONINFO
{
	uint8_t uYear;
	uint8_t uMonth;
	uint8_t uDay;
	uint8_t uMajor;
	uint8_t uMinor;
	uint8_t uRevision;
}VERSIONINFO;


typedef struct _VERSION_STRICT
{
	char chVersion[10];
	char chSWBuildDate[8];
}VERSION_STRUCT;






#endif
