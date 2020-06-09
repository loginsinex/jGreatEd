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

#include "h\NES\NESEditor\NesObjects\CNesEnemyObject.h"

class CSimpleEnemyItem : public CNesEnemyObject
{
	const BOOL		m_fOverridenSprite;
	const BYTE		m_spriteId;
	const BYTE		m_attribute;
	BYTE			m_overriddenAttribute;
	BYTE			m_uIncrementedFrame;
	BYTE			m_uMultiplier;
	const BYTE		m_uFrenzy;


protected:
	virtual	BOOL	WidthChanging( int & cx ) override;
	virtual	BOOL	HeightChanging( int & cy ) override;
	virtual BOOL	PlaceChanging( int & x, int & y ) override;
	virtual BOOL	ChangingMultyObjectWidth( int & cx );
	virtual int		GetRealY() override;
	virtual VOID	DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject ) override;
	BYTE			GetSprite( BYTE uSpriteOffset );
	VOID			SetMultiplier( BYTE uMultiplier );
	virtual BOOL	ModifyMultyItem( int & y, BYTE & itemId );
	VOID			ReinitObject();
	VOID			OverrideAttribute( BYTE bAttribute );
	BYTE			OverrideAttribute();
	BYTE			Attribute();
	
	VOID			IncFrame();

public:
	CSimpleEnemyItem( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item, BYTE uAttribute, BYTE uOverrideSprite = 0xFF, BYTE bFrenzy = 0, BOOL fRawSprite = FALSE );
	virtual ~CSimpleEnemyItem();
	VOID			PostInitObject() override;
};

class CNesKoopaTroopaObject : public CSimpleEnemyItem
{
	BOOL			ChangingMultyObjectWidth( int & cx ) override;
	BOOL			ModifyMultyItem( int & y, BYTE & bItemId ) override;

public:
	CNesKoopaTroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	int				GetRealX() override;
};

class CNesRedKoopaTroopaObject : public CSimpleEnemyItem
{

public:
	CNesRedKoopaTroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesBuzzyBeetleObject : public CSimpleEnemyItem
{
public:
	CNesBuzzyBeetleObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesHummerBro : public CSimpleEnemyItem
{
public:
	CNesHummerBro( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesGoombaObject : public CSimpleEnemyItem
{
	BOOL			ChangingMultyObjectWidth( int & cx ) override;
	BOOL			ModifyMultyItem( int & y, BYTE & bItemId ) override;

public:
	CNesGoombaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	int				GetRealX() override;
};

class CNesBlooberObject : public CSimpleEnemyItem
{
public:
	CNesBlooberObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesBulletBillObject : public CSimpleEnemyItem
{
public:
	CNesBulletBillObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesKoopaParatroopaObject : public CSimpleEnemyItem
{
	BYTE				m_bSpriteMark;

public:
	CNesKoopaParatroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesCheepCheepObject : public CSimpleEnemyItem
{
public:
	CNesCheepCheepObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesRedCheepCheepObject : public CSimpleEnemyItem
{
public:
	CNesRedCheepCheepObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesPodobooObject : public CSimpleEnemyItem
{
public:
	CNesPodobooObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesPirahnaPlantObject : public CSimpleEnemyItem
{
	VOID	DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject ) override;

public:
	CNesPirahnaPlantObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesRedKoopaParatroopaObject : public CSimpleEnemyItem
{
public:
	CNesRedKoopaParatroopaObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesLakituObject : public CSimpleEnemyItem
{
public:
	CNesLakituObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesSpinyObject : public CSimpleEnemyItem
{
public:
	CNesSpinyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesRetainerObject : public CSimpleEnemyItem
{
	BOOL	PlaceChanging( int & x, int & y ) override;

public:
	CNesRetainerObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesEOFFrenzy : public CSimpleEnemyItem
{
public:
	CNesEOFFrenzy( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};