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

#include "h\NES\NESEditor\NESEditor.h"


CNesLevelInterface::CNesLevelInterface( CNesMetatiles & mTiles, CNesGameEngineHack & hack, CPaletteBitmap & bm, CNesLevel & level, BYTE uWorldId )
	: m_view( bm, mTiles ), m_level( level ), m_uWorldId( uWorldId ), m_coinColors( hack.Read().rotateCols ), m_bCoinColor( 0 )
{
	m_level.InitObjects();
	m_level.InitView( m_view );
	m_hl.ptLast.x = m_hl.ptLast.y = -1;
	m_hl.pHilite = nullptr;
	m_hl.fRedrawRequired = FALSE;
}

VOID CNesLevelInterface::PrepareRenderLevel()
{
	m_level.InitObjectsQueue();
}

VOID CNesLevelInterface::RenderLevel( CBitmap & bm )
{
	_tprintf( TEXT( "Rendering level...\n" ) );

	m_level.DrawLevel( m_view, m_uWorldId );
	m_view.RenderMap();
	SetCoinColor();
	Refresh( bm );
}

VOID CNesLevelInterface::Refresh( CBitmap & bm )
{
	SIZE sz = { m_view.Bitmap().Width(), m_view.Bitmap().Height() };
	BitBlt( bm, 0, 0, sz.cx, sz.cy, m_view, 0, 0, SRCCOPY );
}

BOOL CNesLevelInterface::IsPtStale( int x, int y )
{
	return ( x == m_hl.ptLast.x && y == m_hl.ptLast.y );
}

VOID CNesLevelInterface::SetPt( int x, int y )
{
	m_hl.ptLast.x = x;
	m_hl.ptLast.y = y;
}

VOID CNesLevelInterface::FlushPt()
{
	m_hl.ptLast.x = m_hl.ptLast.y = -1;
}

VOID CNesLevelInterface::MoveObject( int x, int y )
{
	CHKOBJ( m_hl.pSelected );

	if ( m_hl.pSelected.size() > 0 )
	{
		size_t cObj = 0, cMax = m_hl.vptObjDelta.size();

		for ( auto obj : m_hl.pSelected )
		{
			if ( cObj >= cMax ) break;
			POINT & ptObject = m_hl.vptObjDelta[ cObj++ ];
			obj->PlaceObject( ptObject.x + x, ptObject.y + y );
		}

		m_level.ResortObjects();
		m_hl.fRedrawRequired = TRUE;
	}
}

VOID CNesLevelInterface::ResizeObject( int x, int y, SIZE_MODE szMode )
{
	CHKOBJ( m_hl.pSelected );

	if ( 1 == m_hl.pSelected.size() )
	{
		CNesObject * pObject = m_hl.pSelected[ 0 ];

		switch ( szMode )
		{
			case smLeft:
				{
					int lNewWidth = pObject->GetObjectRect().right - x;
					pObject->SetWidth( lNewWidth );
					if ( lNewWidth == pObject->Width() )
					{
						pObject->PlaceObject( x, pObject->GetObjectRect().top );
						m_hl.fRedrawRequired = ( x == pObject->GetObjectRect().left );
					}
					break;
				}
			case smRight:
				{
					int lNewWidth = x - pObject->GetObjectRect().left;
					pObject->SetWidth( lNewWidth );
					m_hl.fRedrawRequired = ( lNewWidth == pObject->Width() );
					break;
				}
			case smTop:
				{
					int lNewHeight = pObject->GetObjectRect().bottom - y;
					pObject->SetHeight( lNewHeight );
					if ( lNewHeight == pObject->Height() )
					{
						pObject->PlaceObject( pObject->GetObjectRect().left, y );
						m_hl.fRedrawRequired = ( y == pObject->GetObjectRect().top );
					}
					break;
				}
			case smBottom:
				{
					int lNewHeight = y - pObject->GetObjectRect().top;
					pObject->SetHeight( lNewHeight );
					m_hl.fRedrawRequired = ( lNewHeight == pObject->Height() );
					break;
				}
		}
	}

	if ( m_hl.fRedrawRequired )
	{
		m_level.ResortObjects();
	}
}

