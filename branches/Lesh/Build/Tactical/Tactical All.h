#ifndef __TACTICAL_ALL_H
#define __TACTICAL_ALL_H

#pragma message("GENERATED PCH FOR TACTICAL PROJECT.")

#include "sgp.h"
#include "air raid.h"
#include "game event hook.h"
#include "game clock.h"
#include "auto bandage.h"
#include "strategicmap.h"
#include "screenids.h"
#include "jascreens.h"
#include "random.h"
#include "overhead types.h"
#include "sound control.h"
#include "timer control.h"
#include "Dialogue Control.h"
#include "Overhead.h"
#include "message.h"
#include "isometric utils.h"
#include "soldier macros.h"
#include "worldman.h"
#include "los.h"
#include "math.h"
#include "explosion control.h"
#include "interface.h"
#include "music control.h"
#include "Campaign Types.h"
#include "GameSettings.h"
#include "Text.h"
#include "Morale.h"
#include "Map screen helicopter.h"
#include <stdio.h>
#include <string.h>
#include "Types.h"
#include "wcheck.h"
#include <stdlib.h>
#include "Animation Cache.h"
#include "Animation Data.h"
#include "Animation Control.h"
#include "sys globals.h"
#include "Debug Control.h"
#include "FileMan.h"
#include "weapons.h"
#include "structure.h"
#include "worlddef.h"
#include "rotting corpses.h"
#include "points.h"
#include "Soldier Control.h"
#include "tiledef.h"
#include "Utilities.h"
#include "Arms Dealer Init.h"
#include "ArmsDealerInvInit.h"
#include "soldier profile.h"
#include "Handle Items.h"
#include "Item Types.h"
#include "messageboxscreen.h"
#include "Handle UI.h"
#include "items.h"
#include "MercTextBox.h"
#include "renderworld.h"
#include "strategic turns.h"
#include "Event Pump.h"
#include "ai.h"
#include "interface control.h"
#include "Map Screen Interface.h"
#include "Map Screen Interface Bottom.h"
#include "Assignments.h"
#include "WordWrap.h"
#include "cursors.h"
#include "English.h"
#include "Boxing.h"
#include "Render Fun.h"
#include "NPC.h"
#include "Opplist.h"
#include <errno.h>
#include "vsurface.h"
#include "Render Dirty.h"
#include "sysutil.h"
#include "container.h"
#include "video.h"
#include "vobject_blitters.h"
#include "faces.h"
#include "gap.h"
#include "Bullets.h"
#include <wchar.h>
#include "MemMan.h"
#include "campaign.h"
#include "Strategic Mines.h"
#include "Strategic Status.h"
#include "Encrypted File.h"
#include "mercs.h"
#include "interface dialogue.h"
#include "squads.h"
#include "interface utils.h"
#include "Quests.h"
#include "gamescreen.h"
#include "ShopKeeper Interface.h"
#include "Merc Contract.h"
#include "history.h"
#include "Town Militia.h"
#include "meanwhile.h"
#include "SkillCheck.h"
#include "finances.h"
#include "drugs and alcohol.h"
#include "teamturns.h"
#include "font control.h"
#include "line.h"
#include "structure wrap.h"
#include "pathai.h"
#include "smell.h"
#include "fov.h"
#include "keys.h"
#include "input.h"
#include "exit grids.h"
#include "environment.h"
#include "Fog Of War.h"
#include "soundman.h"
#include <stdarg.h>
#include <time.h>
#include "tile animation.h"
#include "Interactive Tiles.h"
#include "handle doors.h"
#include "Action Items.h"
#include "World items.h"
#include "interface items.h"
#include "physics.h"
#include "interface panels.h"
#include "Strategic Town Loyalty.h"
#include "soldier functions.h"
#include "SaveLoadMap.h"
#include "soldier add.h"
#include "soldier ani.h"
#include "qarray.h"
#include "Handle UI Plan.h"
#include "soldier create.h"
#include "mousesystem.h"
#include "cursor control.h"
#include "interface cursors.h"
#include "UI cursors.h"
#include "Strategic Pathing.h"
#include "strategic movement.h"
#include "strategic.h"
#include "vehicles.h"
#include "gameloop.h"
#include "himage.h"
#include "vobject.h"
#include "Button System.h"
#include "radar screen.h"
#include "lighting.h"
#include "Strategic Exit GUI.h"
#include "PopUpBox.h"
#include "spread burst.h"
#include "Tactical Save.h"
#include "fade screen.h"
#include "Strategic AI.h"
#include "mapscreen.h"
#include "LaptopSave.h"
#include "Map Screen Interface Map.h"
#include "Map Screen Interface Map Inventory.h"
#include "overhead map.h"
#include "Options Screen.h"
#include <memory.h>
#include "Inventory Choosing.h"
#include "Smoothing Utils.h"
#include "tiledat.h"
#include <math.h>
#include "phys math.h"
#include "Map Information.h"
#include "Soldier Init List.h"
#include "EditorMercs.h"
#include "ja2.h"
#include "Road Smoothing.h"
#include "tile cache.h"
#include "merc entering.h"
#include "Merc Hiring.h"
#include "Strategic Merc Handler.h"
#include "Militia Control.h"
#include "Queen Command.h"
#include "editscreen.h"  
#include "soldier tile.h"
#ifdef  NETWORKED
	#include "Networking.h"
	#include "NetworkEvent.h"
#endif
#include "Player Command.h"
#include "Game Init.h"
#include "Buildings.h"
#include "rt time defines.h"
#include	"GameSettings.h"
#include "Text Input.h"
#include "ShopKeeper Quotes.h"
#include "Personnel.h"
#include "pits.h"
#include "smokeeffects.h"
#include "SaveLoadGame.h"
#include "Scheduling.h"
#include "Auto Resolve.h"
#include "soldier find.h"
#include "aim.h"
#include "strategic town reputation.h"
#include "Tactical Turns.h"
#include "lighteffects.h"
#include "soldier profile type.h"
#include "AIList.h"
#include "QuestDebug.h"
#include "Game Events.h"
#include "BobbyR.h"

#include "EnemyItemDrops.h"

#ifdef JA2BETAVERSION
	#include	"Quest Debug System.h"
#endif

#include "GameVersion.h"
#include "SaveLoadScreen.h"
#include "Cheats.h"
#include "Animated ProgressBar.h"
#include "civ quotes.h"
#include "AimMembers.h"
#include "BobbyRMailOrder.h"
#include "end game.h"
#include "DisplayCover.h"
#include "expat.h"
#include "spread burst.h"
#include "XML.h"

#endif