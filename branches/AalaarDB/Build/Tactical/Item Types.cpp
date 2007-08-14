
#include "Item Types.h"
#include "Debug.h"
OBJECTTYPE gTempObject;

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

// Constructor
OBJECTTYPE::OBJECTTYPE()
{
	PERFORMANCE_MARKER
	initialize();
}
void OBJECTTYPE::initialize()
{
	PERFORMANCE_MARKER
	//change this if OBJECTTYPE ever gets any OO members
	memset(this, 0, sizeof(OBJECTTYPE));
}

bool OBJECTTYPE::operator==(OBJECTTYPE& compare)
{
	PERFORMANCE_MARKER
	//change this if OBJECTTYPE ever gets any OO members
	return ( memcmp(this, &compare, sizeof(OBJECTTYPE)) == 0);
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
		for (int x = 0; x < MAX_ATTACHMENTS; ++x)
		{
			this->usAttachItem[x] = src.usAttachItem[x];
			this->bAttachStatus[x] = src.bAttachStatus[x];
		}
		memcpy(&this->gun, &src.gun, SIZEOF_OBJECTTYPE_UNION);
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
		for (int x = 0; x < MAX_ATTACHMENTS; ++x)
		{
			this->usAttachItem[x] = src.usAttachItem[x];
			this->bAttachStatus[x] = src.bAttachStatus[x];
		}
		memcpy(&this->gun, &src.gun, SIZEOF_OBJECTTYPE_UNION);
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

		//it's unlikely max will get less over the versions, but still, check the min
		for (int x = 0; x < __min(MAX_ATTACHMENTS, OLD_MAX_ATTACHMENTS_101); ++x)
		{
			this->usAttachItem[x] = src.usAttachItem[x];
			this->bAttachStatus[x] = src.bAttachStatus[x];
		}

		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		memcpy(&(this->gun), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,SIZEOF_OBJECTTYPE_UNION));
	}
	return *this;
}


OBJECTTYPE::~OBJECTTYPE() {}