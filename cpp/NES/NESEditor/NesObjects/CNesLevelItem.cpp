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

CNesLevelItem::CNesLevelItem( CNesLevel & nLevel, const NES_LEVEL_ITEM & item )
	: CNesObject( nLevel, FALSE ), m_item( item ), m_fForceForeground( FALSE )
{

}

CNesLevelItem::CNesLevelItem( CNesLevel & nLevel )
	: CNesObject( nLevel, FALSE ), m_fForceForeground( FALSE )
{
	ZeroMemory( &m_item, sizeof( m_item ) );
}

const NES_LEVEL_ITEM & CNesLevelItem::Item()
{
	return m_item;
}

VOID CNesLevelItem::ModifyItemId( BYTE bItemId )
{
	m_item.item.itemId = bItemId;
}

BOOL CNesLevelItem::IsObjectForeground()
{
	return m_fForceForeground;
}

VOID CNesLevelItem::SetObjectToForeground()
{
	m_fForceForeground = TRUE;
}

VOID CNesLevelItem::DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	if ( fAfterObject )
	{
		if ( IsEnterableObject() )
		{
			view.DrawMetatile( GetObjectRect().left, 15, { 0xc0 } );
		}
	}
}

NES_METATILE CNesLevelItem::HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile )
{
	/*
	if ( IsObjectForeground() )
	{
		return newTile;
	}
	*/

//	return newTile;

	const BYTE & srcTile = currentTile.bMetatile;
	const BYTE & dstTile = newTile.bMetatile;

	if ( srcTile == 0x00 )
	{
		return newTile;
	}

	if ( dstTile == 0x00 )
	{
		if (
			srcTile != 0x51 && srcTile != 0x52 && srcTile != 0x53 &&
			!( srcTile > 0x55 && srcTile < 0x5F )
			)
		{
			return newTile;
		}
		else
		{
			return currentTile;
		}
	}

	if ( srcTile == 0x17 )
		return currentTile;

	if ( srcTile == 0x1a )
		return currentTile;

	if ( srcTile >= 0xc0 )
		return currentTile;

	if ( srcTile != 0x54 )
		return newTile;

	if ( dstTile == 0x50 )
		return currentTile;

	return newTile;
}

BOOL CNesLevelItem::IsEntranceObject()
{
	return FALSE;
}

BOOL CNesLevelItem::IsEnterableObject()
{
	return ( m_mLinks.size() > 0 );
}

NES_ENTRANCE_TYPE CNesLevelItem::EntranceType()
{
	return etNotEntrance;
}

CNesLink * CNesLevelItem::FindLink( NES_AREA_SET areaSet, BYTE bWorld )
{
	auto itAS = m_mLinks.find( areaSet );
	if ( itAS != m_mLinks.end() )
	{
		auto itW = itAS->second.begin();
		if ( asWorld9 != areaSet )
		{
			itW = itAS->second.find( bWorld );
		}
		
		if ( itW != itAS->second.end() )
		{
			return itW->second;
		}
	}

	return nullptr;
}

BYTE CNesLevelItem::NormalizeWorldId( BYTE bWorld, NES_AREA_SET & areaSet )
{
	switch ( bWorld )
	{
		case 0:
		case 1:
		case 2:
		case 3:
			{
				areaSet = asWorld1to4;
				return bWorld;
			}
		case 4:
		case 5:
		case 6:
		case 7: 
			{
				areaSet = asWorld5to8;
				return bWorld;
			}
		case 8: 
			{
				areaSet = asWorld9;
				return 0x00;	// any world accepted for world #9
			}
		case 9:
		case 0x0A:
		case 0x0B:
		case 0x0C: 
			{
				areaSet = asWorldAtoD;
				return bWorld - 9;
			}
	}

	return 0;

}

CNesLevel * CNesLevelItem::GetTargetLevel( BYTE uWorld, BYTE & uPage )
{
	NES_AREA_SET areaSet = asWorld1to4;
	BYTE bRealWorldId = NormalizeWorldId( uWorld, areaSet );
	CNesLink * pLink = FindLink( areaSet, bRealWorldId );

	if ( pLink )
	{
		uPage = pLink->TargetPage();
		return pLink->TargetLevel();
	}

	return nullptr;
}

