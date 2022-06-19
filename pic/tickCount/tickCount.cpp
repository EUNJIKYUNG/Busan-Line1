#include "tickCount.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <time.h>
#endif

int64_t getTickCount()
{

#if defined(WIN32) || defined(WIN64)
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now.QuadPart / freq.QuadPart * 1000;
#else
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC, &tp);
		return (tp.tv_sec * 1000ul) + (tp.tv_nsec / 1000ul / 1000ul);
#endif
}