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

CEngineHackSheet::CEngineHackSheet( HINSTANCE hInstance )
	: m_hInstance( hInstance )
{

}

CEngineHackSheet::~CEngineHackSheet()
{
	FreeDialogs();
}

CEngineHackDlg & CEngineHackSheet::pushdlg( CEngineHackDlg * pDlg )
{
	m_vpDialog.push_back( pDlg );
	return *pDlg;
}

VOID CEngineHackSheet::InitDialogs()
{
	pushdlg( new CEngineHackCoins1UP( GetInstance(), m_hack ) );
	pushdlg( new CEngineHackWarpZones( GetInstance(), m_hack ) );
	pushdlg( new CEngineHackCoinColors( GetInstance(), m_hack ) );
	pushdlg( new CEngineHackHardLevel( GetInstance(), m_hack ) );
	pushdlg( new CEngineBowserHammers( GetInstance(), m_hack ) );
	pushdlg( new CEngineTimerValues( GetInstance(), m_hack ) );
	pushdlg( new CEngineSpinyEggBehavior( GetInstance(), m_hack ) );
}

VOID CEngineHackSheet::FreeDialogs()
{
	for ( auto pDlg : m_vpDialog )
	{
		delete pDlg;
	}

	m_vpDialog.clear();
}

HINSTANCE CEngineHackSheet::GetInstance()
{
	return m_hInstance;
}

VOID CEngineHackSheet::Set_Param( const NES_ENGINE_HACK & hack )
{
	m_hack = hack;
}

VOID CEngineHackSheet::Get_Param( NES_ENGINE_HACK & hack )
{
	hack = m_hack;
}

INT_PTR CEngineHackSheet::Show( CWindow * pParent )
{
	INT_PTR result = 0;
	CPropSheet pSheet( GetInstance() );
	pSheet.Title( TEXT( "Game settings" ) );

	InitDialogs();

	for ( auto pDialog : m_vpDialog )
	{
		pSheet.AddSheet( *pDialog, 0 );
	}

	result = pSheet.Show( pParent );

	FreeDialogs();

	return result;
}

// base class

CEngineHackDlg::CEngineHackDlg( HINSTANCE hInstance, LPCTSTR pszTitle, NES_ENGINE_HACK & hack )
	: CDialog( hInstance, 300, 250, pszTitle ), m_hack( hack ),
	m_ctlFrame( hInstance, -1, WC_BUTTON, pszTitle, 7, 7, -7, -7, WS_VISIBLE | BS_GROUPBOX )
{
	pushctl( m_ctlFrame );
}

CEngineHackDlg::~CEngineHackDlg()
{

}

NES_ENGINE_HACK & CEngineHackDlg::Hack()
{
	return m_hack;
}

VOID CEngineHackDlg::Changed()
{
	PropSheet()->FChanged( this );
}

// derived classes

CEngineHackCoins1UP::CEngineHackCoins1UP( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Coins for 1-UP" ), hack )
{
	const int left = 40, top = 30;
	CString strWorld;
	strWorld = TEXT( "Coins for 1-UP:" );
	m_stWorld.PostInit( hInstance, 0x1000, WC_STATIC, strWorld, left, top + 2, 50, 10, SS_RIGHT | WS_VISIBLE );
	m_edWorld.PostInit( hInstance, 0x2000, WC_EDIT, nullptr, left + 60, top, 40, 13, WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER );
	m_udWorld.PostInit( hInstance, 0x3000, UPDOWN_CLASS, nullptr, 0, 0, 0, 0, WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_AUTOBUDDY | UDS_SETBUDDYINT | UDS_WRAP );
	
	pushctl( m_stWorld );
	pushctl( m_edWorld );
	pushctl( m_udWorld );
}

BOOL CEngineHackCoins1UP::OnInit( LPARAM lParam )
{
	m_udWorld.SetRange( 99, 0 );
	m_udWorld.Value( Hack().coins.bCoins[ 0 ] );
	SetFocus( m_edWorld );
	return FALSE;
}

BOOL CEngineHackCoins1UP::PSOnApply( BOOL fOkPressed )
{
	for ( USHORT i = 0; i < NES_WORLDS_COUNT; ++i )
	{
		if ( m_udWorld.IsError() )
		{
			ShowError( TEXT( "Erroneous value for coins count" ) );
			return PSNRET_INVALID;
		}
		else
		{
			Hack().coins.bCoins[ 0 ] = LOBYTE( m_udWorld.Value() );
		}
	}

	return PSNRET_NOERROR;
}

VOID CEngineHackCoins1UP::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( EN_CHANGE == uCmd )
	{
		Changed();
	}
}

