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

#include "h/NES/NESEditor/NESEditor.h"

CNesLevelSet::CNesLevelSet( CNesLevelsHolder & holder, NES_AREA_SET areaSet )
	:
	m_holder( holder ),
	m_file( holder.File() ),
	m_eptr( holder.Pointers() ),
	m_objectData( holder.File(), holder.Pointers(), areaSet ),
	m_areaSet( areaSet ),
	m_links( areaSet )
{
	InitArrays();
}

CNesLevelSet::~CNesLevelSet()
{
	ClearSet();
}

VOID CNesLevelSet::InitArrays()
{
	NES_LEVEL_TYPE nlt[] = { nltWater, nltGround, nltUnderground, nltCastle };

	// initialize areas array with NULL values
	for ( const auto & type : nlt )
	{
		auto & vAreaList = m_areas[ type ];
		vAreaList.insert( vAreaList.begin(), 0x40, nullptr );
	}

	for ( size_t i = 0; i < NES_WORLDS_COUNT; ++i )
	{
		std::vector<CNesLevel*> vEmptyAreas;
		std::vector<std::vector<CNesLevel*>> vLevel( NES_LEVEL_COUNT, vEmptyAreas );
		m_mWorld.push_back( vLevel );
	}
}

VOID CNesLevelSet::ClearSet()
{
	// don't delete pLevel pointers
	// it will be deleted when CNesLevelsHolders destructor executes

	m_areas.clear();
	m_mWorld.clear();
}

VOID CNesLevelSet::LoadRequiredFiles()
{
	USHORT uFileListPtrLo = m_eptr.Pointer( eFilesListPtrLo );
	USHORT uFileListPtrHi = m_eptr.Pointer( eFilesListPtrHi );
	USHORT uFileListPtr = MAKEWORD( m_file.Data<BYTE>( uFileListPtrLo + m_areaSet ), m_file.Data<BYTE>( uFileListPtrHi + m_areaSet ) );
	BYTE bFileId = 0x00;
	m_vFiles.clear();

	while ( 0xFF != ( bFileId = m_file.Data<BYTE>( uFileListPtr++ ) ) )
	{
		m_file.SelectFile( bFileId );
		m_vFiles.push_back( bFileId );
	}
}

VOID CNesLevelSet::LoadPointers()
{
	USHORT uWorldsOffsetPtr = m_eptr.Pointer( eWorldAreasOffsets );
	USHORT uEnemyOffsetsPtr = m_eptr.Pointer( eEnemyAddrHOffset );
	USHORT uAreasOffsetsPtr = m_eptr.Pointer( eLevelAddrHOffset );
	USHORT uEnemyPtr = m_eptr.Pointer( eEnemyPtr );
	USHORT uAreaPtr = m_eptr.Pointer( eLevelPtr );
	USHORT & uWorldsPtr = m_levelPointers.uWorldsPtr = m_eptr.Pointer( eWorldAreas );
	BYTE & bWorldOffset = m_levelPointers.bWorldOffset;
	BYTE & bWorldCount = m_levelPointers.bWorldCount;
	BYTE bEnemyOffsets[ nltTypeCount ], bAreaOffsets[ nltTypeCount ], bWorldsOffsets[ NES_WORLDS_COUNT ];

	switch ( m_areaSet )
	{
		case asWorld1to4: bWorldOffset = NES_SET1_OFFSET; bWorldCount = NES_WORLDS_COUNT; break;
		case asWorld5to8: bWorldOffset = NES_SET2_OFFSET; bWorldCount = NES_WORLDS_COUNT; break;
		case asWorld9: bWorldOffset = NES_SET3_OFFSET; bWorldCount = 1; break;
		case asWorldAtoD: bWorldOffset = NES_SET4_OFFSET; bWorldCount = NES_WORLDS_COUNT; break;
	}

	CopyMemory( bEnemyOffsets, m_file.Data < BYTE[ nltTypeCount ] > ( uEnemyOffsetsPtr ), nltTypeCount );
	CopyMemory( bAreaOffsets, m_file.Data < BYTE[ nltTypeCount ] > ( uAreasOffsetsPtr ), nltTypeCount );
	CopyMemory( bWorldsOffsets, m_file.Data<BYTE[ NES_WORLDS_COUNT ]>( uWorldsOffsetPtr + bWorldOffset ), NES_WORLDS_COUNT );

	for ( size_t i = 0; i < _ARRAYSIZE( m_levelPointers.uEnemyPtr ); ++i )
	{
		m_levelPointers.uEnemyPtr[ i ] = uEnemyPtr + sizeof( USHORT )*static_cast<USHORT>( bEnemyOffsets[ i ] );
		m_levelPointers.uAreaPtr[ i ] = uAreaPtr + sizeof( USHORT )*static_cast<USHORT>( bAreaOffsets[ i ] );
	}

	for ( size_t i = 0; i < bWorldCount; ++i )
	{
		m_levelPointers.uWorldPtr[ i ] = uWorldsPtr + bWorldsOffsets[ i ];
	}
}

