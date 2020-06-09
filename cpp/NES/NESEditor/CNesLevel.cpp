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

#ifdef _DEBUG
std::map<size_t, size_t>		g_usedEnemyMemory, g_usedAreaMemory;
#endif


CNesLevel::CNesLevel(
	CNESFile & file,
	NES_AREA_PTR uPtr,
	const CNesPointers & eptr,
	CNesMetatiles & mtiles,
	const CNesObjectData & objData,
	CNesGameLinks & gLinks,
	const CNesPlayerEntrance & plr )
	:
	m_file( file ),
	m_lvlPtr( uPtr ),
	m_iLevelStatus( 0 ),
	m_mTiles( mtiles ),
	m_eptr( eptr ),
	m_liGround( *this ),
	m_objData( objData ),
	m_gLinks( gLinks ),
	m_uPages( 0 ),
	m_player( plr ),
	m_fLevelIsShared( FALSE ),
	m_uWarpData( 0 )
{

}

CNesLevel::~CNesLevel()
{
	FreeObjects();
	m_gLinks.FreeStaleLinks();
}

VOID CNesLevel::AddBankRef( BYTE bBankId )
{
	if ( m_vBankRef.end() == std::find( m_vBankRef.begin(), m_vBankRef.end(), bBankId ) )
	{
		m_vBankRef.push_back( bBankId );
	}
}

VOID CNesLevel::MarkLevelAsShared()
{
	m_fLevelIsShared = TRUE;
}

BOOL CNesLevel::IsSharedLevel()
{
	return ( m_vBankRef.size() > 1 || m_fLevelIsShared );
}

const NES_LEVEL_METADATA CNesLevel::Metadata() const
{
	return m_lvlMeta;
}

VOID CNesLevel::Metadata( const NES_LEVEL_METADATA & md )
{
	m_lvlMeta = md;
}

VOID CNesLevel::GetLevelLinks( NES_AREA_SET areaSet, std::vector<CNesLevel*>& vpLinks )
{
	for ( auto & obj : m_obj.vpItem )
	{
		obj->GetLinks( areaSet, vpLinks );
	}
}

VOID CNesLevel::ReadAreaLinks( std::map<BYTE, std::pair<UINT, CNesLevel*>>& mLinks )
{
	ReadItemLinks( m_obj.vpItem[ 0 ], mLinks );
}

VOID CNesLevel::ReadItemLinks( CNesLevelItem * pItem, std::map<BYTE, std::pair<UINT, CNesLevel*>>& mLinks )
{
	BYTE bWorldInc[] = { 0, 0, 8, 9 };
	std::vector<CNesLevel*> vpLinks;

	for ( BYTE i = asWorld1to4; i < asWorldSetsCount; ++i )
	{
		std::map<BYTE, std::pair<UINT, CNesLevel*>> mAsLinks;
		const NES_AREA_SET as = static_cast<NES_AREA_SET>( i );
		pItem->ReadLinks( as, mAsLinks );
		for ( const auto & itAsLink : mAsLinks )
		{
			mLinks[ itAsLink.first + bWorldInc[ i ] ] = itAsLink.second;
		}
	}
}

CNesLevel * CNesLevel::Copy()
{
	std::vector<BYTE> vItemsDump, vEnemiesDump;
	std::vector<NES_LINK> vLinks;	// doesnt required for this purposes
	std::vector<NES_LOOP> vLoops;
	CNesLevel * pCopy = new CNesLevel( m_file, m_lvlPtr, m_eptr, m_mTiles, m_objData, m_gLinks, m_player );
	GetLevelBinaryData( vItemsDump, vEnemiesDump );
	DumpLoops( vLoops );

	if ( vItemsDump.size() > 0 && vEnemiesDump.size() > 0 )
	{
		// ensure that this arrays filled correctly
		// and remove terminators markers
		vItemsDump.pop_back();
		vEnemiesDump.pop_back();

		try
		{
			pCopy->LoadLevelData( vItemsDump, vEnemiesDump, vLinks );
			pCopy->LoadLoops( vLoops );
			pCopy->m_uPages = m_uPages;
			pCopy->LevelSizeChanged();
			return pCopy;
		}
		catch ( std::exception & e )			
		{
			UNREFERENCED_PARAMETER( e );
			delete pCopy;
			return nullptr;
		}
	}

	return nullptr;
}

