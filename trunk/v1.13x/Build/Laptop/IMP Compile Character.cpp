#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "GameSettings.h"
#else
	#include "laptop.h"
	#include "CharProfile.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "GameSettings.h"
	#include "IMP HomePage.h"
	#include "IMPVideoObjects.h"
	#include "IMP AboutUs.h"
	#include "IMP MainPage.h"
	#include "IMP Begin Screen.h"
	#include "IMP Personality Entrance.h"
	#include "IMP Personality Quiz.h"
	#include "IMP Personality Finish.h"
	#include "IMP Skill Trait.h"
	#include "IMP Attribute Entrance.h"
	#include "IMP Attribute Selection.h"
	#include "IMP Attribute Finish.h"
	#include "IMP Portraits.h"
	#include "IMP Compile Character.h"
	#include "IMP Voices.h"
	#include "IMP Finish.h"
	#include "Soldier Profile Type.h"
	#include "Soldier Profile.h"
	#include "Animation Data.h"
	#include "random.h"
#endif

// how many times should a 'die' be rolled for skills of the same type?

#define HOW_MANY_ROLLS_FOR_SAME_SKILL_CHECK 20

INT32 AttitudeList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInAttitudeList = 0;

INT32 SkillsList[ ATTITUDE_LIST_SIZE ];
INT32 BackupSkillsList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInSkillsList = 0;

INT32 PersonalityList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInPersonalityList = 0;

extern BOOLEAN fLoadingCharacterForPreviousImpProfile;

// positions of the face x and y for eyes and mouth for the 10 portraits
INT16 sFacePositions[ NUMBER_OF_PLAYER_PORTRAITS ][ 4 ]=
{
  {0,0,0,0},
  {0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
  {0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
};

STR8 pPlayerSelectedFaceFileNames[ NUMBER_OF_PLAYER_PORTRAITS ]=
{
	"Faces\\200.sti",
	"Faces\\201.sti",
	"Faces\\202.sti",
	"Faces\\203.sti",
	"Faces\\204.sti",
	"Faces\\205.sti",
	"Faces\\206.sti",
	"Faces\\207.sti",
	"Faces\\208.sti",
	"Faces\\209.sti",
	"Faces\\210.sti",
	"Faces\\211.sti",
	"Faces\\212.sti",
	"Faces\\213.sti",
	"Faces\\214.sti",
	"Faces\\215.sti",
};

STR8 pPlayerSelectedBigFaceFileNames[ NUMBER_OF_PLAYER_PORTRAITS ]=
{
	"Faces\\BigFaces\\200.sti",
	"Faces\\BigFaces\\201.sti",
	"Faces\\BigFaces\\202.sti",
	"Faces\\BigFaces\\203.sti",
	"Faces\\BigFaces\\204.sti",
	"Faces\\BigFaces\\205.sti",
	"Faces\\BigFaces\\206.sti",
	"Faces\\BigFaces\\207.sti",
	"Faces\\BigFaces\\208.sti",
	"Faces\\BigFaces\\209.sti",
	"Faces\\BigFaces\\210.sti",
	"Faces\\BigFaces\\211.sti",
	"Faces\\BigFaces\\212.sti",
	"Faces\\BigFaces\\213.sti",
	"Faces\\BigFaces\\214.sti",
	"Faces\\BigFaces\\215.sti",
};


// function declarations
void SelectMercFace( void );
void SetMercSkinAndHairColors( void );
BOOLEAN ShouldThisMercHaveABigBody( void );





void CreateACharacterFromPlayerEnteredStats( void )
{

		// Kaiden: Seems like as good a place as any to stash this function call to
		// ensure that these lists don't get overwritten or Nulled due to the amount
		// of changes and revisions that have been made to personalities and attitudes.
	CreatePlayersPersonalitySkillsAndAttitude();



	// copy over full name
  wcscpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].zName, pFullName );

	// the nickname
	wcscpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].zNickname, pNickName ); 
  
	// gender 
	if ( fCharacterIsMale == TRUE )
	{
    // male
    gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bSex = MALE;
	}
	else
	{
		// female
    gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bSex = FEMALE;
	}


	// attributes
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bLifeMax		= ( INT8 )iHealth;
  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bLife       = ( INT8 )iHealth;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bAgility    = ( INT8 )iAgility;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bStrength   = ( INT8 )iStrength;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bDexterity  = ( INT8 )iDexterity;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bWisdom     = ( INT8 )iWisdom; 
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bLeadership = ( INT8 )iLeadership;


		// skills
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bMarksmanship = ( INT8 )iMarksmanship;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bMedical      = ( INT8 )iMedical;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bMechanical   = ( INT8 )iMechanical;
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bExplosive    = ( INT8 )iExplosives;


  // personality
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bPersonalityTrait = ( INT8 )iPersonality;
  
	// attitude
  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bAttitude = ( INT8 )iAttitude; 
  
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bExpLevel = 1;
  
	// set time away
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bMercStatus = 0;

	
  
	// face
	 SelectMercFace( );

	 return;

}


