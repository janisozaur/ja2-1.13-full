#include "Tactical All.h"

struct MOUSETT 
{	
	CHAR16 FastHelpText[ 1024 ];
	INT32 iX;
	INT32 iY;
	INT32 iW;
};

extern struct MOUSETT mouseTT;
extern BOOL mouseTTrender,mouseTTdone;

const int	DL_Limited		= 1;
const int	DL_Basic		= 2;
const int	DL_Full			= 3;
const int	DL_Debug		= 4;

void DisplayWeaponInfo( SOLDIERTYPE*, CHAR16*, UINT8, UINT8 );
void DrawMouseTooltip(void);

#define MAX(a, b) (a > b ? a : b)

void SoldierTooltip( SOLDIERTYPE* pSoldier )
{			
	SGPRect		aRect;
	extern void GetSoldierScreenRect(SOLDIERTYPE*,SGPRect*);
	GetSoldierScreenRect( pSoldier,  &aRect );
	INT16		a1,a2;
	BOOLEAN		fDrawTooltip = FALSE;

	if ( gfKeyState[ALT] && pSoldier &&
		IsPointInScreenRectWithRelative( gusMouseXPos, gusMouseYPos, &aRect, &a1, &a2 ) )
	{
		MOUSETT		*pRegion = &mouseTT;
		CHAR16		pStrInfo[ sizeof( pRegion->FastHelpText ) ];
		int			iNVG = 0;
		INT16		sSoldierGridNo;
		BOOLEAN		fDisplayBigSlotItem	= FALSE;
		BOOLEAN		fMercIsUsingScope	= FALSE;
		UINT16		iCarriedRL = 0;
		INT32		iRangeToTarget = 0;
		UINT8		ubTooltipDetailLevel = gGameExternalOptions.ubSoldierTooltipDetailLevel;
		UINT32		uiMaxTooltipDistance = gGameExternalOptions.ubStraightSightRange;

		fDrawTooltip = TRUE;

		// get the gridno the cursor is at
		GetMouseMapPos( &sSoldierGridNo );

		// get the distance to enemy's tile from the selected merc
		if ( gusSelectedSoldier != NOBODY )
		{		
			iRangeToTarget = GetRangeInCellCoordsFromGridNoDiff( MercPtrs[ gusSelectedSoldier ]->sGridNo, sSoldierGridNo ) / 10;
		}
		// WANNE: If we want to show the tooltip of milita and no merc is present in the sector
		else
		{
			return;
		}

		if ( gGameExternalOptions.fEnableDynamicSoldierTooltips )
		{			
			for ( INT32 cnt = 0; cnt < MAX_ATTACHMENTS; cnt++ )
			{
				if ( Item[MercPtrs[gusSelectedSoldier]->inv[HANDPOS].usAttachItem[cnt]].visionrangebonus > 0 )
				{
					fMercIsUsingScope = TRUE;
					break;
				}
			}

			if ( fMercIsUsingScope )
			{
				// set detail level to (at least) Full
				ubTooltipDetailLevel = MAX(DL_Full,ubTooltipDetailLevel);
			}
			else
			{
				// add 10% to max tooltip viewing distance per level of the merc
				uiMaxTooltipDistance *= 1 + (MercPtrs[ gusSelectedSoldier ]->bExpLevel / 10);

				if ( gGameExternalOptions.gfAllowLimitedVision )
					uiMaxTooltipDistance *= 1 - (gGameExternalOptions.ubVisDistDecreasePerRainIntensity / 100);

				if ( !(Item[MercPtrs[gusSelectedSoldier]->inv[HEAD1POS].usItem].nightvisionrangebonus > 0) &&
					 !(Item[MercPtrs[gusSelectedSoldier]->inv[HEAD2POS].usItem].nightvisionrangebonus > 0) &&
 					 !DayTime() )
				{
					// if night reduce max tooltip viewing distance by a factor of 4 if merc is not wearing NVG
					uiMaxTooltipDistance >>= 2;
				}

				if ( iRangeToTarget <= (INT32)(uiMaxTooltipDistance / 2) )
				{
					// at under half the maximum view distance set tooltip detail to (at least) Basic
					ubTooltipDetailLevel = MAX(DL_Basic,ubTooltipDetailLevel);
				}
				else if ( iRangeToTarget <= (INT32)uiMaxTooltipDistance )
				{
					// at under the maximum view distance set tooltip detail to (at least) Limited
					ubTooltipDetailLevel = MAX(DL_Limited,ubTooltipDetailLevel);
				}
				else
				{
					// beyond visual range, do not display tooltip if player has not chosen full or debug details
					if ( ubTooltipDetailLevel < DL_Full )
						return;
				}
			} // fMercIsUsingScope is false
		} // gGameExternalOptions.fEnableDynamicSoldierTooltips


		// WANNE: Check if enemy soldier is in line of sight but only if player has not choosen debug details
		if ( ubTooltipDetailLevel < DL_Full)
		{
			// Get the current selected merc
			SOLDIERTYPE* pMerc = MercPtrs[ gusSelectedSoldier ];
			
			if ( pMerc->bOppList[pSoldier->ubID] != SEEN_CURRENTLY )
			{
				// We do not see the enemy. Return and do not display the tooltip.
				return;
			}
		}

		swprintf( pStrInfo, L"" );
		if ( ubTooltipDetailLevel == DL_Debug )
		{
			// display "debug" info
			if ( gGameExternalOptions.fEnableSoldierTooltipLocation )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_LOCATION], pStrInfo, sSoldierGridNo );
			if ( gGameExternalOptions.fEnableSoldierTooltipBrightness )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_BRIGHTNESS], pStrInfo, SHADE_MIN - LightTrueLevel( sSoldierGridNo, gsInterfaceLevel ), SHADE_MIN );
			if ( gGameExternalOptions.fEnableSoldierTooltipRangeToTarget )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_RANGE_TO_TARGET], pStrInfo, iRangeToTarget );
			if ( gGameExternalOptions.fEnableSoldierTooltipID )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ID], pStrInfo, pSoldier->ubID );
			if ( gGameExternalOptions.fEnableSoldierTooltipOrders )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ORDERS], pStrInfo, pSoldier->bOrders );
			if ( gGameExternalOptions.fEnableSoldierTooltipAttitude )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ATTITUDE], pStrInfo, pSoldier->bAttitude );
			if ( gGameExternalOptions.fEnableSoldierTooltipActionPoints )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_CURRENT_APS], pStrInfo, pSoldier->bActionPoints );
			if ( gGameExternalOptions.fEnableSoldierTooltipHealth )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_CURRENT_HEALTH], pStrInfo, pSoldier->bLife );
		}

		// armor info code block start
		if ( ubTooltipDetailLevel >= DL_Full )
		{
			if ( gGameExternalOptions.fEnableSoldierTooltipHelmet )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_HELMET], pStrInfo, pSoldier->inv[HELMETPOS].usItem ? ItemNames[ pSoldier->inv[HELMETPOS].usItem ] : gzTooltipStrings[STR_TT_NO_HELMET] );
			if ( gGameExternalOptions.fEnableSoldierTooltipVest )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_VEST], pStrInfo, pSoldier->inv[VESTPOS].usItem ? ItemNames[ pSoldier->inv[VESTPOS].usItem ] : gzTooltipStrings[STR_TT_NO_VEST] );
			if ( gGameExternalOptions.fEnableSoldierTooltipLeggings )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_LEGGINGS], pStrInfo, pSoldier->inv[LEGPOS].usItem ? ItemNames[ pSoldier->inv[LEGPOS].usItem ] : gzTooltipStrings[STR_TT_NO_LEGGING] );
		}
		else
		{
			if ( !(	!gGameExternalOptions.fEnableSoldierTooltipHelmet &&
					!gGameExternalOptions.fEnableSoldierTooltipVest &&
					!gGameExternalOptions.fEnableSoldierTooltipLeggings) )
				// do not display the armor line if all the armor toggles are set to false
			{
				if ( ArmourPercent( pSoldier ) )
				{
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ARMOR] );
					if ( ubTooltipDetailLevel == DL_Basic )
					{
						if ( gGameExternalOptions.fEnableSoldierTooltipHelmet )
							swprintf( pStrInfo, L"%s%s", pStrInfo, pSoldier->inv[HELMETPOS].usItem ? gzTooltipStrings[STR_TT_HELMET] : L"" );
						if ( gGameExternalOptions.fEnableSoldierTooltipVest )
							swprintf( pStrInfo, L"%s%s", pStrInfo, pSoldier->inv[VESTPOS].usItem ? gzTooltipStrings[STR_TT_VEST] : L"" );
						if ( gGameExternalOptions.fEnableSoldierTooltipLeggings )
							swprintf( pStrInfo, L"%s%s", pStrInfo, pSoldier->inv[LEGPOS].usItem ? gzTooltipStrings[STR_TT_LEGGINGS] : L"" );
						wcscat( pStrInfo, L"\n" );
					}
					else // ubTooltipDetailLevel == DL_Limited
					{
						swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ARMOR_2], gzTooltipStrings[STR_TT_WORN] );
					}
				}
				else
				{
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_ARMOR_2], gzTooltipStrings[STR_TT_NO_ARMOR] );
				}
			}
		}
		// armor info code block end

		// head slots info code block start
		if ( ubTooltipDetailLevel != DL_Debug )
		{

			if( Item[pSoldier->inv[HEAD1POS].usItem].nightvisionrangebonus > 0 )
				iNVG = HEAD1POS;
			else if( Item[pSoldier->inv[HEAD2POS].usItem].nightvisionrangebonus > 0 )
				iNVG = HEAD2POS;

			if ( !(	!gGameExternalOptions.fEnableSoldierTooltipHeadItem1 &&
					!gGameExternalOptions.fEnableSoldierTooltipHeadItem2) )
				// do not display the NVG/mask lines if both head slot toggles are set to false
			{
				if ( ubTooltipDetailLevel >= DL_Full )
				{
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_NVG], pStrInfo,
						iNVG ? ItemNames[ pSoldier->inv[ iNVG ].usItem ] : gzTooltipStrings[STR_TT_NO_NVG] );
				}
				else
				{
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_NVG], pStrInfo, iNVG ? gzTooltipStrings[STR_TT_WORN] : gzTooltipStrings[STR_TT_NO_NVG] );
				}
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_GAS_MASK], pStrInfo,
					( FindGasMask(pSoldier) != NO_SLOT ) ? gzTooltipStrings[STR_TT_WORN] : gzTooltipStrings[STR_TT_NO_MASK] );
			}
		}
		else // gGameExternalOptions.ubSoldierTooltipDetailLevel == DL_Debug
		{
			if ( gGameExternalOptions.fEnableSoldierTooltipHeadItem1 )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_HEAD_POS_1], pStrInfo, ItemNames[ pSoldier->inv[HEAD1POS].usItem ] );
			if ( gGameExternalOptions.fEnableSoldierTooltipHeadItem2 )
				swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_HEAD_POS_2], pStrInfo, ItemNames[ pSoldier->inv[HEAD2POS].usItem ] );
		}
		// head slots info code block end

		// weapon in primary hand info code block start
		if ( gGameExternalOptions.fEnableSoldierTooltipWeapon )
		{
			DisplayWeaponInfo( pSoldier, pStrInfo, HANDPOS, ubTooltipDetailLevel );
		} // gGameExternalOptions.fEnableSoldierTooltipWeapon
		// weapon in primary hand info code block end

		// weapon in off hand info code block start
		if ( gGameExternalOptions.fEnableSoldierTooltipSecondHand )
		{
			if ( pSoldier->inv[SECONDHANDPOS].usItem )
			{
				// if there's something in the slot display it
				wcscat( pStrInfo, L"\n" );
				DisplayWeaponInfo( pSoldier, pStrInfo, SECONDHANDPOS, ubTooltipDetailLevel );
			}
		}
		// weapon in off hand info code block end

		// large objects in big inventory slots info code block start
		for ( UINT8 BigSlot = BIGPOCK1POS; BigSlot <= BIGPOCK4POS; BigSlot++ )
		{
			if ( pSoldier->inv[ BigSlot ].usItem == 0 )
				continue; // slot is empty, move on to the next slot

			switch( BigSlot )
			{
				// if display of this big slot is toggled off then move on to the next slot
				case BIGPOCK1POS:
					if ( !gGameExternalOptions.fEnableSoldierTooltipBigSlot1 )
						continue;
					break;
				case BIGPOCK2POS:
					if ( !gGameExternalOptions.fEnableSoldierTooltipBigSlot2 )
						continue;
					break;
				case BIGPOCK3POS:
					if ( !gGameExternalOptions.fEnableSoldierTooltipBigSlot3 )
						continue;
				    break;
				case BIGPOCK4POS:
					if ( !gGameExternalOptions.fEnableSoldierTooltipBigSlot4 )
						continue;
					break;
			}

			if ( Item[ pSoldier->inv[ BigSlot ].usItem ].rocketlauncher )
				iCarriedRL = pSoldier->inv[ BigSlot ].usItem; // remember that enemy is carrying a rocket launcher when check for rocket ammo is made later on

			if ( (  Item[ pSoldier->inv[ BigSlot ].usItem ].usItemClass == IC_LAUNCHER ) ||
				 (	Item[ pSoldier->inv[ BigSlot ].usItem ].usItemClass == IC_GUN ) )
			{
				// it's a firearm
				if ( (	Weapon[pSoldier->inv[ BigSlot ].usItem].ubWeaponClass != HANDGUNCLASS ) &&
					 (	Weapon[pSoldier->inv[ BigSlot ].usItem].ubWeaponClass != SMGCLASS ) )
				{
					// it's a long gun or heavy weapon
					fDisplayBigSlotItem = TRUE;
				}
			}
			else
			{
				// check for rocket ammo
				if ( ( iCarriedRL != 0 ) &&												// soldier is carrying a RL ...
					 ValidLaunchable( pSoldier->inv[ BigSlot ].usItem, iCarriedRL ) )	// this item is launchable by the RL
				{
					fDisplayBigSlotItem = TRUE;
				}
			}
			if ( fDisplayBigSlotItem )
			{
				wcscat( pStrInfo, gzTooltipStrings[STR_TT_IN_BACKPACK] );
				DisplayWeaponInfo( pSoldier, pStrInfo, BigSlot, ubTooltipDetailLevel );
				fDisplayBigSlotItem = FALSE;
			}
		}
		// large objects in big inventory slots info code block end

		pRegion->iX = gusMouseXPos;
		pRegion->iY = gusMouseYPos;				

