#ifndef ITEM_TYPES_H
#define ITEM_TYPES_H

#include "types.h"
#include <vector>
#include <list>

//if the number of slots are ever changed, the loading / saving checksum should use this value to make conversion easier
#define NUM_ORIGINAL_INV_SLOTS 19


// I don't care if this isn't intuitive!	The hand positions go right
// before the big pockets so we can loop through them that way. --CJC
#define NO_SLOT -1

// NOTE NOTE NOTE!	Leave this alone until it is no longer needed.	It must match the
// original definition so old files can be read.
namespace OldInventory {
	enum {
		HELMETPOS = 0,
		VESTPOS,
		LEGPOS,
		HEAD1POS,
		HEAD2POS,
		HANDPOS,
		SECONDHANDPOS,
		BIGPOCK1POS,
		BIGPOCK2POS,
		BIGPOCK3POS,
		BIGPOCK4POS,
		SMALLPOCK1POS,
		SMALLPOCK2POS,
		SMALLPOCK3POS,
		SMALLPOCK4POS,
		SMALLPOCK5POS,
		SMALLPOCK6POS,
		SMALLPOCK7POS,
		SMALLPOCK8POS, // = 18, so 19 pockets needed

		NUM_INV_SLOTS = NUM_ORIGINAL_INV_SLOTS
	};
};

/* CHRISL: Added listings for each of the new inventory pockets.  Also split the enum so we could include
endpoint markers for each type (big, med, sml) of pocket. */
typedef enum INVENTORY_SLOT{
	HELMETPOS = 0,
	VESTPOS,
	LEGPOS,
	HEAD1POS,
	HEAD2POS,
	HANDPOS,
	SECONDHANDPOS,
	VESTPOCKPOS,
	LTHIGHPOCKPOS,
	RTHIGHPOCKPOS,
	CPACKPOCKPOS,
	BPACKPOCKPOS,
	GUNSLINGPOCKPOS,
	KNIFEPOCKPOS,
	BIGPOCK1POS,
	BIGPOCK2POS,
	BIGPOCK3POS,
	BIGPOCK4POS,
	BIGPOCK5POS,
	BIGPOCK6POS,
	BIGPOCK7POS,
	MEDPOCK1POS,
	MEDPOCK2POS,
	MEDPOCK3POS,
	MEDPOCK4POS,
	SMALLPOCK1POS,
	SMALLPOCK2POS,
	SMALLPOCK3POS,
	SMALLPOCK4POS,
	SMALLPOCK5POS,
	SMALLPOCK6POS,
	SMALLPOCK7POS,
	SMALLPOCK8POS,
	SMALLPOCK9POS,
	SMALLPOCK10POS,
	SMALLPOCK11POS,
	SMALLPOCK12POS,
	SMALLPOCK13POS,
	SMALLPOCK14POS,
	SMALLPOCK15POS,
	SMALLPOCK16POS,
	SMALLPOCK17POS,
	SMALLPOCK18POS,
	SMALLPOCK19POS,
	SMALLPOCK20POS,
	SMALLPOCK21POS,
	SMALLPOCK22POS,
	SMALLPOCK23POS,
	SMALLPOCK24POS,
	SMALLPOCK25POS,
	SMALLPOCK26POS,
	SMALLPOCK27POS,
	SMALLPOCK28POS,
	SMALLPOCK29POS,
	SMALLPOCK30POS,
	NUM_INV_SLOTS,
};

#define		INV_START_POS		0
#define		BODYPOSSTART		HELMETPOS
extern	int	BODYPOSFINAL		;//= GUNSLINGPOCKPOS;//RESET in initInventory
#define		BIGPOCKSTART		BIGPOCK1POS
extern	int	BIGPOCKFINAL		;//= MEDPOCK1POS;//RESET in initInventory
extern	int	MEDPOCKSTART		;//= MEDPOCK1POS;//RESET in initInventory
extern	int	MEDPOCKFINAL		;//= SMALLPOCK1POS;//RESET in initInventory
#define		SMALLPOCKSTART		SMALLPOCK1POS
extern	int	SMALLPOCKFINAL		;//= NUM_INV_SLOTS;//RESET in initInventory
#define		STACK_SIZE_LIMIT	NUM_INV_SLOTS

#define INVALIDCURS 0
#define QUESTCURS 1
#define PUNCHCURS 2
#define TARGETCURS 3
#define KNIFECURS 4
#define AIDCURS 5
#define TOSSCURS 6
#define MINECURS 8
#define LPICKCURS 9
#define MDETECTCURS 10
#define CROWBARCURS 11
#define SURVCAMCURS 12
#define CAMERACURS 13
#define KEYCURS 14
#define SAWCURS 15
#define WIRECUTCURS	16
#define	REMOTECURS	17
#define BOMBCURS		18 // ( only calculated, not set item table )
#define	REPAIRCURS	19
#define TRAJECTORYCURS	20
#define	JARCURS			21
#define	TINCANCURS	22
#define REFUELCURS  23

#define CAMERARANGE 10

#define CONDBUL 0
#define COND 0
#define SINGLE 0
#define LIQ 0
#define USAGE 0
#define BUCKS 0

#define ITEM_NOT_FOUND -1


#define USABLE          10      // minimum work% of items to still be usable

#define MAX_OBJECTS_PER_SLOT 255
#define MAX_ATTACHMENTS 4
#define MAX_MONEY_PER_SLOT 20000

typedef enum
{
	BOMB_TIMED = 1,
	BOMB_REMOTE,
	BOMB_PRESSURE,
	BOMB_SWITCH
} DetonatorType;

