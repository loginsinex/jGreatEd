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

CAreasDlg::CAreasDlg( HINSTANCE hInstance, CNesLevelsKeeper * pKeeper, BYTE uAutoLevel )
	: CDialog( hInstance, 400, 200, TEXT( "Select a target area" ), TRUE ), m_lvlKeeper( *pKeeper ),
	m_lv( hInstance, 0x100, 7, 7, 100, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER | WS_BORDER ),
	m_ed( hInstance, 0x101, nullptr, 110, 7, 30, 13, FALSE, FALSE, FALSE ),
	m_ud( hInstance, 0x102 ),
	m_cv( hInstance, 0x103, 110, 30, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_pObj( nullptr ), m_pLevel( nullptr ), m_uTargetPage( 0 ), m_pParentLevel( nullptr ), m_uWorld( 0 ), m_fFreePointers( FALSE ),
	m_btnFreePipe( hInstance, 0x104, TEXT( "Delete pointers..." ), 7, -( 7 + 15 ), 100, 15 ),
	m_uAutoLevel( uAutoLevel )
{
	pushctl( m_lv );
	pushctl( m_ed );
	pushctl( m_ud );
	pushctl( m_cv );
	pushctl( m_btnFreePipe );
}

VOID CAreasDlg::OnDestroy()
{
	delete m_cv.GetView();
	m_cv.DelView();
}

BOOL CAreasDlg::OnInit( LPARAM lParam )
{
	NES_LEVEL_TYPE type[] = { nltWater, nltGround, nltUnderground, nltCastle };
	CString strGroup[] = { TEXT( "Water areas" ), TEXT( "Ground areas" ), TEXT( "Underground areas" ), TEXT( "Castle areas" ) };
	std::map<NES_LEVEL_TYPE, int> mGroups;

	if ( !m_pObj || etVerticalPipe != m_pObj->EntranceType() )
	{
		DestroyWindow( m_btnFreePipe );
	}

	m_lv.ExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );
	m_lv.AddColumn( TEXT( "Areas list" ) );
	m_lv.EnableGroups( TRUE );
	
	for ( const auto & t : type )
	{
		mGroups[t] = LODWORD( m_lv.AddGroup( t, strGroup[ t ] ) );
	}

	size_t cAreas = m_lvlKeeper.AreasCount();
	for ( size_t i = 0; i < cAreas; ++i )
	{
		CString str;
		CNesLevel * pLevel = m_lvlKeeper.EnumAreas( i );

		if ( ( pLevel->GetLevelStatus() & 2 ) && ( 1 == m_uAutoLevel ) )
		{
			continue;
		}
		else if ( !( pLevel->GetLevelStatus() & 2 ) && ( 2 == m_uAutoLevel ) )
		{
			continue;
		}

		str.Format( TEXT( "%s%sArea: %Iu" ), 
			( ( pLevel->GetLevelStatus() & 4 ) ? TEXT( "[!] " ) : TEXT( "" ) ),
			( ( pLevel->GetLevelStatus() & 2 ) ? TEXT( "* " ) : TEXT( "" ) ), 
			pLevel->Metadata().uLevelId + 1 );

		INT_PTR item = m_lv.AddItem( str, (LPARAM)pLevel, mGroups[ pLevel->AreaType() ] );
		if ( !m_pLevel && pLevel->Metadata().uLevelId == m_pParentLevel->Metadata().uLevelId || m_pLevel && pLevel->Metadata().uLevelId == m_pLevel->Metadata().uLevelId )
		{
			m_lv.CurSel( LODWORD( item ) );
			m_lv.ScrollToVisible();
		}
	}

	m_ud.SetRange( ( m_pLevel ? m_pLevel : m_pParentLevel )->PagesCount() - 1, 0 );
	m_ud.Value( m_uTargetPage );
	SelectLevel( m_pLevel ? m_pLevel : m_pParentLevel );
	SetFocus( m_lv );
	return FALSE;
}

