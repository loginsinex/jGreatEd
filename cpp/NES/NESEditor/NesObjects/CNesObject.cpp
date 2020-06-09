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

// Primary class for objects

CNesObject::CNesObject( CNesLevel & nLevel, BOOL fSprite )
	: m_nLevel( nLevel ), m_file( nLevel.ROM() ), m_fSprite( fSprite ), m_uFixedY( -1 ), m_uOverrideY( 0xFF ), m_fPutBehind( FALSE )
{
	SetRect( &m_rcObject, 0, 0, 0, 0 );
}

CNesObject::~CNesObject()
{

}

CNESFile & CNesObject::ROM()
{
	return m_file;
}

CNesLevel & CNesObject::Level()
{
	return m_nLevel;
}

BOOL CNesObject::IsHeightFixed() const
{
	return ( m_uFixedY >= 0 );
}

int CNesObject::FixedHeight()
{
	return m_uFixedY;
}

VOID CNesObject::SetMetatile( int X, int Y, const NES_METATILE & mtile )
{
	m_mpt[ X ][ Y ] = mtile;
}

VOID CNesObject::SetSprite( BYTE bSprite, BYTE bAttribute, int X, int Y )
{
	auto & spr = m_msprt[ X ][ Y ];
	spr.attrData = bAttribute;
	spr.id = bSprite;
}

VOID CNesObject::EraseSprite( int X, int Y )
{
	auto itX = m_msprt.find( X );
	if ( itX != m_msprt.end() )
	{
		auto mptY = itX->second;
		auto itY = mptY.find( Y );
		if ( itY != mptY.end() )
		{
			mptY.erase( itY );
		}

		// check if X is empty, if so - remove X from map
		itX = m_msprt.find( X );
		if ( itX != m_msprt.end() && !itX->second.size() )
		{
			m_msprt.erase( itX );
		}
	}
}

VOID CNesObject::FreeSprites()
{
	m_msprt.clear();
}

VOID CNesObject::EraseMetatile( int X, int Y )
{
	auto itX = m_mpt.find( X );
	if ( itX != m_mpt.end() )
	{
		auto mptY = itX->second;
		auto itY = mptY.find( Y );
		if ( itY != mptY.end() )
		{
			mptY.erase( itY );
		}

		// check if X is empty, if so - remove X from map
		itX = m_mpt.find( X );
		if ( itX != m_mpt.end() && !itX->second.size() )
		{
			m_mpt.erase( itX );
		}
	}
}

VOID CNesObject::FreeMetatiles()
{
	m_mpt.clear();
}

VOID CNesObject::DrawMetatile( CNesLevelView & view, int X, int Y, const NES_METATILE & mtile )
{
	view.DrawMetatile( X, Y, mtile );
}

VOID CNesObject::DrawSprite( CNesLevelView & view, int X, int Y, const NES_SPRITE & msprite )
{
	view.DrawSprite( X, Y, msprite );
}

VOID CNesObject::SetObjPlace( int x, int y )
{
	int lWidth = Width(), lHeight = Height();
	SetRect( &m_rcObject, x, y, x + lWidth, y + lHeight );
}

VOID CNesObject::PlaceObject( int x, int y )
{
	if ( x >= 0 && y >= 0 && x + Width() <= m_nLevel.LevelSize().cx )
	{
		if ( PlaceChanging( x, y ) )
		{
			if ( x >= 0 && y >= 0 )
			{
				SetObjPlace( x, y );
				PlaceChanged( x, y );
			}
		}
	}
}

VOID CNesObject::SetObjWidth( int cx )
{
	m_rcObject.right = m_rcObject.left + cx;
}

VOID CNesObject::SetObjHeight( int cy )
{
	m_rcObject.bottom = m_rcObject.top + cy;
}

VOID CNesObject::SetWidth( int cx )
{
	if ( cx >= 0 && cx + GetObjectPoint().x <= m_nLevel.LevelSize().cx )
	{
		if ( WidthChanging( cx ) )
		{
			if ( cx >= 0 )
			{
				SetObjWidth( cx );
				WidthChanged( cx );
			}
		}
	}
}

VOID CNesObject::SetHeight( int cy )
{
	if ( cy >= 0 )
	{
		if ( HeightChanging( cy ) )
		{
			if ( cy >= 0 )
			{			
				SetObjHeight( cy );
				HeightChanged( cy );
			}
		}
	}
}

BOOL CNesObject::WidthChanging( int & cx )
{
	// derived class handler
	return TRUE;
}

VOID CNesObject::WidthChanged( int cx )
{
	// derived class handler
}

BOOL CNesObject::HeightChanging( int & cy )
{
	// derived class handler
	return TRUE;
}

VOID CNesObject::HeightChanged( int cy )
{
	// derived class handler
}

BOOL CNesObject::PlaceChanging( int & x, int & y )
{
	// derived class handler
	return TRUE;
}

VOID CNesObject::PlaceChanged( int x, int y )
{
	// derived class handler
}

