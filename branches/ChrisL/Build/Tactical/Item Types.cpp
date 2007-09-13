
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"
#include "GameSettings.h"

OBJECTTYPE gTempObject;
OBJECTTYPE gStackTempObject;
std::list<LBENODE>	LBEArray;
int gLastLBEUniqueID = 0;

bool IsSlotAnLBESlot(int slot)
{
	PERFORMANCE_MARKER
	switch(slot) {
	case VESTPOCKPOS:
	case LTHIGHPOCKPOS:
	case RTHIGHPOCKPOS:
	case CPACKPOCKPOS:
	case BPACKPOCKPOS:
		return true;
	}
	return false;
}

void CreateLBE (OBJECTTYPE* pObj, UINT8 ubID, int numSubPockets)
{
	PERFORMANCE_MARKER
	int uniqueID;
	LBENODE* pLBE = NULL;
	if (pObj->IsActiveLBE(0) == true) {
		uniqueID = (*pObj)[0]->data.lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				pLBE = &(*iter);
				break;
			}
		}
		Assert(pLBE);
	}
	else {
		uniqueID = gLastLBEUniqueID++;
		LBEArray.push_back(LBENODE());
		pLBE = &LBEArray.back();
		pLBE->uniqueID = uniqueID;
	}
	pLBE->ubID = ubID;
	pLBE->lbeIndex = Item[pObj->usItem].ubClassIndex;
	pLBE->lbeClass = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass;
	pLBE->inv.clear();
	pLBE->inv.resize(numSubPockets);
	(*pObj)[0]->data.lbe.bLBE = -1;
	(*pObj)[0]->data.lbe.uniqueID = uniqueID;
}

