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

#define		NES_J_VAR_STORE_AREA		0x0765
#define		NES_J_VAR_LEVEL_NUMBER		0x075C

#pragma pack(push,1)
typedef struct _tagNES_WORLD_A_D_HWP_DATA
{
	BYTE			bHalfwayData[ 8 ];
	BYTE			bProc[ 12 ];
} NES_WORLD_A_D_HWP_DATA, *PNES_WORLD_A_D_HWP_DATA;
#pragma pack(pop)

typedef struct _tagNES_WORLD_A_D_DATA
{
	std::vector<USHORT>		vEnemyPtr;
	std::vector<USHORT>		vAreaPtr;
	BYTE					bEnemyHOffset[ nltTypeCount ];
	BYTE					bAreaHOffset[ nltTypeCount ];
	USHORT					uDataPtr[ 4 ];
	std::vector<BYTE>		vLoopRewindData;
} NES_WORLD_A_D_DATA, *PNES_WORLD_A_D_DATA;

class CNesPatch
{
	CNESFile				& m_file;
	CNesPointers			& m_eptr;
	
	NES_WORLD_A_D_DATA		m_adData;

	BOOL					IsROMPatched();
	VOID					MakeLoopPatch();
	VOID					MakeWorldsADPatch();
	VOID					LoadRequiredFiles( NES_AREA_SET as );
	USHORT					LoadLastSectionData();
	VOID					LoadLoopRewindData();
	USHORT					StoreLoopRewindData();
	VOID					StoreLastSectionData( USHORT uPtr );

public:
	CNesPatch( CNESFile & file, CNesPointers & eptr );

	BOOL					DoPatch();
};