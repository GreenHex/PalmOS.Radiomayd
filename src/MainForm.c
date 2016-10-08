/*
 * MainForm.c
 */

#include "MainForm.h"

// Prototypes
static Boolean 			HandlePhnNumListSelection( FormType* pForm, EventType* pEvent );
static Boolean 			MainFormHandleMenu( UInt16 menuID );
static void 			InitializeMainForm( FormType* pForm );
static void 			SetVisibility( FormType* pForm, Boolean bVisible );
static Boolean 			SavePrefsFromMainForm( FormType* pForm );

/*
 * HandlePhnNumListSelection()
 */
static Boolean HandlePhnNumListSelection( FormType* pForm, EventType* pEvent )
{	
	Boolean 			handled = false;
	msg_t 				msg;
	
	if ( pEvent->data.lstSelect.listID == MAIN_NUM_LST )
	{
		gPhnNumListP = pEvent->data.lstSelect.pList;
		gPhnNumListIdx = pEvent->data.lstSelect.selection;
		
		if ( gPhnNumListIdx != noListSelection )
		{
			switch ( LstPopupList( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_NUM_MENU_LST ) ) ) )
			{
				case 1: // Edit
					
					FrmPopupForm( ADDNUM_FORM );
									
					break;
	
				case 2: // Send Msg

					if ( !FrmCustomAlert( ConfirmationOKCancelAlert, "Send test message to selected address?", "", "" ) )
					{	
						MakeMessage( msg.Message );
					
						SendMsgToAddressAtIdx( gPhnNumListIdx, &msg, true );
					}
					break;
					
				case 3:	// Delete
				
					if ( !FrmCustomAlert( ConfirmationOKCancelAlert, "Delete selected item?", "", "" ) )
					{
						PhnSecDBRemoveRecordAtIdx( &gPhnNumListIdx );
						
						RedrawPhnNumList();
					}
					
					break;
											
				default:
					break;
			}
		}
	
		handled = true;
	}
	
	return ( handled );	

} // HandlePhnNumListSelection()

/* 
 * RedrawPhnNumList()
 */
void RedrawPhnNumList( void )
{
	if ( gPhnNumListP )
	{
		InitializePhnSecList( gPhnNumListP );
								
		gPhnNumListIdx = ( PhnSecDBGetNumberOfRecords() ) ? gPhnNumListIdx : noListSelection;
		
		LstSetSelection( gPhnNumListP, gPhnNumListIdx );	
	
		LstDrawList( gPhnNumListP );
		
		gPhnNumListP = NULL;
	}
	
	return;	
	
} // RedrawPhnNumList()
					
/*
 * MainFormHandleMenu()
 */
Boolean MainFormHandleMenu( UInt16 menuID )
{
	Boolean 			handled = false;
	EventType			newEvent;

	switch ( menuID )
	{
		case MAIN_MENU_EXIT:
		
			newEvent.eType = appStopEvent;
									
			EvtAddEventToQueue( &newEvent );	
					
			handled = true;
		
			break;
			
		case MAIN_MENU_PREFS:
		
			FrmPopupForm( PREFS_FORM );
			
			handled = true;
			
			break;
		
		case MAIN_MENU_HELP:
		
			FrmHelp( STRING_MAIN_HELP );
					
			handled = true;
			
			break;
			
		case MAIN_MENU_ABOUT:
		
			FrmPopupForm( ABOUT_FORM );
			
			handled = true;
			
			break;
						
		default:
			break;
	}

	return ( handled );

} // MainFormHandleMenu()

/*
 * MainFormHandleEvent()
 */
