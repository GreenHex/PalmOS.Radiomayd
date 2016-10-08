/*
 * PhnEventHandler.c
 */
 
#include "PhnEventHandler.h"

// Prototypes
static Err 				GetAddressFromSMS( UInt16 phnLibRefNum, PhnDatabaseID msgID, Char* address, UInt16 buflenAddress );
static Err 				SetCallForwarding( Boolean set );

/*
 * HandlePhnEvent()
 */
Err HandlePhnEvent( PhnEventType* phnEventP )
{
	Err							error = errNone;
	phnSecInfoBlock_t 			phnSecInfo;
	appPrefs_t					prefs;
	Char						IMSI[STRLEN_IMSI] = "\0";
	UInt32						timeoutSecs[13] = { 300, 600, 900, 1800, 2700, 3600, 5400, 7200, 10800, 14400, 21600, 28800, 43200 };
	// UInt16						timeoutIdx = noListSelection;
	
	switch ( phnEventP->eventType )
	{	
/*
 * unique identifier for Indication notification.   This value will be
 * contained in PhnEventType notification events of phnEvtIndication type.
 *
 * typedef enum {
 * 	indicationSIMReady,
 * 	indicationSIMMessages,
 * 	indicationNetworkSearch,
 * 	indicationPasswordAccepted,
 * 	indicationNetworkAvailable,
 * 	indicationStartingRadio,
 * 	indicationPoweringOffRadio,
 * 	indicationResettingRadio,
 * 	indicationAuthFailure,
 * 	indicationSIMChanged,
 * 	indicationInitComplete,
 * 	indicationOtaspMsg
 * } PhnIndicationKind;
 */
		case phnEvtIndication: // 0x0007  
			
			switch ( phnEventP->data.indication.kind )
			{
				case indicationSIMChanged:
					
					if ( ( !PhnSecDBGetAppInfo( &phnSecInfo ) ) && ( phnSecInfo.bEnabled ) )
					{
						if ( StrLen( phnSecInfo.IMSI ) )
						{
							GetIMSI( IMSI, STRLEN_IMSI );
							
							if ( StrCompare( IMSI, phnSecInfo.IMSI ) )
							{
								phnSecInfo.bMayDay = true;
								phnSecInfo.numberDispatches = phnSecInfo.numberMessagingAttempts;
							}
						}
						else
						{
							phnSecInfo.bMayDay = true;
							phnSecInfo.numberDispatches = phnSecInfo.numberMessagingAttempts;							
						}
						
						PhnSecDBSetAppInfo( &phnSecInfo );
					}
					
					break;
							
				case indicationSIMReady:
				
					if ( ( !PhnSecDBGetAppInfo( &phnSecInfo ) ) && ( phnSecInfo.bEnabled ) )
					{
						if ( StrLen( phnSecInfo.IMSI ) )
						{
							GetIMSI( IMSI, STRLEN_IMSI );
							
							if ( StrCompare( IMSI, phnSecInfo.IMSI ) )
							{
								phnSecInfo.bMayDay = true;
								phnSecInfo.numberDispatches = phnSecInfo.numberMessagingAttempts;
	
								PhnSecDBSetAppInfo( &phnSecInfo );
								
								if ( ( phnSecInfo.bEnabled ) 
										&& ( phnSecInfo.bMayDay ) )
								{
									SendMayDayNotification();
								}
							}						
						}
					}
						
					break;
					
				case indicationInitComplete: // after reset
					
					if ( ( !PhnSecDBGetAppInfo( &phnSecInfo ) ) 
							&& ( phnSecInfo.bEnabled ) 
							&& ( phnSecInfo.bMayDay ) )
					{
						SendMayDayNotification();
					}
					
					break;
					
				case indicationStartingRadio:
				
					ReadAppPrefs( &prefs );
					
					if ( prefs.bTrapManualPhnOnOff )
					{
						SetAlarm( 0, alarm_invalid );
						CancelAlerts();
					}
					
					break;
					
				/* case indicationPoweringOffRadio:
				
					ReadAppPrefs( &prefs );
					SetAlarm( timeoutSecs[prefs.timeoutIdx], radio_on );
					DoAlert( attnRadioOFF, kAttnLevelSubtle );
					
					break;
				*/
				default:
					break;
			}
			
			phnEventP->acknowledge = true;
			
			break;
			
		case phnEvtMessageInd: // 0x0011 // Single segment msg indication
		
			HandleSMSEvent( &(phnEventP->data.params), &(phnEventP->acknowledge) );
						
			break;

/*
 * Power state of the radio
 *
 * enum _PhnPowerType
 * {
 * 	phnPowerOff,
 * 	phnPowerOn,
 * 	phnPowerStartCharging,
 * 	phnPowerStopCharging,
 * 	phnPowerLow,
 * 	phnPowerSave
 * };
 * typedef UInt8 PhnPowerType;	
 */			
		case phnEvtPower:	
		
			switch ( phnEventP->data.power.state )
			{
				case phnPowerOff:
				
					ReadAppPrefs( &prefs );
					
					if ( prefs.bTrapManualPhnOnOff )
					{
						/*
						if ( prefs.bShowPopup )
						{	
							if ( ( timeoutIdx = ShowPopup( prefs.timeoutIdx ) ) != noListSelection )
							{
								prefs.timeoutIdx = timeoutIdx; 
								WriteAppPrefs( &prefs );	
							}
						}
						*/
						SetAlarm( timeoutSecs[prefs.timeoutIdx], radio_on );
						DoAlert( attnRadioOFF, kAttnLevelSubtle );
					}
						
					break;
					
				case phnPowerOn:
					
					ReadAppPrefs( &prefs );
					
					if ( prefs.bTrapManualPhnOnOff )
					{
						SetAlarm( 0, alarm_invalid );
						CancelAlerts();
					}
										
					break;
								
				default:
					break;	
			}
		
			break;

		default:		
			break;
	}
	
	return ( error );
	
} // HandlePhnEvent()

