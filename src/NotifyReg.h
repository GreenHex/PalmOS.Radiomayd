/*
 * NotifyReg.h
 */

#ifndef __NOTIFYREG_H__
#define __NOTIFYREG_H__

#include <Hs.h>
#include <HsPhone.h>

#include "Global.h"

#define mdayDeferredNotifyEvent			appFileCreator
#define raonDeferredNotifyEvent			'RaON'

// Prototypes
extern Err					RegisterForNotifications( Boolean bRegister, Boolean bRegisterVchrNotifications );

#endif /* __NOTIFYREG_H__ */
 
/*
 * NotifyReg.h
 */
 