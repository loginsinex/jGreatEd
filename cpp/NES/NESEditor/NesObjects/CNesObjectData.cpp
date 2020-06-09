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

#include "h\NES\NESEditor\NESEditor.h"

const NES_EPOINTERS g_eTile[] = 
{
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
//	eTileListMushroomLedgeTop,		// UNUSED for J version
//	eTileListMushroomLedgeBottom,	// UNUSED for J version
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
	eTileListStoneBlock
};

CNesObjectData::CNesObjectData( CNESFile & file, const CNesPointers & eptr, NES_AREA_SET areaSet )
	: m_file( file ), m_eptr( eptr ), m_areaSet( areaSet )
{

}

VOID CNesObjectData::LoadData( )
{
	LoadGroundData();
	LoadQBlockMetatiles();
	LoadBricksMetatiles();
	LoadSolid();
	LoadPipeMetatiles();
	LoadPulleyRopeMetatiles();
	LoadSpecObjData();
	LoadCastleMetatiles();
	LoadStaircaseData();
	LoadBgColors();
	LoadEnemyGFX();
	LoadItemTiles();
	LoadUpsidePipeHeights();
	LoadJumpspringData();
	LoadWarpWorldData();
	LoadSpriteAttributes();
	LoadRedPirahnaData();
}

VOID CNesObjectData::DumpData()
{
	DumpBgColors();
	DumpItemTiles();
}

VOID CNesObjectData::LoadGroundData()
{
	const CNesPointers & eptr = m_eptr;

	CopyMemory( m_grData.bBgSceneOffset, m_file.Data<BYTE[ 3 ]>( eptr.Pointer( eBgSceneDataOffset ) + 1 ), _ARRAYSIZE( m_grData.bBgSceneOffset ) );
	CopyMemory( m_grData.bBgSceneData, m_file.Data<BYTE[ 144 ]>( eptr.Pointer( eBgSceneData ) ), _ARRAYSIZE( m_grData.bBgSceneData ) );
	CopyMemory( m_grData.bBgSceneMTiles, m_file.Data<NES_METATILE[ 36 ]>( eptr.Pointer( eBgSceneMTiles ) ), _ARRAYSIZE( m_grData.bBgSceneMTiles ) );
	CopyMemory( m_grData.bFgSceneOffset, m_file.Data<BYTE[ 3 ]>( eptr.Pointer( eFgSceneDataOffset ) + 1 ), _ARRAYSIZE( m_grData.bFgSceneOffset ) );
	CopyMemory( m_grData.bFgSceneData, m_file.Data<NES_METATILE[ 39 ]>( eptr.Pointer( eFgSceneData ) ), _ARRAYSIZE( m_grData.bFgSceneData ) );
	CopyMemory( m_grData.bGroundMTiles, m_file.Data<NES_METATILE[ 4 ]>( eptr.Pointer( eGroundMTiles ) ), _ARRAYSIZE( m_grData.bGroundMTiles ) );
	CopyMemory( m_grData.bGroundBits, m_file.Data<WORD[ 16 ]>( eptr.Pointer( eGroundBits ) ), _ARRAYSIZE( m_grData.bGroundBits ) * sizeof( WORD ) );
	CopyMemory( m_grData.bBlockBounds, m_file.Data<NES_METATILE[ 4 ]>( eptr.Pointer( eBlockBounds ) ), 4 );

	m_grData.bSpecWaterBlockForWorld = m_file.Data<BYTE>( eptr[ eBlockSpecialWaterWorldId ].ptr );
	m_grData.bSpecWaterBlock = m_file.Data<NES_METATILE>( eptr[ eBlockSpecialWaterWorld8 ].ptr );
	m_grData.bBlockCloud = m_file.Data<NES_METATILE>( eptr[ eBlockCloud ].ptr );
	m_grData.bBlockDefault = m_file.Data<NES_METATILE>( eptr[ eBlockDefault ].ptr );
}

VOID CNesObjectData::LoadBricksMetatiles()
{
	m_brTiles = m_file.Data<NES_BRICKS_METATILES>( m_eptr.Pointer( eBricksMetatiles ) );
}

VOID CNesObjectData::LoadQBlockMetatiles()
{
	m_qbTiles = m_file.Data<NES_QBLOCK_METATILES>( m_eptr.Pointer( eBrickQMetatiles ) );
}

VOID CNesObjectData::LoadSolid()
{
	m_brSolid = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eSolidMetatiles ) );
	m_brCoins = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eCoinsMetatiles ) );
	m_holeTiles = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eHoleMetatiles ) );
}

