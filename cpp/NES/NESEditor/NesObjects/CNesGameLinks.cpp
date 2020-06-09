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

CNesLink::CNesLink( NES_AREA_SET areaSet, BYTE uSourceWorld, CNesLevel * pSourceLevel )
	: m_pSourceLevel( pSourceLevel ), m_areaSet( areaSet ), m_bWorld( uSourceWorld ),
	m_pDestLevel( pSourceLevel ), m_uDestPage( 0 )
{
	
}

CNesLink::~CNesLink()
{
	
}

VOID CNesLink::SetTarget( CNesLevel * pDestLevel, BYTE uDestPage )
{
	m_pDestLevel = pDestLevel;
	m_uDestPage = min( uDestPage, pDestLevel->PagesCount() - 1 );
}

CNesLink::operator CNesLevel*( )
{
	return m_pSourceLevel;
}

CNesLevel * CNesLink::TargetLevel()
{
	return m_pDestLevel;
}

BYTE CNesLink::TargetPage()
{
	return m_uDestPage;
}

BYTE CNesLink::SourceWorld()
{
	return m_bWorld;
}

VOID CNesLink::AddRef( CNesLevelItem * pItem )
{
	auto it = std::find( m_vRefs.begin(), m_vRefs.end(), pItem );

	if ( it == m_vRefs.end() )
	{
		m_vRefs.push_back( pItem );
	}
}

BOOL CNesLink::Release( CNesLevelItem * pItem )
{
	auto it = std::find( m_vRefs.begin(), m_vRefs.end(), pItem );

	if ( it != m_vRefs.end() )
	{
		m_vRefs.erase( it );
	}

	return IsReferenced();
}

BOOL CNesLink::IsReferenced()
{
	return ( m_vRefs.size() > 0 );
}

CNesLevel * CNesLink::SourceLevel()
{
	return m_pSourceLevel;
}

NES_AREA_SET CNesLink::AreaSet()
{
	return m_areaSet;
}

/// game links container

CNesGameLinks::CNesGameLinks( NES_AREA_SET areaSet )
	: m_areaSet( areaSet )
{

}

CNesLink * CNesGameLinks::AddLink( BYTE bWorld, CNesLevel * pLevel )
{
	std::pair<BYTE, CNesLevel *> id = { bWorld, pLevel };

	CNesLink * pLink = new CNesLink( AreaSet(), bWorld, pLevel );
	m_mLink[ id ].push_back( pLink );
	return pLink;
}

VOID CNesGameLinks::RemoveLink( CNesLink * pLink )
{
	for ( auto & link : m_mLink )
	{
		std::vector<CNesLink*> & vLinks = link.second;

		auto itLink = std::find( vLinks.begin(), vLinks.end(), pLink );
		if ( itLink != vLinks.end() )
		{
			if ( !( *itLink )->IsReferenced() )
			{
				delete *itLink;
				vLinks.erase( itLink );
			}
		}
	}
}

/*
size_t CNesGameLinks::RemoveTarget( CNesLevel * pTarget )
{
	size_t uCounter = 0;
	for ( auto & link : m_mLink )
	{
		std::vector<CNesLink*> & vLinks = link.second;

		std::vector<CNesLink*>::iterator itLink;
		while 
			( 
				vLinks.end() != 
				( itLink = 
					std::find_if
					( 
						vLinks.begin(), vLinks.end(), 
						[pTarget]( CNesLink * it ) { return pTarget == it->TargetLevel(); } 
					) 
				) 
			)
		{
			uCounter++;
			delete *( itLink );
			vLinks.erase( itLink );
		}
	}

	return uCounter;
}
*/

VOID CNesGameLinks::FreeStaleLinks()
{
	for ( auto & link : m_mLink )
	{
		std::vector<CNesLink*> & vLinks = link.second;

		std::vector<CNesLink*>::iterator itLink;
		while
			(
				vLinks.end() !=
				( itLink =
				  std::find_if
				  (
					  vLinks.begin(), vLinks.end(),
					  [ ] ( CNesLink * it ) { return !it->IsReferenced(); }
				  )
				  )
				)
		{
			delete *( itLink );
			vLinks.erase( itLink );
		}
	}
}

BOOL CNesGameLinks::GetSourceList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>> & vLink )
{
	BYTE bPage = LOBYTE( uPage );
	vLink.clear();
	for ( auto & lvl : m_mLink )
	{
		std::pair<BYTE, CNesLevel*> pl = lvl.first;

		for ( auto & lnk : lvl.second )
		{
			if ( lnk->IsReferenced() && lnk->TargetLevel() == pLevel && lnk->TargetPage() == bPage )
			{
				vLink.push_back( { lnk->SourceWorld(), lnk->SourceLevel() } );
			}
		}
	}

	return ( vLink.size() > 0 );
}

BOOL CNesGameLinks::SetSourcePage( CNesLevel * pLevel, UINT uPage, UINT uNewPage )
{
	BYTE bPage = LOBYTE( uPage ), bNewPage = LOBYTE( uNewPage );
	BOOL fResult = FALSE;

	for ( auto & lvl : m_mLink )
	{
		std::pair<BYTE, CNesLevel*> pl = lvl.first;

		for ( auto & lnk : lvl.second )
		{
			if ( lnk->IsReferenced() && lnk->TargetLevel() == pLevel && lnk->TargetPage() == bPage )
			{
				lnk->SetTarget( lnk->TargetLevel(), uNewPage );
				fResult |= TRUE;
			}
		}
	}

	return fResult;
}

BOOL CNesGameLinks::IsPageReferenced( CNesLevel * pLevel, UINT uPage )
{
	std::vector<std::pair<BYTE, CNesLevel*>> vLinks;
	return GetSourceList( pLevel, uPage, vLinks );
}

CNesGameLinks::~CNesGameLinks()
{
	for ( auto & link : m_mLink )
	{
		std::vector<CNesLink*> & vLinks = link.second;

		while ( vLinks.size() > 0 )
		{
			delete vLinks.back();
			vLinks.pop_back();
		}
	}

	m_mLink.clear();
}

BOOL CNesGameLinks::GetReferencesForTarget( std::vector<CNesLevel*> & vpSource, CNesLevel * pLevel, BOOL fIncludeSelfReference ) const
{
	vpSource.clear();

	for ( auto & link : m_mLink )
	{
		const BYTE bWorld = link.first.first;
		CNesLevel * pSourceLevel = link.first.second;
		const auto & vTarget = link.second;

		if ( fIncludeSelfReference || ( pSourceLevel != pLevel ) )
		{
			for ( auto pLink : vTarget )
			{
				if ( pLink->TargetLevel() == pLevel )
				{
					vpSource.push_back( pSourceLevel );
				}
			}
		}
	}

	return ( vpSource.size() > 0 );
}

NES_AREA_SET CNesGameLinks::AreaSet() const
{
	return m_areaSet;
}