VOID CNesLevel::UpdateAreaType( NES_LEVEL_TYPE bType )
{
	m_lvlPtr.bType = bType;
}

VOID CNesLevel::SetAreaId( BYTE bId )
{
	m_lvlPtr.bAreaId = bId;
}

VOID CNesLevel::SetOptionalAreaId( NES_AREA_SET nas, BYTE bId )
{
	NES_AREA_PTR aptr = { 0 };
	aptr.bAreaId = bId; aptr.bType = AreaType();
	m_mLvlOptionalPtr[ nas ] = aptr;
}

BYTE CNesLevel::GetOptionalAreaId( NES_AREA_SET nas )
{
	auto itAreaId = m_mLvlOptionalPtr.find( nas );
	if ( itAreaId != m_mLvlOptionalPtr.end() )
	{
		return itAreaId->second.bAreaId;
	}

	return 0xFF;
}

NES_AREA_SET CNesLevel::World2Areaset( BYTE bWorld )
{
	switch ( bWorld )
	{
		case 0:
		case 1:
		case 2:
		case 3: return asWorld1to4;
		case 4:
		case 5:
		case 6:
		case 7: return asWorld5to8;
		case 8: return asWorld9;
		case 9:
		case 0x0a:
		case 0x0b:
		case 0x0c: return asWorldAtoD;
	}

	return asWorldSetsCount;
}

const CNesObjectData & CNesLevel::ObjData()
{
	return m_objData;
}

int CNesLevel::GetLevelStatus()
{
	return m_iLevelStatus;
}

NES_AREA_PTR CNesLevel::AreaPtr()
{
	return m_lvlPtr;
}

const CNesLevelHeader & CNesLevel::Header()
{
	return m_header;
}

BOOL CNesLevel::UpdateHeader( const CNesLevelHeader & header )
{
	m_header = header;
	if ( !m_header.IsCloudLevel() )
	{
		m_liGround.RemoveLinks();
	}
	
	m_liGround.InitGroundInfo();
	InitObjects();
	return TRUE;
}

NES_LEVEL_TYPE CNesLevel::AreaType()
{
	return m_lvlPtr.bType;
}

SIZE CNesLevel::LevelSize()
{
	SIZE sz = { m_uPages * 16, 16 };
	return sz;
}

BYTE CNesLevel::PagesCount()
{
	return LOBYTE( m_uPages );
}

VOID CNesLevel::FreeObjects()
{
	while ( m_obj.vpItem.size() > 1 )
	{
		delete m_obj.vpItem.back();
		m_obj.vpItem.pop_back();
	}

	while ( m_obj.vpEnemy.size() > 0 )
	{
		delete m_obj.vpEnemy.back();
		m_obj.vpEnemy.pop_back();
	}

	m_obj.vpItem.clear();
	m_obj.vpEnemy.clear();
}

BYTE CNesLevel::LevelBgCtl()
{
	BYTE bBgCtl = m_liGround.BgCtl();
	if ( !bBgCtl )
	{
		bBgCtl = AreaType();
	}
	return bBgCtl;
}

BYTE CNesLevel::GetBgColor()
{
	return ObjData().BgColors().bColor[ LevelBgCtl() & 7 ] & 0x3f;
}

VOID CNesLevel::ValidateLevelPalette( CNesLevelView & view )
{
	view.SelectBg( GetBgColor() );
	view.SelectPalette( AreaType(), m_header.AreaStyle(), static_cast<NES_BG_COLOR_CTL>( LevelBgCtl() ) );
}

VOID CNesLevel::LevelSizeChanged()
{
	SIZE sz = { m_uPages * 16, 16 };
	for ( auto & it : m_obj.vpItem )
	{
		it->LevelSizeChanged( sz );
	}
	
	for ( auto & it : m_obj.vpEnemy )
	{
		it->LevelSizeChanged( sz );
	}
}

