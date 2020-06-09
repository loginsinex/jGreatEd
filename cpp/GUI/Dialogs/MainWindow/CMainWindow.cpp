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


CMainWindow::CMainWindow( HINSTANCE hInstance )
	: CWindow( hInstance, TEXT( "jGreatEd_class" ) ),
	m_status( hInstance, IDCU_MAINWINDOW_STATUS, TRUE, TRUE, TRUE ),
	m_menu( hInstance, IDC_PROJECT ),
	m_tb( hInstance, IDCU_MAINWINDOW_TOOLBAR, 0, 0, 0, 0, TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | WS_TABSTOP | WS_VISIBLE ),
	m_rb( hInstance, IDCU_MAINWINDOW_REBAR, 0, 0, 0, 0, WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_AUTOSIZE | CCS_TOP | CCS_ADJUSTABLE | RBS_BANDBORDERS | WS_VISIBLE ),
	m_canvas( hInstance, IDCU_MAINWINDOW_CANVAS, 7, 30, -7, -30, WS_VISIBLE | WS_TABSTOP ),
	m_nNames( hInstance ), m_dlgDemo( hInstance )
{
	Register( CS_DBLCLKS, NULL, LoadCursor( NULL, IDC_ARROW ), (HBRUSH)( COLOR_BTNFACE + 1 ) );

	pushctl( m_status );
	pushctl( m_canvas );
	pushctl( m_tb );
	pushctl( m_rb );
}

BOOL CMainWindow::OnCreate( LPCREATESTRUCT lpcs )
{
	CBitmap bmIcons( GetInstance(), MAKEINTRESOURCE( IDB_ICONS ) );
	HBITMAP hBmIcons = nullptr;
	HICON hWndIcon = LoadIcon( GetInstance(), MAKEINTRESOURCE( IDI_SMALL ) );

	hBmIcons = bmIcons.Bitmap();

	wSendMessage( WM_SETICON, ICON_BIG, (LPARAM)hWndIcon );
	wSendMessage( WM_SETICON, ICON_SMALL, (LPARAM)hWndIcon );

	m_tb.SetImageList( hBmIcons );
	SetMenu( *this, m_menu );
	::DeleteBitmap( hBmIcons );

	m_tb.AddButton( 12, ID_FILE_OPEN, nullptr, 0, TRUE, IDS_FILE_OPEN );
	m_tb.AddButton( 13, ID_FILE_SAVE, nullptr, 0, FALSE, IDS_FILE_SAVE );
	m_tb.AddSeparator();
	m_tb.AddButton( 11, ID_EDIT_INSERTOBJECT, nullptr, 0, FALSE, IDS_EDIT_INSERTOBJECT );
	m_tb.AddButton( 10, ID_EDIT_DELETEOBJECT, nullptr, 0, FALSE, IDS_EDIT_DELETEOBJECT );
	m_tb.AddSeparator();
	m_tb.AddButton( 29, ID_EDIT_CREATENEWAREA, nullptr, 0, FALSE, IDS_EDIT_CREATENEWAREA );
	m_tb.AddButton( 21, ID_EDIT_EDITWORLDCONFIGURATION, nullptr, 0, FALSE, IDS_EDIT_EDITWORLDCONFIGURATION );
	m_tb.AddSeparator();
	m_tb.AddButton( 4, ID_VIEW_SELECTLEVEL, nullptr, 0, FALSE, IDS_VIEW_SELECTLEVEL );
	m_tb.AddButton( 1, ID_COMMAND_RUNEMULATORWITHROM, nullptr, 0, FALSE, IDS_COMMAND_RUNEMULATORWITHROM );
	m_tb.AddSeparator();
	m_tb.AddButton( 37, ID_TOOLS_PALETTEEDITOR, nullptr, 0, FALSE, IDS_TOOLS_PALETTEEDITOR );
	m_tb.AddButton( 3, ID_TOOLS_BACKGROUNDCOLORSEDITOR, nullptr, 0, FALSE, IDS_TOOLS_BACKGROUNDCOLORSEDITOR );
	m_tb.AddButton( 34, ID_TOOLS_DEMO, nullptr, 0, FALSE, IDS_TOOLS_DEMO );
	m_tb.AddButton( 23, ID_TOOLS_GAMESSETTINGS, nullptr, 0, FALSE, IDS_TOOLS_GAMESSETTINGS );
	m_tb.AddSeparator();
	m_tb.AddButton( 16, IDM_ABOUT, nullptr, 0, TRUE, IDS_ABOUT );

	m_tb.Visible( TRUE );
	m_rb.InsertBand( m_tb, 0, TRUE );
	m_status.SetParts( 300, 300, 350, 350, 0 );
	wSendMessage( WM_UPDATEUISTATE, MAKELONG( UIS_SET, UISF_HIDEACCEL | UISF_HIDEFOCUS ) );

	OnRebarHeightChange();

	return TRUE;
}

