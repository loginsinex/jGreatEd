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

CNesPlayerEntrance::CNesPlayerEntrance( CNESFile & file, CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr )
{
	
}

VOID CNesPlayerEntrance::LoadData()
{
	m_tmr = m_file.Data<NES_TIMER_CONTROL>( m_eptr.Pointer( ePlayerTimerCtrl ) );
	m_entr = m_file.Data<NES_ENTRANCE_CONTROL>( m_eptr.Pointer( ePlayerEntranceCtrl ) );
	m_plrX = m_file.Data<NES_PLAYER_X>( m_eptr.Pointer( ePlayerEntranceX ) );
}

VOID CNesPlayerEntrance::DumpData()
{
	m_file.Data<NES_TIMER_CONTROL>( m_eptr.Pointer( ePlayerTimerCtrl ) ) = m_tmr;
	m_file.Data<NES_ENTRANCE_CONTROL>( m_eptr.Pointer( ePlayerEntranceCtrl ) ) = m_entr;
	m_file.Data<NES_PLAYER_X>( m_eptr.Pointer( ePlayerEntranceX ) ) = m_plrX;
}

VOID CNesPlayerEntrance::UpdateTimerControl( NES_TIMER_CTRL tmrCtrl, BYTE bTimer )
{
	if ( tmrCtrl > tmrCtrl0 && tmrCtrl < tmrCtrlMax )
	{
		m_tmr.bData[ tmrCtrl ] = bTimer % 10;
	}
}

BYTE CNesPlayerEntrance::TimerControl( NES_TIMER_CTRL tmrCtrl ) const
{
	if ( tmrCtrl > tmrCtrl0 && tmrCtrl < tmrCtrlMax )
	{
		return m_tmr.bData[ tmrCtrl ];
	}
	return 0;
}

VOID CNesPlayerEntrance::UpdateEntranceControl( NES_PLAYER_CTRL plrCtrl, BYTE bCtrl )
{
	if ( plrCtrl >= plrCtrl0 && plrCtrl < plrCtlMax )
	{
		m_entr.bData[ plrCtrl ] = ( bCtrl << 4 );
	}
}

BYTE CNesPlayerEntrance::EntranceControl( NES_PLAYER_CTRL plrCtrl ) const
{
	if ( plrCtrl >= plrCtrl0 && plrCtrl < plrCtlMax )
	{
		return ( m_entr.bData[ plrCtrl ] >> 4 );
	}

	return 0;
}

POINT CNesPlayerEntrance::GetPlayerPoint( NES_PLAYER_CTRL plrCtrl ) const
{
	POINT pt = { 0, 0 };
	if ( plrCtrl >= plrCtrl0 && plrCtrl < plrCtlMax )
	{
		pt.x = m_plrX.bData[ 0 ] / 8;
		pt.y = m_entr.bData[ plrCtrl ] / 8;
	}

	return pt;
}