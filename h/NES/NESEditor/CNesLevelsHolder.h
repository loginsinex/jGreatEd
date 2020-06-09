#pragma once

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

typedef std::map<NES_AREA_SET, std::map<NES_LEVEL_TYPE, std::vector<CNesLevel*>>> NES_AREA_SET_ARRAY, *PNES_AREA_SET_ARRAY;

typedef struct _tagNES_LIMITS
{
	size_t nMaximumDataSize;
	size_t nMaxAreas;
	size_t nMaxAreasSlots;
	size_t nMaximumSharedSection;
	size_t uDataSize;
	size_t uAreas;
	size_t nAreasSlots;
	size_t nSharedSection;
} NES_LIMITS, *PNES_LIMITS;

typedef struct _tagNES_AREA_HOLD_DATA
{
	BYTE					uFileId[ 2 ];
	BYTE					uPointerFileId[ 2 ];
	USHORT					uAreaPtr[ 2 ];
	NES_LEVEL_TYPE			uType;
	CNesLevel *				pLevel;
	std::vector<NES_LINK>	vLinks;
} NES_AREA_HOLD_DATA, *PNES_AREA_HOLD_DATA;

typedef struct _tagNES_AREA_SECTION
{
	NES_AREA_SET			as;
	USHORT					uDataPtr[ 2 ];
	std::vector<BYTE>		vData[ 2 ];
} NES_AREA_SECTION, *PNES_AREA_SECTION;

typedef struct _tagNES_SECTION_PTR
{
	NES_AREA_SET		as;
	USHORT				ptr;
} NES_SECTION_PTR, *PNES_SECTION_PTR;

class CNesLevelsHolder;

class CNesLevelSection
{
	CNESFile			& m_file;
	CNesLevelsHolder	& m_holder;
	USHORT				& m_uWorldOffsetsPtr;
	USHORT				& m_uWorldsPtr;
	USHORT				& m_uEnemyOffsetsPtr;
	USHORT				& m_uAreaOffsetsPtr;
	USHORT				& m_uEnemyPtr;
	USHORT				& m_uEnemyPtrHi;
	USHORT				& m_uAreaPtr;
	USHORT				& m_uAreaPtrHi;

	const std::map<BYTE, USHORT>								& m_mSection;
	NES_AREA_SET_ARRAY											m_mLevels;
	std::map<NES_LEVEL_TYPE, std::vector<NES_SECTION_PTR>>		m_mDataPtr[ 2 ];
	std::vector<std::vector<NES_AREA_PTR>>						m_vWorld;
	std::map<NES_AREA_SET, NES_AREA_SECTION>					m_localSection;
	NES_AREA_SECTION	&										m_sharedSection;

	NES_SECTION_PTR		DumpData( NES_AREA_SECTION & section, const std::vector<BYTE> & vData, BYTE bTypeOfData );
	VOID				DumpLevels( NES_AREA_SET as );
	VOID				ValidateSectionPointers( NES_AREA_SECTION & section );
	VOID				ValidatePointers( NES_AREA_SET as );
	VOID				SelectSectionPointers( NES_AREA_SET as, BYTE uFileId );
	BOOL				IsLevelPushed( CNesLevel * pLevel );
	BYTE				GetNextAreaId( NES_LEVEL_TYPE nlt );

public:
	CNesLevelSection(
		CNESFile			& m_file,
		CNesLevelsHolder	& m_holder,
		USHORT				& uWorldOffsetsPtr,
		USHORT				& uWorldsPtr,
		USHORT				& uEnemyOffsetsPtr,
		USHORT				& uAreaOffsetsPtr,
		USHORT				& uEnemyPtr,
		USHORT				& uEnemyPtrHi,
		USHORT				& uAreaPtr,
		USHORT				& uAreaPtrHi,
		NES_AREA_SECTION	& sharedSection,
		const std::map<BYTE, USHORT>	& mSection
	);

	VOID					PushLevel( NES_AREA_SET as, CNesLevel * pLevel, BOOL fHiWorlds );
	VOID					PushWorld( const std::vector<NES_AREA_PTR> & vpLevels );
	VOID					DumpSection( NES_AREA_SET as );
	VOID					DumpData( NES_AREA_SET as );
	NES_LIMITS				ValidateLimits( NES_AREA_SET as, BOOL fThrowException );

};

class CNesLevelsHolder
{
	std::vector<NES_AREA_HOLD_DATA>			m_vArea;
	size_t									m_uNextAreaId;
	const BOOL								m_fPatchMode;
	CNesLoop								& m_loop;
	CNESFile								& m_file;
	CNesPointers							& m_eptr;
	CNesMetatiles							& m_metaTiles;
	const CNesPlayerEntrance				& m_player;
	std::map<BYTE, CNesLevelSection*>		m_lvlSection;

	struct
	{
		std::map<BYTE, USHORT>				mSection;

		struct
		{
			BYTE								uFileId;
			USHORT								uPtr;
		} mSharedSection;

		NES_AREA_SECTION					nasDumper;
	} m_sections;

	VOID						ReadROMAreaData( std::vector<BYTE> & vAreaData, USHORT uPtr );
	VOID						ReadROMEnemyData( std::vector<BYTE> & vEnemyData, USHORT uPtr );
	VOID						AddSectionRef( USHORT uDataPtrPtr, CNesLevel * pLevel );
	VOID						AddSectionRef( USHORT uDataPtr, BYTE uFileId );
	BOOL						IsPointerInSharedSection( USHORT uPtr );
	VOID						ClearSections();
	VOID						InitSharedSection();
	size_t						GetNextAreaId();

public:
	CNesLevelsHolder( CNESFile & file, CNesPointers & eptr, CNesMetatiles & metaTiles, const CNesPlayerEntrance & player, CNesLoop & loop, BOOL fPatchMode );
	~CNesLevelsHolder();

	VOID					AppendSharedAreas( NES_AREA_GROUPS & vAreas );
	CNesLevel *				PushLevel( USHORT uEnemyPtrPtr, USHORT uAreaPtrPtr, NES_LEVEL_TYPE nlt, CNesGameLinks & links, CNesObjectData & objData, std::vector<NES_LINK> & vResultLinks );
	VOID					FinalizeLoader();
	VOID					InitDumper();
	VOID					FinalizeDumper();
	VOID					MarkLevelAsShared( USHORT uDataPtrPtr, CNesLevel * pLevel );

	CNesLevelSection &		GetSection( NES_AREA_SET		areaSet,
										USHORT				& uWorldOffsetsPtr,
										USHORT				& uWorldsPtr,
										USHORT				& uEnemyOffsetsPtr,
										USHORT				& uAreaOffsetsPtr,
										USHORT				& uEnemyPtr,
										USHORT				& uEnemyPtrHi,
										USHORT				& uAreaPtr,
										USHORT				& uAreaPtrHi );

	CNESFile &				File();
	CNesPointers &			Pointers();
	BOOL					IsPatchMode();
	CNesLoop &				Loop();
	CNesLevel *				CreateArea( const NES_LEVEL_METADATA & baseMeta, CNesObjectData & objData, CNesGameLinks & links, NES_LEVEL_TYPE type );
};