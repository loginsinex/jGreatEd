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

#define ASSERT_EOF(it,end)	{ if ( it >= end ) throw std::exception("Parse data failed"); }


class CNesEditor;
class CNesLevel;

typedef std::map<NES_GROUND_TYPE, NES_GROUND_INFO>		NES_SNAPSHOT_GROUND, *PNES_SNAPSHOT_GROUND;

typedef struct _tagNES_SNAPSHOT_ITEM
{
	BOOL																	fEntrance;
	BOOL																	fFakeItem;	// for grounds
	NES_LEVEL_ITEM															item;
	std::map<NES_AREA_SET, std::map<BYTE, std::pair<UINT, CNesLevel*>>>		mLinks;	// [areaset][world][page][pLevel]
} NES_SNAPSHOT_ITEM, *PNES_SNAPSHOT_ITEM;

typedef NES_LEVEL_ENEMY_ITEM	NES_SNAPSHOT_ENEMY, *PNES_SNAPSHOT_ENEMY;

typedef struct _tagNES_OBJECTS
{
	std::vector<CNesEnemyObject*>			vpEnemy;
	std::vector<CNesLevelItem*>				vpItem;
} NES_OBJECTS, *PNES_OBJECTS;

typedef struct _tagNES_LEVEL_METADATA
{
	BYTE		uPointerFileId[ 2 ];
	BYTE		uDataFileId[ 2 ];
	size_t		uLevelId;
} NES_LEVEL_METADATA, *PNES_LEVEL_METADATA;


class CNesLevel
{
	CNESFile &							m_file;
	const CNesPointers &				m_eptr;
	NES_AREA_PTR						m_lvlPtr;
	std::map<NES_AREA_SET, NES_AREA_PTR>	m_mLvlOptionalPtr;
	NES_LEVEL_METADATA					m_lvlMeta;
	const CNesObjectData &				m_objData;
	const CNesPlayerEntrance &			m_player;
	std::vector<BYTE>					m_vBankRef;
	BOOL								m_fLevelIsShared;
	BYTE								m_uWarpData;
	CNesLevelHeader						m_header;
	CNesGameLinks &						m_gLinks;
	std::vector<NES_LEVEL_ITEM>			m_lvl;
	std::vector<NES_LEVEL_ENEMY_ITEM>	m_enm;
	std::vector<CNesLink*>				m_vSrcLink;

	CNesLevelItemGroundType				m_liGround;
	NES_OBJECTS							m_obj;

	int									m_iLevelStatus;
	LONG								m_uPages;
	CNesMetatiles &						m_mTiles;

	VOID								UpdateLevelStatus();
	VOID								FreeObjects();

	VOID								ValidateLevelPalette( CNesLevelView & view );