VOID CNesLevel::InitObjects()
{
//	_tprintf( TEXT( "Initializing level objects...\n" ) );
	for ( auto & it : m_obj.vpItem )
	{
		it->InitObject();
	}

	for ( auto & it : m_obj.vpEnemy )
	{
		it->InitObject();
	}
}

VOID CNesLevel::InitView( CNesLevelView & view )
{
	view.SetMapSize( m_uPages * 16, 16 );
}

VOID CNesLevel::InitDrawQueue( std::vector<NES_DRAW_QUEUE_ITEM>& vDrawQueue )
{
	size_t cItems = m_obj.vpItem.size();
	vDrawQueue.reserve( cItems );
	for ( size_t i = 1; i < cItems; ++i )
	{
		CNesLevelItem * pItem = m_obj.vpItem[ i ];
		NES_DRAW_QUEUE_ITEM dqi = { pItem, nullptr, pItem->GetRealPoint().x, pItem->Width(), pItem->GetRealPoint().y };
		dqi.right += dqi.left;
		vDrawQueue.push_back( std::move( dqi ) );
	}

	m_liGround.InitDrawQueue( vDrawQueue );

	// compare by following WORD: <High byte: PAGE><Low byte: <High nibble: X><Low nibble: Y>>
	// to be a more predictable
	std::sort( vDrawQueue.begin(), vDrawQueue.end(), 
		[]( const NES_DRAW_QUEUE_ITEM & dqi1, const NES_DRAW_QUEUE_ITEM & dqi2 )
	{
		WORD bData1 = MAKEWORD( LOBYTE( ( ( dqi1.left & 0x0F ) << 4 ) | dqi1.y & 0x0F ), LOBYTE( dqi1.left >> 4 ) );
		WORD bData2 = MAKEWORD( LOBYTE( ( ( dqi2.left & 0x0F ) << 4 ) | dqi2.y & 0x0F ), LOBYTE( dqi2.left >> 4 ) );

		return ( bData1 < bData2 );
	}
	);
}

VOID CNesLevel::SetupBehindFlag( std::vector<NES_DRAW_QUEUE_ITEM>& vdqi, BOOL fBehind )
{
	for ( const auto & dqi : vdqi )
	{
		if ( dqi.pGround )
		{
			dqi.pGround->fBehind = fBehind;
		}

		if ( dqi.pObject )
		{
			dqi.pObject->PutBehind( fBehind );
		}
	}
}

VOID CNesLevel::InitObjectsQueue()
{
	std::vector<NES_DRAW_QUEUE_ITEM> vFront, vBack, vQueue;
	CNesDrawQueue queue( vFront, vBack );
	InitDrawQueue( vQueue );
	vFront.reserve( vQueue.size() );
	vBack.reserve( vQueue.size() );

	for ( auto & dqi : vQueue )
	{
		queue.NextObject( dqi );
	}

	queue.FinalizeQueue();

	SetupBehindFlag( vFront, FALSE );
	SetupBehindFlag( vBack, TRUE );

	m_liGround.InitGroundInfo();
	m_liGround.InitObject();
}

VOID CNesLevel::DrawLevel( CNesLevelView & view, BYTE bWorldId )
{
	size_t cObjects = m_obj.vpItem.size(), cEnemies = m_obj.vpEnemy.size();

	view.EraseSpriteMap();
	ValidateLevelPalette( view );

	for ( size_t i = 0; i < cObjects; ++i )
	{
		m_obj.vpItem[ i ]->DrawObject( view, bWorldId );
	}

	for ( size_t i = 0; i < cEnemies; ++i )
	{
		m_obj.vpEnemy[ i ]->DrawObject( view, bWorldId );
	}

	m_liGround.DrawBounds( view );
}

VOID CNesLevel::UpdateLevelStatus()
{
	size_t c = m_lvl.size();
	int & result = m_iLevelStatus;	// 1 - EOF world, 2 - requires next area, 4 - into pipe was found

	if ( m_header.PlayerCtl() == 0x06 || m_header.PlayerCtl() == 0x07 )
	{
		result |= 2;
	}
}

