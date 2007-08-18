#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "Types.h"
	#include "stdlib.h"
	#include "Arms Dealer Init.h"
	#include "String.h"
	#include "Debug.h"
	#include "Random.h"
	#include "Weapons.h"
	#include "FileMan.h"
	#include "Game Clock.h"
	#include "ArmsDealerInvInit.h"
	#include "Message.h"
	#include "soldier profile.h"
	#include "Handle Items.h"
	#include "Quests.h"
	#include "Scheduling.h"
#endif

#include <list>

#define	MIN_REPAIR_TIME_IN_MINUTES				15		// minutes
#define MIN_REPAIR_COST										10		// dollars

// price classes
#define PRICE_CLASS_JUNK									0
#define PRICE_CLASS_CHEAP									1
#define PRICE_CLASS_EXPENSIVE							2

void ConvertCreatureBloodToElixir( void );

UINT8 gubLastSpecialItemAddedAtElement = 255;


// THIS STRUCTURE HAS UNCHANGING INFO THAT DOESN'T GET SAVED/RESTORED/RESET
// TODO: externalize 
ARMS_DEALER_INFO	ArmsDealerInfo[ NUM_ARMS_DEALERS ] = 
{
									//Buying		Selling	Merc ID#	Type									Initial						Flags	
									//Price			Price							Of											Cash	
									//Modifier	Modifier					Dealer

/* Tony	*/					{ 0.75f,	1.25f,	TONY,			ARMS_DEALER_BUYS_SELLS, 15000,	ARMS_DEALER_SOME_USED_ITEMS | ARMS_DEALER_GIVES_CHANGE	},
/* Franz Hinkle */	{ 1.0f,		1.5f,		FRANZ,		ARMS_DEALER_BUYS_SELLS,	5000,	ARMS_DEALER_SOME_USED_ITEMS | ARMS_DEALER_GIVES_CHANGE	},
/* Keith Hemps */		{ 0.75f,	1.0f,		KEITH,		ARMS_DEALER_BUYS_SELLS,	1500,	ARMS_DEALER_ONLY_USED_ITEMS | ARMS_DEALER_GIVES_CHANGE	},
/* Jake Cameron */	{ 0.8f,		1.1f,		JAKE,			ARMS_DEALER_BUYS_SELLS,	2500,	ARMS_DEALER_ONLY_USED_ITEMS | ARMS_DEALER_GIVES_CHANGE	},
/* Gabby Mulnick*/	{ 1.0f,		1.0f,		GABBY,		ARMS_DEALER_BUYS_SELLS,	3000,	ARMS_DEALER_GIVES_CHANGE																},

/* Devin Connell*/	{ 0.75f,	1.25f,	DEVIN,		ARMS_DEALER_SELLS_ONLY,	5000,	ARMS_DEALER_GIVES_CHANGE																},
/* Howard Filmore*/	{ 1.0f,		1.0f,		HOWARD,		ARMS_DEALER_SELLS_ONLY,	3000,	ARMS_DEALER_GIVES_CHANGE																},
/* Sam Rozen */			{ 1.0f,		1.0f,		SAM,			ARMS_DEALER_SELLS_ONLY,	3000,	ARMS_DEALER_GIVES_CHANGE																},
/* Frank */					{ 1.0f,		1.0f,		FRANK,		ARMS_DEALER_SELLS_ONLY,	500,	ARMS_DEALER_ACCEPTS_GIFTS																},

/* Bar Bro 1 */			{ 1.0f,		1.0f,		HERVE,		ARMS_DEALER_SELLS_ONLY,	250,	ARMS_DEALER_ACCEPTS_GIFTS																},
/* Bar Bro 2 */			{ 1.0f,		1.0f,		PETER,		ARMS_DEALER_SELLS_ONLY,	250,	ARMS_DEALER_ACCEPTS_GIFTS																},
/* Bar Bro 3 */			{ 1.0f,		1.0f,		ALBERTO,	ARMS_DEALER_SELLS_ONLY,	250,	ARMS_DEALER_ACCEPTS_GIFTS																},
/* Bar Bro 4 */			{ 1.0f,		1.0f,		CARLO,		ARMS_DEALER_SELLS_ONLY,	250,	ARMS_DEALER_ACCEPTS_GIFTS																},

/* Micky O'Brien*/	{ 1.0f,		1.4f,		MICKY,		ARMS_DEALER_BUYS_ONLY, 10000,	ARMS_DEALER_HAS_NO_INVENTORY | ARMS_DEALER_GIVES_CHANGE },

										//Repair	Repair
										//Speed		Cost
/* Arnie Brunzwell*/{ 0.1f,		0.8f,		ARNIE,		ARMS_DEALER_REPAIRS,		1500,	ARMS_DEALER_HAS_NO_INVENTORY | ARMS_DEALER_GIVES_CHANGE	},
/* Fredo */					{ 0.6f,		0.6f,		FREDO,		ARMS_DEALER_REPAIRS,		1000,	ARMS_DEALER_HAS_NO_INVENTORY | ARMS_DEALER_GIVES_CHANGE },
/* Perko */					{ 1.0f,		0.4f,	PERKO,		ARMS_DEALER_REPAIRS,		1000,	ARMS_DEALER_HAS_NO_INVENTORY | ARMS_DEALER_GIVES_CHANGE },

/* Elgin */					{ 1.0f,		1.0f,	DRUGGIST,	ARMS_DEALER_SELLS_ONLY,	500,	ARMS_DEALER_ACCEPTS_GIFTS																},
/* Manny */					{ 1.0f,		1.0f,	MANNY,		ARMS_DEALER_SELLS_ONLY,	500,	ARMS_DEALER_ACCEPTS_GIFTS																},

};


// THESE GET SAVED/RESTORED/RESET
ARMS_DEALER_STATUS	gArmsDealerStatus[ NUM_ARMS_DEALERS ];
DealerItemList	gArmsDealersInventory[ NUM_ARMS_DEALERS ];




void		AddAmmoToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubShotsLeft );
void		AddItemToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, INT8 objectStatus, UINT8 ubHowMany );
void		AddSpecialItemToArmsDealerInventoryAtElement( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubElement, SPECIAL_ITEM_INFO *pSpclItemInfo );

void		RemoveRandomItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );

void		DailyCheckOnItemQuantities();
void		SimulateArmsDealerCustomer();

BOOLEAN AdjustCertainDealersInventory();
void		LimitArmsDealersInventory( UINT8 ubArmsDealer, UINT32 uDealerItemType, UINT8 ubMaxNumberOfItemType );
void		GuaranteeAtLeastOneItemOfType( UINT8 ubArmsDealer, UINT32 uiDealerItemType );
void		GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );

BOOLEAN AllocMemsetSpecialItemArray( OLD_DEALER_ITEM_HEADER_101 *pDealerItem, UINT8 ubElementsNeeded );
BOOLEAN ResizeSpecialItemArray( OLD_DEALER_ITEM_HEADER_101 *pDealerItem, UINT8 ubElementsNeeded );
void		FreeSpecialItemArray( OLD_DEALER_ITEM_HEADER_101 *pDealerItem );

void		ArmsDealerGetsFreshStock( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubNumItems );
BOOLEAN ItemContainsLiquid( UINT16 usItemIndex );
UINT8		DetermineDealerItemCondition( UINT8 ubArmsDealer, UINT16 usItemIndex );

BOOLEAN IsItemInfoSpecial( SPECIAL_ITEM_INFO *pSpclItemInfo, UINT16 usItemIndex );

BOOLEAN DoesItemAppearInDealerInventoryList( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer );

//INT16 GetSpecialItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, SPECIAL_ITEM_INFO *pSpclItemInfo );

void GuaranteeMinimumAlcohol( UINT8 ubArmsDealer );

BOOLEAN ItemIsARocketRifle( INT16 sItemIndex );

BOOLEAN GetArmsDealerShopHours( UINT8 ubArmsDealer, UINT32 *puiOpeningTime, UINT32 *puiClosingTime );

ARMS_DEALER_STATUS& ARMS_DEALER_STATUS::operator=(const OLD_ARMS_DEALER_STATUS_101& status)
{
	this->initialize();
	this->fOutOfBusiness = status.fOutOfBusiness;
	this->fRepairDelayBeenUsed = status.fRepairDelayBeenUsed;
	this->fUnusedKnowsPlayer = status.fUnusedKnowsPlayer;
	this->ubSpecificDealerFlags = status.ubSpecificDealerFlags;
	this->uiArmsDealersCash = status.uiArmsDealersCash;
	this->uiTimePlayerLastInSKI = status.uiTimePlayerLastInSKI;
	return *this;
}

void ARMS_DEALER_STATUS::initialize()
{
	memset(this, 0, sizeof(ARMS_DEALER_STATUS));
}
/*
BOOLEAN SPECIAL_ITEM_INFO::operator==(SPECIAL_ITEM_INFO& compare)
{
	PERFORMANCE_MARKER
	if (this->bItemCondition == compare.bItemCondition && this->ubImprintID == compare.ubImprintID)
	{
		for (int x = 0; x < MAX_ATTACHMENTS; ++x)
		{
			if (this->usAttachment[x] != compare.usAttachment[x]
			|| this->bAttachmentStatus[x] != compare.bAttachmentStatus[x])
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}
*/
void SPECIAL_ITEM_INFO::initialize()
{
	PERFORMANCE_MARKER
	memset(this, 0, sizeof(SPECIAL_ITEM_INFO));
}
/*
SPECIAL_ITEM_INFO& SPECIAL_ITEM_INFO::operator=(OLD_SPECIAL_ITEM_INFO_101& src)
{
	PERFORMANCE_MARKER
	//set all the new empty slots to 0
	this->initialize();

	//the size of these haven't changed
	this->bItemCondition = src.bItemCondition;
	this->ubImprintID = src.ubImprintID;

	//it's unlikely max will get less over the versions, but still, check the min
	for (int x = 0; x < __min(MAX_ATTACHMENTS, OLD_MAX_ATTACHMENTS_101); ++x)
	{
		this->usAttachment[x] = src.usAttachment[x];
		this->bAttachmentStatus[x] = src.bAttachmentStatus[x];
	}
	//if max gets greater, which is likely, then data was memset to 0 in initialize
	return *this;
}
*/
void DEALER_SPECIAL_ITEM::initialize()
{
	PERFORMANCE_MARKER
}

void InitAllArmsDealers()
{
	PERFORMANCE_MARKER
	UINT8		ubArmsDealer;

	//Memset all dealers' status tables to zeroes
	memset( gArmsDealerStatus, 0, sizeof( gArmsDealerStatus ) );

	//Initialize the initial status & inventory for each of the arms dealers
	for( ubArmsDealer = 0; ubArmsDealer < NUM_ARMS_DEALERS; ubArmsDealer++ )
	{
		InitializeOneArmsDealer( ubArmsDealer );
	}

	//make sure certain items are in stock and certain limits are respected
	AdjustCertainDealersInventory( );
}


