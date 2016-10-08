/*
 * AppMain.c
 */

#include <Hs.h>
#include <HsPhone.h>
#include <PalmTypes.h>
#include <Form.h>
#include <AlarmMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "Utils.h"
#include "Prefs.h"
#include "NotifyReg.h"
#include "Messaging.h"
#include "Security.h"
#include "PhnEventHandler.h"
#include "MainForm.h"
#include "PrefsForm.h"
#include "NumForm.h"
#include "PopupForm.h"
#include "PhnInfo.h"
#include "AttnAlerts.h"

// Prototypes
static Err 					AppStart( void );
static void 				AppStop( void );
static Boolean 				AppHandleEvent( EventType* pEvent );
static void 				AppEventLoop( void );
UInt32 						PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags );

/*
 * AppStart()
 */
static Err AppStart( void )
{
	gIsPrivate = false;
	gPhnNumListP = NULL;
	gPhnNumListIdx = noListSelection;
	
	FrmGotoForm( MAIN_FORM );
	
	return ( errNone );
	
} // AppStart()

/*
 * AppStop()
 */
static void AppStop( void )
{
	FrmCloseAllForms();	

	return;
	
} // AppStop()

/*
 * AppHandleEvent()
 */
static Boolean AppHandleEvent( EventType* pEvent )
{
	Boolean			handled = false;
	FormType* 		pForm = NULL;

	if ( pEvent->eType == frmLoadEvent )
	{
		pForm = FrmInitForm( pEvent->data.frmLoad.formID );
		if ( !pForm ) return ( handled );
		
		FrmSetActiveForm( pForm );
		
		switch ( pEvent->data.frmLoad.formID )
		{		
			case MAIN_FORM:
				
				FrmSetEventHandler( pForm, MainFormHandleEvent );
						
				handled = true;
				
				break;
			
			case PREFS_FORM:
				
				FrmSetEventHandler( pForm, PrefsFormHandleEvent );
						
				handled = true;
				
				break;
				
			case ADDNUM_FORM:
				
				FrmSetEventHandler( pForm, AddNumFormHandleEvent );
						
				handled = true;
				
				break;
				
			case ABOUT_FORM:
				
				FrmSetEventHandler( pForm, AboutFormHandleEvent );
						
				handled = true;
				
				break;
			
			default:
				break;
		}
	}
	
	return ( handled );

} // AppHandleEvent()

/*
 * AppEventLoop()
 */
static void AppEventLoop( void )
{
	Err			error;
	EventType	event;

	do {
		
		EvtGetEvent( &event, evtWaitForever );

		if ( SysHandleEvent( &event ) )
			continue;
			
		if ( MenuHandleEvent( 0, &event, &error ) )
			continue;
			
		if ( AppHandleEvent( &event ) )
			continue;

		FrmDispatchEvent( &event );

	} while ( event.eType != appStopEvent );

} // AppEventLoop()

/*
 * PilotMain()
 */
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
{
	Err 						error = errNone;
	
	switch ( cmd )
	{
		case phnLibLaunchCmdEvent:
		
			error = HandlePhnEvent( (PhnEventType *) cmdPBP );
			
			break;
			
		case sysAppLaunchCmdNotify:
			
			error = HandleNotifyEvent( (SysNotifyParamType *) cmdPBP );
			
			break;
			
		case sysAppLaunchCmdAlarmTriggered:
		
			((SysAlarmTriggeredParamType *) cmdPBP)->purgeAlarm = ( ( ((SysAlarmTriggeredParamType *)cmdPBP)->ref != send_message )
																		&& ( ((SysAlarmTriggeredParamType *)cmdPBP)->ref != radio_on ) );
						
			break;
			
		case sysAppLaunchCmdDisplayAlarm:
		
			HandleAlarm( (SysDisplayAlarmParamType *) cmdPBP );
			
			break;

		case sysAppLaunchCmdSystemReset:

			if ( ( error = RomVersionCompatible( MIN_VERSION, launchFlags ) ) == errNone )
			{
				HandleReset();
			}
				
			break;
			
		case sysAppLaunchCmdAttention:
		
			ProcessAttention( ( AttnLaunchCodeArgsType *) cmdPBP );
			
			break;
			
		case sysAppLaunchCmdGoTo:
						
			break;
				
		case sysAppLaunchCmdNormalLaunch:
	    case sysAppLaunchCmdPanelCalledFromApp:
	    case sysAppLaunchCmdReturnFromPanel:
			
			if ( ( error = RomVersionCompatible( MIN_VERSION, launchFlags ) ) )
			{
				FrmCustomAlert( ErrOKAlert, "Incompatible Device", "", "" );
				return ( error );
			} 
			
			if ( ( error = AppStart() ) == 0 )
			{
				AppEventLoop();
				AppStop();
			}
			
			break;
			
		default:
			break;
	}

	return ( error );
	
} // PilotMain()

/*
 * AppMain.c
 */
