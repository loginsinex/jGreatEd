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

class CPageReferencesDlg : public CDialog
{
	CNesLevelInterface							* m_pInterface;
	std::vector<std::pair<BYTE, CNesLevel*>>	m_vLinks;
	UINT										m_uPage;
	UINT										m_uMaxPage;
	std::pair<BYTE, CNesLevel*>					m_tg;

	CCanvasCtl			m_cv;
	CListView			m_lv;
	CEditControl		m_edPage;
	CUpDownControl		m_udPage;
	CButtonControl		m_btnJump;

	BOOL				OnInit( LPARAM lParam ) override;
	VOID				OnOK() override;
	VOID				OnButton( USHORT uId ) override;
	VOID				OnCommand( USHORT uCmd, USHORT uId, HWND hCtl );

public:
	CPageReferencesDlg( HINSTANCE hInstance );


	VOID				Set_Param( CNesLevelInterface * pInterface, UINT uPage, UINT uMaxPage, std::vector<std::pair<BYTE, CNesLevel*>>& vLinks );
	VOID				Get_Param( UINT & uPage, std::pair<BYTE, CNesLevel*> & tg );
};