void InitializeOneArmsDealer( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	UINT16	usItemIndex;
	UINT8		ubNumItems=0;


	memset( &( gArmsDealerStatus[ ubArmsDealer ] ), 0, sizeof( ARMS_DEALER_STATUS ) );
	gArmsDealersInventory[ubArmsDealer].clear();

	//Reset the arms dealers cash on hand to the default initial value
	gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash = ArmsDealerInfo[ ubArmsDealer ].iInitialCash;

	//if the arms dealer isn't supposed to have any items (includes all repairmen)
	if( ArmsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
	{
		return;
	}


	//loop through all the item types
	for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
	{
		if ( Item[usItemIndex].usItemClass	== 0 )
			break;
		//Can the item be sold by the arms dealer
		if( CanDealerTransactItem( ubArmsDealer, usItemIndex, FALSE ) )
		{
			//Setup an initial amount for the items (treat items as new, how many are used isn't known yet)
			ubNumItems = DetermineInitialInvItems( ubArmsDealer, usItemIndex, GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ), FALSE );

			//if there are any initial items
			if( ubNumItems > 0 )
			{
				ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, ubNumItems );
			}
		}
	}
}


void ShutDownArmsDealers()
{
	PERFORMANCE_MARKER
	UINT8		ubArmsDealer;
	// loop through all the dealers
	for( ubArmsDealer=0; ubArmsDealer<NUM_ARMS_DEALERS; ubArmsDealer++ )
	{
		gArmsDealersInventory[ ubArmsDealer ].clear();
	}
}



BOOLEAN SaveArmsDealerInventoryToSaveGameFile( HWFILE hFile )
{
	PERFORMANCE_MARKER
	UINT32	uiNumBytesWritten;
	UINT8		ubArmsDealer;

	//save the number of dealers to load
	int dealers = NUM_ARMS_DEALERS;
	if (!FileWrite( hFile, &dealers, sizeof( int ), &uiNumBytesWritten ))
	{
		return( FALSE );
	}

	//Save the arms dealers status
	if (!FileWrite( hFile, gArmsDealerStatus, sizeof( gArmsDealerStatus ), &uiNumBytesWritten ))
	{
		return( FALSE );
	}

	//loop through all the dealers inventories
	for( ubArmsDealer=0; ubArmsDealer<NUM_ARMS_DEALERS; ubArmsDealer++ )
	{
		int size = gArmsDealersInventory[ubArmsDealer].size();
		if (!FileWrite( hFile, &size, sizeof( int ), &uiNumBytesWritten ))
		{
			return( FALSE );
		}
		//loop through this dealer's individual items
		for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
			iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
			if (! iter->Save(hFile) ) {
				return FALSE;
			}
		}
	}

	return( TRUE );
}


void DEALER_SPECIAL_ITEM::ConvertFrom101(OLD_DEALER_ITEM_HEADER_101& header, OLD_DEALER_SPECIAL_ITEM_101& special, int usItemIndex)
{
	this->bItemCondition = special.oldInfo.bItemCondition;
	this->ubImprintID = special.oldInfo.ubImprintID;

	this->uiRepairDoneTime = special.uiRepairDoneTime;
	this->fActive = special.fActive;
	this->ubOwnerProfileId = special.ubOwnerProfileId;

	this->uiOrderArrivalTime = header.uiOrderArrivalTime;
	this->ubQtyOnOrder = header.ubQtyOnOrder;

	CreateItem(usItemIndex, bItemCondition, &(this->object));
	for (int x = 0; x < OLD_MAX_ATTACHMENTS_101; ++x) {
		if (special.oldInfo.usAttachment[x]) {
			CreateItem(special.oldInfo.usAttachment[x], special.oldInfo.bAttachmentStatus[x], &gTempObject);
			AttachObject(0, &(this->object), &gTempObject, FALSE);
		}
	}
};


void DailyUpdateOfArmsDealersInventory()
{
	PERFORMANCE_MARKER
	// if Gabby has creature blood, start turning it into extra elixir
	ConvertCreatureBloodToElixir();

	//Simulate other customers buying inventory from the dealer
	SimulateArmsDealerCustomer();

	//if there are some items that are out of stock, order some more
	DailyCheckOnItemQuantities();

	//make sure certain items are in stock and certain limits are respected
	AdjustCertainDealersInventory( );
}


// Once a day, loop through each dealer's inventory items and possibly sell some
void SimulateArmsDealerCustomer()
{
	PERFORMANCE_MARKER
	UINT8		ubArmsDealer=0;
	UINT16	usItemIndex;
	UINT8	ubItemsSold=0;

	static int numPerfectItems[MAXITEMS];

	//loop through all the arms dealers
	for( ubArmsDealer=0;ubArmsDealer<NUM_ARMS_DEALERS;ubArmsDealer++ )
	{
		if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
			continue;

		//if the arms dealer isn't supposed to have any items (includes all repairmen)
		if( ArmsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
			continue;

		memset (&numPerfectItems, 0, sizeof(numPerfectItems));

		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
			iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
			if (iter->bItemCondition == 100) {
				numPerfectItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
			}
		}

		//loop through all items of the same type
		for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
		{
			if ( Item[usItemIndex].usItemClass	== 0 )
				break;
			//if there are some of these in stock
			if( numPerfectItems[usItemIndex] > 0)
			{
				// first, try to sell all the new (perfect) ones
				if ( usItemIndex == JAR_ELIXIR )
				{
					// only allow selling of standard # of items so those converted from blood given by player will be available
					ubItemsSold = HowManyItemsAreSold( ubArmsDealer, usItemIndex, (UINT8) __min( 3, numPerfectItems[usItemIndex]), FALSE);
				}
				else
				{
					ubItemsSold = HowManyItemsAreSold( ubArmsDealer, usItemIndex, numPerfectItems[usItemIndex], FALSE);
				}
				if ( ubItemsSold > 0)
				{
					RemoveItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, ubItemsSold);
				}
			}
		}

		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
			iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
			// next, try to sell all the used ones, gotta do these one at a time so we can remove them by element
			// don't worry about negative condition, repairmen can't come this far, they don't sell!
			if (iter->bItemCondition < 100 && iter->bItemCondition > 0) {
				if ( iter->fActive ) {
					// try selling just this one
					if (HowManyItemsAreSold( ubArmsDealer, iter->object.usItem, 1, TRUE) > 0)
					{
						iter = gArmsDealersInventory[ ubArmsDealer ].erase(iter);
					}
				}
			}
		}
	}
}


void DailyCheckOnItemQuantities()
{
	PERFORMANCE_MARKER
	UINT8		ubArmsDealer;
	UINT16	usItemIndex;
	UINT8		ubMaxSupply;
	UINT8		ubNumItems;
	UINT32	uiArrivalDay;
	BOOLEAN fPrevElig;
	UINT8		ubReorderDays;


	//loop through all the arms dealers
	for( ubArmsDealer=0;ubArmsDealer<NUM_ARMS_DEALERS;ubArmsDealer++ )
	{
		if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
			continue;

		//Reset the arms dealers cash on hand to the default initial value
		gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash = ArmsDealerInfo[ ubArmsDealer ].iInitialCash;

		//if the arms dealer isn't supposed to have any items (includes all repairmen)
		if( ArmsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
			continue;

		int numTotalItems[MAXITEMS] = { 0 };
		bool itemsAreOnOrder[MAXITEMS] = { false };
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
			iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
			numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
			if (iter->ubQtyOnOrder) {
				itemsAreOnOrder[iter->object.usItem] = true;
				//and today is the day the items come in
				if( iter->uiOrderArrivalTime >= GetWorldDay() )
				{
					UINT16 usItem = iter->object.usItem;
					int quantity = iter->ubQtyOnOrder;
					iter = gArmsDealersInventory[ ubArmsDealer ].erase(iter);
					ArmsDealerGetsFreshStock( ubArmsDealer, usItem, quantity);
				}
			}
		}

		//loop through all items of the same type
		for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
		{
			if ( Item[usItemIndex].usItemClass	== 0 )
				break;
			//if the dealer can sell the item type
			if( CanDealerTransactItem( ubArmsDealer, usItemIndex, FALSE ) )
			{
				//if there are no items on order
				if ( itemsAreOnOrder[ usItemIndex ] == false )
				{
					ubMaxSupply = GetDealersMaxItemAmount( ubArmsDealer, usItemIndex );

					//if the qty on hand is half the desired amount or fewer
					if( numTotalItems[ usItemIndex ] <= (INT32)( ubMaxSupply / 2 ) )
					{
						//determine if the item can be restocked (assume new, use items aren't checked for until the stuff arrives)
						if (ItemTransactionOccurs( ubArmsDealer, usItemIndex, DEALER_BUYING, FALSE ))
						{
							// figure out how many items to reorder (items are reordered an entire batch at a time)
							ubNumItems = HowManyItemsToReorder( ubMaxSupply, numTotalItems[ usItemIndex ] );

							// if this is the first day the player is eligible to have access to this thing
							if ( gArmsDealerStatus[ubArmsDealer].fPreviouslyEligible[ usItemIndex ] == false )
							{
								// eliminate the ordering delay and stock the items instantly!
								// This is just a way to reward the player right away for making progress without the reordering lag...
								ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, ubNumItems );
							}
							else
							{
								if ( ( ubArmsDealer == ARMS_DEALER_TONY ) || ( ubArmsDealer == ARMS_DEALER_DEVIN ) )
								{
									// the stuff Tony and Devin sell is imported, so it takes longer to arrive (for game balance)
									ubReorderDays = ( UINT8) ( 2 + Random( 2 ) );	// 2-3 days
								}
								else
								{
									ubReorderDays = ( UINT8) ( 1 + Random( 2 ) );	// 1-2 days
								}

								//Determine when the inventory should arrive
								uiArrivalDay = GetWorldDay() + ubReorderDays;	// consider changing this to minutes

								// post new order
								Todo
								
								//gOldArmsDealersInventory[ ubArmsDealer ][ usItemIndex ].ubQtyOnOrder = ubNumItems;
								//gOldArmsDealersInventory[ ubArmsDealer ][ usItemIndex ].uiOrderArrivalTime = uiArrivalDay;
							}
						}
					}
				}
			}
		}
	}
}

