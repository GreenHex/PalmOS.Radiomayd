/*
 * PhnSecDB.c
 */

#include "PhnSecDB.h"

// Prototypes
static Err 			OpenPhnSecDB( DmOpenRef* dbPP, UInt16* numRecordsP, UInt16 dbOpenMode );
static Err 			PhnSecDBInitializeAppInfoBlock( DmOpenRef dbP,  UInt16 cardNo, LocalID dbID, LocalID* pAppInfoID );
static void 		PhnSecDBInitializeAppInfoPrefs( phnSecInfoBlock_t* pPhnSecInfo );

/*
 * OpenPhnSecDB()
 */
static Err OpenPhnSecDB( DmOpenRef* dbPP, UInt16* numRecordsP, UInt16 dbOpenMode )
{
	Err					error = errNone;
	
	error = DBOpen( dbPP, PhnSecDBName, PhnSecDBType, PhnSecDBCreator, dbOpenMode, PhnSecDBVersion, sizeof( phnNum_t ) );

	if ( ( error == errNone ) && ( *dbPP ) && ( numRecordsP ) )
	{
		*numRecordsP = DmNumRecordsInCategory( *dbPP, dmAllCategories );
	}
	
	return ( error );
	
} // OpenPhnSecDB()

/*
 * PhnSecDBGetNumberOfRecords()
 */
UInt16 PhnSecDBGetNumberOfRecords( void )
{
	Err 			error = errNone;
	UInt16			retVal = 0;
	DmOpenRef 		dbP = NULL;
	
	error = OpenPhnSecDB( &dbP, &retVal, dmModeReadOnly );
	
    if ( dbP )
    {	
    	retVal = DmNumRecordsInCategory( dbP, dmAllCategories );
		DmCloseDatabase( dbP );
    }
    
	return ( retVal );
	
} // PhnSecDBGetNumberOfRecords()

/*
 * PhnSecDBSaveNewRecord()
 */
Err PhnSecDBSaveNewRecord( phnNum_t* pPhnNum, Boolean isSecret, UInt16* idxP )
{
	Err 			error = errNone;
	DmOpenRef 		dbP = NULL;
	
	error = OpenPhnSecDB( &dbP, NULL, dmModeReadWrite );
	
	if ( ( error == errNone) && ( dbP ) )
	{
		MemHandle		recordH = NULL;
		phnNum_t*		recordP = NULL;
		UInt16			idx = dmMaxRecordIndex;
		UInt16			recordAttributes = 0;		
		
		recordH = DmNewRecord( dbP, &idx, sizeof( phnNum_t ) );
		if (recordH)
		{
			recordP = MemHandleLock( recordH );
			
			if ( recordP )
			{
				EncBuf( (UInt8 *) pPhnNum, sizeof( phnNum_t ), encrypt );
				error = DmWrite( recordP, 0, pPhnNum, sizeof( phnNum_t ) );
				EncBuf( (UInt8 *) pPhnNum, sizeof( phnNum_t ), decrypt );
			}
			
			MemHandleUnlock( recordH );
			recordP = NULL;
		
			DmRecordInfo( dbP, idx, &recordAttributes, NULL, NULL );
			if (isSecret)
			{
				recordAttributes |= dmRecAttrSecret;
			}
			else
			{
				recordAttributes &= ~dmRecAttrSecret;
			}
			error = DmSetRecordInfo( dbP, idx, &recordAttributes, NULL );
			
			error = ReleaseRecord( &dbP, &idx );
			
			if ( idxP )
			{
				*idxP = idx;
			}
		}		
    	
    	// DmQuickSort( dbP, (DmComparF *) &PhnSecDB_SortNameF, 0 );
	    		
		DmCloseDatabase( dbP );
	}
		
	return ( error );
	
} // PhnSecDBSaveNewRecord()

/*
 * PhnSecDBReplaceRecordAtIdx()
 */
