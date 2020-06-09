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

CNesSingleBlockObjects::CNesSingleBlockObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	SetObjPlace( item.pt.x, item.pt.y );
}

BOOL CNesSingleBlockObjects::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesSingleBlockObjects::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesSingleBlockObjects::PlaceChanging( int & x, int & y )
{
	if ( y > 0x0b )
	{
		y = 0x0b;
	}

	return TRUE;
}

/// -- objects

CNesQBlock::CNesQBlock( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSingleBlockObjects( nLevel, item )
{
	SetObjectToForeground();
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

VOID CNesQBlock::InitObject()
{
	BYTE mtileId = Item().item.itemId;
	BYTE bSpriteId = 0xFF, bAttribute = 0;

	NES_METATILE mTile = Level().ObjData().Tile( eTileListStoneBlock );

	if ( mtileId < 0x0e )
	{
		if ( mtileId == 4 || mtileId >= 0x07 )
		{
			if ( Level().AreaType() != nltGround )
				mtileId += 6;			
		}

		mTile = Level().ObjData().QBlockMetatiles().bMetatile[ min( mtileId, 19 ) ];
	}
	SetMetatile( 0, 0, mTile );

	switch ( Item().item.itemId )
	{
		case 0x00: bSpriteId = nustPowerup; bAttribute = 2; break;
		case 0x01: bSpriteId = nustPowerup; bAttribute = 3; break;
		// case 0x02: bSpriteId = nustCoin; bAttribute = 2; break;
		case 0x03: bSpriteId = nustCoin; bAttribute = 2; break;
		case 0x04: bSpriteId = nustPowerup; bAttribute = 1; break;
		case 0x05: bSpriteId = nustPowerup; bAttribute = 3; break;
		case 0x06: bSpriteId = nustPowerup; bAttribute = 2; break;
		case 0x07: bSpriteId = nustPowerup; bAttribute = 2; break;
		case 0x08: bSpriteId = nustPowerup; bAttribute = 3; break;
		case 0x09: bSpriteId = nustVine; bAttribute = 1; break;
		case 0x0a: bSpriteId = nustStarman; bAttribute = 2; break;
		case 0x0b: bSpriteId = nustCoin; bAttribute = 2; break;
		case 0x0c: bSpriteId = nustPowerup; bAttribute = 1; break;
	}

	if ( 0xFF != bSpriteId )
	{
		SetSprite( bSpriteId, bAttribute, 0, 3 );
	}
}

BOOL CNesQBlock::IsEntranceObject()
{
	return ( Item().item.itemId == 0x09 );
}

NES_ENTRANCE_TYPE CNesQBlock::EntranceType()
{
	return ( 0x09 == Item().item.itemId ? etVine : etNotEntrance );
}

CNesWaterPipe::CNesWaterPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSingleBlockObjects( nLevel, item )
{
	SetObjWidth( 1 );
	SetObjHeight( 2 );
}

VOID CNesWaterPipe::InitObject()
{
	SetMetatile( 0, 0, Level().ObjData().Tile( eTileListWaterPipeTop ) ); // { 0x6b } );
	SetMetatile( 0, 1, Level().ObjData().Tile( eTileListWaterPipeBottom ) ); // { 0x6c } );
}

BOOL CNesWaterPipe::IsEntranceObject()
{
	return TRUE;
}

NES_ENTRANCE_TYPE CNesWaterPipe::EntranceType()
{
	return etSideEntrance;
}

// jumpspring

CNesJumpspring::CNesJumpspring( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSingleBlockObjects( nLevel, item ), m_uGreenFlag( 0 ), m_gjsData( nLevel.ObjData().GreenJumpspringData() )
{
	SetObjectToForeground();
	SetObjWidth( 1 );
	SetObjHeight( 2 );
}

VOID CNesJumpspring::DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	if ( fAfterObject )
	{
		return;
	}

	// reinitialize jumpspring if we're world with forced jumpsprings

	DWORD uGreenFlag = 0;
	BYTE bRealWorld = bWorldId;

	// worlds after 9 counts from 0
	if ( bRealWorld >= 9 )
	{
		bRealWorld -= 9;
	}

	for ( const auto & uGJWorld : m_gjsData.bWorld )
	{
		if ( uGJWorld == bRealWorld )
		{
			uGreenFlag = 1;
			break;
		}
	}

	if ( uGreenFlag != m_uGreenFlag )
	{
		m_uGreenFlag = uGreenFlag;
		FreeSprites();
		InitObject();
	}
}

VOID CNesJumpspring::InitObject()
{
	SetMetatile( 0, 0, Level().ObjData().Tile( eTileListJumpspringTop ) );// { 0x67 } );
	SetMetatile( 0, 1, Level().ObjData().Tile( eTileListJumpspringBottom ) );// { 0x68 } );
	
	if ( m_uGreenFlag )
	{
		SetSprite( 40, ( m_gjsData.bAttribute >> 1 ), 0, 3 );
	}
	else
	{
		SetSprite( 40, m_gjsData.bAttribute, 0, 3 );
	}
}

// intro pipe
CNesIntroPipe::CNesIntroPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	if ( 0x40 == Item().item.itemId )
	{
		OverrideSpecial( NES_SPECIAL_OBJECT_D );
	}

	SetObjPlace( item.pt.x, 0 );
	SetObjWidth( 4 );
	SetObjHeight( 11 );
}

BOOL CNesIntroPipe::PlaceChanging( int & x, int & y )
{
	if ( NES_SPECIAL_OBJECT_D == GetRealPoint().y && 0x0F == ( 0x0F & x ) )
	{
		return FALSE;
	}

	y = 0;
	return TRUE;
}

BOOL CNesIntroPipe::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesIntroPipe::WidthChanging( int & cx )
{
	return FALSE;
}

VOID CNesIntroPipe::InitObject()
{
	NES_PIPE_METATILES pipeMetatiles = Level().ObjData().PipeMetatiles();
	NES_LPIPE_METATILES lpipeMetatiles = Level().ObjData().LPipeMetatiles();
	
	int y = 0;
	for ( y = 0; y <= 6; ++y )
	{
		SetMetatile( 2, y, { 0 } );
		SetMetatile( 3, y, { 0 } );
	}

	SetMetatile( 2, 0 + y, pipeMetatiles.bMetatile[ 5 ] );
	SetMetatile( 3, 0 + y, pipeMetatiles.bMetatile[ 4 ] );
	SetMetatile( 2, 1 + y, pipeMetatiles.bMetatile[ 7 ] );
	SetMetatile( 3, 1 + y, pipeMetatiles.bMetatile[ 6 ] );

	SetMetatile( 0, 2 + y, lpipeMetatiles.SideTop.bMetatile[ 3 ] );
	SetMetatile( 1, 2 + y, lpipeMetatiles.SideTop.bMetatile[ 2 ] );
	SetMetatile( 2, 2 + y, lpipeMetatiles.SideTop.bMetatile[ 1 ] );
	SetMetatile( 3, 2 + y, lpipeMetatiles.SideTop.bMetatile[ 0 ] );
	SetMetatile( 0, 3 + y, lpipeMetatiles.SideBottom.bMetatile[ 3 ] );
	SetMetatile( 1, 3 + y, lpipeMetatiles.SideBottom.bMetatile[ 2 ] );
	SetMetatile( 2, 3 + y, lpipeMetatiles.SideBottom.bMetatile[ 1 ] );
	SetMetatile( 3, 3 + y, lpipeMetatiles.SideBottom.bMetatile[ 0 ] );
}

BOOL CNesIntroPipe::IsEntranceObject()
{
	return TRUE;
}

NES_ENTRANCE_TYPE CNesIntroPipe::EntranceType()
{
	return etIntroPipe;
}

// flagpole

CNesFlagpoleObject::CNesFlagpoleObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesLevelItem( nLevel, item )
{
	if ( 0x41 == Item().item.itemId )
	{
		OverrideSpecial( NES_SPECIAL_OBJECT_D );
	}

	SetObjPlace( item.pt.x, 0 );
	SetObjWidth( 1 );
	SetObjHeight( 11 );
}

BOOL CNesFlagpoleObject::PlaceChanging( int & x, int & y )
{
	if ( NES_SPECIAL_OBJECT_D == GetRealPoint().y && 0x0F == ( 0x0F & x ) )
	{
		return FALSE;
	}

	y = 0;
	return TRUE;
}

BOOL CNesFlagpoleObject::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesFlagpoleObject::WidthChanging( int & cx )
{
	return FALSE;
}

VOID CNesFlagpoleObject::InitObject()
{
	const NES_METATILE fpTop = Level().ObjData().Tile( eTileListFlagpoleTop ); // { 0x24 };
	const NES_METATILE fpMiddle = Level().ObjData().Tile( eTileListFlagpoleRope ); // { 0x25 };
	const NES_METATILE fpBottom = Level().ObjData().Tile( eTileListFlagpoleBottom ); // { 0x61 };

	SetSprite( nustFlagpole, 1, -1, 3 );
	SetMetatile( 0, 0, fpTop );
	SetMetatile( 0, 10, fpBottom );
	for ( BYTE y = 1; y < 10; ++y )
		SetMetatile( 0, y, fpMiddle );
}

// spec objects

CNesSpecObj::CNesSpecObj( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, BYTE bItemId, BYTE len )
	:
	CNesLevelItem( nLevel, item ),
	m_y( nLevel.ObjData().SpecObjData().Y[ bItemId ] ),
	m_mTile( nLevel.ObjData().SpecObjData().tile[ bItemId ] ),
	m_len( len )
{
	SetObjPlace( item.pt.x, m_y );
	SetObjWidth( len );
	SetObjHeight( 1 );
}

BOOL CNesSpecObj::PlaceChanging( int & x, int & y )
{
	if ( NES_SPECIAL_OBJECT_D == Item().item.Y )
	{
		if ( 0x0F == ( x & 0x0F ) )		// objects with X=0x0F and Y=0x0D marks as 0xFD - EOF level
		{
			return FALSE;
		}
	}
	y = m_y;
	return TRUE;
}

BOOL CNesSpecObj::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesSpecObj::WidthChanging( int & cx )
{
	return FALSE;
}

VOID CNesSpecObj::InitObject()
{
	for ( BYTE x = 0; x < m_len; ++x )
		SetMetatile( x, 0, m_mTile );
}

// axe

CNesAxeObject::CNesAxeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSpecObj( nLevel, item, 0, 1 )
{
	if ( 0x42 == Item().item.itemId )
	{
		OverrideSpecial( NES_SPECIAL_OBJECT_D );
	}

	SetObjectToForeground();
}

// chain

CNesChainObject::CNesChainObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSpecObj( nLevel, item, 1, 1 )
{
	if ( Item().item.itemId == 0x43 )
	{
		OverrideSpecial( NES_SPECIAL_OBJECT_D );
	}

	SetObjectToForeground();
}

// bridge

CNesCastleBridgeObject::CNesCastleBridgeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesSpecObj( nLevel, item, 2, 13 )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_D );
}

