
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"
OBJECTTYPE gTempObject;

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, howMany);
	for (int x = ubNumberOfObjects; x < ubNumberOfObjects + numToAdd; ++x) {
	}

	return howMany - numToAdd;
}

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& object)
{
	Assert(object.usItem == usItem);
	int numToAdd = max (ItemSlotLimit( usItem, BIGPOCK1POS ) - ubNumberOfObjects, object.ubNumberOfObjects);

	for (int x = 0; x < numToAdd; ++x) {
		
	}
	return ItemSlotLimit( usItem, BIGPOCK1POS ) - numToAdd;
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany)
{
	int numToRemove = min(howMany, ubNumberOfObjects);
	attachments.resize(attachments.size() - numToRemove);
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

OBJECTTYPE* OBJECTTYPE::GetAttachmentAtIndex(UINT8 index)
{
	attachmentList::iterator iter = objectStack[0].attachments.begin();
	for (int x = 0; x <= index && iter != objectStack[0].attachments.end(); ++x) {
		++iter;
	}

	if (iter != objectStack[0].attachments.end()) {
		return &(*iter);
	}
	return 0;
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
	memset(&(this->pUnion), 0, SIZEOF_OBJECTTYPE_UNION);
	attachments.clear();
}

bool OBJECTTYPE::operator==(const OBJECTTYPE& compare)
{
	return (*this) == const_cast<OBJECTTYPE&>(compare);
}

bool OBJECTTYPE::operator==(OBJECTTYPE& compare)
{
	PERFORMANCE_MARKER
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if ( memcmp(&(this->pUnion), &(compare.pUnion), SIZEOF_OBJECTTYPE_UNION) == 0) {
			if (attachments.size() == compare.attachments.size()) {
				attachmentList::iterator iter = objectStack[0].attachments.begin();
				attachmentList::iterator compareIter = compare.objectStack[0].attachments.begin();
				while (iter != objectStack[0].attachments.end() && *iter == *compareIter) {
					++iter;
					++compareIter;
				}
				if (iter == objectStack[0].attachments.end()) {
					return true;
				}
			}
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
		this->ubMission = src.ubMission;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not
		memcpy(&this->pUnion, &src.pUnion, SIZEOF_OBJECTTYPE_UNION);
		this->objectStack[0].attachments = src.attachments;
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
		this->ubMission = src.ubMission;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not
		memcpy(&this->pUnion, &src.pUnion, SIZEOF_OBJECTTYPE_UNION);
		this->objectStack[0].attachments = src.attachments;
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
		this->ubMission = src.ubMission;
		this->bTrap = src.bTrap;		// 1-10 exp_lvl to detect
		this->ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
		this->ubWeight = src.ubWeight;
		this->fUsed = src.fUsed;				// flags for whether the item is used or not
		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		memcpy(&(this->pUnion), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,SIZEOF_OBJECTTYPE_UNION));

		//it's unlikely max will get less over the versions, but still, check the min
		for (int x = 0; x < OLD_MAX_ATTACHMENTS_101; ++x)
		{
			if (src.usAttachItem[x] != NOTHING) {
				CreateItem(src.usAttachItem[x], src.bAttachStatus[x], &gTempObject);
				attachments.push_back(gTempObject);
			}
		}

	}
	return *this;
}


OBJECTTYPE::~OBJECTTYPE()
{
	attachments.clear();
}
