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
#include <shellapi.h>

VOID CMainWindow::OnButton( USHORT uId )
{
	BOOL fUpdateFocus = TRUE;
	switch ( uId )
	{
		case ID_FILE_OPEN: Ctl_OnOpen(); break;
		case ID_FILE_SAVE: Ctl_OnSave( FALSE ); break;
		case ID_FILE_SAVEAS: Ctl_OnSave( TRUE ); break;
		case ID_VIEW_SELECTLEVEL: Ctl_OnSelectLevel(); break;
		case ID_EDIT_DELETEOBJECT: Ctl_OnDeleteObject(); break;
		case ID_EDIT_INSERTOBJECT: Ctl_OnInsertObject(); break;
		case ID_EDIT_LOADAREAFROMFILE: Ctl_OnLoadAreaFromFile(); break;
		case ID_EDIT_DUMPAREAINTOAFILE: Ctl_OnDumpAreaIntoFile(); break;
		case ID_EDIT_CREATENEWAREA: Ctl_OnCreateNewArea(); break;
		case ID_EDIT_EDITWORLDCONFIGURATION: Ctl_OnWorldConfig(); break;
		case ID_EDIT_SELECTALL: Ctl_OnSelectAll(); break;
		case ID_EDIT_COPY: Ctl_OnBufferEvent( beCopy ); break;
		case ID_EDIT_CUT: Ctl_OnBufferEvent( beCut ); break;
		case ID_EDIT_PASTE: Ctl_OnBufferEvent( bePaste ); break;
		case ID_AREA_SETTINGS: Ctl_OnAreaSettings(); break;
		case ID_AREA_TYPE: Ctl_OnChangeAreaType(); break;
		case ID_TOOLS_PALETTEEDITOR: Ctl_OnPaletteEditor(); break;
		case ID_TOOLS_BACKGROUNDCOLORSEDITOR: Ctl_OnBgColorEditor(); break;
		case ID_EDIT_UNDO: Ctl_OnUndo(); break;
		case ID_TOOLS_AREAPOINTERSMANAGER: Ctl_OnAreaLinkManager(); break;
		case ID_ADVANCED_UNUSEDAREAS: Ctl_OnShowLimits(); break;
		case ID_TOOLS_DEMO: Ctl_OnDemoPlay(); fUpdateFocus = FALSE; break;
		case ID_TOOLS_GAMESSETTINGS: Ctl_OnGameSettings(); break;
		case ID_COMMAND_RUNEMULATORWITHROM: Ctl_OnExec(); break;
		case IDM_ABOUT: Ctl_OnAbout(); break;
		case IDM_EXIT: Ctl_OnExit(); break;
	}

	if ( fUpdateFocus )
	{
		SetFocus( m_canvas );
	}
}

VOID CMainWindow::Ctl_OnAbout()
{
	CAboutDlg dlgAbout( GetInstance() );
	dlgAbout.Show( this );
}

VOID CMainWindow::OnCancel()
{
	Ctl_OnExit();
}

BOOL CMainWindow::OnClose()
{
	Ctl_OnExit();
	return 0;
}

VOID CMainWindow::Ctl_OnOpen()
{
	DWORD w = 0;
	if ( m_nEditor.IsFileLoaded() )
	{
		return;
	}

	try
	{
		CString strFile = OpenFile( TEXT( "Super Mario Brothers 2 (Japan).fds" ), TEXT( "Family Disk System ROM (*.fds)" ), TEXT( "*.fds" ), 0 ).c_str();

		if ( strFile.GetLength() > 0 )
		{
			m_nEditor.LoadFile( strFile );
			LoadGame();
		}
	}
	catch ( std::exception & e )
	{
		CString sMessage = e.what();
		ShowErrorF( TEXT( "Loader error. Is the file corrupted or not valid FDS file?\n\n%s" ), sMessage.GetString() );
	}
}

VOID CMainWindow::Ctl_OnSave( BOOL fSaveAs )
{
	CString sFile;
	if ( fSaveAs )
	{
		sFile = SaveFile( m_nEditor.Filename(), TEXT( "Family Disk System ROM (*.fds)" ), TEXT( "*.fds" ), 0 ).c_str();
		if ( !sFile.GetLength() )
		{
			return;
		}
	}

	try
	{
		m_nEditor.SaveFile( sFile.GetLength() ? sFile.GetString() : nullptr );
		UpdateHeader();
	}
	catch ( std::exception & e )
	{
		CString sMessage = e.what();
		ShowErrorF( TEXT( "Unable to save the file.\n\n%s" ), sMessage.GetString() );
	}
}

