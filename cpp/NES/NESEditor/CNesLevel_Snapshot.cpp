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

VOID CNesLevel::TakePointerSnapshot( CNesLevelItem * pItem, NES_SNAPSHOT_ITEM & item )
{
	if ( pItem->IsEntranceObject() )
	{
		NES_AREA_SET asSets[] = { asWorld1to4, asWorld5to8, asWorld9, asWorldAtoD };
		item.fEntrance = TRUE;
		for ( const auto & as : asSets )
		{
			pItem->ReadLinks( as, item.mLinks[ as ] );
		}
	}
}

VOID CNesLevel::TakeLevelItemsSnapshot( std::vector<NES_SNAPSHOT_ITEM>& vItems )
{
	size_t c = m_obj.vpItem.size();
	NES_SNAPSHOT_ITEM nsiGround = { 0 };
	nsiGround.fFakeItem = TRUE;
	TakePointerSnapshot( &m_liGround, nsiGround );
	vItems.push_back( nsiGround );

	for ( size_t i = 1; i < c; ++i )
	{
		NES_SNAPSHOT_ITEM nsi = { 0 };
		CNesLevelItem * pItem = m_obj.vpItem[ i ];
		nsi.item.pt = pItem->GetRealPoint();
		nsi.item.item.X = LOBYTE( nsi.item.pt.x );
		nsi.item.item.Y = LOBYTE( nsi.item.pt.y );
		nsi.item.item.itemId = pItem->Item().item.itemId;
		nsi.fFakeItem = FALSE;

		TakePointerSnapshot( pItem, nsi );
		vItems.push_back( nsi );
	}
}

VOID CNesLevel::TakeEnemySnapshot( std::vector<NES_SNAPSHOT_ENEMY>& vItems )
{
	for ( auto & obj : m_obj.vpEnemy )
	{
		NES_SNAPSHOT_ENEMY item = { 0 };
		const POINT pt = obj->GetRealPoint();
		// skip unknown objects with Y > 0x0D, cause to no special objects, except of page skippers and pointers may be exists
		if ( pt.y < NES_SPECIAL_OBJECT_E )
		{
			item.pt.x = pt.x;
			item.pt.y = pt.y;
			item.item.X = LOBYTE( item.pt.x ) & 0x0F;
			item.item.Y = LOBYTE( item.pt.y ) & 0x0F;
			item.item.objectId = obj->Item().item.objectId;
			vItems.push_back( item );
		}
	}
}

VOID CNesLevel::RevertPointerSnapshot( CNesLevelItem * pItem, const NES_SNAPSHOT_ITEM & item )
{
	if ( item.fEntrance )
	{
		for ( const auto & itSet : item.mLinks )
		{
			for ( const auto & itWorld : itSet.second )
			{
				const BYTE bWorld = itWorld.first;
				const auto & link = itWorld.second;
				CNesLevel * pTargetLevel = link.second;
				const BYTE bPage = link.first;

				CNesLink * pLink = m_gLinks.AddLink( bWorld, this );
				pLink->SetTarget( pTargetLevel, ( bPage < pTargetLevel->PagesCount() ? bPage : pTargetLevel->PagesCount() ) );
				pItem->SetLink( pLink );
			}
		}
	}
}

VOID CNesLevel::RevertLevelItemsSnapshot( const std::vector<NES_SNAPSHOT_ITEM>& vItems )
{
	for ( const auto & item : vItems )
	{
		if ( item.fFakeItem )
		{
			RevertPointerSnapshot( &m_liGround, item );
		}
		else
		{
			CNesLevelItem * pItem = FetchLevelItem( item.item );
			if ( pItem )
			{
				m_obj.vpItem.push_back( pItem );
				RevertPointerSnapshot( pItem, item );
			}
#ifdef _DEBUG
			else
			{
				_tprintf( TEXT( "WARNING! One of level item was not reverted\n" ) );
			}
#endif
		}
	}
}

VOID CNesLevel::RevertEnemySnapshot( const std::vector<NES_SNAPSHOT_ENEMY>& vItems )
{
	for ( const auto & item : vItems )
	{
		m_obj.vpEnemy.push_back( FetchEnemyItem( item ) );
	}
}


VOID CNesLevel::TakeLevelSnapshot( 
	NES_LEVEL_HEADER & header, 
	std::vector<NES_GROUND_PLACE>& vGround,
	std::vector<NES_SNAPSHOT_ITEM>& vItems, 
	std::vector<NES_SNAPSHOT_ENEMY>& vEnemy, 
	std::vector<NES_LOOP>& vLoop,
	UINT & uPages, NES_LEVEL_TYPE & type )
{
	header.wData = m_header.GetRawHeader();
	m_liGround.TakeGroundSnapshot( vGround );
	TakeLevelItemsSnapshot( vItems );
	TakeEnemySnapshot( vEnemy );
	DumpLoops( vLoop );
	uPages = m_uPages;
	type = AreaType();
}

VOID CNesLevel::RevertSnapshot( 
	const NES_LEVEL_HEADER & header,
	const std::vector<NES_GROUND_PLACE>& vGround,
	const std::vector<NES_SNAPSHOT_ITEM>& vItems,
	const std::vector<NES_SNAPSHOT_ENEMY>& vEnemy,
	const std::vector<NES_LOOP>& vLoop,
	UINT uPages, NES_LEVEL_TYPE type )
{
	m_lvl.clear();
	m_enm.clear();
	m_liGround.Flush();
	FreeObjects();
	m_gLinks.FreeStaleLinks();

	m_obj.vpItem.push_back( &m_liGround );
	m_header.SetRawHeader( header.wData );
	m_liGround.RevertGroundSnapshot( vGround );
	RevertLevelItemsSnapshot( vItems );
	RevertEnemySnapshot( vEnemy );
	LoadLoops( vLoop );
	m_uPages = uPages;
	LevelSizeChanged();
	UpdateAreaType( type );
	InitObjects();
}