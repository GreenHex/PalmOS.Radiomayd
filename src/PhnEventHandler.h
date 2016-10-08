/*
 * PhnEventHandler.h
 */
 
#ifndef __PHNEVENTHANDLER_H__
#define __PHNEVENTHANDLER_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsPhoneTypes.h>
#include <HsPhoneEvent.h>
#include <HsPhonePower.h>

#include "Global.h"
#include "Utils.h"
#include "NotifyReg.h"
#include "Security.h"
#include "PhnSecDB.h"
#include "Messaging.h"
#include "Alarms.h"
#include "Prefs.h"
#include "PopupForm.h"
#include "PhnInfo.h"
#include "AttnAlerts.h"
#include "../../TreoFlex2/src/SMSUtils.h"

// Prototypes
extern Err 				HandlePhnEvent( PhnEventType* phnEventP );
extern Err 				HandleNotifyEvent( SysNotifyParamType* notifyParamP );
extern Err 				HandleSMSEvent( PhnSMSParams* paramsP, Boolean* handledP );
extern void 			HandleReset( void );
extern void 			SendMayDayNotification( void );
extern void 			HandleAlarm( SysDisplayAlarmParamType* cmdPBP );

#endif /* __PHNEVENTHANDLER_H__ */

/*
 * PhnEventHandler.h
 */