BOOL CNesLevelInterface::TryCatchObject( int x, int y, BOOL fMultipleSelection )
{
	SelectLevelObject( x, y, fMultipleSelection );

	return ( m_hl.pSelected.size() > 0 );
}

BOOL CNesLevelInterface::TryCatchObjects( const RECT & rc )
{
	return m_level.ObjectsFromRect( rc, m_hl.pSelected );
}

BOOL CNesLevelInterface::TryCatchGroundBound( int x )
{
	std::map<NES_GROUND_TYPE, NES_GROUND_INFO> gr;
	if ( m_level.GetGround( x, gr ) )
	{
		m_hl.uXGround = x;
		return TRUE;
	}

	return FALSE;
}

VOID CNesLevelInterface::ReleaseObject( int x, int y )
{
	const POINT ptNull = { -1, -1 };
	std::fill( m_hl.vptObjDelta.begin(), m_hl.vptObjDelta.end(), ptNull );
}

BOOL CNesLevelInterface::IsOverGroundBound( int x )
{
	std::map<NES_GROUND_TYPE, NES_GROUND_INFO> gr;
	
	return m_level.GetGround( x, gr );
}

BOOL CNesLevelInterface::IsOverObject( int x, int y )
{
	HiliteLevelObject( x, y );
	return ( m_hl.pHilite != nullptr );
}

VOID CNesLevelInterface::ResizeGroundBound( int x )
{
	if ( m_level.MoveGround( m_hl.uXGround, x ) )
	{
		m_hl.uXGround = x;
		m_hl.fRedrawRequired = TRUE;
	}
}

BOOL CNesLevelInterface::GetSelObjArea( std::vector<RECT> & vrcObj )
{
	CHKOBJ( m_hl.pSelected );
	for ( auto obj : m_hl.pSelected )
	{
		vrcObj.push_back( obj->GetObjectRect() );
	}

	return ( vrcObj.size() > 0 );
}

BOOL CNesLevelInterface::GetHlObjArea( RECT & rcObj )
{
	if ( m_hl.pHilite && m_level.IsValidObject( m_hl.pHilite ) )
	{
		rcObj = m_hl.pHilite->GetObjectRect();
		return TRUE;
	}

	return FALSE;
}

NES_METATILE CNesLevelInterface::GetMetatile( int x, int y )
{
	return m_view.GetMetatile( x, y );
}

VOID CNesLevelInterface::EraseAll()
{
	m_view.EraseAll();
}

BOOL CNesLevelInterface::IsMultipleSelection()
{
	return ( m_hl.pSelected.size() > 1 );
}

BOOL CNesLevelInterface::GetSelectedObjectList( std::vector<CNesObject*> & vpObjects )
{
	vpObjects = m_hl.pSelected;
	return ( vpObjects.size() > 0 );
}

VOID CNesLevelInterface::DropSelection()
{
	m_hl.pSelected.clear();
	m_hl.vptObjDelta.clear();
}

VOID CNesLevelInterface::SetSelection( std::vector<CNesObject*>& vpObjects )
{
	CHKOBJ( vpObjects );
	m_hl.pSelected = vpObjects;
}

VOID CNesLevelInterface::HiliteLevelObject( int x, int y )
{
	CNesObject * pObject = m_level.ObjectFromPoint( x, y );

	if ( pObject )
	{
		m_hl.pHilite = pObject;
	}
	else if ( !pObject )
	{
		m_hl.pHilite = nullptr;
	}
}

VOID CNesLevelInterface::SetupCatchPoints( int x, int y )
{
	size_t cObj = 0;
	if ( m_hl.pSelected.size() > m_hl.vptObjDelta.size() )
	{
		m_hl.vptObjDelta.insert( m_hl.vptObjDelta.end(), m_hl.pSelected.size() - m_hl.vptObjDelta.size(), { 0 } );
	}

	for ( auto pObj : m_hl.pSelected )
	{
		const POINT & pt = pObj->GetObjectPoint();
		m_hl.vptObjDelta[ cObj++ ] = { pt.x - x, pt.y - y };
	}
}

