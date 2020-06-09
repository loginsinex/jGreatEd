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

typedef enum _tagNES_GROUND_TYPE
{
	ngtGroundMod,
	ngtSceneMod
} NES_GROUND_TYPE, *PNES_GROUND_TYPE;

typedef struct _tagNES_GROUND_INFO
{
	BOOL				fCloudLevel;
	//	BOOL				fGround;
	BYTE				bGroundCtl;
	NES_BG_COLOR_CTL	bBgCtl;
	NES_FG_SCENERY		bFgScenery;
	NES_BG_SCENERY		bBgScenery;
} NES_GROUND_INFO, *PNES_GROUND_INFO;

typedef struct _tagNES_GROUND_PLACE
{
	int												X;
	BOOL											fBehind;
	std::map<NES_GROUND_TYPE, NES_GROUND_INFO>		gr;
} NES_GROUND_PLACE, *PNES_GROUND_PLACE;

typedef struct _tagNES_GROUND_DATA
{
	BYTE			bBgSceneOffset[ 3 ];
	BYTE			bBgSceneData[ 144 ];
	NES_METATILE	bBgSceneMTiles[ 36 ];
	BYTE			bFgSceneOffset[ 3 ];
	NES_METATILE	bFgSceneData[ 39 ];
	NES_METATILE	bGroundMTiles[ 4 ];
	WORD			bGroundBits[ 16 ];
	NES_METATILE	bBlockBounds[ 4 ];
	BYTE			bSpecWaterBlockForWorld;
	NES_METATILE	bSpecWaterBlock;
	NES_METATILE	bBlockCloud;
	NES_METATILE	bBlockDefault;
} NES_GROUND_DATA, *PNES_GROUND_DATA;