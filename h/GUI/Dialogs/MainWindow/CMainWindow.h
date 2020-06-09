#pragma once

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

#define IDCU_MAINWINDOW_STATUS		0x2010
#define IDCU_MAINWINDOW_REBAR		0x2030
#define IDCU_MAINWINDOW_CANVAS		0x1000
#define IDCU_MAINWINDOW_TOOLBAR		0x1002

typedef struct _tagBUFFER_OBJECT
{
	NES_LEVEL_ITEM			nItem;
	NES_LEVEL_ENEMY_ITEM	nObject;
	BOOL					fObject;
} BUFFER_OBJECT, *PBUFFER_OBJECT;

typedef enum _tagBUFFER_EVENT
{
	beCopy,
	beCut,
	bePaste
} BUFFER_EVENT, *PBUFFER_EVENT;

class CObjBuffer
{
	std::vector<BUFFER_OBJECT>		m_vObjects;

public:
	CObjBuffer();

	VOID			Copy( std::vector<CNesObject*> & vObject );
	BOOL			Paste( CNesLevel * pLevel, const POINT & pt, std::vector<CNesObject*> & vpCreatedObjects );
	BOOL			CanPaste();
};

class CMainWindow : public CWindow
{
	CPaletteBitmap	m_bmLevelView;
	CMenu			m_menu;
	CToolbar		m_tb;
	CStatusBar		m_status;
	CRebarWindow	m_rb;
	CCanvasCtl		m_canvas;
	CObjBuffer		m_buffer;
	CDemoPlayEditorDlg	m_dlgDemo;

	CNesEditor			m_nEditor;
	CNesObjectsNames	m_nNames;

	struct
	{
		CNesLevel *		pSublevel;
		CNesLevel *		pLevel;
		BYTE			bWorld;
		BYTE			nLevel;
	} m_cursel;

	BOOL			OnCreate( LPCREATESTRUCT lpcs ) override;
	VOID			OnDestroy() override;
	VOID			OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;
	VOID			OnButton( USHORT uId ) override;
	VOID			OnCancel() override;
	VOID			OnSize( INT dwState, USHORT cx, USHORT cy ) override;
	VOID			OnSizing( INT dwState, USHORT cx, USHORT cy ) override;
	BOOL			OnClose() override;
	INT_PTR			OnNotify( LPNMHDR lpnm ) override;

	VOID			Ctl_OnToolbarTip( UINT_PTR cmd, LPNMTTDISPINFO lpnmtt, BOOL fAnsi );
	VOID			Ctl_OnOpen();
	VOID			Ctl_OnSave(BOOL fSaveAs);
	VOID			Ctl_OnSelectLevel();
	VOID			Ctl_OnDeleteObject();
	VOID			Ctl_OnInsertObject();
	VOID			Ctl_OnSelectAll();
	VOID			Ctl_OnAreaSettings();
	VOID			Ctl_OnChangeAreaType();
	VOID			Ctl_OnPaletteEditor();
	VOID			Ctl_OnBgColorEditor();
	VOID			Ctl_OnBufferEvent( BUFFER_EVENT be );
	VOID			Ctl_OnGameSettings();
	VOID			Ctl_OnDemoPlay();
	VOID			Ctl_OnLoadAreaFromFile();
	VOID			Ctl_OnDumpAreaIntoFile();
	VOID			Ctl_OnWorldConfig();
	VOID			Ctl_OnAreaLinkManager();
	VOID			Ctl_OnCreateNewArea();
	VOID			Ctl_OnUndo();
	VOID			Ctl_OnShowLimits();

	VOID			Ctl_OnExec();
	VOID			Ctl_OnExit();
	VOID			Ctl_OnAbout();

	INT_PTR			Ctl_OnHilite( PNCVNVIEW pnc );
	INT_PTR			Ctl_OnSelect( PNCVNVIEW pnc );
	INT_PTR			Ctl_OnRedraw( PNCVNVIEW pnc );
	INT_PTR			Ctl_OnMapMove( PNCNVIEWMAP pncm );
	INT_PTR			Ctl_OnDblClick( PNCVNVIEW pncm );
	INT_PTR			Ctl_OnRightClick( PNCVNVIEW pncm );
	INT_PTR			Ctl_OnBeginMove( PNCVNVIEW pncm );

	VOID			OnRebarHeightChange();
	VOID			LoadGame();
	VOID			LoadLevel( BYTE bWorld, BYTE bLevel );
	VOID			LoadLevel( BYTE bWorld, BYTE bLevel, CNesLevel * pLevel );
	VOID			LoadSubLevel( BYTE bWorld, CNesLevel * pLevel );
	VOID			UpdateBufferMenu();
	VOID			UpdateHeader();
	VOID			UpdateUndoMenu();
	CString			GetWarpZoneString();

	VOID			InsertObject( POINT pt );
	VOID			DeleteObject();
	VOID			DeleteObjects( std::vector<CNesObject*> & vpObj );
	VOID			ChangePointer( CNesLevelItem * pObject );
	VOID			IncreaseAreaSize( UINT uPage );
	VOID			DecreaseAreaSize( UINT uPage );
	VOID			ShowReferences( UINT uPage );
	VOID			LoopSettings( CNesLoopCommand * pLoop );
	VOID			ModifyGround( int x, BOOL fNewGround );
	VOID			RemoveGround( int x );
	VOID			ChangeAreaPointer();
	VOID			FollowTheAreaPointer();
	VOID			AreaSettings();
	VOID			ChangeLevelType();
	VOID			DumpArea( LPCTSTR pszFile );
	VOID			LoadArea( LPCTSTR pszFile );
	VOID			AreaLinkManager();

	VOID			TakeLevelSnapshot();
	VOID			PopLevelSnapshot();


public:
	CMainWindow( HINSTANCE hInstance );
};