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

#pragma pack(push,1)
typedef enum _tagNES_BG_COLOR_CTL : BYTE
{
	bgCtl0 = 0,
	bgCtl1 = 4,
	bgCtl2 = 5,
	bgCtl3 = 6,
	bgCtl4 = 7,
	bgCtlMax
} NES_BG_COLOR_CTL, *PNES_BG_COLOR_CTL;

typedef enum _tagNES_FG_SCENERY : BYTE
{
	fgScenery0 = 0,
	fgScenery1 = 1,
	fgScenery2 = 2,
	fgScenery3 = 3,
	fgSceneryMax
} NES_FG_SCENERY, *PNES_FG_SCENERY;

typedef enum _tagNES_PLAYER_CTRL : BYTE
{
	plrCtrl0 = 0,
	plrCtrl1 = 1,
	plrCtrl2 = 2,
	plrCtrl3 = 3,
	plrCtrl4 = 4,
	plrCtrl5 = 5,
	plrCtrlAuto1 = 6,
	plrCtrlAuto2 = 7,
	plrCtlMax
} NES_PLAYER_CTRL, *PNES_PLAYER_CTRL;

typedef enum _tagNES_TIMER_CTRL : BYTE
{
	tmrCtrl0 = 0,
	tmrCtrl1 = 1,
	tmrCtrl2 = 2,
	tmrCtrl3 = 3,
	tmrCtrlMax
} NES_TIMER_CTRL, *PNES_TIMER_CTRL;

typedef enum _tagNES_BG_SCENERY : BYTE
{
	bgScenery0 = 0,
	bgScenery1 = 1,
	bgScenery2 = 2,
	bgScenery3 = 3,
	bgSceneryMax
} NES_BG_SCENERY, *PNES_BG_SCENERY;

typedef enum _tagNES_AREA_STYLE : BYTE
{
	areaStyle0 = 0,
	areaStyle1 = 1,
	areaStyle2 = 2,
	areaStyleCloud = 3,
	areaStyleMax = 3
} NES_AREA_STYLE, *PNES_AREA_STYLE;

typedef struct _tagNES_LEVEL_HEADER
{
	union
	{
		WORD			wData;
		struct
		{
			BYTE				bForeground : 3;
			NES_PLAYER_CTRL		bEntranceType : 3;
			NES_TIMER_CTRL		bTimer : 2;
			BYTE				bGroundType : 4;
			NES_BG_SCENERY		bBgScene : 2;
			NES_AREA_STYLE		bAreaStyle : 2;
		};
	};
} NES_LEVEL_HEADER, *PNES_LEVEL_HEADER;
#pragma pack(pop)

class CNesLevelHeader
{
	NES_LEVEL_HEADER					m_header;

public:
	CNesLevelHeader();

	NES_BG_COLOR_CTL			BgColorCtl() const;
	NES_FG_SCENERY				FgScenery() const;
	NES_PLAYER_CTRL				PlayerCtl() const;
	NES_TIMER_CTRL				TimerCtl() const;
	BYTE						GroundCtl() const;
	NES_BG_SCENERY				BgScenery() const;
	BOOL						IsCloudLevel() const;
	NES_AREA_STYLE				AreaStyle() const;

	VOID			SetRawHeader( WORD wHeader );
	WORD			GetRawHeader() const;

	VOID			UpdateBgColorCtl( NES_BG_COLOR_CTL bBgColorCtl );
	VOID			UpdateFgScenery( NES_FG_SCENERY bFgScenery );
	VOID			UpdatePlayerCtrl( NES_PLAYER_CTRL bPlayerCtrl );
	VOID			UpdateTimerCtrl( NES_TIMER_CTRL bTimerCtrl );
	VOID			UpdateGroundCtrl( BYTE bGroundCtl );
	VOID			UpdateBgScenery( NES_BG_SCENERY bBgScenery );
	VOID			UpdateAreaStyle( NES_AREA_STYLE bAreaStyle );
	VOID			OverrideCloud();
};