#define FIRST_MAP_PLACED_FREQUENCY 50
#define PANIC_FREQUENCY 127
#define PANIC_FREQUENCY_2 126
#define PANIC_FREQUENCY_3 125

#define OBJECT_UNDROPPABLE					0x01
#define OBJECT_MODIFIED						0x02
#define OBJECT_AI_UNUSABLE					0x04
#define OBJECT_ARMED_BOMB					0x08
#define OBJECT_KNOWN_TO_BE_TRAPPED			0x10
#define OBJECT_DISABLED_BOMB				0x20
#define OBJECT_ALARM_TRIGGER				0x40
#define OBJECT_NO_OVERWRITE					0x80

#define GS_CARTRIDGE_IN_CHAMBER				0x01
#define GS_WEAPON_BEING_RELOADED			0x02

//forward declaration
class OBJECTTYPE;
class SOLDIERTYPE;

#define MAX_ITEMS_IN_LBE 12

//CHRISL:
class LBENODE
{
public:
	LBENODE() { initialize();};
	void	initialize() {inv.clear();};
	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	UINT32				lbeClass;
	UINT16				lbeIndex;
	UINT8				ubID;
	BOOLEAN				ZipperFlag;
	int					uniqueID;
	UINT32				uiNodeChecksum;
	char				endOfPOD;
	//compiler complains about too big an array since OBJECTTYPE's size is unknown at this time, because of forward declaration
	//OBJECTTYPE			inv[ITEMS_IN_LBE];
	std::vector<OBJECTTYPE> inv;
};
#define SIZEOF_LBENODE_POD (offsetof(LBENODE, endOfPOD))
void	CreateLBE(OBJECTTYPE* pObj, UINT8 ubID, int numSubPockets);
bool	DestroyLBEIfEmpty(OBJECTTYPE* pObj);
void	DestroyLBE(OBJECTTYPE* pObj);
void	GetLBESlots(UINT32 LBEType, std::vector<INT8>& LBESlots);
void	MoveItemsInSlotsToLBE( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, LBENODE* pLBE, OBJECTTYPE* pObj);

// CHRISL:
BOOLEAN MoveItemsToActivePockets( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, UINT32 uiHandPos, OBJECTTYPE *pObj );
BOOLEAN	MoveItemToLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos );
BOOLEAN MoveItemFromLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj );
bool	IsSlotAnLBESlot(int slot);
bool	IsSlotASmallPocket(int slot);

extern	std::list<LBENODE>	LBEArray;

//do not alter or saves will break, create new defines if the size changes
#define OLD_MAX_ATTACHMENTS_101 4
#define OLD_MAX_OBJECTS_PER_SLOT_101 8

namespace Version101
{
	//union was originally unnamed
	union OLD_OBJECTTYPE_101_UNION
	{
		struct
		{
			INT8		bGunStatus;			// status % of gun
			UINT8		ubGunAmmoType;	// ammo type, as per weapons.h
			UINT8		ubGunShotsLeft;	// duh, amount of ammo left
			UINT16		usGunAmmoItem;	// the item # for the item table
			INT8		bGunAmmoStatus; // only for "attached ammo" - grenades, mortar shells
			UINT8		ubGunState; // SB manual recharge
	//warning, this unused space is the wrong size, 7 bytes above, 2 in the array, but it's been saved like that
			UINT8		ubGunUnused[OLD_MAX_OBJECTS_PER_SLOT_101 - 6];
		};
		struct
		{
			UINT8		ubShotsLeft[OLD_MAX_OBJECTS_PER_SLOT_101];
		};
		struct
		{
			INT8		bStatus[OLD_MAX_OBJECTS_PER_SLOT_101];
		};		
		struct
		{
			INT8		bMoneyStatus;
			UINT32		uiMoneyAmount;
			UINT8		ubMoneyUnused[OLD_MAX_OBJECTS_PER_SLOT_101 - 5];
		};
		struct
		{ // this is used by placed bombs, switches, and the action item
			INT8		bBombStatus;			// % status
			INT8		bDetonatorType;		// timed, remote, or pressure-activated
			UINT16		usBombItem;				// the usItem of the bomb.
			union
			{
				struct
				{
					INT8		bDelay;				// >=0 values used only
				};
				struct
				{
					INT8		bFrequency;		// >=0 values used only
				};
			};
			UINT8	ubBombOwner; // side which placed the bomb
			UINT8	bActionValue;// this is used by the ACTION_ITEM fake item
			union
			{
				struct
				{
					UINT8 ubTolerance; // tolerance value for panic triggers
				};
				struct 
				{
					UINT8 ubLocationID; // location value for remote non-bomb (special!) triggers
				};
			};		
		};
		struct
		{
			INT8 bKeyStatus[ 6 ];
			UINT8 ubKeyID;
			UINT8 ubKeyUnused[1];
		};
		struct
		{
			UINT8 ubOwnerProfile;
			UINT8 ubOwnerCivGroup;
			UINT8 ubOwnershipUnused[6];
		};
	};
};
#define SIZEOF_OLD_OBJECTTYPE_101_UNION (sizeof(Version101::OLD_OBJECTTYPE_101_UNION))

class OLD_OBJECTTYPE_101
{
public:
	UINT16		usItem;
	UINT8		ubNumberOfObjects;

	Version101::OLD_OBJECTTYPE_101_UNION	ugYucky;

  // attached objects
	UINT16		usAttachItem[OLD_MAX_ATTACHMENTS_101];
	INT8		bAttachStatus[OLD_MAX_ATTACHMENTS_101];

