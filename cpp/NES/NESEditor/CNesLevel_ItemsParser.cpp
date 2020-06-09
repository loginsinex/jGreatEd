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

size_t CNesLevel::SerializeLevelData( std::vector<BYTE>& vLevelData )
{
	std::vector<NES_LEVEL_ITEM> vItem;
	std::map<BYTE, std::vector<NES_LEVEL_ITEM>> mPage;
	std::map<BYTE, std::vector<CNesLoopCommand*>> mLoop;
	size_t cObjects = m_obj.vpItem.size();
	BYTE bPage = 0;
	vLevelData.push_back( LOBYTE( m_header.GetRawHeader() ) );
	vLevelData.push_back( HIBYTE( m_header.GetRawHeader() ) );
	m_liGround.DumpBoundsAsObjects( vItem );

	DumpLoops( mLoop );

	for ( size_t i = 1; i < cObjects; ++i )
	{
		CNesLevelItem * pObj = m_obj.vpItem[ i ];
		NES_LEVEL_ITEM item = { 0 };
		const POINT pt = pObj->GetRealPoint();

		item.item.itemId = pObj->Item().item.itemId;
		item.pt.x = pt.x;
		item.pt.y = pt.y;
		item.item.X = LOBYTE( item.pt.x ) & 0x0F;
		item.item.Y = LOBYTE( item.pt.y ) & 0x0F;
		
		vItem.push_back( item );
	}

	SplitItemsToPages( mPage, vItem );
	vItem.clear();

	for ( const auto & pgLoop : mLoop )
	{
		NES_LEVEL_ITEM dummyItem = { 0 };
		dummyItem.item.itemId = 0x4F;	// 0x4f - prohibited objects for Y=0x0D, so use it for dummy
		dummyItem.item.Y = NES_SPECIAL_OBJECT_D;
		mPage[ pgLoop.first ].push_back( dummyItem );
	}

	for ( auto & item : mPage )
	{
		auto itLoop = mLoop.find( bPage );
		auto itLoopNext = mLoop.find( bPage + 1 );
		BOOL fLoopRewind = ( itLoop != mLoop.end() );
		BOOL fLoopRewindNext = ( itLoopNext != mLoop.end() );

		// dump loop rewind with page skipper even if page 0
		// usualy we've not place page skipper for page 1 (its enough flag NewPage of first object on the page 1)
		if ( !bPage && 1 == item.first && !fLoopRewindNext )
		{
			// realy this flag is already set by SplitItemsToPages subroutine
			// but without this ^^^ clause page skipper will be added
			item.second[ 0 ].item.fNewPage = 1;
			DumpItems( vLevelData, item.second );
			bPage = item.first;
		}
		else if ( bPage != item.first || fLoopRewind )
		{
			// skip some of pages and drop 'new page flag' for object on next page
			// if looper points to this page, so create dummy PAGE_SKIPPER object that points to itself page
			NES_LEVEL_ITEM itemPage = { 0 };
			itemPage.item.itemId = ( item.first & 0x3F );
			itemPage.item.Y = NES_SPECIAL_OBJECT_D;
			itemPage.item.X = 0x00;
			bPage = item.first;
			item.second[ 0 ].item.fNewPage = 0;

			// recheck loop rewind if page was changed
			fLoopRewind = ( mLoop.end() != ( itLoop = mLoop.find( bPage ) ) );

			if ( fLoopRewind )
			{
				for ( auto lp : itLoop->second )
				{
					lp->RewindOffset( LOBYTE( vLevelData.size() ) - sizeof( NES_LEVEL_HEADER ) );
				}
			}

			AddItemToDump( vLevelData, itemPage );
			DumpItems( vLevelData, item.second );
		}
		else
		{
			DumpItems( vLevelData, item.second );
		}

		bPage++;
	}

	return ( vLevelData.size() );
}

