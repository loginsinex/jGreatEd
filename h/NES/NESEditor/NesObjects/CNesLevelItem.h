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

#include "CNesObject.h"
#include "CNesObjectData.h"

typedef enum _tagNES_ENTRANCE_TYPE
{
	etNotEntrance,
	etVine,
	etIntroPipe,
	etReversePipe,
	etVerticalPipe,
	etSideEntrance
} NES_ENTRANCE_TYPE, *PNES_ENTRANCE_TYPE;

class CNesLevelItem: public CNesObject
{
	BOOL							m_fForceForeground;
	NES_LEVEL_ITEM					m_item;

	std::map< NES_AREA_SET, std::map<BYTE, CNesLink*>>		m_mLinks;
	BYTE							NormalizeWorldId( BYTE bWorld, NES_AREA_SET & areaSet );
	CNesLink *						FindLink( NES_AREA_SET areaSet, BYTE bWorld );

protected:
	VOID							ModifyItemId( BYTE bItemId );

	NES_METATILE					HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile ) override;
	BOOL							IsObjectForeground();
	VOID							SetObjectToForeground();
	virtual VOID					DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject ) override;

public:
	const NES_LEVEL_ITEM &			Item();
	CNesLevelItem( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	CNesLevelItem( CNesLevel & nLevel );
	VOID							SetLink( CNesLink * pLink );
	VOID							RemoveLink( BYTE uWorld );
	VOID							RemoveLinks();

	virtual BOOL					IsEntranceObject();
	BOOL							IsEnterableObject();
	CNesLevel *						GetTargetLevel( BYTE uWorld, BYTE & uPage );
	VOID							DumpPointers( std::vector<NES_LEVEL_ENEMY_ITEM> & vItem );
	VOID							GetLinks( NES_AREA_SET areaSet, std::vector<CNesLevel*> & vpLinks );
	virtual NES_ENTRANCE_TYPE		EntranceType();
	VOID							ReadLinks( NES_AREA_SET as, std::map<BYTE, std::pair<UINT, CNesLevel*>>& mLinks );

	virtual ~CNesLevelItem();
};

