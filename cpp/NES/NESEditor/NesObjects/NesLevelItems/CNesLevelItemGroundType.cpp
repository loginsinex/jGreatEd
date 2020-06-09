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

CNesLevelItemGroundType::CNesLevelItemGroundType( CNesLevel & nLevel )
	: CNesLevelItem( nLevel ), m_grData( nLevel.ObjData().GroundData() )
{
	FixHeight( 0 );
	m_szLevel.cx = m_szLevel.cy = 0;
}

VOID CNesLevelItemGroundType::OverrideForeground( NES_BG_COLOR_CTL uForeground )
{
	if ( m_vGroundInfo.size() > 0 )
	{
		m_vGroundInfo[ 0 ].gr[ ngtSceneMod ].bBgCtl = uForeground;
	}
}

VOID CNesLevelItemGroundType::DropOverridenForeground()
{
	m_vGroundInfo[ 0 ].gr[ ngtSceneMod ].bBgCtl = Level().Header().BgColorCtl();
}

BYTE CNesLevelItemGroundType::BgCtl()
{
	if ( m_vGroundInfo.size() > 0 )
	{
		return m_vGroundInfo[ 0 ].gr[ ngtSceneMod ].bBgCtl;
	}

	return 0;
}

VOID CNesLevelItemGroundType::InitGroundInfo()
{
	CNesLevel & nLevel = Level();

	if ( m_vGroundInfo.size() < 1 )
	{
		NES_GROUND_INFO ngiNull = { 0 };
		AddGroundInfo( 0, ngtGroundMod, ngiNull );
		AddGroundInfo( 0, ngtSceneMod, ngiNull );
	}
	
	NES_GROUND_INFO & ngiBg = m_vGroundInfo[ 0 ].gr[ ngtGroundMod ];
	NES_GROUND_INFO & ngiSc = m_vGroundInfo[ 0 ].gr[ ngtSceneMod ];
	const CNesLevelHeader & header = nLevel.Header();

	m_szLevel = nLevel.LevelSize();
	ngiBg.fCloudLevel = ngiSc.fCloudLevel = header.IsCloudLevel();
	ngiBg.bBgScenery = header.BgScenery();
	ngiSc.bBgCtl = header.BgColorCtl();
	ngiSc.bFgScenery = header.FgScenery();
	ngiBg.bGroundCtl = header.GroundCtl();
	BuildGroundLinks();
}

VOID CNesLevelItemGroundType::InitDrawQueue( std::vector<NES_DRAW_QUEUE_ITEM>& vDrawQueue )
{
	size_t cItems = m_vGroundInfo.size();
	vDrawQueue.reserve( vDrawQueue.size() + m_vGroundInfo.size() );
	for ( size_t i = 1; i < cItems; ++i )
	{
		auto & grInfo = m_vGroundInfo[ i ];
		NES_DRAW_QUEUE_ITEM dqi = { nullptr, &grInfo, grInfo.X - 1, grInfo.X - 1, NES_SPECIAL_OBJECT_E };
		vDrawQueue.push_back( std::move( dqi ) );
	}
}

VOID CNesLevelItemGroundType::AddGroundInfo( int X, NES_GROUND_TYPE ngt, const NES_GROUND_INFO & vGround )
{
	if ( X >= 0 )
	{
		std::map<NES_GROUND_TYPE, NES_GROUND_INFO> mGround;
		mGround[ ngt ] = vGround;
		NES_GROUND_PLACE vPlace = { X, FALSE, mGround };

		auto it = FindGround( X );

		if ( it != m_vGroundInfo.end() )
		{
			it->gr[ ngt ] = vGround;
		}
		else
		{
			m_vGroundInfo.push_back( vPlace );
		}

		BuildGroundLinks();
	}
}

VOID CNesLevelItemGroundType::DrawBounds( CNesLevelView & view )
{
	size_t c = m_vGroundInfo.size();

	view.ResetBounds();

	for ( size_t i = 1; i < c; ++i )
	{
		view.AddBound( m_vGroundInfo[ i ].X );
	}
}

BOOL CNesLevelItemGroundType::MoveGround( int uOldX, int uNewX )
{
	if ( uOldX <= 0 || uNewX <= 0 || uOldX >= Width() || uNewX >= Width() )
	{
		return FALSE;
	}

	auto it1 = FindGround( uOldX );
	auto it2 = FindGround( uNewX );

	if ( it2 != m_vGroundInfo.end() )
	{
		return FALSE;
	}

	it1->X = uNewX;
	BuildGroundLinks();
	InitObject();
	return TRUE;
}

