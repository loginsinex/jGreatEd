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

CMenu::CMenu( HINSTANCE hInstance, LPCTSTR pszMenuRes )
	: m_hMenu( LoadMenu( hInstance, pszMenuRes ) )
{

}

CMenu::CMenu( HINSTANCE hInstance, UINT uMenuRes )
	: m_hMenu( LoadMenu( hInstance, MAKEINTRESOURCE( uMenuRes ) ) )
{

}

CMenu::CMenu( HMENU hMenu )
	: m_hMenu( hMenu )
{
}

CMenu::operator HMENU()
{
	return m_hMenu;
}

HMENU CMenu::FindItem( HMENU hMenu, UINT uId, UINT uDeep )
{
	UINT uItems = GetMenuItemCount( hMenu );

	for ( UINT i = 0; i < uItems; ++i )
	{
		HMENU hSubmenu = GetSubMenu( hMenu, i );
		if ( hSubmenu && uDeep < MENU_SEARCH_MAX_DEEP )
		{
			if ( ( hSubmenu = FindItem( hSubmenu, uId, uDeep + 1 ) ) )
			{
				return hSubmenu;
			}
		}
		else
		{
			if ( GetMenuItemID( hMenu, i ) == uId ) return hMenu;
		}
	}

	return nullptr;
}

HMENU CMenu::FindItem( UINT uId )
{
	return FindItem( m_hMenu, uId, 0 );
}

VOID CMenu::EnableItems( BOOL fEnable, ... )
{
	va_list argptr;
	va_start( argptr, fEnable );

	UINT uEnabled = ( fEnable ? MF_ENABLED : ( MF_DISABLED | MF_GRAYED ) );

	do
	{
		int id = va_arg( argptr, int );
		if ( !id )
			break;

		HMENU hSubmenu = FindItem( id );
		if ( hSubmenu )
		{
			EnableMenuItem( hSubmenu, id, MF_BYCOMMAND | uEnabled );
		}
	} while ( true );

	va_end( argptr );
}

UINT CMenu::TrackMenu( SHORT X, SHORT Y )
{
	UINT cx = 0, cy = 0;

	if ( -1 == X && -1 == Y )
	{
		POINT pt = { 0 };
		GetCursorPos( &pt );
		X = SHORT( pt.x & 0xFFFF );
		Y = SHORT( pt.y & 0xFFFF );
	}

	return TrackPopupMenu( m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, X, Y, 0, GetActiveWindow(), nullptr );
}