/*
 * HandleNotifyEvent()
 */
Err HandleNotifyEvent( SysNotifyParamType* notifyParamP )
{
	Err							error = errNone;
	phnSecInfoBlock_t 			phnSecInfo;
	appPrefs_t					prefs;
	WChar						wChar = 0;
	UInt16						keyCode = 0;
	UInt16						modifier = 0;
	UInt32						timeoutSecs[13] = { 300, 600, 900, 1800, 2700, 3600, 5400, 7200, 10800, 14400, 21600, 28800, 43200 };
	PhnPowerType				phnPowerState;
	UInt32						nextAlmSec = 0;
	UInt32						ref = 0;
	
	switch ( notifyParamP->notifyType )
	{
		case sysNotifyVirtualCharHandlingEvent:
		
			wChar = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.chr;
			keyCode = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.keyCode;
			modifier = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.modifiers;
				
			ReadAppPrefs( &prefs );
			
			if ( EvtCharEqualsLaunchChar( wChar, keyCode, modifier, prefs.activationKeyIdx, prefs.activationModifierKeyIdx ) 
					/* || ( ( prefs.bTrapManualPhnOnOff ) && ( wChar == hsChrRadioPower ) && ( modifier & commandKeyMask ) ) */ )
			{
				GetNextAlarm( &nextAlmSec, &ref );
				
				if ( ( prefs.activationKeyIdx ) 
							&& ( ref != send_message )			
							&& ( !PhnSecDBGetAppInfo( &phnSecInfo ) ) 
							&& ( !phnSecInfo.bMayDay ) )
				{
					phnPowerState = GetPhonePowerState();
					
					if ( phnPowerState == phnPowerOn ) // Switch off...
					{
						if ( prefs.bShowPopup )
						{	
							if ( ( prefs.timeoutIdx = ShowPopup( prefs.timeoutIdx ) ) != noListSelection )
							{
								WriteAppPrefs( &prefs );	
							}
						}
						
						if ( prefs.timeoutIdx != noListSelection )
						{
							SetAlarm( timeoutSecs[prefs.timeoutIdx], radio_on );
							SetRadioState( false );
							DoAlert( attnRadioOFF, kAttnLevelSubtle );					
						}
					}
					else if ( phnPowerState == phnPowerOff ) // switch on...
					{
						SetAlarm( 0, alarm_invalid );
						SetRadioState( true );
						CancelAlerts();
					}
				
					notifyParamP->handled = true;
				}
			}
			
			break;
			
		case sysNotifyDeleteProtectedEvent:
		case sysNotifyDBDeletedEvent:
		
			if ( ( ((SysNotifyDBInfoType *)notifyParamP->notifyDetailsP)->creator == appFileCreator )
							|| ( ((SysNotifyDBInfoType *)notifyParamP->notifyDetailsP)->creator == PhnSecDBCreator ) )	
			{
				LockDBs( true );
				SetLockPassword();
				LockDevice();
			}
					
			notifyParamP->handled = true;
			
			break;

		case mdayDeferredNotifyEvent:

			SetCallForwarding( true );
			DispatchMessages();
			
			notifyParamP->handled = true;

			break;
			
		case sysNotifyDeviceUnlocked:
			
			if ( !PhnSecDBGetAppInfo( &phnSecInfo ) )
			{
				LockDBs( false );
				phnSecInfo.bMayDay = false;
				phnSecInfo.numberDispatches = 0;
				PhnSecDBSetAppInfo( &phnSecInfo );
				SetCallForwarding( false );
				SetAlarm( 0, alarm_invalid );
			}
					
			notifyParamP->handled = true;
			
			break;
		
		case raonDeferredNotifyEvent:
		
			SetRadioState( true );
			
			notifyParamP->handled = true;
			
			break;
						
		default:
			break;
	}
	
	return ( error );
	
} // HandleNotifyEvent()