VOID CNesLevelSet::LoadAreas()
{
	for ( size_t w = 0; w < m_levelPointers.bWorldCount; ++w )
	{
		USHORT c = 0;
		for ( int i = 0; i < NES_LEVEL_COUNT; ++i, ++c )
		{
			NES_AREA_PTR areaPtr = m_file.Data<NES_AREA_PTR>( m_levelPointers.uWorldPtr[ w ] + c );
			BOOL fAutoArea = LoadArea( areaPtr );
			CNesLevel * pLevel = m_areas[ areaPtr.bType ][ areaPtr.bAreaId ];

			m_mWorld[ w ][ i ].push_back( pLevel );
			if ( m_holder.IsPatchMode() )
			{				
				/*
				* Loop command found at World 3-4.
				* Loop command found at World 5-3.
				* Loop command found at World 6-4.
				* Loop command found at World 7-2.
				* Loop command found at World 8-4.
				*/

				if ( pLevel->IsObjectPresentOnPageRange( 0, 0x40, 0x4B, NES_SPECIAL_OBJECT_D ) )
				{
					LoadLoops( w, i, pLevel );
				}
			}

			if ( fAutoArea ) i--;	// still previous level after 'auto-level'
		}
	}
}

// returns TRUE if auto-level
BOOL CNesLevelSet::LoadArea( NES_AREA_PTR ptr )
{
	if ( !m_areas[ ptr.bType ][ ptr.bAreaId ] )
	{
		return TryLoadArea( ptr );
	}
	else
	{
		CNesLevel * pLevel = m_areas[ ptr.bType ][ ptr.bAreaId ];
		return ( 2 == ( pLevel->GetLevelStatus() & 2 ) );
	}
}

BOOL CNesLevelSet::IsPointerOutsideSetArea( USHORT uPtr )
{
	return ( m_file.Data<USHORT>( uPtr ) < m_levelPointers.uWorldsPtr );
}

BOOL CNesLevelSet::CheckForLevelIsInOurSection( CNesLevel * pLevel )
{
	BYTE bEnemyFileId = pLevel->Metadata().uDataFileId[ 0 ];
	BYTE bAreaFileId = pLevel->Metadata().uDataFileId[ 1 ];

	// it is not perfect solution, but I dont know another...
	if ( std::find( m_vFiles.begin(), m_vFiles.end(), bEnemyFileId ) != m_vFiles.end() ||
		std::find( m_vFiles.begin(), m_vFiles.end(), bAreaFileId ) != m_vFiles.end() )
	{
		return TRUE;
	}

	return FALSE;
}

// this function used ONLY in PATCH MODE (first load of this ROM)
VOID CNesLevelSet::LoadLoops( size_t world, size_t level, CNesLevel * pLevel )
{
	BYTE uRealWorld = LOBYTE( world );
	NES_AREA_PTR aptr = { 0 };	

	uRealWorld = AreaSet2World( uRealWorld );

#ifdef _DEBUG
	_tprintf( TEXT( "* Found loop data for level %d-%Iu. Trying to load it...\n" ), uRealWorld + 1, level + 1 );
#endif

	aptr.bAreaId = uRealWorld;
	LoadLoops( aptr, pLevel );
}

// this function used ONLY in NORMAL MODE (already patched ROM)
VOID CNesLevelSet::LoadLoops( NES_AREA_PTR aptr, CNesLevel * pLevel )
{
	std::vector<NES_LOOP> vLoop;
	if ( m_holder.Loop().GetAreaLoops( aptr, vLoop ) )
	{
		pLevel->LoadLoops( vLoop );
	}
#ifdef _DEBUG
	else if ( !m_holder.IsPatchMode() )
	{
		_tprintf( TEXT( "* WARNING: Loop commands for area 0x%02X was not loaded, but supplied!\n" ), aptr.bPtr );
	}
#endif
}