BOOL CNesLevelInterface::AcquireSelectionMode( CNesObject * pObject, int x, int y, BOOL fMultipleSelection )
{
	if ( fMultipleSelection )
	{
		return FALSE;
	}

	auto itObject = std::find( m_hl.pSelected.begin(), m_hl.pSelected.end(), pObject );
	if ( itObject != m_hl.pSelected.end() )
	{
		// object already selected, so allow user to drag items
		SetupCatchPoints( x, y );
		return TRUE;
	}

	return FALSE;
}

VOID CNesLevelInterface::SelectLevelObject( int x, int y, BOOL fMultipleSelection )
{
	CNesObject * pObject = m_level.ObjectFromPoint( x, y );

	if ( pObject )
	{
		RECT rcObject = pObject->GetObjectRect();
		POINT ptObject = { rcObject.left, rcObject.top };

		if ( AcquireSelectionMode( pObject, x, y, fMultipleSelection ) )
		{
			// user attempts to drag items by already selected object.
			// catched X and Y has been updated so return without changing selection
			return;
		}

		if ( !fMultipleSelection )
		{
			m_hl.pSelected.clear();
			m_hl.pSelected.push_back( pObject );
		}
		else
		{
			auto itObject = std::find( m_hl.pSelected.begin(), m_hl.pSelected.end(), pObject );
			
			// if object is already exists in the selected objects
			// remove it from selection
			if ( m_hl.pSelected.end() != itObject )
			{
				m_hl.pSelected.erase( itObject );
			}
			else
			{
				m_hl.pSelected.push_back( pObject );
			}
		}
		SetupCatchPoints( x, y );
	}
	else if ( !pObject )
	{
		DropSelection();
	}
}

BOOL CNesLevelInterface::LeaveHilite()
{
	FlushPt();
	m_hl.pHilite = nullptr;
	return TRUE;
}

SIZE CNesLevelInterface::CanvasSize()
{
	return m_level.LevelSize();
}

BOOL CNesLevelInterface::RedrawRequired()
{
	BOOL fRedrawRequired = m_hl.fRedrawRequired;
	m_hl.fRedrawRequired = FALSE;
	return fRedrawRequired;
}

CNesObject * CNesLevelInterface::GetSelObj()
{
	CHKOBJ( m_hl.pSelected );
	return ( m_hl.pSelected.size() > 0 ? m_hl.pSelected[ 0 ] : nullptr );
}

CNesObject * CNesLevelInterface::GetHlObj()
{
	return m_hl.pHilite;
}

BOOL CNesLevelInterface::IsInSizeMode( SIZE_MODE szMode )
{
	switch ( szMode )
	{
		case smLeft:
		case smRight:
		case smTop:
		case smBottom:
			{
				return TRUE;
			}
		default:
			{
				return FALSE;
			}
	}
}

VOID CNesLevelInterface::CheckObject( std::vector<CNesObject *> & vpObjects )
{
	if ( !vpObjects.size() )
	{
		return;
	}

	std::vector<CNesObject*> vpChecked;
	vpChecked.reserve( vpObjects.size() );

	for ( auto & obj : vpObjects )
	{
		if ( m_level.IsValidObject( obj ) )
		{
			vpChecked.push_back( obj );
		}
	}
}

BOOL CNesLevelInterface::IsLevelSizeChanged()
{
	if ( m_view.PagesCount() != m_level.PagesCount() )
	{
		m_level.InitView( m_view );
		return TRUE;
	}

	return FALSE;
}

int CNesLevelInterface::ReservedHeight()
{
	return m_view.ReservedSpace();
}

VOID CNesLevelInterface::SetCoinColor()
{
	m_view.Bitmap().Entry( 0x0D, XCHGCOLORREF( g_crNesColor[ m_coinColors.bCol[ m_bCoinColor ] ] ) );
}

VOID CNesLevelInterface::UpdateCoinColor()
{
	SetCoinColor();
	m_bCoinColor = ( m_bCoinColor + 1 ) % sizeof( m_coinColors.bCol );
}