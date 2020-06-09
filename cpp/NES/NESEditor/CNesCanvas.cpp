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

#include "h/GUI/Wrapper/Controls.h"
#include "h/NES/NESEditor/NESEditor.h"
#include "h/NES/NESEditor/CNesCanvas.h"

CNesCanvas::CNesCanvas( CREATESTRUCT & cs, HWND hWnd )
	: CCustomControl( cs, hWnd ), m_cs( cs ), m_fEditable( !( cs.style & ES_READONLY ) ), m_uReservedHeight( 0 )
{
	m_scroll.SetScrollChars( 0x10, 0x10 );
	m_scroll.Assign( *this );
	m_capture.fCapture = FALSE;
	m_capture.fFocused = FALSE;
	m_capture.hilite.fVisible = FALSE;
	m_capture.hilite.pt.x = m_capture.hilite.pt.y = 0;
	m_capture.sel.fVisible = FALSE;
	m_capture.sel.fSelRectVisible = FALSE;
	m_capture.sel.fBeginMove = FALSE;
	m_capture.fMouseLeaved = TRUE;
	m_capture.fContextMenu = FALSE;
	CtlSize( 0, 0 );
	m_szMode = smNone;

	m_cursor.mCursor[ cmArrow ] = LoadCursor( nullptr, IDC_ARROW );
	m_cursor.mCursor[ cmHorzSize ] = LoadCursor( nullptr, IDC_SIZEWE );
	m_cursor.mCursor[ cmVertSize ] = LoadCursor( nullptr, IDC_SIZENS );
	m_cursor.mCursor[ cmFullSize ] = LoadCursor( nullptr, IDC_SIZENESW );
	m_cursor.mCursor[ cmHand ] = LoadCursor( nullptr, IDC_HAND );
	m_cursor.mCursor[ cmGround ] = LoadCursor( nullptr, IDC_UPARROW );
	m_cursor.cMode = cmArrow;
	m_cursor.hCursor = m_cursor.mCursor[ cmArrow ];
	m_fDrawLock = FALSE;

	Cursor( cmArrow );
}

CNesCanvas::~CNesCanvas()
{
	for ( const auto & mCurs : m_cursor.mCursor )
	{
		DestroyCursor( mCurs.second );
	}

	m_cursor.mCursor.clear();
	
	m_pInterface = nullptr;
}

VOID CNesCanvas::Register( HINSTANCE hInstance )
{
	CCustomControl::Register( WC_NESCANVAS, hInstance, CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, NULL, LoadCursor( NULL, IDC_ARROW ), NULL );
}

const CREATESTRUCT CNesCanvas::CtlData()
{
	return m_cs;
}

BOOL CNesCanvas::OnCreate( LPCREATESTRUCT lpcs )
{
	SetTimer( *this, 0x10, NES_COINS_COL_UPDATE_TIMER, nullptr );
	return InitBuffer();
}

SIZE CNesCanvas::CtlViewSize()
{
	SIZE sz;
	sz.cx = max( CtlData().cx, m_szControl.cx );
	sz.cy = max( CtlData().cy, m_szControl.cy );
	return sz;
}

BOOL CNesCanvas::CtlSize( int cx, int cy )
{
	m_szControl.cx = max( cx, 1 );
	m_szControl.cy = max( cy, 1 );
	m_scroll.SetScrollArea( m_szControl.cx, m_szControl.cy );
	return InitBuffer();
}

BOOL CNesCanvas::InitBuffer()
{
	SIZE szCurrent = { m_bm.Width(), m_bm.Height() }, sz = szCurrent;
	SIZE szNew = CtlViewSize();

	while ( sz.cx < szNew.cx )
		sz.cx += 0x100;

	while ( sz.cy < szNew.cy )
		sz.cy += 0x100;

	if ( sz.cx != szCurrent.cx || sz.cy != szCurrent.cy )
	{
		m_bm.Create( sz.cx, sz.cy );
		RenderView();
		return TRUE;
	}

	return FALSE;
}

VOID CNesCanvas::OnSize( INT dwState, USHORT cx, USHORT cy )
{
	m_cs.cx = cx;
	m_cs.cy = cy;

	m_scroll.UpdateScroll();
	InitBuffer();
	InvalidateRect( *this, nullptr, TRUE );
}

