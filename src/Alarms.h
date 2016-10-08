/*
 * Alarms.h
 */
 
#ifndef __ALARMS_H__
#define __ALARMS_H__

#include <Hs.h>
#include <AlarmMgr.h>

#include "Global.h"
#include "Messaging.h"

typedef enum {
	alarm_invalid		= -1,				
	send_message 		= 0,
	radio_on			= 1
} alarmAction_e;

// Prototypes
extern Err 				SetAlarm( UInt32 secondsNext, alarmAction_e alarmAction );
extern Err 				GetNextAlarm( UInt32* nextAlmSecP, UInt32* refP );

#endif /* __ALARMS_H__ */
 
/*
 * Alarms.h
 */