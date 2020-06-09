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

class CNesComplexEnemyObject : public CNesEnemyObject
{
	virtual BOOL		WidthChanging( int & cx ) override;
	virtual BOOL		HeightChanging( int & cy ) override;

protected:
	VOID				ReinitObject();
	virtual int			GetRealY();

public:
	CNesComplexEnemyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	BOOL	PlaceChanging( int & x, int & y ) override;
	virtual ~CNesComplexEnemyObject();
};

class CNesBowserObject : public CNesComplexEnemyObject
{

public:
	CNesBowserObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesBowserFlameObject : public CNesComplexEnemyObject
{
public:
	CNesBowserFlameObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesFirebarObject : public CNesComplexEnemyObject
{
	BYTE				m_uFiresCount;
	BYTE				m_bSpriteMark;
	BOOL				HeightChanging( int &cy ) override;

public:
	CNesFirebarObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesPlatformObject : public CNesComplexEnemyObject
{
	const BYTE				m_bPlatformWidth;
	const BYTE				m_bObjectWidth;
	BYTE					m_bSprite;
	const BYTE				m_bOffset;

	BYTE					m_bMarkSprite;

protected:
	VOID				SetMarkSprite( BYTE bMarkSprite );
	VOID				SetPlatformSprite( BYTE bSprite );

public:
	CNesPlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item, BYTE bObjectWidth, BYTE bPlatformWidth, BYTE bSprite, BYTE bOffset );
	VOID				PostInitObject() override;
};

class CNesLargePlatformObject : public CNesPlatformObject
{

	VOID				PostInitObject() override;

public:
	CNesLargePlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesSmallPlatformObject : public CNesPlatformObject
{
public:
	CNesSmallPlatformObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
};

class CNesPowerupObject : public CNesComplexEnemyObject
{

public:
	CNesPowerupObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesVineObject : public CNesComplexEnemyObject
{

public:
	CNesVineObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesWarpObject : public CNesComplexEnemyObject
{

public:
	CNesWarpObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};

class CNesEnemyFrenzyObject : public CNesComplexEnemyObject
{

public:
	CNesEnemyFrenzyObject( CNesLevel & nLevel, const NES_LEVEL_ENEMY_ITEM & item );
	VOID				PostInitObject() override;
};