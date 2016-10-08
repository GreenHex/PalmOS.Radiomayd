/*
 * Utils.h
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>	
#include <Form.h>
#include <StringMgr.h>
#include <PmSysGadgetLib.h>
#include <PmSysGadgetLibCommon.h>

#include "Global.h"
#include "Prefs.h"
#include "AppResources.h"

#define MIN_VERSION  					sysMakeROMVersion( 5, 0, 0, sysROMStageRelease, 0 )
#define LAUNCH_FLAGS 					( sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp )

typedef enum {
	colorDefault		= 0,
	colorBlue 			= 0,
	colorRed			= 1,
	colorGreen 			= 2,
	colorGrey			= 3,
	colorBlack			= 4,
	colorWhite			= 5
} color_e;

typedef enum {
	hAlignLeft			= -1,
	hAlignCenter		= 0,
	hAlignRight			= 1
} hAlign_e;

typedef enum {
	vAlignTop			= -1,
	vAlignMiddle		= 0,
	vAlignBottom		= 1
} vAlign_e;

// Prototypes
extern void 					DrawStr( Char* str, RectangleType* rectP, FontID fontID, hAlign_e hAlign, vAlign_e vAlign, color_e color, Boolean draw );
extern void 					MakeTimeInterval( UInt32 SecsToGo, Char* strDuration );
extern void 					MakeTimeDateStr2( UInt32 timeSecs, Char* strTimeDateP );
extern void 					MakeTimeDateStr( UInt32 timeSecs, Char** strTimeDatePP, Char* strTimeP, Char* strDateP, Boolean bRemoveYear );
extern void 					ShiftControl( FormType* pForm, UInt16 ctlID, Coord shiftX, Coord shiftY );
extern Char* 					MakeNum( unsigned char* s );
extern Char* 					ReplaceChar(unsigned char *s, char orgChr, char newChr);
extern Char* 					StripSpaces(unsigned char *s);
extern Err 						RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags );
extern Boolean 					AboutFormHandleEvent( EventType* pEvent );
extern Boolean 					EvtCharEqualsLaunchChar( WChar evt_wChar, UInt16 evt_keyCode, UInt16 evt_modifier, UInt16 keyCharIdx, UInt16 keyModifierIdx );

/*
enum fontID {
  stdFont = 0x00, 
  boldFont, 
  largeFont, 
  symbolFont, 
  symbol11Font, 
  symbol7Font, 
  ledFont, 
  largeBoldFont, 
  fntAppCustomBase = 0x80
};
*/

/*
typedef enum privateRecordViewEnum {
  showPrivateRecords = 0x00,
  maskPrivateRecords,
  hidePrivateRecords
} privateRecordViewEnum;
*/

#endif /* __UTILS_H__ */

/*
 * Utils.h
 */ 