Boolean MainFormHandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	ListType*				pPhnNumList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_NUM_LST ) );
	FieldType*				pAddrField = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_CALLFWD_FLD ) );
	EventType				newEvent;
	Char					strIMSI[STRLEN_IMSI] = "\0";
	AddrLookupParamsType 	AddrParams;
			
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
			
			CancelAlerts();
			
			InitializeMainForm( pForm );
			
			FrmDrawForm( pForm );
		
			FrmNavObjectTakeFocus( pForm, MAIN_DONE_BTN );
								
			handled = true;
						
			break;
			
		case frmCloseEvent:
			
			SavePrefsFromMainForm( pForm );
								
			handled = false;
			
			break;
			
		case lstSelectEvent:
			
			handled = HandlePhnNumListSelection( pForm, pEvent ); // pForm, pEvent->data.lstSelect.selection );				
			
			break;	
			
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case MAIN_TEST_BTN:			
					{
						phnSecInfoBlock_t 			phnSecInfo;
						
						if ( ( !PhnSecDBGetAppInfo( &phnSecInfo ) ) 
								&& ( phnSecInfo.bEnabled ) )
						{
							phnSecInfo.bMayDay = true;
							phnSecInfo.numberDispatches = phnSecInfo.numberMessagingAttempts;
						
							PhnSecDBSetAppInfo( &phnSecInfo );
							
							SendMayDayNotification();
						}
					}
					handled = true;
					
					break;
					
				case MAIN_DONE_BTN:
					
					newEvent.eType = appStopEvent;
									
					EvtAddEventToQueue( &newEvent );
					
					handled = true;

					break;
					
				case MAIN_LOAD_IMSI_BTN:
				
					if ( !GetIMSI( strIMSI, STRLEN_IMSI ) )
					{
						SetFieldStr( pForm, MAIN_IMSI_FLD, strIMSI );
						FldDrawField( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_IMSI_FLD ) ) );
					}
					
					handled = true;
					
					break;
					
				case MAIN_CALLFWD_BTN:
					
					if ( pAddrField )
					{
						AddrParams.title = "Radimayd: Call Fwd Num";
						AddrParams.pasteButtonText = "Copy";
						AddrParams.field1 = addrLookupSortField;
						AddrParams.field2 = addrLookupAnyPhones;
						AddrParams.field2Optional = false;
						AddrParams.userShouldInteract = true;  
						AddrParams.formatStringP = "^matchphone"; // "^smsphone"; // "^listphone, ";
						
						PhoneNumberLookupCustom( pAddrField, &AddrParams, false );
					}
					
					handled = true;
					
					break;
				
				case MAIN_NEW_NUM_BTN:
		
					gPhnNumListP = pPhnNumList;
					gPhnNumListIdx = noListSelection;
					
					FrmPopupForm( ADDNUM_FORM );
					
					handled = true;
					
					break;
													
				default:
					break;
			}
		
			break;
			
		case menuOpenEvent:
		
			if ( gIsPrivate )
			{
				MenuShowItem( MAIN_MENU_PREFS );
				MenuShowItem( MAIN_MENU_SEPARATOR );
				MenuShowItem( MAIN_MENU_HELP );
			}
			else
			{
				MenuHideItem( MAIN_MENU_PREFS );
				MenuHideItem( MAIN_MENU_SEPARATOR );
				MenuHideItem( MAIN_MENU_HELP );
			}
			
			break;
			
		case menuEvent:
										
			handled = MainFormHandleMenu( pEvent->data.menu.itemID );
			
		default:
								
			break;
	}
	
	return ( handled );
	
} // MainFormHandleEvent()

/*
 * InitializeMainForm()
 */
