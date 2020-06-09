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

#include "stdafx.h"

CAreaPropertiesDlg::CAreaPropertiesDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 300, 300, TEXT( "Area settings" ), TRUE ),
	m_stGround( hInstance, TEXT( "Ground:" ), 7, 142, 50, 10 ),
	m_tbGround( hInstance, 0x100, TRACKBAR_CLASS, nullptr, 60, 142, -7, 15, WS_VISIBLE | WS_TABSTOP | TBS_HORZ | TBS_TOOLTIPS ),
	m_stForeground( hInstance, TEXT( "Foreground:" ), 7, 162, 50, 10 ),
	m_cbForeground( hInstance, 0x101, WC_COMBOBOX, nullptr, 60, 160, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_stBackground( hInstance, TEXT( "Background:" ), 7, 182, 50, 10 ),
	m_cbBackground( hInstance, 0x102, WC_COMBOBOX, nullptr, 60, 180, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_stPlayerCtl( hInstance, TEXT( "Player:" ), 7, 202, 50, 10 ),
	m_cbPlayerCtl( hInstance, 0x103, WC_COMBOBOX, nullptr, 60, 200, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_stTimer( hInstance, TEXT( "Timer:" ), 7, 222, 50, 10 ),
	m_cbTimer( hInstance, 0x104, WC_COMBOBOX, nullptr, 60, 220, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_stAreaStyle( hInstance, TEXT( "Area style:" ), 7, 242, 50, 10 ),
	m_cbAreaStyle( hInstance, 0x105, WC_COMBOBOX, nullptr, 60, 240, -7, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_cv( hInstance, 0x200, 7, 7, -7, 130, WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_pLevel( nullptr ), m_pInterface( nullptr ), m_fInitialCloudStatus( FALSE )
{
	pushctl( m_stGround );
	pushctl( m_tbGround );
	pushctl( m_stForeground );
	pushctl( m_cbForeground );
	pushctl( m_stBackground );
	pushctl( m_cbBackground );
	pushctl( m_stPlayerCtl );
	pushctl( m_cbPlayerCtl );
	pushctl( m_stTimer );
	pushctl( m_cbTimer );
	pushctl( m_stAreaStyle );
	pushctl( m_cbAreaStyle );
	pushctl( m_cv );
}

BOOL CAreaPropertiesDlg::OnInit( LPARAM lParam )
{
	m_cv.SetView( m_pInterface );
	
	InitCB( m_cbForeground, TEXT( "Foreground #%d" ), fgScenery0, fgSceneryMax, m_pLevel->Header().FgScenery() );	
	InitCB( m_cbBackground, TEXT( "Background #%d" ), bgScenery0, bgSceneryMax, m_pLevel->Header().BgScenery() );
	InitCB( m_cbPlayerCtl, TEXT( "Position #%d" ), plrCtrl0, plrCtrlAuto1, m_pLevel->Header().PlayerCtl() );
	InitCB( m_cbTimer, TEXT( "Timer set #%d" ), tmrCtrl0, tmrCtrlMax, m_pLevel->Header().TimerCtl() );
	InitCB( m_cbAreaStyle, TEXT( "Style #%d" ), areaStyle0, areaStyle2 + 1, m_pLevel->Header().AreaStyle() );
	PushString( m_cbAreaStyle, 3, TEXT( "Cloud area" ) );
	if ( m_pLevel->Header().IsCloudLevel() )
	{
		m_cbAreaStyle.cSendMessage( CB_SETCURSEL, 3 );
		m_fInitialCloudStatus = TRUE;
	}

	for ( int i = bgCtl1; i < bgCtlMax; ++i )
	{
		INT_PTR id = PushString( m_cbForeground, i, TEXT( "Modifier #%d" ), i - bgCtl1 + 1 );
		if ( i == m_pLevel->Header().BgColorCtl() )
		{
			m_cbForeground.cSendMessage( CB_SETCURSEL, i );
		}
	}

	m_tbGround.cSendMessage( TBM_SETRANGE, TRUE, MAKELONG( 1, 16 ) );
	m_tbGround.cSendMessage( TBM_SETPOS, TRUE, m_pLevel->Header().GroundCtl() + 1 );

	if ( m_pLevel->Header().PlayerCtl() >= plrCtrlAuto1 )
	{
		m_cbPlayerCtl.Enable( FALSE );
	}


	RenderView();
	SetFocus( m_tbGround );
	return FALSE;
}

VOID CAreaPropertiesDlg::OnOK()
{
	if ( !m_pLevel->Header().IsCloudLevel() && m_fInitialCloudStatus )
	{
		if ( !Confirm(
			TEXT( "WARNING! Level status was changed from Cloud Area to Area, so all links from this level will be lost!\n" )
			TEXT( "Do you want to continue?" ) ) )
		{
			return;
		}
	}
	Close( 1 );
}

VOID CAreaPropertiesDlg::OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl )
{
	if ( WM_HSCROLL == vScrollType && m_tbGround == hCtl )
	{
		CNesLevelHeader header = m_pLevel->Header();
		INT_PTR bgCtl = m_tbGround.cSendMessage( TBM_GETPOS );
		if ( bgCtl >= 1 && bgCtl <= 16 )
		{
			header.UpdateGroundCtrl( LOBYTE( bgCtl - 1 ) );
			m_pLevel->UpdateHeader( header );
			RenderView();
		}
	}
}

VOID CAreaPropertiesDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		HandleCB( uId );
	}
}

VOID CAreaPropertiesDlg::HandleCB( USHORT uId )
{
	INT_PTR id = dSendMessage( uId, CB_GETCURSEL );
	BYTE val = LOBYTE( dSendMessage( uId, CB_GETITEMDATA, id ) );
	CNesLevelHeader header = m_pLevel->Header();

	switch ( uId )
	{
		case 0x101:
			{
				if ( val < fgSceneryMax )
				{
					header.UpdateFgScenery( static_cast<NES_FG_SCENERY>( val ) );
				}
				else if ( val >= bgCtl1 )
				{
					header.UpdateBgColorCtl( static_cast<NES_BG_COLOR_CTL>( val ) );
				}
				break;
			}
		case 0x102:
			{
				if ( val >= bgScenery0 && val < bgSceneryMax )
				{
					header.UpdateBgScenery( static_cast<NES_BG_SCENERY>( val ) );
				}
				break;
			}
		case 0x103:
			{
				if ( val >= plrCtrl0 && val < plrCtrlAuto1 )
				{
					header.UpdatePlayerCtrl( static_cast<NES_PLAYER_CTRL>( val ) );
				}
				break;
			}
		case 0x104:
			{
				if ( val >= tmrCtrl0 && val < tmrCtrlMax )
				{
					header.UpdateTimerCtrl( static_cast<NES_TIMER_CTRL>( val ) );
				}
				break;
			}
		case 0x105:
			{
				if ( val >= areaStyle0 && val < areaStyleMax )
				{
					header.UpdateAreaStyle( static_cast<NES_AREA_STYLE>( val ) );
				}
				else if ( val == areaStyleCloud )
				{
					header.OverrideCloud();
				}

				break;
			}
	}

	m_pLevel->UpdateHeader( header );
	RenderView();
}

VOID CAreaPropertiesDlg::Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface )
{
	m_pInterface = pInterface;
	m_pLevel = pLevel;
}

VOID CAreaPropertiesDlg::RenderView()
{
	m_cv.RenderView();
	InvalidateRect( m_cv, nullptr, TRUE );
}

VOID CAreaPropertiesDlg::Get_Param( CNesLevelHeader & header )
{
	header = m_pLevel->Header();
}

VOID CAreaPropertiesDlg::InitCB( CControl & cbCtl, LPCTSTR pszString, int from, int to, int selected )
{
	for ( int i = from; i < to; ++i )
	{
		INT_PTR id = PushString( cbCtl, i, pszString, i + 1 );
		if ( i == selected )
		{
			cbCtl.cSendMessage( CB_SETCURSEL, id );
		}
	}
}

INT_PTR CAreaPropertiesDlg::PushString( CControl & cbCtl, LPARAM lData, LPCTSTR pszString, ... )
{
	va_list argptr;
	CString msg;

	va_start( argptr, pszString );
	msg.FormatV( pszString, argptr );
	INT_PTR id = cbCtl.cSendMessage( CB_ADDSTRING, 0, (LPARAM)msg.GetString() );
	cbCtl.cSendMessage( CB_SETITEMDATA, id, lData );
	va_end( argptr );
	return id;
}