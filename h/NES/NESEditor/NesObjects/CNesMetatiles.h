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

#define NES_COUNT_METATILES_TABLES			4
#define NES_METATILE_SIZE					16
#define NES_METATILE_COUNT_FOR_GROUP		64
#define NES_METATILE_GROUP_COUNT			NES_COUNT_METATILES_TABLES

#define NES_SPRITE_WIDTH					16
#define NES_SPRITE_HEIGHT					24
#define NES_ENEMY_GFX_COUNT					43
#define NES_PLAYER_GFX_OFFSET				0x20
#define NES_PLAYER_GFX_SPRITES_LO			65
#define NES_PLAYER_GFX_SPRITES_HI			( NES_PLAYER_GFX_SPRITES_LO + NES_PLAYER_GFX_OFFSET )
#define NES_ENEMY_CACHE_SIZE				128

typedef enum _tagNES_POWERUP_GFX
{
	npuMushroom1,
	npuFlower,
	npuStarman,
	npuMushroom2,
	npuMushroom3,
	npuTableSize
} NES_POWERUP_GFX, *PNES_POWERUP_GFX;

#pragma pack(push,1)
typedef struct _tagNES_PLAYER_TILES_TABLE
{
	BYTE		bOffset[ 16 ];
} NES_PLAYER_TABLE, *PNES_PLAYER_TABLE;

typedef struct _tagNES_PATCH_PALETTE
{
	BYTE		bPatchPal[ nltTypeCount ][ 4 ];
} NES_PATCH_PALETTE, *PNES_PATCH_PALETTE;

typedef struct _tagNES_PLAYER_TILES
{
	BYTE		bTiles[ 158 ];
} NES_PLAYER_TILES, *PNES_PLAYER_TILES;

typedef struct _tagNES_POWERUP_GFX_TABLE
{
	BYTE		bTile[ npuTableSize ][ 4 ];
} NES_POWERUP_GFX_TABLE, *PNES_POWERUP_GFX_TABLE;
#pragma pack(pop)


typedef struct _tagNES_PAL_ARRAY
{
	BYTE			bPal[ 2 ][ 16 ];
} NES_PAL_ARRAY, *PNES_PAL_ARRAY;

typedef enum _tagNES_PLAYER_SPRITE_TABLE : BYTE
{
	npstBigJumping = NES_PLAYER_GFX_SPRITES_LO + 0,
	npstBigSwimming = NES_PLAYER_GFX_SPRITES_LO + 1,
	npstBigTurning = NES_PLAYER_GFX_SPRITES_LO + 2,
	npstBigRunning = NES_PLAYER_GFX_SPRITES_LO + 3,
	npstBigClimbing = NES_PLAYER_GFX_SPRITES_LO + 4,
	npstBigSitting = NES_PLAYER_GFX_SPRITES_LO + 5,
	npstBigClimbUp = NES_PLAYER_GFX_SPRITES_LO + 6,
	npstJumping = NES_PLAYER_GFX_SPRITES_LO + 7,
	npstSwimming = NES_PLAYER_GFX_SPRITES_LO + 8,
	npstTurning = NES_PLAYER_GFX_SPRITES_LO + 9,
	npstRunning = NES_PLAYER_GFX_SPRITES_LO + 10
} NES_PLAYER_SPRITE_TABLE, *PNES_PLAYER_SPRITE_TABLE;

typedef enum _tagNES_PAL_TABLE : BYTE
{
	nptWater = 0,
	nptGround = 4,
	nptDaySnowGround = 5,
	nptNightSnowGround = 6,
	nptMushroomGround = 7,
	nptUnderground = 9,
	nptCastle = 13,
} NES_PAL_TABLE, *PNES_PAL_TABLE;