VOID CNesLevel::SplitItemsToPages( std::map<BYTE, std::vector<NES_LEVEL_ITEM>>& mPage, std::vector<NES_LEVEL_ITEM>& vItem )
{
	std::sort( vItem.begin(), vItem.end(), []( const NES_LEVEL_ITEM & it1, const NES_LEVEL_ITEM & it2 )
	{ 
		WORD bData1 = MAKEWORD( LOBYTE( ( ( it1.pt.x & 0x0F ) << 4 ) | it1.pt.y & 0x0F ), LOBYTE( it1.pt.x >> 4 ) );
		WORD bData2 = MAKEWORD( LOBYTE( ( ( it2.pt.x & 0x0F ) << 4 ) | it2.pt.y & 0x0F ), LOBYTE( it2.pt.x >> 4 ) );

		return ( bData1 < bData2 );
	} 
	);

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

VOID CNesLevel::DumpItems( std::vector<BYTE>& vDump, const std::vector<NES_LEVEL_ITEM>& vItem )
{
	for ( const auto & it : vItem )
	{
		AddItemToDump( vDump, it );
	}
}

VOID CNesLevel::AddItemToDump( std::vector<BYTE>& vDump, const NES_LEVEL_ITEM & item )
{
	if ( !( NES_SPECIAL_OBJECT_D == item.item.Y && item.item.itemId == 0x4F ) )	// prohibited (dummy?) object found
	{
		vDump.push_back( item.item.bData[ 0 ] );
		vDump.push_back( item.item.bData[ 1 ] );
	}
#ifdef _DEBUG
	else
	{
		_tprintf( TEXT( "Found prohibited object: 0x%02X in area 0x%02X\n" ), item.item.itemId, static_cast<BYTE>( AreaPtr().bPtr ) );
	}
#endif
}

LONG CNesLevel::ParseLevelData( std::vector<BYTE>::const_iterator areaIt, std::vector<BYTE>::const_iterator areaEnd )
{
	LONG uPage = 0, uNewPage = 0, fPageSel = 0, uOldX = 0, fPageSkip = 0;
	NES_GROUND_INFO ngi;
	ngi.fCloudLevel = m_header.IsCloudLevel();
	ngi.bGroundCtl = m_header.GroundCtl();
	ngi.bBgCtl = m_header.BgColorCtl();
	ngi.bFgScenery = m_header.FgScenery();
	ngi.bBgScenery = m_header.BgScenery();

	while ( areaIt < areaEnd )
	{
		CNesLevelItem * pItem = nullptr;
		NES_LEVEL_ITEM item;
		ASSERT_EOF( areaIt, areaEnd );
		item.item.bData[ 0 ] = *( areaIt++ );

		ASSERT_EOF( areaIt, areaEnd );
		item.item.bData[ 1 ] = *( areaIt++ );

		if ( item.item.fNewPage )
		{
			uPage++;
			fPageSel = TRUE;
			fPageSkip = FALSE;
		}

		item.pt.y = item.item.Y;
		item.pt.x = 16 * uPage + item.item.X;

		if ( item.pt.y == NES_SPECIAL_OBJECT_D && ( !( item.item.itemId & 0x40 ) ) )
		{
			if ( !fPageSel )
			{
				uNewPage = item.item.itemId & 0x3f;
				if ( uNewPage >= uPage )
				{
					fPageSel = TRUE;
					uPage = uNewPage;
				}
				else
				{
					_tprintf( TEXT( "WARNING: Page objects points to BEFORE page at %d:%Iu!\n" ), AreaType(), m_lvlMeta.uLevelId );
				}
			}
			/*
			else
			{
				areaIt++;
				areaIt++;
			}
			*/
			continue;
		}
		else
		{
			if ( item.pt.x < uOldX || fPageSkip )	// dont load objects thats places AFTER previous
			{
				fPageSkip = TRUE;
				continue;
			}

			uOldX = item.pt.x;

			if ( item.pt.y == NES_SPECIAL_OBJECT_E || ( item.pt.y == NES_SPECIAL_OBJECT_D && 0x4E == item.item.itemId ) )
			{
				ParseGroundData( ngi, item, uPage );
			}
			else
			{
				pItem = FetchLevelItem( item );
			}
		}

		if ( pItem )
		{
			m_obj.vpItem.push_back( pItem );
		}

		fPageSel = 0;
	}

	return uPage;
}

VOID CNesLevel::ParseGroundData( NES_GROUND_INFO & ngi, const NES_LEVEL_ITEM & item, LONG uPage )
{
	NES_GROUND_TYPE ngt = ngtGroundMod;
	if ( item.item.itemId & 0x40 )
	{
		BYTE bScenery = item.item.itemId & 0x07;
		if ( bScenery < fgSceneryMax )
		{
			ngi.bFgScenery = static_cast<NES_FG_SCENERY>( bScenery );
		}
		else
		{
			ngi.bFgScenery = fgScenery0;
			ngi.bBgCtl = static_cast<NES_BG_COLOR_CTL>( bScenery );
		}
		ngt = ngtSceneMod;
	}
	else
	{
		ngi.bGroundCtl = item.item.itemId & 0x0f;
		ngi.bBgScenery = static_cast<NES_BG_SCENERY>( ( item.item.itemId & 0x30 ) >> 4 );
	}

	m_liGround.AddGroundInfo( item.pt.x + 1, ngt, ngi );
}

CNesLevelItem * CNesLevel::FetchLevelItem( const NES_LEVEL_ITEM & item )
{
	switch ( item.item.Y )
	{
		case NES_SPECIAL_OBJECT_F: return FetchMasvItemF( item );
		case NES_SPECIAL_OBJECT_D: return FetchMasvItemD( item );
		case NES_SPECIAL_OBJECT_C: return FetchMasvItemC( item );
		default:
			{
				if ( item.item.itemId < 0x10 )
				{
					return FetchSingleItem( item );
				}
				else
				{
					return FetchLongItem( item );
				}
				break;
			}
	}
	return nullptr;
}

CNesLevelItem * CNesLevel::FetchSingleItem( const NES_LEVEL_ITEM & item )
{
	switch ( item.item.itemId )
	{
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		case 0x0A: 
		case 0x0B:
		case 0x0C:
		case 0x0E: return new CNesQBlock( *this, item );
		case 0x0D: return new CNesWaterPipe( *this, item );
		case 0x0F: return new CNesJumpspring( *this, item );
	}
	return nullptr;
}

CNesLevelItem * CNesLevel::FetchLongItem( const NES_LEVEL_ITEM & item )
{
	BYTE itemId = ( ( item.item.itemId & 0xF0 ) >> 4 );
	BYTE itemLen = ( item.item.itemId & 0x0F ) + 1;

	switch ( itemId )
	{
		case 1: return new CNesAreaStyleLedge( *this, item );
		case 2: return new CNesRowOfBricks( *this, item );
		case 3: return new CNesRowOfSolid( *this, item );
		case 4: return new CNesRowOfCoins( *this, item );
		case 5: return new CNesVertBricksColumn( *this, item );
		case 6: return new CNesVertSolidColumn( *this, item );
		case 7: return new CNesVertPipe( *this, item );
	}

	return nullptr;
}

CNesLevelItem * CNesLevel::FetchMasvItemC( const NES_LEVEL_ITEM & item )
{
	BYTE itemId = ( ( item.item.itemId & 0xF0 ) >> 4 );
	BYTE itemLen = ( item.item.itemId & 0x0F ) + 1;

	switch ( itemId )
	{
		case 0: return new CNesHole( *this, item );
		case 1: return new CNesPulleyRopeObject( *this, item );
		case 2: return new CNesBridgeHigh( *this, item );
		case 3: return new CNesBridgeMiddle( *this, item );
		case 4: return new CNesBridgeLow( *this, item );
		case 5: return new CNesWater( *this, item );
		case 6: return new CNesQBlocksRowHigh( *this, item );
		case 7: return new CNesQBlocksRowLow( *this, item );
	}

	return nullptr;
}

CNesLevelItem * CNesLevel::FetchMasvItemD( const NES_LEVEL_ITEM & item )
{
	switch ( item.item.itemId )
	{
		case 0x40: return new CNesIntroPipe( *this, item );
		case 0x41: return new CNesFlagpoleObject( *this, item );
		case 0x42: return new CNesAxeObject( *this, item );
		case 0x43: return new CNesChainObject( *this, item );
		case 0x44: return new CNesCastleBridgeObject( *this, item );
		case 0x45: return new CNesWarpScrollStop( *this, item );
		case 0x46: return new CNesScrollStop( *this, item );
		case 0x47: return new CNesScrollStop( *this, item );
		case 0x48: return new CNesFrenzyObject1( *this, item );
		case 0x49: return new CNesFrenzyObject2( *this, item );
		case 0x4A: return new CNesFrenzyObject3( *this, item );
		case 0x4B: return new CNesLoopCommand( *this, item );
		case 0x4C: return new CNesWind( *this, item );
		case 0x4D: return new CNesWind( *this, item );
		case 0x4E: // alter area attributes?
		case 0x4F: break;	// forbidden objects

	}
	return nullptr;
}

CNesLevelItem * CNesLevel::FetchMasvItemF( const NES_LEVEL_ITEM & item )
{
	BYTE itemId = ( ( item.item.itemId & 0xF0 ) >> 4 );
	BYTE itemLen = ( item.item.itemId & 0x0F ) + 1;

	switch ( itemId )
	{
		case 0: return new CNesRope( *this, item );
		case 1: return new CNesBalanceRope( *this, item );
		case 2: return new CNesCastle( *this, item );
		case 3: return new CNesStaircase( *this, item );
		case 4: return new CNesReversePipe( *this, item );
		case 5: return new CNesUnusedRope( *this, item );
		case 6: return new CNesUpsidePipe( *this, item );
		case 7: return new CNesUpsidePipe( *this, item );
	}
	return nullptr;
}