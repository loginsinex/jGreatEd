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

CNewObjectDlg::CNewObjectDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 530, 300, TEXT( "Select an object to create" ), TRUE ),
	m_lv( hInstance, 0x100, 7, 7, 170, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER | WS_BORDER ),
	m_cv( hInstance, 0x103, 180, 7, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ), m_fIsSprite( FALSE ), m_fSpriteHardmode( FALSE ),
	m_chkHard( hInstance, 0x104, TEXT( "Show enemy only in hard mode" ), 7, -( 7 + 15 ), 170, 10 ), m_lParam( -1 ),
	m_pObject( nullptr )
{
	pushctl( m_lv );
	pushctl( m_cv );
	pushctl( m_chkHard );

	ZeroMemory( &m_lvlItem, sizeof( m_lvlItem ) );
	ZeroMemory( &m_enmItem, sizeof( m_enmItem ) );

}

VOID CNewObjectDlg::OnDestroy()
{
	m_pLevel->DeleteObject( m_pObject );
}

BOOL CNewObjectDlg::OnInit( LPARAM lParam )
{
	UINT uResStr[] = { IDS_ITEM_LIST, IDS_ITEM_C_LIST, IDS_ITEM_D_LIST, IDS_ITEM_F_LIST, IDS_ENEMY_LIST };
	UINT uGroup = 0;
	CString str;
	m_lv.ExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );
	m_lv.AddColumn( TEXT( "Objects" ) );
	m_lv.EnableGroups( TRUE );

	for ( auto & u : uResStr )
	{
		str.LoadStringW( u );
		m_lv.AddGroup( uGroup, str );
		AddItemsToGroup( uGroup++, u );
	}

	m_cv.SetView( m_pInterface );
	m_cv.ScrollTo( LOBYTE( m_pt.x / 16 ) );

	m_lvlItem.pt.x = m_pt.x;
	m_enmItem.pt.x = m_pt.x;
	m_lvlItem.pt.y = min( m_pt.y, 11 );
	m_enmItem.pt.y = min( m_pt.y, 12 );
	m_lvlItem.item.X = LOBYTE( m_pt.x & 0x0F );
	m_enmItem.item.X = LOBYTE( m_pt.x & 0x0F );
	m_lvlItem.item.Y = LOBYTE( m_lvlItem.pt.y );
	m_enmItem.item.Y = LOBYTE( m_enmItem.pt.y );

	SetFocus( m_lv );
	m_cv.RenderView();
	InvalidateRect( m_cv, nullptr, TRUE );
	return FALSE;
}

