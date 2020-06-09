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

typedef enum _tagSTB_PART_STYLE
{
	stsNone,		// The text is drawn with a border to appear lower than the plane of the window.
	stsNoBorders,	// The text is drawn without borders.
	stsOwnerDraw,	// The text is drawn by the parent window.
	stsPopout,		// The text is drawn with a border to appear higher than the plane of the window.
	stsRTL,			// The text will be displayed in the opposite direction to the text in the parent window
	stsTooltips,	// Tooltips
	stsNoTabParse
} STB_PART_STYLE, *PSTB_PART_STYLE;

class CStatusBar : public CControl
{
public:
	CStatusBar( HINSTANCE hInstance, USHORT uId, BOOL fSizeGrip = TRUE, BOOL fTooltips = FALSE, BOOL fComposited = FALSE );
	CStatusBar( HINSTANCE hInstance, USHORT uId, tstring sText, BOOL fSizeGrip = TRUE, BOOL fTooltips = FALSE, BOOL fComposited = FALSE );

	VOID		SetParts( int iPartSize, ... );
	VOID		SetPartsV( const std::vector<int> & viPartSize );
	VOID		SetText( BYTE iPartId, LPCTSTR pszText );
	VOID		SetTexts( LPCTSTR pszText, ... );
	VOID		SetPartStyle( BYTE iPartId, STB_PART_STYLE iStyle );
};