/*
 * HandleSMSEvent()
 */
Err HandleSMSEvent( PhnSMSParams* paramsP, Boolean* handledP )
{
	Err						error = errNone;
	UInt16 					phnLibRefNum = sysInvalidRefNum;
	PhnDatabaseID 			msgID = paramsP->id;
	MemHandle 				memH = NULL;
	Char*					memP = NULL;
	phnSecInfoBlock_t 		phnSecInfo;
	msg_t 					msg;
	
	if ( handledP  )
	{
		*handledP = false;	
	}
	
	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) != errNone )
  		return ( error );

	if ( ( error = PhnLibOpen( phnLibRefNum ) ) != errNone ) 
		return ( error );
	
	error = PhnLibGetText( phnLibRefNum, msgID, &memH );
	
	if ( error == errNone )
	{
		if ( ( error = PhnSecDBGetAppInfo( &phnSecInfo ) ) == errNone )
		{
			memP = MemHandleLock( memH );
			
			if ( StrStr( memP, APP_NAME ) 
					&& StrStr( memP, phnSecInfo.LockMsg ) 
					&& StrStr( memP, phnSecInfo.LockPwd ) )
			{
				if ( GetAddressFromSMS( phnLibRefNum, msgID, msg.Address, STRLEN_PHN_NUMBER ) == errNone )
				{
					MakeMessage( msg.Message );
					error = SendMessage( &msg, NULL, NULL );
				}
				
				phnSecInfo.bMayDay = true;
				phnSecInfo.numberDispatches = phnSecInfo.numberMessagingAttempts;
						
				PhnSecDBSetAppInfo( &phnSecInfo );
				
				SendMayDayNotification();
				
				PhnLibDeleteMessage( phnLibRefNum, msgID, false );
						
				if ( handledP  )
				{
					*handledP = true;	
				}
			}
			
			if ( memH )
			{
				MemHandleUnlock( memH );
				MemHandleFree( memH );
			}
		}	
	}
	
	PhnLibClose( phnLibRefNum );
		
	return ( error );
	
} // HandleSMSEvent()

/*
 * GetAddressFromSMS()
 */
static Err GetAddressFromSMS( UInt16 phnLibRefNum, PhnDatabaseID msgID, Char* address, UInt16 buflenAddress )
{
	Err						error = errNone;
	PhnAddressList 			addrList;
    PhnAddressHandle 		addrH = NULL;
    Char*					addrP = NULL;

	if ( phnLibRefNum == sysInvalidRefNum ) return ( 1 );
	
	if ( ( error = PhnLibGetAddresses( phnLibRefNum, msgID, &addrList ) ) != errNone ) return error ;

    error = PhnLibGetNth( phnLibRefNum, addrList, 1, &addrH );

	if ( !error )
	{
    	addrP = PhnLibGetField( phnLibRefNum, addrH, phnAddrFldPhone );
    	
    	if ( ( address ) && ( buflenAddress ) )
    	{
	    	StrNCopy( address, addrP, buflenAddress - 1 );
			address[buflenAddress - 1] = chrNull;
    	}
	}
	
    PhnLibDisposeAddressList( phnLibRefNum, addrList );
    if ( addrH ) MemHandleFree( addrH );
    if ( addrP ) MemPtrFree( addrP );
	
	return ( error );
		
} // GetAddressFromSMS

/*
 * HandleReset()
 */
