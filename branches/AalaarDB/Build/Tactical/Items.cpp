#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "Action Items.h"
	#include "weapons.h"
	#include "Interface Cursors.h"

	#include "overhead.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "Sound Control.h"
	#include "Sys globals.h"
	#include "Isometric Utils.h"
	#include "Animation Data.h"
	#include "worldman.h"
	#include "Random.h"
	#include "Campaign.h"
	#include "interface.h"
	#include "interface panels.h"
	#include "explosion control.h"
	#include "Keys.h"

	#include "wcheck.h"
	#include "soldier profile.h"
	#include "SkillCheck.h"
	#include "los.h"
	#include "message.h"
	#include "text.h"

	#include "fov.h"
	#include "MessageBoxScreen.h"

	#include "PathAIDebug.h"
	#include "Interface Control.h"
	#include "ShopKeeper Interface.h"
	#include "Cursors.h"

	#include "GameSettings.h"
	#include "environment.h"
	#include "Auto Resolve.h"
	#include "Interface Items.h"
	#include "Campaign Types.h"
	#include "history.h"
	#include "Game Clock.h"
	#include "strategicmap.h"
	#include "Inventory Choosing.h"
	#include "Soldier macros.h"
	#include "Smell.h"
	#include "lighting.h"
	#include "utilities.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define ANY_MAGSIZE 255

void RemoveObjs( OBJECTTYPE * pObj, UINT8 ubNumberToRemove );
void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem );

extern	SOLDIERTYPE *gpItemDescSoldier;

// weight units are 100g each

////////////////////////////////////////////////////////////////////////////
//ATE: When adding new items, make sure to update text.c with text description
///////////////////////////////////////////////////////////////////////////

INVTYPE Item[MAXITEMS]; //=
//{
////  							CLASS								SOUND			GRPH	GRA-			PER			         
////CLASS						INDEX		CURSOR			TYPE			TYPE	PHIC	WT	PCKT	PRICE COOL	DESCRIPTION							REL		REPAIR	FLAGS
////---------				-----		-------     -------		----	--	  --  ----  -----	----	-----------							---		------		-----
//{	IC_PUNCH,					0,			PUNCHCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			ITEM_NOT_BUYABLE , 0, "Nothing"},
////---WEAPONS---
//// NB For convenience, in accessing the Weapons table, the class index
//// of a weapon must be equal to its position in the Item table
//{	IC_GUN,			 			1,			TARGETCURS,	CONDBUL,	0,		1,		6,	1,		 350,	2,		/* Glock 17        */	+2,		+2,			IF_STANDARD_GUN },
//{	IC_GUN,			 			2,			TARGETCURS,	CONDBUL,	0,		2,		6,	1,		 480,	2,		/* Glock 18        */	+1,		+1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			3,			TARGETCURS,	CONDBUL,	0,		3,		11,	1,		 450,	2,		/* Beretta 92F     */	-1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			4,			TARGETCURS,	CONDBUL,	0,		4,		11,	1,		 470,	2,		/* Beretta 93R     */	-2,		-2,			IF_STANDARD_GUN },
//{	IC_GUN,			 			5,			TARGETCURS,	CONDBUL,	0,		5,		11,	1,		 250,	1,		/* .38 S&W Special */	+4,		+4,			IF_STANDARD_GUN },
//{	IC_GUN,			 			6,			TARGETCURS,	CONDBUL,	0,		6,		10,	1,		 300,	1,		/* .357 Barracuda  */	+3,		+3,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			7,			TARGETCURS,	CONDBUL,	0,		7,		17,	1,		 300,	1,		/* .357 DesertEagle*/ -1,		-1,			IF_STANDARD_GUN },
//{	IC_GUN,			 			8,			TARGETCURS,	CONDBUL,	0,		8,		11,	1,		 400,	2,		/* .45 M1911			 */  0,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,			 			9,			TARGETCURS,	CONDBUL,	0,		9,		21,	0,		 980,	3,		/* H&K MP5K      	 */	-1,		0,			IF_STANDARD_GUN},
//{	IC_GUN,						10,			TARGETCURS,	CONDBUL,	0,		10,		28,	0,		1170,	4,		/* .45 MAC-10	     */	-2,		-1,			IF_STANDARD_GUN },
//
//{	IC_GUN,						11,			TARGETCURS,	CONDBUL,	0,		11,		48,	0,		 700,	3,		/* Thompson M1A1   */	+3,		-3,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						12,			TARGETCURS,	CONDBUL,	0,		12,		26,	0,		1330,	5,		/* Colt Commando   */	 0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						13,			TARGETCURS,	CONDBUL,	0,		13,		28,	0,		1000,	4,		/* H&K MP53		 		 */	-1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						14,			TARGETCURS,	CONDBUL,	0,		14,		39,	0,		1180,	4,		/* AKSU-74         */ -2,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,						15,			TARGETCURS,	CONDBUL,	0,		15,		28,	0,		2750,	10,		/* 5.7mm FN P90    */ -2,		-4,			IF_STANDARD_GUN  },
//{	IC_GUN,						16,			TARGETCURS,	CONDBUL,	0,		16,		19,	0,		 620,	3,		/* Type-85         */ -4,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						17,			TARGETCURS,	CONDBUL,	0,		17,		39,	0,		1350,	5,		/* SKS             */ -4,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						18,			TARGETCURS,	CONDBUL,	0,		18,		43,	0,		1930,	6,		/* Dragunov        */ +2,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,						19,			TARGETCURS,	CONDBUL,	0,		19,		55,	0,		1950,	6,		/* M24             */ +4,		+4,			IF_TWOHANDED_GUN  },
//{	IC_GUN,						20,			TARGETCURS,	CONDBUL,	0,		20,		36,	0,		2380,	8,		/* Steyr AUG       */ +1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//
//{	IC_GUN,						21,			TARGETCURS,	CONDBUL,	0,		21,		41,	0,		1620,	6,		/* H&K G41         */ +1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						22,			TARGETCURS,	CONDBUL,	0,		22,		29,	0,		1100,	4,		/* Ruger Mini-14   */  0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						23,			TARGETCURS,	CONDBUL,	0,		23,		36,	0,		2680,	8,		/* C-7             */ -1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						24,			TARGETCURS,	CONDBUL,	0,		24,		36,	0,		1970,	7,		/* FA-MAS          */ -2,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						25,			TARGETCURS,	CONDBUL,	0,		25,		36,	0,		1830,	6,		/* AK-74           */ -1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						26,			TARGETCURS,	CONDBUL,	0,		26,		43,	0,		1450,	5,		/* AKM             */ +2,		+2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						27,			TARGETCURS,	CONDBUL,	0,		27,		29,	0,		2120,	7,		/* M-14            */ +1,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						28,			TARGETCURS,	CONDBUL,	0,		28,		43,	0,		2680,	8,		/* FN-FAL          */	 0,		-1,			IF_TWOHANDED_GUN },
//{	IC_GUN,						29,			TARGETCURS,	CONDBUL,	0,		29,		44,	0,		1570,	5,		/* H&K G3A3        */ +1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						30,			TARGETCURS,	CONDBUL,	0,		30,		38,	0,		2530,	8,		/* H&K G11         */	+3,		-4,			IF_TWOHANDED_GUN },
//
//{	IC_GUN,						31,			TARGETCURS,	CONDBUL,	0,		31,		36,	0,		 670,	3,		/* Remington M870  */	+3,		+3,			IF_TWOHANDED_GUN },
//{	IC_GUN,						32,			TARGETCURS,	CONDBUL,	0,		32,		38,	0,		 980,	4,		/* SPAS-15         */ -2,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						33,			TARGETCURS,	CONDBUL,	0,		33,		41,	0,		2900,	9,		/* CAWS            */	-3,		-3,			IF_TWOHANDED_GUN },
//{	IC_GUN,						34,			TARGETCURS,	CONDBUL,	0,		34,		68,	0,		3100,	10,		/* FN Minimi       */	-1,		-2,			IF_TWOHANDED_GUN },
//{	IC_GUN,						35,			TARGETCURS,	CONDBUL,	0,		35,		48,	0,		3180,	10,		/* RPK-74          */ -1,		-2,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,						36,			TARGETCURS,	CONDBUL,	0,		36,		93,	0,		3420,	10,		/* H&K 21E         */	+2,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_BLADE,					37,			KNIFECURS,  0,			1,		79,		9,	2,		 100,	2,		/* combat knife    */	+2,		+2,			IF_STANDARD_BLADE},
//{	IC_THROWING_KNIFE,38,					TARGETCURS, 0,			1,		53,		1,	4,		  50,	3,		/* throwing knife  */	-1,		-1,			IF_STANDARD_BLADE},
//{	IC_THROWN,				39,				TOSSCURS,	0,			1,		57,		5,	2,		   0,	0,		/* rock            */ 0,		0,			ITEM_NOT_BUYABLE},
//{	IC_LAUNCHER,			40,				TRAJECTORYCURS,	0,		0,		37,		26,	0,		 900,	7,		/* grenade launcher*/	0,		-1,			IF_TWOHANDED_GUN },
//
//{	IC_LAUNCHER,			41,				TRAJECTORYCURS,	0,		0,		0,		77,	0,		1800,	10,		/* mortar */					0,		-2,			IF_TWOHANDED_GUN},
//{	IC_THROWN,				42,				TOSSCURS,		0,		1,		60,		4,	3,		 0,		0,		/* another rock    */	0,		0,			ITEM_NOT_BUYABLE},
//{	IC_BLADE,					43,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* yng male claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					44,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* yng fem claws  */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					45,			KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* old male claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					46,			KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* old fem claws  */	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_TENTACLES,			47,				KNIFECURS,	0,			0,		0,		0,	1,		   0,	0,		/* queen tentacles*/	0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						48,			TARGETCURS,	0,			0,		0,		0,	1,		   0,	0,		/* queen spit */			0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_PUNCH,					49,			PUNCHCURS,	0,			1,		102,	1,	4,		  20,	2,		/* brass knuckles */	0,		0,			IF_STANDARD_BLADE },
//{	IC_LAUNCHER,			50,				INVALIDCURS,0,			0,		39,		13,	0,		 500,	8,		/* underslung g.l.*/	0,		0,			IF_STANDARD_GUN},
//
//{	IC_GUN,						51,			TARGETCURS,	0,			0,		38,		21,	0,		 500,	9,		/* rocket Launcher*/	0,		-3,			IF_TWOHANDED_GUN },	// now repairable
//{	IC_BLADE,					52,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* bloodcat claws*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					53,			KNIFECURS,	0,			0,		0,		0,	1, 		   0,	0,		/* bloodcat bite */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_BLADE,					54,			KNIFECURS,	0,			0,		41,		13,	0,		 200,	3,		/* machete */					0,		+3,			IF_STANDARD_BLADE},
//{	IC_GUN,						55,			TARGETCURS,	0,			0,		45,		40,	0,		5000,	10,		/* rocket rifle */		0,		-5,			IF_TWOHANDED_GUN | ITEM_ELECTRONIC },
//{	IC_GUN,						56,			TARGETCURS,	0,			0,		40,		12,	0,		1000,	0,		/* Automag III */			0,		-2,			IF_STANDARD_GUN },
//{	IC_GUN,						57,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* infant spit */			0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						58,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* yng male spit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//
//{	IC_GUN,						59,			TARGETCURS,	0,			0,		0,		0,	0,		   0,	0,		/* old male spit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE },
//{	IC_GUN,						60,			TARGETCURS,	0,			0,		37,		26,	0,		   0,	0,		/* tank cannon  */		0,		0,			ITEM_NOT_BUYABLE },
//
//{	IC_GUN,						61,			TARGETCURS,		0,		0,		46,		12,	1,		 500,	5,		/* dart gun */				0,		+3,			IF_STANDARD_GUN },
//{	IC_THROWING_KNIFE,62,					TARGETCURS,		0,		1,		95,		1,	4,		  50,	0,		/*bloody throw.knife*/0,		+4,			IF_STANDARD_BLADE | ITEM_NOT_BUYABLE },
//{	IC_GUN,						63,			TARGETCURS,		0,		0,		48,		18,	0,		   0,	0,		/* flamethrower */		0,		0,			IF_STANDARD_GUN },
//{	IC_PUNCH,					64,			PUNCHCURS,		0,		1,		85,		30,	0,		  40,	1,		/* Crowbar       */		0,		-4,			ITEM_METAL | ITEM_DAMAGEABLE },
//{	IC_GUN,						65,			TARGETCURS,		0,		0,		45,		40,	0,		10000,	10,		/* rocket rifle */		0,		-5,			IF_TWOHANDED_GUN | ITEM_ELECTRONIC },
//// MADD MARKER
//{	IC_GUN,					66,				TARGETCURS,	CONDBUL,	0,		55,		122,0,		   6200,	10,		/* Barrett */				+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					67,				TARGETCURS,	CONDBUL,	0,		56,		25,	0,		   4100,	9,		/* VAL Silent */				-3,		-3,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					68,				TARGETCURS,	CONDBUL,	0,		57,		80,	0,		   2700,	8,		/* PSG */				+4,		+4,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					69,				TARGETCURS,	CONDBUL,	0,		58,		24,	0,		   2100,	8,		/* TAR 21 */				+3,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
////MAX WEAPONS spot
//{	IC_NONE,					0,			INVALIDCURS,	0,				0,		0,	0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////---AMMO---
//{	IC_AMMO,					0,			INVALIDCURS,	0,		1,		32,		2,	8,		  15,	2,		/* CLIP9_15 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					1,			INVALIDCURS,	0,		1,		35,		3,	4,		  30,	4,		/* CLIP9_30 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					2,			INVALIDCURS,	0,		1,		33,		2,	8,		  45,	4,		/* CLIP9_15_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					3,			INVALIDCURS,	0,		1,		36,		3,	4,		  90,	6,		/* CLIP9_30_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					4,			INVALIDCURS,	0,		1,		34,		2,	8,		  30,	3,		/* CLIP9_15_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					5,			INVALIDCURS,	0,		1,		37,		3,	4,		  60,	5,		/* CLIP9_30_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					6,			INVALIDCURS,	0,		1,		24,		1,	8,		   5,	1,		/* CLIP38_6 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					7,			INVALIDCURS,	0,		1,		25,		1,	8,		  15,	3,		/* CLIP38_6_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					8,			INVALIDCURS,	0,		1,		26,		1,	8,		  10,	2,		/* CLIP38_6_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					9,			INVALIDCURS,	0,		1,		14,		2,	8,		  10,	2,		/* CLIP45_7 */				0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					10,			INVALIDCURS,	0,		1,		 4,		10,	4,		  45,	3,		/* CLIP45_30 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					11,			INVALIDCURS,	0,		1,		15,		2,	8,		  45,	4,		/* CLIP45_7_AP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					12,			INVALIDCURS,	0,		1,		 5,		10,	4,		 135,	5,		/* CLIP45_30_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					13,			INVALIDCURS,	0,		1,		16,		2,	8,		  30,	3,		/* CLIP45_7_HP */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					14,			INVALIDCURS,	0,		1,		 6,		10,	4,		  90,	4,		/* CLIP45_30_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					15,			INVALIDCURS,	0,		1,		11,		1,	8,		  10,	1,		/* CLIP357_6 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					16,			INVALIDCURS,	0,		1,		17,		3,	8,			15,	1,		/* CLIP357_9 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					17,			INVALIDCURS,	0,		1,		12,		1,	8,		  30,	3,		/* CLIP357_6_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					18,			INVALIDCURS,	0,		1,		18,		3,	8,		  45,	3,		/* CLIP357_9_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					19,			INVALIDCURS,	0,		1,		13,		1,	8,		  20,	2,		/* CLIP357_6_HP */		0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					20,			INVALIDCURS,	0,		1,		19,		3,	8,		  30,	2,		/* CLIP357_9_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					21,			INVALIDCURS,	0,		1,		 9,		6,	4,		 150,	5,		/* CLIP545_30_AP */		0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					22,			INVALIDCURS,	0,		1,		10,		6,	4,		 100,	4,		/* CLIP545_30_HP */		0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					23,			INVALIDCURS,	0,		1,		 7,		5,	4,		 150,	4,		/* CLIP556_30_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					24,			INVALIDCURS,	0,		1,		 8,		5,	4,		 100,	3,		/* CLIP556_30_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					25,			INVALIDCURS,	0,		1,		22,		3,	6,		  60,	6,		/* CLIP762W_10_AP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					26,			INVALIDCURS,	0,		1,		29,		8,	4,		 180,	4,		/* CLIP762W_30_AP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					27,			INVALIDCURS,	0,		1,		23,		3,	6,		  40,	5,		/* CLIP762W_10_HP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					28,			INVALIDCURS,	0,		1,		30,		8,	4,		 120,	3,		/* CLIP762W_30_HP */	0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					29,			INVALIDCURS,	0,		1,		 3,		1,	6,		  30,	7,		/* CLIP762N_5_AP */		0,		0,			IF_STANDARD_CLIP},
//
//{	IC_AMMO,					30,			INVALIDCURS,	0,		1,		27,		8,	4,		 120,	6,		/* CLIP762N_20_AP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					31,			INVALIDCURS,	0,		1,		 2,		1,	6,		  20,	6,		/* CLIP762N_5_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					32,			INVALIDCURS,	0,		1,		28,		8,	4,		  80,	5,		/* CLIP762N_20_HP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					33,			INVALIDCURS,	0,		1,		31,		5,	4,		 700,	8,		/* CLIP47_50_SAP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					34,			INVALIDCURS,	0,		1,		20,		9 ,	4,		 750,	9,		/* CLIP57_50_SAP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					35,			INVALIDCURS,	0,		1,		21,		9,	4,		 500,	9,		/* CLIP57_50_HP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					36,			INVALIDCURS,	0,		2,		22,		5,	6,		  20,	3,		/* CLIP12G_7 */				0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					37,			INVALIDCURS,	0,		2,		 4,		5,	6,		  20,	3,		/* CLIP12G_7_BUCKSHOT */	0,		0,	IF_STANDARD_CLIP},
//{	IC_AMMO,					38,			INVALIDCURS,	0,		1,		 0,		10,	6,		 300,	9,		/* CLIPCAWS_10_SAP */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					39,			INVALIDCURS,	0,		1,		 1,		10,	6,		 300,	9,		/* CLIPCAWS_10_FLECH */		0,		0,	IF_STANDARD_CLIP},
//
//{	IC_AMMO,					40,			INVALIDCURS,	0,		1,		110,	10,	4,		 500,	9,		/* CLIPROCKET_AP */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					41,			INVALIDCURS,	0,		1,		115,	10,	4,		 500,	9,		/* CLIPROCKET_HE */		0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					42,			INVALIDCURS,	0,		1,		114,	10,	4,		 500,	9,		/* CLIPROCKET_HEAT */	0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					43,			INVALIDCURS,	0,		1,		119,	1,	8,		  10,	4,		/* sleep dart */			0,		0,			IF_STANDARD_CLIP},
//{	IC_AMMO,					44,			INVALIDCURS,	0,		0,		49,		8,	4,		   0,	0,		/* flameThrwr clip */	0,		0,			IF_STANDARD_CLIP },
//// MADD MARKER
//{	IC_AMMO,					45,			INVALIDCURS,	0,		1,		138,	9,	4,		   300,	10,		/* CLIP50_11 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					46,			INVALIDCURS,	0,		1,		139,	4,	4,		   50,	9,		/* CLIP9H_20 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					47,			INVALIDCURS,	0,		1,		140,	8,	4,		   50,	6,		/* CLIP9_50 */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					48,			INVALIDCURS,	0,		1,		141,	8,	4,		   50,	7,		/* CLIP9_50_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					49,			INVALIDCURS,	0,		1,		142,	8,	4,		   60,	7,		/* CLIP9_50_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//
//{	IC_AMMO,					50,			INVALIDCURS,	0,		1,		143,	15,	1,		   375,	9,		/* DRUM545_75_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					51,			INVALIDCURS,	0,		1,		144,	15,	1,		   250,	9,		/* DRUM545_75_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					52,			INVALIDCURS,	0,		1,		145,	30,	1,		   1000,9,		/* BELT556_200_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					53,			INVALIDCURS,	0,		1,		146,	30,	1,		   650,	9,		/* BELT556_200_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					54,			INVALIDCURS,	0,		1,		153,	40,	1,		   600, 9,		/* BELT762N_100_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					55,			INVALIDCURS,	0,		1,		154,	40,	1,		   400,	9,		/* BELT762N_100_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					56,			INVALIDCURS,	0,		1,		147,	3,	8,		   300,	6,		/* CLIP57_20_AP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
//{	IC_AMMO,					57,			INVALIDCURS,	0,		1,		148,	3,	8,		   200,	6,		/* CLIP57_20_HP */				0,		0,			IF_STANDARD_CLIP | ITEM_BIGGUNLIST },
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,					0,			INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////---EXPLOSIVES---
//
//{	IC_GRENADE,	 			0,				TOSSCURS,		0,		1,		38,		6,	4,		 100,	5,		/* stun grenade				*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE },
//{	IC_GRENADE,	 			1,				TOSSCURS,		0,		1,		48,		6,	4,		 120,	4,		/* tear gas grenade   */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			2,				TOSSCURS,		0,		1,		41,		6,	4,		 500,	7, 		/* mustard gas grenade*/	0,		-3,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			3,				TOSSCURS,		0,		1,		50,		3,	6,		 150,	5,		/* mini hand grenade  */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,	 			4,				TOSSCURS,		0,		1,		49,		6,	4,		 200,	6,		/* reg hand grenade   */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			5,			INVALIDCURS,	0,		2,		 3,		11,	2,		 400,	7,		/* RDX                */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			6,			INVALIDCURS,	0,		2,		 0,		11,	1,		 500,	4,		/* TNT (="explosives")*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			7,			INVALIDCURS,	0,		2,		23,		11,	1,		1000,	8,		/* HMX (=RDX+TNT)     */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			8,			INVALIDCURS,	0,		1,		45,		11,	1,		 750,	7,		/* C1  (=RDX+min oil) */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,		 			9,			INVALIDCURS,	0,		1,		40,		41,	2,		 400,	9,		/* mortar shell       */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//
//{	IC_BOMB,					10,			BOMBCURS,		0,		1,		46,		8,	1,		 300,	5,		/* mine               */	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE },
//{	IC_BOMB,					11,			INVALIDCURS,	0,		1,		44,		11,	1,		1500,	9,		/* C4  ("plastique")  */	0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_BOMB,					12,			BOMBCURS,		0,		1,		42,		4,	2,		   0,	0,		/* trip flare				  */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_REPAIRABLE },
//{	IC_BOMB,					13,			BOMBCURS,		0,		1,		43,		4,	2,		   0,	0,		/* trip klaxon        */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_REPAIRABLE },
//{	IC_BOMB,					14,			INVALIDCURS,	0,		1,		107,	2,	4,		 250,	5,		/* shaped charge			*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				15,				TOSSCURS,		0,		2,		24,		1,	6,		  50,	3,		/* break light (flare)*/	0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE},
//{	IC_GRENADE,				16,				INVALIDCURS,	0,		1,		97,		10,	4,		 400,	8,		/* 40mm HE grenade		*/	0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				17,				INVALIDCURS,	0,		1,		111,	10,	4,		 250,	6,		/* 40mm tear gas grnd */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				18,				INVALIDCURS,	0,		1,		113,	10,	4,		 200,	5,		/* 40mm stun grenade  */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				19,				INVALIDCURS,	0,		1,		112,	10,	4,		 100,	7,		/* 40mm smoke grenade */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//
//{	IC_GRENADE,				20,				TOSSCURS,		0,		1,		98,		6,	4,		  50,	3,		/* smoke hand grenade */	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_BOMB,					21,			INVALIDCURS,	0,		1,		40,		41,	8,		 450,	0,		/* tank shell       */		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR },
//{	IC_BOMB,					22,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake struct ignite*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,				23,				TOSSCURS,		0,		2,		37,		6,	4,		  50,	0,		/* creature cocktail*/		0,		0,			ITEM_DAMAGEABLE | ITEM_METAL },
//{	IC_BOMB,					24,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake struct xplod*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_BOMB,					25,			INVALIDCURS,	0,		1,		40,		41,	2,		 450,	0,		/* fake vehicle xplod*/		0,		-4,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,	 			26,				TOSSCURS,		0,		1,		48,		6,	4,		   0,	0,		/* BIG tear gas grenade*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE | ITEM_NOT_BUYABLE },
//{	IC_GRENADE,				27,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* small creature gas */	0,		0,			0},
//{	IC_GRENADE,				28,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* big creature gas */		0,		0,			0},
//{	IC_GRENADE,				29,				INVALIDCURS,	0,		0,		0,		0,	0,		   0,	0,		/* very sm creat gas */	  0,		0,			0},
//
////---ARMOUR---
//{ IC_ARMOUR,			 0,				INVALIDCURS,	COND,		1,		66,		20,	0,		 300, 2,		/* Flak jacket     */ 0,		+2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 1,				INVALIDCURS,	COND,		2,		18,		22,	0,		 350, 0,		/* Flak jacket w X */ 0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 2,				INVALIDCURS,	COND,		2,		11,		18,	0,		 400, 0,		/* Flak jacket w Y */ 0,		+3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 3,				INVALIDCURS,	COND,		1,		64,		32,	0,		 500, 4,		/* Kevlar jacket   */ 0,		0,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 4,				INVALIDCURS,	COND,		2,		16,		35,	0,		 600, 0,		/* Kevlar jack w X */ 0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 5,				INVALIDCURS,	COND,		2,		 9,		29,	0,		 700, 0,		/* Kevlar jack w Y */ 0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 6,				INVALIDCURS,	COND,		1,		65,		32,	0,		1000, 8,		/* Spectra jacket  */ 0,		-2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			 7,				INVALIDCURS,	COND,		2,		17,		35,	0,		1100, 0,		/* Spectra jack w X*/ 0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 8,				INVALIDCURS,	COND,		2,		10,		29,	0,		1200, 0,		/* Spectra jack w Y*/ 0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			 9,				INVALIDCURS,	COND,		1,		67,		39,	0,		 650,	5,		/* Kevlar leggings */	0,		0,			IF_STANDARD_ARMOUR},
//
//{ IC_ARMOUR,			10,				INVALIDCURS,	COND,		2,		19,		43,	0,		 800,	0,		/* Kevlar legs w X */	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			11,				INVALIDCURS,	COND,		2,		12,		35,	0,		 950,	0,		/* Kevlar legs w Y */	0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			12,				INVALIDCURS,	COND,		1,		68,		39,	0,		 900,	8,		/* Spectra leggings*/	0,		-2,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			13,				INVALIDCURS,	COND,		2,		20,		43,	0,		1100,	0,		/* Spectra legs w X*/	0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			14,				INVALIDCURS,	COND,		2,		13,		35,	0,		1300,	0,		/* Spectra legs w Y*/	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			15,				INVALIDCURS,	COND,		1,		61,		14,	0,		  50,	2,		/* Steel helmet    */	0,		+2,			IF_STANDARD_ARMOUR | ITEM_METAL},
//{ IC_ARMOUR,			16,				INVALIDCURS,	COND,		1,		63,		14,	0,		 200,	4,		/* Kevlar helmet   */	0,		0,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			17,				INVALIDCURS,	COND,		2,		15,		15,	0,		 250,	0,		/* Kevlar helm w X */	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			18,				INVALIDCURS,	COND,		2,		 8,		13,	0,		 300,	0,		/* Kevlar helm w Y */	0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			19,				INVALIDCURS,	COND,		1,		62,		14,	0,		 450,	7,		/* Spectra helmet  */	0,		-2,			IF_STANDARD_ARMOUR},
//	
//{ IC_ARMOUR,			20,				INVALIDCURS,	COND,		2,		14,		15,	0,		 550,	0,		/* Spectra helm w X*/	0,		-3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			21,				INVALIDCURS,	COND,		2,		 7,		13,	0,		 650,	0,		/* Spectra helm w Y*/	0,		-1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			22,				INVALIDCURS,	COND,		1,		81,		12,	2,		 250,	5,		/* Ceramic plates  */ 0,		-4,			(IF_STANDARD_ARMOUR | ITEM_ATTACHMENT) & (~ITEM_REPAIRABLE) },
//{ IC_ARMOUR,			23,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Infant crt hide */ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			24,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Yng male hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			25,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Old male hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			26,				INVALIDCURS,	COND,		1,		0,		0,	0,			 0,	0,		/* Queen cret hide */ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{ IC_ARMOUR,			27,				INVALIDCURS,	COND,		1,		96,		20,	0,		 200,	2,		/* Leather jacket  */ 0,		+4,			IF_STANDARD_ARMOUR },
//// NOTE: THE FOLLOWING ITEM'S PRICE VALUE IS IN DIALOGUE AND SHOULD NOT BE CHANGED
//{ IC_ARMOUR,			28,				INVALIDCURS,	COND,		1,		116,	20,	0,		 950,	3,		/* L jacket w kev  */ 0,		+2,			IF_STANDARD_ARMOUR },
//{ IC_ARMOUR,			29,				INVALIDCURS,	COND,		1,		117,	20,	0,		1200,	0,		/* L jacket w kev 18*/0,		+1,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//
//{ IC_ARMOUR,			30,				INVALIDCURS,	COND,		1,		118,	20,	0,		1500,	0,		/* L jacket w kev c*/ 0,		+3,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{	IC_ARMOUR,			31,				INVALIDCURS,	COND,		1,		0,		0,	0,		   0,	0,		/* yng fem hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{	IC_ARMOUR,			32,				INVALIDCURS,	COND,		1,		0,		0,	0,		   0,	0,		/* old fem hide */		0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE | ITEM_NOT_EDITOR | ITEM_DEFAULT_UNDROPPABLE},
//{	IC_ARMOUR,			33,				INVALIDCURS,	COND,		2,		25,		3,	1,		  10,	1,		/* t-shirt */					0,		0,			ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_UNAERODYNAMIC},
//{	IC_ARMOUR,			33,				INVALIDCURS,	COND,		2,		34,		3,	1,		  10,	1,		/* t-shirt D. rules*/	0,		0,			ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_UNAERODYNAMIC},
//{ IC_ARMOUR,			34,				INVALIDCURS,	COND,		1,		137,		32,	0,		 700, 6,		/* Kevlar2 jacket  */ 0,		-1,			IF_STANDARD_ARMOUR},
//{ IC_ARMOUR,			35,				INVALIDCURS,	COND,		2,		40,		35,	0,		 800, 0,		/* Kevlar2 jack w X*/ 0,		-2,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{ IC_ARMOUR,			36,				INVALIDCURS,	COND,		2,		41,		29,	0,		 900, 0,		/* Kevlar2 jack w Y*/ 0,		0,			IF_STANDARD_ARMOUR | ITEM_NOT_BUYABLE },
//{	IC_NONE,				0,			INVALIDCURS,	0,			0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,			0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////---MISC---
//{ IC_MEDKIT,			0,				AIDCURS,		USAGE,		1,		73,		5,	4,		 100,	1,		/* First aid kit */		0,		0,			IF_STANDARD_KIT},
//{ IC_MEDKIT,			0,				AIDCURS,		USAGE,		1,		86,		18,	0,		 300,	1,		/* Medical Kit   */		0,		0,			IF_STANDARD_KIT | ITEM_METAL},
//{ IC_KIT,					0,			REPAIRCURS,		COND,			2,		21,		50, 0,		 250,	1,		/* Tool Kit	     */		0,		0,			IF_STANDARD_KIT | ITEM_METAL},
//{ IC_KIT,					0,			INVALIDCURS,	COND,			1,		78,		3,	1,		 250,	3,	  /* Locksmith kit */		0,		-2,			IF_STANDARD_KIT | ITEM_METAL | ITEM_REPAIRABLE},
//{ IC_KIT,					0,			INVALIDCURS,	COND,			1,		58,		1,	4,		 250,	5,	  /* Camouflage kit*/		0,		0,			IF_STANDARD_KIT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		52,		5,	4,		 300,	4,		/* Silencer      */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		76,		9,	4,		 500,	5,		/* Sniper scope  */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		69,		5,	2,		  50,	3,		/* Bipod         */		0,		+5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		77,		9,	1,		 400,	7,		/* Extended ear	 */		0,		-3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		74,		9,	1,		 800,	7,		/* Night goggles */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		55,		2,	4,		 150,	3,		/* Sun goggles	 */		0,		+3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE },
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,	  75,		9,	1,		 100,	4,		/* Gas mask   	 */		0,		+1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE },
//{	IC_KIT,					0,			INVALIDCURS,	0,				2,		 5,	 10,	4,		  10,	1,		/* Canteen       */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		80,	 10,	1,		 200,	4,		/* Metal detector*/		0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		 6,		1,	4,		 900,	7,		/* Compound 18	 */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		42,		1,	4,		2500,	0,		/* Jar/QueenBlood*/		0,		0,			ITEM_DAMAGEABLE | ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		 2,		1,	4,		 500,	1,		/* Jar/Elixir    */		0,		0,			ITEM_DAMAGEABLE },
//{	IC_MONEY,				0,			INVALIDCURS,	0,				2,		 1,		1,	1,		   0,	0,		/* Money         */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				JARCURS,			COND,			1,		71,		1,	2,			10, 1,		/* Glass jar		 */		0,		0,			ITEM_DAMAGEABLE},
//
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		72,		5,	2,			50, 1,		/* Jar/CreatureBlood*/0,		0,			ITEM_DAMAGEABLE | ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		1,	8,		 150,	4,		/* Adren Booster */		0,		0,			ITEM_DAMAGEABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		47,		1,	4,		 100, 3,		/* Detonator     */		0,		+1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ATTACHMENT | ITEM_METAL},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		47,		1,	4,		 200, 6,		/* Rem Detonator */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ATTACHMENT | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		93,		1,	8,		   0,	0,		/* Videotape     */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		94,		1,	8,		   0,	0,		/* Deed          */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		56,		1,	1,		   0,	0,		/* Letter				 */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		59,		1,	1,		   0,	0,		/* Diskette	  	 */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	124,		0,	1,		3000,	0,		/* Chalice	     */		0,		0,			ITEM_NOT_BUYABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	120,		1,	4,		  50,	0,		/* Bloodcat claws*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	121,		1,	4,		 100,	0,		/* Bloodcat teeth*/		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		9,	 60,	0,		 400, 0,		/* Bloodcat pelt */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		54,		0,	99,		   0,	0,		/* Switch        */		0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		47,		0,	99,		   0,	0,		/* Action item   */		0,		0,			ITEM_NOT_BUYABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		1,	6,		 300,	6,		/* Regen Booster */		0,		0,			ITEM_DAMAGEABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 3     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 4     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		70,		0,	99,		   0,	0,		/* syringe 5     */		0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{ IC_MISC,				0,				INVALIDCURS,	COND,			1,		72,		5,	2,			10, 1,		/* Jar/Human Blood*/	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		0,		0,	0,		   0,	0,		/* ownership     */		0,		0,			ITEM_NOT_BUYABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		51,		4,	4,		 750,	7,		/* Laser scope   */		0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			REMOTECURS,		0,				1,		54,		9,	4,		 400,	6,		/* Remote bomb trig*/	0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			WIRECUTCURS,	0,				1,		88,		4,	2,		  20,	2,		/* Wirecutters   */		0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		90,		9,	4,		  30,	2,		/* Duckbill      */		0,		+5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		106,	20,	1,		  30,	1,		/* Alcohol  */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_FACE,				0,			INVALIDCURS,	0,				1,		74,		11,	1,		1500,	9,		/* UV goggles */			0,		-1,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		44,		21,	0,		  30,	0,		/* discarded LAW*/		0,		0,			IF_TWOHANDED_GUN | ITEM_NOT_BUYABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		0,		40,	0,		   0,	0,		/* head - generic */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		1,		40,	0,		   0,	0,		/* head - Imposter*/	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		2,		40,	0,		   0,	0,		/* head - T-Rex */		0,		0,			ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		3,		40,	0,		   0,	0,		/* head - Slay */			0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		4,		40,	0,		   0,	0,		/* head - Druggist */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		5,		40,	0,		   0,	0,		/* head - Matron */		0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		6,		40,	0,		   0,	0,		/* head - Tiffany */	0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		100,	12,	1,		  20,	1,		/* wine     */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		101,	4,	4,		  10,	1,		/* beer		  */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		99,		0,	2,		  20,	3,		/* pornos   */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		43,		20,	0,		 900,	6,		/* video camera */		0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC  },
//{	IC_FACE,				0,			INVALIDCURS,	0,				0,		42,		5,	1,		2500,	0,		/* robot remote */		0,		-5,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC  },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		103, 20,	0,		 500,	0,		/* creature claws */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		7,	 40,	0,		 250,	0,		/* creature flesh */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		104, 10,	0,		1000,	0,		/* creature organ */	0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			REMOTECURS,		0,				1,		54,		9,	4,		 400,	6,		/* Remote trigger*/		0,		-2,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				0,		47,		2,	8,		 500,	2,		/* gold watch */			0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		136, 100,	0,		 200,	2,		/* golf clubs */			0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL},
//{	IC_FACE,				0,			INVALIDCURS,	0,				3,		11,		5,	1,		 100,	1,		/* walkman */					0,		-4,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		8,	 50,	0,		 300,	2,		/* portable tv */			0,		-3,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC },
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_MISC,				0,			INVALIDCURS,	0,				3,		10,	 10,	1,		  30,	1,		/* cigars */					0,		0,			ITEM_DAMAGEABLE },
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
//{	IC_KEY,					0,			INVALIDCURS,	0,				1,		82,		1,	8,		   0,	0,		/* dull gold key */		0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					1,			INVALIDCURS,	0,				1,		83,		1,	8,		   0,	0,		/* silver key */			0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					2,			INVALIDCURS,	0,				1,		84,		1,	8,		   0,	0,		/* diamond-shpd key */0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					3,			INVALIDCURS,	0,				1,		87,		1,	8,		   0,	0,		/* bright gold key */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					4,			INVALIDCURS,	0,				1,		91,		1,	8,		   0,	0,		/* gold key */				0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					5,			INVALIDCURS,	0,				1,		92,		1,	8,		   0,	0,		/* small gold key */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					6,			INVALIDCURS,	0,				1,		108,	1,	8,		   0,	0,		/* electronic */			0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL | ITEM_ELECTRONIC },
//{	IC_KEY,					7,			INVALIDCURS,	0,				1,		109,	1,	8,		   0,	0,		/* passcard       */	0,		0,			ITEM_NOT_BUYABLE | ITEM_METAL},
//{	IC_KEY,					8,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					9,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		149,	4,	4,		 100,	3,		/* Flash Suppressor */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_LAUNCHER,			282,		TARGETCURS,		0,				0,		68,		79,	0,		 1500,	10,		/* rpg 7 */					0,		-3,				IF_TWOHANDED_GUN },
//{	IC_GRENADE,				30,			INVALIDCURS,	0,				1,		150,	21,	0,		 1200,	10,		/* HE rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				31,			INVALIDCURS,	0,				1,		152,	21,	0,		 2000,	10,		/* AP rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_GRENADE,				32,			INVALIDCURS,	0,				1,		151,	21,	0,		 1000,	10,		/* Frag rpg ammo	*/	0,		0,					ITEM_DAMAGEABLE | ITEM_METAL | ITEM_REPAIRABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		155,	4,	4,		 1200,	6,		/* Reflex scoped */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,		156,	2,	4,		 1200,	6,		/* Reflex unscoped */		0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ATTACHMENT},
////{	IC_KEY,					10,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					11,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					12,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					13,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					14,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					15,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
////{	IC_KEY,					16,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					17,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					18,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					19,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_KEY,					20,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					21,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					22,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					23,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					24,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					25,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					26,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					27,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					28,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					29,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//
//{	IC_KEY,					30,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_KEY,					31,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* key      */				0,		0,			ITEM_NOT_EDITOR | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		31,		4,	0,		 150,	2,		/* silver platter */	0,		0,			ITEM_DAMAGEABLE | ITEM_METAL},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		30,		1,	6,		   5,	1,		/* duct tape */				0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON },
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		28,		3,	1,		  20,	0,		/* aluminum rod */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		32,		1,	8,		   0,	0,		/* spring */					0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_UNAERODYNAMIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		33,		4,	1,		  25,	0,		/* a. rod & spring */	0,		0,			ITEM_NOT_BUYABLE | ITEM_REPAIRABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_INSEPARABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 133,		4,	1,		  20,	0,		/* steel rod */				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 132,		2,	6,		   5,	3,		/* quick glue */			0,		0,			ITEM_DAMAGEABLE  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	 134,		6,	1,		 150,	0,		/* gun barrel xtndr */0,		0,			ITEM_NOT_BUYABLE | ITEM_REPAIRABLE | ITEM_DAMAGEABLE | ITEM_METAL | ITEM_INSEPARABLE},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		35,		1,	8,		   0,	0,		/* string */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,		27,		1,	1,		   0,	0,		/* tin can */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC  | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			TINCANCURS,		0,				2,		36,		2,	4,		   0,	0,		/* string & tin can */0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	122,		3,	6,		   5,	0,		/* marbles */					0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	127,		6,	1,		 200,	6,		/* lame boy */				0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	128,		1,	8,		   5,	1,		/* copper wire */			0,		0,			ITEM_METAL | ITEM_HIDDEN_ADDON },
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	126,		7,	1,		  50,	0,		/* display unit */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	130,		1,	1,		 100,	5,		/* fumble pak */			0,		0,			ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_ELECTRONIC},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	131,		1,	2,		  10,	5,		/* xray bulb */				0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,	 29,		1,	8,		   1,	1,		/* chewing gum */			0,		0,			ITEM_DAMAGEABLE | ITEM_HIDDEN_ADDON},
//
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	129, 		3,	1,		 100,	0,		/* flash device */		0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC | ITEM_HIDDEN_ADDON},
//{	IC_MISC,				0,			INVALIDCURS,	0,				2,	 26,		1,	6,		   5,	1,		/* batteries */				0,		0,			ITEM_DAMAGEABLE},
//{	IC_MISC,				0,			INVALIDCURS,	0,				1,	123,		1,	8,		   0,	0,		/* elastic */					0,		0,			ITEM_NOT_BUYABLE | ITEM_UNAERODYNAMIC},
//{	IC_MISC,				0,			REMOTECURS,		0,				1,	125,	 10,	1,		2500,	0,		/* xray device */			0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_METAL | ITEM_ELECTRONIC},
//{	IC_MONEY,				0,			INVALIDCURS,	0,				2,	 38,		2,	1,		 100,	0,		/* silver   */				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_MONEY,				0,			INVALIDCURS,	0,				1,	135,		2,	1,		 300,	0,		/* gold			*/				0,		0,			ITEM_NOT_BUYABLE | ITEM_DAMAGEABLE},
//{	IC_KIT,					0,			REFUELCURS,	  0,				2,	 39,	 20,	0,		 250,	0,		/* gas can */					0,		0,			ITEM_DAMAGEABLE},
//
//{	IC_GUN,					328,			TARGETCURS,	CONDBUL,	0,		50,		22,	0,		900,	8,		/* M900         */	+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					329,			TARGETCURS,	CONDBUL,	0,		51,		10,	0,		400,	6,		/* M950         */	+1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					330,			TARGETCURS,	CONDBUL,	0,		52,		22,	0,		900,	7,		/* M960A         */	+1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST  },
//{	IC_GUN,					331,			TARGETCURS,	CONDBUL,	0,		53,		19,	1,		300,	5,		/* Micro Uzi         */	+1,		-1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					332,			TARGETCURS,	CONDBUL,	0,		54,		38,	0,		1300,	6,		/* Enfield         */	0,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					333,			TARGETCURS,	CONDBUL,	0,		59,		25,	0,		900,	4,		/* MP5A2         */	0,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					334,			TARGETCURS,	CONDBUL,	0,		60,		29,	0,		1000,	4,		/* MP5SD         */	0,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					335,			TARGETCURS,	CONDBUL,	0,		61,		29,	0,		1600,	7,		/* MP5N         */	+2,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					336,			TARGETCURS,	CONDBUL,	0,		62,		22,	0,		1300,	6,		/* UMP45         */	0,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					337,			TARGETCURS,	CONDBUL,	0,		63,		6,	1,		1000,	7,		/* FIVE7         */	+1,		0,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					338,			TARGETCURS,	CONDBUL,	0,		64,		7,	1,		400,	4,		/* p7m8         */	+2,		+1,			IF_STANDARD_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					339,			TARGETCURS,	CONDBUL,	0,		65,		33,	0,		2500,	9,		/* g36k         */	+3,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					340,			TARGETCURS,	CONDBUL,	0,		66,		28,	0,		2400,	9,		/* g36c         */	+3,		0,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					341,			TARGETCURS,	CONDBUL,	0,		67,		64,	0,		3000,	10,		/* MSG90A1 */			+4,		+4,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					342,			TARGETCURS,	CONDBUL,	0,		69,		32,	0,		1400,	7,		/* BENNELLI */			+2,		+1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//{	IC_GUN,					343,			TARGETCURS,	CONDBUL,	0,		70,		34,	0,		2200,	8,		/* AK103 */			-1,		-1,			IF_TWOHANDED_GUN | ITEM_BIGGUNLIST },
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
////{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//{	IC_NONE,				0,			INVALIDCURS,	0,				0,		0,		0,	0,		   0,	0,		/* nothing! */				0,		0,			0},
//
//};


