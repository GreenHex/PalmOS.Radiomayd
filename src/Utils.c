/*
 * Utils.c
 */
 
#include "Utils.h"

/*
 * DrawStr()
 */
void DrawStr( Char* str, RectangleType* rectP, FontID fontID, hAlign_e hAlign, vAlign_e vAlign, color_e color, Boolean draw )
{
	Coord 					posX = 0;
	Coord 					posY = 0;
	Int16					strHeight = 0;
	Int16					strWidth = 0;	
    RGBColorType 			rgb;
    RGBColorType 			rgbRed = { 0, 250, 0, 51 };
    RGBColorType 			rgbGreen = { 0, 0, 153, 0 };
    RGBColorType 			rgbBlue = { 0, 0, 51, 200 };
    RGBColorType 			rgbGrey = { 0, 187, 187, 187 };
    RGBColorType 			rgbBlack = { 0, 0, 0, 0 };
    RGBColorType 			rgbWhite = { 0, 255, 255, 255 };
    IndexedColorType		strColor;
	
	if ( ( !str ) || ( !rectP ) ) return;
		
	WinPushDrawState();
	
	WinEraseRectangle( rectP, 0 ); 
	
	if ( draw )
	{
		switch ( color )
		{
			case colorRed:
			
				rgb = rgbRed;
	
				break;
				
			case colorGreen:
	
				rgb = rgbGreen;
			
				break;
 
			case colorGrey:
			
				rgb = rgbGrey;
	
				break;
				
			case colorBlack:
			
				rgb = rgbBlack;
	
				break;
			
			case colorWhite:
			
				rgb = rgbWhite;
	
				break;
				
			default:
			
				rgb = rgbBlue;
				
				break;		
		}
		
		strColor = WinRGBToIndex( &rgb );	
	    WinSetTextColor( strColor );
	    
		FntSetFont( fontID );
		strHeight = FntLineHeight();
		// strWidth = FntLineWidth( str, StrLen( str ) );
		FntWidthToOffset( str, StrLen( str ), rectP->extent.x, NULL, &strWidth );
		
		switch ( hAlign )
		{
			case hAlignLeft:
				posX = rectP->topLeft.x;		
				break;
			case hAlignRight:
				posX = rectP->topLeft.x + ( rectP->extent.x - strWidth );
				break;
			default:
				posX = rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 );
				break;
		}
		
		switch ( vAlign )
		{
			case vAlignTop:
				posY = rectP->topLeft.y;
				break;
			case vAlignBottom:
				posY = rectP->topLeft.y + ( rectP->extent.y - strHeight );
				break;
			default:
				posY = rectP->topLeft.y + ( ( rectP->extent.y - strHeight ) / 2 );
				break;
		}
		
		// WinDrawChars( str, StrLen( str ), posX, posY );
		WinDrawTruncChars( str, StrLen( str ), posX, posY, rectP->extent.x );
	}
		    
	WinPopDrawState();
	
	return;
	
} // DrawStr()

/*
 * MakeTimeInterval()
 */
void MakeTimeInterval( UInt32 SecsToGo, Char* strDuration )
{
	UInt32					DaysToGo = 0;
	UInt32					HoursToGo = 0;
	UInt32					MinutesToGo = 0;
	
	if ( !strDuration ) return;
	
	DaysToGo = ( SecsToGo - ( SecsToGo % 86400 ) ) / 86400;				
	HoursToGo = ( SecsToGo - ( DaysToGo * 86400 ) ) / 3600;
	MinutesToGo = ( SecsToGo - ( DaysToGo * 86400 ) - ( HoursToGo * 3600 ) ) / 60;
				
	StrPrintF( strDuration, "%lud %luh %lum", DaysToGo, HoursToGo, MinutesToGo );
	
	return;	
	
} // MakeTimeInterval()

/*
 * MakeTimeDateStr2()
 */
void MakeTimeDateStr2( UInt32 timeSecs, Char* strTimeDateP )
{
	DateTimeType 			dt;
	DateType				d;
	TimeFormatType			tfmt = PrefGetPreference( prefTimeFormat );
	DateFormatType 			dfmt = PrefGetPreference( prefDateFormat );
	Char					strTime[timeStringLength] = "\0";
	Char					strDate[longDateStrLength] = "\0";
	
	// Time
	TimSecondsToDateTime( timeSecs, &dt );
	TimeToAscii( dt.hour, dt.minute, tfmt, strTime );
    ReplaceChar( strTime, 'm', chrNull );
	StripSpaces( strTime );

	// Date
	DateSecondsToDate( timeSecs, &d );
	DateToAscii( d.month, d.day, d.year + 1904, dfmt, strDate );
	
	if ( strTimeDateP )
	{	 
		StrCopy( strTimeDateP, strTime );
		StrCat( strTimeDateP, "  " );
		StrCat( strTimeDateP, strDate );
	}
		
	return;
		
} // MakeTimeDateStr2()

