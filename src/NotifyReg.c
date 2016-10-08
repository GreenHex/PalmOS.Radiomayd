/*
 * NotifyReg.c
 */
 
#include "NotifyReg.h"

/*
 * RegisterForNotifications()
 */
Err RegisterForNotifications( Boolean bRegister, Boolean bRegisterVchrNotifications )
{
	Err					error = errNone;
	UInt16 				cardNo = 0; 
	LocalID  			dbID = 0;	
	UInt16 				PhoneLibRefNum = sysInvalidRefNum;
	DmSearchStateType 	stateInfo;

	if ( ( error = DmGetNextDatabaseByTypeCreator( true, &stateInfo, sysFileTPanel, appFileCreator, true, &cardNo, &dbID ) ) )
		return ( error );
	
	if ( ( error = HsGetPhoneLibrary( &PhoneLibRefNum ) ) )
		return ( error );
		
	if ( ( error = PhnLibOpen( PhoneLibRefNum ) ) )
		return ( error );
	
	if ( ( bRegister ) || ( bRegisterVchrNotifications ) )
	{
		error = PhnLibRegister( PhoneLibRefNum, appFileCreator, phnServiceVoice | phnServiceSMS );
	}
	else
	{
		error = PhnLibRegister( PhoneLibRefNum, appFileCreator, 0 );	
	}
		
	if ( bRegister )
	{
		SysNotifyRegister( cardNo, dbID, mdayDeferredNotifyEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, raonDeferredNotifyEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyDeviceUnlocked, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyDeleteProtectedEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyDBDeletedEvent, NULL, sysNotifyNormalPriority, NULL );
	}
	else
	{
		SysNotifyUnregister( cardNo, dbID, mdayDeferredNotifyEvent, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, raonDeferredNotifyEvent, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyDeviceUnlocked, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyDeleteProtectedEvent , sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyDBDeletedEvent, sysNotifyNormalPriority );
	}
	
	if ( bRegisterVchrNotifications )
	{
		SysNotifyRegister( cardNo, dbID, sysNotifyVirtualCharHandlingEvent, NULL, sysNotifyNormalPriority, NULL ); 
	}
	else
	{
		SysNotifyUnregister( cardNo, dbID, sysNotifyVirtualCharHandlingEvent, sysNotifyNormalPriority );
	}
			
	PhnLibClose( PhoneLibRefNum );

	return ( error );
		
} // RegisterForNotifications()

/*
 * NotifyReg.c
 */