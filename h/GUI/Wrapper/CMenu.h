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

#define MENU_SEARCH_MAX_DEEP		16

class CMenu
{
	HMENU			m_hMenu;

	HMENU			FindItem( UINT uId );
	HMENU			FindItem( HMENU hMenu, UINT uId, UINT uDeep );

public:
	CMenu( HINSTANCE hInstance, LPCTSTR pszMenuRes );
	CMenu( HINSTANCE hInstance, UINT uMenuRes );
	CMenu( HMENU hMenu );

	operator HMENU();

	VOID	EnableItems( BOOL fEnable, ... );
	UINT	TrackMenu( SHORT x = -1, SHORT y = -1 );
};