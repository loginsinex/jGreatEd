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

CStatusBar::CStatusBar( HINSTANCE hInstance, USHORT uId, BOOL fSizeGrip, BOOL fTooltips, BOOL fComposited )
	: CControl
	( hInstance, uId, STATUSCLASSNAME, NULL,
		0, 0, 0, 0, 
		WS_VISIBLE | 
		( fSizeGrip ? SBARS_SIZEGRIP : 0 ) | 
		( fTooltips ? ( SBT_TOOLTIPS | SBARS_TOOLTIPS ) : 0 ),
		( fComposited ? WS_EX_COMPOSITED : 0 )
	)
{
	AutoSizedControl();
}

CStatusBar::CStatusBar( HINSTANCE hInstance, USHORT uId, tstring pszText, BOOL fSizeGrip, BOOL fTooltips, BOOL fComposited )
	: CControl
	( hInstance, uId, STATUSCLASSNAME, pszText.c_str(),
		0, 0, 0, 0, 
		WS_VISIBLE | 
		( fSizeGrip ? SBARS_SIZEGRIP : 0 ) | 
		( fTooltips ? ( SBT_TOOLTIPS | SBARS_TOOLTIPS ) : 0 ),
		( fComposited ? WS_EX_COMPOSITED : 0 )
	)
{
	AutoSizedControl();
}

VOID CStatusBar::SetParts( int iPartSize, ... )
{
	va_list argptr;
	va_start( argptr, iPartSize );

	std::vector<int> vParts;
	vParts.push_back( iPartSize );

	int iSize = 0;
	while( iSize = va_arg( argptr, int ) )
	{
		vParts.push_back( iSize );
	};

	va_end( argptr );

	SetPartsV( vParts );
}

VOID CStatusBar::SetPartsV( const std::vector<int>& viPartSize )
{
	std::vector<int> vSize;
	int iSize = 0;
	for ( auto v = viPartSize.begin(); v < viPartSize.end(); ++v )
	{
		iSize += *v;
		vSize.push_back( iSize );
	}

	vSize.push_back( -1 );
	cSendMessage( SB_SETPARTS, vSize.size(), (LPARAM)vSize.data() );
}

VOID CStatusBar::SetText( BYTE iPartId, LPCTSTR pszText )
{
	USHORT uStyle = HIWORD( cSendMessage( SB_GETTEXTLENGTH, iPartId ) );
	cSendMessage( SB_SETTEXT, iPartId | uStyle, (LPARAM)pszText );
}

VOID CStatusBar::SetTexts( LPCTSTR pszText, ... )
{
	va_list argptr;
	va_start( argptr, pszText );
	int iPart = 0;
	SetText( iPart++, pszText );

	LPCTSTR psz = 0;
	while ( psz = va_arg( argptr, LPCTSTR ) )
	{
		SetText( iPart++, psz );
	};

	va_end( argptr );
}

VOID CStatusBar::SetPartStyle( BYTE iPartId, STB_PART_STYLE iStyle )
{
	USHORT uTextLen = LOWORD( cSendMessage( SB_GETTEXTLENGTH, iPartId ) );
	std::vector<TCHAR> vsText( uTextLen + 1, 0 );

	cSendMessage( SB_GETTEXT, iPartId, (LPARAM)vsText.data() );
	USHORT uStyle = 0;
	switch ( iStyle )
	{
		case stsNone:		uStyle = 0;					break;
		case stsNoBorders:	uStyle = SBT_NOBORDERS;		break;
		case stsOwnerDraw:	uStyle = SBT_OWNERDRAW;		break;
		case stsPopout:		uStyle = SBT_POPOUT;		break;
		case stsRTL:		uStyle = SBT_RTLREADING;	break;
		case stsTooltips:	uStyle = SBT_TOOLTIPS;		break;
		case stsNoTabParse:	uStyle = SBT_NOTABPARSING;	break;
	}

	cSendMessage( SB_SETTEXT, iPartId | uStyle, (LPARAM)vsText.data() );
}