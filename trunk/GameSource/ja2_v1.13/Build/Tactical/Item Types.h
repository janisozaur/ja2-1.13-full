#ifndef ITEM_TYPES_H
#define ITEM_TYPES_H

#include "types.h"

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

#define MAX_OBJECTS_PER_SLOT 8
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

typedef struct
{
	UINT16	usItem;
	UINT8		ubNumberOfObjects;
	union
	{
		struct
		{
			INT8		bGunStatus;			// status % of gun
			UINT8		ubGunAmmoType;	// ammo type, as per weapons.h
			UINT8		ubGunShotsLeft;	// duh, amount of ammo left
			UINT16		usGunAmmoItem;	// the item # for the item table
			INT8		bGunAmmoStatus; // only for "attached ammo" - grenades, mortar shells
			UINT8		ubGunState; // SB manual recharge
			UINT8		ubGunUnused[MAX_OBJECTS_PER_SLOT - 6];
		};
		struct
		{
			UINT8		ubShotsLeft[MAX_OBJECTS_PER_SLOT];
		};
		struct
		{
			INT8		bStatus[MAX_OBJECTS_PER_SLOT];
		};		
		struct
		{
			INT8		bMoneyStatus;
			UINT32	uiMoneyAmount;
			UINT8		ubMoneyUnused[MAX_OBJECTS_PER_SLOT - 5];
		};
		struct
		{ // this is used by placed bombs, switches, and the action item
			INT8		bBombStatus;			// % status
			INT8		bDetonatorType;		// timed, remote, or pressure-activated
			UINT16	usBombItem;				// the usItem of the bomb.
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
			UINT8 ubBombOwner; // side which placed the bomb
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
  // attached objects
	UINT16	usAttachItem[MAX_ATTACHMENTS];
	INT8		bAttachStatus[MAX_ATTACHMENTS];

	INT8		fFlags;
	UINT8		ubMission;
	INT8		bTrap;        // 1-10 exp_lvl to detect
	UINT8		ubImprintID;	// ID of merc that item is imprinted on
	UINT8		ubWeight;
	UINT8		fUsed;				// flags for whether the item is used or not
} OBJECTTYPE;

/*
typedef struct
{
	UINT8		ubCursor;
	INT8		bSoundType;
	UINT8		ubGraphicNum;
	INT8		bMaxLoad;

	UINT8		ubPerPocket;
	UINT8		ubCanDamage;
	UINT8		ubWaterDamage;
	UINT8		ubCanRepair;

	UINT8		ubSeeMeter;
	UINT8		ubRange;
	UINT8		ubMetal;
	UINT8		ubSinkable;

	UINT16	ubPrice;
	UINT8		ubMission;
	UINT8		ubCoolness;
} INVTYPE;

*/



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

#define IC_MISC						0x10000000
#define IC_MONEY					0x20000000

// PARENT TYPES
#define IC_WEAPON					( IC_GUN | IC_BLADE | IC_THROWING_KNIFE | IC_LAUNCHER | IC_TENTACLES )
#define IC_EXPLOSV				( IC_GRENADE | IC_BOMB )

#define IC_BOBBY_GUN			( IC_GUN | IC_LAUNCHER )
#define IC_BOBBY_MISC			( IC_GRENADE | IC_BOMB | IC_MISC | IC_MEDKIT | IC_KIT | IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH | IC_FACE )


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
	UINT16		usPrice;
	UINT8			ubCoolness;
	INT8			bReliability;
	INT8			bRepairEase;
	UINT16		fFlags;

	UINT32	uiIndex; // added
	INT8	szItemName[80]; //+1 for the null terminator //added

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
	BOOLEAN biggunlist;
	BOOLEAN notineditor;
	BOOLEAN defaultundroppable;
	BOOLEAN unaerodynamic;
	BOOLEAN electronic;
	BOOLEAN inseparable;

	INT8 szLongItemName[80];
	INT8 szItemDesc[400];
	INT8 szBRName[80];
	INT8 szBRDesc[400];

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
	BOOLEAN bipod;
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

	UINT16 defaultattachment;
} INVTYPE;

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
  VERY_SMALL_CREATURE_GAS,

	// armor
	FLAK_JACKET,						//= FIRST_ARMOUR, ( We're out of space! )
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

	MAXITEMS = 5001
} ITEMDEFINE;

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
extern UINT16 Attachment[MAXATTACHMENTS][2];

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

extern UINT16 Merge[MAXITEMS+1][4];

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