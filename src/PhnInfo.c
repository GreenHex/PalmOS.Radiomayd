/*
 * PhnInfo.c
 */

#include "PhnInfo.h"


/*
typedef struct {
	Char 					IMEI[STRLEN_IMEI];
	Char 					Firmware[STRLEN_MISC];
	Boolean					isRoaming;
	Char					GID[STRLEN_GID];
	Char 					IMSI[STRLEN_IMSI];
	Char 					PhnNumber[STRLEN_PHN_NUMBER];
	UInt32					uCurrentOperatorID;
	Char					CurrentOperatorName[STRLEN_OPERATOR];
	Char 					CurrentOperatorID[STRLEN_MCC_MNC]; // MCC + MNC
	Char					HomeOperatorID[STRLEN_MCC_MNC]; // MCC + MNC
	Char					CurrentProvider[STRLEN_OPERATOR];
	Char					BatteryLevel[STRLEN_BATT_LEVEL];
} phnInfo_t;
*/

// Prototypes
static Err 					GetPhnInfo( phnInfo_t* pPhnInfo );
static void 				PrvCopyStr( Char* dest, Char* source, Int16 bufSize );
static Boolean 				IsDeviceCentro( void );

/*
 * MakeMessage()
 */
void MakeMessage( Char* msgP )
{
	phnInfo_t			phnInfo;

	MemSet( &phnInfo, sizeof( phnInfo_t ), 0 );
	
	if ( !GetPhnInfo( &phnInfo ) )
	{
		StrCopy( msgP, "IMSI: " ); 										// 6
		StrCat( msgP, phnInfo.IMSI ); 									// 16
		StrCat( msgP, "\nPh#: " );										// 6
		StrCat( msgP, phnInfo.PhnNumber );								// 16
		//
		StrCat( msgP, "\nCurOp: " );									// 8
		StrCat( msgP, phnInfo.CurrentOperatorName );					// 16
		StrCat( msgP, ( phnInfo.isRoaming ) ? "\nRoam: 1" : "\nRoam: 0" ); 	// 8
		StrCat( msgP, "\nGID: " );										// 6
		StrCat( msgP, phnInfo.GID );									// 16
		// StrCat( msgP, "\nCurOpID: " );								  // 10
		// StrCat( msgP, phnInfo.CurrentOperatorID );					  // 16 ???
		StrCat( msgP, "\nHomeOpID: " );									// 11
		StrCat( msgP, phnInfo.HomeOperatorID );							// 16 ???
		//
		StrCat( msgP, "\nIMEI: " );										// 7
		StrCat( msgP, phnInfo.IMEI );									// 16
		// StrCat( msgP, "\nFW: " );									  // 5
		// StrCat( msgP, phnInfo.Firmware );							  // 12
		StrCat( msgP, "\nBatt: " );										// 8
		StrCat( msgP, phnInfo.BatteryLevel );							// 3
																		// 158 Total
	}
	else
	{
		StrCopy( msgP, STR_DEFAULT_MESSAGE );
	}

	return;
	
} // MakeMessage()

/*
 * GetIMSI()
 */
Err GetIMSI( Char* strIMSI, UInt16 strlenIMSI )
{
	Err			error = errNone;
	UInt16 		phnLibRefNum = sysInvalidRefNum;
	Char*		pIMSI = NULL;
	
	if ( ( !strIMSI ) || ( !strlenIMSI ) )
		return ( memErrNotEnoughSpace );
	
	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) != errNone)
  		return ( error );

	if ( ( error = PhnLibOpen( phnLibRefNum ) ) != errNone ) 
		return ( error );
	
	error = PhnLibSIMInfo( phnLibRefNum, &pIMSI );
	
	PhnLibClose( phnLibRefNum );
	
	StrNCopy( strIMSI, pIMSI, strlenIMSI - 1 );
	strIMSI[strlenIMSI - 1] = chrNull;
	
	if ( pIMSI ) MemPtrFree( pIMSI );
	
	error = errNone;
		
	return ( error );
	
} // GetIMSI()

/*
 * GetPhnInfo()
 */