VOID CMainWindow::OnDestroy()
{
	delete m_canvas.GetView();
}

VOID CMainWindow::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( 1 == uCmd )
	{
		OnButton( uId );
	}
}

INT_PTR CMainWindow::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		
		case IDCU_MAINWINDOW_CANVAS:
			{
				switch ( lpnm->code )
				{
					case NCVN_OBJECT_HILITE: return Ctl_OnHilite( (PNCVNVIEW)lpnm );
					case NCVN_OBJECT_SELECT: return Ctl_OnSelect( (PNCVNVIEW)lpnm );
					case NCVN_REDRAW_REQUEST: return Ctl_OnRedraw( (PNCVNVIEW)lpnm );
					case NCVN_RIGHT_CLICK: return Ctl_OnRightClick( (PNCVNVIEW)lpnm );
					case NCVN_DBL_CLICK: return Ctl_OnDblClick( (PNCVNVIEW)lpnm );
					case NCVN_MAP_MOVE: return Ctl_OnMapMove( (PNCNVIEWMAP)lpnm );
					case NCVN_BEGIN_MOVE: return Ctl_OnBeginMove( (PNCVNVIEW)lpnm );
				}
				break;
			}
		case IDCU_MAINWINDOW_REBAR:
			{
				switch ( lpnm->code )
				{
					case RBN_HEIGHTCHANGE: OnRebarHeightChange(); break;
				}
				break;
			}
		default:
			{
				if ( TTN_GETDISPINFOA == lpnm->code || TTN_GETDISPINFOW == lpnm->code )
				{
					Ctl_OnToolbarTip( lpnm->idFrom, (LPNMTTDISPINFO)lpnm, ( TTN_GETDISPINFOA == lpnm->code ) );
				}
				break;
			}
	}

	return 0;
}

VOID CMainWindow::OnSize( INT dwState, USHORT cx, USHORT cy )
{
	OnRebarHeightChange();
}

VOID CMainWindow::OnSizing( INT dwState, USHORT cx, USHORT cy )
{
	OnSize( dwState, cx, cy );
}

VOID CMainWindow::OnRebarHeightChange()
{
	RECT rcWnd = { 0 }, rcStatus = { 0 }, rcRebar = { 0 };
	GetClientRect( *this, &rcWnd );
	GetWindowRect( m_status, &rcStatus );
	GetWindowRect( m_rb, &rcRebar );

	SetWindowPos( m_canvas, NULL, 0, rcRebar.bottom - rcRebar.top,
				  rcWnd.right - rcWnd.left,
				  rcWnd.bottom - ( rcStatus.bottom - rcStatus.top ) - ( rcRebar.bottom - rcRebar.top ), 0 );
}