BOOLEAN DoesCharacterHaveAnAttitude( void )
{

	// simply checks if caracter has an attitude other than normal
	switch( iAttitude )
	{
		case ATT_LONER:
		case ATT_PESSIMIST:
		case ATT_ARROGANT:
		case ATT_BIG_SHOT:
		case ATT_ASSHOLE:
		case ATT_COWARD:
			return( TRUE );
		default:
			return( FALSE );
	}
}


BOOLEAN DoesCharacterHaveAPersoanlity( void )
{


	  if( iPersonality != NO_PERSONALITYTRAIT )
	{
		// yep
	  return ( TRUE );
	}
	else
	{
		// nope
		return ( FALSE );
	}


	// Kaiden: Following is the original UB section:
	// The fist return( FALSE ); Statement was the only
	// thing uncommented.
/*
	// only one we can get is PSYCHO, and that is not much of a penalty
	return( FALSE );

	// simply checks if caracter has a personality other than normal
  if( iPersonality != NO_PERSONALITYTRAIT )
	{
		// yep
	  return ( TRUE );
	}
	else
	{
		// nope
		return ( FALSE );
	}
	*/


}

void CreatePlayerAttitude( void )
{

	if(gGameExternalOptions.fPers_att)
		{
			AddAnAttitudeToAttitudeList( ATT_OPTIMIST );
			AddAnAttitudeToAttitudeList(  ATT_LONER );
			AddAnAttitudeToAttitudeList(  ATT_FRIENDLY );
			AddAnAttitudeToAttitudeList( ATT_ARROGANT );
			AddAnAttitudeToAttitudeList( ATT_NORMAL );
			AddAnAttitudeToAttitudeList( ATT_ASSHOLE );
			AddAnAttitudeToAttitudeList( ATT_COWARD );
			AddAnAttitudeToAttitudeList( ATT_AGGRESSIVE );
			AddAnAttitudeToAttitudeList( ATT_PESSIMIST );
			AddAnAttitudeToAttitudeList( ATT_BIG_SHOT );	

			// this function will 'roll a die' and decide if any attitude does exists
			INT32 iDiceValue = 0;
			INT32 iCounter = 0, iCounter2 = 0;

			INT32	iAttitudeHits[NUM_ATTITUDES] = { 0 };
			INT32	iHighestHits = 0;
			INT32	iNumAttitudesWithHighestHits = 0;

			iAttitude = ATT_NORMAL;

			if ( iLastElementInAttitudeList == 0 )
			{
				return;
			}

			// count # of hits for each attitude
			for ( iCounter = 0; iCounter < iLastElementInAttitudeList; iCounter++ )
			{
				iAttitudeHits[ AttitudeList[ iCounter ] ]++;
			}

			// find highest # of hits for any attitude
			for ( iCounter = 0; iCounter < NUM_ATTITUDES; iCounter++ )
			{
				if ( iAttitudeHits[ iCounter ] )
				{
					if ( iAttitudeHits[ iCounter ] > iHighestHits )
					{
						iHighestHits = __max( iHighestHits, iAttitudeHits[ iCounter ] );
						iNumAttitudesWithHighestHits = 1;
					}
					else if ( iAttitudeHits[ iCounter ] == iHighestHits )
					{
						iNumAttitudesWithHighestHits++;
					}
				}
			}

			// roll dice
			iDiceValue = Random( iNumAttitudesWithHighestHits + 1 );

			// find attitude
			for ( iCounter = 0; iCounter < NUM_ATTITUDES; iCounter++ )
			{
				if ( iAttitudeHits[ iCounter ] == iHighestHits )
				{
					if ( iCounter2 == iDiceValue )
					{
						// this is it!
						iAttitude = iCounter2;
						break;
					}
					else
					{
						// one of the next attitudes...
						iCounter2++;
					}
				}
			}
		}
		else
		{
			iAttitude =	gGameExternalOptions.iCustomAttitude;
		}
}


