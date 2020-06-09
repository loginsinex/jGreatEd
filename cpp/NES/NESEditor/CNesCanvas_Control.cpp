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

VOID CNesCanvas::Cursor( CURSOR_MODE cm )
{
	const auto & itCursor = m_cursor.mCursor.find( cm );
	if ( itCursor != m_cursor.mCursor.end() )
	{
		if ( m_cursor.cMode != cm )
		{
			m_cursor.hCursor = itCursor->second;
			m_cursor.cMode = cm;
		}
	}
	
	if ( GetCursor() != m_cursor.hCursor )
		SetCursor( m_cursor.hCursor );
}

VOID CNesCanvas::UpdateCursor()
{
	switch ( m_szMode )
	{
		case smNone: Cursor( cmArrow ); break;
		case smGroundBound: Cursor( cmGround ); break;
		case smLeft:
		case smRight: Cursor( cmHorzSize ); break;
		case smTop:
		case smBottom: Cursor( cmVertSize ); break;
		case smHand: Cursor( cmHand ); break;
		case smCross: Cursor( cmFullSize ); break;
	}	
}

VOID CNesCanvas::SetMode( SIZE_MODE szMode )
{
	switch ( szMode )
	{
		case smLeft:
		case smRight:
		case smTop:
		case smBottom: m_szMode = szMode; break;
		case smGroundBound:
		case smHand:
		case smCross:
			{
				if ( !IsInSizeMode() )
				{
					m_szMode = szMode;
				}
				break;
			}
		default: break;
	}

	UpdateCursor();
}

BOOL CNesCanvas::IsInSizeMode()
{
	switch ( m_szMode )
	{
		case smLeft:
		case smRight:
		case smTop:
		case smBottom:
			{
				return TRUE;
			}
		default:
			{
				return FALSE;
			}
	}
}

BOOL CNesCanvas::IsInGroundMode()
{
	switch ( m_szMode )
	{
		case smGroundBound:
			{
				return TRUE;
			}
		default:
			{
				return FALSE;
			}
	}
}

VOID CNesCanvas::BeginMove()
{
	m_capture.sel.fBeginMove = TRUE;
}

BOOL CNesCanvas::BeginMoveNotify()
{
	if ( m_capture.sel.fBeginMove )
	{
		m_capture.sel.fBeginMove = FALSE;
		NotifyBeginMove( nullptr );
		return TRUE;
	}

	return FALSE;
}

VOID CNesCanvas::LeaveHiliteMode()
{
	switch ( m_szMode )
	{
		case smHand:
			{
				_tprintf( TEXT( "Leaving hilite mode\n" ) );
				m_szMode = smNone;
				break;
			}
		default: break;
	}

	UpdateCursor();
}

VOID CNesCanvas::LeaveSizeMode()
{
	switch ( m_szMode )
	{
		case smLeft:
		case smRight:
		case smBottom:
		case smTop:
			{
				_tprintf( TEXT( "Leaved size mode\n" ) );
				m_szMode = smNone;
				break;
			}
		default: break;
	}

	UpdateCursor();
}

VOID CNesCanvas::LeaveGroundBoundMode()
{
	switch ( m_szMode )
	{
		case smGroundBound:
			{
				_tprintf( TEXT( "Leaved ground mode\n" ) );
				m_szMode = smNone;
				break;
			}
		default: break;
	}

	UpdateCursor();
}

VOID CNesCanvas::DrawFocusedObjectRect( HDC hDC, RECT rc )
{
	for ( int x = 0; x < 3; ++x )
	{
		DrawFocusRect( hDC, &rc );
		rc.left--; rc.top--;
		rc.right++; rc.bottom++;
	}
}

VOID CNesCanvas::MouseCatchObject( int x, int y, BOOL fCatchOnly )
{
	if ( m_pInterface )
	{
		if ( !IsInSizeMode() && !IsInGroundMode() )
		{
			RECT rcObject = { 0 };
			m_capture.sel.fVisible = m_pInterface->TryCatchObject( x, y, HIWORD( GetKeyState( VK_CONTROL ) ) );
			
			if ( !m_capture.sel.fVisible && !fCatchOnly )
			{
				m_capture.sel.fSelRectVisible = TRUE;
				m_capture.sel.rcSel.left = m_capture.sel.rcSel.right = x;
				m_capture.sel.rcSel.top = m_capture.sel.rcSel.bottom = y;
			}
			else
			{
				BeginMove();
			}
			
			UpdateFocusRect();
			NotifyObjSel( m_pInterface->GetSelObj() );
			// WARNING: GUI can change m_pInterface after this call!!!
		}
		else if ( IsInGroundMode() && !fCatchOnly )
		{
			m_pInterface->TryCatchGroundBound( x );
			BeginMove();
		}
		else if ( IsInSizeMode() )
		{
			BeginMove();
		}

		UpdateCursor();
		InvalidateRect( *this, nullptr, FALSE );
	}
}