void ConvertCreatureBloodToElixir( void )
{
	PERFORMANCE_MARKER
	UINT8	ubBloodAvailable = 0;
	UINT8 ubAmountToConvert;

	for (DealerItemList::iterator iter = gArmsDealersInventory[ ARMS_DEALER_GABBY ].begin();
		iter != gArmsDealersInventory[ ARMS_DEALER_GABBY ].end(); ++iter) {
		if (iter->object.usItem == JAR_CREATURE_BLOOD) {
			//should these be perfect? only perfect are deleted
			ubBloodAvailable += iter->object.ubNumberOfObjects;
		}
	}
	if ( ubBloodAvailable )
	{
		// start converting blood into elixir!
		//ubAmountToConvert = (UINT8) __min( 5 + Random( 3 ), ubBloodAvailable );
		ubAmountToConvert = ubBloodAvailable;

		//Now remove that many NEW ones (condition 100) of that item
		RemoveItemFromArmsDealerInventory( ARMS_DEALER_GABBY, JAR_CREATURE_BLOOD, ubAmountToConvert );

		ArmsDealerGetsFreshStock( ARMS_DEALER_GABBY, JAR_ELIXIR, ubAmountToConvert );
	}
}

BOOLEAN AdjustCertainDealersInventory( )
{
	PERFORMANCE_MARKER
	//Adjust Tony's items (this restocks *instantly* 1/day, doesn't use the reorder system)
	GuaranteeAtLeastOneItemOfType( ARMS_DEALER_TONY, ARMS_DEALER_BIG_GUNS );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_BIG_GUNS, 2 );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_HANDGUNCLASS, 3 );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_AMMO, 8 );

	//Adjust all bartenders' alcohol levels to a minimum
	GuaranteeMinimumAlcohol( ARMS_DEALER_FRANK );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_1 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_2 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_3 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_4 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_ELGIN );
	GuaranteeMinimumAlcohol( ARMS_DEALER_MANNY );

	//make sure Sam (hardware guy) has at least one empty jar
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_SAM, JAR, 1 );

	if ( CheckFact( FACT_ESTONI_REFUELLING_POSSIBLE, 0 ) )
	{
		// gas is restocked regularly, unlike most items
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_JAKE, GAS_CAN, ( UINT8 ) ( 4 + Random( 3 ) ) );
	}

	//If the player hasn't bought a video camera from Franz yet, make sure Franz has one to sell
	if( !( gArmsDealerStatus[ ARMS_DEALER_FRANZ ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__FRANZ_HAS_SOLD_VIDEO_CAMERA_TO_PLAYER ) )
	{
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_FRANZ, VIDEO_CAMERA, 1 );
	}

	return( TRUE );
}



void LimitArmsDealersInventory( UINT8 ubArmsDealer, UINT32 uiDealerItemType, UINT8 ubMaxNumberOfItemType )
{
	PERFORMANCE_MARKER
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	Assert( !DoesDealerDoRepairs( ubArmsDealer ) );

	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	int numTotalItems[MAXITEMS] = { 0 };
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
	}

	UINT32	uiTotalNumberOfItems = 0, uiRandomChoice;
	UINT32	uiIndex;

	UINT16	usItemIndex=0;
	std::vector<int> usAvailableItems;
	std::vector<UINT32> ubNumberOfAvailableItem;
	//loop through all items of the same class and count the number in stock
	for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
	{
		if ( Item[usItemIndex].usItemClass	== 0 )
			break;
		//if there is some items in stock
		if( numTotalItems[usItemIndex] > 0)
		{
			//if the item is of the same dealer item type
			if( uiDealerItemType & GetArmsDealerItemTypeFromItemNumber( usItemIndex ) )
			{
				usAvailableItems.push_back(usItemIndex);
				//if the dealer item type is ammo
				if( uiDealerItemType == ARMS_DEALER_AMMO )
				{
					// all ammo of same type counts as only one item
					ubNumberOfAvailableItem.push_back(1);
					uiTotalNumberOfItems++;
				}
				else
				{
					// items being repaired don't count against the limit
					ubNumberOfAvailableItem.push_back(numTotalItems[usItemIndex]);
					uiTotalNumberOfItems += ubNumberOfAvailableItem.back();
				}
			}
		}
	}

	//if there is more of the given type than we want
	UINT32 uiItemsToRemove = usAvailableItems.size() - ubMaxNumberOfItemType;

	while (uiItemsToRemove > 0)
	{
		uiRandomChoice = Random( uiTotalNumberOfItems );

		for ( uiIndex = 0; uiIndex < usAvailableItems.size(); uiIndex++ )
		{
			if ( uiRandomChoice <= ubNumberOfAvailableItem[ uiIndex ] )
			{
				usItemIndex = usAvailableItems[ uiIndex ];
				if ( uiDealerItemType == ARMS_DEALER_AMMO )
				{
					// remove all of them, since each ammo item counts as only one "item" here
					// ammo will always be only condition 100, there's never any in special slots
					RemoveItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, ubNumberOfAvailableItem[uiIndex] );
				}
				else
				{
					// pick 1 random one, don't care about its condition
					RemoveRandomItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, 1 );
				}
				// now remove entry from the array by replacing it with the last and decrementing
				// the size of the array
				usAvailableItems[ uiIndex ] = usAvailableItems.back();
				usAvailableItems.pop_back();
				ubNumberOfAvailableItem[ uiIndex ] = ubNumberOfAvailableItem.back();
				ubNumberOfAvailableItem.pop_back();

				// decrement count of # of items to remove
				uiItemsToRemove--;
				break; // and out of 'for' loop

			}
			else
			{
				// next item!
				uiRandomChoice -= ubNumberOfAvailableItem[ uiIndex ];
			}
		}

		/*
		//loop through all items of the same type
		for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
		{
			//if there are some non-repairing items in stock
			if( gOldArmsDealersInventory[ ubArmsDealer ][ usItemIndex ].ubTotalItems )
			{
				//if the item is of the same dealer item type
				if( uiDealerItemType & GetArmsDealerItemTypeFromItemNumber( usItemIndex ) )
				{
					// a random chance that the item will be removed
					if( Random( 100 ) < 30 )
					{
						//remove the item

						//if the dealer item type is ammo
						if( uiDealerItemType == ARMS_DEALER_AMMO )
						{
							// remove all of them, since each ammo item counts as only one "item" here

							// create item info describing a perfect item
							SetSpecialItemInfoToDefaults( &SpclItemInfo );
							// ammo will always be only condition 100, there's never any in special slots
							RemoveItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, gOldArmsDealersInventory[ ubArmsDealer ][ usItemIndex ].ubTotalItems );
						}
						else
						{
							// pick 1 random one, don't care about its condition
							RemoveRandomItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, 1 );
						}

						uiItemsToRemove--;
						if( uiItemsToRemove == 0)
							break;
					}
				}
			}
		}
		*/
	}
}


void GuaranteeAtLeastOneItemOfType( UINT8 ubArmsDealer, UINT32 uiDealerItemType )
{
	PERFORMANCE_MARKER
	UINT16 usItemIndex;
	UINT8 ubChance;
	UINT32	uiTotalChances = 0;
	UINT32	uiNumAvailableItems = 0, uiIndex, uiRandomChoice;
	
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	Assert( !DoesDealerDoRepairs( ubArmsDealer ) );

	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	int numTotalItems[MAXITEMS] = { 0 };
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
	}

	std::vector<UINT16> usAvailableItems;
	std::vector<UINT8> ubChanceForAvailableItem;
	//loop through all items of the same type
	for( usItemIndex = 1; usItemIndex < MAXITEMS; usItemIndex++ )
	{
		if ( Item[usItemIndex].usItemClass	== 0 )
			break;
		//if the item is of the same dealer item type
		if( uiDealerItemType & GetArmsDealerItemTypeFromItemNumber( usItemIndex ) )
		{
			//if there are any of these in stock
			if( numTotalItems[usItemIndex] > 0 )
			{
				//there is already at least 1 item of that type, return
				return;
			}

			// if he can stock it (it appears in his inventory list)
			if( GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ) > 0)
			{
				// and the stage of the game gives him a chance to have it (assume new)
				ubChance = ChanceOfItemTransaction( ubArmsDealer, usItemIndex, DEALER_BUYING, FALSE );
				if ( ubChance > 0 )
				{
					usAvailableItems.push_back(usItemIndex);
					ubChanceForAvailableItem.push_back(ubChance);
					uiTotalChances += ubChance; 
				}
			}
		}
	}

	// if there aren't any such items, the following loop would never finish, so quit before trying it!
	if (usAvailableItems.empty() == true)
	{
		return;
	}


	// CJC: randomly pick one of available items by weighted random selection.

	// randomize number within uiTotalChances and then loop forwards till we find that item
	uiRandomChoice = Random( uiTotalChances );

	for ( uiIndex = 0; uiIndex < usAvailableItems.size(); uiIndex++ )
	{
		if ( uiRandomChoice <= ubChanceForAvailableItem[ uiIndex ] )
		{
			ArmsDealerGetsFreshStock( ubArmsDealer, usAvailableItems[ uiIndex ], 1 );
			return;
		}
		else
		{
			// next item!
			uiRandomChoice -= ubChanceForAvailableItem[ uiIndex ];
		}
	}

	Assert(false);
	// internal logic failure!
}


void GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany )
{
	PERFORMANCE_MARKER
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	Assert( !DoesDealerDoRepairs( ubArmsDealer ) );

	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	int itemsIHave = 0;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		if (iter->object.usItem == usItemIndex) {
			itemsIHave += iter->object.ubNumberOfObjects;
			//if there are any of these in stock
			if( itemsIHave >= ubHowMany )
			{
				// have what we need...
				return;
			}
		}
	}


	// if he can stock it (it appears in his inventory list)
// RESTRICTION REMOVED: Jake must be able to guarantee GAS even though it's not in his list, it's presence is conditional
//	if( GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ) > 0)
	{
		//add the item
		ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, (UINT8)( ubHowMany - itemsIHave ) );
	}
}



