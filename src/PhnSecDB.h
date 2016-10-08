/*
 * PhnSecDB.h
 */
 
#ifndef __PHNSECDB_H__
#define __PHNSECDB_H__

#include <Hs.h>
#include <PalmOS.h>
#include <DataMgr.h>
#include <List.h>

#include "Global.h"
#include "CryptUtils.h"
#include "../../Centroid/Src/DBUtils.h"

#define PhnSecDBCreator				'phnS'
#define PhnSecDBName				"PhnSecDB"
#define PhnSecDBType				'DSEC'
#define PhnSecDBVersion				0x01

#define STR_LOCK_MSG				"lock-my-phone"
#define STR_LOCK_PWD				"0123456789"
#define STR_CALL_FWD				""
#define MSG_DELAY_IDX 				3
#define MSG_INTERVAL_IDX 			3
#define MSG_NUM_ATTEMPTS			5

typedef struct {
	Boolean							bMayDay;					// 1 
	UInt16							numberDispatches;			// 2
	Boolean							bEnabled;					// 1
	Boolean							bSwitchOnRadio;				// 1
	Char							IMSI[STRLEN_IMSI];			// 24
	Char							LockPwd[STRLEN_LOCK_PWD];	// 32 + 1
	Char							LockMsg[STRLEN_LOCK_MSG];	// 16 + 1
	Char							CallFwdNumber[STRLEN_PHN_NUMBER]; // 16 + 1
	UInt16							messagingDelayIdx; 			// 2 // between two messages within list
	UInt16							messagingIntervalIdx; 		// 2 // between attempts
	UInt16							numberMessagingAttempts;	// 2 = 102
	UInt8							padding[7];					// 10
																// = 112 = 14 * 8
} phnSecInfoBlock_t;

typedef struct {
	Char							Number[STRLEN_PHN_NUMBER]; 	// 16 + 1
	Char							Name[STRLEN_CALLER_ID]; 	// 32
	UInt8							padding[7];					// 7
																// = 56
} phnNum_t;

// Prototypes
extern UInt16 				PhnSecDBGetNumberOfRecords( void );
extern Err 					PhnSecDBSaveNewRecord( phnNum_t* pPhnNum, Boolean isSecret, UInt16* idxP );
extern Err 					PhnSecDBReplaceRecordAtIdx( UInt16* idxP, phnNum_t* pPhnNum, Boolean isSecret );
extern Err 					PhnSecDBRemoveRecordAtIdx( UInt16* idxP );
extern Err 					PhnSecDBGetRecordAtIdx( UInt16 idx, phnNum_t* pPhnNum );
// extern Err 					PhnSecDBFindRecord( phnNum_t* pPhnNum, UInt16* idxP, Boolean bLoadRecord, Boolean* bFound, DmComparF* comparFP );
// extern Int16 				PhnSecDB_SortNameF( phnNum_t* recP1, phnNum_t* recP2, Int16 other, SortRecordInfoPtr rec1SortInfo, SortRecordInfoPtr rec2SortInfo, MemHandle appInfoH );
extern UInt16				InitializePhnSecList( ListType* pList );
extern void 				DrawPhnSecListItem( Int16 itemNum, RectangleType* pRect, Char **itemsText );
extern Err 					PhnSecDBSetAppInfo( phnSecInfoBlock_t* pPhnSecInfo );
extern Err 					PhnSecDBGetAppInfo( phnSecInfoBlock_t* pPhnSecInfo );

#endif /* __PHNSECDB_H__ */

/*
 * PhnSecDB.h
 */