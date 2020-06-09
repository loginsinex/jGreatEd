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
typedef struct _tagNES_TIMER_CONTROL
{
	BYTE		bData[ 4 ];
} NES_TIMER_CONTROL, *PNES_TIMER_CONTROL;

typedef struct _tagNES_ENTRANCE_CONTROL
{
	BYTE		bData[ 8 ];
} NES_ENTRANCE_CONTROL, *PNES_ENTRANCE_CONTROL;

typedef struct _tagNES_PLAYER_X
{
	BYTE		bData[ 2 ];
} NES_PLAYER_X, *PNES_PLAYER_X;
#pragma pack(pop)

class CNesPlayerEntrance
{
	CNESFile		& m_file;
	CNesPointers		& m_eptr;

	NES_TIMER_CONTROL		m_tmr;
	NES_ENTRANCE_CONTROL	m_entr;
	NES_PLAYER_X			m_plrX;

public:
	CNesPlayerEntrance( CNESFile & file, CNesPointers & eptr );

	VOID				LoadData();
	VOID				DumpData();

	VOID				UpdateTimerControl( NES_TIMER_CTRL tmrCtrl, BYTE bTimer );
	BYTE				TimerControl( NES_TIMER_CTRL tmrCtrl ) const;
	VOID				UpdateEntranceControl( NES_PLAYER_CTRL plrCtrl, BYTE bCtrl );
	BYTE				EntranceControl( NES_PLAYER_CTRL plrCtrl ) const;
	POINT				GetPlayerPoint( NES_PLAYER_CTRL plrCtrl ) const;
};