VOID CNewObjectDlg::AddItemsToGroup( UINT uGroup, UINT uResStr )
{
	const UINT Y[] = { 0x000, 0xC00, 0xD00, 0xF00, 0x000 };
	const UINT uGrp1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 48, 64, 80, 96, 112 };	
	const UINT uGrp2[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	const UINT uGrp3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
	const UINT uGrp4[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	const UINT uGrp5[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
		20, 21,
		23, 24,
		27, 28, 29, 30, 31, // 32, 33, 34,
		36, 37, 38, 39, 40, 41, 42, 43, 44,
		45, 46, 47,
		52, 53,
		55, 56, 57, 58, 59, 60, 61, 62
	};

	switch ( uGroup )
	{
		case 0: AddItemsToList( uGroup, uResStr, 0x00, 0x01, 0x00000 | Y[ uGroup ], uGrp1 ); break;
		case 1: AddItemsToList( uGroup, uResStr, 0x00, 0x10, 0x00003 | Y[ uGroup ], uGrp2 ); break;
		case 2: AddItemsToList( uGroup, uResStr, 0x40, 0x01, 0x00000 | Y[ uGroup ], uGrp3 ); break;
		case 3: AddItemsToList( uGroup, uResStr, 0x00, 0x10, 0x00006 | Y[ uGroup ], uGrp4 ); break;
		case 4: AddItemsToList( uGroup, uResStr, 0x00, 0x01, 0x10000 | Y[ uGroup ], uGrp5 ); break;
	}
}

template<class T>
inline VOID CNewObjectDlg::AddItemsToList( UINT uGroup, UINT uResStr, UINT uDiff, UINT uMultiplier, UINT uFlag, const T & arr )
{
	for ( const auto & id : arr )
	{
		CString str;
		str.LoadStringW( uResStr + 1 + id );
		m_lv.AddItem( str, uFlag | ( id * uMultiplier + uDiff ), uGroup );
	}
}

VOID CNewObjectDlg::TestItem( LPARAM lParam )
{
	BOOL fAllowedObject = TRUE;

	m_lParam = lParam;
	if ( m_pObject )
	{
		m_pLevel->DeleteObject( m_pObject );
		m_pObject = nullptr;
	}

	const BYTE Y = LOBYTE( ( lParam & 0xFF00 ) >> 8 );
	const BYTE item = LOBYTE( lParam & 0xFF );
	const BOOL fSprite = ( lParam & 0x10000 );

	if ( !fSprite )
	{
		// level item
		m_lvlItem.item.itemId = item;
		if ( Y )
		{
			m_lvlItem.item.Y = Y;
			m_lvlItem.pt.y = Y;
		}
		else
		{
			if ( item & 0xF0 )
			{
				m_lvlItem.item.itemId |= 0x02;	// for massive object set width (height) to 3 (2 + 1)
			}

			m_lvlItem.item.Y = min( LOBYTE( m_pt.y ), 0x0b );
			m_lvlItem.pt.y = min( m_pt.y, 0x0b );
		}

		if ( NES_SPECIAL_OBJECT_D == Y && 0x0F == ( m_lvlItem.item.X ) )
		{
			fAllowedObject = FALSE;
		}

		if ( m_bWorld < 4 )
		{
			switch ( Y )
			{
				case NES_SPECIAL_OBJECT_D:
					{
						switch ( item & 0x0F )
						{
							case 12:
							case 13: fAllowedObject = FALSE; break;
						}
					}
				case NES_SPECIAL_OBJECT_F:
					{
						switch ( ( item & 0xF0 ) >> 4 )
						{
							case 6:
							case 7: fAllowedObject = FALSE; break;
						}
					}
			}
		}

		m_pObject = m_pLevel->AddLevelItem( m_lvlItem );
		m_fIsSprite = FALSE;
	}
	else
	{
		// enemy item
		m_enmItem.item.objectId = item;
		if ( m_fSpriteHardmode )
		{
			m_enmItem.item.objectId |= 0x40;
		}

		if ( NES_SPECIAL_OBJECT_F == Y && 0x0F == ( m_enmItem.item.X ) ||
			 Y < NES_SPECIAL_OBJECT_C && 0x04 == m_enmItem.item.objectId && m_bWorld < 4
			 )
		{
			fAllowedObject = FALSE;
		}

		m_pObject = m_pLevel->AddEnemyItem( m_enmItem );
		m_fIsSprite = TRUE;
	}

	if ( m_pObject )
	{
		m_cv.RenderView();
		InvalidateRect( m_cv, nullptr, TRUE );
	}
	else
	{
		fAllowedObject = FALSE;
	}

	EnableAll( fAllowedObject, IDOK, 0 );
}

VOID CNewObjectDlg::OnOK()
{
	Close( 1 );
}

VOID CNewObjectDlg::OnButton( USHORT uId )
{
	if ( 0x104 == uId )
	{
		if ( -1 != m_lParam )
		{
			m_fSpriteHardmode = !m_fSpriteHardmode;

			if ( 0x10000 & m_lParam )
			{
				TestItem( m_lParam );
			}
		}
	}
}

INT_PTR CNewObjectDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x100:
			{
				switch ( lpnm->code )
				{
					case LVN_ITEMCHANGED: 
						{
							NMLISTVIEW * plv = (NMLISTVIEW*)lpnm;
							if ( plv->uNewState & LVIS_SELECTED )
							{
								TestItem( ( (LPNMLISTVIEW)lpnm )->lParam );
							}
							break;
						}
				}
			}
	}

	return 0;
}

VOID CNewObjectDlg::Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, BYTE bWorld, POINT pt )
{
	m_pLevel = pLevel;
	m_pInterface = pInterface;
	m_pt = pt;
	m_bWorld = bWorld;
}

VOID CNewObjectDlg::Get_Param( NES_LEVEL_ITEM & lvlItem )
{
	lvlItem = m_lvlItem;
}

VOID CNewObjectDlg::Get_Param( NES_LEVEL_ENEMY_ITEM & enmItem )
{
	enmItem = m_enmItem;
}

BOOL CNewObjectDlg::IsSpriteCreated()
{
	return m_fIsSprite;
}