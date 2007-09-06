
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"

OBJECTTYPE gTempObject;
OBJECTTYPE gStackTempObject;
std::vector<LBENODE>	LBEArray;

void CreateLBE (OBJECTTYPE* pObj, UINT8 ubID, int numSubPockets)
{
	int index = GetFreeLBEPackIndex();
	LBEArray[index].ubID = ubID;
	LBEArray[index].lbeIndex = Item[pObj->usItem].ubClassIndex;
	LBEArray[index].lbeClass = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass;
	LBEArray[index].inv.clear();
	LBEArray[index].inv.resize(numSubPockets);
	(*pObj)[0]->data.misc.bDetonatorType = -1;
	(*pObj)[0]->data.misc.usBombItem = index;
}

bool DestroyLBEIfEmpty(OBJECTTYPE* pObj)
{
	LBENODE* pLBE = pObj->GetLBEPointer();
	for (unsigned int x = 0; x < pLBE->inv.size(); ++x) {
		if (pLBE->inv[x].exists() == true) {
			return true;
		}
	}
	pLBE->initialize();
	(*pObj)[0]->data.misc.usBombItem = 0;
	(*pObj)[0]->data.misc.bDetonatorType = 0;
	return true;
}

void MoveItemsInSlotsToLBE( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, LBENODE* pLBE, OBJECTTYPE* pObj)
{
	for(unsigned int i=0; i<LBESlots.size(); i++)	// Go through default pockets one by one
	{
		if(pSoldier->inv[LBESlots[i]].exists() == false)	// No item in this pocket
			continue;

		// Found an item in a default pocket so get it's ItemSize
		UINT16 dSize = CalculateItemSize(&pSoldier->inv[LBESlots[i]]);
		for(unsigned int j=0; j<pLBE->inv.size(); j++)	// Search through LBE and see if item fits anywhere
		{
			if(pLBE->inv[j].exists() == true)	// Item already stored in LBENODE pocket
				continue;
			// No item in this LBENODE pocket, is pocket active?
			if(LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j] == NONE)	// Pocket is inactive
				continue;
			// Pocket is active, can default item fit in this pocket?
			if(LBEPocketType[LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j]].ItemCapacityPerSize[dSize] == NONE)	// Pocket can't hold this item size
				continue;
			// Default item will fit in this pocket.  Setup the LBENODE if necessary
			pSoldier->inv[LBESlots[i]].MoveThisObjectTo(pLBE->inv[j]);
			if (pSoldier->inv[LBESlots[i]].exists() == false) {
				break;
			}
			else {
				//move didn't work, maybe it was a stack?
				dSize = CalculateItemSize(&pSoldier->inv[LBESlots[i]]);
			}
		}
	}
	return;
}

extern BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement );

