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

#include "h\NES\NESEditor\NesObjects\CNesLevelItem.h"

class CNesMassiveObjects: public CNesLevelItem
{

protected:
	BYTE				ItemId();
	BYTE				ItemSize();
	VOID				UpdateItemSize( BYTE lSize );

public:
	CNesMassiveObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesMassiveRow: public CNesMassiveObjects
{
	const int	m_y, m_height;
	BOOL		PlaceChanging( int & x, int & y ) override;
	BOOL		WidthChanging( int & cx ) override;
	BOOL		HeightChanging( int & cy ) override;

public:
	CNesMassiveRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y, int height );
};

class CNesHole: public CNesMassiveRow
{

public:
	CNesHole( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesBridge: public CNesMassiveRow
{

public:
	CNesBridge( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y );
	VOID		InitObject() override;
};

class CNesWater: public CNesMassiveRow
{
public:
	CNesWater( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesBridgeHigh: public CNesBridge
{
public:
	CNesBridgeHigh( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesBridgeMiddle: public CNesBridge
{
public:
	CNesBridgeMiddle( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesBridgeLow: public CNesBridge
{
public:
	CNesBridgeLow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesQBlockRow: public CNesMassiveRow
{
public:
	CNesQBlockRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int y );
	VOID		InitObject() override;
};

class CNesQBlocksRowHigh: public CNesQBlockRow
{
public:
	CNesQBlocksRowHigh( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesQBlocksRowLow: public CNesQBlockRow
{
public:
	CNesQBlocksRowLow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesPulleyRopeObject: public CNesMassiveRow
{
public:
	CNesPulleyRopeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID		InitObject() override;
};

// rope for bridges

class CNesRope: public CNesLevelItem
{
	BOOL		PlaceChanging( int & x, int & y ) override;
	BOOL		WidthChanging( int & cx ) override;
	BOOL		HeightChanging( int & cy ) override;

public:
	CNesRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;

};

class CNesVertMassiveObject : public CNesLevelItem
{
	const int	m_bFromY;
	const int	m_bMinHeight;

	BOOL		ItemHeight( int & cy );
	BOOL		HeightChanging( int & cy ) override;
	BOOL		WidthChanging( int & cx ) override;
	BOOL		PlaceChanging( int & x, int &y ) override;

protected:
	BYTE		ItemHeight();

public:
	CNesVertMassiveObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, int bFromY, int bMinHeight );
};

class CNesBalanceRope : public CNesVertMassiveObject
{

public:
	CNesBalanceRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesReversePipe : public CNesVertMassiveObject
{

public:
	CNesReversePipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID				InitObject() override;
	BOOL				IsEntranceObject() override;
	NES_ENTRANCE_TYPE	EntranceType() override;
};

class CNesUnusedRope : public CNesVertMassiveObject
{

public:
	CNesUnusedRope( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesCastle: public CNesLevelItem
{
	BOOL		PlaceChanging( int & x, int & y ) override;
	BOOL		WidthChanging( int & cx ) override;
	BOOL		HeightChanging( int & cy ) override;
	BYTE		ItemY();
	VOID		ItemY( BYTE cy );

public:
	CNesCastle( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesStaircase: public CNesLevelItem
{
	BOOL		PlaceChanging( int & x, int & y ) override;
	BOOL		WidthChanging( int & cx ) override;
	BOOL		HeightChanging( int & cy ) override;
	BYTE		ItemWidth();
	VOID		ItemWidth( BYTE cx );

public:
	CNesStaircase( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};

class CNesAreaStyleLedge : public CNesMassiveObjects
{

	VOID			InitTreeLedge();
	VOID			InitMushroom();
	VOID			InitBulletBillCannon();
	
	BOOL			TreeLedge_PlaceChanging( int & x, int & y );
	BOOL			Mushroom_PlaceChanging( int & x, int & y );
	BOOL			Cannon_PlaceChanging( int & x, int & y );
	
	BOOL			TreeLedge_WidthChanging( int & cx );
	BOOL			Mushroom_WidthChanging( int & cx );
	BOOL			Cannon_WidthChanging( int & cx );

	BOOL			TreeLedge_HeightChanging( int & cy );
	BOOL			Mushroom_HeightChanging( int & cy );
	BOOL			Cannon_HeightChanging( int & cy );

	VOID			SetObjByArea();

	BOOL			PlaceChanging( int & x, int & y ) override;
	BOOL			WidthChanging( int & cx ) override;
	BOOL			HeightChanging( int &cy ) override;

	VOID			ReinitObject();

public:
	CNesAreaStyleLedge( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID		InitObject() override;
};