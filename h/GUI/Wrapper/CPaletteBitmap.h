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

class CPaletteBitmap
{
	struct
	{
		BOOL						fInit;
		SIZE						sz;
		HDC							hDesktopDC;
		HDC							hDC;
		HBITMAP						hBm;
		HGDIOBJ						hGdiObj[ GDI_OBJ_LAST ];
		PVOID						pvBits;
		std::vector<RGBQUAD>		vEntry;
		std::vector<PALETTEENTRY>	vSysEntry;
	} m_bm;

	VOID							InitBitmap();
	VOID							InitPalette();
	HBITMAP							CreateBm( int cx, int cy );
	VOID							DumpObjects();
	VOID							RevertObjects();
	VOID							ApplyPalette();

	VOID							InitSysEntry();

	HDC								CheckDC();
	VOID							ResetBitmapDC();

public:
	CPaletteBitmap();
	~CPaletteBitmap();
	CPaletteBitmap( int cx, int cy );

	VOID				Create( int cx, int cy );
	VOID				Destroy();
	int					Width() const;
	int					Height() const;
	BYTE *				Bits() const;
	operator HDC();

	VOID				Entry( size_t index, COLORREF clr );
	VOID				Entry( size_t index, const RGBQUAD & rgb );
	VOID				Entries( const std::vector<RGBQUAD> & vEntries, size_t nBegin = 0 );
	VOID				Entries( const std::vector<COLORREF> & vEntries, size_t nBegin = 0 );
	size_t				EntriesCount() const;
	COLORREF			Entry( size_t index ) const;

	VOID				FillRect( int x, int y, int cx, int cy, size_t index );
};