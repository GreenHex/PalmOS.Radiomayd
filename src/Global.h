/*
 * Global.h
 */
 
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Hs.h>
#include <HsPhone.h>

#define APP_NAME							"Radimayd"
#define APP_COPYRIGHT						"(c) 2006-08 MVK"
#define appFileCreator						'TrPr'
#define appVersionNum						0x01

#define STRLEN_CALLER_ID				32 
#define STRLEN_LOCK_MSG					16 + 1
#define STRLEN_LOCK_PWD					pwdLength + 1 // 32 + 1
#define STRLEN_MISC						32
#define STRLEN_CLIR						24 + 1
#define STRLEN_MSG						160 + 1
#define STRLEN_NAME_FLD					160 // hidden filed in AddNumber form
//
#define STRLEN_IMSI						24 // 15 + 1
#define STRLEN_IMEI						24 // 15 + 1
#define STRLEN_PHN_NUMBER				kMaxPhoneNumberLen + 1 // 16 + 1
#define STRLEN_MCC_MNC					24
#define STRLEN_GID						20 // 123:56:8901:2345 = 16
#define STRLEN_OPERATOR					MAX_SHORTOPERATORNAME_LENGTH + 1 // 16 + 1
#define STRLEN_BATT_LEVEL				8

// Globals
Boolean									gIsPrivate;
ListType*								gPhnNumListP;
UInt16									gPhnNumListIdx;

#endif /* __GLOBAL_H__ */

/*
 * Global.h
 */


