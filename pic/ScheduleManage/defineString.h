#ifndef DEFINE_STRING
#define DEFINE_STRING
#include <stdint.h>


#ifdef _WIN32X
	#define XCHAR wchar_t
	#define STRCAT				wcscat
	#define SPRINTF				swprintf
	#define STRCPY              wcscpy
	#define STRLEN              wcslen
	#define STRCMP              wcscmp
	#define STRNCPY             wcsncpy
#else
	#define XCHAR char
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
#endif

#endif