// NB hack:  if an item appears in this array with an item class of IC_MISC,
// it is a slot used for noting the skill check required for a merge or multi-item attachment

AttachmentInfoStruct AttachmentInfo[MAXITEMS+1];// =
//{
//	{ SILENCER,									IC_GUN,			NO_CHECK,																			0 },
//	{ SNIPERSCOPE,							IC_GUN,			NO_CHECK,																			0 },
//	{ LASERSCOPE,								IC_GUN,			NO_CHECK,																			0 },
//	{ BIPOD,										IC_GUN,			NO_CHECK,																			0 },
//	{ UNDER_GLAUNCHER,					IC_GUN,			NO_CHECK,																			0 },
//	{ DUCKBILL,									IC_GUN,			NO_CHECK,																			0 },
//	{	SPRING_AND_BOLT_UPGRADE,	IC_GUN,			ATTACHING_SPECIAL_ITEM_CHECK,									0 },
//	{ GUN_BARREL_EXTENDER,			IC_GUN,			ATTACHING_SPECIAL_ITEM_CHECK,								  0 },
//	{ DETONATOR,								IC_BOMB,		ATTACHING_DETONATOR_CHECK,										0 },
//	{ REMDETONATOR,							IC_BOMB,		ATTACHING_REMOTE_DETONATOR_CHECK,						-10 },
//	{ REMDETONATOR,							IC_BOMB,		ATTACHING_REMOTE_DETONATOR_CHECK,						-10 },
//	{ XRAY_BULB,								IC_NONE,		ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,		-15 },
//	{ COPPER_WIRE,							IC_NONE,		ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,		+20 },
//	{ CERAMIC_PLATES,						IC_ARMOUR,	NO_CHECK,																			0 },
//	{ FLASH_SUPPRESSOR,									IC_GUN,			NO_CHECK,																			0 },
//	{ REFLEX_SCOPED,									IC_GUN,			NO_CHECK,																			0 },
//	{ REFLEX_UNSCOPED,									IC_GUN,			NO_CHECK,																			0 },
//	{ 0,												0 }
//};

UINT16 Attachment[MAXATTACHMENTS][3];// =
//{
//	{SILENCER, GLOCK_17},
//	{SILENCER, GLOCK_18},
//	{SILENCER, BERETTA_92F},
//	{SILENCER, BERETTA_93R},
//	{SILENCER, SW38},
//	{SILENCER, BARRACUDA},
//	{SILENCER, DESERTEAGLE},
//	{SILENCER, M1911},
//	{SILENCER, MP5K},
//	{SILENCER, MAC10},
//	{SILENCER, THOMPSON},
//	{SILENCER, P90},
//	{SILENCER, M960},
//	{SILENCER, MICRO_UZI},
//	{SILENCER, M950},
//	{SILENCER, MP5},
//	{SILENCER, MP5N},
//	{SILENCER, FIVE7},
//	{SILENCER, P7M8},
//	{SILENCER, MSG90A1},
//
//	{SNIPERSCOPE, COMMANDO},
//	{SNIPERSCOPE, SKS},
//	{SNIPERSCOPE, DRAGUNOV},
//	{SNIPERSCOPE, M24},
//	{SNIPERSCOPE, AUG},
//	{SNIPERSCOPE, G41},
//	{SNIPERSCOPE, MINI14},
//	{SNIPERSCOPE, C7},
//	{SNIPERSCOPE, FAMAS},
//	{SNIPERSCOPE, AK74},
//	{SNIPERSCOPE, AKM},
//	{SNIPERSCOPE, M14},
//	{SNIPERSCOPE, FNFAL},
//	{SNIPERSCOPE, G3A3},
//	{SNIPERSCOPE, G11},
//	{SNIPERSCOPE, M900},
//	{SNIPERSCOPE, BARRETT},
//	{SNIPERSCOPE, PSG},
//	{SNIPERSCOPE, VAL_SILENT},
//	{SNIPERSCOPE, TAR21},
//	{SNIPERSCOPE, ENFIELD},
//	{SNIPERSCOPE, G36K},
//	{SNIPERSCOPE, G36C},
//	{SNIPERSCOPE, MSG90A1},
//	{SNIPERSCOPE, AK103},
//
//	{LASERSCOPE, GLOCK_17},
//	{LASERSCOPE, GLOCK_18},
//	{LASERSCOPE, BERETTA_92F},
//	{LASERSCOPE, BERETTA_93R},
//	{LASERSCOPE, SW38},
//	{LASERSCOPE, BARRACUDA},
//	{LASERSCOPE, DESERTEAGLE},
//	{LASERSCOPE, M1911},
//	{LASERSCOPE, MP5K},
//	{LASERSCOPE, MAC10},
//	{LASERSCOPE, THOMPSON},
//	{LASERSCOPE, COMMANDO},
//	{LASERSCOPE, MP53},
//	{LASERSCOPE, AKSU74},
//	{LASERSCOPE, P90},
//	{LASERSCOPE, TYPE85},
//	{LASERSCOPE, SKS},
//	{LASERSCOPE, DRAGUNOV},
//	{LASERSCOPE, M24},
//	{LASERSCOPE, AUG},
//	{LASERSCOPE, G41},
//	{LASERSCOPE, MINI14},
//	{LASERSCOPE, C7},
//	{LASERSCOPE, FAMAS},
//	{LASERSCOPE, AK74},
//	{LASERSCOPE, AKM},
//	{LASERSCOPE, M14},
//	{LASERSCOPE, FNFAL},
//	{LASERSCOPE, G3A3},
//	{LASERSCOPE, G11},
//	{LASERSCOPE, M870},
//	{LASERSCOPE, SPAS15},
//	{LASERSCOPE, CAWS},
//	{LASERSCOPE, MINIMI},
//	{LASERSCOPE, RPK74},
//	{LASERSCOPE, HK21E},
//	{LASERSCOPE, AUTOMAG_III},
//	{LASERSCOPE, MICRO_UZI},
//	{LASERSCOPE, M900},
//	{LASERSCOPE, M950},
//	{LASERSCOPE, M960},
//	{LASERSCOPE, BARRETT},
//	{LASERSCOPE, PSG},
//	{LASERSCOPE, VAL_SILENT},
//	{LASERSCOPE, TAR21},
//	{LASERSCOPE, ENFIELD},
//	{LASERSCOPE, MP5},
//	{LASERSCOPE, MP5N},
//	{LASERSCOPE, UMP45},
//	{LASERSCOPE, MP5SD},
//	{LASERSCOPE, FIVE7},
//	{LASERSCOPE, P7M8},
//	{LASERSCOPE, G36K},
//	{LASERSCOPE, G36C},
//	{LASERSCOPE, MSG90A1},
//	{LASERSCOPE, BENNELLI},
//	{LASERSCOPE, AK103},
//
//	{BIPOD,	SKS},
//	{BIPOD,	DRAGUNOV},
//	{BIPOD,	M24},
//	{BIPOD,	AUG},
//	{BIPOD,	G41},
//	{BIPOD,	MINI14},
//	{BIPOD,	C7},
//	{BIPOD,	FAMAS},
//	{BIPOD,	AK74},
//	{BIPOD,	AKM},
//	{BIPOD,	M14},
//	{BIPOD,	FNFAL},
//	{BIPOD,	G3A3},
//	{BIPOD,	G11},
//	{BIPOD,	CAWS},
//	{BIPOD,	MINIMI},
//	{BIPOD,	RPK74},
//	{BIPOD,	HK21E},
//	{BIPOD, M900},
//	{BIPOD, BARRETT},
//	{BIPOD, PSG},
//	{BIPOD, VAL_SILENT},
//	{BIPOD, TAR21},
//	{BIPOD, ENFIELD},
//	{BIPOD, MSG90A1},
//	{BIPOD, AK103},
//
//	{DUCKBILL, M870},
//	{DUCKBILL, SPAS15},
//	{DUCKBILL, CAWS},
//	{DUCKBILL, BENNELLI},
//
//	{UNDER_GLAUNCHER, COMMANDO},
//	{UNDER_GLAUNCHER, AKSU74},
//	{UNDER_GLAUNCHER, AUG},
//	{UNDER_GLAUNCHER, G41},
//	{UNDER_GLAUNCHER, MINI14},
//	{UNDER_GLAUNCHER, C7},
//	{UNDER_GLAUNCHER, FAMAS},
//	{UNDER_GLAUNCHER, AK74},
//	{UNDER_GLAUNCHER, AKM},
//	{UNDER_GLAUNCHER, M14},
//	{UNDER_GLAUNCHER, FNFAL},
//	{UNDER_GLAUNCHER, G3A3},
//	{UNDER_GLAUNCHER, TAR21},
//	{UNDER_GLAUNCHER, ENFIELD},
//	{UNDER_GLAUNCHER, M900},
//	{UNDER_GLAUNCHER, G11},
//	{UNDER_GLAUNCHER, SKS},
//	{UNDER_GLAUNCHER, G36K},
//	{UNDER_GLAUNCHER, G36C},
//	{UNDER_GLAUNCHER, AK103},
//
//	{SPRING_AND_BOLT_UPGRADE, GLOCK_17},
//	{SPRING_AND_BOLT_UPGRADE, GLOCK_18},
//	{SPRING_AND_BOLT_UPGRADE, BERETTA_92F},
//	{SPRING_AND_BOLT_UPGRADE, BERETTA_93R},
//	{SPRING_AND_BOLT_UPGRADE, SW38},
//	{SPRING_AND_BOLT_UPGRADE, BARRACUDA},
//	{SPRING_AND_BOLT_UPGRADE, DESERTEAGLE},
//	{SPRING_AND_BOLT_UPGRADE, M1911},
//	{SPRING_AND_BOLT_UPGRADE, MP5K},
//	{SPRING_AND_BOLT_UPGRADE, MAC10},
//	{SPRING_AND_BOLT_UPGRADE, THOMPSON},
//	{SPRING_AND_BOLT_UPGRADE, COMMANDO},
//	{SPRING_AND_BOLT_UPGRADE, MP53},
//	{SPRING_AND_BOLT_UPGRADE, AKSU74},
//	{SPRING_AND_BOLT_UPGRADE, P90},
//	{SPRING_AND_BOLT_UPGRADE, TYPE85},
//	{SPRING_AND_BOLT_UPGRADE, SKS},
//	{SPRING_AND_BOLT_UPGRADE, DRAGUNOV},
//	{SPRING_AND_BOLT_UPGRADE, M24},
//	{SPRING_AND_BOLT_UPGRADE, AUG},
//	{SPRING_AND_BOLT_UPGRADE, G41},
//	{SPRING_AND_BOLT_UPGRADE, MINI14},
//	{SPRING_AND_BOLT_UPGRADE, C7},
//	{SPRING_AND_BOLT_UPGRADE, FAMAS},
//	{SPRING_AND_BOLT_UPGRADE, AK74},
//	{SPRING_AND_BOLT_UPGRADE, AKM},
//	{SPRING_AND_BOLT_UPGRADE, M14},
//	{SPRING_AND_BOLT_UPGRADE, FNFAL},
//	{SPRING_AND_BOLT_UPGRADE, G3A3},
//	{SPRING_AND_BOLT_UPGRADE, G11},
//	{SPRING_AND_BOLT_UPGRADE, M870},
//	{SPRING_AND_BOLT_UPGRADE, SPAS15},
//	{SPRING_AND_BOLT_UPGRADE, CAWS},
//	{SPRING_AND_BOLT_UPGRADE, MINIMI},
//	{SPRING_AND_BOLT_UPGRADE, RPK74},
//	{SPRING_AND_BOLT_UPGRADE, HK21E},
//	{SPRING_AND_BOLT_UPGRADE, AUTOMAG_III},
//	{SPRING_AND_BOLT_UPGRADE, MICRO_UZI},
//	{SPRING_AND_BOLT_UPGRADE, M900},
//	{SPRING_AND_BOLT_UPGRADE, M950},
//	{SPRING_AND_BOLT_UPGRADE, M960},
//	{SPRING_AND_BOLT_UPGRADE, BARRETT},
//	{SPRING_AND_BOLT_UPGRADE, PSG},
//	{SPRING_AND_BOLT_UPGRADE, VAL_SILENT},
//	{SPRING_AND_BOLT_UPGRADE, TAR21},
//	{SPRING_AND_BOLT_UPGRADE, ENFIELD},
//	{SPRING_AND_BOLT_UPGRADE, MP5},
//	{SPRING_AND_BOLT_UPGRADE, MP5N},
//	{SPRING_AND_BOLT_UPGRADE, UMP45},
//	{SPRING_AND_BOLT_UPGRADE, MP5SD},
//	{SPRING_AND_BOLT_UPGRADE, FIVE7},
//	{SPRING_AND_BOLT_UPGRADE, P7M8},
//	{SPRING_AND_BOLT_UPGRADE, G36K},
//	{SPRING_AND_BOLT_UPGRADE, G36C},
//	{SPRING_AND_BOLT_UPGRADE, MSG90A1},
//	{SPRING_AND_BOLT_UPGRADE, BENNELLI},
//	{SPRING_AND_BOLT_UPGRADE, AK103},
//
//	{GUN_BARREL_EXTENDER, GLOCK_17},
//	{GUN_BARREL_EXTENDER, GLOCK_18},
//	{GUN_BARREL_EXTENDER, BERETTA_92F},
//	{GUN_BARREL_EXTENDER, BERETTA_93R},
//	{GUN_BARREL_EXTENDER, SW38},
//	{GUN_BARREL_EXTENDER, BARRACUDA},
//	{GUN_BARREL_EXTENDER, DESERTEAGLE},
//	{GUN_BARREL_EXTENDER, M1911},
//	{GUN_BARREL_EXTENDER, MP5K},
//	{GUN_BARREL_EXTENDER, MAC10},
//	{GUN_BARREL_EXTENDER, THOMPSON},
//	{GUN_BARREL_EXTENDER, COMMANDO},
//	{GUN_BARREL_EXTENDER, MP53},
//	{GUN_BARREL_EXTENDER, AKSU74},
//	{GUN_BARREL_EXTENDER, P90},
//	{GUN_BARREL_EXTENDER, TYPE85},
//	{GUN_BARREL_EXTENDER, SKS},
//	{GUN_BARREL_EXTENDER, DRAGUNOV},
//	{GUN_BARREL_EXTENDER, M24},
//	{GUN_BARREL_EXTENDER, AUG},
//	{GUN_BARREL_EXTENDER, G41},
//	{GUN_BARREL_EXTENDER, MINI14},
//	{GUN_BARREL_EXTENDER, C7},
//	{GUN_BARREL_EXTENDER, FAMAS},
//	{GUN_BARREL_EXTENDER, AK74},
//	{GUN_BARREL_EXTENDER, AKM},
//	{GUN_BARREL_EXTENDER, M14},
//	{GUN_BARREL_EXTENDER, FNFAL},
//	{GUN_BARREL_EXTENDER, G3A3},
//	{GUN_BARREL_EXTENDER, G11},
//	{GUN_BARREL_EXTENDER, M870},
//	{GUN_BARREL_EXTENDER, SPAS15},
//	{GUN_BARREL_EXTENDER, CAWS},
//	{GUN_BARREL_EXTENDER, MINIMI},
//	{GUN_BARREL_EXTENDER, RPK74},
//	{GUN_BARREL_EXTENDER, HK21E},
//	{GUN_BARREL_EXTENDER, AUTOMAG_III},
//	{GUN_BARREL_EXTENDER, MICRO_UZI},
//	{GUN_BARREL_EXTENDER, M900},
//	{GUN_BARREL_EXTENDER, M950},
//	{GUN_BARREL_EXTENDER, M960},
//	{GUN_BARREL_EXTENDER, BARRETT},
//	{GUN_BARREL_EXTENDER, PSG},
//	{GUN_BARREL_EXTENDER, VAL_SILENT},
//	{GUN_BARREL_EXTENDER, TAR21},
//	{GUN_BARREL_EXTENDER, ENFIELD},
//	{GUN_BARREL_EXTENDER, MP5},
//	{GUN_BARREL_EXTENDER, MP5N},
//	{GUN_BARREL_EXTENDER, UMP45},
//	{GUN_BARREL_EXTENDER, MP5SD},
//	{GUN_BARREL_EXTENDER, FIVE7},
//	{GUN_BARREL_EXTENDER, P7M8},
//	{GUN_BARREL_EXTENDER, G36K},
//	{GUN_BARREL_EXTENDER, G36C},
//	{GUN_BARREL_EXTENDER, MSG90A1},
//	{GUN_BARREL_EXTENDER, BENNELLI},
//	{GUN_BARREL_EXTENDER, AK103},
//
//	{FLASH_SUPPRESSOR, MP5K},
//	{FLASH_SUPPRESSOR, MAC10},
//	{FLASH_SUPPRESSOR, THOMPSON},
//	{FLASH_SUPPRESSOR, COMMANDO},
//	{FLASH_SUPPRESSOR, MP53},
//	{FLASH_SUPPRESSOR, AKSU74},
//	{FLASH_SUPPRESSOR, P90},
//	{FLASH_SUPPRESSOR, TYPE85},
//	{FLASH_SUPPRESSOR, SKS},
//	{FLASH_SUPPRESSOR, DRAGUNOV},
//	{FLASH_SUPPRESSOR, M24},
//	{FLASH_SUPPRESSOR, AUG},
//	{FLASH_SUPPRESSOR, G41},
//	{FLASH_SUPPRESSOR, MINI14},
//	{FLASH_SUPPRESSOR, C7},
//	{FLASH_SUPPRESSOR, FAMAS},
//	{FLASH_SUPPRESSOR, AK74},
//	{FLASH_SUPPRESSOR, AKM},
//	{FLASH_SUPPRESSOR, M14},
//	{FLASH_SUPPRESSOR, FNFAL},
//	{FLASH_SUPPRESSOR, G3A3},
//	{FLASH_SUPPRESSOR, G11},
//	{FLASH_SUPPRESSOR, MINIMI},
//	{FLASH_SUPPRESSOR, RPK74},
//	{FLASH_SUPPRESSOR, HK21E},
//	{FLASH_SUPPRESSOR, MICRO_UZI},
//	{FLASH_SUPPRESSOR, M900},
//	{FLASH_SUPPRESSOR, M950},
//	{FLASH_SUPPRESSOR, M960},
//	{FLASH_SUPPRESSOR, BARRETT},
//	{FLASH_SUPPRESSOR, PSG},
//	{FLASH_SUPPRESSOR, VAL_SILENT},
//	{FLASH_SUPPRESSOR, TAR21},
//	{FLASH_SUPPRESSOR, ENFIELD},
//	{FLASH_SUPPRESSOR, MP5},
//	{FLASH_SUPPRESSOR, MP5N},
//	{FLASH_SUPPRESSOR, UMP45},
//	{FLASH_SUPPRESSOR, MP5SD},
//	{FLASH_SUPPRESSOR, G36K},
//	{FLASH_SUPPRESSOR, G36C},
//	{FLASH_SUPPRESSOR, MSG90A1},
//	{FLASH_SUPPRESSOR, AK103},
//
//	{REFLEX_SCOPED, GLOCK_17},
//	{REFLEX_SCOPED, GLOCK_18},
//	{REFLEX_SCOPED, BERETTA_92F},
//	{REFLEX_SCOPED, BERETTA_93R},
//	{REFLEX_SCOPED, SW38},
//	{REFLEX_SCOPED, BARRACUDA},
//	{REFLEX_SCOPED, DESERTEAGLE},
//	{REFLEX_SCOPED, M1911},
//	{REFLEX_SCOPED, MP5K},
//	{REFLEX_SCOPED, MAC10},
//	{REFLEX_SCOPED, THOMPSON},
//	{REFLEX_SCOPED, COMMANDO},
//	{REFLEX_SCOPED, MP53},
//	{REFLEX_SCOPED, AKSU74},
//	{REFLEX_SCOPED, P90},
//	{REFLEX_SCOPED, TYPE85},
//	{REFLEX_SCOPED, SKS},
//	{REFLEX_SCOPED, AUG},
//	{REFLEX_SCOPED, G41},
//	{REFLEX_SCOPED, MINI14},
//	{REFLEX_SCOPED, C7},
//	{REFLEX_SCOPED, FAMAS},
//	{REFLEX_SCOPED, AK74},
//	{REFLEX_SCOPED, AKM},
//	{REFLEX_SCOPED, M14},
//	{REFLEX_SCOPED, FNFAL},
//	{REFLEX_SCOPED, G3A3},
//	{REFLEX_SCOPED, G11},
//	{REFLEX_SCOPED, M870},
//	{REFLEX_SCOPED, SPAS15},
//	{REFLEX_SCOPED, CAWS},
//	{REFLEX_SCOPED, MINIMI},
//	{REFLEX_SCOPED, RPK74},
//	{REFLEX_SCOPED, HK21E},
//	{REFLEX_SCOPED, AUTOMAG_III},
//	{REFLEX_SCOPED, MICRO_UZI},
//	{REFLEX_SCOPED, M900},
//	{REFLEX_SCOPED, M950},
//	{REFLEX_SCOPED, M960},
//	{REFLEX_SCOPED, TAR21},
//	{REFLEX_SCOPED, ENFIELD},
//	{REFLEX_SCOPED, MP5},
//	{REFLEX_SCOPED, MP5N},
//	{REFLEX_SCOPED, UMP45},
//	{REFLEX_SCOPED, MP5SD},
//	{REFLEX_SCOPED, FIVE7},
//	{REFLEX_SCOPED, P7M8},
//	{REFLEX_SCOPED, G36K},
//	{REFLEX_SCOPED, G36C},
//	{REFLEX_SCOPED, BENNELLI},
//	{REFLEX_SCOPED, AK103},
//
//	{REFLEX_UNSCOPED, GLOCK_17},
//	{REFLEX_UNSCOPED, GLOCK_18},
//	{REFLEX_UNSCOPED, BERETTA_92F},
//	{REFLEX_UNSCOPED, BERETTA_93R},
//	{REFLEX_UNSCOPED, SW38},
//	{REFLEX_UNSCOPED, BARRACUDA},
//	{REFLEX_UNSCOPED, DESERTEAGLE},
//	{REFLEX_UNSCOPED, M1911},
//	{REFLEX_UNSCOPED, MP5K},
//	{REFLEX_UNSCOPED, MAC10},
//	{REFLEX_UNSCOPED, THOMPSON},
//	{REFLEX_UNSCOPED, COMMANDO},
//	{REFLEX_UNSCOPED, MP53},
//	{REFLEX_UNSCOPED, AKSU74},
//	{REFLEX_UNSCOPED, P90},
//	{REFLEX_UNSCOPED, TYPE85},
//	{REFLEX_UNSCOPED, SKS},
//	{REFLEX_UNSCOPED, DRAGUNOV},
//	{REFLEX_UNSCOPED, M24},
//	{REFLEX_UNSCOPED, AUG},
//	{REFLEX_UNSCOPED, G41},
//	{REFLEX_UNSCOPED, MINI14},
//	{REFLEX_UNSCOPED, C7},
//	{REFLEX_UNSCOPED, FAMAS},
//	{REFLEX_UNSCOPED, AK74},
//	{REFLEX_UNSCOPED, AKM},
//	{REFLEX_UNSCOPED, M14},
//	{REFLEX_UNSCOPED, FNFAL},
//	{REFLEX_UNSCOPED, G3A3},
//	{REFLEX_UNSCOPED, G11},
//	{REFLEX_UNSCOPED, M870},
//	{REFLEX_UNSCOPED, SPAS15},
//	{REFLEX_UNSCOPED, CAWS},
//	{REFLEX_UNSCOPED, MINIMI},
//	{REFLEX_UNSCOPED, RPK74},
//	{REFLEX_UNSCOPED, HK21E},
//	{REFLEX_UNSCOPED, AUTOMAG_III},
//	{REFLEX_UNSCOPED, MICRO_UZI},
//	{REFLEX_UNSCOPED, M900},
//	{REFLEX_UNSCOPED, M950},
//	{REFLEX_UNSCOPED, M960},
//	{REFLEX_UNSCOPED, BARRETT},
//	{REFLEX_UNSCOPED, PSG},
//	{REFLEX_UNSCOPED, VAL_SILENT},
//	{REFLEX_UNSCOPED, TAR21},
//	{REFLEX_UNSCOPED, ENFIELD},
//	{REFLEX_UNSCOPED, MP5},
//	{REFLEX_UNSCOPED, MP5N},
//	{REFLEX_UNSCOPED, UMP45},
//	{REFLEX_UNSCOPED, MP5SD},
//	{REFLEX_UNSCOPED, FIVE7},
//	{REFLEX_UNSCOPED, P7M8},
//	{REFLEX_UNSCOPED, G36K},
//	{REFLEX_UNSCOPED, G36C},
//	{REFLEX_UNSCOPED, MSG90A1},
//	{REFLEX_UNSCOPED, BENNELLI},
//	{REFLEX_UNSCOPED, AK103},
//
//	{DETONATOR, TNT},
//	{DETONATOR, HMX},
//	{DETONATOR, C1},
//	{DETONATOR, C4},
//
//	{REMDETONATOR, TNT},
//	{REMDETONATOR, HMX},
//	{REMDETONATOR, C1},
//	{REMDETONATOR, C4},
//
//	{CERAMIC_PLATES, FLAK_JACKET},
//	{CERAMIC_PLATES, FLAK_JACKET_18},
//	{CERAMIC_PLATES, FLAK_JACKET_Y},
//	{CERAMIC_PLATES, KEVLAR_VEST},
//	{CERAMIC_PLATES, KEVLAR_VEST_18},
//	{CERAMIC_PLATES, KEVLAR_VEST_Y},
//	{CERAMIC_PLATES, SPECTRA_VEST},
//	{CERAMIC_PLATES, SPECTRA_VEST_18},
//	{CERAMIC_PLATES, SPECTRA_VEST_Y},
//	{CERAMIC_PLATES, KEVLAR2_VEST},
//	{CERAMIC_PLATES, KEVLAR2_VEST_18},
//	{CERAMIC_PLATES, KEVLAR2_VEST_Y},
//	
//	{SPRING,					ALUMINUM_ROD},
//	{QUICK_GLUE,			STEEL_ROD},
//	{DUCT_TAPE,				STEEL_ROD},
//	{XRAY_BULB,				FUMBLE_PAK},
//	{CHEWING_GUM,			FUMBLE_PAK},
//	{BATTERIES,				XRAY_DEVICE},
//	{COPPER_WIRE,			LAME_BOY},
//	{COPPER_WIRE,			GOLDWATCH},
//	{0, 0}
//};

UINT16 Launchable[MAXITEMS+1][2];// =
//{
//	{GL_HE_GRENADE, GLAUNCHER},
//	{GL_HE_GRENADE, UNDER_GLAUNCHER},
//	{GL_TEARGAS_GRENADE, GLAUNCHER},
//	{GL_TEARGAS_GRENADE, UNDER_GLAUNCHER},
//	{GL_STUN_GRENADE, GLAUNCHER},
//	{GL_STUN_GRENADE, UNDER_GLAUNCHER},
//	{GL_SMOKE_GRENADE, GLAUNCHER},
//	{GL_SMOKE_GRENADE, UNDER_GLAUNCHER},
//	{MORTAR_SHELL, MORTAR},
//	{TANK_SHELL, TANK_CANNON},
//	{RPG_HE_ROCKET, RPG7},
//	{RPG_AP_ROCKET, RPG7},
//	{RPG_FRAG_ROCKET, RPG7},
//	{0, 0}
//};

UINT16 CompatibleFaceItems[MAXITEMS+1][2];// =
//{
//	{EXTENDEDEAR,			NIGHTGOGGLES},
//	{EXTENDEDEAR,			UVGOGGLES},
//	{EXTENDEDEAR,			SUNGOGGLES},
//	{EXTENDEDEAR,			GASMASK},
//	{EXTENDEDEAR,			NOTHING},
//	{WALKMAN,					NIGHTGOGGLES},
//	{WALKMAN,					UVGOGGLES},
//	{WALKMAN,					SUNGOGGLES},
//	{WALKMAN,					GASMASK},
//	{WALKMAN,					NOTHING},
//
//	{NIGHTGOGGLES,		EXTENDEDEAR},
//	{NIGHTGOGGLES,		WALKMAN},
//	{NIGHTGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{NIGHTGOGGLES,		GASMASK},
//	{NIGHTGOGGLES,		NOTHING},
//	{SUNGOGGLES,			EXTENDEDEAR},
//	{SUNGOGGLES,			WALKMAN},
//	{SUNGOGGLES,			GASMASK},
//	{SUNGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{SUNGOGGLES,			NOTHING},
//	{UVGOGGLES,				EXTENDEDEAR},
//	{UVGOGGLES,				WALKMAN},
//	{UVGOGGLES,				GASMASK},
//	{UVGOGGLES,			ROBOT_REMOTE_CONTROL},
//	{UVGOGGLES,				NOTHING},
//	{GASMASK,				EXTENDEDEAR},
//	{GASMASK,				WALKMAN},
//	{GASMASK,			NIGHTGOGGLES},
//	{GASMASK,			UVGOGGLES},
//	{GASMASK,			SUNGOGGLES},
//	{GASMASK,			ROBOT_REMOTE_CONTROL},
//	{GASMASK,					NOTHING},
//
//	{ROBOT_REMOTE_CONTROL,			NIGHTGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			UVGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			SUNGOGGLES},
//	{ROBOT_REMOTE_CONTROL,			GASMASK},
//	{ROBOT_REMOTE_CONTROL, NOTHING},
//	{0,								0},
//};


