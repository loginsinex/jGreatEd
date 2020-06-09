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

typedef std::vector<std::vector<std::vector<CNesLevel*>>> NES_GAME_AREA_LIST, *PNES_GAME_AREA_LIST;
typedef std::vector<std::vector<BYTE>> NES_GAME_AREA_HP_LIST, *PNES_GAME_AREA_HP_LIST;
typedef std::map<NES_LEVEL_TYPE, std::vector<CNesLevel*>> NES_AREA_GROUPS, *PNES_AREA_GROUPS;

class CNesLevelsKeeper
{
	CNesGameLinks			& m_gLinks;
	std::vector<CNesLevel*>	m_vpLevels;
	CNesMetatiles			& m_mTiles;
	CNesGameEngineHack		& m_hack;

	BOOL					FindTarget( CNesLevel * pLevel, size_t & id );

public:
	CNesLevelsKeeper( CNesGameLinks & links, CNesGameEngineHack & hack, NES_AREA_GROUPS & nAreaList, CNesMetatiles & mTiles );
	size_t					AreasCount();
	CNesLevel *				EnumAreas( size_t id );
	BOOL					SetObjectTarget( BYTE bWorld, CNesLevelItem * pObj, CNesLevel * pLevel, BYTE uTargetPage );
	CNesLevelInterface *	GetInterface( BYTE bWorld, CPaletteBitmap & bmView, CNesLevel * pLevel );
};