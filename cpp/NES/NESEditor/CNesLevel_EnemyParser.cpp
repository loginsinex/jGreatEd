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

size_t CNesLevel::SerializeEnemyData( std::vector<BYTE>& vEnemyData )
{
	std::vector<NES_LEVEL_ENEMY_ITEM> vItem;
	std::map<BYTE, std::vector<NES_LEVEL_ENEMY_ITEM>> mPage;
	BYTE bPage = 0;

	DumpPointersAsObjects( vItem );

	// dump all objects into the array
	for ( auto & obj : m_obj.vpEnemy )
	{
		NES_LEVEL_ENEMY_ITEM item = { 0 };
		const POINT pt = obj->GetRealPoint();
		// skip unknown objects with Y > 0x0D, cause to no special objects, except of page skippers and pointers may be exists
		if ( pt.y < NES_SPECIAL_OBJECT_E )
		{
			item.pt.x = pt.x;
			item.pt.y = pt.y;
			item.item.X = LOBYTE( item.pt.x ) & 0x0F;
			item.item.Y = LOBYTE( item.pt.y ) & 0x0F;
			item.item.objectId = obj->Item().item.objectId;
			vItem.push_back( item );
		}
	}

	SplitEnemiesToPages( mPage, vItem );

	vItem.clear();

	for ( auto & item : mPage )
	{
		if ( !bPage && 1 == item.first )
		{
			// realy this flag is already set by SplitItemsToPages subroutine
			// but without this ^^^ clause page skipper will be added
			item.second[ 0 ].item.fNewPage = 1;
			DumpEnemies( vEnemyData, item.second );
			bPage = item.first;
		}
		else if ( bPage != item.first )
		{
			// skip some of pages and drop 'new page flag' for object on next page
			NES_LEVEL_ENEMY_ITEM itemPage = { 0 };
			itemPage.item.objectId = ( item.first & 0x3F );
			itemPage.item.Y = NES_SPECIAL_OBJECT_F;
			itemPage.item.X = 0x00;
			bPage = item.first;
			item.second[ 0 ].item.fNewPage = 0;

			AddEnemyToDump( vEnemyData, itemPage );
			DumpEnemies( vEnemyData, item.second );
		}
		else
		{
			DumpEnemies( vEnemyData, item.second );
		}


		bPage++;
	}

	return ( vEnemyData.size() );
}

VOID CNesLevel::SplitEnemiesToPages( std::map<BYTE, std::vector<NES_LEVEL_ENEMY_ITEM>>& mPage, std::vector<NES_LEVEL_ENEMY_ITEM>& vItem )
{
	std::sort( vItem.begin(), vItem.end(), []( const auto & it1, const auto & it2 ) { return it1.pt.x < it2.pt.x; } );
	for ( const auto & item : vItem )
	{
		mPage[ LOBYTE( item.pt.x / 16 ) ].push_back( item );
	}

	// set 'new page' flag for items in each pages, except of first page

	for ( auto & item : mPage )
	{
		if ( item.first > 0 )
		{
			item.second[ 0 ].item.fNewPage = TRUE;
		}
	}
}

// deletes duplicated pointers from the 'tail' pointers sets as follows:
// * if first occurance of world in original vector is equal to current - remove it from 'tail', repeat with current position
// * if first occurance of world in original vector is NOT equal to current - skip check, go to the next pointer in the 'tail'
VOID CNesLevel::RemoveDuplicatePointers( const std::vector<NES_LEVEL_ENEMY_ITEM>& v, std::vector<NES_LEVEL_ENEMY_ITEM>& tail )
{
	size_t i = 0;
	auto it = tail.begin() + i;
	if ( !v.size() ) return;

	while ( it < tail.end() )
	{
		NES_LINK lnkTgt;
		BOOL fFound = FALSE;
		auto & itemTgt = *it;
		lnkTgt.aptrTarget.bPtr = itemTgt.item.bData[ 1 ] & 0x7f;
		lnkTgt.uSourceWorld = ( itemTgt.item.bData[ 2 ] >> 5 );
		lnkTgt.uTargetPage = ( itemTgt.item.bData[ 2 ] & 0x1f );

		for ( auto itb = v.crbegin(); itb < v.crend(); ++itb )
		{
			const auto & itemSrc = *itb;
			NES_LINK lnkSrc;
			lnkSrc.aptrTarget.bPtr = itemSrc.item.bData[ 1 ] & 0x7f;
			lnkSrc.uSourceWorld = ( itemSrc.item.bData[ 2 ] >> 5 );
			lnkSrc.uTargetPage = ( itemSrc.item.bData[ 2 ] & 0x1f );
			if ( lnkSrc.uSourceWorld == lnkTgt.uSourceWorld )
			{
				if ( lnkSrc.uTargetPage == lnkTgt.uTargetPage && lnkSrc.aptrTarget.bPtr == lnkTgt.aptrTarget.bPtr )
				{
					tail.erase( it );
					fFound = TRUE;
				}
				break;
			}
		}

		if ( !fFound )
		{
			i++;
		}

		it = tail.begin() + i;
	}
}

VOID CNesLevel::DumpPointersAsObjects( std::vector<NES_LEVEL_ENEMY_ITEM>& vItem )
{
	for ( const auto & obj : m_obj.vpItem )
	{
		if ( obj->IsEnterableObject() )
		{
			std::vector<NES_LEVEL_ENEMY_ITEM> vLocalItem;

			obj->DumpPointers( vLocalItem );

			RemoveDuplicatePointers( vItem, vLocalItem );
			vItem.insert( vItem.end(), vLocalItem.begin(), vLocalItem.end() );
		}
	}
}