VOID CNesObjectData::LoadPulleyRopeMetatiles()
{
	m_plRopeTiles = m_file.Data<NES_PULLEY_ROPE_METATILES>( m_eptr.Pointer( ePulleyRopeMetatiles ) );
}

VOID CNesObjectData::LoadPipeMetatiles()
{
	m_pipeTiles = m_file.Data<NES_PIPE_METATILES>( m_eptr.Pointer( eVertPipeMetatiles ) );
	LoadLPipeMetatiles();
}

VOID CNesObjectData::LoadLPipeMetatiles()
{
	m_lpipeMetatiles.SideShaft = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eLPipeShaftMetatiles ) );
	m_lpipeMetatiles.SideBottom = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eLPipeBottomMetatiles ) );
	m_lpipeMetatiles.SideTop = m_file.Data<NES_SOLID_METATILES>( m_eptr.Pointer( eLPipeTopMetatiles ) );
}

VOID CNesObjectData::LoadSpecObjData()
{
	CopyMemory( m_specObj.Y, m_file.Data<BYTE[ 3 ]>( m_eptr.Pointer( eSpecObjY ) + 2 ), _ARRAYSIZE( m_specObj.Y ) );
	CopyMemory( m_specObj.tile, m_file.Data<NES_METATILE[ 3 ]>( m_eptr.Pointer( eSpecObjMetatiles ) + 2 ), _ARRAYSIZE( m_specObj.tile ) );
}

VOID CNesObjectData::LoadCastleMetatiles()
{
	m_castleMetatiles = m_file.Data<NES_CASTLE_METATILES>( m_eptr.Pointer( eCastleMetatiles ) );
}

VOID CNesObjectData::LoadStaircaseData()
{
	CopyMemory( m_staircaseData.Y, m_file.Data<BYTE[ 9 ]>( m_eptr.Pointer( eStaircaseY ) ), _ARRAYSIZE( m_staircaseData.Y ) );
	CopyMemory( m_staircaseData.height, m_file.Data<BYTE[ 9 ]>( m_eptr.Pointer( eStaircaseHeight ) ), _ARRAYSIZE( m_staircaseData.height ) );
}

VOID CNesObjectData::LoadBgColors()
{
	m_bgColors = m_file.Data<NES_BG_COLORS>( m_eptr.Pointer( eBgColors ) );
}

VOID CNesObjectData::DumpBgColors()
{
	m_file.Data<NES_BG_COLORS>( m_eptr.Pointer( eBgColors ) ) = m_bgColors;
}

VOID CNesObjectData::LoadEnemyGFX()
{
	m_gfxEnemy = m_file.Data<NES_GFX_OFFSETS>( m_eptr.Pointer( eGFXEnemyOffsets ) );
}

VOID CNesObjectData::LoadItemTiles()
{
	BYTE bFileId = 0xFF;

	for ( auto e : g_eTile )
	{
		const NES_EPOINTER & ptr = m_eptr[ e ];
		if ( ptr.bFileId != bFileId && ptr.bFileId != 0xFF )
		{
			m_file.SelectFile( ptr.bFileId );
			bFileId = ptr.bFileId;
		}

		m_mTile[ e ].bMetatile = m_file.Data<BYTE>( m_eptr[ e ].ptr );
	}
}

VOID CNesObjectData::DumpItemTiles()
{
	BYTE bFileId = 0xFF;

	for ( auto e : g_eTile )
	{
		const NES_EPOINTER & ptr = m_eptr[ e ];
		if ( ptr.bFileId != bFileId && ptr.bFileId != 0xFF )
		{
			m_file.SelectFile( ptr.bFileId );
			bFileId = ptr.bFileId;
		}

		m_file.Data<BYTE>( m_eptr[ e ].ptr ) = m_mTile[ e ].bMetatile;
	}
}

VOID CNesObjectData::LoadUpsidePipeHeights()
{
	if ( asWorld1to4 == m_areaSet )
	{
		m_uspHeight.bHeight[ 0 ] = m_uspHeight.bHeight[ 1 ] = 0xFF;
	}
	else
	{
		m_uspHeight.bHeight[ 0 ] = m_file.Data<BYTE>( m_eptr[ eUpSidePipeHeightHi ].ptr );
		m_uspHeight.bHeight[ 1 ] = m_file.Data<BYTE>( m_eptr[ eUpSidePipeHeightLo ].ptr );
	}
}

