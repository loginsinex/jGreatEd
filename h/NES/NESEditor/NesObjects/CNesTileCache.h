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

typedef union _tagNES_METATILE
{
	BYTE			bMetatile;
	struct
	{
		BYTE		tileId : 6;
		BYTE		attr : 2;
	};
} NES_METATILE, *PNES_METATILE;

typedef struct _tagNES_METATILE_DATA
{
	BYTE			bHiLeft;
	BYTE			bBottomLeft;
	BYTE			bHiRight;
	BYTE			bBottomRight;
} NES_METATILE_DATA, *PNES_METATILE_DATA;

typedef struct _tagNES_MAP_METATILE
{
	NES_METATILE	bMetatile;
	BOOL			fBehind;
} NES_MAP_METATILE, *PNES_MAP_METATILE;

typedef union _tagNES_SPRITE
{
	WORD			wSprite;
	struct
	{
		BYTE		id;
		union
		{
			BYTE		attrData;
			struct
			{
				BYTE	attr : 2;
				BYTE	__unused : 3;
				BYTE	fForeground : 1;
				BYTE	fHorzFlip : 1;
				BYTE	fVertFlip : 1;
			};
		};
	};
} NES_SPRITE, *PNES_SPRITE;

class CNesTileNCache
{
	CNesBitmap				m_nbm;
	CNesBitmap				m_nbmSprite;
	CNESFile				& m_file;

	VOID					InitBitmap();

public:
	CNesTileNCache( CNESFile & file );

	VOID					CacheSprite
	(
		UINT uSpriteId,
		const BYTE pbXOffset[ 6 ],
		const BYTE pbYOffset[ 6 ],
		const BYTE pbFlips[ 6 ],
		const BYTE pbTiles[ 6 ]
	);

	VOID					CacheMetatile( const NES_METATILE & mTile, const NES_METATILE_DATA & mTileData );

	VOID					DrawMetatile( CPaletteBitmap & bm, const NES_METATILE & mTile, INT x, INT y, int uReservedSpace ) const;
	VOID					DrawSprite( CPaletteBitmap & bm, const NES_SPRITE & mTile, INT x, INT y, int uReservedSpace ) const;
};