VOID CMainWindow::LoadGame()
{
	m_menu.EnableItems( TRUE, ID_FILE_SAVE, ID_FILE_SAVEAS, ID_VIEW_SELECTLEVEL, ID_EDIT_INSERTOBJECT, ID_EDIT_DELETEOBJECT,
		ID_EDIT_LOADAREAFROMFILE, ID_EDIT_DUMPAREAINTOAFILE,
		ID_EDIT_CREATENEWAREA, ID_EDIT_EDITWORLDCONFIGURATION,
		ID_EDIT_SELECTALL, ID_TOOLS_PALETTEEDITOR, ID_TOOLS_BACKGROUNDCOLORSEDITOR,
		ID_TOOLS_AREAPOINTERSMANAGER, ID_ADVANCED_UNUSEDAREAS, ID_TOOLS_DEMO, ID_TOOLS_GAMESSETTINGS,
		ID_COMMAND_RUNEMULATORWITHROM,
		0 );

	m_tb.EnableAll( TRUE, ID_FILE_SAVE, ID_FILE_SAVEAS, ID_VIEW_SELECTLEVEL, ID_EDIT_INSERTOBJECT, ID_EDIT_DELETEOBJECT,
		ID_EDIT_LOADAREAFROMFILE, ID_EDIT_DUMPAREAINTOAFILE,
		ID_EDIT_CREATENEWAREA, ID_EDIT_EDITWORLDCONFIGURATION,
		ID_EDIT_SELECTALL, ID_TOOLS_PALETTEEDITOR, ID_TOOLS_BACKGROUNDCOLORSEDITOR,
		ID_TOOLS_AREAPOINTERSMANAGER, ID_ADVANCED_UNUSEDAREAS, ID_TOOLS_DEMO, ID_TOOLS_GAMESSETTINGS,
		ID_COMMAND_RUNEMULATORWITHROM,
		0 );

	m_menu.EnableItems( FALSE, ID_FILE_OPEN, 0 );
	m_tb.EnableAll( FALSE, ID_FILE_OPEN, 0 );

	UpdateBufferMenu();
	LoadLevel( 0, static_cast<BYTE>( 0 ) );
}

VOID CMainWindow::LoadSubLevel( BYTE bWorld, CNesLevel * pLevel )
{
	if ( m_nEditor.IsFileLoaded() )
	{
		const NES_AREA_PTR aptr = pLevel->AreaPtr();
		CNesLevelInterface * pInterface = m_canvas.GetView();

		m_canvas.DelView();
		delete pInterface;

		m_cursel.pSublevel = pLevel;
		m_canvas.SetView( m_nEditor.GetInterface( m_bmLevelView, bWorld, pLevel ) );
		m_canvas.RenderView();
		InvalidateRect( m_canvas, nullptr, TRUE );
		UpdateHeader();

		UpdateUndoMenu();
		m_status.SetText( 3, GetWarpZoneString() );
	}
	return VOID();
}


VOID CMainWindow::LoadLevel( BYTE bWorld, BYTE bLevel, CNesLevel * pLevel )
{
	if ( m_nEditor.IsFileLoaded() )
	{
		m_cursel.bWorld = bWorld;
		m_cursel.nLevel = bLevel;
		m_cursel.pLevel = pLevel;
		LoadSubLevel( bWorld, pLevel );
	}
}


VOID CMainWindow::LoadLevel( BYTE bWorld, BYTE bLevel )
{
	if ( m_nEditor.IsFileLoaded() )
	{
		std::vector<CNesLevel*> vpLevels;
		if ( m_nEditor.GetLevelAreas( bWorld, bLevel, vpLevels ) )
		{
			LoadLevel( bWorld, bLevel, vpLevels[ 0 ] );
		}
	}
}

VOID CMainWindow::UpdateBufferMenu()
{
	std::vector<CNesObject*> vpObject;
	m_menu.EnableItems( m_canvas.GetSelectedObject( vpObject ), ID_EDIT_COPY, ID_EDIT_CUT, 0 );
	m_menu.EnableItems( m_buffer.CanPaste(), ID_EDIT_PASTE, 0 );
}

VOID CMainWindow::UpdateUndoMenu()
{
	m_menu.EnableItems( m_nEditor.IsLevelCanReverted( m_cursel.pSublevel ), ID_EDIT_UNDO, 0 );
}