BYTE CNesLevelSet::AreaSet2World( BYTE bWorld )
{
	BYTE uRealWorld = bWorld;
	switch ( m_areaSet )
	{
		case asWorld5to8: uRealWorld += 4; break;
		case asWorld9: uRealWorld += 8; break;
		case asWorldAtoD: uRealWorld += 9; break;
		default: break;
	}

	return uRealWorld;
}

VOID CNesLevelSet::MakeWorld5WarpPatch()
{
	BYTE bWorldFirst = AreaSet2World( 0 ), bWorldEnd = bWorldFirst + m_levelPointers.bWorldCount - 1;
	const NES_WARP_WORLD_DATA & wwd = m_objectData.WarpWorldData();
	if ( wwd.bWorldData[ 1 ] >= bWorldFirst && wwd.bWorldData[ 1 ] <= bWorldEnd )
	{
		// fetch level X-1 from worlds list
		BYTE bWorldId = wwd.bWorldData[ 1 ] - bWorldFirst;
		const std::vector<CNesLevel*> & vpLevels = m_mWorld[ bWorldId ][ 0 ];
		if ( vpLevels.size() > 0 )
		{
			m_file.Data<BYTE>( m_eptr[ eWarpControlCode3 ].ptr ) = vpLevels[ 0 ]->GetOptionalAreaId( m_areaSet );
		}
	}
}

BOOL CNesLevelSet::TryLoadArea( NES_AREA_PTR ptr )
{
	BOOL fAutoLevel = FALSE;
	std::vector<NES_LINK> & vLinks = m_levelPointers.mLinks[ ptr ];
	USHORT uAreaPtr = m_levelPointers.uAreaPtr[ ptr.bType ] + sizeof( USHORT )*static_cast<USHORT>( ptr.bAreaId );
	USHORT uEnemyPtr = m_levelPointers.uEnemyPtr[ ptr.bType ] + sizeof( USHORT )*static_cast<USHORT>( ptr.bAreaId );
	const BOOL fOutsideEnemyPtr = IsPointerOutsideSetArea( uEnemyPtr );
	const BOOL fOutsideAreaPtr = IsPointerOutsideSetArea( uAreaPtr );
	CNesLevel * pLevel = m_holder.PushLevel( uEnemyPtr, uAreaPtr, ptr.bType, m_links, m_objectData, vLinks );

	pLevel->SetOptionalAreaId( m_areaSet, ptr.bAreaId );

	if ( !CheckForLevelIsInOurSection( pLevel ) || fOutsideAreaPtr || fOutsideEnemyPtr )
	{
		if ( fOutsideEnemyPtr ) m_holder.MarkLevelAsShared( uEnemyPtr, pLevel );
		if ( fOutsideAreaPtr ) m_holder.MarkLevelAsShared( uAreaPtr, pLevel );
	}

	if ( !m_holder.IsPatchMode() )
	{
		if ( pLevel->IsObjectPresentOnPageRange( 0, 0x40, 0x4B, NES_SPECIAL_OBJECT_D ) )
		{
			LoadLoops( ptr, pLevel );
		}
	}

	fAutoLevel = ( 2 == ( 2 & pLevel->GetLevelStatus() ) );
	m_areas[ ptr.bType ][ ptr.bAreaId ] = pLevel;

	for ( const auto & link : vLinks )
	{
		LoadArea( link.aptrTarget );
	}
	
	return fAutoLevel;
}

CNesLevel * CNesLevelSet::LevelFromPtr( NES_AREA_PTR areaPtr )
{
	return m_areas[ areaPtr.bType ][ areaPtr.bAreaId ];
}

