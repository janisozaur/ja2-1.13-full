#ifdef PRECOMPILEDHEADERS
	#include "laptop_all.h"
#else
	#include "char_profile.h"
	#include "imp_portraits.h"
	#include "imp_main_page.h"
	#include "imp_home_page.h"
	#include "imp_video_objects.h"
	#include "utilities.h"
	#include "wcheck.h"
	#include "input.h"
	#include "isometric_utils.h"
	#include "debug.h"
	#include "word_wrap.h"
	#include "render_dirty.h"
	#include "encrypted_file.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "imp_text_system.h"
	#include "imp_voices.h"
	#include "imp_compile_character.h"
#endif

//current and last pages
INT32 iCurrentPortrait = 0;
INT32 iLastPicture = 7;

// buttons needed for the IMP portrait screen
INT32 giIMPPortraitButton[ 3 ];
INT32 giIMPPortraitButtonImage[ 3 ];

// redraw protrait screen
BOOLEAN fReDrawPortraitScreenFlag = FALSE;

// face index
INT32 iPortraitNumber = 0;


// function definitions

void IncrementPictureIndex( void );
void DecrementPicture( void );
void CreateIMPPortraitButtons( void );
void DestroyIMPPortraitButtons( void );
BOOLEAN CameBackToPortraitsPageButNotFinished();

// callbacks
void BtnIMPPortraitNextCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPortraitPreviousCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPortraitDoneCallback(GUI_BUTTON *btn,INT32 reason);

void EnterIMPPortraits( void )
{
  
		// create buttons
	CreateIMPPortraitButtons( );
	
	// render background
	RenderIMPPortraits( );


	return;
}


void RenderIMPPortraits( void )
{
  

  // render background
	RenderProfileBackGround( );

		// the Voices frame
	RenderPortraitFrame( 191, 167 );

	// render the current portrait
	RenderPortrait( 200, 176 );

	// indent for the text
	RenderAttrib1IndentFrame( 128, 65); 
	
	// text
	PrintImpText( );

	return;
}


void ExitIMPPortraits( void )
{
	// destroy buttons for IMP portrait page
  DestroyIMPPortraitButtons( ); 

	return;
}

void HandleIMPPortraits( void )
{

	// do we need to re write screen
	if ( fReDrawPortraitScreenFlag == TRUE ) 
	{
    RenderIMPPortraits( );
		
		// reset redraw flag
		fReDrawPortraitScreenFlag = FALSE;
	}
	return;
}


BOOLEAN RenderPortrait( INT16 sX, INT16 sY )
{
  // render the portrait of the current picture
  VOBJECT_DESC    VObjectDesc;
	HVOBJECT hHandle;
	UINT32 uiGraphicHandle;

  if( fCharacterIsMale  )
	{

		// load it
    VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	  FilenameForBPP( pPlayerSelectedBigFaceFileNames[ iCurrentPortrait ] , VObjectDesc.ImageFile);
	  CHECKF(AddVideoObject(&VObjectDesc, &uiGraphicHandle));
       
	  // show it
    GetVideoObject(&hHandle, uiGraphicHandle);
    BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + sX, LAPTOP_SCREEN_WEB_UL_Y + sY , VO_BLT_SRCTRANSPARENCY,NULL);


    // and kick it's sorry ..umm never mind, outta here
    DeleteVideoObjectFromIndex( uiGraphicHandle );

	  
	}
	else
	{
    	// load it
    VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	  FilenameForBPP( pPlayerSelectedBigFaceFileNames[ iCurrentPortrait + 8 ] , VObjectDesc.ImageFile);
	  CHECKF(AddVideoObject(&VObjectDesc, &uiGraphicHandle));
       
	  // show it
    GetVideoObject(&hHandle, uiGraphicHandle);
    BltVideoObject(FRAME_BUFFER, hHandle, 0, LAPTOP_SCREEN_UL_X + sX, LAPTOP_SCREEN_WEB_UL_Y + sY , VO_BLT_SRCTRANSPARENCY,NULL);


    // and kick it's sorry ..umm never mind, outta here
    DeleteVideoObjectFromIndex( uiGraphicHandle );

	}

	return ( TRUE );
}



void IncrementPictureIndex( void )
{
	// cycle to next picture
	
	iCurrentPortrait++;

	// gone too far?
	if( iCurrentPortrait > iLastPicture )
	{
		iCurrentPortrait = 0;
	}


  return; 
}


void DecrementPicture( void )
{
  // cycle to previous picture
	
	iCurrentPortrait--;

	// gone too far?
  if( iCurrentPortrait < 0 )
	{
    iCurrentPortrait = iLastPicture;
	}

	return;
}