int CNesObject::Width()
{
	return m_rcObject.right - m_rcObject.left;
}

int CNesObject::Height()
{
	return m_rcObject.bottom - m_rcObject.top;
}


POINT CNesObject::GetObjectPoint()
{
	return { m_rcObject.left, m_rcObject.top };
}

VOID CNesObject::DrawObject( CNesLevelView & view, BYTE bWorldId )
{
	POINT ptObject = GetObjectPoint();
	if ( !m_fSprite )
	{
		DrawObject( view, bWorldId, ptObject.x, ptObject.y );
	}

	DrawSprite( view, bWorldId, ptObject.x, ptObject.y );
}

VOID CNesObject::DrawSprite( CNesLevelView & view, BYTE bWorldId, int x, int y )
{
	DrawSpriteExtra( view, bWorldId, x, y, FALSE );
	
	if ( !m_msprt.size() )
	{
		// nothing to draw?
		return;
	}

	// find maximum Y to draw sprite form down to up
	auto itMaxY = std::max_element
	( 
		m_msprt.begin(), m_msprt.end(),
		[]( const auto & el1, const auto & el2 )
		{
			if ( el1.second.size() < 1 )
			{
				return false;
			}
			if ( el2.second.size() < 1 )
			{
				return false;
			}

			auto y1 = el1.second.end(); --y1;
			auto y2 = el2.second.end(); --y2;
			return ( y1->first < y2->first );
		}
	);
	int uMaxY = itMaxY->first;

	for ( auto itX = m_msprt.begin(); itX != m_msprt.end(); ++itX )
	{
		auto X = itX->first;
		auto mtY = itX->second;
		for ( auto itY = mtY.begin(); itY != mtY.end(); ++itY )
		{
			auto Y = itY->first;
			DrawSprite( view, 2 * x + X, 2 * y + Y - ( 3 + uMaxY ), itY->second );
		}
	}
	DrawSpriteExtra( view, bWorldId, x, y, TRUE );
}

VOID CNesObject::DrawObject( CNesLevelView & view, BYTE bWorldId, int x, int y )
{
	view.SetBehindFlag( IsBehindObject() );
	DrawExtra( view, bWorldId, x, y, FALSE );

	if ( IsHeightFixed() )
	{
		auto uFixedY = FixedHeight();

		for ( auto itX = m_mpt.begin(); itX != m_mpt.end(); ++itX )
		{
			auto X = itX->first;
			auto mtY = itX->second;
			for ( auto itY = mtY.begin(); itY != mtY.end(); ++itY )
			{
				auto Y = itY->first;
				DrawMetatile( view, x + X, uFixedY + Y, HandleMetatile( bWorldId, X, uFixedY + Y, view.GetMetatile( x + X, uFixedY + Y ), itY->second ) );
			}
		}
	}
	else
	{
		for ( auto itX = m_mpt.begin(); itX != m_mpt.end(); ++itX )
		{
			auto X = itX->first;
			auto mtY = itX->second;
			for ( auto itY = mtY.begin(); itY != mtY.end(); ++itY )
			{
				auto Y = itY->first;
				DrawMetatile( view, x + X, y + Y, HandleMetatile( bWorldId, X, Y, view.GetMetatile( x + X, y + Y ), itY->second ) );
			}
		}
	}

	DrawExtra( view, bWorldId, x, y, TRUE );
}

NES_METATILE CNesObject::HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile )
{
	return newTile;
}

VOID CNesObject::DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	// do nothing. Only for derived classes
}

VOID CNesObject::DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	// do nothing. Only for derived classes
}

VOID CNesObject::InitObject()
{
	// do nothing. Only for derived classes
}

VOID CNesObject::LevelSizeChanged( const SIZE & sz )
{
	// do nothing. Only for derived classes
}

VOID CNesObject::FixHeight( int y )
{
	m_uFixedY = ( y >= 0 && y < 16 ? y : m_uFixedY );
}

VOID CNesObject::UnfixHeight()
{
	m_uFixedY = -1;
}

const RECT CNesObject::GetObjectRect() const
{
	return m_rcObject;
}

BOOL CNesObject::IsSprite()
{
	return m_fSprite;
}

VOID CNesObject::OverrideSpecial( BYTE y )
{
	m_uOverrideY = y;
}

int CNesObject::GetRealY()
{
	return m_rcObject.top;
}

int CNesObject::GetRealX()
{
	return m_rcObject.left;
}

POINT CNesObject::GetRealPoint()
{
	if ( 0xFF != m_uOverrideY )
	{
		return { GetRealX(), m_uOverrideY };
	}
	else
	{
		return { GetRealX(), GetRealY() };
	}
}

VOID CNesObject::EraseObjectCanvas()
{
	FreeMetatiles();
	FreeSprites();
}

VOID CNesObject::PutBehind( BOOL fBehind )
{
	m_fPutBehind = fBehind;
}

BOOL CNesObject::IsBehindObject()
{
	return m_fPutBehind;
}