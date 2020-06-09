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

CSimpleEnemyItem::CSimpleEnemyItem
(
	CNesLevel & nLevel,
	const NES_LEVEL_ENEMY_ITEM & item,
	BYTE uAttribute,
	BYTE uOverrideSprite,
	BYTE bFrenzy, BOOL fRawSprite
)
	:
	CNesEnemyObject( nLevel, item ),
	m_spriteId( ( ( uOverrideSprite == 0xFF ) ? item.item.objectId : uOverrideSprite ) & 0x3F ),
	m_attribute( item.item.objectId < 27 ? Level().ObjData().SpriteAttributes().bAttr[ item.item.objectId ] | uAttribute : uAttribute ),
	m_uIncrementedFrame( 0 ),
	m_uFrenzy( bFrenzy ),
	m_fOverridenSprite( fRawSprite ),
	m_overriddenAttribute( 0xFF )
{
	SetObjPlace( item.pt.x, max( 0, item.pt.y - 1 ) );
	SetMultiplier( 1 );
	SetObjHeight( 1 );
}

CSimpleEnemyItem::~CSimpleEnemyItem()
{

}

int CSimpleEnemyItem::GetRealY()
{
	return GetObjectPoint().y + 1;
}

VOID CSimpleEnemyItem::DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	// do nothing. Only for derived objects
}

BOOL CSimpleEnemyItem::WidthChanging( int & cx )
{
	switch ( cx )
	{
		case 1:
		case 3:
		case 5: return ChangingMultyObjectWidth( cx ); break;
	}
	return FALSE;
}

BOOL CSimpleEnemyItem::HeightChanging( int & cy )
{
	return FALSE;
}

BOOL CSimpleEnemyItem::ChangingMultyObjectWidth( int & cx )
{
	return FALSE;
}

BOOL CSimpleEnemyItem::PlaceChanging( int & x, int & y )
{
	BYTE itemId = ItemId();
	if ( y >= NES_SPECIAL_OBJECT_E )
	{
		y = 0x0D;
	}

	if ( ModifyMultyItem( y, itemId ) )
	{
		ModifyItemId( itemId );
		return TRUE;
	}

	return FALSE;
}

VOID CSimpleEnemyItem::IncFrame()
{
	m_uIncrementedFrame++;
}

BYTE CSimpleEnemyItem::GetSprite( BYTE uSpriteOffset )
{
	const NES_GFX_OFFSETS & ngo = Level().ObjData().GFXEnemy();
	if ( uSpriteOffset < _ARRAYSIZE( ngo.bPtr ) )
	{
		return ngo.bPtr[ uSpriteOffset ] / 6;
	}

	return 0;
}

VOID CSimpleEnemyItem::SetMultiplier( BYTE uMultiplier )
{
	m_uMultiplier = max( 1, uMultiplier );
	SetObjWidth( 1 + 2 * ( uMultiplier - 1 ) );
}

BOOL CSimpleEnemyItem::ModifyMultyItem( int & y, BYTE & itemId )
{
	return TRUE;
}

VOID CSimpleEnemyItem::ReinitObject()
{
	FreeSprites();
	InitObject();
}

VOID CSimpleEnemyItem::PostInitObject()
{
	if ( m_uFrenzy )
	{
		SetSprite( nustFrenzyMark, 0, 1, 0 );
	}

	BYTE bAttribute = ( 0xFF == m_overriddenAttribute ? m_attribute : m_overriddenAttribute );

	for ( BYTE n = 0; n < m_uMultiplier; ++n )
	{
		SetSprite( ( m_fOverridenSprite ? m_spriteId : GetSprite( m_spriteId ) ) + m_uIncrementedFrame, bAttribute, 0 + 3 * n, 2 );
	}
}

BYTE CSimpleEnemyItem::Attribute()
{
	return m_attribute;
}

VOID CSimpleEnemyItem::OverrideAttribute( BYTE bAttribute )
{
	m_overriddenAttribute = bAttribute;
}