VOID CNesLevel::DumpEnemies( std::vector<BYTE>& vDump, const std::vector<NES_LEVEL_ENEMY_ITEM>& vItem )
{
	for ( const auto & it : vItem )
	{
		AddEnemyToDump( vDump, it );
	}
}

VOID CNesLevel::AddEnemyToDump( std::vector<BYTE>& vDump, const NES_LEVEL_ENEMY_ITEM & item )
{
	vDump.push_back( item.item.bData[ 0 ] );
	vDump.push_back( item.item.bData[ 1 ] );

	if ( NES_SPECIAL_OBJECT_E == item.item.Y )
	{
		vDump.push_back( item.item.bData[ 2 ] );
	}
}

LONG CNesLevel::ParseEnemyData( std::vector<BYTE>::const_iterator enmIt, std::vector<BYTE>::const_iterator enmEnd, std::vector<NES_LINK> & vLinks )
{
	LONG uPage = 0, uNewPage = 0;
	LONG fPageSel = 0;

	// if ( m_lvlPtr.bAreaId == 0x00 && m_lvlPtr.bType == 0x02 )
	//	__debugbreak();

	while ( enmIt < enmEnd )
	{
		CNesEnemyObject * pItem = nullptr;
		NES_LEVEL_ENEMY_ITEM item;
		ASSERT_EOF( enmIt, enmEnd );
		item.item.bData[ 0 ] = *( enmIt++ );
		ASSERT_EOF( enmIt, enmEnd );
		item.item.bData[ 1 ] = *( enmIt++ );

		if ( item.item.fNewPage && !fPageSel )
		{
			uPage++;
			fPageSel = TRUE;
		}

		item.pt.x = 16 * uPage + item.item.X;
		item.pt.y = item.item.Y;
		switch ( item.item.Y )
		{
			case NES_SPECIAL_OBJECT_E:
				{
					NES_LINK lnk;
					ASSERT_EOF( enmIt, enmEnd );
					item.item.bData[ 2 ] = *( enmIt++ );
					// pItem = FetchEnemyItem( item );
					// It's a pointer, fetch pointer and attach it to entrance object
					lnk.aptrTarget.bPtr = item.item.bData[ 1 ] & 0x7f;
					lnk.uSourceWorld = ( item.item.bData[ 2 ] >> 5 );
					lnk.uTargetPage = ( item.item.bData[ 2 ] & 0x1f );
					lnk.ptLink = item.pt;
					vLinks.push_back( lnk );
					break;
				}
			case NES_SPECIAL_OBJECT_F:
				{
					uNewPage = ( item.item.objectId & 0x3f );
					if ( !fPageSel )
					{
						if ( uNewPage >= uPage )
						{
							fPageSel = TRUE;
							uPage = uNewPage;
						}
						else
						{
							_tprintf( TEXT( "WARNING! Enemy page object points to BEFORE page at %d:%Iu\n" ), AreaType(), m_lvlMeta.uLevelId );
						}
					}
					/*
					else
					{
						enmIt++;
						enmIt++;
					}
					*/
					continue;
				}
			default:
				{
					pItem = FetchEnemyItem( item );
					break;
				}
		}

		if ( pItem )
		{
			m_obj.vpEnemy.push_back( pItem );
		}

		fPageSel = 0;
	}

	return uPage;
}

CNesEnemyObject * CNesLevel::FetchEnemyItem( const NES_LEVEL_ENEMY_ITEM & item )
{
	switch ( item.item.objectId & 0x3f )
	{
		case 0x00:		
		case 0x3B:
		case 0x3C:
		case 0x3D:
		case 0x3E: return new CNesKoopaTroopaObject( *this, item );
		case 0x01:
		case 0x03: return new CNesRedKoopaTroopaObject( *this, item );
		case 0x02: return new CNesBuzzyBeetleObject( *this, item );
		case 0x05: return new CNesHummerBro( *this, item );
		case 0x06: 
		case 0x37:
		case 0x38:
		case 0x39:
		case 0x3A: return new CNesGoombaObject( *this, item );
		case 0x07: return new CNesBlooberObject( *this, item );
		case 0x08: 
		case 0x16: return new CNesBulletBillObject( *this, item );
		case 0x09: 
		case 0x0E: 
		case 0x10: return new CNesKoopaParatroopaObject( *this, item );
		case 0x0F: return new CNesRedKoopaParatroopaObject( *this, item );
		case 0x0A: return new CNesCheepCheepObject( *this, item );
		case 0x0B: return new CNesRedCheepCheepObject( *this, item );
		case 0x0C: return new CNesPodobooObject( *this, item );
		case 0x04: 
		case 0x0D: return new CNesPirahnaPlantObject( *this, item );
		case 0x11: return new CNesLakituObject( *this, item );
		case 0x12: return new CNesSpinyObject( *this, item );
		case 0x14: return new CNesRedCheepCheepObject( *this, item );
		case 0x15: return new CNesBowserFlameObject( *this, item ); 
		case 0x17: return new CNesEnemyFrenzyObject( *this, item );
		case 0x18: return new CNesEOFFrenzy( *this, item ); 
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22: return new CNesFirebarObject( *this, item ); 
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2A: return new CNesLargePlatformObject( *this, item ); 
		case 0x2B:
		case 0x2C: return new CNesSmallPlatformObject( *this, item );
		case 0x2D: return new CNesBowserObject( *this, item ); 
		case 0x2E: return new CNesPowerupObject( *this, item ); 
		case 0x2F: return new CNesVineObject( *this, item ); 
		case 0x34: return new CNesWarpObject( *this, item ); 
		case 0x35: return new CNesRetainerObject( *this, item ); 
		default: return nullptr;
	}

	return nullptr;
}