/*
 * MakeTimeDateStr()
 */
void MakeTimeDateStr( UInt32 timeSecs, Char** strTimeDatePP, Char* strTimeP, Char* strDateP, Boolean bRemoveYear )
{
	DateTimeType 			dt;
	DateType				d;
	TimeFormatType			tfmt = PrefGetPreference( prefTimeFormat );
	DateFormatType 			dfmt = PrefGetPreference( prefDateFormat );
	Char					strTime[timeStringLength] = "\0";
	Char					strDate[longDateStrLength] = "\0";
	
	// Time
	TimSecondsToDateTime( timeSecs, &dt );
	TimeToAscii( dt.hour, dt.minute, tfmt, strTime );
    // ReplaceChar( strTime, 'm', chrNull );
	// StripSpaces( strTime );
	
	if ( strTimeP )
	{
		StrCopy( strTimeP, strTime );	
	}

	// Date
	DateSecondsToDate( timeSecs, &d );
	DateToAscii( d.month, d.day, d.year + 1904, dfmt, strDate );

	if ( bRemoveYear )
	{
		if ( ( dfmt == dfYMDWithSlashes ) 
			|| ( dfmt == dfYMDWithDots ) 
			|| ( dfmt == dfYMDWithDashes ) )
		{
			MemMove( strDate, strDate + 3, StrLen( strDate ) );
		}
		else
		{
			strDate[StrLen(strDate) - 3] = chrNull;
		}		
	}
	
	if ( strDateP )
	{
		StrCopy( strDateP, strDate );	
	}
	
	if ( strTimeDatePP )
	{
		if ( !( *strTimeDatePP ) ) *strTimeDatePP = MemPtrNew( timeStringLength + longDateStrLength + 3 );
		
		if ( *strTimeDatePP )
		{
			StrCopy( *strTimeDatePP, strTime );
			StrCat( *strTimeDatePP, "  " );
			StrCat( *strTimeDatePP, strDate );
		}
	}
		
	return;
		
} // MakeTimeDateStr()

/*
 * ShiftControl()
 */
void ShiftControl( FormType* pForm, UInt16 ctlID, Coord shiftX, Coord shiftY )
{
	UInt16				ctlIdx = FrmGetObjectIndex( pForm, ctlID );
	RectangleType		rect;
	
	if ( ctlIdx == frmInvalidObjectId ) return;
	
	FrmGetObjectBounds( pForm, ctlIdx, &rect );
	rect.topLeft.x += shiftX;
	rect.topLeft.y += shiftY;
	FrmSetObjectBounds( pForm, ctlIdx, &rect );

	return;
	
} // ShiftControl()

/*
 * MakeNum
 */
Char* MakeNum( unsigned char* s )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
	{
		if ( TxtCharIsDigit( c ) )
		{
			*d++ = c;
		}
	}
					
	*d = 0;
	
	return ( s );
	
} // MakeNum

/*
 * ReplaceChar
 */
Char* ReplaceChar( unsigned char *s, char orgChr, char newChr )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
	{
		if ( c == orgChr )
			*d++ = newChr;
		else
			*d++ = c;
	}
					
	*d = 0;
	
	return ( s );
	
} // ReplaceChar

/*
 * StripSpace
 */
Char* StripSpaces( unsigned char *s )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
	{
		if ( c != chrSpace )
		{
			*d++ = c;
		}
	}
					
	*d = 0;
	
	return ( s );
	
} // StripSpaces

/*
 * RomVersionCompatible()
 */