Err PhnSecDBReplaceRecordAtIdx( UInt16* idxP, phnNum_t* pPhnNum, Boolean isSecret )
{
	Err				error = errNone;
	DmOpenRef 		dbP = NULL;
	MemHandle		recordH;
	phnNum_t*		recordP;
	
	error = OpenPhnSecDB( &dbP, NULL, dmModeReadWrite );
	
	if ( ( error == errNone) && ( dbP ) )
	{
		recordH = DmGetRecord( dbP, *idxP );
		
		if ( recordH )
		{
			recordP = (phnNum_t *) MemHandleLock( recordH );
			if ( recordP )
			{
				EncBuf( (UInt8 *) pPhnNum, sizeof( phnNum_t ), encrypt );	
				error = DmWrite( recordP, 0, pPhnNum, sizeof( phnNum_t ) );
				EncBuf( (UInt8 *) pPhnNum, sizeof( phnNum_t ), decrypt );
			}
				
			MemHandleUnlock( recordH );			
			recordP = NULL;
		}
		
		error = ReleaseRecord( &dbP, idxP );		
		
		// DmQuickSort( dbP, (DmComparF *) &PhnSecDB_SortNameF, 0 );
	    
		DmCloseDatabase( dbP );
	}
	
	return ( error );

} // PhnSecDBReplaceRecordAtIdx()

/*
 * PhnSecDBRemoveRecordAtIdx()
 */
Err PhnSecDBRemoveRecordAtIdx( UInt16* idxP )
{
	Err				error = errNone;
	DmOpenRef 		dbP = NULL;
	UInt16			numRecords = dmMaxRecordIndex;

	error = OpenPhnSecDB( &dbP, &numRecords, dmModeReadWrite );
	
	if ( ( error == errNone ) && ( dbP ) )
	{
		if ( numRecords > 0 )
		{
			if ( ( error = DmRemoveRecord( dbP, *idxP ) ) == errNone )
			{	
				*idxP = ( ( *idxP ) > 0 ) ? --( *idxP ) : 0;				
			}
		}
		
		DmCloseDatabase( dbP );
	}
	
	return ( error );
	
} // PhnSecDBRemoveRecordAtIdx()

/*
 * PhnSecDBGetRecordAtIdx
 */
Err PhnSecDBGetRecordAtIdx( UInt16 idx, phnNum_t* pPhnNum )
{
	Err				error = errNone;
	DmOpenRef 		dbP = NULL;
	UInt16			numRecords = dmMaxRecordIndex;
	
	if ( !pPhnNum )
	{ 
		return ( error = dmErrInvalidParam  );
	}

	MemSet( pPhnNum, sizeof( phnNum_t ), 0 );

	error = OpenPhnSecDB( &dbP, &numRecords, dmModeReadOnly );
	
	if ( ( error == errNone ) && ( dbP ) )
	{
		MemHandle		recordH = NULL;
		phnNum_t*		recordP = NULL;
		
		if ( idx < numRecords )
		{
			recordH = DmQueryRecord( dbP, idx );
			
			if ( recordH )
			{		
				recordP = (phnNum_t *) MemHandleLock( recordH );

				if ( recordP )
				{
					MemMove( pPhnNum, recordP, sizeof( phnNum_t ) );
					EncBuf( (UInt8 *) pPhnNum, sizeof( phnNum_t ), decrypt );
				}
				
				MemHandleUnlock( recordH );
				recordP = NULL;
			}
			else
			{
				error =  DmGetLastErr();
			}
		}
		else
		{
			error = dmErrIndexOutOfRange;
		}
		
		DmCloseDatabase( dbP );
	}
	
	return ( error );
	
} // PhnSecDBGetRecordAtIdx()

/*
 * PhnSecDBSetAppInfo()
 */