UINT32 GetArmsDealerItemTypeFromItemNumber( UINT16 usItem )
{
	PERFORMANCE_MARKER
	switch( Item[ usItem ].usItemClass )
	{
		case IC_NONE:
			return( 0 );
			break;

		case IC_GUN:
			switch(	Weapon[ Item[ usItem ].ubClassIndex ].ubWeaponClass )
			{
				case HANDGUNCLASS:
					return( ARMS_DEALER_HANDGUNCLASS );
					break;
				case RIFLECLASS:
					if ( ItemIsARocketRifle( usItem ) )
						return( ARMS_DEALER_ROCKET_RIFLE );
					else
						return( ARMS_DEALER_RIFLECLASS );
					break;
				case SHOTGUNCLASS:
					return( ARMS_DEALER_SHOTGUNCLASS );
					break;
				case SMGCLASS:
					return( ARMS_DEALER_SMGCLASS );
					break;
				case MGCLASS:
					return( ARMS_DEALER_MGCLASS );
					break;
				case MONSTERCLASS:
					return( 0 );
					break;
				case KNIFECLASS:
					return( ARMS_DEALER_KNIFECLASS );
					break;
			}
			break;

		case IC_PUNCH:
			if (usItem == NOTHING)
			{
				return( 0 );
			}
			// else treat as blade
		case IC_BLADE:
		case IC_THROWING_KNIFE:
			return( ARMS_DEALER_BLADE );
			break;
		case IC_LAUNCHER:
			return( ARMS_DEALER_LAUNCHER );
			break;
		case IC_ARMOUR:
			return( ARMS_DEALER_ARMOUR );
			break;
		case IC_MEDKIT:
			return( ARMS_DEALER_MEDKIT );
			break;
		case IC_KIT:
			return( ARMS_DEALER_KIT );
			break;
		case IC_MISC:
		{
			if ( Item [usItem].alcohol )
				return( ARMS_DEALER_ALCOHOL );
			else if ( Item [usItem].electronic )
				return( ARMS_DEALER_ELECTRONICS );
			else if ( Item [usItem].hardware )
				return( ARMS_DEALER_HARDWARE );
			else if ( Item [usItem].medical )
				return( ARMS_DEALER_MEDICAL );
			else if ( Item [usItem].attachment )
				return( ARMS_DEALER_ATTACHMENTS );
			else if ( Item [usItem].detonator || Item [usItem].remotedetonator || Item [usItem].remotetrigger )
				return( ARMS_DEALER_DETONATORS );
			else
				return( ARMS_DEALER_MISC );
			//switch on the type of item
//			switch( usItem )
//			{
//				case BEER:
//				case WINE:
//				case ALCOHOL:
//					return( ARMS_DEALER_ALCOHOL );
//					break;
//
//				case METALDETECTOR:
//				case LASERSCOPE:
////				case REMDETONATOR:
//					return( ARMS_DEALER_ELECTRONICS );
//					break;
//
//				case CANTEEN:
//				case CROWBAR:
//				case WIRECUTTERS:
//					return( ARMS_DEALER_HARDWARE );
//					break;
//
//				case ADRENALINE_BOOSTER:
//				case REGEN_BOOSTER:
//				case SYRINGE_3:
//				case SYRINGE_4:
//				case SYRINGE_5:
//					return( ARMS_DEALER_MEDICAL );
//					break;
//
//				case SILENCER:
//				case SNIPERSCOPE:
//				case BIPOD:
//				case DUCKBILL:
//					return( ARMS_DEALER_ATTACHMENTS );
//					break;
//
//				case DETONATOR:
//				case REMDETONATOR:
//				case REMOTEBOMBTRIGGER:
//					return( ARMS_DEALER_DETONATORS );
//					break;
//
//				default:
//					return( ARMS_DEALER_MISC );
//
//					break;
//		}

		}
		break;
		case IC_AMMO:
			return( ARMS_DEALER_AMMO );
			break;
		case IC_FACE:
			if (Item[usItem].electronic )
				return ARMS_DEALER_ELECTRONICS;
			else
				return ARMS_DEALER_FACE;
			//switch( usItem )
			//{
			//	case EXTENDEDEAR:
			//	case NIGHTGOGGLES:
			//	case ROBOT_REMOTE_CONTROL:
			//		return( ARMS_DEALER_ELECTRONICS );
			//	break;

			//	default:
			//		return( ARMS_DEALER_FACE );
			//}
			break;
		case IC_THROWN:
			return( 0 );
//			return( ARMS_DEALER_THROWN );
			
			break;
		case IC_KEY:
			return( 0 );
//			return( ARMS_DEALER_KEY );
			break;
		case IC_GRENADE:
			return( ARMS_DEALER_GRENADE );
			break;
		case IC_BOMB:
			return( ARMS_DEALER_BOMB );
			break;
		case IC_EXPLOSV:
			return( ARMS_DEALER_EXPLOSV );
			break;
		case IC_TENTACLES:
		case IC_MONEY:
			return( 0 );
			break;

	//	case IC_APPLIABLE:
			break;

		default:
			AssertMsg( FALSE, String( "GetArmsDealerItemTypeFromItemNumber(), invalid class %d for item %d.	DF 0.", Item[ usItem ].usItemClass, usItem ) );
			break;
	}
	return( 0 );
}



BOOLEAN IsMercADealer( UINT8 ubMercID )
{
	PERFORMANCE_MARKER
	UINT8	cnt;

	// Manny is not actually a valid dealer unless a particular event sets that fact
	if( ( ubMercID == MANNY ) && !CheckFact( FACT_MANNY_IS_BARTENDER, 0 ) )
	{
		return( FALSE );
	}

	//loop through the list of arms dealers
	for( cnt=0; cnt<NUM_ARMS_DEALERS; cnt++ )
	{
		if( ArmsDealerInfo[ cnt ].ubShopKeeperID == ubMercID )
			return( TRUE );
	}
	return( FALSE );
}



INT8 GetArmsDealerIDFromMercID( UINT8 ubMercID )
{
	PERFORMANCE_MARKER
	INT8	cnt;

	//loop through the list of arms dealers
	for( cnt=0; cnt<NUM_ARMS_DEALERS; cnt++ )
	{
		if( ArmsDealerInfo[ cnt ].ubShopKeeperID == ubMercID )
			return( cnt );
	}

	return( -1 );
}



UINT8 GetTypeOfArmsDealer( UINT8	ubDealerID )
{
	PERFORMANCE_MARKER
	return( ArmsDealerInfo[ ubDealerID ].ubTypeOfArmsDealer );
}


BOOLEAN	DoesDealerDoRepairs( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	if( ArmsDealerInfo[ ubArmsDealer ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		return( TRUE );
	else
		return( FALSE );
}


BOOLEAN RepairmanIsFixingItemsButNoneAreDoneYet( UINT8 ubProfileID )
{
	PERFORMANCE_MARKER
	INT8		bArmsDealer;
	BOOLEAN fHaveOnlyUnRepairedItems=FALSE;
	UINT8		ubElement;
	UINT16	usItemIndex;


	bArmsDealer = GetArmsDealerIDFromMercID( ubProfileID );
	if( bArmsDealer == -1 )
		return( FALSE );

	//if the dealer is not a repair dealer, return
	if( !DoesDealerDoRepairs( bArmsDealer ) )
		return( FALSE );

	//loop through the dealers inventory and check if there are only unrepaired items
	for (DealerItemList::iterator iter = gArmsDealersInventory[ bArmsDealer ].begin();
		iter != gArmsDealersInventory[ bArmsDealer ].end(); ++iter) {
		if ( Item[iter->object.usItem].usItemClass == 0 )
			continue;
		if (iter->fActive == false)
			continue;

		//if the items status is below 0, the item is being repaired
		if( iter->bItemCondition < 0 )
		{
			//if the item has been repaired
			if( iter->uiRepairDoneTime <= GetWorldTotalMin() )
			{
				//A repair item is ready, therefore, return false
				return( FALSE );
			}
			else
			{
				fHaveOnlyUnRepairedItems = TRUE;
			}
		}
	}

	return( fHaveOnlyUnRepairedItems );
}

BOOLEAN CanDealerTransactItem( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer )
{
	PERFORMANCE_MARKER
	switch ( ArmsDealerInfo[ ubArmsDealer ].ubTypeOfArmsDealer )
	{
		case ARMS_DEALER_SELLS_ONLY:
			if ( fPurchaseFromPlayer )
			{
				// this dealer only sells stuff to player, so he can't buy anything from him
				return( FALSE );
			}
			break;

		case ARMS_DEALER_BUYS_ONLY:
			if ( !fPurchaseFromPlayer )
			{
				// this dealer only buys stuff from player, so he can't sell anything to him
				return( FALSE );
			}
			break;

		case ARMS_DEALER_BUYS_SELLS:
			switch ( ubArmsDealer )
			{
				case ARMS_DEALER_JAKE:
				case ARMS_DEALER_KEITH:
				case ARMS_DEALER_FRANZ:
					if ( fPurchaseFromPlayer )
					{
						// these guys will buy nearly anything from the player, regardless of what they carry for sale!
						return( CalcValueOfItemToDealer( ubArmsDealer, usItemIndex, FALSE ) > 0 );
					}
					//else selling inventory uses their inventory list
					break;

				default:
					// the others go by their inventory list
					break;
			}
			break;

		case ARMS_DEALER_REPAIRS:
			// repairmen don't have a complete list of what they'll repair in their inventory,
			// so we must check the item's properties instead.
			return( CanDealerRepairItem( ubArmsDealer, usItemIndex ) );

		default:
			AssertMsg( FALSE, String( "CanDealerTransactItem(), type of dealer %d.	AM 0.", ArmsDealerInfo[ ubArmsDealer ].ubTypeOfArmsDealer ) );
			return(FALSE);
	}

	return( DoesItemAppearInDealerInventoryList( ubArmsDealer, usItemIndex, fPurchaseFromPlayer ) );
}


BOOLEAN CanDealerRepairItem( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
//	UINT32 uiFlags;

//	uiFlags = Item[ usItemIndex ].fFlags;

	// can't repair anything that's not repairable!
//	if ( !( uiFlags & ITEM_REPAIRABLE ) )
	if ( !( Item[ usItemIndex ].repairable	) )
	{
		return(FALSE);
	}

	switch ( ubArmsDealer )
	{
		case ARMS_DEALER_ARNIE:
		case ARMS_DEALER_PERKO:
			// repairs ANYTHING non-electronic
//			if ( !( uiFlags & ITEM_ELECTRONIC ) )
			if ( !( Item[ usItemIndex ].electronic ) )
			{
				return(TRUE);
			}
			break;

		case ARMS_DEALER_FREDO:
			// repairs ONLY electronics
//			if ( uiFlags & ITEM_ELECTRONIC )
			if ( Item[ usItemIndex ].electronic	)
			{
				return(TRUE);
			}
			break;

		default:
			AssertMsg( FALSE, String( "CanDealerRepairItem(), Arms Dealer %d is not a recognized repairman!.	AM 1.", ubArmsDealer ) );
	}

	// can't repair this...
	return(FALSE);
}


BOOLEAN ResizeSpecialItemArray( OLD_DEALER_ITEM_HEADER_101 *pDealerItem, UINT8 ubElementsNeeded )
{
	PERFORMANCE_MARKER
	Assert(pDealerItem);
	// must already have a ptr allocated!
	Assert(pDealerItem->SpecialItem);

	
	if ( ubElementsNeeded == pDealerItem->ubElementsAlloced)
	{
		// shouldn't have been called, but what they hey, it's not exactly a problem
		return(TRUE);
	}

	// already allocated, but change its size
	OLD_DEALER_SPECIAL_ITEM_101* pNew = new OLD_DEALER_SPECIAL_ITEM_101[ ubElementsNeeded ];
	if( pNew == NULL )
	{
		Assert( 0 );
		return(FALSE);
	}

	for (int x = 0; x < __min(pDealerItem->ubElementsAlloced, ubElementsNeeded); ++x)
	{
		pNew[x] = pDealerItem->SpecialItem[x];
	}

	delete[] pDealerItem->SpecialItem;
	pDealerItem->SpecialItem = pNew;

	pDealerItem->ubElementsAlloced = ubElementsNeeded;

	return(TRUE);
}


void FreeSpecialItemArray( OLD_DEALER_ITEM_HEADER_101 *pDealerItem)
{
	PERFORMANCE_MARKER
	Assert(pDealerItem);
	// must already have a ptr allocated!
	Assert(pDealerItem->SpecialItem);

	delete[]( pDealerItem->SpecialItem );
	pDealerItem->SpecialItem = NULL;

	pDealerItem->ubElementsAlloced = 0;
	pDealerItem->ubTotalItems = pDealerItem->ubPerfectItems;

	// doesn't effect perfect items, orders or stray bullets!
}



void ArmsDealerGetsFreshStock( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubNumItems )
{
	PERFORMANCE_MARKER
	UINT8 ubCnt;
	UINT8 ubItemCondition;
	UINT8 ubPerfectOnes = 0;

	// determine the condition of each one, counting up new ones, but adding damaged ones right away
	for ( ubCnt = 0; ubCnt < ubNumItems; ubCnt++ )
	{
		ubItemCondition = DetermineDealerItemCondition( ubArmsDealer, usItemIndex);

		// if the item is brand new
		if ( ubItemCondition == 100)
		{
			ubPerfectOnes++;
		}
		else
		{
			// add a used item with that condition to his inventory
			CreateItem(usItemIndex, ubItemCondition, &gTempObject);
			AddObjectToArmsDealerInventory( ubArmsDealer, &gTempObject );
		}
	}

	// now add all the perfect ones, in one shot
	if ( ubPerfectOnes > 0)
	{
		CreateItems(usItemIndex, ubItemCondition, ubPerfectOnes, &gTempObject);
		AddObjectToArmsDealerInventory( ubArmsDealer, &gTempObject );
	}
}



UINT8 DetermineDealerItemCondition( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	UINT8 ubCondition = 100;

	// if it's a damagable item, and not a liquid (those are always sold full)
//	if ( ( Item[ usItemIndex ].fFlags & ITEM_DAMAGEABLE ) && !ItemContainsLiquid( usItemIndex ) )
	if ( ( Item[ usItemIndex ].damageable ) && !ItemContainsLiquid( usItemIndex ) )
	{
		// if he ONLY has used items, or 50% of the time if he carries both used & new items
		if ( ( ArmsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_ONLY_USED_ITEMS ) ||
			( ( ArmsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_SOME_USED_ITEMS ) && ( Random( 100 ) < 50 ) ) )
		{
			// make the item a used one
			ubCondition = (UINT8)(20 + Random( 60 ));
		}
	}

	return( ubCondition);
}



BOOLEAN ItemContainsLiquid( UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	if(Item[usItemIndex].containsliquid)
		return TRUE;
	else
		return FALSE;

	//switch ( usItemIndex )
	//{
	//	case CANTEEN:
	//	case BEER:
	//	case ALCOHOL:
	//	case JAR_HUMAN_BLOOD:
	//	case JAR_CREATURE_BLOOD:
	//	case JAR_QUEEN_CREATURE_BLOOD:
	//	case JAR_ELIXIR:
	//	case GAS_CAN:
	//		return( TRUE );
	//}

	//return( FALSE );
}

bool ItemIsSpecial(DEALER_SPECIAL_ITEM& item)
{
	if (item.object[0]->attachments.empty() == false) {
		//items with attachments are special
		return true;
	}
	if (item.ubQtyOnOrder) {
		//empty item being ordered
		return true;
	}
	if (item.bItemCondition < 0) {
		//item in for repairs
		return true;
	}
	return false;
}

UINT32 CountDistinctItemsInArmsDealersInventory( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	UINT32	uiNumOfItems=0;
	UINT16	usItemIndex;


	for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
		iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
		//if there are any items, doesn't count items on order
		if (iter->object.ubNumberOfObjects) {
			if (ItemIsSpecial((*iter)) == true) {
				// if the items can be stacked
				// NOTE: This test must match the one inside AddItemsToTempDealerInventory() exactly!
				if ( DealerItemIsSafeToStack( iter->object.usItem ) )
				{
					// regardless of how many there are, they count as 1 *distinct* item!	They will all be together in one box...
					uiNumOfItems++;
				}
				else
				{
					// non-stacking items must be stored in one / box , because each may have unique fields besides status.bStatus[]
					// Example: guns all have ammo, ammo type, etc.	We need these uniquely represented for pricing & manipulation
					uiNumOfItems += iter->object.ubNumberOfObjects;
				}
			}

			// each *active* special item counts as one additional distinct item (each one occupied a separate shopkeeper box!)
			// NOTE: This is including items being repaired!!!
			uiNumOfItems += CountActiveSpecialItemsInArmsDealersInventory( ubArmsDealer, usItemIndex);
		}
	}

	return( uiNumOfItems );
}


UINT8 CountActiveSpecialItemsInArmsDealersInventory( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	UINT8 ubActiveSpecialItems = 0;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		if (iter->object.usItem == usItemIndex) {
			if (iter->fActive) {
				ubActiveSpecialItems++;
			}
		}
	}
	return( ubActiveSpecialItems );
}