typedef enum _tagNES_UP_SPRITE_TABLE
{
	nustFirebar = NES_ENEMY_GFX_COUNT,
	nustBowserFire1 = NES_ENEMY_GFX_COUNT + 1,
	nustBowserFire2 = NES_ENEMY_GFX_COUNT + 2,
	nustFlagpole = NES_ENEMY_GFX_COUNT + 3,
	nustBridge1 = NES_ENEMY_GFX_COUNT + 4,
	nustBridge2 = NES_ENEMY_GFX_COUNT + 5,
	nustBridge3 = NES_ENEMY_GFX_COUNT + 6,
	nustPowerup = NES_ENEMY_GFX_COUNT + 7,
	nustStarman = NES_ENEMY_GFX_COUNT + 8,
	nustFlower = NES_ENEMY_GFX_COUNT + 9,
	nustCoin = NES_ENEMY_GFX_COUNT + 10,
	nustVine = NES_ENEMY_GFX_COUNT + 11,
	nustFrenzyMark = NES_ENEMY_GFX_COUNT + 12,
	nustDigitMark00 = NES_ENEMY_GFX_COUNT + 13,
	nustDigitMark10 = NES_ENEMY_GFX_COUNT + 14,
	nustDigitMark20 = NES_ENEMY_GFX_COUNT + 15,
	nustDigitMark40 = NES_ENEMY_GFX_COUNT + 16,
	nustDigitMark50 = NES_ENEMY_GFX_COUNT + 17,
	nustDigitMark80 = NES_ENEMY_GFX_COUNT + 18,
	nustWarpMark = NES_ENEMY_GFX_COUNT + 19,
	nustObjFrenzy = NES_ENEMY_GFX_COUNT + 20,
	nustHardMark = NES_ENEMY_GFX_COUNT + 21,
	nustUpsideDownPirahna = NES_ENEMY_GFX_COUNT + 22
} NES_UP_SPRITE_TABLE, *PNES_UP_SPRITE_TABLE;

typedef enum _tagNES_VRAM_TABLE : BYTE
{
	nvtVRAM_Buffer1,
	nvtWaterPaletteData,
	nvtGroundPaletteData,
	nvtUndergroundPaletteData,
	nvtCastlePaletteData,
	nvtTitleScreenGfxData,
	nvtVRAM_Buffer2_0,
	nvtVRAM_Buffer2_1,
	nvtBowserPaletteData,
	nvtDaySnowPaletteData,
	nvtNightSnowPaletteData,
	nvtMushroomPaletteData,
	nvtThankYouMessage,
	nvtMushroomRetainerMsg,
	nvtUnusedAttribData,
	nvtFinalRoomPalette,
	nvtThankYouMessageFinal,
	nvtPeaceIsPavedMsg,
	nvtWithKingdomSavedMsg,
	nvtHurrahMsg,
	nvtOurOnlyHeroMsg,
	nvtThisEndsYourTripMsg,
	nvtOfALongFriendshipMsg,
	nvtPointsAddedMsg,
	nvtForEachPlayerLeftMsg,
	nvtDiskErrorMainMsg,
	nvtDiskScreenPalette,
	nvtPrincessPeachsRoom,
	nvtMenuCursorTemplate,
	nvtpFantasyWorld9Msg,
	nvtpSuperPlayerMsg,
	nvtpVRAMTablesCount,
	nvtpSpecialWater,
	nvtpSpecialGround,
	nvtpSpecialUnderground,
	nvtpSpecialCastle,
	nvtpSpecialStyle0,
	nvtpSpecialStyle1,
	nvtpSpecialStyle2,
	nvtpSpecialStyle3
} NES_VRAM_TABLE, *PNES_VRAM_TABLE;

typedef struct _tagNES_ENEMY_GFX
{
	BYTE					bTiles[ NES_ENEMY_GFX_COUNT ][ 6 ];
} NES_ENEMY_GFX, *PNES_ENEMY_GFX;

typedef struct _tagNES_VRAM_PALETTE
{
	USHORT					uPtr;
	BOOL					fRepeat;
	BOOL					fCustomTable;
	std::vector<BYTE>		vTable;
	BYTE					bAddress;
} NES_VRAM_PALETTE, *PNES_VRAM_PALETTE;

typedef std::vector<NES_METATILE_DATA>					NES_METATILE_LIST;

