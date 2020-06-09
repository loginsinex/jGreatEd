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

typedef struct _tagNES_LEVEL_SNAPSHOT
{
	NES_LEVEL_HEADER				header;
	std::vector<NES_GROUND_PLACE>	vGround;
	std::vector<NES_SNAPSHOT_ITEM>	vItems;
	std::vector<NES_SNAPSHOT_ENEMY>	vEnemy;
	std::vector<NES_LOOP>			vLoop;
	UINT							uPages;
	NES_LEVEL_TYPE					type;

	bool operator=( const _tagNES_LEVEL_SNAPSHOT & snap ) const
	{
		return false;
	}
} NES_LEVEL_SNAPSHOT, *PNES_LEVEL_SNAPSHOT;


typedef std::map<CNesLevel*, std::vector<NES_LEVEL_SNAPSHOT>>		LEVEL_SNAPSHOT_LIST;


class CNesEditor
{
	CNESFile								m_file;
	CNesPointers							m_eptr;
	std::map<NES_AREA_SET, CNesLevelSet*>	m_set;
	CNesMetatiles							m_metaTiles;
	CNesPlayerEntrance						m_player;
	CNesLevelsHolder	*					m_pHolder;
	CNesLoop								m_loop;
	CNesGameEngineHack						m_hack;
	CNesDemoPlay							m_demo;
	LEVEL_SNAPSHOT_LIST						m_mSnap;


	VOID						ClearSet();

	VOID						LoadLevelsSets();
	VOID						SaveLevelsSets();

	VOID						LoadGame();
	VOID						SaveGame();

	CNesLevelSet *				GetAreaSet( BYTE bWorld, BYTE & bRealWorldId );
	BYTE						SetupWarp( size_t uWorld, size_t uLevel, CNesLevel * pLevel );
	VOID						InitNewArea( CNesLevel * pLevel, NES_LEVEL_TYPE type, BOOL fAutoPlay );

public:
	CNesEditor();
	~CNesEditor();

	VOID						LoadFile( LPCTSTR pszFile );
	VOID						SaveFile( LPCTSTR pszFile );
	BOOL						IsFileLoaded();
	LPCTSTR						Filename();

	BOOL						GetLevelAreas( BYTE bWorld, BYTE bLevel, std::vector<CNesLevel*> & vpLevels );
	CNesLevelInterface *		GetInterface( CPaletteBitmap & bmView, BYTE bWorld, CNesLevel * pLevel );
	BYTE						GetTimerControl( NES_TIMER_CTRL tmrCtrl );
	BOOL						GetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages );
	BOOL						SetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages );
	CNesLevelsKeeper *			GetAreaList( BYTE bWorld );
	VOID						UpdateAreaType( CNesLevel * pLevel, NES_LEVEL_TYPE nlt );
	BOOL						GetPalette( NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, NES_PAL_ARRAY & npa );
	BOOL						SetPalette( NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, const NES_PAL_ARRAY & npa );
	BOOL						GetBgColors( BYTE bWorld, NES_BG_COLORS & nbg );
	BOOL						SetBgColors( BYTE bWorld, const NES_BG_COLORS & nbg );
	BOOL						GetHack( NES_ENGINE_HACK & hack );
	BOOL						SetHack( const NES_ENGINE_HACK & hack );
	BOOL						GetDemoData( std::vector<std::pair<BYTE, BYTE>> & vDemoData );
	BOOL						SetDemoData( const std::vector<std::pair<BYTE, BYTE>> & vDemoData );

	BYTE						WarpTarget( size_t uWorld, size_t uLevel, CNesLevel * pLevel );
	VOID						PointersChanged();

	CNesLevel *					CreateLevel( BYTE bWorld, NES_LEVEL_TYPE type, BOOL fSharedSection, BOOL fAutoPlay );
	CNesLevel *					CreateLevel( BYTE bWorld, CNesLevel * pBasedLevel );

	BOOL						IsPageReferenced( CNesLevel * pLevel, UINT uPage );

	BOOL						GetSourceLinkList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>> & vLinks );
	BOOL						SetSourcePageLink( CNesLevel * pLevel, UINT uPage, UINT uNewPage );
	BOOL						AddPage( CNesLevel * pLevel, UINT uPage );
	BOOL						DelPage( CNesLevel * pLevel, UINT uPage );

	BOOL						TakeLevelSnapshot( CNesLevel * pLevel );
	BOOL						RevertLevelState( CNesLevel * pLevel );
	BOOL						IsLevelCanReverted( CNesLevel * pLevel );

	BOOL						GetLimits( std::map<NES_AREA_SET, NES_LIMITS> & mLimits );
};