VOID CNesLevelItem::DumpPointers( std::vector<NES_LEVEL_ENEMY_ITEM>& vItem )
{
	const NES_AREA_SET aSets[] = { asWorld1to4, asWorld5to8, asWorldAtoD, asWorld9 };
	// set asWorld9 as last pointers, because world 9 will use LAST pointer in pointers array (will overwrite all previous pointers)

	// we cannot handle case with asWorld1to4 and asWorldAtoD+asWorld9 sets together in one array cause to conflict worlds
	// in this case dump only asWorld1to4+asWorld5to8
	BYTE bSets[ asWorldSetsCount ] = { 0 };
	for ( const auto & set : aSets )
	{
		BYTE bOverrideWorld = 0xFF;
		auto itSet = m_mLinks.find( set );
		if ( itSet == m_mLinks.end() )
		{
			continue;
		}

		bSets[ set ] = TRUE;

		switch ( set )
		{
			case asWorld9:
				{
					// due to pointers parser accepts ANY world in pointer data,
					// so, will be used LAST pointer in array as world == #00
					bOverrideWorld = 0x00;

					// fall down, don't break!
				}
			case asWorldAtoD:
				{
					if ( bSets[ asWorld1to4 ] )
					{
						return;
					}
					break;
				}
		}

		for ( const auto & wl : itSet->second )
		{
			const BYTE bWorld = ( 0xFF == bOverrideWorld ? wl.first : bOverrideWorld );
			CNesLink * pLink = wl.second;
			NES_LEVEL_ENEMY_ITEM item = { 0 };

			item.pt.x = GetObjectRect().left;
			item.pt.y = NES_SPECIAL_OBJECT_E;
			item.item.Y = NES_SPECIAL_OBJECT_E;
			item.item.X = LOBYTE( item.pt.x ) & 0x0F;
			item.item.bData[ 1 ] = pLink->TargetLevel()->AreaPtr().bPtr;
			item.item.bData[ 2 ] = ( ( ( bWorld & 7 ) << 5 ) | pLink->TargetPage() & 0x1f );

			vItem.push_back( item );
		}
	}
}

VOID CNesLevelItem::GetLinks( NES_AREA_SET areaSet, std::vector<CNesLevel*>& vpLinks )
{
	auto itSet = m_mLinks.find( areaSet );
	if ( itSet != m_mLinks.end() )
	{
		for ( const auto & wl : itSet->second )
		{
			CNesLink * pLink = wl.second;
			vpLinks.push_back( pLink->TargetLevel() );
		}
	}
}

VOID CNesLevelItem::ReadLinks( NES_AREA_SET as, std::map<BYTE, std::pair<UINT, CNesLevel*>>& mLinks )
{
	auto itSet = m_mLinks.find( as );
	if ( itSet != m_mLinks.end() )
	{
		for ( const auto & world : itSet->second )
		{
			mLinks[ world.first ] = { world.second->TargetPage(), world.second->TargetLevel() };
		}
	}
}

CNesLevelItem::~CNesLevelItem()
{
	for ( auto & set : m_mLinks )
	{
		for ( auto lnk : set.second )
		{
			lnk.second->Release( this );
		}
	}
}

VOID CNesLevelItem::SetLink( CNesLink * pLink )
{
	NES_AREA_SET areaSet = asWorld1to4;
	BYTE uWorld = pLink->SourceWorld();
	BYTE bRealWorld = NormalizeWorldId( uWorld, areaSet );
	RemoveLink( uWorld );
	m_mLinks[ pLink->AreaSet() ][ bRealWorld ] = pLink;
	pLink->AddRef( this );
	InitObject();
}

VOID CNesLevelItem::RemoveLink( BYTE uWorld )
{
	NES_AREA_SET areaSet = asWorld1to4;
	BYTE bRealWorldId = NormalizeWorldId( uWorld, areaSet );
	CNesLink * pLink = FindLink( areaSet, bRealWorldId );

	if ( pLink )
	{
		pLink->Release( this );
		m_mLinks[ areaSet ].erase( bRealWorldId );
		InitObject();
	}
}

VOID CNesLevelItem::RemoveLinks()
{
	for ( auto & set : m_mLinks )
	{
		for ( auto lnk : set.second )
		{
			lnk.second->Release( this );
		}
	}

	m_mLinks.clear();

	InitObject();
}