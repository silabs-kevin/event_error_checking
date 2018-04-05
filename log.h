#ifndef _LOG_H_
#define _LOG_H_

#include "stdio.h"
#include "retargetserial.h"
#include "SEGGER_RTT.h"
#include "event_log.h"
/* High level defines, needs user to decide */
#define LOG_LEVEL									LVL_VERBOSE
#define LOG_PORT									SEGGER_JLINK_VIEWER

#define PORT_VCOM									1
#define SEGGER_JLINK_VIEWER				2

#define NO_LOG									0
#define LVL_ERROR								1
#define LVL_WARNING							2
#define LVL_INFO								3
#define LVL_DEBUG								4
#define LVL_VERBOSE							5

#if (LOG_PORT == SEGGER_JLINK_VIEWER)
#define INIT_LOG()							SEGGER_RTT_Init()
#define LOG(...)								SEGGER_RTT_printf(0, __VA_ARGS__)
#elif (LOG_PORT == PORT_VCOM)
#define INIT_LOG()							RETARGET_SerialInit()
#define LOG(...)								printf(__VA_ARGS__)
#else
#define INIT_LOG()
#define LOG(...)
#endif

#define UINT8_ARRAY_DUMP(array_base, array_size)				\
	do {                                                                           \
        for (int i = 0; i<(array_size); i++)                                       \
            LOG("%02X ", ((char*)(array_base))[i]);\
	} while(0)

#define LOGN()											\
	do{																		\
		LOG("\r\n");												\
	}while(0)															\

#define TRY_OUT_ALL_COLORS()				\
	do{																\
		for(uint8_t i=1; i<8; i++){			\
			SEGGER_RTT_printf(0, "[2;3%dm""Normal color. Test For Log out...\r\n", i);	\
			SEGGER_RTT_printf(0, "[1;3%dm""Bright color. Test For Log out...\r\n", i);	\
		}																																							\
	}while(0)

#define LOG_ERROR_MSG(_prefix_, ...)				\
	do{															\
			LOG(RTT_CTRL_TEXT_BRIGHT_RED _prefix_, ##__VA_ARGS__);			\
}while(0)

/* Error */
#define LOGE(_prefix_, ...)				\
	do{															\
		if(LOG_LEVEL >=  LVL_ERROR){	\
			LOG(RTT_CTRL_TEXT_BRIGHT_RED"ERROR   -> " _prefix_, ##__VA_ARGS__);			\
		}																																																\
}while(0)

/* Warning */
#define LOGW(_prefix_, ...)				\
	do{															\
		if(LOG_LEVEL >=  LVL_WARNING){	\
			LOG(RTT_CTRL_TEXT_BRIGHT_YELLOW"WARNING -> " _prefix_, ##__VA_ARGS__);			\
		}																																																\
}while(0)

/* Information */
#define LOGI(_prefix_, ...)				\
	do{															\
		if(LOG_LEVEL >=  LVL_INFO){	\
			LOG(RTT_CTRL_TEXT_BRIGHT_CYAN"INFO    -> " _prefix_, ##__VA_ARGS__);			\
		}																																																\
}while(0)

/* DEBUG */
#define LOGD(_prefix_, ...)				\
	do{															\
		if(LOG_LEVEL >=  LVL_DEBUG){	\
			LOG(RTT_CTRL_TEXT_BRIGHT_GREEN"DEBUG   -> " _prefix_, ##__VA_ARGS__);			\
		}																																																\
}while(0)

/* Vobase */
#define LOGV(_prefix_, ...)				\
	do{															\
		if(LOG_LEVEL >=  LVL_VERBOSE){	\
			LOG(RTT_CTRL_RESET"VERBOSE  -> " _prefix_, ##__VA_ARGS__);			\
		}																																																\
}while(0)

uint16_t error_checking(uint16_t error_code, uint8_t directly);

#define ERROR_ADDRESSING()						\
	do{																	\
			LOGE("  |--> File - %s, Line - %d\r\n", __FILE__, __LINE__);			\
	}while(0)

#endif