///

CEngineHackWarpZones::CEngineHackWarpZones( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Warp zones" ), hack )
{
	for ( USHORT z = 0; z < 11; ++z )
	{
		CString strFormat;
		strFormat.Format( TEXT( "Warp zone #%d:" ), z + 1 );
		m_stZone[ z ].PostInit( hInstance, 0x1000, WC_STATIC, strFormat, 15 + 140 * ( z % 2 ), 22 + 25 * ( z / 2 ), 60, 10, SS_RIGHT | WS_VISIBLE );
		pushctl( m_stZone[ z ] );

		m_cbWorld[ z ].PostInit( hInstance, 0x2000 + 3 * z, WC_COMBOBOX, nullptr, 80 + 140 * ( z % 2 ), 20 + 25 * ( z / 2 ), 60, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL );
		pushctl( m_cbWorld[ z ] );
	}
}

BOOL CEngineHackWarpZones::OnInit( LPARAM lParam )
{
	
	for ( USHORT z = 0; z < 11; ++z )
	{
		const BYTE bWorld = Hack().warp.bWorlds[ z ];
		m_cbWorld[ z ].cSendMessage( CB_SETCURSEL );
		USHORT wWorldsCount = ( z < 8 ? 8 : 4 );
		wchar_t wLiteral = ( z < 8 ? _T( '1' ) : _T( 'A' ) );
		USHORT wWorldOffset = ( z < 8 ? 0 : 9 );

		for ( USHORT i = 0; i < wWorldsCount; ++i )
		{
			CString strFormat;
			strFormat.Format( TEXT( "World #%c" ), wLiteral + i );
			INT_PTR id = m_cbWorld[ z ].cSendMessage( CB_ADDSTRING, 0, (LPARAM)strFormat.GetString() );
			m_cbWorld[ z ].cSendMessage( CB_SETITEMDATA, id, wWorldOffset + i );
		}

		if ( bWorld >= 1 && bWorld <= 13 )
		{
			m_cbWorld[ z ].cSendMessage( CB_SETCURSEL, bWorld - 1 - LOBYTE( wWorldOffset ) );
		}
	}

	SetFocus( m_cbWorld[ 0 ] );
	return FALSE;
}

BOOL CEngineHackWarpZones::PSOnApply( BOOL fOkPressed )
{
	for ( USHORT z = 0; z < 11; ++z )
	{
		BYTE bWorld = static_cast<BYTE>( m_cbWorld[ z ].cSendMessage( CB_GETITEMDATA, m_cbWorld[ z ].cSendMessage( CB_GETCURSEL ) ) ) + 1;
		Hack().warp.bWorlds[ z ] = bWorld;
	}

	return PSNRET_NOERROR;
}

VOID CEngineHackWarpZones::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		Changed();
	}
}

///

CEngineHackCoinColors::CEngineHackCoinColors( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Blinking colors" ), hack )
{
	for ( USHORT i = 0; i < 6; ++i )
	{
		m_btnCol[ i ].PostInit( hInstance, 0x1000 + i, WC_BUTTON, nullptr, 40 + 30 * i, 30, 20, 20, BS_FLAT | WS_VISIBLE | WS_TABSTOP | BS_BITMAP );
		pushctl( m_btnCol[ i ] );
	}
}

BOOL CEngineHackCoinColors::OnInit( LPARAM lParam )
{
	for ( USHORT i = 0; i < 6; ++i )
	{
		InitColor( i, Hack().rotateCols.bCol[ i ] );
	}


	SetFocus( m_btnCol[ 0 ] );
	return FALSE;
}

VOID CEngineHackCoinColors::OnNCDestroy()
{

}

VOID CEngineHackCoinColors::InitColor( USHORT i, BYTE bCol )
{
	m_btnCol[ i ].cSendMessage( BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ bCol ] );
}

