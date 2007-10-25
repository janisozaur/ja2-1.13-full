
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"
#include "GameSettings.h"


OBJECTTYPE gTempObject;
OBJECTTYPE gStackTempObject;

bool IsSlotASmallPocket(unsigned int slot)
{
	if (slot >= SMALLPOCKSTART && slot < SMALLPOCKFINAL) {
		return true;
	}
	return false;
}

bool OBJECTTYPE::exists()
{
	return (ubNumberOfObjects > 0 && usItem != NOTHING);
}

void OBJECTTYPE::SpliceData(OBJECTTYPE& sourceObject, unsigned int numToSplice, StackedObjects::iterator beginIter)
{
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
	objectStack.splice(objectStack.end(), sourceObject.objectStack, beginIter, stopIter);

	ubNumberOfObjects += numToSplice;
	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//ubWeight = CalculateObjectWeight(this);

	if (sourceObject.objectStack.empty() == true) {
		//init it so it is not empty, even though it no longer exists
		sourceObject.initialize();
	}
	else {
		sourceObject.ubNumberOfObjects -= numToSplice;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//sourceObject.ubWeight = CalculateObjectWeight(&sourceObject);
	}
}

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	//This function is never called from a soldier, so get the max size
	int numToAdd = max(0, ItemSlotLimit( this, STACK_SIZE_LIMIT ) - ubNumberOfObjects);

	//if howMany is ALL_OBJECTS the stack will become full
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}

	if (numToAdd) {
		CreateItems(usItem, objectStatus, numToAdd, &gStackTempObject);
		this->AddObjectsToStack(gStackTempObject);
	}

	//returns how many were NOT added
	return howMany - numToAdd;
}

bool OBJECTTYPE::CanStack(OBJECTTYPE& sourceObject, int& numToStack)
{
	//stacking control, restrict certain things here
	if (numToStack > 0) {
		if (exists() == true) {
			if (Item[usItem].usItemClass == IC_MONEY) {
				//money doesn't stack, it merges
				// average out the status values using a weighted average...
				int thisStatus = (*this)[0]->data.money.bMoneyStatus * (*this)[0]->data.money.uiMoneyAmount;
				int sourceStatus = sourceObject[0]->data.money.bMoneyStatus * sourceObject[0]->data.money.uiMoneyAmount;
				int average = (*this)[0]->data.money.uiMoneyAmount + sourceObject[0]->data.money.uiMoneyAmount;

				(*this)[0]->data.objectStatus = ( (thisStatus + sourceStatus) / average);
				(*this)[0]->data.money.uiMoneyAmount += sourceObject[0]->data.money.uiMoneyAmount;
				sourceObject.initialize();
				return false;
			}

			//keys can stack if the same key
			if (Item[usItem].usItemClass == IC_KEY) {
				if ((*this)[0]->data.key.ubKeyID != sourceObject[0]->data.key.ubKeyID) {
					return false;
				}
			}

			//disallow stacking for objects with dissimilar UNDROPPABLE flags
			//objects can stack if they are both droppable or both undroppable, but not one droppable and the other not
			//they can also stack regardless of the other flags that are set, like OBJECT_NO_OVERWRITE
			//in the old code objects with OBJECT_NO_OVERWRITE not only are dropped, but also can stack with others without the same flag
			//I don't care to find out if this is a bug or not, I'm just going to mimic the original code.
			if ((sourceObject.fFlags & OBJECT_UNDROPPABLE) != (this->fFlags & OBJECT_UNDROPPABLE)) {
				DebugBreakpoint();
				numToStack = 0;
			}

			//nor allow trapped items to stack
			if (sourceObject[0]->data.bTrap > 0
				|| (*this)[0]->data.bTrap > 0) {
				return false;
			}
			//TODO other specials if there are any
		}
	}
	return true;
}

