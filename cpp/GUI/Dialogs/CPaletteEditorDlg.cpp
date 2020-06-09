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

CPaletteChooseDlg::CPaletteChooseDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 375, 120, TEXT( "Choose a color" ), TRUE ), m_bColor( 0 ),
	m_hTooltip( CreateWindowEx( 0, TOOLTIPS_CLASS, nullptr, WS_POPUP | TTS_ALWAYSTIP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr ) )
{
	for ( USHORT i = 0; i < 64; ++i )
	{
		m_btnColor[ i ].PostInit( hInstance, 0x1000 + i, WC_BUTTON, nullptr, 7 + ( ( 7 + 15 )*( i % 16 ) ), 7 + ( ( 7 + 15 )*( i / 16 ) ), 15, 15, WS_VISIBLE | WS_TABSTOP | BS_BITMAP | BS_FLAT );
		pushctl( m_btnColor[ i ] );
	}

	m_stColorId.PostInit( hInstance, 0x3000, WC_STATIC, nullptr, 7, -( 7 + 15 - 2 ), 60, 10, SS_RIGHT | WS_VISIBLE );
	m_stColor.PostInit( hInstance, 0x2000, WC_STATIC, nullptr, 80, -( 7 + 15 ), 50, 15, WS_VISIBLE | SS_BITMAP, WS_EX_CLIENTEDGE );
	pushctl( m_stColorId );
	pushctl( m_stColor );
}

CPaletteChooseDlg::~CPaletteChooseDlg()
{
	DestroyWindow( m_hTooltip );
}

BOOL CPaletteChooseDlg::OnInit( LPARAM lParam )
{
	for ( BYTE i = 0; i < 64; ++i )
	{
		TOOLINFO ti = { 0 };
		m_strTip[ i ].Format( TEXT( "Color %d (0x%02X)" ), i, i );
		m_btnColor[ i ].cSendMessage( BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ i ] );

		ti.cbSize = sizeof( ti );
		ti.hinst = GetInstance();
		ti.hwnd = m_btnColor[ i ];
		ti.uId = (UINT_PTR)ti.hwnd;
		ti.lpszText = (LPTSTR) m_strTip[ i ].GetString();
		ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		SendMessage( m_hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti );
	}

	SendMessage( m_hTooltip, TTM_SETWINDOWTHEME, 0, (LPARAM)TEXT( "EXPLORER" ) );

	OnButton( 0x1000 + m_bColor );
	m_btnColor[ m_bColor ].cSendMessage( BM_SETSTATE, TRUE );
	wSendMessage( DM_SETDEFID, 0x1000 + m_bColor );
	SetFocus( m_btnColor[ m_bColor ] );
	return FALSE;
}

VOID CPaletteChooseDlg::OnOK()
{
	Close( 1 );
}

VOID CPaletteChooseDlg::OnButton( USHORT uId )
{
	BYTE uColId = LOBYTE( uId - 0x1000 );
	if ( uColId < 64 )
	{
		CString strColor;
		strColor.Format( TEXT( "Color: %d (0x%02X)" ), uColId, uColId );
		m_bColor = LOBYTE( uColId );
		m_stColor.cSendMessage( STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ uColId ] );
		m_stColorId.Text( strColor.GetString() );
	}
}

VOID CPaletteChooseDlg::Set_Param( BYTE bColor )
{
	m_bColor = bColor;
}

VOID CPaletteChooseDlg::Get_Param( BYTE & bColor )
{
	bColor = m_bColor;
}

CPaletteEditorDlg::CPaletteEditorDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 200, 220, TEXT( "Palette editor" ), TRUE )
{
	for ( USHORT i = 0; i < 24; ++i )
	{
		if ( !( i % 3 ) )
		{
			CString sLabel;
			USHORT k = i / 3;	// attribute id
			if ( k < 4 )
			{
				sLabel.Format( TEXT( "Tiles attribute #%d:" ), ( k % 4 ) + 1 );
			}
			else
			{
				sLabel.Format( TEXT( "Sprites attribute #%d:" ), ( k % 4 ) + 1 );
			}

			m_stColor[ k ].PostInit( hInstance, 0x2000 + i, WC_STATIC, sLabel, 7, 9 + ( k*( 15 + 7 ) ), 100, 10, SS_RIGHT | WS_VISIBLE );
			pushctl( m_stColor[ k ] );
		}

		m_btnColor[ i ].PostInit( hInstance, 0x1000 + i, WC_BUTTON, nullptr, 110 + ( ( 15 + 7 ) * ( i % 3 ) ), 7 + ( ( 15 + 7 )*( i / 3 ) ), 15, 15, WS_VISIBLE | WS_TABSTOP | BS_BITMAP | BS_FLAT );
		pushctl( m_btnColor[ i ] );
	}
}

