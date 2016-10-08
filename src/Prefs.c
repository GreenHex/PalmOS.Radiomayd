/*
 * Prefs.c
 */

#include "Prefs.h"

// Prototypes
static void 			InitializeAppPrefs( appPrefs_t* pPrefs );

/*
 * WriteAppPrefs()
 */
void WriteAppPrefs( appPrefs_t* pPrefs )
{
	if ( !pPrefs ) return;
	
	PrefSetAppPreferences( appPrefCreator, appPrefID, appPrefVersionNum, pPrefs, sizeof( appPrefs_t ), appPrefType );
	
	return;
	
} // WriteAppPrefs()

/*
 * ReadAppPrefs()
 */
void ReadAppPrefs( appPrefs_t* pPrefs )
{
	UInt16 			prefSize = sizeof( appPrefs_t );
	UInt16			prefsVersion = 0;

	if ( !pPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( appPrefCreator, appPrefID, pPrefs, &prefSize, appPrefType );
	
	if ( prefsVersion != appPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( appPrefCreator, appPrefID, prefsVersion, NULL, 0, appPrefType );				
	
		InitializeAppPrefs( pPrefs );
		
		WriteAppPrefs( pPrefs );
	}
	
	return;
	
} // ReadAppPrefs()

/*
 * InitializeAppPrefs()
 */
static void InitializeAppPrefs( appPrefs_t* pPrefs )
{
	if ( !pPrefs ) return;
	
	MemSet( pPrefs, sizeof( appPrefs_t ), 0 );
	
	pPrefs->bEnabled = true;
	pPrefs->bSwitchOnRadio = true;
	
	return;
	
} // InitializeAppPrefs

/*
 * WriteChPrefs()
 */
void WriteChPrefs( chPrefs_t* pChPrefs )
{
	if ( !pChPrefs ) return;
	
	PrefSetAppPreferences( chPrefCreator, chPrefID, chPrefVersionNum, pChPrefs, sizeof( chPrefs_t ), chPrefType );
	
	return;
	
} // WriteChPrefs()

/*
 * ReadChPrefs()
 */
void ReadChPrefs( chPrefs_t* pChPrefs )
{
	UInt16 			prefSize = sizeof( chPrefs_t );
	UInt16			prefsVersion = 0;

	if ( !pChPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( chPrefCreator, chPrefID, pChPrefs, &prefSize, chPrefType );
	
	if ( prefsVersion != chPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( chPrefCreator, chPrefID, prefsVersion, NULL, 0, chPrefType );				
	
		InitializeChPrefs( pChPrefs );
		
		WriteChPrefs( chPrefType );
	}
	
	return;
	
} // ReadAppPrefs()

/*
 * InitializeChPrefs()
 */
void InitializeChPrefs( chPrefs_t* pChPrefs )
{
	if ( !pChPrefs ) return;
	
	MemSet( pChPrefs, sizeof( chPrefs_t ), 0 );
	
	return;

} // InitializeChPrefs()

/*
 * WriteUptimePrefs()
 */
void WriteUptimePrefs( void )
{
	uptimePrefs_t	uptimePrefs;
	
	uptimePrefs.secondsAtReset = TimGetSeconds();
	
	PrefSetAppPreferences( uptimePrefCreator, uptimePrefID, uptimePrefVersionNum, &uptimePrefs, sizeof( uptimePrefs_t ), uptimePrefType );
	
	return;
	
} // WriteUptimePrefs()

/*
 * ReadUptimePrefs()
 */
UInt32 ReadUptimePrefs( void )
{
	uptimePrefs_t	uptimePrefs;
	UInt16 			prefSize = sizeof( uptimePrefs_t );
	UInt16			prefsVersion = 0;
	
	prefsVersion = PrefGetAppPreferences( uptimePrefCreator, uptimePrefID, &uptimePrefs, &prefSize, uptimePrefType );
	
	if ( prefsVersion != uptimePrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( uptimePrefCreator, uptimePrefID, prefsVersion, NULL, 0, uptimePrefType );				
	
		WriteUptimePrefs();
		
		uptimePrefs.secondsAtReset = TimGetSeconds();
	}

	return ( uptimePrefs.secondsAtReset );	
	
} // ReadUptimePrefs()

/*
 * WriteModifierCharPrefs()
 */
void WriteModifierCharPrefs( prevModPref_t* pModPrefs )
{
	if ( !pModPrefs ) return;
	
	PrefSetAppPreferences( modifierPrefCreator, modifierPrefID, modifierPrefVersionNum, pModPrefs, sizeof( prevModPref_t ), modifierPrefType );
	
	return;
	
} // WriteModifierCharPrefs()

/*
 * ReadModifierCharPrefs()
 */
void ReadModifierCharPrefs( prevModPref_t* pModPrefs )
{
	UInt16 			prefSize = sizeof( prevModPref_t );
	UInt16			prefsVersion = 0;

	if ( !pModPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( modifierPrefCreator, modifierPrefID, pModPrefs, &prefSize, modifierPrefType );
	
	if ( prefsVersion != modifierPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( modifierPrefCreator, modifierPrefID, prefsVersion, NULL, 0, modifierPrefType );				
	
		InitializeModifierCharPrefs( pModPrefs );
		
		WriteModifierCharPrefs( pModPrefs );
	}
	
	return;
	
} // ReadModifierCharPrefs()

/*
 * InitializeModifierCharPrefs()
 */
void InitializeModifierCharPrefs( prevModPref_t* pModPrefs )
{
	if ( !pModPrefs ) return;
	
	MemSet( pModPrefs, sizeof( prevModPref_t ), 0 );
	
	return;

} // InitializeModifierCharPrefs()

/*
 * Prefs.c
 */