int OBJECTTYPE::ForceAddObjectsToStack(OBJECTTYPE& sourceObject, int howMany)
{
	if (exists() == false) {
		//we are adding to an empty object, it can happen
		Assert(sourceObject.exists() == true);
		usItem = sourceObject.usItem;
		this->fFlags = sourceObject.fFlags;
	}
	Assert(sourceObject.usItem == usItem);

	int numToAdd = sourceObject.ubNumberOfObjects;
	//if howMany is ALL_OBJECTS the stack will become full if sourceObject has enough
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}

	//this recalcs weight too!
	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());

	//returns how many were NOT added
	if (howMany != ALL_OBJECTS) {
		return howMany - numToAdd;
	}
	else {
		//ALL_OBJECTS means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& sourceObject, int howMany, SOLDIERTYPE* pSoldier, int slot)
{
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

	if (this->CanStack(sourceObject, numToAdd) == false) {
		return 0;
	}

	//this recalcs weight too!
	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());

	//returns how many were NOT added
	if (howMany != ALL_OBJECTS) {
		return howMany - numToAdd;
	}
	else {
		//ALL_OBJECTS means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

int OBJECTTYPE::MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove, SOLDIERTYPE* pSoldier, int slot)
{
	//ADB yes I said I normally remove functions like this, but this is different
	//this exists to make reading easier and to be more descriptive.
	return (PrivateRemoveObjectsFromStack(numToMove, &destObject, pSoldier, slot));
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany)
{
	return (PrivateRemoveObjectsFromStack(howMany, NULL, NULL, STACK_SIZE_LIMIT));
}

int OBJECTTYPE::PrivateRemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject, SOLDIERTYPE* pSoldier, int slot)
{
	//ADB this function only needs to know soldier and slot
	//if there is a dest object we are putting the removed objects into
	//in this case it is acting as a move and has probably been called by MoveThisObjectTo
	//otherwise it is acting as a delete and has probably been called by RemoveObjectsFromStack
	if (howMany == ALL_OBJECTS) {
		howMany = ubNumberOfObjects;
	}

	int numToRemove = min(ubNumberOfObjects, howMany);

	if (destObject) {
		//destObject should be empty especially if numToRemove is 0
		if (destObject->exists() == true) {
			destObject->initialize();
		}
		if (numToRemove > 0) {
			//this handles the removal too
			return destObject->AddObjectsToStack(*this, numToRemove, pSoldier, slot);
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
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//ubWeight = CalculateObjectWeight(this);
		}
	}

	//returns how many were NOT removed
	return howMany - numToRemove;
}

bool OBJECTTYPE::RemoveObjectAtIndex(unsigned int index, OBJECTTYPE* destObject)
{
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
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//ubWeight = CalculateObjectWeight(this);
		}
	}

	return true;
}

StackedObjectData* OBJECTTYPE::operator [](const unsigned int index)
{
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
	//this is necessary because memcpy is no longer good enough for OBJECTTYPE
	if (*ppTarget == NULL) {
		*ppTarget = new OBJECTTYPE(*pSource);
	}
	else {
		//ADB leaving this in for a while, not sure if the code ever even reaches here and this will tell me
		DebugBreakpoint();
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Found mem leak, but it was corrected."));
		**ppTarget = *pSource;
	}
	return;
}

//OR write OBJECTTYPE::DeleteMe(&pSoldier->pTempObject) if there is no memcpy
void OBJECTTYPE::DeleteMe(OBJECTTYPE** ppObject)
{
	if (*ppObject != NULL) {
		delete *ppObject;
		*ppObject = NULL;
	}
	return;
}

StackedObjectData::StackedObjectData()
{
	initialize();
}

StackedObjectData::~StackedObjectData()
{
	attachments.clear();
}

OBJECTTYPE* StackedObjectData::GetAttachmentAtIndex(UINT8 index)
{
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

OBJECTTYPE::~OBJECTTYPE()
{
	objectStack.clear();
}

// Constructor
OBJECTTYPE::OBJECTTYPE()
{
	initialize();
}
void OBJECTTYPE::initialize()
{

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
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if (this->objectStack == compare.objectStack) {
			return true;
		}
	}
	return false;
}

bool OBJECTTYPE::operator==(OBJECTTYPE& compare)
{
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
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
	}
}

// Assignment operator
OBJECTTYPE& OBJECTTYPE::operator=(const OBJECTTYPE& src)
{
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
	}
	return *this;
}


// Conversion operator
OBJECTTYPE& OBJECTTYPE::operator=(const OLD_OBJECTTYPE_101& src)
{
	if ((void*)this != (void*)&src) {
		//makes changes to size easier as we don't have to init new arrays with 0 by hand
		this->initialize();

		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;

		this->ubMission = src.ubMission;

		//in some cases we need to reference the objectStatus or something, even though the item is totally empty
		//therefore, keep ubNumberOfObjects at 0 but resize objectStack to at least 1
		this->objectStack.resize(max(ubNumberOfObjects, 1));
		if (ubNumberOfObjects == 0) {
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
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = CalculateObjectWeight(this);
	}
	return *this;
}