// CHRISL: New function to move items from default pockets to usable pockets
BOOLEAN MoveItemsToActivePockets( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, UINT32 uiHandPos, OBJECTTYPE *pObj )
{
	BOOLEAN	flag=FALSE;

	if(pObj->IsLBE() == false) {
		CreateLBE(pObj, pSoldier->ubID, LBESlots.size());
	}

	LBENODE* pLBE = &(LBEArray[pObj->GetLBEIndex()]);
	MoveItemsInSlotsToLBE(pSoldier, LBESlots, pLBE, pObj);
	DestroyLBEIfEmpty(pObj);

	// We've put everything into the LBENODE that we could, now search other pockets for openings
	for(unsigned int x=0; x<LBESlots.size(); x++)
	{
		if(pSoldier->inv[LBESlots[x]].exists() == false)
			continue;
		for(int i=BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
		{
			if(pSoldier->inv[i].exists() == true)	// Item already in that location
				continue;
			for(unsigned int j=0; j<LBESlots.size(); j++)
			{
				flag = FALSE;
				if(i==LBESlots[j])
				{
					flag=true;
					break;
				}
			}
			if(flag)
				continue;
			if(CanItemFitInPosition(pSoldier, &(pSoldier->inv[LBESlots[x]]), i, FALSE))
			{
				pSoldier->inv[LBESlots[x]].MoveThisObjectTo(pSoldier->inv[i]);
				break;
			}
		}
	}


	// now drop everything that wouldn't fit anywhere else
	for(unsigned int i=0; i<LBESlots.size() ;i++)
	{
		if(pSoldier->inv[LBESlots[i]].exists() == false)	// No item in pocket
			continue;
		AddItemToPool( pSoldier->sGridNo, &pSoldier->inv[LBESlots[i]], 1, pSoldier->stats.bExpLevel, 0 , -1 );
		NotifySoldiersToLookforItems( );
		DeleteObj(&pSoldier->inv[LBESlots[i]]);
	}

	return(TRUE);
}

void GetLBESlots(UINT32 LBEType, std::vector<INT8>& LBESlots)
{
	switch (LBEType)
	{
		case VESTPOCKPOS:
			LBESlots.push_back(SMALLPOCK1POS);
			LBESlots.push_back(SMALLPOCK2POS);
			LBESlots.push_back(SMALLPOCK3POS);
			LBESlots.push_back(SMALLPOCK4POS);
			LBESlots.push_back(SMALLPOCK5POS);
			LBESlots.push_back(SMALLPOCK6POS);
			LBESlots.push_back(SMALLPOCK7POS);
			LBESlots.push_back(SMALLPOCK8POS);
			LBESlots.push_back(SMALLPOCK9POS);
			LBESlots.push_back(SMALLPOCK10POS);
			LBESlots.push_back(MEDPOCK1POS);
			LBESlots.push_back(MEDPOCK2POS);
			break;
		case LTHIGHPOCKPOS:
			LBESlots.push_back(SMALLPOCK11POS);
			LBESlots.push_back(SMALLPOCK12POS);
			LBESlots.push_back(SMALLPOCK13POS);
			LBESlots.push_back(SMALLPOCK14POS);
			LBESlots.push_back(MEDPOCK3POS);
			break;
		case RTHIGHPOCKPOS:
			LBESlots.push_back(SMALLPOCK15POS);
			LBESlots.push_back(SMALLPOCK16POS);
			LBESlots.push_back(SMALLPOCK17POS);
			LBESlots.push_back(SMALLPOCK18POS);
			LBESlots.push_back(MEDPOCK4POS);
			break;
		case CPACKPOCKPOS:
			LBESlots.push_back(SMALLPOCK19POS);
			LBESlots.push_back(SMALLPOCK20POS);
			LBESlots.push_back(SMALLPOCK21POS);
			LBESlots.push_back(SMALLPOCK22POS);
			LBESlots.push_back(BIGPOCK1POS);
			LBESlots.push_back(BIGPOCK2POS);
			LBESlots.push_back(BIGPOCK3POS);
			break;
		case BPACKPOCKPOS:
			LBESlots.push_back(SMALLPOCK23POS);
			LBESlots.push_back(SMALLPOCK24POS);
			LBESlots.push_back(SMALLPOCK25POS);
			LBESlots.push_back(SMALLPOCK26POS);
			LBESlots.push_back(SMALLPOCK27POS);
			LBESlots.push_back(SMALLPOCK28POS);
			LBESlots.push_back(SMALLPOCK29POS);
			LBESlots.push_back(SMALLPOCK30POS);
			LBESlots.push_back(BIGPOCK4POS);
			LBESlots.push_back(BIGPOCK5POS);
			LBESlots.push_back(BIGPOCK6POS);
			LBESlots.push_back(BIGPOCK7POS);
			break;
		default:
			DebugBreak();
	}
	return;
}


// CHRISL: New function to handle moving soldier items to lbe items
BOOLEAN MoveItemToLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj )
{
	std::vector<INT8> LBESlots;
	// Determine which LBE item we're removing so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	CreateLBE(&(pSoldier->inv[uiHandPos]), pSoldier->ubID, LBESlots.size());
	LBENODE* pLBE = pSoldier->inv[uiHandPos].GetLBEPointer();
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this pocket?
		if(pSoldier->inv[LBESlots[i]].exists() == true)
		{
			pSoldier->inv[LBESlots[i]].MoveThisObjectTo(pLBE->inv[i]);
		}
	}

	if (DestroyLBEIfEmpty(&pSoldier->inv[uiHandPos]) == true) {
		return(FALSE);
	}

	return (TRUE);
}

