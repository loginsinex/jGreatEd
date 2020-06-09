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

#include <shellapi.h>

CAboutDlg::CAboutDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 320, 100, TEXT( "About an editor" ) ),
	m_icon( hInstance, 0x100, WC_STATIC, NULL, 7, 7, 20, 20, SS_ICON | SS_SUNKEN | WS_VISIBLE ),
	m_static( hInstance, 0x200, WC_STATIC, NULL, 35, 7, -7, -( 15 + 7 + 7 ), SS_SUNKEN | WS_VISIBLE ),
	m_lnk( hInstance, 0x300, WC_LINK, TEXT( "<a href=\"mailto:greated@filezz.net\">greated@filezz.net</a>" ), 7, -( 7 + 15 ), 100, 15, WS_TABSTOP | WS_VISIBLE ),
	m_lnkForum( hInstance, 0x301, WC_LINK, TEXT( "<a href=\"http://www.romhacking.net/utilities/1485/\">Check for the latest version</a>" ),
				114, -( 7 + 15 ), 100, 15, WS_TABSTOP | WS_VISIBLE ),
	m_ok( hInstance, IDOK, WC_BUTTON, MB_GetString( IDOK - 1 ).c_str(), -( 7 + 50 ), -( 7 + 15 ), 50, 15, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_VISIBLE )
{
	pushctl( m_ok );
	pushctl( m_lnk );
	pushctl( m_lnkForum );
	pushctl( m_static );
	pushctl( m_icon );
}

BOOL CAboutDlg::OnInit( LPARAM lParam )
{
	m_icon.cSendMessage( STM_SETICON, SendMessage( *Parent(), WM_GETICON, ICON_BIG, 0 ), 0 );
	m_static.Text( TEXT( "A FDS Super Mario Bros II Level Editor\n© Copyright by ALXR, 2020\n\n" ) );

	return TRUE;
}

INT_PTR CAboutDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x300:
			{
				if ( lpnm->code == NM_CLICK || lpnm->code == NM_RETURN )
				{
					NMLINK * nml = (NMLINK*)lpnm;
					ShellExecute( *this, TEXT( "open" ), TEXT( "mailto:greated@filezz.net?subject=jGreatEd%20question" ), NULL, NULL, SW_SHOW );
				}
				break;
			}
		case 0x301:
			{
				if ( lpnm->code == NM_CLICK || lpnm->code == NM_RETURN )
				{
					NMLINK * nml = (NMLINK*)lpnm;
					ShellExecute( *this, TEXT( "open" ), TEXT( "http://www.romhacking.net/utilities/1485/" ), NULL, NULL, SW_SHOW );
				}
				break;
			}
	}
	return 0;
}

VOID CAboutDlg::OnOK()
{
	Close( 1 );
}