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

CNesLevelSection::CNesLevelSection(
	CNESFile & file,
	CNesLevelsHolder & holder,
	USHORT & uWorldOffsetsPtr,
	USHORT & uWorldsPtr,
	USHORT & uEnemyOffsetsPtr,
	USHORT & uAreaOffsetsPtr,
	USHORT & uEnemyPtr,
	USHORT & uEnemyPtrHi,
	USHORT & uAreaPtr,
	USHORT & uAreaPtrHi,
	NES_AREA_SECTION & sharedSection,
	const std::map<BYTE, USHORT> & mSection
)
	:
	m_file( file ),
	m_holder( holder ),
	m_uWorldOffsetsPtr( uWorldOffsetsPtr ),
	m_uWorldsPtr( uWorldsPtr ),
	m_uEnemyOffsetsPtr( uEnemyOffsetsPtr ),
	m_uAreaOffsetsPtr( uAreaOffsetsPtr ),
	m_uEnemyPtr( uEnemyPtr ),
	m_uEnemyPtrHi( uEnemyPtrHi ),
	m_uAreaPtr( uAreaPtr ),
	m_uAreaPtrHi( uAreaPtrHi ),
	m_sharedSection( sharedSection ),
	m_mSection( mSection )
{

}

BOOL CNesLevelSection::IsLevelPushed( CNesLevel * pLevel )
{
	for ( const auto & itSet : m_mLevels )
	{
		for ( const auto & itType : itSet.second )
		{
			const auto & vLevel = itType.second;

			if ( std::find( vLevel.begin(), vLevel.end(), pLevel ) != vLevel.end() )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BYTE CNesLevelSection::GetNextAreaId( NES_LEVEL_TYPE nlt )
{
	BYTE bAreaId = 0;
	for ( const auto & itSet : m_mLevels )
	{
		auto itLevels = itSet.second.find( nlt );
		if ( itLevels != itSet.second.end() )
		{
			bAreaId += LOBYTE( itLevels->second.size() );
		}
	}

	return bAreaId;
}

VOID CNesLevelSection::PushLevel( NES_AREA_SET as, CNesLevel * pLevel, BOOL fHiWorlds )
{
	NES_AREA_PTR aptr = { 0 };

	if ( IsLevelPushed( pLevel ) )
	{
		return;
	}

	aptr.bType = pLevel->AreaType();
	aptr.bUnused = ( fHiWorlds ? 1 : 0 );
	BYTE bNextAreaId = GetNextAreaId( aptr.bType );
	if ( bNextAreaId >= 0x20 )
	{
		char strErr[ 256 ];
		sprintf_s( strErr, "Too many areas for type %d for section %d.", aptr.bType, as );

		throw std::exception( strErr );
	}

	aptr.bAreaId = bNextAreaId;
	pLevel->SetAreaId( aptr.bAreaId );
	pLevel->SetOptionalAreaId( as, aptr.bAreaId );
	m_mLevels[ as ][ aptr.bType ].push_back( pLevel );
}

VOID CNesLevelSection::PushWorld( const std::vector<NES_AREA_PTR>& vpLevels )
{
	// std::vector<NES_AREA_PTR> vptr;

	/*
	for ( auto pLevel : vpLevels )
	{
		vptr.push_back( pLevel->AreaPtr() );
	}
	

	m_vWorld.push_back( vptr );
	*/

	m_vWorld.push_back( vpLevels );
}

VOID CNesLevelSection::SelectSectionPointers( NES_AREA_SET as, BYTE uFileId )
{
	auto itSection = m_mSection.find( uFileId );
	if ( itSection != m_mSection.end() )
	{
		m_localSection[ as ].as = as;
		m_localSection[ as ].uDataPtr[ 0 ] = itSection->second;
	}
}

VOID CNesLevelSection::DumpLevels( NES_AREA_SET as )
{
	for ( const auto & itType : m_mLevels[ as ] )
	{
		const NES_LEVEL_TYPE & type = itType.first;
		for ( auto pLevel : itType.second )
		{
			std::vector<BYTE> vEnemy, vArea;
			std::vector<NES_LOOP> vLoops;
			auto & enemy = m_mDataPtr[ 0 ][ type ];
			auto & area = m_mDataPtr[ 1 ][ type ];
			auto & section = ( pLevel->IsSharedLevel() ? m_sharedSection : m_localSection[ as ] );
			if ( !pLevel->IsSharedLevel() )
			{
				SelectSectionPointers( as, pLevel->Metadata().uDataFileId[ 0 ] );
			}

			pLevel->GetLevelBinaryData( vArea, vEnemy );			

			enemy.push_back( DumpData( section, vEnemy, 0 ) );
			area.push_back( DumpData( section, vArea, 1 ) );
			
			if ( pLevel->IsObjectPresentOnPageRange( 0, pLevel->PagesCount(), 0x4B, NES_SPECIAL_OBJECT_D ) )
			{
				pLevel->DumpLoops( vLoops );
				if ( vLoops.size() > 0 )
				{
					m_holder.Loop().SetLoops( vLoops );
				}
			}
		}
	}
}

VOID CNesLevelSection::ValidateSectionPointers( NES_AREA_SECTION & section )
{
	section.uDataPtr[ 1 ] = section.uDataPtr[ 0 ] + LOWORD( section.vData[ 0 ].size() );
}

VOID CNesLevelSection::ValidatePointers( NES_AREA_SET as )
{
	ValidateSectionPointers( m_sharedSection );
	ValidateSectionPointers( m_localSection[ as ] );
}

VOID CNesLevelSection::DumpSection( NES_AREA_SET as )
{
	DumpLevels( as );
	ValidatePointers( as );
}

NES_LIMITS CNesLevelSection::ValidateLimits( NES_AREA_SET as, BOOL fThrowException )
{
	NES_LIMITS limits = { 0 };
	size_t & nAreas = limits.uAreas;
	size_t & nAreasSlots = limits.nAreasSlots;
	size_t & nMaximumDataSize = limits.nMaximumDataSize;
	size_t & nMaxAreas = limits.nMaxAreas;
	size_t & nMaxAreasSlots = limits.nMaxAreasSlots;
	size_t & uRealSize = limits.uDataSize;
	size_t & uRealSharedSize = limits.nSharedSection;

	limits.nMaximumSharedSection = NES_J_SIZE_OF_SECTION_SHARED;

	switch ( as )
	{
		case asWorld1to4: nMaximumDataSize = NES_J_SIZE_OF_SECTION_0; nMaxAreas = NES_J_SIZE_OF_WORLDS_0; nMaxAreasSlots = NES_J_AREAS_COUNT_0; break;
		case asWorld5to8: nMaximumDataSize = NES_J_SIZE_OF_SECTION_1; nMaxAreas = NES_J_SIZE_OF_WORLDS_1; nMaxAreasSlots = NES_J_AREAS_COUNT_1; break;
		case asWorld9: nMaximumDataSize = NES_J_SIZE_OF_SECTION_2; nMaxAreas = NES_J_SIZE_OF_WORLDS_2; nMaxAreasSlots = NES_J_AREAS_COUNT_2; break;
		case asWorldAtoD: nMaximumDataSize = NES_J_SIZE_OF_SECTION_3; nMaxAreas = NES_J_SIZE_OF_WORLDS_3; nMaxAreasSlots = NES_J_AREAS_COUNT_3; break;
	}

	// perform some checks
	for ( const auto & itWorld : m_vWorld )
	{
		nAreas += itWorld.size();
	}

	if ( nAreas > nMaxAreas && fThrowException )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of slots for worlds for section %d.\nRequired %Iu slots, but available %Iu.", as, nAreas, nMaxAreas );
		throw std::exception( strError );
	}

	// check pointers amount
	for ( auto & itType : m_mDataPtr[ 0 ] )
	{
		nAreasSlots += itType.second.size();
	}

	if ( nAreasSlots > nMaxAreasSlots && fThrowException )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of areas for section %d.\nRequired %Iu slots, but available %Iu.", as, nAreasSlots, nMaxAreasSlots );
	}

	// get the maximum size of data
	uRealSize = m_localSection[ as ].vData[ 0 ].size() + m_localSection[ as ].vData[ 1 ].size();

	if ( uRealSize > nMaximumDataSize && fThrowException )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of area data size for section %d.\nRequested %Iu bytes, but available %Iu bytes.", as, uRealSize, nMaximumDataSize );
		throw std::exception( strError );
	}

	// check for shared section
	uRealSharedSize = m_sharedSection.vData[ 0 ].size() + m_sharedSection.vData[ 1 ].size();
	if ( uRealSharedSize > NES_J_SIZE_OF_SECTION_SHARED && fThrowException )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of area data size for shared section.\nRequested %Iu bytes, but available %u bytes.", uRealSharedSize, NES_J_SIZE_OF_SECTION_SHARED );
		throw std::exception( strError );
	}

	return limits;
}

