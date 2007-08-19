
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"

OBJECTTYPE gTempObject;

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	PERFORMANCE_MARKER
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, howMany);
	if (numToAdd) {
		CreateItem(usItem, objectStatus, &gTempObject);
		for (int x = ubNumberOfObjects; x < ubNumberOfObjects + numToAdd; ++x) {
			objectStack.push_front(gTempObject.objectStack.front());
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
	Assert(sourceObject.usItem == usItem);
	//can't add too much, can't take too many
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, sourceObject.ubNumberOfObjects);
	if (howMany > 0) {
		numToAdd = max(numToAdd, howMany);
	}

	StackedObjects::iterator stopIter = sourceObject.objectStack.begin();
	for (int x = 0; x < numToAdd; ++x) {
		++stopIter;
	}
	objectStack.splice(objectStack.begin(), sourceObject.objectStack, sourceObject.objectStack.begin(), stopIter);

	sourceObject.ubNumberOfObjects -= numToAdd;
	ubNumberOfObjects += numToAdd;
	sourceObject.ubWeight = CalculateObjectWeight(&sourceObject);
	ubWeight = CalculateObjectWeight(this);

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
	int numToDupe;
	if (howMany == -1) {
		numToDupe = sourceObject.ubNumberOfObjects;
	}
	else {
		numToDupe = min(howMany, sourceObject.ubNumberOfObjects);
	}

	initialize();
	for (int x = 0; x < numToDupe; ++x) {
		objectStack.push_back(sourceObject.objectStack.front());		
	}
	ubNumberOfObjects = numToDupe;
	ubWeight = CalculateObjectWeight(this);
	return;
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject)
{
	PERFORMANCE_MARKER
	if (ubNumberOfObjects == 1) {
		return howMany;
	}

	int numToRemove = min(ubNumberOfObjects - 1, howMany);
	if (destObject) {
		StackedObjects::iterator stopIter = objectStack.begin();
		for (int x = 0; x < numToRemove; ++x) {
			++stopIter;
		}
		destObject->objectStack.splice(destObject->objectStack.begin(), objectStack, objectStack.begin(), stopIter);
		destObject->ubNumberOfObjects += numToRemove;
		destObject->ubWeight = CalculateObjectWeight(destObject);
	}
	else {
		for (int x = 0; x < numToRemove; ++x) {
			objectStack.pop_front();
		}
	}
	ubNumberOfObjects -= numToRemove;
	ubWeight = CalculateObjectWeight(this);

	//returns how many were NOT removed
	return howMany - numToRemove;
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
	for (int x = 0; x <= index && iter != attachments.end(); ++x) {
		++iter;
	}

	if (iter != attachments.end()) {
		return &(*iter);
	}
	Assert(false);
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
	memset(this, 0, SIZEOF_OBJECTTYPE_POD);
	//ubNumberOfObjects = 1;
	objectStack.clear();
	//objectStack.resize(1);
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


		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		if (ubNumberOfObjects == 1) {
			memcpy(&((*this)[0]->data), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,sizeof(ObjectData)));

			(*this)[0]->data.fFlags = src.fFlags;
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
			objectStack.resize(ubNumberOfObjects);
			for (int x = 0; x < ubNumberOfObjects; ++x) {
				(*this)[x]->data.objectStatus = ugYucky.bStatus[x];
				(*this)[x]->data.fFlags = src.fFlags;
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