BOOL CNesLevelItemGroundType::GetGround( int X, std::map <NES_GROUND_TYPE, NES_GROUND_INFO> & gr )
{
	if ( X < 1 )
	{
		return FALSE;
	}

	auto it = FindGround( X );
	if ( it != m_vGroundInfo.end() )
	{
		gr = it->gr;
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevelItemGroundType::EraseGround( int X )
{
	if ( X < 1 )
	{
		return FALSE;
	}

	auto it = FindGround( X );
	if ( it != m_vGroundInfo.end() )
	{
		m_vGroundInfo.erase( it );
		BuildGroundLinks();
		InitObject();
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevelItemGroundType::DumpBoundsAsObjects( std::vector<NES_LEVEL_ITEM>& vBounds )
{
	size_t cBounds = m_vGroundInfo.size();

	for ( size_t i = 1; i < cBounds; ++i )
	{
		const NES_GROUND_PLACE & ntgi = m_vGroundInfo[ i ];

		for ( const auto & itGround : ntgi.gr )
		{
			const NES_GROUND_TYPE ngt = itGround.first;
			const NES_GROUND_INFO & gi = itGround.second;

			NES_LEVEL_ITEM item = { 0 };
			item.pt.x = ntgi.X - 1;
			item.pt.y = 0x0E;
			item.item.X = LOBYTE( item.pt.x ) & 0x0F;
			item.item.Y = 0x0E;
			if ( ngtGroundMod == ngt )
			{
				item.item.itemId = gi.bGroundCtl & 0x0F;
				item.item.itemId |= ( ( gi.bBgScenery & 3 ) << 4 );
			}
			else
			{
				item.item.itemId = 0x40;

				if ( gi.bFgScenery )
				{
					item.item.itemId |= ( gi.bFgScenery & 3 );
				}
				else if ( gi.bBgCtl >= 4 && gi.bBgCtl <= 7 )
				{
					item.item.itemId |= gi.bBgCtl;
				}
			}

			vBounds.push_back( item );
		}
	}

	return ( vBounds.size() > 0 );
}

VOID CNesLevelItemGroundType::BuildGroundLinks()
{
	const size_t uNull = static_cast<size_t>( -1 );
	const SIZE szLevel = Level().LevelSize();
	const size_t cGroundData = m_vGroundInfo.size();
	m_vGround.clear();

	DropOverridenForeground();
	std::sort( m_vGroundInfo.begin(), m_vGroundInfo.end(), [] ( const NES_GROUND_PLACE & pl1, const NES_GROUND_PLACE & pl2 ) { return pl1.X < pl2.X; } );

	if ( szLevel.cx > 0 )
	{
		m_vGround.insert( m_vGround.begin(), szLevel.cx, uNull );

		// generate points for filling vector
		for ( size_t i = 0; i < cGroundData; ++i )
		{
			const NES_GROUND_PLACE & pl = m_vGroundInfo[ i ];
			if ( pl.X >= 0 && pl.X < szLevel.cx )
			{
				m_vGround[ pl.X ] = i;
				auto itGround = pl.gr.find( ngtSceneMod );
				
				// check for override background
				if ( itGround != pl.gr.end() )
				{
					const auto & gr = itGround->second;
					if ( pl.X <= 24 && gr.bBgCtl && !pl.fBehind )
					{
						OverrideForeground( gr.bBgCtl );
					}
				}
			}
		}

		// fill points in the ground vector
		size_t pt = m_vGround[ 0 ];
		for ( int x = 1; x < szLevel.cx; ++x )
		{
			if ( m_vGround[ x ] == uNull )
			{
				m_vGround[ x ] = pt;
			}
			else
			{
				pt = m_vGround[ x ];
			}
		}
	}
}

const NES_GROUND_PLACE & CNesLevelItemGroundType::GroundDataEx( int x )
{
	if ( x < m_szLevel.cx )
	{
		return m_vGroundInfo[ m_vGround[ x ] ];
	}

	throw std::exception( "Out of bounds ground was requested" );
}

const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & CNesLevelItemGroundType::GroundData( int x )
{
	if ( x < m_szLevel.cx )
	{
		return m_vGroundInfo[ m_vGround[ x ] ].gr;
	}

	throw std::exception( "Out of bounds ground was requested" );
}

VOID CNesLevelItemGroundType::FillColumnWithTile( std::vector<NES_METATILE>& vColumn, WORD wTemplate, const NES_METATILE & tile )
{
	WORD wBit = 1;
	for ( size_t y = 0; y < 16; ++y, wBit <<= 1 )
	{
		if ( wBit & wTemplate )
		{
			if ( y >= 0x0B && Level().AreaType() == nltUnderground )
			{
				vColumn[ y ] = m_grData.bBlockDefault;
			}
			else
			{
				vColumn[ y ] = tile;
			}
		}
		else
		{
			vColumn[ y ] = { 0 };
		}
	}
}

VOID CNesLevelItemGroundType::LevelSizeChanged( const SIZE & sz )
{
	m_szLevel = sz;
	SetObjWidth( sz.cx );
	SetObjHeight( sz.cy );
	BuildGroundLinks();
}

BOOL CNesLevelItemGroundType::IsEntranceObject()
{
	// when we're as cloud, we can take a pointers
	return Level().Header().IsCloudLevel();
}

BOOL CNesLevelItemGroundType::WidthChanging( int & cx )
{
	cx = m_szLevel.cx;
	return TRUE;
}

BOOL CNesLevelItemGroundType::HeightChanging( int & cy )
{
	cy = m_szLevel.cy;
	return TRUE;
}

BOOL CNesLevelItemGroundType::PlaceChanging( int & x, int & y )
{
	x = y = 0;
	return TRUE;
}

VOID CNesLevelItemGroundType::InitObject()
{
	const int lWidth = Width();
	const POINT ptPlayer = Level().PlayerEntrance().GetPlayerPoint( Level().Header().PlayerCtl() );
	const BOOL fCloudLevel = Level().Header().IsCloudLevel();
	const NES_METATILE nmGroundTile = ( fCloudLevel ? m_grData.bBlockCloud : m_grData.bGroundMTiles[ Level().AreaType() & 3 ] );
	std::vector<NES_METATILE> vColumn( 16, { 0 } );	
	const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & ctngi = GroundData( 0 );
	const auto & giGround = ctngi.find( ngtGroundMod )->second;
	const auto & giScene = ctngi.find( ngtSceneMod )->second;
	NES_GROUND_INFO cngi = { 0 };
	BYTE bSceneryBg = 0, bSceneryFg = 0;
	WORD wGround = m_grData.bGroundBits[ giGround.bGroundCtl & 0x0F ];
	cngi.bBgCtl = giScene.bBgCtl;
	cngi.bBgScenery = giScene.bBgScenery;
	cngi.bFgScenery = giScene.bFgScenery;
	cngi.bGroundCtl = giGround.bGroundCtl;
	cngi.fCloudLevel = fCloudLevel;

	if ( fCloudLevel )
	{
		wGround &= 0x08FF;
	}

	if ( cngi.bBgScenery )
	{
		bSceneryBg = m_grData.bBgSceneOffset[ cngi.bBgScenery - 1 ];
	}

	if ( cngi.bFgScenery )
	{
		bSceneryFg = m_grData.bFgSceneOffset[ cngi.bFgScenery - 1 ];
	}

	FillColumnWithTile( vColumn, wGround, nmGroundTile );
	int xGround = 0;
	for ( int x = 0; x < lWidth; ++x )
	{
		const int p = x / 16;
		const NES_GROUND_PLACE & ngrpl = GroundDataEx( x );
		if ( !ngrpl.fBehind )
		{
			xGround = x;
		}

		const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & ntgi = GroundData( xGround );
		for ( const auto & itngi : ntgi )
		{
			const NES_GROUND_TYPE ngt = itngi.first;
			const NES_GROUND_INFO & ngi = itngi.second;

			if ( ngtGroundMod == ngt )
			{
				WORD wNewGround = m_grData.bGroundBits[ ngi.bGroundCtl & 0x0F ];
				if ( wNewGround != wGround )
				{
					wGround = wNewGround;
					if ( fCloudLevel )
					{
						wGround &= 0x08FF;
					}

					FillColumnWithTile( vColumn, wGround, nmGroundTile );
				}

				cngi.bBgScenery = ngi.bBgScenery;
			}
			else
			{
				if ( ngi.bFgScenery )
				{
					bSceneryFg = m_grData.bFgSceneOffset[ ngi.bFgScenery - 1 ];
				}
				cngi.bFgScenery = ngi.bFgScenery;
			}

			// update bg scenery on every new page
			if ( cngi.bBgScenery )
			{
				bSceneryBg = m_grData.bBgSceneOffset[ cngi.bBgScenery - 1 ] + ( ( p % 3 ) << 4 );
			}

			for ( BYTE y = 0; y < 16; ++y )
			{
				NES_METATILE nTile = vColumn[ y ];
				if ( !nTile.tileId )
				{
					if ( cngi.bBgScenery && y < NES_SPECIAL_OBJECT_C - 1 )
					{
						BYTE bSceneryData = m_grData.bBgSceneData[ bSceneryBg + ( x & 0x0F ) ];
						if ( bSceneryData )
						{
							BYTE yScenery = ( ( bSceneryData & 0xf0 ) >> 4 );
							if ( yScenery <= y && y - yScenery < 3 )
							{
								BYTE bSceneryOffset = 3 * ( ( bSceneryData & 0x0f ) - 1 ) + ( y - yScenery );
								nTile = m_grData.bBgSceneMTiles[ bSceneryOffset ];
							}
						}
					}

					if ( cngi.bFgScenery && y < NES_SPECIAL_OBJECT_D )
					{
						if ( m_grData.bFgSceneData[ bSceneryFg + y ].tileId )
							nTile = m_grData.bFgSceneData[ bSceneryFg + y ];
					}
				}

				SetMetatile( x, y, nTile );
			}
		}
	}

	FreeSprites();
	SetSprite( npstRunning, 0, ptPlayer.x, 5 + max( ptPlayer.y, 3 ) );
	SetSprite( npstRunning + NES_PLAYER_GFX_OFFSET, 0, ptPlayer.x, 5 + max( ptPlayer.y, 3 ) );
}

NES_METATILE CNesLevelItemGroundType::HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile )
{
	if (
		bWorldId == m_grData.bSpecWaterBlockForWorld &&
		Level().AreaType() == nltWater &&
		newTile.bMetatile == m_grData.bGroundMTiles[ Level().AreaType() ].bMetatile
		)
	{
		return m_grData.bSpecWaterBlock;
	}

	return newTile;
}

std::vector<NES_GROUND_PLACE>::iterator CNesLevelItemGroundType::FindGround( int X )
{
	return std::find_if( m_vGroundInfo.begin(), m_vGroundInfo.end(),
							[ &X ] ( const NES_GROUND_PLACE & place )
							{
								return ( place.X == X );
							} );
}

VOID CNesLevelItemGroundType::AddPage( UINT uPage )
{
	int x0 = uPage * 16;
	std::find_if(m_vGroundInfo.begin(),m_vGroundInfo.end(),
		[x0]( NES_GROUND_PLACE & gr )
	{
		if ( gr.X > 0 && gr.X >= x0 )
		{
			gr.X += 0x10;
		}
		return false;
	} );

	BuildGroundLinks();
}

VOID CNesLevelItemGroundType::DelPage( UINT uPage )
{
	int x0 = uPage * 16;
	int x1 = x0 + 0x10;
	auto it = m_vGroundInfo.begin();

	while ( m_vGroundInfo.end() !=
		(
			it = std::find_if( m_vGroundInfo.begin(), m_vGroundInfo.end(),
				[x0, x1]( NES_GROUND_PLACE & gr )
	{
		if ( gr.X > 0 && gr.X >= x0 && gr.X < x1 )
		{
			return true;
		}
		return false;
	} ) ) )
	{
		m_vGroundInfo.erase( it );
	};

	std::find_if( m_vGroundInfo.begin(), m_vGroundInfo.end(),
		[x0]( NES_GROUND_PLACE & gr )
	{
		if ( gr.X > 0 && gr.X >= x0 )
		{
			gr.X -= 0x10;
		}
		return false;
	} );

	BuildGroundLinks();
}

VOID CNesLevelItemGroundType::Flush()
{
	m_vGround.clear();
	m_vGroundInfo.clear();
	FreeSprites();
	FreeMetatiles();
}

VOID CNesLevelItemGroundType::TakeGroundSnapshot( std::vector<NES_GROUND_PLACE>& vSnapshot )
{
	vSnapshot = m_vGroundInfo;
}

VOID CNesLevelItemGroundType::RevertGroundSnapshot( const std::vector<NES_GROUND_PLACE>& vSnapshot )
{
	Flush();
	InitGroundInfo();

	for ( const auto & ngp : vSnapshot )
	{
		for ( const auto & itGround : ngp.gr )
		{
			AddGroundInfo( ngp.X, itGround.first, itGround.second );
		}
	}

	InitObject();
}