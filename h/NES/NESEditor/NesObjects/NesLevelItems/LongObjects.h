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

class CNesLongObjects : public CNesLevelItem
{
	BOOL				PlaceChanging( int & x, int & y ) override;

protected:
	BYTE				ItemId();
	BYTE				ItemLength();

public:
	CNesLongObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesHorzRow : public CNesLongObjects
{
	NES_METATILE		m_mTile;

	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;

protected:
	VOID				Metatile( const NES_METATILE & tile );
	virtual NES_METATILE GetMetatile() PURE;

public:
	CNesHorzRow( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID				InitObject() override;
};

class CNesVertColumn : public CNesLongObjects
{
	NES_METATILE		m_mTile;

	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;

protected:
	VOID				Metatile( const NES_METATILE & tile );
	virtual NES_METATILE GetMetatile() PURE;

public:
	CNesVertColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID				InitObject() override;
};

class CNesRowOfBricks : public CNesHorzRow
{
	NES_METATILE		GetMetatile() override;

public:
	CNesRowOfBricks( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesRowOfSolid : public CNesHorzRow
{

	NES_METATILE		GetMetatile() override;

public:
	CNesRowOfSolid( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesRowOfCoins : public CNesHorzRow
{
	NES_METATILE		GetMetatile() override;

public:
	CNesRowOfCoins( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesVertBricksColumn : public CNesVertColumn
{
	NES_METATILE		GetMetatile() override;

public:
	CNesVertBricksColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesVertSolidColumn : public CNesVertColumn
{
	NES_METATILE		GetMetatile() override;

public:
	CNesVertSolidColumn( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesVertPipe : public CNesLongObjects
{
	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;
	BYTE				PipeHeight();
	BOOL				IsWarpPipe();

public:
	CNesVertPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	BOOL				IsEntranceObject() override;

	VOID				InitObject() override;
	NES_ENTRANCE_TYPE	EntranceType() override;

};

class CNesUpsidePipe : public CNesLongObjects
{
	int					m_iPipeY;
	const BOOL			m_fInvalidWorld;
	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;
	BOOL				PlaceChanging( int & x, int & y ) override;
	BYTE				PipeHeight();

public:
	CNesUpsidePipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID				InitObject() override;
};

// wind object

class CNesWind : public CNesLevelItem
{
	const BOOL			m_fWindOff;
	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;
	BOOL				PlaceChanging( int & x, int & y ) override;

public:
	CNesWind( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID				InitObject() override;
};
