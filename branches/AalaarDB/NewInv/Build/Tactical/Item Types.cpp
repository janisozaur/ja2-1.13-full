
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"
OBJECTTYPE gTempObject;

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, howMany);
	for (int x = ubNumberOfObjects; x < ubNumberOfObjects + numToAdd; ++x) {
	}

	ubWeight = CalculateObjectWeight(this);
	return howMany - numToAdd;
}

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& object, int howMany)
{
	Assert(object.usItem == usItem);
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, object.ubNumberOfObjects);
	if (howMany > 0) {
		numToAdd = max(numToAdd, howMany);
	}

	for (int x = 0; x < numToAdd; ++x) {
		
	}
	object.ubWeight = CalculateObjectWeight(&object);
	ubWeight = CalculateObjectWeight(this);
	return ItemSlotLimit( usItem, BIGPOCK1POS ) - numToAdd;
}

bool OBJECTTYPE::RemoveTopObjectFromStack(OBJECTTYPE* pSecondObject)
{
	if (ubNumberOfObjects == 1) {
		return false;
	}

	if (pSecondObject) {
		CreateItem(usItem, (*this)[0]->data.objectStatus, pSecondObject);
		pSecondObject->objectStack.front() = objectStack.front();
		pSecondObject->ubWeight = CalculateObjectWeight(pSecondObject);
	}
	objectStack.pop_front();
	ubWeight = CalculateObjectWeight(this);

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
	ubNumberOfObjects = 1;
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
		this->fFlags = src.fFlags;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not
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
		this->fFlags = src.fFlags;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not
		this->objectStack = src.objectStack;
	}
	return *this;
}


// Conversion operator
OBJECTTYPE& OBJECTTYPE::operator=(const OLD_OBJECTTYPE_101& src)
{
	PERFORMANCE_MARKER
	if ((void*)this != (void*)&src) {
		//makes changes to size easier as we don't have to fill new arrays by hand
		this->initialize();

		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->fFlags = src.fFlags;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not




		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		if (ubNumberOfObjects == 1) {
			memcpy(&((*this)[0]->data), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,SIZEOF_OBJECTTYPE_UNION));
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

			Version101::OLD_OBJECTTYPE_101_UNION ugYucky;
			memcpy(&ugYucky, &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,SIZEOF_OBJECTTYPE_UNION));
			objectStack.resize(ubNumberOfObjects);
			for (int x = 0; x < ubNumberOfObjects; ++x) {
				(*this)[x]->data.objectStatus = ugYucky.bStatus[x];
			}
		}
	}
	return *this;
}


OBJECTTYPE::~OBJECTTYPE()
{
	objectStack.clear();
}