VOID CNesLevelSet::ParseLinks( NES_LINK_ARRAY & mLink )
{
	for ( auto & lnk : mLink )
	{
		CNesLevel * pLevel = LevelFromPtr( lnk.first );
		if ( pLevel )
		{
			std::vector<NES_LINK> & vLink = lnk.second;
			std::sort( vLink.begin(), vLink.end(),
				[]( const auto & it1, const auto & it2 )
			{
				return it1.ptLink.x < it2.ptLink.x;
			} );

			for ( const auto & l : vLink )
			{
				CNesLevel * pTargetLevel = LevelFromPtr( l.aptrTarget );
				if ( pTargetLevel )
				{
					CNesLink * pLink = m_links.AddLink( l.uSourceWorld, pLevel );
					pLink->SetTarget( pTargetLevel, l.uTargetPage );

					if ( !pLevel->AssignLink( pLink, l.ptLink.x ) )
					{
						if ( !pLevel->AssignGroundLink( pLink ) )
						{
							m_links.RemoveLink( pLink );
							_tprintf( TEXT( "WARNING: Removed link for area 0x%0X (point %d). Target is: 0x%0X\n" ),
								lnk.first.bPtr, l.ptLink.x, l.aptrTarget.bPtr );
						}
					}
				}
			}
		}
	}
}

VOID CNesLevelSet::LoadSet()
{
	LoadRequiredFiles();
	m_objectData.LoadData();	// Load objects data AFTER selecting files
	LoadPointers();
	LoadHalfwayPages();
	LoadAreas();
	ParseLinks( m_levelPointers.mLinks );
}

USHORT CNesLevelSet::GetADHalfWayPagesPtr()
{
	USHORT uHalfWayCallPtr = m_eptr.Pointer( eHalfwayADCallPtr );
	return m_file.Data<USHORT>( uHalfWayCallPtr + 3 );		// its LDA #07 \n LDA pAtoDHalfwayPages,Y
}

VOID CNesLevelSet::LoadHalfwayPages()
{
	USHORT uBegin = AreaSet2World( 0 ), uEnd = uBegin + m_levelPointers.bWorldCount;
	USHORT uHPWPtr = 0;
	switch ( m_areaSet )
	{
		case asWorld1to4: uHPWPtr = m_eptr.Pointer( eHPAreaSet0 ); break;
		case asWorld5to8: uHPWPtr = m_eptr.Pointer( eHPAreaSet1 ); break;
		case asWorld9: uHPWPtr = m_eptr.Pointer( eHPAreaSet2 ); break;
		case asWorldAtoD: uHPWPtr = GetADHalfWayPagesPtr(); uBegin = 0; uEnd = m_levelPointers.bWorldCount; break;
	}

	m_hpData.clear();

	for ( USHORT u = uBegin; u < uEnd; ++u )
	{
		std::vector<BYTE> hpWorldData;
		USHORT uhwData = m_file.Data<USHORT>( uHPWPtr + sizeof( USHORT ) * u );
		hpWorldData.push_back( ( LOBYTE( uhwData ) & 0xF0 ) >> 4 );
		hpWorldData.push_back( ( LOBYTE( uhwData ) & 0x0F ) );
		hpWorldData.push_back( ( HIBYTE( uhwData ) & 0xF0 ) >> 4 );
		hpWorldData.push_back( ( HIBYTE( uhwData ) & 0x0F ) );
		m_hpData.push_back( hpWorldData );
	}
}

VOID CNesLevelSet::DumpHalfwayPages()
{
	USHORT uBegin = AreaSet2World( 0 ), uEnd = uBegin + m_levelPointers.bWorldCount;
	USHORT uHPWPtr = 0, uw = 0;
	switch ( m_areaSet )
	{
		case asWorld1to4: uHPWPtr = m_eptr.Pointer( eHPAreaSet0 ); break;
		case asWorld5to8: uHPWPtr = m_eptr.Pointer( eHPAreaSet1 ); break;
		case asWorld9: uHPWPtr = m_eptr.Pointer( eHPAreaSet2 ); break;
		case asWorldAtoD: uHPWPtr = GetADHalfWayPagesPtr(); uBegin = 0; uEnd = m_levelPointers.bWorldCount; break;
	}

	for ( USHORT u = uBegin; u < uEnd; ++u, ++uw )
	{
		const std::vector<BYTE> & hpWorldData = m_hpData[ uw ];
		if ( hpWorldData.size() != 4 )
		{
			throw std::exception( "Levels count in the world must be equal to 4" );
		}

		BYTE bLoLevel = 0, bHiLevel = 0;
		bLoLevel = ( hpWorldData[ 0 ] << 4 ) | ( hpWorldData[ 1 ] & 0x0F );
		bHiLevel = ( hpWorldData[ 2 ] << 4 ) | ( hpWorldData[ 3 ] & 0x0F );
		m_file.Data<USHORT>( uHPWPtr + sizeof( USHORT ) * u ) = MAKEWORD( bLoLevel, bHiLevel );
	}
}