void AddAnAttitudeToAttitudeList( INT8 bAttitude )
{
	// adds an attitude to attitude list

	if( iLastElementInAttitudeList < ATTITUDE_LIST_SIZE)
	{
		// add element
		AttitudeList[ iLastElementInAttitudeList ] = ( INT32 )bAttitude;
    
		// increment attitude list counter	
		iLastElementInAttitudeList++;
	}

	return;
}


void AddSkillToSkillList( INT8 bSkill )
{
  // adds a skill to skills list

  if( iLastElementInSkillsList < ATTITUDE_LIST_SIZE)
	{
		// add element
		SkillsList[ iLastElementInSkillsList ] = ( INT32 )bSkill;
    
		// increment attitude list counter	
		iLastElementInSkillsList++;
	}

	return;
}

// Kaiden : Added to clear the list when first visiting the IMP homepage,
// Or for each visit there after.
void ClearAllSkillsList( void )
{
	INT32		iLoop;

	for ( iLoop = 0; iLoop < iLastElementInSkillsList; iLoop++ )
	{
		SkillsList[ iLoop ] = 0;
	}
	iLastElementInSkillsList = 0;
	return;
}



void RemoveSkillFromSkillsList( INT32 iIndex )
{	
	INT32		iLoop;

	// remove a skill from the index given and shorten the list
	if ( iIndex < iLastElementInSkillsList )
	{
		memset( BackupSkillsList, 0, ATTITUDE_LIST_SIZE * sizeof( INT32 ) );

		// use the backup array to create a version of the array without
		// this index
		for ( iLoop = 0; iLoop < iIndex; iLoop++ )
		{
			BackupSkillsList[ iLoop ] = SkillsList[ iLoop ];
		}
		for ( iLoop = iIndex + 1; iLoop < iLastElementInSkillsList; iLoop++ )
		{
			BackupSkillsList[ iLoop - 1 ] = SkillsList[ iLoop ];
		}
		// now copy this over to the skills list
		memcpy( SkillsList, BackupSkillsList, ATTITUDE_LIST_SIZE * sizeof( INT32 ) );

		// reduce recorded size by 1
		iLastElementInSkillsList--;
	}
}

INT32	FindSkillInSkillsList( INT32 iSkill )
{
	INT32		iLoop;

	for ( iLoop = 0; iLoop < iLastElementInSkillsList; iLoop++ )
	{
		if ( SkillsList[ iLoop ] == iSkill )
		{
			return( iLoop );
		}
	}

	return( -1 );
}