UINT16 CountTotalItemsRepairDealerHasInForRepairs( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	UINT16	usItemIndex;
	UINT16	usHowManyInForRepairs = 0;

	//if the dealer is not a repair dealer, no need to count, return 0
	if( !DoesDealerDoRepairs( ubArmsDealer ) )
		return( 0 );

	//loop through the dealers inventory and count the number of items in for repairs
	for( usItemIndex=0; usItemIndex < MAXITEMS; usItemIndex++ )
	{
		if ( Item[usItemIndex].usItemClass	== 0 )
			break;
		usHowManyInForRepairs += CountSpecificItemsRepairDealerHasInForRepairs( ubArmsDealer, usItemIndex );
	}

	return( usHowManyInForRepairs );
}


UINT8 CountSpecificItemsRepairDealerHasInForRepairs( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	UINT8		ubElement;
	UINT8		ubHowManyInForRepairs = 0;

	//if the dealer is not a repair dealer, no need to count, return 0
	if( !DoesDealerDoRepairs( ubArmsDealer ) )
		return( 0 );


	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		if (iter->object.usItem == usItemIndex) {
			if (iter->fActive) {
				//if the item's status is below 0, the item is being repaired
				if( iter->bItemCondition < 0 )
				{
					ubHowManyInForRepairs++;
				}
			}
		}
	}
	return( ubHowManyInForRepairs );
}



void AddObjectToArmsDealerInventory( UINT8 ubArmsDealer, OBJECTTYPE *pObject )
{
	PERFORMANCE_MARKER

	// split up all the components of an objecttype and add them as seperate items into the dealer's inventory
	int total = 0;
	for (UINT8 ubCnt = 0; ubCnt < pObject->ubNumberOfObjects; ubCnt++ )
	{
		switch ( Item [ pObject->usItem ].usItemClass )
		{
			case IC_GUN:
				TODO add gun
				// add the gun (keeps the object's status and imprintID)
				// if the gun was jammed, this will forget about the jam (i.e. dealer immediately unjams anything he buys)

				// if any GunAmmoItem is specified
				if( (*pObject)[ubCnt]->data.gun.usGunAmmoItem != NONE)
				{
					// if it's regular ammo
					if( Item[ (*pObject)[ubCnt]->data.gun.usGunAmmoItem ].usItemClass == IC_AMMO )
					{
						// and there are some remaining
						if ( (*pObject)[ubCnt]->data.gun.ubGunShotsLeft > 0 )
						{
							// add the bullets of its remaining ammo
							total += (*pObject)[ubCnt]->data.gun.ubGunShotsLeft;
						}
					}
					else	// assume it's attached ammo (mortar shells, grenades)
					{
						// add the launchable item (can't be imprinted, or have attachments!)
						int bItemCondition = (*pObject)[ubCnt]->data.gun.bGunAmmoStatus;

						// if the gun it was in was jammed, get rid of the negative status now
						if ( bItemCondition < 0 )
						{
							bItemCondition *= -1;
						}

						AddItemToArmsDealerInventory( ubArmsDealer, (*pObject)[ubCnt]->data.gun.usGunAmmoItem, bItemCondition, 1 );
					}
				}
				break;

			case IC_AMMO:
				// add the contents of each magazine (multiple mags may have vastly different #bullets left)
				total += (*pObject)[ubCnt]->data.ubShotsLeft;
				break;

			default:
				// add each object seperately (multiple objects may have vastly different statuses, keep any imprintID)
				if ((*pObject)[ubCnt]->data.objectStatus == 100) {
					total += 1;
				}
				else {
					AddItemToArmsDealerInventory( ubArmsDealer, pObject->usItem, (*pObject)[ubCnt]->data.objectStatus, 1 );
				}
		}


		// loop through any detachable attachments and add them as seperate items
		for (attachmentList::iterator iter = (*pObject)[ubCnt]->attachments.begin(); iter != (*pObject)[ubCnt]->attachments.end(); ++iter) {
	// ARM: Note: this is only used for selling, not repairs, so attachmentes are seperated when sold to a dealer
			// If the attachment is detachable
			if (! (Item[ iter->usItem ].inseparable	) )
			{
				// add this particular attachment (they can't be imprinted, or themselves have attachments!)
				AddObjectToArmsDealerInventory( ubArmsDealer, &(*iter) );
				iter = (*pObject)[ubCnt]->attachments.erase(iter);
			}
		}
	}

	switch ( Item [ pObject->usItem ].usItemClass )
	{
		case IC_GUN:
			AddAmmoToArmsDealerInventory( ubArmsDealer, (*pObject)[0]->data.gun.usGunAmmoItem, total );
			break;
		case IC_AMMO:
			AddAmmoToArmsDealerInventory( ubArmsDealer, pObject->usItem, total );
			break;
		default:
			if (total) {
				AddItemToArmsDealerInventory( ubArmsDealer, pObject->usItem, 100, total );
			}
			break;
	}

	//ADB not necessary any more
	// nuke the original object to prevent any possible item duplication
	//pObject->initialize();
}


void AddAmmoToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubShotsLeft )
{
	PERFORMANCE_MARKER
	UINT8 ubMagCapacity;
	UINT8 *pubStrayAmmo;

	// Ammo only, please!!!
	if (Item [ usItemIndex ].usItemClass != IC_AMMO )
	{
		Assert(0);
		return;
	}

	if ( ubShotsLeft == 0)
	{
		return;
	}


	ubMagCapacity = Magazine[ Item[ usItemIndex ].ubClassIndex ].ubMagSize;

	if ( ubShotsLeft >= ubMagCapacity )
	{
		// add however many FULL magazines the #shot left represents	
		AddItemToArmsDealerInventory( ubArmsDealer, usItemIndex, ubMagCapacity, ( UINT8 ) ( ubShotsLeft / ubMagCapacity ) );
		ubShotsLeft %= ubMagCapacity;
	}

	// any shots left now are "strays" - not enough to completely fill a magazine of this type
	if ( ubShotsLeft > 0 )
	{
		// handle "stray" ammo - add it to the dealer's stray pile

		pubStrayAmmo = &(gArmsDealerStatus[ubArmsDealer].ubStrayAmmo[ usItemIndex ]);
		*pubStrayAmmo += ubShotsLeft;

		// if dealer has accumulated enough stray ammo to make another full magazine, convert it!
		if ( *pubStrayAmmo >= ubMagCapacity )
		{
			AddItemToArmsDealerInventory( ubArmsDealer, usItemIndex, ubMagCapacity, ( UINT8 ) ( *pubStrayAmmo / ubMagCapacity ) );
			*pubStrayAmmo %= ubMagCapacity;
		}
		// I know, I know, this is getting pretty anal...	But what the hell, it was easy enough to do.	ARM.
	}
}


//Use AddObjectToArmsDealerInventory() instead of this when converting a complex item in OBJECTTYPE format.
void AddItemToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, INT8 objectStatus, UINT8 ubHowMany )
{
	PERFORMANCE_MARKER
	UINT8 ubRoomLeft;
	UINT8 ubElement;
	UINT8 ubElementsToAdd;
	BOOLEAN fFoundOne;
	BOOLEAN fSuccess;


	Assert( ubHowMany > 0);
	TODO handle full ammo magazines as objectStatus == 100
	if (objectStatus == 100) {
		//first find existing items with same perfect status, if found add to that, else create new one
		for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
			iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
			if (iter->object.usItem == usItemIndex &&
				iter->bItemCondition == 100) {//won't find objects in for repair
				//make sure it's not something special, objects with default attachments aren't handled, but oh well
				if (ItemIsSpecial(*iter) == false) {
					iter->object.AddObjectsToStack(ubHowMany, objectStatus);
					return;
				}
			}
		}
	}

	gArmsDealersInventory[ubArmsDealer].push_back(DEALER_SPECIAL_ITEM());
	DEALER_SPECIAL_ITEM* pItem = &(gArmsDealersInventory[ubArmsDealer].back());
	CreateItems(usItemIndex, objectStatus, ubHowMany, &(pItem->object));
	pItem->bItemCondition = objectStatus;
	pItem->fActive = true;
	pItem->ubImprintID = imprintID;

}


// WDS bug fix (07/24/2007) - Items with default attachments have to be handled specially

BOOLEAN SpecialItemInfoIsIdentical (SPECIAL_ITEM_INFO& baseItem, SPECIAL_ITEM_INFO& actualItem, UINT16 usItemIndex)
{
	BOOLEAN firstAttachmentIsDefault = 
		((actualItem.usAttachment[ 0 ] == Item [ usItemIndex ].defaultattachment) &&
		(actualItem.bAttachmentStatus[0] == 100));
	for (int idx=0; idx < MAX_ATTACHMENTS; ++idx) {
		if ((idx != 0) || (!firstAttachmentIsDefault)) {
			if (baseItem.usAttachment[idx] != actualItem.usAttachment[idx])
				return FALSE;
			if (baseItem.bAttachmentStatus[idx] != actualItem.bAttachmentStatus[idx])
				return FALSE;
		}
	}
	if (baseItem.bItemCondition != actualItem.bItemCondition)
		return FALSE;
	if (baseItem.ubImprintID != actualItem.ubImprintID)
		return FALSE;

	return TRUE;
}



// removes ubHowMany items of usItemIndex with the matching Info from dealer ubArmsDealer
void RemoveItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany )
{
	PERFORMANCE_MARKER
	if ( ubHowMany == 0)
	{
		return;
	}
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
		iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		if (iter->object.usItem == usItemIndex) {
			if (iter->bItemCondition > 0) {
				if (ubHowMany >= iter->object.ubNumberOfObjects) {
					ubHowMany -= iter->object.ubNumberOfObjects;
					gArmsDealersInventory[ ubArmsDealer ].erase(iter);
					if (ubHowMany > 0) {
						return;
					}
				}
				else {
					for (int x = 0; x < ubHowMany; ++x) {
						iter->object.RemoveTopObjectFromStack();
					}
					return;
				}
			}
		}
	}
}



void RemoveRandomItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany )
{
	PERFORMANCE_MARKER

	// not permitted for repair dealers - would take extra code to subtract items under repair from ubTotalItems!!!
	Assert( !DoesDealerDoRepairs( ubArmsDealer ) );

	while ( ubHowMany > 0)
	{
		int numItems = 0;
		std::vector<DealerItemList::iterator> foundObjects;
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();
			iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
			if (iter->object.usItem == usItemIndex) {
				foundObjects.push_back(iter);
				numItems += iter->object.ubNumberOfObjects;
			}
		}
		if (numItems == 0) {
			return;
		}

		// pick a random one to get rid of
		UINT8 ubWhichOne = (UINT8)Random(numItems);
		TODO
		ubHowMany--;
	}
}


BOOLEAN AddDeadArmsDealerItemsToWorld( UINT8 ubMercID )
{
	PERFORMANCE_MARKER
	INT8	bArmsDealer;
	//Get Dealer ID from from merc Id
	bArmsDealer = GetArmsDealerIDFromMercID( ubMercID );
	if( bArmsDealer == -1 )
	{
		// not a dealer, that's ok, we get called for every dude that croaks.
		return( FALSE );
	}

	SOLDIERTYPE	*pSoldier;
	UINT16	usItemIndex;	
	UINT8 ubElement;
	UINT8 ubHowManyMaxAtATime;
	UINT8 ubLeftToDrop;
	UINT8	ubNowDropping;
	DEALER_SPECIAL_ITEM *pSpecialItem;

	// mark the dealer as being out of business!
	gArmsDealerStatus[ bArmsDealer ].fOutOfBusiness = TRUE;


	//Get a pointer to the dealer
	pSoldier = FindSoldierByProfileID( ubMercID, FALSE );
	if( pSoldier == NULL )
	{
		// This should never happen, a dealer getting knocked off without the sector being loaded, should it?
		// If it's possible, we should modify code below to dump his belongings into the sector without using pSoldier->sGridNo
		Assert(0);
		return( FALSE );
	}

	//loop through all the items in the dealer's inventory, and drop them all where the dealer was set up.
	for (DealerItemList::iterator iter = gArmsDealersInventory[bArmsDealer].begin();
		iter != gArmsDealersInventory[bArmsDealer].end(); ++iter) {
		AddItemToPool( pSoldier->sInitialGridNo, &(iter->object), INVISIBLE, 0, 0, 0 );
	}

	gArmsDealersInventory[bArmsDealer].clear();


	//if the dealer has money
	if( gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash > 0 )
	{
		//Create the object
		if( !CreateMoney( gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash, &gTempObject ) )
		{
			return( FALSE );
		}

		//add the money item to the dealers feet
		AddItemToPool( pSoldier->sInitialGridNo, &gTempObject, INVISIBLE, 0, 0, 0 );

		gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash = 0;
	}
	return( TRUE );
}


void MakeObjectOutOfDealerItems( DEALER_SPECIAL_ITEM *pSpclItemInfo, OBJECTTYPE *pObject )
{
	PERFORMANCE_MARKER
	INT8 bItemCondition;
	UINT8 ubCnt;

	*pObject = pSpclItemInfo->object;

	bItemCondition = pSpclItemInfo->bItemCondition;
	//if the item condition is below 0, the item is in for repairs, so flip the sign
	if( bItemCondition < 0 )
	{
		bItemCondition *= -1;
	}

	(*pObject)[0]->data.objectStatus = bItemCondition;

	// set the ImprintID
	pObject->ubImprintID = pSpclItemInfo->ubImprintID;

	// if it's a gun
	if (Item [ pObject->usItem ].usItemClass == IC_GUN )
	{
		// Empty out the bullets put in by CreateItem().	We now sell all guns empty of bullets.	This is so that we don't 
		// have to keep track of #bullets in a gun throughout dealer inventory.	Without this, players could "reload" guns
		// they don't have ammo for by selling them to Tony & buying them right back fully loaded!	One could repeat this
		// ad nauseum (empty the gun between visits) as a (really expensive) way to get unlimited special ammo like rockets.
		(*pObject)[0]->data.gun.ubGunShotsLeft = 0;
	}
}



void GiveObjectToArmsDealerForRepair( UINT8 ubArmsDealer, OBJECTTYPE *pObject, UINT8 ubOwnerProfileId )
{
	PERFORMANCE_MARKER


	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// Any object passed into here must already be:
	//		a) Unstacked
	Assert( pObject->ubNumberOfObjects == 1 );

	//		b) Repairable
	Assert( CanDealerRepairItem( ubArmsDealer, pObject->usItem ) );

	//		c) Actually damaged, or a rocket rifle (being reset)
	Assert( ( (*pObject)[0]->data.objectStatus < 100 ) || ItemIsARocketRifle( pObject->usItem ) );

/* ARM: Can now repair with removeable attachments still attached...
	//		d) Already stripped of all *detachable* attachments
*/

	//		e) If a gun, stripped of any non-ammo-class GunAmmoItems, and bullets
	if (Item [ pObject->usItem ].usItemClass == IC_GUN )
	{
		// if any GunAmmoItem is specified
		if( (*pObject)[0]->data.gun.usGunAmmoItem != NONE)
		{
			// it better be regular ammo, and empty
			Assert( Item[ (*pObject)[0]->data.gun.usGunAmmoItem ].usItemClass == IC_AMMO );
			Assert( (*pObject)[0]->data.gun.ubGunShotsLeft == 0 );
		}
	}

	// ok, given all that, now everything is easy!
	// if the gun was jammed, this will forget about the jam (i.e. dealer immediately unjams anything he will be repairing)
	GiveItemToArmsDealerforRepair( ubArmsDealer, pObject, ubOwnerProfileId );
}