CNESFile & CNesLevel::ROM()
{
	return m_file;
}

const CNesPointers & CNesLevel::Pointers()
{
	return m_eptr;
}

int CNesLevel::LoadLevelData(
	const std::vector<BYTE>& vAreaData,
	const std::vector<BYTE>& vEnemyData,
	std::vector<NES_LINK> & vLinks
)
{
	LONG uPage = 0;

#ifdef _DEBUG
	g_usedEnemyMemory[ m_lvlMeta.uLevelId ] = vEnemyData.size() + 1;
	g_usedAreaMemory[ m_lvlMeta.uLevelId ] = vAreaData.size() + 1;
#endif

	if ( vAreaData.size() < 2 )
	{
		return 0;
		// throw std::exception( "Insufficient data for level" );
	}

	// load area data
	auto areaIt = vAreaData.cbegin(), areaEnd = vAreaData.cend();
	auto enmIt = vEnemyData.cbegin(), enmEnd = vEnemyData.cend();

	// clear all data before anyting loading
	m_lvl.clear();
	m_enm.clear();
	m_liGround.Flush();
	FreeObjects();

	// fetch header
	m_header.SetRawHeader( MAKEWORD( *( areaIt ), *( areaIt + 1 ) ) );
	areaIt += 2;
	m_lvl.reserve( 64 );
	m_enm.reserve( 64 );

	// insert ground object as first and initialize ground info
	m_obj.vpItem.push_back( &m_liGround );
	m_liGround.InitGroundInfo();

	try
	{
		// parse raw data
		m_uPages = ParseLevelData( areaIt, areaEnd );
	}
	catch ( std::exception & e )
	{
		UNREFERENCED_PARAMETER( e );
		m_iLevelStatus |= 4;
	}

	try
	{
		uPage = ParseEnemyData( enmIt, enmEnd, vLinks );
	}
	catch ( std::exception & e )
	{
		UNREFERENCED_PARAMETER( e );
		m_iLevelStatus |= 8;
	}

	m_uPages = max( uPage, m_uPages ) + 2;

	UpdateLevelStatus();
	LevelSizeChanged();

	return m_iLevelStatus;
}

BOOL CNesLevel::GetLevelBinaryData( std::vector<BYTE>& vLevelData, std::vector<BYTE>& vEnemyData )
{
	// reinitialize objects,
	// so they can have chance to update its ItemId and all points

	InitObjects();
	SerializeLevelData( vLevelData );
	SerializeEnemyData( vEnemyData );
	vLevelData.push_back( 0xFD );	// finalize level data
	vEnemyData.push_back( 0xFF );	// finalize enemy data

#ifdef _DEBUG

	if ( g_usedEnemyMemory[ m_lvlMeta.uLevelId ] < vEnemyData.size() )
	{
		_tprintf( TEXT( "* WARNING: Enemy data for level %Iu is grow up. Was %Iu, now %Iu\n" ), m_lvlMeta.uLevelId, g_usedEnemyMemory[ m_lvlMeta.uLevelId ], vEnemyData.size() );
	}
	else if ( g_usedEnemyMemory[ m_lvlMeta.uLevelId ] > vEnemyData.size() )
	{
		_tprintf( TEXT( "* WARNING: Enemy data for level %Iu is reduced. Was %Iu, now %Iu\n" ), m_lvlMeta.uLevelId, g_usedEnemyMemory[ m_lvlMeta.uLevelId ], vEnemyData.size() );
	}

	if ( g_usedAreaMemory[ m_lvlMeta.uLevelId ] < vLevelData.size() )
	{
		_tprintf( TEXT( "* WARNING: Area data for level %Iu is grow up. Was %Iu, now %Iu\n" ), m_lvlMeta.uLevelId, g_usedAreaMemory[ m_lvlMeta.uLevelId ], vLevelData.size() );
	}
	else if ( g_usedAreaMemory[ m_lvlMeta.uLevelId ] > vLevelData.size() )
	{
		_tprintf( TEXT( "* WARNING: Area data for level %Iu is reduced. Was %Iu, now %Iu\n" ), m_lvlMeta.uLevelId, g_usedAreaMemory[ m_lvlMeta.uLevelId ], vLevelData.size() );
	}

	g_usedEnemyMemory[ m_lvlMeta.uLevelId ] = vEnemyData.size();
	g_usedAreaMemory[ m_lvlMeta.uLevelId ] = vLevelData.size();
#endif

	return ( vLevelData.size() > 1 || vEnemyData.size() > 1 );
}


