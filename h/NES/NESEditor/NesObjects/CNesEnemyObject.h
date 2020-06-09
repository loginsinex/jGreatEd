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

#include "CNesObject.h"
#include "CNesObjectData.h"

class CNesEnemyObject: public CNesObject
{
	NES_LEVEL_ENEMY_ITEM			m_item;


protected:
	BYTE							ItemId();
	VOID							ModifyItemId( BYTE itemId );
	virtual VOID					PostInitObject();

public:
	CNesEnemyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	CNesEnemyObject( CNesLevel & nLevel );
	VOID							InitObject();
	const NES_LEVEL_ENEMY_ITEM &	Item() const;
	BOOL							IsHardObject() const;
	VOID							SetHardObject( BOOL fHardMode );
	virtual ~CNesEnemyObject();
};