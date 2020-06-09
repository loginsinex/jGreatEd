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

BOOL CNesEditor::TakeLevelSnapshot( CNesLevel * pLevel )
{
	auto & vSnap = m_mSnap[ pLevel ];
	NES_LEVEL_SNAPSHOT snap;
	pLevel->TakeLevelSnapshot(
		snap.header,
		snap.vGround,
		snap.vItems,
		snap.vEnemy,
		snap.vLoop,
		snap.uPages,
		snap.type
	);

	vSnap.push_back( snap );
	return TRUE;
}

BOOL CNesEditor::IsLevelCanReverted( CNesLevel * pLevel )
{
	auto itSnapshot = m_mSnap.find( pLevel );
	if ( itSnapshot != m_mSnap.end() )
	{
		return ( itSnapshot->second.size() > 0 );
	}

	return FALSE;
}

BOOL CNesEditor::RevertLevelState( CNesLevel * pLevel )
{
	auto itSnapshot = m_mSnap.find( pLevel );
	if ( itSnapshot != m_mSnap.end() )
	{
		if ( itSnapshot->second.size() > 0 )
		{
			const NES_LEVEL_SNAPSHOT & snap = itSnapshot->second.back();
			pLevel->RevertSnapshot(
				snap.header,
				snap.vGround,
				snap.vItems,
				snap.vEnemy,
				snap.vLoop,
				snap.uPages,
				snap.type
			);
			
			itSnapshot->second.pop_back();
			return TRUE;
		}
	}

	return FALSE;
}