void CreateIMPPortraitButtons( void )
{
  // will create buttons need for the IMP portrait screen

	// next button
  giIMPPortraitButtonImage[0]=  LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,1,-1,3,-1 );
	/*giIMPPortraitButton[0] = QuickCreateButton( giIMPPortraitButtonImage[0], LAPTOP_SCREEN_UL_X +  ( 18 ), LAPTOP_SCREEN_WEB_UL_Y + ( 184 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitNextCallback );
  */
	giIMPPortraitButton[0] = CreateIconAndTextButton( giIMPPortraitButtonImage[0], pImpButtonText[ 13 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 343 ), LAPTOP_SCREEN_WEB_UL_Y + ( 205 ),BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 	BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitNextCallback);


	// previous button
	giIMPPortraitButtonImage[ 1 ]=  LoadButtonImage( "LAPTOP\\voicearrows.sti" ,-1,0,-1,2,-1 );
/*	giIMPPortraitButton[ 1 ] = QuickCreateButton( giIMPPortraitButtonImage[ 1 ], LAPTOP_SCREEN_UL_X +  ( 18 ), LAPTOP_SCREEN_WEB_UL_Y + ( 254 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitPreviousCallback );
  */
  giIMPPortraitButton[ 1 ] = CreateIconAndTextButton( giIMPPortraitButtonImage[ 1 ], pImpButtonText[ 12 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														  LAPTOP_SCREEN_UL_X +  ( 93), LAPTOP_SCREEN_WEB_UL_Y + ( 205 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 	BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitPreviousCallback);


	// done button
  giIMPPortraitButtonImage[ 2 ]=  LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	/* giIMPPortraitButton[ 2 ] = QuickCreateButton( giIMPPortraitButtonImage[ 1 ], LAPTOP_SCREEN_UL_X +  ( 349 ), LAPTOP_SCREEN_WEB_UL_Y + ( 220 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitDoneCallback );
  */
  giIMPPortraitButton[ 2 ] = CreateIconAndTextButton( giIMPPortraitButtonImage[ 2 ], pImpButtonText[ 11 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 187 ), LAPTOP_SCREEN_WEB_UL_Y + ( 330 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 	BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPortraitDoneCallback);



   SetButtonCursor(giIMPPortraitButton[0], CURSOR_WWW);
	 SetButtonCursor(giIMPPortraitButton[1], CURSOR_WWW);
	 SetButtonCursor(giIMPPortraitButton[2], CURSOR_WWW);
}



void DestroyIMPPortraitButtons( void )
{

	// will destroy buttons created for IMP Portrait screen
  
	// the next button
  RemoveButton(giIMPPortraitButton[ 0 ] );
  UnloadButtonImage(giIMPPortraitButtonImage[ 0 ] );
	
	// the previous button
  RemoveButton(giIMPPortraitButton[ 1 ] );
  UnloadButtonImage(giIMPPortraitButtonImage[ 1 ] );
	
	// the done button
  RemoveButton(giIMPPortraitButton[ 2 ] );
  UnloadButtonImage(giIMPPortraitButtonImage[ 2 ] );

	return;
}


void BtnIMPPortraitNextCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// next picture!!
			IncrementPictureIndex( );

			fReDrawPortraitScreenFlag = TRUE;
		}
	}	
} 

void BtnIMPPortraitPreviousCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// previous picture, please!!!
			DecrementPicture( );

			fReDrawPortraitScreenFlag = TRUE;
		}
	}	
} 

void BtnIMPPortraitDoneCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP attrbite begin button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
      
			// go to main page
			iCurrentImpPage = IMP_MAIN_PAGE;
/*
			// current mode now is voice
		  if( iCurrentProfileMode < IMP__VOICE )
			{
        iCurrentProfileMode = IMP__VOICE;
			}
*/			
			// if we are already done, leave
	    if( iCurrentProfileMode == IMP__FINISH )
			{
	      iCurrentImpPage = IMP_FINISH;
			} 
			else
			{
				if( CameBackToPortraitsPageButNotFinished() )
				{
//					iCurrentProfileMode = IMP__VOICE;
		      iCurrentImpPage = IMP_MAIN_PAGE;
				}
				else
				{
					iCurrentProfileMode = IMP__VOICE;
				} 
			}

			// grab picture number
			if( fCharacterIsMale  )
			{
        // male
				iPortraitNumber = iCurrentPortrait;
			}
			else
			{
        // female
				iPortraitNumber = iCurrentPortrait + ( 8 );

			}
			
			fButtonPendingFlag = TRUE;
		}
	}	
} 


BOOLEAN CameBackToPortraitsPageButNotFinished()
{
	//if we are in a page that comes after this one
	if( iCurrentProfileMode == IMP__PERSONALITY ||
			iCurrentProfileMode == IMP__ATTRIBUTES ||
			iCurrentProfileMode == IMP__VOICE )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}