	INT8		fFlags;
	UINT8		ubMission;
	INT8		bTrap;        // 1-10 exp_lvl to detect
	UINT8		ubImprintID;	// ID of merc that item is imprinted on
	UINT8		ubWeight;
	UINT8		fUsed;				// flags for whether the item is used or not
};

namespace ObjectDataStructs {
	struct OBJECT_GUN
	{
		INT16		bGunStatus;		// status % of gun
		UINT8		ubGunAmmoType;	// ammo type, as per weapons.h
		UINT16		ubGunShotsLeft;	// duh, amount of ammo left
		UINT16		usGunAmmoItem;	// the item # for the item table
		INT16		bGunAmmoStatus; // only for "attached ammo" - grenades, mortar shells
		UINT8		ubGunState;		// SB manual recharge
	};
	struct OBJECT_MONEY
	{
		INT16		bMoneyStatus;
		UINT32		uiMoneyAmount;
	};
	struct OBJECT_BOMBS_AND_OTHER
	{ // this is used by placed bombs, switches, and the action item
		INT16		bBombStatus;		// % status
		INT8		bDetonatorType;		// timed, remote, or pressure-activated
		UINT16		usBombItem;			// the usItem of the bomb.
		union
		{
			INT8		bDelay;			// >=0 values used only
			INT8		bFrequency;		// >=0 values used only
		};
		UINT8	ubBombOwner;			// side which placed the bomb
		UINT8	bActionValue;			// this is used by the ACTION_ITEM fake item
		union
		{
			UINT8 ubTolerance;			// tolerance value for panic triggers
			UINT8 ubLocationID;			// location value for remote non-bomb (special!) triggers
		};	
	};
	struct OBJECT_KEY
	{
		INT16 bKeyStatus;
		UINT8 ubKeyID;
	};
	struct OBJECT_OWNER
	{
		UINT16 ubOwnerProfile;
		UINT8 ubOwnerCivGroup;
	};
	struct OBJECT_LBE
	{
		INT16	bLBEStatus;
		INT8	bLBE;				// Marks item as LBENODE
		int		uniqueID;			// how the LBENODE is accessed
	};
};

class ObjectData
{
public:
	//needs a default ctor that inits stuff so that an objectStack can be init with 1 empty ObjectData
	ObjectData() {initialize();};
	~ObjectData();
	// Copy Constructor
	ObjectData(const ObjectData&);
	// Assignment operator
    ObjectData& operator=(const ObjectData&);


	void	initialize() {memset(this, 0, sizeof(ObjectData));};
	void	DeleteLBE();
	void	DuplicateLBE();
	bool operator==(ObjectData& compare);
	bool operator==(const ObjectData& compare)const;


	union {
		INT16										objectStatus;//holds the same value as bStatus[0]
		UINT16										ubShotsLeft;//holds the same value as ubShotsLeft[0]
		ObjectDataStructs::OBJECT_GUN				gun;
		ObjectDataStructs::OBJECT_MONEY				money;
		ObjectDataStructs::OBJECT_BOMBS_AND_OTHER	misc;
		ObjectDataStructs::OBJECT_KEY				key;
		ObjectDataStructs::OBJECT_OWNER				owner;
		ObjectDataStructs::OBJECT_LBE				lbe;
	};
	INT8		bTrap;			// 1-10 exp_lvl to detect
	UINT8		fUsed;			// flags for whether the item is used or not
	UINT8		ubImprintID;	// ID of merc that item is imprinted on
};


typedef	std::list<OBJECTTYPE>	attachmentList;
class StackedObjectData  {
public:
	StackedObjectData();
	~StackedObjectData();
	void	initialize() {attachments.clear(); data.initialize();};
	OBJECTTYPE* GetAttachmentAtIndex(UINT8 index);
	bool operator==(StackedObjectData& compare);
	bool operator==(const StackedObjectData& compare)const;

	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	attachmentList	attachments;
	ObjectData		data;
};
typedef std::list<StackedObjectData>	StackedObjects;


#define ALL_OBJECTS -1
class OBJECTTYPE
{
public:
	// Constructor
	OBJECTTYPE();
	// Conversion operator
    OBJECTTYPE& operator=(const OLD_OBJECTTYPE_101&);
	// Copy Constructor
	OBJECTTYPE(const OBJECTTYPE&);
	// Assignment operator
    OBJECTTYPE& operator=(const OBJECTTYPE&);
	// Destructor
	~OBJECTTYPE();

	StackedObjectData* operator[](const unsigned int index);

	// Initialize the soldier.  
	//  Use this instead of the old method of calling memset.
	//  Note that the constructor does this automatically.
	void	initialize();

	bool	operator==(OBJECTTYPE& compare);
	bool	operator==(const OBJECTTYPE& compare)const;
	bool	exists();
	bool	IsActiveLBE(unsigned int index);
	bool	HasAnyActiveLBEs(SOLDIERTYPE * pSoldier = NULL, UINT8 iter = 0);
	LBENODE*	GetLBEPointer(unsigned int index);


	UINT16	GetWeightOfObjectInStack(unsigned int index = 0);
	int		AddObjectsToStack(int howMany, int objectStatus = 100);
	int		AddObjectsToStack(OBJECTTYPE& sourceObject, int howManyWanted = ALL_OBJECTS, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0, bool allowLBETransfer = true);
	int		ForceAddObjectsToStack(OBJECTTYPE& sourceObject, int howManyWanted = ALL_OBJECTS);
	int		MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove = ALL_OBJECTS, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0);
	int		RemoveObjectsFromStack(int howMany);
	bool	RemoveObjectAtIndex(unsigned int index, OBJECTTYPE* destObject = NULL);
