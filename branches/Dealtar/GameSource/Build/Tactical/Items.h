#ifndef ITEMS_H
#define ITEMS_H
#include "Item Types.h"
#include "Soldier Control.h"
#include "Overhead Types.h"

// Snap: Moved here from opplist.cpp
// These defined are no longer used to calculate item bonuses,
// but for lack of anything better, they are still used in other places
#define NIGHTSIGHTGOGGLES_BONUS 2 * STRAIGHT_RATIO
#define UVGOGGLES_BONUS 4 * STRAIGHT_RATIO

extern UINT8 SlotToPocket[7];

// WDS - Clean up inventory handling
class SOLDIERTYPE;
extern BOOLEAN WeaponInHand( SOLDIERTYPE * pSoldier );

extern INT8 FindObj( SOLDIERTYPE * pSoldier, UINT16 usItem );
INT8 FindAmmo( SOLDIERTYPE * pSoldier, UINT8 ubCalibre, UINT8 ubMagSize, INT8 bExcludeSlot );

extern INT8 FindAttachment( OBJECTTYPE * pObj, UINT16 usItem );
extern INT8 FindObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass );
extern INT8 FindObjClassAfterSlot( SOLDIERTYPE * pSoldier, INT8 bStart, UINT32 usItemClass );
extern INT8 FindAIUsableObjClass( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass );
extern INT8 FindAIUsableObjClassWithin( SOLDIERTYPE * pSoldier, 	UINT32 usItemClass, INT8 bLower, INT8 bUpper );
extern INT8 FindEmptySlotWithin( SOLDIERTYPE * pSoldier, INT8 bLower, INT8 bUpper );
extern INT8 FindExactObj( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj );
extern INT8 FindObjInObjRange( SOLDIERTYPE * pSoldier, UINT16 usItem1, UINT16 usItem2 );
extern INT8 FindLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon );
extern INT8 FindGLGrenade( SOLDIERTYPE * pSoldier );
extern INT8 FindThrowableGrenade( SOLDIERTYPE * pSoldier );
extern INT8 FindUsableObj( SOLDIERTYPE * pSoldier, UINT16 usItem );

extern void DeleteObj(OBJECTTYPE * pObj );
extern void CopyObj( OBJECTTYPE * pSourceObj, OBJECTTYPE * pTargetObj );
extern void SwapObjs( OBJECTTYPE * pObj1, OBJECTTYPE * pObj2 );
extern void SwapObjs(SOLDIERTYPE* pSoldier, int leftSlot, int rightSlot, BOOLEAN fPermanent);
extern void SwapObjs(SOLDIERTYPE* pSoldier, int slot, OBJECTTYPE* pObject, BOOLEAN fPermanent);

extern void SwapWithinObj( OBJECTTYPE * pObj, UINT8 ubIndex1, UINT8 ubIndex2 );
extern void RemoveObjFrom( OBJECTTYPE * pObj, UINT8 ubRemoveIndex );
//Returns true if swapped, false if added to end of stack
extern BOOLEAN PlaceObjectAtObjectIndex( OBJECTTYPE * pSourceObj, OBJECTTYPE * pTargetObj, UINT8 ubIndex );
extern void GetObjFrom( OBJECTTYPE * pObj, UINT8 ubGetIndex, OBJECTTYPE * pDest );

extern BOOLEAN AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pTargetObj, OBJECTTYPE * pAttachment, BOOLEAN playSound );
extern BOOLEAN AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pTargetObj, OBJECTTYPE * pAttachment );
extern BOOLEAN RemoveAttachment( OBJECTTYPE * pObj, INT8 bAttachPos, OBJECTTYPE * pNewObj );

extern UINT8	CalculateObjectWeight( OBJECTTYPE *pObject );
extern UINT32 CalculateCarriedWeight( SOLDIERTYPE * pSoldier );

