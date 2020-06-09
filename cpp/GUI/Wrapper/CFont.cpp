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

#include "Controls.h"

CFont::CFont(int nHeight,               // height of font
		  int nWidth,                // average character width
		  int nEscapement,           // angle of escapement
		  int nOrientation,          // base-line orientation angle
		  int fnWeight,              // font weight
		  DWORD fdwItalic,           // italic attribute option
		  DWORD fdwUnderline,        // underline attribute option
		  DWORD fdwStrikeOut,        // strikeout attribute option
		  DWORD fdwCharSet,          // character set identifier
		  DWORD fdwOutputPrecision,  // output precision
		  DWORD fdwClipPrecision,    // clipping precision
		  DWORD fdwQuality,          // output quality
		  DWORD fdwPitchAndFamily,   // pitch and family
		  LPCTSTR lpszFace           // typeface 
		  )
		  :
	m_hFont( CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
		fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily,
		lpszFace) )
{

}

CFont::CFont()
	: m_hFont( NULL )
{
	NONCLIENTMETRICS ncm = { 0 };
	ncm.cbSize = sizeof(ncm);
	while( !SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0) )
		ncm.cbSize -= sizeof(WORD);

	m_hFont = CreateFontIndirect(&ncm.lfMenuFont);
}

HFONT CFont::Create(
		  int nHeight,               // height of font
		  int nWidth,                // average character width
		  int nEscapement,           // angle of escapement
		  int nOrientation,          // base-line orientation angle
		  int fnWeight,              // font weight
		  DWORD fdwItalic,           // italic attribute option
		  DWORD fdwUnderline,        // underline attribute option
		  DWORD fdwStrikeOut,        // strikeout attribute option
		  DWORD fdwCharSet,          // character set identifier
		  DWORD fdwOutputPrecision,  // output precision
		  DWORD fdwClipPrecision,    // clipping precision
		  DWORD fdwQuality,          // output quality
		  DWORD fdwPitchAndFamily,   // pitch and family
		  LPCTSTR lpszFace           // typeface 
		  )
{
	if ( m_hFont )
		DeleteObject( m_hFont );

	m_hFont = CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
		fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily,
		lpszFace);

	return m_hFont;
}

CFont::operator HFONT()
{
	return m_hFont;
}

CFont::~CFont()
{
	if ( m_hFont )
		DeleteObject( m_hFont );

	m_hFont = NULL;
}

HFONT CFont::Create(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality)
{
	if ( m_hFont ) return m_hFont;
	m_hFont = CreateFont(iHeight, 0, 0, 0, ( bBold ? FW_BLACK : 0 ), bItalic, bUnderline, bStriked, ANSI_CHARSET, 0, 0, fQuality, 0, lpszFontFace);
	return m_hFont;
}

HFONT CFont::operator=(HFONT hFont)
{
	if ( m_hFont )
		DeleteFont( m_hFont );

	HDC hDesktopDC = GetDC( NULL );
	HDC hDC = CreateCompatibleDC( hDesktopDC );
	HBITMAP hBmp = CreateCompatibleBitmap( hDesktopDC, 1, 1 );
	HGDIOBJ holdBm = SelectObject( hDC, hBmp );
	HGDIOBJ holdFn = SelectObject( hDC, hFont );

	TEXTMETRIC tm = { 0 };
	GetTextMetrics(hDC, &tm);

	TCHAR pszFontFace[MAX_PATH+1];
	GetTextFace(hDC, MAX_PATH, pszFontFace);

	m_hFont = CreateFont(tm.tmHeight, tm.tmMaxCharWidth, tm.tmInternalLeading, 0, tm.tmWeight, tm.tmItalic, tm.tmUnderlined, tm.tmStruckOut, tm.tmCharSet, 
		0, 0, DEFAULT_QUALITY, tm.tmPitchAndFamily, pszFontFace);

	SelectObject(hDC, holdFn);
	SelectObject(hDC, holdBm);
	DeleteBitmap( hBmp );
	DeleteDC( hDC );
	ReleaseDC( NULL, hDesktopDC );

	return m_hFont;
}

void CFont::LoadFromWindow(HWND hWndFrom, INT iHeight, INT bBold, INT bItalic, INT bStriked, INT bUnderline, LPTSTR lpszFontFace, UINT fQuality)
{
	if ( !IsWindow(hWndFrom) ) return;

	if ( m_hFont )
		DeleteFont( m_hFont );

	m_hFont = NULL;

	HDC hWindowDC = GetWindowDC(hWndFrom);
	HGDIOBJ holdFn = SelectObject(hWindowDC, (HFONT) SendMessage(hWndFrom, WM_GETFONT, 0, 0));
	TEXTMETRIC tm = { 0 };
	GetTextMetrics(hWindowDC, &tm);
	
	TCHAR pszFontFace[MAX_PATH+1];
	GetTextFace(hWindowDC, MAX_PATH, pszFontFace);

	if ( -1 == iHeight ) iHeight = tm.tmHeight;
	if ( -1 == bBold ) bBold = ( 0 != tm.tmWeight );
	if ( -1 == bItalic ) bItalic = tm.tmItalic;
	if ( -1 == bStriked ) bStriked = tm.tmStruckOut;
	if ( -1 == bUnderline ) bUnderline = tm.tmUnderlined;
	if ( -1 == fQuality ) fQuality = 0;
	if ( NULL == lpszFontFace ) lpszFontFace = pszFontFace;

	SelectObject(hWindowDC, holdFn);
	ReleaseDC(hWndFrom, hWindowDC);

	Create(iHeight, bBold, bItalic, bStriked, bUnderline, lpszFontFace, fQuality);
}

