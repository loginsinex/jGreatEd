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

#include "stdafx.h"

HDC	__hDesktopDC = nullptr;
int __alloc_counter = 0;

HDC __alloc_dc()
{
	if ( !__hDesktopDC )
	{
		__hDesktopDC = GetDC( GetDesktopWindow() );
	}

	__alloc_counter++;
	return __hDesktopDC;
}

VOID __release_dc()
{
	__alloc_counter--;
	if ( __alloc_counter <= 0 )
	{
		__alloc_counter = 0;
		if ( __hDesktopDC )
		{
			ReleaseDC( GetDesktopWindow(), __hDesktopDC );
			__hDesktopDC = nullptr;
		}
	}
}


CPaletteBitmap::CPaletteBitmap()
{
	InitBitmap();
}

CPaletteBitmap::CPaletteBitmap( int cx, int cy )
{
	InitBitmap();
	Create( cx, cy );
}

CPaletteBitmap::~CPaletteBitmap()
{
	Destroy();
	__release_dc();
}

VOID CPaletteBitmap::ApplyPalette()
{
	UINT ret = SetDIBColorTable( m_bm.hDC, 0, LODWORD( m_bm.vEntry.size() ), m_bm.vEntry.data() );
	if ( !ret )
	{
		_tprintf( TEXT( "SetDIBColorTable failed: %d\n" ), GetLastError() );
	}
}

VOID CPaletteBitmap::InitPalette()
{
	if ( m_bm.vSysEntry.size() > 0 )
	{
		size_t cCount = min( m_bm.vSysEntry.size(), 256 );
		for ( size_t n = 0; n < cCount; ++n )
		{
			PALETTEENTRY & pe = m_bm.vSysEntry[ n ];
			m_bm.vEntry[ n ] = { pe.peRed, pe.peGreen, pe.peBlue, 0 };
		}
	}
	else
	{
		m_bm.vEntry.insert( m_bm.vEntry.end(), 256, { 0 } );
	}
	
	ApplyPalette();
}

VOID CPaletteBitmap::InitSysEntry()
{
	UINT cCount = GetSystemPaletteEntries( m_bm.hDC, 0, 256, nullptr );
	if ( cCount > 0 )
	{
		m_bm.vSysEntry.clear();
		m_bm.vSysEntry.insert( m_bm.vSysEntry.begin(), cCount, { 0 } );
		GetSystemPaletteEntries( m_bm.hDC, 0, cCount, m_bm.vSysEntry.data() );
	}
}

HBITMAP CPaletteBitmap::CreateBm( int cx, int cy )
{
	std::vector<BYTE> vbmi( sizeof( BITMAPINFOHEADER ) + sizeof( RGBQUAD ) * EntriesCount() );

	PBITMAPINFO pbmi = (PBITMAPINFO)vbmi.data();
	BITMAPINFOHEADER & bmih = pbmi->bmiHeader;

	bmih.biSize = sizeof( bmih );
	bmih.biWidth = cx;
	bmih.biHeight = -cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = sizeof( RGBQUAD ) * cx * cy;
	bmih.biClrUsed = LODWORD( EntriesCount() );
	m_bm.sz.cx = cx;
	m_bm.sz.cy = cy;

	for ( size_t i = 0; i < bmih.biClrUsed; ++i )
	{
		RGBQUAD & pe = m_bm.vEntry[ i ];
		pbmi->bmiColors[ i ] = pe;
	}

	return CreateDIBSection( m_bm.hDesktopDC, pbmi, DIB_RGB_COLORS, (PVOID*)&m_bm.pvBits, NULL, 0 );
}

VOID CPaletteBitmap::Create( int cx, int cy )
{
	if ( cx <= 0 || cy <= 0 )
	{
		return;
	}

	Destroy();

	m_bm.fInit = TRUE;
	m_bm.hDesktopDC = __alloc_dc();
	m_bm.hDC = CreateCompatibleDC( m_bm.hDesktopDC );
	DumpObjects();
	InitSysEntry();
	m_bm.hBm = CreateBm( cx, cy );
	SelectObject( m_bm.hDC, m_bm.hBm );
	InitPalette();
	__release_dc();

	if ( !m_bm.hBm )
	{
		Destroy();
	}

	GdiFlush();
}

VOID CPaletteBitmap::Destroy()
{
	if ( m_bm.fInit )
	{
		RevertObjects();
		DeleteBitmap( m_bm.hBm );
		DeleteDC( m_bm.hDC );
		InitBitmap();
	}
}

int CPaletteBitmap::Width() const
{
	return m_bm.sz.cx;
}

int CPaletteBitmap::Height() const
{
	return m_bm.sz.cy;
}

VOID CPaletteBitmap::InitBitmap()
{
	m_bm.fInit = FALSE;
	m_bm.hBm = nullptr;
	m_bm.hDC = nullptr;
	m_bm.hDesktopDC = nullptr;
	ZeroMemory( m_bm.hGdiObj, GDI_OBJ_LAST * sizeof( m_bm.hGdiObj[ 0 ] ) );
	m_bm.pvBits = nullptr;
	m_bm.sz.cx = m_bm.sz.cy = 0;
	m_bm.vEntry.clear();
}

