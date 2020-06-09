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

class CLoopSettingsDlg: public CDialog
{
	CNesLevel				* m_pLevel;
	CNesLevelInterface		* m_pLevelInterface;
	CNesLoopCommand			* m_pLoop;
	CCanvasCtl				m_cv;

	CStaticControl			m_stPage;
	CEditControl			m_edPage;
	CUpDownControl			m_udPage;
	CStaticControl			m_stHeight;
	CEditControl			m_edHeight;
	CUpDownControl			m_udHeight;
	CCheckBoxControl		m_chkConnected;

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnOK() override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;
	VOID					OnButton( USHORT uId ) override;

	VOID					Render();

public:
	CLoopSettingsDlg( HINSTANCE hInstance );

	VOID					Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, size_t idObject );
	VOID					Get_Param( NES_LOOP & nlp );
};