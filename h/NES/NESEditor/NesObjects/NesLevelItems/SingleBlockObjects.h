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

class CNesSingleBlockObjects : public CNesLevelItem
{
	BOOL				PlaceChanging( int & x, int & y );
	BOOL				WidthChanging( int & cx ) override;
	BOOL				HeightChanging( int & cy ) override;

public:
	CNesSingleBlockObjects( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesQBlock : public CNesSingleBlockObjects
{

public:
	CNesQBlock( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID			InitObject() override;
	BOOL			IsEntranceObject() override;
	NES_ENTRANCE_TYPE	EntranceType() override;
};

class CNesWaterPipe : public CNesSingleBlockObjects
{

public:
	CNesWaterPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID			InitObject() override;
	BOOL			IsEntranceObject() override;
	NES_ENTRANCE_TYPE	EntranceType() override;
};

class CNesJumpspring : public CNesSingleBlockObjects
{
	DWORD			m_uGreenFlag;
	const NES_GREEN_JUMPSPRING_WORLDS	m_gjsData;

public:
	VOID			DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject ) override;

	CNesJumpspring( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
	VOID			InitObject() override;
};

class CNesIntroPipe: public CNesLevelItem
{
	BOOL			PlaceChanging( int & x, int & y ) override;
	BOOL			HeightChanging( int & cy ) override;
	BOOL			WidthChanging( int & cx ) override;

public:
	CNesIntroPipe( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID			InitObject() override;
	BOOL			IsEntranceObject() override;
	NES_ENTRANCE_TYPE	EntranceType() override;
};

class CNesFlagpoleObject : public CNesLevelItem
{
	BOOL			PlaceChanging( int & x, int & y ) override;
	BOOL			HeightChanging( int & cy ) override;
	BOOL			WidthChanging( int & cx ) override;

public:
	CNesFlagpoleObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID			InitObject() override;
};

class CNesSpecObj: public CNesLevelItem
{
	const int				m_y;
	const NES_METATILE		m_mTile;
	const BYTE				m_len;
	BOOL			PlaceChanging( int & x, int & y ) override;
	BOOL			HeightChanging( int & cy ) override;
	BOOL			WidthChanging( int & cx ) override;

public:
	CNesSpecObj( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, BYTE bItemId, BYTE length );

	VOID			InitObject() override;
};

class CNesAxeObject: public CNesSpecObj
{

public:
	CNesAxeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesChainObject: public CNesSpecObj
{
public:
	CNesChainObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

class CNesCastleBridgeObject: public CNesSpecObj
{

public:
	CNesCastleBridgeObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// command objects

class CNesCommandObject: public CNesLevelItem
{
	const NES_METATILE			m_tile;
	BOOL			PlaceChanging( int & x, int & y ) override;
	BOOL			HeightChanging( int & cy ) override;
	BOOL			WidthChanging( int & cx ) override;

public:
	CNesCommandObject( CNesLevel & nLevel, const NES_LEVEL_ITEM & item, const NES_METATILE & tile );

	VOID			InitObject() override;
};

// warp scroll stop

class CNesWarpScrollStop: public CNesCommandObject
{

public:
	CNesWarpScrollStop( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// simple scroll stop
class CNesScrollStop: public CNesCommandObject
{

public:
	CNesScrollStop( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// frenzy 1
class CNesFrenzyObject1: public CNesCommandObject
{

public:
	CNesFrenzyObject1( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// frenzy 2
class CNesFrenzyObject2: public CNesCommandObject
{

public:
	CNesFrenzyObject2( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// frenzy 3
class CNesFrenzyObject3: public CNesCommandObject
{

public:
	CNesFrenzyObject3( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );
};

// loop command
class CNesLoopCommand: public CNesCommandObject
{
	NES_LOOP			m_lpc;
	BOOL				m_fSlaveLoop;

	BOOL				PlaceChanging( int & x, int & y ) override;
	VOID				DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject ) override;

public:
	CNesLoopCommand( CNesLevel & nLevel, const NES_LEVEL_ITEM & item );

	VOID				LoadLoop( const NES_LOOP & lpc );
	VOID				DumpLoop( NES_LOOP & lpc );
	BOOL				LoopConnected();
	VOID				LoopConnected( BOOL fSlaveLoop );
	BYTE				TriggerPage();
	VOID				TriggerPage( BYTE bPage );
	VOID				RewindOffset( BYTE bOffset );
	BYTE				RewindOffset();
	VOID				SlaveData( BYTE bSlaveData );
	BYTE				AllowedHeight();
	VOID				AllowedHeight( BYTE Y );
};
