/*
 * NumForm.c
 */

#include "NumForm.h"

static void 			InitializeAddNumForm( FormType* pForm );
static Boolean 			CopyAndSaveAddNumFormInfo( FormType* pForm );

/*
 * AddNumFormHandleEvent()
 */
Boolean AddNumFormHandleEvent( EventType* pEvent )
{
	Boolean					handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	AddrLookupParamsType 	AddrParams;
	FieldType*				pAddrField = NULL;
	Char					addr[STRLEN_NAME_FLD] = "\0";
	Char*					pNumPos = NULL;
							
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
			
			InitializeAddNumForm( pForm );
			
			FrmDrawForm( pForm );
			
			handled = true;
						
			break;
		
		case frmCloseEvent:
		
			// this is not generated...
			
			handled = false;
						
			break;
			
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case ADDNUM_OK_BTN:

					if ( CopyAndSaveAddNumFormInfo( pForm ) )
					{	
						FrmReturnToForm( 0 );
						RedrawPhnNumList();		
					}
					
					handled = true;

					break;

				case ADDNUM_CANCEL_BTN:
				
					FrmReturnToForm( 0 );
					RedrawPhnNumList();		
					
					handled = true;
					
					break;
				
				case ADDNUM_LOOKUP_BTN:
					
					pAddrField = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ADDNUM_NAME_FLD ) );
					
					if ( pAddrField )
					{
						AddrParams.title = "Radimayd: Address Lookup";
						AddrParams.pasteButtonText = "Copy";
						AddrParams.field1 = addrLookupSortField;
						AddrParams.field2 = addrLookupAnyPhones;
						AddrParams.field2Optional = false;
						AddrParams.userShouldInteract = true;  
						AddrParams.formatStringP = "^friendlyname (^label)|^smsphone"; // "^listphone, ";
						PhoneNumberLookupCustom( pAddrField, &AddrParams, false );
						
						addr[0] = chrNull;
						pNumPos = NULL;
						
						GetFieldStr( pForm, ADDNUM_NAME_FLD, addr, STRLEN_NAME_FLD );
							
						if ( StrLen( addr ) )
						{
							pNumPos = StrChr( addr, '|' );
							if ( pNumPos )
							{
								SetFieldStr( pForm, ADDNUM_NUM_FLD, (Char *)(pNumPos + 1) );
								*pNumPos = chrNull;				
								SetFieldStr( pForm, ADDNUM_NAME_FLD, addr );
						
								FldDrawField( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ADDNUM_NUM_FLD ) ) );	
								FldDrawField( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, ADDNUM_NAME_FLD ) ) );
							}
						}		
					}
					
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
	
} // AddNumFormHandleEvent()

/*
 * InitializeAddNumForm()
 */
static void InitializeAddNumForm( FormType* pForm )
{
	phnNum_t		phnNum;
	
	if ( gPhnNumListIdx != noListSelection )
	{
		if ( !PhnSecDBGetRecordAtIdx( gPhnNumListIdx, &phnNum ) )
		{
			SetFieldStr( pForm, ADDNUM_NUM_FLD, phnNum.Number );
			SetFieldStr( pForm, ADDNUM_NAME_FLD, phnNum.Name );
		}
	}
		
	return;
	
} // InitializeAddNumForm()

	
/*
 * CopyAndSaveAddNumFormInfo()
 */
static Boolean CopyAndSaveAddNumFormInfo( FormType* pForm )
{	
	Boolean 		retVal = false;
	phnNum_t		phnNum;
	
	GetFieldStr( pForm, ADDNUM_NUM_FLD, phnNum.Number, STRLEN_PHN_NUMBER );
	GetFieldStr( pForm, ADDNUM_NAME_FLD, phnNum.Name, STRLEN_CALLER_ID );
	
	if ( StrLen( phnNum.Number ) )
	{
		if ( gPhnNumListIdx != noListSelection )
		{
			PhnSecDBReplaceRecordAtIdx( &gPhnNumListIdx, &phnNum, false );
		}
		else
		{
			PhnSecDBSaveNewRecord( &phnNum, false, NULL );
		}
	}
	
	retVal = true;
						
	return ( retVal );
		
} // CopyAndSaveAddNumFormInfo()

/*
 * NumForm.c
 */