VOID CNesLevelSection::DumpData( NES_AREA_SET as )
{
	// init offsets (for areas the same as for enemies)
	std::vector<BYTE> vWorldOffsets;
	BYTE bOffsets[ nltTypeCount ] = { 0 };
	BYTE bOffset = 0;
	size_t nWorlds = m_vWorld.size();
	USHORT uWorldsPtr = m_uWorldsPtr;
	USHORT uEnemiesPtr = m_uEnemyPtr;
	USHORT uAreasOffsetsPtr = m_uAreaOffsetsPtr;
	USHORT uAreasPtr = m_uAreaPtr;

	ValidateLimits( as, TRUE );

	// worlds offsets
	for ( size_t n = 0; n < nWorlds; ++n )
	{
		vWorldOffsets.push_back( bOffset );
		bOffset += LOBYTE( m_vWorld[ n ].size() );
	}

	// data offsets
	bOffset = 0;
	for ( BYTE uType = 0; uType < nltTypeCount; ++uType )
	{
		bOffsets[ uType ] = bOffset;
		bOffset += LOBYTE( m_mDataPtr[ 0 ][ static_cast<NES_LEVEL_TYPE>( uType ) ].size() );
	}

	// store the worlds offsets
	for ( USHORT n = 0; n < LOWORD( nWorlds ); ++n )
	{
		m_file.Data<BYTE>( m_uWorldOffsetsPtr + n ) = vWorldOffsets[ n ];
	}

	for ( const auto & itWorld : m_vWorld )
	{
		for ( const auto & itArea : itWorld )
		{
			m_file.Data<BYTE>( uWorldsPtr++ ) = itArea.bPtr;
		}
	}

	// write enemies offset
	CopyMemory( m_file.Data<BYTE[ nltTypeCount ]>( m_uEnemyOffsetsPtr ), bOffsets, nltTypeCount );

	// write pointers to enemies
	for ( auto & itType : m_mDataPtr[ 0 ] )
	{
		for ( auto & ptr : itType.second )
		{
			const NES_AREA_SECTION & section = ( asWorldSetsCount == ptr.as ? m_sharedSection : m_localSection[ ptr.as ] );
			USHORT myptr = section.uDataPtr[ 0 ] + ptr.ptr;

			m_file.Data<USHORT>( uEnemiesPtr ) = myptr;
			uEnemiesPtr += sizeof( USHORT );
		}
	}

	// write areas offsets
	//  we've new areas offsets ptr and areas ptr
	m_uAreaOffsetsPtr = uEnemiesPtr;
	uAreasOffsetsPtr = uEnemiesPtr;
	CopyMemory( m_file.Data<BYTE[ nltTypeCount ]>( uAreasOffsetsPtr ), bOffsets, nltTypeCount );
	m_uAreaPtr = m_uAreaOffsetsPtr + nltTypeCount;
	m_uAreaPtrHi = m_uAreaPtr + 1;
	uAreasPtr = m_uAreaPtr;

	// write pointers to areas
	for ( auto & itType : m_mDataPtr[ 1 ] )
	{
		for ( auto & ptr : itType.second )
		{
			const NES_AREA_SECTION & section = ( asWorldSetsCount == ptr.as ? m_sharedSection : m_localSection[ ptr.as ] );
			USHORT myptr = section.uDataPtr[ 1 ] + ptr.ptr;

			m_file.Data<USHORT>( uAreasPtr ) = myptr;
			uAreasPtr += sizeof( USHORT );
		}
	}

	// dump enemies and areas data

	// write area data
	for ( BYTE bTypeOfData = 0; bTypeOfData < 2; ++bTypeOfData )
	{
		USHORT uDataPtr = m_localSection[ as ].uDataPtr[ bTypeOfData ];

		for ( const auto & b : m_localSection[ as ].vData[ bTypeOfData ] )
		{
			m_file.Data<BYTE>( uDataPtr++ ) = b;
		}
	}

	// write shared section area data
	for ( BYTE bTypeOfData = 0; bTypeOfData < 2; ++bTypeOfData )
	{
		USHORT uDataPtr = m_sharedSection.uDataPtr[ bTypeOfData ];
		for ( const auto & b : m_sharedSection.vData[ bTypeOfData ] )
		{
			m_file.Data<BYTE>( uDataPtr++ ) = b;
		}
	}
}