private://these are only helpers for the above functions
	int		PrivateRemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject = NULL, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0);
	void	SpliceData(OBJECTTYPE& sourceObject, unsigned int numToSplice, StackedObjects::iterator beginIter);
	bool	CanStack(OBJECTTYPE& sourceObject, int& numToStack);
public:

	BOOLEAN AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound = TRUE, UINT8 subObject = 0);
	BOOLEAN RemoveAttachment( OBJECTTYPE* pAttachment, OBJECTTYPE * pNewObj = NULL, UINT8 subObject = 0);


	//see comments in .cpp
	static	void DeleteMe(OBJECTTYPE** ppObject);
	static	void CopyToOrCreateAt(OBJECTTYPE** ppTarget, OBJECTTYPE* pSource);

	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	//POD
	UINT16		usItem;
	UINT8		ubNumberOfObjects;
	UINT8		ubMission;		//EDIT THIS OUT WHEN THERE ARE NO ASSERTS!

	//ADB ubWeight has been removed because it is always recalculated at every object change but only used in 1 place!!!
	//much better to recalculate it only where it is used once!!!
	//UINT16		ubWeight;		//used to be UINT8
	UINT8		fFlags;//used to be INT8, but that makes anything with OBJECT_NO_OVERWRITE negative

	char		endOfPOD;
#define SIZEOF_OBJECTTYPE_POD	(offsetof(OBJECTTYPE, endOfPOD))

	StackedObjects		objectStack;
};

extern OBJECTTYPE gTempObject;

// SUBTYPES
#define IC_NONE						0x00000001
#define IC_GUN						0x00000002
#define IC_BLADE					0x00000004
#define IC_THROWING_KNIFE	0x00000008

#define IC_LAUNCHER				0x00000010
#define IC_TENTACLES			0x00000020

#define IC_THROWN					0x00000040
#define IC_PUNCH					0x00000080

#define IC_GRENADE				0x00000100
#define IC_BOMB						0x00000200
#define IC_AMMO						0x00000400
#define IC_ARMOUR					0x00000800

#define IC_MEDKIT					0x00001000
#define IC_KIT						0x00002000
#define IC_APPLIABLE			0x00004000
#define IC_FACE           0x00008000

#define IC_KEY						0x00010000
#define IC_LBEGEAR					0x00020000	// Added for LBE items as part of the new inventory system

#define IC_MISC						0x10000000
#define IC_MONEY					0x20000000

// PARENT TYPES
#define IC_WEAPON					( IC_GUN | IC_BLADE | IC_THROWING_KNIFE | IC_LAUNCHER | IC_TENTACLES )
#define IC_EXPLOSV				( IC_GRENADE | IC_BOMB )

#define IC_BOBBY_GUN			( IC_GUN | IC_LAUNCHER )
#define IC_BOBBY_MISC			( IC_GRENADE | IC_BOMB | IC_MISC | IC_MEDKIT | IC_KIT | IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH | IC_FACE | IC_LBEGEAR )


// replaces candamage
//#define ITEM_DAMAGEABLE			0x0001
//// replaces canrepair
//#define ITEM_REPAIRABLE			0x0002
//// replaces waterdamage
//#define ITEM_WATER_DAMAGES	0x0004
//// replaces metal
//#define ITEM_METAL					0x0008
//// replaces sinkable
//#define ITEM_SINKS					0x0010
//// replaces seemeter
//#define ITEM_SHOW_STATUS		0x0020
//// for attachers/merges, hidden
//#define ITEM_HIDDEN_ADDON		0x0040
//// needs two hands
//#define ITEM_TWO_HANDED			0x0080
//// can't be found for sale
//#define ITEM_NOT_BUYABLE		0x0100
//// item is an attachment for something
//#define ITEM_ATTACHMENT			0x0200
//// item only belongs in the "big gun list"
//#define ITEM_BIGGUNLIST			0x0400
//// item should not be placed via the editor
//#define ITEM_NOT_EDITOR			0x0800
//// item defaults to undroppable
//#define ITEM_DEFAULT_UNDROPPABLE	0x1000
//// item is terrible for throwing
//#define ITEM_UNAERODYNAMIC	0x2000
//// item is electronic for repair (etc) purposes
//#define ITEM_ELECTRONIC			0x4000
//// item is a PERMANENT attachment
//#define ITEM_INSEPARABLE		0x8000
//
//// item flag combinations
//
//#define IF_STANDARD_GUN ITEM_DAMAGEABLE | ITEM_WATER_DAMAGES | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_METAL | ITEM_SINKS
//#define IF_TWOHANDED_GUN IF_STANDARD_GUN | ITEM_TWO_HANDED
//#define IF_STANDARD_BLADE ITEM_DAMAGEABLE | ITEM_WATER_DAMAGES | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_METAL | ITEM_SINKS
//#define IF_STANDARD_ARMOUR ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_SINKS
//#define IF_STANDARD_KIT ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_SINKS
//#define IF_STANDARD_CLIP ITEM_SINKS | ITEM_METAL

//#define EXPLOSIVE_GUN( x ) ( x == ROCKET_LAUNCHER || x == TANK_CANNON || x == RPG7 )

