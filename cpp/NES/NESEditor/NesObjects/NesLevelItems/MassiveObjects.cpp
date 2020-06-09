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

CNesMassiveObjects::CNesMassiveObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{

}

BYTE CNesMassiveObjects::ItemId()
{
	return ( ( Item().item.itemId & 0xF0 ) >> 4 );
}

BYTE CNesMassiveObjects::ItemSize()
{
	return ( ( Item().item.itemId & 0x0F ) + 1 );
}

VOID CNesMassiveObjects::UpdateItemSize( BYTE lSize )
{
	FreeMetatiles();
	ModifyItemId( ( Item().item.itemId & 0xF0 ) | ( ( lSize - 1 ) & 0x0F ) );
	InitObject();
}


///
CNesMassiveRow::CNesMassiveRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y, int height )
	: CNesMassiveObjects( nLevel, item ), m_y( y ), m_height( height )
{
	SetObjWidth( ItemSize() );
	SetObjHeight( height );
	SetObjPlace( item.pt.x, m_y );
}

BOOL CNesMassiveRow::PlaceChanging( int & x, int & y )
{
	y = m_y;
	return TRUE;
}

BOOL CNesMassiveRow::WidthChanging( int & cx )
{
	if ( cx >= 1 && cx <= 16 )
	{
		UpdateItemSize( LOBYTE( cx ) );
		return TRUE;
	}

	return FALSE;
}

BOOL CNesMassiveRow::HeightChanging( int & cy )
{
	return FALSE;
}

/// hole

CNesHole::CNesHole( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesMassiveRow( nLevel, item, 8, 5 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_C );
}

VOID CNesHole::InitObject()
{
	const BYTE bLength = ItemSize();
	const BYTE bHeight = Height();
	const NES_METATILE tile = Level().ObjData().HoleMetatiles().bMetatile[ Level().AreaType() ];

	for ( BYTE y = 0; y < bHeight; ++y )
	{
		for ( BYTE x = 0; x < bLength; ++x )
		{
			SetMetatile( x, y, tile );
		}
	}
}

CNesBridge::CNesBridge( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y )
	: CNesMassiveRow( nLevel, item, y, 2 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_C );
}

VOID CNesBridge::InitObject()
{
	BYTE bLength = ItemSize();
	const NES_METATILE tile1 = Level().ObjData().Tile( eTileListBridgeTile1 ); // { 0x0b };
	const NES_METATILE tile2 = Level().ObjData().Tile( eTileListBridgeTile2 ); // { 0x63 };

	for ( BYTE x = 0; x < bLength; ++x )
	{
		SetMetatile( x, 0, tile1 );
		SetMetatile( x, 1, tile2 );
	}
}

CNesBridgeHigh::CNesBridgeHigh( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesBridge( nLevel, item, nLevel.ObjData().Tile( eTileListBridgeHeight0 ).bMetatile )
{

}

CNesBridgeMiddle::CNesBridgeMiddle( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesBridge( nLevel, item, nLevel.ObjData().Tile( eTileListBridgeHeight1 ).bMetatile )
{

}

CNesBridgeLow::CNesBridgeLow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesBridge( nLevel, item, nLevel.ObjData().Tile( eTileListBridgeHeight2 ).bMetatile )
{

}

// water
CNesWater::CNesWater( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesMassiveRow( nLevel, item, 10, 3 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_C );
}

VOID CNesWater::InitObject()
{
	BYTE bLength = ItemSize();
	const NES_METATILE tile1 = Level().ObjData().Tile( eTileListWaterTile1 ); // { 0x86 };
	const NES_METATILE tile2 = Level().ObjData().Tile( eTileListWaterTile2 ); // { 0x87 };

	for ( BYTE x = 0; x < bLength; ++x )
	{
		SetMetatile( x, 0, tile1 );		
		SetMetatile( x, 1, tile2 );
		SetMetatile( x, 2, tile2 );
	}
}

// q-blocks row
CNesQBlockRow::CNesQBlockRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y )
	: CNesMassiveRow( nLevel, item, y, 1 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_C );
	SetObjectToForeground();
}