NES_SECTION_PTR CNesLevelSection::DumpData( NES_AREA_SECTION & section, const std::vector<BYTE>& vData, BYTE bTypeOfData )
{
	// checkout shared section
	auto & vWhere = section.vData[ bTypeOfData ];
	const auto & vWhat = vData;
	auto itPtr = std::search( vWhere.begin(), vWhere.end(), vWhat.begin(), vWhat.end() );
	NES_SECTION_PTR uResultPtr = { section.as,  LOWORD( vWhere.size() ) };

	if ( itPtr != vWhere.end() )
	{		
		uResultPtr.ptr = LOWORD( itPtr - vWhere.begin() );
	}
	else
	{
		vWhere.insert( vWhere.end(), vWhat.begin(), vWhat.end() );
	}

	return uResultPtr;
}

// ---- holder

CNesLevelsHolder::CNesLevelsHolder( CNESFile & file, CNesPointers & eptr, CNesMetatiles & metaTiles, const CNesPlayerEntrance & player, CNesLoop & loop, BOOL fPatchMode )
	: m_file( file ), m_eptr( eptr ), m_metaTiles( metaTiles ), m_player( player ), m_loop( loop ), m_fPatchMode( fPatchMode ), m_uNextAreaId( 0 )
{
	m_sections.mSharedSection.uFileId = 0xFF;
	m_sections.mSharedSection.uPtr = 0xFFFF;
}

