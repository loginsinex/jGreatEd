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

#define CHKOBJ(p)	{ CheckObject(p); }

typedef enum _tagSIZE_MODE
{
	smNone,
	smLeft,
	smTop,
	smRight,
	smBottom,
	smHand,
	smCross,
	smGroundBound
} SIZE_MODE, *PSIZE_MODE;

class CNesLevelInterface
{
	CNesLevelView						m_view;
	CNesLevel &							m_level;
	const BYTE							m_uWorldId;
	const NES_COIN_PAL_ROTATE_COLOR &	m_coinColors;
	BYTE								m_bCoinColor;

	struct
	{
		CNesObject *					pHilite;
		std::vector<CNesObject *>		pSelected;		
		POINT							ptLast;
		std::vector <POINT>				vptObjDelta;
		BOOL							fRedrawRequired;
		int								uXGround;
	} m_hl;

	BOOL					IsPtStale( int x, int y );
	VOID					SetPt( int x, int y );
	VOID					FlushPt();

	VOID					HiliteLevelObject( int x, int y );
	VOID					SelectLevelObject( int x, int y, BOOL fMultipleSelection );
	BOOL					AcquireSelectionMode( CNesObject * pObject, int x, int y, BOOL fMultipleSelection );
	VOID					SetupCatchPoints( int x, int y );
	BOOL					IsInSizeMode( SIZE_MODE szMode );

	VOID					CheckObject( std::vector<CNesObject *> & vpObjects );
	VOID					SetCoinColor();

public:
	CNesLevelInterface( CNesMetatiles & mTiles, CNesGameEngineHack & hack, CPaletteBitmap & bm, CNesLevel & level, BYTE uWorldId );

	VOID					PrepareRenderLevel();
	VOID					RenderLevel( CBitmap & bm );
	VOID					Refresh( CBitmap & bm );
	SIZE					CanvasSize();
	BOOL					LeaveHilite();
	BOOL					RedrawRequired();
	CNesObject *			GetSelObj();
	CNesObject *			GetHlObj();

	BOOL					TryCatchObject( int x, int y, BOOL fMultipleSelection = FALSE );
	BOOL					TryCatchObjects( const RECT & rc );
	BOOL					TryCatchGroundBound( int x );
	VOID					ReleaseObject( int x, int y );
	BOOL					IsOverGroundBound( int x );
	BOOL					IsOverObject( int x, int y );
	VOID					ResizeObject( int x, int y, SIZE_MODE szMode );
	VOID					MoveObject( int x, int y );
	VOID					ResizeGroundBound( int x );
	BOOL					GetSelObjArea( std::vector<RECT> & vrcObj );
	BOOL					GetHlObjArea( RECT & rcObj );

	NES_METATILE			GetMetatile( int x, int y );

	VOID					EraseAll();
	BOOL					IsMultipleSelection();
	BOOL					GetSelectedObjectList( std::vector<CNesObject*> & vpObjects );
	VOID					DropSelection();
	VOID					SetSelection( std::vector<CNesObject*> & vpObjects );

	BOOL					IsLevelSizeChanged();

	int						ReservedHeight();

	VOID					UpdateCoinColor();

};