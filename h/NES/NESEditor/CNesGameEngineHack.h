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

#pragma pack(push,1)
typedef struct _tagNES_COINS_1UP_WORLD
{
	BYTE		bCoins[ 1 ];
} NES_COINS_1UP_WORLD, *PNES_COINS_1UP_WORLD;

typedef struct _tagNES_WARP_WORLD
{
	BYTE		bWorlds[ 11 ];
} NES_WARP_WORLD, *PNES_WARP_WORLD;

typedef struct _tagNES_HARD_MODE_LEVEL
{
	BYTE		bWorld;
	BYTE		bLevel;
} NES_HARD_MODE_LEVEL, *PNES_HARD_MODE_LEVEL;

typedef struct _tagNES_COIN_PAL_ROTATE_COLOR
{
	BYTE		bCol[ 6 ];
} NES_COIN_PAL_ROTATE_COLOR, *PNES_COIN_PAL_ROTATE_COLOR;

typedef BYTE NES_BOWSER_HAMMERS_WORLD;
typedef BOOL NES_DEFAULT_SPINY_EGG_BEHAVIOR;

typedef struct _tagNES_TIMER_VALUES
{
	BYTE		bTimerValue[ 4 ];
} NES_TIMER_VALUES, *PNES_TIMER_VALUES;
#pragma pack(pop)

typedef struct _tagNES_ENGINE_HACK
{
	NES_COINS_1UP_WORLD				coins;
	NES_WARP_WORLD					warp;
	NES_COIN_PAL_ROTATE_COLOR		rotateCols;
	NES_HARD_MODE_LEVEL				hardMode;
	NES_BOWSER_HAMMERS_WORLD		bowserHammers;
	NES_TIMER_VALUES				timerValues;
	NES_DEFAULT_SPINY_EGG_BEHAVIOR	defaultEggBehavior;
} NES_ENGINE_HACK, *PNES_ENGINE_HACK;

class CNesGameEngineHack
{
	CNESFile				& m_file;
	CNesPointers			& m_eptr;

	NES_ENGINE_HACK			m_data;

	BOOL					IsSpinyEggPatched();
	VOID					SetSpinyEggPatch( BOOL fPatch );

public:
	CNesGameEngineHack( CNESFile & file, CNesPointers & eptr );

	VOID					LoadData();
	VOID					DumpData();

	const NES_ENGINE_HACK &	Read() const;
	VOID					Write( const NES_ENGINE_HACK & hack );

};