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

CLinkManagerDlg::CLinkManagerDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 400, 200, TEXT( "Pointers manager" ), TRUE ),
	m_tv( hInstance, 0x100, 7, 7, 150, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES | TVS_CHECKBOXES | TVS_SHOWSELALWAYS, TVS_EX_DOUBLEBUFFER ),
	m_cv( hInstance, 0x101, 160, 7, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_stInfo( hInstance, TEXT( "Select items which shall be removed" ), 7, -( 7 + 15 ), 150, 10 ),
	m_name( hInstance )
{
	pushctl( m_tv );
	pushctl( m_stInfo );
	pushctl( m_cv );
}

CLinkManagerDlg::~CLinkManagerDlg()
{
	for ( auto & ptoli : m_vItemBuffer )
	{
		delete ptoli;
	}

	m_vItemBuffer.clear();
}

TREE_OBJECT_LINK_ITEM & CLinkManagerDlg::GetNewItem( TREE_OBJECT_LINK_ITEM_TYPE ti, CNesLevelItem * pItem )
{
	PTREE_OBJECT_LINK_ITEM ptoli = new TREE_OBJECT_LINK_ITEM;
	ptoli->ti = ti;
	ptoli->pObject = pItem;
	ptoli->hTreeItem = nullptr;
	m_vItemBuffer.push_back( ptoli );
	return *ptoli;
}

BOOL CLinkManagerDlg::OnInit( LPARAM lParam )
{
	for ( auto & item : m_mLinks )
	{
		CNesLevelItem * pItem = item.first;
		const WORLD_OBJECT_LINK & wol = item.second;
		TREE_OBJECT_LINK_ITEM & tvItem = GetNewItem( tiObject, pItem );
		CString strName = ( pItem ? m_name.GetNameOfItem( pItem ).GetString() : TEXT( "Global pointer" ) );
		HTREEITEM hTreeItem = tvItem.hTreeItem = m_tv.AddItem( TVI_ROOT, strName, (LPARAM)&tvItem, TRUE, FALSE, TRUE );

		for ( const auto & world : wol )
		{
			const BYTE bWorld = world.first;
			const auto & lnk = world.second;
			CString strFormat;
			TREE_OBJECT_LINK_ITEM & tvLink = GetNewItem( tiLink, pItem );
			strFormat.Format( TEXT( "World #%wc" ), ( bWorld < 9 ? ( _T( '0' ) + bWorld + 1 ) : ( _T( 'A' ) + ( bWorld - 9 ) ) ) );
			tvLink.link = lnk.first;
			tvLink.bWorld = bWorld;
			tvLink.hTreeItem = m_tv.AddItem( hTreeItem, strFormat, (LPARAM)&tvLink );
		}
	}

	if ( m_vItemBuffer.size() > 0 )
	{
		SelectItem( m_vItemBuffer[ 0 ]->hTreeItem );
	}

	SetFocus( m_tv );
	return FALSE;
}

VOID CLinkManagerDlg::FreeCanvasInterface()
{
	CNesLevelInterface * pInterface = m_cv.GetView();
	m_cv.DelView();
	delete pInterface;
}

VOID CLinkManagerDlg::OnDestroy()
{
	FreeCanvasInterface();
}

INT_PTR CLinkManagerDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x100:
			{
				switch ( lpnm->code )
				{
					case TVN_ITEMCHANGED: return TVItemChanged( (NMTVITEMCHANGE*)lpnm );
				}
				break;
			}
	}

	return 0;
}

INT_PTR CLinkManagerDlg::TVItemChanged( NMTVITEMCHANGE * pnmtv )
{
	if ( ( pnmtv->uStateNew & 0x3000 ) != ( pnmtv->uStateOld & 0x3000 ) )
	{
		CheckItem( pnmtv->hItem, ( pnmtv->uStateNew & 0x2000 ) );
	}
	else if ( ( pnmtv->uStateNew & TVIS_SELECTED ) != ( pnmtv->uStateOld & TVIS_SELECTED ) )
	{
		if ( pnmtv->uStateNew & TVIS_SELECTED )
		{
			SelectItem( pnmtv->hItem );
		}			
	}
	return 0;
}