// command objects

CNesCommandObject::CNesCommandObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, const NES_METATILE & tile )
	: CNesLevelItem( nLevel, item ), m_tile( tile )
{
	OverrideSpecial( NES_SPECIAL_OBJECT_D );
	SetObjectToForeground();
	SetObjPlace( item.pt.x, 13 );
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

BOOL CNesCommandObject::PlaceChanging( int & x, int & y )
{
	if ( 0x0F == ( x & 0x0F ) )
	{
		return FALSE;
	}

	y = 13;
	return TRUE;
}

BOOL CNesCommandObject::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CNesCommandObject::WidthChanging( int & cx )
{
	return FALSE;
}

VOID CNesCommandObject::InitObject()
{
	SetMetatile( 0, 0, m_tile );
}

// warp scroll stop

CNesWarpScrollStop::CNesWarpScrollStop( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x41 } )
{
	
}

// scroll stop

CNesScrollStop::CNesScrollStop( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x40 } )
{

}

// frenzy 1
CNesFrenzyObject1::CNesFrenzyObject1( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x0e } ) // { 0x88 } )
{

}

// frenzy 2
CNesFrenzyObject2::CNesFrenzyObject2( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x65 } ) // { 0x0d } )
{

}

// frenzy 3
CNesFrenzyObject3::CNesFrenzyObject3( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x88 } ) // { 0x67 } )
{

}