typedef struct
{
	UINT32		usItemClass;
	UINT16			ubClassIndex;
	UINT8			ubCursor;
	INT8			bSoundType;
	UINT8			ubGraphicType;
	UINT16			ubGraphicNum;
	UINT8			ubWeight; //2 units per kilogram; roughly 1 unit per pound
	UINT8			ubPerPocket;
	UINT8			ItemSize;
	UINT16		usPrice;
	UINT8			ubCoolness;
	INT8			bReliability;
	INT8			bRepairEase;
	UINT16		fFlags;

	UINT32	uiIndex; // added
	CHAR16	szItemName[80]; //+1 for the null terminator //added

	BOOLEAN damageable;
	BOOLEAN repairable;
	BOOLEAN waterdamages;
	BOOLEAN metal;
	BOOLEAN sinks;
	BOOLEAN showstatus;
	BOOLEAN hiddenaddon;
	BOOLEAN twohanded;
	BOOLEAN notbuyable;
	BOOLEAN attachment;
	BOOLEAN hiddenattachment;
	BOOLEAN biggunlist;
	BOOLEAN notineditor;
	BOOLEAN defaultundroppable;
	BOOLEAN unaerodynamic;
	BOOLEAN electronic;
	BOOLEAN inseparable;

	CHAR16 szLongItemName[80];
	CHAR16 szItemDesc[400];
	CHAR16 szBRName[80];
	CHAR16 szBRDesc[400];

	//TODO: quest items, boosters, money

	// special item/attachment functions:
	BOOLEAN cannon;
	BOOLEAN rocketrifle;
	BOOLEAN fingerprintid;
	BOOLEAN metaldetector;
	BOOLEAN gasmask;
	BOOLEAN lockbomb;
	BOOLEAN flare;
	INT16 percentnoisereduction;
	INT16 bipod;
	INT16 tohitbonus;
	INT16 bestlaserrange;
	INT16 rangebonus;
	INT16 aimbonus;
	INT16 minrangeforaimbonus;
	INT16 percentapreduction;
	INT16 percentstatusdrainreduction;
	BOOLEAN grenadelauncher;
	BOOLEAN mortar;
	BOOLEAN duckbill;
	BOOLEAN detonator;
	BOOLEAN remotedetonator;
	BOOLEAN hidemuzzleflash;
	BOOLEAN rocketlauncher;
	BOOLEAN singleshotrocketlauncher;
	UINT16	discardedlauncheritem;
	BOOLEAN brassknuckles;
	UINT16  bloodieditem;
	BOOLEAN crowbar;
	BOOLEAN glgrenade;
	BOOLEAN flakjacket;
	INT16 hearingrangebonus;
	INT16	visionrangebonus;
	INT16	nightvisionrangebonus;
	INT16	dayvisionrangebonus;
	INT16	cavevisionrangebonus;
	INT16	brightlightvisionrangebonus;
	INT16	itemsizebonus;
	BOOLEAN leatherjacket;
	BOOLEAN batteries;
	BOOLEAN needsbatteries;
	BOOLEAN xray;
	BOOLEAN wirecutters;
	BOOLEAN toolkit;
	BOOLEAN firstaidkit;
	BOOLEAN medicalkit;
	BOOLEAN canteen;
	BOOLEAN jar;
	BOOLEAN canandstring;
	BOOLEAN marbles;
	BOOLEAN walkman;
	BOOLEAN remotetrigger;
	BOOLEAN robotremotecontrol;
	BOOLEAN camouflagekit;
	BOOLEAN locksmithkit;
	BOOLEAN mine;
	BOOLEAN alcohol;
	BOOLEAN hardware;
	BOOLEAN medical;
	BOOLEAN gascan;
	BOOLEAN containsliquid;
	BOOLEAN rock;
	
	INT16 damagebonus;
	INT16 meleedamagebonus;
	
	INT16 magsizebonus;
	INT16 percentautofireapreduction;
	INT16 autofiretohitbonus;
	INT16 APBonus;
	INT16 rateoffirebonus;
	INT16 burstsizebonus;
	INT16 bursttohitbonus;
	INT16 percentreadytimeapreduction;
	INT16 bulletspeedbonus;
	BOOLEAN thermaloptics;
	UINT8 percenttunnelvision;
	INT16 percentreloadtimeapreduction;
	INT16 percentburstfireapreduction;

	INT16 camobonus;
	INT16 stealthbonus;
	INT16 urbanCamobonus;
	INT16 desertCamobonus;
	INT16 snowCamobonus;

	BOOLEAN scifi; // item only available in scifi mode
	BOOLEAN newinv;	// item only available in new inventory mode

	UINT16 defaultattachment;
} INVTYPE;

// CHRISL: Added new structures to handle LBE gear and the two new XML files that will be needed to deal
// with the IC pockets and the new inventory system.
class LBETYPE{
public:
	LBETYPE();
	LBETYPE(const LBETYPE&);
	LBETYPE& operator=(const LBETYPE&);
	~LBETYPE();
	UINT16			lbeIndex;
	UINT32			lbeClass;
	UINT8			lbeCombo;
	UINT8			lbeFilledSize;
	char			POD;
	std::vector<UINT8>	lbePocketIndex;
};
#define SIZEOF_LBETYPE offsetof( LBETYPE, POD )
extern std::vector<LBETYPE> LoadBearingEquipment;
typedef enum eLBE_CLASS	// Designation of lbeClass
{
	THIGH_PACK=1,
	VEST_PACK,
	COMBAT_PACK,
	BACKPACK,
	LBE_POCKET,
	OTHER_POCKET
};


class POCKETTYPE{
public:
	POCKETTYPE();
	POCKETTYPE(const POCKETTYPE&);
	POCKETTYPE& operator=(const POCKETTYPE&);
	~POCKETTYPE();
	UINT16			pIndex;
	CHAR8			pName[80];
	UINT8			pSilhouette;
	UINT16			pType;
	UINT32			pRestriction;
	char			POD;
	std::vector<UINT8>	ItemCapacityPerSize;
};
#define SIZEOF_POCKETTYPE offsetof( POCKETTYPE, POD )
extern std::vector<POCKETTYPE> LBEPocketType;
typedef enum ePOCKET_TYPE
{
	NO_POCKET_TYPE = 0,
	GUNSLING_POCKET_TYPE = 1,
	KNIFE_POCKET_TYPE = 2,
	VEHICLE_POCKET_TYPE = 3,
};