CNesLevelsHolder::~CNesLevelsHolder()
{
	for ( auto & nhd : m_vArea )
	{
		delete nhd.pLevel;
		nhd.pLevel = nullptr;
	}

	m_vArea.clear();
}

VOID CNesLevelsHolder::ClearSections()
{
	for ( auto & itSection : m_lvlSection )
	{
		delete itSection.second;
	}

	m_lvlSection.clear();
}

VOID CNesLevelsHolder::AppendSharedAreas( NES_AREA_GROUPS & vAreas )
{
	for ( const auto & hld : m_vArea )
	{
		CNesLevel * pLevel = hld.pLevel;
		if ( pLevel->IsSharedLevel() )
		{
			auto & vLocalAreas = vAreas[ pLevel->AreaType() ];
			if ( vLocalAreas.end() == std::find( vLocalAreas.begin(), vLocalAreas.end(), pLevel ) )
			{
				vLocalAreas.push_back( pLevel );
			}
		}
	}
}

CNesLevel * CNesLevelsHolder::PushLevel( USHORT uEnemyPtrPtr, USHORT uAreaPtrPtr, NES_LEVEL_TYPE nlt, CNesGameLinks & links, CNesObjectData & objData, std::vector<NES_LINK> & vResultLinks )
{
	NES_AREA_HOLD_DATA nhd;
	USHORT uEnemyPtr = m_file.Data<USHORT>( uEnemyPtrPtr );
	USHORT uAreaPtr = m_file.Data<USHORT>( uAreaPtrPtr );
	nhd.uPointerFileId[ 0 ] = m_file.GetFileIdFromPtr( uEnemyPtrPtr );
	nhd.uPointerFileId[ 1 ] = m_file.GetFileIdFromPtr( uAreaPtrPtr );
	nhd.uAreaPtr[ 0 ] = uEnemyPtr;
	nhd.uAreaPtr[ 1 ] = uAreaPtr;
	nhd.uFileId[ 0 ] = m_file.GetFileIdFromPtr( uEnemyPtr );
	nhd.uFileId[ 1 ] = m_file.GetFileIdFromPtr( uAreaPtr );
	nhd.uType = nlt;

	auto itLevel = std::find_if( m_vArea.begin(), m_vArea.end(), 
								 [ &nhd ]( const NES_AREA_HOLD_DATA & itnhd )
								 {
									 if ( itnhd.uAreaPtr[ 0 ] == nhd.uAreaPtr[ 0 ] &&
										  itnhd.uAreaPtr[ 1 ] == nhd.uAreaPtr[ 1 ] &&
										  itnhd.uFileId[ 0 ] == nhd.uFileId[ 0 ] &&
										  itnhd.uFileId[ 1 ] == nhd.uFileId[ 1 ] && 
										  itnhd.uType == nhd.uType )
									 {
										 return TRUE;
									 }
									 return FALSE;
								 }
	);

	if ( itLevel != m_vArea.end() )
	{
		const NES_AREA_HOLD_DATA & foundNHD = *itLevel;
		vResultLinks = foundNHD.vLinks;
		AddSectionRef( uEnemyPtrPtr, foundNHD.pLevel );
		AddSectionRef( uAreaPtrPtr, foundNHD.pLevel );
		return foundNHD.pLevel;
	}
	else
	{
		NES_LEVEL_METADATA md;
		NES_AREA_PTR ptr = { 0 };
		std::vector<BYTE> vEnemy, vData;
		ReadROMAreaData( vData, uAreaPtr );
		ReadROMEnemyData( vEnemy, uEnemyPtr );
		ptr.bAreaId = 0;
		ptr.bType = nlt;
		md.uPointerFileId[ 0 ] = nhd.uPointerFileId[ 0 ];
		md.uPointerFileId[ 1 ] = nhd.uPointerFileId[ 1 ];
		md.uDataFileId[ 0 ] = nhd.uFileId[ 0 ];
		md.uDataFileId[ 1 ] = nhd.uFileId[ 1 ];
		md.uLevelId = GetNextAreaId();

		nhd.pLevel = new CNesLevel( m_file, ptr, m_eptr, m_metaTiles, objData, links, m_player );
		nhd.pLevel->Metadata( md );
		nhd.pLevel->LoadLevelData( vData, vEnemy, nhd.vLinks );
		vResultLinks = nhd.vLinks;
		m_vArea.push_back( nhd );
		AddSectionRef( uEnemyPtrPtr, nhd.pLevel );
		AddSectionRef( uAreaPtrPtr, nhd.pLevel );
		return nhd.pLevel;
	}

	return nullptr;
}