// loop command
CNesLoopCommand::CNesLoopCommand( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesCommandObject( nLevel, item, { 0x21 } ), // { 0x26 } )
	m_fSlaveLoop( FALSE )
{
	m_lpc.bHeight = 0xB0;
	m_lpc.bPageNumber = NES_LOOP_COMMAND_MINIMUM_PAGE;
	m_lpc.bSlaveData = 0;
	m_lpc.aptr = { 0 };
	m_lpc.bPageRewind = 0;
}

BOOL CNesLoopCommand::PlaceChanging( int & x, int & y )
{
	if ( x / 16 >= static_cast<int>( m_lpc.bPageNumber ) || 0x0F == ( x & 0x0F ) )
	{
		return FALSE;
	}

	y = NES_SPECIAL_OBJECT_D;
	return TRUE;
}

VOID CNesLoopCommand::DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	if ( fAfterObject )
	{
		// draw player as marker of pass view
		// get the right point for sprite
		NES_SPRITE spr = { 0 };
		POINT pt = { ( m_lpc.bPageNumber - 1 ) << 4, m_lpc.bHeight >> 4 };
		spr.attr = ( m_fSlaveLoop  ? 1 : 0 );
		spr.id = npstBigRunning;
		view.DrawSprite( pt.x << 1, ( pt.y - 2 ) << 1, spr );
		spr.id += NES_PLAYER_GFX_OFFSET;
		view.DrawSprite( pt.x << 1, ( ( pt.y - 2 ) << 1 ) + 1, spr );

		// draw sprite when player occurs
		if ( !m_fSlaveLoop && m_lpc.bPageNumber >= 4 )
		{
			pt.x = ( m_lpc.bPageNumber - 4 ) << 4;
			pt.y = ( m_lpc.bHeight >> 4 );
			spr.attr = 0;
			spr.id = npstBigTurning;
			view.DrawSprite( pt.x << 1, ( pt.y - 2 ) << 1, spr );
			spr.id += NES_PLAYER_GFX_OFFSET;
			view.DrawSprite( pt.x << 1, ( ( pt.y - 2 ) << 1 ) + 1, spr );
		}
	}
}