extern UINT16 TotalPoints( OBJECTTYPE * pObj );
extern UINT16 UseKitPoints( OBJECTTYPE * pObj, UINT16 usPoints, SOLDIERTYPE *pSoldier );

extern BOOLEAN EmptyWeaponMagazine( OBJECTTYPE * pWeapon, OBJECTTYPE *pAmmo );
extern BOOLEAN CreateItem( UINT16 usItem, INT8 bStatus, OBJECTTYPE * pObj );
extern BOOLEAN CreateItems( UINT16 usItem, INT8 bStatus, UINT8 ubNumber, OBJECTTYPE * pObj );
extern BOOLEAN CreateMoney( UINT32 uiMoney, OBJECTTYPE * pObj );
extern UINT16 DefaultMagazine( UINT16 usItem );
UINT16 RandomMagazine( UINT16 usItem, UINT8 ubPercentStandard );
UINT16 RandomMagazine( OBJECTTYPE * pGun, UINT8 ubPercentStandard );
extern BOOLEAN ReloadGun( SOLDIERTYPE * pSoldier, OBJECTTYPE * pGun, OBJECTTYPE * pAmmo );
extern BOOLEAN UnloadGun( SOLDIERTYPE * pSoldier, OBJECTTYPE * pGun); 

UINT8 ItemSlotLimit( UINT16 usItem, INT8 bSlot );

// Function to put an item in a soldier profile
// It's very primitive, just finds an empty place!
BOOLEAN PlaceObjectInSoldierProfile( UINT8 ubProfile, OBJECTTYPE *pObject );
BOOLEAN RemoveObjectFromSoldierProfile( UINT8 ubProfile, UINT16 usItem );
INT8 FindObjectInSoldierProfile( UINT8 ubProfile, UINT16 usItem );

BOOLEAN ObjectExistsInSoldierProfile( UINT8 ubProfile, UINT16 usItem );
void SetMoneyInSoldierProfile( UINT8 ubProfile, UINT32 uiMoney );

void CheckEquipmentForDamage( SOLDIERTYPE *pSoldier, INT32 iDamage );
BOOLEAN ArmBomb( OBJECTTYPE * pObj, INT8 bSetting );

// NOTE TO ANDREW: 
// 
// The following functions expect that pObj points to the object
// "in the cursor", which should have memory allocated for it already
BOOLEAN PlaceObject( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj );

// Send fNewItem to true to set off new item glow in inv panel
BOOLEAN AutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem );
BOOLEAN RemoveObjectFromSlot( SOLDIERTYPE * pSoldier, INT8 bPos, OBJECTTYPE * pObj );

void RemoveInvObject( SOLDIERTYPE *pSoldier, UINT16 usItem );

UINT8 SwapKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );

// create a keyobject
BOOLEAN CreateKeyObject( OBJECTTYPE * pObj , UINT8 ubNumberOfKeys, UINT8 ubKeyIdValue );
BOOLEAN DeleteKeyObject( OBJECTTYPE * pObj );
BOOLEAN AllocateObject( OBJECTTYPE **pObj );

// removes a key from a *KEYRING* slot
BOOLEAN RemoveKeyFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );

// take several
BOOLEAN RemoveKeysFromSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, UINT8 ubNumberOfKeys ,OBJECTTYPE * pObj );

// add the keys to an inventory slot
UINT8 AddKeysToSlot( SOLDIERTYPE * pSoldier, INT8 bKeyRingPosition, OBJECTTYPE * pObj );



//Kris:  December 9, 1997
//I need a bunch of validation functions for ammo, attachments, etc., so I'll be adding them here.
//Chris, maybe you might find these useful, or add your own.  I don't really know what I'm doing yet,
//so feel free to correct me...

//Simple check to see if the item has any attachments
BOOLEAN ItemHasAttachments( OBJECTTYPE *pItem );

