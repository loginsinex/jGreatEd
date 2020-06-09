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

CWinPaneScroller::CWinPaneScroller()
	: m_hWndScroll( NULL )
{
	ZeroMemory( &m_szArea, sizeof( m_szArea ) );
	ZeroMemory( &m_szWindow, sizeof( m_szWindow ) );
	ZeroMemory( &m_scrPos, sizeof( m_scrPos ) );

	// get default height of text for scroll width and height
	RECT rc = { 0 };
	HDC hDC = GetDC( NULL );
	DrawText( hDC, TEXT( "W" ), 1, &rc, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	m_scrChars[ WINPANESCROLL_VERTICAL ].width = rc.right;
	m_scrChars[ WINPANESCROLL_HORIZONTAL ].width = rc.bottom;
	m_scrChars[ WINPANESCROLL_VERTICAL ].page = 5;
	m_scrChars[ WINPANESCROLL_HORIZONTAL ].page = 5;
	ReleaseDC( NULL, hDC );
}

SIZE CWinPaneScroller::GetVisibleArea()
{
	SIZE sz = m_szWindow;	 // client rect does not include title bar, borders, scroll bars, status bar
	return sz;
}

BOOL CWinPaneScroller::IsScrollPresent( int vBar )
{
	switch ( vBar )
	{
		case WINPANESCROLL_VERTICAL: return ( m_szWindow.cy < m_szArea.cy );
		case WINPANESCROLL_HORIZONTAL: return ( m_szWindow.cx < m_szArea.cx );
	}
	return FALSE;
}

void CWinPaneScroller::Assign( HWND hWndScroll )
{
	m_hWndScroll = hWndScroll;
	UpdateScroll();
}

void CWinPaneScroller::ScrollEvent( UINT uMsg, int iScrollEvent )
{
	if ( !m_hWndScroll )
		return;

	int iBar = SB_VERT, vBar = WINPANESCROLL_VERTICAL;
	if ( WM_HSCROLL == uMsg )
	{
		iBar = SB_HORZ; vBar = WINPANESCROLL_HORIZONTAL;
	}
	else if ( WM_VSCROLL == uMsg )
	{
		iBar = SB_VERT; vBar = WINPANESCROLL_VERTICAL;
	}
	else
		return;

	SCROLLINFO si = { 0 };
	si.cbSize = sizeof( si );
	si.fMask = SIF_RANGE | SIF_POS;

	GetScrollInfo( m_hWndScroll, iBar, &si );

	switch ( LOWORD( iScrollEvent ) )
	{
		case SB_LEFT:	// same as SB_TOP
			{
				si.nPos = 0;
				break;
			}
		case SB_RIGHT:	// same as SB_BOTTOM
			{
				si.nPos = si.nMax;
				break;
			}
		case SB_LINELEFT:
			{
				si.nPos--;
				break;
			}
		case SB_LINERIGHT:
			{
				si.nPos++;
				break;
			}
		case SB_PAGELEFT:
			{
				si.nPos -= m_scrChars[ vBar ].page;
				break;
			}
		case SB_PAGERIGHT:
			{
				si.nPos += m_scrChars[ vBar ].page;
				break;
			}
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				si.nPos = HIWORD( iScrollEvent );
				break;
			}
	}

	si.fMask = SIF_POS;
	si.nPos = min( si.nPos, si.nMax );
	si.nPos = max( si.nPos, 0 );

	SetScrollInfo( m_hWndScroll, iBar, &si, TRUE );
}

void CWinPaneScroller::SetScrollArea( int cx, int cy )
{
	RECT rc = { 0 };
	GetClientRect( m_hWndScroll, &rc );

	if ( m_szArea.cx == cx && m_szArea.cy == cy && rc.right == m_szWindow.cx && rc.bottom == m_szWindow.cy )
		return;

	if ( -1 != cx ) m_szArea.cx = cx;
	if ( -1 != cy ) m_szArea.cy = cy;
	UpdateScroll();
}

void CWinPaneScroller::SetScrollChars( int width, int page )
{
	m_scrChars[ WINPANESCROLL_VERTICAL ].width = width;
	m_scrChars[ WINPANESCROLL_HORIZONTAL ].width = width;
	m_scrChars[ WINPANESCROLL_VERTICAL ].page = page;
	m_scrChars[ WINPANESCROLL_HORIZONTAL ].page = page;
	UpdateScroll();
}


