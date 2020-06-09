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

#include "h/NES/NESEditor/NESEditor.h"

VOID CNesEditor::SaveLevelsSets()
{
	m_pHolder->InitDumper();
	for ( auto itSet : m_set )
	{
		itSet.second->SaveSet();
	}

	for ( auto itSet : m_set )
	{
		itSet.second->DumpSet();
	}

	m_pHolder->FinalizeDumper();
}

VOID CNesEditor::SaveGame()
{
	m_loop.Flush();	// erase all loops before making it by levels sets

	m_file.InitBootFiles();
	m_demo.DumpData();
	m_metaTiles.Dump();
	m_player.DumpData();
	SaveLevelsSets();
	m_loop.Dump();	// it will be stored in section for A-D worlds

	m_file.InitBootFiles();
	m_loop.Dump();	// it will be stored in section for 1-4 worlds (aka shared)
	m_hack.DumpData();
}

BOOL CNesEditor::GetLimits( std::map<NES_AREA_SET, NES_LIMITS>& mLimits )
{
	if ( !m_file.IsFileLoaded() ) return FALSE;

	m_pHolder->InitDumper();
	for ( auto itSet : m_set )
	{
		itSet.second->SaveSet();
	}

	for ( auto itSet : m_set )
	{
		mLimits[ *itSet.second ] = itSet.second->ValidateLimits();
	}

	m_pHolder->FinalizeDumper();

	return TRUE;
}