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

CPageReferencesDlg::CPageReferencesDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 450, 300, TEXT( "Page references" ), TRUE ),
	m_lv( hInstance, 0x100, 7, 7, 150, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS | WS_BORDER ),
	m_cv( hInstance, 0x101, 160, 7 + 20, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_edPage( hInstance, 0x102, nullptr, 160, 7, 30, 13 ),
	m_udPage( hInstance, 0x103 ),
	m_btnJump( hInstance, 0x104, TEXT( "Open level..." ), 7, -( 7 + 15 ), 50, 15 ),
	m_uPage( 0 )
{
	m_tg = { 0xFF, nullptr };
	pushctl( m_lv );
	pushctl( m_edPage );
	pushctl( m_udPage );
	pushctl( m_btnJump );
	pushctl( m_cv );
}

BOOL CPageReferencesDlg::OnInit( LPARAM lParam )
{
	LPCTSTR pszType[] = { TEXT( "Water" ), TEXT( "Ground" ), TEXT( "Underground" ), TEXT( "Castle" ) };
	size_t cLinks = m_vLinks.size();

	m_lv.ExtendedStyle( LVS_EX_FULLROWSELECT );
	m_lv.AddColumn( TEXT( "World" ), 50 );
	m_lv.AddColumn( TEXT( "Area" ) );

	for ( size_t i = 0; i < cLinks; ++i )
	{
		const auto & lv = m_vLinks[ i ];
		CString strFormatWorld, strFormatArea;
		const BYTE bWorld = lv.first;
		strFormatWorld.Format( TEXT( "#%wc" ), ( bWorld < 9 ? ( _T( '0' ) + bWorld + 1 ) : ( _T( 'A' ) + ( bWorld - 9 ) ) ) );
		strFormatArea.Format( TEXT( "%s #%Iu" ), pszType[ lv.second->AreaType() ], lv.second->Metadata().uLevelId + 1 );
		INT_PTR id = m_lv.AddItem( strFormatWorld, (LPARAM)&lv );
		m_lv.SetItem( LODWORD( id ), 1, strFormatArea );
	}

	m_cv.SetView( m_pInterface );
	m_cv.RenderView();
	m_cv.ScrollTo( m_uPage );
	m_udPage.SetRange( m_uMaxPage, 0 );
	m_udPage.Value( m_uPage );
	InvalidateRect( m_cv, nullptr, TRUE );
	SetFocus( m_lv );
	return FALSE;
}

VOID CPageReferencesDlg::OnOK()
{
	Close( 1 );
}

VOID CPageReferencesDlg::OnButton( USHORT uId )
{
	if ( 0x104 == uId )
	{
		std::pair<BYTE, CNesLevel*> * pData = ( std::pair<BYTE, CNesLevel*>* )m_lv.Param( LODWORD( m_lv.CurSel() ) );
		if ( pData )
		{
			m_tg = *pData;
			Close( 2 );
		}
	}
}

VOID CPageReferencesDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( EN_CHANGE == uCmd && 0x102 == uId )
	{
		if ( !m_udPage.IsError() )
		{
			m_uPage = m_udPage.Value();
			m_cv.ScrollTo( m_uPage );
		}
	}
}

VOID CPageReferencesDlg::Set_Param( CNesLevelInterface * pInterface, UINT uPage, UINT uMaxPage, std::vector<std::pair<BYTE, CNesLevel*>>& vLinks )
{
	m_uPage = uPage;
	m_uMaxPage = uMaxPage;
	m_vLinks = vLinks;
	m_pInterface = pInterface;
}

VOID CPageReferencesDlg::Get_Param( UINT & uPage, std::pair<BYTE, CNesLevel*> & tg )
{
	uPage = m_uPage;
	tg = m_tg;
}