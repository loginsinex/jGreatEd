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

CNesComplexEnemyObject::CNesComplexEnemyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesEnemyObject( nLevel, item )
{
	SetObjPlace( item.pt.x, max( 0, item.pt.y - 2 ) );
}

CNesComplexEnemyObject::~CNesComplexEnemyObject()
{

}

BOOL CNesComplexEnemyObject::PlaceChanging( int & x, int & y )
{
	if ( y >= NES_SPECIAL_OBJECT_E )
	{
		y = 0x0D;
	}

	return TRUE;
}

BOOL CNesComplexEnemyObject::WidthChanging( int & cx )
{
	return FALSE;
}

BOOL CNesComplexEnemyObject::HeightChanging( int & cy )
{
	return FALSE;
}

VOID CNesComplexEnemyObject::ReinitObject()
{
	FreeSprites();
	InitObject();
}

int CNesComplexEnemyObject::GetRealY()
{
	return GetObjectPoint().y + 2;
}

CNesBowserObject::CNesBowserObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 2 );
	SetObjHeight( 3 );
}

VOID CNesBowserObject::PostInitObject()
{
	SetSprite( 35, 1, 0, 5 );
	SetSprite( 36, 1, 2, 6 );
}

CNesBowserFlameObject::CNesBowserFlameObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 2 );
	SetObjHeight( 1 );
}

VOID CNesBowserFlameObject::PostInitObject()
{
	SetSprite( nustBowserFire1, 2, 0, 4 );
	SetSprite( nustBowserFire2, 2, 2, 4 );
}

CNesFirebarObject::CNesFirebarObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 1 );	
	switch ( ItemId() )
	{
		case 0x1B:
		case 0x1C:
		case 0x1D: 
		case 0x1E: SetObjHeight( 3 ); m_uFiresCount = 6; break;
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22: SetObjHeight( 6 ); m_uFiresCount = 12;  break;
	}

	switch ( ( ItemId() - 0x1B ) & 3 )
	{
		case 0x00: m_bSpriteMark = nustDigitMark00; break;
		case 0x01: m_bSpriteMark = nustDigitMark10; break;
		case 0x02: m_bSpriteMark = nustDigitMark20; break;
		case 0x03: m_bSpriteMark = nustDigitMark40; break;
	}
}

BOOL CNesFirebarObject::HeightChanging( int & cy )
{
	switch ( cy )
	{
		case 3:
			{
				if ( ItemId() >= 0x1f )
				{
					ModifyItemId( ItemId() - 4 );
					m_uFiresCount = 6;
					ReinitObject();
					return TRUE;
				}
				break;
			}
		case 6:
			{
				if ( ItemId() <= 0x1e )
				{
					ModifyItemId( ItemId() + 4 );
					m_uFiresCount = 12;
					ReinitObject();
					return TRUE;
				}
				break;
			}
	}
	return FALSE;
}

VOID CNesFirebarObject::PostInitObject()
{
	for ( BYTE n = 0; n < m_uFiresCount; ++n )
	{
		SetSprite( nustFirebar, 2, 0, 3 + n );
	}

	SetSprite( m_bSpriteMark, 1, -1, 1 );
}

CNesPlatformObject::CNesPlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item, BYTE bObjectWidth, BYTE bPlatformWidth, BYTE bSprite, BYTE bOffset )
	: CNesComplexEnemyObject( nLevel, item ), m_bPlatformWidth( bPlatformWidth ), m_bObjectWidth( bObjectWidth ), m_bSprite( bSprite ),
	m_bMarkSprite( 0xFF ), m_bOffset( bOffset )
{
	SetObjWidth( m_bObjectWidth );
	SetObjHeight( 1 );
}

VOID CNesPlatformObject::SetMarkSprite( BYTE bMarkSprite )
{
	m_bMarkSprite = bMarkSprite;
}

VOID CNesPlatformObject::PostInitObject()
{
	for ( BYTE n = 0; n < m_bPlatformWidth; ++n )
	{
		SetSprite( m_bSprite, 2, n + m_bOffset, 3 );
	}

	if ( 0xFF != m_bMarkSprite )
	{
		SetSprite( m_bMarkSprite, 1, -1, 1 );
	}
}

VOID CNesPlatformObject::SetPlatformSprite( BYTE bSprite )
{
	m_bSprite = bSprite;
}

CNesLargePlatformObject::CNesLargePlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesPlatformObject( 
		nLevel, item, 
		( nLevel.AreaType() == nltCastle ? 2 : 3 ), 
		( nLevel.AreaType() == nltCastle ? 4 : 6 ), 
		( nLevel.Header().IsCloudLevel() ? nustBridge3 : nustBridge1 ),
		0 )
{
	switch ( ItemId() )
	{
		case 0x24: SetMarkSprite( nustDigitMark00 ); break;
		case 0x25: SetMarkSprite( nustDigitMark10 ); break;
		case 0x26: SetMarkSprite( nustDigitMark20 ); break;
		case 0x27: SetMarkSprite( nustDigitMark40 ); break;
		case 0x28: SetMarkSprite( nustDigitMark50 ); break;
		case 0x29: SetMarkSprite( nustDigitMark80 ); break;
	}
}

VOID CNesLargePlatformObject::PostInitObject()
{
	SetPlatformSprite( Level().Header().IsCloudLevel() ? nustBridge3 : nustBridge1 );
	CNesPlatformObject::PostInitObject();
}

CNesSmallPlatformObject::CNesSmallPlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesPlatformObject( nLevel, item, 3, 3, nustBridge2, 1 )
{

}

CNesPowerupObject::CNesPowerupObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

VOID CNesPowerupObject::PostInitObject()
{
	SetSprite( nustPowerup, 2, 0, 3 );
}

CNesVineObject::CNesVineObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

VOID CNesVineObject::PostInitObject()
{
	SetSprite( nustVine, 2, 0, 3 );
}

CNesWarpObject::CNesWarpObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

VOID CNesWarpObject::PostInitObject()
{
	SetSprite( nustWarpMark, 2, 0, 3 );
}

CNesEnemyFrenzyObject::CNesEnemyFrenzyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item )
	: CNesComplexEnemyObject( nLevel, item )
{
	SetObjWidth( 1 );
	SetObjHeight( 1 );
}

VOID CNesEnemyFrenzyObject::PostInitObject()
{
	SetSprite( nustObjFrenzy, 3, 0, 3 );
}