UINT16 Merge[MAXITEMS+1][6];// =
//{ // first item			second item						resulting item,					merge type
//	{FIRSTAIDKIT,			FIRSTAIDKIT,					FIRSTAIDKIT,						COMBINE_POINTS},
//	{MEDICKIT,				MEDICKIT,							MEDICKIT,								COMBINE_POINTS},
//	{CANTEEN,				CANTEEN,							CANTEEN,								COMBINE_POINTS},// Madd Combine canteens
//	{LOCKSMITHKIT,		LOCKSMITHKIT,					LOCKSMITHKIT,						COMBINE_POINTS},
//	{TOOLKIT,					TOOLKIT,							TOOLKIT,								COMBINE_POINTS},
//	{GAS_CAN,					GAS_CAN,							GAS_CAN,								COMBINE_POINTS},
//	{CAMOUFLAGEKIT,		CAMOUFLAGEKIT,				CAMOUFLAGEKIT,					COMBINE_POINTS},
//	{BEER,						BEER,									BEER,										COMBINE_POINTS},
//	{WINE,						WINE,									WINE,										COMBINE_POINTS},
//	{ALCOHOL,					ALCOHOL,							ALCOHOL,								COMBINE_POINTS},
//
//	{COMPOUND18,			FLAK_JACKET,							FLAK_JACKET_18,							TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_VEST,							KEVLAR_VEST_18,							TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR2_VEST,							KEVLAR2_VEST_18,						TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_VEST,							SPECTRA_VEST_18,						TREAT_ARMOUR},
//	{COMPOUND18,			LEATHER_JACKET_W_KEVLAR,	LEATHER_JACKET_W_KEVLAR_18,	TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_LEGGINGS,					KEVLAR_LEGGINGS_18,					TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_LEGGINGS,					SPECTRA_LEGGINGS_18,				TREAT_ARMOUR},
//	{COMPOUND18,			KEVLAR_HELMET,						KEVLAR_HELMET_18,						TREAT_ARMOUR},
//	{COMPOUND18,			SPECTRA_HELMET,						SPECTRA_HELMET_18,					TREAT_ARMOUR},
//	{COMPOUND18,			FLAK_JACKET_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_VEST_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_VEST_Y,						NOTHING,										DESTRUCTION},
//	{COMPOUND18,			LEATHER_JACKET_W_KEVLAR_Y,NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_LEGGINGS_Y,				NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_LEGGINGS_Y,				NOTHING,										DESTRUCTION},
//	{COMPOUND18,			KEVLAR_HELMET_Y,					NOTHING,										DESTRUCTION},
//	{COMPOUND18,			SPECTRA_HELMET_Y,					NOTHING,										DESTRUCTION},
//
//	{JAR_QUEEN_CREATURE_BLOOD,	FLAK_JACKET,					FLAK_JACKET_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_VEST,					KEVLAR_VEST_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_VEST,					SPECTRA_VEST_Y,					TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	LEATHER_JACKET_W_KEVLAR,	LEATHER_JACKET_W_KEVLAR_Y,	TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR2_VEST,					KEVLAR2_VEST_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_LEGGINGS,			KEVLAR_LEGGINGS_Y,			TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_LEGGINGS,			SPECTRA_LEGGINGS_Y,			TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_HELMET,				KEVLAR_HELMET_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_HELMET,				SPECTRA_HELMET_Y,				TREAT_ARMOUR},
//	{JAR_QUEEN_CREATURE_BLOOD,	FLAK_JACKET_18,				NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_VEST_18,				NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR2_VEST_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_VEST_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	LEATHER_JACKET_W_KEVLAR_18,	NOTHING,					DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_LEGGINGS_18,		NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_LEGGINGS_18,	NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	KEVLAR_HELMET_18,			NOTHING,								DESTRUCTION},
//	{JAR_QUEEN_CREATURE_BLOOD,	SPECTRA_HELMET_18,		NOTHING,								DESTRUCTION},
//
//	{RDX,							TNT,									HMX,										EXPLOSIVE},
//	{RDX,							C1,										C4,											EXPLOSIVE},
//	{TNT,							RDX,									HMX,										EXPLOSIVE},
//	{C1,							RDX,									C4,											EXPLOSIVE},
//
//	{STRING,					TIN_CAN,							STRING_TIED_TO_TIN_CAN,	EASY_MERGE},
//	{TIN_CAN,					STRING,								STRING_TIED_TO_TIN_CAN,	EASY_MERGE},
//
//	{FLASH_DEVICE,		DISPLAY_UNIT,					XRAY_DEVICE,						ELECTRONIC_MERGE},
//	{DISPLAY_UNIT,		FLASH_DEVICE,					XRAY_DEVICE,						ELECTRONIC_MERGE},
//
//	{0, 0, 0, 0}
//};


ComboMergeInfoStruct AttachmentComboMerge[MAXITEMS+1];// =
//{
//	// base item							attach 1								attach 2						 result										
//	{ALUMINUM_ROD,						{SPRING,								NOTHING},						SPRING_AND_BOLT_UPGRADE	},
//	{STEEL_ROD,								{QUICK_GLUE,						DUCT_TAPE},					GUN_BARREL_EXTENDER },
//	{FUMBLE_PAK,							{XRAY_BULB,							CHEWING_GUM},				FLASH_DEVICE },	
//	{LAME_BOY,								{COPPER_WIRE,						NOTHING},						DISPLAY_UNIT },
//	{GOLDWATCH,						{COPPER_WIRE,								NOTHING},						DETONATOR},
//	{NOTHING,									{NOTHING,								NOTHING},						NOTHING },
//};


UINT16 IncompatibleAttachments[MAXATTACHMENTS][2];// =
//{
//	{BIPOD,UNDER_GLAUNCHER},
//	{UNDER_GLAUNCHER,BIPOD},
//	{DETONATOR,REMDETONATOR},
//	{REMDETONATOR,DETONATOR},
//	{SNIPERSCOPE,REFLEX_SCOPED},
//	{REFLEX_SCOPED,SNIPERSCOPE},
//	{REFLEX_SCOPED,REFLEX_UNSCOPED},
//	{REFLEX_UNSCOPED,REFLEX_SCOPED},
//	{SILENCER,FLASH_SUPPRESSOR},
//	{FLASH_SUPPRESSOR,SILENCER},
//	{LASERSCOPE,REFLEX_UNSCOPED},
//	{REFLEX_UNSCOPED,LASERSCOPE},
//};

UINT16 ReplacementGuns[][2] =
{
	{ BARRACUDA,		DESERTEAGLE	},
	{ M1911,				GLOCK_17		},
	{ GLOCK_18,			BERETTA_93R	},
	{ BERETTA_92F,	GLOCK_17		},
	{ TYPE85,				BERETTA_93R	},
	{	THOMPSON,			MP5K				},
	{	MP53,					MP5K				},
	{ SPAS15,				M870				},
	{ AKSU74,				MAC10				},
	{ SKS,					MINI14			},
	{	AKM,					G41					},
	{ G3A3,					G41					},
	{ AK74,					G41					},
	{	DRAGUNOV,			M24					},
	{ FAMAS,				M14					},
	{ AUG,					C7					},
	{ RPK74,				MINIMI			},
	{ HK21E,				MINIMI			},
	{ 0,						0						}
};

UINT16 ReplacementAmmo[][2] =
{
	{ CLIP545_30_AP,		CLIP556_30_AP  },
	{ CLIP545_30_HP,		CLIP556_30_HP  },
	{ CLIP762W_10_AP,		CLIP762N_5_AP  },
	{ CLIP762W_30_AP,		CLIP762N_20_AP },
	{ CLIP762W_10_HP,		CLIP762N_5_HP  },
	{ CLIP762W_30_HP,		CLIP762N_20_HP },
	{ 0,								0							 }
};

// CHRISL: Structure Definitions for new inventory system items.
std::vector<LBETYPE> LoadBearingEquipment;
//LBETYPE LoadBearingEquipment[MAXITEMS+1];
//LBETYPE LoadBearingEquipment[] =
//{
//	//	Index	Class						Pocket Types---------------------------------
//	{	0,		/*Blank Entry*/			0,	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	1,		/*Default Thigh Pack*/	0,	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	2,		/*Default Vest Pack*/	1,	{1,	1,	0,	0,	0,	0,	0,	0,	2,	0,	0,	0} },
//	{	3,		/*Default Combat Pack*/	2,	{1,	1,	0,	0,	2,	0,	0,	0,	0,	0,	0,	0} },
//	{	4,		/*Default Back Pack*/	3,	{1,	1,	0,	0,	0,	0,	0,	0,	3,	3,	0,	0} },
//	{	5,		/*6P Backpack*/			3,	{1,	1,	1,	1,	0,	0,	0,	0,	3,	3,	0,	0} },
//	{	6,		/*6P Combat Pack*/		2,	{1,	1,	1,	1,	2,	2,	0,	0,	0,	0,	0,	0} }
//};

std::vector<POCKETTYPE> LBEPocketType;
//POCKETTYPE LBEPocketType[MAXITEMS+1]; //= 
//{
//	{	/* Blank Entry */			0,	0,	0,	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Small General Pocket */	1,	0,	0,	{4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Med General Pocket */	2,	0,	0,	{7,	6,	5,	4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Lg General Pocket */		3,	0,	0,	{10,9,	8,	7,	6,	5,	4,	3,	2,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Gun Sling */				4,	0,	1,	{0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} },
//	{	/* Knife Pocket */			5,	0,	1,	{0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0} }
//};

BOOLEAN ItemIsLegal( UINT16 usItemIndex )
{
	PERFORMANCE_MARKER
	//if the user has selected the reduced gun list
	if ( Item[usItemIndex].ubCoolness == 0 )
		return FALSE;

	if( !gGameOptions.fGunNut )
	{
		//if the item is a gun, or ammo
		//if( (Item[ usItemIndex ].usItemClass == IC_GUN) || (Item[ usItemIndex ].usItemClass == IC_AMMO )) //Madd: restriction removed
		{
			// and the item is only available with the extended guns
			if( Item[usItemIndex].biggunlist )
			{
				return(FALSE);
			}
		}
	}

	if ( gGameOptions.ubGameStyle != STYLE_SCIFI && Item[usItemIndex].scifi )
	{
		return FALSE;
	}

	// CHRISL: Restrict system specific items
	if( (UsingInventorySystem() == true) )
	{
		if(Item[usItemIndex].ItemSize == 99)
			return FALSE;
	}
	else
	{
		if(Item[usItemIndex].usItemClass == IC_LBEGEAR)
			return FALSE;
	}

	return(TRUE);
} 

// also used for ammo
BOOLEAN ExtendedGunListGun( UINT16 usGun )
{
	PERFORMANCE_MARKER
//	return( (Item[ usGun ].fFlags & ITEM_BIGGUNLIST) != 0 );
	return( (Item[ usGun ].biggunlist ) != 0 );
} 

UINT16 StandardGunListReplacement( UINT16 usGun )
{
	PERFORMANCE_MARKER
	UINT8 ubLoop;

	if ( ExtendedGunListGun( usGun ) )
	{
		ubLoop = 0;
		while ( ReplacementGuns[ ubLoop ][ 0 ] != 0 )
		{
			if ( ReplacementGuns[ ubLoop ][ 0 ] == usGun )
			{
				return( ReplacementGuns[ ubLoop ][ 1 ] );
			}
			ubLoop++;
		}
		// ERROR!
		AssertMsg( 0, String( "Extended gun with no replacement %d, CC:0", usGun ) );
		return( NOTHING );
	}
	else
	{
		return( NOTHING );
	}
}

UINT16 StandardGunListAmmoReplacement( UINT16 usAmmo )
{
	PERFORMANCE_MARKER
	UINT8 ubLoop;

	if ( ExtendedGunListGun( usAmmo ) )
	{
		ubLoop = 0;
		while ( ReplacementAmmo[ ubLoop ][ 0 ] != 0 )
		{
			if ( ReplacementAmmo[ ubLoop ][ 0 ] == usAmmo )
			{
				return( ReplacementAmmo[ ubLoop ][ 1 ] );
			}
			ubLoop++;
		}
		// ERROR!
		AssertMsg( 0, String( "Extended gun with no replacement %d, CC:0", usAmmo ) );
		return( NOTHING );
	}
	else
	{
		return( NOTHING );
	}
}

BOOLEAN WeaponInHand( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass & (IC_WEAPON | IC_THROWN) )
	{
		if (Item[pSoldier->inv[HANDPOS].usItem].fingerprintid )
		{
			if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != NO_PROFILE)
			{
				if (pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile != MADLAB )
				{
					if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != pSoldier->ubProfile)
					{
						return( FALSE );
					}
				}
				else
				{
					if (pSoldier->inv[HANDPOS][0]->data.ubImprintID != (NO_PROFILE + 1) )
					{
						return( FALSE );
					}
				}
			}
		}
		if (pSoldier->inv[HANDPOS][0]->data.gun.bGunStatus >= USABLE)
		{
			return( TRUE );
		}
	}
	// return -1 or some "broken" value if weapon is broken?
	return( FALSE );
}

// CHRISL:
UINT8 ItemSlotLimit( UINT16 usItem, INT8 bSlot )
{
	UINT8 ubSlotLimit;

	if ( bSlot < BODYPOSFINAL )
	{
		return( 1 );
	}
	else
	{
		ubSlotLimit = Item[usItem].ubPerPocket;
		if (bSlot >= BIGPOCKFINAL && ubSlotLimit > 1)
		{
			ubSlotLimit /= 2;
		}
		return( ubSlotLimit );
	}
}

// CHRISL: New definition for this function so that we can look at soldiers LBE pockets.
UINT8 ItemSlotLimit( OBJECTTYPE * pObject, INT16 bSlot, SOLDIERTYPE *pSoldier )
{
	UINT8	ubSlotLimit;
	UINT8	pIndex;
	UINT16	usItem, iSize;
	UINT32	iClass, pRestrict;

	usItem = pObject->usItem;
	if((UsingInventorySystem() == true) && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
	{
		ubSlotLimit = Item[usItem].ubPerPocket;
		if ( ubSlotLimit > MAX_OBJECTS_PER_SLOT )
			ubSlotLimit = MAX_OBJECTS_PER_SLOT;
		return( ubSlotLimit );
	}
	else if ( bSlot < BODYPOSFINAL && bSlot != KNIFEPOCKPOS && bSlot != GUNSLINGPOCKPOS )
	{
		return( 1 );
	}
	else
	{
		// IC Group Slots
		if(bSlot == GUNSLINGPOCKPOS)
			pIndex = 1;
		else if(bSlot == KNIFEPOCKPOS)
			pIndex = 2;
		else
			pIndex = (pSoldier->inv[icLBE[bSlot]].usItem == NOTHING) ? LoadBearingEquipment[icClass[bSlot]].lbePocketIndex[icPocket[bSlot]] : LoadBearingEquipment[Item[pSoldier->inv[icLBE[bSlot]].usItem].ubClassIndex].lbePocketIndex[icPocket[bSlot]];
		
		//ubSlotLimit = LBEPocketType[pIndex].ItemCapacityPerSize[Item[usItem].ItemSize];
		iSize = CalculateItemSize(pObject);
		ubSlotLimit = LBEPocketType[pIndex].ItemCapacityPerSize[iSize];
		pRestrict = LBEPocketType[pIndex].pRestriction;
		iClass = Item[usItem].usItemClass;
		// CHRISL: Include maximum capacity limits to keep game from crashing
		if ( ubSlotLimit > MAX_OBJECTS_PER_SLOT )
			ubSlotLimit = MAX_OBJECTS_PER_SLOT;
		//if ( Item[usItem].ItemSize < 10 && ubSlotLimit > 1)	// Item is a gun
		if ( iSize < 10 && ubSlotLimit > 1)
			ubSlotLimit = 1;
		if(pRestrict != 0)
			if(pRestrict != iClass)
				ubSlotLimit = 0;
		
		return( ubSlotLimit );
	}
}

UINT32 MoneySlotLimit( INT8 bSlot )
{
	PERFORMANCE_MARKER
	if ( bSlot >= BIGPOCKFINAL )	/* CHRISL */
	{
		return( MAX_MONEY_PER_SLOT / 2 );
	}
	else
	{
		return( MAX_MONEY_PER_SLOT );
	}
}

INT8 FindBestWeaponIfCurrentIsOutOfRange(SOLDIERTYPE * pSoldier, INT8 bCurrentWeaponIndex, UINT16 bWantedRange)
{
	PERFORMANCE_MARKER
	//assuming current weapon is in the handpos
	if (GunRange(&pSoldier->inv[bCurrentWeaponIndex]) >= bWantedRange)
	{
		//our current weapon is good enough
		return( bCurrentWeaponIndex );
	}

	UINT16 range;
	UINT16 bestRange = 0;
	INT8 bestWeaponThatMeetsRange = 0;
	INT8 secondBestWeapon = 0;
	//search for weapons that meet the range, then sort by damage.
	//if there are no weapons that meet the range, then use the longest range we can find
	for (INT8 bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		//if this is a weapon
		if (Item[pSoldier->inv[bLoop].usItem].usItemClass & (IC_WEAPON | IC_THROWN) )
		{
			range = GunRange(&pSoldier->inv[bLoop]);
			if (range >= bWantedRange)
			{
				if (bestWeaponThatMeetsRange == 0)
				{
					bestWeaponThatMeetsRange = bLoop;
				}
				else if (GetDamage(&pSoldier->inv[bLoop]) > GetDamage(&pSoldier->inv[bestWeaponThatMeetsRange]))
				{
					//does this weapon have more damage?
					bestWeaponThatMeetsRange = bLoop;
				}
			}
			else if (range > bestRange)
			{
				//weapon does not meet range, but it could be better anyways
				bestRange = range;
				secondBestWeapon = bLoop;
			}
			else if (range == bestRange)
			{
				//weapon ties with secondBestWeapon's range
				if (secondBestWeapon == 0)
				{
					//this if can happen if range of bLoop is 0!
					secondBestWeapon = bLoop;
				}
				else if (GetDamage(&pSoldier->inv[bLoop]) > GetDamage(&pSoldier->inv[secondBestWeapon]))
				{
					secondBestWeapon = bLoop;
				}
			}
		}
	}

	if (bestWeaponThatMeetsRange)
	{
		return bestWeaponThatMeetsRange;
	}
	else if (secondBestWeapon)
	{
		return secondBestWeapon;
	}
	return( bCurrentWeaponIndex );
}

INT8 FindObj( SOLDIERTYPE * pSoldier, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (pSoldier->inv[bLoop].usItem == usItem)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindMetalDetector( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].metaldetector )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindLockBomb( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].lockbomb )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindUsableObj( SOLDIERTYPE * pSoldier, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem == usItem && pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}


INT8 FindObjExcludingSlot( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bExcludeSlot )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (bLoop == bExcludeSlot)
		{
			continue;
		}
		if (pSoldier->inv[bLoop].usItem == usItem)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindExactObj( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if ( (pObj == &(pSoldier->inv[bLoop])) && (pSoldier->inv[bLoop] == *pObj) )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}


INT8 FindObjWithin( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bLower, INT8 bUpper )
{
	PERFORMANCE_MARKER
	INT8	bLoop;

	for (bLoop = bLower; bLoop <= bUpper; bLoop++)
	{
		if (pSoldier->inv[bLoop].usItem == usItem)			
		{
			return( bLoop );
		}
	}
	return( ITEM_NOT_FOUND );
}

INT8 FindObjInObjRange( SOLDIERTYPE * pSoldier, UINT16 usItem1, UINT16 usItem2 )
{
	PERFORMANCE_MARKER
	INT8		bLoop;
	UINT16	usTemp;

	if (usItem1 > usItem2 )
	{
		// swap the two...
		usTemp = usItem2;
		usItem2 = usItem1;
		usItem1 = usTemp;
	}

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		usTemp = pSoldier->inv[bLoop].usItem;
		if ( usTemp >= usItem1 && usTemp <= usItem2 )
		{
			return( bLoop );
		}
	}

	return( ITEM_NOT_FOUND );
}


INT8 FindObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindObjClassAfterSlot( SOLDIERTYPE * pSoldier, INT8 bStartAfter, UINT32 usItemClass )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = bStartAfter + 1; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].usItemClass == usItemClass)
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindAIUsableObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass )
{
	PERFORMANCE_MARKER
	// finds the first object of the specified class which does NOT have
	// the "unusable by AI" flag set.

	// uses & rather than == so that this function can search for any weapon
	INT8 bLoop;

	// This is for the AI only so:

	// Do not consider tank cannons or rocket launchers to be "guns"

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if ( (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass) && !(pSoldier->inv[bLoop].fFlags & OBJECT_AI_UNUSABLE) && (pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE ) )
		{
			if ( usItemClass == IC_GUN && EXPLOSIVE_GUN( pSoldier->inv[bLoop].usItem ) )
			{
				continue;
			}
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindAIUsableObjClassWithin( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass, INT8 bLower, INT8 bUpper )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	// This is for the AI only so:
	// Do not consider tank cannons or rocket launchers to be "guns"

	for (bLoop = bLower; bLoop <= bUpper; bLoop++)
	{
		if ( (Item[pSoldier->inv[bLoop].usItem].usItemClass & usItemClass) && !(pSoldier->inv[bLoop].fFlags & OBJECT_AI_UNUSABLE) && (pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE ) )
		{
			if ( usItemClass == IC_GUN && EXPLOSIVE_GUN( pSoldier->inv[bLoop].usItem ) )
			{
				continue;
			}
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindEmptySlotWithin( SOLDIERTYPE * pSoldier, INT8 bLower, INT8 bUpper )
{
	PERFORMANCE_MARKER
	INT8	bLoop;

	for (bLoop = bLower; bLoop <= bUpper; bLoop++)
	{
		// CHRISL: Only look at valid pockets
		if((UsingInventorySystem() == false) && !oldInv[bLoop])
			continue;
		if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && !vehicleInv[bLoop])
			continue;
		if (pSoldier->inv[bLoop].usItem == 0)
		{
//			if (bLoop == SECONDHANDPOS && Item[pSoldier->inv[HANDPOS].usItem].fFlags & ITEM_TWO_HANDED)
			if (bLoop == SECONDHANDPOS && Item[pSoldier->inv[HANDPOS].usItem].twohanded )
			{
				continue;
			}
			else
			{
				return( bLoop );
			}
		}
	}
	return( ITEM_NOT_FOUND );
}

BOOLEAN GLGrenadeInSlot(SOLDIERTYPE *pSoldier, INT8 bSlot )
{
	PERFORMANCE_MARKER
	if (Item[pSoldier->inv[bSlot].usItem].glgrenade)
		return TRUE;
	else
		return FALSE;
	//switch (pSoldier->inv[bSlot].usItem)
	//{
	//	case GL_HE_GRENADE:
	//	case GL_TEARGAS_GRENADE:
	//	case GL_STUN_GRENADE:
	//	case GL_SMOKE_GRENADE:
	//		return(TRUE);
	//	default:
	//		return(FALSE);
	//}
}

// for grenade launchers
INT8 FindGLGrenade( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (GLGrenadeInSlot( pSoldier, bLoop ))
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindThrowableGrenade( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;
	BOOLEAN fCheckForFlares = FALSE;

	// JA2Gold: give some priority to looking for flares when at night
	// this is AI only so we can put in some customization for night
	if (GetTimeOfDayAmbientLightLevel() == NORMAL_LIGHTLEVEL_NIGHT)
	{
		if (pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 2))
		{
			fCheckForFlares = TRUE;
		}
	}
	if (fCheckForFlares)
	{
		// Do a priority check for flares first
		for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
		{
			if ( Item[pSoldier->inv[ bLoop ].usItem].flare )
			{
				return( bLoop );
			}
		}
	}
	
	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if ( (Item[ pSoldier->inv[ bLoop ].usItem ].usItemClass & IC_GRENADE) && // Try this check instead, to avoid tossing RPG rounds     !GLGrenadeInSlot( pSoldier, bLoop ) &&
			GetLauncherFromLaunchable( pSoldier->inv[ bLoop ].usItem) == NOTHING )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

OBJECTTYPE* FindAttachment( OBJECTTYPE * pObj, UINT16 usItem )
{
	PERFORMANCE_MARKER
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (iter->usItem == usItem)
		{
			return &(*iter);
		}
	}
	return( 0 );
}

OBJECTTYPE* FindAttachmentByClass( OBJECTTYPE * pObj, UINT32 uiItemClass )
{
	PERFORMANCE_MARKER
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == uiItemClass)
		{
			return &(*iter);
		}
	}
	return( 0 );
}

INT8 FindLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: weapon=%d",usWeapon));
	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if ( ValidLaunchable( pSoldier->inv[ bLoop ].usItem , usWeapon ) )
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: returning slot %d",bLoop));
			return( bLoop );
		}
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindLaunchable: item not found"));
	return( ITEM_NOT_FOUND );
}

INT8 FindNonSmokeLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: weapon=%d",usWeapon));
	for (bLoop = 0; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if ( ValidLaunchable( pSoldier->inv[ bLoop ].usItem , usWeapon ) && Explosive[Item[pSoldier->inv[ bLoop ].usItem].ubClassIndex].ubType != EXPLOSV_SMOKE )
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: returning slot %d",bLoop));
			return( bLoop );
		}
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("FindNonSmokeLaunchable: item not found, so find items including smoke"));
	return( FindLaunchable(pSoldier,usWeapon) );
}

OBJECTTYPE* FindLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon )
{
	PERFORMANCE_MARKER
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (ValidLaunchable( iter->usItem, usWeapon))
		{
			return &(*iter);
		}
	}

	return( 0 );
}

OBJECTTYPE* FindNonSmokeLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon )
{
	PERFORMANCE_MARKER

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (ValidLaunchable( iter->usItem, usWeapon) && Explosive[Item[iter->usItem].ubClassIndex].ubType != EXPLOSV_SMOKE)
		{
			return &(*iter);
		}
	}

	return( FindLaunchableAttachment(pObj,usWeapon) );
}


//Simple check to see if the item has any attachments
BOOLEAN ItemHasAttachments( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	return ((*pObj)[0]->attachments.empty() == false);
}

// Determine if it is possible to add this attachment to the CLASS of this item
// (i.e. to any item in the class)
BOOLEAN ValidAttachmentClass( UINT16 usAttachment, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;
	while( 1 )
	{
		// see comment for AttachmentInfo array for why we skip IC_NONE
		if ( AttachmentInfo[ iLoop ].uiItemClass != IC_NONE )
		{
			if ( AttachmentInfo[ iLoop ].usItem == usAttachment )
			{
				if ( AttachmentInfo[ iLoop ].uiItemClass == Item[ usItem ].usItemClass )
				{
					return( TRUE );
				}
			}
		}
		iLoop++;
		if (AttachmentInfo[iLoop].usItem == 0)
		{
			// end of the array
			break;
		}
	}
	return( FALSE );	
}

INT8 GetAttachmentInfoIndex( UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;

	while( 1 )
	{
		if ( AttachmentInfo[ iLoop ].usItem == usItem )
		{
			return( (INT8) iLoop );
		}
		iLoop++;
		if (AttachmentInfo[iLoop].usItem == 0)
		{
			// end of the array
			break;
		}
	}
	return( -1 );
}

//Determine if it is possible to add this attachment to the item.
BOOLEAN internalValidAttachment( UINT16 usAttachment, UINT16 usItem, UINT8 * pubAPCost )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;
	*pubAPCost = (UINT8)AP_RELOAD_GUN; //default value

	// look for the section of the array pertaining to this attachment...
	while( 1 )
	{
		if (Attachment[iLoop][0] == usAttachment)
		{
			break;
		}
		iLoop++;
		if (Attachment[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Attachment[iLoop][1] == usItem)
		{
			*pubAPCost = (UINT8)Attachment[iLoop][2]; //Madd: get ap cost of attaching items :)
			break;
		}
		iLoop++;
		if (Attachment[iLoop][0] != usAttachment)
		{
			// the proposed item cannot be attached to the item in question
			return( FALSE );
		}
	}
	return( TRUE );
}

//Determine if it is possible to add this attachment to the item.
BOOLEAN ValidAttachment( UINT16 usAttachment, UINT16 usItem )
{
	PERFORMANCE_MARKER
	UINT8 ubAPCost;

	return internalValidAttachment( usAttachment, usItem, &ubAPCost);
}
UINT8 AttachmentAPCost( UINT16 usAttachment, UINT16 usItem )
{
	PERFORMANCE_MARKER
	UINT8 ubAPCost;

	internalValidAttachment( usAttachment, usItem, &ubAPCost);

	return ubAPCost;
}

//Determine if this item can receive this attachment.  This is different, in that it may
//be possible to have this attachment on this item, but may already have an attachment on
//it which doesn't work simultaneously with the new attachment (like a silencer and duckbill).
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment )
{
	PERFORMANCE_MARKER
	return ValidItemAttachment ( pObj, usAttachment, fAttemptingAttachment, TRUE );
}
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage )
{
	PERFORMANCE_MARKER
	BOOLEAN		fSameItem = FALSE, fSimilarItems = FALSE;
	UINT16		usSimilarItem = NOTHING;

	if ( !ValidAttachment( usAttachment, pObj->usItem ) )
	{
		// check for an underslung grenade launcher attached to the gun
		if ( (IsGrenadeLauncherAttached ( pObj ) ) && ValidLaunchable( usAttachment, GetAttachedGrenadeLauncher( pObj ) ) )
		{
			return ( TRUE );
			/*
			if ( fAttemptingAttachment )
			{
				// if there is no other grenade attached already, then we can attach it
				if (FindAttachmentByClass( pObj, IC_GRENADE) != ITEM_NOT_FOUND)
				{
					return( FALSE );
				}
				// keep going, it can be attached to the grenade launcher
			}
			else
			{
				// logically, can be added
				return( TRUE );
			}
			*/
		}
		else
		{
			if ( fAttemptingAttachment && ValidAttachmentClass( usAttachment, pObj->usItem ) )
			{
				// well, maybe the player thought he could
				CHAR16	zTemp[ 100 ];
				
				swprintf( zTemp, Message[ STR_CANT_ATTACH ], ItemNames[ usAttachment ], ItemNames[ pObj->usItem ] );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			}

			return( FALSE );
		}
	}
	// special conditions go here
	// can't have two of the same attachment on an item
	/*
	if (FindAttachment( pObj, usAttachment ) != 0)
	{
		fSameItem = TRUE;
	}
	*/

	for(int i = 0;i<sizeof(IncompatibleAttachments);i++)
	{
		if ( IncompatibleAttachments[i][0] == NONE )
			break;
	
		if ( IncompatibleAttachments[i][0] == usAttachment && FindAttachment (pObj,IncompatibleAttachments[i][1]) != 0 )
		{
			fSimilarItems = TRUE;
			usSimilarItem = IncompatibleAttachments[i][1];
			break;
		}
	}


	//// special code for items which won't attach if X is present
	//switch( usAttachment )
	//{
	//	case BIPOD:
	//		if ( FindAttachment( pObj, UNDER_GLAUNCHER) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = UNDER_GLAUNCHER;
	//		}
	//		break;
	//	case UNDER_GLAUNCHER:
	//		if ( FindAttachment( pObj, BIPOD ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = BIPOD;
	//		}
	//		break;
	///*
	//	case LASERSCOPE:
	//		if (FindAttachment( pObj, SNIPERSCOPE ) != 0)
	//		{
	//			return( FALSE );
	//		}
	//		break;
	//	case SNIPERSCOPE:
	//		if (FindAttachment( pObj, LASERSCOPE ) != 0)
	//		{
	//			return( FALSE );
	//		}
	//		break;
	//		*/
	//	case DETONATOR:
	//		if( FindAttachment( pObj, REMDETONATOR ) != 0 )
	//		{
	//			fSameItem = TRUE;
	//		}
	//		break;
	//	case REMDETONATOR:
	//		if( FindAttachment( pObj, DETONATOR ) != 0 )
	//		{
	//			fSameItem = TRUE;
	//		}
	//		break;
	//	case SNIPERSCOPE:
	//		if( FindAttachment( pObj, REFLEX_SCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_SCOPED;
	//		}
	//		break;
	//	case REFLEX_SCOPED:
	//		if( FindAttachment( pObj, SNIPERSCOPE ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = SNIPERSCOPE;
	//		}
	//		if( FindAttachment( pObj, REFLEX_UNSCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_UNSCOPED;
	//		}
	//		break;
	//	case REFLEX_UNSCOPED:
	//		if( FindAttachment( pObj, REFLEX_SCOPED ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = REFLEX_SCOPED;
	//		}
	//		break;
	//	case SILENCER:
	//		if( FindAttachment( pObj, FLASH_SUPPRESSOR  ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = FLASH_SUPPRESSOR;
	//		}
	//		break;
	//	case FLASH_SUPPRESSOR:
	//		if( FindAttachment( pObj, SILENCER  ) != 0 )
	//		{
	//			fSimilarItems = TRUE;
	//			usSimilarItem = SILENCER;
	//		}
	//		break;
	//}

	if (fAttemptingAttachment)	
	{
		if (fSameItem)
		{
			if (fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_ATTACHMENT_ALREADY ] );
			return( FALSE );
		} 
		else if (fSimilarItems)
		{
			if (fDisplayMessage) ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ usSimilarItem ], ItemNames[ usAttachment ] );
			return( FALSE );
		}
	}

	return( TRUE );
}

//Determines if it is possible to equip this weapon with this ammo.
BOOLEAN ValidAmmoType( UINT16 usItem, UINT16 usAmmoType )
{
	PERFORMANCE_MARKER
	if (Item[usItem].usItemClass == IC_GUN && Item[usAmmoType].usItemClass == IC_AMMO)
	{
		if (Weapon[usItem].ubCalibre == Magazine[Item[usAmmoType].ubClassIndex].ubCalibre)
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN CompatibleFaceItem( UINT16 usItem1, UINT16 usItem2 )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;

	//Madd: skip this function if either item is nothing
	//this will let us trim some lines from compatiblefaceitems.xml
	if ( usItem1 == NONE || usItem2 == NONE )
		return TRUE;

	// look for the section of the array pertaining to this attachment...
	while( 1 )
	{
		if (CompatibleFaceItems[iLoop][0] == usItem1)
		{
			break;
		}
		iLoop++;
		if (CompatibleFaceItems[iLoop][0] == 0)
		{
			// the proposed item cannot fit with anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (CompatibleFaceItems[iLoop][1] == usItem2)
		{
			break;
		}
		iLoop++;
		if (CompatibleFaceItems[iLoop][0] != usItem1)
		{
			// the proposed item cannot be attached to the item in question
			return( FALSE );
		}
	}
	return( TRUE );
}


//Determines if this item is a two handed item.
BOOLEAN TwoHandedItem( UINT16 usItem )
{
	PERFORMANCE_MARKER
//	if (Item[usItem].fFlags & ITEM_TWO_HANDED)
	if (Item[usItem].twohanded )
	{
		return( TRUE );
	}
	return FALSE;
}

BOOLEAN ValidLaunchable( UINT16 usLaunchable, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;
	//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d",usLaunchable,usItem));
	// look for the section of the array pertaining to this launchable item...
	while( 1 )
	{
		if (Launchable[iLoop][0] == usLaunchable)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return FALSE, launchable not found",usLaunchable,usItem));
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Launchable[iLoop][1] == usItem)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] != usLaunchable)
		{
			// the proposed item cannot be attached to the item in question
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return FALSE, item not found",usLaunchable,usItem));
			return( FALSE );
		}
	}
	//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("ValidLaunchable: launchable=%d, item=%d, return TRUE",usLaunchable,usItem));
	return( TRUE );
}

BOOLEAN ValidItemLaunchable( OBJECTTYPE * pObj, UINT16 usAttachment )
{
	PERFORMANCE_MARKER
	if ( !ValidLaunchable( usAttachment, pObj->usItem ) )
	{
		return( FALSE );
	}
	// if we can find another of the same class as the attachment, it's not possible
	if ( FindAttachmentByClass( pObj, Item[ usAttachment ].usItemClass ) != 0 )
	{
		return( FALSE );
	}
	return( TRUE );
}


