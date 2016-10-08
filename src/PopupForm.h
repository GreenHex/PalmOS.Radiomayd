/*
 * PopupForm.h
 */

#ifndef __POPUPFORM_H__
#define __POPUPFORM_H__

#include <Hs.h>
#include <HsNav.h>
#include <HsExt.h>
#include <SysEvtMgr.h>
#include <palmOneResources.h>	
#include <Form.h>
#include <StringMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "Prefs.h"
#include "Utils.h"

#define POPUP_FORM_WAIT_TIME			5

// Prototypes
extern UInt16  				ShowPopup( UInt16 timeoutIdx );
extern Boolean 				PopupFormHandleEvent( EventType* pEvent );
extern void 				PopupFormCallback( UInt16 almProcCmd, SysAlarmTriggeredParamType *paramP );


#endif /* __POPUPFORM_H__ */

/*
 * PopupForm.h
 */