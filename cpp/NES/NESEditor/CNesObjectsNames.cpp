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
#include "h/NES/NESEditor/CNesObjectsNames.h"
#include <resource.h>

CNesObjectsNames::CNesObjectsNames( HINSTANCE hInstance )
	: m_hInstance( hInstance )
{
	
}

HINSTANCE CNesObjectsNames::GetInstance()
{
	return m_hInstance;
}

CString CNesObjectsNames::GetNameOfItem( CNesLevelItem * pObj )
{
	UINT uStringId = IDS_UNKNOWN;
	const int Y = pObj->GetRealPoint().y;
	const BYTE itemId = pObj->Item().item.itemId & 0x7F;
	const BYTE objId = ( ( pObj->Item().item.itemId & 0x70 ) >> 4 );
	CString str;

	switch ( Y )
	{
		case NES_SPECIAL_OBJECT_F: uStringId = objId + IDS_ITEM_F_LIST + 1; break;
		case NES_SPECIAL_OBJECT_D: uStringId = itemId + IDS_ITEM_D_LIST + 1 - 0x40; break;
		case NES_SPECIAL_OBJECT_C: uStringId = objId + IDS_ITEM_C_LIST + 1; break;
		default:
			{
				if ( Y >= 0 && Y < NES_SPECIAL_OBJECT_C )
				{
					if ( itemId < 0x10 )
					{
						uStringId = itemId + IDS_ITEM_LIST + 1;
					}
					else
					{
						uStringId = ( itemId & 0x70 ) + IDS_ITEM_LIST + 1;
					}
				}
				break;
			}
	}

	str = m_str.GetString( GetInstance(), uStringId ).c_str();
	if ( !str.GetLength() )
	{
		uStringId = IDS_UNKNOWN;
	}

	if ( IDS_UNKNOWN == uStringId )
	{
		str.Format( TEXT( "0x%02X" ), itemId );
	}
	/*
	// debug purposes
	else
	{
		CString s1;
		s1.Format( TEXT( "%s [0x%02x]" ), str, itemId );
		str = s1;
	}
	*/
	return str;
}

CString CNesObjectsNames::GetNameOfEnemy( CNesEnemyObject * pObj )
{
	const BYTE itemId = ( pObj->Item().item.objectId & 0x3F );
	UINT uStringId = IDS_ENEMY_LIST + itemId + 1;
	CString str = m_str.GetString( GetInstance(), uStringId ).c_str();

	if ( !str.GetLength() )
	{
		str.Format( TEXT( "0x%02X" ), itemId );
	}

	return str;
}