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

#define			WINPANESCROLL_PAGE			10
#define			WINPANESCROLL_VERTICAL		0
#define			WINPANESCROLL_HORIZONTAL	1

class CWinPaneScroller
{
	HWND				m_hWndScroll;
	SIZE				m_szArea;
	SIZE				m_szWindow;
	POINT				m_scrPos;
	struct
	{
		int				width;
		int				page;
	} m_scrChars[ 2 ];

	SIZE				GetVisibleArea();
	BOOL				IsScrollPresent( int vBar );

public:
	CWinPaneScroller();
	void				Assign( HWND hWndScroll );
	void				ScrollEvent( UINT uMsg, int iScrollEvent );
	void				SetScrollArea( int cx, int cy );
	void				UpdateScroll();
	void				SetScrollChars( int width, int page );
	RECT				GetClientArea();
	void				ScrollTo( int cx, int cy );
};