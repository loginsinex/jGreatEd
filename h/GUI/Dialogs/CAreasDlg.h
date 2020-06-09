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

class CAreasDlg : public CDialog
{
	CNesLevelsKeeper	&	m_lvlKeeper;

	const BYTE				m_uAutoLevel;

	CPaletteBitmap			m_bm;
	CListView				m_lv;
	CEditControl			m_ed;
	CUpDownControl			m_ud;
	CCanvasCtl				m_cv;
	CButtonControl			m_btnFreePipe;

	CNesLevelItem *			m_pObj;
	CNesLevel *				m_pLevel;
	CNesLevel *				m_pParentLevel;
	BYTE					m_uTargetPage;
	BYTE					m_uWorld;
	CNesLevelInterface *	m_pInterface;
	BOOL					m_fFreePointers;

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnDestroy() override;
	VOID					OnButton( USHORT uId ) override;

	VOID					SelectLevel( CNesLevel * pLevel );
	VOID					ScrollTo( BYTE uTargetPage );

	INT_PTR					OnNotify( LPNMHDR lpnm )  override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;
	VOID					OnOK() override;


public:
	CAreasDlg( HINSTANCE hInstance, CNesLevelsKeeper * pKeeper, BYTE uAutoLevel = 0 );
	

	VOID					Set_Obj( BYTE bWorld, CNesLevelItem * pObj );
	VOID					Set_Obj( BYTE bWorld, CNesLevel * pParentLevel, CNesLevel * pTargetLevel, BYTE uTargetPage );
	VOID					Get_Param( CNesLevel ** pLevel, BYTE & uTargetPage, BOOL & fFreePointers );
};