Err PhnSecDBSetAppInfo( phnSecInfoBlock_t* pPhnSecInfo )
{
	Err 					error = 0;
	UInt16 					cardNo = 0;
 	LocalID 				dbID = 0;
 	DmOpenRef				dbP; 
 	LocalID  				appInfoID;
	phnSecInfoBlock_t* 		pPhnSecInfoTmp = NULL;

	if ( !pPhnSecInfo ) return ( error );
	
	if ( ( error = OpenPhnSecDB( &dbP, NULL, dmModeReadWrite ) ) != errNone ) return ( error );
	
	error = DmOpenDatabaseInfo( dbP, &dbID, NULL, NULL, &cardNo, NULL );	

	if ( !error )
	{
		error  = DmDatabaseInfo( cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL );
			
		if ( !error )
		{
			if ( !appInfoID )
			{		
				error = PhnSecDBInitializeAppInfoBlock( dbP, cardNo, dbID, &appInfoID );
			}
		
			pPhnSecInfoTmp = (phnSecInfoBlock_t *)MemLocalIDToLockedPtr( appInfoID, cardNo );
			EncBuf( (UInt8 *) pPhnSecInfo, sizeof( phnSecInfoBlock_t ), encrypt );				
			DmWrite( pPhnSecInfoTmp, 0, pPhnSecInfo, sizeof( phnSecInfoBlock_t ) );
			EncBuf( (UInt8 *) pPhnSecInfo, sizeof( phnSecInfoBlock_t ), decrypt );
			MemPtrUnlock( pPhnSecInfoTmp );	

		}
	}

	DmCloseDatabase( dbP );
		
	return ( error );
	
} // PhnSecDBSetAppInfo()

/*
 * PhnSecDBGetAppInfo()
 */
Err PhnSecDBGetAppInfo( phnSecInfoBlock_t* pPhnSecInfo )
{
	Err 					error = dmErrInvalidParam;
	UInt16 					cardNo = 0;
 	LocalID 				dbID = 0;
 	DmOpenRef				dbP; 
 	LocalID  				appInfoID;
	phnSecInfoBlock_t*		pPhnSecInfoTmp = NULL;
	
	if ( ( error = OpenPhnSecDB( &dbP, NULL, dmModeReadWrite ) ) != errNone ) return ( error );
	
	error = DmOpenDatabaseInfo( dbP, &dbID, NULL, NULL, &cardNo, NULL );	

	if ( !error )
	{
		error  = DmDatabaseInfo( cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL );
			
		if ( !error )
		{
			if ( !appInfoID )
			{		
				error = PhnSecDBInitializeAppInfoBlock( dbP, cardNo, dbID, &appInfoID );
			}

			if ( pPhnSecInfo )
			{
				pPhnSecInfoTmp = (phnSecInfoBlock_t *)MemLocalIDToLockedPtr( appInfoID, cardNo );
				
				MemMove( pPhnSecInfo, pPhnSecInfoTmp, sizeof( phnSecInfoBlock_t ) );
				EncBuf( (UInt8 *) pPhnSecInfo, sizeof( phnSecInfoBlock_t ), decrypt );
			}
			
			MemPtrUnlock( pPhnSecInfoTmp );	
		}
	}
	
	DmCloseDatabase( dbP );
		
	return ( error );
	
} // PhnSecDBGetAppInfo()

/*
 * PhnSecDBInitializeAppInfoBlock()
 */
static Err PhnSecDBInitializeAppInfoBlock( DmOpenRef dbP,  UInt16 cardNo, LocalID dbID, LocalID* pAppInfoID )
{
	Err 					error = dmErrInvalidParam;
	MemHandle				memH = 0;
	phnSecInfoBlock_t 		PhnSecInfo;
	phnSecInfoBlock_t* 		pPhnSecInfoTmp = NULL;

	if ( !( *pAppInfoID ) )
	{		
		memH = DmNewHandle( dbP, sizeof( phnSecInfoBlock_t ) );
		
		if ( !memH ) return ( dmErrMemError );
		
		*pAppInfoID = MemHandleToLocalID( memH );
		
		error = DmSetDatabaseInfo( cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pAppInfoID, NULL, NULL, NULL );	
	}	
	
	pPhnSecInfoTmp = MemLocalIDToLockedPtr( *pAppInfoID, cardNo );
	
	// DmSet( pPhnSecInfoTmp, 0, sizeof( phnSecInfoBlock_t ), 0 );
	
	PhnSecDBInitializeAppInfoPrefs( &PhnSecInfo );
	EncBuf( (UInt8 *) &PhnSecInfo, sizeof( phnSecInfoBlock_t ), encrypt );
	DmWrite( pPhnSecInfoTmp, 0, &PhnSecInfo, sizeof( phnSecInfoBlock_t ) );
	EncBuf( (UInt8 *) &PhnSecInfo, sizeof( phnSecInfoBlock_t ), decrypt );

	MemPtrUnlock( pPhnSecInfoTmp );
				

	return ( error );
	
} // PhnSecDBInitializeAppInfoBlock()

