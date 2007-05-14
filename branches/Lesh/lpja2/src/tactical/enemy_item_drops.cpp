#ifdef PRECOMPILEDHEADERS
	#include "tactical_all.h"
#else
	#include "types.h"
	#include "item_types.h"
	#include "debug.h"
	#include "enemy_item_drops.h"
#endif

WEAPON_DROPS gEnemyWeaponDrops[MAX_DROP_ITEMS];
AMMO_DROPS gEnemyAmmoDrops[MAX_DROP_ITEMS];
EXPLOSIVE_DROPS gEnemyExplosiveDrops[MAX_DROP_ITEMS];
ARMOUR_DROPS gEnemyArmourDrops[MAX_DROP_ITEMS];
MISC_DROPS gEnemyMiscDrops[MAX_DROP_ITEMS];