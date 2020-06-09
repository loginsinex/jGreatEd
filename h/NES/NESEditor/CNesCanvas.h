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

#define WC_NESCANVAS		TEXT("SysView32")
#define NES_COINS_COL_UPDATE_TIMER	150

#define NCVN_FIRST				(-900)
#define NCVN_REDRAW_REQUEST		(-901)
#define NCVN_OBJECT_HILITE		(-902)
#define NCVN_OBJECT_SELECT		(-903)
#define NCVN_MAP_MOVE			(-904)
#define NCVN_DBL_CLICK			(-905)
#define NCVN_RIGHT_CLICK		(-906)
#define NCVN_BEGIN_MOVE			(-907)

typedef enum _tagCURSOR_MODE
{
	cmArrow,
	cmHorzSize,
	cmVertSize,
	cmFullSize,
	cmHand,
	cmGround
} CURSOR_MODE, *PCURSOR_MODE;

typedef struct _tagNCVNVIEW
{
	NMHDR		nm;
	BOOL		fRedrawRequired;
	POINT		pt;
	CNesObject * pObject;
} NCVNVIEW, *PNCVNVIEW;

typedef struct _tagNCNVIEWMAP
{
	NMHDR			nm;
	POINT			pt;
	NES_METATILE	tile;
} NCNVIEWMAP, *PNCNVIEWMAP;

class CNesCanvas : public CCustomControl<CNesCanvas>
{
	CNesLevelInterface				* m_pInterface;
	CBitmap							m_bm;
	CWinPaneScroller				m_scroll;
	CREATESTRUCT					m_cs;
	SIZE							m_szControl;	
	SIZE_MODE						m_szMode;
	const BOOL						m_fEditable;
	int								m_uReservedHeight;
	BOOL							m_fDrawLock;

	struct
	{
		BOOL						fCapture;
		BOOL						fFocused;
		POINT						ptCatch;
		POINT						ptMove;

		struct
		{
			CBitmap					bmHilite;
			POINT					pt;
			BOOL					fVisible;
		} hilite;

		struct
		{
			std::vector<RECT>		vrcSel;
			BOOL					fVisible;
			BOOL					fSelRectVisible;
			BOOL					fBeginMove;
			RECT					rcSel;
		} sel;

		BOOL						fMouseLeaved;
		BOOL						fContextMenu;
	} m_capture;

	struct
	{
		std::map<CURSOR_MODE, HCURSOR>	mCursor;
		HCURSOR							hCursor;
		CURSOR_MODE						cMode;
	} m_cursor;

	friend CCustomControl<CNesCanvas>;

	const CREATESTRUCT				CtlData();
	SIZE							CtlViewSize();
	BOOL							CtlSize( int cx, int cy );

	BOOL			OnCreate( LPCREATESTRUCT lpcs ) override;
	VOID			OnSize( INT dwState, USHORT cx, USHORT cy ) override;
	VOID			OnSizing( INT dwState, USHORT cx, USHORT cy ) override;
	VOID			OnPaint( HDC hDC, BOOL fErase, PRECT prc ) override;
	VOID			OnEraseBg( HDC hDC ) override;
	VOID			OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl ) override;
	VOID			OnMouseButton( MOUSEACTION ma, MOUSEBUTTON mb, UINT vKey, SHORT x, SHORT y ) override;
	LRESULT			OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & fHandle ) override;
	VOID			OnGotFocus( HWND hLostFocus ) override;
	VOID			OnLostFocus( HWND hGotFocus ) override;
	VOID			OnKeyEvent( DWORD vEvent, DWORD vKey, DWORD vScanCode ) override;
	LRESULT			OnDlgCode( DWORD vKey, LPMSG lpmsg ) override;
	VOID			OnTimer( DWORD_PTR idTimer, FARPROC pTimerProc ) override;

	BOOL			InitBuffer();

	CNesCanvas( CREATESTRUCT & cs, HWND hWnd );
	virtual				~CNesCanvas();

	VOID			DrawFocusedObjectRect( HDC hDC, RECT rc );
	BOOL			CheckSizeMode( int rx, int ry );
	
	BOOL			IsEqualCatchPoint( int x, int y );
	BOOL			IsEqualCatchPoint( int x );
	BOOL			IsEqualMovePoint( int x, int y );
	VOID			MouseCatchObject( int x, int y, BOOL fCatchOnly = FALSE );
	VOID			MouseDoubleClick( int x, int y );
	VOID			MouseRightClick( int x, int y );
	VOID			MouseFreeObject( int x, int y );
	VOID			MouseMove( int rx, int ry );
	VOID			MouseMoveObject( int x, int y );
	RECT			NormalizeRect( const RECT & rc );
	VOID			MouseLeave();
	BOOL			IsInSizeMode();
	BOOL			IsInGroundMode();
	VOID			UpdateFocusRect();
	VOID			BeginMove();
	BOOL			BeginMoveNotify();

	INT_PTR			NotifyMapMove( const NES_METATILE & tile, int x, int y );
	INT_PTR			NotifyObjSel( CNesObject * pObject );
	INT_PTR			NotifyObjHl( CNesObject * pObject );
	INT_PTR			NotifyRedraw( CNesObject * pObject );
	INT_PTR			NotifyDblClick( CNesObject * pObject );
	INT_PTR			NotifyBeginMove( CNesObject * pObject );
	INT_PTR			NotifyRightClick( CNesObject * pObject );
	INT_PTR			Notify( int uCode, PNCVNVIEW pnc );
	VOID			Cursor( CURSOR_MODE cm );
	VOID			UpdateCursor();

	VOID			SetMode( SIZE_MODE szMode );
	VOID			LeaveHiliteMode();
	VOID			LeaveSizeMode();
	VOID			LeaveGroundBoundMode();
	VOID			LockDraw();
	VOID			UnlockDraw();
	BOOL			IsDrawLocked();

public:
	static VOID Register( HINSTANCE hInstance );

	VOID				SetView( CNesLevelInterface * pInterface );
	VOID				DelView();
	CNesLevelInterface *	GetView();
	VOID				RenderView();
	BOOL			GetSelectedObjects( std::vector<CNesObject*> & vpObject );
	POINT			GetLastPoint();
	VOID			DropSelection();
	VOID			SetSelection( std::vector<CNesObject*> & vpObj );
	int				ReservedHeight();
};