UINT16 GetLauncherFromLaunchable( UINT16 usLaunchable )
{
	PERFORMANCE_MARKER
	INT32 iLoop = 0;
	// look for the section of the array pertaining to this launchable item...
	while( 1 )
	{
		if (Launchable[iLoop][0] == usLaunchable)
		{
			break;
		}
		iLoop++;
		if (Launchable[iLoop][0] == 0)
		{
			// the proposed item cannot be attached to anything!
			return( NOTHING );
		}
	}

	return( Launchable[iLoop][1] );
}



BOOLEAN EvaluateValidMerge( UINT16 usMerge, UINT16 usItem, UINT16 * pusResult, UINT16 * pusResult2, UINT8 * pubType, UINT8 * pubAPCost )
{
	PERFORMANCE_MARKER
	// NB "usMerge" is the object being merged with (e.g. compound 18)
	// "usItem" is the item being merged "onto" (e.g. kevlar vest)
	INT32 iLoop = 0;

	if (usMerge == usItem && Item[ usItem ].usItemClass == IC_AMMO)
	{
		*pusResult = usItem;
		*pubType = COMBINE_POINTS;
		return( TRUE );
	}
	// look for the section of the array pertaining to this Merge...
	while( 1 )
	{
		if (Merge[iLoop][0] == usMerge)
		{
			break;
		}
		iLoop++;
		if (Merge[iLoop][0] == 0)
		{
			// the proposed item cannot be merged with anything!
			return( FALSE );
		}
	}
	// now look through this section for the item in question
	while( 1 )
	{
		if (Merge[iLoop][1] == usItem)
		{
			break;
		}
		iLoop++;
		if (Merge[iLoop][0] != usMerge)
		{
			// the proposed item cannot be merged with the item in question
			return( FALSE );
		}
	}
	*pusResult = Merge[iLoop][2];
	*pusResult2 = Merge[iLoop][3];
	*pubType = (UINT8) Merge[iLoop][4];
	*pubAPCost = (UINT8) Merge[iLoop][5];
	return( TRUE );
}

BOOLEAN ValidMerge( UINT16 usMerge, UINT16 usItem )
{
	PERFORMANCE_MARKER
	UINT16	usIgnoreResult, usIgnoreResult2;
	UINT8		ubIgnoreType, ubIgnoreAPCost;
	return( EvaluateValidMerge( usMerge, usItem, &usIgnoreResult, &usIgnoreResult2, &ubIgnoreType, &ubIgnoreAPCost ) );
}

//ADB TODO: Chris, check these out and see if they are ok with you

// CHRISL: New function to dynamically modify ItemSize based on attachments
UINT16 CalculateItemSize( OBJECTTYPE *pObject )
{
	UINT16		iSize, newSize, testSize;
	UINT32		cisIndex;
	BOOLEAN		psFound = FALSE;
	UINT8		cisPocketSize[6][4] =
	{
		11, 12, 13, 14,
		15, 16, 17, 18,
		19, 20, 21, 22,
		23, 24, 25, 26,
		27, 28, 29, 30,
		31, 32, 33, 34
	};
	int			cnt, originalSizeX, originalSizeY;
	int			newSizeX, newSizeY;
	// Determine default ItemSize based on item and attachments
	cisIndex = pObject->usItem;
	iSize = Item[cisIndex].ItemSize;
	if(iSize>34)
		iSize = 34;

	//store the original size for later calculations
	for(originalSizeX=0; originalSizeX<6; originalSizeX++)
	{
		for(originalSizeY=0; originalSizeY<4; originalSizeY++)
		{
			if(iSize == cisPocketSize[originalSizeX][originalSizeY])
			{
				psFound = TRUE;
				break;
			}
		}
		if(psFound)
			break;
	}

	//for each object in the stack, hopefully there is only 1
	for (int numStacked = 0; numStacked = pObject->ubNumberOfObjects; ++numStacked) {

#if 0
		//I don't think there will ever be any attachments, ChrisL delete this if so
		//for each attachment to this object
		for (attachmentList::iterator iter = (*pObject)[numStacked]->attachments.begin();
			iter != (*pObject)[numStacked]->attachments.end(); ++iter) {
				//plus? minus?
			iSize -= CalculateItemSize(&(*iter));;
		}
#endif

		// Check if we're looking at a LBENODE or not
		if(pObject->IsLBE())
		{
			if(pObject->GetLBEIndex(numStacked) != NULL)
			{
				newSize = 0;
				for(cnt=0; cnt<ITEMS_IN_LBE; cnt++)
				{
					if(pObject->GetLBEPointer(numStacked)->inv[cnt].usItem != NOTHING)
					{
						testSize = CalculateItemSize(&(pObject->GetLBEPointer(numStacked)->inv[cnt]));
						newSize = (testSize > newSize) ? testSize : newSize;
					}
				}
			}
			// Resize based on contents
			if(newSize > 0)
			{
				psFound = FALSE;
				for(newSizeX=0; newSizeX<6; newSizeX++)
				{
					for(newSizeY=0; newSizeY<4; newSizeY++)
					{
						if(newSize == cisPocketSize[newSizeX][newSizeY])
						{
							psFound = TRUE;
							break;
						}
					}
					if(psFound)
						break;
				}
				if((originalSizeX-2)>=newSizeX)	// Stored item is much smaller then an empty LBE item.  Don't change size
					iSize = iSize;
				else if((originalSizeX-1)==newSizeX)	// Stored item is large enough to increase LBE item size.
				{
					if(newSizeY > originalSizeY)
						originalSizeY = newSizeY;
					iSize = cisPocketSize[originalSizeX][originalSizeY];
				}
				else	// Stored item is very large compared to the LBE item.
				{
					originalSizeY = 3;
					iSize = cisPocketSize[originalSizeX][originalSizeY];
				}
			}
		}
	}
	return(iSize);
}

UINT16 CalculateAmmoWeight( UINT16 usGunAmmoItem, UINT8 ubShotsLeft )
{
	if( 0 == usGunAmmoItem ) /* Sergeant_Kolja: 2007-06-11, Fix for Creature Spit. This has no Ammo, so the old code calculated accidentally -1.6 resulting in 0xFFFF */
	{
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "'no ammo weight' FIX for Creatures\r\n" );
		return 0;
	}

	//Temporary calculation for minWeight. 0.2*ubWeight rounded correctly 
	UINT32 uiMinWeight = (UINT32)((Item[usGunAmmoItem].ubWeight / 5.0) + 0.5);
	if( uiMinWeight < 1 || uiMinWeight > Item[usGunAmmoItem].ubWeight)
	{
		uiMinWeight = 1;
	}

	double weight = 0.0;

	if(ubShotsLeft > 0)
	{
		if( uiMinWeight == Item[usGunAmmoItem].ubWeight )
		{
			weight += (double)uiMinWeight;
		}
		else
		{
			weight += (double)uiMinWeight + (( (double)ubShotsLeft / (double)Magazine[ Item[usGunAmmoItem].ubClassIndex ].ubMagSize) * ( (double)Item[usGunAmmoItem].ubWeight - (double)uiMinWeight ));
		}
	}
	weight += 0.5; //Pulmu:To round correctly
	return (UINT16)weight;
	//Pulmu end
}

/*CHRISL: Change to a 16bit integer for a max weight of 6553.5kg.  Also changed to account for
new inventory system. */
UINT16 CalculateObjectWeight( OBJECTTYPE *pObject )
{
	PERFORMANCE_MARKER
	INT32 cnt;
	UINT16 usWeight;
	INVTYPE * pItem;

	pItem = &(Item[ pObject->usItem ]);

	// Start with base weight
	usWeight = pItem->ubWeight;
	//multiply by the number of objects, can be 0
	usWeight *= pObject->ubNumberOfObjects;

	if ( pItem->usItemClass != IC_AMMO )
	{
		for( cnt = 0; cnt < pObject->ubNumberOfObjects; cnt++ )
		{
			// Are we looking at an LBENODE item?  New inventory only.
			if(pItem->usItemClass == IC_LBEGEAR && pObject->IsLBE() && (UsingInventorySystem() == true))
			{
				for ( int subObjects = 0; subObjects < 12; subObjects++)
				{
					if (pObject->GetLBEPointer(cnt)->inv[subObjects].usItem != NOTHING)
					{
						usWeight += CalculateObjectWeight(&(pObject->GetLBEPointer(cnt)->inv[subObjects]));
					}
				}
				//do not search for attachments to an LBE
				continue;
			}


			// account for any attachments
			for (attachmentList::iterator iter = (*pObject)[cnt]->attachments.begin(); iter != (*pObject)[cnt]->attachments.end(); ++iter) {
				usWeight += CalculateObjectWeight(&(*iter));
			}

			// add in weight of ammo
			if (Item[ pObject->usItem ].usItemClass == IC_GUN && (*pObject)[cnt]->data.gun.ubGunShotsLeft > 0)
			{
				if( gGameExternalOptions.fAmmoDynamicWeight == TRUE )
				{
					usWeight += CalculateAmmoWeight((*pObject)[cnt]->data.gun.usGunAmmoItem, (*pObject)[cnt]->data.gun.ubGunShotsLeft);
				}
				else
				{
					usWeight += Item[ (*pObject)[cnt]->data.gun.usGunAmmoItem ].ubWeight;
				}
			}
		}
	}
	else if ( pItem->usItemClass == IC_AMMO && gGameExternalOptions.fAmmoDynamicWeight == TRUE )//Pulmu: added weight allowance for ammo not being full
	{
		int ammoLeft = 0;
		for( cnt = 0; cnt < pObject->ubNumberOfObjects; cnt++ )
		{
			ammoLeft += (*pObject)[cnt]->data.ubShotsLeft;
		}
		usWeight = CalculateAmmoWeight(pObject->usItem, ammoLeft);
	}

	return( usWeight );
}

#if 0

UINT16 CalculateObjectWeight( OBJECTTYPE *pObject )
{
	INT32 cnt;
	UINT16 usWeight, tWeight;
	INVTYPE * pItem;

	pItem = &(Item[ pObject->usItem ]);

	// Start with base weight
	usWeight = pItem->ubWeight;

	// Are we looking at an LBENODE item?  New inventory only.
	if(pItem->usItemClass == IC_LBEGEAR && pObject->IsLBE() && (UsingInventorySystem() == true))
	{
		for ( cnt = 0; cnt < ITEMS_IN_LBE; cnt++)
		{
			if (pObject->GetLBEPointer->inv[cnt].usItem != NOTHING)
			{
				tWeight = CalculateObjectWeight(&(pObject->GetLBEPointer->inv[cnt]));
				if(Item[pObject->GetLBEPointer->inv[cnt].usItem].usItemClass != IC_AMMO)
					tWeight *= pObject->GetLBEPointer->inv[cnt].ubNumberOfObjects;
				usWeight += tWeight;
			}
		}
	}
	// Are we looking at an item that can have attachments?
	else if ( pItem->ubPerPocket < 2 && pItem->usItemClass != IC_AMMO )
	{
		// account for any attachments
		for ( cnt = 0; cnt < MAX_ATTACHMENTS; cnt++ )
		{
			if (pObject->usAttachItem[cnt] != NOTHING )
			{
				usWeight += Item[ pObject->usAttachItem[cnt] ].ubWeight;
			}
		}
		// add in weight of ammo
		if (Item[ pObject->usItem ].usItemClass == IC_GUN && (*pObject)[0]->data.gun.ubGunShotsLeft > 0)
		{
			if( 0==(*pObject)[0]->data.gun.usGunAmmoItem ) /* Sergeant_Kolja: 2007-06-11, Fix for Creature Spit. This has no Ammo, so the old code calculated accidentally -1.6 resulting in 0xFFFF */
			{
				DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "'no ammo weight' FIX for Creatures\r\n" );
			}
			else if( gGameExternalOptions.fAmmoDynamicWeight == TRUE )
			{
				//Pulmu:
				//Temporary calculation for minWeight
				UINT32 uiMinWeight = (UINT32)((Item[ (*pObject)[0]->data.gun.usGunAmmoItem].ubWeight / 5.0) + 0.5);
				if( uiMinWeight < 1 || uiMinWeight > Item[ (*pObject)[0]->data.gun.usGunAmmoItem].ubWeight)
				{
					uiMinWeight = 1;
				}

				if( uiMinWeight == Item[ (*pObject)[0]->data.gun.usGunAmmoItem].ubWeight )
				{
					usWeight += uiMinWeight;
				}
				else
				{
					// WANNE: Added Pulmu's weight fix
					double weight = (double)uiMinWeight + (( (double)(*pObject)[0]->data.gun.ubGunShotsLeft / Magazine[Item[(*pObject)[0]->data.gun.usGunAmmoItem].ubClassIndex].ubMagSize ) * ( (double)Item[ (*pObject)[0]->data.gun.usGunAmmoItem].ubWeight - (double)uiMinWeight )) + 0.5; //Pulmu: Account for number of rounds left.
					usWeight += (UINT16)weight;
				}
			}
			else
			{
				usWeight += Item[ (*pObject)[0]->data.gun.usGunAmmoItem ].ubWeight;
			}
		}
	}
	else if ( pItem->usItemClass == IC_AMMO && gGameExternalOptions.fAmmoDynamicWeight == TRUE )//Pulmu: added weight allowance for ammo not being full
	{
		usWeight = 0;
		//Temporary calculation for minWeight. 0.2*ubWeight rounded correctly 
		UINT32 uiMinWeight = (UINT32)((Item[pObject->usItem].ubWeight / 5.0) + 0.5);
		if( uiMinWeight < 1 || uiMinWeight > Item[pObject->usItem].ubWeight)
		{
			uiMinWeight = 1;
		}

		double weight = 0.0;
		
		for( cnt = 0; cnt < pObject->ubNumberOfObjects; cnt++ )
		{
			if((*pObject)[cnt]->data.ubShotsLeft > 0)
			{
				if( uiMinWeight == Item[pObject->usItem].ubWeight )
				{
					weight += (double)uiMinWeight;
				}
				else
				{
					weight += (double)uiMinWeight + (( (double)(*pObject)[cnt]->data.ubShotsLeft / (double)Magazine[ Item[ pObject->usItem].ubClassIndex ].ubMagSize) * ( (double)Item[pObject->usItem].ubWeight - (double)uiMinWeight ));
				}
			}
		}
		weight += 0.5; //Pulmu:To round correctly
		usWeight = (UINT16)weight;
		//Pulmu end
	}


	// make sure it really fits into that UINT8, in case we ever add anything real heavy with attachments/ammo
	// CHRISL: Remove this assertion since the next line resets usWeight to the max value
	//Assert(usWeight <= 255);

	// CHRISL: Change max value from 8bit (255) to 16bit (65535)
	if ( usWeight > 65535 )
		usWeight = 65535; //Madd: limit to 255 to prevent negative weights, at least until we can change the OBJECTTYPE structure

	// CHRISL: Return a 16bit value
	return (usWeight);
}
#endif

UINT32 CalculateCarriedWeight( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	UINT32	uiTotalWeight = 0;
	UINT32	uiPercent;
	UINT8		ubLoop;
	UINT8		ubStrengthForCarrying;

	//Pulmu: Changes for dynamic ammo weight
	for( ubLoop = 0; ubLoop < pSoldier->inv.size(); ubLoop++)
	{
		//ADB the weight of the object is already counting stacked objects, attachments, et al
		uiTotalWeight += pSoldier->inv[ubLoop].ubWeight;
	}
	// for now, assume soldiers can carry 1/2 their strength in KGs without penalty.
	// instead of multiplying by 100 for percent, and then dividing by 10 to account 
	// for weight units being in 10ths of kilos, not kilos... we just start with 10 instead of 100!
	ubStrengthForCarrying = EffectiveStrength( pSoldier );
	if ( ubStrengthForCarrying > 80 ) 
	{
		ubStrengthForCarrying += (ubStrengthForCarrying - 80);
	}
	uiPercent = (10 * uiTotalWeight) / ( ubStrengthForCarrying / 2 );
	return( uiPercent );

}

#if 0
// CHRISL: Calculate weight of item stacks for large items in new inventory system
UINT32 CalculateCarriedWeight( SOLDIERTYPE * pSoldier )
{
	UINT32	uiTotalWeight = 0;
	UINT32	uiPercent;
	UINT8		ubLoop;
	UINT16  usWeight;
	UINT8		ubStrengthForCarrying;

	//Pulmu: Changes for dynamic ammo weight
	for( ubLoop = 0; ubLoop < NUM_INV_SLOTS; ubLoop++)
	{
		usWeight = pSoldier->inv[ubLoop].ubWeight;
		if(Item[ pSoldier->inv[ubLoop].usItem].usItemClass != IC_AMMO || gGameExternalOptions.fAmmoDynamicWeight == FALSE)
		{
			if((UsingInventorySystem() == true) || Item[ pSoldier->inv[ubLoop].usItem ].ubPerPocket > 1)
			{
				// account for # of items
				usWeight *= pSoldier->inv[ubLoop].ubNumberOfObjects;
			}
		}
		uiTotalWeight += usWeight;

	}
	// for now, assume soldiers can carry 1/2 their strength in KGs without penalty.
	// instead of multiplying by 100 for percent, and then dividing by 10 to account 
	// for weight units being in 10ths of kilos, not kilos... we just start with 10 instead of 100!
	ubStrengthForCarrying = EffectiveStrength( pSoldier );
	if ( ubStrengthForCarrying > 80 ) 
	{
		ubStrengthForCarrying += (ubStrengthForCarrying - 80);
	}
	uiPercent = (10 * uiTotalWeight) / ( ubStrengthForCarrying / 2 );
	return( uiPercent );

}
#endif

void DeleteObj(OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	pObj->initialize();
}

void SwapObjs( OBJECTTYPE * pObj1, OBJECTTYPE * pObj2 )
{
	PERFORMANCE_MARKER
	OBJECTTYPE Temp (*pObj1 );
	*pObj1 = *pObj2;
	*pObj2 = Temp;
/*
	//if we are in the shop keeper interface, switch the items
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		gMoveingItem.initialize();
		gMoveingItem.fActive = TRUE;
		gMoveingItem.sItemIndex = pObj1->usItem;

		gMoveingItem.ubLocationOfObject = PLAYERS_INVENTORY;
		gMoveingItem.ubIdOfMercWhoOwnsTheItem = gpItemPointerSoldier->ubProfile;

		//Get the item from the slot.
		gMoveingItem.ItemObject = *pObj1;

	}
*/
}

void RemoveObjFrom( OBJECTTYPE * pObj, UINT8 ubRemoveIndex )
{
	PERFORMANCE_MARKER
	// remove 1 object from an OBJECTTYPE, starting at index bRemoveIndex
	if (pObj->ubNumberOfObjects < ubRemoveIndex)
	{
		// invalid index!
		return;
	}
	else if (pObj->ubNumberOfObjects == 1)
	{
		// delete!
		DeleteObj( pObj );
	} 
	else
	{
		StackedObjects::iterator iter = pObj->objectStack.begin();
		for (int x = 0; x < ubRemoveIndex; ++x) {
			++iter;
		}
		pObj->objectStack.erase(iter);
		// make the number of objects recorded match the array
		pObj->ubNumberOfObjects--;
		pObj->ubWeight = CalculateObjectWeight( pObj );
	}
}

void RemoveObjs( OBJECTTYPE * pObj, UINT8 ubNumberToRemove )
{
	PERFORMANCE_MARKER
	// remove a certain number of objects from an OBJECTTYPE, starting at index 0
	UINT8 ubLoop;

	if (ubNumberToRemove == 0)
	{
		return;
	}
	if (ubNumberToRemove >= pObj->ubNumberOfObjects)
	{
		// delete!
		DeleteObj( pObj );
	}
	else
	{
		for (ubLoop = 0; ubLoop < ubNumberToRemove; ubLoop++)
		{
			RemoveObjFrom( pObj, 0 );
		}
	}
}

void GetObjFrom( OBJECTTYPE * pObj, UINT8 ubGetIndex, OBJECTTYPE * pDest )
{
	PERFORMANCE_MARKER
	if (!pDest || ubGetIndex >= pObj->ubNumberOfObjects)
	{
		return;
	}
	if (pObj->ubNumberOfObjects == 1)
	{
		*pDest = *pObj;
		DeleteObj( pObj );	
	}
	else
	{
		//ADB TODO clean up code or change all calls getobjfrom to something that is already written
		pDest->usItem = pObj->usItem;
		(*pDest)[0]->data.objectStatus = (*pObj)[ubGetIndex]->data.objectStatus;
		pDest->ubNumberOfObjects = 1;
		pDest->ubWeight = CalculateObjectWeight( pDest );
		RemoveObjFrom( pObj, ubGetIndex );
		pObj->ubWeight = CalculateObjectWeight( pObj );
	}
}

void SwapWithinObj( OBJECTTYPE * pObj, UINT8 ubIndex1, UINT8 ubIndex2 )
{
	PERFORMANCE_MARKER
	if (pObj->ubNumberOfObjects >= ubIndex1 || pObj->ubNumberOfObjects >= ubIndex2)
	{
		return;
	}

	if (ubIndex1 == ubIndex2) {
		return;
	}

	StackedObjects::iterator left = pObj->objectStack.begin(), right = pObj->objectStack.begin();
	for (int x = 0; x < ubIndex1; ++x) {
		++left;
	}
	for (int x = 0; x < ubIndex2; ++x) {
		++right;
	}
	std::swap(left, right);
}

void DamageObj( OBJECTTYPE * pObj, INT8 bAmount )
{
	PERFORMANCE_MARKER
	//usually called from AttachObject, where the attachment is known to be a single item,
	//and the attachment is only being attached to the top of the stack
	if (bAmount >= (*pObj)[0]->data.objectStatus)
	{
		(*pObj)[0]->data.objectStatus = 1;
	}
	else
	{
		(*pObj)[0]->data.objectStatus -= bAmount;
	}
}

void CleanUpStack( OBJECTTYPE * pObj, OBJECTTYPE * pCursorObj )
{
	PERFORMANCE_MARKER
	INT8	bMaxPoints;

	if ( !(Item[ pObj->usItem ].usItemClass & IC_AMMO || Item[ pObj->usItem ].usItemClass & IC_KIT || Item[ pObj->usItem ].usItemClass & IC_MEDKIT  || Item[pObj->usItem].canteen ) )
	{
		return;
	}

	if ( Item[ pObj->usItem ].usItemClass & IC_AMMO )
	{
		bMaxPoints = Magazine[ Item[ pObj->usItem ].ubClassIndex ].ubMagSize;
	}
	else
	{
		bMaxPoints = 100;
	}

	if ( pCursorObj && pCursorObj->usItem == pObj->usItem )
	{
		DistributeStatus(pCursorObj, pObj, bMaxPoints);
	}
	DistributeStatus(pObj, pObj, bMaxPoints);
}

void DistributeStatus(OBJECTTYPE* pSourceObject, OBJECTTYPE* pTargetObject, INT8 bMaxPoints)
{
	INT8 bPointsToMove;
	for ( int bLoop = pSourceObject->ubNumberOfObjects - 1; bLoop >= 0; bLoop-- )
	{
		StackedObjectData* pSource = (*pSourceObject)[ bLoop ];
		if ( pSource->data.objectStatus > 0 )
		{
			// take the points here and distribute over the lower #d items
			int bLoop2;
			if (pSourceObject == pTargetObject) {
				//we are averaging out the same object
				bLoop2 = bLoop - 1;
			}
			else {
				//we are moving from the cursor object to this one
				bLoop2 = pTargetObject->ubNumberOfObjects - 1;
			}

			for (; bLoop2 >= 0; bLoop2-- )
			{
				StackedObjectData* pDest = (*pTargetObject)[ bLoop ];
				if ( pDest->data.objectStatus < bMaxPoints )
				{
					bPointsToMove = bMaxPoints - pDest->data.objectStatus;
					bPointsToMove = __min( bPointsToMove, pSource->data.objectStatus );

					pDest->data.objectStatus += bPointsToMove;
					pSource->data.objectStatus -= bPointsToMove;

					if ( pSource->data.objectStatus == 0 )
					{
						StackedObjects::iterator iter = pSourceObject->objectStack.begin();
						for (int x = 0; x < bLoop; ++x) {
							++iter;
						}
						pSourceObject->objectStack.erase(iter);
						pSourceObject->ubNumberOfObjects--;
						pSourceObject->ubWeight = CalculateObjectWeight(pSourceObject);
						// done!
						break;
					}
				}
			}
		}
	}
}


BOOLEAN PlaceObjectAtObjectIndex( OBJECTTYPE * pSourceObj, OBJECTTYPE * pTargetObj, UINT8 ubIndex )
{
	PERFORMANCE_MARKER
	if (pSourceObj->usItem != pTargetObj->usItem)
	{
		return( TRUE );
	}
	if (ubIndex < pTargetObj->ubNumberOfObjects)
	{
		// swap
		gTempObject.DuplicateObjectsInStack(*pSourceObj, 1);

		*((*pSourceObj)[0]) = *((*pTargetObj)[ubIndex]);
		pSourceObj->ubWeight = CalculateObjectWeight(pSourceObj);

		*((*pTargetObj)[ubIndex]) = *(gTempObject[0]);
		pTargetObj->ubWeight = CalculateObjectWeight(pTargetObj);
		return( TRUE );
	}
	else
	{
		// add to end
		pTargetObj->AddObjectsToStack( *pSourceObj, 1 );
		return( FALSE );
	}
}

#define RELOAD_NONE 0
#define RELOAD_PLACE 1
#define RELOAD_SWAP 2
#define RELOAD_TOPOFF 3
#define RELOAD_AUTOPLACE_OLD 4

BOOLEAN ReloadGun( SOLDIERTYPE * pSoldier, OBJECTTYPE * pGun, OBJECTTYPE * pAmmo )
{
	PERFORMANCE_MARKER
	UINT8			ubBulletsToMove;
	INT8			bAPs;
	UINT16			usReloadSound;
	BOOLEAN			fSameAmmoType;
	BOOLEAN			fSameMagazineSize;
	BOOLEAN			fReloadingWithStack;
	BOOLEAN			fEmptyGun;
	INT8			bReloadType;
	UINT16			usNewAmmoItem;

	bAPs = 0;

	if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		bAPs = GetAPsToReloadGunWithAmmo( pGun, pAmmo );
		if ( !EnoughPoints( pSoldier, bAPs, 0,TRUE ) )
		{
			return( FALSE );
		}
	}

	if ( Item[ pGun->usItem ].usItemClass == IC_LAUNCHER || Item[pGun->usItem].cannon )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("ReloadGun: Loading launcher - new ammo type = %d, weight = %d", pAmmo->usItem,pAmmo->ubWeight  ) );
		(*pGun)[0]->data.gun.usGunAmmoItem = pAmmo->usItem;
		if ( pGun->AttachObject( pSoldier, pAmmo ) == FALSE )
		{
			(*pGun)[0]->data.gun.usGunAmmoItem = NONE;
			// abort
			return( FALSE );
		}
	}
	else
	{
		fEmptyGun = ((*pGun)[0]->data.gun.ubGunShotsLeft == 0);
		fReloadingWithStack = (pAmmo->ubNumberOfObjects > 1);
		fSameAmmoType = ( (*pGun)[0]->data.gun.ubGunAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType );
		fSameMagazineSize = ( Magazine[ Item[ pAmmo->usItem ].ubClassIndex ].ubMagSize == GetMagSize( pGun));

		if (fEmptyGun)
		{
			bReloadType = RELOAD_PLACE;
		}
		else
		{
			// record old ammo
			CreateAmmo((*pGun)[0]->data.gun.usGunAmmoItem, &gTempObject, (*pGun)[0]->data.gun.ubGunShotsLeft);
	
			if (fSameMagazineSize)
			{
				if (fSameAmmoType)
				{
					if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && Weapon[pGun->usItem].swapClips )
					{
						bReloadType = RELOAD_SWAP;
					}
					else
					{
						bReloadType = RELOAD_TOPOFF;
					}
				}
				else
				{
					if (!fReloadingWithStack)
					{
						bReloadType = RELOAD_SWAP;
					}
					else
					{
						bReloadType = RELOAD_AUTOPLACE_OLD;
					}
				}
			}
			else  // diff sized magazines
			{
				if (fSameAmmoType)
				{
					bReloadType = RELOAD_TOPOFF;
				}
				else
				{
					bReloadType = RELOAD_AUTOPLACE_OLD;
				}
			}
		}

		if (fSameMagazineSize)
		{
			// record new ammo item for gun
			usNewAmmoItem = pAmmo->usItem;

			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[0]->data.ubShotsLeft, GetMagSize(pGun) - (*pGun)[0]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = (*pAmmo)[0]->data.ubShotsLeft;
			}

		}
		else if (Magazine[Item[pAmmo->usItem].ubClassIndex].ubMagSize > GetMagSize(pGun))
		{
//MADD MARKER
			//usNewAmmoItem = pAmmo->usItem - 1;
			usNewAmmoItem = FindReplacementMagazine(Weapon[pGun->usItem].ubCalibre ,GetMagSize(pGun),Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType);
			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[0]->data.ubShotsLeft, GetMagSize(pGun) - (*pGun)[0]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = __min( (*pAmmo)[0]->data.ubShotsLeft, GetMagSize(pGun) );
			}
		}
		else // mag is smaller than weapon mag
		{
//MADD MARKER
			//usNewAmmoItem = pAmmo->usItem + 1;
			usNewAmmoItem = FindReplacementMagazine(Weapon[pGun->usItem].ubCalibre ,GetMagSize(pGun),Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType);
			if (bReloadType == RELOAD_TOPOFF)
			{
				ubBulletsToMove = __min( (*pAmmo)[0]->data.ubShotsLeft, GetMagSize(pGun) - (*pGun)[0]->data.gun.ubGunShotsLeft );
			}
			else
			{
				ubBulletsToMove = __min( (*pAmmo)[0]->data.ubShotsLeft, GetMagSize(pGun));
			}
		}


		switch( bReloadType )
		{

			case RELOAD_PLACE:
				(*pGun)[0]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[0]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[0]->data.gun.usGunAmmoItem = usNewAmmoItem;
				break;

			case RELOAD_SWAP:
				(*pGun)[0]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[0]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[0]->data.gun.usGunAmmoItem = usNewAmmoItem;
				if (fReloadingWithStack)
				{
					// add to end of stack
					pAmmo->AddObjectsToStack( gTempObject, 1 );
				}
				else
				{
					// Copying the old ammo to the cursor in turnbased could screw up for the player
					// (suppose his inventory is full!)

					//ADB copying the old ammo to the cursor at any time will screw it up if the cursor ammo is a stack!

					//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && !EnoughPoints( pSoldier, (INT8) (bAPs + AP_PICKUP_ITEM), 0, FALSE ) )
					//{
						// try autoplace
						if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
						{
							// put it on the ground
							AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
						}
						// delete the object now in the cursor
						pAmmo->RemoveObjectsFromStack(1);
						/*
					}
					else
					{
						// copy the old ammo to the cursor
						*pAmmo = OldAmmo;
					}
					*/
				}
				break;
			case RELOAD_AUTOPLACE_OLD:
				if ( !AutoPlaceObject( pSoldier, &gTempObject, TRUE ) )
				{
					// error msg!
					return( FALSE );
				}
				// place first ammo in gun
				(*pGun)[0]->data.gun.ubGunShotsLeft = ubBulletsToMove;
				(*pGun)[0]->data.gun.ubGunAmmoType = Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType;
				(*pGun)[0]->data.gun.usGunAmmoItem = usNewAmmoItem;

				break;

			case RELOAD_TOPOFF:
				// ADD that many bullets to gun
				(*pGun)[0]->data.gun.ubGunShotsLeft += ubBulletsToMove;
				break;

		}

		if ( ! ( bReloadType == RELOAD_SWAP && !fReloadingWithStack ) )
		{
			// remove # of bullets, delete 1 object if necessary

			(*pAmmo)[0]->data.ubShotsLeft -= ubBulletsToMove;
			if ((*pAmmo)[0]->data.ubShotsLeft == 0)
			{
				RemoveObjs( pAmmo, 1 );					
			}

		}
	}

	// OK, let's play a sound of reloading too...
	// If this guy is visible...
	if ( pSoldier->bVisible != -1 )
	{
		// Play some effects!
		usReloadSound	= Weapon[ pGun->usItem ].sReloadSound;

		if ( usReloadSound != 0 && !IsAutoResolveActive() )
		{
			PlayJA2Sample( usReloadSound, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );	
		}
	}

	if (pSoldier->bTeam == gbPlayerNum)
	{
		// spit out a message if this is one of our folks reloading
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_PLAYER_RELOADS], pSoldier->name );
	}

	DeductPoints( pSoldier, bAPs, 0 );
	pGun->ubWeight = CalculateObjectWeight( pGun );

	if ( (*pGun)[0]->data.gun.bGunAmmoStatus >= 0 )
	{
		// make sure gun ammo status is 100, if gun isn't jammed
		(*pGun)[0]->data.gun.bGunAmmoStatus = 100;
	}

	(*pGun)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER; // Madd: reloading should automatically put cartridge in chamber

	return( TRUE );
}

