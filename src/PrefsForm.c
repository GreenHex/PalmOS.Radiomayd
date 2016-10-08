/*
 * PrefsForm.c
 */

#include "PrefsForm.h"
 
// Protypes
static void 			InitializePrefsForm( FormType* pForm );
static Boolean 			SavePrefsFromPrefsForm( FormType* pForm );

/*
 * MainFormHandleEvent()
 */
Boolean PrefsFormHandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
		
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
		
			InitializePrefsForm( pForm );
			
			FrmDrawForm( pForm );
		
			FrmNavObjectTakeFocus( pForm, PREFS_OK_BTN );
								
			handled = true;
						
			break;
			
		case frmCloseEvent:
			
			handled = false;
			
			break;
			
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case PREFS_OK_BTN:
					
					SavePrefsFromPrefsForm( pForm );
					
					// NO break;
					
				case PREFS_CANCEL_BTN:
					
					FrmReturnToForm( 0 );
					
					handled = true;

					break;	
													
				default:
					break;
			}
		
			break;
			
		default:
								
			break;
	}
	
	return ( handled );
	
} // PrefsFormHandleEvent()

/*
 * InitializePrefsForm()
 */
static void InitializePrefsForm( FormType* pForm )
{	
	phnSecInfoBlock_t		phnSecInfo;

	if ( !pForm ) return;
	
	PhnSecDBGetAppInfo( &phnSecInfo );
	
	SetPopLabelFromList( pForm, PREFS_DELAY_POP, PREFS_DELAY_LST, phnSecInfo.messagingDelayIdx );
	SetPopLabelFromList( pForm, PREFS_INTERVAL_POP, PREFS_INTERVAL_LST, phnSecInfo.messagingIntervalIdx );
	SetPopLabelFromList( pForm, PREFS_NUM_ATTEMPTS_POP, PREFS_NUM_ATTEMPTS_LST, phnSecInfo.numberMessagingAttempts );
		
	return;
	
} // InitializePrefsForm()

/*
 * SavePrefsFromPrefsForm()
 */
static Boolean SavePrefsFromPrefsForm( FormType* pForm )
{
	Boolean					retVal = false;
	phnSecInfoBlock_t		phnSecInfo;
	
	if ( !pForm ) return ( retVal );

	PhnSecDBGetAppInfo( &phnSecInfo );
	
	phnSecInfo.messagingDelayIdx = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, PREFS_DELAY_LST ) ) );
	phnSecInfo.messagingIntervalIdx = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, PREFS_INTERVAL_LST ) ) );
	phnSecInfo.numberMessagingAttempts = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, PREFS_NUM_ATTEMPTS_LST ) ) );
	
	PhnSecDBSetAppInfo( &phnSecInfo );	
		
	return ( retVal );	

} // SavePrefsFromPrefsForm()

/*
 * PrefsForm.c
 */