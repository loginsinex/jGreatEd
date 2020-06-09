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
#include "h/NES/NESEditor/CNesCanvas_Win32Control.h"

///

CCanvasCtl::CCanvasCtl( HINSTANCE hInstance, USHORT uId, INT x, INT y, INT cx, INT cy, DWORD dwStyle, DWORD dwStyleEx, WORD wAnchor )
	: CControl( hInstance, uId, WC_NESCANVAS, nullptr, x, y, cx, cy, dwStyle, dwStyleEx, wAnchor ), m_pCanvas( nullptr )
{
	CNesCanvas::Register( hInstance );
}

VOID CCanvasCtl::PostCreate()
{
	m_pCanvas = (CNesCanvas*)cSendMessage( WM_GETCUSTOMINSTANCE );
}

VOID CCanvasCtl::SetView( CNesLevelInterface * pInterface )
{
	if ( m_pCanvas )
	{
		m_pCanvas->SetView( pInterface );
	}
}

VOID CCanvasCtl::DelView()
{
	if ( m_pCanvas )
	{
		m_pCanvas->DelView();
	}
}

VOID CCanvasCtl::RenderView()
{
	if ( m_pCanvas )
	{
		m_pCanvas->RenderView();
	}
}

CNesLevelInterface * CCanvasCtl::GetView()
{
	if ( m_pCanvas )
	{
		return m_pCanvas->GetView();
	}

	return nullptr;
}

VOID CCanvasCtl::ScrollTo( BYTE uTargetPage )
{
	cSendMessage( WM_HSCROLL, MAKELPARAM( SB_THUMBPOSITION, 0x10 * static_cast<WORD>( uTargetPage ) ) );
}

BOOL CCanvasCtl::GetSelectedObject( std::vector<CNesObject*>& vpObject )
{
	if ( m_pCanvas )
	{
		return m_pCanvas->GetSelectedObjects( vpObject );
	}
	
	return FALSE;
}

VOID CCanvasCtl::DropSelection()
{
	if ( m_pCanvas )
	{
		m_pCanvas->DropSelection();
	}
}

POINT CCanvasCtl::GetLastPoint()
{
	POINT pt = { 0, 0 };
	
	if ( m_pCanvas )
	{
		return m_pCanvas->GetLastPoint();
	}

	return pt;
}

VOID CCanvasCtl::SetSelection( std::vector<CNesObject*>& vpObj )
{
	if ( m_pCanvas )
	{
		m_pCanvas->SetSelection( vpObj );
	}
}