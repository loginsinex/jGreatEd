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

class CFont
{
	HFONT				m_hFont;

public:
	CFont(int nHeight,               // height of font
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
		  );

	CFont();
	HFONT				Create(
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
		  );

	HFONT			Create(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality);

	void LoadFromWindow(HWND hWndFrom, INT iHeight = -1, INT bBold = -1, INT bItalic = -1, INT bStriked = -1, INT bUnderline = -1, LPTSTR lpszFontFace = NULL, UINT fQuality = -1);

	operator HFONT();
	HFONT operator=(HFONT);
	~CFont();
};