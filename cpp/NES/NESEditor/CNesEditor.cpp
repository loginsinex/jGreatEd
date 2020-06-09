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

CNesEditor::CNesEditor()
	:
	m_eptr( m_file ),
	m_metaTiles( m_file, m_eptr ),
	m_player( m_file, m_eptr ),
	m_pHolder( nullptr ),
	m_loop( m_file, m_eptr ),
	m_hack( m_file, m_eptr ),
	m_demo( m_file, m_eptr )
{

}

CNesEditor::~CNesEditor()
{
	ClearSet();
}

VOID CNesEditor::ClearSet()
{
	// delete holder with level in first
	// because levels uses CNesGameLinks which will be deleted in sets
	delete m_pHolder;

	for ( auto & set : m_set )
	{
		delete set.second;
	}

	m_set.clear();
}

VOID CNesEditor::LoadFile( LPCTSTR pszFile )
{
	if ( IsFileLoaded() )
	{
		return;
	}

	m_file.LoadFile( pszFile );
	LoadGame();
}

VOID CNesEditor::SaveFile( LPCTSTR pszFile )
{
	if ( !IsFileLoaded() )
	{
		return;
	}

	SaveGame();

	if ( !m_file.SaveFile( pszFile ) )
	{
		throw std::exception( "Can't write to file. It is possible that the file is read only or locked to write." );
	}
}

BOOL CNesEditor::IsFileLoaded()
{
	return m_file.IsFileLoaded();
}

LPCTSTR CNesEditor::Filename()
{
	return m_file.Filename();
}

// interface

CNesLevelSet * CNesEditor::GetAreaSet( BYTE bWorld, BYTE & bRealWorldId )
{
	CNesLevelSet * pSet = nullptr;

	if ( !m_file.IsFileLoaded() )
	{
		return pSet;
	}

	switch ( bWorld )
	{
		case 0:
		case 1:
		case 2:
		case 3: pSet = m_set[ asWorld1to4 ]; bRealWorldId = bWorld; break;
		case 4:
		case 5:
		case 6:
		case 7: pSet = m_set[ asWorld5to8 ]; bRealWorldId = bWorld - 4; break;
		case 8: pSet = m_set[ asWorld9 ]; bRealWorldId = 0; break;
		case 9:
		case 10:
		case 11:
		case 12: pSet = m_set[ asWorldAtoD ]; bRealWorldId = bWorld - 9; break;
	}

	return pSet;
}

BOOL CNesEditor::GetLevelAreas( BYTE bWorld, BYTE bLevel, std::vector<CNesLevel*> & vpLevels )
{
	if ( !m_file.IsFileLoaded() )
	{
		return FALSE;
	}

	BYTE bRealWorldId = 0, bAreaId = 0;
	CNesLevelSet * pSet = GetAreaSet( bWorld, bRealWorldId );
	CNesLevel * pLevel = nullptr;


	while ( pLevel = pSet->GetLevel( bRealWorldId, bLevel, bAreaId++ ) )
	{
		vpLevels.push_back( pLevel );
	}

	return ( vpLevels.size() > 0 );
}

CNesLevelInterface * CNesEditor::GetInterface( CPaletteBitmap & bmView, BYTE bWorld, CNesLevel * pLevel )
{
	if ( !m_file.IsFileLoaded() || !pLevel )
		return nullptr;

	return new CNesLevelInterface( m_metaTiles, m_hack, bmView, *pLevel, bWorld );
}

BOOL CNesEditor::GetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages )
{
	if ( !m_file.IsFileLoaded() )
	{
		return FALSE;
	}

	nAreaList.clear();
	nHalfwayPages.clear();

	for ( auto pSet : m_set )
	{
		pSet.second->GetAreaList( nAreaList, nHalfwayPages );
	}

	return ( nAreaList.size() > 0 );
}