CString CMainWindow::GetWarpZoneString()
{
	CString str;
	BYTE bWorld = m_nEditor.WarpTarget( m_cursel.bWorld, m_cursel.nLevel, m_cursel.pSublevel ) + 1;
	str.Format( TEXT( "Warp target world: #%c" ), ( bWorld < 10 ? _T( '0' ) : _T( 'A' ) - 10 ) + bWorld );
	return str;
}

VOID CMainWindow::UpdateHeader()
{
	CString strHeader;

	if ( 0xFF != m_cursel.nLevel )
	{
		strHeader.Format( TEXT( "%s - %s, World %wc-%d" ),
			Str( IDS_APP_TITLE ).c_str(),
			PathFindFileName( m_nEditor.Filename() ),
			( m_cursel.bWorld < 9 ? ( _T( '0' ) + m_cursel.bWorld + 1 ) : ( _T( 'A' ) + ( m_cursel.bWorld - 9 ) ) ),
			m_cursel.nLevel + 1 );
	}
	else
	{
		strHeader.Format( TEXT( "%s - %s, World %wc" ),
			Str( IDS_APP_TITLE ).c_str(),
			PathFindFileName( m_nEditor.Filename() ),
			( m_cursel.bWorld < 9 ? ( _T( '0' ) + m_cursel.bWorld + 1 ) : ( _T( 'A' ) + ( m_cursel.bWorld - 9 ) ) ) );
	}

	wSendMessage( WM_SETTEXT, 0, (LPARAM)strHeader.GetString() );

	m_status.SetText( 0, PathFindFileName( m_nEditor.Filename() ) );
}

VOID CMainWindow::AreaLinkManager()
{
	size_t id = 1;
	CNesLevelItem * pObject = static_cast<CNesLevelItem*>( m_cursel.pSublevel->ObjectById( id++, FALSE ) );
	LEVEL_ITEM_LINK mLinks;

	std::map<BYTE, std::pair<UINT, CNesLevel*>> mAreaLinks;
	m_cursel.pSublevel->ReadAreaLinks( mAreaLinks );
	if ( mAreaLinks.size() > 0 )
	{
		WORLD_OBJECT_LINK & wol = mLinks[ nullptr ];
		for ( const auto & lnk : mAreaLinks )
		{
			wol[ lnk.first ] = { lnk.second, FALSE };
		}
	}

	while ( pObject )
	{
		if ( pObject->IsEnterableObject() )
		{
			WORLD_OBJECT_LINK & wol = mLinks[ pObject ];
			std::map<BYTE, std::pair<UINT, CNesLevel*>> mObjLinks;
			m_cursel.pSublevel->ReadItemLinks( pObject, mObjLinks );
			for ( const auto & lnk : mObjLinks )
			{
				wol[ lnk.first ] = { lnk.second, FALSE };
			}
		}
		pObject = static_cast<CNesLevelItem*>( m_cursel.pSublevel->ObjectById( id++, FALSE ) );
	};

	if ( !mLinks.size() )
	{
		ShowError( TEXT( "No pointers found in this area" ) );
		return;
	}

	CNesLevelsKeeper * pKeepers[ asWorldSetsCount ] =
	{
		m_nEditor.GetAreaList( 0 ),
		m_nEditor.GetAreaList( 4 ),
		m_nEditor.GetAreaList( 8 ),
		m_nEditor.GetAreaList( 9 )
	};

	CLinkManagerDlg dlg( GetInstance() );
	dlg.Set_Param( pKeepers, mLinks, m_cursel.pSublevel, m_cursel.bWorld );

	if ( dlg.Show( this ) )
	{
		TakeLevelSnapshot();
		dlg.Get_Param( mLinks );

		for ( const auto & obj : mLinks )
		{
			CNesLevelItem * pItem = obj.first;
			for ( const auto & wol : obj.second )
			{
				const BOOL fDelete = wol.second.second;
				const BYTE bWorld = wol.first;

				if ( fDelete )
				{
					if ( pItem )
					{
						pItem->RemoveLink( bWorld );
					}
					else
					{
						m_cursel.pSublevel->RemoveAreaLink( bWorld );
					}
				}
			}
		}

		m_nEditor.PointersChanged();

		m_canvas.RenderView();
		InvalidateRect( m_canvas, nullptr, TRUE );
	}

	for ( auto pKeeper : pKeepers )
	{
		delete pKeeper;
	}
}

