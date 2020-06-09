#pragma once

/*
	A FDA Super Mario Bros 2 [J] [public version]
	Copyright (C) 2020 ALXR aka loginsin
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "h/NES/NESMap/CNESFile.h"
#include "h/NES/NESEditor/Pointers/J_Pointers.h"

typedef union _tagNES_EPOINTER
{
	struct
	{
		USHORT		ptr;
		BYTE		bFileId;
		BYTE		bUnused;
	};
	DWORD			eptr;
} NES_EPOINTER, *PNES_EPOINTER;

typedef enum _tagNES_VERSION : TCHAR
{
	ver_J = _T( 'J' ),
	ver_Unknown = _T( '0' )
} NES_VERSION, *PNES_VERSION;

typedef enum _tagNES_EPOINTERS
{
	eFilesListPtrLo,
	eFilesListPtrHi,
	eEnemyPtr,
	eEnemyPtrHi,
	eLevelPtr,
	eLevelPtrHi,
	eEnemyAddrHOffset,
	eLevelAddrHOffset,
	eWorldAreasOffsets,
	eWorldAreas,

	eTileListBricksRowTileId,
	eTileListBridgeTile1,
	eTileListBridgeTile2,
	eTileListBridgeHeight0,
	eTileListBridgeHeight1,
	eTileListBridgeHeight2,
	eTileListWaterTile1,
	eTileListWaterTile2,
	eTileListQBlockRow,
	eTileListQBlockHeight1,
	eTileListQBlockHeight2,
	eTileListRope,
	eTileListResidualBall,
	eTileListStaircase,
	eTileListTreeLedgeLeft,
	eTileListTreeLedgeMiddle,
	eTileListTreeLedgeRight,
	eTileListTreeLedgeBottom,
	eTileListMushroomLedgeLeft,
	eTileListMushroomLedgeRight,
	eTileListMushroomLedgeMiddle,
	eTileListMushroomLedgeTop,
	eTileListMushroomLedgeBottom,
	eTileListBulletBillTop,
	eTileListBulletBillMiddle,
	eTileListBulletBillBottom,
	eTileListWaterPipeTop,
	eTileListWaterPipeBottom,
	eTileListJumpspringTop,
	eTileListJumpspringBottom,
	eTileListFlagpoleTop,
	eTileListFlagpoleRope,
	eTileListFlagpoleBottom,
	eTileListStoneBlock,

	eBlockSpecialWaterWorldId,
	eBlockSpecialWaterWorld8,
	eBlockCloud,
	eBlockDefault,
	eBricksMetatiles,
	eBrickQMetatiles,
	eSolidMetatiles,
	eCoinsMetatiles,
	eHoleMetatiles,
	ePulleyRopeMetatiles,
	eVertPipeMetatiles,
	eLPipeShaftMetatiles,
	eLPipeBottomMetatiles,
	eLPipeTopMetatiles,
	eSpecObjY,
	eSpecObjMetatiles,
	eCastleMetatiles,
	eStaircaseY,
	eStaircaseHeight,
	eBgColors,
	eGFXEnemy,
	eGFXEnemyOffsets,
	eGFXPowerUp,

	eBgSceneDataOffset,
	eBgSceneData,
	eBgSceneMTiles,
	eFgSceneDataOffset,
	eFgSceneData,
	eGroundMTiles,
	eGroundBits,
	eBlockBounds,

	eVRAMAddrTableLo,
	eVRAMAddrTableHi,
	eMetatiesPtrHi,
	eMetatiesPtrLo,
	ePlayerTimerCtrl,
	ePlayerEntranceCtrl,
	ePlayerEntranceX,
	ePlayerSpriteTable,
	ePlayerTiles,
	eUpSidePipeHeightHi,
	eUpSidePipeHeightLo,

	eJumpspringAttr,
	eGJumpspringW0,
	eGJumpspringW1,
	eGJumpspringW2,
	eFindAreaPointerPtr,
	eFL05PaddingPtr,
	eLoopAreaDataOffsetPtr,
	eLoopAreaNumberPtr,
	eLoopPageNumberPtr,
	eLoopYPtr,
	eLoopSlaveDataPtr,
	eLoopLdaWorldCodePtr,
	eLoopCount,

	eWarpControlCode1,	// LDA vAreaAddrsLOffset (aka vAreaId) -> LDA vLevelNumber
	eWarpControlCode2,	// LDA vAreaAddrsLOffset (aka vAreaId) -> LDA vLevelNumber
	eWarpControlCode3,	// CMB #$B -> CMP #$0 (for World 5-1)
	eWarpControlWorld3,	// CMB #World_3
	eWarpControlWorld5,	// CMB #World_5
	eWarpWorlds,
	eCoinsFor1Up,
	eHardModeWorld,
	eHardModeLevel,
	eBowserHammersWorld,
	ePalRotateColors,

	eSpinyEggPatchPlace,
	eSmallBBox,

	eDemoActionArray,
	eDemoTimingArray,

	eBowserIdentities,
	eBowserHits,

	eHPAreaSet0,
	eHPAreaSet1,
	eHPAreaSet2,

	eHalfwayADData,
	eHalfwayADCallPtr,
	ePatchPal,
	eStyleVRAMTables,
	eBlankPal,
	eSpriteAttributes,
	eRedPirahnaWorld,
	eRedPirahnaAttr,
	eEOFPointers
} NES_EPOINTERS, *PNES_EPOINTERS;

class CNesPointers
{
	CNESFile						&			m_file;
	NES_VERSION									m_uVersion;
	std::map<NES_EPOINTERS, NES_EPOINTER>		m_eptr;

	VOID			LoadPointers();
	VOID			LoadPointersJ();

	VOID			LoadSharedPointers();

public:
	CNesPointers( CNESFile & mmc3 );
	VOID			Version( NES_VERSION id );

	NES_EPOINTER	operator[]( NES_EPOINTERS eptr ) const;

	VOID			Select( NES_EPOINTERS eptr ) const;
	USHORT	&		Pointer( NES_EPOINTERS eptr ) const;
};