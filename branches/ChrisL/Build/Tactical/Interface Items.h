#ifndef __INTERFACE_ITEMS_H
#define __INTERFACE_ITEMS_H

#include "items.h"
#include "Handle Items.h"

// DEFINES FOR ITEM SLOT SIZES IN PIXELS
#define		BIG_INV_SLOT_WIDTH				61
#define		BIG_INV_SLOT_HEIGHT				22
#define		SM_INV_SLOT_WIDTH					30
#define		SM_INV_SLOT_HEIGHT				23
#define		VEST_INV_SLOT_WIDTH				43
#define		VEST_INV_SLOT_HEIGHT			24
#define		LEGS_INV_SLOT_WIDTH				43
#define		LEGS_INV_SLOT_HEIGHT			24
#define		HEAD_INV_SLOT_WIDTH				43
#define		HEAD_INV_SLOT_HEIGHT			24



// A STRUCT USED INTERNALLY FOR INV SLOT REGIONS
typedef struct
{
	BOOLEAN		fBigPocket;
	INT16			sBarDx;
	INT16			sBarDy;
	INT16			sWidth;
	INT16			sHeight;
	INT16			sX;				// starts at 0, gets set via InitInvSlotInterface()
	INT16			sY;				// starts at 0, gets set via InitInvSlotInterface()
} INV_REGIONS;

// USED TO SETUP REGION POSITIONS, ETC
typedef struct
{
	INT16			sX;
	INT16			sY;
} INV_REGION_DESC;


class OLD_ITEM_CURSOR_SAVE_INFO_101
{
public:
	OLD_OBJECTTYPE_101	ItemPointerInfo;
	UINT8				ubSoldierID;
	UINT8				ubInvSlot;
	BOOLEAN				fCursorActive;
	INT8				bPadding[5];

};


class ITEM_CURSOR_SAVE_INFO
{
public:
	ITEM_CURSOR_SAVE_INFO& operator=(OLD_ITEM_CURSOR_SAVE_INFO_101& src)
	{
		this->fCursorActive = src.fCursorActive;
		this->ItemPointerInfo = src.ItemPointerInfo;
		this->ubInvSlot = src.ubInvSlot;
		this->ubSoldierID = src.ubSoldierID;
		return *this;
	}
	//could use a little tidying up
	BOOLEAN Save(HWFILE hFile);
	BOOLEAN Load(HWFILE hFile);

	UINT8				ubSoldierID;
	UINT8				ubInvSlot;
	BOOLEAN				fCursorActive;
	OBJECTTYPE			ItemPointerInfo;
};

// Itempickup stuff
BOOLEAN InitializeItemPickupMenu( SOLDIERTYPE *pSoldier, INT16 sGridNo, ITEM_POOL *pItemPool, INT16 sScreenX, INT16 sScreenY, INT8 bZLevel );
void RenderItemPickupMenu( );
void RemoveItemPickupMenu( );
void SetItemPickupMenuDirty( BOOLEAN fDirtyLevel );
BOOLEAN HandleItemPickupMenu( );
void SetPickUpMenuDirtyLevel( BOOLEAN fDirtyLevel );


// FUNCTIONS FOR INTERFACEING WITH ITEM PANEL STUFF
void INVRenderINVPanelItem( SOLDIERTYPE *pSoldier, INT16 sPocket, UINT8 fDirtyLevel );
BOOLEAN InitInvSlotInterface( INV_REGION_DESC *pRegionDesc , INV_REGION_DESC *pCamoRegion, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, MOUSE_CALLBACK INVMoveCammoCallback, MOUSE_CALLBACK INVClickCammoCallback, BOOLEAN fSetHighestPrioity );
void ShutdownInvSlotInterface( );
void GetSlotInvXY( UINT8 ubPos, INT16 *psX, INT16 *psY );
void GetSlotInvHeightWidth( UINT8 ubPos, INT16 *psWidth, INT16 *psHeight );
void HandleRenderInvSlots( SOLDIERTYPE *pSoldier, UINT8 fDirtyLevel );
void HandleNewlyAddedItems( SOLDIERTYPE *pSoldier, BOOLEAN *fDirtyLevel );
void RenderInvBodyPanel( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY );
void DisableInvRegions( BOOLEAN fDisable );

void DegradeNewlyAddedItems( );
void CheckForAnyNewlyAddedItems( SOLDIERTYPE *pSoldier );


BOOLEAN HandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, INT8 bInvPos, BOOLEAN fOn );



// THIS FUNCTION IS CALLED TO RENDER AN ITEM.
// uiBuffer - The Dest Video Surface - can only be FRAME_BUFFER or guiSAVEBUFFER
// pSoldier - used for determining whether burst mode needs display
// pObject	- Usually taken from pSoldier->inv[HANDPOS]
// sX, sY, Width, Height,	- Will Center it in the Width
// fDirtyLevel	if == DIRTYLEVEL2 will render everything
//							if == DIRTYLEVEL1 will render bullets and status only
// 
//	pubHighlightCounter - if not null, and == 2 - will display name above item
//											-	if == 1 will only dirty the name space and then set counter to 0
//	Last parameter used mainly for when mouse is over item

