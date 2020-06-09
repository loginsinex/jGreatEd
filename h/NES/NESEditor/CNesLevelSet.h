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

#define NES_WORLDS_COUNT			4
#define NES_LEVEL_COUNT				4
#define NES_SET1_OFFSET				0
#define NES_SET2_OFFSET				4
#define NES_SET3_OFFSET				8
#define NES_SET4_OFFSET				0

#define NES_AREA_EOF				0xFD
#define NES_ENEMY_EOF				0xFF

typedef std::map<NES_AREA_PTR, std::vector<NES_LINK>> NES_LINK_ARRAY, *PNES_LINK_ARRAY;

class CNesLevelSet
{
	CNESFile					& m_file;
	CNesPointers				& m_eptr;
	CNesObjectData				m_objectData;
	CNesLevelsHolder			& m_holder;
	CNesGameLinks				m_links;
	const NES_AREA_SET			m_areaSet;
	NES_AREA_GROUPS				m_areas;
	NES_GAME_AREA_LIST			m_mWorld;
	std::vector<BYTE>			m_vFiles;
	NES_GAME_AREA_HP_LIST		m_hpData;


	struct
	{
		USHORT					uAreaPtr[ nltTypeCount ];
		USHORT					uEnemyPtr[ nltTypeCount ];
		USHORT					uWorldPtr[ NES_WORLDS_COUNT ];
		USHORT					uWorldsPtr;
		BYTE					bWorldCount;
		BYTE					bWorldOffset;
		NES_LINK_ARRAY			mLinks;
	} m_levelPointers;

	VOID						ParseLinks( NES_LINK_ARRAY & mLink );
	CNesLevel*					LevelFromPtr( NES_AREA_PTR areaPtr );
	CNesLevelSection &			GetSection();

	VOID						GetLevelsList( CNesLevel * pLevel, std::vector<CNesLevel*> & vpLevel );

	VOID						InitArrays();
	VOID						LoadRequiredFiles();
	VOID						LoadPointers();
	VOID						LoadAreas();
	VOID						AddArea( std::vector<CNesLevel*> vpLevels, CNesLevel * pLevel );
	VOID						StoreAreas();
	VOID						DumpAreas();
	BOOL						LoadArea( NES_AREA_PTR ptr );
	BOOL						TryLoadArea( NES_AREA_PTR ptr );
	VOID						ClearSet();
	BOOL						CheckForLevelIsInOurSection( CNesLevel * pLevel );
	BOOL						IsPointerOutsideSetArea( USHORT uPtr );
	VOID						LoadLoops( size_t world, size_t level, CNesLevel * pLevel );
	VOID						LoadLoops( NES_AREA_PTR aptr, CNesLevel * pLevel );
	BYTE						AreaSet2World( BYTE bWorld );
	VOID						MakeWorld5WarpPatch();

	USHORT						GetADHalfWayPagesPtr();

	VOID						LoadHalfwayPages();
	VOID						DumpHalfwayPages();

	VOID						PushMeta( std::vector<NES_LEVEL_METADATA> & vSections, const NES_LEVEL_METADATA & meta );
	VOID						GetAreasSections( std::vector<NES_LEVEL_METADATA> & vSections, BOOL fSharedSection );

public:
	CNesLevelSet( CNesLevelsHolder & holder, NES_AREA_SET areaSet );
	~CNesLevelSet();

	VOID						LoadSet();
	VOID						SaveSet();
	VOID						DumpSet();
	CNesLevel *					GetLevel( BYTE bWorld, BYTE bLevel, BYTE bAreaId );
	VOID						GetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages );
	BOOL						SetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages );
	CNesLevelsKeeper *			GetAreaList( CNesMetatiles & mTiles, CNesGameEngineHack & hack );
	CNesObjectData &			ObjData();
	operator NES_AREA_SET();
	VOID						FreeStaleLinks();
	CNesLevel *					CreateArea( NES_LEVEL_TYPE type, BOOL fSharedSection );
	BOOL						IsPageReferenced( CNesLevel * pLevel, UINT uPage );
	BOOL						GetSourceList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>> & vLink );
	BOOL						SetSourcePage( CNesLevel * pLevel, UINT uPage, UINT uNewPage );
	NES_LIMITS					ValidateLimits();
};