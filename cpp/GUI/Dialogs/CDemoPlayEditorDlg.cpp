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

CDemoPlayEditorDlg::CDemoPlayEditorDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 300, 320, TEXT( "Demo play controls" ), FALSE, TRUE ),
	m_lv( hInstance, 0x100, 7, 7, -7, 200, WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | WS_BORDER, 0, ANCHOR_FULL ),
	m_btnAdd( hInstance, 0x200, TEXT( "+" ), 7, 210, 20, 20, FALSE, FALSE, ANCHOR_LEFT_BOTTOM ),
	m_btnDel( hInstance, 0x201, TEXT( "-" ), 30, 210, 20, 20, FALSE, FALSE, ANCHOR_LEFT_BOTTOM ),
	m_btnOK( hInstance, IDOK, MB_GetString( IDOK - 1 ).c_str(), -( 50 + ( 7 + 50 ) + ( 7 + 50 ) + 7 ), -( 7 + 15 ), 50, 15, TRUE, FALSE, ANCHOR_RIGHT_BOTTOM ),
	m_btnApply( hInstance, 0x1000, TEXT( "Apply" ), -( 50 + ( 7 + 50 ) + 7 ), -( 7 + 15 ), 50, 15, FALSE, FALSE, ANCHOR_RIGHT_BOTTOM ),
	m_btnCancel( hInstance, IDCANCEL, MB_GetString( IDCANCEL - 1 ).c_str(), -( 50 + 7 ), -( 7 + 15 ), 50, 15, FALSE, FALSE, ANCHOR_RIGHT_BOTTOM ),
	m_edTimer( hInstance, 0x101, nullptr, -( 7 + 70 ), 220, 70, 17, FALSE, FALSE, FALSE, WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER, 0, ANCHOR_RIGHT_BOTTOM ),
	m_udTimer( hInstance, 0x102 ),
	m_fntListview( 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT( "Courier new" ) )
{
	m_btnCtrl[ 0 ].PostInit( hInstance, 0x501, WC_BUTTON, nullptr, 7 + ( 15 + 15 ), 250 + ( 15 ), 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );
	m_btnCtrl[ 1 ].PostInit( hInstance, 0x502, WC_BUTTON, nullptr, 7, 250 + ( 15 ), 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );
	m_btnCtrl[ 2 ].PostInit( hInstance, 0x504, WC_BUTTON, nullptr, 7 + ( 15 ), 250 + ( 15 + 15 ), 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );
	m_btnCtrl[ 3 ].PostInit( hInstance, 0x508, WC_BUTTON, nullptr, 7 + ( 15 ), 250, 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );
	m_btnCtrl[ 4 ].PostInit( hInstance, 0x580, WC_BUTTON, TEXT( "B" ), 7 + ( 15 + 15 + 15 + 15 ), 250 + 15, 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );
	m_btnCtrl[ 5 ].PostInit( hInstance, 0x540, WC_BUTTON, TEXT( "A" ), 7 + ( 15 + 15 + 15 + 15 + 15 + 15 ), 250 + 15, 15, 15, WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_PUSHLIKE, 0, ANCHOR_LEFT_BOTTOM );

	pushctl( m_lv );
	pushctl( m_edTimer );
	pushctl( m_udTimer );
	// pushctl( m_btnAdd );
	// pushctl( m_btnDel );

	for ( auto & ctl : m_btnCtrl )
	{
		pushctl( ctl );
	}

	pushctl( m_btnOK );
	pushctl( m_btnApply );
	pushctl( m_btnCancel );
}

CString CDemoPlayEditorDlg::GetBitsString( BYTE bBits )
{
	TCHAR chBits[] = { _T( 'R' ), _T( 'L' ), _T( 'D' ),  _T( 'U' ),  _T( 'S' ),  _T( 'T' ),  _T( 'B' ),  _T( 'A' ) };
	TCHAR str[ 9 ];
	str[ 8 ] = 0;

	for ( int bits = 0; bits < 8; ++bits )
	{
		str[ bits ] = ( ( bBits & 1 ) ? chBits[ bits ] : _T( '-' ) );
		bBits >>= 1;
	}

	return str;
}

VOID CDemoPlayEditorDlg::UpdateFields()
{
	INT_PTR id = m_lv.CurSel();
	if ( id >= 0 && static_cast<size_t>( id ) < m_vDemoPlay.size() )
	{
		for ( auto & ctl : m_btnCtrl )
		{
			ctl.Enable( TRUE );
		}
		m_edTimer.Enable( TRUE );
		m_udTimer.Enable( TRUE );
		SetBits( m_vDemoPlay[ id ].second );
		SetTiming( m_vDemoPlay[ id ].first );
	}
	else
	{
		for ( auto & ctl : m_btnCtrl )
		{
			ctl.Enable( FALSE );
		}
		m_edTimer.Enable( FALSE );
		m_udTimer.Enable( FALSE );
		SetBits( 0 );
		SetTiming( 0 );
	}
}

VOID CDemoPlayEditorDlg::SetBits( BYTE bBits )
{
	m_btnCtrl[ 0 ].Check( bBits & 0x01 );
	m_btnCtrl[ 1 ].Check( bBits & 0x02 );
	m_btnCtrl[ 2 ].Check( bBits & 0x04 );
	m_btnCtrl[ 3 ].Check( bBits & 0x08 );
	m_btnCtrl[ 4 ].Check( bBits & 0x40 );
	m_btnCtrl[ 5 ].Check( bBits & 0x80 );
}