// CHRISL: New function to handle moving lbe items to soldier items
BOOLEAN MoveItemFromLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj )
{
	std::vector<INT8> LBESlots;

	// Determine which LBE item we're adding so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	if(pSoldier->inv[uiHandPos].exists() == false)
		MoveItemsToActivePockets(pSoldier, LBESlots, uiHandPos, pObj);
	if(pObj->IsLBE() == false) {
		return (FALSE);
	}

	LBENODE* pLBE = pObj->GetLBEPointer();
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this LBE pocket?
		if(pLBE->inv[i].exists() == true)
		{
			pLBE->inv[i].MoveThisObjectTo(pSoldier->inv[LBESlots[i]]);
		}
	}
	if (DestroyLBEIfEmpty(pObj) == false) {
		//we should have copied all the items from the LBE to the soldier
		//which means the LBE should be empty and destroyed
		DebugBreak();
	}

	return (TRUE);
}

// CHRISL: Find an unused LBE Pack index
INT16 GetFreeLBEPackIndex( void )
{
	UINT16 uiCount;
	for(uiCount=0; uiCount < LBEArray.size(); uiCount++)
	{
		if ( LBEArray[ uiCount ].lbeIndex == 0)
			return( (INT16)uiCount );
	}

	//ADB this is a mem leak!!!!!!!
	//LBENODE *filler = new LBENODE;
	//LBEArray.push_back(*filler);
	LBEArray.push_back(LBENODE());
	return(uiCount);
}

