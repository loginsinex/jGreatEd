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

CGroundDlg::CGroundDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 300, 350, TEXT( "Ground modifier" ), TRUE ),
	m_cv( hInstance, 0x100, 7, 7, -7, 170, WS_VISIBLE | WS_TABSTOP | ES_READONLY, WS_EX_CLIENTEDGE ),
	m_lvType( hInstance, 0x101, 7, 190, -7, 40, WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_BORDER ), 
	m_stGround( hInstance, TEXT( "Ground:" ), 7, 252, 50, 10, FALSE, SS_RIGHT, 0, 0x1001 ),
	m_tbGround( hInstance, 0x102, TRACKBAR_CLASS, nullptr, 60, 250, -7, 15, WS_TABSTOP | TBS_HORZ | TBS_TOOLTIPS, 0 ),
	m_stBgScenery( hInstance, TEXT( "Modifier:" ), 7, 272, 50, 10, FALSE, SS_RIGHT, 0, 0x1002 ),
	m_cbBgScenery( hInstance, 0x103, WC_COMBOBOX, nullptr, 60, 270, -7, 150, WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, 0 ),
	m_stBgControl( hInstance, TEXT( "Background:" ), 7, 292, 50, 10, FALSE, SS_RIGHT, 0, 0x1003 ),
	m_cbBgControl( hInstance, 0x104, WC_COMBOBOX, nullptr, 60, 290, -7, 150, WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, 0 )
{
	pushctl( m_lvType );
	pushctl( m_stGround );
	pushctl( m_tbGround );
	pushctl( m_stBgScenery );
	pushctl( m_cbBgScenery );
	pushctl( m_stBgControl );
	pushctl( m_cbBgControl );
	pushctl( m_cv );

	for ( auto & gr : m_grInit )
	{
		gr.bBgCtl = bgCtl0;
		gr.bBgScenery = bgScenery0;
		gr.bFgScenery = fgScenery0;
		gr.bGroundCtl = 0;
		gr.fCloudLevel = FALSE;
	}
}

BOOL CGroundDlg::OnInit( LPARAM lParam )
{
	INT id[ 2 ] = { 0 };
	INT idSel = -1;
	m_cv.SetView( m_pInterface );
	m_cv.ScrollTo( m_ngp.X / 16 );
	m_lvType.ExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	m_lvType.AddColumn( TEXT( "" ) );

	id[ ngtGroundMod ] = LOWORD( m_lvType.AddItem( TEXT( "Ground modifier" ), ngtGroundMod ) );
	id[ ngtSceneMod ] = LOWORD( m_lvType.AddItem( TEXT( "Background modifier" ), ngtSceneMod ) );

	m_tbGround.cSendMessage( TBM_SETRANGE, TRUE, MAKELONG( 1, 32 ) );
	
	for ( const auto & itGr : m_ngp.gr )
	{
		const NES_GROUND_TYPE ngt = itGr.first;
		const NES_GROUND_INFO & gr = itGr.second;

		m_lvType.Check( id[ ngt ], TRUE );
		if ( idSel < 0 )
		{
			m_lvType.CurSel( id[ ngt ] );
			idSel = id[ ngt ];
		}
	}

	for ( BYTE scenery = 0; scenery < bgCtlMax; ++scenery )
	{
		CString str;
		if ( !scenery )
		{
			str = TEXT( "No modifier" );
		}
		else if ( scenery < fgSceneryMax )
		{
			str.Format( TEXT( "Scenery #%d" ), scenery );
		}
		else if ( scenery >= bgCtl1 )
		{
			if ( m_ngp.X <= 24 )
			{
				str.Format( TEXT( "Background #%d" ), scenery - bgCtl1 + 1 );
			}
			else
			{
				continue;
			}
		}

		INT_PTR id = m_cbBgScenery.cSendMessage( CB_ADDSTRING, 0, (LPARAM)str.GetString() );
		m_cbBgScenery.cSendMessage( CB_SETITEMDATA, id, scenery );
	}
	
	for ( BYTE scenery = 0; scenery < bgSceneryMax; ++scenery )
	{
		CString str;
		if ( !scenery )
		{
			str = TEXT( "No modifier" );
		}
		else
		{
			str.Format( TEXT( "Scenery #%d" ), scenery );
		}

		INT_PTR id = m_cbBgControl.cSendMessage( CB_ADDSTRING, 0, (LPARAM)str.GetString() );
		m_cbBgControl.cSendMessage( CB_SETITEMDATA, id, scenery );
	}

	SetControls();

	SetFocus( m_lvType );
	m_cv.RenderView();
	InvalidateRect( m_cv, nullptr, TRUE );
	return FALSE;
}