BOOLEAN EmptyWeaponMagazine( OBJECTTYPE * pWeapon, OBJECTTYPE *pAmmo )
{
	PERFORMANCE_MARKER
	UINT16 usReloadSound;

	CHECKF( pAmmo != NULL );

	if ( (*pWeapon)[0]->data.gun.ubGunShotsLeft > 0 )
	{
		CreateAmmo((*pWeapon)[0]->data.gun.ubGunAmmoType, pAmmo, (*pWeapon)[0]->data.gun.ubGunShotsLeft);

		(*pWeapon)[0]->data.gun.ubGunShotsLeft		= 0;
		(*pWeapon)[0]->data.gun.ubGunAmmoType	  = 0;
		//(*pWeapon)[0]->data.gun.usGunAmmoItem		= 0; // leaving the ammo item the same for auto-reloading purposes

		// Play some effects!
		usReloadSound	= Weapon[ pWeapon->usItem ].sReloadSound;

		if ( usReloadSound != 0 )
		{
			PlayJA2Sample( usReloadSound, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );	
		}

		pWeapon->ubWeight = CalculateObjectWeight( pWeapon );
		// Pulmu bugfix:
		pAmmo->ubWeight = CalculateObjectWeight( pAmmo );
		// Pulmu end:
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

/*
BOOLEAN ReloadLauncher( OBJECTTYPE * pLauncher, OBJECTTYPE * pAmmo )
{
	PERFORMANCE_MARKER
	BOOLEAN			fOldAmmo;
	OBJECTTYPE	OldAmmo;

	if (pLauncher->gun.ubGunShotsLeft == 0)
	{
		fOldAmmo = FALSE;
	}
	else
	{
		if (pAmmo->ubNumberOfObjects > 1)
		{
			// can't do the swap out to the cursor
			return( FALSE );
		}
		// otherwise temporarily store the launcher's old ammo
		OldAmmo.initialize();
		fOldAmmo = TRUE;
		OldAmmo.usItem = pLauncher->gun.usGunAmmoItem;
		OldAmmo.ubNumberOfObjects = 1;
		OldAmmo.objectStatus = pLauncher->gun.bGunAmmoStatus;
	}

	// put the new ammo in the gun
	pLauncher->gun.usGunAmmoItem = pAmmo->usItem;
	pLauncher->gun.ubGunShotsLeft = 1;
	pLauncher->gun.ubGunAmmoType = AMMO_GRENADE;
	pLauncher->gun.bGunAmmoStatus = pAmmo->objectStatus;


	if (fOldAmmo)
	{
		// copy the old ammo back to the item in the cursor
		*pAmmo = OldAmmo;
	}
	else
	{
		// reduce the number of objects in the cursor by 1
		RemoveObjs( pAmmo, 1 );
	}
	return( TRUE );
}
*/

INT8 FindAmmo( SOLDIERTYPE * pSoldier, UINT8 ubCalibre, UINT8 ubMagSize, INT8 bExcludeSlot )
{
	PERFORMANCE_MARKER
	INT8				bLoop;
	INVTYPE *		pItem;

	for (bLoop = HANDPOS; bLoop < (INT8)pSoldier->inv.size(); bLoop++)
	{
		if (bLoop == bExcludeSlot)
		{
			continue;
		}
		pItem = &(Item[pSoldier->inv[bLoop].usItem]);
		if (pItem->usItemClass == IC_AMMO)
		{
			if (Magazine[pItem->ubClassIndex].ubCalibre == ubCalibre && (Magazine[pItem->ubClassIndex].ubMagSize == ubMagSize || ubMagSize == ANY_MAGSIZE))
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}


INT8 FindAmmoToReload( SOLDIERTYPE * pSoldier, INT8 bWeaponIn, INT8 bExcludeSlot )
{
	PERFORMANCE_MARKER
	OBJECTTYPE *	pObj;
	INT8					bSlot;

	if (pSoldier == NULL)
	{
		return( NO_SLOT );
	}
	pObj = &(pSoldier->inv[bWeaponIn]);

//<SB> manual recharge
	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
		return bWeaponIn;
//</SB>

	if ( Item[pObj->usItem].usItemClass == IC_GUN && !Item[pObj->usItem].cannon )
	{
		// look for same ammo as before
		bSlot = FindObjExcludingSlot( pSoldier, (*pObj)[0]->data.gun.usGunAmmoItem, bExcludeSlot );
		if (bSlot != NO_SLOT)
		{
			// reload using this ammo!
			return( bSlot );
		}
		// look for any ammo that matches which is of the same calibre and magazine size
		bSlot = FindAmmo( pSoldier, Weapon[pObj->usItem].ubCalibre, GetMagSize(pObj), bExcludeSlot );
		if (bSlot != NO_SLOT)
		{
			return( bSlot );
		}
		else
		{
			// look for any ammo that matches which is of the same calibre (different size okay)
			return( FindAmmo( pSoldier, Weapon[pObj->usItem].ubCalibre, ANY_MAGSIZE, bExcludeSlot ) );
		}
	}
	else
	{
		for(int i=0;i<MAXITEMS;i++)
		{
			if ( Launchable[i][0] == 0 && Launchable[i][1] == 0)
				break;

			if ( pObj->usItem == Launchable[i][1] )
			{
				bSlot = FindObj( pSoldier, Launchable[i][0] );

				if ( bSlot != NO_SLOT )
					return bSlot;
			}
		}
		return NO_SLOT;

		//switch( pObj->usItem )
		//{
		//	case RPG7: // TODO: madd fix this
		//		return( FindObjInObjRange( pSoldier, RPG_HE_ROCKET, RPG_FRAG_ROCKET ) );
		//	case MORTAR:
		//		return( FindObj( pSoldier, MORTAR_SHELL ) );
		//	case TANK_CANNON:
		//		return( FindObj( pSoldier, TANK_SHELL ) );
		//	case GLAUNCHER:
		//	case UNDER_GLAUNCHER:
		//		return( FindObjInObjRange( pSoldier, GL_HE_GRENADE, GL_SMOKE_GRENADE ) );
		//	default:
		//		return( NO_SLOT );
		//}
	}
}

BOOLEAN AutoReload( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	OBJECTTYPE *pObj, *pObj2;
	INT8		bSlot, bAPCost;
	BOOLEAN		fRet;

	CHECKF( pSoldier );
	pObj = &(pSoldier->inv[HANDPOS]);

//<SB> manual recharge
	if ((*pObj)[0]->data.gun.ubGunShotsLeft && !((*pObj)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
	{
		(*pObj)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;

		DeductPoints(pSoldier, Weapon[Item[(pObj)->usItem].ubClassIndex].APsToReloadManually, 0);

		PlayJA2Sample( Weapon[ Item[pObj->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{				
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
			}
		}

		return TRUE;
	}
	else
	{
		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			pObj2 = &(pSoldier->inv[SECONDHANDPOS]);

			if ((*pObj2)[0]->data.gun.ubGunShotsLeft && !((*pObj2)[0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) )
			{
				(*pObj2)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;

				DeductPoints(pSoldier, Weapon[Item[(pObj2)->usItem].ubClassIndex].APsToReloadManually, 0);

				PlayJA2Sample( Weapon[ Item[pObj2->usItem].ubClassIndex ].ManualReloadSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

				return TRUE;
			}
		}
	}
//</SB>

	if (Item[pObj->usItem].usItemClass == IC_GUN || Item[pObj->usItem].usItemClass == IC_LAUNCHER)
	{
		bSlot = FindAmmoToReload( pSoldier, HANDPOS, NO_SLOT );
		if (bSlot != NO_SLOT)
		{
			// reload using this ammo!
			fRet = ReloadGun( pSoldier, pObj, &(pSoldier->inv[bSlot]) );
			// if we are valid for two-pistol shooting (reloading) and we have enough APs still
			// then do a reload of both guns!
			if ( (fRet == TRUE) && pSoldier->IsValidSecondHandShotForReloadingPurposes( ) )
			{
				pObj = &(pSoldier->inv[SECONDHANDPOS]);
				bSlot = FindAmmoToReload( pSoldier, SECONDHANDPOS, NO_SLOT );
				if (bSlot != NO_SLOT)
				{
					// ce would reload using this ammo!
					bAPCost = GetAPsToReloadGunWithAmmo( pObj, &(pSoldier->inv[bSlot] ) );
					if ( EnoughPoints( pSoldier, (INT16) bAPCost, 0, FALSE ) )
					{
						// reload the 2nd gun too
						fRet = ReloadGun( pSoldier, pObj, &(pSoldier->inv[bSlot]) );
					}
					else
					{						
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_RELOAD_ONLY_ONE_GUN ], pSoldier->name );
					}
				}
			}

			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			return( fRet );
		}
	}

	// couldn't reload
	return( FALSE );
}

INT8 GetAttachmentComboMerge( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT8		bIndex = 0;
	INT8		bAttachLoop;

	/* check the whole Array of possible Attachements, while there are still entries ... */
	while( AttachmentComboMerge[ bIndex ].usItem != NOTHING )
	{
		/* if we found our current Object as "basic hand" item, then 
     * we have found at least ONE entry of our item (may be there are more)
     */
		OBJECTTYPE* pAttachment = 0;
		if ( pObj->usItem == AttachmentComboMerge[ bIndex ].usItem )
		{
			// search for all the appropriate attachments 
		  /* every ComboMerge must have at least one attachments Field */
			for ( bAttachLoop = 0; bAttachLoop < 2; bAttachLoop++ )
			{
				/* if the none of both Fields contains anything, do not merge */
				if ( AttachmentComboMerge[ bIndex ].usAttachment[ bAttachLoop ] == NOTHING )
				{
					continue;
				}

			/* 2007-05-27, Sgt_Kolja: 
			 * do not return, but break the inner loop moved away, otherwise 
			 * we can only have ONE attachmentCombo per basic item. F.I. if we want
			 * to make a Dart gun from Dart pistol by adding (a buttstock and) wheter a
			 * steel tube /or/ a Gun Barrel Extender, the old code wouldn't work for
			 * the Gun Barel Extender, since it would never been tested.
			 */
				pAttachment = FindAttachment( pObj, AttachmentComboMerge[ bIndex ].usAttachment[ bAttachLoop ] );
				if ( pAttachment == 0 )
				{
					// didn't find something required
					break;
				}
			}
				// found everything required?
		  /* 2007-05-27, Sgt_Kolja: Not-found-condition moved from above, otherwise we can only have ONE attachmentCombo per basic item */
			if ( pAttachment )
			{
				return( bIndex );
			}
		} /* end-if-this-is-our-item */

		/* try next Attachment Order */
		bIndex++;
	}

	return( -1 );
}



void PerformAttachmentComboMerge( OBJECTTYPE * pObj, INT8 bAttachmentComboMerge )
{
	PERFORMANCE_MARKER
	INT8		bAttachLoop;
	UINT32	uiStatusTotal = 0;
	INT8		bNumStatusContributors = 0;

	// This object has been validated as available for an attachment combo merge.
	// - find all attachments in list and destroy them
	// - status of new object should be average of items including attachments
	// - change object

	for ( bAttachLoop = 0; bAttachLoop < 2; bAttachLoop++ )
	{
		if ( AttachmentComboMerge[ bAttachmentComboMerge ].usAttachment[ bAttachLoop ] == NOTHING )
		{
			continue;
		}

		OBJECTTYPE* pAttachment = FindAttachment( pObj, AttachmentComboMerge[ bAttachmentComboMerge ].usAttachment[ bAttachLoop ] );
		AssertMsg( pAttachment != 0, String( "Attachment combo merge couldn't find a necessary attachment" ) );
		
		uiStatusTotal += (*pAttachment)[0]->data.objectStatus;
		bNumStatusContributors++;

		pObj->RemoveAttachment(pAttachment);
	}

	uiStatusTotal += (*pObj)[0]->data.objectStatus;
	bNumStatusContributors++;

	pObj->usItem = AttachmentComboMerge[ bAttachmentComboMerge ].usResult;
	(*pObj)[0]->data.objectStatus = (INT8) (uiStatusTotal / bNumStatusContributors );
}

BOOLEAN OBJECTTYPE::AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound )
{
	PERFORMANCE_MARKER
	if ( this->ubNumberOfObjects > 1 )
	{
		return( FALSE );
	}
	if (pAttachment->ubNumberOfObjects == 0) {
		return FALSE;
	}

	static OBJECTTYPE attachmentObject;

	UINT16		usResult, usResult2;
	INT8		bLoop;
	UINT8		ubType, ubLimit, ubAPCost;
	INT32		iCheckResult;
	INT8		bAttachInfoIndex = -1, bAttachComboMerge;
	BOOLEAN		fValidLaunchable = FALSE;

	bool canOnlyAttach1 = false;

	//if this is an attachment or ammo for a launchable item
	fValidLaunchable = ValidLaunchable( pAttachment->usItem, this->usItem );
	if ( fValidLaunchable || ValidItemAttachment( this, pAttachment->usItem, TRUE ) )
	{
		//if there is already an attachment of the same type, we want to try swapping / replacing it
		OBJECTTYPE* pAttachmentPosition = 0;

		// find an attachment position... 
		// second half of this 'if' is for attaching GL grenades to a gun w/attached GL
		if ( fValidLaunchable || (Item[pAttachment->usItem].glgrenade && FindAttachmentByClass(this, IC_LAUNCHER) != 0 ) )
		{
			// try replacing if possible
			pAttachmentPosition = FindAttachmentByClass( this, Item[ pAttachment->usItem ].usItemClass );
			if ( pAttachmentPosition )
			{
				canOnlyAttach1 = true;
				// we can only do a swap if there is only 1 grenade being attached
			}
		}
		else
		{
			// try replacing if possible
			pAttachmentPosition = FindAttachment( this, pAttachment->usItem );
		}

		if (pAttachmentPosition) {
			if (canOnlyAttach1 == true) {
				//we have requested to swap the attachment on the cursor with the current attachment
				//the problem is, the attachment object is a stack, and can only attach 1, so exit!
			}
		}

		if ( pSoldier )
		{
			//did the soldier damage it?
			bAttachInfoIndex = GetAttachmentInfoIndex( pAttachment->usItem );
			// in-game (not behind the scenes) attachment
			if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck != NO_CHECK )
			{
				iCheckResult = SkillCheck( pSoldier, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheck, AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod );
				if (iCheckResult < 0)		
				{
					// the attach failure damages both items
					DamageObj( this, (INT8) -iCheckResult );
					DamageObj( pAttachment, (INT8) -iCheckResult );

					// there should be a quote here!
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					if ( gfInItemDescBox )
					{
						DeleteItemDescriptionBox();
					}
					return( FALSE );
				}
			}

			if ( ValidItemAttachment( this, pAttachment->usItem, TRUE ) && playSound ) // not launchable
			{
				// attachment sounds
				if ( Item[ this->usItem ].usItemClass & IC_WEAPON )
				{
					PlayJA2Sample( ATTACH_TO_GUN, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_ARMOUR )
				{
					PlayJA2Sample( ATTACH_CERAMIC_PLATES, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else if ( Item[ this->usItem ].usItemClass & IC_BOMB )
				{
					PlayJA2Sample( ATTACH_DETONATOR, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
			}
		}

		//ADB moved after skill check!
		//lalien: added to make a GL/RL work when reloaded manually
		// the AP costs for reloading GL/RL will be taken from weapons.xml ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )
		if (pAttachmentPosition) {
			//we know we are replacing this attachment
			if ( Item[ this->usItem ].usItemClass == IC_LAUNCHER || Item[this->usItem].cannon )
			{
				if ( fValidLaunchable )
				{
					(*this)[0]->data.gun.usGunAmmoItem = pAttachment->usItem;
					//we have reloaded a launchable, so the ammo is gone from the original object
				}
			}
		}

		//unfortunately must come before possible attachment swap
		if (Item[pAttachment->usItem].grenadelauncher )
		{
			// transfer any attachments from the grenade launcher to the gun
			(*this)[0]->attachments.splice((*this)[0]->attachments.begin(), (*pAttachment)[0]->attachments,
				(*pAttachment)[0]->attachments.begin(), (*pAttachment)[0]->attachments.end());
		}

		if (pAttachmentPosition) {
			//we are swapping the attachments, and we know we do NOT need to worry about attachment stack size
			//backup the original attachment
			attachmentObject = *pAttachmentPosition;

			//place the new one
			*pAttachmentPosition = *pAttachment;

			//whatever pAttachment pointed to is now the original attachment
			*pAttachment = attachmentObject;
		}
		else {
			//it's a new attachment
			if (canOnlyAttach1 == true) {
				//we only placed one of the stack, pAttachment could have any number of objects
				if (pAttachment->RemoveObjectsFromStack(1, &attachmentObject) == 0) {
					(*this)[0]->attachments.push_back(attachmentObject);
				}
			}
			else {
				//pAttachment could have any number of objects
				(*this)[0]->attachments.push_back(*pAttachment);
			}
		}


		// Check for attachment merge combos here
		bAttachComboMerge = GetAttachmentComboMerge( this );
		if ( bAttachComboMerge != -1 )
		{
			PerformAttachmentComboMerge( this, bAttachComboMerge );
			if ( bAttachInfoIndex != -1 && AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod < 20 )
			{
				StatChange( pSoldier, MECHANAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
				StatChange( pSoldier, WISDOMAMT, (INT8) ( 20 - AttachmentInfo[ bAttachInfoIndex ].bAttachmentSkillCheckMod ), FALSE );
			}
		}

		this->ubWeight = CalculateObjectWeight( this );
		pAttachment->ubWeight = CalculateObjectWeight(pAttachment);

		if ( pSoldier != NULL )
			ApplyEquipmentBonuses(pSoldier);

		return( TRUE );
	}
	// check for merges
	else if (EvaluateValidMerge( pAttachment->usItem, this->usItem, &usResult, &usResult2, &ubType, &ubAPCost ))	
	{
		if ( ubType != COMBINE_POINTS )
		{
			if ( pSoldier )
			{
				if ( !EnoughPoints( pSoldier, ubAPCost, 0, TRUE ) )
				{
					return( FALSE );
				}

				//lalien: don't charge AP's for reloading a GL/RL ( wrong place!!! the AP's are deducted in DeleteItemDescriptionBox() )
				//if ( IsAGLorRL == FALSE )
				//{
					DeductPoints( pSoldier, ubAPCost, 0 );
				//}
			}
		}

		switch( ubType )
		{
			case USE_ITEM:
			case USE_ITEM_HARD:
				// the merge will combine the two items
				if ( pSoldier )
				{
					if ( ubType == USE_ITEM_HARD )
					{
						// requires a skill check, and gives experience 				
						iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ITEM_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}				
						StatChange( pSoldier, MECHANAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );
					}


					//Madd: note that use_item cannot produce two different items!!! so it doesn't use usResult2

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[0]->data.gun.usGunAmmoItem != NONE && (*this)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, this);
						}
					}

					RemoveProhibitedAttachments(pSoldier, this, usResult);

					this->usItem = usResult;
					//AutoPlaceObject( pAttachment );
					this->ubWeight = CalculateObjectWeight( this );
					if (pSoldier->bTeam == gbPlayerNum)
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
					}
					ApplyEquipmentBonuses(pSoldier);
					return TRUE;
				}
				else
					return FALSE;
			case COMBINE_POINTS:
				// transfer points...
				if ( Item[ this->usItem ].usItemClass == IC_AMMO )
				{
					ubLimit = Magazine[ Item[ this->usItem ].ubClassIndex ].ubMagSize;
				}
				else
				{
					ubLimit = 100;
				}

				// count down through # of attaching items and add to status of item in position 0
				for (bLoop = pAttachment->ubNumberOfObjects - 1; bLoop >= 0; bLoop--)
				{
					if ((*this)[0]->data.objectStatus + (*pAttachment)[bLoop]->data.objectStatus <= ubLimit)
					{
						// consume this one totally and continue
						(*this)[0]->data.objectStatus += (*pAttachment)[bLoop]->data.objectStatus;
						RemoveObjFrom( pAttachment, bLoop );
						// reset loop limit
						bLoop = pAttachment->ubNumberOfObjects; // add 1 to counteract the -1 from the loop
					}
					else
					{
						// add part of this one and then we're done
						(*pAttachment)[bLoop]->data.objectStatus -= (ubLimit - (*this)[0]->data.objectStatus);
						(*this)[0]->data.objectStatus = ubLimit;
						break;
					}
				}
				break;
			case DESTRUCTION:
				// the merge destroyed both items!
				this->RemoveObjectsFromStack(1);
				pAttachment->RemoveObjectsFromStack(1);
				if ( pSoldier )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
				}
				break;
			case ELECTRONIC_MERGE:
				if ( pSoldier ) 
				{
					iCheckResult = SkillCheck( pSoldier, ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK, -30 );
					if ( iCheckResult < 0 )
					{
						DamageObj( this, (INT8) -iCheckResult );
						DamageObj( pAttachment, (INT8) -iCheckResult );
						pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
						return( FALSE );
					}
					// grant experience!
				}
				// fall through
			case EXPLOSIVE:
				if ( ubType == EXPLOSIVE ) /// coulda fallen through
				{
					if (pSoldier)
					{
						// requires a skill check, and gives experience 				
						iCheckResult = SkillCheck( pSoldier, ATTACHING_DETONATOR_CHECK, -30 );
						if (iCheckResult < 0)
						{
							// could have a chance of detonation
							// for now, damage both objects
							DamageObj( this, (INT8) -iCheckResult );
							DamageObj( pAttachment, (INT8) -iCheckResult );
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							return( FALSE );
						}				
						StatChange( pSoldier, EXPLODEAMT, 25, FALSE );
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );
					}
				}
				// fall through
			default:
				// the merge will combine the two items
				//Madd: usResult2 only works for standard merges->item1 + item2 = item3 + item4

				//Madd: unload guns after merge if ammo caliber or mag size don't match
				if ( Item[this->usItem].usItemClass == IC_GUN && (*this)[0]->data.gun.usGunAmmoItem != NONE && (*this)[0]->data.gun.ubGunShotsLeft > 0 )
				{
					if ( Item[usResult].usItemClass != IC_GUN || Weapon[Item[usResult].ubClassIndex].ubCalibre != Weapon[Item[this->usItem].ubClassIndex].ubCalibre || (*this)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult].ubClassIndex].ubMagSize )
					{ // item types/calibers/magazines don't match, spit out old ammo
						EjectAmmoAndPlace(pSoldier, this);
					}
				}

				RemoveProhibitedAttachments(pSoldier, this, usResult);

				this->usItem = usResult;
				if ( ubType != TREAT_ARMOUR )
				{
					(*this)[0]->data.objectStatus = ((*this)[0]->data.objectStatus + (*pAttachment)[0]->data.objectStatus) / 2;
				}

				this->ubWeight = CalculateObjectWeight( this );

				if ( usResult2 != NOTHING )
				{
					//Madd: usResult2 is what the original attachment/source item turns into

					//Madd: unload guns after merge if ammo caliber or mag size don't match
					if ( Item[pAttachment->usItem].usItemClass == IC_GUN && (*pAttachment)[0]->data.gun.usGunAmmoItem != NONE && (*pAttachment)[0]->data.gun.ubGunShotsLeft > 0 )
					{
						if ( Item[usResult2].usItemClass != IC_GUN || Weapon[Item[usResult2].ubClassIndex].ubCalibre != Weapon[Item[pAttachment->usItem].ubClassIndex].ubCalibre || (*pAttachment)[0]->data.gun.ubGunShotsLeft > Weapon[Item[usResult2].ubClassIndex].ubMagSize )
						{ // item types/calibers/magazines don't match, spit out old ammo
							EjectAmmoAndPlace(pSoldier, pAttachment);
						}
					}

					RemoveProhibitedAttachments(pSoldier, pAttachment, usResult2);

					pAttachment->usItem = usResult2;
					if ( ubType != TREAT_ARMOUR )
					{
						(*pAttachment)[0]->data.objectStatus = ((*pAttachment)[0]->data.objectStatus + (*this)[0]->data.objectStatus) / 2;
					}
					pAttachment->ubWeight = CalculateObjectWeight( pAttachment );
				}
				else	
					pAttachment->RemoveObjectsFromStack(1);

				if (pSoldier && pSoldier->bTeam == gbPlayerNum)
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
				}
				break;
			}
			if ( pSoldier != NULL )
				ApplyEquipmentBonuses(pSoldier);
			return( TRUE );
	}
	return( FALSE );
}

void RemoveProhibitedAttachments(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj, UINT16 usItem)
{
	//Madd: remove any prohibited attachments
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (!ValidAttachment(iter->usItem, usItem) && !ValidLaunchable(iter->usItem, usItem) )
		{
			if ( !Item[iter->usItem].inseparable )
			{
				if (pSoldier) {
					if ( !AutoPlaceObject( pSoldier, &(*iter), FALSE ) )
					{   // put it on the ground
						AddItemToPool( pSoldier->sGridNo, &(*iter), 1, pSoldier->pathing.bLevel, 0 , -1 );
					}
				}
			}
			pObj->RemoveAttachment(&(*iter));
		}
	}
	return;
}

void EjectAmmoAndPlace(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj)
{
	CreateAmmo((*pObj)[0]->data.gun.usGunAmmoItem, &gTempObject, (*pObj)[0]->data.gun.ubGunShotsLeft);
	(*pObj)[0]->data.gun.ubGunShotsLeft = 0;
	(*pObj)[0]->data.gun.usGunAmmoItem = NONE;
	if ( pSoldier )
	{
		if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
		{   // put it on the ground
			AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
		}
	}
	return;
}

/* CHRISL: This function is edited to handle the new inventory system when we have an item in our cursor.
Not only do we have to hatch out pockets that the item won't fit in, we also have to hatch out pockets that
our current LBE gear haven't activated.  We'll also need to display the number of items of the type currently
held in the cursor that each active pocket can hold.*/
extern BOOLEAN CompatibleAmmoForGun( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject );
BOOLEAN CanItemFitInVehicle( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement )
{
	if((UsingInventorySystem() == false) || !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
		return(FALSE);
	if(!vehicleInv[bPos])
		return(FALSE);

	UINT8					ubSlotLimit;
	ubSlotLimit = ItemSlotLimit( pObj, bPos, pSoldier );

	if ( ubSlotLimit == 0 )
		return ( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );

	return( TRUE );
}


//ADB TODO
BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement )
{
	PERFORMANCE_MARKER
	UINT8					ubSlotLimit, lbePocket=1;
	INT8					bNewPos=ITEM_NOT_FOUND;
	UINT32					pRestrict=0;

	// CHRISL: Only check valid pockets
	if((UsingInventorySystem() == false) && !oldInv[bPos])
		return(FALSE);
	if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
		return(CanItemFitInVehicle(pSoldier, pObj, bPos, fDoingPlacement));

	ubSlotLimit = ((UsingInventorySystem() == false)) ? ItemSlotLimit( pObj->usItem, bPos ) : ItemSlotLimit( pObj, bPos, pSoldier );

	switch( bPos )
	{
		case SECONDHANDPOS:
			if (Item[pSoldier->inv[HANDPOS].usItem].twohanded )
			{
				return( FALSE );
			}
			break;
		case HANDPOS:
			if (Item[ pObj->usItem ].twohanded )
			{
				if (pSoldier->inv[HANDPOS].usItem != NOTHING && pSoldier->inv[SECONDHANDPOS].usItem != NOTHING)
				{
					// two items in hands; try moving the second one so we can swap 
					// CHRISL: Adjust parameters to include the new inventory system
					if (Item[pSoldier->inv[SECONDHANDPOS].usItem].ubPerPocket == 0)
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCK1POS, (BIGPOCKFINAL-1) );
					}
					else
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCK1POS, (NUM_INV_SLOTS-1) );
					}

					if (bNewPos == NO_SLOT)
					{
						// nowhere to put second item
						return( FALSE );
					}

					if ( fDoingPlacement )
					{
						// otherwise move it.
						pSoldier->inv[bNewPos] = pSoldier->inv[SECONDHANDPOS];
						DeleteObj( &(pSoldier->inv[SECONDHANDPOS]) );
					}
				}
			}
			break;
		case VESTPOS:
		case HELMETPOS:
		case LEGPOS:
			if (Item[pObj->usItem].usItemClass != IC_ARMOUR)
			{
				return( FALSE );
			}
			switch (bPos)
			{
				case VESTPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_VEST)
					{
						return( FALSE );
					}		
					break;
				case HELMETPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_HELMET)
					{
						return( FALSE );
					}		
					break;
				case LEGPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_LEGGINGS)
					{
						return( FALSE );
					}		
					break;
				default:
					break;
			}
			break;
		case HEAD1POS:
		case HEAD2POS:
			if (Item[pObj->usItem].usItemClass != IC_FACE)
			{
				return( FALSE );
			}
		case VESTPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != VEST_PACK)
			{
				return( FALSE );
			}
			break;
		case LTHIGHPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != THIGH_PACK)
			{
				return( FALSE );
			}
			break;
		case RTHIGHPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != THIGH_PACK)
			{
				return( FALSE );
			}
			break;
		case CPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != COMBAT_PACK)
			{
				return( FALSE );
			}
			if(pSoldier->inv[BPACKPOCKPOS].usItem != NOTHING)
			{
				if(LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo != LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo ||
				LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case BPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != BACKPACK)
			{
				return( FALSE );
			}
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[GUNSLINGPOCKPOS].usItem != NOTHING)
			//	return( FALSE );
			if(pSoldier->inv[CPACKPOCKPOS].usItem != NOTHING)
			{
				if(LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo != LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo ||
				LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case GUNSLINGPOCKPOS:	// Gun Sling
			//if (Item[pObj->usItem].usItemClass != IC_GUN && Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_LAUNCHER)
			if(Item[pObj->usItem].usItemClass == IC_AMMO || Item[pObj->usItem].usItemClass == IC_GRENADE)
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]));
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[BPACKPOCKPOS].usItem != NOTHING)
			//	return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]));
			break;
		case KNIFEPOCKPOS:	// Knife sheath
			if (Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_THROWING_KNIFE )
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[KNIFEPOCKPOS]));
			break;
		// IC Pockets
		case BIGPOCK1POS:
		case BIGPOCK2POS:
		case BIGPOCK3POS:
		case BIGPOCK4POS:
		case BIGPOCK5POS:
		case BIGPOCK6POS:
		case BIGPOCK7POS:
		case MEDPOCK1POS:
		case MEDPOCK2POS:
		case MEDPOCK3POS:
		case MEDPOCK4POS:
		case SMALLPOCK2POS:
		case SMALLPOCK3POS:
		case SMALLPOCK4POS:
		case SMALLPOCK5POS:
		case SMALLPOCK6POS:
		case SMALLPOCK7POS:
		case SMALLPOCK8POS:
		case SMALLPOCK9POS:
		case SMALLPOCK10POS:
		case SMALLPOCK11POS:
		case SMALLPOCK12POS:
		case SMALLPOCK13POS:
		case SMALLPOCK14POS:
		case SMALLPOCK15POS:
		case SMALLPOCK16POS:
		case SMALLPOCK17POS:
		case SMALLPOCK18POS:
		case SMALLPOCK19POS:
		case SMALLPOCK20POS:
		case SMALLPOCK21POS:
		case SMALLPOCK22POS:
		case SMALLPOCK23POS:
		case SMALLPOCK24POS:
		case SMALLPOCK25POS:
		case SMALLPOCK26POS:
		case SMALLPOCK27POS:
		case SMALLPOCK28POS:
		case SMALLPOCK29POS:
		case SMALLPOCK30POS:
			if((UsingInventorySystem() == true))
			{
				if(icLBE[bPos] == BPACKPOCKPOS && (!(pSoldier->flags.ZipperFlag) || (pSoldier->flags.ZipperFlag && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)) && (gTacticalStatus.uiFlags & INCOMBAT))
					return( FALSE );
				lbePocket = (pSoldier->inv[icLBE[bPos]].usItem == NOTHING) ? LoadBearingEquipment[icClass[bPos]].lbePocketIndex[icPocket[bPos]] : LoadBearingEquipment[Item[pSoldier->inv[icLBE[bPos]].usItem].ubClassIndex].lbePocketIndex[icPocket[bPos]];
				pRestrict = LBEPocketType[lbePocket].pRestriction;
				if(pRestrict != 0)
					if(pRestrict != Item[pObj->usItem].usItemClass)
						lbePocket = 0;
			}
			break;
		default:
			break;
	}

	if((UsingInventorySystem() == false))
	{
		if (ubSlotLimit == 0 && bPos >= SMALLPOCK1POS )
		{
			// doesn't fit!
			return( FALSE );
		}
	}
	else
	{
		// CHRISL: lbePocket==0 means pocket disabled.  ubSlotLimit==0 means pocket can't hold item
		if ( lbePocket == 0 || ubSlotLimit == 0 )
			return ( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );

		// CHRISL: Adjust parameters to include the new inventory system
		if (ubSlotLimit == 0 && bPos >= BIGPOCKFINAL )
			return( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );
	}

	return( TRUE );
}
/*
BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement )
{
	UINT8					ubSlotLimit, lbePocket=1;
	INT8					bNewPos=ITEM_NOT_FOUND;
	UINT32					pRestrict=0;

	// CHRISL: Only check valid pockets
	if((UsingInventorySystem() == false) && !oldInv[bPos])
		return(FALSE);
	if((pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
		return(CanItemFitInVehicle(pSoldier, pObj, bPos, fDoingPlacement));

	ubSlotLimit = ((UsingInventorySystem() == false)) ? ItemSlotLimit( pObj->usItem, bPos ) : ItemSlotLimit( pObj, bPos, pSoldier );

	switch( bPos )
	{
		case SECONDHANDPOS:
//			if (Item[pSoldier->inv[HANDPOS].usItem].fFlags & ITEM_TWO_HANDED)
			if (Item[pSoldier->inv[HANDPOS].usItem].twohanded )
			{
				return( FALSE );
			}
			break;
		case HANDPOS:
//			if (Item[ pObj->usItem ].fFlags & ITEM_TWO_HANDED)
			if (Item[ pObj->usItem ].twohanded )
			{
				if (pSoldier->inv[HANDPOS].usItem != NOTHING && pSoldier->inv[SECONDHANDPOS].usItem != NOTHING)
				{
					// two items in hands; try moving the second one so we can swap 
					// CHRISL: Adjust parameters to include the new inventory system
					if (Item[pSoldier->inv[SECONDHANDPOS].usItem].ubPerPocket == 0)
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCK1POS, (BIGPOCKFINAL-1) );
					}
					else
					{
						bNewPos = FindEmptySlotWithin( pSoldier, BIGPOCK1POS, (NUM_INV_SLOTS-1) );
					}
					if (bNewPos == NO_SLOT)
					{
						// nowhere to put second item
						return( FALSE );
					}

					if ( fDoingPlacement )
					{
						// otherwise move it.
						pSoldier->inv[bNewPos] = pSoldier->inv[SECONDHANDPOS];
						DeleteObj( &(pSoldier->inv[SECONDHANDPOS]) );
					}
				}
			}
			break;
		case VESTPOS:
		case HELMETPOS:
		case LEGPOS:
			if (Item[pObj->usItem].usItemClass != IC_ARMOUR)
			{
				return( FALSE );
			}
			switch (bPos)
			{
				case VESTPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_VEST)
					{
						return( FALSE );
					}		
					break;
				case HELMETPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_HELMET)
					{
						return( FALSE );
					}		
					break;
				case LEGPOS:
					if (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass != ARMOURCLASS_LEGGINGS)
					{
						return( FALSE );
					}		
					break;
				default:
					break;
			}
			break;
		case HEAD1POS:
		case HEAD2POS:
			if (Item[pObj->usItem].usItemClass != IC_FACE)
			{
				return( FALSE );
			}
			break;
		case VESTPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != VEST_PACK)
			{
				return( FALSE );
			}
			break;
		case LTHIGHPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != THIGH_PACK)
			{
				return( FALSE );
			}
			break;
		case RTHIGHPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != THIGH_PACK)
			{
				return( FALSE );
			}
			break;
		case CPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != COMBAT_PACK)
			{
				return( FALSE );
			}
			if(pSoldier->inv[BPACKPOCKPOS].usItem != NOTHING)
			{
				if(LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo != LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo ||
				LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case BPACKPOCKPOS:
			if (Item[pObj->usItem].usItemClass != IC_LBEGEAR || LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass != BACKPACK)
			{
				return( FALSE );
			}
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[GUNSLINGPOCKPOS].usItem != NOTHING)
			//	return( FALSE );
			if(pSoldier->inv[CPACKPOCKPOS].usItem != NOTHING)
			{
				if(LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo != LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo ||
				LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo == 0)
				{
					return( FALSE );
				}
			}
			break;
		case GUNSLINGPOCKPOS:	// Gun Sling
			//if (Item[pObj->usItem].usItemClass != IC_GUN && Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_LAUNCHER)
			if(Item[pObj->usItem].usItemClass == IC_AMMO || Item[pObj->usItem].usItemClass == IC_GRENADE)
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]));
			// Removed backpack/gunsling restrictions
			//if(pSoldier->inv[BPACKPOCKPOS].usItem != NOTHING)
			//	return(CompatibleAmmoForGun(pObj, &pSoldier->inv[GUNSLINGPOCKPOS]));
			break;
		case KNIFEPOCKPOS:	// Knife sheath
			if (Item[pObj->usItem].usItemClass != IC_BLADE && Item[pObj->usItem].usItemClass != IC_THROWING_KNIFE )
				return(CompatibleAmmoForGun(pObj, &pSoldier->inv[KNIFEPOCKPOS]));
			break;
		// IC Pockets
		case BIGPOCK1POS:
		case BIGPOCK2POS:
		case BIGPOCK3POS:
		case BIGPOCK4POS:
		case BIGPOCK5POS:
		case BIGPOCK6POS:
		case BIGPOCK7POS:
		case MEDPOCK1POS:
		case MEDPOCK2POS:
		case MEDPOCK3POS:
		case MEDPOCK4POS:
		case SMALLPOCK2POS:
		case SMALLPOCK3POS:
		case SMALLPOCK4POS:
		case SMALLPOCK5POS:
		case SMALLPOCK6POS:
		case SMALLPOCK7POS:
		case SMALLPOCK8POS:
		case SMALLPOCK9POS:
		case SMALLPOCK10POS:
		case SMALLPOCK11POS:
		case SMALLPOCK12POS:
		case SMALLPOCK13POS:
		case SMALLPOCK14POS:
		case SMALLPOCK15POS:
		case SMALLPOCK16POS:
		case SMALLPOCK17POS:
		case SMALLPOCK18POS:
		case SMALLPOCK19POS:
		case SMALLPOCK20POS:
		case SMALLPOCK21POS:
		case SMALLPOCK22POS:
		case SMALLPOCK23POS:
		case SMALLPOCK24POS:
		case SMALLPOCK25POS:
		case SMALLPOCK26POS:
		case SMALLPOCK27POS:
		case SMALLPOCK28POS:
		case SMALLPOCK29POS:
		case SMALLPOCK30POS:
			if((UsingInventorySystem() == true))
			{
				if(icLBE[bPos] == BPACKPOCKPOS && (!(pSoldier->flags.ZipperFlag) || (pSoldier->flags.ZipperFlag && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)) && (gTacticalStatus.uiFlags & INCOMBAT))
					return( FALSE );
				lbePocket = (pSoldier->inv[icLBE[bPos]].usItem == NOTHING) ? LoadBearingEquipment[icClass[bPos]].lbePocketIndex[icPocket[bPos]] : LoadBearingEquipment[Item[pSoldier->inv[icLBE[bPos]].usItem].ubClassIndex].lbePocketIndex[icPocket[bPos]];
				pRestrict = LBEPocketType[lbePocket].pRestriction;
				if(pRestrict != 0)
					if(pRestrict != Item[pObj->usItem].usItemClass)
						lbePocket = 0;
			}
			break;
		default:
			break;
	}

	if((UsingInventorySystem() == false))
	{
		if (ubSlotLimit == 0 && bPos >= SMALLPOCK1POS )
		{
			// doesn't fit!
			return( FALSE );
		}
	}
	else
	{
		// CHRISL: lbePocket==0 means pocket disabled.  ubSlotLimit==0 means pocket can't hold item
		if ( lbePocket == 0 || ubSlotLimit == 0 )
			return ( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );

		// CHRISL: Adjust parameters to include the new inventory system
		if (ubSlotLimit == 0 && bPos >= BIGPOCKFINAL )
			return( CompatibleAmmoForGun(pObj, &pSoldier->inv[bPos]) );
	}

	return( TRUE );
}


END TODO
*/
BOOLEAN DropObjIfThereIsRoom( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	// try autoplacing item in bSlot elsewhere, then do a placement
	BOOLEAN fAutoPlacedOld;

	fAutoPlacedOld = AutoPlaceObject( pSoldier, &(pSoldier->inv[bPos]), FALSE );
	if ( fAutoPlacedOld )
	{
		return( PlaceObject( pSoldier, bPos, pObj ) );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN PlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	// returns object to have in hand after placement... same as original in the
	// case of error

	UINT8					ubSlotLimit, ubNumberToDrop;
	OBJECTTYPE *	pInSlot;
	BOOLEAN				fObjectWasRobotRemote = FALSE;

	if ( Item[pObj->usItem].robotremotecontrol )
	{
		fObjectWasRobotRemote = TRUE;
	}

	if ( !CanItemFitInPosition( pSoldier, pObj, bPos, TRUE ) )
	{
		return( FALSE );
	}

	// If the position is either head slot, then the item must be IC_FACE (checked in
	// CanItemFitInPosition).
	if ( bPos == HEAD1POS )
	{
		if ( !CompatibleFaceItem( pObj->usItem, pSoldier->inv[ HEAD2POS ].usItem ) )
		{
			CHAR16	zTemp[ 150 ];

			swprintf( zTemp, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ pObj->usItem ], ItemNames[ pSoldier->inv[ HEAD2POS ].usItem ] );
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			return( FALSE );
		}
	}
	else if ( bPos == HEAD2POS )
	{
		if ( !CompatibleFaceItem( pObj->usItem, pSoldier->inv[ HEAD1POS ].usItem ) )
		{
			CHAR16	zTemp[ 150 ];

			swprintf( zTemp, Message[ STR_CANT_USE_TWO_ITEMS ], ItemNames[ pObj->usItem ], ItemNames[ pSoldier->inv[ HEAD1POS ].usItem ] );
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			return( FALSE );
		}
	}

	if ( Item[ pObj->usItem ].usItemClass == IC_KEY && pSoldier->flags.uiStatusFlags & SOLDIER_PC )
	{
		if ( KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound == 0 )
		{
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound = (UINT16) GetWorldDay();
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usSectorFound = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
		}
	}

    // Lesh: bugfix - replacing weapon in auto with another weapon w/o auto-mode 
    if (bPos == HANDPOS && Item[ pObj->usItem ].usItemClass == IC_GUN)
    {
		//Madd: added code for nosemiauto tag
		if (!Weapon[ Item[pObj->usItem].ubClassIndex ].NoSemiAuto)
		{
			pSoldier->bWeaponMode = WM_NORMAL;
			pSoldier->bDoBurst = FALSE;
			pSoldier->bDoAutofire = FALSE;
		}
		else
		{
			pSoldier->bWeaponMode = WM_AUTOFIRE;
			pSoldier->bDoBurst = TRUE;
			pSoldier->bDoAutofire = TRUE;
		}
    }
    // Lesh: end

	// CHRISL:
	ubSlotLimit = ((UsingInventorySystem() == false)) ? ItemSlotLimit( pObj->usItem, bPos ) : ItemSlotLimit( pObj, bPos, pSoldier );

	pInSlot = &(pSoldier->inv[bPos]);

	if (pInSlot->ubNumberOfObjects == 0)
	{
		// placement in an empty slot
		ubNumberToDrop = pObj->ubNumberOfObjects;

		if (ubNumberToDrop > __max( ubSlotLimit, 1 ) )
		{
			// drop as many as possible into pocket
			ubNumberToDrop = __max( ubSlotLimit, 1 );
		}

		// could be wrong type of object for slot... need to check...
		// but assuming it isn't
		*pInSlot = *pObj;
/*
		//if we are in the shopkeeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			gMoveingItem.initialize();
			SetSkiCursor( CURSOR_NORMAL );
		}
*/

		if (ubNumberToDrop != pObj->ubNumberOfObjects)
		{
			// in the InSlot copy, zero out all the objects we didn't drop
			for (int x = ubNumberToDrop; x < pObj->ubNumberOfObjects; ++x) {
				pObj->objectStack.pop_back();
			}
		}
		pInSlot->ubNumberOfObjects = ubNumberToDrop;

		// remove a like number of objects from pObj
		RemoveObjs( pObj, ubNumberToDrop );
		if (pObj->ubNumberOfObjects == 0)
		{
			// dropped everything
//			if (bPos == HANDPOS && Item[pInSlot->usItem].fFlags & ITEM_TWO_HANDED)
			if (bPos == HANDPOS && Item[pInSlot->usItem].twohanded )
			{
				// We just performed a successful drop of a two-handed object into the
				// main hand
				if (pSoldier->inv[SECONDHANDPOS].usItem != 0)
				{
					// swap what WAS in the second hand into the cursor
					SwapObjs( pObj, &(pSoldier->inv[SECONDHANDPOS]));
				}
			}
		 }

	}
	else
	{
		// replacement/reloading/merging/stacking	
		// keys have an additional check for key ID being the same
		if ( (pObj->usItem == pInSlot->usItem) && ( Item[ pObj->usItem ].usItemClass != IC_KEY || (*pObj)[0]->data.key.ubKeyID == (*pInSlot)[0]->data.key.ubKeyID ) )
		{
			if (Item[ pObj->usItem ].usItemClass == IC_MONEY)
			{
	
				UINT32 uiMoneyMax = MoneySlotLimit( bPos );

				// always allow money to be combined!
				// IGNORE STATUS!

				if ((*pInSlot)[0]->data.money.uiMoneyAmount + (*pObj)[0]->data.money.uiMoneyAmount > uiMoneyMax)
				{
					// remove X dollars
					(*pObj)[0]->data.money.uiMoneyAmount -= (uiMoneyMax - (*pInSlot)[0]->data.money.uiMoneyAmount);
					// set in slot to maximum
					(*pInSlot)[0]->data.money.uiMoneyAmount = uiMoneyMax;
				}
				else
				{
					(*pInSlot)[0]->data.money.uiMoneyAmount += (*pObj)[0]->data.money.uiMoneyAmount;
					DeleteObj( pObj );
/*
					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						gMoveingItem.initialize();
						SetSkiCursor( CURSOR_NORMAL );
					}
*/
				}
			}
			// CHRISL
			else if ( ubSlotLimit == 1 || (ubSlotLimit == 0 && bPos >= HANDPOS && bPos < BIGPOCKFINAL ) )
			{
				if (pObj->ubNumberOfObjects <= 1)
				{
					// swapping
					SwapObjs( pObj, pInSlot );
				}
				else
				{
					return( DropObjIfThereIsRoom( pSoldier, bPos, pObj ) );
				}
			}
			else if (ubSlotLimit == 0) // trying to drop into a small pocket
			{
				return( DropObjIfThereIsRoom( pSoldier, bPos, pObj ) );
			}
			else
			{
				// stacking
				ubNumberToDrop = ubSlotLimit - pInSlot->ubNumberOfObjects;
				if (ubNumberToDrop > pObj->ubNumberOfObjects)
				{
					ubNumberToDrop = pObj->ubNumberOfObjects;
				}
				pInSlot->AddObjectsToStack( *pObj, ubNumberToDrop );
			}
		}
		else
		{
			// replacement, unless reloading...	
			switch (Item[pInSlot->usItem].usItemClass)
			{
				case IC_GUN:
					if (Item[pObj->usItem].usItemClass == IC_AMMO)
					{
						if (Weapon[pInSlot->usItem].ubCalibre == Magazine[Item[pObj->usItem].ubClassIndex].ubCalibre)
						{
							// reload... 
							return( ReloadGun( pSoldier, pInSlot, pObj ) );
						}
						else
						{
							// invalid ammo
							break;
							//return( FALSE );
						}
					}
					break;
				case IC_LAUNCHER:
				{			
					if ( ValidLaunchable( pObj->usItem, pInSlot->usItem ) )
					{
						// reload... 						
						return( ReloadGun( pSoldier, pInSlot, pObj ) );
					}
				}
				break;
			}

//			if ( (Item[pObj->usItem].fFlags & ITEM_TWO_HANDED) && (bPos == HANDPOS) )
			if ( (Item[pObj->usItem].twohanded ) && (bPos == HANDPOS) )
			{
				if (pSoldier->inv[SECONDHANDPOS].usItem != 0)
				{
					// both pockets have something in them, so we can't swap
					return( FALSE );
				}
				else
				{
					SwapObjs( pObj, pInSlot );	
				}
			}
			else if (pObj->ubNumberOfObjects <= __max( ubSlotLimit, 1 ) )
			{
				// swapping
				SwapObjs( pObj, pInSlot );
			}
			else
			{
				return( DropObjIfThereIsRoom( pSoldier, bPos, pObj ) );
			}

		}
	}

	// ATE: Put this in to see if we should update the robot, if we were given a controller...
	if ( pSoldier->bTeam == gbPlayerNum && fObjectWasRobotRemote )
	{
		pSoldier->UpdateRobotControllerGivenController( );
	}
	
	ApplyEquipmentBonuses(pSoldier);
	//Pulmu bugfix
	pInSlot->ubWeight = CalculateObjectWeight(pInSlot);
	//Pulmu end
	return( TRUE );
}

