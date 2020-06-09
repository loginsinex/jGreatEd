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

CNesLevelHeader::CNesLevelHeader()
	: m_header( { 0 } )
{

}

NES_BG_COLOR_CTL CNesLevelHeader::BgColorCtl() const
{
	return static_cast<NES_BG_COLOR_CTL>( m_header.bForeground >= 4 ? m_header.bForeground : 0 );
}

NES_FG_SCENERY CNesLevelHeader::FgScenery() const
{
	return static_cast<NES_FG_SCENERY>( m_header.bForeground < 4 ? m_header.bForeground : 0 );
}

NES_PLAYER_CTRL CNesLevelHeader::PlayerCtl() const
{
	return m_header.bEntranceType;
}

NES_TIMER_CTRL CNesLevelHeader::TimerCtl() const
{
	return m_header.bTimer;
}

BYTE CNesLevelHeader::GroundCtl() const
{
	return m_header.bGroundType;
}

NES_BG_SCENERY CNesLevelHeader::BgScenery() const
{
	return m_header.bBgScene;
}

BOOL CNesLevelHeader::IsCloudLevel() const
{
	return ( m_header.bAreaStyle == areaStyleCloud );
}

NES_AREA_STYLE CNesLevelHeader::AreaStyle() const
{
	return ( IsCloudLevel() ? areaStyle0 : m_header.bAreaStyle );
}

VOID CNesLevelHeader::SetRawHeader( WORD wHeader )
{
	m_header.wData = wHeader;
}

WORD CNesLevelHeader::GetRawHeader() const
{
	return m_header.wData;
}

VOID CNesLevelHeader::UpdateBgColorCtl( NES_BG_COLOR_CTL bBgColorCtl )
{
	if ( bBgColorCtl >= bgCtl1 && bBgColorCtl < bgCtlMax )
	{
		m_header.bForeground = bBgColorCtl;
	}
}

VOID CNesLevelHeader::UpdateFgScenery( NES_FG_SCENERY bFgScenery )
{
	if ( bFgScenery < fgSceneryMax )
	{
		m_header.bForeground = bFgScenery;
	}
}

VOID CNesLevelHeader::UpdatePlayerCtrl( NES_PLAYER_CTRL bPlayerCtrl )
{
	if ( bPlayerCtrl < plrCtlMax )
	{
		m_header.bEntranceType = bPlayerCtrl;
	}
}

VOID CNesLevelHeader::UpdateTimerCtrl( NES_TIMER_CTRL bTimerCtrl )
{
	if ( bTimerCtrl < tmrCtrlMax )
	{
		m_header.bTimer = bTimerCtrl;
	}
}

VOID CNesLevelHeader::UpdateGroundCtrl( BYTE bGroundCtl )
{
	if ( bGroundCtl < 0x10 )
	{
		m_header.bGroundType = bGroundCtl;
	}
}

VOID CNesLevelHeader::UpdateBgScenery( NES_BG_SCENERY bBgScenery )
{
	if ( bBgScenery < bgSceneryMax )
	{
		m_header.bBgScene = bBgScenery;
	}
}

VOID CNesLevelHeader::UpdateAreaStyle( NES_AREA_STYLE bAreaStyle )
{
	if ( bAreaStyle < areaStyleMax )
	{
		m_header.bAreaStyle = bAreaStyle;
	}
}

VOID CNesLevelHeader::OverrideCloud()
{
	m_header.bAreaStyle = areaStyleCloud;
}