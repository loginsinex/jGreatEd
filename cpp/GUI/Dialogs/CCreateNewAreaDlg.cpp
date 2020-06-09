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

CCreateNewAreaDlg::CCreateNewAreaDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 200, 170, TEXT( "New area specifications" ), TRUE ),
	m_rdNewArea( hInstance, 0x100, TEXT( "Empty area" ), 7, 7, -7, 13, FALSE ),
	m_rdBaseArea( hInstance, 0x101, TEXT( "Area based on existing" ), 7, 80, -7, 13, FALSE ),
	m_stType( hInstance, TEXT( "Area type:" ), 7, 32, 50, 10 ),
	m_cbType( hInstance, 0x200, WC_COMBOBOX, nullptr, 60, 30, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_chkAuto( hInstance, 0x201, TEXT( "Auto level" ), 60, 50, 50, 10 ),
	m_chkShared( hInstance, 0x202, TEXT( "Shared area" ), 120, 50, 60, 10 ),
	m_stArea( hInstance, TEXT( "Base area:" ), 7, 102, 50, 10 ),
	m_edArea( hInstance, 0x300, nullptr, 60, 100, 70, 13, FALSE, FALSE, TRUE ),
	m_btnBrowse( hInstance, 0x301, TEXT( "Browse..." ), 140, 100, 50, 15 ),
	m_bWorld( 0 ), m_uType( nltGround ), m_pKeeper( nullptr ), m_pLevel( nullptr ), m_fAutoLevel( FALSE ), m_fSharedSection( FALSE )
{
	pushctl( m_rdNewArea );
	pushctl( m_rdBaseArea );
	pushctl( m_stType );
	pushctl( m_cbType );
	pushctl( m_chkAuto );
	pushctl( m_chkShared );
	pushctl( m_stArea );
	pushctl( m_edArea );
	pushctl( m_btnBrowse );

	m_mType[ nltWater ] = TEXT( "Water area" );
	m_mType[ nltGround ] = TEXT( "Ground area" );
	m_mType[ nltUnderground ] = TEXT( "Underground area" );
	m_mType[ nltCastle ] = TEXT( "Castle area" );
}

BOOL CCreateNewAreaDlg::OnInit( LPARAM lParam )
{
	for ( const auto & type : m_mType )
	{
		INT_PTR id = m_cbType.cSendMessage( CB_ADDSTRING, 0, (LPARAM)type.second.GetString() );
		m_cbType.cSendMessage( CB_SETITEMDATA, id, type.first );
		if ( type.first == m_uType )
		{
			m_cbType.cSendMessage( CB_SETCURSEL, id );
		}
	}

	CString sArea;
	sArea.Format( TEXT( "%s #%d" ), m_mType[ m_pLevel->AreaPtr().bType ].GetString(), m_pLevel->AreaPtr().bAreaId + 1 );
	m_edArea.Text( sArea.GetString() );

	m_rdNewArea.Check( TRUE );
	m_edArea.Enable( FALSE );
	m_btnBrowse.Enable( FALSE );

	SetFocus( m_rdNewArea );
	return FALSE;
}

VOID CCreateNewAreaDlg::OnButton( USHORT uId )
{
	switch ( uId )
	{
		case 0x100:
		case 0x101:
			{
				m_rdNewArea.Check( uId == 0x100 );
				m_rdBaseArea.Check( uId == 0x101 );
				if ( 0x100 == uId )
				{
					m_cbType.Enable( TRUE );
					m_chkAuto.Enable( TRUE );
					m_chkShared.Enable( TRUE );
					m_edArea.Enable( FALSE );
					m_btnBrowse.Enable( FALSE );
				}
				else
				{
					m_cbType.Enable( FALSE );
					m_chkAuto.Enable( FALSE );
					m_chkShared.Enable( FALSE );
					m_edArea.Enable( TRUE );
					m_btnBrowse.Enable( TRUE );
				}
				break;
			}
		case 0x201: 
			{
				m_fAutoLevel = m_chkAuto.Check();
				break;
			}
		case 0x202:
			{
				m_fSharedSection = m_chkShared.Check();
				break;
			}
		case 0x301:
			{
				CAreasDlg dlg( GetInstance(), m_pKeeper );
				dlg.Set_Obj( m_bWorld, m_pLevel, nullptr, 0 );

				if ( dlg.Show( this ) )
				{
					CString sArea;
					BYTE dummy1;
					BOOL dummy2;
					dlg.Get_Param( &m_pLevel, dummy1, dummy2 );

					sArea.Format( TEXT( "%s #%Iu" ), m_mType[ m_pLevel->AreaPtr().bType ].GetString(), m_pLevel->Metadata().uLevelId + 1 );
					m_edArea.Text( sArea.GetString() );
				}
				break;
			}
	}
}

VOID CCreateNewAreaDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		if ( 0x200 == uId )
		{
			INT_PTR id = m_cbType.cSendMessage( CB_GETCURSEL );
			if ( id >= 0 )
			{
				m_uType = static_cast<NES_LEVEL_TYPE>( LOBYTE( m_cbType.cSendMessage( CB_GETITEMDATA, id ) ) & 0x03 );
			}
		}
	}
}

VOID CCreateNewAreaDlg::OnOK()
{
	INT_PTR res = 0;
	if ( m_rdNewArea.Check() )
	{
		res = 1;
	}
	else
	{
		res = 2;
	}

	Close( res );
}

VOID CCreateNewAreaDlg::Set_Param( BYTE bWorld, CNesLevelsKeeper * pKeeper, CNesLevel * pLevel )
{
	m_pKeeper = pKeeper;
	m_pLevel = pLevel;
}

VOID CCreateNewAreaDlg::Get_Param( NES_LEVEL_TYPE & uType, BOOL & fAutoLevel, BOOL & fSharedSection, CNesLevel ** pBaseLevel )
{
	uType = m_uType;
	*pBaseLevel = m_pLevel;
	fAutoLevel = m_fAutoLevel;
	fSharedSection = m_fSharedSection;
}