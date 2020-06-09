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

class CAreaPropertiesDlg : public CDialog
{
	CCanvasCtl				m_cv;
	CNesLevel				*m_pLevel;
	CNesLevelInterface		*m_pInterface;
	BOOL					m_fInitialCloudStatus;

	CStaticControl			m_stGround;
	CControl				m_tbGround;
	CStaticControl			m_stForeground;
	CControl				m_cbForeground;
	CStaticControl			m_stBackground;
	CControl				m_cbBackground;
	CStaticControl			m_stPlayerCtl;
	CControl				m_cbPlayerCtl;
	CStaticControl			m_stTimer;
	CControl				m_cbTimer;
	CStaticControl			m_stAreaStyle;
	CControl				m_cbAreaStyle;

	INT_PTR					PushString( CControl & cbCtl, LPARAM lData, LPCTSTR pszString, ... );
	VOID					InitCB( CControl & cbCtl, LPCTSTR pszString, int from, int to, int selected );

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnOK() override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;
	VOID					OnScroll( UINT vScrollType, USHORT uPos, USHORT uRequest, HWND hCtl ) override;

	VOID					HandleCB( USHORT uId );
	VOID					RenderView();

public:
	CAreaPropertiesDlg( HINSTANCE hInstance );

	VOID					Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface );
	VOID					Get_Param( CNesLevelHeader & header );
};