VOID CNesCanvas::OnSizing( INT dwState, USHORT cx, USHORT cy )
{
	OnSize( dwState, cx, cy );
}

VOID CNesCanvas::OnPaint( HDC hDC, BOOL fErase, PRECT prc )
{
	OnEraseBg( hDC );
}

VOID CNesCanvas::OnEraseBg( HDC hDC )
{
	RECT rc = { 0, 0, CtlData().cx , CtlData().cy };
	SIZE sz = CtlViewSize();
	RECT rcScroll = m_scroll.GetClientArea();
	int left = sz.cx <= rc.right ? 0 : rcScroll.left,
		top = sz.cy <= rc.bottom ? 0 : rcScroll.top;
	const int uReservedHeight = ReservedHeight();

	if ( IsDrawLocked() )
	{
		return;
	}

	int ret = BitBlt( hDC, 0, 0, sz.cx, sz.cy,
		m_bm,
		left, top,
		SRCCOPY );

	if ( !ret )
	{
		MessageBeep( 0 );

	}

	if ( m_capture.sel.fVisible && m_capture.fFocused )
	{
		for ( const auto & rcObj : m_capture.sel.vrcSel )
		{
			RECT rc = rcObj;
			SetRect( &rc, rc.left - left, rc.top - top + uReservedHeight, rc.right - left, rc.bottom - top + uReservedHeight );

			DrawFocusedObjectRect( hDC, rc );
		}
	}

	if ( m_capture.sel.fSelRectVisible )
	{
		const RECT rcSel = NormalizeRect( m_capture.sel.rcSel );
		RECT rc;
		SetRect( &rc, rcSel.left * 16 - left, rcSel.top * 16 - top + uReservedHeight, rcSel.right * 16 - left, rcSel.bottom * 16 - top + uReservedHeight );
		DrawFocusRect( hDC, &rc );
	}
}

VOID CNesCanvas::OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl )
{
	m_scroll.ScrollEvent( vScrollType, MAKELONG( uRequest, uPos ) );
	InvalidateRect( *this, NULL, TRUE );
}

VOID CNesCanvas::OnMouseButton( MOUSEACTION ma, MOUSEBUTTON mb, UINT vKey, SHORT rx, SHORT ry )
{
	RECT rc = { 0, 0, CtlData().cx , CtlData().cy };
	SIZE sz = CtlViewSize();
	RECT rcScroll = m_scroll.GetClientArea();
	int left = ( ( sz.cx <= rc.right ) ? 0 : rcScroll.left ),
		top = ( ( sz.cy <= rc.bottom ) ? 0 : rcScroll.top );

	rx += left;
	ry += top - ReservedHeight();

	int x = rx / 16;
	int y = ry / 16;

	switch ( ma )
	{
		case maDown:
			{
				switch ( mb )
				{
					case mbLeft:
					case mbRight:
						{
							if ( mbLeft == mb )
							{
								m_capture.fCapture = TRUE;
								SetCapture( *this );
							}

							SetFocus( *this );
							m_capture.fFocused = TRUE;
							m_capture.ptCatch.x = x;
							m_capture.ptCatch.y = y;

							if ( rx < m_szControl.cx && ry < m_szControl.cy && m_fEditable )
							{
								if ( mbRight == mb )
								{
									MouseCatchObject( x, y, TRUE );
									MouseRightClick( x, y );
								}
								else
								{
									MouseCatchObject( x, y, FALSE );
								}
							}
							break;
						}
				}
				break;
			}
		case maDblClick:
			{
				switch ( mb )
				{
					case mbLeft:
						{
							if ( m_fEditable )
							{
								MouseDoubleClick( x, y );
							}
							break;
						}
				}
				break;
			}
		case maUp:
			{
				switch ( mb )
				{
					case mbLeft:
						{
							if ( m_capture.fCapture )
							{
								m_capture.fCapture = FALSE;
								ReleaseCapture();

								if ( m_fEditable )
								{
									MouseFreeObject( x, y );
								}
								break;
							}
						}
				}
				break;
			}
		case maMove:
			{
				if ( m_fEditable )
				{
					if ( !m_capture.fCapture )
					{
						if ( rx < m_szControl.cx && ry < m_szControl.cy )
						{
							TRACKMOUSEEVENT tme = { 0 };
							tme.cbSize = sizeof( tme );
							tme.dwFlags = TME_LEAVE;
							tme.dwHoverTime = 10;
							tme.hwndTrack = *this;
							_TrackMouseEvent( &tme );

							MouseMove( rx, ry );
							m_capture.ptMove.x = x;
							m_capture.ptMove.y = y;
						}
						else
						{
							MouseLeave();
						}
					}
					else
					{
						MouseMoveObject( x, y );
						m_capture.ptCatch.x = x;
						m_capture.ptCatch.y = y;
					}
				}
				break;
			}
	}
}