VOID CMainWindow::Ctl_OnSelectLevel()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		NES_GAME_AREA_LIST nAreaList;
		NES_GAME_AREA_HP_LIST __unused;
		CSelectLevelDlg dlg( GetInstance() );

		m_nEditor.GetAreaList( nAreaList, __unused );
		dlg.SetParam( nAreaList, m_cursel.bWorld, m_cursel.nLevel, m_cursel.pLevel );

		if ( dlg.Show( this ) )
		{
			dlg.GetParam( &m_cursel.pLevel, m_cursel.bWorld, m_cursel.nLevel );
			LoadLevel( m_cursel.bWorld, m_cursel.nLevel, m_cursel.pLevel );
		}
	}
}


VOID CMainWindow::Ctl_OnExit()
{
	if ( !m_nEditor.IsFileLoaded() )
	{
		Close( 0 );
	}
	else if ( ConfirmF( TEXT( "Do you want to close '%s' now?" ), FALSE, PathFindFileName( m_nEditor.Filename() ) ) )
	{
		Close( 0 );
	}
}

VOID CMainWindow::Ctl_OnToolbarTip( UINT_PTR cmd, LPNMTTDISPINFO lpnmtt, BOOL fAnsi )
{
	TBBUTTONINFO tbi = { 0 };
	tbi.cbSize = sizeof( tbi );
	tbi.dwMask = TBIF_LPARAM;
	m_tb.cSendMessage( TB_GETBUTTONINFO, cmd, (LPARAM)&tbi );

	if ( tbi.lParam )
		lstrcpy( lpnmtt->lpszText, Str( LODWORD( tbi.lParam ) ).c_str() );
}

VOID CMainWindow::Ctl_OnExec()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		ShellExecute( *this, TEXT( "open" ), m_nEditor.Filename(), NULL, NULL, SW_SHOW );
	}
}

VOID CMainWindow::Ctl_OnDeleteObject()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		DeleteObject();
	}
}

VOID CMainWindow::Ctl_OnInsertObject()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		TakeLevelSnapshot();
		InsertObject( m_canvas.GetLastPoint() );
	}
}

VOID CMainWindow::Ctl_OnSelectAll()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		std::vector<CNesObject*> vpObject;
		CNesObject * pObject = nullptr;

		// retreive all objects from level
		size_t idObj = 1;
		while ( pObject = m_cursel.pSublevel->ObjectById( idObj++, FALSE ) )
		{
			vpObject.push_back( pObject );
		}

		idObj = 0;
		while ( pObject = m_cursel.pSublevel->ObjectById( idObj++, TRUE ) )
		{
			vpObject.push_back( pObject );
		}

		// send this list to canvas
		m_canvas.SetSelection( vpObject );
		InvalidateRect( m_canvas, nullptr, TRUE );
		UpdateBufferMenu();
	}
}

VOID CMainWindow::Ctl_OnAreaSettings()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		AreaSettings();
	}
}

VOID CMainWindow::Ctl_OnChangeAreaType()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		ChangeLevelType();
	}
}


VOID CMainWindow::Ctl_OnPaletteEditor()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		struct pl
		{
			NES_LEVEL_TYPE nlt;
			NES_AREA_STYLE nas;
			NES_BG_COLOR_CTL bgModifier;
		};

		std::map<NES_PAL_TABLE, NES_PAL_ARRAY> mPal;
		std::map<NES_PAL_TABLE, pl> mPalId;

		NES_PAL_TABLE table[] = { nptGround, nptNightSnowGround, nptDaySnowGround, nptMushroomGround, nptWater, nptUnderground, nptCastle };

		CPaletteDlg dlg( GetInstance() );

		for ( const auto & t : table )
		{
			pl plData;
			switch ( t )
			{
				case nptGround: plData.nlt = nltGround; plData.nas = areaStyle0; plData.bgModifier = bgCtl0; break;
				case nptWater: plData.nlt = nltWater; plData.nas = areaStyle0; plData.bgModifier = bgCtl0; break;
				case nptUnderground: plData.nlt = nltUnderground; plData.nas = areaStyle0; plData.bgModifier = bgCtl0; break;
				case nptCastle: plData.nlt = nltCastle; plData.nas = areaStyle0; plData.bgModifier = bgCtl0; break;
				case nptMushroomGround: plData.nlt = nltGround; plData.nas = areaStyle1; plData.bgModifier = bgCtl0; break;
				case nptDaySnowGround: plData.nlt = nltGround; plData.nas = areaStyle0; plData.bgModifier = bgCtl2; break;
				case nptNightSnowGround: plData.nlt = nltGround; plData.nas = areaStyle0; plData.bgModifier = bgCtl3; break;
			}
			mPalId[ t ] = plData;

			m_nEditor.GetPalette( plData.nlt, plData.nas, plData.bgModifier, mPal[ t ] );
		}

		dlg.Set_Param( mPal );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( mPal );

			for ( const auto & pal : mPal )
			{
				const auto & plData = mPalId[ pal.first ];
				m_nEditor.SetPalette( plData.nlt, plData.nas, plData.bgModifier, pal.second );
			}

			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}
	}
}

