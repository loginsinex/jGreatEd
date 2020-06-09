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

CNesLongObjects::CNesLongObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	SetObjPlace( item.pt.x, item.pt.y );
}

BYTE CNesLongObjects::ItemId()
{
	return ( ( Item().item.itemId & 0xF0 ) >> 4 );
}

BYTE CNesLongObjects::ItemLength()
{
	return ( Item().item.itemId & 0x0F ) + 1;
}

BOOL CNesLongObjects::PlaceChanging( int & x, int & y )
{
	if ( y > 0x0b )
	{
		y = 0x0b;
	}

	return TRUE;
}

CNesHorzRow::CNesHorzRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLongObjects( nLevel, item ), m_mTile( { 0 } )
{
	SetObjectToForeground();
	SetObjWidth( ItemLength() );
	SetObjHeight( 1 );
}

VOID CNesHorzRow::Metatile( const NES_METATILE & tile )
{
	m_mTile = tile;
}

BOOL CNesHorzRow::WidthChanging( int & cx )
{
	if ( cx >= 1 && cx <= 16 )
	{
		ModifyItemId( ( Item().item.itemId & 0xF0 ) | LOBYTE( cx - 1 ) );
		FreeMetatiles();
		InitObject();
		return TRUE;
	}
	return FALSE;
}

BOOL CNesHorzRow::HeightChanging( int & cy )
{
	return FALSE;
}

VOID CNesHorzRow::InitObject()
{
	BYTE len = ItemLength();

	Metatile( GetMetatile() );

	for ( BYTE x = 0; x < len; ++x )
	{
		SetMetatile( x, 0, m_mTile );
	}
}

CNesVertColumn::CNesVertColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLongObjects( nLevel, item ), m_mTile( { 0 } )
{
	SetObjWidth( 1 );
	SetObjHeight( ItemLength() );
}

VOID CNesVertColumn::InitObject()
{
	BYTE len = ItemLength();

	Metatile( GetMetatile() );

	for ( BYTE y = 0; y < len; ++y )
	{
		SetMetatile( 0, y, m_mTile );
	}
}

BOOL CNesVertColumn::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesVertColumn::HeightChanging( int & cy )
{
	if ( cy >= 1 && cy <= 16 )
	{
		ModifyItemId( ( Item().item.itemId & 0xF0 ) | LOBYTE( cy - 1 ) );
		FreeMetatiles();
		InitObject();
		return TRUE;
	}
	return FALSE;
}

VOID CNesVertColumn::Metatile( const NES_METATILE & tile )
{
	m_mTile = tile;
}

// bricks
CNesRowOfBricks::CNesRowOfBricks( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesHorzRow( nLevel, item )
{
	SetObjectToForeground();
}

NES_METATILE CNesRowOfBricks::GetMetatile()
{
	BYTE mtileId = Level().ObjData().Tile( eTileListBricksRowTileId ).bMetatile % 5;		//4

	if ( !Level().Header().IsCloudLevel() )
	{
		mtileId = Level().AreaType();
	}

	return Level().ObjData().BricksMetatiles().bMetatile[ mtileId ];
}

// solid
CNesRowOfSolid::CNesRowOfSolid( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesHorzRow( nLevel, item )
{
	SetObjectToForeground();
}

NES_METATILE CNesRowOfSolid::GetMetatile()
{
	return Level().ObjData().SolidMetatiles().bMetatile[ Level().AreaType() ];
}

// coins
CNesRowOfCoins::CNesRowOfCoins( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesHorzRow( nLevel, item )
{
	SetObjectToForeground();
}

NES_METATILE CNesRowOfCoins::GetMetatile()
{
	return Level().ObjData().CoinsMetatiles().bMetatile[ Level().AreaType() ];
}

// vert bricks
CNesVertBricksColumn::CNesVertBricksColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesVertColumn( nLevel, item )
{

}

NES_METATILE CNesVertBricksColumn::GetMetatile()
{
	return Level().ObjData().BricksMetatiles().bMetatile[ Level().AreaType() ];
}

// vert solid
CNesVertSolidColumn::CNesVertSolidColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesVertColumn( nLevel, item )
{

}

NES_METATILE CNesVertSolidColumn::GetMetatile()
{
	return Level().ObjData().SolidMetatiles().bMetatile[ Level().AreaType() ];
}

// vert pipe
CNesVertPipe::CNesVertPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLongObjects( nLevel, item )
{
	SetObjWidth( 2 );
	SetObjHeight( PipeHeight() );
}

BOOL CNesVertPipe::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesVertPipe::HeightChanging( int & cy )
{
	if ( cy >= 1 && cy <= 8 )
	{
		FreeMetatiles();
		ModifyItemId( ( Item().item.itemId & 0xF8 ) | ( LOBYTE( cy - 1 ) ) );
		InitObject();
		return TRUE;
	}

	return FALSE;
}

BYTE CNesVertPipe::PipeHeight()
{
	BYTE len = Item().item.itemId & 0x07;
	if ( len < 1 )
	{
		len = 7;
	}

	return ( len + 1 );
}

BOOL CNesVertPipe::IsWarpPipe()
{
	const int x = GetRealPoint().x;
	const BYTE bPage = static_cast<BYTE>( x / 16 );
	if ( Level().IsObjectPresentOnPageRange( bPage, bPage + 1, 0x45, NES_SPECIAL_OBJECT_D ) )
	{
		// check pipe place right after digit of text
		return TRUE;
	}

	return FALSE;
}

VOID CNesVertPipe::InitObject()
{
	const BYTE itemLen = PipeHeight();
	const BYTE lPipeEntrance = ( ( ItemLength() & 8 ) ? 4 : 0 );
	
	// check if we're in a warp zone
	// WARN: ALL pipes will be enterable for this zone, cause to engine doesnt check for pipe place within zone
	if ( IsEnterableObject() || IsWarpPipe() )
	{
		ModifyItemId( Item().item.itemId | 0x08 );
	}
	else
	{
		ModifyItemId( Item().item.itemId & ~0x08 );
	}

	const NES_METATILE mTileLeft = Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 3 ];
	const NES_METATILE mTileRight = Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 2 ];

	SetMetatile( 0, 0, Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 1 ] );
	SetMetatile( 1, 0, Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 0 ] );

	for ( BYTE y = 1; y < itemLen; ++y )
	{
		SetMetatile( 0, y, mTileLeft );
		SetMetatile( 1, y, mTileRight );
	}
}