void ValidateSkillsList( void )
{
	INT32	iIndex;//, iValue;
	MERCPROFILESTRUCT * pProfile;

	// remove from the generated traits list any traits that don't match
	// the character's skills
	pProfile = &(gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ]);
	if ( pProfile->bMechanical == 0 )
	{
		// without mechanical, electronics is useless
		iIndex = FindSkillInSkillsList( ELECTRONICS );
		if ( iIndex != -1 )
		{
			pProfile->bMechanical = 1;
		}
	}

        // Kaiden using UB's code, lockpicking can't be removed from the list "yet".
	// special check for lockpicking
	//iValue = pProfile->bMechanical;
	//iValue = ( iValue * pProfile->bWisdom ) / 100;
	//iValue = ( iValue * pProfile->bDexterity ) / 100;
	//if ( iValue + gbSkillTraitBonus[ LOCKPICKING ] < 50 )
	//{
	//	// not good enough for lockpicking!
	//
	//	// so is lockpicking
	//	iIndex = FindSkillInSkillsList( LOCKPICKING );
	//	while ( iIndex != -1 )
	//	{
	//		RemoveSkillFromSkillsList( iIndex );
	//		iIndex = FindSkillInSkillsList( LOCKPICKING );
	//	}
	//}

	if ( pProfile->bMarksmanship == 0 )
	{
		// without marksmanship, the following traits are useless:
		// auto weapons, heavy weapons
		iIndex = FindSkillInSkillsList( AUTO_WEAPS );
		if( iIndex != -1 )
		{
			//RemoveSkillFromSkillsList( iIndex );
			//iIndex = FindSkillInSkillsList( AUTO_WEAPS );
			pProfile->bMarksmanship = 1;
		}
		// so is lockpicking
		iIndex = FindSkillInSkillsList( HEAVY_WEAPS );
		if( iIndex != -1 )
		{
			//RemoveSkillFromSkillsList( iIndex );
			//iIndex = FindSkillInSkillsList( HEAVY_WEAPS );
			pProfile->bMarksmanship = 1;
		}
	}
}

void CreatePlayerSkills( void )
{

  // this function will 'roll a die' and decide if any attitude does exists
  INT32 iDiceValue = 0;
	INT32 iCounter = 0;

	ValidateSkillsList();
/* Kaiden - Commented out in UB to skip the whole random skill thing
	// roll dice
	iDiceValue = Random( iLastElementInSkillsList );

	// set attitude
	iSkillA = SkillsList[ iDiceValue ];

	// second dice value
	iDiceValue = Random( iLastElementInSkillsList );

	iSkillB = SkillsList[ iDiceValue ];
*/
  
        // Kaiden: This section was added in it's place:
	if( iLastElementInSkillsList > 0 )
	{
		// set attitude
		iSkillA = SkillsList[ 0 ];
	}

	if( iLastElementInSkillsList > 1 )
	{
		iSkillB = SkillsList[ 1 ];
	}
	else
	{
		iSkillB = SkillsList[ 0 ];
	}

	// allow expert level for generated merc so you CAN have two of the same
	// but there is no such thing as expert level for electronics


	while ( iSkillA == iSkillB && ( iSkillB == ELECTRONICS || iSkillB == AMBIDEXT ) )
	{
		// remove electronics as an option and roll again
		RemoveSkillFromSkillsList( iDiceValue );
		if ( iLastElementInSkillsList == 0 )
		{
			// ok, only one trait!
			iSkillB = NO_SKILLTRAIT;
			break;
		}
		else
		{
			iDiceValue = Random( iLastElementInSkillsList );
			iSkillB = SkillsList[ iDiceValue ];
		}
	}

	/*
	// are the same, 
  // reroll until different, or until ATTITUDE_LIST_SIZE times
	iSkillB = Random( iLastElementInSkillsList + 1 );

	while( (iSkillA == iSkillB ) && ( iCounter < HOW_MANY_ROLLS_FOR_SAME_SKILL_CHECK ) )
	{
		// increment counter
		iCounter++;

		// next random 
    iSkillB = Random( iLastElementInSkillsList + 1 );

	}
  if( iCounter == ATTITUDE_LIST_SIZE ) 
	{
    iSkillB = NO_SKILLTRAIT;
	}
	return;


	*/
}