//		if ( gGameExternalOptions.ubSoldierTooltipDetailLevel == DL_Debug )
//			swprintf( pRegion->FastHelpText, L"%s\n|String |Length|: %d", pStrInfo, wcslen(pStrInfo) );
//		else
			wcscpy( pRegion->FastHelpText, pStrInfo );
	}

	if ( gfKeyState[ ALT ] && fDrawTooltip )
	{
		DrawMouseTooltip();
		SetRenderFlags( RENDER_FLAG_FULL );
	}
} // SoldierTooltip(SOLDIERTYPE* pSoldier)


void DisplayWeaponInfo( SOLDIERTYPE* pSoldier, CHAR16* pStrInfo, UINT8 ubSlot, UINT8 ubTooltipDetailLevel )
{
	INT32		iNumAttachments		= 0;
	BOOLEAN		fDisplayAttachment	= FALSE;

	if ( ubTooltipDetailLevel >= DL_Full )
	{
		// display exact weapon model
		swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo,
			WeaponInHand( pSoldier ) ? ItemNames[ pSoldier->inv[ubSlot].usItem ] : gzTooltipStrings[STR_TT_NO_WEAPON] );
	}
	else
	{
		if ( ubTooltipDetailLevel == DL_Limited )
		{
			// display general weapon class
			switch( Weapon[pSoldier->inv[ubSlot].usItem].ubWeaponClass )
			{
				case HANDGUNCLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_HANDGUN] );
					break;
				case SMGCLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_SMG] );
					break;
				case RIFLECLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_RIFLE] );
					break;
				case MGCLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_MG] );
					break;
				case SHOTGUNCLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_SHOTGUN] );
					break;
				case KNIFECLASS:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_KNIFE] );
					break;
				default:
					swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo, gzTooltipStrings[STR_TT_HEAVY_WEAPON] );
					break;
			}
		}
		else
		{
			// display general weapon type
			swprintf( pStrInfo, gzTooltipStrings[STR_TT_CAT_WEAPON], pStrInfo,
				WeaponInHand( pSoldier) ? WeaponType[Weapon[pSoldier->inv[ubSlot].usItem].ubWeaponType] : gzTooltipStrings[STR_TT_NO_WEAPON] );
		}
	}

	if ( gGameExternalOptions.ubSoldierTooltipDetailLevel >= DL_Basic )
	{
		// display weapon attachments
		for ( INT32 cnt = 0; cnt < MAX_ATTACHMENTS; cnt++ )
		{
			if ( pSoldier->inv[ubSlot].usAttachItem[ cnt ] != NOTHING )
			{	
				if ( ubTooltipDetailLevel == DL_Basic )
				{
					// display only externally-visible weapon attachments
					if ( !Item[pSoldier->inv[ubSlot].usAttachItem[ cnt ]].hiddenattachment )
						{
							fDisplayAttachment = TRUE;
					}
				}
				else
				{
					// display all weapon attachments
					fDisplayAttachment = TRUE;
				}
				if ( fDisplayAttachment )
				{
					iNumAttachments++;
					if ( iNumAttachments == 1 )
						wcscat( pStrInfo, L"\n[" );
					else
						wcscat( pStrInfo, L", " );
					wcscat( pStrInfo, ItemNames[ pSoldier->inv[ubSlot].usAttachItem[ cnt ] ] );
					fDisplayAttachment = FALSE; // clear flag for next loop iteration
				}
			} // pSoldier->inv[HANDPOS].usAttachItem[ cnt ] != NOTHING
		} // for
		if ( iNumAttachments > 0 )
			wcscat( pStrInfo, pMessageStrings[ MSG_END_ATTACHMENT_LIST ] ); // append ' attached]' to end of string
	} // gGameExternalOptions.ubSoldierTooltipDetailLevel >= DL_Basic
}

