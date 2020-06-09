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

class CPaletteChooseDlg : public CDialog
{
	const HWND		m_hTooltip;
	CControl		m_btnColor[ 64 ];
	CString			m_strTip[ 64 ];
	CControl		m_stColor;
	CStaticControl	m_stColorId;
	BYTE			m_bColor;

	BOOL			OnInit( LPARAM lParam ) override;
	VOID			OnOK() override;
	VOID			OnButton( USHORT uId ) override;

public:
	CPaletteChooseDlg( HINSTANCE hInstance );
	~CPaletteChooseDlg();

	VOID			Set_Param( BYTE bColor );
	VOID			Get_Param( BYTE & bColor );
};

class CPaletteEditorDlg : public CDialog
{
	CControl			m_stColor[ 8 ];
	CControl			m_btnColor[ 24 ];
	NES_PAL_ARRAY		m_npa;
	BOOL				m_fDisableShared;
	std::map<USHORT, std::pair<USHORT, USHORT>> m_btnMap;

	VOID				OnOK() override;
	VOID				OnButton( USHORT uId ) override;
	BOOL				OnInit( LPARAM lParam ) override;
	VOID				OnNCDestroy() override;

	VOID				UpdateBitmap( USHORT uId, BYTE bCol );


public:
	CPaletteEditorDlg( HINSTANCE hInstance );

	VOID				Set_Param( const NES_PAL_ARRAY & npa, BOOL fDisableShared );
	VOID				Get_Param( NES_PAL_ARRAY & npa );
};

class CPaletteDlg : public CDialog
{
	CStaticControl		m_stPaletteId;
	CControl			m_cbPaletteId;
	CButtonControl		m_btnEdit;

	std::map<NES_PAL_TABLE, NES_PAL_ARRAY>	m_mPal;

	VOID				OnOK() override;
	BOOL				OnInit( LPARAM lParam ) override;
	VOID				OnButton( USHORT uId ) override;
	LPCTSTR				GetPalString( NES_PAL_TABLE npt );

public:
	CPaletteDlg( HINSTANCE hInstance );

	VOID				Set_Param( const std::map<NES_PAL_TABLE, NES_PAL_ARRAY> & mPal );
	VOID				Get_Param( std::map<NES_PAL_TABLE, NES_PAL_ARRAY> & mPal );
};

class CPaletteBgDlg : public CDialog
{
	NES_BG_COLORS			m_bgColors;
	BYTE					m_bColor;

	CStaticControl			m_stBgColor;
	CControl				m_cbBgColor;
	CButtonControl			m_btnEdit;
	CControl				m_stColor;

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnOK() override;
	VOID					OnCommand( USHORT uCmd, USHORT uID, HWND hCtl ) override;
	VOID					OnButton( USHORT uId ) override;
	VOID					OnNCDestroy() override;

	VOID					SelectColor( BYTE bColor );
	VOID					UpdateColor( BYTE bColor );

public:
	CPaletteBgDlg( HINSTANCE hInstance );

	VOID					Set_Param( const NES_BG_COLORS & nbg );
	VOID					Get_Param( NES_BG_COLORS & nbg );

};