VOID CMainWindow::Ctl_OnBgColorEditor()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		NES_BG_COLORS nbg;
		CPaletteBgDlg dlg( GetInstance() );

		m_nEditor.GetBgColors( m_cursel.bWorld, nbg );
		nbg = m_cursel.pSublevel->ObjData().BgColors();
		dlg.Set_Param( nbg );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( nbg );
			m_nEditor.SetBgColors( m_cursel.bWorld, nbg );
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}
	}
}


VOID CMainWindow::Ctl_OnBufferEvent( BUFFER_EVENT be )
{
	std::vector<CNesObject*> vpObj;
	const BOOL fSelected = m_canvas.GetSelectedObject( vpObj );

	switch ( be )
	{
		case beCopy:
			{
				if ( fSelected )
				{
					m_buffer.Copy( vpObj );
					m_menu.EnableItems( TRUE, ID_EDIT_PASTE, 0 );
				}
				break;
			}
		case beCut:
			{
				if ( fSelected )
				{
					m_buffer.Copy( vpObj );
					m_menu.EnableItems( TRUE, ID_EDIT_PASTE, 0 );
					DeleteObjects( vpObj );
				}
				break;
			}
		case bePaste:
			{
				const POINT pt = m_canvas.GetLastPoint();
				TakeLevelSnapshot();
				if ( m_buffer.Paste( m_cursel.pSublevel, pt, vpObj ) )
				{
					m_canvas.SetSelection( vpObj );
					m_canvas.RenderView();
					InvalidateRect( m_canvas, nullptr, TRUE );
				}
				else
				{
					PopLevelSnapshot();
					ShowError( TEXT( "Too many objects for this area.\nCan't paste object." ) );
				}
				break;
			}
	}

	UpdateBufferMenu();
}

VOID CMainWindow::Ctl_OnGameSettings()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		NES_ENGINE_HACK hack;
		CEngineHackSheet dlg( GetInstance() );

		m_nEditor.GetHack( hack );
		dlg.Set_Param( hack );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( hack );
			m_nEditor.SetHack( hack );
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}
	}
}

VOID CMainWindow::Ctl_OnDemoPlay()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		std::vector<std::pair<BYTE, BYTE>> vDemoPlay;

		if ( !static_cast<HWND>( m_dlgDemo ) )
		{
			m_nEditor.GetDemoData( vDemoPlay );
			m_dlgDemo.Set_Param( vDemoPlay );
			m_dlgDemo.Create( this );
			ShowWindow( m_dlgDemo, SW_SHOW );
			SetForegroundWindow( m_dlgDemo );
		}

		m_dlgDemo.Get_Param( vDemoPlay );
		m_nEditor.SetDemoData( vDemoPlay );
	}
}

VOID CMainWindow::Ctl_OnLoadAreaFromFile()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		CString strFile;
		strFile.Format( TEXT( "area_%Iu_world_%d.bin" ), m_cursel.pSublevel->Metadata().uLevelId, m_cursel.bWorld );
		CString str = OpenFile( strFile, TEXT( "BIN files(*.bin)" ), TEXT( "*.bin" ), TEXT( "All files(*.*)" ), TEXT( "*.*" ), 0 ).c_str();

		if ( str.GetLength() )
		{
			LoadArea( str );
		}
	}
}

VOID CMainWindow::Ctl_OnDumpAreaIntoFile()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		CString strFile;
		strFile.Format( TEXT( "area_%Iu_world_%d.bin" ), m_cursel.pSublevel->Metadata().uLevelId, m_cursel.bWorld );
		CString str = SaveFile( strFile, TEXT( "BIN files(*.bin)" ), TEXT( "*.bin" ), TEXT( "All files(*.*)" ), TEXT( "*.*" ), 0 ).c_str();

		if ( str.GetLength() )
		{
			DumpArea( str );
		}
	}
}