VOID CPaletteEditorDlg::Set_Param( const NES_PAL_ARRAY & npa, BOOL fDisableShared )
{
	m_npa = npa;
	m_fDisableShared = fDisableShared;
}

VOID CPaletteEditorDlg::Get_Param( NES_PAL_ARRAY & npa )
{
	npa = m_npa;
}

BOOL CPaletteEditorDlg::OnInit( LPARAM lParam )
{
	USHORT c = 0;
	for ( USHORT pal = 0; pal < 2; ++pal )
	{
		for ( USHORT col = 0; col < 16; ++col )
		{
			if ( col % 4 )
			{
				m_btnColor[ c ].cSendMessage( BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ m_npa.bPal[ pal ][ col ] ] );
				m_btnMap[ 0x1000 + c ] = { pal, col };
				if ( m_fDisableShared && c >= 3 )
				{
					m_btnColor[ c ].Enable( FALSE );

					if ( !( c % 3 ) )
					{
						m_stColor[ c / 3 ].Enable( FALSE );
					}
				}
				c++;
			}
		}
	}

	SetFocus( m_btnColor[ 0 ] );
	return FALSE;
}

VOID CPaletteEditorDlg::OnNCDestroy()
{

}

VOID CPaletteEditorDlg::OnOK()
{
	Close( 1 );
}

VOID CPaletteEditorDlg::OnButton( USHORT uId )
{
	auto itCol = m_btnMap.find( uId );
	if ( itCol != m_btnMap.end() )
	{
		const std::pair<USHORT, USHORT> & pal = itCol->second;
		BYTE & bCol = m_npa.bPal[ pal.first ][ pal.second ];
		CPaletteChooseDlg dlg( GetInstance() );
		dlg.Set_Param( bCol );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( bCol );

			UpdateBitmap( itCol->first - 0x1000, bCol );
		}

	}
}

VOID CPaletteEditorDlg::UpdateBitmap( USHORT uId, BYTE bCol )
{
	m_btnColor[ uId ].cSendMessage( BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ bCol ] );
}

CPaletteDlg::CPaletteDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 275, 75, TEXT( "Palette editor" ), TRUE ),
	m_stPaletteId( hInstance, TEXT( "Palette:" ), 7, 9, 50, 10 ),
	m_cbPaletteId( hInstance, 0x100, WC_COMBOBOX, nullptr, 60, 7, -( 7 + 50 + 7 ), 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL | CBS_SORT),
	m_btnEdit( hInstance, 0x101, TEXT( "Modify..." ), -( 7 + 50 ), 7, 50, 15 )
{
	pushctl( m_stPaletteId );
	pushctl( m_cbPaletteId );
	pushctl( m_btnEdit );
}

BOOL CPaletteDlg::OnInit( LPARAM lParam )
{
	for ( const auto & pal : m_mPal )
	{
		INT_PTR id = m_cbPaletteId.cSendMessage( CB_ADDSTRING, 0, (LPARAM)GetPalString( pal.first ) );
		m_cbPaletteId.cSendMessage( CB_SETITEMDATA, id, pal.first );
	}

	m_cbPaletteId.cSendMessage( CB_SETCURSEL );
	SetFocus( m_cbPaletteId );
	return FALSE;
}

VOID CPaletteDlg::OnOK()
{
	Close( 1 );
}

VOID CPaletteDlg::OnButton( USHORT uId )
{
	if ( 0x101 == uId )
	{
		INT_PTR id = m_cbPaletteId.cSendMessage( CB_GETCURSEL );
		if ( id >= 0 )
		{
			NES_PAL_TABLE npt = static_cast<NES_PAL_TABLE>( LOBYTE( m_cbPaletteId.cSendMessage( CB_GETITEMDATA, id ) ) );

			auto itPal = m_mPal.find( npt );

			if ( itPal != m_mPal.end() )
			{
				CPaletteEditorDlg dlg( GetInstance() );
				BOOL fDisableShared = FALSE;
				switch ( itPal->first )
				{
					case nptNightSnowGround:
					case nptDaySnowGround:
					case nptMushroomGround: fDisableShared = TRUE; break;
					default: break;
				}

				dlg.Set_Param( itPal->second, fDisableShared );
				if ( dlg.Show( this ) )
				{
					dlg.Get_Param( itPal->second );
					SetFocus( m_cbPaletteId );
				}
			}
		}
	}
}

