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

#include "stdafx.h"

CChangeAreaTypeDlg::CChangeAreaTypeDlg( HINSTANCE hInstance )
	:
	CDialog( hInstance, 300, 200, TEXT( "Select a type for level" ), TRUE ),
	m_cv( hInstance, 0x100, 7, 7, -7, 130, WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_stType( hInstance, TEXT( "Level type:" ), 7, 152, 50, 10 ),
	m_cbType( hInstance, 0x101, WC_COMBOBOX, nullptr, 60, 150, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL )
{
	pushctl( m_stType );
	pushctl( m_cbType );
	pushctl( m_cv );
}

BOOL CChangeAreaTypeDlg::OnInit( LPARAM lParam )
{
	m_cv.SetView( m_pInterface );

	m_cbType.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Water" ) );
	m_cbType.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Ground" ) );
	m_cbType.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Underground" ) );
	m_cbType.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Castle" ) );

	m_cbType.cSendMessage( CB_SETCURSEL, m_pLevel->AreaPtr().bType );

	SetFocus( m_cbType );
	m_cv.RenderView();
	InvalidateRect( m_cv, nullptr, TRUE );
	return FALSE;
}

VOID CChangeAreaTypeDlg::OnOK()
{
	Close( 1 );
}

VOID CChangeAreaTypeDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd && 0x101 == uId )
	{
		NES_LEVEL_TYPE type = static_cast<NES_LEVEL_TYPE>( m_cbType.cSendMessage( CB_GETCURSEL ) );
		m_pLevel->UpdateAreaType( type );
		m_pLevel->InitObjects();
		m_cv.RenderView();
		InvalidateRect( m_cv, nullptr, TRUE );
	}
}

VOID CChangeAreaTypeDlg::Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface )
{
	m_pLevel = pLevel;
	m_pInterface = pInterface;
}

VOID CChangeAreaTypeDlg::Get_Param( NES_LEVEL_TYPE & type )
{
	type = m_pLevel->AreaPtr().bType;
}