BYTE CSimpleEnemyItem::OverrideAttribute()
{
	return m_overriddenAttribute;
}

CNesKoopaTroopaObject::CNesKoopaTroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 1, 0x00 )
{
	switch ( ItemId() )
	{
		case 0x3B: SetMultiplier( 2 ); SetObjPlace( item.pt.x - 3, 0x0a ); break;
		case 0x3C: SetMultiplier( 3 ); SetObjPlace( item.pt.x - 5, 0x0a ); break;
		case 0x3D: SetMultiplier( 2 ); SetObjPlace( item.pt.x - 3, 0x06 ); break;
		case 0x3E: SetMultiplier( 3 ); SetObjPlace( item.pt.x - 5, 0x06 ); break;
	}
}

int CNesKoopaTroopaObject::GetRealX()
{
	const POINT pt = GetObjectPoint();
	switch ( ItemId() )
	{
		case 0x3B: 
		case 0x3D: return pt.x + 3;
		case 0x3C: 
		case 0x3E: return pt.x + 5;
	}

	return pt.x;
}

BOOL CNesKoopaTroopaObject::ModifyMultyItem( int & y, BYTE & bItemId )
{
	const int cx = Width();
	switch ( cx )
	{
		case 1: return TRUE;
		case 3:
		case 5:
			{
				if ( y <= 0x6 ) y = 0x06;
				else y = 0x0a;

				switch ( y )
				{
					case 0x06: bItemId = ( 3 == cx ? 0x3d : 0x3e ); break;
					case 0x0a: bItemId = ( 3 == cx ? 0x3b : 0x3c ); break;
					default: return FALSE;
				}
				return TRUE;
			}
	}
	return FALSE;
}

BOOL CNesKoopaTroopaObject::ChangingMultyObjectWidth( int & cx )
{
	switch ( cx )
	{
		case 1: ModifyItemId( 0 ); SetMultiplier( 1 ); ReinitObject(); return TRUE;
		case 3:
		case 5:
			{
				int y = GetObjectPoint().y;
				if ( y <= 0x6 ) y = 0x06;
				else y = 0x0a;

				SetObjPlace( GetObjectPoint().x, y );
				SetMultiplier( ( cx - 1 ) / 2 + 1 );

				switch ( y )
				{
					case 0x06: ModifyItemId( 3 == cx ? 0x3d : 0x3e ); break;
					case 0x0a: ModifyItemId( 3 == cx ? 0x3b : 0x3c ); break;
				}

				ReinitObject();
				return TRUE;
			}
	}

	return FALSE;
}

CNesRedKoopaTroopaObject::CNesRedKoopaTroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesBuzzyBeetleObject::CNesBuzzyBeetleObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesHummerBro::CNesHummerBro( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesGoombaObject::CNesGoombaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 3, 0x06 )
{
	switch ( ItemId() )
	{
		case 0x37: SetMultiplier( 2 ); SetObjPlace( item.pt.x - 3, 0x0a ); break;	// move object cause to its draw from right to left
		case 0x38: SetMultiplier( 3 ); SetObjPlace( item.pt.x - 5, 0x0a ); break;	// this point will be reverted back when saving
		case 0x39: SetMultiplier( 2 ); SetObjPlace( item.pt.x - 3, 0x06 ); break;
		case 0x3A: SetMultiplier( 3 ); SetObjPlace( item.pt.x - 5, 0x06 ); break;
	}
}

int CNesGoombaObject::GetRealX()
{
	const POINT pt = GetObjectPoint();
	
	// revert back valid X point, see above constructor
	switch ( ItemId() )
	{
		case 0x37: 
		case 0x39: return pt.x + 3;
		case 0x38: 
		case 0x3A: return pt.x + 5;
	}

	return pt.x;
}

