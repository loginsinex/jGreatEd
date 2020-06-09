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

class CCreateNewAreaDlg : public CDialog
{
	CNesLevelsKeeper			* m_pKeeper;
	CNesLevel					* m_pLevel;
	BYTE						m_bWorld;
	NES_LEVEL_TYPE				m_uType;
	BOOL						m_fAutoLevel;
	BOOL						m_fSharedSection;

	CRadioControl				m_rdNewArea;
	CStaticControl				m_stType;
	CControl					m_cbType;
	CCheckBoxControl			m_chkAuto;
	CCheckBoxControl			m_chkShared;
	CRadioControl				m_rdBaseArea;
	CStaticControl				m_stArea;
	CEditControl				m_edArea;
	CButtonControl				m_btnBrowse;

	std::map<NES_LEVEL_TYPE, CString>	m_mType;

	BOOL						OnInit( LPARAM lParam ) override;
	VOID						OnButton( USHORT uId ) override;
	VOID						OnCommand( USHORT uCmd, USHORT uId, HWND hCtl );
	VOID						OnOK();


public:
	CCreateNewAreaDlg( HINSTANCE hInstance );

	VOID						Set_Param( BYTE bWorld, CNesLevelsKeeper * pKeeper, CNesLevel * pLevel );
	VOID						Get_Param( NES_LEVEL_TYPE & uType, BOOL & fAutoLevel, BOOL & fSharedSection, CNesLevel ** pBaseLevel );
};