LPCTSTR CPaletteDlg::GetPalString( NES_PAL_TABLE npt )
{
	switch ( npt )
	{
		case nptGround: return TEXT( "Ground palette" );
		case nptNightSnowGround: return TEXT( "Night snow ground palette" );
		case nptDaySnowGround: return TEXT( "Day snow ground palette" );
		case nptMushroomGround: return TEXT( "Mushroom ground palette" );
		case nptWater: return TEXT( "Water palette" );
		case nptUnderground: return TEXT( "Underground palette" );
		case nptCastle: return TEXT( "Castle palette" );
	}

	return TEXT( "Unknown" );
}

VOID CPaletteDlg::Set_Param( const std::map<NES_PAL_TABLE, NES_PAL_ARRAY> & mPal )
{
	m_mPal = mPal;
}

VOID CPaletteDlg::Get_Param( std::map<NES_PAL_TABLE, NES_PAL_ARRAY> & mPal )
{
	mPal = m_mPal;
}


CPaletteBgDlg::CPaletteBgDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 250, 60, TEXT( "Background color editor" ), TRUE ),
	m_stBgColor( hInstance, TEXT( "Color:" ), 7, 9, 50, 10 ),
	m_cbBgColor( hInstance, 0x100, WC_COMBOBOX, nullptr, 60, 7, -( 7 + 50 + 7 ), 150, WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL ),
	m_btnEdit( hInstance, 0x101, TEXT( "Modify..." ), -( 7 + 50 ), 7, 50, 15 ),
	m_stColor( hInstance, 0x1000, WC_STATIC, nullptr, 7, -( 7 + 15 ), 50, 15, WS_VISIBLE | SS_BITMAP, WS_EX_CLIENTEDGE ),
	m_bColor( 0 )
{
	pushctl( m_stBgColor );
	pushctl( m_cbBgColor );
	pushctl( m_btnEdit );
	pushctl( m_stColor );
}


BOOL CPaletteBgDlg::OnInit( LPARAM lParam )
{
	LPCTSTR pszStr[] =
	{
		TEXT("Water area background"),
		TEXT("Ground area background"),
		TEXT("Underground area background"),
		TEXT("Castle area background"),
		TEXT("Background color #1"),
		TEXT("Background color #2"),
		TEXT("Background color #3"),
		TEXT("Background color #4")
	};
	int cbId = 0, id = 0;

	for ( const auto & psz : pszStr )
	{
		INT_PTR id = m_cbBgColor.cSendMessage( CB_ADDSTRING, 0, (LPARAM)psz );
		m_cbBgColor.cSendMessage( CB_SETITEMDATA, id, cbId++ );
	}

	m_cbBgColor.cSendMessage( CB_SETCURSEL );
	SelectColor( 0 );
	SetFocus( m_cbBgColor );
	return FALSE;
}

VOID CPaletteBgDlg::OnOK()
{
	Close( 1 );
}

VOID CPaletteBgDlg::OnCommand( USHORT uCmd, USHORT uID, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd && 0x100 == uID )
	{
		INT_PTR id = m_cbBgColor.cSendMessage( CB_GETCURSEL );
		if ( id >= 0 )
		{
			BYTE bColor = LOBYTE( m_cbBgColor.cSendMessage( CB_GETITEMDATA, id ) );
			if ( bColor < 8 )
			{
				SelectColor( bColor );
			}
		}
	}
}

VOID CPaletteBgDlg::OnButton( USHORT uId )
{
	if ( 0x101 == uId )
	{
		CPaletteChooseDlg dlg( GetInstance() );
		BYTE & bCol = m_bgColors.bColor[ m_bColor ];

		dlg.Set_Param( bCol );

		if ( dlg.Show( this ) )
		{
			dlg.Get_Param( bCol );
			UpdateColor( m_bColor );
		}
	}
}

VOID CPaletteBgDlg::OnNCDestroy()
{

}

VOID CPaletteBgDlg::SelectColor( BYTE bColor )
{
	m_bColor = bColor;
	m_stColor.cSendMessage( STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ m_bgColors.bColor[ m_bColor ] ] );
}

VOID CPaletteBgDlg::UpdateColor( BYTE bColor )
{
	m_stColor.cSendMessage( STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_hbmColor[ m_bgColors.bColor[ bColor ] ] );
	m_bColor = bColor;
}

VOID CPaletteBgDlg::Set_Param( const NES_BG_COLORS & nbg )
{
	m_bgColors = nbg;
}

VOID CPaletteBgDlg::Get_Param( NES_BG_COLORS & nbg )
{
	nbg = m_bgColors;
}