static void InitializeMainForm( FormType* pForm )
{	
	ListType* 				pList = NULL;
	appPrefs_t				appPrefs;
	phnSecInfoBlock_t		phnSecInfo;

	if ( !pForm ) return;
	
	gIsPrivate = IsPrivate();
		
	if ( gIsPrivate )
	{			
		pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_NUM_LST ) );
	
		PhnSecDBGetAppInfo( &phnSecInfo );
		
		SetCtlBoolValue( pForm, MAIN_ENABLE_CBX, phnSecInfo.bEnabled );
		SetCtlBoolValue( pForm, MAIN_RADIO_ON_CBX, phnSecInfo.bSwitchOnRadio );
		SetFieldStr( pForm, MAIN_IMSI_FLD, phnSecInfo.IMSI );
		SetFieldStr( pForm, MAIN_LOCK_PWD_FLD, phnSecInfo.LockPwd );
		SetFieldStr( pForm, MAIN_LOCK_MSG_FLD, phnSecInfo.LockMsg );
		SetFieldStr( pForm, MAIN_CALLFWD_FLD, phnSecInfo.CallFwdNumber );
				
		if ( pList )
		{
			InitializePhnSecList( pList );	
			LstSetDrawFunction( pList, (ListDrawDataFuncPtr) DrawPhnSecListItem );
		}
		
	}
	else
	{
		ReadAppPrefs( &appPrefs );
		SetCtlBoolValue( pForm, MAIN_ENABLE_CBX, appPrefs.bEnabled );
		SetCtlBoolValue( pForm, MAIN_RADIO_ON_CBX, appPrefs.bSwitchOnRadio );
		SetCtlBoolValue( pForm, MAIN_SHOW_POPUP_CBX, appPrefs.bShowPopup );
		SetCtlBoolValue( pForm, MAIN_TRAP_PHN_ON_OFF_CBX
		, appPrefs.bTrapManualPhnOnOff );
		SetPopLabelFromList( pForm, MAIN_ACTIVATION_KEY_POP, MAIN_ACTIVATION_KEY_LST, appPrefs.activationKeyIdx );
		SetPopLabelFromList( pForm, MAIN_MODIFIER_POP, MAIN_MODIFIER_LST, appPrefs.activationModifierKeyIdx ); 
		SetPopLabelFromList( pForm, MAIN_TIME_INTERVAL_POP, MAIN_TIME_INTERVAL_LST, appPrefs.timeoutIdx );
	}
			
	SetVisibility( pForm, gIsPrivate );
	
	return;
	
} // InitializeMainForm()

/*
 * SetVisibility()
 */
static void SetVisibility( FormType* pForm, Boolean bVisible )
{
	UInt32					lastResetTime = 0;
	UInt32					uptime = 0;
	Char*					strTimeDate = NULL;
	Char					strLastReset[45] = "\0";
	Char					strUptime[45] = "\0";
	Char					strDuration[45] = "\0";
	
	if ( !pForm ) return;
	
	if ( bVisible )
	{
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_RADIO_ON_OFF_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_ACTIVATION_KEY_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_MODIFIER_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_TIME_INTERVAL_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_LAST_RESET_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_UPTIME_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_ACTIVATION_KEY_POP ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_MODIFIER_POP ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_TIME_INTERVAL_POP ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_SHOW_POPUP_CBX ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_TRAP_PHN_ON_OFF_CBX ) );
		//
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_IMSI_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_PWD_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_MSG_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_NOTIFY_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_NUM_LST ) );
	}
	else
	{
		lastResetTime = ReadUptimePrefs();
		MakeTimeDateStr( lastResetTime, &strTimeDate, NULL, NULL, false );
		
		StrCopy( strLastReset, "Last Reset: " );
		StrCat( strLastReset, strTimeDate );
		
		SetLabelStr( pForm, MAIN_LAST_RESET_LBL, strLastReset );
		
		uptime = TimGetSeconds() - lastResetTime;
		MakeTimeInterval( uptime, strDuration );
		StrCopy( strUptime, "Uptime: " );
		StrCat( strUptime, strDuration );
		
		SetLabelStr( pForm, MAIN_UPTIME_LBL, strUptime );
		
		ShiftControl( pForm, MAIN_RADIO_ON_CBX, 0, 2 );	
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_RADIO_ON_OFF_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_ACTIVATION_KEY_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_MODIFIER_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_TIME_INTERVAL_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_LAST_RESET_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_UPTIME_LBL ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_ACTIVATION_KEY_POP ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_MODIFIER_POP ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_TIME_INTERVAL_POP ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_SHOW_POPUP_CBX ) );
		FrmShowObject( pForm, FrmGetObjectIndex( pForm, MAIN_TRAP_PHN_ON_OFF_CBX ) );
		//
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_IMSI_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_PWD_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_MSG_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_NOTIFY_LBL ) );
		FrmHideObject( pForm, FrmGetObjectIndex( pForm, MAIN_NUM_LST ) );
	}
	//	
	CtlSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_LOAD_IMSI_BTN ) ), bVisible );
	CtlSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_NEW_NUM_BTN ) ), bVisible ); 
	CtlSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_CALLFWD_BTN ) ), bVisible );
	//
	FldSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_IMSI_FLD ) ), bVisible );
	FldSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_MSG_FLD ) ), bVisible );
	FldSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_LOCK_PWD_FLD ) ), bVisible );
	FldSetUsable( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_CALLFWD_FLD ) ), bVisible );
	
	if ( strTimeDate ) MemPtrFree( strTimeDate );
	
	return;
	
} // SetVisibility()

