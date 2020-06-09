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

// Cache for metatiles

CNesTileNCache::CNesTileNCache( CNESFile & file )
	: m_file( file )
{
	m_nbm.Create( NES_METATILE_SIZE * NES_METATILE_COUNT_FOR_GROUP, NES_METATILE_SIZE * NES_ATTR_COUNT );
	m_nbmSprite.Create( NES_SPRITE_WIDTH * NES_ENEMY_CACHE_SIZE, NES_SPRITE_HEIGHT * NES_ATTR_COUNT );
	InitBitmap();
}

VOID CNesTileNCache::InitBitmap()
{
	size_t uSizeSprite = m_nbmSprite.Width() * m_nbmSprite.Height();
	PNES_PIXEL pBits = m_nbmSprite.Bits();

	for ( size_t u = 0; u < uSizeSprite; ++u )
	{
		pBits[ u ].fSprite = TRUE;
	}
}

VOID CNesTileNCache::CacheSprite( UINT uSpriteId, const BYTE pbXOffset[ 6 ], const BYTE pbYOffset[ 6 ], const BYTE pbFlips[ 6 ], const BYTE pbTiles[ 6 ] )
{
	if ( uSpriteId < NES_ENEMY_CACHE_SIZE )
	{
		CNesBitmap & bm = m_nbmSprite;

		for ( BYTE uAttribute = 0; uAttribute < NES_ATTR_COUNT; ++uAttribute )
		{
			const POINT pt = { static_cast<LONG>( uSpriteId ) * NES_SPRITE_WIDTH, ( static_cast<LONG>( uAttribute ) ) * NES_SPRITE_HEIGHT };
			for ( BYTE i = 0; i < 6; ++i )
			{
				m_file.DrawTile
				( bm,
				  pt.x + pbXOffset[ i ],
				  pt.y + pbYOffset[ i ],
				  pbTiles[ i ], uAttribute, TRUE,
				  pbFlips[ i ] & 1,
				  pbFlips[ i ] & 2
				);
			}
		}
	}
}

VOID CNesTileNCache::CacheMetatile( const NES_METATILE & mTile, const NES_METATILE_DATA & mTileData )
{
	CNesBitmap & bm = m_nbm;
	const POINT pt = { NES_METATILE_SIZE * static_cast<LONG>( mTile.tileId ), NES_METATILE_SIZE * static_cast<LONG>( mTile.attr ) };

	m_file.DrawTile( bm, pt.x + 0, pt.y + 0, mTileData.bHiLeft, mTile.attr, FALSE, FALSE, FALSE );
	m_file.DrawTile( bm, pt.x + NES_TILE_WIDTH, pt.y + 0, mTileData.bHiRight, mTile.attr, FALSE, FALSE, FALSE );
	m_file.DrawTile( bm, pt.x + 0, pt.y + NES_TILE_HEIGHT, mTileData.bBottomLeft, mTile.attr, FALSE, FALSE, FALSE );
	m_file.DrawTile( bm, pt.x + NES_TILE_WIDTH, pt.y + NES_TILE_HEIGHT, mTileData.bBottomRight, mTile.attr, FALSE, FALSE, FALSE );
}

VOID CNesTileNCache::DrawMetatile( CPaletteBitmap & bm, const NES_METATILE & mTile, INT x, INT y, int uReservedSpace ) const
{
	const CNesBitmap & nbm = m_nbm;
	const POINT pt = { NES_METATILE_SIZE * static_cast<LONG>( mTile.tileId ), NES_METATILE_SIZE * static_cast<LONG>( mTile.attr ) };
	nbm.RBlitRaw( bm.Bits(), pt.x, pt.y, NES_METATILE_SIZE, NES_METATILE_SIZE, x * NES_METATILE_SIZE, y * NES_METATILE_SIZE + uReservedSpace, bm.Width(), bm.Height() );
}

VOID CNesTileNCache::DrawSprite( CPaletteBitmap & bm, const NES_SPRITE & mTile, INT x, INT y, int uReservedSpace ) const
{
	const CNesBitmap & nbm = m_nbmSprite;
	const POINT pt = { static_cast<LONG>( mTile.id ) * NES_SPRITE_WIDTH, ( static_cast<LONG>( mTile.attr ) ) * NES_SPRITE_HEIGHT };
	nbm.RBlitRaw( 
		bm.Bits(), 
		pt.x, pt.y, NES_SPRITE_WIDTH, NES_SPRITE_HEIGHT, 
		x * 8, y * 8 + uReservedSpace, bm.Width(), bm.Height(),
		mTile.fHorzFlip, mTile.fVertFlip
		);
}