void INVRenderItem( UINT32 uiBuffer, SOLDIERTYPE * pSoldier, OBJECTTYPE	*pObject, INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, UINT8 fDirtyLevel, UINT8 *pubHighlightCounter, UINT8 ubStatusIndex, BOOLEAN fOutline, INT16 sOutlineColor, UINT8 iter = 0 );
// CHRISL: Add a new function that will be used to render a pocket silhouette
void INVRenderSilhouette( UINT32 uiBugger, INT16 PocketIndex, INT16 SilIndex, INT16 sX, INT16 sY, INT16 sWideth, INT16 sHeight);
// CHRISL: New function to handle display of inventory quantities based on item current in cursor
void RenderPocketItemCapacity( INT8 pCapacity, INT16 bPos, SOLDIERTYPE *pSoldier );
// CHRISL: New function to display items stored in an LBENODE
void RenderLBENODEItems( OBJECTTYPE *pObj, BOOLEAN activeNode, BOOLEAN stratScreen, int subObject );
// CHRISL: New function to setup GSMInvData based on game options
void InitInvData(INT8 num, BOOLEAN fBigPocket, INT16 sBarDx, INT16 sBarDy, INT16 sWidth, INT16 sHeight, INT16 sX, INT16 sY);
void InitInventoryOld();
void InitInventoryNew();
void InitInventoryVehicle(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity);
void InitInventorySoldier(INV_REGION_DESC *pRegionDesc, MOUSE_CALLBACK INVMoveCallback, MOUSE_CALLBACK INVClickCallback, BOOLEAN fSetHighestPrioity);



extern BOOLEAN		gfInItemDescBox;

BOOLEAN InItemDescriptionBox( );
BOOLEAN InitItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex );
BOOLEAN InternalInitItemDescriptionBox( OBJECTTYPE *pObject, INT16 sX, INT16 sY, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier, UINT8 ubPosition = NUM_INV_SLOTS );
BOOLEAN InitKeyItemDescriptionBox( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sX, INT16 sY, UINT8 ubStatusIndex );
void RenderItemDescriptionBox( );
void HandleItemDescriptionBox( BOOLEAN *pfDirty );
void DeleteItemDescriptionBox( );


BOOLEAN InItemStackPopup( );
BOOLEAN InitItemStackPopup( SOLDIERTYPE *pSoldier, UINT8 ubPosition, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight );
void RenderItemStackPopup( BOOLEAN fFullRender );
void HandleItemStackPopup( );
void DeleteItemStackPopup( );
void EndItemStackPopupWithItemInHand( );


// keyring handlers
BOOLEAN InitKeyRingPopup( SOLDIERTYPE *pSoldier, INT16 sInvX, INT16 sInvY, INT16 sInvWidth, INT16 sInvHeight );
void RenderKeyRingPopup( BOOLEAN fFullRender );
void InitKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback );
void InitMapKeyRingInterface( MOUSE_CALLBACK KeyRingClickCallback );
void DeleteKeyRingPopup( );


void ShutdownKeyRingInterface( void );
BOOLEAN InKeyRingPopup( void );
void BeginKeyRingItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubKeyRingPosition );


extern OBJECTTYPE		*gpItemPointer;
extern OBJECTTYPE		gItemPointer;
extern SOLDIERTYPE		*gpItemPointerSoldier;
extern UINT16				usItemSnapCursor;
extern UINT16				us16BPPItemCyclePlacedItemColors[ 20 ];
extern BOOLEAN				gfItemPointerDifferentThanDefault;


void BeginItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubHandPos );
void InternalBeginItemPointer( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject, INT8 bHandPos );
void EndItemPointer( );
void DrawItemFreeCursor( );
void DrawItemTileCursor( );
void HideItemTileCursor( );
void InitItemInterface( );
BOOLEAN ItemCursorInLobRange( INT16 sMapPos );
BOOLEAN	HandleItemPointerClick( INT16 sMapPos );
UINT32 GetInterfaceGraphicForItem( INVTYPE *pItem );
UINT16 GetTileGraphicForItem( INVTYPE *pItem );
BOOLEAN LoadTileGraphicForItem( INVTYPE *pItem, UINT32 *puiVo );

void GetHelpTextForItem( STR16 pzStr, OBJECTTYPE *pObject, SOLDIERTYPE *pSoldier, int subObject = -1 );

void CancelItemPointer( );

BOOLEAN LoadItemCursorFromSavedGame( HWFILE hFile );
BOOLEAN SaveItemCursorToSavedGame( HWFILE hFile );

void EnableKeyRing( BOOLEAN fEnable );

// handle compatable items for merc and map inventory
BOOLEAN HandleCompatibleAmmoUIForMapScreen( SOLDIERTYPE *pSoldier, INT32 bInvPos, BOOLEAN fOn, BOOLEAN fFromMerc );
BOOLEAN HandleCompatibleAmmoUIForMapInventory( SOLDIERTYPE *pSoldier, INT32 bInvPos, INT32 iStartSlotNumber, BOOLEAN fOn, BOOLEAN fFromMerc	);
void ResetCompatibleItemArray( );

void CycleItemDescriptionItem( );

BOOLEAN InitializeStealItemPickupMenu( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent, ITEM_POOL *pItemPool, UINT8 ubCount);

#endif
