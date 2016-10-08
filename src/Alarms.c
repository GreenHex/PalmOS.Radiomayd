/*
 * Alarms.c
 */

#include "Alarms.h"
 
/*
 * SetAlarm()
 */
Err SetAlarm( UInt32 secondsNext, alarmAction_e alarmAction )
{
	Err							error = errNone;
	UInt16 						cardNo = 0;
	LocalID						dbID = 0;

	if ( ( error = SysCurAppDatabase( &cardNo, &dbID ) ) ) return ( error );

	if ( ( !secondsNext ) || ( alarmAction == alarm_invalid ) )
	{
		error = AlmSetAlarm( cardNo, dbID, 0, 0, false );
	}
	else
	{
		error = AlmSetAlarm( cardNo, dbID, alarmAction, TimGetSeconds() + secondsNext, false );
	}
	
	
	return ( error );
	
} // SetAlarm()

/*
 * GetNextAlarm()
 */
Err GetNextAlarm( UInt32* nextAlmSecP, UInt32* refP )
{
	Err					error = errNone;
	UInt16 				cardNo = 0;
	LocalID				dbID = 0;
	
	if ( ( !nextAlmSecP ) || ( !refP ) ) return ( 1 );
	
	if ( ( error = SysCurAppDatabase( &cardNo, &dbID ) ) ) return ( error );
	
	*nextAlmSecP = AlmGetAlarm( cardNo, dbID, refP );
		
	if ( !( *nextAlmSecP  ) )
	{
		*refP = alarm_invalid;	
	}	
	
	return ( error );
	
} // GetNextAlarm()

/*
 * Alarms.c
 */