#define FIRST_WEAPON 1
#define FIRST_AMMO 71
#define FIRST_EXPLOSIVE 131
#define FIRST_ARMOUR 161
#define FIRST_MISC 201
#define FIRST_KEY 271

#define NOTHING NONE
typedef enum
{
	NONE									= 0,

	// weapons
	GLOCK_17							= FIRST_WEAPON,
	GLOCK_18,
	BERETTA_92F,
	BERETTA_93R,
	SW38,
	BARRACUDA,
	DESERTEAGLE,
	M1911,
	MP5K,
	MAC10,	//10

	THOMPSON,
	COMMANDO,
	MP53,
	AKSU74,
	P90,
	TYPE85,
	SKS,
	DRAGUNOV,
	M24,
	AUG,	//20

	G41,
	MINI14,
	C7,
	FAMAS,
	AK74,
	AKM,
	M14,
	FNFAL,
	G3A3,
	G11,	//30

	M870,
	SPAS15,
	CAWS,
	MINIMI,
	RPK74,
	HK21E,
	COMBAT_KNIFE,
	THROWING_KNIFE,
	ROCK,
	GLAUNCHER,	//40

	MORTAR,					
	ROCK2,
	CREATURE_YOUNG_MALE_CLAWS,
	CREATURE_OLD_MALE_CLAWS,
	CREATURE_YOUNG_FEMALE_CLAWS,
	CREATURE_OLD_FEMALE_CLAWS,
	CREATURE_QUEEN_TENTACLES,
	CREATURE_QUEEN_SPIT,
	BRASS_KNUCKLES,
	UNDER_GLAUNCHER,	//50

	ROCKET_LAUNCHER,
	BLOODCAT_CLAW_ATTACK,
	BLOODCAT_BITE,
	MACHETE,
	ROCKET_RIFLE,
	AUTOMAG_III,
	CREATURE_INFANT_SPIT,
	CREATURE_YOUNG_MALE_SPIT,
	CREATURE_OLD_MALE_SPIT,
	TANK_CANNON,	//60

	DART_GUN,
	BLOODY_THROWING_KNIFE,
	FLAMETHROWER,
	CROWBAR,
	AUTO_ROCKET_RIFLE,	//65
// MADD MARKER

	BARRETT, //previously unused
	VAL_SILENT, //previously unused
	PSG, //previously unused
	TAR21, //previously unused
	
	MAX_WEAPONS							= ( FIRST_AMMO - 1 ), 

	CLIP9_15							= FIRST_AMMO, 
	CLIP9_30,
	CLIP9_15_AP,
	CLIP9_30_AP,
	CLIP9_15_HP,
	CLIP9_30_HP,
	CLIP38_6,
	CLIP38_6_AP,
	CLIP38_6_HP,
	CLIP45_7,  

	CLIP45_30,
	CLIP45_7_AP,
	CLIP45_30_AP,
	CLIP45_7_HP,
	CLIP45_30_HP,
	CLIP357_6,
	CLIP357_9,
	CLIP357_6_AP,
	CLIP357_9_AP,
	CLIP357_6_HP,

	CLIP357_9_HP,
	CLIP545_30_AP,
	CLIP545_30_HP,
	CLIP556_30_AP,
	CLIP556_30_HP,
	CLIP762W_10_AP,
	CLIP762W_30_AP,
	CLIP762W_10_HP,
	CLIP762W_30_HP,
	CLIP762N_5_AP,

	CLIP762N_20_AP,
	CLIP762N_5_HP,
	CLIP762N_20_HP,
	CLIP47_50_SAP,
	CLIP57_50_AP,
	CLIP57_50_HP,
	CLIP12G_7,
	CLIP12G_7_BUCKSHOT,
	CLIPCAWS_10_SAP,
	CLIPCAWS_10_FLECH,

	CLIPROCKET_AP,
	CLIPROCKET_HE,
	CLIPROCKET_HEAT,
	CLIPDART_SLEEP,

	CLIPFLAME,
// MADD MARKER
	CLIP50_11, // unused 
	CLIP9H_20, // unused 
	CLIP9_50, 
	CLIP9_50_AP, 
	CLIP9_50_HP, 
	DRUM545_75_AP,
	DRUM545_75_HP,
	BELT556_200_AP,
	BELT556_200_HP,
	BELT762N_100_AP,
	BELT762N_100_HP,
	CLIP57_20_AP,
	CLIP57_20_HP,
	// unused 
	// unused 

	// explosives
	STUN_GRENADE					= FIRST_EXPLOSIVE,
	TEARGAS_GRENADE,
	MUSTARD_GRENADE,
	MINI_GRENADE,
	HAND_GRENADE,
	RDX,
	TNT,
	HMX,
	C1,
	MORTAR_SHELL,

	MINE,
	C4,
	TRIP_FLARE,
	TRIP_KLAXON,
	SHAPED_CHARGE,
	BREAK_LIGHT,
	GL_HE_GRENADE,
	GL_TEARGAS_GRENADE,
	GL_STUN_GRENADE,
	GL_SMOKE_GRENADE,

	SMOKE_GRENADE,
	TANK_SHELL,
	STRUCTURE_IGNITE,
	CREATURE_COCKTAIL,
	STRUCTURE_EXPLOSION,
	GREAT_BIG_EXPLOSION,
	BIG_TEAR_GAS,
	SMALL_CREATURE_GAS,
	LARGE_CREATURE_GAS,
	VERY_SMALL_CREATURE_GAS=270, // Renumbered because the original 160 was lost

	// armor
	FLAK_JACKET=161,					//= FIRST_ARMOUR, ( We're out of space! )
	FLAK_JACKET_18,
	FLAK_JACKET_Y,
	KEVLAR_VEST,
	KEVLAR_VEST_18,
	KEVLAR_VEST_Y,
	SPECTRA_VEST,
	SPECTRA_VEST_18,
	SPECTRA_VEST_Y,
	KEVLAR_LEGGINGS,

	KEVLAR_LEGGINGS_18,
	KEVLAR_LEGGINGS_Y,
	SPECTRA_LEGGINGS,
	SPECTRA_LEGGINGS_18,
	SPECTRA_LEGGINGS_Y,
	STEEL_HELMET,
	KEVLAR_HELMET,
	KEVLAR_HELMET_18,
	KEVLAR_HELMET_Y,
	SPECTRA_HELMET,

	SPECTRA_HELMET_18,
	SPECTRA_HELMET_Y,
	CERAMIC_PLATES,
	CREATURE_INFANT_HIDE,
	CREATURE_YOUNG_MALE_HIDE,
	CREATURE_OLD_MALE_HIDE,
	CREATURE_QUEEN_HIDE,
	LEATHER_JACKET,
	LEATHER_JACKET_W_KEVLAR,
	LEATHER_JACKET_W_KEVLAR_18,
	
	LEATHER_JACKET_W_KEVLAR_Y,
	CREATURE_YOUNG_FEMALE_HIDE,
	CREATURE_OLD_FEMALE_HIDE,
	TSHIRT,
	TSHIRT_DEIDRANNA,
	KEVLAR2_VEST,
	KEVLAR2_VEST_18,
	KEVLAR2_VEST_Y,

	// kits
	FIRSTAIDKIT								= FIRST_MISC,
	MEDICKIT,
	TOOLKIT,
	LOCKSMITHKIT,
	CAMOUFLAGEKIT,
	BOOBYTRAPKIT,
	// miscellaneous
	SILENCER,
	SNIPERSCOPE,
	BIPOD,
	EXTENDEDEAR,

	NIGHTGOGGLES,
	SUNGOGGLES,
	GASMASK,
	CANTEEN,
	METALDETECTOR,
	COMPOUND18,
	JAR_QUEEN_CREATURE_BLOOD,
	JAR_ELIXIR,
	MONEY,
	JAR,

	JAR_CREATURE_BLOOD,
	ADRENALINE_BOOSTER,
	DETONATOR,
	REMDETONATOR,	
	VIDEOTAPE,
	DEED,
	LETTER,
	TERRORIST_INFO,
	CHALICE,
	BLOODCAT_CLAWS,

	BLOODCAT_TEETH,
	BLOODCAT_PELT,
	SWITCH,
	// fake items
	ACTION_ITEM,
	REGEN_BOOSTER,
	SYRINGE_3,
	SYRINGE_4,
	SYRINGE_5,
	JAR_HUMAN_BLOOD,
	OWNERSHIP,

	// additional items
	LASERSCOPE,
	REMOTEBOMBTRIGGER,
	WIRECUTTERS,
	DUCKBILL,
	ALCOHOL,
	UVGOGGLES,
	DISCARDED_LAW,
	HEAD_1,
	HEAD_2,
	HEAD_3,
	HEAD_4,
	HEAD_5,
	HEAD_6,
	HEAD_7,
	WINE,
	BEER,
	PORNOS,
	VIDEO_CAMERA,
	ROBOT_REMOTE_CONTROL,
	CREATURE_PART_CLAWS,
	CREATURE_PART_FLESH,
	CREATURE_PART_ORGAN,
	REMOTETRIGGER,
	GOLDWATCH,
	GOLFCLUBS,
	WALKMAN,
	PORTABLETV,
	MONEY_FOR_PLAYERS_ACCOUNT,
	CIGARS,

	KEY_1 = FIRST_KEY,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_10,

	FLASH_SUPPRESSOR,
	RPG7,
	RPG_HE_ROCKET,
	RPG_AP_ROCKET,
	RPG_FRAG_ROCKET,
	REFLEX_SCOPED,
	REFLEX_UNSCOPED,
	KEY_18,
	KEY_19,
	KEY_20,

	KEY_21,
	KEY_22,
	KEY_23,
	KEY_24,
	KEY_25,
	KEY_26,
	KEY_27,
	KEY_28,
	KEY_29,
	KEY_30,

	KEY_31,
	KEY_32,		// 302
	SILVER_PLATTER,
	DUCT_TAPE,
	ALUMINUM_ROD,
	SPRING,
	SPRING_AND_BOLT_UPGRADE,
	STEEL_ROD,
	QUICK_GLUE,
	GUN_BARREL_EXTENDER,

	STRING,
	TIN_CAN,
	STRING_TIED_TO_TIN_CAN,
	MARBLES,
	LAME_BOY,
	COPPER_WIRE,
	DISPLAY_UNIT,
	FUMBLE_PAK,
	XRAY_BULB,
	CHEWING_GUM, // 320

	FLASH_DEVICE,
	BATTERIES,
	ELASTIC,
	XRAY_DEVICE,
	SILVER,
	GOLD,
	GAS_CAN,
	M900, //UNUSED_26,
	M950, //UNUSED_27,
	M960, //UNUSED_28,
	MICRO_UZI, // UNUSED_29,
	
	ENFIELD, // UNUSED_30,
	MP5,
	MP5SD,
	MP5N,
	UMP45,
	FIVE7,
	P7M8,
	G36K,
	G36C, // 340

	MSG90A1,
	BENNELLI,
	AK103,
	UNUSED_42,
	UNUSED_43,
	UNUSED_44,
	UNUSED_45,
	UNUSED_46,
	UNUSED_47,
	UNUSED_48, // 350

	//CHRISL: Default NIV items
	DEFAULT_THIGH = 428,
	DEFAULT_VEST = 434,
	DEFAULT_CPACK = 442,
	DEFAULT_BPACK = 448,

	MAXITEMS = 5001
} ITEMDEFINE;