VOID CNesCanvas::MouseDoubleClick( int x, int y )
{
	if ( m_pInterface )
	{
		NotifyDblClick( m_pInterface->GetSelObj() );
		// WARNING: GUI can change m_pInterface after this call!!!
	}
}

VOID CNesCanvas::MouseRightClick( int x, int y )
{
	if ( m_pInterface )
	{
		NotifyRightClick( m_pInterface->GetSelObj() );
		// WARNING: GUI can change m_pInterface after this call!!!
	}
}

VOID CNesCanvas::MouseFreeObject( int x, int y )
{
	if ( m_pInterface )
	{
		m_capture.sel.fBeginMove = FALSE;
		if ( !IsInSizeMode() )
		{
			m_capture.sel.fSelRectVisible = FALSE;
			m_pInterface->ReleaseObject( x, y );
			InvalidateRect( *this, nullptr, TRUE );
		}
	}
}

VOID CNesCanvas::MouseMove( int rx, int ry )
{
	m_capture.fMouseLeaved = FALSE;

	if ( m_pInterface )
	{
		if ( !CheckSizeMode( rx, ry ) )
		{
			int x = rx / 16, y = ry / 16;
			if ( !IsEqualMovePoint( x, y ) )
			{
				NotifyMapMove( m_pInterface->GetMetatile( x, y ), x, y );
			}

			if ( m_pInterface->IsOverObject( x, y ) )
			{
				NotifyObjHl( m_pInterface->GetHlObj() );
				SetMode( smHand );
			}
			else
			{
				NotifyObjHl( nullptr );
				LeaveHiliteMode();
				if ( m_pInterface->IsOverGroundBound( x ) )
				{
					SetMode( smGroundBound );
				}
				else
				{
					LeaveGroundBoundMode();
				}
			}
		}
	}
}

VOID CNesCanvas::MouseMoveObject( int x, int y )
{
	if ( m_pInterface )
	{
		if ( IsInSizeMode() )
		{
			if ( !IsEqualCatchPoint( x, y ) )
			{
				BeginMoveNotify();

				if ( m_pInterface )
				{
					m_pInterface->ResizeObject( x, y, m_szMode );
				}
			}
		}
		else if ( IsInGroundMode() )
		{
			if ( !IsEqualCatchPoint( x ) )
			{
				BeginMoveNotify();

				if ( m_pInterface )
				{
					m_pInterface->ResizeGroundBound( x );
				}
			}
		}
		else
		{
			if ( !IsEqualCatchPoint( x, y ) )
			{
				if ( !m_capture.sel.fSelRectVisible )
				{
					BeginMoveNotify();

					if ( m_pInterface )
					{
						m_pInterface->MoveObject( x, y );
					}
				}
				else
				{
					m_capture.sel.rcSel.right = x;
					m_capture.sel.rcSel.bottom = y;
					m_capture.sel.fVisible = m_pInterface->TryCatchObjects( NormalizeRect( m_capture.sel.rcSel ) );
					UpdateFocusRect();
					InvalidateRect( *this, nullptr, TRUE );
				}
			}
		}

		if ( m_pInterface->RedrawRequired() )
		{
			UpdateFocusRect();
			RenderView();
			InvalidateRect( *this, nullptr, TRUE );
		}
	}
}

VOID CNesCanvas::MouseLeave()
{
	if ( m_pInterface && !m_capture.fMouseLeaved )
	{
		// _tprintf( TEXT( "Mouse leaved\n" ) );
		m_capture.fMouseLeaved = TRUE;
		NotifyObjHl( nullptr );
		NotifyMapMove( { 0 }, 0, 0 );
		m_pInterface->LeaveHilite();
		LeaveGroundBoundMode();
		LeaveHiliteMode();
		LeaveSizeMode();
		UpdateCursor();
		InvalidateRect( *this, nullptr, TRUE );
	}
}