VOID CGroundDlg::SetupControls( NES_GROUND_TYPE ngt )
{
	const auto itGround = m_ngp.gr.find( ngt );
	BOOL fEnable = ( itGround != m_ngp.gr.end() );

	switch ( ngt )
	{
		case ngtGroundMod:
			{
				EnableAll( fEnable, 0x1001, 0x102, 0x1003, 0x104, 0 );

				if ( fEnable )
				{
					const NES_GROUND_INFO & gr = itGround->second;
					m_cbBgControl.cSendMessage( CB_SETCURSEL, gr.bBgScenery );
					m_tbGround.cSendMessage( TBM_SETPOS, TRUE, gr.bGroundCtl + 1 );
				}
				else
				{
					m_cbBgControl.cSendMessage( CB_SETCURSEL, -1 );
					m_tbGround.cSendMessage( TBM_SETPOS, TRUE, 0 );
				}
				break;
			}
		case ngtSceneMod:
			{
				EnableAll( fEnable, 0x1002, 0x103, 0 );

				if ( fEnable )
				{
					const NES_GROUND_INFO & gr = itGround->second;

					if ( m_ngp.X <= 24 )
					{
						if ( gr.bBgCtl > 0 )
						{
							m_cbBgScenery.cSendMessage( CB_SETCURSEL, gr.bBgCtl );
						}
						else if ( gr.bFgScenery >= 0 )
						{
							m_cbBgScenery.cSendMessage( CB_SETCURSEL, gr.bFgScenery );
						}
					}
					else
					{
						m_cbBgScenery.cSendMessage( CB_SETCURSEL, gr.bFgScenery );
					}
				}
				else
				{
					m_cbBgScenery.cSendMessage( CB_SETCURSEL, -1 );
				}

				break;
			}
	}

}

VOID CGroundDlg::SetControls()
{
	INT idSel = LODWORD( m_lvType.CurSel() );
	BOOL fSel = ( idSel >= 0 );
	NES_GROUND_TYPE ngt = static_cast<NES_GROUND_TYPE>( m_lvType.Param( idSel ) );
	BOOL fChecked = m_lvType.Check( idSel );	

	ShowAll( fSel && ngt == ngtGroundMod, 0x1001, 0x102, 0x1003, 0x104, 0 );
	ShowAll( fSel && ngt == ngtSceneMod, 0x1002, 0x103, 0 );

	if ( fSel )
	{
		SetupControls( ngt );
	}
}

VOID CGroundDlg::OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl )
{
	if ( WM_HSCROLL == vScrollType && m_tbGround == hCtl )
	{
		UpdateView( LODWORD( m_lvType.CurSel() ) );
	}
}

VOID CGroundDlg::OnCommand( USHORT uCmd, USHORT uId, HWND hCtl )
{
	if ( CBN_SELENDOK == uCmd )
	{
		switch ( uId )
		{
			case 0x103:
			case 0x104: UpdateView( LODWORD( m_lvType.CurSel() ) ); break;
		}
	}
}

