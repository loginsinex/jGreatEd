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

class CNesLevelItemGroundType: public CNesLevelItem
{
	const NES_GROUND_DATA			& m_grData;

	SIZE							m_szLevel;
	std::vector<size_t>				m_vGround;
	std::vector<NES_GROUND_PLACE>	m_vGroundInfo;

	VOID			BuildGroundLinks();
	VOID			FillColumnWithTile( std::vector<NES_METATILE> & vColumn, WORD wTemplate, const NES_METATILE & tile );

	NES_METATILE	HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile ) override;
	const std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & GroundData( int x );
	const NES_GROUND_PLACE & GroundDataEx( int x );

	BOOL			WidthChanging( int & cx ) override;
	BOOL			HeightChanging( int & cy ) override;
	BOOL			PlaceChanging( int & x, int & y ) override;
	std::vector<NES_GROUND_PLACE>::iterator FindGround( int X );

public:
	CNesLevelItemGroundType( CNesLevel & nLevel );
	VOID			InitObject() override;
	VOID			LevelSizeChanged( const SIZE & sz ) override;
	BOOL			IsEntranceObject() override;

	VOID			InitGroundInfo();
	VOID			InitDrawQueue( std::vector<NES_DRAW_QUEUE_ITEM> & vDrawQueue );
	VOID			OverrideForeground( NES_BG_COLOR_CTL uForeground );
	VOID			DropOverridenForeground();
	VOID			AddGroundInfo( int X, NES_GROUND_TYPE ngt, const NES_GROUND_INFO & vGround );
	VOID			DrawBounds( CNesLevelView & view );
	BOOL			MoveGround( int uOldX, int uNewX );
	BOOL			GetGround( int X, std::map<NES_GROUND_TYPE, NES_GROUND_INFO> & gr );
	BOOL			EraseGround( int X );
	BOOL			DumpBoundsAsObjects( std::vector<NES_LEVEL_ITEM> & vBounds );
	BYTE			BgCtl();
	VOID			DelPage( UINT uPage );
	VOID			AddPage( UINT uPage );
	VOID			Flush();

	VOID			TakeGroundSnapshot( std::vector<NES_GROUND_PLACE> & vSnapshot );
	VOID			RevertGroundSnapshot( const std::vector<NES_GROUND_PLACE> & vSnapshot );
};

// terrain object
/*
AreaParserCore:
	if ( vBackloadingFlag )
		ProcessAreaData()

	while(X <= 0x0c)
		vMetatileBuffer[X] = 0

	if ( vBackgroundScenery )
	{
		A = vCurrentPageLoc
		while( A > 0x03 )
			A -= 0x03

		A = ( A << 4 ) + bSceneDataOffsets[vBackgroundScenery-1] +	vCurrentColumnPos
		if ( bBackSceneryData[A] )
		{
			X = 3 * ( bBackSceneryData[A] & 0x0f )
			Y = bBackSceneryData[A] >> 4
			tmp0 = 3

			do
			{
				vMetatileBuffer[Y] = bBackSceneryMetatiles[X]
				X++
				Y++
			} while( Y != 0x0b && tmp0-- )

		}
	}

	if ( vForegroundScenery )
	{
		Y = bForeSceneryOffsets[vForegroundScenery]
		X = 0

		do
		{
			if ( bForeSceneryData[Y] )
				vMetatileBuffer[X] = bForeSceneryData[Y]

			X++
			Y++
		} while( X != 0x0D )
	}

	if ( !vAreaType && vWorldNumber == #World_8 )
	{
		A = 0x62
	}
	else
	{
		A = bTerraitMetatiles[vAreaType]
		if ( vCloudTypeOverride )
			A = 0x88
	}

	tmp7 = A
	X = 0
	Y = vTerrainControl * 2

	do
	{
		tmp0 = bTerrainRenderBits[Y++]
		tmp1 = Y

		if ( vCloudTypeOverride && !X )
		{
			tmp0 &= 0x08
		}
		Y = 0

		do
		{
			if ( tmp0 && bBitmask[Y] )
				vMetatileBuffer[X] = tmp7

			X++
			if ( X == 0x0D )
			{
				break
			}

			if ( vAreaType == 0x02 && X == 0x0B )
				tmp7 = 0x54

			Y++
		} while( Y != 0x08 )
	} while ( tmp1 && X != 0x0D )

	ProcessAreaData()
	GetBlockBufferAddr( vBlockBufferColumnPos )	// load ptr to tmp6:tmp7

	X = 0
	Y = 0

	do
	{
		tmp0 = Y
		Y = vMetatileBuffer[X] >> 6
		if ( vMetatileBuffer[X] <= bBlockBuffLowBounds[Y] )
			A = 0
		else
			A = vMetatileBuffer[X]

		tmp6[tmp0] = A

		Y += 0x10
		X++
	} while( X <= 0x0D )
*/