void AddAPersonalityToPersonalityList( INT8 bPersonlity )
{
	// CJC, Oct 26 98: prevent personality list from being generated
	// because no dialogue was written to support PC personality quotes
	
	// BUT we can manage this for PSYCHO okay

	//Kaiden: But we're going to try it anyway screw dialoge.
	// Commenting out the below if test

	//if ( bPersonlity != PSYCHO )
	//{
	//	return;
	//}

  // will add a persoanlity to persoanlity list
  if( iLastElementInPersonalityList < ATTITUDE_LIST_SIZE)
	{
		// add element
		PersonalityList[ iLastElementInPersonalityList ] = ( INT32 )bPersonlity;
    
		// increment attitude list counter	
		iLastElementInPersonalityList++;
	}

	return;
}

void CreatePlayerPersonality( void )
{

	// Kaiden: Added for optional Mercenary personalities and attitudes

	if(gGameExternalOptions.fPers_att)
		{
				// Kaiden: More chances for Psycho and Normal.
			AddAPersonalityToPersonalityList( NO_PERSONALITYTRAIT );
			AddAPersonalityToPersonalityList( PSYCHO );
			AddAPersonalityToPersonalityList( FORGETFUL );
			AddAPersonalityToPersonalityList( NERVOUS );
			AddAPersonalityToPersonalityList( HEAT_INTOLERANT );
			AddAPersonalityToPersonalityList( NO_PERSONALITYTRAIT );
			AddAPersonalityToPersonalityList( PSYCHO );
			AddAPersonalityToPersonalityList( CLAUSTROPHOBIC );
			AddAPersonalityToPersonalityList( NONSWIMMER );
			AddAPersonalityToPersonalityList( FEAR_OF_INSECTS );
			AddAPersonalityToPersonalityList( NO_PERSONALITYTRAIT );
			AddAPersonalityToPersonalityList( PSYCHO );			
			AddAPersonalityToPersonalityList( FORGETFUL );
			AddAPersonalityToPersonalityList( NERVOUS );
			AddAPersonalityToPersonalityList( HEAT_INTOLERANT );
			AddAPersonalityToPersonalityList( NO_PERSONALITYTRAIT );
			AddAPersonalityToPersonalityList( PSYCHO );
			AddAPersonalityToPersonalityList( CLAUSTROPHOBIC );
			AddAPersonalityToPersonalityList( NONSWIMMER );
			AddAPersonalityToPersonalityList( FEAR_OF_INSECTS );


			INT32 iDiceValue = 0;
			INT32 iCounter = 0;
			INT32 iCounter2 = 0;

			// Kaiden: Roll dice 20 times just to be on the safe side 
			// was getting too many repeats. Will end up scrapping
			// rand() later anyway so will worry about fixing it then.
			for (iCounter2 = 0; iCounter < 20; iCounter2++)
			{
				iDiceValue = Random( iLastElementInPersonalityList + 1 );
			}

			// Kaiden two chances to avoid a normal personality. As IMP
			// says, They check it twice just to make sure :p
  			for( iCounter = 0; iCounter < iLastElementInPersonalityList; iCounter++ )
				{

					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("iDiceValue = %d",iDiceValue));

					if( PersonalityList[ iDiceValue ] =  NO_PERSONALITYTRAIT )
						{
							//Kaiden: Roll one more time for good measure:
							iDiceValue = Random( iLastElementInPersonalityList + 1 );
						}
				}
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("And our Personality is... = %d",PersonalityList[ iDiceValue ]));
				iPersonality = PersonalityList[ iDiceValue ];
		}
		else
		{
			iPersonality = gGameExternalOptions.iCustomPersonality;
		}
	return;

}


void CreatePlayersPersonalitySkillsAndAttitude( void )
{



  
	// creates personality, skills and attitudes from curretly built list

	// personality
	CreatePlayerPersonality( );

	// skills are now created later after stats have been chosen
  //CreatePlayerSkills( );

	// attitude
	CreatePlayerAttitude( );

	return;
}