VOID CAreasDlg::OnButton( USHORT uId )
{
	if ( 0x104 == uId && ( m_pObj && etVerticalPipe == m_pObj->EntranceType() ) )
	{
		m_fFreePointers = Confirm( TEXT( "WARNING: Will be deleted ALL pointers from ALL worlds and this pipe will become unentrable.\nDo you want to continue?" ) );
		
		if ( m_fFreePointers )
		{
			m_lv.Enable( FALSE );
			m_ed.Enable( FALSE );
			m_ud.Enable( FALSE );
			m_cv.Enable( FALSE );
			m_btnFreePipe.Enable( FALSE );
		}
	}
}

VOID CAreasDlg::SelectLevel( CNesLevel * pLevel )
{
	m_pLevel = pLevel;
	CNesLevelInterface * pOldInterface = m_cv.GetView();
	m_cv.DelView();
	delete pOldInterface;

	if ( pLevel )
	{
		m_ud.Enable( TRUE );
		m_ed.Enable( TRUE );
		EnableAll( TRUE, IDOK, 0 );
		m_cv.SetView( m_lvlKeeper.GetInterface( m_uWorld, m_bm, pLevel ));
		m_cv.RenderView();
		m_ud.SetRange( pLevel->PagesCount() - 1, 0 );
		ScrollTo( m_uTargetPage );
	}
	else
	{
		m_ud.Enable( FALSE );
		m_ed.Enable( FALSE );
		EnableAll( FALSE, IDOK, 0 );
		m_bm.Destroy();
	}

	InvalidateRect( m_cv, nullptr, TRUE );
}

VOID CAreasDlg::Set_Obj( BYTE bWorld, CNesLevelItem * pObj )
{
	m_uWorld = bWorld;
	m_pObj = pObj;
	m_pParentLevel = &m_pObj->Level();
	m_pLevel = pObj->GetTargetLevel( bWorld, m_uTargetPage );
}

VOID CAreasDlg::Set_Obj( BYTE bWorld, CNesLevel * pParentLevel, CNesLevel * pTargetLevel, BYTE uTargetPage )
{
	m_uWorld = bWorld;
	m_pObj = nullptr;
	m_pParentLevel = pParentLevel;
	m_pLevel = pTargetLevel;
	m_uTargetPage = uTargetPage;
}

VOID CAreasDlg::Get_Param( CNesLevel ** pLevel, BYTE & uTargetPage, BOOL & fFreePointers )
{
	*pLevel = m_pLevel;
	uTargetPage = m_uTargetPage;
	fFreePointers = m_fFreePointers;
}

INT_PTR CAreasDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x100:
			{
				switch ( lpnm->code )
				{
					case LVN_ITEMCHANGED:
						{
							LPNMLISTVIEW lplv = (LPNMLISTVIEW)lpnm;
							if ( ( lplv->uChanged & LVIF_STATE ) && ( lplv->uNewState & LVIS_SELECTED ) )
							{
								SelectLevel( (CNesLevel*)lplv->lParam );
							}
							break;
						}
				}
			}
	}

	return 0;
}

VOID CAreasDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( 0x101 == uId && EN_CHANGE == uCmd )
	{
		if ( !m_ud.IsError() && m_pLevel )
		{
			m_uTargetPage = LOBYTE( m_ud.Value() );
			ScrollTo( m_uTargetPage );
		}
	}
}

VOID CAreasDlg::ScrollTo( BYTE uTargetPage )
{
	m_cv.ScrollTo( uTargetPage );
}

VOID CAreasDlg::OnOK()
{
	if ( !m_pLevel )
	{
		return;
	}

	if ( ( 1 == m_uAutoLevel && ( 2 & m_pLevel->GetLevelStatus() ) ) 
		 || ( 2 == m_uAutoLevel && !( 2 & m_pLevel->GetLevelStatus() ) ) )
	{
		ShowAlert( TEXT( "Please select a valid level!" ) );
		return;
	}

	Close( 1 );
}