BOOL CLinkManagerDlg::IsAnySelected()
{
	for ( auto & item : m_mLinks )
	{
		const auto & wol = item.second;

		for ( const auto & ol : wol )
		{
			if ( ol.second.second )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

VOID CLinkManagerDlg::OnOK()
{
	if ( IsAnySelected() )
	{
		if ( Confirm( TEXT( "Are you sure that you want completely remove selected pointers?" ) ) )
		{
			Close( 1 );
		}
		else
		{
			return;
		}
	}
	else
	{
		Close( 0 );
	}
}

VOID CLinkManagerDlg::SelectItem( HTREEITEM hItem )
{
	PTREE_OBJECT_LINK_ITEM pItem = (PTREE_OBJECT_LINK_ITEM)m_tv.Param( hItem );
	if ( pItem )
	{
		switch ( pItem->ti )
		{
			case tiObject:
				{
					CNesLevelInterface * pInterface =
						World2Keeper( m_bWorld )->GetInterface
						(
							m_bWorld, m_bmView,
							( pItem->pObject ? &pItem->pObject->Level() : m_pCurrentLevel )
						);
					FreeCanvasInterface();
					m_cv.SetView( pInterface );
					if ( pItem->pObject )
					{
						m_cv.ScrollTo( LOBYTE( pItem->pObject->GetObjectPoint().x / 16 ) );
					}
					else
					{
						m_cv.ScrollTo( 0 );
					}

					m_cv.RenderView();
					InvalidateRect( m_cv, nullptr, TRUE );
					break;
				}
			case tiLink:
				{
					CNesLevelInterface * pInterface = World2Keeper( pItem->bWorld )->GetInterface( pItem->bWorld, m_bmView, pItem->link.second );
					FreeCanvasInterface();
					m_cv.SetView( pInterface );
					m_cv.ScrollTo( LOBYTE( pItem->link.first ) );
					m_cv.RenderView();
					InvalidateRect( m_cv, nullptr, TRUE );
					break;
				}
		}
	}
}

CNesLevelsKeeper * CLinkManagerDlg::World2Keeper( size_t bWorld )
{
	switch ( bWorld )
	{
		case 0:
		case 1:
		case 2:
		case 3: return m_pKeepers[ asWorld1to4 ];
		case 4:
		case 5:
		case 6:
		case 7: return m_pKeepers[ asWorld5to8 ];
		case 8: return m_pKeepers[ asWorld9 ];
		case 0x09:
		case 0x0A:
		case 0x0B:
		case 0x0C: return m_pKeepers[ asWorldAtoD ];
	}

	return nullptr;
}

VOID CLinkManagerDlg::CheckItem( HTREEITEM hTreeItem, BOOL fCheck )
{
	PTREE_OBJECT_LINK_ITEM pItem = (PTREE_OBJECT_LINK_ITEM)m_tv.Param( hTreeItem );
	if ( pItem )
	{
		switch ( pItem->ti )
		{
			case tiObject:
				{
					HTREEITEM hChildFirst = nullptr, hChild = hChildFirst = TreeView_GetChild( m_tv, hTreeItem );
					do
					{
						CheckItem( hChild, fCheck );
						m_tv.CheckItem( hChild, fCheck );
						hChild = TreeView_GetNextSibling( m_tv, hChild );
					} while ( hChild && hChild != hChildFirst );
					break;
				}
			case tiLink:
				{
					HTREEITEM hParent = TreeView_GetParent( m_tv, hTreeItem );
					if ( hParent && !fCheck )
					{
						m_tv.CheckItem( hParent, fCheck );
					}

					m_mLinks[ pItem->pObject ][ pItem->bWorld ].second = fCheck;

					break;
				}
		}
	}
}

VOID CLinkManagerDlg::Set_Param( CNesLevelsKeeper ** pKeepers, const LEVEL_ITEM_LINK & lnk, CNesLevel * pCurrentLevel, BYTE bWorld )
{
	for ( BYTE n = asWorld1to4; n < asWorldSetsCount; ++n )
	{
		m_pKeepers[ n ] = pKeepers[ n ];
	}

	m_mLinks = lnk;
	m_bWorld = bWorld;
	m_pCurrentLevel = pCurrentLevel;
}

VOID CLinkManagerDlg::Get_Param( LEVEL_ITEM_LINK & lnk )
{
	lnk = m_mLinks;
}