RECT CNesCanvas::NormalizeRect( const RECT & rc )
{
	RECT rcTg = rc;
	if ( rcTg.left > rcTg.right )
	{
		LONG x = rcTg.left;
		rcTg.left = rcTg.right;
		rcTg.right = x;
	}
	else
	{
		rcTg.right++;
	}

	if ( rcTg.top > rcTg.bottom )
	{
		LONG y = rcTg.top;
		rcTg.top = rcTg.bottom;
		rcTg.bottom = y;
	}
	else
	{
		rcTg.bottom++;
	}
	return rcTg;
}

VOID CNesCanvas::UpdateFocusRect()
{
	if ( m_pInterface )
	{
		std::vector<RECT> vrc;
		if ( m_pInterface->GetSelObjArea( vrc ) )
		{
			m_capture.sel.vrcSel.clear();
			m_capture.sel.vrcSel.reserve( vrc.size() );

			for ( const auto & rcObj : vrc )
			{
				RECT rc;
				SetRect( &rc,
					rcObj.left * 16,
					rcObj.top * 16,
					rcObj.right * 16,
					rcObj.bottom * 16 );
				m_capture.sel.vrcSel.push_back( rc );
			}
		}
	}
}

BOOL CNesCanvas::IsEqualCatchPoint( int x, int y )
{
	return ( m_capture.ptCatch.x == x && m_capture.ptCatch.y == y );
}

BOOL CNesCanvas::IsEqualCatchPoint( int x )
{
	return ( m_capture.ptCatch.x == x );
}

BOOL CNesCanvas::IsEqualMovePoint( int x, int y )
{
	return ( m_capture.ptMove.x == x && m_capture.ptMove.y == y );
}

INT_PTR CNesCanvas::NotifyMapMove( const NES_METATILE & tile, int x, int y )
{
	NCNVIEWMAP nmap;
	nmap.tile = tile;
	nmap.pt.x = x; nmap.pt.y = y;
	nmap.nm.code = NCVN_MAP_MOVE;
	nmap.nm.idFrom = (UINT_PTR)CtlData().hMenu;
	nmap.nm.hwndFrom = *this;
	return SendMessage( CtlData().hwndParent, WM_NOTIFY, 0, (LPARAM)&nmap );
}

INT_PTR CNesCanvas::NotifyObjSel( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = FALSE;
	view.pObject = pObject;
	return Notify( NCVN_OBJECT_SELECT, &view );
}

INT_PTR CNesCanvas::NotifyObjHl( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = FALSE;
	view.pObject = pObject;
	return Notify( NCVN_OBJECT_HILITE, &view );
}

INT_PTR CNesCanvas::NotifyRedraw( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = TRUE;
	view.pObject = pObject;
	return Notify( NCVN_REDRAW_REQUEST, &view );
}

INT_PTR CNesCanvas::NotifyDblClick( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = FALSE;
	view.pObject = pObject;
	return Notify( NCVN_DBL_CLICK, &view );
}

INT_PTR CNesCanvas::NotifyBeginMove( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = FALSE;
	view.pObject = pObject;
	return Notify( NCVN_BEGIN_MOVE, &view );
}

INT_PTR CNesCanvas::NotifyRightClick( CNesObject * pObject )
{
	NCVNVIEW view;
	view.fRedrawRequired = FALSE;
	view.pObject = pObject;
	return Notify( NCVN_RIGHT_CLICK, &view );
}

INT_PTR CNesCanvas::Notify( int uCode, PNCVNVIEW pnc )
{
	pnc->nm.code = uCode;
	pnc->nm.idFrom = (UINT_PTR)CtlData().hMenu;
	pnc->nm.hwndFrom = *this;
	pnc->pt = m_capture.ptCatch;
	return SendMessage( CtlData().hwndParent, WM_NOTIFY, 0, (LPARAM)pnc );
}

VOID CNesCanvas::SetView( CNesLevelInterface * pInterface )
{
	LockDraw();
	DelView();
	m_bm.FillRect( 0, 0, m_bm.Width(), m_bm.Height(), RGB( 0, 0, 0 ) );	// fill rect with black

	if ( nullptr != pInterface )
	{
		SIZE szArea = { 16 * pInterface->CanvasSize().cx, 16 * pInterface->CanvasSize().cy + pInterface->ReservedHeight() };
		m_pInterface = pInterface;
		m_uReservedHeight = pInterface->ReservedHeight();

		if ( !CtlSize( szArea.cx, szArea.cy ) )
		{
			RenderView();
		}

		UnlockDraw();
		InvalidateRect( *this, nullptr, TRUE );
	}

	UnlockDraw();
}