//Determine if this item can receive this attachment.  This is different, in that it may
//be possible to have this attachment on this item, but may already have an attachment on
//it which doesn't work simultaneously with the new attachment (like a silencer and duckbill).
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment );
BOOLEAN ValidItemAttachment( OBJECTTYPE * pObj, UINT16 usAttachment, BOOLEAN fAttemptingAttachment, BOOLEAN fDisplayMessage );

// Determines if it is possible to merge an item with any item whose class 
// is the same as the indicated item
BOOLEAN ValidAttachmentClass( UINT16 usAttachment, UINT16 usItem );

//Determines if it is possible to equip this weapon with this ammo.
BOOLEAN ValidAmmoType( UINT16 usItem, UINT16 usAmmoType );

//Determines if this item is a two handed item.
BOOLEAN TwoHandedItem( UINT16 usItem );

//Existing functions without header def's, added them here, just incase I'll need to call
//them from the editor.
BOOLEAN ValidAttachment( UINT16 usAttachment, UINT16 usItem );
UINT8 AttachmentAPCost( UINT16 usAttachment, UINT16 usItem );

BOOLEAN ValidLaunchable( UINT16 usLaunchable, UINT16 usItem );
UINT16 GetLauncherFromLaunchable( UINT16 usLaunchable );

BOOLEAN ValidMerge( UINT16 usMerge, UINT16 usItem );
BOOLEAN EvaluateValidMerge( UINT16 usMerge, UINT16 usItem, UINT16 * pusResult, UINT16 * pusResult2, UINT8 * pubType, UINT8 * pubAPCost );


// is the item passed a medical/ first aid kit? and what type?
INT8 IsMedicalKitItem( OBJECTTYPE *pObject );

BOOLEAN AutoReload( SOLDIERTYPE * pSoldier );
INT8 FindAmmoToReload( SOLDIERTYPE * pSoldier, INT8 bWeaponIn, INT8 bExcludeSlot );

void SwapOutHandItem( SOLDIERTYPE * pSoldier );
void SwapHandItems( SOLDIERTYPE * pSoldier );

INT8 FindAttachmentByClass( OBJECTTYPE * pObj, UINT32 uiItemClass );
void RemoveObjs( OBJECTTYPE * pObj, UINT8 ubNumberToRemove );

void WaterDamage( SOLDIERTYPE *pSoldier );

INT8 FindObjWithin( SOLDIERTYPE * pSoldier, UINT16 usItem, INT8 bLower, INT8 bUpper );

BOOLEAN ApplyCammo( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs );

UINT32 ConvertProfileMoneyValueToObjectTypeMoneyValue( UINT8 ubStatus );
UINT8 ConvertObjectTypeMoneyValueToProfileMoneyValue( UINT32 uiMoneyAmount );

BOOLEAN CheckForChainReaction( UINT16 usItem, INT8 bStatus, INT8 bDamage, BOOLEAN fOnGround );

BOOLEAN ItemIsLegal( UINT16 usItemIndex );
BOOLEAN ExtendedGunListGun( UINT16 usGun );
UINT16 StandardGunListReplacement( UINT16 usGun );
UINT16 FindReplacementMagazine( UINT8 ubCalibre, UINT8 ubMagSize, UINT8 ubAmmoType);
UINT16 FindReplacementMagazineIfNecessary( UINT16 usOldGun, UINT16 usOldAmmo, UINT16 usNewGun );

BOOLEAN DamageItemOnGround( OBJECTTYPE * pObject, INT16 sGridNo, INT8 bLevel, INT32 iDamage, UINT8 ubOwner );

BOOLEAN ApplyCanteen( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs );
BOOLEAN ApplyElixir( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN *pfGoodAPs );

BOOLEAN CompatibleFaceItem( UINT16 usItem1, UINT16 usItem2 );

UINT32 MoneySlotLimit( INT8 bSlot );

void CheckEquipmentForFragileItemDamage( SOLDIERTYPE *pSoldier, INT32 iDamage );