VOID CGroundDlg::UpdateView( INT idSel )
{
	BOOL fSel = ( idSel >= 0 );
	NES_GROUND_TYPE ngt = static_cast<NES_GROUND_TYPE>( m_lvType.Param( idSel ) );
	BOOL fChecked = m_lvType.Check( idSel );

	if ( !fSel )
	{
		return;
	}

	auto itGround = m_ngp.gr.find( ngt );
	BOOL fEnabled = ( itGround != m_ngp.gr.end() );

	if ( !fChecked && itGround != m_ngp.gr.end() )
	{
		m_grInit[ ngt ] = itGround->second;
		m_ngp.gr.erase( itGround );
	}
	else if ( fChecked )
	{
		NES_GROUND_INFO & gr = m_ngp.gr[ ngt ];
		if ( !fEnabled )
		{
			gr = m_grInit[ ngt ];
			SetupControls( ngt );
		}

		switch ( ngt )
		{
			case ngtGroundMod:
				{
					BYTE bgScenery = min( LOBYTE( m_cbBgControl.cSendMessage( CB_GETCURSEL ) ), bgSceneryMax - 1 );
					gr.bBgScenery = static_cast<NES_BG_SCENERY>( bgScenery );
					gr.bGroundCtl = min( LOBYTE( m_tbGround.cSendMessage( TBM_GETPOS ) ) - 1, 31 );
					break;
				}
			case ngtSceneMod:
				{
					BYTE scenery = min( LOBYTE( m_cbBgScenery.cSendMessage( CB_GETCURSEL ) ), bgCtlMax - 1 );
					if ( m_ngp.X <= 24 )
					{
						if ( scenery < fgSceneryMax )
						{
							gr.bBgCtl = bgCtl0;
							gr.bFgScenery = static_cast<NES_FG_SCENERY>( scenery );
						}
						else if ( scenery >= bgCtl1 && scenery < bgCtlMax )
						{
							gr.bFgScenery = fgScenery0;
							gr.bBgCtl = static_cast<NES_BG_COLOR_CTL>( scenery );
						}
					}
					else
					{
						gr.bBgCtl = bgCtl0;
						if ( scenery < fgSceneryMax )
						{
							gr.bFgScenery = static_cast<NES_FG_SCENERY>( scenery );
						}
					}
				}
		}
	}

	m_pLevel->SetGround( m_ngp.X, m_ngp.gr );

	m_cv.RenderView();
	InvalidateRect( m_cv, nullptr, TRUE );
}

INT_PTR CGroundDlg::OnNotify( LPNMHDR lpnm )
{
	switch ( lpnm->idFrom )
	{
		case 0x101: 
			{
				switch ( lpnm->code )
				{
					case LVN_ITEMCHANGED: return LvControl( (NMLISTVIEW*)lpnm ); break;
				}
			}
	}

	return 0;
}

INT_PTR CGroundDlg::LvControl( NMLISTVIEW * pnmlv )
{
	if ( pnmlv->uNewState & LVIS_SELECTED && !( pnmlv->uOldState & LVIS_SELECTED ) )
	{
		SetControls();
	}
	else if ( !( pnmlv->uNewState & LVIS_SELECTED ) && ( pnmlv->uOldState & LVIS_SELECTED ) )
	{
		ShowAll( FALSE, 0x1001, 0x102, 0x1003, 0x104, 0x1002, 0x103, 0 );
	}
	else if ( ( pnmlv->uNewState & LVIS_STATEIMAGEMASK ) != ( pnmlv->uOldState & LVIS_STATEIMAGEMASK ) )
	{
		UpdateView( pnmlv->iItem );
		SetControls();
	}

	return 0;
}

VOID CGroundDlg::Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, const NES_GROUND_PLACE & ngp )
{
	m_pLevel = pLevel;
	m_pInterface = pInterface;
	m_ngp = ngp;
	pLevel->SetGround( ngp.X, ngp.gr );
}

VOID CGroundDlg::Get_Param( NES_GROUND_PLACE & ngp )
{
	ngp = m_ngp;
}

VOID CGroundDlg::OnOK()
{
	if ( !m_ngp.gr.size() )
	{
		if ( !Confirm( TEXT( "Warning! Selected ground will be removed!\nAre you sure that you want to continue?" ) ) )
		{
			return;
		}
	}
	Close( 1 );
}