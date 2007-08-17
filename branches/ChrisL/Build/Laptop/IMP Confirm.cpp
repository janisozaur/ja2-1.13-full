#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "CharProfile.h"
	#include "IMP MainPage.h"
	#include "IMP HomePage.h"
	#include "IMPVideoObjects.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "IMP Compile Character.h"
	#include "soldier profile type.h"
	#include "IMP Text System.h"
	#include "IMP Confirm.h"
	#include "finances.h"
	#include "Soldier Profile.h"
	#include "Soldier Profile Type.h"
	#include "Soldier Control.h"
	#include "IMP Portraits.h"
	#include "IMP Voices.h"
	#include "Overhead.h"
	#include "finances.h"
	#include "history.h"
	#include "Game Clock.h"
	#include "email.h"
	#include "Game Event Hook.h"
	#include "LaptopSave.h"
	#include "strategic.h"
	#include "weapons.h"
	#include "Random.h"
	#include "GameSettings.h"
	#include "GameVersion.h"
#endif

#define IMP_FILENAME_SUFFIX ".dat"

//CHRISL: structure needed to store temporary inventory information during IMP creation
typedef struct
{
	UINT16		inv;
	UINT8		iSize;
	UINT32		iClass;
	UINT8		iStatus;
	UINT8		iNumber;
}	INVNODE;

IMP_ITEM_CHOICE_TYPE gIMPItemChoices[MAX_IMP_ITEM_TYPES];
	
void GiveIMPRandomItems( MERCPROFILESTRUCT *pProfile, UINT8 typeIndex );
void GiveIMPItems( MERCPROFILESTRUCT *pProfile, INT8 abilityValue, UINT8 typeIndex );

UINT32 giIMPConfirmButton[ 2 ];
UINT32 giIMPConfirmButtonImage[ 2 ];
BOOLEAN fNoAlreadySelected = FALSE;
UINT16 uiEyeXPositions[ ]={ 
	8,
	9,
	8,
	6,
	13,
	11,
	8,
	8,
	4,				//208
	5,				//209
	7,
	5,				//211
	7,
	11,
	8,				//214
	5,
};

UINT16 uiEyeYPositions[ ]=
{ 
	5,
	4,
	5,
	6,
	5,
	5,
	4,
	4,
	4,		//208
	5,
	5,		//210
	7,
	6,		//212
	5,
	5,		//214
	6,
};

UINT16 uiMouthXPositions[]=
{
	8,
	9,
	7,
	7,
	11,
	10,
	8,
	8,
	5,		//208
	6,
	7,		//210
	6,
	7,		//212
	9,
	7,		//214
	5,
};

UINT16 uiMouthYPositions[]={
	21,
	23,
	24,
	25,
	23,
	24,
	24,
	24,
	25,		//208
	24,
	24,		//210
	26,
	24,		//212
	23,
	24,		//214
	26,
};

BOOLEAN fLoadingCharacterForPreviousImpProfile = FALSE;

void CreateConfirmButtons( void );
void DestroyConfirmButtons( void );
void GiveItemsToPC( UINT8 ubProfileId );
void MakeProfileInvItemAnySlot(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubStatus, UINT8 ubHowMany);
void MakeProfileInvItemThisSlot(MERCPROFILESTRUCT *pProfile, UINT32 uiPos, UINT16 usItem, UINT8 ubStatus, UINT8 ubHowMany);
INT32 FirstFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubHowMany);
// CHRISL:
void RedistributeStartingItems(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 sPocket);
void DistributeInitialGear(MERCPROFILESTRUCT *pProfile);
INT32 FirstFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, INVNODE *tInv);
INT32 AnyFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, INVNODE *tInv);
INT32 SpecificFreePocket(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubHowMany, UINT32 usClass);

// callbacks
void BtnIMPConfirmNo( GUI_BUTTON *btn,INT32 reason );
void BtnIMPConfirmYes( GUI_BUTTON *btn,INT32 reason );


void EnterIMPConfirm( void )
{
	// create buttons
	CreateConfirmButtons( );
	return;
}

void RenderIMPConfirm( void )
{

	 // the background
	RenderProfileBackGround( );
	
		// indent
  RenderAvgMercIndentFrame(90, 40 ); 

	// highlight answer
  PrintImpText( );

	return;
}

void ExitIMPConfirm( void )
{

	// destroy buttons
  DestroyConfirmButtons( );
	return;
}

void HandleIMPConfirm( void )
{
	return;
}