MOUSETT mouseTT;
BOOL mouseTTrender, mouseTTdone;

void DrawMouseTooltip()
{
	UINT8 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	UINT16 usFillColor =	Get16BPPColor(FROMRGB(250, 240, 188));
	UINT16 usRectColor1 =	Get16BPPColor( FROMRGB( 65, 57, 15 ) );
	UINT16 usRectColor2 =	Get16BPPColor( FROMRGB( 227, 198, 88 ) );
	static INT32 iX, iY, iW, iH;
	
	extern INT16 GetWidthOfString(STR16);
	extern INT16 GetNumberOfLinesInHeight(STR16);
	extern void DisplayHelpTokenizedString(STR16,INT16,INT16);
	extern BOOLEAN initTooltipBuffer();
	extern PTR LockTooltipBuffer(UINT32*);
	extern void UnlockTooltipBuffer();
	extern void DisplayTooltipString( STR16 pStringA, INT16 sX, INT16 sY );
	extern void j_log(PTR,...);

	iX = mouseTT.iX;iY = mouseTT.iY;
	iW = (INT32)GetWidthOfString( mouseTT.FastHelpText ) + 10;
	iH = (INT32)( GetNumberOfLinesInHeight( mouseTT.FastHelpText ) * (GetFontHeight(FONT10ARIAL)+1) + 8 );

	if(1)//draw at cursor
	{
		iY -= (iH / 2);
		if (gusMouseXPos > (SCREEN_WIDTH / 2))
			iX = gusMouseXPos - iW - 24;
		else 
			iX = gusMouseXPos + 24;
		//if (gusMouseYPos > (SCREEN_HEIGHT / 2))
		//	iY -= 32;

		if (iY <= 0) iY += 32;
	}
	else
	{	//draw in panel
		//502,485 658,596   160*110 580,540
		iX = 580 - (iW / 2);
		iY = 540 - (iH/2);
		if (iY + iH > SCREEN_HEIGHT)  iY = SCREEN_HEIGHT - iH - 3 ;
	}

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	RectangleDraw( TRUE, iX + 1, iY + 1, iX + iW - 1, iY + iH - 1, Get16BPPColor( FROMRGB( 65, 57, 15 ) ), pDestBuf );
	RectangleDraw( TRUE, iX, iY, iX + iW - 2, iY + iH - 2, Get16BPPColor( FROMRGB( 227, 198, 88 ) ), pDestBuf );
	UnLockVideoSurface( FRAME_BUFFER );
	ShadowVideoSurfaceRect( FRAME_BUFFER, iX + 2, iY + 2, iX + iW - 3, iY + iH - 3 );
	ShadowVideoSurfaceRect( FRAME_BUFFER, iX + 2, iY + 2, iX + iW - 3, iY + iH - 3 );

	SetFont( FONT10ARIAL );
	SetFontShadow( FONT_NEARBLACK );
	DisplayHelpTokenizedString( mouseTT.FastHelpText ,( INT16 )( iX + 5 ), ( INT16 )( iY + 5 ) );
	InvalidateRegion(  iX, iY, (iX + iW) , (iY + iH) );

	//InvalidateScreen();
}