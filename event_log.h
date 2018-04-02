#ifndef _EVENT_LOG_H_
#define _EVENT_LOG_H_

#include "log.h"
#include "native_gecko.h"
#define EVENT_LOG_LEVEL				VERBOSE

#define NO_EVENT_LOG					0

#define CRITICAL							1
#define IMPORTANT							2
#define VERBOSE								3

#define CRITICAL_COLOR				RTT_CTRL_TEXT_BRIGHT_RED
#define IMPORTANT_COLOR				RTT_CTRL_TEXT_BRIGHT_YELLOW
#define VERBOSE_COLOR					RTT_CTRL_RESET


#define FUNCTION							""
#define EVT_CATEGORY					""

#define EVT_CRITICAL_PREFIX		CRITICAL_COLOR FUNCTION EVT_CATEGORY
#define EVT_IMPORTANT_PREFIX	IMPORTANT_COLOR FUNCTION EVT_CATEGORY
#define EVT_VERBOSE_PREFIX		VERBOSE_COLOR FUNCTION EVT_CATEGORY

#define COEX									1
#define DFU										1
#define ENDPOINT							1
#define PSTORE								1
#define	GATT									1
#define GATT_SERVER						1
#define HARDWARE							1
#define LE_CONNECTION					1
#define LE_GAP								1
#define	LE_SM									1
#define SYSTEM								1
#define TEST									1
#define USER									1


#define EVT_LOG_N()											\
	do{																		\
		LOG("\r\n");												\
	}while(0)															\

#define EVT_LOG_C(_evt_name_, _attached_, ...)				\
	do{																									\
		if(EVENT_LOG_LEVEL >= CRITICAL){									\
			LOG(EVT_CRITICAL_PREFIX "<" _evt_name_ "> - " _attached_, ##__VA_ARGS__);	\
		}																									\
	}while(0)

#define EVT_LOG_I(_evt_name_, _attached_, ...)				\
	do{																									\
		if(EVENT_LOG_LEVEL >= IMPORTANT){									\
			LOG(EVT_IMPORTANT_PREFIX "<" _evt_name_ "> - " _attached_, ##__VA_ARGS__);	\
		}																									\
	}while(0)

#define EVT_LOG_V(_evt_name_, _attached_, ...)				\
	do{																									\
		if(EVENT_LOG_LEVEL >= VERBOSE){									\
			LOG(EVT_VERBOSE_PREFIX "<" _evt_name_ "> - " _attached_, ##__VA_ARGS__);	\
		}																									\
	}while(0)


void log_events(struct gecko_cmd_packet* evt);
#endif
