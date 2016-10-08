/*
 * Messaging.h
 */

#ifndef __MESSAGING_H__
#define __MESSAGING_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsPhoneTypes.h>
#include <HsPhoneSMSTypes.h>
#include <HsPhoneSMS.h>

#include "Global.h"
#include "AppResources.h"
#include "PhnSecDB.h"
#include "PhnInfo.h"
#include "Alarms.h"
#include "DispProgress.h"
#include "Security.h"
#include "../../TreoFlex2/src/SMSUtils.h"

// Prototypes
extern Err 				DispatchMessages( void );
extern Err 				SendMsgToAddressAtIdx( UInt16 idx, msg_t* msgP, Boolean bDispProgress );

#endif /* __MESSAGING_H__ */
 
/*
 * Messaging.h
 */