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

CNesLevelView::CNesLevelView( CPaletteBitmap & bm, CNesMetatiles & mtiles )
	: m_bm( bm ), m_mTiles( mtiles ), m_uReservedSpace( NES_LEVEL_VIEW_RESERVED_HEIGHT ), m_fBehind( FALSE )
{
	m_szMap.cx = m_szMap.cy = 0;
	m_bm.Entry( 64, RGB( 0xb0, 0xb0, 0xb0 ) );
	m_bm.Entry( 65, RGB( 0x80, 0x80, 0x80 ) );
}

CNesLevelView::operator HDC()
{
	return m_bm;
}

CPaletteBitmap & CNesLevelView::Bitmap()
{
	return m_bm;
}

VOID CNesLevelView::DrawMetatile( INT x, INT y, const NES_METATILE & uMTileId )
{
	if ( x >= 0 && y >= 0 && x < static_cast<int>( m_szMap.cx ) && y < static_cast<int>( m_szMap.cy ))
	{
		m_nmap[ y ][ x ].bMetatile = uMTileId;
		m_nmap[ y ][ x ].fBehind = IsViewBehindMode();
	}
}

VOID CNesLevelView::DrawSprite( INT x, INT y, const NES_SPRITE & uMTileId )
{
	if ( x >= 0 && y >= 0 && x < 2 * static_cast<__int64>( m_szMap.cx ) && y < 2 * static_cast<__int64>( m_szMap.cy ) )
	{
		m_nsprmap[ y + ReservedLogicalSpace() ][ x ] = uMTileId;
	}
}

VOID CNesLevelView::AddBound( int x )
{
	m_vGroundBounds.push_back( x );
}

VOID CNesLevelView::ResetBounds()
{
	m_vGroundBounds.clear();
	m_vGroundBounds.reserve( 32 * 16 );
}

NES_METATILE CNesLevelView::GetMetatile( INT x, INT y )
{
	if ( static_cast<size_t>( x ) < m_szMap.cx && static_cast<size_t>( y ) < m_szMap.cy )
	{
		return m_nmap[ y ][ x ].bMetatile;
	}

	return { 0 };
}

VOID CNesLevelView::EraseSpriteMap()
{
	NES_SPRITE sprNull = { 0xFFFF };
	std::vector<NES_SPRITE> vsprx( m_szMap.cx * 2, sprNull );
	m_nsprmap.clear();
	m_nsprmap.insert( m_nsprmap.end(), m_szMap.cy * 2 + ReservedLogicalSpace(), vsprx );
}

VOID CNesLevelView::EraseAll()
{
	const DWORD cSize = LODWORD( sizeof( NES_METATILE )*m_szMap.cx );
	for ( auto & v : m_nmap )
	{
		ZeroMemory( v.data(), cSize );
	}

	EraseSpriteMap();
}

VOID CNesLevelView::SetMapSize( size_t cx, size_t cy )
{
	NES_MAP_METATILE nmNull = { 0 };

	m_szMap.cx = cx;
	m_szMap.cy = cy;

	std::vector<NES_MAP_METATILE> vx( cx, nmNull );

	m_nmap.clear();
	m_nmap.insert( m_nmap.end(), cy, vx );

	EraseSpriteMap();

	m_bm.Create( 16 * LODWORD( cx ), 16 * LODWORD( cy ) + ReservedSpace() );
}

VOID CNesLevelView::DrawPageBorders( HDC dc )
{
	int cy = LODWORD( m_szMap.cy ), cx = LODWORD( m_szMap.cx );

	HGDIOBJ holdfn = SelectObject( dc, GetStockObject( SYSTEM_FIXED_FONT ) );
	HPEN pen1 = CreatePen( 0, 1, RGB( 0xb0, 0xb0, 0xb0 ) );
	HPEN pen2 = CreatePen( 0, 1, RGB( 0x80, 0x80, 0x80 ) );
	HGDIOBJ holdpen = SelectObject( dc, pen1 );

	for ( int x = 0; x < cx; x += 16 )
	{
		RECT rc = { x * 16 - 10, 240 + ReservedSpace(), x * 16 + 10, 250 + ReservedSpace() };
		CString str;
		str.Format( TEXT( "%02d" ), x / 16 );

		SelectObject( dc, pen1 );
		MoveToEx( dc, x * 16 - 1, ReservedSpace(), nullptr );
		LineTo( dc, x * 16 - 1, 256 + ReservedSpace() );
		MoveToEx( dc, x * 16 + 1, ReservedSpace(), nullptr );
		LineTo( dc, x * 16 + 1, 256 + ReservedSpace() );
		SelectObject( dc, pen2 );
		MoveToEx( dc, x * 16, ReservedSpace(), nullptr );
		LineTo( dc, x * 16, 256 + ReservedSpace() );

		DrawText( dc, str, str.GetLength(), &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
	}
	SelectObject( dc, holdfn );
	SelectObject( dc, holdpen );

	DeletePen( pen2 );
	DeletePen( pen1 );
}

VOID CNesLevelView::RenderMap()
{
	int cy = LODWORD( m_szMap.cy ), cx = LODWORD( m_szMap.cx );
	HDC hDCTiles = m_bm;
	HDC & hDCSprites = hDCTiles;
	RECT rc = { 0 };
	HDC & dc = hDCTiles;

	int wBkMode = SetBkMode( hDCTiles, TRANSPARENT );
	COLORREF clrBkText = SetTextColor( hDCTiles, RGB( 0xFF, 0, 0 ) );

	for ( int y = 0; y < cy; ++y )
	{
		const std::vector<NES_MAP_METATILE> & vy = m_nmap[ y ];
		for ( int x = 0; x < cx; ++x )
		{
			m_mTiles.DrawMetatile( m_bm, x, y, vy[ x ], ReservedSpace() );
		}
	}

	SetTextColor( hDCTiles, clrBkText );
	SetBkMode( hDCTiles, wBkMode );

	for ( int y = 0; y < 2 * cy + ReservedLogicalSpace(); ++y )
	{
		const std::vector<NES_SPRITE> & vy = m_nsprmap[ y ];
		for ( int x = 0; x < 2 * cx; ++x )
		{
			if ( vy[ x ].attr < 4 )
			{
				m_mTiles.DrawSprite(m_bm, x, y - ReservedLogicalSpace(), vy[ x ], ReservedSpace() );
			}
		}
	}

	for ( const auto & X : m_vGroundBounds )
	{
		int x = 16 * X;

		for ( int i = 1; i < 3; ++i )
		{
			SetRect( &rc, x - i, ReservedSpace(), x + i, 255 + ReservedSpace() );
			DrawFocusRect( dc, &rc );
		}
	}

	DrawPageBorders( dc );
}

VOID CNesLevelView::SelectBg( BYTE bBgCol )
{
	m_mTiles.SelectBg( bBgCol );
}

VOID CNesLevelView::SelectPalette( NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier )
{
	return m_mTiles.SelectPalette( m_bm, nlt, nas, bgModifier );
}

BYTE CNesLevelView::PagesCount()
{
	return LOBYTE( m_szMap.cx / 16 );
}

int CNesLevelView::ReservedSpace()
{
	return 0; //  m_uReservedSpace;
}

int CNesLevelView::ReservedLogicalSpace()
{
	return m_uReservedSpace / 8;
}

VOID CNesLevelView::SetBehindFlag( BOOL fBehind )
{
	m_fBehind = fBehind;
}

BOOL CNesLevelView::IsViewBehindMode()
{
	return m_fBehind;
}