VOID CNesQBlockRow::InitObject()
{
	BYTE bLength = ItemSize();
	const NES_METATILE tile1 = Level().ObjData().Tile( eTileListQBlockRow ); // { 0xc0 };

	for ( BYTE x = 0; x < bLength; ++x )
	{
		SetMetatile( x, 0, tile1 );
	}
}

CNesQBlocksRowHigh::CNesQBlocksRowHigh( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesQBlockRow( nLevel, item, nLevel.ObjData().Tile( eTileListQBlockHeight1 ).bMetatile % 0x0b )
{

}

CNesQBlocksRowLow::CNesQBlocksRowLow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesQBlockRow( nLevel, item, nLevel.ObjData().Tile( eTileListQBlockHeight2 ).bMetatile % 0x0b )
{

}

// pulley rope

CNesPulleyRopeObject::CNesPulleyRopeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesMassiveRow( nLevel, item, 0, 1 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_C );
	SetObjectToForeground();
}

VOID CNesPulleyRopeObject::InitObject()
{
	const NES_PULLEY_ROPE_METATILES & pr = Level().ObjData().PulleyRopeMetatiles();
	BYTE bLength = ItemSize();

	for ( BYTE x = 0; x < bLength; ++x )
	{
		NES_METATILE tile = pr.bMetatile[ 1 ];
		if ( !x )
		{
			tile = pr.bMetatile[ 0 ];
		}
		else if ( x + 1 == bLength )
		{
			tile = pr.bMetatile[ 2 ];
		}

		SetMetatile( x, 0, tile );
	}

}

// rope

CNesRope::CNesRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
	SetObjHeight( 13 );
	SetObjWidth( 1 );
	SetObjPlace( item.pt.x, 0 );
}

BOOL CNesRope::PlaceChanging( int & x, int & y )
{
	y = 0;
	return TRUE;
}

BOOL CNesRope::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesRope::HeightChanging( int & cy )
{
	return FALSE;
}

VOID CNesRope::InitObject()
{
	const NES_METATILE tile = Level().ObjData().Tile( eTileListRope ); // { 0x40 };
	for ( BYTE y = 0; y < 13; ++y )
	{
		SetMetatile( 0, y, tile );
	}
}

CNesVertMassiveObject::CNesVertMassiveObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int bFromY, int bMinHeight )
	: CNesLevelItem( nLevel, item ), m_bFromY( bFromY ), m_bMinHeight( bMinHeight )
{
	SetObjPlace( item.pt.x, bFromY );
	SetObjHeight( ItemHeight() );
}

BYTE CNesVertMassiveObject::ItemHeight()
{
	return ( ( Item().item.itemId & 0x0f ) + 1 );
}

BOOL CNesVertMassiveObject::ItemHeight( int & cy )
{
	if ( cy >= m_bMinHeight && cy <= 16 )
	{
		ModifyItemId( ( Item().item.itemId & 0xF0 ) | LOBYTE( cy - 1 ) );
		FreeMetatiles();
		InitObject();
		return TRUE;
	}

	return FALSE;
}

BOOL CNesVertMassiveObject::HeightChanging( int & cy )
{
	return ItemHeight( cy );
}

BOOL CNesVertMassiveObject::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesVertMassiveObject::PlaceChanging( int & x, int & y )
{
	y = m_bFromY;
	return TRUE;
}

CNesBalanceRope::CNesBalanceRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesVertMassiveObject( nLevel, item, 1, 1 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
	SetObjWidth( 1 );
}

VOID CNesBalanceRope::InitObject()
{
	int lItemHeight = ItemHeight();
	for ( int y = 0; y < lItemHeight; ++y )
	{
		SetMetatile( 0, y, Level().ObjData().Tile( eTileListRope ) ); // { 0x40 } );
	}
}

CNesReversePipe::CNesReversePipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesVertMassiveObject( nLevel, item, 0, 3 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
	SetObjWidth( 4 );
}

