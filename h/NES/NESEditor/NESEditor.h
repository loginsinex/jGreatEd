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

#include "h/NES/NES.h"

typedef enum _tagNES_LEVEL_TYPE : BYTE
{
	nltWater,
	nltGround,
	nltUnderground,
	nltCastle,
	nltTypeCount
} NES_LEVEL_TYPE, *PNES_LEVEL_TYPE;

typedef struct _tagNES_AREA_PTR
{
	union
	{
		BYTE		bPtr;
		struct
		{
			BYTE			bAreaId : 5;
			NES_LEVEL_TYPE	bType : 2;
			BYTE			bUnused : 1;
		};
	};

	bool operator<( const _tagNES_AREA_PTR & x ) const
	{
		return this->bPtr < x.bPtr;
	}

	bool operator==( const _tagNES_AREA_PTR & x ) const
	{
		return this->bPtr == x.bPtr;
	}

} NES_AREA_PTR, *PNES_AREA_PTR;


typedef enum _tagNES_AREA_SET : BYTE
{
	asWorld1to4,
	asWorld5to8,
	asWorld9,
	asWorldAtoD,
	asWorldSetsCount
} NES_AREA_SET, *PNES_AREA_SET;

#include "h/NES/NESEditor/CNesPointers.h"
#include "h/NES/NESEditor/CNesLeveleader.h"
#include "h/NES/NESEditor/CNesPatch.h"
#include "h/NES/NESEditor/CNesLoop.h"
#include "h/NES/NESEditor/CNesDemoPlay.h"
#include "h/NES/NESEditor/CNesGameEngineHack.h"
#include "h/NES/NESEditor/NesObjects/CNesGameLinks.h"
#include "h/NES/NESEditor/NesObjects/CNesTileCache.h"
#include "h/NES/NESEditor/NesObjects/CNesMetatiles.h"
#include "h/NES/NESEditor/CNesDrawQueue.h"
#include "h/NES/NESEditor/NesObjects/NesEnemyItems/ComplexEnemyObjects.h"
#include "h/NES/NESEditor/NesObjects/NesEnemyItems/SimpleEnemyItems.h"
#include "h/NES/NESEditor/NesObjects/NesLevelItems/SingleBlockObjects.h"
#include "h/NES/NESEditor/NesObjects/NesLevelItems/LongObjects.h"
#include "h/NES/NESEditor/NesObjects/NesLevelItems/MassiveObjects.h"
#include "h/NES/NESEditor/NesObjects/NesLevelItems/SingleBlockObjects.h"
#include "h/NES/NESEditor/NesObjects/NesLevelItems/CNesLevelItemGroundType.h"
#include "h/NES/NESEditor/CNesPlayerEntrance.h"
#include "h/NES/NESEditor/CNesLevel.h"
#include "h/NES/NESEditor/NesObjects/CNesLevelView.h"
#include "h/NES/NESEditor/CNesLevelInterface.h"
#include "h/NES/NESEditor/CNesLevelsKeeper.h"
#include "h/NES/NESEditor/CNesLevelsHolder.h"
#include "h/NES/NESEditor/CNesLevelSet.h"
#include "h/NES/NESEditor/CNesEditor.h"