void CWinPaneScroller::UpdateScroll()
{
	if ( !m_hWndScroll )
		return;

	RECT rc = { 0 };
	GetClientRect( m_hWndScroll, &rc );
	m_szWindow.cx = rc.right;
	m_szWindow.cy = rc.bottom;

	if ( !m_szArea.cx && !m_szArea.cy )
	{
		m_szArea.cx = m_szWindow.cx;
		m_szArea.cy = m_szWindow.cy;
	}

	// get visible area to set max for hor and vert scrollbars
	SIZE sz = GetVisibleArea();
	int cx = sz.cx / m_scrChars[ WINPANESCROLL_HORIZONTAL ].width;
	int cy = sz.cy / m_scrChars[ WINPANESCROLL_VERTICAL ].width;
	int maxcx = m_szArea.cx / m_scrChars[ WINPANESCROLL_HORIZONTAL ].width;
	int maxcy = m_szArea.cy / m_scrChars[ WINPANESCROLL_VERTICAL ].width;

	SCROLLINFO si = { 0 };
	si.cbSize = sizeof( si );
	si.fMask = SIF_RANGE | SIF_PAGE;

	// set for vertical bar
	si.nMin = 0; si.nMax = ( IsScrollPresent( WINPANESCROLL_VERTICAL ) ? maxcy - cy + m_scrChars[ WINPANESCROLL_VERTICAL ].page : -1 );
	si.nPage = m_scrChars[ WINPANESCROLL_VERTICAL ].page;
	SetScrollInfo( m_hWndScroll, SB_VERT, &si, TRUE );


	si.cbSize = sizeof( si );
	si.fMask = SIF_RANGE | SIF_PAGE;

	// set for horizontal bar
	si.nMin = 0; si.nMax = ( IsScrollPresent( WINPANESCROLL_HORIZONTAL ) ? maxcx - cx + m_scrChars[ WINPANESCROLL_HORIZONTAL ].page : -1 );
	si.nPage = m_scrChars[ WINPANESCROLL_HORIZONTAL ].page;
	SetScrollInfo( m_hWndScroll, SB_HORZ, &si, TRUE );
}

RECT CWinPaneScroller::GetClientArea()
{
	int cx = ( IsScrollPresent( WINPANESCROLL_HORIZONTAL ) ? GetScrollPos( m_hWndScroll, SB_HORZ ) * m_scrChars[ WINPANESCROLL_HORIZONTAL ].width : 0 );
	int cy = ( IsScrollPresent( WINPANESCROLL_VERTICAL ) ? GetScrollPos( m_hWndScroll, SB_VERT ) * m_scrChars[ WINPANESCROLL_VERTICAL ].width : 0 );

	SIZE sz = GetVisibleArea();

	RECT rc = { cx, cy, sz.cx + cx, sz.cy + cy };

	if ( rc.right > m_szArea.cx )
	{
		int idelta = rc.right - m_szArea.cx;
		rc.right -= idelta;
		rc.left -= idelta;
	}

	if ( rc.bottom > m_szArea.cy )
	{
		int idelta = rc.bottom - m_szArea.cy;
		rc.bottom -= idelta;
		rc.top -= idelta;
	}

	return rc;
}

void CWinPaneScroller::ScrollTo( int wcx, int wcy )
{
	int cx = wcx / m_scrChars[ WINPANESCROLL_HORIZONTAL ].width;
	int cy = wcy / m_scrChars[ WINPANESCROLL_VERTICAL ].width;

	struct
	{
		int nMin;
		int nMax;
	} ptX = { 0 }, ptY = { 0 };

	GetScrollRange( m_hWndScroll, SB_HORZ, &ptX.nMin, &ptX.nMax );
	GetScrollRange( m_hWndScroll, SB_VERT, &ptY.nMin, &ptY.nMax );

	cx = max( ptX.nMin, cx ); cx = min( ptX.nMax, cx );
	cy = max( ptY.nMin, cy ); cy = min( ptY.nMax, cy );


	if ( wcx >= 0 )
		SetScrollPos( m_hWndScroll, SB_HORZ, cx, TRUE );

	if ( wcy >= 0 )
		SetScrollPos( m_hWndScroll, SB_VERT, cy, TRUE );
}