//PLEASE: Use GiveObjectToArmsDealerForRepair() instead of this when repairing a item in OBJECTTYPE format.
void GiveItemToArmsDealerforRepair( UINT8 ubArmsDealer, OBJECTTYPE* pObject, UINT8 ubOwnerProfileId )
{
	PERFORMANCE_MARKER
	UINT32	uiTimeWhenFreeToStartIt;
	UINT32	uiMinutesToFix;
	UINT32	uiMinutesShopClosedBeforeItsDone;
	UINT32	uiDoneWhen;


	Assert( DoesDealerDoRepairs( ubArmsDealer ) );
	Assert( (*pObject)[0]->data.objectStatus > 0 );
	Assert( ( (*pObject)[0]->data.objectStatus < 100 ) || ItemIsARocketRifle( pObject->usItem ) );

	// figure out the earliest the repairman will be free to start repairing this item
	uiTimeWhenFreeToStartIt = WhenWillRepairmanBeAllDoneRepairing( ubArmsDealer );

	//Determine how long it will take to fix
	uiMinutesToFix = CalculateObjectItemRepairTime( ubArmsDealer, pObject );

	uiMinutesShopClosedBeforeItsDone = CalculateOvernightRepairDelay( ubArmsDealer, uiTimeWhenFreeToStartIt, uiMinutesToFix );

	// clock time when this will finally be ready
	uiDoneWhen = uiTimeWhenFreeToStartIt + uiMinutesToFix + uiMinutesShopClosedBeforeItsDone;

	// Negate the status
	(*pObject)[0]->data.objectStatus *= -1;

	gArmsDealersInventory[ ubArmsDealer ].push_back(DEALER_SPECIAL_ITEM());
	gArmsDealersInventory[ ubArmsDealer ].back().object = *pObject;

	//Set the time at which item will be fixed
	gArmsDealersInventory[ ubArmsDealer ].back().uiRepairDoneTime = uiDoneWhen;
	//Remember the original owner of the item
	gArmsDealersInventory[ ubArmsDealer ].back().ubOwnerProfileId = ubOwnerProfileId;
}



UINT32 WhenWillRepairmanBeAllDoneRepairing( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	UINT32	uiWhenFree;
	UINT16	usItemIndex;
	UINT8		ubElement;

	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// if nothing is in for repairs, he'll be free RIGHT NOW!
	uiWhenFree = GetWorldTotalMin();

	//loop through the dealers inventory
	for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
		iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
		if ( iter->fActive )
		{
			//if the item is in for repairs
			if( iter->bItemCondition < 0 )
			{
				// if this item will be done later than the latest we've found so far
				if ( iter->uiRepairDoneTime > uiWhenFree )
				{
					// then we're busy til then!
					uiWhenFree = iter->uiRepairDoneTime;
				}
			}
		}
	}
	return( uiWhenFree );
}


UINT32 CalculateObjectItemRepairTime( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject )
{
	PERFORMANCE_MARKER
	UINT32 uiRepairTime = 0;

	for (int x = 0; x < pItemObject->ubNumberOfObjects; ++x) {
		uiRepairTime += CalculateSimpleItemRepairTime( ubArmsDealer, pItemObject->usItem, (*pItemObject)[x]->data.objectStatus );

		// add time to repair any attachments on it
		for (attachmentList::iterator iter = (*pItemObject)[x]->attachments.begin(); iter != (*pItemObject)[x]->attachments.end(); ++iter) {
			// if damaged and repairable
			if ( ( (*iter)[x]->data.objectStatus < 100 ) && CanDealerRepairItem( ubArmsDealer, iter->usItem ) )
			{
				//uiRepairTime += CalculateSimpleItemRepairTime( ubArmsDealer, iter->usItem, (*iter)[x]->data.objectStatus );
				uiRepairTime += CalculateObjectItemRepairTime( ubArmsDealer, &(*iter) );
			}
		}
	}

	return( uiRepairTime );
}


UINT32 CalculateSimpleItemRepairTime( UINT8 ubArmsDealer, UINT16 usItemIndex, INT8 bItemCondition )
{
	PERFORMANCE_MARKER
	UINT32	uiTimeToRepair = 0;
	UINT32	uiRepairCost = 0;

	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// first calc what he'll charge - that takes care of item condition, repair ease, and his repair cost "markup"
	uiRepairCost = CalculateSimpleItemRepairCost( ubArmsDealer, usItemIndex, bItemCondition );

	// Now adjust that for the repairman's individual repair speed.
	// For a repairman, his BUY modifier controls his REPAIR SPEED (1.0 means minutes to repair = price in $)
	// with a REPAIR SPEED of 1.0, typical gun price of $2000, and a REPAIR COST of 0.5 this works out to 16.6 hrs
	//		for a full 100% status repair...	Not bad.
	uiTimeToRepair = (UINT32)( uiRepairCost * ArmsDealerInfo[ ubArmsDealer ].dRepairSpeed );

	// repairs on electronic items take twice as long if the guy doesn't have the skill
	// for dealers, this means anyone but Fredo the Electronics guy takes twice as long (but doesn't charge double)
	// (Mind you, current he's the ONLY one who CAN repair Electronics at all!	Oh well.)
//	if( ( Item[ usItemIndex ].fFlags & ITEM_ELECTRONIC ) && ( ubArmsDealer != ARMS_DEALER_FREDO ) )
	if( ( Item[ usItemIndex ].electronic	) && ( ubArmsDealer != ARMS_DEALER_FREDO ) )
	{
		uiTimeToRepair *= 2;
	}

	// avoid "instant" repairs on really cheap, barely damaged crap...
	if (uiTimeToRepair < MIN_REPAIR_TIME_IN_MINUTES)
	{
		uiTimeToRepair = MIN_REPAIR_TIME_IN_MINUTES;
	}

	return( uiTimeToRepair );
}



UINT32 CalculateSpecialItemRepairCost( UINT8 ubArmsDealer, UINT16 usItemIndex, SPECIAL_ITEM_INFO *pSpclItemInfo )
{
	PERFORMANCE_MARKER
	UINT32 uiRepairCost;
	UINT8 ubCnt;

	uiRepairCost = CalculateSimpleItemRepairCost( ubArmsDealer, usItemIndex, pSpclItemInfo->bItemCondition );

	// add cost of repairing any attachments on it
	for ( ubCnt = 0; ubCnt < MAX_ATTACHMENTS; ubCnt++ )
	{
		if ( pSpclItemInfo->usAttachment[ ubCnt ] != NONE )
		{
			// if damaged and repairable
			if ( ( pSpclItemInfo->bAttachmentStatus[ ubCnt ] < 100 ) && CanDealerRepairItem( ubArmsDealer, pSpclItemInfo->usAttachment[ ubCnt ] ) )
			{
				uiRepairCost += CalculateSimpleItemRepairCost( ubArmsDealer, pSpclItemInfo->usAttachment[ ubCnt ], pSpclItemInfo->bAttachmentStatus[ ubCnt ] );
			}
		}
	}

	return( uiRepairCost );
}


UINT32 CalculateObjectItemRepairCost( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject )
{
	PERFORMANCE_MARKER
	UINT32 uiRepairCost = 0;

	for (int x = 0; x < pItemObject->ubNumberOfObjects; ++x) {
		uiRepairCost = CalculateSimpleItemRepairCost( ubArmsDealer, pItemObject->usItem, (*pItemObject)[x]->data.objectStatus );

		// add cost of repairing any attachments on it
		for (attachmentList::iterator iter = (*pItemObject)[x]->attachments.begin(); iter != (*pItemObject)[x]->attachments.end(); ++iter) {
			// if damaged and repairable
			if ( ( (*iter)[x]->data.objectStatus < 100 ) && CanDealerRepairItem( ubArmsDealer, iter->usItem ) )
			{
				uiRepairCost += CalculateObjectItemRepairCost( ubArmsDealer, &(*iter));
			}
		}
	}

	return( uiRepairCost );
}


UINT32 CalculateSimpleItemRepairCost( UINT8 ubArmsDealer, UINT16 usItemIndex, INT8 bItemCondition )
{
	PERFORMANCE_MARKER
	UINT32	uiItemCost = 0;
	UINT32	uiRepairCost = 0;
	INT16		sRepairCostAdj = 0;
//	UINT32	uiDifFrom10=0;

	// figure out the full value of the item, modified by this dealer's personal Sell (i.e. repair cost) modifier
	// don't use CalcShopKeeperItemPrice - we want FULL value!!!
	uiItemCost = (UINT32)(( Item[ usItemIndex ].usPrice * ArmsDealerInfo[ ubArmsDealer ].dRepairCost ) );

	// get item's repair ease, for each + point is 10% easier, each - point is 10% harder to repair
	sRepairCostAdj = 100 - ( 10 * Item[ usItemIndex ].bRepairEase );

	// make sure it ain't somehow gone too low!
	if (sRepairCostAdj < 10)
	{
		sRepairCostAdj = 10;
	}

	// calculate repair cost, the more broken it is the more it costs, and the difficulty of repair it is also a factor
	uiRepairCost = (UINT32)( uiItemCost * ( sRepairCostAdj * (100 - bItemCondition) / ((FLOAT)100 * 100) ));

/*
	//if the price is not diviseble by 10, make it so
	uiDifFrom10 = 10 - uiRepairCost % 10;
	if( uiDifFrom10 != 0 )
	{
		uiRepairCost += uiDifFrom10;
	}
*/

	if ( ItemIsARocketRifle( usItemIndex ) )
	{
		// resetting imprinting for a rocket rifle costs something extra even if rifle is at 100%
		uiRepairCost += 100;
	}


	// anything repairable has to have a minimum price
	if ( uiRepairCost < MIN_REPAIR_COST )
	{
		uiRepairCost = MIN_REPAIR_COST;
	}

	return( uiRepairCost );
}



void SetSpecialItemInfoToDefaults( SPECIAL_ITEM_INFO *pSpclItemInfo )
{
	PERFORMANCE_MARKER
	UINT8 ubCnt;

	pSpclItemInfo->initialize();

	pSpclItemInfo->bItemCondition = 100;
	pSpclItemInfo->ubImprintID = NO_PROFILE;

	for ( ubCnt = 0; ubCnt < MAX_ATTACHMENTS; ubCnt++ )
	{
		pSpclItemInfo->usAttachment[ ubCnt ] = NONE;
		pSpclItemInfo->bAttachmentStatus[ ubCnt ] = 0;
	}
}


BOOLEAN IsItemInfoSpecial( SPECIAL_ITEM_INFO *pSpclItemInfo, UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	UINT8 ubCnt;


	// being damaged / in repairs makes an item special
	if ( pSpclItemInfo->bItemCondition != 100 )
	{
		return(TRUE);
	}

	// being imprinted makes an item special
	if (pSpclItemInfo->ubImprintID != NO_PROFILE)
	{
		return(TRUE);
	}

	// having an attachment makes an item special
	// ...unless it is a default attachment (WDS fix 07/24/2007)
	if ( (pSpclItemInfo->usAttachment[ 0 ] != NONE) )
	{
		//ADB: takes the if ubCnt != 0 out of the loop below, and only calcs firstAttachmentIsDefault sometimes,
		//cuz ya know, performance is sooo critical here (not)
		BOOLEAN firstAttachmentIsDefault = 
			((pSpclItemInfo->usAttachment[ 0 ] == Item [ usItemIndex ].defaultattachment) &&
			(pSpclItemInfo->bAttachmentStatus[0] == 100));
		if ( firstAttachmentIsDefault == false)
		{
			return(TRUE);
		}
	}

	for ( ubCnt = 1; ubCnt < MAX_ATTACHMENTS; ubCnt++ )
	{
		if ( (pSpclItemInfo->usAttachment[ ubCnt ] != NONE) )
		{
			return(TRUE);
		}
	}

	// otherwise, it's just a "perfect" item, nothing special about it
	return(FALSE);
}