	CNesLevelItem *						FetchMasvItemC( const NES_LEVEL_ITEM & item );
	CNesLevelItem *						FetchMasvItemD( const NES_LEVEL_ITEM & item );
	CNesLevelItem *						FetchMasvItemF( const NES_LEVEL_ITEM & item );
	CNesLevelItem *						FetchLongItem( const NES_LEVEL_ITEM & item );
	CNesLevelItem *						FetchSingleItem( const NES_LEVEL_ITEM & item );
	CNesLevelItem *						FetchLevelItem( const NES_LEVEL_ITEM & item );
	CNesEnemyObject *					FetchEnemyItem( const NES_LEVEL_ENEMY_ITEM & item );	
	LONG								ParseLevelData( std::vector<BYTE>::const_iterator areaIt, std::vector<BYTE>::const_iterator areaEnd );
	VOID								ParseGroundData( NES_GROUND_INFO & ngi, const NES_LEVEL_ITEM & item, LONG uPage );
	LONG								ParseEnemyData( std::vector<BYTE>::const_iterator atBegin, std::vector<BYTE>::const_iterator atEnd, std::vector<NES_LINK> & vLinks );
	size_t								SerializeLevelData( std::vector<BYTE> & vLevelData );
	size_t								SerializeEnemyData( std::vector<BYTE> & vEnemyData );
	VOID								SplitItemsToPages( std::map<BYTE, std::vector<NES_LEVEL_ITEM>> & mPage, std::vector<NES_LEVEL_ITEM> & vItem );
	VOID								SplitEnemiesToPages( std::map<BYTE, std::vector<NES_LEVEL_ENEMY_ITEM>> & mPage, std::vector<NES_LEVEL_ENEMY_ITEM> & vItem );
	VOID								AddItemToDump( std::vector<BYTE> & vDump, const NES_LEVEL_ITEM & item );
	VOID								AddEnemyToDump( std::vector<BYTE> & vDump, const NES_LEVEL_ENEMY_ITEM & item );
	VOID								DumpItems( std::vector<BYTE> & vDump, const std::vector<NES_LEVEL_ITEM> & vItem );
	VOID								DumpEnemies( std::vector<BYTE> & vDump, const std::vector<NES_LEVEL_ENEMY_ITEM> & vItem );
	VOID								DumpPointersAsObjects( std::vector<NES_LEVEL_ENEMY_ITEM> & vItem );
	VOID								DumpLoops( std::map<BYTE, std::vector<CNesLoopCommand*>> & mLoop );
	VOID								LevelSizeChanged();

	VOID								RemoveDuplicatePointers( const std::vector<NES_LEVEL_ENEMY_ITEM> & v, std::vector<NES_LEVEL_ENEMY_ITEM> & tail );

	BOOL								AddPage( UINT uBeforePage );
	BOOL								DelPage( UINT uPage );

	VOID								TakePointerSnapshot( CNesLevelItem * pItem, NES_SNAPSHOT_ITEM & item );
	VOID								TakeLevelItemsSnapshot( std::vector<NES_SNAPSHOT_ITEM>& vItems );
	VOID								TakeEnemySnapshot( std::vector<NES_SNAPSHOT_ENEMY>& vItems );
	VOID								RevertPointerSnapshot( CNesLevelItem * pItem, const NES_SNAPSHOT_ITEM & item );
	VOID								RevertLevelItemsSnapshot( const std::vector<NES_SNAPSHOT_ITEM>& vItems );
	VOID								RevertEnemySnapshot( const std::vector<NES_SNAPSHOT_ENEMY>& vItems );

	VOID								InitDrawQueue( std::vector<NES_DRAW_QUEUE_ITEM> & vDrawQueue );
	VOID								SetupBehindFlag( std::vector<NES_DRAW_QUEUE_ITEM> & vdqi, BOOL fBehind );

	friend CNesEditor;

public:
	CNesLevel( 
		CNESFile & file, 
		NES_AREA_PTR uPtr, 
		const CNesPointers & eptr, 
		CNesMetatiles & mtiles, 
		const CNesObjectData & objData,
		CNesGameLinks & gLinks,
		const CNesPlayerEntrance & plr );

	~CNesLevel();

	CNESFile	&	ROM();
	const CNesPointers	&	Pointers();
	const CNesObjectData &	ObjData();

	int				LoadLevelData(
		const std::vector<BYTE> & vAreaData,
		const std::vector<BYTE> & vEnemyData,
		std::vector<NES_LINK> & vLinks
	);
	
	int				GetLevelStatus();

	NES_AREA_PTR	AreaPtr();
	const CNesLevelHeader & Header();
	BOOL			UpdateHeader( const CNesLevelHeader & header );

	NES_LEVEL_TYPE	AreaType();
	SIZE			LevelSize();
	BYTE			PagesCount();

	VOID			InitView( CNesLevelView & view );
	VOID			InitObjectsQueue();

	VOID			DrawLevel( CNesLevelView & view, BYTE bWorldId );
	VOID			InitObjects();
	BYTE			GetBgColor();
	BYTE			LevelBgCtl();

	CNesObject *	ObjectFromPoint( int x, int y );
	BOOL			ObjectsFromRect( const RECT & rc, std::vector<CNesObject*> & vpObj );
	VOID			ResortObjects();

