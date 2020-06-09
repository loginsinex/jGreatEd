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

#define NES_LOOP_COMMAND_MINIMUM_PAGE				5
#define NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE			1

typedef struct _tagNES_LOOP
{
	NES_AREA_PTR			aptr;
	BYTE					bPageNumber;
	BYTE					bHeight;
	BYTE					bSlaveData;
	BYTE					bPageRewind;
} NES_LOOP, *PNES_LOOP;

class CNesLoop
{
	CNESFile				& m_file;
	CNesPointers			& m_eptr;
	std::vector<NES_LOOP>	m_vLoop;

public:
	CNesLoop( CNESFile & file, CNesPointers & ptr );

	VOID					Load();
	VOID					Dump();
	VOID					Flush();

	VOID					GetLoops( std::vector<NES_LOOP> & vLoops );
	VOID					SetLoops( const std::vector<NES_LOOP> & vLoops );
	BOOL					GetAreaLoops( NES_AREA_PTR aptr, std::vector<NES_LOOP> & vLoops );
};