// obj buffer
CObjBuffer::CObjBuffer()
{

}

VOID CObjBuffer::Copy( std::vector<CNesObject*>& vObject )
{
	POINT leftPoint = { 1024, 1024 };
	m_vObjects.clear();

	for ( auto pObj : vObject )
	{
		BUFFER_OBJECT bo = { 0 };
		const POINT ptObject = pObj->GetRealPoint();

		if ( ptObject.x < leftPoint.x ) leftPoint.x = ptObject.x;
		if ( ptObject.y < leftPoint.y ) leftPoint.y = ptObject.y;


		if ( pObj->IsSprite() )
		{
			CNesEnemyObject * pEnemy = static_cast<CNesEnemyObject*>( pObj );
			bo.fObject = TRUE;
			bo.nObject.item.objectId = pEnemy->Item().item.objectId;
			bo.nObject.pt = ptObject;
			bo.nObject.item.Y = LOBYTE( pEnemy->GetRealPoint().y );
		}
		else
		{
			CNesLevelItem * pItem = static_cast<CNesLevelItem*>( pObj );
			bo.fObject = FALSE;
			bo.nItem.item.itemId = pItem->Item().item.itemId;
			bo.nItem.pt = ptObject;
			bo.nItem.item.Y = LOBYTE( pItem->GetRealPoint().y );
		}

		m_vObjects.push_back( bo );
	}

	// normalize points
	for ( auto & bo : m_vObjects )
	{
		if ( bo.fObject )
		{
			bo.nObject.pt.x -= leftPoint.x;
			bo.nObject.pt.y -= leftPoint.y;
		}
		else
		{
			bo.nItem.pt.x -= leftPoint.x;
			bo.nItem.pt.y -= leftPoint.y;
		}
	}
}


BOOL CObjBuffer::Paste( CNesLevel * pLevel, const POINT & pt, std::vector<CNesObject*> & vpCreatedObjects )
{
	vpCreatedObjects.clear();
	int uMaxX = 16 * pLevel->PagesCount() - 1;

	for ( const auto & bo : m_vObjects )
	{
		CNesObject * pObj = nullptr;
		if ( bo.fObject )
		{
			NES_LEVEL_ENEMY_ITEM nEnemy = bo.nObject;
			nEnemy.pt.x = min( pt.x + nEnemy.pt.x, uMaxX );
			nEnemy.pt.y = min( NES_SPECIAL_OBJECT_D - 1, static_cast<int>( nEnemy.pt.y ) + pt.y );
			pObj = pLevel->AddEnemyItem( nEnemy );
		}
		else
		{
			NES_LEVEL_ITEM nItem = bo.nItem;
			nItem.pt.x = min( pt.x + nItem.pt.x, uMaxX );

			if ( nItem.item.Y < NES_SPECIAL_OBJECT_C )
			{
				nItem.pt.y = min( NES_SPECIAL_OBJECT_C - 1, static_cast<int>( nItem.pt.y ) + pt.y );
			}
			else
			{
				nItem.pt.y = nItem.item.Y;
			}
			pObj = pLevel->AddLevelItem( nItem );
		}

		if ( pObj )
		{
			vpCreatedObjects.push_back( pObj );
		}
	}

	return ( vpCreatedObjects.size() > 0 );
}

BOOL CObjBuffer::CanPaste()
{
	return ( m_vObjects.size() > 0 );
}