VOID CNesCanvas::OnGotFocus( HWND hLostFocus )
{
	m_capture.fFocused = TRUE;
	InvalidateRect( *this, nullptr, TRUE );
}

VOID CNesCanvas::OnLostFocus( HWND hGotFocus )
{
	m_capture.fFocused = FALSE;
	InvalidateRect( *this, nullptr, TRUE );
}

LRESULT CNesCanvas::OnDlgCode( DWORD vKey, LPMSG lpmsg )
{
	return DLGC_WANTARROWS;
}

VOID CNesCanvas::OnTimer( DWORD_PTR idTimer, FARPROC pTimerProc )
{
	if ( m_pInterface )
	{
		m_pInterface->UpdateCoinColor();
		m_pInterface->Refresh( m_bm );
		InvalidateRect( *this, nullptr, TRUE );
	}
}

VOID CNesCanvas::OnKeyEvent( DWORD vEvent, DWORD vKey, DWORD vScanCode )
{
	if ( WM_KEYDOWN == vEvent && m_capture.fFocused )
	{
		switch ( vKey )
		{
			case VK_LEFT: OnScroll( WM_HSCROLL, 0, SB_LINELEFT, nullptr ); break;
			case VK_RIGHT: OnScroll( WM_HSCROLL, 0, SB_LINERIGHT, nullptr ); break;
			case VK_UP: OnScroll( WM_VSCROLL, 0, SB_LINEUP, nullptr ); break;
			case VK_DOWN: OnScroll( WM_VSCROLL, 0, SB_LINEDOWN, nullptr ); break;
			case VK_HOME:
				{
					OnScroll( WM_HSCROLL, 0, SB_LEFT, nullptr );
					OnScroll( WM_VSCROLL, 0, SB_TOP, nullptr );
					break;
				}
			case VK_END: OnScroll( WM_HSCROLL, 0, SB_RIGHT, nullptr ); break;
			case VK_PRIOR: OnScroll( WM_HSCROLL, 0, SB_PAGELEFT, nullptr ); break;
			case VK_NEXT: OnScroll( WM_HSCROLL, 0, SB_PAGERIGHT, nullptr ); break;
		}
	}
}

LRESULT CNesCanvas::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & fHandle )
{
	switch ( uMsg )
	{
		case WM_MOUSELEAVE:
			{
				MouseLeave();
				break;
			}
		case WM_SETCURSOR:
			{
				fHandle = TRUE;
				UpdateCursor();
				return TRUE;
			}
		case WM_CONTEXTMENU:
			{
				SHORT x = GET_X_LPARAM( lParam );
				SHORT y = GET_Y_LPARAM( lParam );
				SetFocus( *this );
				if ( !m_capture.fContextMenu )
				{
					m_capture.fContextMenu = TRUE;
					if ( -1 == x && -1 == y )
					{
						POINT pt = { 0, 0 };
						ClientToScreen( *this, &pt );
						x = LOWORD( pt.x );
						y = LOWORD( pt.y );
					}
					OnMouseButton( maDown, mbRight, 0, x, y );
					OnMouseButton( maUp, mbRight, 0, x, y );
					m_capture.fContextMenu = FALSE;
				}
				break;
			}
		case WM_ENABLE:
			{
				if ( !wParam )
				{
					MouseLeave();
					m_bm.GrayScale();
				}
				else
				{
					RenderView();
				}
				InvalidateRect( *this, nullptr, TRUE );
				break;
			}
	}

	return 0;
}