BOOL CNesEditor::SetAreaList( NES_GAME_AREA_LIST & nAreaList, NES_GAME_AREA_HP_LIST & nHalfwayPages )
{
	if ( !m_file.IsFileLoaded() || nAreaList.size() != nHalfwayPages.size() )
	{
		return FALSE;
	}

	BOOL fResult = TRUE;
	size_t cWorlds = nAreaList.size();
	std::map<NES_AREA_SET, NES_GAME_AREA_LIST> mAreaList;
	std::map<NES_AREA_SET, NES_GAME_AREA_HP_LIST> mAreaHPList;

	for ( size_t n = 0; n < cWorlds; ++n )
	{
		BYTE __unused;
		CNesLevelSet * pSet = GetAreaSet( LOBYTE( n ), __unused );
		if ( !pSet ) continue;
		NES_AREA_SET as = *pSet;

		mAreaList[ as ].push_back( nAreaList[ n ] );
		mAreaHPList[ as ].push_back( nHalfwayPages[ n ] );
	}

	for ( auto & itSet : mAreaList )
	{
		NES_AREA_SET as = itSet.first;
		fResult &= m_set[ itSet.first ]->SetAreaList( mAreaList[ as ], mAreaHPList[ as ] );
	}

	return fResult;
}

CNesLevelsKeeper * CNesEditor::GetAreaList( BYTE bWorld )
{
	if ( m_file.IsFileLoaded() )
	{
		BYTE __unused = 0;
		CNesLevelSet * pSet = GetAreaSet( bWorld, __unused );
		if ( pSet )
		{
			return pSet->GetAreaList( m_metaTiles, m_hack );
		}
	}

	return nullptr;
}

VOID CNesEditor::UpdateAreaType( CNesLevel * pLevel, NES_LEVEL_TYPE nlt )
{
	if ( nlt >= 0 && nlt < nltTypeCount && m_file.IsFileLoaded() )
	{
		pLevel->UpdateAreaType( nlt );	// its not required to update levels set arrays, because its not used to dump into a file
	}
}

BYTE CNesEditor::GetTimerControl( NES_TIMER_CTRL tmrCtrl )
{
	if ( m_file.IsFileLoaded() )
	{
		return m_player.TimerControl( tmrCtrl );
	}

	return 0;
}

BOOL CNesEditor::GetPalette( NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, NES_PAL_ARRAY & npa )
{
	if ( m_file.IsFileLoaded() )
	{
		m_metaTiles.ReadPalette( npa, nlt, nas, bgModifier );
	}
	return FALSE;
}

BOOL CNesEditor::SetPalette( NES_LEVEL_TYPE nlt, NES_AREA_STYLE nas, NES_BG_COLOR_CTL bgModifier, const NES_PAL_ARRAY & npa )
{
	if ( m_file.IsFileLoaded() )
	{
		m_metaTiles.UpdatePalette( npa, nlt, nas, bgModifier );
		return TRUE;
	}
	return FALSE;
}

BOOL CNesEditor::GetBgColors( BYTE bWorld, NES_BG_COLORS & nbg )
{
	if ( m_file.IsFileLoaded() )
	{
		BYTE __unused = 0;
		CNesLevelSet * pSet = GetAreaSet( bWorld, __unused );
		if ( pSet )
		{
			nbg = pSet->ObjData().BgColors();
			return TRUE;
		}		
	}

	return FALSE;
}

