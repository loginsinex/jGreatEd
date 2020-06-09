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

CSelectLevelDlg::CSelectLevelDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 150, 200, TEXT( "Select a level" ), TRUE ),
	m_tv( hInstance, 0x100, 7, 7, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, TVS_EX_DOUBLEBUFFER )
{
	pushctl( m_tv );
}

DWORD CSelectLevelDlg::AddParam( const WORLD_PARAM & param )
{
	m_vParam.push_back( param );
	return LODWORD( m_vParam.size() - 1 );
}

WORLD_PARAM CSelectLevelDlg::GetParam( DWORD lParam )
{
	WORLD_PARAM wp = { 0 };
	if ( lParam < m_vParam.size() )
	{
		return m_vParam[ lParam ];
	}

	return wp;
}

BOOL CSelectLevelDlg::OnInit( LPARAM lParam )
{
	HTREEITEM hSelected = nullptr;

	BYTE bWorldCounter = 0;
	for ( const auto & world : m_nAreaList )
	{
		const BYTE bWorld = bWorldCounter++;
		BYTE bLevelCounter = 0;
		CString strWorld;
		WORLD_PARAM param = { 0 };

		param.bWorld = bWorld;
		param.type = ptWorld;

		if ( world.size() > 0 && world[ 0 ].size() > 0 )
		{
			param.lType = world[ 0 ][ 0 ]->AreaPtr();
			strWorld.Format( TEXT( "World #%wc" ), ( bWorld < 9 ? ( _T( '0' ) + bWorld + 1 ) : ( _T( 'A' ) + ( bWorld - 9 ) ) ) );
			HTREEITEM hItem = m_tv.AddItem( TVI_ROOT, strWorld.GetString(), AddParam( param ), TRUE, FALSE, TRUE );

			if ( bWorld == m_world )
			{
				hSelected = hItem;
				m_lParam = param;
			}

			for ( const auto & level : world )
			{
				const BYTE bLevel = bLevelCounter++;
				WORLD_PARAM levelParam = { 0 };
				levelParam.type = ptLevel;
				levelParam.bWorld = bWorld;
				levelParam.bLevel = bLevel;

				if ( level.size() > 0 )
				{
					levelParam.lType = level[ 0 ]->AreaPtr();
					strWorld.Format( TEXT( "Level #%d" ), bLevel + 1 );
					HTREEITEM hLevelItem = m_tv.AddItem( hItem, strWorld.GetString(), AddParam( levelParam ), FALSE, FALSE, TRUE );

					if ( bWorld == m_world && bLevel == m_level )
					{
						hSelected = hLevelItem;
						m_lParam = levelParam;
					}

					if ( level.size() > 1 )
					{
						BYTE bAreaCounter = 0;
						for ( auto pLevel : level )
						{
							const BYTE bArea = bAreaCounter++;
							WORLD_PARAM areaParam = { 0 };
							areaParam.bWorld = bWorld;
							areaParam.bLevel = bLevel;
							areaParam.bArea = bArea;
							areaParam.type = ptArea;
							areaParam.lType = pLevel->AreaPtr();
							strWorld.Format( TEXT( "Area #%d" ), bArea + 1 );

							HTREEITEM hAreaItem = m_tv.AddItem( hLevelItem, strWorld.GetString(), AddParam( areaParam ) );
							if ( bWorld == m_world && bLevel == m_level && m_aptr == pLevel->AreaPtr() && bArea > 0 )
							{
								hSelected = hAreaItem;
								m_lParam = areaParam;
							}
						}
					}
				}
			}
		}
	}

	if ( hSelected )
	{
		m_tv.SetCurSel( hSelected );
	}

	SetFocus( m_tv );
	return FALSE;
}

VOID CSelectLevelDlg::OnOK()
{
	Close( 1 );
}

VOID CSelectLevelDlg::SetParam( const NES_GAME_AREA_LIST & nAreaList, BYTE bWorld, BYTE bLevel, CNesLevel * pLevel )
{
	m_nAreaList = nAreaList;
	m_aptr = pLevel->AreaPtr();
	m_world = bWorld;
	m_level = bLevel;
}

VOID CSelectLevelDlg::GetParam( CNesLevel ** ppLevel, BYTE & bWorld, BYTE & bLevel )
{
	bWorld = m_lParam.bWorld;
	bLevel = m_lParam.bLevel;
	*ppLevel = m_nAreaList[ m_lParam.bWorld ][ m_lParam.bLevel ][ m_lParam.bArea ];
}

INT_PTR CSelectLevelDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x100:
			{
				switch ( lpnm->code )
				{
					case TVN_SELCHANGED:
						{
							LPNMTREEVIEW ptvi = (LPNMTREEVIEW)lpnm;
							m_lParam = GetParam( LODWORD( ptvi->itemNew.lParam ) );
							break;
						}
				}
				break;
			}
	}

	return 0;
}