VOID CNesObjectData::LoadJumpspringData()
{
	m_gjsData.bAttribute = m_file.Data<BYTE>( m_eptr[ eJumpspringAttr ].ptr );
	m_gjsData.bWorld[ 0 ] = m_file.Data<BYTE>( m_eptr[ eGJumpspringW0 ].ptr );
	m_gjsData.bWorld[ 1 ] = m_file.Data<BYTE>( m_eptr[ eGJumpspringW1 ].ptr );
	m_gjsData.bWorld[ 2 ] = m_file.Data<BYTE>( m_eptr[ eGJumpspringW2 ].ptr );
}

VOID CNesObjectData::LoadWarpWorldData()
{
	m_warpWorldData.bWorldData[ 0 ] = m_file.Data<BYTE>( m_eptr[ eWarpControlWorld3 ].ptr );
	m_warpWorldData.bWorldData[ 1 ] = m_file.Data<BYTE>( m_eptr[ eWarpControlWorld5 ].ptr );
	m_warpWorldData.bWorld5AreaId = m_file.Data<BYTE>( m_eptr[ eWarpControlCode3 ].ptr );
}

VOID CNesObjectData::LoadSpriteAttributes()
{
	m_sprAttr = m_file.Data<NES_SPRITE_ATTRIBUTE>( m_eptr.Pointer( eSpriteAttributes ) );
}

VOID CNesObjectData::LoadRedPirahnaData()
{
	m_redPirahna.bWorld = m_file.Data<BYTE>( m_eptr[ eRedPirahnaWorld ].ptr );
	m_redPirahna.bAttr = m_file.Data<BYTE>( m_eptr[ eRedPirahnaAttr ].ptr );
}

const NES_QBLOCK_METATILES & CNesObjectData::QBlockMetatiles() const
{
	return m_qbTiles;
}

const NES_GROUND_DATA & CNesObjectData::GroundData() const
{
	return m_grData;
}

const NES_BRICKS_METATILES & CNesObjectData::BricksMetatiles() const
{
	return m_brTiles;
}

const NES_SOLID_METATILES & CNesObjectData::SolidMetatiles() const
{
	return m_brSolid;
}

const NES_SOLID_METATILES & CNesObjectData::CoinsMetatiles() const
{
	return m_brCoins;
}

const NES_SOLID_METATILES & CNesObjectData::HoleMetatiles() const
{
	return m_holeTiles;
}

const NES_PIPE_METATILES & CNesObjectData::PipeMetatiles() const
{
	return m_pipeTiles;
}

const NES_PULLEY_ROPE_METATILES & CNesObjectData::PulleyRopeMetatiles() const
{
	return m_plRopeTiles;
}

const NES_LPIPE_METATILES & CNesObjectData::LPipeMetatiles() const
{
	return m_lpipeMetatiles;
}

const NES_SPEC_OBJ & CNesObjectData::SpecObjData() const
{
	return m_specObj;
}

const NES_CASTLE_METATILES & CNesObjectData::CastleMetatiles() const
{
	return m_castleMetatiles;
}

const NES_STAIRCASE_DATA & CNesObjectData::StaircaseData() const
{
	return m_staircaseData;
}

const NES_BG_COLORS & CNesObjectData::BgColors() const
{
	return m_bgColors;
}

VOID CNesObjectData::SetBgColors( const NES_BG_COLORS & nbg )
{
	m_bgColors = nbg;
}

const NES_GFX_OFFSETS & CNesObjectData::GFXEnemy() const
{
	return m_gfxEnemy;
}

const NES_METATILE & CNesObjectData::Tile( NES_EPOINTERS tileId ) const
{
	const auto & tile = m_mTile.find( tileId );
	if ( tile != m_mTile.end() )
	{
		return tile->second;
	}

	return { 0 };
}

const NES_UPSIDE_PIPE_HEIGHT & CNesObjectData::UpsidePipeHeight() const
{
	return m_uspHeight;
}

const NES_GREEN_JUMPSPRING_WORLDS & CNesObjectData::GreenJumpspringData() const
{
	return m_gjsData;
}

const NES_WARP_WORLD_DATA & CNesObjectData::WarpWorldData() const
{
	return m_warpWorldData;
}

const NES_SPRITE_ATTRIBUTE & CNesObjectData::SpriteAttributes() const
{
	return m_sprAttr;
}

const NES_RED_PIRAHNA_DATA & CNesObjectData::RedPirahnaData() const
{
	return m_redPirahna;
}