CNesObject * CNesLevel::ObjectFromPoint( int x, int y )
{
	const size_t cObjects = m_obj.vpItem.size();
	const size_t cEnemies = m_obj.vpEnemy.size();
	CNesObject * pSelObject = nullptr;
	POINT pt = { x,y };

	for ( size_t i = 0; i < cEnemies; ++i )
	{
		CNesObject * pObj = m_obj.vpEnemy[ i ];
		const RECT rcCur = pObj->GetObjectRect();
		if ( PtInRect( &rcCur, pt ) )
		{
			if ( pSelObject )
			{
				const RECT rcSel = pSelObject->GetObjectRect();
				if ( ( rcCur.bottom - rcCur.top ) <= ( rcSel.bottom - rcSel.top ) &&
					( rcCur.right - rcCur.left ) <= ( rcSel.right - rcSel.left ) )
				{
					pSelObject = pObj;
				}
			}
			else
			{
				pSelObject = pObj;
			}
		}
	}

	if ( pSelObject )
	{
		return pSelObject;
	}

	for ( size_t i = 1; i < cObjects; ++i )
	{
		CNesObject * pObj = m_obj.vpItem[ i ];
		const RECT rcCur = pObj->GetObjectRect();
		if ( PtInRect( &rcCur, pt ) )
		{
			if ( pSelObject )
			{
				const RECT rcSel = pSelObject->GetObjectRect();
				if ( ( rcCur.bottom - rcCur.top ) <= ( rcSel.bottom - rcSel.top ) &&
					( rcCur.right - rcCur.left ) <= ( rcSel.right - rcSel.left ) )
				{
					pSelObject = pObj;
				}
			}
			else
			{
				pSelObject = pObj;
			}
		}
	}

	return pSelObject;
}

BOOL CNesLevel::ObjectsFromRect( const RECT & rc, std::vector<CNesObject*>& vpObj )
{
	const size_t cObjects = m_obj.vpItem.size();
	const size_t cEnemies = m_obj.vpEnemy.size();
	vpObj.clear();

	for ( size_t i = 0; i < cEnemies; ++i )
	{
		CNesObject * pObj = m_obj.vpEnemy[ i ];
		const RECT rcObj = pObj->GetObjectRect();
		RECT rcIntersect;

		if ( IntersectRect( &rcIntersect, &rc, &rcObj ) )
		{
			vpObj.push_back( pObj );
		}
	}

	for ( size_t i = 1; i < cObjects; ++i )
	{
		CNesObject * pObj = m_obj.vpItem[ i ];
		const RECT rcObj = pObj->GetObjectRect();
		RECT rcIntersect;

		if ( IntersectRect( &rcIntersect, &rc, &rcObj ) )
		{
			vpObj.push_back( pObj );
		}
	}

	return ( vpObj.size() > 0 );
}

