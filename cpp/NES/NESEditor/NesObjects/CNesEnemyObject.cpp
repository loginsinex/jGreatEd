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

CNesEnemyObject::CNesEnemyObject( CNesLevel & nLevel )
	: CNesObject( nLevel, TRUE )
{

}

BOOL CNesEnemyObject::IsHardObject() const
{
	return ( ( m_item.item.objectId & 0x40 ) > 0 );
}

VOID CNesEnemyObject::SetHardObject( BOOL fHardMode )
{
	if ( fHardMode )
	{
		m_item.item.objectId |= 0x40;
	}
	else
	{
		m_item.item.objectId &= ~0x40;
	}

	FreeSprites();
	InitObject();
}

CNesEnemyObject::CNesEnemyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesObject( nLevel, TRUE ), m_item( item )
{

}

VOID CNesEnemyObject::InitObject()
{
	if ( IsHardObject() )
	{
		SetSprite( nustHardMark, 2, 1, 0 );
	}

	PostInitObject();
}

VOID CNesEnemyObject::PostInitObject()
{
	// for derived classes
}

const NES_LEVEL_ENEMY_ITEM & CNesEnemyObject::Item() const
{
	return m_item;
}

BYTE CNesEnemyObject::ItemId()
{
	return ( m_item.item.objectId & 0x3f );
}

VOID CNesEnemyObject::ModifyItemId( BYTE itemId )
{
	BYTE bHardItem = m_item.item.objectId & 0x40;
	m_item.item.objectId = ( itemId & 0x3f ) | bHardItem;
}

CNesEnemyObject::~CNesEnemyObject()
{

}