BOOL CEngineHackCoinColors::PSOnApply( BOOL fOkPressed )
{
	return PSNRET_NOERROR;
}

VOID CEngineHackCoinColors::OnButton( USHORT uId )
{
	if ( uId >= 0x1000 )
	{
		uId -= 0x1000;
		if ( uId < 6 )
		{
			CPaletteChooseDlg dlg( GetInstance() );
			dlg.Set_Param( Hack().rotateCols.bCol[ uId ] );

			if ( dlg.Show( this ) )
			{
				dlg.Get_Param( Hack().rotateCols.bCol[ uId ] );
				InitColor( uId, Hack().rotateCols.bCol[ uId ] );
				Changed();
			}
		}
	}
}

///

CEngineHackHardLevel::CEngineHackHardLevel( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Hard mode level" ), hack ),
	m_stWorld( hInstance, TEXT( "Raise hard mode at world:" ), 20, 22, 100, 10 ),
	m_cbWorld( hInstance, 0x100, WC_COMBOBOX, nullptr, 130, 20, 80, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_stLevel( hInstance, TEXT( "Level:" ), 20, 52, 100, 10 ),
	m_edLevel( hInstance, 0x101, nullptr, 130, 50, 40, 13 ),
	m_udLevel( hInstance, 0x102 )
{
	pushctl( m_stWorld );
	pushctl( m_cbWorld );
	pushctl( m_stLevel );
	pushctl( m_edLevel );
	pushctl( m_udLevel );
}

BOOL CEngineHackHardLevel::OnInit( LPARAM lParam )
{
	for ( USHORT i = 0; i < 8; ++i )
	{
		CString strFormat;
		strFormat.Format( TEXT( "World #%d" ), i + 1 );
		m_cbWorld.cSendMessage( CB_ADDSTRING, 0, (LPARAM)strFormat.GetString() );
	}
	m_cbWorld.cSendMessage( CB_SETCURSEL, Hack().hardMode.bWorld );

	m_udLevel.SetRange( 9, 1 );
	m_udLevel.Value( Hack().hardMode.bLevel + 1 );

	SetFocus( m_cbWorld );
	return FALSE;
}

BOOL CEngineHackHardLevel::PSOnApply( BOOL fOkPressed )
{
	BYTE bWorld = static_cast<BYTE>( LOBYTE( m_cbWorld.cSendMessage( CB_GETCURSEL ) ) );
	if ( bWorld < 8 && !m_udLevel.IsError() )
	{
		Hack().hardMode.bWorld = bWorld;
		Hack().hardMode.bLevel = static_cast<BYTE>( LOBYTE( m_udLevel.Value() - 1 ) );
		return PSNRET_NOERROR;
	}

	ShowError( TEXT( "Invalid field value" ) );
	return PSNRET_INVALID;
}

VOID CEngineHackHardLevel::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd || EN_CHANGE == uCmd )
	{
		Changed();
	}
}

///

CEngineBowserHammers::CEngineBowserHammers( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Bowser" ), hack ),
	m_stWorld( hInstance, TEXT( "Bowser throws a hammers from world:" ), 20, 22, 150, 10, FALSE, SS_RIGHT | WS_VISIBLE ),
	m_cbWorld( hInstance, 0x100, WC_COMBOBOX, nullptr, 180, 20, -20, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL )
{
	pushctl( m_stWorld );
	pushctl( m_cbWorld );
}

BOOL CEngineBowserHammers::OnInit( LPARAM lParam )
{
	for ( USHORT i = 0; i < 8; ++i )
	{
		CString strFormat;
		strFormat.Format( TEXT( "World #%d" ), i + 1 );
		m_cbWorld.cSendMessage( CB_ADDSTRING, 0, (LPARAM)strFormat.GetString() );
	}
	m_cbWorld.cSendMessage( CB_SETCURSEL, Hack().bowserHammers );
	SetFocus( m_cbWorld );
	return FALSE;
}

BOOL CEngineBowserHammers::PSOnApply( BOOL fOkPressed )
{
	BYTE bWorld = static_cast<BYTE>( LOBYTE( m_cbWorld.cSendMessage( CB_GETCURSEL ) ) );
	if ( bWorld < 8 )
	{
		Hack().bowserHammers = bWorld;
		return PSNRET_NOERROR;
	}

	ShowError( TEXT( "Invalid field value" ) );
	return PSNRET_INVALID;
}

VOID CEngineBowserHammers::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		Changed();
	}
}