void ResetSkillsAttributesAndPersonality( void )
{

	// reset count of skills attributes and personality
  
	iLastElementInPersonalityList = 0;

	iLastElementInSkillsList = 0;

	iLastElementInAttitudeList = 0;

}


void ResetIncrementCharacterAttributes( void )
{
	// this resets any increments due to character generation
	
	// attributes
	iAddStrength = 0;
	iAddDexterity = 0;
	iAddWisdom = 0;
	iAddAgility = 0;
	iAddHealth = 0;
	iAddLeadership = 0;

  // skills
	iAddMarksmanship = 0;
	iAddExplosives = 0;
	iAddMedical = 0;
	iAddMechanical = 0;

}

void SelectMercFace( void )
{
	// this procedure will select the approriate face for the merc and save offsets

	// grab face filename
//  strcpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].ubUnusedFaceFileName , pPlayerSelectedFaceFileNames[ iPortraitNumber ]);

	// now the offsets
  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].ubFaceIndex = 200 + ( UINT8 )iPortraitNumber;

	// eyes
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].usEyesX = sFacePositions[ iPortraitNumber ][ 0 ];
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].usEyesY = sFacePositions[ iPortraitNumber ][ 1 ];
	
	// mouth
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].usMouthX = sFacePositions[ iPortraitNumber ][ 2 ];
	gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].usMouthY = sFacePositions[ iPortraitNumber ][ 3 ];

	// set merc skins and hair color
	SetMercSkinAndHairColors( );
	return;
}

void SetMercSkinAndHairColors( void )
{
	enum{ PINKSKIN, TANSKIN, DARKSKIN, BLACKSKIN, NUMSKINS };
	enum{ BROWNHEAD, BLACKHEAD, //black skin (only this line )
		    WHITEHEAD,						//dark skin (this line plus all above)
				BLONDHEAD, REDHEAD,   //pink/tan skin (this line plus all above )
				NUMHEADS  
			};

	enum
	{
		WHITEVEST,
		GYELLOWSHIRT,
		YELLOWVEST,
		GREYVEST,
		BROWNVEST,
		PURPLESHIRT,
		BLUEVEST,
		JEANVEST,
		GREENVEST,
		REDVEST,
		BLACKSHIRT,
		NUMSHIRTS
	};//shirts

	enum
	{
		BLUEPANTS,
		BLACKPANTS,
		JEANPANTS,
		TANPANTS,
		BEIGEPANTS,
		GREENPANTS,
		NUMPANTS
	};//pants

	// skin strings
	STR sSkinStrings[]={
		"PINKSKIN",
		"TANSKIN",
		"DARKSKIN",
		"BLACKSKIN",
	};

	// the hair colors
	STR sHairStrings[]={
		"BROWNHEAD",
		"BLACKHEAD",
		"WHITEHEAD",
		"BLONDHEAD",
		"REDHEAD",
	};

	// shirts
	STR sShirtStrings[]={
		"WHITEVEST",
		"GYELLOWSHIRT",
		"YELLOWVEST",
		"greyVEST",
		"BROWNVEST",
		"PURPLESHIRT",
		"BLUEVEST",
		"JEANVEST",
		"GREENVEST",
		"REDVEST",
		"BLACKSHIRT",
	};//shirts


	// shirts
	STR sPantStrings[]={
		"BLUEPANTS",
		"BLACKPANTS",
		"JEANPANTS",
		"TANPANTS",
		"BEIGEPANTS",
		"GREENPANTS",
	};//pants

	// given the portrait number, set the merc's skin and hair color
	INT16 sSkinColor = 0, sHairColor = 0, sShirtColor, sPantColor;

	switch( iPortraitNumber )
	{
		case( 0 ):
			sSkinColor = BLACKSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = JEANVEST;
			sPantColor = JEANPANTS;
		break;
		case( 1 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = TANPANTS;
		break;
		case( 2 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = WHITEVEST;
			sPantColor = BLUEPANTS;
		break;
		case( 3 ):
			sSkinColor = DARKSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = WHITEVEST;
			sPantColor = BLACKPANTS;
		break;
		case( 4 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = GREYVEST;
			sPantColor = GREENPANTS;
		break;
		case( 5 ):
			sSkinColor = DARKSKIN;
			sHairColor = BLACKHEAD;
			sShirtColor = GREYVEST;
			sPantColor = BLACKPANTS;
			break;
		case( 6 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = BLUEPANTS;
			break;
		case( 7 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = BLACKSHIRT;
			sPantColor = BLACKPANTS;
			break;
		case( 8 ):
			sSkinColor = TANSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = TANPANTS;
		break;
		case( 9 ):
			sSkinColor = PINKSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = BEIGEPANTS;
		break;
		case( 10 ):
			sSkinColor = TANSKIN;
			sHairColor = BLACKHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = BLACKPANTS;
		break;
		case( 11 ):
			sSkinColor = TANSKIN;
			sHairColor = BLACKHEAD;
			sShirtColor = BLACKSHIRT;
			sPantColor = BLACKPANTS;
		break;
		case( 12 ):
			sSkinColor = PINKSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = WHITEVEST;
			sPantColor = GREENPANTS;
		break;
		case( 13 ):
			sSkinColor = BLACKSKIN;
			sHairColor = BROWNHEAD;
			sShirtColor = GREYVEST;
			sPantColor = BLACKPANTS;
			break;
		case( 14 ):
			sSkinColor = TANSKIN;
			sHairColor = REDHEAD;
			sShirtColor = WHITEVEST;
			sPantColor = JEANPANTS;
			break;
		case( 15 ):
			sSkinColor = TANSKIN;
			sHairColor = BLONDHEAD;
			sShirtColor = BROWNVEST;
			sPantColor = GREENPANTS;
			break;
	}

	// now set them
	strcpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].HAIR, sHairStrings[ sHairColor ] );
	strcpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].SKIN, sSkinStrings[ sSkinColor ] );
	strcpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].PANTS, sPantStrings[ sPantColor ] );
	strcpy( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].VEST, sShirtStrings[ sShirtColor ] );

}