VOID CNesReversePipe::InitObject()
{
	NES_PIPE_METATILES pipeMetatiles = Level().ObjData().PipeMetatiles();
	NES_LPIPE_METATILES lpipeMetatiles = Level().ObjData().LPipeMetatiles();
	int lItemHeight = ItemHeight();
	int y = 0;
	for ( y = 0; y < lItemHeight - 2; ++y )
	{
		SetMetatile( 2, y, pipeMetatiles.bMetatile[ 7 ] );
		SetMetatile( 3, y, pipeMetatiles.bMetatile[ 6 ] );
	}

	SetMetatile( 0, y + 0, lpipeMetatiles.SideTop.bMetatile[ 3 ] );
	SetMetatile( 1, y + 0, lpipeMetatiles.SideTop.bMetatile[ 2 ] );
	SetMetatile( 2, y + 0, lpipeMetatiles.SideTop.bMetatile[ 1 ] );
	SetMetatile( 3, y + 0, lpipeMetatiles.SideTop.bMetatile[ 0 ] );
	SetMetatile( 0, y + 1, lpipeMetatiles.SideBottom.bMetatile[ 3 ] );
	SetMetatile( 1, y + 1, lpipeMetatiles.SideBottom.bMetatile[ 2 ] );
	SetMetatile( 2, y + 1, lpipeMetatiles.SideBottom.bMetatile[ 1 ] );
	SetMetatile( 3, y + 1, lpipeMetatiles.SideBottom.bMetatile[ 0 ] );
}

BOOL CNesReversePipe::IsEntranceObject()
{
	return TRUE;
}

NES_ENTRANCE_TYPE CNesReversePipe::EntranceType()
{
	return etReversePipe;
}

CNesUnusedRope::CNesUnusedRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesVertMassiveObject( nLevel, item, 2, 1 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
	SetObjWidth( 1 );
}

VOID CNesUnusedRope::InitObject()
{
	int lItemHeight = ItemHeight();
	for ( int y = 0; y < lItemHeight; ++y )
	{
		SetMetatile( 0, y, Level().ObjData().Tile( eTileListResidualBall ) ); // { 0x6d } );
	}
}

// castle

CNesCastle::CNesCastle( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_F );
	SetObjPlace( item.pt.x, ItemY() );
	SetObjWidth( 5 );
	SetObjHeight( 0x0b - ItemY() );
}

BOOL CNesCastle::PlaceChanging( int & x, int & y )
{
	y = max( 0, y );
	y = min( 0x0a, y );

	ItemY( y );

	return TRUE;
}

BYTE CNesCastle::ItemY()
{
	return min( 0x0a, ( Item().item.itemId & 0x0f ) );
}

VOID CNesCastle::ItemY( BYTE y )
{
	if ( y >= 0 && y <= 15 && y != ItemY() )
	{
		ModifyItemId( ( Item().item.itemId & 0xf0 ) | y );
		FreeMetatiles();
		InitObject();
		SetObjPlace( GetObjectPoint().x, ItemY() );
		SetObjHeight( 0x0b - ItemY() );
	}
}

BOOL CNesCastle::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesCastle::HeightChanging( int & cy )
{
	return FALSE;
}

VOID CNesCastle::InitObject()
{
	const NES_CASTLE_METATILES & tiles = Level().ObjData().CastleMetatiles();
	BYTE lHeight = 11 - ItemY();
	for ( BYTE y = 0; y < lHeight; ++y )
	{
		for ( BYTE x = 0; x < 5; ++x )
		{
			SetMetatile( 4 - x, y, tiles.bMetatiles[ y ][ x ] );
		}
	}
}

// staircase

CNesStaircase::CNesStaircase( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	BYTE lWidth = ItemWidth();
	FixHeight( 0 );
	OverrideSpecial( NES_SPECIAL_OBJECT_F );

	SetObjPlace( item.pt.x, Level().ObjData().StaircaseData().Y[ 9 - lWidth ] );
	SetObjWidth( lWidth );
	SetObjHeight( Level().ObjData().StaircaseData().height[ 9 - lWidth ] + 1 );
}

BOOL CNesStaircase::PlaceChanging( int & x, int & y )
{
	BYTE lWidth = ItemWidth();
	y = Level().ObjData().StaircaseData().Y[ 9 - lWidth ];
	return TRUE;
}

BOOL CNesStaircase::WidthChanging( int & cx )
{
	if ( cx >= 1 && cx <= 9 )
	{
		ItemWidth( cx );
		return TRUE;
	}
	return FALSE;
}