class CNesLevelView;

class CNesPalette
{
	CNESFile &													m_file;
	const CNesPointers &										m_eptr;
	BYTE														m_bgDefault;

	NES_VRAM_TABLE												m_nvtType[ 4 ];

	std::map<NES_VRAM_TABLE, std::vector<NES_VRAM_PALETTE>>		m_mVRAMTable;
	USHORT														GetVRAMAddrTablePtr( NES_VRAM_TABLE tableId );
	VOID														LoadVRAMTable( NES_VRAM_TABLE nvt );
	VOID														DumpVRAMTables( );

	VOID							LoadSpecialTypes();
	VOID							LoadSpecialStyles();

	VOID							ForceLoadTable( std::vector<NES_VRAM_PALETTE> & vTable, BOOL fSpecial, USHORT & uPtr, BYTE bLength, USHORT uPPUPtr );
	VOID							ModPalette( NES_PAL_ARRAY & pal, NES_VRAM_TABLE nvt );
	VOID							WrtPalette( NES_PAL_ARRAY & pal, NES_VRAM_TABLE nvt );

public:
	CNesPalette( CNESFile & file, const CNesPointers & eptr );

	VOID					LoadPalettes();
	VOID					DumpPalettes();

	VOID					GetPalette( NES_PAL_ARRAY & pal, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, BOOL fApplyDefaultBg = FALSE );
	VOID					SetPalette( const NES_PAL_ARRAY & pal, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier );
	VOID					ApplyPalette( CPaletteBitmap & bmp, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier );
	VOID					SetDefaultBg( BYTE bDefaultBg );
};

class CNesMetatiles
{
	CNESFile							    & m_file;
	const CNesPointers						& m_eptr;
	CNesPalette								m_palette;
	CNesTileNCache							m_ntNCache;
	NES_METATILE_LIST						m_vList[ NES_COUNT_METATILES_TABLES ];
	NES_ENEMY_GFX							m_gfx;
	NES_POWERUP_GFX_TABLE					m_pu;

	struct
	{
		NES_PLAYER_TABLE					table;
		NES_PLAYER_TILES					tiles;
	} m_player;

	VOID									CacheMetatileBitmap( const NES_METATILE & uMTileId );
	VOID									CacheEnemyBitmap( UINT uSpriteId, UINT uAttribute );
	VOID									CacheEnemyBitmap
											(
												UINT uSpriteId, UINT uAttribute,
												const BYTE pbXOffset[ 6 ],
												const BYTE pbYOffset[ 6 ],
												const BYTE pbFlips[ 6 ],
												const BYTE pbTiles[ 6 ]
											);

	VOID									CacheUpSprites( BYTE bAttribute );
	VOID									CachePlayerSprites( BYTE bAttribute );
	VOID									CacheUpSprite( NES_UP_SPRITE_TABLE nust, BYTE bAttribute, const BYTE bSpriteData[ 4 ][ 6 ] );
	VOID									DrawCache();
	USHORT									GetVRAMAddrTablePtr( NES_VRAM_TABLE tableId );
	VOID									LoadMetatilesList();
	VOID									LoadGFXList();
	VOID									LoadGFXPlayer();
	VOID									LoadPalettes();

	VOID									DumpPalettes();

public:
	CNesMetatiles( CNESFile & file, const CNesPointers & eptr );
	VOID									Load();
	VOID									Dump();

	VOID									ReadPalette( NES_PAL_ARRAY & npa, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier );
	VOID									UpdatePalette( const NES_PAL_ARRAY & npa, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier );

	VOID									SelectBg( BYTE bBgCol );
	VOID									SelectPalette( CPaletteBitmap & bmp, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier );
	VOID									DrawMetatile( CPaletteBitmap & bmp, INT x, INT y, const NES_MAP_METATILE & uMTileId, int uReservedSpace ) const;
	VOID									DrawSprite( CPaletteBitmap & bmp, INT x, INT y, const NES_SPRITE & uMTileId, int uReservedSpace ) const;
};