NES_ENTRANCE_TYPE CNesVertPipe::EntranceType()
{
	return etVerticalPipe;
}

BOOL CNesVertPipe::IsEntranceObject()
{
	return ( Item().item.itemId & 0x08 );
}

// upside pipe

CNesUpsidePipe::CNesUpsidePipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLongObjects( nLevel, item ), m_fInvalidWorld( 0xFF == nLevel.ObjData().UpsidePipeHeight().bHeight[ 0 ] )
{
	m_iPipeY = Level().ObjData().UpsidePipeHeight().bHeight[ ( 6 == ItemId() ? 0 : 1 ) ];

	if ( m_fInvalidWorld )
	{
		m_iPipeY = 0;
	}

	SetObjPlace( item.pt.x, m_iPipeY );
	SetObjWidth( 2 );
	SetObjHeight( PipeHeight() );
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
}

VOID CNesUpsidePipe::InitObject()
{
	if ( m_fInvalidWorld )
	{
		return;	// dont do anything for invalid worlds set
	}

	const BYTE itemLen = PipeHeight();
	const BYTE lPipeEntrance = 0;

	const NES_METATILE mTileLeft = Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 3 ];
	const NES_METATILE mTileRight = Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 2 ];

	SetMetatile( 0, itemLen - 1, Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 1 ] );
	SetMetatile( 1, itemLen - 1, Level().ObjData().PipeMetatiles().bMetatile[ lPipeEntrance + 0 ] );

	for ( BYTE y = 0; y < itemLen - 1; ++y )
	{
		SetMetatile( 0, y, mTileLeft );
		SetMetatile( 1, y, mTileRight );
	}
}

BYTE CNesUpsidePipe::PipeHeight()
{
	BYTE len = Item().item.itemId & 0x07;
	if ( len < 1 )
	{
		len = 7;
	}

	return ( len + 1 );
}

BOOL CNesUpsidePipe::PlaceChanging( int & x, int & y )
{
	y = m_iPipeY;
	return TRUE;
}

BOOL CNesUpsidePipe::HeightChanging( int & cy )
{
	if ( cy >= 1 && cy <= 8 )
	{
		FreeMetatiles();
		ModifyItemId( ( Item().item.itemId & 0x70 ) | ( LOBYTE( cy - 1 ) ) );
		InitObject();
		return TRUE;
	}

	return FALSE;
}

BOOL CNesUpsidePipe::WidthChanging( int & cx )
{
	return FALSE;
}

// wind object

CNesWind::CNesWind( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item ), m_fWindOff( 0x4D == item.item.itemId )
{
	SetObjWidth( 1 );
	SetObjHeight( 0x0d );
	SetObjPlace( item.pt.x, 0 );
	OverrideSpecial( NES_SPECIAL_OBJECT_D );
}

VOID CNesWind::InitObject()
{
	BYTE bAttr = ( m_fWindOff ? 3 : 1 );
	for ( int y = 0; y <= 0x0d; y += 2 )
	{
		SetSprite( nustFrenzyMark, bAttr, 0, 2 * y );
	}
}

BOOL CNesWind::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesWind::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesWind::PlaceChanging( int & x, int & y )
{
	y = 0;
	return TRUE;
}