VOID CNesLevelSet::PushMeta( std::vector<NES_LEVEL_METADATA>& vSections, const NES_LEVEL_METADATA & meta )
{
	auto itMeta = std::find_if( vSections.begin(), vSections.end(),
		[ &meta ]( const NES_LEVEL_METADATA & lvlMeta )
	{
		return (
			lvlMeta.uDataFileId[ 0 ] == meta.uDataFileId[ 0 ] &&
			lvlMeta.uDataFileId[ 1 ] == meta.uDataFileId[ 1 ] &&
			lvlMeta.uPointerFileId[ 0 ] == meta.uPointerFileId[ 0 ] &&
			lvlMeta.uPointerFileId[ 1 ] == meta.uPointerFileId[ 1 ]
			);
	}
	);

	if ( itMeta == vSections.end() )
	{
		vSections.push_back( meta );
	}
}

VOID CNesLevelSet::GetAreasSections( std::vector<NES_LEVEL_METADATA>& vSections, BOOL fSharedSection )
{
	for ( const auto & itType : m_areas )
	{
		for ( auto pLevel : itType.second )
		{
			if ( !pLevel ) continue;

			if ( fSharedSection && pLevel->IsSharedLevel() )
			{
				PushMeta( vSections, pLevel->Metadata() );
			}
			else if ( !fSharedSection && !pLevel->IsSharedLevel() )
			{
				PushMeta( vSections, pLevel->Metadata() );
			}
		}
	}
}

VOID CNesLevelSet::AddArea( std::vector<CNesLevel*> vpLevels, CNesLevel * pLevel )
{
	if ( std::find( vpLevels.begin(), vpLevels.end(), pLevel ) != vpLevels.end() )
	{
		vpLevels.push_back( pLevel );
	}
}

VOID CNesLevelSet::GetLevelsList( CNesLevel * pLevel, std::vector<CNesLevel*>& vpLevel )
{
	std::vector<CNesLevel*> vpLink;
	if ( std::find( vpLevel.begin(), vpLevel.end(), pLevel ) != vpLevel.end() )
	{
		return;
	}

	vpLevel.push_back( pLevel );

	pLevel->GetLevelLinks( m_areaSet, vpLink );
	for ( auto pLink : vpLink )
	{
		GetLevelsList( pLink, vpLevel );
	}
}

CNesLevelSection & CNesLevelSet::GetSection()
{
	return m_holder.GetSection(
		m_areaSet,
		m_eptr.Pointer( eWorldAreasOffsets ),
		m_eptr.Pointer( eWorldAreas ),
		m_eptr.Pointer( eEnemyAddrHOffset ),
		m_eptr.Pointer( eLevelAddrHOffset ),
		m_eptr.Pointer( eEnemyPtr ),
		m_eptr.Pointer( eEnemyPtrHi ),
		m_eptr.Pointer( eLevelPtr ),
		m_eptr.Pointer( eLevelPtrHi )
	);
}

VOID CNesLevelSet::StoreAreas()
{
	CNesLevelSection & section = GetSection();
	BOOL fHiWorlds = ( asWorldAtoD == m_areaSet );
	// ^ its required for loop commands flag
	// to differentiate lo worlds (1-4) from hi (A-D)

	std::vector<CNesLevel*> vpLevels;

	for ( auto & itWorld : m_mWorld )
	{
		for ( auto & itLevel : itWorld )
		{
			for ( auto pLevel : itLevel )
			{
				GetLevelsList( pLevel, vpLevels );
			}
		}
	}

	for ( auto pLevel : vpLevels )
	{
		section.PushLevel( m_areaSet, pLevel, fHiWorlds );
	}

	// area pointers already validated,
	// so push it into the section instead read levels
	for ( size_t nWorld = 0; nWorld < m_levelPointers.bWorldCount; ++nWorld )
	{
		auto & itWorld = m_mWorld[ nWorld ];
		std::vector<NES_AREA_PTR> vpWorld;
		for ( auto & itLevel : itWorld )
		{
			for ( auto pLevel : itLevel )
			{
				vpWorld.push_back( pLevel->AreaPtr() );
			}
		}

		section.PushWorld( vpWorld );
	}

	section.DumpSection( m_areaSet );
}