BOOLEAN DoesItemAppearInDealerInventoryList( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer )
{
	PERFORMANCE_MARKER
	DEALER_POSSIBLE_INV *pDealerInv=NULL;
	UINT16 usCnt;


	// the others will buy only things that appear in their own "for sale" inventory lists
	pDealerInv = GetPointerToDealersPossibleInventory( ubArmsDealer );
	Assert( pDealerInv != NULL );

	// loop through the dealers' possible inventory and see if the item exists there
	usCnt = 0;
	while( pDealerInv[ usCnt ].sItemIndex != LAST_DEALER_ITEM )
	{
		//if the initial dealer inv contains the required item, the dealer can sell the item
		if( pDealerInv[ usCnt ].sItemIndex == usItemIndex )
		{
			// if optimal quantity listed is 0, it means dealer won't sell it himself, but will buy it from the player!
			if ( ( pDealerInv[ usCnt ].ubOptimalNumber > 0 ) || fPurchaseFromPlayer )
			{
				return( TRUE );
			}
		}

		usCnt++;
	}

	return( FALSE );
}



UINT16 CalcValueOfItemToDealer( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerSelling )
{
	PERFORMANCE_MARKER
	UINT16 usBasePrice;
	UINT8 ubItemPriceClass;
	UINT8 ubDealerPriceClass;
	UINT16 usValueToThisDealer;


	usBasePrice = Item[ usItemIndex ].usPrice;

	if ( usBasePrice == 0 )
	{
		// worthless to any dealer
		return( 0 );
	}


	// figure out the price class this dealer prefers
	switch ( ubArmsDealer )
	{
		case ARMS_DEALER_JAKE:
			ubDealerPriceClass = PRICE_CLASS_JUNK;
			break;
		case ARMS_DEALER_KEITH:
			ubDealerPriceClass = PRICE_CLASS_CHEAP;
			break;
		case ARMS_DEALER_FRANZ:
			ubDealerPriceClass = PRICE_CLASS_EXPENSIVE;
			break;

		// other dealers don't use this system
		default:
			if ( DoesItemAppearInDealerInventoryList( ubArmsDealer, usItemIndex, TRUE ) )
			{
				return( usBasePrice );
			}
			else
			{
				return( 0 );
			}
	}


	// the rest of this function applies only to the "general" dealers ( Jake, Keith, and Franz )

	// Micky & Gabby specialize in creature parts & such, the others don't buy these at all (exception: jars)
	if ( ( !Item[usItemIndex].jar ) &&
				( DoesItemAppearInDealerInventoryList( ARMS_DEALER_MICKY, usItemIndex, TRUE ) ||
					DoesItemAppearInDealerInventoryList( ARMS_DEALER_GABBY, usItemIndex, TRUE ) ) )
	{
		return( 0 );
	}

	if ( ( ubArmsDealer == ARMS_DEALER_KEITH ) && ( Item [ usItemIndex].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) )
	{
		// Keith won't buy guns until the Hillbillies are vanquished
		if( CheckFact( FACT_HILLBILLIES_KILLED, KEITH ) == FALSE )
		{
			return( 0 );
		}
	}


	// figure out which price class it belongs to
	if ( usBasePrice < 100 )
	{
		ubItemPriceClass = PRICE_CLASS_JUNK;
	}
	else
	if ( usBasePrice < 1000 )
	{
		ubItemPriceClass = PRICE_CLASS_CHEAP;
	}
	else
	{
		ubItemPriceClass = PRICE_CLASS_EXPENSIVE;
	}


	if( !fDealerSelling )
	{
		// junk dealer won't buy expensive stuff at all, expensive dealer won't buy junk at all
		if ( abs( (INT8) ubDealerPriceClass - (INT8) ubItemPriceClass ) == 2 )
		{
			return( 0 );
		}
	}

	// start with the base price
	usValueToThisDealer = usBasePrice;

	// if it's out of their preferred price class
	if ( ubDealerPriceClass != ubItemPriceClass )
	{
		// exception: Gas (Jake's)
//		if ( usItemIndex != GAS_CAN )
		if ( !Item[usItemIndex].gascan )
		{
			// they pay only 1/3 of true value!
			usValueToThisDealer /= 3;
		}
	}

	// Tony specializes in guns, weapons, and ammo, so make others pay much less for that kind of stuff
	if ( DoesItemAppearInDealerInventoryList( ARMS_DEALER_TONY, usItemIndex, TRUE ) )
	{
		// others pay only 1/2 of that value!
		usValueToThisDealer /= 2;
	}

	// minimum bet $1 !
	if ( usValueToThisDealer == 0 )
	{
		usValueToThisDealer = 1;
	}

	return( usValueToThisDealer );
}


BOOLEAN DealerItemIsSafeToStack( UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	// basically any item type with nothing unique about it besides its status can be stacked in dealer's inventory boxes...
	// NOTE: This test is only applied to items already KNOWN to be perfect - special items are obviously not-stackable

	if ( Item[ usItemIndex ].usItemClass == IC_GUN )
	{
		return( FALSE );
	}

	return( TRUE );
}



void GuaranteeMinimumAlcohol( UINT8 ubArmsDealer )
{
	PERFORMANCE_MARKER
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, BEER,		( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, BEER ) / 3 ) );
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, WINE,		( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, WINE ) / 3 ) );
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, ALCOHOL, ( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, ALCOHOL ) / 3 ) );
}



BOOLEAN ItemIsARocketRifle( INT16 sItemIndex )
{
	PERFORMANCE_MARKER
	if ( Item[sItemIndex].fingerprintid	)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}



BOOLEAN GetArmsDealerShopHours( UINT8 ubArmsDealer, UINT32 *puiOpeningTime, UINT32 *puiClosingTime )
{
	PERFORMANCE_MARKER
	SOLDIERTYPE *pSoldier;

	pSoldier = FindSoldierByProfileID( ArmsDealerInfo[ ubArmsDealer ].ubShopKeeperID, FALSE );
	if ( pSoldier == NULL )
	{
		return( FALSE );
	}

	if ( ExtractScheduleDoorLockAndUnlockInfo( pSoldier, puiOpeningTime, puiClosingTime ) == FALSE )
	{
		return( FALSE );
	}

	Assert( *puiOpeningTime < *puiClosingTime );

	return( TRUE );
}



UINT32 CalculateOvernightRepairDelay( UINT8 ubArmsDealer, UINT32 uiTimeWhenFreeToStartIt, UINT32 uiMinutesToFix )
{
	PERFORMANCE_MARKER
	UINT32 uiOpeningTime, uiClosingTime;
	UINT32 uiMinutesClosedOvernight;
	UINT32 uiDelayInDays = 0;
	UINT32 uiDoneToday;


	Assert( uiMinutesToFix > 0 );

	// convert world time into 24hr military time for the day he's gonna start on it
	uiTimeWhenFreeToStartIt = uiTimeWhenFreeToStartIt % NUM_MIN_IN_DAY;

	if ( GetArmsDealerShopHours( ubArmsDealer, &uiOpeningTime, &uiClosingTime ) == FALSE )
	{
		return( 0 );
	}

	// if it won't get done by the end of a day
	while ( ( uiTimeWhenFreeToStartIt + uiMinutesToFix ) > uiClosingTime )
	{
		// this is to handle existing saves with overnight repairs
		if ( uiTimeWhenFreeToStartIt < uiClosingTime )
		{
			// he gets this much done before closing
			uiDoneToday = uiClosingTime - uiTimeWhenFreeToStartIt;
			// subtract how much he got done
			uiMinutesToFix -= uiDoneToday;
			Assert( uiMinutesToFix > 0 );
		}

		// he starts back at it first thing in the morning
		uiTimeWhenFreeToStartIt = uiOpeningTime;
		uiDelayInDays++;
	}

	uiMinutesClosedOvernight = NUM_MIN_IN_DAY - ( uiClosingTime - uiOpeningTime );

	return ( uiDelayInDays * uiMinutesClosedOvernight );
}



UINT32 CalculateMinutesClosedBetween( UINT8 ubArmsDealer, UINT32 uiStartTime, UINT32 uiEndTime )
{
	PERFORMANCE_MARKER
	UINT32 uiOpeningTime, uiClosingTime;
	UINT32 uiMinutesClosedOvernight;
	UINT32 uiDaysDifference = 0;
	UINT32 uiMinutesClosed = 0;

	Assert( uiStartTime <= uiEndTime );

	if ( GetArmsDealerShopHours( ubArmsDealer, &uiOpeningTime, &uiClosingTime ) == FALSE )
	{
		return( 0 );
	}

	uiMinutesClosedOvernight = NUM_MIN_IN_DAY - ( uiClosingTime - uiOpeningTime );

	// NOTE: this assumes stored are only closed overnight, so all we have to do is compare the day portion
	uiDaysDifference = ( uiEndTime / NUM_MIN_IN_DAY ) - ( uiStartTime / NUM_MIN_IN_DAY );

	if ( uiDaysDifference >= 2 )
	{
		// close for 1 less than that many full nights...
		uiMinutesClosed = ( uiDaysDifference - 1 ) * uiMinutesClosedOvernight;
	}


	// add partial day's closing

	// convert start and end times into 24hr military time
	uiStartTime = uiStartTime % NUM_MIN_IN_DAY;
	uiEndTime = uiEndTime % NUM_MIN_IN_DAY;

	// treat end time of midnight as 24:00 hours to prevent indefinite recursion and make formulas work
	if ( uiEndTime == 0 )
	{
		uiEndTime = NUM_MIN_IN_DAY;
	}


	if ( uiStartTime == uiEndTime )
	{
		if ( uiDaysDifference == 0 )
		{
			return( 0 );
		}
		else
		{
			uiMinutesClosed += uiMinutesClosedOvernight;
		}
	}
	if ( uiStartTime < uiEndTime )
	{
		if ( uiStartTime < uiOpeningTime )
		{
			// add how many minutes in the time range BEFORE the store opened that day
			uiMinutesClosed += ( min( uiOpeningTime, uiEndTime ) - uiStartTime );
		}

		if ( uiEndTime > uiClosingTime )
		{
			// add how many minutes in the time range AFTER the store closed that day
			uiMinutesClosed += ( uiEndTime - max( uiClosingTime, uiStartTime ) );
		}
	}
	else
	{
		Assert( uiEndTime < uiStartTime );

		// recursive calls!	Add two separate times: before midnight, and after midnight
		uiMinutesClosed += CalculateMinutesClosedBetween( ubArmsDealer, uiStartTime, NUM_MIN_IN_DAY );
		uiMinutesClosed += CalculateMinutesClosedBetween( ubArmsDealer, 0, uiEndTime );
	}

	return ( uiMinutesClosed );
}