VOID CNesCanvas::DelView()
{
	MouseLeave();
	m_capture.fCapture = m_capture.hilite.fVisible = m_capture.sel.fVisible = m_capture.sel.fSelRectVisible = m_capture.sel.fBeginMove = FALSE;
	ReleaseCapture();
	m_pInterface = nullptr;
	CtlSize( 0, 0 );
}

CNesLevelInterface * CNesCanvas::GetView()
{
	return m_pInterface;
}

VOID CNesCanvas::RenderView()
{
	if ( m_pInterface )
	{
		if ( m_pInterface->IsLevelSizeChanged() )
		{
			POINT pt = { GetScrollPos( *this, SB_HORZ ), GetScrollPos( *this, SB_VERT ) };
			SetView( m_pInterface );
			OnScroll( WM_HSCROLL, LOWORD( pt.x ), SB_THUMBPOSITION, nullptr );
			OnScroll( WM_VSCROLL, LOWORD( pt.y ), SB_THUMBPOSITION, nullptr );

		}
		else
		{
			m_pInterface->PrepareRenderLevel();
			m_pInterface->RenderLevel( m_bm );
		}
	}
}

BOOL CNesCanvas::CheckSizeMode( int rx, int ry )
{
	SIZE_MODE szMode = smNone;

	if ( !m_pInterface )
	{
		return FALSE;
	}

	if ( m_capture.sel.fVisible && !m_pInterface->IsMultipleSelection() && m_capture.sel.vrcSel.size() > 0 )
	{
		const RECT & rcSel = m_capture.sel.vrcSel[ 0 ];

		// first check if object was deleted
		if ( !m_pInterface->GetSelObj() )
		{
			m_capture.sel.fVisible = FALSE;
			return FALSE;
		}

		if ( ry >= rcSel.top && ry <= rcSel.bottom )
		{
			if ( abs( rx - rcSel.left ) < 4 )
			{
				_tprintf( TEXT( "Entered LEFT size mode\n" ) );
				szMode = smLeft;
			}
			else if ( abs( rcSel.right - rx ) < 4 )
			{
				_tprintf( TEXT( "Entered RIGHT size mode\n" ) );
				szMode = smRight;
			}
		}

		if ( smNone == szMode && rx >= rcSel.left  && rx <= rcSel.right )
		{
			if ( abs( ry - rcSel.top ) < 4 )
			{
				_tprintf( TEXT( "Entered TOP size mode\n" ) );
				szMode = smTop;
			}
			else if ( abs( rcSel.bottom - ry ) < 4 )
			{
				_tprintf( TEXT( "Entered BOTTOM size mode\n" ) );
				szMode = smBottom;
			}
		}
	}

	if ( szMode != smNone )
	{
		SetMode( szMode );
	}
	else
	{
		LeaveSizeMode();
	}

	return ( szMode != smNone );
}

BOOL CNesCanvas::GetSelectedObjects( std::vector<CNesObject*>& vpObject )
{
	if ( m_pInterface )
	{
		return m_pInterface->GetSelectedObjectList( vpObject );
	}

	return FALSE;
}

VOID CNesCanvas::DropSelection()
{
	m_capture.sel.vrcSel.clear();
	m_capture.sel.fVisible = FALSE;
	if ( m_pInterface )
	{
		m_pInterface->DropSelection();
	}

	InvalidateRect( *this, nullptr, TRUE );
}

POINT CNesCanvas::GetLastPoint()
{
	POINT pt = { 0,0 };
	if ( m_pInterface )
	{
		return m_capture.ptCatch;
	}

	return pt;
}

VOID CNesCanvas::SetSelection( std::vector<CNesObject*>& vpObj )
{
	if ( m_pInterface )
	{
		m_pInterface->SetSelection( vpObj );
		UpdateFocusRect();
		m_capture.sel.fVisible = TRUE;
	}
}

int CNesCanvas::ReservedHeight()
{
	return m_uReservedHeight;
}

VOID CNesCanvas::LockDraw()
{
	m_fDrawLock = TRUE;
}

VOID CNesCanvas::UnlockDraw()
{
	m_fDrawLock = FALSE;
}

BOOL CNesCanvas::IsDrawLocked()
{
	return m_fDrawLock;
}