VOID CNesLevelSet::DumpAreas()
{
	CNesLevelSection & section = GetSection();
	section.DumpData( m_areaSet );
}

VOID CNesLevelSet::SaveSet()
{
	LoadRequiredFiles();
	StoreAreas();
	MakeWorld5WarpPatch();
}

VOID CNesLevelSet::DumpSet()
{
	LoadRequiredFiles();
	DumpHalfwayPages();
	DumpAreas();
	m_objectData.DumpData();
	m_objectData.LoadData();	// load it again to fetch valid warp data
}

NES_LIMITS CNesLevelSet::ValidateLimits()
{
	LoadRequiredFiles();

	CNesLevelSection & section = GetSection();
	return section.ValidateLimits( m_areaSet, FALSE );
}

CNesLevel * CNesLevelSet::GetLevel( BYTE bWorld, BYTE bLevel, BYTE bAreaId )
{
	if ( bWorld >= m_levelPointers.bWorldCount || bLevel >= NES_LEVEL_COUNT )
	{
		return nullptr;
	}

	if ( bAreaId >= m_mWorld[ bWorld ][ bLevel ].size() )
	{
		return nullptr;
	}

	return m_mWorld[ bWorld ][ bLevel ][ bAreaId ];
}

VOID CNesLevelSet::GetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages )
{
	for ( size_t w = 0; w < m_levelPointers.bWorldCount; ++w )
	{
		nAreaList.push_back( m_mWorld[ w ] );
	}

	nHalfwayPages.insert( nHalfwayPages.end(), m_hpData.begin(), m_hpData.end() );
}

BOOL CNesLevelSet::SetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages )
{
	if ( nAreaList.size() != m_levelPointers.bWorldCount || nAreaList.size() != nHalfwayPages.size() )
	{
		return FALSE;
	}

	for ( size_t n = 0; n < m_levelPointers.bWorldCount; ++n )
	{
		if ( nAreaList[ n ].size() != nHalfwayPages[ n ].size() || nAreaList[ n ].size() != 4 )
		{
			return FALSE;
		}

		for ( const auto & vLevel : nAreaList[ n ] )
		{
			if ( vLevel.size() < 1 || vLevel.size() > 2 ) return FALSE;
		}

		m_mWorld[ n ] = nAreaList[ n ];
		m_hpData[ n ] = nHalfwayPages[ n ];
	}

	return TRUE;
}

CNesLevelsKeeper * CNesLevelSet::GetAreaList( CNesMetatiles & mTiles, CNesGameEngineHack & hack )
{
	NES_AREA_GROUPS vAreas = m_areas;
	m_holder.AppendSharedAreas( vAreas );
	return new CNesLevelsKeeper( m_links, hack, vAreas, mTiles );
}

CNesObjectData & CNesLevelSet::ObjData()
{
	return m_objectData;
}

VOID CNesLevelSet::FreeStaleLinks()
{
	m_links.FreeStaleLinks();
}

CNesLevelSet::operator NES_AREA_SET()
{
	return m_areaSet;
}

CNesLevel * CNesLevelSet::CreateArea( NES_LEVEL_TYPE type, BOOL fSharedSection )
{
	std::vector<NES_LEVEL_METADATA> vMeta;
	GetAreasSections( vMeta, fSharedSection );

	if ( !vMeta.size() ) return nullptr;

#ifdef _DEBUG
	if ( vMeta.size() > 1 )
	{
		_tprintf( TEXT( "WARNING! Multiple sections was found for new area at section %d\n" ), m_areaSet );
	}
#endif

	CNesLevel * pLevel = m_holder.CreateArea( vMeta[ 0 ], m_objectData, m_links, type );
	if ( fSharedSection )
	{
		pLevel->MarkLevelAsShared();
	}

	m_areas[ pLevel->AreaType() ].push_back( pLevel );

	return pLevel;
}

BOOL CNesLevelSet::IsPageReferenced( CNesLevel * pLevel, UINT uPage )
{
	return m_links.IsPageReferenced( pLevel, uPage );
}

BOOL CNesLevelSet::GetSourceList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>>& vLink )
{
	return m_links.GetSourceList( pLevel, uPage, vLink );
}

BOOL CNesLevelSet::SetSourcePage( CNesLevel * pLevel, UINT uPage, UINT uNewPage )
{
	return m_links.SetSourcePage( pLevel, uPage, uNewPage );
}