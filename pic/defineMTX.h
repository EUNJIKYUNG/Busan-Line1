#ifndef DEFINE_MTX
#define DEFINE_MTX
#include <stdint.h>
#include "defineCommon.h"



typedef struct _CNCS_2_MTX
{
	uint8_t uLeft[2];
	uint8_t uTop[2];
	uint8_t uRight[2];
	uint8_t uBottom[2];
	uint8_t	uVideoFileName[256];
	uint8_t	uVideoPlayPosition[4];
	uint8_t	bVideoPlay : 1;
	uint8_t	bVideoStop : 1;
	uint8_t : 6;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t	uAudioFileName[256];
	uint8_t	uAudioPlayPosition[4];
	uint8_t	bAudioPlay : 1;
	uint8_t	bAudioStop : 1;
	uint8_t : 6;
	uint8_t bStopStreaming : 1;
	uint8_t : 7;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t uGPSLatitude[4];
	uint8_t uGPSLongitude[4];
	TIMEINFO tInfo;
}CNCS_2_MTX;


typedef struct _MTX_2_CNCS
{
	uint8_t	uVideoFileName[256];
	uint8_t	uVideoPlayPosition[4];
	uint8_t	bVideoPlaying : 1;
	uint8_t	bVideoStandBy : 1;
	uint8_t bVideoFinished : 1;
	uint8_t bVideoError : 1;
	uint8_t : 4;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t	uAudioFileName[256];
	uint8_t	uAudioPlayPosition[4];
	uint8_t	bAudioPlaying : 1;
	uint8_t	bAudioStopped : 1;
	uint8_t bAudioError : 1;
	uint8_t : 5;
	uint8_t : 8;
	uint8_t : 8;
	uint8_t : 8;
    VERSION_STRUCT tVersion;
}MTX_2_CNCS;



#endif
