#ifndef WORLD_TILESET_NUMS_H
#define WORLD_TILESET_NUMS_H

// Snap: NUM_TILESETS no longer limits the number of tilesets.
// See gubNumSets in WorldDat.cpp

enum
{
	TLS_GENERIC_1,           // 0, JA2 default
	TLS_CAVES_1,
	TLS_DESERT_1,
	TLS_LUSH_1,
	TLS_TROPICAL_1,
	TLS_MOUNTAINS_1,
	TLS_COASTAL_1,
	TLS_SWAMP_1,
	TLS_FARM_1,
	TLS_OMERTA,

	TLS_GENERIC_2,           // 10
	TLS_FARM_2,
	TLS_PRISON,
	TLS_HOSPITAL,
	TLS_DEMO_BASEMENT,
	TLS_BURNT_TREES,
	TLS_LAWLESS_1,
	TLS_AIRSTRIP,
	TLS_LAWLESS_2,
	TLS_DEAD_AIRSTRIP,

	TLS_BASEMENT,            // 20
	TLS_LAWLESS_3,
	TLS_PRISON_DUNGEON,
	TLS_ACTIVE_DRASSEN,
	TLS_SAM_SITES,
	TLS_LUSH_2,
	TLS_MILITARY_BASE,
	TLS_MILITARY_JAIL,
	TLS_MILITARY_WAREHOUSE,
	TLS_MILITARY_TOWN,

	TLS_OLD_SCHOOL,          // 30
	TLS_CAMBRIA_STRIP,
	TLS_CAMBRIA_HOMES,
	TLS_PALACE,
	TLS_TROPICAL_SAM,
	TLS_GRUMM_1,
	TLS_GRUMM_2,
	TLS_BALIME,
	TLS_BALIME_MUSEUM,
	TLS_DESERT_SAM,

	TLS_ORTA,                // 40
	TLS_ORTA_WEAPONS,
	TLS_SWAMP_BARETREES,
	TLS_ESTONI,
	TLS_QUEENS_PRISON,
	TLS_QUEENS_TROPICAL,
	TLS_MEDUNA_INNER_TOWN,
	TLS_QUEENS_SAM,
	TLS_QUEENS_AIRPORT,
	TLS_DEMO_TILESET,

	TLS_HEAVY_SNOW,          // 50, JA2.5 default
	TLS_MIXED_SNOW,
	TLS_GRASS_SNOW,
	TLS_FALL_TOWN,
	TLS_MINING_TOWN,
	TLS_POWER_PLANT,
	TLS_SEWERS,
	TLS_UNDERGROUND_COMPLEX,
	TLS_UPPER_COMPLEX,
	TLS_LOWEST_LEVEL_COMPLEX,

	TILESET_60,              // 60
	TILESET_61,
	TILESET_62,
	TILESET_63,
	TILESET_64,
	TILESET_65,
	TILESET_66,
	TILESET_67,
	TILESET_68,
	TILESET_69,

	NUM_TILESETS             // 70
};

#endif