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

#include "NES/NESMap/CNESFile.h"

CNESPPU::CNESPPU( CFDSFile & file )
	: CNESMap( file, NES_PPU_SIZE )
{

}

USHORT CNESPPU::FileMapping( const FDS_FILE_HEADER_BLOCK & fhb )
{
	return fhb.uTargetPtr;
}

VOID CNESPPU::FileMapped( CFDSDiskFile & file, const NES_RAM_MAP & fileId )
{
	LoadTiles();
}

VOID CNESPPU::LoadTiles()
{
	for ( size_t n = 0; n < 2; ++n )
	{		
		size_t ptr = 0x1000 * n;

		for ( size_t nTile = 0; nTile < NES_TILES_COUNT; ++nTile, ptr += 0x10 )
		{
			NES_TILE & tl = m_tiles[ n ][ nTile ];
			for ( int y = 0; y < 8; ++y )
			{
				for ( int x = 0; x < 8; ++x )
				{
					tl.pix[ y ][ x ] =
						( 1 & ( m_vRAM[ ptr + y ] >> ( 7 - x ) ) |
						( 1 & ( m_vRAM[ 8 + ptr + y ] >> ( 7 - x ) ) ) << 1 );

				}
			}
		}
	}
}

VOID CNESPPU::DrawTile( CNesBitmap & bm, DWORD x, DWORD y, BYTE tileId, BYTE bAttribute, BOOL fSprite, BOOL fFlipHorz, BOOL fFlipVert )
{
	if ( !fSprite )
	{
		bm.BlitTile( m_tiles[ m_fReversed ? 1 : 0 ][ tileId ], x, y, bAttribute );
	}
	else
	{
		bm.BlitSprite( m_tiles[ m_fReversed ? 0 : 1 ][ tileId ], x, y, bAttribute, fFlipHorz, fFlipVert );
	}
}

VOID CNESPPU::ReverseMode( BOOL fReverse )
{
	m_fReversed = fReverse;
}
