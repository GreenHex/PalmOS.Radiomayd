/*
 * AttnAlerts.h
 */
 
#ifndef __ATTNALERTS_H__
#define __ATTNALERTS_H__

#include <Hs.h>
#include <AlarmMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "Utils.h"
#include "Alarms.h"
#include "../../Centroid/src/Bitmaps.h"
// #include "../../TreoFlex2/src/Utils.h"

#define STRLEN_ALERT_NOTE		32

typedef enum {
	attnRadioOFF		= 0,
	attnRadioON			= 1
} attnType_e;

// Prototypes
extern void 			DoAlert( attnType_e attnType, AttnLevelType level );
extern void 			ProcessAttention( AttnLaunchCodeArgsType* paramP );
extern void 			CancelAlerts( void );

/* AttnLevelType
		kAttnLevelInsistent 
		kAttnLevelSubtle 
*/

/*
typedef enum SndSysBeepTag {
  sndInfo = 1,
  sndWarning,
  sndError,
  sndStartUp,
  sndAlarm,
  sndConfirmation,
  sndClick
}
*/

#endif /* __ATTNALERTS_H__ */

/*
 * AttnAlerts.h
 */