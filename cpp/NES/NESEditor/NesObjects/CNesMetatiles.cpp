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


// METATILES object

const BYTE g_sprFlips[ NES_ENEMY_GFX_COUNT ][ 3 ][ 6 ] =		// { flip flags: 1 - horz flip, 2 - vert flip }, { X }, { Y }
{
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 0, 2, 1, 2, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// spiny sprite 1
	{ { 0, 0, 2, 1, 2, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// spiny sprite 2
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// blooper sprite 1
	{ { 0, 1, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// blooper sprite 2
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// kooper 1
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// kooper 2
	{ { 0, 0, 2, 3, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// kooper 3
	{ { 0, 0, 2, 3, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// kooper 4
	{ { 0, 0, 2, 3, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// beetle1
	{ { 0, 0, 2, 3, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// beetle2
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// beetle3
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// beetle4
	{ { 0, 0, 0, 0, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// defeated goomba
	{ { 0, 0, 0, 0, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// lakitu1
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// lakitu2
	{ { 0, 1, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 1, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// toadstool
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 1, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// pirahna plant 1
	{ { 0, 1, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// pirahna plant 2
	{ { 0, 0, 0, 1, 0, 1 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// firework
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 1, 1, 1, 1, 1, 1 }, { 8, 0, 8, 0, 8, 0 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },
	{ { 0, 1, 0, 1, 2, 3 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// jumpspring1
	{ { 0, 1, 2, 3, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, },	// jumpspring2
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 8, 0, 8, 0, 8 }, { 0, 0, 8, 8, 16, 16 }, }
};

CNesMetatiles::CNesMetatiles( CNESFile & file, const CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr ), m_ntNCache( m_file ), m_palette( file, eptr )
{
	m_file.ReversedPPU( TRUE );
}

VOID CNesMetatiles::CacheMetatileBitmap( const NES_METATILE & uMTileId )
{
	m_ntNCache.CacheMetatile( uMTileId, m_vList[ uMTileId.attr ][ uMTileId.tileId ] );
}

VOID CNesMetatiles::CacheEnemyBitmap( UINT uSpriteId, UINT uAttribute )
{
	if ( uSpriteId < NES_ENEMY_GFX_COUNT )
	{
		CacheEnemyBitmap(
			uSpriteId, uAttribute,
			g_sprFlips[ uSpriteId ][ 1 ],
			g_sprFlips[ uSpriteId ][ 2 ],
			g_sprFlips[ uSpriteId ][ 0 ],
			m_gfx.bTiles[ uSpriteId ] );
	}
}

VOID CNesMetatiles::CacheEnemyBitmap
(
	UINT uSpriteId, UINT uAttribute,
	const BYTE pbXOffset[ 6 ], const BYTE pbYOffset[ 6 ], const BYTE pbFlips[ 6 ], const BYTE pbTiles[ 6 ]
)
{
	m_ntNCache.CacheSprite( uSpriteId, pbXOffset, pbYOffset, pbFlips, pbTiles );
}

/*
VOID CNesMetatiles::DrawMetatile( HDC hDC, INT x, INT y, const NES_MAP_METATILE & uMTileId, int uReservedSpace ) const
{
	POINT tpt;
	BYTE bPalId = LOBYTE( GetSelectedPalette() );
	if ( m_ntCache.MTile( bPalId, uMTileId.bMetatile, tpt ) )
	{
		BitBlt
		( 
			hDC, NES_METATILE_SIZE * x, NES_METATILE_SIZE * y + uReservedSpace, NES_METATILE_SIZE, NES_METATILE_SIZE, 
			m_ntCache, tpt.x, tpt.y, 
			SRCCOPY 
		);

		if ( uMTileId.fBehind )
		{
			COLORREF clrBk = SetTextColor( hDC, RGB( 0xff, 0xff, 0xff ) );
			RECT rc = { NES_METATILE_SIZE * x, NES_METATILE_SIZE * y + uReservedSpace, NES_METATILE_SIZE, NES_METATILE_SIZE };
			rc.right += rc.left;
			rc.bottom += rc.top;
			DrawText( hDC, TEXT( "X" ), 1, &rc, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
			OffsetRect( &rc, 1, 1 );
			SetTextColor( hDC, clrBk );
			DrawText( hDC, TEXT( "X" ), 1, &rc, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
		}
	}
}
*/

VOID CNesMetatiles::DrawCache()
{
	for ( UINT i = 0; i < NES_COUNT_METATILES_TABLES; ++i )
	{
		const INT cTiles = LODWORD( m_vList[ i ].size() );
		const NES_METATILE_LIST & vList = m_vList[ i ];
		NES_METATILE tile;

		tile.attr = LOBYTE( i );
		tile.tileId = 0;
		for ( INT t = 0; t < cTiles; ++t, tile.tileId++ )
		{
			CacheMetatileBitmap( tile );
		}
	}

	for ( UINT t = 0; t < NES_COUNT_METATILES_TABLES; ++t )
	{
		CacheUpSprites( t );
		CachePlayerSprites( t );
		for ( UINT g = 0; g < NES_ENEMY_GFX_COUNT; ++g )
		{
			CacheEnemyBitmap( g, t );
		}
	}
}


VOID CNesMetatiles::CacheUpSprites( BYTE bAttribute )
{
	BYTE bFireworks[4][6] = 
	{ 
		{ 0x04, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x04, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x64, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC } 
	};

	BYTE bFlagpole[4][6]=
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0xFC, 0xFC, 0x7E, 0x7F, 0xFC, 0x7E }
	};

	BYTE bBowserFlame[ 2 ][ 4 ][ 6 ] =
	{
		{
			{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
			{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x51, 0x52, 0xFC, 0xFC, 0xFC, 0xFC }
		},
		{
			{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
			{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x53, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC }
		}
	};

	BYTE bBridge[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x02, 0x03, 0x00, 0x00 },
		{ 0x5B, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC }
	};

	BYTE bBridge2[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x02, 0x03, 0x00, 0x00 },
		{ 0x75, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC }
	};

	BYTE bPowerup[ 4 ][ 6 ]=
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 
			m_pu.bTile[ npuMushroom1 ][ 0 ], 
			m_pu.bTile[ npuMushroom1 ][ 1 ], 
			m_pu.bTile[ npuMushroom1 ][ 2 ], 
			m_pu.bTile[ npuMushroom1 ][ 3 ], 
			0xFC, 0xFC 
		}
	};

	BYTE bStarman[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x00, 0x01, 0x00, 0x00 },
		{ 
			m_pu.bTile[ npuStarman ][ 0 ],
			m_pu.bTile[ npuStarman ][ 1 ],
			m_pu.bTile[ npuStarman ][ 2 ],
			m_pu.bTile[ npuStarman ][ 3 ],
			0xFC, 0xFC
		}
	};

	BYTE bFlower[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x00, 0x01, 0x00, 0x00 },
		{ 
			m_pu.bTile[ npuFlower ][ 0 ],
			m_pu.bTile[ npuFlower ][ 1 ],
			m_pu.bTile[ npuFlower ][ 2 ],
			m_pu.bTile[ npuFlower ][ 3 ],
			0xFC, 0xFC
		}
	};

	BYTE bCoin[ 4 ][ 6 ] =
	{
		{ 0x04, 0x08, 0x04, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 },
		{ 0x60, 0xFC, 0x60, 0xFC, 0xFC, 0xFC }
	};

	BYTE bVine[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x00, 0x01, 0x00, 0x00 },
		{ 0xE0, 0xE1, 0xE0, 0xE1, 0xFC, 0xFC }
	};

	BYTE bFrenzyMark[ 4 ][ 6 ] = 
	{
		{ 0x04, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x7A, 0xFC, 0xFC, 0xFC, 0xFC, 0x7B }
	};

	BYTE bDigitMark[ 4 ][ 6 ] =
	{
		{ 0x04, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x50, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC }
	};

	BYTE bWarpMark[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x54, 0x55, 0x56, 0x57, 0xFC, 0xFC }
	};

	BYTE bObjFrenzy[ 4 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x84, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC }
	};

	BYTE bHardMark[ 4 ][ 6 ]=
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x01, 0x02, 0x03, 0x00, 0x00 },
		{ 0x68, 0x68, 0x68, 0x68, 0xFC, 0xFC }
	};

	CacheUpSprite( nustFirebar, bAttribute, bFireworks );
	CacheUpSprite( nustFlagpole, bAttribute, bFlagpole );
	CacheUpSprite( nustBowserFire1, bAttribute, bBowserFlame[ 0 ] );
	CacheUpSprite( nustBowserFire2, bAttribute, bBowserFlame[ 1 ] );
	CacheUpSprite( nustBridge1, bAttribute, bBridge );
	bBridge[ 0 ][ 0 ] = 4;  bBridge[ 1 ][ 0 ] = 8; CacheUpSprite( nustBridge2, bAttribute, bBridge );
	CacheUpSprite( nustBridge3, bAttribute, bBridge2 );
	CacheUpSprite( nustPowerup, bAttribute, bPowerup );
	CacheUpSprite( nustStarman, bAttribute, bStarman );
	CacheUpSprite( nustFlower, bAttribute, bFlower );
	CacheUpSprite( nustCoin, bAttribute, bCoin );
	CacheUpSprite( nustVine, bAttribute, bVine );
	CacheUpSprite( nustFrenzyMark, bAttribute, bFrenzyMark );	
	bDigitMark[ 3 ][ 0 ] = 0x50; CacheUpSprite( nustDigitMark00, bAttribute, bDigitMark );
	bDigitMark[ 3 ][ 0 ] = 0xF6; CacheUpSprite( nustDigitMark10, bAttribute, bDigitMark );
	bDigitMark[ 3 ][ 0 ] = 0xF7; CacheUpSprite( nustDigitMark20, bAttribute, bDigitMark );
	bDigitMark[ 3 ][ 0 ] = 0xF8; CacheUpSprite( nustDigitMark40, bAttribute, bDigitMark );
	bDigitMark[ 3 ][ 0 ] = 0xF9; CacheUpSprite( nustDigitMark50, bAttribute, bDigitMark );
	bDigitMark[ 3 ][ 0 ] = 0xFA; CacheUpSprite( nustDigitMark80, bAttribute, bDigitMark );
	CacheUpSprite( nustWarpMark, bAttribute, bWarpMark );
	CacheUpSprite( nustObjFrenzy, bAttribute, bObjFrenzy );
	CacheUpSprite( nustHardMark, bAttribute, bHardMark );
}

VOID CNesMetatiles::CacheUpSprite( NES_UP_SPRITE_TABLE nust, BYTE bAttribute, const BYTE bSpriteData[ 4 ][ 6 ] )
{
	CacheEnemyBitmap( nust, bAttribute, bSpriteData[ 0 ], bSpriteData[ 1 ], bSpriteData[ 2 ], bSpriteData[ 3 ] );
}

VOID CNesMetatiles::CachePlayerSprites( BYTE bAttribute )
{
	BYTE uSprId = 0;
	BYTE bPlayerChar[ 3 ][ 6 ] =
	{
		{ 0x00, 0x08, 0x00, 0x08, 0x00, 0x08 },
		{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	};

	for ( auto b : m_player.table.bOffset )
	{
		if ( b < 0x90 )
		{
			CacheEnemyBitmap( uSprId + NES_PLAYER_GFX_SPRITES_LO, bAttribute, bPlayerChar[ 0 ], bPlayerChar[ 1 ], bPlayerChar[ 2 ], &m_player.tiles.bTiles[ b ] );
			uSprId++;
		}
	}

	uSprId = 0;
	for ( auto b : m_player.table.bOffset )
	{
		if ( b < 0x90 )
		{
			CacheEnemyBitmap( uSprId + NES_PLAYER_GFX_SPRITES_HI, bAttribute, bPlayerChar[ 0 ], bPlayerChar[ 1 ], bPlayerChar[ 2 ], &m_player.tiles.bTiles[ b + 2 ] );
			uSprId++;
		}
	}
}

USHORT CNesMetatiles::GetVRAMAddrTablePtr( NES_VRAM_TABLE tableId )
{
	USHORT uVRAMAddrTableLo = m_eptr.Pointer( eVRAMAddrTableLo ), uVRAMAddrTableHi = m_eptr.Pointer( eVRAMAddrTableHi );
	USHORT uResultPtr = m_file.Data<USHORT>( MAKEWORD( uVRAMAddrTableLo, uVRAMAddrTableHi ) + static_cast<USHORT>( tableId ) * sizeof( USHORT ) );
	return uResultPtr;
}

VOID CNesMetatiles::LoadMetatilesList()
{
	const CNesPointers & eptr = m_eptr;
	USHORT uNesMetatilesHi = eptr.Pointer( eMetatiesPtrHi ), uNesMetatilesLo = eptr.Pointer( eMetatiesPtrLo );
	USHORT uEOFMetatiles = GetVRAMAddrTablePtr( nvtWaterPaletteData );	// table bWaterPaletteData begins right after metatiles
	std::vector<USHORT> vMetatilesPtr;
	std::vector<size_t> vMetatilesCount;
	vMetatilesPtr.push_back( MAKEWORD( m_file.Data<BYTE>( uNesMetatilesLo + 0 ), m_file.Data<BYTE>( uNesMetatilesHi + 0 ) ) );
	vMetatilesPtr.push_back( MAKEWORD( m_file.Data<BYTE>( uNesMetatilesLo + 1 ), m_file.Data<BYTE>( uNesMetatilesHi + 1 ) ) );
	vMetatilesPtr.push_back( MAKEWORD( m_file.Data<BYTE>( uNesMetatilesLo + 2 ), m_file.Data<BYTE>( uNesMetatilesHi + 2 ) ) );
	vMetatilesPtr.push_back( MAKEWORD( m_file.Data<BYTE>( uNesMetatilesLo + 3 ), m_file.Data<BYTE>( uNesMetatilesHi + 3 ) ) );
	vMetatilesPtr.push_back( uEOFMetatiles );

	// calculate count metatiles in group between neighbors groups
	for ( size_t i = 0; i < NES_COUNT_METATILES_TABLES; ++i )
	{
		size_t uCounter = -1;
		for ( size_t k = 0; k < NES_COUNT_METATILES_TABLES + 1; ++k )
		{
			if ( i == k )
			{
				continue;
			}
			else if ( vMetatilesPtr[ k ] >= vMetatilesPtr[ i ] )
			{
				size_t uLocalCounter = vMetatilesPtr[ k ] - vMetatilesPtr[ i ];
				uCounter = min( uCounter, uLocalCounter );
			}
		}

		vMetatilesCount.push_back( uCounter / sizeof( NES_METATILE_DATA ) );
	}

	// now load metatiles
	for ( size_t i = 0; i < vMetatilesCount.size(); ++i )
	{
		size_t uCounter = min( NES_METATILE_COUNT_FOR_GROUP, vMetatilesCount[ i ] );
		USHORT uPtr = vMetatilesPtr[ i ];

		for ( size_t t = 0; t < uCounter; ++t )
		{
			m_vList[ i ].push_back( m_file.Data<NES_METATILE_DATA>( uPtr ) );
			uPtr += sizeof( NES_METATILE_DATA );
		}
	}
}

VOID CNesMetatiles::LoadGFXList()
{
	m_gfx = m_file.Data<NES_ENEMY_GFX>( m_eptr.Pointer( eGFXEnemy ) );
	m_pu = m_file.Data<NES_POWERUP_GFX_TABLE>( m_eptr.Pointer( eGFXPowerUp ) );
}

VOID CNesMetatiles::LoadGFXPlayer()
{
	m_player.table = m_file.Data<NES_PLAYER_TABLE>( m_eptr.Pointer( ePlayerSpriteTable ) );
	m_player.tiles = m_file.Data<NES_PLAYER_TILES>( m_eptr.Pointer( ePlayerTiles ) );
}

VOID CNesMetatiles::LoadPalettes()
{
	m_palette.LoadPalettes();
}

VOID CNesMetatiles::DumpPalettes()
{
	m_palette.DumpPalettes();
}

VOID CNesMetatiles::Load()
{
	LoadMetatilesList();
	LoadGFXList();
	LoadGFXPlayer();
	LoadPalettes();
	DrawCache();

/*
#ifdef _DEBUG
	CPaletteBitmap palbm( 64 * 16, 4 * 16 );
	CBitmap bm( 64 * 16, 4 * 16 );
	SelectPalette( palbm, nltGround, areaStyle0, bgCtl0 );
	for ( BYTE attr = 0; attr < 4; ++attr )
	{
		for ( BYTE id = 0; id < 64; ++id )
		{
			NES_METATILE mt;
			mt.attr = attr;
			mt.tileId = id;
			m_ntNCache.DrawMetatile( palbm, mt, id, attr, 0 );
		}
	}

	BitBlt( bm, 0, 0, bm.Width(), bm.Height(), palbm, 0, 0, SRCCOPY );

	// CPngLoader png( m_ntCache.Enemy() );
	// png.SavePng( TEXT( "c:\\Users\\AGATHA\\Documents\\STUFF\\src\\jGreatEd\\jGreatEd\\docs\\enemy.png" ) );
	CPngLoader png2( bm );
	png2.SavePng( TEXT( "c:\\Users\\user\\Documents\\STUFF\\src\\jGreatEd\\jGreatEd\\docs\\tiles.png" ) );
#endif
*/
}

VOID CNesMetatiles::Dump()
{
	DumpPalettes();
}

VOID CNesMetatiles::ReadPalette( NES_PAL_ARRAY & npa, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	m_palette.GetPalette( npa, nlt, nas, bgModifier );
}

VOID CNesMetatiles::UpdatePalette( const NES_PAL_ARRAY & npa, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	m_palette.SetPalette( npa, nlt, nas, bgModifier );
}

VOID CNesMetatiles::SelectBg( BYTE bBgCol )
{
	m_palette.SetDefaultBg( bBgCol );
}

VOID CNesMetatiles::SelectPalette( CPaletteBitmap & bmp, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	m_palette.ApplyPalette( bmp, nlt, nas, bgModifier );
}

VOID CNesMetatiles::DrawMetatile( CPaletteBitmap & bmp, INT x, INT y, const NES_MAP_METATILE & uMTileId, int uReservedSpace ) const
{
	m_ntNCache.DrawMetatile( bmp, uMTileId.bMetatile, x, y, uReservedSpace );

	if ( uMTileId.fBehind )
	{
		COLORREF clrBk = SetTextColor( bmp, RGB( 0xff, 0xff, 0xff ) );
		RECT rc = { NES_METATILE_SIZE * x, NES_METATILE_SIZE * y + uReservedSpace, NES_METATILE_SIZE, NES_METATILE_SIZE };
		rc.right += rc.left;
		rc.bottom += rc.top;
		DrawText( bmp, TEXT( "X" ), 1, &rc, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
		OffsetRect( &rc, 1, 1 );
		SetTextColor( bmp, clrBk );
		DrawText( bmp, TEXT( "X" ), 1, &rc, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
	}

}

VOID CNesMetatiles::DrawSprite( CPaletteBitmap & bmp, INT x, INT y, const NES_SPRITE & uMTileId, int uReservedSpace ) const
{
	m_ntNCache.DrawSprite( bmp, uMTileId, x, y, uReservedSpace );
}

// ---- nes palette ----

CNesPalette::CNesPalette( CNESFile & file, const CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr ), m_bgDefault( 0x0F )
{

}

USHORT CNesPalette::GetVRAMAddrTablePtr( NES_VRAM_TABLE tableId )
{
	USHORT uVRAMAddrTableLo = m_eptr.Pointer( eVRAMAddrTableLo ), uVRAMAddrTableHi = m_eptr.Pointer( eVRAMAddrTableHi );
	USHORT uResultPtr = m_file.Data<USHORT>( MAKEWORD( uVRAMAddrTableLo, uVRAMAddrTableHi ) + static_cast<USHORT>( tableId ) * sizeof( USHORT ) );
	return uResultPtr;
}

VOID CNesPalette::LoadVRAMTable( NES_VRAM_TABLE nvt )
{
	USHORT uVRAMTablePtr = GetVRAMAddrTablePtr( nvt );

	if ( uVRAMTablePtr < NES_FDS_BEGIN ) return;	// pointer may points to RAM

	while ( m_file.Data<BYTE>( uVRAMTablePtr ) )
	{
		USHORT uPPUPtr = MAKEWORD( m_file.Data<BYTE>( uVRAMTablePtr + 1 ), m_file.Data<BYTE>( uVRAMTablePtr ) );
		uVRAMTablePtr += sizeof( USHORT );
		if ( uPPUPtr >= 0x3F00 && uPPUPtr <= 0x3F1F )
		{
			BYTE bLength = m_file.Data<BYTE>( uVRAMTablePtr++ );
			auto & table = m_mVRAMTable[ nvt ];

			ForceLoadTable( table, FALSE, uVRAMTablePtr, bLength, uPPUPtr );
		}
	}
}

VOID CNesPalette::ForceLoadTable( std::vector<NES_VRAM_PALETTE>& vTable, BOOL fSpecial, USHORT & uPtr, BYTE bLength, USHORT uPPUPtr )
{
	NES_VRAM_PALETTE nvp;
	BYTE bLen = ( bLength & 0x3F );
	nvp.fCustomTable = TRUE;
	nvp.fRepeat = ( 0x40 == ( 0x40 & bLength ) );
	nvp.uPtr = uPtr;
	nvp.bAddress = ( uPPUPtr - 0x3F00 );
	nvp.vTable.reserve( bLen );

	if ( nvp.fRepeat )
	{
		BYTE b = m_file.Data<BYTE>( uPtr++ );
		nvp.vTable.insert( nvp.vTable.end(), bLen, b );
	}
	else
	{
		for ( BYTE b = 0; b < bLen; ++b )
		{
			nvp.vTable.push_back( m_file.Data<BYTE>( uPtr++ ) );
		}
	}

	vTable.push_back( nvp );
}

VOID CNesPalette::LoadSpecialTypes()
{
	USHORT uTypesPalPtr = m_eptr.Pointer( ePatchPal );
	USHORT uBlankPalPtr = m_eptr.Pointer( eBlankPal );
	NES_VRAM_TABLE nvtarr[] = { nvtpSpecialWater, nvtpSpecialGround, nvtpSpecialUnderground, nvtpSpecialCastle };
	USHORT uPPUPtr = MAKEWORD( m_file.Data<BYTE>( uBlankPalPtr + 1 ), m_file.Data<BYTE>( uBlankPalPtr ) );

	for ( const auto & nvt : nvtarr )
	{
		ForceLoadTable( m_mVRAMTable[ nvt ], TRUE, uTypesPalPtr, 4, uPPUPtr );
	}
}

VOID CNesPalette::LoadSpecialStyles()
{
	CopyMemory( m_nvtType, m_file.Data<BYTE[ 4 ]>( m_eptr.Pointer( eStyleVRAMTables ) + 4 ), 4 );
}

VOID CNesPalette::DumpVRAMTables()
{
	for ( const auto & table : m_mVRAMTable )
	{
		for ( const auto & nvp : table.second )
		{
			USHORT uVRAMPtr = nvp.uPtr;
			if ( !nvp.fCustomTable )
			{
				m_file.Data<BYTE>( uVRAMPtr - 3 ) = 0x3F;
				m_file.Data<BYTE>( uVRAMPtr - 2 ) = nvp.bAddress;
			}

			if ( nvp.fRepeat && !nvp.vTable.empty() )
			{
				if ( !nvp.fCustomTable )
				{
					m_file.Data<BYTE>( uVRAMPtr - 1 ) = 0x40 | ( 0x3F & LOBYTE( nvp.vTable.size() ) );
				}
				m_file.Data<BYTE>( uVRAMPtr ) = nvp.vTable[ 0 ];
			}
			else
			{
				BYTE c = LOBYTE( 0x3F & nvp.vTable.size() );
				if ( !nvp.fCustomTable )
				{
					m_file.Data<BYTE>( uVRAMPtr - 1 ) = c;
				}

				for ( BYTE n = 0; n < c; ++n )
				{
					m_file.Data<BYTE>( uVRAMPtr++ ) = nvp.vTable[ n ];
				}
			}
		}
	}
}

VOID CNesPalette::LoadPalettes()
{
	for ( BYTE vt = 0; vt < nvtpVRAMTablesCount; ++vt )
	{
		const NES_VRAM_TABLE & nvt = static_cast<NES_VRAM_TABLE>( vt );
		LoadVRAMTable( nvt );
	}
	
	LoadSpecialTypes();
	LoadSpecialStyles();
}

VOID CNesPalette::DumpPalettes()
{
	DumpVRAMTables();
}

VOID CNesPalette::ModPalette( NES_PAL_ARRAY & pal, NES_VRAM_TABLE nvt )
{
	auto itTable = m_mVRAMTable.find( nvt );
	if ( itTable != m_mVRAMTable.end() )
	{
		const auto & vTable = itTable->second;

		for ( const auto & nvp : vTable )
		{
			BYTE bLen = LOBYTE( nvp.vTable.size() ), i = 0;
			for ( BYTE b = nvp.bAddress; b < nvp.bAddress + bLen; ++b, ++i )
			{
				pal.bPal[ b / 0x10 ][ b % 0x10 ] = nvp.vTable[ i ];
			}
		}
	}
}

VOID CNesPalette::WrtPalette( NES_PAL_ARRAY & pal, NES_VRAM_TABLE nvt )
{
	auto itTable = m_mVRAMTable.find( nvt );
	if ( itTable != m_mVRAMTable.end() )
	{
		auto & vTable = itTable->second;

		for ( auto & nvp : vTable )
		{
			BYTE bLen = LOBYTE( nvp.vTable.size() ), i = 0;
			for ( BYTE b = nvp.bAddress; b < nvp.bAddress + bLen; ++b, ++i )
			{
				BYTE & col = pal.bPal[ b / 0x10 ][ b % 0x10 ];
				if ( 0xFF != col )
				{
					nvp.vTable[ i ] = col;
					col = 0xFF;
				}
			}
		}
	}
}

VOID CNesPalette::GetPalette( NES_PAL_ARRAY & pal, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, BOOL fApplyDefaultBg )
{
	switch ( nlt )
	{
		case nltWater: ModPalette( pal, nvtWaterPaletteData ); break;
		case nltGround: ModPalette( pal, nvtGroundPaletteData ); break;
		case nltUnderground: ModPalette( pal, nvtUndergroundPaletteData ); break;
		case nltCastle: ModPalette( pal, nvtCastlePaletteData ); break;
		default: break;
	}

	switch( bgModifier )
	{
		case bgCtl2:
		case bgCtl3:
		case bgCtl4: ModPalette( pal, m_nvtType[ bgModifier - bgCtl1 ] ); break;
		default: break;
	}

	switch ( nlt )
	{
		case nltWater: ModPalette( pal, nvtpSpecialWater ); break;
		case nltGround: ModPalette( pal, nvtpSpecialGround ); break;
		case nltUnderground: ModPalette( pal, nvtpSpecialUnderground ); break;
		case nltCastle: ModPalette( pal, nvtpSpecialCastle ); break;
	}

	switch ( nas )
	{
		case areaStyle1: ModPalette( pal, nvtMushroomPaletteData ); break;
		default: break;
	}

	if ( fApplyDefaultBg )
	{
		pal.bPal[ 0 ][ 0 ] = pal.bPal[ 0 ][ 4 ] = pal.bPal[ 0 ][ 8 ] = pal.bPal[ 0 ][ 12 ] = m_bgDefault;
		pal.bPal[ 1 ][ 0 ] = pal.bPal[ 1 ][ 4 ] = pal.bPal[ 1 ][ 8 ] = pal.bPal[ 1 ][ 12 ] = m_bgDefault;
	}
}

VOID CNesPalette::SetPalette( const NES_PAL_ARRAY & pal, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	NES_PAL_ARRAY spal = pal;

	if ( bgModifier >= bgCtl2 && bgModifier <= bgCtl4 )
	{
		switch ( bgModifier )
		{
			case bgCtl2:
			case bgCtl3:
			case bgCtl4: WrtPalette( spal, m_nvtType[ bgModifier - bgCtl1 ] ); break;
			default: break;
		}
	}
	else if ( areaStyle1 == nas )
	{
		WrtPalette( spal, nvtMushroomPaletteData );
	}
	else
	{
		switch ( nlt )
		{
			case nltWater: WrtPalette( spal, nvtpSpecialWater ); break;
			case nltGround: WrtPalette( spal, nvtpSpecialGround ); break;
			case nltUnderground: WrtPalette( spal, nvtpSpecialUnderground ); break;
			case nltCastle: WrtPalette( spal, nvtpSpecialCastle ); break;
			default: break;
		}

		switch ( nlt )
		{
			case nltWater: WrtPalette( spal, nvtWaterPaletteData ); break;
			case nltGround: WrtPalette( spal, nvtGroundPaletteData ); break;
			case nltUnderground: WrtPalette( spal, nvtUndergroundPaletteData ); break;
			case nltCastle: WrtPalette( spal, nvtCastlePaletteData ); break;
			default: break;
		}
	}
}

VOID CNesPalette::SetDefaultBg( BYTE bDefaultBg )
{
	m_bgDefault = ( bDefaultBg & 0x3F );
}

VOID CNesPalette::ApplyPalette( CPaletteBitmap & bmp, NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	NES_PAL_ARRAY pal;
	GetPalette( pal, nlt, nas, bgModifier, TRUE );

	size_t i = 0;
	std::vector<COLORREF> vPal( 64, 0 );
	BYTE bgCol = pal.bPal[ 0 ][ 0 ];

	for ( BYTE bType = 0; bType < 2; ++bType )
	{
		for ( BYTE col = 0; col < NES_PAL_COLORS; ++col, ++i )
		{
			vPal[ i ] = XCHGCOLORREF( g_crNesColor[ ( ( col % 4 ) ? pal.bPal[ bType ][ col ] : bgCol ) ] );
			vPal[ i + 0x20 ] = g_crNesColor[ ( ( col % 4 ) ? pal.bPal[ bType ][ col ] : bgCol ) ];
		}
	}

	bmp.Entries( vPal );
}