/*
 * DispProgress.c
 */
 
#include "DispProgress.h"

/*
 * MsgProgressCallback()
 */
Boolean MsgProgressCallback( PrgCallbackDataPtr pCallbackData )
{	
	pCallbackData->bitmapId = BMP_MSG_PROGRESS;
	pCallbackData->canceled = true;
	pCallbackData->delay = true;
			
	switch ( pCallbackData->stage )
	{
		case 0:
		
			StrCopy( pCallbackData->textP, pCallbackData->message );
			StrCat( pCallbackData->textP, "\n\nConfiguring SMS..." );
			
			break;
				
		case 1:

			StrCopy( pCallbackData->textP, pCallbackData->message );			
			StrCat( pCallbackData->textP, "\n\nSending message..." );
			pCallbackData->delay = false;
							
			break;

		case 2:

			StrCopy( pCallbackData->textP, pCallbackData->message );			
			StrCat( pCallbackData->textP, "\n\nMessage sent." );
			
			break;
			
		case 3:

			StrCopy( pCallbackData->textP, pCallbackData->message );		
			StrCat( pCallbackData->textP, "\n" );
			if ( pCallbackData->error )
			{
				char 		errStr[maxStrIToALen];
				
				StrPrintF(errStr, "[0x%x]", pCallbackData->error );	
				StrCat( pCallbackData->textP, errStr );
			}
			StrCat( pCallbackData->textP, "\nMessage not sent." );
			
			pCallbackData->error = 0; // dismiss the pop-up automatically
			
			break;
			
		default:
			break;
	}

	pCallbackData->textChanged = true;

	return ( true ); 

} // MsgProgressCallback()

/*
 * DispProgress.c
 */