BOOL CNesStaircase::HeightChanging( int & cy )
{
	return FALSE;
}

BYTE CNesStaircase::ItemWidth()
{
	return min( 9, ( Item().item.itemId & 0x0f ) + 1 );
}

VOID CNesStaircase::ItemWidth( BYTE cx )
{
	if ( cx != ItemWidth() )
	{
		FreeMetatiles();
		ModifyItemId(( Item().item.itemId & 0xF0 ) | ( cx - 1 ));
		InitObject();
		SetObjPlace( GetObjectPoint().x, Level().ObjData().StaircaseData().Y[ 9 - cx ] );
		SetObjWidth( cx );
		SetObjHeight( Level().ObjData().StaircaseData().height[ 9 - cx ] + 1 );
	}
}

VOID CNesStaircase::InitObject()
{
	const NES_STAIRCASE_DATA & sdata = Level().ObjData().StaircaseData();
	const BYTE lWidth = ItemWidth();

	for ( BYTE x = 0; x < lWidth; ++x )
	{
		const int y = sdata.Y[ 8 - x ] - 1;
		int lHeight = sdata.height[ 8 - x ] + 1;

		for ( ; lHeight > 0; lHeight-- )
		{
			SetMetatile( x, y + lHeight, Level().ObjData().Tile( eTileListStaircase ) ); // { 0x61 } );
		}
	}
}

