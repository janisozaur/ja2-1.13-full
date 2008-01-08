#pragma once

//this one just for structs, variables and functions used between the client and server scripts...

typedef struct
{
	UINT8 client_num;
	char client_name[30];
	int cl_ops[4];
}client_info;

typedef struct
{
	int	max_clients;
	int	same_merc;
	float damage_multiplier;
	int interrupts;
	INT16 gsMercArriveSectorX;
	INT16 gsMercArriveSectorY;
	int ENEMY_ENABLED;
	int	CREATURE_ENABLED;
	int	MILITIA_ENABLED;
	int	CIV_ENABLED;
	int gsPLAYER_BSIDE;
	INT32 secs_per_tick;
	INT32 starting_balance;
	bool soDis_Bobby;
	bool soDis_Equip;
	BOOLEAN sofGunNut;	
	UINT8	soubGameStyle;
	UINT8	soubDifficultyLevel;
	BOOLEAN	sofTurnTimeLimit;
	BOOLEAN	sofIronManMode;
	UINT8	soubBobbyRay;	
	INT32 gsMAX_MERCS;
	UINT8 client_num;
	char client_name[30];
	char client_names[4][30];
	int cl_ops[4];
} settings_struct;