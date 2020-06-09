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

typedef struct _tagNES_LINK
{
	BYTE			uSourceWorld;
	BYTE			uTargetPage;
	NES_AREA_PTR	aptrTarget;
	POINT			ptLink;
} NES_LINK, *PNES_LINK;

class CNesLevel;
class CNesLevelItem;

class CNesLink
{
	const NES_AREA_SET			m_areaSet;
	const BYTE					m_bWorld;
	CNesLevel *					m_pSourceLevel;
	CNesLevel *					m_pDestLevel;
	BYTE						m_uDestPage;
	std::vector<CNesLevelItem*>	m_vRefs;

public:
	CNesLink( NES_AREA_SET areaSet, BYTE uSourceWorld, CNesLevel * pSourceLevel );
	~CNesLink();
	VOID				SetTarget( CNesLevel * pDestLevel, BYTE uDestPage );

	operator CNesLevel *( );

	CNesLevel *			SourceLevel();
	CNesLevel *			TargetLevel();
	BYTE				TargetPage();
	BYTE				SourceWorld();

	VOID				AddRef( CNesLevelItem * pItem );
	BOOL				Release( CNesLevelItem * pItem );
	BOOL				IsReferenced();
	NES_AREA_SET		AreaSet();
};

class CNesGameLinks
{
	std::map<std::pair<BYTE, CNesLevel*>, std::vector<CNesLink*>>			m_mLink;
	const NES_AREA_SET														m_areaSet;

public:
	CNesGameLinks( NES_AREA_SET areaSet );
	~CNesGameLinks();

	CNesLink			* AddLink( BYTE bWorld, CNesLevel * pLevel );
	VOID				RemoveLink( CNesLink * pLink );
	VOID				FreeStaleLinks();

	BOOL				GetSourceList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>> & vLink );
	BOOL				SetSourcePage( CNesLevel * pLevel, UINT uPage, UINT uNewPage );

	BOOL				IsPageReferenced( CNesLevel * pLevel, UINT uPage );
	BOOL				GetReferencesForTarget( std::vector<CNesLevel*> & vpSource, CNesLevel * pLevel, BOOL fIncludeSelfReference = TRUE ) const;
	NES_AREA_SET		AreaSet() const;
	BOOL				GetAreaLinks( CNesLevel * pLevel );
};