/*
 * SavePrefsFromMainForm()
 */
static Boolean SavePrefsFromMainForm( FormType* pForm )
{
	Boolean					retVal = false;
	appPrefs_t				appPrefs;
	phnSecInfoBlock_t		phnSecInfo;
	
	if ( !pForm ) return ( retVal );

	ReadAppPrefs( &appPrefs );
	PhnSecDBGetAppInfo( &phnSecInfo );
	
	if ( gIsPrivate )
	{	
		phnSecInfo.bEnabled = GetCtlBoolValue( pForm, MAIN_ENABLE_CBX );
		phnSecInfo.bSwitchOnRadio = GetCtlBoolValue( pForm, MAIN_RADIO_ON_CBX );
		GetFieldStr( pForm, MAIN_IMSI_FLD, phnSecInfo.IMSI, STRLEN_IMSI );
		GetFieldStr( pForm, MAIN_LOCK_PWD_FLD, phnSecInfo.LockPwd, STRLEN_LOCK_PWD );
		GetFieldStr( pForm, MAIN_LOCK_MSG_FLD, phnSecInfo.LockMsg, STRLEN_LOCK_MSG );
		GetFieldStr( pForm, MAIN_CALLFWD_FLD, phnSecInfo.CallFwdNumber, STRLEN_PHN_NUMBER );
		
		if ( ( !StrLen( phnSecInfo.LockPwd ) ) 
			|| ( !StrLen( phnSecInfo.LockMsg ) ) )
		{
			phnSecInfo.bEnabled = false;
			FrmCustomAlert( ErrOKAlert, "<Lock PWD> or <Lock MSG> is blank. ", APP_NAME, " is DISABLED." );
		}	
		PhnSecDBSetAppInfo( &phnSecInfo );	
	}
	else
	{
		appPrefs.bEnabled = GetCtlBoolValue( pForm, MAIN_ENABLE_CBX );
		appPrefs.bSwitchOnRadio = GetCtlBoolValue( pForm, MAIN_RADIO_ON_CBX );
		appPrefs.bShowPopup = GetCtlBoolValue( pForm, MAIN_SHOW_POPUP_CBX );
		appPrefs.bTrapManualPhnOnOff = GetCtlBoolValue( pForm, MAIN_TRAP_PHN_ON_OFF_CBX );
		//
		appPrefs.activationKeyIdx = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_ACTIVATION_KEY_LST ) ) );
		appPrefs.activationModifierKeyIdx = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_MODIFIER_LST ) ) );
		appPrefs.timeoutIdx = LstGetSelection( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, MAIN_TIME_INTERVAL_LST ) ) );
	
		WriteAppPrefs( &appPrefs );
	}

	RegisterForNotifications( phnSecInfo.bEnabled, ( appPrefs.activationKeyIdx ) | appPrefs.bTrapManualPhnOnOff );
	ProtectDBs( phnSecInfo.bEnabled );
	SetPrivacy( true );
	
	return ( retVal );	

} // SavePrefsFromMainForm()

/*
 * MainForm.c
 */