bool DestroyLBEIfEmpty(OBJECTTYPE* pObj)
{
	PERFORMANCE_MARKER
	if (pObj->IsActiveLBE(0) == true) {
		LBENODE* pLBE = pObj->GetLBEPointer(0);
		if (pLBE) {
			for (unsigned int x = 0; x < pLBE->inv.size(); ++x) {
				if (pLBE->inv[x].exists() == true) {
					return false;
				}
			}
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == pLBE->uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
			(*pObj)[0]->data.lbe.uniqueID = 0;
			(*pObj)[0]->data.lbe.bLBE = 0;
			return true;
		}
	}
	return false;
}
/*
void DestroyLBE(OBJECTTYPE* pObj)
{
	PERFORMANCE_MARKER
	for (int x = 0; x < pObj->ubNumberOfObjects; ++x) {
		if (pObj->IsActiveLBE(x) == true) {
			LBENODE* pLBE = pObj->GetLBEPointer(x);
			if (pLBE) {
				for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
					if (iter->uniqueID == pLBE->uniqueID) {
						LBEArray.erase(iter);
						break;
					}
				}
				(*pObj)[x]->data.lbe.uniqueID = 0;
				(*pObj)[x]->data.lbe.bLBE = 0;
			}
		}
	}
}
*/
void MoveItemsInSlotsToLBE( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, LBENODE* pLBE, OBJECTTYPE* pObj)
{
	PERFORMANCE_MARKER
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

			//ADB the object is already in the LBE, we are just moving it from soldier inv to LBE inv
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
	PERFORMANCE_MARKER
	BOOLEAN	flag=FALSE;

	if(pObj->IsActiveLBE(0) == false) {
		CreateLBE(pObj, pSoldier->ubID, LBESlots.size());
	}

	LBENODE* pLBE = pObj->GetLBEPointer(0);
	MoveItemsInSlotsToLBE(pSoldier, LBESlots, pLBE, pObj);
	DestroyLBEIfEmpty(pObj);

	// We've put everything into the LBENODE that we could, now search other pockets for openings
	for(unsigned int x=0; x<LBESlots.size(); x++)
	{
		if(pSoldier->inv[LBESlots[x]].exists() == false)
			continue;
		for(int i=INV_START_POS; i<NUM_INV_SLOTS; i++)
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
				pSoldier->inv[LBESlots[x]].MoveThisObjectTo(pSoldier->inv[i], ALL_OBJECTS, pSoldier, i);
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
	PERFORMANCE_MARKER
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
BOOLEAN MoveItemToLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos )
{
	PERFORMANCE_MARKER
	std::vector<INT8> LBESlots;
	// Determine which LBE item we're removing so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	CreateLBE(&(pSoldier->inv[uiHandPos]), pSoldier->ubID, LBESlots.size());
	LBENODE* pLBE = pSoldier->inv[uiHandPos].GetLBEPointer(0);
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this pocket?
		if(pSoldier->inv[LBESlots[i]].exists() == true)
		{
			//ADB the object is already in the LBE, we are just moving it from soldier inv to LBE inv
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
	PERFORMANCE_MARKER
	std::vector<INT8> LBESlots;

	// Determine which LBE item we're adding so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	if(pSoldier->inv[uiHandPos].exists() == false)
		MoveItemsToActivePockets(pSoldier, LBESlots, uiHandPos, pObj);
	if(pObj->IsActiveLBE(0) == false) {
		return (FALSE);
	}

	LBENODE* pLBE = pObj->GetLBEPointer(0);
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this LBE pocket?
		if(pLBE->inv[i].exists() == true)
		{
			pLBE->inv[i].MoveThisObjectTo(pSoldier->inv[LBESlots[i]], ALL_OBJECTS, pSoldier, LBESlots[i]);
		}
	}
	if (DestroyLBEIfEmpty(pObj) == false) {
		//we should have copied all the items from the LBE to the soldier
		//which means the LBE should be empty and destroyed
		DebugBreak();
	}

	return (TRUE);
}

LBETYPE::LBETYPE(){
	PERFORMANCE_MARKER
	memset(this, 0, SIZEOF_LBETYPE);
	lbePocketIndex.clear();
	lbePocketIndex.resize(MAX_ITEMS_IN_LBE);
}
LBETYPE::LBETYPE(const LBETYPE& src) {
	PERFORMANCE_MARKER
	memcpy(this, &src, SIZEOF_LBETYPE);
	lbePocketIndex = src.lbePocketIndex;
}
LBETYPE& LBETYPE::operator=(const LBETYPE& src){
	PERFORMANCE_MARKER
    if (this != &src) {
		memcpy(this, &src, SIZEOF_LBETYPE);
		lbePocketIndex = src.lbePocketIndex;
    }
    return *this;
}
LBETYPE::~LBETYPE(){
}
POCKETTYPE::POCKETTYPE(){
	PERFORMANCE_MARKER
	memset(this, 0, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(35);
}
POCKETTYPE::POCKETTYPE(const POCKETTYPE& src){
	PERFORMANCE_MARKER
	memcpy(this, &src, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(35);
	ItemCapacityPerSize = src.ItemCapacityPerSize;
}
POCKETTYPE& POCKETTYPE::operator=(const POCKETTYPE& src){
	PERFORMANCE_MARKER
    if (this != &src) {
		memcpy(this, &src, SIZEOF_POCKETTYPE);
		ItemCapacityPerSize = src.ItemCapacityPerSize;
    }
    return *this;
}
POCKETTYPE::~POCKETTYPE(){
}

bool OBJECTTYPE::IsActiveLBE(unsigned int index)
{
	PERFORMANCE_MARKER
	if (exists() == true) {
		return ((*this)[index]->data.lbe.bLBE == -1);
	}
	return false;
}

bool OBJECTTYPE::HasAnyActiveLBEs()
{
	PERFORMANCE_MARKER
	if (exists() == true) {
		for (int x = 0; x < ubNumberOfObjects; ++x) {
			if ((*this)[x]->data.lbe.bLBE == -1) {
				return true;
			}
		}
	}
	return false;
}

LBENODE* OBJECTTYPE::GetLBEPointer(unsigned int index)
{
	PERFORMANCE_MARKER
	if (LBEArray.empty() == false) {
		int uniqueID = (*this)[index]->data.lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				return &(*iter);
			}
		}
	}
	return NULL;
}

bool OBJECTTYPE::exists()
{
	PERFORMANCE_MARKER
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
	//ADB TODO make the stacking restrictions match!!!!

	//This function is never called from a soldier, so get the max size
	int numToAdd = max(0, ItemSlotLimit( this, STACK_SIZE_LIMIT ) - ubNumberOfObjects);

	//if howMany is ALL_OBJECTS the stack will become full
	if (howMany != ALL_OBJECTS) {
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

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& sourceObject, int howMany, SOLDIERTYPE* pSoldier, int slot, bool allowLBETransfer)
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
	int freeObjectsInStack = max(0, (ItemSlotLimit( this, slot, pSoldier ) - ubNumberOfObjects));
	int numToAdd = min (freeObjectsInStack, sourceObject.ubNumberOfObjects);
	//if howMany is ALL_OBJECTS the stack will become full if sourceObject has enough
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}


	//stacking control, restrict certain things here
	if (numToAdd > 0) {
		if (exists() == true) {
			//Triggers, and other specials can never stack
			if (Item[usItem].usItemClass == IC_BOMB) {
				//exit and do not continue
				return 0;
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

			//keys can stack if the same key
			if (Item[usItem].usItemClass == IC_KEY) {
				if ((*this)[0]->data.key.ubKeyID != sourceObject[0]->data.key.ubKeyID) {
				return 0;
				}
			}

			//for convenience sake, do not allow mixed flags to stack!
			//continue on because you might find something else with the same flags
			if (sourceObject.fFlags != this->fFlags) {
				numToAdd = 0;
			}

			if (sourceObject[0]->data.ubImprintID != (*this)[0]->data.ubImprintID) {
				numToAdd = 0;
			}

			//nor allow trapped items to stack
			if (sourceObject[0]->data.bTrap > 0
				|| (*this)[0]->data.bTrap > 0) {
				return 0;
			}
			//ADB TODO, other specials
		}
	}

	//this recalcs weight too!
	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());
	
	//ADB TODO
	if (numToAdd && UsingNewInventorySystem() == true && pSoldier != NULL) {
		//CHRISL: For New Inventory system.  Are we handling an LBE item FROM a soldier?
		if(IsSlotAnLBESlot(slot) == true && allowLBETransfer == true)
		{
			//we know this must be an LBE because of the slot and ubNumberOfObjects
			//since we are putting this LBE into an LBE slot in a soldier
			//we need to move the items inside the LBE into the soldier's inventory
			MoveItemFromLBEItem( pSoldier, slot, this );
		}
	}

	//returns how many were NOT added
	if (howMany != ALL_OBJECTS) {
		return howMany - numToAdd;
	}
	else {
		//ALL_OBJECTS means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

void OBJECTTYPE::DuplicateObjectsInStack(OBJECTTYPE& sourceObject, int howMany)
{
	PERFORMANCE_MARKER
	int numToDupe;
	if (howMany != ALL_OBJECTS) {
		numToDupe = min(howMany, sourceObject.ubNumberOfObjects);
	}
	else {
		numToDupe = sourceObject.ubNumberOfObjects;
	}

	if (numToDupe) {
		objectStack.clear();
		this->usItem = sourceObject.usItem;
		this->fFlags = sourceObject.fFlags;
		StackedObjects::iterator iter = sourceObject.objectStack.begin();
		for (int x = 0; x < numToDupe; ++x) {
			objectStack.push_back(*iter);
			++iter;
		}
		ubNumberOfObjects = numToDupe;
		ubWeight = CalculateObjectWeight(this);
	}
	else {
		initialize();
	}
	return;
}

int OBJECTTYPE::MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove, SOLDIERTYPE* pSoldier, int slot)
{
	//ADB yes I said I normally remove functions like this, but this is different
	//this exists to make reading easier and to be more descriptive.
	return (RemoveObjectsFromStack(numToMove, &destObject, pSoldier, slot));
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject, SOLDIERTYPE* pSoldier, int slot)
{
	PERFORMANCE_MARKER
	//ADB this function only needs to know soldier and slot
	//if there is a dest object we are putting the removed objects into
	//in this case it is acting as a move and has probably been called by MoveThisObjectTo
	//otherwise it is acting as a delete and has probably been called by name
	if (howMany == ALL_OBJECTS) {
		howMany = ubNumberOfObjects;
	}

	int numToRemove = min(ubNumberOfObjects, howMany);

	bool allowLBETransfer = false;
	if (numToRemove && UsingNewInventorySystem() == true && pSoldier != NULL) {
		//CHRISL: For New Inventory system.  Are we handling an LBE item FROM a soldier?
		if(IsSlotAnLBESlot(slot) == true) {
			//we know this must be an LBE because of the slot and ubNumberOfObjects
			//but are we taking the LBE out of the pocket or putting it in?

			if (pSoldier->inv[slot].exists() == true) {
				//the object exists and it is an LBE, so we must be taking it out of the pocket
				//since we are moving this LBE, it needs to have the items in its pockets moved
				//from the soldier's inv to the LBE's inv
				MoveItemToLBEItem( pSoldier, slot );
			}
			else {
				//we must be putting it into the pocket, so allow the LBE to
				//transfer its contents into the soldier
				//but first make room by putting anything in the soldier's default LBE pockets into other pockets
				std::vector<INT8> LBESlots;
				GetLBESlots(slot, LBESlots);
				MoveItemsToActivePockets(pSoldier, LBESlots, slot, this);
				allowLBETransfer = true;
			}
		}
	}

	if (destObject) {
		//destObject should be empty especially if numToRemove is 0
		if (destObject->exists() == true) {
			destObject->initialize();
		}
		if (numToRemove > 0) {
			//this handles the removal too
			return destObject->AddObjectsToStack(*this, numToRemove, pSoldier, slot, allowLBETransfer);
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
	//Note, this function does NOT care what the stack size of destObject will be
	//because destObject will never be an object in a soldier's inventory!
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
		//handles weight calc and ubNumber too
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

ObjectData::ObjectData(const ObjectData& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		//first get rid of any LBE this might have
		DeleteLBE();

		//copy over the data
		this->bTrap = src.bTrap;
		this->fUsed = src.fUsed;
		this->ubImprintID = src.ubImprintID;
		//copy over the union
		this->gun = src.gun;

		//duplicate the LBE data
		DuplicateLBE();
	}
}

ObjectData& ObjectData::operator =(const ObjectData& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		//first get rid of any LBE this might have
		DeleteLBE();

		//copy over the data
		this->bTrap = src.bTrap;
		this->fUsed = src.fUsed;
		this->ubImprintID = src.ubImprintID;
		//copy over the union
		this->gun = src.gun;

		//duplicate the LBE data
		DuplicateLBE();
	}
	return *this;
}

ObjectData::~ObjectData()
{
	DeleteLBE();
}

void ObjectData::DeleteLBE()
{
	if (LBEArray.empty() == false) {
		if (this->lbe.bLBE == -1) {
			int uniqueID = this->lbe.uniqueID;
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
		}
	}
}

void ObjectData::DuplicateLBE()
{
	if (this->lbe.bLBE == -1) {
		LBENODE* pLBE = NULL;
		int uniqueID = this->lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				pLBE = &(*iter);
				break;
			}
		}
		uniqueID = gLastLBEUniqueID++;
		LBEArray.push_back(*pLBE);
		pLBE = &LBEArray.back();
		pLBE->uniqueID = uniqueID;
		this->lbe.uniqueID = uniqueID;
	}
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

	memset(this, 0, SIZEOF_OBJECTTYPE_POD);

	//this is an easy way to init it and get rid of attachments
	objectStack.clear();
	//ubNumberOfObjects should be 0 so any loop checking it will not work
	//however objectStack should always have at least one, because there are so
	//many uses of (*pObj)[0]->data.objectStatus and such
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
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
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
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
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
}