// CHRISL: Function needed for LBENODE
extern BOOLEAN MoveItemFromLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj );

BOOLEAN InternalAutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem, INT8 bExcludeSlot )
{
	PERFORMANCE_MARKER
	INT8			bSlot;
	INVTYPE	* pItem;
	UINT8			ubPerSlot, packCombo, backCombo;

	// statuses of extra objects would be 0 if the # exceeds the maximum
	Assert( pObj->ubNumberOfObjects <= MAX_OBJECTS_PER_SLOT);

	//Pulmu bugfix		
	pObj->ubWeight = CalculateObjectWeight( pObj);
	pItem = &(Item[pObj->usItem]);
	ubPerSlot = pItem->ubPerPocket;

	// Overrides to the standard system: put guns in hand, armour on body (if slot empty)
	switch (pItem->usItemClass)
	{
		case IC_GUN:
			// CHRISL: 
			if((UsingInventorySystem() == true) && (pItem->twohanded) && pSoldier->inv[GUNSLINGPOCKPOS].usItem == NONE)	// Long Gun use Gun Sling
			{
				PlaceObject( pSoldier, GUNSLINGPOCKPOS, pObj );
				SetNewItem( pSoldier, GUNSLINGPOCKPOS, fNewItem );
				if (pObj->ubNumberOfObjects == 0)
					return( TRUE );
			}
		case IC_BLADE:
			// CHRISL:
			if((UsingInventorySystem() == true) && pSoldier->inv[KNIFEPOCKPOS].usItem == NONE)	// Knife
			{
				PlaceObject( pSoldier, KNIFEPOCKPOS, pObj );
				SetNewItem( pSoldier, KNIFEPOCKPOS, fNewItem );
				if (pObj->ubNumberOfObjects == 0)
					return( TRUE );
			}
		case IC_LAUNCHER:
		case IC_BOMB:
		case IC_GRENADE:
//			if (!((*pItem).fFlags & ITEM_TWO_HANDED))
			if (!(pItem->twohanded))
			{
				if (pSoldier->inv[HANDPOS].usItem == NONE)
				{
					// put the one-handed weapon in the guy's hand...
					PlaceObject( pSoldier, HANDPOS, pObj );
					SetNewItem( pSoldier, HANDPOS, fNewItem );
					if ( pObj->ubNumberOfObjects == 0 )
					{
						return( TRUE );
					}
				}
//				else if ( !(Item[pSoldier->inv[HANDPOS].usItem].fFlags & ITEM_TWO_HANDED) && pSoldier->inv[SECONDHANDPOS].usItem == NONE)
				else if ( !(Item[pSoldier->inv[HANDPOS].usItem].twohanded ) && pSoldier->inv[SECONDHANDPOS].usItem == NONE)
				{
					// put the one-handed weapon in the guy's 2nd hand...
					PlaceObject( pSoldier, SECONDHANDPOS, pObj );
					SetNewItem( pSoldier, SECONDHANDPOS, fNewItem );
					if ( pObj->ubNumberOfObjects == 0 )
					{
						return( TRUE );
					}
				}
			}
			// two-handed objects are best handled in the main loop for large objects,
			// which checks the hands first anyhow			
			break;

		case IC_ARMOUR:
			switch (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass)
			{
				case ARMOURCLASS_VEST:
					if (pSoldier->inv[VESTPOS].usItem == NONE)
					{
						// put on the armour!
						PlaceObject( pSoldier, VESTPOS, pObj );
						SetNewItem( pSoldier, VESTPOS, fNewItem );
						if ( pObj->ubNumberOfObjects == 0 )
						{
							return( TRUE );
						}
					}
					break;
				case ARMOURCLASS_LEGGINGS:
					// CHRISL:
					if(Item[pSoldier->inv[LEGPOS].usItem].attachment)
					{
						//ADB TODO, figure out what this code does, and fix it
						/*
						pObj->usAttachItem[0] = pSoldier->inv[LEGPOS].usItem;
						pObj->bAttachStatus[0] = pSoldier->inv[LEGPOS].ItemData.Generic.bStatus[0];
						pSoldier->inv[LEGPOS].usItem = NONE;
						*/
					}
					if (pSoldier->inv[LEGPOS].usItem == NONE)
					{
						// put on the armour!
						PlaceObject( pSoldier, LEGPOS, pObj );
						SetNewItem( pSoldier, LEGPOS, fNewItem );
						if ( pObj->ubNumberOfObjects == 0 )
						{
							return( TRUE );
						}
					}
					break;
				case ARMOURCLASS_HELMET:
					if (pSoldier->inv[HELMETPOS].usItem == NONE)
					{
						// put on the armour!
						PlaceObject( pSoldier, HELMETPOS, pObj );
						SetNewItem( pSoldier, HELMETPOS, fNewItem );
						if ( pObj->ubNumberOfObjects == 0 )
						{
							return( TRUE );
						}
					}
					break;
				default:
					break;
			}
			// otherwise stuff it in a slot somewhere
			break;
		case IC_FACE:
			if ( (pSoldier->inv[HEAD1POS].usItem == NOTHING) && CompatibleFaceItem( pObj->usItem, pSoldier->inv[HEAD2POS].usItem ) )
			{
				PlaceObject( pSoldier, HEAD1POS, pObj );
				SetNewItem( pSoldier, HEAD1POS, fNewItem );
				if ( pObj->ubNumberOfObjects == 0 )
				{
					return( TRUE );
				}
			}
			else if ( (pSoldier->inv[HEAD2POS].usItem == NOTHING) && CompatibleFaceItem( pObj->usItem, pSoldier->inv[HEAD1POS].usItem ) )
			{
				PlaceObject( pSoldier, HEAD2POS, pObj );
				SetNewItem( pSoldier, HEAD2POS, fNewItem );
				if ( pObj->ubNumberOfObjects == 0 )
				{
					return( TRUE );
				}
			}
			break;
		// CHRISL:
		case IC_LBEGEAR:
			if((UsingInventorySystem() == true))
			{
				if(pSoldier->inv[LTHIGHPOCKPOS].usItem == NOTHING && LoadBearingEquipment[pItem->ubClassIndex].lbeClass == THIGH_PACK)	// Thigh pack
				{
					MoveItemFromLBEItem( pSoldier, LTHIGHPOCKPOS, pObj);
					PlaceObject( pSoldier, LTHIGHPOCKPOS, pObj );
					SetNewItem( pSoldier, LTHIGHPOCKPOS, fNewItem );
					if(pObj->ubNumberOfObjects == 0)
						return( TRUE );
				}
				else if(pSoldier->inv[RTHIGHPOCKPOS].usItem == NOTHING && LoadBearingEquipment[pItem->ubClassIndex].lbeClass == THIGH_PACK)	// Thigh pack
				{
					MoveItemFromLBEItem( pSoldier, RTHIGHPOCKPOS, pObj);
					PlaceObject( pSoldier, RTHIGHPOCKPOS, pObj );
					SetNewItem( pSoldier, RTHIGHPOCKPOS, fNewItem );
					if(pObj->ubNumberOfObjects == 0)
						return( TRUE );
				}
				else if(pSoldier->inv[VESTPOCKPOS].usItem == NOTHING && LoadBearingEquipment[pItem->ubClassIndex].lbeClass == VEST_PACK)	// Vest pack
				{
					MoveItemFromLBEItem( pSoldier, VESTPOCKPOS, pObj);
					PlaceObject( pSoldier, VESTPOCKPOS, pObj );
					SetNewItem( pSoldier, VESTPOCKPOS, fNewItem );
					if(pObj->ubNumberOfObjects == 0)
						return( TRUE );
				}
				else if(pSoldier->inv[CPACKPOCKPOS].usItem == NOTHING && LoadBearingEquipment[pItem->ubClassIndex].lbeClass == COMBAT_PACK)	// Combat pack
				{
					packCombo = LoadBearingEquipment[pItem->ubClassIndex].lbeCombo;
					backCombo = LoadBearingEquipment[Item[pSoldier->inv[BPACKPOCKPOS].usItem].ubClassIndex].lbeCombo;
					if((pSoldier->inv[BPACKPOCKPOS].usItem != NOTHING && packCombo != 0 && backCombo == packCombo) || pSoldier->inv[BPACKPOCKPOS].usItem == NOTHING)
					{
						MoveItemFromLBEItem( pSoldier, CPACKPOCKPOS, pObj);
						PlaceObject( pSoldier, CPACKPOCKPOS, pObj );
						SetNewItem( pSoldier, CPACKPOCKPOS, fNewItem );
						if(pObj->ubNumberOfObjects == 0)
							return( TRUE );
					}
				}
				else if(pSoldier->inv[BPACKPOCKPOS].usItem == NOTHING && LoadBearingEquipment[pItem->ubClassIndex].lbeClass == BACKPACK)	// Backpack
				{
					if(pSoldier->inv[GUNSLINGPOCKPOS].usItem == NOTHING)
					{
						packCombo = LoadBearingEquipment[Item[pSoldier->inv[CPACKPOCKPOS].usItem].ubClassIndex].lbeCombo;
						backCombo = LoadBearingEquipment[pItem->ubClassIndex].lbeCombo;
						if((pSoldier->inv[CPACKPOCKPOS].usItem != NOTHING && backCombo != 0 && backCombo == packCombo) || pSoldier->inv[CPACKPOCKPOS].usItem == NOTHING)
						{
							MoveItemFromLBEItem( pSoldier, BPACKPOCKPOS, pObj);
							PlaceObject( pSoldier, BPACKPOCKPOS, pObj );
							SetNewItem( pSoldier, BPACKPOCKPOS, fNewItem );
							pSoldier->flags.DropPackFlag = FALSE;
							pSoldier->flags.ZipperFlag = FALSE;
							RenderBackpackButtons(0);
							if(pObj->ubNumberOfObjects == 0)
								return( TRUE );
						}
					}
				}
			}
			break;
		default:
			break;
	}

	if (ubPerSlot == 0)
	{
		// Large object; look for an empty hand/large pocket and dump it in there
		// FindObjWithin with 0 will search for empty slots!
		// Madd
		//bSlot = HANDPOS;
		//while (1) 
		// CHRISL
		for( bSlot = HANDPOS; bSlot < BIGPOCKFINAL; bSlot++)
		{	
			bSlot = FindEmptySlotWithin( pSoldier, bSlot, BIGPOCK4POS );
			if (bSlot == ITEM_NOT_FOUND)
			{
				return( FALSE );
			}
			if (bSlot == SECONDHANDPOS)
			{
				if (pSoldier->inv[HANDPOS].usItem != NONE)
				{
//					bSlot++;
					continue;
				}
			}
			// this might fail if we're trying to place in HANDPOS,
			// and SECONDHANDPOS is full
			PlaceObject( pSoldier, bSlot, pObj );
			SetNewItem( pSoldier, bSlot, fNewItem );
			if (pObj->ubNumberOfObjects == 0)
			{
				return( TRUE );
			}
//			bSlot++;
		} 
	}
	else
	{
		// Small items; don't allow stack/dumping for keys right now as that
		// would require a bunch of functions for finding the same object by two values...
		if ( ubPerSlot > 1 || Item[ pObj->usItem ].usItemClass == IC_KEY || Item[ pObj->usItem ].usItemClass == IC_MONEY )
		{
			// First, look for slots with the same object, and dump into them.
			bSlot = HANDPOS;
			while (1) 
			{
				// CHRISL
				bSlot = FindObjWithin( pSoldier, pObj->usItem, bSlot, (NUM_INV_SLOTS-1) );
				if (bSlot == ITEM_NOT_FOUND)
				{
					break;
				}
				if ( bSlot != bExcludeSlot )
				{
					// CHRISL: Use new ItemSlotLimit function if we're using the new inventory system
					UINT8 isLimit = ((UsingInventorySystem() == false)) ? ItemSlotLimit( pObj->usItem, bSlot ) : ItemSlotLimit( pObj, bSlot, pSoldier );
					if ( ( (Item[ pObj->usItem ].usItemClass == IC_MONEY) && pSoldier->inv[ bSlot ][0]->data.money.uiMoneyAmount < MoneySlotLimit( bSlot ) ) || (Item[ pObj->usItem ].usItemClass != IC_MONEY && pSoldier->inv[bSlot].ubNumberOfObjects < isLimit ) )
					{
						// NEW: If in SKI, don't auto-place anything into a stackable slot that's currently hatched out!  Such slots
						// will disappear in their entirety if sold/moved, causing anything added through here to vanish also!
						if( !( ( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) && ShouldSoldierDisplayHatchOnItem( pSoldier->ubProfile, bSlot ) ) )
						{
							PlaceObject( pSoldier, bSlot, pObj );
							SetNewItem( pSoldier, bSlot, fNewItem );
							if (pObj->ubNumberOfObjects == 0)
							{
								return( TRUE );
							}
						}
					}
				}
				bSlot++;
			}
		}
		// Search for empty slots to dump into, starting with small pockets
		bSlot = BIGPOCKFINAL;	/* CHRISL */
		while( 1 )
		{
			// CHRISL
			bSlot = FindEmptySlotWithin( pSoldier, bSlot, (NUM_INV_SLOTS-1) );
			if (bSlot == ITEM_NOT_FOUND)
			{
				break;
			}
			PlaceObject( pSoldier, bSlot, pObj );
			SetNewItem( pSoldier, bSlot, fNewItem );
			if (pObj->ubNumberOfObjects == 0)
			{
				return( TRUE );
			}
			bSlot++;
		}
		// now check hands/large pockets
		bSlot = HANDPOS;
		while (1)		
		{
			// CHRISL
			bSlot = FindEmptySlotWithin( pSoldier, bSlot, (BIGPOCKFINAL-1) );
			if (bSlot == ITEM_NOT_FOUND)
			{
				break;
			}
			PlaceObject( pSoldier, bSlot, pObj );
			SetNewItem( pSoldier, bSlot, fNewItem );
			if (pObj->ubNumberOfObjects == 0)
			{
				return( TRUE );
			}
			bSlot++;
		}
	}
	return( FALSE );
}

BOOLEAN AutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem )
{
	PERFORMANCE_MARKER
	return( InternalAutoPlaceObject( pSoldier, pObj, fNewItem, NO_SLOT ) );
}

BOOLEAN RemoveObjectFromSlot( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	CHECKF( pObj );
	if (pSoldier->inv[bPos].ubNumberOfObjects == 0)
	{
		return( FALSE );
	}
	else
	{
		*pObj = pSoldier->inv[bPos];
		DeleteObj( &(pSoldier->inv[bPos]) );
		return( TRUE );
	}
}

BOOLEAN RemoveKeyFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT8 ubItem = 0;

	CHECKF( pObj );
	
	if( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 ) || ( pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID == INVALID_KEY_NUMBER ) )
	{
		return( FALSE );
	}
	else
	{
		//*pObj = pSoldier->inv[bPos];
		
		// create an object
		ubItem = pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID;

		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber > 1 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber--;
		}
		else
		{
			
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = 0;
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = INVALID_KEY_NUMBER;
		}

		return( CreateKeyObject( pObj, 1, ubItem ) );
	}

	return( FALSE );
}


BOOLEAN RemoveKeysFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, UINT8 ubNumberOfKeys ,OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT8 ubItems = 0;

	CHECKF( pObj );
	

	if( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 ) || ( pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID == INVALID_KEY_NUMBER ) )
	{
		return( FALSE );
	}
	else
	{
		//*pObj = pSoldier->inv[bPos];
		
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber < ubNumberOfKeys )
		{
			ubNumberOfKeys = pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber;
		}

		
		ubItems = pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID;
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber - ubNumberOfKeys > 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber--;
		}
		else
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = 0;
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = INVALID_KEY_NUMBER;
		}

		// create an object
		return( CreateKeyObject( pObj, ubNumberOfKeys, ubItems ) );
	}
}

// return number added
UINT8 AddKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT8 ubNumberNotAdded = 0;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC ) // redundant but what the hey
	{
		if ( KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound == 0 )
		{
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usDateFound = (UINT16) GetWorldDay();
			KeyTable[ (*pObj)[0]->data.key.ubKeyID ].usSectorFound = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
		}
	}

	// check if we are going to far
	if ( ( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber + pObj->ubNumberOfObjects ) > Item[ pObj->usItem ].ubPerPocket )
	{
		// only take what we can
		ubNumberNotAdded = pObj->ubNumberOfObjects - ( Item[ pObj->usItem ].ubPerPocket - pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber );
		
		// set to max
		pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = Item[ pObj->usItem ].ubPerPocket; 
		
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;
		}

		// return number used
		return( pObj->ubNumberOfObjects - ubNumberNotAdded );
	}
	else 
	{
		// check 
		if( pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber == 0 )
		{
			pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;
		}

		pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber += pObj->ubNumberOfObjects;
	}

	return( pObj->ubNumberOfObjects );
}

UINT8 SwapKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	// swap keys in keyring slot and keys in pocket
	// create temp object to hold keys currently in key ring slot
	CreateKeyObject( &gTempObject, pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber, pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID );

	pSoldier->pKeyRing[ bKeyRingPosition ].ubNumber = pObj->ubNumberOfObjects;
	pSoldier->pKeyRing[ bKeyRingPosition ].ubKeyID = (*pObj)[0]->data.key.ubKeyID;
	
	// swap params?
	*pObj = gTempObject;

	return( 1 );
}


BOOLEAN CreateKeyObject( OBJECTTYPE * pObj , UINT8 ubNumberOfKeys, UINT8 ubKeyID )
{
	PERFORMANCE_MARKER
	BOOLEAN fRet;

	pObj->initialize();
	fRet = CreateItems( (UINT16) (FIRST_KEY + LockTable[ ubKeyID ].usKeyItem), 100, ubNumberOfKeys, pObj );
	if (fRet)
	{
		(*pObj)[0]->data.key.ubKeyID = ubKeyID;
	}
	//fRet = CreateItems( (UINT16)(ubKeyIdValue + FIRST_KEY) , 100, ubNumberOfKeys, pObj )
	//return(  );
	return( fRet );
}


BOOLEAN AllocateObject( OBJECTTYPE **pObj )
{
	PERFORMANCE_MARKER
	// create a key object
	*pObj = new OBJECTTYPE;
	Assert( pObj );

	return( TRUE );
}

BOOLEAN DeleteKeyObject( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if( pObj == FALSE )
	{
		return( FALSE );
	}

	// free up space
	delete( pObj );

	return( TRUE );
}

UINT16 TotalPoints( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT16	usPoints = 0;
	UINT8		ubLoop;

	for (ubLoop = 0; ubLoop < pObj->ubNumberOfObjects; ubLoop++)
	{
		usPoints += (*pObj)[ubLoop]->data.objectStatus;
	}
	return( usPoints );
}

UINT16 UseKitPoints( OBJECTTYPE * pObj, UINT16 usPoints, SOLDIERTYPE *pSoldier )
{
	PERFORMANCE_MARKER
	// start consuming from the last kit in, so we end up with fewer fuller kits rather than
	// lots of half-empty ones.
	INT8		bLoop;
	UINT16 usOriginalPoints = usPoints;

	for (bLoop = pObj->ubNumberOfObjects - 1; bLoop >= 0; bLoop--)
	{
		if (Item[pObj->usItem].percentstatusdrainreduction  > 0 && ((usPoints * (100 - Item[pObj->usItem].percentstatusdrainreduction))/100) < (*pObj)[bLoop]->data.objectStatus )
		{
			(*pObj)[bLoop]->data.objectStatus -= (INT8) ((usPoints * (100 - Item[pObj->usItem].percentstatusdrainreduction ) )/100);
			return( usOriginalPoints );
		}
		else if (usPoints < (UINT16) (*pObj)[bLoop]->data.objectStatus)
		{
			(*pObj)[bLoop]->data.objectStatus -= (INT8) usPoints;
			return( usOriginalPoints );
		}
		else
		{
			// consume this kit totally
			usPoints -= (*pObj)[bLoop]->data.objectStatus;
			(*pObj)[bLoop]->data.objectStatus = 0;

			pObj->ubNumberOfObjects--;
		}
	}

	// check if pocket/hand emptied..update inventory, then update panel
	if( pObj->ubNumberOfObjects == 0 )
	{
		// Delete object
		DeleteObj( pObj );

		// dirty interface panel
		DirtyMercPanelInterface(  pSoldier, DIRTYLEVEL2 );
	}

	return( usOriginalPoints -  usPoints );
}

#ifdef PATHAI_VISIBLE_DEBUG

	extern BOOLEAN gfDrawPathPoints;

#else

	#ifdef AI_TIMING_TESTS
	extern UINT32 guiGreenTimeTotal, guiYellowTimeTotal, guiRedTimeTotal, guiBlackTimeTotal;
	extern UINT32 guiGreenCounter, guiYellowCounter, guiRedCounter, guiBlackCounter;
	extern UINT32 guiRedSeekTimeTotal, guiRedHelpTimeTotal, guiRedHideTimeTotal;
	extern UINT32 guiRedSeekCounter, guiRedHelpCounter; guiRedHideCounter;
	#endif

#endif


UINT16 MagazineClassIndexToItemType(UINT16 usMagIndex)
{
	PERFORMANCE_MARKER
	UINT16				usLoop;

	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index usMagIndex
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType" ) );
	for (usLoop = FIRST_AMMO; usLoop < MAXITEMS; usLoop++)  
	{
		if ( Item[usLoop].usItemClass  == 0 )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: breaking at index %d", usLoop ) );
			break;
		}
		if (Item[usLoop].ubClassIndex == usMagIndex && Item[usLoop].usItemClass == IC_AMMO )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: return %d", usLoop ) );
			return( usLoop );
		}
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "MagazineClassIndexToItemType: return none, got to %d", usLoop ) );
	return(NONE);
}


UINT16 DefaultMagazine( UINT16 usItem )
{
	PERFORMANCE_MARKER
	WEAPONTYPE *	pWeapon;
	UINT16				usLoop;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: item = %d",usItem));
	if (!(Item[usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[usItem]);
	usLoop = 0;
	while ( Magazine[usLoop].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == pWeapon->ubMagSize &&
				AmmoTypes[ Magazine[usLoop].ubAmmoType ].standardIssue )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: found at index %d",usLoop));
			return(MagazineClassIndexToItemType(usLoop));
		}

		usLoop++;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DefaultMagazine: can't find any"));
	return( 0 );
}

UINT16 FindReplacementMagazine( UINT8 ubCalibre, UINT8 ubMagSize, UINT8 ubAmmoType )
{
	PERFORMANCE_MARKER
	UINT16 usLoop;
	UINT16 usDefault;
	
	usLoop = 0;
	usDefault = NOTHING;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: calibre = %d, Mag size = %d, ammo type = %d",ubCalibre,ubMagSize,ubAmmoType));

	while ( Magazine[usLoop].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == ubCalibre &&
				Magazine[usLoop].ubMagSize == ubMagSize )
		{
			if ( Magazine[usLoop].ubAmmoType == ubAmmoType )
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: returning item = %d",MagazineClassIndexToItemType( usLoop )));
				return( MagazineClassIndexToItemType( usLoop ) );
			}
			else if ( usDefault == NOTHING )
			{
				// store this one to use if all else fails
				usDefault = MagazineClassIndexToItemType( usLoop );
			}
			
		}

		usLoop++;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FindReplacementMagazine: returning default item = %d",usDefault));
	return( usDefault );

}

UINT16 FindReplacementMagazineIfNecessary( UINT16 usOldGun, UINT16 usOldAmmo, UINT16 usNewGun )
{
	PERFORMANCE_MARKER
	UINT16 usNewAmmo = NOTHING;

	if ( (Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubCalibre == Weapon[ usOldGun ].ubCalibre) &&
			 (Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubMagSize == Weapon[ usOldGun ].ubMagSize) )
	{
		// must replace this!
		usNewAmmo = FindReplacementMagazine( Weapon[ usNewGun ].ubCalibre, Weapon[ usNewGun ].ubMagSize, Magazine[ Item[ usOldAmmo ].ubClassIndex ].ubAmmoType );
	}

	return( usNewAmmo );
}

// increase this if any gun can have more types that this
#define MAX_AMMO_TYPES_PER_GUN		24  // MADD MARKER