LBETYPE::LBETYPE(){
	memset(this, 0, SIZEOF_LBETYPE);
	lbePocketIndex.clear();
	lbePocketIndex.resize(MAX_ITEMS_IN_LBE);
}
LBETYPE::LBETYPE(const LBETYPE& src) {
	memcpy(this, &src, SIZEOF_LBETYPE);
	lbePocketIndex = src.lbePocketIndex;
}
LBETYPE& LBETYPE::operator=(const LBETYPE& src){
    if (this != &src) {
		memcpy(this, &src, SIZEOF_LBETYPE);
		lbePocketIndex = src.lbePocketIndex;
    }
    return *this;
}
LBETYPE::~LBETYPE(){
}
POCKETTYPE::POCKETTYPE(){
	memset(this, 0, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(35);
}
POCKETTYPE::POCKETTYPE(const POCKETTYPE& src){
	memcpy(this, &src, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(35);
	ItemCapacityPerSize = src.ItemCapacityPerSize;
}
POCKETTYPE& POCKETTYPE::operator=(const POCKETTYPE& src){
    if (this != &src) {
		memcpy(this, &src, SIZEOF_POCKETTYPE);
		ItemCapacityPerSize = src.ItemCapacityPerSize;
    }
    return *this;
}
POCKETTYPE::~POCKETTYPE(){
}



//ADB TODO: ChrisL, rewrite this to your liking, then insert it everywhere you have (*pObject)[0]->data.misc.bDetonatorType == -1.
//Go ahead and add a new variable to OBJECTTYPE if you want, but then add it to all the functions.
bool OBJECTTYPE::IsLBE()
{
	PERFORMANCE_MARKER
	if (exists() == true) {
		//stacks cannot have mixed types, so who cares if there are more than 1
		return ((*this)[0]->data.misc.bDetonatorType == -1);
	}
	return false;
}

//here we DO care about which subObject in the stack it is
//although currently LBEs cannot be stacked, it could change
LBENODE* OBJECTTYPE::GetLBEPointer(int stackSlot)
{
	PERFORMANCE_MARKER
	return &(LBEArray[GetLBEIndex(stackSlot)]);
}

//here we DO care about which subObject in the stack it is
//although currently LBEs cannot be stacked, it could change
int OBJECTTYPE::GetLBEIndex(int stackSlot)
{
	PERFORMANCE_MARKER
	return (*this)[stackSlot]->data.misc.usBombItem;
}

bool OBJECTTYPE::exists()
{
	PERFORMANCE_MARKER
	if (objectStack.size() > 20) {
		DebugBreak();
	}
	return (ubNumberOfObjects > 0 && usItem != NOTHING);
}

void OBJECTTYPE::SpliceData(OBJECTTYPE& sourceObject, unsigned int numToSplice, StackedObjects::iterator beginIter)
{
	PERFORMANCE_MARKER
	if (numToSplice == 0) {
		return;
	}
	//remember that sometimes objectStack is 1 while ubNumberOfObjects is 0
	if (ubNumberOfObjects == 0) {
		objectStack.clear();
	}
	StackedObjects::iterator stopIter = beginIter;
	for (unsigned int x = 0; x < numToSplice; ++x) {
		++stopIter;
	}
	objectStack.splice(objectStack.begin(), sourceObject.objectStack, beginIter, stopIter);
	if (objectStack.size() > 20) {
		DebugBreak();
	}

	ubNumberOfObjects += numToSplice;
	ubWeight = CalculateObjectWeight(this);

	if (sourceObject.objectStack.empty() == true) {
		//init it so it is not empty, even though it no longer exists
		sourceObject.initialize();
	}
	else {
		sourceObject.ubNumberOfObjects -= numToSplice;
		sourceObject.ubWeight = CalculateObjectWeight(&sourceObject);
	}
}

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	PERFORMANCE_MARKER
	//Keys, triggers, and other specials can never stack, should LBEs?
	if (Item[usItem].usItemClass == IC_KEY
		|| Item[usItem].usItemClass == IC_BOMB
		|| Item[usItem].usItemClass == IC_LBEGEAR
		|| Item[usItem].usItemClass == IC_MONEY) {
		//exit and do not continue
		return 0;
	}
	if ((*this)[0]->data.bTrap > 0) {
		return 0;
	}

	//if howMany is -1 the stack will become full
	int numToAdd = ItemSlotLimit( this, STACK_SIZE_LIMIT ) - ubNumberOfObjects;
	if (howMany >= 0) {
		numToAdd = min(numToAdd, howMany);
	}
	if (numToAdd) {
		CreateItem(usItem, objectStatus, &gStackTempObject);
		for (int x = ubNumberOfObjects; x < ubNumberOfObjects + numToAdd; ++x) {
			objectStack.push_front(gStackTempObject.objectStack.front());
		}

		ubNumberOfObjects += numToAdd;
		ubWeight = CalculateObjectWeight(this);
	}

	//returns how many were NOT added
	return howMany - numToAdd;
}

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& sourceObject, int howMany)
{
	PERFORMANCE_MARKER
	if (exists() == false) {
		//we are adding to an empty object, it can happen
		Assert(sourceObject.exists() == true);
		usItem = sourceObject.usItem;
		this->fFlags = sourceObject.fFlags;
	}
	Assert(sourceObject.usItem == usItem);

	//can't add too much, can't take too many
	int freeObjectsInStack = max(0, (ItemSlotLimit( this, STACK_SIZE_LIMIT ) - ubNumberOfObjects));
	int numToAdd = min (freeObjectsInStack, sourceObject.ubNumberOfObjects);
	//if howMany is -1 the stack will become full if sourceObject has enough
	if (howMany >= 0) {
		numToAdd = min(numToAdd, howMany);
	}

	//stacking control, restrict certain things here
	if (numToAdd > 0) {
		//Keys, triggers, and other specials can never stack, should LBEs?
		if (Item[usItem].usItemClass == IC_KEY
			|| Item[usItem].usItemClass == IC_BOMB
			|| Item[usItem].usItemClass == IC_LBEGEAR) {
			//we could have been asked to put the item into an empty object, in which case we allow 1
			if (ubNumberOfObjects == 0) {
				numToAdd = 1;
			}
			else {
				//exit and do not continue
				return 0;
			}
		}

		if (Item[usItem].usItemClass == IC_MONEY) {
			//money doesn't stack, it merges
			// average out the status values using a weighted average...
			int thisStatus = (*this)[0]->data.money.bMoneyStatus * (*this)[0]->data.money.uiMoneyAmount;
			int sourceStatus = sourceObject[0]->data.money.bMoneyStatus * sourceObject[0]->data.money.uiMoneyAmount;
			int average = (*this)[0]->data.money.uiMoneyAmount + sourceObject[0]->data.money.uiMoneyAmount;
			(*this)[0]->data.objectStatus = ( (thisStatus + sourceStatus) / average);

			(*this)[0]->data.money.uiMoneyAmount += sourceObject[0]->data.money.uiMoneyAmount;
			return 0;
		}

		//for convenience sake, do not allow mixed flags to stack!
		//continue on because you might find something else with the same flags
		if (sourceObject.fFlags != this->fFlags) {
			numToAdd = 0;
		}

		if (exists() == true) {
			if (sourceObject[0]->data.ubImprintID != (*this)[0]->data.ubImprintID) {
				numToAdd = 0;
			}

			//nor allow trapped items to stack
			if (sourceObject[0]->data.bTrap > 0
				|| (*this)[0]->data.bTrap > 0) {
				return 0;
			}
		}
		//ADB TODO, other specials
	}

	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());
	

	//returns how many were NOT added
	if (howMany > 0) {
		return howMany - numToAdd;
	}
	else {
		//-1 means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

void OBJECTTYPE::DuplicateObjectsInStack(OBJECTTYPE& sourceObject, int howMany)
{
	PERFORMANCE_MARKER
	int numToDupe;
	if (howMany >= 0) {
		numToDupe = min(howMany, sourceObject.ubNumberOfObjects);
	}
	else {
		numToDupe = sourceObject.ubNumberOfObjects;
	}

	initialize();
	StackedObjects::iterator iter = sourceObject.objectStack.begin();
	for (int x = 0; x < numToDupe; ++x) {
		objectStack.push_back(*iter);
		++iter;
	}
	ubNumberOfObjects = numToDupe;
	ubWeight = CalculateObjectWeight(this);
	return;
}

int OBJECTTYPE::MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove)
{
	return (RemoveObjectsFromStack(numToMove, &destObject));
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject)
{
	PERFORMANCE_MARKER
	if (howMany == -1) {
		howMany = ubNumberOfObjects;
	}

	int numToRemove = min(ubNumberOfObjects, howMany);
	if (destObject) {
		//destObject should be empty especially if numToRemove is 0
		destObject->initialize();
		if (numToRemove > 0) {
			//this handles the removal too
			return destObject->AddObjectsToStack(*this, numToRemove);
		}
	}
	else if (numToRemove > 0) {
		for (int x = 0; x < numToRemove; ++x) {
			objectStack.pop_front();
		}
		if (objectStack.empty() == true) {
			//init it so it is not empty, even though it no longer exists
			initialize();
		}
		else {
			ubNumberOfObjects -= numToRemove;
			ubWeight = CalculateObjectWeight(this);
		}
	}

	//returns how many were NOT removed
	return howMany - numToRemove;
}

