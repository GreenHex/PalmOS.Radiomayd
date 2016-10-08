/*
 * Messaging.h
 */

#include "Messaging.h"

/*
 * DispatchMessages()
 */
Err DispatchMessages( void )
{
	Err						error = errNone;
	msg_t					msg;
	phnSecInfoBlock_t 		phnSecInfo;
	UInt16					idx = 0;
	UInt16					numRecords = PhnSecDBGetNumberOfRecords();
	UInt32					delaySecs[5] = { 1, 2, 3, 5, 10 };
	UInt32					intervalSecs[5] = { 30, 60 , 180, 300, 600 };

	if ( !numRecords ) return ( dmErrIndexOutOfRange );
	if ( ( error = PhnSecDBGetAppInfo( &phnSecInfo ) ) != errNone ) return ( error );

	if ( ( !phnSecInfo.bEnabled ) 
			|| ( !phnSecInfo.bMayDay ) 
			|| ( !phnSecInfo.numberDispatches ) ) 
		return ( 1 );

	if ( ( !phnSecInfo.messagingDelayIdx ) || ( phnSecInfo.messagingDelayIdx > 5 ) )
	{
		phnSecInfo.messagingDelayIdx = 1;	
	}

	MakeMessage( msg.Message );

	for ( idx = 0; idx < numRecords; idx++ )
	{
		SendMsgToAddressAtIdx( idx, &msg, false );

		SysTaskDelay( SysTicksPerSecond() * delaySecs[phnSecInfo.messagingDelayIdx] );
	}
	
	--(phnSecInfo.numberDispatches);
/*	if ( !( phnSecInfo.numberDispatches ) )
	{
		phnSecInfo.bMayDay = false;
	}
*/	
	PhnSecDBSetAppInfo( &phnSecInfo );
	
	if ( phnSecInfo.numberDispatches )
	{
		SetAlarm( intervalSecs[phnSecInfo.messagingIntervalIdx], send_message );
	}
	else
	{
		SetAlarm( 0, alarm_invalid );
	}
	
	return ( error );
	
} // DispatchMessages()

/*
 * SendMsgToAddressAtIdx()
 */
Err SendMsgToAddressAtIdx( UInt16 idx, msg_t* msgP, Boolean bDispProgress )
{
	Err						error = errNone;
	phnNum_t				phnNum;
	ProgressPtr				pProgress = NULL;

	error = PhnSecDBGetRecordAtIdx( idx, &phnNum );

	if ( !error )
	{
		StrCopy( msgP->Address, phnNum.Number );
		StrCopy( msgP->CallerID, phnNum.Name );
		
		if ( bDispProgress )
		{
			pProgress = PrgStartDialog( APP_NAME, MsgProgressCallback, NULL );	
		}
		
		if ( pProgress )
		{
			PrgUpdateDialog( pProgress, 0, 0, msgP->CallerID, true );
		}
		
		error = SendMessage( msgP, pProgress, NULL );
		
		if ( pProgress ) 
		{
			PrgHandleEvent( pProgress, NULL );
			PrgStopDialog( pProgress, true );
			pProgress = NULL;										
		}
	}

	return ( error );
	
} // SendMsgToAddressAtIdx()

/*
 * Messaging.h
 */