UINT16 RandomMagazine( UINT16 usItem, UINT8 ubPercentStandard )
{
	PERFORMANCE_MARKER
	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index ubLoop
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine (by index)"));

	WEAPONTYPE *	pWeapon;
	UINT16				usLoop;
	UINT16				usPossibleMagIndex[ MAX_AMMO_TYPES_PER_GUN ];
	UINT16				usPossibleMagCnt = 0;
	UINT8					ubMagChosen;

	if (!(Item[usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[usItem]);

	// find & store all possible mag types that fit this gun
	usLoop = 0;
	while ( Magazine[ usLoop ].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == pWeapon->ubMagSize && ItemIsLegal(MagazineClassIndexToItemType(usLoop)))
		{
			// store it! (make sure array is big enough)
			Assert(usPossibleMagCnt < MAX_AMMO_TYPES_PER_GUN);
			// Madd: check to see if allowed by army
			if ( gArmyItemChoices[ENEMYAMMOTYPES].ubChoices > 0 )
			{
				for ( int i=0;i<gArmyItemChoices[ENEMYAMMOTYPES].ubChoices;i++ )
				{
					if ( gArmyItemChoices[ENEMYAMMOTYPES].bItemNo[i] == Magazine[usLoop].ubAmmoType )
					{
						usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
						break;
					}
				}
			}
			else
			{
				usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
			}
		}

		usLoop++;
	}

	// no matches?
	if (usPossibleMagCnt == 0)
	{
		return( 0 );
	}
	else
	// only one match?
	if (usPossibleMagCnt == 1)
	{
		// use that, no choice
		return(MagazineClassIndexToItemType( usPossibleMagIndex[ 0 ] ));
	}
	else	// multiple choices
	{
		// Pick one at random, using supplied probability to pick the default
		if (Random(100) < ubPercentStandard)
		{
			ubMagChosen = 0;
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
			return( DefaultMagazine(usItem) );
		}
		else
		{
			// pick a non-standard type instead
			ubMagChosen = ( UINT8 ) (1 + Random(( UINT32 ) ( usPossibleMagCnt - 1 )));
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
		return( MagazineClassIndexToItemType(usPossibleMagIndex[ ubMagChosen ] ) );
	}
}

UINT16 RandomMagazine( OBJECTTYPE * pGun, UINT8 ubPercentStandard )
{
	PERFORMANCE_MARKER
	// Note: if any ammo items in the item table are separated from the main group,
	// this function will have to be rewritten to scan the item table for an item
	// with item class ammo, which has class index ubLoop
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine"));

	WEAPONTYPE *	pWeapon;
	UINT16				usLoop;
	UINT16				usPossibleMagIndex[ MAX_AMMO_TYPES_PER_GUN ];
	UINT16				usPossibleMagCnt = 0;
	UINT8					ubMagChosen;

	if (!(Item[pGun->usItem].usItemClass & IC_GUN))
	{
		return( 0 );
	}

	pWeapon = &(Weapon[pGun->usItem]);

	// find & store all possible mag types that fit this gun
	usLoop = 0;
	while ( Magazine[ usLoop ].ubCalibre != NOAMMO )
	{
		if (Magazine[usLoop].ubCalibre == pWeapon->ubCalibre &&
				Magazine[usLoop].ubMagSize == GetMagSize(pGun) && ItemIsLegal(MagazineClassIndexToItemType(usLoop)))
		{
			// store it! (make sure array is big enough)
			Assert(usPossibleMagCnt < MAX_AMMO_TYPES_PER_GUN);
			// Madd: check to see if allowed by army
			if ( gArmyItemChoices[ENEMYAMMOTYPES].ubChoices > 0 )
			{
				for ( int i=0;i<gArmyItemChoices[ENEMYAMMOTYPES].ubChoices;i++ )
				{
					if ( gArmyItemChoices[ENEMYAMMOTYPES].bItemNo[i] == Magazine[usLoop].ubAmmoType )
					{
						usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
						break;
					}
				}
			}
			else
			{
				usPossibleMagIndex[usPossibleMagCnt++] = usLoop;
			}
		}

		usLoop++;
	}

	// no matches?
	if (usPossibleMagCnt == 0)
	{
		return( 0 );
	}
	else
	// only one match?
	if (usPossibleMagCnt == 1)
	{
		// use that, no choice
		return(MagazineClassIndexToItemType(usPossibleMagIndex[ 0 ] ));
	}
	else	// multiple choices
	{
		// Pick one at random, using supplied probability to pick the default
		if (Random(100) < ubPercentStandard)
		{
			ubMagChosen = 0;
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
			return( DefaultMagazine(pGun->usItem) );
		}
		else
		{
			// pick a non-standard type instead
			ubMagChosen = ( UINT8 ) (1 + Random(( UINT32 ) ( usPossibleMagCnt - 1 )));
		}

		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("RandomMagazine: ubMagChosen = %d",ubMagChosen ));
		return( MagazineClassIndexToItemType(usPossibleMagIndex[ ubMagChosen ] ) );
	}
}

BOOLEAN CreateGun( UINT16 usItem, INT8 bStatus, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT16 usAmmo;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateGun: usItem = %d",usItem));


	Assert( pObj != NULL);
	if ( pObj == NULL )
	{
		return( FALSE );
	}

	pObj->initialize();
	pObj->usItem = usItem;
	StackedObjectData* pStackedObject = (*pObj)[0];
	pStackedObject->data.gun.bGunStatus = bStatus;
	pStackedObject->data.ubImprintID = NO_PROFILE;

	if (Weapon[ usItem ].ubWeaponClass == MONSTERCLASS)
	{
		pStackedObject->data.gun.ubGunShotsLeft = GetMagSize(pObj);
		pStackedObject->data.gun.ubGunAmmoType = AMMO_MONSTER;
	}
	else if ( EXPLOSIVE_GUN( usItem ) )
	{
		if ( Item[usItem].singleshotrocketlauncher ) 
		{
			pStackedObject->data.gun.ubGunShotsLeft = 1;
		}
		else
		{
			// cannon
			pStackedObject->data.gun.ubGunShotsLeft = 0;
		}
		pStackedObject->data.gun.bGunAmmoStatus = 100;
		pStackedObject->data.gun.ubGunAmmoType = 0;
	}
	else
	{
		usAmmo = DefaultMagazine( usItem );
		Assert( usAmmo != 0 );
		if (usAmmo == 0)
		{
			// item's calibre & mag size not found in magazine list!
			return( FALSE );
		}
		else
		{
			pStackedObject->data.gun.usGunAmmoItem = usAmmo;
			pStackedObject->data.gun.ubGunAmmoType = Magazine[ Item[ usAmmo ].ubClassIndex].ubAmmoType;
			pStackedObject->data.gun.bGunAmmoStatus = 100;
			pStackedObject->data.gun.ubGunShotsLeft = Magazine[ Item[ usAmmo ].ubClassIndex ].ubMagSize;
			pStackedObject->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER; // Madd: new guns should have cartridge in chamber
		}
	}

	pObj->ubWeight = CalculateObjectWeight( pObj );
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateGun: Done"));
	// succesful
	return( TRUE );
}

BOOLEAN CreateAmmo( UINT16 usItem, OBJECTTYPE * pObj, INT16 ubShotsLeft )
{
	PERFORMANCE_MARKER
	if (pObj == NULL)
	{
		return( FALSE );
	}
	pObj->initialize();
	pObj->usItem = usItem;
	pObj->ubNumberOfObjects = 1;
	if (ubShotsLeft < 0) {
		(*pObj)[0]->data.ubShotsLeft = Magazine[ Item[usItem].ubClassIndex ].ubMagSize;
	}
	else {
		(*pObj)[0]->data.ubShotsLeft = ubShotsLeft;
	}
	pObj->ubWeight = CalculateObjectWeight( pObj );
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateAmmo: done"));

	return( TRUE );
}

BOOLEAN CreateItem( UINT16 usItem, INT8 bStatus, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateItem: usItem = %d",usItem));
	BOOLEAN fRet;

	if (usItem >= MAXITEMS)
	{
		DebugBreak();
		return( FALSE );
	}
	if (Item[ usItem ].usItemClass == IC_GUN)
	{
		fRet = CreateGun( usItem, bStatus, pObj );
	}
	else if (Item[ usItem ].usItemClass == IC_AMMO)
	{
		fRet = CreateAmmo( usItem, pObj );		
	}
	else
	{
		pObj->initialize();
		pObj->usItem = usItem;
		pObj->ubNumberOfObjects = 1;
		if (usItem == MONEY || Item[usItem].usItemClass == IC_MONEY )
		{
			// special case... always set status to 100 when creating
			// and use status value to determine amount!
			(*pObj)[0]->data.objectStatus = 100;
			(*pObj)[0]->data.money.uiMoneyAmount = bStatus * 50;
		}
		else
		{
			(*pObj)[0]->data.objectStatus = bStatus;
		}
		pObj->ubWeight = CalculateObjectWeight( pObj );
		fRet = TRUE;
	}
	if (fRet)
	{
//		if (Item[ usItem ].fFlags & ITEM_DEFAULT_UNDROPPABLE)
		if (Item[ usItem ].defaultundroppable )
		{
			(*pObj).fFlags |= OBJECT_UNDROPPABLE;
		}
		if (Item [ usItem ].defaultattachment > 0 )
		{
			CreateItem(Item [ usItem ].defaultattachment,100,&gTempObject);
			pObj->AttachObject(NULL,&gTempObject);
		}
	}


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateItem: return %d",fRet));
	return( fRet );
}

BOOLEAN CreateItems( UINT16 usItem, INT8 bStatus, UINT8 ubNumber, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	BOOLEAN fOk;
	// ARM: to avoid whacking memory once Assertions are removed...  Items will be lost in this situation!
	if ( ubNumber > MAX_OBJECTS_PER_SLOT )
	{
		DebugBreak();
		ubNumber = MAX_OBJECTS_PER_SLOT;
	}

	fOk = CreateItem( usItem, bStatus, pObj );
	if (fOk)
	{
		for (int x = 1; x < ubNumber; ++x) {
			pObj->objectStack.push_back(pObj->objectStack.front());
		}
		pObj->ubNumberOfObjects = ubNumber;
		pObj->ubWeight *= ubNumber;
		return( TRUE );
	}
	return( FALSE );
}

BOOLEAN CreateMoney( UINT32 uiMoney, OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	BOOLEAN fOk; 

	fOk = CreateItem( MONEY, 100, pObj );
	if (fOk)
	{
		(*pObj)[0]->data.money.uiMoneyAmount = uiMoney;
	}
	return( fOk );
}

BOOLEAN ArmBomb( OBJECTTYPE * pObj, INT8 bSetting )
{
	PERFORMANCE_MARKER
	BOOLEAN fRemote = FALSE;
	BOOLEAN fPressure = FALSE;
	BOOLEAN fTimed = FALSE;
	BOOLEAN	fSwitch = FALSE;

	if (pObj->usItem == ACTION_ITEM)
	{
		switch( (*pObj)[0]->data.misc.bActionValue )
		{
			case ACTION_ITEM_SMALL_PIT:
			case ACTION_ITEM_LARGE_PIT:
				fPressure = TRUE;
				break;
			default:
				fRemote = TRUE;
				break;

		}
	}
	else if ( IsDetonatorAttached( pObj ) )
	{
		fTimed = TRUE;
	}
	else if ( (IsRemoteDetonatorAttached( pObj ) ) || (pObj->usItem == ACTION_ITEM) )
	{
		fRemote = TRUE;
	}
	else if ( Item[pObj->usItem].mine || pObj->usItem == TRIP_FLARE || pObj->usItem == TRIP_KLAXON || pObj->usItem == ACTION_ITEM )
	{
		fPressure = TRUE;
	}
	else if ( pObj->usItem == SWITCH )
	{
		// this makes a remote detonator into a pressure-sensitive trigger
		if ( bSetting == PANIC_FREQUENCY )
		{
			// panic trigger is only activated by expending APs, not by
			// stepping on it... so don't define a detonator type
			fSwitch = TRUE;
		}
		else
		{
			fPressure = TRUE;
		}
	}
	else
	{
		// no sorta bomb at all!
		return( FALSE );
	}

	if (fRemote)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_REMOTE;
		(*pObj)[0]->data.misc.bFrequency = bSetting;
	}
	else if (fPressure)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_PRESSURE;
		(*pObj)[0]->data.misc.bFrequency = 0;
	}
	else if (fTimed)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_TIMED;
		// In realtime the player could choose to put down a bomb right before a turn expires, SO
		// add 1 to the setting in RT
		(*pObj)[0]->data.misc.bDelay = bSetting;
		if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
		{
			(*pObj)[0]->data.misc.bDelay++;
		}

	}
	else if (fSwitch)
	{
		(*pObj)[0]->data.misc.bDetonatorType = BOMB_SWITCH;
		(*pObj)[0]->data.misc.bFrequency = bSetting;
	}
	else
	{
		return( FALSE );
	}

	(*pObj).fFlags |= OBJECT_ARMED_BOMB;
	(*pObj)[0]->data.misc.usBombItem = pObj->usItem;
	return( TRUE );
}

BOOLEAN OBJECTTYPE::RemoveAttachment( OBJECTTYPE* pAttachment, OBJECTTYPE * pNewObj )
{
	PERFORMANCE_MARKER
	if ( pAttachment == 0 )
	{
		return( FALSE );
	}

	if ( Item[ pAttachment->usItem ].inseparable  )
	{
		return( FALSE );
	}

	// if pNewObj is passed in NULL, then we just delete the attachment
	if (pNewObj != NULL)
	{
		*pNewObj = *pAttachment;
	}

	(*this)[0]->attachments.remove(*pAttachment);

	if (pNewObj && Item[pNewObj->usItem].grenadelauncher )//UNDER_GLAUNCHER)
	{
		// look for any grenade; if it exists, we must make it an 
		// attachment of the grenade launcher
		OBJECTTYPE* pGrenade = FindAttachmentByClass( this, IC_GRENADE );
		if (pGrenade)
		{
			(*pNewObj)[0]->attachments.push_back(*pGrenade);
			pNewObj->ubWeight = CalculateObjectWeight( pNewObj );
			this->RemoveAttachment(pGrenade);
		}
	}

	this->ubWeight = CalculateObjectWeight( this );
	return( TRUE );
}

void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem )
{
	PERFORMANCE_MARKER
	if( fNewItem )
	{
		pSoldier->inv.bNewItemCount[ ubInvPos ]						 = -1;
		pSoldier->inv.bNewItemCycleCount[ ubInvPos ]			 = NEW_ITEM_CYCLE_COUNT;
		pSoldier->flags.fCheckForNewlyAddedItems             = TRUE;
	}
}


BOOLEAN PlaceObjectInSoldierProfile( UINT8 ubProfile, OBJECTTYPE *pObject )
{
	PERFORMANCE_MARKER
	INT8				bLoop;
	SOLDIERTYPE *pSoldier;
	UINT16			usItem;
	INT8				bStatus;
	BOOLEAN			fReturnVal = FALSE;

	usItem	= pObject->usItem;
	Assert(pObject->ubNumberOfObjects == 1);
	bStatus = (*pObject)[0]->data.objectStatus;
	pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

	if ( Item[ usItem ].usItemClass == IC_MONEY && gMercProfiles[ ubProfile ].uiMoney > 0 )
	{
		gMercProfiles[ ubProfile ].uiMoney += (*pObject)[0]->data.money.uiMoneyAmount;
		SetMoneyInSoldierProfile( ubProfile, gMercProfiles[ ubProfile ].uiMoney );
		return( TRUE );
	}

	// CHRISL:
	for (bLoop = BIGPOCK1POS; bLoop < NUM_INV_SLOTS; bLoop++)
	{
		if ( gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] == 0 && (pSoldier == NULL || pSoldier->inv[ bLoop ].usItem == NOTHING ) )
		{

			// CJC: Deal with money by putting money into # stored in profile
			if ( Item[ usItem ].usItemClass == IC_MONEY )
			{
				gMercProfiles[ ubProfile ].uiMoney += (*pObject)[0]->data.money.uiMoneyAmount;
				// change any gold/silver to money
				usItem = MONEY;
			}
			else
			{
				gMercProfiles[ ubProfile ].inv[ bLoop ] = usItem;
				gMercProfiles[ ubProfile ].bInvStatus[ bLoop ] = bStatus;
				gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] = pObject->ubNumberOfObjects;	
			}

			fReturnVal = TRUE;
			break;
		}
	}

	//uiMoneyAmount
	if ( fReturnVal )
	{
		// ATE: Manage soldier pointer as well....
		//pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

		// Do we have a valid profile?
		if ( pSoldier != NULL )
		{
			// OK, place in soldier...
			if ( usItem == MONEY )
			{
				CreateMoney( gMercProfiles[ ubProfile ].uiMoney, &(pSoldier->inv[ bLoop ] ) );
			}
			else
			{
				if ( pSoldier->ubProfile == MADLAB )
				{
					// remove attachments and drop them
					for (attachmentList::iterator iter = (*pObject)[0]->attachments.begin(); iter != (*pObject)[0]->attachments.end(); ++iter) {
						// drop it in Madlab's tile
						AddItemToPool( pSoldier->sGridNo, &(*iter), 1, 0, 0, 0 );
						pObject->RemoveAttachment(&(*iter));
					}
				}

				CreateItem( usItem, bStatus, &(pSoldier->inv[ bLoop ] ) );
			}
		}
	}

	return( fReturnVal );
}

BOOLEAN RemoveObjectFromSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8 bLoop;
	SOLDIERTYPE *pSoldier;
	BOOLEAN	fReturnVal = FALSE;

	if ( usItem == NOTHING )
	{
		return( TRUE );
	}

	for (bLoop = 0; bLoop < 19; bLoop++)
	{
		if ( gMercProfiles[ ubProfile ].inv[ bLoop ] == usItem )
		{
			gMercProfiles[ ubProfile ].inv[ bLoop ] = NOTHING;
			gMercProfiles[ ubProfile ].bInvStatus[ bLoop ] = 0;
			gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] = 0;				

			fReturnVal = TRUE;
			break;
		}
	}

	// ATE: Manage soldier pointer as well....
	pSoldier = FindSoldierByProfileID( ubProfile, FALSE );

	// Do we have a valid profile?
	if ( pSoldier != NULL )
	{
		// Remove item...
		RemoveInvObject( pSoldier, usItem );
	}

	return( fReturnVal );
}


void SetMoneyInSoldierProfile( UINT8 ubProfile, UINT32 uiMoney )
{
	PERFORMANCE_MARKER
	BOOLEAN					fRet;

	// remove all money from soldier	
	do
	{
		fRet = RemoveObjectFromSoldierProfile( ubProfile, MONEY );
	}
	while ( fRet == TRUE );

	gMercProfiles[ ubProfile ].uiMoney = 0;

	if (uiMoney > 0)
	{
		// now add the amount specified
		CreateMoney( uiMoney, &gTempObject );
		PlaceObjectInSoldierProfile( ubProfile, &gTempObject );
	}
}

INT8 FindObjectInSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8	bLoop;

	for (bLoop = 0; bLoop < 19; bLoop++)
	{
		if ( gMercProfiles[ ubProfile ].bInvNumber[ bLoop ] > 0 )
		{
			if ( gMercProfiles[ ubProfile ].inv[ bLoop ] == usItem )
			{
				return( bLoop );
			}
		}
	}
	return( NO_SLOT );
}

BOOLEAN ObjectExistsInSoldierProfile( UINT8 ubProfile, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8	bSlot;

	bSlot = FindObjectInSoldierProfile( ubProfile, usItem );
	return( bSlot != NO_SLOT );
}

void RemoveInvObject( SOLDIERTYPE *pSoldier, UINT16 usItem )
{
	PERFORMANCE_MARKER
	INT8 bInvPos;

	// find object
	bInvPos = FindObj( pSoldier, usItem );
	if (bInvPos != NO_SLOT)
	{

		// Erase!
		pSoldier->inv[ bInvPos ].initialize();

		//Dirty!
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}

}

INT8 CheckItemForDamage( UINT16 usItem, INT32 iMaxDamage )
{
	PERFORMANCE_MARKER
	INT8	bDamage = 0;

	// if the item is protective armour, reduce the amount of damage
	// by its armour value
	if (Item[usItem].usItemClass == IC_ARMOUR)
	{
		iMaxDamage -= (iMaxDamage * Armour[Item[usItem].ubClassIndex].ubProtection) / 100;
	}
	// metal items are tough and will be damaged less
//	if (Item[usItem].fFlags & ITEM_METAL)
	if (Item[usItem].metal )
	{
		iMaxDamage /= 2;
	}
	else if ( usItem == BLOODCAT_PELT )
	{
		iMaxDamage *= 2;
	}
	if (iMaxDamage > 0)
	{
		bDamage = (INT8) PreRandom( iMaxDamage );
	}
	return( bDamage );
}

BOOLEAN CheckForChainReaction( UINT16 usItem, INT8 bStatus, INT8 bDamage, BOOLEAN fOnGround )
{
	PERFORMANCE_MARKER
	INT32 iChance;

	iChance = Explosive[Item[usItem].ubClassIndex].ubVolatility;
	if (iChance > 0)
	{
		
		// Scale the base chance by the damage caused to the item
		// (bigger the shock, bigger chance) and the condition of 
		// the item after being hit!
		if (fOnGround)
		{
			// improve chance to make it practical to blow up explosives on the ground
			iChance = 50 + (iChance - 1) * 10;
		}

		iChance = iChance * ( 100 + ( (100 - bStatus) + bDamage ) / 2 ) / 100;
		if ((INT32) PreRandom( 100 ) < iChance)
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN DamageItem( OBJECTTYPE * pObject, INT32 iDamage, BOOLEAN fOnGround )
{
	PERFORMANCE_MARKER
	INT8		bLoop;
	INT8		bDamage;

//	if ( (Item[pObject->usItem].fFlags & ITEM_DAMAGEABLE || Item[ pObject->usItem ].usItemClass == IC_AMMO) && pObject->ubNumberOfObjects > 0)
	if ( (Item[pObject->usItem].damageable  || Item[ pObject->usItem ].usItemClass == IC_AMMO) && pObject->ubNumberOfObjects > 0)
	{

		for (bLoop = 0; bLoop < pObject->ubNumberOfObjects; bLoop++)
		{
			bool removed = false;
			// if the status of the item is negative then it's trapped/jammed;
			// leave it alone
			if (pObject->usItem != NOTHING && (*pObject)[bLoop]->data.objectStatus > 0)
			{
				bDamage = CheckItemForDamage( pObject->usItem, iDamage );
				switch( pObject->usItem )
				{
					case JAR_CREATURE_BLOOD:
					case JAR:
					case JAR_HUMAN_BLOOD:
					case JAR_ELIXIR:
						if ( PreRandom( bDamage ) > 5 )
						{
							// smash!
							bDamage = (*pObject)[bLoop]->data.objectStatus;
						}
						break;
					default:
						break;
				}
				if ( Item[ pObject->usItem ].usItemClass == IC_AMMO  )
				{
					if ( PreRandom( 100 ) < (UINT32) bDamage )
					{
						// destroy clip completely
						(*pObject)[bLoop]->data.objectStatus = 1;
					}
				}
				else
				{
					(*pObject)[bLoop]->data.objectStatus -= bDamage;
					if ((*pObject)[bLoop]->data.objectStatus < 1)
					{
						(*pObject)[bLoop]->data.objectStatus = 1;
					}
				}
				// I don't think we increase viewrange based on items any more
				// FUN STUFF!  Check for explosives going off as a result!
				if (Item[pObject->usItem].usItemClass & IC_EXPLOSV)
				{
					if (CheckForChainReaction( pObject->usItem, (*pObject)[bLoop]->data.objectStatus, bDamage, fOnGround ))
					{
						return( TRUE );
					}
				}

				// remove item from index AFTER checking explosions because need item data for explosion!
				if ( (*pObject)[bLoop]->data.objectStatus == 1 )
				{
					if ( pObject->ubNumberOfObjects > 1 )
					{
						removed = true;
						RemoveObjFrom( pObject, bLoop );
						// since an item was just removed, the items above the current were all shifted down one;
						// to process them properly, we have to back up 1 in the counter
						--bLoop;
					}
				}
			}

			if (removed == false) {
				for (attachmentList::iterator iter = (*pObject)[bLoop]->attachments.begin(); iter != (*pObject)[bLoop]->attachments.end();) {
					DamageItem(&(*iter), iDamage, fOnGround);
					//could have removed the object at iter
					if (iter != (*pObject)[bLoop]->attachments.end()) {
						++iter;
					}
				}
			}
		}
	}

	return( FALSE );
}

void CheckEquipmentForDamage( SOLDIERTYPE *pSoldier, INT32 iDamage )
{
	PERFORMANCE_MARKER
	INT8				bSlot;
	BOOLEAN			fBlowsUp;
	UINT8				ubNumberOfObjects;

	if ( TANK( pSoldier ) )
	{
		return;
	}

	for (bSlot = 0; bSlot < (INT8) pSoldier->inv.size(); bSlot++)
	{
		ubNumberOfObjects = pSoldier->inv[bSlot].ubNumberOfObjects;
		fBlowsUp = DamageItem( &(pSoldier->inv[bSlot]), iDamage, FALSE );
		if (fBlowsUp)
		{
			// blow it up!
			if ( gTacticalStatus.ubAttackBusyCount )
			{
				IgniteExplosion( pSoldier->ubAttackerID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, pSoldier->inv[ bSlot ].usItem, pSoldier->pathing.bLevel );
			}
			else
			{
				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, pSoldier->inv[ bSlot ].usItem, pSoldier->pathing.bLevel );
			}

			//ADB when something in a stack blows up the whole stack goes, so no need to worry about number of items
			// Remove item!
			DeleteObj( &(pSoldier->inv[ bSlot ]) );

			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}
		else if ( ubNumberOfObjects != pSoldier->inv[bSlot].ubNumberOfObjects )
		{
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}
	}
}

void CheckEquipmentForFragileItemDamage( SOLDIERTYPE *pSoldier, INT32 iDamage )
{
	PERFORMANCE_MARKER
	// glass jars etc can be damaged by falling over
	INT8				bSlot;
	UINT8				ubNumberOfObjects;
	BOOLEAN			fPlayedGlassBreak = FALSE;

	for (bSlot = 0; bSlot < (INT8) pSoldier->inv.size(); bSlot++)
	{
		switch( pSoldier->inv[bSlot].usItem )
		{
			case JAR_CREATURE_BLOOD:
			case JAR:
			case JAR_HUMAN_BLOOD:
			case JAR_ELIXIR:
				ubNumberOfObjects = pSoldier->inv[bSlot].ubNumberOfObjects;
				DamageItem( &(pSoldier->inv[bSlot]), iDamage, FALSE );
				if ( !fPlayedGlassBreak && (ubNumberOfObjects != pSoldier->inv[bSlot].ubNumberOfObjects) )
				{
					PlayJA2Sample( GLASS_CRACK, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
					fPlayedGlassBreak = TRUE;
					// only dirty once
					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				}
				break;
			default:
				break;
		}
	}
}


BOOLEAN DamageItemOnGround( OBJECTTYPE * pObject, INT16 sGridNo, INT8 bLevel, INT32 iDamage, UINT8 ubOwner )
{
	PERFORMANCE_MARKER
	BOOLEAN			fBlowsUp;

	fBlowsUp = DamageItem( pObject, iDamage, TRUE );
	if ( fBlowsUp )
	{
		// OK, Ignite this explosion!
		IgniteExplosion( ubOwner, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, pObject->usItem, bLevel );

		// Remove item!
		return( TRUE );
	}
	else if ( (pObject->ubNumberOfObjects < 2) && ((*pObject)[0]->data.objectStatus < USABLE) )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

// is the item a medical kit/first aid kit item?
INT8 IsMedicalKitItem( OBJECTTYPE *pObject )
{
	PERFORMANCE_MARKER
	// check item id against current medical kits
	if ( Item[pObject->usItem].medicalkit )
		return 1;

	//switch( pObject->usItem )
	//{
	//	case( MEDICKIT ):
	//		// medical bag, return 1
	//		return ( 1 );
	//	break;
	//}

	return( 0 );
}

void SwapHandItems( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	BOOLEAN		fOk;

	CHECKV( pSoldier );
	if (pSoldier->inv[HANDPOS].usItem == NOTHING || pSoldier->inv[SECONDHANDPOS].usItem == NOTHING)
	{
		// whatever is in the second hand can be swapped to the main hand!
		SwapObjs( &(pSoldier->inv[HANDPOS]), &(pSoldier->inv[SECONDHANDPOS]) );
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
	else
	{
		if (TwoHandedItem( pSoldier->inv[SECONDHANDPOS].usItem ) )
		{
			// must move the item in the main hand elsewhere in the inventory
			fOk = InternalAutoPlaceObject( pSoldier, &(pSoldier->inv[HANDPOS]), FALSE, HANDPOS );
			if (!fOk)
			{
				return;
			}
			// the main hand is now empty so a swap is going to work...
		}
		SwapObjs( &(pSoldier->inv[HANDPOS]), &(pSoldier->inv[SECONDHANDPOS]) );
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
}

void SwapOutHandItem( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	BOOLEAN			fOk;

	CHECKV( pSoldier );

	// puts away the item in the main hand
	if (pSoldier->inv[HANDPOS].usItem != NOTHING )
	{
		if (pSoldier->inv[SECONDHANDPOS].usItem == NOTHING)
		{
			// just swap the hand item to the second hand
			SwapObjs( &(pSoldier->inv[HANDPOS]), &(pSoldier->inv[SECONDHANDPOS]) );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			return;
		}
		else
		{
			// try placing it somewhere else in our inventory
			fOk = AutoPlaceObject( pSoldier, &(pSoldier->inv[HANDPOS]), FALSE );
			if (fOk)
			{
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			}
			// otherwise there's no room for the item anywhere!
		}
	}	
}

void WaterDamage( SOLDIERTYPE *pSoldier )
{
	PERFORMANCE_MARKER
	// damage guy's equipment and camouflage due to water
	INT8		bLoop, bDamage, bDieSize;
	UINT32	uiRoll;

	if ( pSoldier->bOverTerrainType == DEEP_WATER )
	{
		for ( bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++ )
		{
			// if there's an item here that can get water damaged...
//			if (pSoldier->inv[ bLoop ].usItem && Item[pSoldier->inv[ bLoop ].usItem].fFlags & ITEM_WATER_DAMAGES)
			if (pSoldier->inv[ bLoop ].usItem && Item[pSoldier->inv[ bLoop ].usItem].waterdamages )
			{
				// roll the 'ol 100-sided dice
				uiRoll = PreRandom(100);

				// 10% chance of getting damage!
				if (uiRoll < 10)
				{
					// lose between 1 and 10 status points each time
					bDamage = (INT8) (10 - uiRoll);

					// but don't let anything drop lower than 1%
					pSoldier->inv[bLoop][0]->data.objectStatus -= bDamage;
					if (pSoldier->inv[bLoop][0]->data.objectStatus < 1)
					{
						pSoldier->inv[bLoop][0]->data.objectStatus = 1;
					}
				}
			}
		}
	}
	BOOLEAN camoWoreOff = FALSE;
	if (pSoldier->bCamo > 0 && !HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED ) )
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bOverTerrainType == DEEP_WATER )
			pSoldier->bCamo = __max( 0, pSoldier->bCamo - 2 );
		else
			pSoldier->bCamo = __max( 0, pSoldier->bCamo - 1 );

		if ( (pSoldier->bCamo)== 0)
			camoWoreOff = TRUE;
	}

	if (pSoldier->urbanCamo > 0 && !HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_URBAN ) )
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bOverTerrainType == DEEP_WATER )
			pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 2 );
		else
			pSoldier->urbanCamo = __max( 0, pSoldier->urbanCamo - 1 );

		if ( (pSoldier->urbanCamo)== 0)
			camoWoreOff = TRUE;
	}

	if (pSoldier->desertCamo > 0 && !HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_DESERT ) )
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bOverTerrainType == DEEP_WATER )
			pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 2 );
		else
			pSoldier->desertCamo = __max( 0, pSoldier->desertCamo - 1 );

		if ( (pSoldier->desertCamo)== 0)
			camoWoreOff = TRUE;
	}

	if (pSoldier->snowCamo > 0 && !HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_SNOW ) )
	{
		// reduce camouflage by 2% per tile of deep water
		// and 1% for medium water
		if ( pSoldier->bOverTerrainType == DEEP_WATER )
			pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 2 );
		else
			pSoldier->snowCamo = __max( 0, pSoldier->snowCamo - 1 );

		if ( (pSoldier->snowCamo)== 0)
			camoWoreOff = TRUE;
	}

	if ( camoWoreOff )
	{
		// Reload palettes....
		if ( pSoldier->bInSector )
		{	
			pSoldier->CreateSoldierPalettes( );
		}
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_CAMMO_WASHED_OFF], pSoldier->name );
	}






	if ( pSoldier->bTeam == gbPlayerNum && pSoldier->aiData.bMonsterSmell > 0 )
	{
		if ( pSoldier->bOverTerrainType == DEEP_WATER )
		{
			bDieSize = 10;
		}
		else
		{
			bDieSize = 20;
		}
		if ( Random( bDieSize ) == 0 )
		{
			pSoldier->aiData.bMonsterSmell--;
		}		
	}

	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
}

BOOLEAN ApplyCammo( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	PERFORMANCE_MARKER
	INT8		bPointsToUse;
	UINT16	usTotalKitPoints;

  (*pfGoodAPs) = TRUE;

	if (!Item[pObj->usItem].camouflagekit )
	{
		return( FALSE );
	}

	if (!EnoughPoints( pSoldier, AP_CAMOFLAGE, 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH??? 
		return( FALSE );
	}

	//get total camo bonus for kit -- note that camo kits now require the camobonus tag to be set
	//int itemCamo = Item[pObj->usItem].camobonus + Item[pObj->usItem].urbanCamobonus + Item[pObj->usItem].desertCamobonus + Item[pObj->usItem].snowCamobonus;

	int totalCamo = pSoldier->bCamo + pSoldier->wornCamo + pSoldier->urbanCamo+pSoldier->wornUrbanCamo+pSoldier->desertCamo+pSoldier->wornDesertCamo+pSoldier->snowCamo+pSoldier->wornSnowCamo;
	if ((totalCamo) >= 100)
	{
		// nothing more to add
		return( FALSE );
	}

	// points are used up at a rate of 50% kit = 100% cammo on guy
	// add 1 to round off
	bPointsToUse = (100 - (totalCamo) + 1 ) / 2;
	bPointsToUse = __min( bPointsToUse, usTotalKitPoints );

	//figure out proportions of each to be applied, one item can theoretically have more than one camouflage type this way
	int urban = (int)(Item[pObj->usItem].urbanCamobonus * bPointsToUse * 2 / 100 );
	int jungle = (int)(Item[pObj->usItem].camobonus * bPointsToUse * 2 / 100 );
	int desert = (int)(Item[pObj->usItem].desertCamobonus * bPointsToUse * 2 / 100 );
	int snow = (int)(Item[pObj->usItem].snowCamobonus * bPointsToUse * 2 / 100 );

	pSoldier->bCamo = __min( 100, pSoldier->bCamo + jungle );
	pSoldier->urbanCamo = __min( 100, pSoldier->urbanCamo + urban );
	pSoldier->desertCamo = __min( 100, pSoldier->desertCamo + desert );
	pSoldier->snowCamo = __min( 100, pSoldier->snowCamo + snow );
	
	UseKitPoints( pObj, bPointsToUse, pSoldier );

	DeductPoints( pSoldier, AP_CAMOFLAGE, 0 );

	// Reload palettes....
	if ( pSoldier->bInSector )
	{	
		pSoldier->CreateSoldierPalettes( );
	}

	return( TRUE );
}

BOOLEAN ApplyCanteen( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	PERFORMANCE_MARKER
	INT16		sPointsToUse;
	UINT16	usTotalKitPoints;

  (*pfGoodAPs) = TRUE;

	if (!Item[pObj->usItem].canteen )
	{
		return( FALSE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH??? 
		return( FALSE );
	}

	if (!EnoughPoints( pSoldier, AP_DRINK, 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

  if ( pSoldier->bTeam == gbPlayerNum )
  {
    if ( gMercProfiles[ pSoldier->ubProfile ].bSex == MALE )
    {
		  PlayJA2Sample( DRINK_CANTEEN_MALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
    }
    else
    {
		  PlayJA2Sample( DRINK_CANTEEN_FEMALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
    }
  }

	sPointsToUse = __min( 20, usTotalKitPoints );

	// CJC Feb 9.  Canteens don't seem effective enough, so doubled return from them
	DeductPoints( pSoldier, AP_DRINK, (INT16) (2 * sPointsToUse * -(100 - pSoldier->bBreath) ) );

	UseKitPoints( pObj, sPointsToUse, pSoldier );

	return( TRUE );
}

#define MAX_HUMAN_CREATURE_SMELL (NORMAL_HUMAN_SMELL_STRENGTH - 1)

BOOLEAN ApplyElixir( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs )
{
	PERFORMANCE_MARKER
	INT16		sPointsToUse;
	UINT16	usTotalKitPoints;

  (*pfGoodAPs) = TRUE;

	if (pObj->usItem != JAR_ELIXIR )
	{
		return( FALSE );
	}

	usTotalKitPoints = TotalPoints( pObj );
	if (usTotalKitPoints == 0)
	{
		// HUH??? 
		return( FALSE );
	}

	if (!EnoughPoints( pSoldier, AP_CAMOFLAGE, 0, TRUE ) )
	{
    (*pfGoodAPs) = FALSE;
		return( TRUE );
	}

	DeductPoints( pSoldier, AP_CAMOFLAGE, 0 );

	sPointsToUse = ( MAX_HUMAN_CREATURE_SMELL - pSoldier->aiData.bMonsterSmell ) * 2;
	sPointsToUse = __min( sPointsToUse, usTotalKitPoints );

	UseKitPoints( pObj, sPointsToUse, pSoldier );

	pSoldier->aiData.bMonsterSmell += sPointsToUse / 2;

	return( TRUE );
}

UINT32 ConvertProfileMoneyValueToObjectTypeMoneyValue( UINT8 ubStatus )
{
	PERFORMANCE_MARKER
	return( ubStatus * 50 );
}

UINT8 ConvertObjectTypeMoneyValueToProfileMoneyValue( UINT32 uiMoneyAmount )
{
	PERFORMANCE_MARKER
	return( (UINT8)( uiMoneyAmount / 50 ) );
}

BOOLEAN ItemIsCool( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if ((*pObj)[0]->data.objectStatus < 60)
	{
		return( FALSE );
	}
	if ( Item[ pObj->usItem ].usItemClass & IC_WEAPON )
	{
		if ( Weapon[ pObj->usItem ].ubDeadliness >= 30 )
		{
			return( TRUE );
		}
	}
	else if ( Item[ pObj->usItem ].usItemClass & IC_ARMOUR )
	{
		if ( Armour[ Item[ pObj->usItem ].ubClassIndex ].ubProtection >= 20 )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

void ActivateXRayDevice( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	SOLDIERTYPE *	pSoldier2;
	UINT32				uiSlot;

	if ( Item[pSoldier->inv[ HANDPOS ].usItem].needsbatteries )
	{
		// check for batteries
		OBJECTTYPE* pBatteries = FindAttachedBatteries( &(pSoldier->inv[HANDPOS]) );
		if ( pBatteries == 0 )
		{
			// doesn't work without batteries!
			return;
		}

		// use up 8-12 percent of batteries
		if ( Item[pBatteries->usItem].percentstatusdrainreduction > 0 )
			(*pBatteries)[0]->data.objectStatus -= (INT8)( (8 + Random( 5 )) * (100 - Item[(*pBatteries)[0]->data.objectStatus].percentstatusdrainreduction)/100 );
		else
			(*pBatteries)[0]->data.objectStatus -= (INT8)( (8 + Random( 5 )) );
		if ( (*pBatteries)[0]->data.objectStatus <= 0 )
		{
			// destroy batteries
			pBatteries->RemoveObjectsFromStack(1);
			if (pBatteries->ubNumberOfObjects == 0) {
				pSoldier->inv[HANDPOS].RemoveAttachment(pBatteries);
			}
		}
	}
	// first, scan through all mercs and turn off xrayed flag for anyone
	// previously xrayed by this guy
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( (pSoldier2->ubMiscSoldierFlags & SOLDIER_MISC_XRAYED) && (pSoldier2->aiData.ubXRayedBy == pSoldier->ubID) )
			{
				pSoldier2->ubMiscSoldierFlags &= (~SOLDIER_MISC_XRAYED);
				pSoldier2->aiData.ubXRayedBy = NOBODY;
			}
		}
	}
	// now turn on xray for anyone within range
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( pSoldier2->bTeam != pSoldier->bTeam && PythSpacesAway( pSoldier->sGridNo, pSoldier2->sGridNo ) < XRAY_RANGE )
			{
				pSoldier2->ubMiscSoldierFlags |= SOLDIER_MISC_XRAYED;
				pSoldier2->aiData.ubXRayedBy = pSoldier->ubID;
			}
		}
	}
	pSoldier->uiXRayActivatedTime = GetWorldTotalSeconds();
}

void TurnOffXRayEffects( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	SOLDIERTYPE *	pSoldier2;
	UINT32				uiSlot;

	if ( !pSoldier->uiXRayActivatedTime )
	{
		return;
	}

	// scan through all mercs and turn off xrayed flag for anyone
	// xrayed by this guy
	for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
	{
		pSoldier2 = MercSlots[ uiSlot ];
		if ( pSoldier2 )
		{
			if ( (pSoldier2->ubMiscSoldierFlags & SOLDIER_MISC_XRAYED) && (pSoldier2->aiData.ubXRayedBy == pSoldier->ubID) )
			{
				pSoldier2->ubMiscSoldierFlags &= (~SOLDIER_MISC_XRAYED);
				pSoldier2->aiData.ubXRayedBy = NOBODY;
			}
		}
	}
	pSoldier->uiXRayActivatedTime = 0;
}



#ifdef JA2TESTVERSION
void DumpItemsList( void )
{
	PERFORMANCE_MARKER
  CHAR8 zPrintFileName[60];
  FILE *FDump;
	UINT16 usItem;
	INVTYPE *pItem;

  // open output file
 	strcpy(zPrintFileName, "ItemDump.txt");
  FDump = fopen(zPrintFileName, "wt");

  if (FDump == NULL)
    return;

	// print headings
	fprintf(FDump, "            ITEM              COOLNESS  VALUE\n");
	fprintf(FDump, "============================  ========  =====\n");

	for( usItem = 0; usItem < MAXITEMS; usItem++ )
	{
		pItem= &( Item[ usItem ] );
	
		if (pItem->ubCoolness > 0 )
		{
			fprintf(FDump, "%28ls     %2d     $%4d\n", ItemNames[ usItem ], pItem->ubCoolness, pItem->usPrice );
		}
	}

  fclose(FDump);
}
#endif // JA2TESTVERSION


// Snap: status modifiers for various item bonuses:

// In JA Gold status above this limit does not affect item performance (for some items)
const INT8 STANDARD_STATUS_CUTOFF = 85;

// Scale bonus with item status
INT16 BonusReduce( INT16 bonus, INT8 status, INT8 statusCutoff = STANDARD_STATUS_CUTOFF )
{
	PERFORMANCE_MARKER
	if ( bonus > 0 && status < statusCutoff && statusCutoff > 0 && statusCutoff <= 100 )
		return ( ( status * 100 ) / statusCutoff * bonus ) / 100;
	else // A penalty can't be reduced by status!
		return bonus;
}

// Scale bonus with item status. Status < 50% creates a penalty!
INT16 BonusReduceMore( INT16 bonus, INT8 status, INT8 statusCutoff = 100 )
{
	PERFORMANCE_MARKER
	if ( bonus > 0 && status < statusCutoff && statusCutoff > 0 && statusCutoff <= 100 )
		return ( ( ( status * 100 ) / statusCutoff - 50 ) * bonus ) / 50;
	else // A penalty can't be reduced by status!
		return bonus;
}

// Some items either work or they don't...
INT16 BonusOnOff( INT16 bonus, INT8 status )
{
	PERFORMANCE_MARKER
	if ( bonus > 0 )
		return (status >= 50) ? bonus : 0;
	else // A penalty can't be reduced by status!
		return bonus;
}


// Snap: a fast aimbonus check for AI
BOOLEAN IsScoped( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if ( Item[pObj->usItem].aimbonus > 0 || Item[(*pObj)[0]->data.gun.usGunAmmoItem].aimbonus > 0 )
		return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( Item[iter->usItem].aimbonus > 0 )
			return TRUE;
	}

	return FALSE;
}

// Snap: get aim bonus for a single item
INT16 GetItemAimBonus( const INVTYPE* pItem, INT32 iRange, UINT8 ubAimTime )
{
	PERFORMANCE_MARKER
	if ( iRange <= pItem->minrangeforaimbonus )	return 0;

	// reduce effective sight range by aimbonus% per extra aiming time AP
	// of the distance beyond minrangeforaimbonus.
	if ( pItem->aimbonus < 0)
		ubAimTime = 1;

	return ( pItem->aimbonus * ubAimTime	* ( iRange - pItem->minrangeforaimbonus ) ) / 100;
}	

INT16 GetAimBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 ubAimTime )
{
	PERFORMANCE_MARKER
	INT16 bonus;

	bonus = BonusReduceMore( GetItemAimBonus( &Item[pObj->usItem], iRange, ubAimTime ), (*pObj)[0]->data.objectStatus );
	bonus += GetItemAimBonus( &Item[(*pObj)[0]->data.gun.usGunAmmoItem], iRange, ubAimTime );

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( GetItemAimBonus( &Item[iter->usItem], iRange, ubAimTime ), (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

// Madd: check equipment for aim bonus (penalties)
INT16 GetGearAimBonus( SOLDIERTYPE * pSoldier, INT32 iRange, UINT8 ubAimTime  )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++) 
	{
		bonus += GetItemAimBonus( &Item[pSoldier->inv[j].usItem], iRange, ubAimTime );
		OBJECTTYPE* pObj = &pSoldier->inv[j];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += GetItemAimBonus(&Item[iter->usItem],iRange,ubAimTime);
		}
	}

	return( bonus );
}

// Madd: check equipment for to hit bonus (penalties)
INT16 GetGearToHitBonus( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++) 
	{
		bonus += Item[pSoldier->inv[j].usItem].tohitbonus;
		OBJECTTYPE* pObj = &pSoldier->inv[j];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += Item[iter->usItem].tohitbonus;
		}
	}

	return( bonus );
}

