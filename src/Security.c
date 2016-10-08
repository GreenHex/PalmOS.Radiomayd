/*
 * Security.c
 */
 
#include "Security.h"

/*
 * LockDBs()
 */
void LockDBs( Boolean bLock )
{
	static DmOpenRef		secDbP = NULL;
	static DmOpenRef		prcDbP = NULL;
	
	return;
	
	if ( bLock )
	{
		if ( !secDbP )
		{
			secDbP = DmOpenDatabaseByTypeCreator( PhnSecDBType, PhnSecDBCreator, dmModeReadOnly | dmModeLeaveOpen );
		}
		
		if ( !prcDbP )
		{
			
		}
	}
	else
	{
		if ( secDbP )
		{
			DmCloseDatabase( secDbP );
			secDbP = NULL;
		}
		
		if ( prcDbP )
		{
			DmCloseDatabase( prcDbP );
			prcDbP = NULL;
		}
	}
	
	return;
		
} // LockDBs()

/*
 * ProtectDBs()
 */
void ProtectDBs( Boolean bProtect )
{
	UInt16 					cardNo = 0;
  	LocalID 				dbID = 0;
	DmSearchStateType 		stateInfo;
	
	// PhnSecDB
	if ( !DmGetNextDatabaseByTypeCreator( true, &stateInfo, PhnSecDBType, PhnSecDBCreator, true, &cardNo, &dbID ) )
    {
    	DmDatabaseProtect( cardNo, dbID, false );
    	
    	if ( ( bProtect ) && ( DmDatabaseProtect( cardNo, dbID, false ) == dmErrDatabaseNotProtected ) )
    	{ 
    		DmDatabaseProtect( cardNo, dbID, true );
    	}
    }
    
    // Radiomayd
    if ( !SysCurAppDatabase( &cardNo, &dbID ) )
    {
    	DmDatabaseProtect( cardNo, dbID, false );
    	
    	if ( ( bProtect ) && ( DmDatabaseProtect( cardNo, dbID, false ) == dmErrDatabaseNotProtected ) )
    	{ 
    		DmDatabaseProtect( cardNo, dbID, true );
    	}
    }
    	
	return;
	
} // ProtectDBs()

/*
 * LockDevice
 */
void LockDevice( void )
{
  	UInt16 					cardNo = 0;
  	LocalID 				dbID = 0;
  	DmSearchStateType 		stateInfo;

	PrefSetPreference( prefShowPrivateRecords, hidePrivateRecords );

  	if ( !PrefGetPreference( prefDeviceLocked ) && ( PwdExists() ) )
    {
      	if ( !DmGetNextDatabaseByTypeCreator( true, &stateInfo, sysFileTApplication,
      				sysFileCSecurity, true, &cardNo, &dbID ) )
      	{			
      		SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdSystemLock, 0 );
      	}
    }
    else
    {
    	SysSleep( false, false );
    }
    
    return;
    
} // LockDevice()

/*
 * SetLockPassword()
 */
void SetLockPassword( void )
{
	phnSecInfoBlock_t 		phnSecInfo;

	if ( !PhnSecDBGetAppInfo( &phnSecInfo ) )
	{
		if ( !PwdExists() )
		{
			PwdRemove();
			PwdSet( "", phnSecInfo.LockPwd );
		}
	}
	
	return;
	
} // SetLockPassword()

/*
 * IsPrivate()
 */
Boolean IsPrivate( void )
{
	return ( PrefGetPreference( prefShowPrivateRecords ) == showPrivateRecords );

} // IsPrivate()

/*
 * SetPrivacy()
 */
void SetPrivacy( Boolean bHide )
{
	if ( bHide )
	{
		PrefSetPreference( prefShowPrivateRecords, hidePrivateRecords );
	}
	else
	{
		PrefSetPreference( prefShowPrivateRecords, showPrivateRecords );
	}
	
	return;
		
} // SetPrivacy()

/*
 * Security.c
 */