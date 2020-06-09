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

#include "h\NES\NESEditor\NESEditor.h"

CNesLevelsKeeper::CNesLevelsKeeper( CNesGameLinks & links, CNesGameEngineHack & hack, NES_AREA_GROUPS & nAreaList, CNesMetatiles & mTiles )
	: m_gLinks( links ), m_mTiles( mTiles ), m_hack( hack )
{
	for ( const auto & type : nAreaList )
	{
		for ( const auto pLevel : type.second )
		{
			if ( pLevel )
			{
				m_vpLevels.push_back( pLevel );
			}
		}
	}

	std::sort( m_vpLevels.begin(), m_vpLevels.end(), []( CNesLevel *p1, CNesLevel *p2 ) { return p1->Metadata().uLevelId < p2->Metadata().uLevelId; } );
}

size_t CNesLevelsKeeper::AreasCount()
{
	return m_vpLevels.size();
}

CNesLevel * CNesLevelsKeeper::EnumAreas( size_t id )
{
	if ( id < AreasCount() ) return m_vpLevels[ id ];

	return nullptr;
}

BOOL CNesLevelsKeeper::FindTarget( CNesLevel * pLevel, size_t & id )
{
	auto it = std::find( m_vpLevels.begin(), m_vpLevels.end(), pLevel );
	if ( it != m_vpLevels.end() )
	{
		id = ( it - m_vpLevels.begin() );
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevelsKeeper::SetObjectTarget( BYTE bWorld, CNesLevelItem * pObj, CNesLevel * pLevel, BYTE uTargetPage )
{
	// find the level from areas
	size_t id;

	if ( !pLevel )
	{
		return FALSE;
	}

	if ( FindTarget( pLevel, id ) )
	{
		if ( uTargetPage < pLevel->PagesCount() )
		{
			CNesLink * pLink = m_gLinks.AddLink( bWorld, &pObj->Level() );
			pLink->SetTarget( pLevel, uTargetPage );

			pObj->SetLink( pLink );
			m_gLinks.FreeStaleLinks();
			return TRUE;
		}
	}

	return FALSE;
}

CNesLevelInterface * CNesLevelsKeeper::GetInterface( BYTE bWorld, CPaletteBitmap & bmView, CNesLevel * pLevel )
{
	return new CNesLevelInterface( m_mTiles, m_hack, bmView, *pLevel, bWorld );
}