VOID CMainWindow::Ctl_OnWorldConfig()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		CNesLevelsKeeper * pKeepers[ asWorldSetsCount ] =
		{
			m_nEditor.GetAreaList( 0 ),
			m_nEditor.GetAreaList( 4 ),
			m_nEditor.GetAreaList( 8 ),
			m_nEditor.GetAreaList( 9 )
		};

		NES_GAME_AREA_LIST nag;
		NES_GAME_AREA_HP_LIST nhp;
		if ( m_nEditor.GetAreaList( nag, nhp ) )
		{
			CWorldEditorDlg dlg( GetInstance() );
			dlg.Set_Param( nag, nhp, pKeepers, m_cursel.bWorld, m_cursel.nLevel, m_cursel.pSublevel );

			if ( dlg.Show( this ) )
			{
				CNesLevel * pLevel;
				BYTE bWorld = 0, bLevel = 0;
				dlg.Get_Param( nag, nhp, bWorld, bLevel, &pLevel );
				if ( !m_nEditor.SetAreaList( nag, nhp ) )
				{
					ShowAlert( TEXT( "Worlds configuration is invalid" ) );
				}

				if ( !pLevel )
				{
					LoadLevel( bWorld, bLevel );
				}
				else
				{
					LoadLevel( bWorld, bLevel, pLevel );
				}
			}
		}

		for ( auto pKeeper : pKeepers )
		{
			delete pKeeper;
		}
	}
}

VOID CMainWindow::Ctl_OnAreaLinkManager()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		AreaLinkManager();
	}
}


VOID CMainWindow::Ctl_OnCreateNewArea()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		CNesLevelsKeeper * pKeeper = m_nEditor.GetAreaList( m_cursel.bWorld );
		CCreateNewAreaDlg dlg( GetInstance() );
		CNesLevel * pLevel = nullptr;
		BOOL fAutoLevel = FALSE, fSharedSection = FALSE;
		NES_LEVEL_TYPE type = nltWater;
		dlg.Set_Param( m_cursel.bWorld, pKeeper, m_cursel.pSublevel );

		LRESULT code = dlg.Show( this );
		dlg.Get_Param( type, fAutoLevel, fSharedSection, &pLevel );

		switch ( code )
		{
			case 1: pLevel = m_nEditor.CreateLevel( m_cursel.bWorld, type, fSharedSection, fAutoLevel ); break;
			case 2: pLevel = m_nEditor.CreateLevel( m_cursel.bWorld, pLevel ); break;
		}

		if ( code )
		{
			if ( !pLevel )
			{
				ShowError( TEXT( "Too many areas for this type" ) );
			}
			else
			{
				LoadSubLevel( m_cursel.bWorld, pLevel );
			}
		}

		delete pKeeper;
	}
}

VOID CMainWindow::Ctl_OnUndo()
{
	if ( m_nEditor.IsLevelCanReverted( m_cursel.pSublevel ) )
	{
		m_canvas.DropSelection();
		m_nEditor.RevertLevelState( m_cursel.pSublevel );
		m_canvas.RenderView();
		InvalidateRect( m_canvas, nullptr, TRUE );
		UpdateUndoMenu();
	}
}

VOID CMainWindow::Ctl_OnShowLimits()
{
	if ( m_nEditor.IsFileLoaded() )
	{
		std::map<NES_AREA_SET, NES_LIMITS> mLimits;
		CLimitsDlg dlg( GetInstance() );
		m_nEditor.GetLimits( mLimits );

		dlg.Set_Param( mLimits );
		dlg.Show( this );
	}
}

// NOTIFY Controls