VOID CNesLoopCommand::LoadLoop( const NES_LOOP & lpc )
{
	m_lpc = lpc;
#ifdef _DEBUG
	_tprintf( TEXT( "* Loaded loop command at level %Iu, x=%d: Y=%d, loop is %s\n" ), Level().Metadata().uLevelId, GetRealX(), ( m_lpc.bHeight >> 4 ), ( m_lpc.bSlaveData > 1 ? TEXT( "slave" ) : TEXT( "master" ) ) );
#endif
}

VOID CNesLoopCommand::DumpLoop( NES_LOOP & lpc )
{
	lpc = m_lpc;
}

BOOL CNesLoopCommand::LoopConnected()
{
	return m_fSlaveLoop;
}

VOID CNesLoopCommand::LoopConnected( BOOL fSlaveLoop )
{
	m_fSlaveLoop = ( fSlaveLoop ? TRUE : FALSE );
}

VOID CNesLoopCommand::TriggerPage( BYTE bPage )
{
	m_lpc.bPageNumber = max( ( LoopConnected() ? NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE : NES_LOOP_COMMAND_MINIMUM_PAGE ), bPage );;
}

BYTE CNesLoopCommand::TriggerPage()
{
	return m_lpc.bPageNumber;
}

VOID CNesLoopCommand::RewindOffset( BYTE bOffset )
{
	m_lpc.bPageRewind = bOffset;
}

BYTE CNesLoopCommand::RewindOffset()
{
	return m_lpc.bPageRewind;
}

VOID CNesLoopCommand::SlaveData( BYTE bSlaveData )
{
	m_lpc.bSlaveData = bSlaveData;
}


BYTE CNesLoopCommand::AllowedHeight()
{
	return m_lpc.bHeight / 16;
}

VOID CNesLoopCommand::AllowedHeight( BYTE Y )
{
	m_lpc.bHeight = LOBYTE( Y * 16 );
}