VOID CNesLevel::ResortObjects()
{
	// we sorting in at DrawObjectsQueue and while serializing. So are we need to sort here everytime?
	std::sort( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(),
		[]( CNesLevelItem * pObj1, CNesLevelItem * pObj2 )
	{
		// compare by following WORD: <High byte: PAGE><Low byte: <High nibble: X><Low nibble: Y>>
		// to be a more predictable
		POINT pt1 = pObj1->GetObjectPoint(), pt2 = pObj2->GetObjectPoint();
		WORD bData1 = MAKEWORD( LOBYTE( ( ( pt1.x & 0x0F ) << 4 ) | pt1.y & 0x0F ), LOBYTE( pt1.x >> 4 ) );
		WORD bData2 = MAKEWORD( LOBYTE( ( ( pt2.x & 0x0F ) << 4 ) | pt2.y & 0x0F ), LOBYTE( pt2.x >> 4 ) );
		
		// return ( pObj1->GetObjectPoint().x < pObj2->GetObjectPoint().x );
		return ( bData1 < bData2 );
	} );


	std::sort( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(),
		[]( CNesEnemyObject * pObj1, CNesEnemyObject * pObj2 )
	{
		POINT pt1 = pObj1->GetObjectPoint(), pt2 = pObj2->GetObjectPoint();
		WORD bData1 = MAKEWORD( LOBYTE( ( ( pt1.x & 0x0F ) << 4 ) | pt1.y & 0x0F ), LOBYTE( pt1.x >> 4 ) );
		WORD bData2 = MAKEWORD( LOBYTE( ( ( pt2.x & 0x0F ) << 4 ) | pt2.y & 0x0F ), LOBYTE( pt2.x >> 4 ) );

		return ( bData1 < bData2 );

		//	return ( pObj1->GetObjectPoint().x < pObj2->GetObjectPoint().x );
	} );
}

int CNesLevel::AssignLink( CNesLink * pLink, int uBeginPoint )
{
	int uCounter = 0;
	for ( auto & obj : m_obj.vpItem )
	{
		if ( obj->IsEntranceObject() )
		{
			RECT rcObject = obj->GetObjectRect();

			if ( rcObject.left + 8 >= uBeginPoint )
			{
				obj->SetLink( pLink );
				uCounter++;
			}
		}
	}

	return uCounter;
}