INT_PTR CMainWindow::Ctl_OnHilite( PNCVNVIEW pnc )
{
	CString str;

	if ( pnc->pObject )
	{
		RECT rcObject = pnc->pObject->GetObjectRect();

		if ( pnc->pObject->IsSprite() )
		{
			str.Format( TEXT( "Object: %s, [%d:%d]-[%d:%d]" ),
				m_nNames.GetNameOfEnemy( static_cast<CNesEnemyObject*>( pnc->pObject ) ).GetString(),
				rcObject.left, rcObject.top, rcObject.right - rcObject.left, rcObject.bottom - rcObject.top );
		}
		else
		{
			str.Format( TEXT( "Item: %s%s, [%d:%d]-[%d:%d]" ),
				m_nNames.GetNameOfItem( static_cast<CNesLevelItem*> ( pnc->pObject ) ).GetString(),
				( static_cast<CNesLevelItem*> ( pnc->pObject )->IsEntranceObject() ? TEXT(" (Enterable)") : TEXT("") ),
				rcObject.left, rcObject.top, rcObject.right - rcObject.left, rcObject.bottom - rcObject.top );
		}
	}
	else
	{
		str = TEXT( "No item at this point" );
	}

	m_status.SetText( 1, str );

	return 0;
}

INT_PTR CMainWindow::Ctl_OnSelect( PNCVNVIEW pnc )
{
	CString str;
	if ( pnc->pObject && !pnc->pObject->IsSprite() )
	{
		CNesLevelItem * pObj = static_cast<CNesLevelItem*>( pnc->pObject );

		BYTE uPage = 0;
		CNesLevel * pTarget = pObj->GetTargetLevel( m_cursel.bWorld, uPage );
		if ( pTarget )
		{
			str.Format( TEXT( "Points to area #%ld (Type: %d), Page: %d" ),
				pTarget->Metadata().uLevelId + 1, pTarget->AreaType(), uPage );
		}
		else
		{
			str = GetWarpZoneString();
		}
	}

	m_status.SetText( 3, str );
	UpdateBufferMenu();

	return 0;
}

INT_PTR CMainWindow::Ctl_OnRedraw( PNCVNVIEW pnc )
{
	return INT_PTR();
}

INT_PTR CMainWindow::Ctl_OnMapMove( PNCNVIEWMAP pncm )
{
	CString str;
	if ( pncm->tile.bMetatile )
	{
		str.Format(
			TEXT( "Tile: 0x%02X, Attribute: %d, ID: 0x%02X [%d:%d]" ),
			pncm->tile.bMetatile,
			pncm->tile.attr,
			pncm->tile.tileId,
			pncm->pt.x, pncm->pt.y
		);
	}

	m_status.SetText( 2, str );

	return 0;
}

INT_PTR CMainWindow::Ctl_OnDblClick( PNCVNVIEW pncm )
{
	if ( pncm->pObject )
	{
		if ( !pncm->pObject->IsSprite() )
		{
			CNesLevelItem * pObj = static_cast<CNesLevelItem*>( pncm->pObject );
			if ( pObj->IsEnterableObject() )
			{
				BYTE uPage = 0;
				CNesLevel * pLevel = pObj->GetTargetLevel( m_cursel.bWorld, uPage );
				if ( pLevel )
				{
					LoadSubLevel( m_cursel.bWorld, pLevel );
					m_canvas.ScrollTo( uPage );
				}
			}
		}
		else
		{
			CNesEnemyObject * pObj = static_cast<CNesEnemyObject*>( pncm->pObject );
			pObj->SetHardObject( !pObj->IsHardObject() );
			m_canvas.RenderView();
			InvalidateRect( m_canvas, nullptr, TRUE );
		}
	}

	UpdateBufferMenu();

	return 0;
}