void HandleMercStatsForChangesInFace( )
{
	if ( fLoadingCharacterForPreviousImpProfile )
	{
		return;
	}

	//add the skills to the skills list
	AddSelectedSkillsToSkillsList();

	// now figure out skills
	CreatePlayerSkills();

	// body type
	if ( fCharacterIsMale  )
	{
    // male
		// big or regular
		// Madd - don't override the skills - override the body type instead
		if( ShouldThisMercHaveABigBody() && iSkillA != MARTIALARTS && iSkillB != MARTIALARTS )
		{
			gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].ubBodyType = BIGMALE; 
			
			//if( iSkillA == MARTIALARTS )
			//{
			//	iSkillA = HANDTOHAND;
			//}
			//if( iSkillB == MARTIALARTS )
			//{
			//	iSkillB = HANDTOHAND;
			//}
		}
		else
		{
		  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].ubBodyType = REGMALE; 
    }
	}
	else
	{
     // female
	  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].ubBodyType = REGFEMALE;
		
		if( iSkillA == MARTIALARTS )
		{
			iSkillA = HANDTOHAND;
		}
		if( iSkillB == MARTIALARTS )
		{
			iSkillB = HANDTOHAND;
		}
	}


	// skill trait
  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bSkillTrait =  ( INT8 )iSkillA;
  gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bSkillTrait2 = ( INT8 )iSkillB;

}

BOOLEAN ShouldThisMercHaveABigBody( void )
{
	// should this merc be a big body type
	// Madd - don't limit it by portrait
	//if ( ( iPortraitNumber == 0 ) || ( iPortraitNumber == 6 ) || ( iPortraitNumber == 7 ) )
	//{
		if ( gMercProfiles[ PLAYER_GENERATED_CHARACTER_ID + LaptopSaveInfo.iVoiceId ].bStrength >= 75 )
		{
			return( TRUE );
		}
	//}

	return( FALSE );
}