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

VOID CNesEditor::LoadGame()
{
	m_file.InitBootFiles();
	m_metaTiles.Load();
	m_player.LoadData();
	m_loop.Load();
	m_hack.LoadData();
	m_demo.LoadData();
	LoadLevelsSets();
}

VOID CNesEditor::LoadLevelsSets()
{
	CNesPatch patch( m_file, m_eptr );

	NES_AREA_SET as[] = { asWorld1to4, asWorld5to8, asWorld9, asWorldAtoD };

	ClearSet();

	m_pHolder = new CNesLevelsHolder( m_file, m_eptr, m_metaTiles, m_player, m_loop, patch.DoPatch() );
	for ( const auto & set : as )
	{
		CNesLevelSet * pSet = new CNesLevelSet( *m_pHolder, set );
		m_set[ set ] = pSet;

		pSet->LoadSet();
	}

	m_pHolder->FinalizeLoader();
}