bool OBJECTTYPE::RemoveObjectAtIndex(unsigned int index, OBJECTTYPE* destObject)
{
	PERFORMANCE_MARKER
	if (index >= ubNumberOfObjects || exists() == false) {
		return false;
	}

	if (ubNumberOfObjects == 1) {
		if (destObject) {
			*destObject = *this;
		}
		this->initialize();
		return true;
	}

	if (destObject) {
		//destObject should be empty especially if it fails!!
		destObject->initialize();
	}

	StackedObjects::iterator spliceIter = objectStack.begin();
	for (unsigned int x = 0; x < index; ++x) {
		++spliceIter;
	}

	if (destObject) {
		destObject->usItem = usItem;
		destObject->fFlags = fFlags;
		destObject->SpliceData((*this), 1, spliceIter);
	}
	else {
		objectStack.erase(spliceIter);
		if (objectStack.empty()) {
			this->initialize();
		}
		else {
			ubNumberOfObjects -= 1;
			ubWeight = CalculateObjectWeight(this);
		}
	}

	return true;
}

StackedObjectData* OBJECTTYPE::operator [](const unsigned int index)
{
	PERFORMANCE_MARKER
	if (objectStack.size() > 20) {
		DebugBreak();
	}
	Assert(index < objectStack.size());
	StackedObjects::iterator iter = objectStack.begin();
	for (unsigned int x = 0; x < index; ++x) {
		++iter;
	}
	return &(*iter);
}