// Range of Xray device
#define XRAY_RANGE 40
// Seconds that Xray lasts
#define XRAY_TIME 5

extern void ActivateXRayDevice( SOLDIERTYPE * pSoldier );
extern void TurnOffXRayEffects( SOLDIERTYPE * pSoldier );
extern INT8 FindLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon );

void DamageObj( OBJECTTYPE * pObj, INT8 bAmount );

UINT16 MagazineClassIndexToItemType(UINT16 usMagIndex);

// Item property-related stuff added by Madd Mugsy

BOOLEAN IsScoped( const OBJECTTYPE * pObj );
INT16 GetAimBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 ubAimTime );
INT16 GetMinAimBonusRange( OBJECTTYPE * pObj );
INT16 GetToHitBonus( OBJECTTYPE * pObj, INT32 iRange, UINT8 bLightLevel, BOOLEAN fProneStance = FALSE );
UINT32 FindRangeBonusAttachment( OBJECTTYPE * pObj );
INT16 GetRangeBonus( OBJECTTYPE * pObj );
INT16 GetBurstToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance = FALSE );

INT16 GetDamageBonus( OBJECTTYPE * pObj );
INT16 GetMeleeDamageBonus( OBJECTTYPE * pObj );

INT16 GetPercentAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentAutofireAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentBurstFireAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentReadyTimeAPReduction( OBJECTTYPE * pObj );
INT16 GetPercentReloadTimeAPReduction( OBJECTTYPE * pObj );
INT16 GetAutoToHitBonus( OBJECTTYPE * pObj, BOOLEAN fProneStance = FALSE  );
INT16 GetRateOfFireBonus( OBJECTTYPE * pObj );
INT16 GetBurstSizeBonus( OBJECTTYPE * pObj );
BOOLEAN HasFastBurst( OBJECTTYPE * pObj );
INT16 GetMagSizeBonus( OBJECTTYPE * pObj );

// Snap: GetTotalVisionRangeBonus checks for light levels
// and returns cumulative bonus from the following functions:
//
// GetVisionRangeBonus applies to all light levels
// GetNightVisionRangeBonus: bLightLevel > NORMAL_LIGHTLEVEL_DAY, above ground only
// GetCaveVisionRangeBonus: bLightLevel > NORMAL_LIGHTLEVEL_DAY, underground only
// GetDayVisionRangeBonus: NORMAL_LIGHTLEVEL_DAY <= bLightLevel < NORMAL_LIGHTLEVEL_NIGHT
// GetBrightLightVisionRangeBonus: bLightLevel < NORMAL_LIGHTLEVEL_DAY
// (remember: darker is higher!)
INT16 GetTotalVisionRangeBonus( SOLDIERTYPE * pSoldier, UINT8 bLightLevel );

// Snap: Scale night vision bonus with light level
INT16 NightBonusScale( INT16 bonus, UINT8 bLightLevel );

UINT8 GetPercentTunnelVision( SOLDIERTYPE * pSoldier );
BOOLEAN HasThermalOptics( SOLDIERTYPE * pSoldier );

INT8 FindHearingAid( SOLDIERTYPE * pSoldier );
INT16 GetHearingRangeBonus( SOLDIERTYPE * pSoldier );

INT8 IsGrenadeLauncher( OBJECTTYPE * pObj );
INT8 GetGrenadeLauncherStatus( OBJECTTYPE * pObj );
BOOLEAN IsGrenadeLauncherAttached( OBJECTTYPE * pObj );
INT8 FindAttachment_GrenadeLauncher( OBJECTTYPE * pObj );
UINT16 GetAttachedGrenadeLauncher( OBJECTTYPE * pObj );
INT8 FindRocketLauncher( SOLDIERTYPE * pSoldier );
INT8 FindRocketLauncherOrCannon( SOLDIERTYPE * pSoldier );
INT8 FindNonSmokeLaunchable( SOLDIERTYPE * pSoldier, UINT16 usWeapon );
INT8 FindNonSmokeLaunchableAttachment( OBJECTTYPE * pObj, UINT16 usWeapon );
UINT16 PickARandomLaunchable(UINT16 itemIndex);