VOID CMainWindow::InsertObject( POINT pt )
{
	CNesLevel * pCopy = m_cursel.pSublevel->Copy();
	if ( pCopy )
	{
		CNewObjectDlg dlg( GetInstance() );
		CPaletteBitmap bm;
		CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, pCopy );

		dlg.Set_Param( pCopy, pInterface, m_cursel.bWorld, pt );

		if ( dlg.Show( this ) )
		{
			CNesObject * pNewObject = nullptr;

			TakeLevelSnapshot();
			if ( !dlg.IsSpriteCreated() )
			{
				NES_LEVEL_ITEM item = { 0 };
				dlg.Get_Param( item );
				pNewObject = m_cursel.pSublevel->AddLevelItem( item );
			}
			else
			{
				NES_LEVEL_ENEMY_ITEM item = { 0 };
				dlg.Get_Param( item );
				pNewObject = m_cursel.pSublevel->AddEnemyItem( item );
			}

			if ( !pNewObject )
			{
				PopLevelSnapshot();
				ShowError( TEXT( "Too many objects for this area.\nCan't add new object." ) );
			}
			else
			{
				m_canvas.RenderView();
				InvalidateRect( m_canvas, nullptr, TRUE );
			}
		}

		delete pInterface;
		delete pCopy;
	}
}

VOID CMainWindow::DeleteObject()
{
	CNesObjectsNames nm( GetInstance() );
	std::vector<CNesObject*> vpObject;
	if ( !m_canvas.GetSelectedObject( vpObject ) )
	{
		return;
	}

	CNesObject * pObj = vpObject[ 0 ];
	const BOOL fMultipleSelection = ( vpObject.size() > 1 );

	CString str, strFormat;
	if ( !fMultipleSelection )
	{
		if ( pObj->IsSprite() )
			str = nm.GetNameOfEnemy( (CNesEnemyObject*)pObj );
		else
			str = nm.GetNameOfItem( (CNesLevelItem*)pObj );

		strFormat.Format( TEXT( "Are you sure that you want to delete '%s'?" ), str );
	}
	else
	{
		strFormat = TEXT( "Are you sure that you want to delete selected objects?" );
	}

	if ( ConfirmF( strFormat, TRUE, str.GetString() ) )
	{
		DeleteObjects( vpObject );
	}
}

VOID CMainWindow::DeleteObjects( std::vector<CNesObject*>& vpObj )
{
	TakeLevelSnapshot();
	for ( auto pObject : vpObj )
	{
		m_cursel.pSublevel->DeleteObject( pObject );
	}

	m_canvas.DropSelection();
	m_canvas.RenderView();
	InvalidateRect( m_canvas, nullptr, TRUE );
}

VOID CMainWindow::ChangePointer( CNesLevelItem * pObject )
{
	CNesLevelsKeeper * pKeeper = m_nEditor.GetAreaList( m_cursel.bWorld );
	CAreasDlg dlg( GetInstance(), pKeeper );
	dlg.Set_Obj( m_cursel.bWorld, pObject );
	if ( dlg.Show( this ) )
	{
		CNesLevel * pLevel = nullptr;
		BYTE uTargetPage = 0;
		BOOL fFreePointers = FALSE;

		dlg.Get_Param( &pLevel, uTargetPage, fFreePointers );

		TakeLevelSnapshot();

		if ( fFreePointers && etVerticalPipe == pObject->EntranceType() )
		{
			pObject->RemoveLinks();
			m_canvas.RenderView();
		}
		else if ( pKeeper->SetObjectTarget( m_cursel.bWorld, pObject, pLevel, uTargetPage ) )
		{
			m_canvas.RenderView();
		}
		InvalidateRect( m_canvas, nullptr, TRUE );
	}

	delete pKeeper;
}


VOID CMainWindow::IncreaseAreaSize( UINT uPage )
{
	TakeLevelSnapshot();
	m_nEditor.AddPage( m_cursel.pSublevel, uPage );
	m_canvas.RenderView();
}

VOID CMainWindow::DecreaseAreaSize( UINT uPage )
{
	if ( Confirm( TEXT( "WARNING: All objects within this page will be removed.\nDo you want to continue?" ) ) )
	{
		TakeLevelSnapshot();
		if ( !m_nEditor.DelPage( m_cursel.pSublevel, uPage ) )
		{
			PopLevelSnapshot();
			ShowError( TEXT( "Page cannot be deleted,\nbecause some pointers referenced to it." ) );
		}
		else
		{
			m_canvas.RenderView();
		}
	}
}

