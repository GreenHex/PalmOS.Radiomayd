/*
 * PopupForm.c
 */
 
#include "PopupForm.h"

// Prototypes
static void 			InitializePopupForm( FormType* pForm, UInt16 timeoutIdx );

/*
 * ShowPopup()
 */ 
UInt16 ShowPopup( UInt16 timeoutIdx )
{
	UInt16					retVal = noListSelection;
	FormActiveStateType		formState;
	FormType*				pForm = FrmInitForm( POPUP_FORM );
	
	if ( pForm )
	{
		FrmSaveActiveState( &formState );
		
		InitializePopupForm( pForm, timeoutIdx );
		
		FrmSetEventHandler( pForm, PopupFormHandleEvent );
		
		AlmSetProcAlarm( PopupFormCallback, 0, TimGetSeconds() + POPUP_FORM_WAIT_TIME );
		
		FrmDoDialog( pForm );
		
		AlmSetProcAlarm( PopupFormCallback, 0, 0 );	
		
		retVal = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, POPUP_TIMEOUT_LST ) ) );
		
		FrmEraseForm( pForm );
		FrmDeleteForm( pForm );
	
		FrmRestoreActiveState( &formState );
	}
		
	return ( retVal );
	
} // ShowPopup()

/*
 * PopupFormCallback()
 */
void PopupFormCallback( UInt16 almProcCmd, SysAlarmTriggeredParamType *paramP )
{
	if ( almProcCmd == almProcCmdTriggered )
	{				
		FrmReturnToForm( 0 );
	}
		
	EvtWakeup();		
	
	return;
	
} // PopupFormCallback()

/*
 * PopupFormHandleEvent()
 */
Boolean PopupFormHandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	ListType*				pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, POPUP_TIMEOUT_LST ) );
	appPrefs_t				prefs;
	WChar					wChar = 0;
	UInt16					keyCode = 0;
	UInt16					modifier = 0;
	
	switch ( pEvent->eType )
	{
		case keyDownEvent:
			
			wChar = pEvent->data.keyDown.chr;
			keyCode = pEvent->data.keyDown.keyCode;
			modifier = pEvent->data.keyDown.modifiers;
			
			ReadAppPrefs( &prefs );
			
			if ( EvtCharEqualsLaunchChar( wChar, keyCode, modifier, prefs.activationKeyIdx, prefs.activationModifierKeyIdx ) )
			{	
				LstSetSelection( pList, noListSelection );
			
				FrmReturnToForm( 0 );				
				
				handled = true;
				
				break;
			}
				
			switch ( pEvent->data.keyDown.chr )
			{
				case vchrRockerLeft:
				case vchrRockerRight: 
				
					LstSetSelection( pList, noListSelection );
					
					FrmReturnToForm( 0 );
	
					handled = true;
					
					break;
											
				default:
				
					AlmSetProcAlarm( PopupFormCallback, 0, TimGetSeconds() + POPUP_FORM_WAIT_TIME );
					
					break;
			}
			
			break;
			
		case lstExitEvent:
		
			LstSetSelection( pList, noListSelection );
						
			FrmReturnToForm( 0 );
			
			handled = true;
			
			break;
			
		case lstSelectEvent:
		
			FrmReturnToForm( 0 );
			
			handled = true;
			
			break;
											
		default:
								
			break;
	}
	
	return ( handled );
	
} // PopupFormHandleEvent()

/*
 * InitializePopupForm()
 */
static void InitializePopupForm( FormType* pForm, UInt16 timeoutIdx )
{
	ListType*		pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, POPUP_TIMEOUT_LST ) );
	
	LstSetSelection( pList, timeoutIdx );
	LstSetTopItem( pList, timeoutIdx ); 
	
	return;
	
} // InitializePopupForm()


/*
 * PopupForm.c
 */