Err RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags )
{	
	UInt32 	rom = 0;
	UInt32 	hal = 0;
	UInt32	company = 0;
	UInt32	device = 0;
	UInt32	hsFtrVersion;

	if( FtrGet( hsFtrCreator, hsFtrIDVersion, &hsFtrVersion ) != 0 )
	{
		return ( 1 );
	}
	
    FtrGet( sysFtrCreator, sysFtrNumOEMHALID, &hal );
    FtrGet( sysFtrCreator, sysFtrNumOEMCompanyID, &company );
    FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &device );
	FtrGet( sysFtrCreator, sysFtrNumROMVersion, &rom );
		
	if ( rom >= requiredVersion
		&&	( company == kPalmCompanyIDPalm
			|| company == kPalmCompanyIDOldPalm
			|| kPalmCompanyIDHandspring )

		&&	( device == kPalmOneDeviceIDTreo650
			|| device == kPalmOneDeviceIDTreo650Sim

			|| device == kPalmOneDeviceIDTreo600
			|| device == kPalmOneDeviceIDTreo600Sim

			|| device == 'D053' // Treo 680
			|| device == 'H104'

			|| device == 'D061' // Centro
			)
			
		&&	( hal == hsHALIDHandspringOs5Rev2 // Treo 650
			|| hal == hsHALIDHandspringOs5Rev2Sim

			|| hal == hsHALIDHandspringOs5Rev1 // Treo 600
			|| hal == hsHALIDHandspringOs5Rev1Sim

			|| hal == hsHALIDHandspringOs5Rev4 // Treo 680, 755p
			|| hal == hsHALIDHandspringOs5Rev4Sim

			|| hal == hsHALIDHandspringOs5Rev3 // Treo 700p
			|| hal == hsHALIDHandspringOs5Rev3Sim
			
			|| hal == 'P053' // Centro
		) )
	{
		return 0;
	}

	if ( ( launchFlags & LAUNCH_FLAGS ) == LAUNCH_FLAGS )
	{
		// Pilot 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if ( rom < sysMakeROMVersion( 2, 0, 0, sysROMStageRelease, 0 ) )
			AppLaunchWithCommand( sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL );
	}

	return ( sysErrRomIncompatible );

} // RomVersionCompatible()

/*
 * AboutFormHandleEvent()
 */
Boolean AboutFormHandleEvent( EventType* pEvent )
{
	Boolean 			handled = false;
	FormType*			pForm = FrmGetActiveForm();
	MemHandle			strVerH = NULL;
	Char*				strVerP = NULL;
	Char				strVer[20];
		
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
		
			strVerH = DmGet1Resource('tver', 1000);
	
			if ( strVerH )
			{
				strVerP = MemHandleLock( strVerH );
		
				if ( strVerP )
				{
						StrCopy( strVer, "v" );
						StrCat( strVer, strVerP );
						StrCat( strVer, " [Freeware]" );
						strVer[StrLen( (Char *)FrmGetLabel( pForm, ABOUT_VER_LBL ) )] = chrNull;
						
						FrmCopyLabel( pForm, ABOUT_VER_LBL, strVer );
			
						MemHandleUnlock( strVerH );
				}
	   	
			   	DmReleaseResource( strVerH );
			   	strVerH = NULL;
			}
		
			FrmDrawForm( pForm );
			
			handled = true;
						
			break;
		
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case ABOUT_OK_BTN:
				
					FrmReturnToForm( 0 );
					
					handled = true;

					break;
					
				default:
					break;
			}
		
			break;
			
		default:
							
			break;
	}
	
	return ( handled );
	
} // AboutFormHandleEvent()

/*
 * EvtCharEqualsLaunchChar()
 *
 * <HsChars.h>
 * 
 * // enqueued after the display is turned on
 * // (the display is usually still off for Palm's late wakeup)
 * #define hsChrWakeup			0x1618
 * 
 * // causes the keyguard dialog to be displayed
 * // for internal use only... use hsAttrKeyboardLocked to programmatically enable/disable it
 * #define hsChrKeyboardLock	0x1619
 * #define hsChrModifierKey		0x160D // was hsChrUpdateGrfIndicator
 * 
 * #define vchrHardRockerCenter	0x013D
 * 
 * <Chars.h>
 * 
 * #define	vchrAutoOff			0x0114		// power off due to inactivity timer
 * #define	vchrPowerOff		0x011E		// Posted after autoOffChr or hardPowerChr
 * #define	vchrResumeSleep		0x011F		// Posted by NotifyMgr clients after they
 * 
 * vchrRockerUp 				0x0132 
 * vchrRockerDown 				0x0133
 * vchrLaunch 					0x0108
 * vchrHard4 					0x0207
 *
 * #define shiftKeyMask       	0x0001
 * #define capsLockMask       	0x0002
 * #define numLockMask        	0x0004
 * #define commandKeyMask     	0x0008
 * #define optionKeyMask      	0x0010
 * #define controlKeyMask     	0x0020
 * #define autoRepeatKeyMask  	0x0040      // True if generated due to auto-repeat
 * #define doubleTapKeyMask   	0x0080      // True if this is a double-tap event
 * #define poweredOnKeyMask   	0x0100      // True if this is a double-tap event
 * #define appEvtHookKeyMask  	0x0200      // True if this is an app hook key
 * #define libEvtHookKeyMask  	0x0400      // True if this is a library hook key
 * #define willSendUpKeyMask  	0x0800      // True if a keyUp event will be sent later
 * #define softwareKeyMask    	0x1000 
 *
 */