static Err GetPhnInfo( phnInfo_t* pPhnInfo )
{
	Err						error = errNone;
	UInt16 					phnLibRefNum = sysInvalidRefNum;
	//
	Char*					Firmware = NULL;
	Char*					IMEI = NULL;
	Char*					IMSI = NULL;
	//
	UInt32					uLAC = 0;
	UInt32					uCellID = 0;
	Char					strLAC[maxStrIToALen] = "\0";
	Char					strCellID[maxStrIToALen] = "\0";
	Char					strMCC[maxStrIToALen] = "\0";
	Char					strMNC[maxStrIToALen] = "\0";
	//	
  	PhnOperatorID			uCurrentOperatorID = 0;
  	Char*					CurrentOperatorName = NULL;
  	Char					CurrentOperatorID[STRLEN_MCC_MNC] = "\0";
  	Int16					strlenCurrentOperatorID = STRLEN_MCC_MNC;
  	Char					HomeOperatorID[STRLEN_MCC_MNC] = "\0";
  	Char*					CurrentProviderName = NULL;
  	Int16					strlenHomeOperatorID = STRLEN_MCC_MNC;
  	//
  	PhnAddressList  		AddList = NULL;
	PhnAddressHandle		Address = NULL;
	Char*					PhnNumber = NULL;
  	//
  	PhnRoamStatusEnum		roamingStatus = PhnRoamStatusBlink;
  	//
  	UInt8					BatteryPercent = 0;
  	Char					strBatteryPercent[maxStrIToALen] = "\0";
  	
  	if ( !pPhnInfo ) 
  		return ( memErrNotEnoughSpace );
  	
  	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) != errNone )
  		return ( error );

	if ( ( error = PhnLibOpen( phnLibRefNum ) ) != errNone ) 
		return ( error );
		
	if ( PhnLibModulePowered( phnLibRefNum ) != phnPowerOn )
		return ( 1 );
		
	if ( !PhnLibRegistered( phnLibRefNum ) )
		return ( 2 );
		
	error = PhnLibCardInfo( phnLibRefNum, 0, 0, &Firmware, &IMEI );
	error = PhnLibSIMInfo( phnLibRefNum, &IMSI );

	if ( IsDeviceCentro() )
	{
		error = PhnLibGetGSMCellInfo( phnLibRefNum, &uLAC, &uCellID );
	}
	//
	error = PhnLibCurrentOperator( phnLibRefNum, &uCurrentOperatorID, &CurrentOperatorName, NULL );
	error = PhnLibCurrentOperatorID( phnLibRefNum, CurrentOperatorID, &strlenCurrentOperatorID );
	error = PhnLibCurrentProvider( phnLibRefNum, &CurrentProviderName );
	error = PhnLibHomeOperatorID ( phnLibRefNum, HomeOperatorID, &strlenHomeOperatorID );
	//
	error = PhnLibGetOwnNumbers( phnLibRefNum, &AddList );
	if ( ( !error ) && ( AddList ) )
	{
		error = PhnLibGetNth( phnLibRefNum, AddList, 1, &Address );
		if ( ( !error ) && ( Address ) )
		{
			PhnNumber = PhnLibGetField( phnLibRefNum, Address, phnAddrFldPhone );
			if ( PhnNumber )
			{					
				PrvCopyStr( pPhnInfo->PhnNumber, PhnNumber, (Int16) STRLEN_PHN_NUMBER );
			}
			MemHandleFree( Address );
		}					
		MemHandleFree( AddList );
	}
	error = PhnLibRoaming( phnLibRefNum, &roamingStatus );

	PhnLibClose( phnLibRefNum );
	
	// Copy to pPhnInfo
	PrvCopyStr( pPhnInfo->IMEI, IMEI, (Int16) STRLEN_IMEI );
	PrvCopyStr( pPhnInfo->Firmware, Firmware, (Int16) STRLEN_MISC );
	pPhnInfo->isRoaming = ( roamingStatus == PhnRoamStatusOn );
	//
	MemMove( strMCC, CurrentOperatorID, 3 );
	strMCC[3] = chrNull;
	MemMove( strMNC, CurrentOperatorID + 3, 3 );
	strMNC[2] = chrNull;
	StrIToH( strLAC, uLAC );
	StrIToH( strCellID, uCellID );
	MemMove( strLAC, strLAC + 4, 5 );
	MemMove( strCellID, strCellID + 4, 5 );
	StrCopy( pPhnInfo->GID, strMCC );
	StrCat( pPhnInfo->GID, ":" );
	StrCat( pPhnInfo->GID, strMNC );
	StrCat( pPhnInfo->GID, ":" );
	StrCat( pPhnInfo->GID, strLAC );
	StrCat( pPhnInfo->GID, ":" );
	StrCat( pPhnInfo->GID, strCellID );
	//
	PrvCopyStr( pPhnInfo->IMSI, IMSI, (Int16) STRLEN_IMSI );
	pPhnInfo->uCurrentOperatorID = uCurrentOperatorID;
	PrvCopyStr( pPhnInfo->CurrentOperatorName, CurrentOperatorName, (Int16) STRLEN_OPERATOR );
	PrvCopyStr( pPhnInfo->CurrentOperatorID, CurrentOperatorID, (Int16) STRLEN_MCC_MNC );
	PrvCopyStr( pPhnInfo->HomeOperatorID, HomeOperatorID, (Int16) STRLEN_MCC_MNC );
	PrvCopyStr( pPhnInfo->CurrentProviderName, CurrentProviderName, STRLEN_OPERATOR );
	BatteryPercent = GetBatteryState( NULL );
	StrIToA( strBatteryPercent, (Int32) BatteryPercent );
	PrvCopyStr( pPhnInfo->BatteryLevel, strBatteryPercent, (Int16) STRLEN_BATT_LEVEL );
	StrCat( pPhnInfo->BatteryLevel, "%" );
	
	// Free Memory
	if ( Firmware ) MemPtrFree( Firmware );
	if ( IMEI ) MemPtrFree( IMEI );
	if ( IMSI ) MemPtrFree( IMSI );
	if ( CurrentOperatorName ) MemPtrFree( CurrentOperatorName );
  	if ( CurrentProviderName ) MemPtrFree( CurrentProviderName );

	error = errNone;
		
	return ( error );

} // GetPhnInfo()