VOID CMainWindow::ShowReferences( UINT uPage )
{
	if ( m_nEditor.IsFileLoaded() )
	{
		std::vector<std::pair<BYTE, CNesLevel*>> vLinks;

		if ( m_nEditor.GetSourceLinkList( m_cursel.pSublevel, uPage, vLinks ) )
		{
			CPaletteBitmap bm;
			CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, m_cursel.pSublevel );
			CPageReferencesDlg dlg( GetInstance() );
			std::pair<BYTE, CNesLevel*> tg;

			dlg.Set_Param( pInterface, uPage, m_cursel.pSublevel->PagesCount() - 1, vLinks );
			LRESULT result = dlg.Show( this );

			if ( result )
			{
				UINT uNewPage = 0;
				BOOL fResult = TRUE;
				dlg.Get_Param( uNewPage, tg );
				TakeLevelSnapshot();
				switch ( result )
				{
					case 1: fResult = m_nEditor.SetSourcePageLink( m_cursel.pSublevel, uPage, uNewPage ); break;
					case 2: LoadLevel( tg.first, 0xFF, tg.second ); break;
				}

				if ( !fResult )	// WARNING! We can change loaded area in case of result == 2 ! DONT POP THE SNAPSHOT!
				{
					PopLevelSnapshot();
				}
			}

			delete pInterface;
		}
	}
}

VOID CMainWindow::LoopSettings( CNesLoopCommand * pLoop )
{
	CNesLevel * pCopy = m_cursel.pSublevel->Copy();
	if ( pCopy )
	{
		CLoopSettingsDlg dlg( GetInstance() );
		CPaletteBitmap bm;
		CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, pCopy );
		size_t uObjectId = 0;
		BOOL fSprite = FALSE;

		if ( pCopy->FindLoopObjectId( pLoop, uObjectId ) )
		{
			NES_LOOP nlp = { 0 };
			pLoop->DumpLoop( nlp );
			dlg.Set_Param( pCopy, pInterface, uObjectId );
			if ( dlg.Show( this ) )
			{
				TakeLevelSnapshot();
				dlg.Get_Param( nlp );
				pLoop->TriggerPage( nlp.bPageNumber );
				pLoop->AllowedHeight( nlp.bHeight / 16 );
				pLoop->LoopConnected( nlp.bSlaveData );	// bSlaveData is not realy means connection loop, but now can be used as connection flag
				pLoop->InitObject();
				m_canvas.RenderView();
				InvalidateRect( m_canvas, nullptr, TRUE );
			}
		}
		delete pInterface;
		delete pCopy;
	}
}


VOID CMainWindow::ModifyGround( int x, BOOL fNewGround )
{
	NES_GROUND_INFO ngi = { 0 };
	CPaletteBitmap bm;
	CNesLevel * pLevel = m_cursel.pSublevel->Copy();
	if ( pLevel )
	{
		CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, pLevel );
		CGroundDlg dlg( GetInstance() );
		NES_GROUND_PLACE ngp = { 0 };
		std::map<NES_GROUND_TYPE, NES_GROUND_INFO> mngi;

		if ( !fNewGround )
		{
			m_cursel.pSublevel->GetGround( x, mngi );
		}
		else
		{
			ngi.bGroundCtl = 1;
			mngi[ ngtGroundMod ] = ngi;			
		}

		ngp.X = x;
		ngp.gr = std::move( mngi );
		dlg.Set_Param( pLevel, pInterface, ngp );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( ngp );
			TakeLevelSnapshot();
			m_cursel.pSublevel->SetGround( x, ngp.gr );
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}

		delete pInterface;
		delete pLevel;
	}
}