BOOLEAN IsFlashSuppressor( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier );
INT16 GetFlashSuppressorStatus( OBJECTTYPE * pObj );
BOOLEAN IsRemoteDetonator( OBJECTTYPE * pObj );
BOOLEAN IsDetonator( OBJECTTYPE * pObj );
BOOLEAN IsDuckbill( OBJECTTYPE * pObj );

// Snap: Returns CUMULATIVE noise volume modifier, with component status factored in.
// Noise volume is then calculated as volume * GetPercentNoiseVolume / 100
UINT16 GetPercentNoiseVolume( OBJECTTYPE * pObj );

INT8 FindFirstArmourAttachment( OBJECTTYPE * pObj );
INT16 GetAttachedArmourBonus( OBJECTTYPE * pObj );
INT16 GetBulletSpeedBonus( OBJECTTYPE * pObj );
INT8 FindGasMask( SOLDIERTYPE * pSoldier );
INT8 FindLockBomb( SOLDIERTYPE * pSoldier );
INT8 FindMetalDetector( SOLDIERTYPE * pSoldier );
INT8 FindCannon( SOLDIERTYPE * pSoldier );
INT8 FindUsableCrowbar( SOLDIERTYPE * pSoldier );
INT8 FindToolkit( SOLDIERTYPE * pSoldier );
BOOLEAN IsDetonatorAttached( OBJECTTYPE * pObj );
BOOLEAN IsRemoteDetonatorAttached( OBJECTTYPE * pObj );
INT8 FindAttachedBatteries( OBJECTTYPE * pObj );
INT8 FindMedKit( SOLDIERTYPE * pSoldier );
INT8 FindFirstAidKit( SOLDIERTYPE * pSoldier );
INT8 FindLocksmithKit( SOLDIERTYPE * pSoldier );
INT8 FindCamoKit( SOLDIERTYPE * pSoldier );
INT8 FindWalkman( SOLDIERTYPE * pSoldier );
INT8 FindTrigger( SOLDIERTYPE * pSoldier );
INT8 FindRemoteControl( SOLDIERTYPE * pSoldier );
INT16 GetWornCamo( SOLDIERTYPE * pSoldier );
INT16 GetCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornStealth( SOLDIERTYPE * pSoldier );
INT16 GetStealthBonus( OBJECTTYPE * pObj );

void ApplyEquipmentBonuses(SOLDIERTYPE * pSoldier);

INT16 GetGearAimBonus( SOLDIERTYPE * pSoldier, INT32 iRange, UINT8 ubAimTime);
INT16 GetGearToHitBonus( SOLDIERTYPE * pSoldier );
INT16 GetGearAPBonus( SOLDIERTYPE * pSoldier );

UINT16 GetFirstExplosiveOfType(UINT16 expType);

INT8 FindSunGoggles( SOLDIERTYPE * pSoldier, INT16 bonusToBeat );
INT8 FindNightGoggles( SOLDIERTYPE * pSoldier, INT16 bonusToBeat );

INT16 GetMinRangeForAimBonus( OBJECTTYPE * pObj );
UINT8 AllowedAimingLevels(SOLDIERTYPE * pSoldier);

INT16 GetWornUrbanCamo( SOLDIERTYPE * pSoldier );
INT16 GetUrbanCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornDesertCamo( SOLDIERTYPE * pSoldier );
INT16 GetDesertCamoBonus( OBJECTTYPE * pObj );
INT16 GetWornSnowCamo( SOLDIERTYPE * pSoldier );
INT16 GetSnowCamoBonus( OBJECTTYPE * pObj );



#endif