/*
 * PrvCopyStr()
 */
static void PrvCopyStr( Char* dest, Char* source, Int16 bufSize )
{
	if ( ( !dest ) || ( !source ) || ( !bufSize ) ) return;
	
	StrNCopy( dest, source, bufSize - 1 );
	dest[bufSize - 1] = chrNull;
	
	return;
	
} // PrvCopyStr()

/*
 * IsSIMReady()
 */
Boolean IsSIMReady( void )
{
	Boolean			retVal = false;
	UInt16 			phnLibRefNum = sysInvalidRefNum;
	
	if ( HsGetPhoneLibrary( &phnLibRefNum ) )
		return ( retVal );
					
	if ( PhnLibOpen( phnLibRefNum ) )
		return ( retVal );
		
	if ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOn )
	{
		retVal = ( PhnLibGetSIMStatus( phnLibRefNum ) == simReady );
	}	
	
	PhnLibClose( phnLibRefNum );
	
	return ( retVal );

} // IsSIMReady()

/* 
 * GetPhonePowerState()
 */
PhnPowerType GetPhonePowerState( void )
{
	PhnPowerType		retVal = phnPowerOff;
	UInt16 				phnLibRefNum = sysInvalidRefNum;
	
	if ( HsGetPhoneLibrary( &phnLibRefNum ) )
		return ( retVal );
					
	if ( PhnLibOpen( phnLibRefNum ) )
		return ( retVal );
		
	retVal = PhnLibModulePowered( phnLibRefNum );
		
	PhnLibClose( phnLibRefNum );
	
	return ( retVal );
	
} // GetPhonePowerState()

/* 
 * IsPhoneReady()
 */
Boolean IsPhoneReady( void )
{
	Boolean			retVal = false;
	UInt16 			phnLibRefNum = sysInvalidRefNum;
	
	if ( HsGetPhoneLibrary( &phnLibRefNum ) )
		return ( retVal );
					
	if ( PhnLibOpen( phnLibRefNum ) )
		return ( retVal );
		
	if ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOn )
	{
		retVal = PhnLibRegistered( phnLibRefNum );
	}
		
	PhnLibClose( phnLibRefNum );
	
	return ( retVal );
	
} // IsPhoneReady()

/*
 * IsPhoneGSM()
 */
Boolean IsPhoneGSM( void )
{
  	UInt32 phnType = hsAttrPhoneTypeGSM; 
  
  	HsAttrGet( hsAttrPhoneType, 0, &phnType );
  	
  	return ( phnType == hsAttrPhoneTypeGSM );

} // IsPhoneGSM()

/*
 * IsDeviceCentro()
 */
static Boolean IsDeviceCentro( void )
{
	UInt32	device = 0;
	UInt32 	hal = 0;
	
	FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &device );
	FtrGet( sysFtrCreator, sysFtrNumOEMHALID, &hal );

	return ( ( device == 'D061' ) && ( hal == 'P053' ) );
			
} // IsDeviceCentro()

/*
 * GetBatteryState()
 */
UInt8 GetBatteryState( Boolean* bPluggedinP )
{
   	UInt8 		percent = 0;
   
	SysBatteryInfo ( false, NULL, NULL, NULL, NULL, bPluggedinP, &percent );

	return ( percent );
	
} // GetBatteryState()

/*
 * SetRadioState()
 */
void SetRadioState( Boolean bSwitchOn )
{
	UInt16				phnLibRefNum = sysInvalidRefNum;
	
	if ( HsGetPhoneLibrary( &phnLibRefNum ) )
		return;

	if ( PhnLibOpen( phnLibRefNum ) )
		return;

	if ( bSwitchOn ) 
	{
		if ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOff )
		{
			PhnLibSetModulePower( phnLibRefNum, true );
		}
	}
	else
	{
		if ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOn )
		{
			PhnLibSetModulePower( phnLibRefNum, false );
		}
	}
	
	PhnLibClose( phnLibRefNum );

	return;

} // SetRadioState()

/*
 * PhnInfo.c
 */