/* CHRISL: Arrays to track ic group information.  These allow us to determine which LBE slots control which pockets and
what LBE class the pockets are.*/
//										{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54}
// Determines which LBE Slot controls each pocket
const INT8	icLBE[NUM_INV_SLOTS] =		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,10,10,11,11,11,11, 7, 7, 8, 9, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,11,11,11,11,11,11,11,11};

// Determines the class (and default definition) of the controlling pocket
const INT8	icClass[NUM_INV_SLOTS] =	{-1,-1,-1,-1,-1,-1,-1, 5, 5, 5, 5, 5, 6, 6, 3, 3, 3, 4, 4, 4, 4, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

// Determines the pocket number to look at in LBETYPE
const INT8	icPocket[NUM_INV_SLOTS] =	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4, 5, 6, 8, 9,10,11,10,11, 4, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7};

// Determines which pockets are used in the old inventory system.
const INT8	oldInv[NUM_INV_SLOTS] =		{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Determines which pockets to use for vehicles in the new inventory system.
const INT8	vehicleInv[NUM_INV_SLOTS]=	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};

// Determines the default pocket
const INT16	icDefault[NUM_INV_SLOTS] =	{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK,
	DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK,
	DEFAULT_VEST, DEFAULT_VEST,
	DEFAULT_THIGH, DEFAULT_THIGH,
	DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST,
	DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH,
	DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK,
	DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK};