int CNesLevel::AssignGroundLink( CNesLink * pLink )
{
	if ( m_header.IsCloudLevel() )
	{
		m_liGround.SetLink( pLink );
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevel::MoveGround( int uOldX, int uNewX )
{
	return m_liGround.MoveGround( uOldX, uNewX );
}

BOOL CNesLevel::GetGround( int X, std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & gr )
{
	return m_liGround.GetGround( X, gr );
}

VOID CNesLevel::SetGround( int x, const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & gr )
{
	EraseGround( x );
	auto itGround = gr.find( ngtGroundMod );
	if ( itGround != gr.end() )	m_liGround.AddGroundInfo( x, itGround->first, itGround->second );
	itGround = gr.find( ngtSceneMod );
	if ( itGround != gr.end() )	m_liGround.AddGroundInfo( x, itGround->first, itGround->second );

	m_liGround.InitObject();
}

BOOL CNesLevel::EraseGround( int x )
{
	return m_liGround.EraseGround( x );
}

CNesLevelItem * CNesLevel::AddLevelItem( const NES_LEVEL_ITEM & item )
{
	if ( m_obj.vpItem.size() > 128 )
	{
		return nullptr;
	}

	CNesLevelItem * pItem = FetchLevelItem( item );
	if ( pItem )
	{
		m_obj.vpItem.push_back( pItem );
		pItem->InitObject();
		ResortObjects();
		return pItem;
	}
	return nullptr;
}

CNesEnemyObject * CNesLevel::AddEnemyItem( const NES_LEVEL_ENEMY_ITEM & item )
{
	if ( m_obj.vpEnemy.size() >= 128 )
	{
		return nullptr;
	}

	CNesEnemyObject * pObj = FetchEnemyItem( item );
	if ( pObj )
	{
		m_obj.vpEnemy.push_back( pObj );
		pObj->InitObject();
		ResortObjects();
		return pObj;
	}

	return nullptr;
}

BOOL CNesLevel::IsValidObject( CNesObject * pObject )
{
	auto itEnemy = std::find( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(), pObject );
	if ( itEnemy != m_obj.vpEnemy.end() )
	{
		return TRUE;
	}
	
	auto itItem = std::find( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(), pObject );
	if ( itItem != m_obj.vpItem.end() )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevel::DeleteObject( CNesObject * pObject )
{
	if ( IsValidObject( pObject ) )
	{
		if ( pObject->IsSprite() )
		{
			auto it = std::find( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(), pObject );
			if ( it != m_obj.vpEnemy.end() )
			{
				m_obj.vpEnemy.erase( it );
				delete pObject;
				return TRUE;
			}
		}
		else
		{
			auto it = std::find( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(), pObject );
			if ( it != m_obj.vpItem.end() )
			{
				m_obj.vpItem.erase( it );
				delete pObject;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CNesLevel::AddPage( UINT uBeforePage )
{
	int x = uBeforePage * 16;
	if ( PagesCount() >= 64 )
	{
		return FALSE;
	}

	m_uPages++;

	for ( auto it = m_obj.vpItem.begin() + 1; it < m_obj.vpItem.end(); ++it )
	{
		CNesLevelItem * pItem = *it;
		RECT rc = pItem->GetObjectRect();
		if ( rc.left >= x )
		{
			pItem->PlaceObject( rc.left + 16, rc.top );
		}
	}

	for ( auto & it : m_obj.vpEnemy )
	{
		CNesEnemyObject * pItem = it;
		RECT rc = pItem->GetObjectRect();
		if ( rc.left >= x )
		{
			pItem->PlaceObject( rc.left + 16, rc.top );
		}
	}

	m_liGround.AddPage( uBeforePage );

	LevelSizeChanged();
	InitObjects();

	return TRUE;
}

BOOL CNesLevel::DelPage( UINT uPage )
{
	if ( PagesCount() > 1 )
	{
		int x0 = uPage * 16;
		int x1 = x0 + 16;

		auto itItem = m_obj.vpItem.begin();
		auto itEnemy = m_obj.vpEnemy.begin();
		while
			(
				m_obj.vpItem.end() !=
				(
					itItem = std::find_if
					(
						m_obj.vpItem.begin() + 1, m_obj.vpItem.end(),
						[x0, x1]( CNesLevelItem * pItem )
						{
							int x = pItem->GetObjectPoint().x;
							if ( x >= x0 && x < x1 )
							{
								return TRUE;
							}
							return FALSE;
						}
					)
				)
			)
		{
			DeleteObject( *itItem );
		}

		// decrease all higher objects points by 1 page
		std::find_if(m_obj.vpItem.begin() + 1,m_obj.vpItem.end(),
			[x1, uPage]( CNesLevelItem * pItem )
		{
			POINT pt = pItem->GetObjectPoint();
			if ( pt.x >= x1 )
			{
				pItem->PlaceObject( pt.x - 16, pt.y );
			}

			return false;
		} );

		while
			(
				m_obj.vpEnemy.end() !=
				(
					itEnemy = std::find_if
					(
						m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(),
						[x0, x1]( CNesEnemyObject * pItem )
						{
							int x = pItem->GetObjectPoint().x;
							if ( x >= x0 && x < x1 )
							{
								return TRUE;
							}
							return FALSE;
						}
					)
				)
			)
		{
			DeleteObject( *itEnemy );
		}

		std::find_if( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(),
			[x1]( CNesEnemyObject * pItem )
		{
			POINT pt = pItem->GetObjectPoint();
			if ( pt.x >= x1 )
			{
				pItem->PlaceObject( pt.x - 16, pt.y );
			}
			return false;
		} );

						
		m_uPages--;
		m_liGround.DelPage( uPage );
		LevelSizeChanged();
		InitObjects();
		return TRUE;
	}
	return FALSE;
}


CNesObject * CNesLevel::ObjectById( size_t idObject, BOOL fSprite )
{
	if ( fSprite )
	{
		if ( idObject < m_obj.vpEnemy.size() )
		{
			return m_obj.vpEnemy[ idObject ];
		}
	}
	else
	{
		if ( idObject > 0 && idObject < m_obj.vpItem.size() )
		{
			return m_obj.vpItem[ idObject ];
		}
	}

	return nullptr;
}

BOOL CNesLevel::ObjectId( CNesObject * pObject, size_t & idObject, BOOL & fSprite )
{
	auto itEnemy = std::find( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(), pObject );
	if ( itEnemy != m_obj.vpEnemy.end() )
	{
		fSprite = TRUE;

		idObject = ( itEnemy - m_obj.vpEnemy.begin() );
		return TRUE;
	}

	auto itItem = std::find( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(), pObject );
	if ( itItem != m_obj.vpItem.end() )
	{
		fSprite = FALSE;
		idObject = ( itItem - m_obj.vpItem.begin() );
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevel::GetAreaPointer( BYTE bWorld, CNesLevel ** pLevel, BYTE & uPage )
{
	if ( m_liGround.IsEnterableObject() )
	{
		*pLevel = m_liGround.GetTargetLevel( bWorld, uPage );
		return TRUE;
	}
	return FALSE;
}

BOOL CNesLevel::SetAreaPointer( BYTE bWorld, CNesLevel * pLevel, BYTE uPage )
{
	if ( pLevel && uPage < pLevel->PagesCount() )
	{
		CNesLink * pLink = m_gLinks.AddLink( bWorld, this );
		pLink->SetTarget( pLevel, uPage );

		m_liGround.SetLink( pLink );
		m_gLinks.FreeStaleLinks();
		m_liGround.InitGroundInfo();
		InitObjects();
		return TRUE;
	}

	return FALSE;

}

BOOL CNesLevel::RemoveAreaLink( BYTE bWorld )
{
	if ( m_liGround.IsEnterableObject() )
	{
		m_liGround.RemoveLink( bWorld );
		return TRUE;
	}

	return FALSE;
}

BOOL CNesLevel::IsObjectPresentOnPageRange( BYTE bPageFrom, BYTE bPageTo, BYTE uItemId, BYTE uSpecial, BOOL fSprite )
{
	int x1 = static_cast<int>( bPageFrom ) * 16;
	int x2 = static_cast<int>( bPageTo + 1 ) * 16;

	if ( fSprite )
	{
		auto fnObject = [ x1, x2, uItemId ]( CNesEnemyObject * pObj )
		{
			if ( uItemId == ( pObj->Item().item.objectId & 0x3F ) )
			{
				const POINT pt = pObj->GetRealPoint();
				if ( x1 <= pt.x && pt.x < x2 && pt.y < NES_SPECIAL_OBJECT_E )
				{
					return TRUE;
				}
			}
			return FALSE;
		};

		auto fnObjectSpecial = [ x1, x2, uItemId, uSpecial ]( CNesEnemyObject * pObj )
		{
			if ( uItemId == ( pObj->Item().item.objectId & 0x3F ) )
			{
				const POINT pt = pObj->GetRealPoint();
				if ( x1 <= pt.x && pt.x < x2 && pt.y == uSpecial )
				{
					return TRUE;
				}
			}
			return FALSE;
		};

		auto itObj = m_obj.vpEnemy.end();
		
		if ( !uSpecial )
		{
			itObj = std::find_if( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(), fnObject );
		}
		else
		{
			itObj = std::find_if( m_obj.vpEnemy.begin(), m_obj.vpEnemy.end(), fnObjectSpecial );
		}

		return ( itObj != m_obj.vpEnemy.end() );
	}
	else
	{
		auto fnObj = [ x1, x2, uItemId ]( CNesLevelItem * pObj )
		{
			if ( uItemId == pObj->Item().item.itemId )
			{
				const POINT pt = pObj->GetRealPoint();
				if ( x1 <= pt.x && pt.x < x2 && pt.y < NES_SPECIAL_OBJECT_C )
				{
					return TRUE;
				}
			}
			return FALSE;
		};

		auto fnObjSpecial = [ x1, x2, uItemId, uSpecial ]( CNesLevelItem * pObj )
		{
			if ( uItemId == pObj->Item().item.itemId )
			{
				const POINT pt = pObj->GetRealPoint();
				if ( x1 <= pt.x && pt.x < x2 && pt.y == uSpecial )
				{
					return TRUE;
				}
			}
			return FALSE;
		};

		auto itObj = m_obj.vpItem.end();
		
		if ( !uSpecial )
		{
			itObj = std::find_if( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(), fnObj );
		}
		else
		{
			itObj = std::find_if( m_obj.vpItem.begin() + 1, m_obj.vpItem.end(), fnObjSpecial );
		}

		return ( itObj != m_obj.vpItem.end() );
	}

	return FALSE;
}

const CNesPlayerEntrance CNesLevel::PlayerEntrance()
{
	return m_player;
}