BOOL CNesGoombaObject::ModifyMultyItem( int & y, BYTE & bItemId )
{
	const int cx = Width();
	switch ( cx )
	{
		case 1: return TRUE;
		case 3:
		case 5:
			{				
				if ( y <= 0x6 )
				{
					y = 0x06;
				}
				else
				{
					y = 0x0a;
				}

				switch ( y )
				{
					case 0x06: bItemId = ( 3 == cx ? 0x39 : 0x3a ); break;
					case 0x0a: bItemId = ( 3 == cx ? 0x37 : 0x38 ); break;
					default: return FALSE;
				}
				return TRUE;
			}
	}
	return FALSE;
}

BOOL CNesGoombaObject::ChangingMultyObjectWidth( int & cx )
{
	switch ( cx )
	{
		case 1: ModifyItemId( 6 ); SetMultiplier( 1 ); ReinitObject(); return TRUE;
		case 3:
		case 5:
			{
				int y = GetObjectPoint().y;
				if ( y <= 0x6 ) y = 0x06;
				else y = 0x0a;

				SetObjPlace( GetObjectPoint().x, y );
				SetMultiplier( ( cx - 1 ) / 2 + 1 );

				switch ( y )
				{
					case 0x06: ModifyItemId( 3 == cx ? 0x39 : 0x3a ); break;
					case 0x0a: ModifyItemId( 3 == cx ? 0x37 : 0x38 ); break;
				}

				ReinitObject();
				return TRUE;
			}
	}

	return FALSE;
}

CNesBlooberObject::CNesBlooberObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{
	IncFrame();
}

CNesBulletBillObject::CNesBulletBillObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0, 0xFF, ( 0x16 == ( 0x3f & item.item.objectId ) ) )
{

}

CNesKoopaParatroopaObject::CNesKoopaParatroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 ), m_bSpriteMark( 0 )
{
	switch ( ItemId() )
	{
		case 0x09: m_bSpriteMark = nustDigitMark00; break;
		case 0x0e: m_bSpriteMark = nustDigitMark10; break;
		case 0x10: m_bSpriteMark = nustDigitMark20; break;
	}
}

VOID CNesKoopaParatroopaObject::PostInitObject()
{
	CSimpleEnemyItem::PostInitObject();

	if ( m_bSpriteMark )
	{
		SetSprite( m_bSpriteMark, 1, -1, 1 );
	}
}

CNesCheepCheepObject::CNesCheepCheepObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0, 0xFF, ( 0x14 == ( 0x3f & item.item.objectId ) ) )
{

}

CNesRedCheepCheepObject::CNesRedCheepCheepObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesPodobooObject::CNesPodobooObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesPirahnaPlantObject::CNesPirahnaPlantObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, ( 0x04 == item.item.objectId ? 0x80 : 0 ) )
{

}

VOID CNesPirahnaPlantObject::DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject )
{
	const NES_RED_PIRAHNA_DATA & redPirahnaData = Level().ObjData().RedPirahnaData();

	BYTE bAttribute = redPirahnaData.bAttr | ( Attribute() & 0xF8 );
	if ( bWorldId < redPirahnaData.bWorld && 0x04 != Item().item.objectId )
	{
		bAttribute--;
	}

	if ( OverrideAttribute() != bAttribute )
	{
		OverrideAttribute( bAttribute );
		ReinitObject();
	}
}

CNesRedKoopaParatroopaObject::CNesRedKoopaParatroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesLakituObject::CNesLakituObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0 )
{

}

CNesSpinyObject::CNesSpinyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 0, 0xFF, 1 )
{

}

CNesRetainerObject::CNesRetainerObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 2, 27, 0, TRUE )
{
	SetObjPlace( item.pt.x, 10 );
}

BOOL CNesRetainerObject::PlaceChanging( int & x, int & y )
{
	y = 10;
	return TRUE;
}

CNesEOFFrenzy::CNesEOFFrenzy( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CSimpleEnemyItem( nLevel, item, 2, nustFrenzyMark, 0, TRUE )
{

}