#define FIRST_HELMET STEEL_HELMET
#define LAST_HELMET SPECTRA_HELMET_Y

#define FIRST_VEST FLAK_JACKET
#define LAST_VEST KEVLAR2_VEST_Y

#define FIRST_LEGGINGS KEVLAR_LEGGINGS
#define LAST_LEGGINGS SPECTRA_LEGGINGS_Y

#define FIRST_HEAD_ITEM EXTENDEDEAR
#define LAST_HEAD_ITEM SUNGOGGLES

#define MAXATTACHMENTS 30000

extern INVTYPE Item[MAXITEMS];
extern UINT16 Attachment[MAXATTACHMENTS][3];

typedef struct
{
	UINT16	usItem;
	UINT32	uiItemClass;
	INT8		bAttachmentSkillCheck;
	INT8		bAttachmentSkillCheckMod;
	UINT32	uiIndex;
} AttachmentInfoStruct;

extern AttachmentInfoStruct AttachmentInfo[MAXITEMS+1]; 

extern UINT16 Launchable[MAXITEMS+1][2];
extern UINT16 CompatibleFaceItems[MAXITEMS+1][2];
extern UINT16 IncompatibleAttachments[MAXATTACHMENTS][2];

typedef enum
{
	DESTRUCTION,
	COMBINE_POINTS,
	TREAT_ARMOUR,
	EXPLOSIVE,
	EASY_MERGE,
	ELECTRONIC_MERGE,
	USE_ITEM,
	USE_ITEM_HARD
} MergeType;

extern UINT16 Merge[MAXITEMS+1][6];

typedef struct
{
	UINT16	usItem;
	UINT16	usAttachment[2];
	UINT16	usResult;
	UINT32  uiIndex;
} ComboMergeInfoStruct;

extern ComboMergeInfoStruct AttachmentComboMerge[MAXITEMS+1];
BOOLEAN EXPLOSIVE_GUN(UINT16 x );

typedef struct IMP_ITEM_CHOICE_TYPE
{
	UINT8		ubChoices;						// how many valid choices there are in this category
	INT16		bItemNo[50];						// room for up to 50 choices in each category
	UINT8		ubNumItems;						// # of items to select

	UINT32		uiIndex;						
} IMP_ITEM_CHOICE_TYPE;

enum
{
	IMP_DEFAULT = 0,
	IMP_RANDOMDEFAULT,
//ATTRIBUTES
	IMP_WISDOM,
	IMP_DEXTERITY,
	IMP_AGILITY,
	IMP_STRENGTH,
	IMP_HEALTH,
	IMP_LEADERSHIP,
	IMP_MARKSMANSHIP,
	IMP_EXPLOSIVE,
	IMP_MECHANICAL,
	IMP_MEDICAL,
//SKILLS
	IMP_HTH,
	IMP_MARTIALARTS,
	IMP_NIGHTOPS,
	IMP_STEALTHY,
	IMP_ROOFTOPS,
	IMP_LOCKPICKING,
	IMP_ELECTRONICS,
	IMP_THROWING,
	IMP_KNIFING,
	IMP_HEAVYWEAPONS,
	IMP_AUTOWEAPONS,
	IMP_AMBIDEXTROUS,
	IMP_CAMOUFLAGED,
	IMP_TEACHING,
	IMP_THIEF,

	MAX_IMP_ITEM_TYPES
};

extern IMP_ITEM_CHOICE_TYPE gIMPItemChoices[MAX_IMP_ITEM_TYPES];

#endif