VOID CMainWindow::RemoveGround( int x )
{
	if ( Confirm( TEXT( "Remove this ground modifier?" ) ) )
	{
		TakeLevelSnapshot();
		if ( m_cursel.pSublevel->EraseGround( x ) )
		{
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}
		else
		{
			PopLevelSnapshot();
			ShowAlert( TEXT( "Unable to delete this modifier!" ) );
		}
	}
}

VOID CMainWindow::ChangeAreaPointer()
{
	if ( !m_cursel.pSublevel->Header().IsCloudLevel() )
	{
		return;
	}

	CNesLevelsKeeper * pKeeper = m_nEditor.GetAreaList( m_cursel.bWorld );
	CNesLevel * pTargetLevel = nullptr;
	BYTE uPage = 0;
	m_cursel.pSublevel->GetAreaPointer( m_cursel.bWorld, &pTargetLevel, uPage );

	CAreasDlg dlg( GetInstance(), pKeeper );

	dlg.Set_Obj( m_cursel.bWorld, m_cursel.pSublevel, pTargetLevel, uPage );

	if ( dlg.Show( this ) )
	{
		BOOL dummy = FALSE;
		dlg.Get_Param( &pTargetLevel, uPage, dummy );
		TakeLevelSnapshot();
		m_cursel.pSublevel->SetAreaPointer( m_cursel.bWorld, pTargetLevel, uPage );
		m_canvas.RenderView();
		InvalidateRect( m_canvas, nullptr, TRUE );
	}

	delete pKeeper;

}

VOID CMainWindow::FollowTheAreaPointer()
{
	CNesLevel * pTargetLevel = nullptr;
	BYTE uPage = 0;

	if ( !m_cursel.pSublevel->Header().IsCloudLevel() )
	{
		return;
	}

	if ( m_cursel.pSublevel->GetAreaPointer( m_cursel.bWorld, &pTargetLevel, uPage ) )
	{
		// pointer can be NULL for current world
		if ( pTargetLevel )
		{
			LoadSubLevel( m_cursel.bWorld, pTargetLevel );
		}
		m_canvas.ScrollTo( uPage );
	}
	else
	{
		m_canvas.ScrollTo( 0 );
	}
}

VOID CMainWindow::AreaSettings()
{
	CNesLevel * pLevel = m_cursel.pSublevel->Copy();
	if ( pLevel )
	{
		CPaletteBitmap bm;
		CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, pLevel );
		CAreaPropertiesDlg dlg( GetInstance() );

		dlg.Set_Param( pLevel, pInterface );

		if ( dlg.Show( this ) )
		{
			CNesLevelHeader header;
			CNesLevel * pTargetLevel = nullptr;
			BYTE uTargetPage = 0;
			dlg.Get_Param( header );
			TakeLevelSnapshot();
			m_cursel.pSublevel->UpdateHeader( header );
			m_canvas.DropSelection();
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}

		delete pInterface;
		delete pLevel;
	}
}

VOID CMainWindow::ChangeLevelType()
{
	CNesLevel * pLevel = m_cursel.pSublevel->Copy();
	if ( pLevel )
	{
		CPaletteBitmap bm;
		CNesLevelInterface * pInterface = m_nEditor.GetInterface( bm, m_cursel.bWorld, pLevel );
		CChangeAreaTypeDlg dlg( GetInstance() );
		dlg.Set_Param( pLevel, pInterface );

		if ( dlg.Show( this ) )
		{
			NES_LEVEL_TYPE type;
			dlg.Get_Param( type );
			TakeLevelSnapshot();
			m_nEditor.UpdateAreaType( m_cursel.pSublevel, type );
			LoadSubLevel( m_cursel.bWorld, m_cursel.pSublevel );
		}

		delete pLevel;
		delete pInterface;
	}
}

VOID CMainWindow::TakeLevelSnapshot()
{
	m_nEditor.TakeLevelSnapshot( m_cursel.pSublevel );
	UpdateUndoMenu();
}

VOID CMainWindow::PopLevelSnapshot()
{
	m_nEditor.RevertLevelState( m_cursel.pSublevel );
	UpdateUndoMenu();
}