	BOOL			MoveGround( int uOldX, int uNewX );
	BOOL			GetGround( int X, std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & gr );
	VOID			SetGround( int x, const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & gr );
	BOOL			EraseGround( int x );

	int				AssignLink( CNesLink * pLink, int uBeginPoint );
	int				AssignGroundLink( CNesLink * pLink );

	BOOL			GetLevelBinaryData( std::vector<BYTE> & vLevelData, std::vector<BYTE> & vEnemyData );

	// WARNING: Dont use this methods to update area's id or type
	// this will be overwritten in DumpAreas
	// instead of this use CNesEditor::UpdateAreaType !!!
	VOID			UpdateAreaType( NES_LEVEL_TYPE bType );
	VOID			SetAreaId( BYTE bId );
	VOID			SetOptionalAreaId( NES_AREA_SET nas, BYTE bId );
	BYTE			GetOptionalAreaId( NES_AREA_SET nas );
	NES_AREA_SET	World2Areaset( BYTE bWorld );

	CNesLevel *		Copy();

	CNesLevelItem *		AddLevelItem( const NES_LEVEL_ITEM & item );
	CNesEnemyObject *	AddEnemyItem( const NES_LEVEL_ENEMY_ITEM & item );
	BOOL				IsValidObject( CNesObject * pObject );
	BOOL				DeleteObject( CNesObject * pObject );
	CNesObject *		ObjectById( size_t idObject, BOOL fSprite );
	BOOL				ObjectId( CNesObject * pObject, size_t & idObject, BOOL & fSprite );

	BOOL				GetAreaPointer( BYTE bWorld, CNesLevel ** pLevel, BYTE & uPage );
	BOOL				SetAreaPointer( BYTE bWorld, CNesLevel * pLevel, BYTE uPage );
	BOOL				RemoveAreaLink( BYTE bWorld );

	BOOL				IsObjectPresentOnPageRange( BYTE bPageFrom, BYTE bPageTo, BYTE uItemId, BYTE uSpecial = 0, BOOL fSprite = FALSE );
	const CNesPlayerEntrance PlayerEntrance();

	VOID				AddBankRef( BYTE bBankId );
	VOID				MarkLevelAsShared();
	BOOL				IsSharedLevel();

	const NES_LEVEL_METADATA	Metadata() const;
	VOID				Metadata( const NES_LEVEL_METADATA & md );
	VOID				GetLevelLinks( NES_AREA_SET areaSet, std::vector<CNesLevel*> & vpLinks );

	VOID				LoadLoops( const std::vector<NES_LOOP> & vLoop );
	VOID				DumpLoops( std::vector<NES_LOOP> & vLoop );

	BOOL				FindLoopObjectId( CNesLoopCommand * pObject, size_t & idObject );
	VOID				ReadAreaLinks( std::map<BYTE, std::pair<UINT, CNesLevel*>> & mLinks );
	VOID				ReadItemLinks( CNesLevelItem * pItem, std::map<BYTE, std::pair<UINT, CNesLevel*>> & mLinks );

	VOID				TakeLevelSnapshot(
		NES_LEVEL_HEADER & header,
		std::vector<NES_GROUND_PLACE> & vGround,
		std::vector<NES_SNAPSHOT_ITEM> & vItems,
		std::vector<NES_SNAPSHOT_ENEMY> & vEnemy,
		std::vector<NES_LOOP> & vLoop,
		UINT & uPages,
		NES_LEVEL_TYPE & type
	);

	VOID				RevertSnapshot(
		const NES_LEVEL_HEADER & header,
		const std::vector<NES_GROUND_PLACE> & vGround,
		const std::vector<NES_SNAPSHOT_ITEM> & vItems,
		const std::vector<NES_SNAPSHOT_ENEMY> & vEnemy,
		const std::vector<NES_LOOP> & vLoop,
		const UINT uPages,
		const NES_LEVEL_TYPE type
	);
};