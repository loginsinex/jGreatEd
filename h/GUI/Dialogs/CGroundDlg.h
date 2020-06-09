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

class CGroundDlg: public CDialog
{
	CCanvasCtl				m_cv;
	CListView				m_lvType;
	CStaticControl			m_stGround;
	CControl				m_tbGround;
	CStaticControl			m_stBgScenery;
	CControl				m_cbBgScenery;
	CStaticControl			m_stBgControl;
	CControl				m_cbBgControl;
	NES_GROUND_INFO			m_grInit[ 2 ];

	NES_GROUND_PLACE		m_ngp;
	CNesLevel				*m_pLevel;
	CNesLevelInterface		*m_pInterface;

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;
	VOID					OnOK() override;
	INT_PTR					OnNotify( LPNMHDR lpnm );
	INT_PTR					LvControl( NMLISTVIEW * pnmlv );

	VOID					SetControls();
	VOID					SetupControls( NES_GROUND_TYPE ngt );
	VOID					UpdateView( INT idSel );

public:
	CGroundDlg( HINSTANCE hInstance );

	VOID					Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, const NES_GROUND_PLACE & ngp );
	VOID					Get_Param( NES_GROUND_PLACE & ngp );
};