///

CEngineTimerValues::CEngineTimerValues( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Timers sets" ), hack )
{
	for ( USHORT i = 0; i < 4; ++i )
	{
		DWORD dwDisabled = ( i > 0 ? 0 : WS_DISABLED );
		CString strFormat;
		strFormat.Format( TEXT( "Timer set #%d:" ), i + 1 );
		m_stTimerSet[ i ].PostInit( hInstance, 0x1000 + i, WC_STATIC, strFormat, 20, 22 + 20 * i, 50, 10, SS_RIGHT | WS_VISIBLE | dwDisabled );
		m_cbTimerSet[ i ].PostInit( hInstance, 0x2000 + i, WC_COMBOBOX, nullptr, 80, 20 + 20 * i, 60, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL | dwDisabled );
		pushctl( m_stTimerSet[ i ] );
		pushctl( m_cbTimerSet[ i ] );
	}
}

BOOL CEngineTimerValues::OnInit( LPARAM lParam )
{
	for ( USHORT i = 0; i < 4; ++i )
	{
		for ( USHORT t = 0; t < 10; ++t )
		{
			CString strFormat;
			strFormat.Format( TEXT( "%03d" ), 100 * t );
			m_cbTimerSet[ i ].cSendMessage( CB_ADDSTRING, 0, (LPARAM)strFormat.GetString() );
		}
		m_cbTimerSet[ i ].cSendMessage( CB_SETCURSEL, Hack().timerValues.bTimerValue[ i ] );
	}

	SetFocus( m_cbTimerSet[ 1 ] );
	return FALSE;
}

BOOL CEngineTimerValues::PSOnApply( BOOL fOkPressed )
{
	for ( USHORT i = 1; i < 4; ++i )
	{
		BYTE bTimerValue = static_cast<BYTE>( LOBYTE( m_cbTimerSet[ i ].cSendMessage( CB_GETCURSEL ) ) );
		if ( bTimerValue < 10 )
		{
			Hack().timerValues.bTimerValue[ i ] = bTimerValue;
		}
		else
		{
			ShowErrorF( TEXT( "Invalid timer set #%d" ), i + 1 );
			return PSNRET_INVALID;
		}
	}

	return PSNRET_NOERROR;
}

VOID CEngineTimerValues::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		Changed();
	}
}

CEngineSpinyEggBehavior::CEngineSpinyEggBehavior( HINSTANCE hInstance, NES_ENGINE_HACK & hack )
	: CEngineHackDlg( hInstance, TEXT( "Hacks" ), hack ),
	m_stBehavior( hInstance, TEXT( "Spiny Egg falling behavior:" ), 20, 22, 120, 10 ),
	m_cbBehavior( hInstance, 0x100, WC_COMBOBOX, nullptr, 150, 20, -20, 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL )
{
	pushctl( m_stBehavior );
	pushctl( m_cbBehavior );
}

BOOL CEngineSpinyEggBehavior::OnInit( LPARAM lParam )
{
	m_cbBehavior.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Use default behavior" ) );
	m_cbBehavior.cSendMessage( CB_ADDSTRING, 0, (LPARAM)TEXT( "Use alternate behavior" ) );

	m_cbBehavior.cSendMessage( CB_SETCURSEL, ( Hack().defaultEggBehavior ? 0 : 1 ) );
	SetFocus( m_cbBehavior );
	
	return FALSE;
}

BOOL CEngineSpinyEggBehavior::PSOnApply( BOOL fOkPressed )
{
	Hack().defaultEggBehavior = ( !m_cbBehavior.cSendMessage( CB_GETCURSEL ) );

	return PSNRET_NOERROR;
}

VOID CEngineSpinyEggBehavior::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		Changed();
	}
}