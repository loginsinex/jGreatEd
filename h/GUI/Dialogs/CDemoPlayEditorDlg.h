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

class CDemoPlayEditorDlg: public CDialog
{
	std::vector<std::pair<BYTE, BYTE>>		m_vDemoPlay;

	CFont									m_fntListview;
	CListView								m_lv;
	CCheckBoxControl						m_btnCtrl[ 6 ];
	CButtonControl							m_btnAdd;
	CButtonControl							m_btnDel;
	CButtonControl							m_btnOK;
	CButtonControl							m_btnApply;
	CButtonControl							m_btnCancel;
	CEditControl							m_edTimer;
	CUpDownControl							m_udTimer;

	BOOL									OnInit( LPARAM lParam ) override;
	VOID									OnButton( USHORT uId ) override;
	INT_PTR									OnNotify( LPNMHDR lpnm ) override;
	VOID									OnCommand( USHORT uCmd, USHORT uId, HWND hCtl );
	VOID									OnOK() override;

	VOID									SaveInput();

	CString									GetBitsString( BYTE bBits );
	VOID									SetBits( BYTE bBits );
	VOID									SetTiming( BYTE bTiming );
	VOID									UpdateFields();

	VOID									NotifyUpdate();

public:
	CDemoPlayEditorDlg( HINSTANCE hInstance );

	VOID									Set_Param( const std::vector<std::pair<BYTE, BYTE>> & vDemoPlay );
	VOID									Get_Param( std::vector<std::pair<BYTE, BYTE>> & vDemoPlay );
};