VOID CPaletteBitmap::DumpObjects()
{
	DWORD iObjType[] = 
	{
		OBJ_BITMAP,		// Returns the current selected bitmap.
		OBJ_BRUSH,		// Returns the current selected brush.
		OBJ_COLORSPACE,	// Returns the current color space.
		OBJ_FONT,		// Returns the current selected font.
		OBJ_PAL,		// Returns the current selected palette.
		OBJ_PEN			// Returns the current selected pen.
	};

	for ( const auto & objType : iObjType )
	{
		m_bm.hGdiObj[ objType ] = GetCurrentObject( m_bm.hDC, objType );
	}
}

VOID CPaletteBitmap::RevertObjects()
{
	for ( DWORD i = 0; i < GDI_OBJ_LAST; ++i )
	{
		if ( m_bm.hGdiObj[ i ] )
		{
			SelectObject( m_bm.hDC, m_bm.hGdiObj[ i ] );
			m_bm.hGdiObj[ i ] = nullptr;
		}
	}
}

BYTE * CPaletteBitmap::Bits() const
{
	return (PBYTE) m_bm.pvBits;
}

HDC CPaletteBitmap::CheckDC()
{
	if ( !GetBkMode( m_bm.hDC ) )
	{
		if ( ERROR_INVALID_PARAMETER == GetLastError() )
		{
			ResetBitmapDC();
		}
	}

	return m_bm.hDC;
}

VOID CPaletteBitmap::ResetBitmapDC()
{
	if ( m_bm.fInit )
	{
		RevertObjects();
		DeleteDC( m_bm.hDC );
		m_bm.hDC = CreateCompatibleDC( m_bm.hDesktopDC );

		SelectObject( m_bm.hDC, m_bm.hBm );
	}
}


CPaletteBitmap::operator HDC()
{
	return CheckDC();
}

size_t CPaletteBitmap::EntriesCount() const
{
	return m_bm.vEntry.size();
}

VOID CPaletteBitmap::Entry( size_t index, COLORREF clr )
{
	RGBQUAD rgbq = { GetBValue( clr ), GetGValue( clr ), GetRValue( clr ), 0xff };
	Entry( index, rgbq );
}

VOID CPaletteBitmap::Entry( size_t index, const RGBQUAD & rgb )
{
	if ( index < EntriesCount() )
	{
		m_bm.vEntry[ index ] = rgb;
		ApplyPalette();
	}
}

VOID CPaletteBitmap::Entries( const std::vector<COLORREF>& vEntries, size_t nBegin )
{
	size_t c = min( vEntries.size(), 256 );
	std::vector<RGBQUAD> vRGB( vEntries.size(), { 0 } );
	for ( size_t n = 0; n < c; ++n )
	{
		const COLORREF & clr = vEntries[ n ];
		vRGB[ n ] = { GetBValue( clr ), GetGValue( clr ), GetRValue( clr ), 0xff };
	}

	Entries( vRGB, nBegin );
}

VOID CPaletteBitmap::Entries( const std::vector<RGBQUAD>& vEntries, size_t nBegin )
{
	size_t nStart = nBegin, nEnd = min( nBegin + vEntries.size(), 256 );
	if ( m_bm.vEntry.empty() )
	{
		m_bm.vEntry.insert( m_bm.vEntry.end(), 256, { 0 } );	// why we've empty palette here???
//		__debugbreak();
	}

	for ( size_t i = nBegin; i < nEnd; ++i )
	{
		m_bm.vEntry[ i ] = vEntries[ i - nBegin ];
	}

	if ( nStart < nEnd )
	{
		ApplyPalette();
	}
}

COLORREF CPaletteBitmap::Entry( size_t index ) const
{
	if ( index < EntriesCount() )
	{
		const RGBQUAD & rgb = m_bm.vEntry[ index ];
		return RGB( rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue );
	}

	return 0xFFFFFFFF;
}

VOID CPaletteBitmap::FillRect( int x, int y, int cx, int cy, size_t index )
{
	RECT rcFill = { x, y, x + cx, y + cy };
	RECT rcSrc = { 0, 0, m_bm.sz.cx, m_bm.sz.cy };
	RECT rc;
	if ( !m_bm.fInit || index >= EntriesCount() || !IntersectRect( &rc, &rcSrc, &rcFill ) )
	{
		return;
	}

	const LONG & im_x = rc.left;
	const LONG & im_y = rc.top;
	const LONG & im_r = rc.right;
	const LONG & im_b = rc.bottom;
	const BYTE col = LOBYTE( index );
	const size_t w = im_r - im_x + 1;
	PBYTE pBitsY = Bits() + im_y * Width() + im_x;

	for ( LONG y = im_y; y <= im_b; ++y )
	{
		memset( pBitsY, col, w );
		pBitsY += Width();
	}
}