CNesLevelSection & CNesLevelsHolder::GetSection( 
	NES_AREA_SET areaSet, 
	USHORT & uWorldOffsetsPtr, 
	USHORT & uWorldsPtr, 
	USHORT & uEnemyOffsetsPtr, 
	USHORT & uAreaOffsetsPtr, 
	USHORT & uEnemyPtr, 
	USHORT & uEnemyPtrHi, 
	USHORT & uAreaPtr,
	USHORT & uAreaPtrHi )
{
	BYTE uFileId = m_file.GetFileIdFromPtr( uWorldOffsetsPtr );

	if ( m_lvlSection.find( uFileId ) != m_lvlSection.end() )
	{
		CNesLevelSection & section = *m_lvlSection[ uFileId ];
		return section;
	}

	CNesLevelSection * pSection = new CNesLevelSection( 
		m_file,
		*this,
		uWorldOffsetsPtr, 
		uWorldsPtr, 
		uEnemyOffsetsPtr, 
		uAreaOffsetsPtr, 
		uEnemyPtr, 
		uEnemyPtrHi, 
		uAreaPtr, 
		uAreaPtrHi, 
		m_sections.nasDumper, m_sections.mSection 
	);

	m_lvlSection[ uFileId ] = pSection;
	return *pSection;
}

VOID CNesLevelsHolder::InitSharedSection()
{
	m_sections.nasDumper.as = asWorldSetsCount;
	m_sections.nasDumper.vData[ 0 ].clear();
	m_sections.nasDumper.vData[ 1 ].clear();
	m_sections.nasDumper.uDataPtr[ 0 ] = m_sections.mSharedSection.uPtr;
	m_sections.nasDumper.uDataPtr[ 1 ] = 0;
}

size_t CNesLevelsHolder::GetNextAreaId()
{
	return m_uNextAreaId++;
}

VOID CNesLevelsHolder::FinalizeLoader()
{
	for ( const auto & nhd : m_vArea )
	{
		AddSectionRef( nhd.uAreaPtr[ 0 ], nhd.uFileId[ 0 ] );
		AddSectionRef( nhd.uAreaPtr[ 1 ], nhd.uFileId[ 1 ] );
	}
}

VOID CNesLevelsHolder::InitDumper()
{
	InitSharedSection();
	ClearSections();
}

VOID CNesLevelsHolder::FinalizeDumper()
{
	InitSharedSection();
	ClearSections();
}

