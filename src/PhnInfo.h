/*
 * PhnInfo.h
 */

#ifndef __PHNINFO_H__
#define __PHNINFO_H__

#include <Hs.h>
#include <HsPhone.h>
#include <HsPhoneTypes.h>
#include <StringMgr.h>

#include "Global.h"

#define STR_DEFAULT_MESSAGE 		"SIM card is changed to this number"
#define BUFSIZE_CARRIER_ID			8

/**
 *  Power state of the radio
 * 
 * enum _PhnPowerType {
 * 	phnPowerOff,
 * 	phnPowerOn,
 * 	phnPowerStartCharging,
 * 	phnPowerStopCharging,
 * 	phnPowerLow,
 * 	phnPowerSave
 * };
 * typedef UInt8 PhnPowerType;	
 */

typedef struct {
	UInt32					MCC;
	UInt32					MNC;
	UInt32					LAC;
	UInt32					CellID;
} GID_t;

typedef struct {
	Boolean					isRoaming;
	Char 					IMSI[STRLEN_IMSI];
	Char 					PhnNumber[STRLEN_PHN_NUMBER];	
	Char					GID[STRLEN_GID];
	PhnOperatorID			uCurrentOperatorID;
	Char 					CurrentOperatorID[STRLEN_MCC_MNC]; // MCC + MNC
	Char					CurrentOperatorName[STRLEN_OPERATOR];
	Char					CurrentProviderName[STRLEN_OPERATOR];
	Char					HomeOperatorID[STRLEN_MCC_MNC]; // MCC + MNC
	Char 					IMEI[STRLEN_IMEI];
	Char 					Firmware[STRLEN_MISC];
	Char					BatteryLevel[STRLEN_BATT_LEVEL];
} phnInfo_t;

// Prototypes
extern void 				MakeMessage( Char* msgP );
extern Err 					GetIMSI( Char* strIMSI, UInt16 strlenIMSI );
extern Boolean 				IsSIMReady( void );
extern PhnPowerType 		GetPhonePowerState( void );
extern Boolean 				IsPhoneReady( void );
extern Boolean 				IsPhoneGSM( void );
extern UInt8 				GetBatteryState( Boolean* bPluggedinP );
extern void 				SetRadioState( Boolean bSwitchOn );

#endif /* __PHNINFO_H__ */
 
/*
 * PhnInfo.h
 */