/*
 * Prefs.h
 */
 
#ifndef __TP_PREFS_H__
#define __TP_PREFS_H__

#include <Hs.h>
#include <Preferences.h>

#include "Global.h"

typedef enum {
	prefUnsaved							= false,
	prefSaved							= true
} prefType_e;

#define appPrefCreator					appFileCreator
#define appPrefID						0x00
#define appPrefVersionNum				0x02
#define appPrefType						prefSaved						
//
#define chPrefCreator					appFileCreator
#define chPrefID						appPrefID
#define chPrefVersionNum				0x01
#define chPrefType						prefUnsaved
//
#define uptimePrefCreator				appFileCreator
#define uptimePrefID					appPrefID + 0x01
#define uptimePrefVersionNum			0x01
#define uptimePrefType					prefUnsaved
//
#define modifierPrefCreator				appFileCreator
#define modifierPrefID					appPrefID + 0x02
#define modifierPrefVersionNum			0x01
#define modifierPrefType				prefUnsaved

/*
 * Saved Preferences
 */
typedef struct {
	Boolean								bEnabled;
	Boolean								bSwitchOnRadio;
	UInt16								activationKeyIdx;
	UInt16								activationModifierKeyIdx;
	UInt16								timeoutIdx;
	Boolean								bShowPopup;
	Boolean								bTrapManualPhnOnOff;
} appPrefs_t;

/*
 * Unsaved Preferences
 */
typedef struct {
	Boolean								bMayDay;
	UInt16								numberDispatches; // number of times messages were dispatched
} chPrefs_t;

typedef struct {
	UInt32								secondsAtReset;
} uptimePrefs_t;

typedef struct {
	UInt16								prevModifier;
} prevModPref_t;

// Prototypes
extern void 				WriteAppPrefs( appPrefs_t* pPrefs );
extern void 				ReadAppPrefs( appPrefs_t* pPrefs );
extern void					WriteChPrefs( chPrefs_t* pChPrefs );
extern void					ReadChPrefs( chPrefs_t* pChPrefs );
extern void 				InitializeChPrefs( chPrefs_t* pChPrefs );
extern void 				WriteUptimePrefs( void );
extern UInt32 				ReadUptimePrefs( void );
extern void 				WriteModifierCharPrefs( prevModPref_t* pModPrefs );
extern void 				ReadModifierCharPrefs( prevModPref_t* pModPrefs );
extern void 				InitializeModifierCharPrefs( prevModPref_t* pModPrefs );

#endif /* __TP_PREFS_H__ */
 
/*
 * Prefs.h
 */