Boolean EvtCharEqualsLaunchChar( WChar evt_wChar, UInt16 evt_keyCode, UInt16 evt_modifier, UInt16 keyCharIdx, UInt16 keyModifierIdx )
{

	UInt16 			wChars[13] = { 0, vchrPageUp /* 0x000B */, vchrPageDown /* 0x000C */, 
										vchrRockerLeft /* 0x0134 */, vchrRockerRight /* 0x0135 */,
										vchrRockerCenter /* 0x0136 */, 
										vchrHard4 /* 0x0207 */, vchrMenu /* 0x0105 */, 
										vchrHard2 /* 0x0205 */, vchrHard3 /* 0x0206 */,
										hsChrSide /* 0x161F */, hsChrVolumeUp /* 0x161B */, hsChrVolumeDown /* 0x161C */ };
	UInt16			modifiers[5] = { willSendUpKeyMask | commandKeyMask, willSendUpKeyMask | shiftKeyMask | commandKeyMask,
			  							willSendUpKeyMask | optionKeyMask | commandKeyMask, willSendUpKeyMask | doubleTapKeyMask | commandKeyMask, 
			  							willSendUpKeyMask | autoRepeatKeyMask | commandKeyMask };		  					
	Boolean 		retVal = false;
	WChar			sel_wChar = wChars[keyCharIdx];
	UInt16			sel_modifier = modifiers[keyModifierIdx];
	prevModPref_t	modPrefs;
	
	ReadModifierCharPrefs( &modPrefs );		
			
	switch ( sel_wChar )
	{	
		case vchrPageUp: 			/* 0x000B */
		case vchrPageDown:			/* 0x000C */
		case vchrRockerLeft:		/* 0x0134 */
		case vchrRockerRight:		/* 0x0135 */
		case hsChrSide:				/* 0x161F */
		case hsChrVolumeUp:			/* 0x161B */
		case hsChrVolumeDown:		/* 0x161C */
		
			if ( evt_wChar == sel_wChar )
			{			
				retVal = ( ( evt_modifier == sel_modifier ) || ( modPrefs.prevModifier == sel_modifier ) );
			}
			
			break;
		
		case vchrHard2:				/* 0x0205 */
		case vchrHard3:				/* 0x0206 */
		case vchrHard4:				/* 0x0207 */
		
			if ( evt_keyCode == sel_wChar )
			{
				retVal = ( ( evt_modifier == sel_modifier ) || ( modPrefs.prevModifier == sel_modifier ) );
			}
			
			break;
			
		case vchrRockerCenter:		/* 0x0136 */
		
			if ( evt_wChar == sel_wChar )
			{	
				if ( modPrefs.prevModifier )
				{
					retVal = ( modPrefs.prevModifier == sel_modifier );
				}
				else
				{
					retVal = ( evt_modifier == ( sel_modifier & 0x00FF ) ) ;
				}
			}
				
			break;
			
		case vchrMenu: 				/* 0x0105 */
		
			if ( evt_wChar == sel_wChar )
			{
				retVal = ( evt_modifier == sel_modifier );
			}
			else if ( evt_keyCode == sel_wChar )
			{
				retVal = ( evt_modifier == ( sel_modifier | controlKeyMask ) );
			}
			
			break;
			
		default:
			break;	
	}
	
	if ( hsChrModifierKey == evt_wChar )
	{	
		modPrefs.prevModifier = evt_modifier;
		WriteModifierCharPrefs( &modPrefs );
	}
	else if ( ( modPrefs.prevModifier != 0 ) 
					/* && ( willSendUpKeyMask == ( evt_modifier & 0xFF00 ) ) */
					&& ( vchrHardRockerCenter != evt_wChar ) )
	{
		modPrefs.prevModifier = 0;
		WriteModifierCharPrefs( &modPrefs );
	}
	
	if ( ( retVal ) && ( keyModifierIdx == 4 /* autoRepeatKeyMask */ ) )
	{
		HsKeyStop( evt_keyCode );
	}
	
	return ( retVal );
	
} // EvtCharEqualsLaunchChar()

/*
 * Utils.c
 */