void HandleReset( void )
{
	phnSecInfoBlock_t 			phnSecInfo;
	appPrefs_t					prefs;
	SysNotifyParamType* 		raonNotifyParamP = NULL;
	prevModPref_t				modPrefs;
	
	if ( !PhnSecDBGetAppInfo( &phnSecInfo ) )
	{
		ReadAppPrefs( &prefs );
		RegisterForNotifications( phnSecInfo.bEnabled, ( prefs.activationKeyIdx ) | prefs.bTrapManualPhnOnOff );
		ProtectDBs( phnSecInfo.bEnabled );
		
		if ( ( phnSecInfo.bEnabled ) && ( phnSecInfo.bSwitchOnRadio ) )
		{
			raonNotifyParamP = MemPtrNew( sizeof( SysNotifyParamType ) );

			if ( raonNotifyParamP )
			{
				MemSet( raonNotifyParamP, sizeof( SysNotifyParamType ), 0 );
											
				raonNotifyParamP->notifyType = raonDeferredNotifyEvent;
				raonNotifyParamP->broadcaster = appFileCreator;
				raonNotifyParamP->notifyDetailsP = NULL;
				raonNotifyParamP->handled = false;
					
				SysNotifyBroadcastDeferred( raonNotifyParamP, NULL );
				
				MemPtrFree( raonNotifyParamP );
			}
		}
	}
	
	WriteUptimePrefs();
	InitializeModifierCharPrefs( &modPrefs );
	WriteModifierCharPrefs( &modPrefs );
	
	return;
		
} // HandleReset()

/*
 * SendMayDayNotification()
 */
void SendMayDayNotification( void )
{
	SysNotifyParamType* 			maydayNotifyParamP = NULL;

	SetLockPassword();
	LockDevice();
	
	maydayNotifyParamP = MemPtrNew( sizeof( SysNotifyParamType ) );
			
	if ( maydayNotifyParamP )
	{
		MemSet( maydayNotifyParamP, sizeof( SysNotifyParamType ), 0 );
		
		maydayNotifyParamP->notifyType = mdayDeferredNotifyEvent;
		maydayNotifyParamP->broadcaster = appFileCreator;
		maydayNotifyParamP->notifyDetailsP = NULL;
		maydayNotifyParamP->handled = false;
	
		SysNotifyBroadcastDeferred( maydayNotifyParamP, NULL );
		
		MemPtrFree( maydayNotifyParamP );
	}
		
	return;

} // SendMayDayNotification()

/*
 * SetCallForwarding()
 */
static Err SetCallForwarding( Boolean set )
{
	Err						error = errNone;
	UInt16 					phnLibRefNum = sysInvalidRefNum;
	phnSecInfoBlock_t 		phnSecInfo;
	MemHandle				phnNumH = NULL;
	Char					CallFwdNumber[STRLEN_PHN_NUMBER] = "\0";
	
	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) )
			return ( error );
					
	if ( ( error = PhnLibOpen( phnLibRefNum ) ) )
			return ( error );
	
	if ( set )
	{
		if ( ( error = PhnSecDBGetAppInfo( &phnSecInfo ) ) == errNone )
		{	
			StrCopy( CallFwdNumber, phnSecInfo.CallFwdNumber );
		}
	
		MakeNum( CallFwdNumber );
	
		if ( StrLen( CallFwdNumber ) )
		{
			phnNumH = PhnLibNewAddress( phnLibRefNum, CallFwdNumber, phnLibUnknownID );
		
			if ( phnNumH )
			{
				error = PhnLibSetForwarding( phnLibRefNum, phnForwardUnconditional, phnForwardModeEnable, phnNumH );							
			}
		}
	}
	else
	{
		phnNumH = PhnLibNewAddress( phnLibRefNum, "", phnLibUnknownID );
			
		if ( phnNumH )
		{
			error = PhnLibSetForwarding( phnLibRefNum, phnForwardUnconditional, phnForwardModeDisable, phnNumH );
		}
	}
	
	if ( phnNumH ) MemHandleFree( phnNumH ); 
		
	PhnLibClose( phnLibRefNum );

	return ( error );		
	
} // SetCallForwarding()

/*
 * HandleAlarm()
 */
void HandleAlarm( SysDisplayAlarmParamType* cmdPBP )
{
	alarmAction_e		almAction = (alarmAction_e) cmdPBP->ref;
	
	cmdPBP->ref = alarm_invalid;
					
	if ( almAction == send_message )
	{
		DispatchMessages();
	}
	else if ( almAction == radio_on )
	{
		SetAlarm( 0, alarm_invalid );
		SetRadioState( true );
		DoAlert( attnRadioON, kAttnLevelSubtle );
	}
	
	return;
	
} // HandleAlarm()

/*
 * PhnEventHandler.c
 */