void CreateConfirmButtons( void )
{
	// create buttons for confirm screen
  
	giIMPConfirmButtonImage[0]=  LoadButtonImage( "LAPTOP\\button_2.sti" ,-1,0,-1,1,-1 );
	giIMPConfirmButton[0] = CreateIconAndTextButton( giIMPConfirmButtonImage[0], pImpButtonText[ 16 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 136 ), LAPTOP_SCREEN_WEB_UL_Y + ( 254 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPConfirmYes);
  
	giIMPConfirmButtonImage[1]=  LoadButtonImage( "LAPTOP\\button_2.sti" ,-1,0,-1,1,-1 );
	giIMPConfirmButton[1] = CreateIconAndTextButton( giIMPConfirmButtonImage[ 1 ], pImpButtonText[ 17 ], FONT12ARIAL, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 FONT_WHITE, DEFAULT_SHADOW, 
														 TEXT_CJUSTIFIED, 
														 LAPTOP_SCREEN_UL_X +  ( 136 ), LAPTOP_SCREEN_WEB_UL_Y + ( 314 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														 BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPConfirmNo);
 
 SetButtonCursor(giIMPConfirmButton[ 0 ], CURSOR_WWW);
 SetButtonCursor(giIMPConfirmButton[ 1 ], CURSOR_WWW);
    
	return;
}


void DestroyConfirmButtons( void )
{
  // destroy buttons for confirm screen
	
  RemoveButton(giIMPConfirmButton[ 0 ] );
  UnloadButtonImage(giIMPConfirmButtonImage[ 0 ] );

  
	RemoveButton(giIMPConfirmButton[ 1 ] );
  UnloadButtonImage(giIMPConfirmButtonImage[ 1 ] );
	return;
}



BOOLEAN AddCharacterToPlayersTeam( void )
{

	MERC_HIRE_STRUCT HireMercStruct;


	// last minute chage to make sure merc with right facehas not only the right body but body specific skills...
	// ie..small mercs have martial arts..but big guys and women don't don't

	HandleMercStatsForChangesInFace( );

	memset(&HireMercStruct, 0, sizeof(MERC_HIRE_STRUCT));

	HireMercStruct.ubProfileID = ( UINT8 )( LaptopSaveInfo.iIMPIndex ) ;

	if( fLoadingCharacterForPreviousImpProfile == FALSE )
	{
		// give them items
		GiveItemsToPC( 	HireMercStruct.ubProfileID );
	}


	HireMercStruct.sSectorX									 = gsMercArriveSectorX;
	HireMercStruct.sSectorY									 = gsMercArriveSectorY;
	HireMercStruct.fUseLandingZoneForArrival = TRUE;

	HireMercStruct.fCopyProfileItemsOver = TRUE;

	// indefinite contract length
	HireMercStruct.iTotalContractLength = -1;

	HireMercStruct.ubInsertionCode	= INSERTION_CODE_ARRIVING_GAME;
	HireMercStruct.uiTimeTillMercArrives = GetMercArrivalTimeOfDay( );

	SetProfileFaceData( HireMercStruct.ubProfileID , ( UINT8 ) ( 200 + iPortraitNumber ), uiEyeXPositions[ iPortraitNumber ], uiEyeYPositions[ iPortraitNumber ], uiMouthXPositions[ iPortraitNumber ], uiMouthYPositions[ iPortraitNumber ] );

	//if we succesfully hired the merc
	if( !HireMerc( &HireMercStruct ) )
	{
		return(FALSE);
	}
	else
	{
		return ( TRUE );
	}
}


void  BtnIMPConfirmYes(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP Homepage About US button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{

			// reset button
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
// Madd
/*			if( LaptopSaveInfo.fIMPCompletedFlag )
			{
				// already here, leave
				return;
			}
*/
			if( LaptopSaveInfo.iCurrentBalance < COST_OF_PROFILE )
			{
				// not enough
		    return;
			}

			// line moved by CJC Nov 28 2002 to AFTER the check for money
			LaptopSaveInfo.fIMPCompletedFlag = TRUE;

			// charge the player
			AddTransactionToPlayersBook(IMP_PROFILE, (UINT8)(LaptopSaveInfo.iIMPIndex), GetWorldTotalMin( ), - ( COST_OF_PROFILE ) );
      AddHistoryToPlayersLog( HISTORY_CHARACTER_GENERATED, 0,GetWorldTotalMin( ), -1, -1 );
			AddCharacterToPlayersTeam( );
			
			// write the created imp merc
			WriteOutCurrentImpCharacter( ( UINT8 )( LaptopSaveInfo.iIMPIndex ) );
		
			fButtonPendingFlag = TRUE;
			iCurrentImpPage = IMP_HOME_PAGE;

			//Kaiden: Below is the Imp personality E-mail as it was.
/*
			// send email notice
			//AddEmail(IMP_EMAIL_PROFILE_RESULTS, IMP_EMAIL_PROFILE_RESULTS_LENGTH, IMP_PROFILE_RESULTS, GetWorldTotalMin( ) );
			AddFutureDayStrategicEvent( EVENT_DAY2_ADD_EMAIL_FROM_IMP, 60 * 7, 0, 2 );
*/

			//Kaiden: And here is my Answer to the IMP E-mails only
			// profiling the last IMP made. You get the results immediately
			AddEmail(IMP_EMAIL_PROFILE_RESULTS, IMP_EMAIL_PROFILE_RESULTS_LENGTH, IMP_PROFILE_RESULTS, GetWorldTotalMin( ), LaptopSaveInfo.iIMPIndex );

			//RenderCharProfile( );

			ResetCharacterStats();

			//Display a popup msg box telling the user when and where the merc will arrive
			//DisplayPopUpBoxExplainingMercArrivalLocationAndTime( LaptopSaveInfo.iIMPIndex );
		
			//reset the id of the last merc so we dont get the DisplayPopUpBoxExplainingMercArrivalLocationAndTime() pop up box in another screen by accident
			LaptopSaveInfo.sLastHiredMerc.iIdOfMerc = -1;
		}
	}
	
}

// fixed? by CJC Nov 28 2002
void BtnIMPConfirmNo( GUI_BUTTON *btn,INT32 reason )
{
  	// btn callback for IMP Homepage About US button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{

			iCurrentImpPage = IMP_FINISH;

			/*

			LaptopSaveInfo.fIMPCompletedFlag = FALSE;
			ResetCharacterStats();

			fButtonPendingFlag = TRUE;
			iCurrentImpPage = IMP_HOME_PAGE;
			*/
			/*
			if( fNoAlreadySelected == TRUE )
			{
				// already selected no 
				fButtonPendingFlag = TRUE;
				iCurrentImpPage = IMP_HOME_PAGE;
			}
      fNoAlreadySelected = TRUE;
			*/
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

/*
void BtnIMPConfirmNo( GUI_BUTTON *btn,INT32 reason )
{
	

  	// btn callback for IMP Homepage About US button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		 btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			LaptopSaveInfo.fIMPCompletedFlag = TRUE;
			if( fNoAlreadySelected == TRUE )
			{
				// already selected no 
				fButtonPendingFlag = TRUE;
				iCurrentImpPage = IMP_HOME_PAGE;
			}
      fNoAlreadySelected = TRUE;
      btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}
*/


#define PROFILE_HAS_SKILL_TRAIT( p, t ) ((p->bSkillTrait == t) || (p->bSkillTrait2 == t))

//CHRISL: New function to handle proper distribution of starting gear
void DistributeInitialGear(MERCPROFILESTRUCT *pProfile)
{
	INVNODE			tInv[NUM_INV_SLOTS];
	int				i, j, number;
	UINT8			count = 0, length;
	vector<int>		iOrder;
	INT32			iSlot;
	BOOLEAN			iSet = FALSE;

	// First move profile information to temporary storage
	for(i=0; i<NUM_INV_SLOTS; i++)
	{
		if(pProfile->inv[i] != NOTHING)
		{
			tInv[count].inv = pProfile->inv[i];
			tInv[count].iSize = Item[pProfile->inv[i]].ItemSize;
			tInv[count].iClass = Item[pProfile->inv[i]].usItemClass;
			tInv[count].iStatus = (pProfile->bInvStatus[i] > 0) ? pProfile->bInvStatus[i] : 100;
			tInv[count].iNumber = (pProfile->bInvNumber[i] == 0) ? 1 :pProfile->bInvNumber[i];
			pProfile->inv[i] = 0;
			pProfile->bInvStatus[i] = 0;
			pProfile->bInvNumber[i] = 0;
			count++;
		}
	}

	length = count;
	count = 0;
	// Next sort list by size
	for(j=34; j>=0; j--)
	{
		for(i=0; i<length; i++)
		{
			if(tInv[i].iSize == j)
			{
				int *filler = new int;
				iOrder.push_back(*filler);
				iOrder[count] = i;
				count++;
			}
		}
	}

	// Last, go through the temp data and put items into appropriate pockets
	// Start by putting items into specific pockets
	for(i=0; i<count; i++)
	{
		iSlot = SpecificFreePocket(pProfile, tInv[iOrder[i]].inv, tInv[iOrder[i]].iNumber, tInv[iOrder[i]].iClass);
		if(iSlot != -1)
		{
			MakeProfileInvItemThisSlot(pProfile, iSlot, tInv[iOrder[i]].inv, tInv[iOrder[i]].iStatus, tInv[iOrder[i]].iNumber);
			iOrder[i]=-1;
		}
	}
	// Next, put anything that isn't an attachment into a pocket
	for(i=0; i<count; i++)
	{
		if(iOrder[i]!=-1)
		{
			// skip if this item is an attachment
			if(Item[tInv[iOrder[i]].inv].attachment)
				continue;
			iSet = FALSE;
			number = tInv[iOrder[i]].iNumber;
			tInv[iOrder[i]].iNumber = 1;
			for(int j=0; j<number; j++)
			{
				iSlot = FirstFreeBigEnoughPocket (pProfile, &tInv[iOrder[i]]);
				if(iSlot != -1)
				{
					MakeProfileInvItemThisSlot(pProfile, iSlot, tInv[iOrder[i]].inv, tInv[iOrder[i]].iStatus, tInv[iOrder[i]].iNumber);
					iSet = TRUE;
				}
			}
			if(iSet)
				iOrder[i]=-1;
		}
	}
	// finish by putting anything that's left into any pocket, including inactive pockets
	for(i=0; i<count; i++)
	{
		if(iOrder[i]!=-1)
		{
			iSet = FALSE;
			number = tInv[iOrder[i]].iNumber;
			tInv[iOrder[i]].iNumber = 1;
			for(int j=0; j<number; j++)
			{
				iSlot = AnyFreeBigEnoughPocket (pProfile, &tInv[iOrder[i]]);
				if(iSlot != -1)
				{
					MakeProfileInvItemThisSlot(pProfile, iSlot, tInv[iOrder[i]].inv, tInv[iOrder[i]].iStatus, tInv[iOrder[i]].iNumber);
					iSet = TRUE;
				}
			}
			if(iSet)
				iOrder[i] = -1;
		}
	}
}

void GiveItemsToPC( UINT8 ubProfileId )
{
	MERCPROFILESTRUCT *pProfile;


  // gives starting items to merc
	// NOTE: Any guns should probably be from those available in regular gun set

	pProfile = &(gMercProfiles[ubProfileId]);


	// STANDARD EQUIPMENT

	GiveIMPItems(pProfile, 100, IMP_DEFAULT);
	GiveIMPRandomItems(pProfile, IMP_RANDOMDEFAULT);
	

	// kevlar vest, leggings, & helmet
//	MakeProfileInvItemThisSlot(pProfile, VESTPOS, FLAK_JACKET, 100, 1);

	GiveIMPItems (pProfile,pProfile->bWisdom,IMP_WISDOM);


	//if ( PreRandom( 100 ) < (UINT32) pProfile->bWisdom )
	//{
	//	MakeProfileInvItemThisSlot(pProfile, HELMETPOS, STEEL_HELMET, 100, 1);
	//}

	// canteen
//	MakeProfileInvItemThisSlot(pProfile, SMALLPOCK4POS, CANTEEN, 100, 1);

	GiveIMPItems (pProfile,pProfile->bMarksmanship,IMP_MARKSMANSHIP);

	
	//Give ANY imp the calico
//	MakeProfileInvItemThisSlot( pProfile, HANDPOS, M950, 100, 1);



	//No ammo clip	MakeProfileInvItemThisSlot( pProfile, SMALLPOCK1POS, CLIP9_30, 100, 2);

	/*
	Ja25 IMP only gets 1 gun now




	if (pProfile->bMarksmanship >= 80)
	{
		// good shooters get a better & matching ammo
		MakeProfileInvItemThisSlot( pProfile, HANDPOS, MP5K, 100, 1);
		MakeProfileInvItemThisSlot( pProfile, SMALLPOCK1POS, CLIP9_30, 100, 2);
	}
	else
	{
		// Automatic pistol, with matching ammo
		MakeProfileInvItemThisSlot( pProfile, HANDPOS, BERETTA_93R, 100, 1 );
		MakeProfileInvItemThisSlot( pProfile, SMALLPOCK1POS, CLIP9_15, 100, 3 );
	}
	*/

	// OPTIONAL EQUIPMENT: depends on skills & special skills

	GiveIMPItems (pProfile,pProfile->bMedical,IMP_MEDICAL);

	//if (pProfile->bMedical >= 60)
	//{
	//	// strong medics get full medic kit
	//	MakeProfileInvItemAnySlot(pProfile, MEDICKIT, 100, 1);
	//}
	//else
	//if (pProfile->bMedical >= 30)
	//{
	//	// passable medics get first aid kit
	//	MakeProfileInvItemAnySlot(pProfile, FIRSTAIDKIT, 100, 1);
	//}

	GiveIMPItems (pProfile,pProfile->bMechanical,IMP_MECHANICAL);

	//if (pProfile->bMechanical >= 50)
	//{
	//	// mechanics get toolkit
	//	MakeProfileInvItemAnySlot(pProfile, TOOLKIT, 100, 1);
	//}

	GiveIMPItems (pProfile,pProfile->bExplosive,IMP_EXPLOSIVE);

	//if (pProfile->bExplosive >= 50)
	//{
	//	// loonies get TNT & Detonator
		//MakeProfileInvItemAnySlot(pProfile, TNT, 100, 1);
		//MakeProfileInvItemAnySlot(pProfile, DETONATOR, 100, 1);
	//}

	GiveIMPItems (pProfile,pProfile->bAgility,IMP_AGILITY);
	GiveIMPItems (pProfile,pProfile->bDexterity,IMP_DEXTERITY);
	GiveIMPItems (pProfile,pProfile->bStrength,IMP_STRENGTH);
	GiveIMPItems (pProfile,pProfile->bLifeMax,IMP_HEALTH);
	GiveIMPItems (pProfile,pProfile->bLeadership,IMP_LEADERSHIP);

	// check for special skills
	if (PROFILE_HAS_SKILL_TRAIT(pProfile, LOCKPICKING) && ( iMechanical ) )
	{
		//MakeProfileInvItemAnySlot(pProfile, LOCKSMITHKIT, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_LOCKPICKING);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, HANDTOHAND))
	{
		//MakeProfileInvItemAnySlot(pProfile, BRASS_KNUCKLES, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_HTH);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, ELECTRONICS) && ( iMechanical ) )
	{
		//MakeProfileInvItemAnySlot(pProfile, METALDETECTOR, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_ELECTRONICS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, NIGHTOPS))
	{
//		MakeProfileInvItemAnySlot(pProfile, SILENCER, 100, 2);
		GiveIMPRandomItems(pProfile,IMP_NIGHTOPS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, THROWING))
	{
//		MakeProfileInvItemAnySlot(pProfile, THROWING_KNIFE, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_THROWING);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, STEALTHY))
	{
//		MakeProfileInvItemAnySlot(pProfile, SILENCER, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_STEALTHY);
	}
	
	if (PROFILE_HAS_SKILL_TRAIT(pProfile, KNIFING))
	{
//		MakeProfileInvItemAnySlot(pProfile, COMBAT_KNIFE, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_KNIFING);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, CAMOUFLAGED)) // TODO: Madd - other camouflage types, once we figure out a way to enable more traits
	{
//		MakeProfileInvItemAnySlot(pProfile, CAMOUFLAGEKIT, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_CAMOUFLAGED);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, AMBIDEXT))
	{
//		MakeProfileInvItemAnySlot(pProfile, M950, 100, 1);
		GiveIMPRandomItems(pProfile,IMP_AMBIDEXTROUS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, MARTIALARTS))
	{
		GiveIMPRandomItems(pProfile,IMP_MARTIALARTS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, ONROOF))
	{
		GiveIMPRandomItems(pProfile,IMP_ROOFTOPS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, TEACHING))
	{
		GiveIMPRandomItems(pProfile,IMP_TEACHING);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, AUTO_WEAPS))
	{
		GiveIMPRandomItems(pProfile,IMP_AUTOWEAPONS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, HEAVY_WEAPS ))
	{
		GiveIMPRandomItems(pProfile,IMP_HEAVYWEAPONS);
	}

	if (PROFILE_HAS_SKILL_TRAIT(pProfile, THIEF ))
	{
		GiveIMPRandomItems(pProfile,IMP_THIEF);
	}

	// CHRISL: Now that all items have been issued, distribute them into appropriate pockets, starting with the largest items
	DistributeInitialGear(pProfile);
}


void MakeProfileInvItemAnySlot(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubStatus, UINT8 ubHowMany)
{
	INT32 iSlot=-1;

	if(!gGameOptions.ubInventorySystem)
		iSlot = FirstFreeBigEnoughPocket(pProfile, usItem, ubHowMany);
	else
	{
		// CHRISL: Alter the placement of initial equipment to come last.  At this stage, just add items, one pocket at a time
		for(int i=0; i<NUM_INV_SLOTS; i++)
		{
			if(pProfile->inv[i] == NOTHING)
			{
				iSlot = i;
				break;
			}
		}
	}

	if (iSlot == -1)
	{
		// no room, item not received
		return;
	}

	// put the item into that slot
	MakeProfileInvItemThisSlot(pProfile, iSlot, usItem, ubStatus, ubHowMany);
}


void MakeProfileInvItemThisSlot(MERCPROFILESTRUCT *pProfile, UINT32 uiPos, UINT16 usItem, UINT8 ubStatus, UINT8 ubHowMany)
{
  pProfile->inv[uiPos]				= usItem;
  pProfile->bInvStatus[uiPos] = ubStatus;
  pProfile->bInvNumber[uiPos] = ubHowMany;
}


// CHRISL: New function to move initial gear into LBE pockets when LBE items are given during creation
void RedistributeStartingItems(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 sPocket)
{
	UINT16	lbeIndex;
	UINT8	lbeClass, iSize;
	UINT16	inv[NUM_INV_SLOTS], istatus[NUM_INV_SLOTS], inumber[NUM_INV_SLOTS];

	lbeIndex = Item[usItem].ubClassIndex;
	lbeClass = LoadBearingEquipment[lbeIndex].lbeClass;

	// Move non-worn items into temporary storage
	for(int i=0; i<NUM_INV_SLOTS; i++)
	{
		if(i>=BODYPOSFINAL)
		{
			inv[i] = pProfile->inv[i];
			istatus[i] = pProfile->bInvStatus[i];
			inumber[i] = pProfile->bInvNumber[i];
			pProfile->inv[i] = 0;
			pProfile->bInvStatus[i] = 0;
			pProfile->bInvNumber[i] = 0;
		}
		else
		{
			switch (i)
			{
				case HANDPOS:
				case SECONDHANDPOS:
					inv[i] = pProfile->inv[i];
					istatus[i] = pProfile->bInvStatus[i];
					inumber[i] = pProfile->bInvNumber[i];
					pProfile->inv[i] = 0;
					pProfile->bInvStatus[i] = 0;
					pProfile->bInvNumber[i] = 0;
					break;
				default:
					inv[i] = 0;
					istatus[i] = 0;
					inumber[i] = 0;
					break;
			}
		}
	}

	// Redistribute stored items with the assumption of the new LBE item
	for(int i=0; i<NUM_INV_SLOTS; i++)
	{
		if(inv[i] != NONE)
		{
			iSize = Item[inv[i]].ItemSize;
			for(int j=0; j<NUM_INV_SLOTS; j++)
			{
				if(icLBE[j] == sPocket && pProfile->inv[j] == NONE && LBEPocketType[LoadBearingEquipment[lbeIndex].lbePocketIndex[icPocket[j]]].ItemCapacityPerSize[iSize] != NONE)
				{
					pProfile->inv[j] = inv[i];
					pProfile->bInvStatus[j] = istatus[i];
					pProfile->bInvNumber[j] = inumber[i];
					inv[i] = istatus[i] = inumber[i] = 0;
					break;
				}
			}
			pProfile->inv[i] = inv[i];
			pProfile->bInvStatus[i] = istatus[i];
			pProfile->bInvNumber[i] = inumber[i];
			inv[i] = istatus[i] = inumber[i] = 0;
		}
	}
}

// CHRISL: New function to work with LBE pockets
INT32 SpecificFreePocket(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubHowMany, UINT32 usClass)
{
	UINT8	lbePocket;

	if (ubHowMany == 1)
	{
		switch (usClass)
		{
			case IC_LBEGEAR:
				if(pProfile->inv[VESTPOCKPOS]==NONE && LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass==VEST_PACK)
					return VESTPOCKPOS;
				if(pProfile->inv[LTHIGHPOCKPOS]==NONE && LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass==THIGH_PACK)
					return LTHIGHPOCKPOS;
				if(pProfile->inv[RTHIGHPOCKPOS]==NONE && LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass==THIGH_PACK)
					return RTHIGHPOCKPOS;
				if(LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass==COMBAT_PACK)
				{
					if(pProfile->inv[CPACKPOCKPOS]==NONE)
					{
						if(LoadBearingEquipment[Item[usItem].ubClassIndex].lbeCombo!=0)
						{
							if((pProfile->inv[BPACKPOCKPOS]!=NONE && LoadBearingEquipment[Item[pProfile->inv[BPACKPOCKPOS]].ubClassIndex].lbeCombo==LoadBearingEquipment[Item[usItem].ubClassIndex].lbeCombo) || pProfile->inv[BPACKPOCKPOS]==NONE)
								return CPACKPOCKPOS;
						}
						else if(pProfile->inv[BPACKPOCKPOS]==NONE)
							return CPACKPOCKPOS;
					}
				}
				if(LoadBearingEquipment[Item[usItem].ubClassIndex].lbeClass==BACKPACK)
				{
					if(pProfile->inv[BPACKPOCKPOS]==NONE)
					{
						if(LoadBearingEquipment[Item[usItem].ubClassIndex].lbeCombo!=0)
						{
							if((pProfile->inv[CPACKPOCKPOS]!=NONE && LoadBearingEquipment[Item[pProfile->inv[CPACKPOCKPOS]].ubClassIndex].lbeCombo==LoadBearingEquipment[Item[usItem].ubClassIndex].lbeCombo) || pProfile->inv[CPACKPOCKPOS]==NONE)
								return BPACKPOCKPOS;
						}
						else if(pProfile->inv[CPACKPOCKPOS]==NONE)
							return BPACKPOCKPOS;
					}
				}
				break;
			case IC_ARMOUR:
				if ( pProfile->inv[HELMETPOS] == NONE && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET )
					return HELMETPOS;
				if ( pProfile->inv[VESTPOS] == NONE && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_VEST )
					return VESTPOS;
				if ( pProfile->inv[LEGPOS] == NONE && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS && !(Item[usItem].attachment))
					return LEGPOS;
				break;
			case IC_BLADE:
				if ( pProfile->inv[KNIFEPOCKPOS] == NONE)
					return KNIFEPOCKPOS;
				break;
			case IC_BOMB:
				for(int i=BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
				{
					if(LoadBearingEquipment[Item[pProfile->inv[icLBE[i]]].ubClassIndex].lbePocketIndex[icPocket[i]]==7)	// TNT Pocket
						if(pProfile->inv[i] == NONE)
							return i;
				}
				break;
			case IC_GRENADE:
				for(int i=BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
				{
					if(pProfile->inv[i]==NONE && Item[usItem].ItemSize==16 && LoadBearingEquipment[Item[pProfile->inv[icLBE[i]]].ubClassIndex].lbePocketIndex[icPocket[i]]==12)	// Rifle Grenade
						return i;
					else if(pProfile->inv[i]==NONE && LoadBearingEquipment[Item[pProfile->inv[icLBE[i]]].ubClassIndex].lbePocketIndex[icPocket[i]]==13)	// Grenade
						return i;
				}
				break;
			case IC_GUN:
				if ( pProfile->inv[HANDPOS] == NONE )
					return HANDPOS;
				if ( pProfile->inv[SECONDHANDPOS] == NONE && !(Item[pProfile->inv[HANDPOS]].twohanded))
					return SECONDHANDPOS;
				if(gGameOptions.ubInventorySystem)
					if ( pProfile->inv[GUNSLINGPOCKPOS] == NONE && pProfile->inv[BPACKPOCKPOS] == NONE && LBEPocketType[1].ItemCapacityPerSize[Item[usItem].ItemSize]!=0)
						return GUNSLINGPOCKPOS;
				for(int i=BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
				{
					lbePocket = LoadBearingEquipment[Item[pProfile->inv[icLBE[i]]].ubClassIndex].lbePocketIndex[icPocket[i]];
					if(pProfile->inv[i]==NONE && (lbePocket==6 || lbePocket==10) && LBEPocketType[lbePocket].ItemCapacityPerSize[Item[usItem].ItemSize]!=0)
						return i;
				}
				break;
			case IC_FACE:
				if ( pProfile->inv[HEAD1POS] == NONE && CompatibleFaceItem(usItem,pProfile->inv[HEAD2POS]) )
					return HEAD1POS;
				if ( pProfile->inv[HEAD2POS] == NONE && CompatibleFaceItem(usItem,pProfile->inv[HEAD1POS]) )
					return HEAD2POS;
				break;
			default:
				break;
		}
	}
	return(-1);
}

//INT32 FirstFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubHowMany, UINT32 usClass)
INT32 FirstFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, INVNODE *tInv)
{
	UINT32	uiPos, iClass;
	UINT16	iSize, pDefault, lbeCap, usItem, pRestrict;
	UINT8	startPos, endPos;

	usItem = tInv->inv;
	iSize = Item[usItem].ItemSize;
	iClass = Item[usItem].usItemClass;
	for(int i=0; i<3; i++)
	{
		if(i==0){
			startPos = MEDPOCKFINAL;
			endPos = NUM_INV_SLOTS;
		}
		else if (i==1){
			startPos = BIGPOCKFINAL;
			endPos = MEDPOCKFINAL;
		}
		else{
			startPos = BODYPOSFINAL;
			endPos = BIGPOCKFINAL;
		}
		for(uiPos = startPos; uiPos<endPos; uiPos++)
		{
			if(icLBE[uiPos] == ITEM_NOT_FOUND)
				continue;
			pDefault = (pProfile->inv[icLBE[uiPos]] == NONE) ? icClass[uiPos] : Item[pProfile->inv[icLBE[uiPos]]].ubClassIndex;
			lbeCap = LBEPocketType[LoadBearingEquipment[pDefault].lbePocketIndex[icPocket[uiPos]]].ItemCapacityPerSize[iSize];
			pRestrict = LBEPocketType[LoadBearingEquipment[pDefault].lbePocketIndex[icPocket[uiPos]]].pRestriction;
			if(lbeCap>0 && (pRestrict == 0 || pRestrict == iClass))
			{
				if(pProfile->inv[uiPos] == NONE)
					return(uiPos);
				else if(pProfile->inv[uiPos] == usItem && lbeCap >= (tInv->iNumber+1))
				{
					tInv->iNumber++;
					return(uiPos);
				}
			}
		}
	}
	return(-1);
}

// CHRISL: This function will place objects anywhere it can.  It tries to use active pockets but will use any pocket as a last resort
INT32 AnyFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, INVNODE *tInv)
{
	INT32		iSlot;
	UINT32		uiPos;
	UINT16		iSize, lbeCap, usItem;

	iSlot = FirstFreeBigEnoughPocket (pProfile, tInv);
	if(iSlot != -1)
		return(iSlot);
	else
	{
		usItem = tInv->inv;
		for(uiPos = BODYPOSFINAL; uiPos < NUM_INV_SLOTS; uiPos ++)
		{
			iSize = Item[usItem].ubPerPocket;
			lbeCap = iSize * 2;
			if(uiPos >= MEDPOCKFINAL && iSize > 0)
				return(-1);
			else
			{
				if(pProfile->inv[uiPos] == NONE)
					return(uiPos);
				else if(pProfile->inv[uiPos] == usItem)
				{
					if((uiPos < BIGPOCKFINAL && iSize >= (tInv->iNumber+1)) || (uiPos >= BIGPOCKFINAL && lbeCap >= (tInv->iNumber+1)))
					{
					tInv->iNumber++;
					return(uiPos);
					}
				}
			}
		}
	}
	return(-1);
}

INT32 FirstFreeBigEnoughPocket(MERCPROFILESTRUCT *pProfile, UINT16 usItem, UINT8 ubHowMany)
{
	UINT32 uiPos;

	if (ubHowMany == 1)
	{
		// check body spots

		if ( pProfile->inv[HELMETPOS] == NONE && Item[usItem].usItemClass == IC_ARMOUR && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET )
			return HELMETPOS;
		if ( pProfile->inv[VESTPOS] == NONE && Item[usItem].usItemClass == IC_ARMOUR && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_VEST )
			return VESTPOS;
		if ( pProfile->inv[LEGPOS] == NONE && Item[usItem].usItemClass == IC_ARMOUR && Armour[Item[usItem].ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS )
			return LEGPOS;
		if ( pProfile->inv[HEAD1POS] == NONE && Item[usItem].usItemClass == IC_FACE && CompatibleFaceItem(usItem,pProfile->inv[HEAD2POS]) )
			return HEAD1POS;
		if ( pProfile->inv[HEAD2POS] == NONE && Item[usItem].usItemClass == IC_FACE && CompatibleFaceItem(usItem,pProfile->inv[HEAD1POS]) )
			return HEAD2POS;
		if ( pProfile->inv[HANDPOS] == NONE )
			return HANDPOS;
		if ( pProfile->inv[SECONDHANDPOS] == NONE )
			return SECONDHANDPOS;
	}
	// if it fits into a small pocket
	if (Item[usItem].ubPerPocket != 0)
	{
		// check small pockets first
		for (uiPos = SMALLPOCK1POS; uiPos <= SMALLPOCK8POS; uiPos++)
		{
			if (pProfile->inv[uiPos] == NONE)
			{
				return(uiPos);
			}
		}
	}

	// check large pockets
	for (uiPos = BIGPOCK1POS; uiPos <= BIGPOCK4POS; uiPos++)
	{
		if (pProfile->inv[uiPos] == NONE)
		{
			return(uiPos);
		}
	}

	return(-1);
}


void WriteOutCurrentImpCharacter( INT32 iProfileId )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("WriteOutCurrentImpCharacter: IMP.dat"));
	char zImpFileName[13];
	strcpy(zImpFileName,IMP_MERC_FILENAME);
	strcat(zImpFileName,IMP_FILENAME_SUFFIX);
	WriteOutCurrentImpCharacter ( iProfileId, zImpFileName);


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("WriteOutCurrentImpCharacter: Nickname.dat"));
	
	char zFileName[13];
	char temp;

	for(int i=0;i < 9;i++) // Madd: I couldn't get strcpy or anything to work here... weird... if s/o wants to fix it, go ahead
	{
		temp = (char) gMercProfiles[iProfileId].zNickname[i];
		zFileName[i] = temp;
	}

	strcat(zFileName,IMP_FILENAME_SUFFIX);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("WriteOutCurrentImpCharacter: %s", zFileName));
	WriteOutCurrentImpCharacter ( iProfileId, zFileName);
}

void WriteOutCurrentImpCharacter( INT32 iProfileId, STR fileName )
{
	// grab the profile number and write out what is contained there in 
	HWFILE hFile;
	UINT32 uiBytesWritten = 0;

	// open the file for writing
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("WriteOutCurrentImpCharacter: %s", fileName));
	hFile = FileOpen(fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE);

	// write out the profile id
	if (!FileWrite(hFile, &iProfileId, sizeof( INT32 ), &uiBytesWritten))
	{
		if (hFile)
			FileClose(hFile);
		return;
	}

	// write out the portrait id
	if (!FileWrite(hFile, &iPortraitNumber, sizeof( INT32 ), &uiBytesWritten))
	{
		if (hFile)
			FileClose(hFile);
		return;
	}

	// write out the profile itself
        // WDS - Clean up inventory handling
	gMercProfiles[ iProfileId ].CopyNewInventoryToOld();
	if (FileWrite(hFile, &gMercProfiles[ iProfileId ], SIZEOF_MERCPROFILESTRUCT_POD, &uiBytesWritten)) {

		// Write all the vectors from the soldier into the file
		if (SaveVector (hFile, gMercProfiles[ iProfileId ].inv)) {
			if (SaveVector (hFile, gMercProfiles[ iProfileId ].bInvStatus)) {
				if (SaveVector (hFile, gMercProfiles[ iProfileId ].bInvNumber)) {
				}
			}
		}
	}

	// close file
	if (hFile)
		FileClose(hFile);

	return;
}

BOOLEAN ImpExists ( STR nickName )
{
	char zFileName[13];

	strcpy(zFileName,nickName);
	strcat(zFileName,IMP_FILENAME_SUFFIX);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ImpExists: %s",  zFileName));
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ImpExists: %d", FileExistsNoDB(zFileName) ));

	return FileExistsNoDB(zFileName);
}

BOOLEAN LoadImpCharacter( STR nickName )
{
	INT32 iProfileId = 0;
	HWFILE hFile;
	UINT32 uiBytesRead = 0;

	char zFileName[13];

	strcpy(zFileName,nickName);
	strcat(zFileName,IMP_FILENAME_SUFFIX);

	// open the file for writing
	hFile = FileOpen(zFileName, FILE_ACCESS_READ, FALSE);

	// valid file?
	if( hFile == -1 )
	{
		DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 7 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return FALSE;
	}

	// read in the profile

	if (!FileRead(hFile, &iProfileId, sizeof( INT32 ), &uiBytesRead))
	{
		DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 7 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return FALSE;
	}

	// read in the portrait
	if (!FileRead(hFile, &iPortraitNumber ,sizeof( INT32 ), &uiBytesRead))
	{
		DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 7 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return FALSE;
	}

	// Set the ID of the new IMP
	iProfileId = GetFreeIMPSlot(iProfileId, iProfileId);

	// We can create the new imp, beacuse we found an empty slot
	if (iProfileId != -1)
	{
		LaptopSaveInfo.iIMPIndex = iProfileId;

		// read in the profile
                // WDS - Clean up inventory handling
		if (!FileRead(hFile, &gMercProfiles[ iProfileId ] , SIZEOF_MERCPROFILESTRUCT_POD, &uiBytesRead))
		{
			DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 7 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
			return FALSE;
		}
		gMercProfiles[ iProfileId ].CopyOldInventoryToNew();

		if (guiSavedGameVersion >= SAVE_GAMES_HAVE_NEW_INVENTORY_VECTORS) {
			if (!LoadVector (hFile, gMercProfiles[ iProfileId ].inv)) { return FALSE; }
			if (!LoadVector (hFile, gMercProfiles[ iProfileId ].bInvStatus)) { return FALSE; }
			if (!LoadVector (hFile, gMercProfiles[ iProfileId ].bInvNumber)) { return FALSE; }
		}

		// close file
		FileClose(hFile);
		
		if( LaptopSaveInfo.iCurrentBalance < COST_OF_PROFILE )
		{
			DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 3 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);

			// not enough
			return FALSE;
		}
		

		// charge the player
		// is the character male?
		fCharacterIsMale = ( gMercProfiles[ iProfileId ].bSex == MALE );
		fLoadingCharacterForPreviousImpProfile = TRUE;
		AddTransactionToPlayersBook(IMP_PROFILE,0, GetWorldTotalMin( ), - ( COST_OF_PROFILE ) );
		AddHistoryToPlayersLog( HISTORY_CHARACTER_GENERATED, 0,GetWorldTotalMin( ), -1, -1 );
		LaptopSaveInfo.iIMPIndex = iProfileId;
		AddCharacterToPlayersTeam( );
		// WANNE: Email is sent immediatly after the imp was created. So no need to send it later again
		//AddFutureDayStrategicEvent( EVENT_DAY2_ADD_EMAIL_FROM_IMP, 60 * 7, 0, 2 );
		LaptopSaveInfo.fIMPCompletedFlag = TRUE;
		fPausedReDrawScreenFlag = TRUE;
		fLoadingCharacterForPreviousImpProfile = FALSE;

		return TRUE;
	}
	else
	{
		// close file
		FileClose(hFile);

		// WDS: Allow flexible numbers of IMPs of each sex
		//  note: check this

		// You cannot have more than 3 I.M.P characters with the same gender on your team.
		DoLapTopMessageBox( MSG_BOX_IMP_STYLE, pImpPopUpStrings[ 9 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return FALSE;
	}
}



void ResetIMPCharactersEyesAndMouthOffsets( UINT8 ubMercProfileID )
{
  // ATE: Check boundary conditions!
  if( ( ( gMercProfiles[ ubMercProfileID ].ubFaceIndex - 200 ) > 16 ) || ( ubMercProfileID >= PROF_HUMMER ) )
  {
    return;
  }

	gMercProfiles[ ubMercProfileID ].usEyesX = uiEyeXPositions[ gMercProfiles[ ubMercProfileID ].ubFaceIndex - 200 ];
	gMercProfiles[ ubMercProfileID ].usEyesY = uiEyeYPositions[ gMercProfiles[ ubMercProfileID ].ubFaceIndex - 200  ];


	gMercProfiles[ ubMercProfileID ].usMouthX = uiMouthXPositions[ gMercProfiles[ ubMercProfileID ].ubFaceIndex - 200  ];
	gMercProfiles[ ubMercProfileID ].usMouthY = uiMouthYPositions[ gMercProfiles[ ubMercProfileID ].ubFaceIndex - 200  ];
}


void GiveIMPRandomItems( MERCPROFILESTRUCT *pProfile, UINT8 typeIndex )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPRandomItems: typeIndex = %d",typeIndex ));

	UINT16 usItem = 0;
	INT32 iChoice;

	if ( gIMPItemChoices[ typeIndex ].ubChoices <= 0 )
		return;

	for ( int i=0; i < gIMPItemChoices[typeIndex].ubNumItems ;i++ )
	{
		while (usItem == 0 )
		{
			iChoice = Random(gIMPItemChoices[ typeIndex ].ubChoices);
			usItem = gIMPItemChoices[ typeIndex ].bItemNo[ iChoice ];
			if (!ItemIsLegal(usItem))
				usItem=0;
		}
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPRandomItems: typeIndex = %d, usItem = %d ",typeIndex, usItem ));
		if ( usItem > 0 )
			MakeProfileInvItemAnySlot(pProfile,usItem,100,1);

		// give ammo for guns
		if ( Item[usItem].usItemClass == IC_GUN && !Item[usItem].rocketlauncher )
		{
			usItem = DefaultMagazine(usItem);
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPRandomItems: give ammo typeIndex = %d, usItem = %d ",typeIndex, usItem ));
			MakeProfileInvItemAnySlot(pProfile,usItem,100,(1+Random(1)));
		}

		// give launchables for launchers
		if ( Item[usItem].usItemClass == IC_LAUNCHER )
		{
			usItem = PickARandomLaunchable(usItem);
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPRandomItems: give launchable typeIndex = %d, usItem = %d",typeIndex, usItem ));
			MakeProfileInvItemAnySlot(pProfile,usItem,100,(2+Random(2)));
		}
	}

}

void GiveIMPItems( MERCPROFILESTRUCT *pProfile, INT8 abilityValue, UINT8 typeIndex )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: typeIndex = %d, abilityValue = %d",typeIndex,abilityValue ));

	UINT16 usItem = 0;
	INT32 iChoice;

	if ( gIMPItemChoices[ typeIndex ].ubChoices <= 0 )
		return;

	iChoice = (UINT32) (abilityValue / (100 / gIMPItemChoices[ typeIndex ].ubChoices)) ;

	if ( iChoice >= gIMPItemChoices[ typeIndex ].ubChoices )
		iChoice = gIMPItemChoices[ typeIndex ].ubChoices - 1;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: 1 iChoice = %d",iChoice));
	for ( int i=0; i < gIMPItemChoices[typeIndex].ubNumItems ;i++ )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: 2 iChoice = %d",iChoice));
		usItem = gIMPItemChoices[ typeIndex ].bItemNo[ iChoice ];
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: typeIndex = %d, usItem = %d, iChoice = %d, abilityValue = %d",typeIndex, usItem,iChoice, abilityValue ));
		if ( usItem > 0 && ItemIsLegal(usItem))
		{
			MakeProfileInvItemAnySlot(pProfile,usItem,100,1);


			// give ammo for guns
			if ( Item[usItem].usItemClass == IC_GUN && !Item[usItem].rocketlauncher )
			{
				usItem = DefaultMagazine(usItem);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: give ammo typeIndex = %d, usItem = %d",typeIndex, usItem ));
				MakeProfileInvItemAnySlot(pProfile,usItem,100,(2+Random(2)));
			}

			// give launchables for launchers
			if ( Item[usItem].usItemClass == IC_LAUNCHER )
			{
				usItem = PickARandomLaunchable(usItem);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("GiveIMPItems: give launchable typeIndex = %d, usItem = %d",typeIndex, usItem ));
				MakeProfileInvItemAnySlot(pProfile,usItem,100,(2+Random(2)));
			}
		}
		iChoice--;
		if (iChoice < 0)
			iChoice = 0;
	}


}