/*
 * PhnSecDBInitializeAppInfoPrefs( )
 */
static void PhnSecDBInitializeAppInfoPrefs( phnSecInfoBlock_t* pPhnSecInfo )
{
	if ( !pPhnSecInfo ) return;
	
	pPhnSecInfo->bMayDay = false;
	pPhnSecInfo->numberDispatches = MSG_NUM_ATTEMPTS;
	pPhnSecInfo->bEnabled = true;
	pPhnSecInfo->bSwitchOnRadio = true;
	pPhnSecInfo->IMSI[0] = chrNull;
	StrCopy( pPhnSecInfo->LockMsg, STR_LOCK_MSG );
	StrCopy( pPhnSecInfo->LockPwd, STR_LOCK_PWD );
	StrCopy( pPhnSecInfo->CallFwdNumber, STR_CALL_FWD );
	pPhnSecInfo->messagingDelayIdx = MSG_DELAY_IDX;
	pPhnSecInfo->messagingIntervalIdx =  MSG_INTERVAL_IDX;
	pPhnSecInfo->numberMessagingAttempts = MSG_NUM_ATTEMPTS;
	
	return;

}
/*
 * InitializePhnSecList()
 */
UInt16 InitializePhnSecList( ListType* pList )
{
	Err				error = errNone;
	DmOpenRef		dbP = NULL;
	UInt16			numRecords = 0;

	if ( pList )
	{
		error = OpenPhnSecDB( &dbP, &numRecords, dmModeReadOnly );
	
		if ( dbP )	
		{
			DmCloseDatabase( dbP );	
			
		    LstSetListChoices( pList, NULL, numRecords );
		}
	}
	
	return ( numRecords );
	
}  // InitializePhnSecList

/*
 * DrawPhnSecListItem()
 */
void DrawPhnSecListItem( Int16 itemNum, RectangleType* pRect, Char **itemsText )
{
	Err						error = errNone;
	DmOpenRef				dbP;
	MemHandle				recordH = NULL;
	phnNum_t*				recordP = NULL;
	phnNum_t				phnNum;
	UInt16					numRecords = dmMaxRecordIndex;
   	
	error = OpenPhnSecDB( &dbP, &numRecords, dmModeReadOnly );

	if ( dbP )	
	{
		if ( itemNum < numRecords )
		{
			recordH = DmQueryRecord( dbP, itemNum );
			if ( recordH )
			{	
				recordP = (phnNum_t *) MemHandleLock( recordH );
				
				if ( recordP )
				{
					MemMove( &phnNum, recordP, sizeof ( phnNum_t ) );
					EncBuf( (UInt8 *) &phnNum, sizeof( phnNum_t ), decrypt );
					
					if ( StrLen( phnNum.Name ) )
					{
						WinDrawTruncChars( phnNum.Name, StrLen( phnNum.Name ), 
										pRect->topLeft.x,
										pRect->topLeft.y,
										pRect->extent.x );
					}
					else
					{
						WinDrawTruncChars( phnNum.Number, StrLen( phnNum.Number ), 
										pRect->topLeft.x,
										pRect->topLeft.y,
										pRect->extent.x );										
					}
				}
				MemHandleUnlock( recordH );
			}
		}
		
		DmCloseDatabase( dbP );	
		
	}
	
	return;
	
} // DrawPhnSecListItem()

/*
 * PhnSecDB.c
 */