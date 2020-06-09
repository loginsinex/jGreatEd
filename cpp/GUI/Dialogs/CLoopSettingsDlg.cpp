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

CLoopSettingsDlg::CLoopSettingsDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 300, 250, TEXT( "Loop keeper settings" ), TRUE ),
	m_cv( hInstance, 0x100, 7, 7, -7, 150, WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_stPage( hInstance, TEXT( "Trigger page:" ), 7, 172, 50, 10 ),
	m_edPage( hInstance, 0x101, nullptr, 60, 170, 30, 13 ),
	m_udPage( hInstance, 0x102 ),
	m_stHeight( hInstance, TEXT( "Pass height:" ), 7, 192, 50, 10 ),
	m_edHeight( hInstance, 0x103, nullptr, 60, 190, 30, 13 ),
	m_udHeight( hInstance, 0x104 ),
	m_chkConnected( hInstance, 0x105, TEXT( "Connected trigger" ), 7, 210, 100, 13 ),
	m_pLevel( nullptr ), m_pLevelInterface( nullptr ), m_pLoop( nullptr )

{
	pushctl( m_stPage );
	pushctl( m_edPage );
	pushctl( m_udPage );
	pushctl( m_stHeight );
	pushctl( m_edHeight );
	pushctl( m_udHeight );
	pushctl( m_chkConnected );
	pushctl( m_cv );
}

BOOL CLoopSettingsDlg::OnInit( LPARAM lParam )
{
	m_cv.SetView( m_pLevelInterface );
	m_udPage.SetRange( m_pLevel->PagesCount() - 1, m_pLoop->LoopConnected() ? NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE : NES_LOOP_COMMAND_MINIMUM_PAGE );
	m_udHeight.SetRange( 2, 15 );
	
	m_udPage.Value( m_pLoop->TriggerPage() );
	m_udHeight.Value( m_pLoop->AllowedHeight() );
	m_chkConnected.Check( m_pLoop->LoopConnected() );

	SetFocus( m_edPage );
	Render();
	return FALSE;
}

VOID CLoopSettingsDlg::OnOK()
{
	NES_LOOP lp;
	m_pLoop->DumpLoop( lp );
	if ( !m_pLoop->LoopConnected() && lp.bPageNumber < NES_LOOP_COMMAND_MINIMUM_PAGE ||
		 m_pLoop->LoopConnected() && lp.bPageNumber < NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE )
	{
		ShowAlertF( TEXT( "For connected loop commands Trigger page cannot be less than %d." ), NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE );
		return;
	}

	Close( 1 );
}

VOID CLoopSettingsDlg::Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, size_t idObject )
{
	m_pLoop = static_cast<CNesLoopCommand*>( pLevel->ObjectById( idObject, FALSE ) );
	m_pLevel = pLevel;
	m_pLevelInterface = pInterface;
}

VOID CLoopSettingsDlg::Get_Param( NES_LOOP & nlp )
{
	m_pLoop->DumpLoop( nlp );
	nlp.bSlaveData = m_pLoop->LoopConnected();
}

VOID CLoopSettingsDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( EN_CHANGE == uCmd )
	{
		switch ( uId )
		{
			case 0x101:
				{
					if ( !m_udPage.IsError() )
					{
						m_pLoop->TriggerPage( LOBYTE( m_udPage.Value() ) );
						Render();
					}
					break;
				}
			case 0x103:
				{
					if ( !m_udHeight.IsError() )
					{
						m_pLoop->AllowedHeight( LOBYTE( m_udHeight.Value() ) );
						Render();
					}
					break;
				}
		}
	}
}

VOID CLoopSettingsDlg::Render()
{
	m_cv.RenderView();
	m_cv.ScrollTo( m_pLoop->TriggerPage() - 1 );
}

VOID CLoopSettingsDlg::OnButton( USHORT uId )
{
	if ( 0x105 == uId )
	{
		m_pLoop->LoopConnected( m_chkConnected.Check() );
		m_udPage.SetRange( m_pLevel->PagesCount() - 1, m_pLoop->LoopConnected() ? NES_LOOP_OWNED_COMMAND_MINIMUM_PAGE : NES_LOOP_COMMAND_MINIMUM_PAGE );
		Render();
	}
}