// Madd: check equipment for AP bonus (penalties)
INT16 GetGearAPBonus( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	for (int j = HELMETPOS; j <= HEAD2POS; j++) 
	{
		bonus += Item[pSoldier->inv[j].usItem].APBonus;
		OBJECTTYPE* pObj = &pSoldier->inv[j];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += Item[iter->usItem].APBonus;
		}
	}

	return( bonus );
}

UINT32 FindRangeBonusAttachment( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].rangebonus > 0 )
		{
			return( Item[iter->usItem].uiIndex );
		}
	}
	return( NONE );
}

INT16 GetRangeBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduce( Item[pObj->usItem].rangebonus, (*pObj)[0]->data.objectStatus );

	if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].rangebonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( !Item[iter->usItem].duckbill || ( Item[iter->usItem].duckbill && (*pObj)[0]->data.gun.ubGunAmmoType == AMMO_BUCKSHOT ))
			bonus += BonusReduce( Item[iter->usItem].rangebonus, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}


INT16 LaserBonus( const INVTYPE * pItem, INT32 iRange, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	// Snap: Reduce laser scope bonus at long ranges and high light levels

	if ( pItem->bestlaserrange == 0 || iRange <= pItem->bestlaserrange ) {
		// No penalty within this range
		return pItem->tohitbonus;
	}
	else {
		// Figure out max. visible distance for the laser dot:
		// day: 1.5*bestlaserrange, night: 2.5*bestlaserrange
		// iMaxLaserRange = bestlaserrange * ( 1.5 + ( bLightLevel - NORMAL_LIGHTLEVEL_DAY )
		//                                 / ( NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY ) )
		INT32 iMaxLaserRange = ( pItem->bestlaserrange*( 2*bLightLevel + 3*NORMAL_LIGHTLEVEL_NIGHT - 5*NORMAL_LIGHTLEVEL_DAY ) )
		                     / ( 2 * ( NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY ) );

		// Beyond bestlaserrange laser bonus drops linearly to 0
		INT16 bonus = ( pItem->tohitbonus * (iMaxLaserRange - iRange) )
		            / ( iMaxLaserRange - pItem->bestlaserrange );
		
		return (bonus > 0 ? bonus : 0);
	}
}

INT16 GetToHitBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 bLightLevel, BOOLEAN fProneStance )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance

	if ( fProneStance )
		bonus += Item[pObj->usItem].bipod;
		
	bonus += BonusReduceMore( LaserBonus( &Item[pObj->usItem], iRange, bLightLevel), (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].tohitbonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( fProneStance )
			bonus += Item[iter->usItem].bipod;

		bonus += BonusReduceMore( LaserBonus( &Item[iter->usItem], iRange, bLightLevel), (*iter)[0]->data.objectStatus );
	}

	// Snap (TODO): add special treatment of laser scopes
	return( bonus );
}

INT16 GetBurstToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance

	if ( fProneStance )
		bonus += Item[pObj->usItem].bipod;

	bonus += BonusReduceMore( Item[pObj->usItem].bursttohitbonus, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].bursttohitbonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( fProneStance )
			bonus += Item[iter->usItem].bipod;

		bonus += BonusReduceMore( Item[iter->usItem].bursttohitbonus, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}


INT16 GetDamageBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduce( Item[pObj->usItem].damagebonus, (*pObj)[0]->data.objectStatus );

	if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 )
		bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].damagebonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduce( Item[iter->usItem].damagebonus, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}


INT16 GetMeleeDamageBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduce( Item[pObj->usItem].meleedamagebonus, (*pObj)[0]->data.objectStatus);
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].meleedamagebonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduce( Item[iter->usItem].meleedamagebonus, (*iter)[0]->data.objectStatus );
	}
	return( bonus );
}


INT16 GetPercentAPReduction( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduceMore( Item[pObj->usItem].percentapreduction, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentapreduction;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].percentapreduction,
			(*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

INT16 GetMagSizeBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusOnOff( Item[pObj->usItem].magsizebonus, (*pObj)[0]->data.objectStatus );
//	bonus = bonus + Item[(*pObj)[0]->data.gun.usGunAmmoItem].magsizebonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusOnOff( Item[iter->usItem].magsizebonus, (*iter)[0]->data.objectStatus );
	}
	return( bonus );
}

INT16 GetBurstSizeBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusOnOff( Item[pObj->usItem].burstsizebonus, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].burstsizebonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusOnOff( Item[iter->usItem].burstsizebonus, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

INT16 GetRateOfFireBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

  if( (MAXITEMS <= pObj->usItem) || (MAXITEMS <= (*pObj)[0]->data.gun.usGunAmmoItem) )
    {
   	DebugMsg(TOPIC_JA2, DBG_LEVEL_1, String("GetRateOfFireBonus would crash: pObj->usItem=%d or (*pObj)[0]->data.gun.usGunAmmoItem=%d ist higher than max %d", pObj->usItem, (*pObj)[0]->data.gun.usGunAmmoItem, MAXITEMS ));
  	ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"GetRateOfFireBonus would crash: pObj->usItem=%d or (*pObj)[0]->data.gun.usGunAmmoItem=%d ist higher than max %d", pObj->usItem, (*pObj)[0]->data.gun.usGunAmmoItem, MAXITEMS );
    AssertMsg( 0, "GetRateOfFireBonus would crash" );
    return 0; /* cannot calculate Bonus, this only happens sometimes in FULLSCREEN */
    }

	bonus = BonusReduceMore( Item[pObj->usItem].rateoffirebonus, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].rateoffirebonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].rateoffirebonus, (*iter)[0]->data.objectStatus );
	}
	return( bonus );
}

INT16 GetAutoToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	// Snap: bipod is effective only in the prone stance

	if ( fProneStance )
		bonus += Item[pObj->usItem].bipod;


	bonus += BonusReduceMore( Item[pObj->usItem].autofiretohitbonus, (*pObj)[0]->data.objectStatus );

	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].autofiretohitbonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( fProneStance )
			bonus += Item[iter->usItem].bipod;

		bonus += BonusReduceMore( Item[iter->usItem].autofiretohitbonus, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

INT16 GetPercentReadyTimeAPReduction( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduceMore( Item[pObj->usItem].percentreadytimeapreduction, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentreadytimeapreduction;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].percentreadytimeapreduction, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

INT16 GetPercentAutofireAPReduction( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduceMore( Item[pObj->usItem].percentautofireapreduction, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentautofireapreduction;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].percentautofireapreduction, (*iter)[0]->data.objectStatus );
	}

	return( bonus );
}

INT16 GetPercentReloadTimeAPReduction( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduceMore( Item[pObj->usItem].percentreloadtimeapreduction, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentreloadtimeapreduction;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].percentreloadtimeapreduction, (*iter)[0]->data.objectStatus );
	}
	return( bonus );
}

INT16 GetPercentBurstFireAPReduction( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = BonusReduceMore( Item[pObj->usItem].percentburstfireapreduction, (*pObj)[0]->data.objectStatus );
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentburstfireapreduction;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduceMore( Item[iter->usItem].percentburstfireapreduction, (*iter)[0]->data.objectStatus );
	}
	return( bonus );
}


INT16 GetVisionRangeBonus( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = HELMETPOS; i < BODYPOSFINAL; i++)
	{
		// Okay, it's time for some optimization here too
		pObj = &( pSoldier->inv[i]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		// Snap (TODO): binoculars and such should not be active by default
		if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true) )
		{
			bonus += BonusReduceMore( pItem->visionrangebonus,	(*pObj)[0]->data.objectStatus );
		}
	}

	// Snap: check only attachments on a raised weapon!
	//AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	if ( usingGunScope == true )
	{
		pObj = &( pSoldier->inv[HANDPOS]);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += BonusReduceMore( Item[iter->usItem].visionrangebonus, (*iter)[0]->data.objectStatus );
		}
	}

	return( bonus );
}

// Snap: Scale night vision bonus with light level
INT16 NightBonusScale( INT16 bonus, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	if ( bLightLevel > NORMAL_LIGHTLEVEL_NIGHT ) {
		return idiv( bonus * ( SHADE_MIN - bLightLevel ),
			SHADE_MIN - NORMAL_LIGHTLEVEL_NIGHT );
	}
	else if ( bLightLevel > NORMAL_LIGHTLEVEL_DAY ) {
		return idiv( bonus * (bLightLevel - NORMAL_LIGHTLEVEL_DAY),
			NORMAL_LIGHTLEVEL_NIGHT - NORMAL_LIGHTLEVEL_DAY );
	}
	else return 0;
}		

INT16 GetNightVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = HELMETPOS; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		// Snap (TODO): binoculars and such should not be active by default
		if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
		{
			bonus += BonusReduceMore(
				NightBonusScale( pItem->nightvisionrangebonus, bLightLevel ),
				(*pObj)[0]->data.objectStatus );
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		pObj = &( pSoldier->inv[HANDPOS]);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += BonusReduceMore(
				NightBonusScale( Item[iter->usItem].nightvisionrangebonus, bLightLevel ),
				(*iter)[0]->data.objectStatus );
		}
	}

	return( bonus );
}

INT16 GetCaveVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = 0; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		// Snap (TODO): binoculars and such should not be active by default
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
		{
			bonus += BonusReduceMore(
				NightBonusScale( pItem->cavevisionrangebonus, bLightLevel ),
				(*pObj)[0]->data.objectStatus );
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		pObj = &( pSoldier->inv[HANDPOS]);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += BonusReduceMore(
				NightBonusScale( Item[iter->usItem].cavevisionrangebonus, bLightLevel ),
				(*iter)[0]->data.objectStatus );
		}
	}

	return( bonus );
}

INT16 GetDayVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	// Snap: Scale the bonus with the light level

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = 0; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		// Snap (TODO): binoculars and such should not be active by default
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
		{
			bonus += BonusReduceMore( idiv( pItem->dayvisionrangebonus
				* (NORMAL_LIGHTLEVEL_NIGHT - bLightLevel), NORMAL_LIGHTLEVEL_NIGHT ),
				(*pObj)[0]->data.objectStatus );
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		pObj = &( pSoldier->inv[HANDPOS]);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += BonusReduceMore( idiv( Item[iter->usItem].dayvisionrangebonus
				* (NORMAL_LIGHTLEVEL_NIGHT - bLightLevel), NORMAL_LIGHTLEVEL_NIGHT ),
				(*iter)[0]->data.objectStatus );
		}
	}

	return( bonus );
}

INT16 GetBrightLightVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;
	OBJECTTYPE *pObj;
	UINT16 usItem;
	INVTYPE *pItem;

	// Snap: Scale the bonus with the light level

	//ADB and AXP 28.03.2007: CtH bug fix: We also want to check on a firing weapon, "raised" alone is not enough ;)
	bool usingGunScope = WeaponReady(pSoldier);
	// CHRISL:
	for (int i = 0; i < BODYPOSFINAL; i++)
	{
		// More optimization
		pObj = &( pSoldier->inv[i]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		// Snap (TODO): binoculars and such should not be active by default
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(usItem) || (IsWeapon(usItem) && usingGunScope == true ) )
		{
			bonus += BonusReduceMore( idiv( pItem->brightlightvisionrangebonus
				* (NORMAL_LIGHTLEVEL_DAY - bLightLevel), NORMAL_LIGHTLEVEL_DAY ),
				(*pObj)[0]->data.objectStatus );
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		pObj = &( pSoldier->inv[HANDPOS]);

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += BonusReduceMore( idiv( Item[iter->usItem].brightlightvisionrangebonus
				* (NORMAL_LIGHTLEVEL_DAY - bLightLevel), NORMAL_LIGHTLEVEL_DAY ),
				(*iter)[0]->data.objectStatus );
		}
	}

	return( bonus );
}

INT16 GetTotalVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel )
{
	PERFORMANCE_MARKER
	INT16 bonus = GetVisionRangeBonus(pSoldier);

	if ( bLightLevel > NORMAL_LIGHTLEVEL_DAY ) 
	{
		if ( pSoldier->pathing.bLevel == 0 ) 
		{
			bonus += GetNightVisionRangeBonus(pSoldier, bLightLevel);
		}
		else 
		{
			bonus += GetCaveVisionRangeBonus(pSoldier, bLightLevel);
		}
	}

	if ( bLightLevel < NORMAL_LIGHTLEVEL_NIGHT ) 
	{
		bonus += GetDayVisionRangeBonus(pSoldier, bLightLevel);
	}

	if ( bLightLevel < NORMAL_LIGHTLEVEL_DAY ) 
	{
		bonus += GetBrightLightVisionRangeBonus(pSoldier, bLightLevel);
	}

	return bonus;
}

UINT8 GetPercentTunnelVision( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	UINT8 bonus = 0;
	UINT16 usItem;
	INVTYPE *pItem;

	// CHRISL:
	for (int i = HELMETPOS; i < BODYPOSFINAL; i++)
	{
		// Okay, it's time for some optimization here
		usItem = pSoldier->inv[i].usItem;
		pItem = &(Item[usItem]);

		if ( (i == HANDPOS || i == SECONDHANDPOS) && (pItem->usItemClass & IC_ARMOUR || pItem->usItemClass & IC_FACE ))
		{
			continue;
		}

		if ( !IsWeapon(usItem) )
		{
			bonus = __max( bonus, pItem->percenttunnelvision );
		}
	}

	bool usingGunScope = WeaponReady(pSoldier);
	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		OBJECTTYPE *pObj = &(pSoldier->inv[HANDPOS]);
		usItem = pObj->usItem;
		pItem = &(Item[usItem]);

		if ( IsWeapon(usItem) ) //if not a weapon, then it was added already above
			bonus += Item[usItem].percenttunnelvision;

		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			bonus += Item[iter->usItem].percenttunnelvision;
		}
	}

	if ( PTR_OURTEAM ) // Madd: adjust tunnel vision by difficulty level
		return( bonus );
	else
		return bonus / gGameOptions.ubDifficultyLevel;
}


BOOLEAN HasThermalOptics( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER

	bool usingGunScope = WeaponReady(pSoldier);
	for (int i = 0; i < BIGPOCK1POS; i++)
	{
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
		{
			continue;
		}

		if (!IsWeapon(pSoldier->inv[i].usItem) || (IsWeapon(pSoldier->inv[i].usItem) && usingGunScope == true) )
		{
			if (Item[pSoldier->inv[i].usItem].thermaloptics)
				return TRUE;
		}
	}

	// Snap: check only attachments on a raised weapon!
	if ( usingGunScope == true )
	{
		OBJECTTYPE* pObj = &pSoldier->inv[HANDPOS];
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
			if ( Item[iter->usItem].thermaloptics )
				return TRUE;
		}
	}

	return( FALSE );
}


INT8 FindHearingAid( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	for (INT8 i = 0; i < BIGPOCK1POS; i++)
	{
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
		{
			continue;
		}

		if ( Item[pSoldier->inv[i].usItem].hearingrangebonus > 0 )
		{
			return( i );
		}
	}
	return( NO_SLOT );
}

INT16 GetHearingRangeBonus( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT16 bonus = 0;

	for (int i = 0; i < BIGPOCK1POS; i++)
	{
		if ( (i == HANDPOS || i == SECONDHANDPOS) && 
			   (Item[pSoldier->inv[i].usItem].usItemClass & IC_ARMOUR || Item[pSoldier->inv[i].usItem].usItemClass & IC_FACE ))
		{
			continue;
		}

		bonus += BonusReduceMore( Item[pSoldier->inv[i].usItem].hearingrangebonus, pSoldier->inv[i][0]->data.objectStatus );
		/*for (UINT8 i=0;i<MAX_ATTACHMENTS;i++)
		{
			if ( pSoldier->inv[bLoop].usAttachItem[i] != NONE && Item[pSoldier->inv[bLoop].usAttachItem[i]].hearingrangebonus  > 0 )
				bonus = bonus + Item[pSoldier->inv[bLoop].usAttachItem[i]].hearingrangebonus  ;
		}*/
	}
	return( bonus );
}


BOOLEAN IsDuckbill( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if (Item[pObj->usItem].duckbill || Item[(*pObj)[0]->data.gun.usGunAmmoItem].duckbill )
		return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].duckbill  )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}


UINT16 GetPercentNoiseVolume( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	UINT16 mod = 100 - BonusReduce( Item[pObj->usItem].percentnoisereduction, (*pObj)[0]->data.objectStatus );
	mod = mod * ( 100 - Item[(*pObj)[0]->data.gun.usGunAmmoItem].percentnoisereduction ) / 100;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		mod = mod * ( 100 - BonusReduce( Item[iter->usItem].percentnoisereduction, (*iter)[0]->data.objectStatus ) ) / 100;
	}

	return (mod > 0) ? mod : 0;
}


INT8 FindGasMask( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < HANDPOS; bLoop++)
	{
		if ( Item[pSoldier->inv[bLoop].usItem].gasmask )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}


BOOLEAN IsDetonatorAttached( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	//if (Item[pObj->usItem].detonator)
	//	return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].detonator  )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN IsRemoteDetonatorAttached( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	//if (Item[pObj->usItem].remotedetonator)
	//	return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].remotedetonator  )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN IsFlashSuppressor( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	//Madd: tracers automatically negate any muzzle flash suppression due to inherent lighting effects
	if (Item[pObj->usItem].usItemClass == IC_GUN && AmmoTypes[(*pObj)[0]->data.gun.ubGunAmmoType].tracerEffect && pSoldier->bDoBurst )
		return FALSE;

	if (Item[pObj->usItem].hidemuzzleflash )
		return TRUE;

	if ( Item[(*pObj)[0]->data.gun.usGunAmmoItem].hidemuzzleflash )
		return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].hidemuzzleflash  )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

INT16 GetFlashSuppressorStatus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 p=100;
	if (Item[pObj->usItem].hidemuzzleflash )
		p=(*pObj)[0]->data.objectStatus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].hidemuzzleflash  )
		{
			p =p+ (*iter)[0]->data.objectStatus;
		}
	}
	p = min(p,100);
	return p;
}

BOOLEAN IsGrenadeLauncherAttached( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if (Item[pObj->usItem].grenadelauncher )
		return TRUE;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].grenadelauncher )
		{
			return TRUE;
		}
	}
	return FALSE;
}

OBJECTTYPE* FindAttachment_GrenadeLauncher( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if (Item[pObj->usItem].grenadelauncher )
		return pObj;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].grenadelauncher )
		{
			return( &(*iter) );
		}
	}
	return( 0 );
}
INT8 GetGrenadeLauncherStatus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if (Item[pObj->usItem].grenadelauncher  )
		return (*pObj)[0]->data.objectStatus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].grenadelauncher )
		{
			return( (*iter)[0]->data.objectStatus );
		}
	}
	return( ITEM_NOT_FOUND );
}
UINT16 GetAttachedGrenadeLauncher( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	if (Item[pObj->usItem].grenadelauncher  )
		return pObj->usItem;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].grenadelauncher )
		{
			return( (UINT16) Item[iter->usItem].uiIndex );
		}
	}
	return( NONE );
}


INT16 GetAttachedArmourBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus=0;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += BonusReduce( Armour[Item[iter->usItem].ubClassIndex].ubProtection,
			(*iter)[0]->data.objectStatus );
	}
	return( bonus );
}


INT16 GetBulletSpeedBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = Item[pObj->usItem].bulletspeedbonus ;
	bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].bulletspeedbonus ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus = bonus +Item[iter->usItem].bulletspeedbonus  ;
	}
	return( bonus );
}

BOOLEAN EXPLOSIVE_GUN ( UINT16 x)
{
	PERFORMANCE_MARKER
	 //DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("EXPLOSIVE_GUN x = %d",x));

	if ( Item[x].rocketlauncher || Item[x].cannon )
		return TRUE;
	else
		return FALSE;
}

INT8 FindRocketLauncherOrCannon( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].rocketlauncher || Item[pSoldier->inv[bLoop].usItem].cannon )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindRocketLauncher( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].rocketlauncher )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindCannon( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if ( Item[pSoldier->inv[bLoop].usItem].cannon )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

INT8 FindUsableCrowbar( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if ( Item[pSoldier->inv[bLoop].usItem].crowbar && pSoldier->inv[bLoop][0]->data.objectStatus >= USABLE )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

OBJECTTYPE* FindAttachedBatteries( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].batteries )
		{
			return( &(*iter) );
		}
	}
	return( 0 );
}
INT8 FindToolkit( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].toolkit )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindMedKit( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].medicalkit  )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindFirstAidKit( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].firstaidkit  )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindCamoKit( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].camouflagekit   )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindLocksmithKit( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].locksmithkit   )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindWalkman( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < BIGPOCK1POS; bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].walkman  )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindTrigger( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < (INT8) pSoldier->inv.size(); bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].remotetrigger   )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}
INT8 FindRemoteControl( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8 bLoop;

	for (bLoop = 0; bLoop < BIGPOCK1POS; bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].robotremotecontrol    )
		{
			return( bLoop );
		}
	}
	return( NO_SLOT );
}

UINT16 LowestLaunchableCoolness(UINT16 launcherIndex)
{
	PERFORMANCE_MARKER
	UINT16 i = 0;
	UINT16 lowestCoolness = 999;

	for( i = 0; i < MAXITEMS; i++ )
	{
		if ( Item[i].usItemClass  == 0 )
			break;

		if( ValidLaunchable( i, launcherIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= lowestCoolness )
		{	
			lowestCoolness = Item[i].ubCoolness;
		}
	}
	return lowestCoolness;
}

UINT16 PickARandomLaunchable(UINT16 itemIndex)
{
	PERFORMANCE_MARKER
	UINT16 usNumMatches = 0;
	UINT16 usRandom = 0;
	UINT16 i = 0;
	UINT16 lowestCoolness = LowestLaunchableCoolness(itemIndex);
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomLaunchable: itemIndex = %d", itemIndex));

	while( !usNumMatches )
	{ //Count the number of valid launchables
		for( i = 0; i < MAXITEMS; i++ )
		{
			if ( Item[i].usItemClass  == 0 )
				break;
			//Madd: quickfix: make it not choose best grenades right away. 
			if( ValidLaunchable( i, itemIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= max(HighestPlayerProgressPercentage()/10,lowestCoolness) )
				usNumMatches++;
		}
	}
	if( usNumMatches )
	{
		usRandom = (UINT16)Random( usNumMatches );
		for( i = 0; i < MAXITEMS; i++ )
		{
			if ( Item[i].usItemClass  == 0 )
				break;

			if( ValidLaunchable( i, itemIndex ) && ItemIsLegal(i) && Item[i].ubCoolness <= max(HighestPlayerProgressPercentage()/10,lowestCoolness) )
			{	
				if( usRandom )
					usRandom--;
				else
				{
					return i;
				}	
			}
		}
	}

	return 0;
}
INT16 GetCamoBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = (INT16) (Item[pObj->usItem].camobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (!Item[iter->usItem].camouflagekit)
			bonus += (INT16) (Item[iter->usItem].camobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
	}
	return( bonus );
}
INT16 GetUrbanCamoBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = (INT16) (Item[pObj->usItem].urbanCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (!Item[iter->usItem].camouflagekit)
			bonus += (INT16) (Item[iter->usItem].urbanCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
	}
	return( bonus );
}
INT16 GetDesertCamoBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = (INT16) (Item[pObj->usItem].desertCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (!Item[iter->usItem].camouflagekit)
			bonus += (INT16) (Item[iter->usItem].desertCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
	}
	return( bonus );
}
INT16 GetSnowCamoBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus = (INT16) (Item[pObj->usItem].snowCamobonus);// * (WEAPON_STATUS_MOD((*pObj)[0]->data.objectStatus) / 100)) ;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (!Item[iter->usItem].camouflagekit)
			bonus += (INT16) (Item[iter->usItem].snowCamobonus);// * (WEAPON_STATUS_MOD((*iter)[0]->data.objectStatus) / 100));
	}
	return( bonus );
}
INT16 GetWornCamo( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem > NONE )
			ttl += GetCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].usItem > NONE )
				ttl += GetCamoBonus(&pSoldier->inv[bLoop]);
		}
	}

	return __min( ttl, 100 );
}
INT16 GetWornUrbanCamo( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem > NONE )
			ttl += GetUrbanCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].usItem > NONE )
				ttl += GetUrbanCamoBonus(&pSoldier->inv[bLoop]);
		}
	}

	return __min( ttl, 100 );
}
INT16 GetWornDesertCamo( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem > NONE )
			ttl += GetDesertCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].usItem > NONE )
				ttl += GetDesertCamoBonus(&pSoldier->inv[bLoop]);
		}
	}
	return __min( ttl, 100 );
}
INT16 GetWornSnowCamo( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem > NONE )
			ttl += GetSnowCamoBonus(&pSoldier->inv[bLoop]);
	}

	// CHRISL: Add additional loop for LBE items while using new inventory system
	if((UsingInventorySystem() == true))
	{
		for (bLoop = VESTPOCKPOS; bLoop <= BPACKPOCKPOS; bLoop++)
		{
			if ( pSoldier->inv[bLoop].usItem > NONE )
				ttl += GetSnowCamoBonus(&pSoldier->inv[bLoop]);
		}
	}
	return __min( ttl, 100 );
}

void ApplyEquipmentBonuses(SOLDIERTYPE * pSoldier)
{
	PERFORMANCE_MARKER
	if ( pSoldier == NULL) return;

	INT16 newCamo = GetWornCamo ( pSoldier );
	INT16 oldCamo = pSoldier->wornCamo;
	if ( oldCamo != newCamo )
		pSoldier->wornCamo = (INT8)newCamo;

	INT16 newUrbanCamo = GetWornUrbanCamo ( pSoldier );
	INT16 oldUrbanCamo = pSoldier->wornUrbanCamo;
	if ( oldUrbanCamo != newUrbanCamo )
		pSoldier->wornUrbanCamo = (INT8)newUrbanCamo;

	INT16 newDesertCamo = GetWornDesertCamo ( pSoldier );
	INT16 oldDesertCamo = pSoldier->wornDesertCamo;
	if ( oldDesertCamo != newDesertCamo )
		pSoldier->wornDesertCamo = (INT8)newDesertCamo;

	INT16 newSnowCamo = GetWornSnowCamo ( pSoldier );
	INT16 oldSnowCamo = pSoldier->wornSnowCamo;
	if ( oldSnowCamo != newSnowCamo )
		pSoldier->wornSnowCamo = (INT8)newSnowCamo;

	if ( (newCamo > oldCamo || newUrbanCamo > oldUrbanCamo || newDesertCamo > oldDesertCamo || newSnowCamo > oldSnowCamo )&& pSoldier->bTeam == OUR_TEAM )
	{	
		pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
		
		// WANNE: Only call the method if oldCame != newCamo
		if ( pSoldier->bInSector)
			pSoldier->CreateSoldierPalettes( );
	}
	else if ( (newCamo < oldCamo || newUrbanCamo < oldUrbanCamo || newDesertCamo < oldDesertCamo || newSnowCamo < oldSnowCamo )&& pSoldier->bTeam == OUR_TEAM )
	{
		// WANNE: Only call the method if oldCame != newCamo
		if ( pSoldier->bInSector)
			pSoldier->CreateSoldierPalettes( );
	}
	// WANNE: Madd, I commented this, because this leads to IRAs INVISIBLE BUG!
	// We should only call the CreateSoldierPalettes if oldCamo != newCamo. See above!
	//Madd: do this regardless of camo.  This will need to be called to do custom part colours and new overlays anyway.
	//if ( pSoldier->bInSector)
	//	pSoldier->CreateSoldierPalettes( );
	
	fInterfacePanelDirty = DIRTYLEVEL2;
}

UINT16 GetFirstExplosiveOfType(UINT16 expType)
{
	PERFORMANCE_MARKER
	for (int i=0;i<MAXITEMS;i++)
	{
		if ( (Item[i].usItemClass == IC_EXPLOSV || Item[i].usItemClass == IC_GRENADE) && Explosive[Item[i].ubClassIndex].ubType == expType )
			return i;
	}

	return 0;
}

OBJECTTYPE* FindSunGogglesInInv( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16	bonusToBeat = 0;
	OBJECTTYPE*	pGoggles = 0;
	// CHRISL:
	// CHRISL:
	for (bLoop = HANDPOS; bLoop < NUM_INV_SLOTS; bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].brightlightvisionrangebonus > bonusToBeat && Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_FACE )			
		{
			pGoggles = &(pSoldier->inv[bLoop]);
			bonusToBeat = Item[pSoldier->inv[bLoop].usItem].brightlightvisionrangebonus;
		}
	}
	return( pGoggles );
}

OBJECTTYPE* FindNightGogglesInInv( SOLDIERTYPE * pSoldier  )
{
	PERFORMANCE_MARKER
	INT8	bLoop;
	INT16	bonusToBeat = 0;
	OBJECTTYPE*	pGoggles = 0;
	// CHRISL:
	// CHRISL:
	for (bLoop = HANDPOS; bLoop < NUM_INV_SLOTS; bLoop++)
	{
		if (Item[pSoldier->inv[bLoop].usItem].nightvisionrangebonus > bonusToBeat && Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_FACE )			
		{
			pGoggles = &(pSoldier->inv[bLoop]);
			bonusToBeat = Item[pSoldier->inv[bLoop].usItem].nightvisionrangebonus;
		}
	}
	return( pGoggles );
}

INT16 GetMinRangeForAimBonus( OBJECTTYPE * pObj )
{
	PERFORMANCE_MARKER
	INT16 bonus;

	bonus = Item[pObj->usItem].minrangeforaimbonus;
	//bonus += Item[(*pObj)[0]->data.gun.usGunAmmoItem].minrangeforaimbonus;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += Item[iter->usItem].minrangeforaimbonus;
	}

	return( bonus );
}

UINT8 AllowedAimingLevels(SOLDIERTYPE * pSoldier)
{
	PERFORMANCE_MARKER
	UINT8 aimLevels = 4;
	float iScopeBonus = 0;
	gTempObject = pSoldier->inv[pSoldier->ubAttackingHand]; 
	BOOLEAN allowed = TRUE;
	
	if ( gGameSettings.fOptions[TOPTION_AIM_LEVEL_RESTRICTION] && Weapon[gTempObject.usItem].ubWeaponType != GUN_RIFLE && Weapon[gTempObject.usItem].ubWeaponType != GUN_SN_RIFLE )
			allowed = FALSE;

	if ( allowed && IsScoped( &gTempObject ) )
	{
		iScopeBonus = ( (float)gGameExternalOptions.ubStraightSightRange * GetMinRangeForAimBonus(&gTempObject) / 100 );

		if ( iScopeBonus >= ( (float)gGameExternalOptions.ubStraightSightRange * 0.3) ) // >= 30% of sight range (~4 tiles by default)
		{
			aimLevels += 2;
		}

		if ( iScopeBonus >= ( (float)gGameExternalOptions.ubStraightSightRange * 0.6) ) // >= 60% of sight range (~9 tiles by default)
		{			
			aimLevels += 2;
		}
	}

	return aimLevels;
}
//Madd: added
INT16 GetStealthBonus( OBJECTTYPE * pObj ) 
{
	INT16 bonus  = (INT16) BonusReduce(Item[pObj->usItem].stealthbonus,(*pObj)[0]->data.objectStatus);

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		bonus += (INT16) BonusReduce(Item[iter->usItem].stealthbonus,(*iter)[0]->data.objectStatus);
	}
	return( bonus );
}
INT16 GetWornStealth( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	//note: Stealth bonus is capped at 100
	//note: Stealth is not a perk! Stealth bonus only applies to equipment, and stacks with camouflage
	//note: stealth bonus is not affected by terrain like the camo bonus, otherwise they're very similar
	//note: stealth bonus also affects noise made by characters walking
	INT8	bLoop;
	INT16 ttl=0;

	for (bLoop = HELMETPOS; bLoop <= LEGPOS; bLoop++)
	{
		if ( pSoldier->inv[bLoop].usItem > NONE )
			ttl += GetStealthBonus(&pSoldier->inv[bLoop]);
	}

	return __min( ttl, 100 );
}