//you may have noticed code like this:
//pSoldier->pTempObject	= (OBJECTTYPE *)MemAlloc( sizeof( OBJECTTYPE ) );
//memcpy( pSoldier->pTempObject, pObject, sizeof( OBJECTTYPE ) );
//That's setting yourself up for a memleak if pSoldier->pTempObject isn't null!

//Whenever you see code like that, write this in it's place:
//OBJECTTYPE::CopyToOrCreateAt(&pSoldier->pTempObject, pObject);

//OR write OBJECTTYPE::DeleteMe(&pSoldier->pTempObject) if there is no memcpy
void OBJECTTYPE::CopyToOrCreateAt(OBJECTTYPE** ppTarget, OBJECTTYPE* pSource)
{
	PERFORMANCE_MARKER
	//this is necessary because memcpy is no longer good enough for OBJECTTYPE
	if (*ppTarget == NULL) {
		*ppTarget = new OBJECTTYPE(*pSource);
	}
	else {
		//ADB leaving this in for a while, not sure if the code ever even reaches here and this will tell me
		DebugBreak();
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Found mem leak, but it was corrected."));
		**ppTarget = *pSource;
	}
	return;
}

//OR write OBJECTTYPE::DeleteMe(&pSoldier->pTempObject) if there is no memcpy
void OBJECTTYPE::DeleteMe(OBJECTTYPE** ppObject)
{
	PERFORMANCE_MARKER
	if (*ppObject != NULL) {
		delete *ppObject;
		*ppObject = NULL;
	}
	return;
}

OBJECTTYPE* StackedObjectData::GetAttachmentAtIndex(UINT8 index)
{
	PERFORMANCE_MARKER
	attachmentList::iterator iter = attachments.begin();
	for (int x = 0; x < index && iter != attachments.end(); ++x) {
		++iter;
	}

	if (iter != attachments.end()) {
		return &(*iter);
	}
	return 0;
}

bool StackedObjectData::operator==(StackedObjectData& compare)
{
	return (this->data == compare.data
		&& this->attachments == compare.attachments);
}

bool StackedObjectData::operator==(const StackedObjectData& compare)const
{
	return (this->data == compare.data
		&& this->attachments == compare.attachments);
}

bool ObjectData::operator==(ObjectData& compare)
{
	return (memcmp(this, &compare, sizeof(ObjectData)) == 0);
}

bool ObjectData::operator==(const ObjectData& compare)const
{
	return (memcmp(this, &compare, sizeof(ObjectData)) == 0);
}