INT_PTR CMainWindow::Ctl_OnRightClick( PNCVNVIEW pncm )
{
	HMENU hMenu = CreatePopupMenu();
	HMENU hSubMenu = CreatePopupMenu();
	CMenu menu( hMenu );


	MENUITEMINFO mi = { 0 };

	mi.cbSize = sizeof( mi );
	mi.fMask = MIIM_SUBMENU | MIIM_STRING;
	mi.fType = MFT_STRING;
	mi.hSubMenu = hSubMenu;
	mi.dwTypeData = (LPWSTR)TEXT( "Pages" );

	AppendMenu( hMenu, 0, 0x1001, TEXT( "Add an object...\tInsert" ) );

	if ( pncm->pObject )
	{
		UpdateBufferMenu();

		if ( !pncm->pObject->IsSprite() )
		{
			CNesLevelItem * pObj = static_cast<CNesLevelItem*>( pncm->pObject );
			if ( pObj->EntranceType() != etNotEntrance )
			{
				AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );
				AppendMenu( hMenu, 0, 0x1003, TEXT( "Change entrance pointer..." ) );
			}

			if ( 0x4B == pObj->Item().item.itemId && NES_SPECIAL_OBJECT_D == pObj->Item().item.Y )
			{
				AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );
				AppendMenu( hMenu, 0, 0x1004, TEXT( "Change loop settings..." ) );
			}
		}
	}

	if ( pncm->pt.x > 0 )
	{
		std::map<NES_GROUND_TYPE, NES_GROUND_INFO> gr;
		AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );

		if ( m_cursel.pSublevel->GetGround( pncm->pt.x, gr ) )
		{
			AppendMenu( hMenu, 0, 0x1200, TEXT( "Modify this ground..." ) );
			AppendMenu( hMenu, 0, 0x1202, TEXT( "Remove this ground..." ) );
		}
		else
		{
			AppendMenu( hMenu, 0, 0x1201, TEXT( "Add new ground..." ) );
		}
	}

	AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );
	AppendMenu( hSubMenu, 0, 0x1101, TEXT( "Insert page here" ) );

	if ( !m_nEditor.IsPageReferenced( m_cursel.pSublevel, pncm->pt.x / 16 ) )
	{
		AppendMenu( hSubMenu, 0, 0x1102, TEXT( "Remove this page" ) );
		AppendMenu( hSubMenu, MFS_DISABLED | MFS_GRAYED, 0x1103, TEXT( "References..." ) );
	}
	else
	{
		AppendMenu( hSubMenu, MFS_DISABLED | MFS_GRAYED, 0x1102, TEXT( "Remove this page" ) );
		AppendMenu( hSubMenu, 0, 0x1103, TEXT( "References..." ) );
	}

	if ( m_cursel.pSublevel->Header().IsCloudLevel() )
	{
		AppendMenu( hMenu, 0, 0x1050, TEXT( "Change area pointer..." ) );
		AppendMenu( hMenu, 0, 0x1051, TEXT( "Follow the pointer..." ) );
		AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );
	}

	// AppendMenu( hMenu, 0, 0x1005, TEXT( "Area properties..." ) );
	InsertMenuItem( hMenu, 0x1005, FALSE, &mi );
	// AppendMenu( hMenu, 0, 0x1105, TEXT( "Change area type..." ) );

	AppendMenu( hMenu, MF_SEPARATOR, -1, nullptr );
	AppendMenu( hMenu, ( pncm->pObject ? 0 : MFS_DISABLED | MFS_GRAYED ), ID_EDIT_COPY, TEXT( "Copy\tCtrl+C" ) );
	AppendMenu( hMenu, ( pncm->pObject ? 0 : MFS_DISABLED | MFS_GRAYED ), ID_EDIT_CUT, TEXT( "Cut\tCtrl+X" ) );
	AppendMenu( hMenu, ( m_buffer.CanPaste() ? 0 : MFS_DISABLED | MFS_GRAYED ), ID_EDIT_PASTE, TEXT( "Paste\tCtrl+V" ) );
	AppendMenu( hMenu, ( pncm->pObject ? 0 : MFS_DISABLED | MFS_GRAYED ), ID_EDIT_DELETEOBJECT, TEXT( "Delete object(s)...\tDel" ) );
	AppendMenu( hMenu, 0, ID_EDIT_SELECTALL, TEXT( "Select all\tCtrl+A" ) );

	switch ( menu.TrackMenu() )
	{
		case 0x1001: InsertObject( pncm->pt ); break;
		case 0x1002: DeleteObject(); break;
		case 0x1003: ChangePointer( (CNesLevelItem*)pncm->pObject ); break;
		case 0x1004: LoopSettings( (CNesLoopCommand*)pncm->pObject ); break;
		case 0x1005: AreaSettings(); break;
		case 0x1050: ChangeAreaPointer(); break;
		case 0x1051: FollowTheAreaPointer(); break;
		case 0x1101: IncreaseAreaSize( pncm->pt.x / 16 ); break;
		case 0x1102: DecreaseAreaSize( pncm->pt.x / 16 ); break;
		case 0x1103: ShowReferences( pncm->pt.x / 16 ); break;
		case 0x1105: ChangeLevelType(); break;
		case 0x1200: ModifyGround( pncm->pt.x, FALSE ); break;
		case 0x1201: ModifyGround( pncm->pt.x, TRUE ); break;
		case 0x1202: RemoveGround( pncm->pt.x ); break;
	}

	DestroyMenu( hSubMenu );
	DestroyMenu( hMenu );

	return 0;
}

INT_PTR CMainWindow::Ctl_OnBeginMove( PNCVNVIEW pncm )
{
	TakeLevelSnapshot();
	return 0;
}