VOID CNesLevelsHolder::ReadROMAreaData( std::vector<BYTE>& vAreaData, USHORT uPtr )
{
	if ( NES_AREA_EOF == m_file.Data<BYTE>( uPtr ) )
	{
		return;
	}

	// store the header
	vAreaData.push_back( m_file.Data<BYTE>( uPtr++ ) );
	vAreaData.push_back( m_file.Data<BYTE>( uPtr++ ) );

	// read area data
	while ( NES_AREA_EOF != m_file.Data<BYTE>( uPtr ) )
	{
		vAreaData.push_back( m_file.Data<BYTE>( uPtr++ ) );
		vAreaData.push_back( m_file.Data<BYTE>( uPtr++ ) );
	}
}

VOID CNesLevelsHolder::ReadROMEnemyData( std::vector<BYTE>& vEnemyData, USHORT uPtr )
{
	BYTE bData = 0x0;

	while ( NES_ENEMY_EOF != m_file.Data<BYTE>( uPtr ) )
	{
		bData = m_file.Data<BYTE>( uPtr++ );
		vEnemyData.push_back( bData );
		if ( NES_SPECIAL_OBJECT_E == ( bData & 0x0F ) )
		{
			vEnemyData.push_back( m_file.Data<BYTE>( uPtr++ ) );
		}

		vEnemyData.push_back( m_file.Data<BYTE>( uPtr++ ) );
	}
}

VOID CNesLevelsHolder::MarkLevelAsShared( USHORT uDataPtrPtr, CNesLevel * pLevel )
{
	pLevel->MarkLevelAsShared();
	AddSectionRef( uDataPtrPtr, pLevel );
}

VOID CNesLevelsHolder::AddSectionRef( USHORT uDataPtrPtr, CNesLevel * pLevel )
{
	pLevel->AddBankRef( m_file.GetFileIdFromPtr( uDataPtrPtr ) );
	if ( pLevel->IsSharedLevel() )
	{
		USHORT uDataPtr = m_file.Data<USHORT>( uDataPtrPtr );
		BYTE uFileId = m_file.GetFileIdFromPtr( uDataPtr );

		m_sections.mSharedSection.uFileId = uFileId;
		m_sections.mSharedSection.uPtr = min( m_sections.mSharedSection.uPtr, uDataPtr );
	}
}

VOID CNesLevelsHolder::AddSectionRef( USHORT uDataPtr, BYTE uFileId )
{
	if ( !IsPointerInSharedSection( uDataPtr ) )
	{
		auto itSection = m_sections.mSection.find( uFileId );
		if ( itSection == m_sections.mSection.end() )
		{
			m_sections.mSection[ uFileId ] = 0xFFFF;
			itSection = m_sections.mSection.find( uFileId );
		}

		auto & uSectionPtr = itSection->second;
		uSectionPtr = min( uSectionPtr, uDataPtr );
	}
}

BOOL CNesLevelsHolder::IsPointerInSharedSection( USHORT uPtr )
{
	return ( uPtr >= m_sections.mSharedSection.uPtr && uPtr < m_sections.mSharedSection.uPtr + NES_J_SIZE_OF_SECTION_SHARED );
}

CNESFile & CNesLevelsHolder::File()
{
	return m_file;
}

CNesPointers & CNesLevelsHolder::Pointers()
{
	return m_eptr;
}

BOOL CNesLevelsHolder::IsPatchMode()
{
	return m_fPatchMode;
}

CNesLoop & CNesLevelsHolder::Loop()
{
	return m_loop;
}

CNesLevel * CNesLevelsHolder::CreateArea( const NES_LEVEL_METADATA & baseMeta, CNesObjectData & objData, CNesGameLinks & links, NES_LEVEL_TYPE type )
{
	NES_LEVEL_METADATA md = baseMeta;
	NES_AREA_HOLD_DATA nhd = { 0 };
	NES_AREA_PTR ptr = { 0 };
	nhd.uFileId[ 0 ] = md.uDataFileId[ 0 ];
	nhd.uFileId[ 1 ] = md.uDataFileId[ 1 ];
	nhd.uPointerFileId[ 0 ] = md.uPointerFileId[ 0 ];
	nhd.uPointerFileId[ 1 ] = md.uPointerFileId[ 1 ];
	nhd.uType = type;
	
	md.uLevelId = GetNextAreaId();
	ptr.bType = type;

	nhd.pLevel = new CNesLevel( m_file, ptr, m_eptr, m_metaTiles, objData, links, m_player );
	nhd.pLevel->Metadata( md );
	m_vArea.push_back( nhd );

	return nhd.pLevel;
}