// Constructor
OBJECTTYPE::OBJECTTYPE()
{
	PERFORMANCE_MARKER
	initialize();
}
void OBJECTTYPE::initialize()
{
	PERFORMANCE_MARKER
	//ubNumberOfObjects should be 0 so any loop checking it will not work
	//however objectStack should always have at least one, because there are so
	//many uses of (*pObj)[0]->data.objectStatus and such
	memset(this, 0, SIZEOF_OBJECTTYPE_POD);
	//ubNumberOfObjects = 1;

	//this is an easy way to init it and get rid of attachments
	objectStack.clear();
	objectStack.resize(1);
}

bool OBJECTTYPE::operator==(const OBJECTTYPE& compare)const
{
	PERFORMANCE_MARKER
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if (this->objectStack == compare.objectStack) {
			return true;
		}
	}
	return false;
}

bool OBJECTTYPE::operator==(OBJECTTYPE& compare)
{
	PERFORMANCE_MARKER
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if (this->objectStack == compare.objectStack) {
			return true;
		}
	}
	return false;
}

//Copy Ctor
OBJECTTYPE::OBJECTTYPE(const OBJECTTYPE& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->ubWeight = src.ubWeight;
		this->objectStack = src.objectStack;
		this->fFlags = src.fFlags;

		this->ubMission = src.ubMission;
	}
}

// Assignment operator
OBJECTTYPE& OBJECTTYPE::operator=(const OBJECTTYPE& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->ubWeight = src.ubWeight;
		this->objectStack = src.objectStack;
		this->fFlags = src.fFlags;

		this->ubMission = src.ubMission;
	}
	return *this;
}


// Conversion operator
OBJECTTYPE& OBJECTTYPE::operator=(const OLD_OBJECTTYPE_101& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		//makes changes to size easier as we don't have to init new arrays with 0 by hand
		this->initialize();

		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;

		this->ubMission = src.ubMission;

		//in some cases we need to reference the objectStatus or something, even though the item is totally empty
		//therefore, keep ubNumberOfObjects at 0 but resize objectStack to at least 1
		this->objectStack.resize(max(ubNumberOfObjects, 1));
		if (ubNumberOfObjects == 0) {
			if (this->usItem != NONE) {
				DebugBreak();
			}
			this->usItem = NONE;
		}

		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		if (ubNumberOfObjects == 1) {
			memcpy(&((*this)[0]->data.gun), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,sizeof(ObjectData)));

			(*this)[0]->data.bTrap = src.bTrap;		// 1-10 exp_lvl to detect
			(*this)[0]->data.ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
			(*this)[0]->data.fUsed = src.fUsed;				// flags for whether the item is used or not

			//it's unlikely max will get less over the versions, but still, check the min
			for (int x = 0; x < OLD_MAX_ATTACHMENTS_101; ++x)
			{
				if (src.usAttachItem[x] != NOTHING) {
					CreateItem(src.usAttachItem[x], src.bAttachStatus[x], &gTempObject);
					(*this)[0]->attachments.push_back(gTempObject);
				}
			}
		}
		else {
			//we are loading a stack of objects, the union must either be
			//bStatus[OLD_MAX_OBJECTS_PER_SLOT_101] or
			//ubShotsLeft[OLD_MAX_OBJECTS_PER_SLOT_101]
			//and we know it has no attachments

			Version101::OLD_OBJECTTYPE_101_UNION ugYucky;
			memcpy(&ugYucky, &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,sizeof(ObjectData)));

			for (int x = 0; x < max(ubNumberOfObjects, 1); ++x) {
				(*this)[x]->data.objectStatus = ugYucky.bStatus[x];
				(*this)[x]->data.bTrap = src.bTrap;		// 1-10 exp_lvl to detect
				(*this)[x]->data.ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
				(*this)[x]->data.fUsed = src.fUsed;				// flags for whether the item is used or not
			}
		}

		//just a precaution
		this->ubWeight = CalculateObjectWeight(this);
	}
	return *this;
}


OBJECTTYPE::~OBJECTTYPE()
{
	objectStack.clear();
}