CNesAreaStyleLedge::CNesAreaStyleLedge( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesMassiveObjects( nLevel, item )
{
	SetObjPlace( item.pt.x, item.pt.y );

	SetObjByArea();
}

VOID CNesAreaStyleLedge::SetObjByArea()
{
	switch ( Level().Header().AreaStyle() )
	{
		case 0:
		case 1:
			{
				SetObjWidth( ItemSize() );
				SetObjHeight( 0x0d - GetObjectPoint().y );
				break;
			}
		case 2:
			{
				SetObjWidth( 1 );
				SetObjHeight( ItemSize() );
				break;
			}
	}	
}

VOID CNesAreaStyleLedge::InitObject()
{
	FreeMetatiles();

	switch ( Level().Header().AreaStyle() )
	{
		case 0: InitTreeLedge(); break;
		case 1: InitMushroom(); break;
		case 2: InitBulletBillCannon(); break;
	}
}

BOOL CNesAreaStyleLedge::PlaceChanging( int & x, int & y )
{
	if ( y > NES_SPECIAL_OBJECT_C - 1 )
	{
		y = NES_SPECIAL_OBJECT_C - 1;
	}

	switch ( Level().Header().AreaStyle() )
	{
		case 0: return TreeLedge_PlaceChanging( x, y );
		case 1: return Mushroom_PlaceChanging( x, y );
		case 2: return Cannon_PlaceChanging( x, y );
	}

	return FALSE;
}

BOOL CNesAreaStyleLedge::WidthChanging( int & cx )
{
	switch ( Level().Header().AreaStyle() )
	{
		case 0: return TreeLedge_WidthChanging( cx );
		case 1: return Mushroom_WidthChanging( cx );
		case 2: return Cannon_WidthChanging( cx );
	}
	return FALSE;
}

BOOL CNesAreaStyleLedge::HeightChanging( int & cy )
{
	switch ( Level().Header().AreaStyle() )
	{
		case 0: return TreeLedge_HeightChanging( cy );
		case 1: return Mushroom_HeightChanging( cy );
		case 2: return Cannon_HeightChanging( cy );
	}
	return FALSE;
}

VOID CNesAreaStyleLedge::ReinitObject()
{
	InitObject();
}

VOID CNesAreaStyleLedge::InitTreeLedge()
{
	int lItemWidth = ItemSize() - 1;
	int lItemHeight = NES_SPECIAL_OBJECT_D - GetObjectPoint().y;
	SetObjWidth( lItemWidth + 1 );
	SetObjHeight( lItemHeight );
	SetMetatile( 0, 0, Level().ObjData().Tile( eTileListTreeLedgeLeft ) ); // { 0x16 } );
	SetMetatile( ItemSize() - 1, 0, Level().ObjData().Tile( eTileListTreeLedgeMiddle ) ); // { 0x18 } );
	for ( int x = 1; x < lItemWidth; ++x )
	{
		SetMetatile( x, 0, Level().ObjData().Tile( eTileListTreeLedgeRight ) ); // { 0x17 } );
		for ( int y = 1; y < lItemHeight; ++y )
		{
			SetMetatile( x, y, Level().ObjData().Tile( eTileListTreeLedgeBottom ) );
		}
	}
}

VOID CNesAreaStyleLedge::InitMushroom()
{
	int lItemWidth = ItemSize() - 1;
	// int lItemHalfWidth = ItemSize() / 2;
	int lItemHeight = NES_SPECIAL_OBJECT_D - GetObjectPoint().y;
	SetObjWidth( lItemWidth + 1 );
	SetObjHeight( lItemHeight );
	SetMetatile( 0, 0, Level().ObjData().Tile( eTileListMushroomLedgeLeft ) ); // { 0x19 } );
	SetMetatile( ItemSize() - 1, 0, Level().ObjData().Tile( eTileListMushroomLedgeRight ) ); // { 0x1B } );

	for ( int x = 1; x < lItemWidth; ++x )
	{
		SetMetatile( x, 0, Level().ObjData().Tile( eTileListMushroomLedgeMiddle ) );
	}

	/*
		if ( x == lItemHalfWidth )
		{
			if ( lItemHeight > 1 )
			{
				SetMetatile( x, 1, Level().ObjData().Tile( eTileListMushroomLedgeTop ) );
				for ( int y = 2; y < lItemHeight; ++y )
				{
					SetMetatile( x, y, Level().ObjData().Tile( eTileListMushroomLedgeBottom ) );
				}
			}
		}
	}
	*/
}

VOID CNesAreaStyleLedge::InitBulletBillCannon()
{
	int lItemHeight = ItemSize();
	SetObjWidth( 1 );
	SetObjHeight( lItemHeight );
	SetMetatile( 0, 0, Level().ObjData().Tile( eTileListBulletBillTop ) ); // { 0x64 } );

	if ( lItemHeight > 1 )
	{
		SetMetatile( 0, 1, Level().ObjData().Tile( eTileListBulletBillMiddle ) ); // { 0x65 } );

		for ( int y = 2; y < lItemHeight; ++y )
		{
			SetMetatile( 0, y, Level().ObjData().Tile( eTileListBulletBillBottom ) ); // { 0x66 } );
		}
	}
}

BOOL CNesAreaStyleLedge::TreeLedge_PlaceChanging( int & x, int & y )
{
	SetObjHeight( NES_SPECIAL_OBJECT_D - y );
	SetObjPlace( x, y );
	ReinitObject();
	return TRUE;
}

BOOL CNesAreaStyleLedge::Mushroom_PlaceChanging( int & x, int & y )
{
	return TreeLedge_PlaceChanging( x, y );
}

BOOL CNesAreaStyleLedge::Cannon_PlaceChanging( int & x, int & y )
{
	return TRUE;
}

BOOL CNesAreaStyleLedge::TreeLedge_WidthChanging( int & cx )
{
	if ( cx >= 1 && cx <= 16 )
	{
		ModifyItemId( Item().item.itemId & 0xf0 | LOBYTE( cx - 1 ) );
		ReinitObject();
		return TRUE;
	}
	return FALSE;
}

BOOL CNesAreaStyleLedge::Mushroom_WidthChanging( int & cx )
{
	return TreeLedge_WidthChanging( cx );
}

BOOL CNesAreaStyleLedge::Cannon_WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesAreaStyleLedge::TreeLedge_HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesAreaStyleLedge::Mushroom_HeightChanging( int & cy )
{
	return TreeLedge_HeightChanging( cy );
}

BOOL CNesAreaStyleLedge::Cannon_HeightChanging( int & cy )
{
	if ( cy >= 1 && cy <= 16 )
	{
		ModifyItemId( Item().item.itemId & 0xf0 | LOBYTE( cy - 1 ) );
		ReinitObject();
		return TRUE;
	}
	return FALSE;
}
