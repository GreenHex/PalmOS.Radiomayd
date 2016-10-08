/*
 * MainForm.h
 */
 
#ifndef __MAINFORM_H__
#define __MAINFORM_H__

#include <Hs.h>
#include <Form.h>
#include <List.h>
#include <StringMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "NotifyReg.h"
#include "Prefs.h"
#include "Utils.h"
#include "Security.h"
#include "PhnSecDB.h"
#include "PhnInfo.h"
#include "Messaging.h"
#include "PhnEventHandler.h"
#include "AttnAlerts.h"
#include "../../TreoFlex2/src/FormUtils.h"

// Prototypes
extern void 				RedrawPhnNumList( void );
extern Boolean 				MainFormHandleEvent( EventType* pEvent );

#endif /* __MAINFORM_H__ */

/*
 * MainForm.h
 */