BOOL CNesEditor::SetBgColors( BYTE bWorld, const NES_BG_COLORS & nbg )
{
	if ( m_file.IsFileLoaded() )
	{
		// update colors in ALL sets
		for ( auto pSet : m_set )
		{
			pSet.second->ObjData().SetBgColors( nbg );
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CNesEditor::GetHack( NES_ENGINE_HACK & hack )
{
	if ( m_file.IsFileLoaded() )
	{
		hack = m_hack.Read();
		for ( BYTE tmrCtrl = tmrCtrl0; tmrCtrl < tmrCtrlMax; ++tmrCtrl )
		{
			hack.timerValues.bTimerValue[ tmrCtrl ] = m_player.TimerControl( static_cast<NES_TIMER_CTRL>( tmrCtrl ) );
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CNesEditor::SetHack( const NES_ENGINE_HACK & hack )
{
	if ( m_file.IsFileLoaded() )
	{
		m_hack.Write( hack );
		for ( BYTE tmrCtrl = tmrCtrl0; tmrCtrl < tmrCtrlMax; ++tmrCtrl )
		{
			m_player.UpdateTimerControl( static_cast<NES_TIMER_CTRL>( tmrCtrl ), hack.timerValues.bTimerValue[ tmrCtrl ] );
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CNesEditor::GetDemoData( std::vector<std::pair<BYTE, BYTE>>& vDemoData )
{
	if ( m_file.IsFileLoaded() )
	{
		m_demo.DemoPlayData( vDemoData );
		return ( vDemoData.size() > 0 );
	}

	return FALSE;
}

BOOL CNesEditor::SetDemoData( const std::vector<std::pair<BYTE, BYTE>>& vDemoData )
{
	if ( m_file.IsFileLoaded() )
	{
		m_demo.WriteDemoPlayData( vDemoData );
		return TRUE;
	}

	return FALSE;
}

BYTE CNesEditor::SetupWarp( size_t uWorld, size_t uLevel, CNesLevel * pLevel )
{
	BYTE uWarpData = 0x80, __unused;
	CNesLevelSet * pSet = GetAreaSet( LOBYTE( uWorld ), __unused );
	const NES_WARP_WORLD_DATA & wwd = pLevel->ObjData().WarpWorldData();
	const NES_AREA_SET as = ( pSet ? *pSet : asWorldSetsCount );
	BYTE bOptionalAreaId = pLevel->GetOptionalAreaId( as );

	if ( uWorld < 9 )
	{
		if ( !uWorld )
		{
			if ( nltGround == pLevel->AreaType() )
			{
				uWarpData++;
			}
			else if ( bOptionalAreaId > 0 )	// check it out at FILE05:7525
			{
				uWarpData += 2;
			}
		}
		else
		{
			uWarpData = 0x83;
			if ( uWorld != wwd.bWorldData[ 0 ] )
			{
				uWarpData++;
				if ( uWorld == wwd.bWorldData[ 1 ] )
				{
					if ( bOptionalAreaId != wwd.bWorld5AreaId )
					{
						if ( nltGround == pLevel->AreaType() ) uWarpData += 2;
						else uWarpData += 1;
					}
				}
				else uWarpData += 3;
			}
		}
	}
	else
	{
		uWarpData = 0x87 + LOBYTE( uLevel );
	}

	return uWarpData;
}

BYTE CNesEditor::WarpTarget( size_t uWorld, size_t uLevel, CNesLevel * pLevel )
{
	BYTE uWarpData = SetupWarp( uWorld, uLevel, pLevel );
	if ( uWarpData >= 0x80 && uWarpData < 0x8B )
	{
		const NES_ENGINE_HACK & hack = m_hack.Read();
		return  hack.warp.bWorlds[ uWarpData - 0x80 ] - 1;
	}

	return 0xFF;
}

VOID CNesEditor::PointersChanged()
{
	for ( auto pSet : m_set )
	{
		pSet.second->FreeStaleLinks();
	}
}

VOID CNesEditor::InitNewArea( CNesLevel * pLevel, NES_LEVEL_TYPE type, BOOL fAutoPlay )
{
	std::vector<BYTE> vDummy1( 2, 0 ), vDummy2;
	std::vector<NES_LINK> __unused;
	CNesLevelHeader header;

	vDummy1.push_back( 0x0D );
	vDummy1.push_back( 0x02 );
	pLevel->LoadLevelData( vDummy1, vDummy2, __unused );
	header.UpdatePlayerCtrl( plrCtrl2 );
	switch ( type )
	{
		case nltWater:
		case nltGround:
		case nltUnderground:
			{
				header.UpdateGroundCtrl( 1 );
				break;
			}
		case nltCastle:
			{
				header.UpdateGroundCtrl( 7 );
				header.UpdateFgScenery( fgScenery3 );
				header.UpdatePlayerCtrl( plrCtrl3 );
				break;
			}
	}

	if ( fAutoPlay )
	{
		header.UpdatePlayerCtrl( plrCtrlAuto2 );
	}

	header.UpdateTimerCtrl( tmrCtrl1 );
	pLevel->UpdateHeader( header );
}

CNesLevel * CNesEditor::CreateLevel( BYTE bWorld, NES_LEVEL_TYPE type, BOOL fSharedSection, BOOL fAutoPlay )
{
	if ( !m_file.IsFileLoaded() )
	{
		return nullptr;
	}

	BYTE __unused;
	CNesLevelSet * pSet = GetAreaSet( bWorld, __unused );
	CNesLevel * pLevel = nullptr;
	if ( pSet )
	{
		pLevel = pSet->CreateArea( type, fSharedSection );
		InitNewArea( pLevel, type, fAutoPlay );
	}

	return pLevel;
}

CNesLevel * CNesEditor::CreateLevel( BYTE bWorld, CNesLevel * pBasedLevel )
{
	CNesLevel * pNewLevel = CreateLevel( bWorld, pBasedLevel->AreaType(), pBasedLevel->IsSharedLevel(), ( 2 & pBasedLevel->GetLevelStatus() ) );
	std::vector<NES_LINK> __unused;
	std::vector<BYTE> vAreaData, vEnemyData;
	
	if ( !pNewLevel )
	{
		return nullptr;
	}

	pBasedLevel->GetLevelBinaryData( vAreaData, vEnemyData );
	if ( vAreaData.size() > 0 && vEnemyData.size() > 0 )
	{
		vAreaData.pop_back();
		vEnemyData.pop_back();
		pNewLevel->LoadLevelData( vAreaData, vEnemyData, __unused );
	}

	return pNewLevel;
}

BOOL CNesEditor::IsPageReferenced( CNesLevel * pLevel, UINT uPage )
{
	BOOL fReferenced = FALSE;
	for ( auto pSet : m_set )
	{
		fReferenced |= pSet.second->IsPageReferenced( pLevel, uPage );
	}

	return fReferenced;
}

BOOL CNesEditor::GetSourceLinkList( CNesLevel * pLevel, UINT uPage, std::vector<std::pair<BYTE, CNesLevel*>> & vLinks )
{
	BOOL fReferenced = FALSE;
	BYTE bWorldInc[] = { 0, 0, 8, 9 };
	if ( m_file.IsFileLoaded() )
	{		
		for ( auto pSet : m_set )
		{
			std::vector<std::pair<BYTE, CNesLevel*>> vLocalLinks;
			fReferenced |= pSet.second->GetSourceList( pLevel, uPage, vLocalLinks );

			for ( auto & lnk : vLocalLinks )
			{
				lnk.first += bWorldInc[ static_cast<NES_AREA_SET>( *pSet.second ) ];
			}
			vLinks.insert( vLinks.end(), vLocalLinks.begin(), vLocalLinks.end() );
		}
	}

	return fReferenced;
}

BOOL CNesEditor::SetSourcePageLink( CNesLevel * pLevel, UINT uPage, UINT uNewPage )
{
	BOOL fReferenced = FALSE;
	if ( m_file.IsFileLoaded() && pLevel->PagesCount() > uPage && pLevel->PagesCount() > uNewPage )
	{
		for ( auto pSet : m_set )
		{
			fReferenced |= pSet.second->SetSourcePage( pLevel, uPage, uNewPage );
		}
	}

	return FALSE;
}

BOOL CNesEditor::AddPage( CNesLevel * pLevel, UINT uPage )
{
	if ( m_file.IsFileLoaded() )
	{
		UINT uLevelSize = 0;
		if ( !pLevel->AddPage( uPage ) ) return FALSE;

		uLevelSize = pLevel->PagesCount();

		for ( UINT page = uPage + 1; page < uLevelSize; ++page )
		{
			if ( IsPageReferenced( pLevel, page ) )
			{
				SetSourcePageLink( pLevel, page, page + 1 );
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CNesEditor::DelPage( CNesLevel * pLevel, UINT uPage )
{
	if ( m_file.IsFileLoaded() && !IsPageReferenced( pLevel, uPage ) )
	{
		UINT uLevelSize = 0;
		if ( !pLevel->DelPage( uPage ) ) return FALSE;

		uLevelSize = pLevel->PagesCount();
		for ( UINT page = uPage + 1; page < uLevelSize; ++page )
		{
			if ( IsPageReferenced( pLevel, page ) )
			{
				SetSourcePageLink( pLevel, page, page - 1 );
			}
		}

		return TRUE;
	}

	return FALSE;
}