VOID CDemoPlayEditorDlg::SetTiming( BYTE bTiming )
{
	m_udTimer.Value( bTiming );
}

VOID CDemoPlayEditorDlg::SaveInput()
{
	INT_PTR id = m_lv.CurSel();
	if ( id >= 0 && static_cast<size_t>( id ) < m_vDemoPlay.size() )
	{
		BYTE & bBits = m_vDemoPlay[ id ].second;
		BYTE & bTiming = m_vDemoPlay[ id ].first;

		bBits &= 0x30;
		bBits |= ( m_btnCtrl[ 0 ].Check() ? 0x01 : 0 );
		bBits |= ( m_btnCtrl[ 1 ].Check() ? 0x02 : 0 );
		bBits |= ( m_btnCtrl[ 2 ].Check() ? 0x04 : 0 );
		bBits |= ( m_btnCtrl[ 3 ].Check() ? 0x08 : 0 );
		bBits |= ( m_btnCtrl[ 4 ].Check() ? 0x40 : 0 );
		bBits |= ( m_btnCtrl[ 5 ].Check() ? 0x80 : 0 );

		if ( !m_udTimer.IsError() )
		{
			bTiming = LOBYTE( m_udTimer.Value() );
		}

		m_lv.SetItem( LODWORD( id ), 0, GetBitsString( bBits ) );
		m_lv.SetItem( LODWORD( id ), 1, bTiming );
		m_btnApply.Enable( TRUE );
	}
}

BOOL CDemoPlayEditorDlg::OnInit( LPARAM lParam )
{
	m_lv.cSendMessage( WM_SETFONT, ( WPARAM )static_cast<HFONT>( m_fntListview ), TRUE );
	m_edTimer.cSendMessage( WM_SETFONT, ( WPARAM )static_cast<HFONT>( m_fntListview ), TRUE );
	m_lv.AddColumn( TEXT( "Control" ), 200 );
	m_lv.AddColumn( TEXT( "Ticks" ) );
	m_lv.ExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );
	
	for ( const auto & ctl : m_vDemoPlay )
	{
		const BYTE bBits = ctl.second;
		const BYTE bTiming = ctl.first;
		INT_PTR id = m_lv.AddItem( GetBitsString( bBits ), 0 );
		m_lv.SetItem( LODWORD( id ), 1, bTiming );
	}

	m_udTimer.SetRange( 255, 1 );
	m_udTimer.Visible( FALSE );
	UpdateFields();
	SetFocus( m_lv );
	m_btnApply.Enable( FALSE );
	return FALSE;
}

VOID CDemoPlayEditorDlg::OnButton( USHORT uId )
{
	size_t id = m_lv.CurSel();

	if ( uId >= 0x500 && uId < 0x600 )
	{
		SaveInput();
	}
	else if ( 0x1000 == uId )
	{
		NotifyUpdate();
		m_btnApply.Enable( FALSE );
	}
	else
	{
		switch ( uId )
		{
			case 0x200: 
				{
					std::pair < BYTE, BYTE > ctl = { 1, 1 };

					if ( id > 255 )
					{
						id = 0;
						m_vDemoPlay.push_back( ctl );
					}
					else
					{
						m_vDemoPlay.insert( m_vDemoPlay.begin() + id, ctl );
					}

					INT_PTR idNew = m_lv.InsertItem( LODWORD( id ), GetBitsString( ctl.second ), 0 );
					m_lv.SetItem( LODWORD( idNew ), 1, ctl.second );
					m_lv.CurSel( LODWORD( idNew ) );
					break;
				}
			case 0x201:
				{
					if ( m_vDemoPlay.size() > 0 )
					{
						m_vDemoPlay.erase( m_vDemoPlay.begin() + id );
						m_lv.DeleteItem( LODWORD( id ) );
						m_lv.CurSel( LODWORD( id ) );
					}
					break;
				}
		}
		UpdateFields();
	}
}

INT_PTR CDemoPlayEditorDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x100:
			{
				switch ( lpnm->code )
				{
					case LVN_ITEMCHANGED:
						{
							NMLISTVIEW * pnmlv = (NMLISTVIEW*)lpnm;
							if ( ( pnmlv->uChanged & LVIF_STATE ) && ( pnmlv->uNewState& LVIS_SELECTED ) )
							{
								UpdateFields();
							}
							break;
						}
				}
				break;
			}
	}

	return 0;
}

VOID CDemoPlayEditorDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( EN_CHANGE == uCmd && 0x101 == uId )
	{
		SaveInput();
	}
}

VOID CDemoPlayEditorDlg::OnOK()
{
	NotifyUpdate();
	Close( 0 );
}

VOID CDemoPlayEditorDlg::NotifyUpdate()
{
	SendMessage( *Parent(), WM_COMMAND, MAKELONG( ID_TOOLS_DEMO, BN_CLICKED ), 0 );
}

VOID CDemoPlayEditorDlg::Set_Param( const std::vector<std::pair<BYTE, BYTE>>& vDemoPlay )
{
	m_vDemoPlay = vDemoPlay;
}

VOID CDemoPlayEditorDlg::Get_Param( std::vector<std::pair<BYTE, BYTE>>& vDemoPlay )
{
	vDemoPlay = m_vDemoPlay;
}