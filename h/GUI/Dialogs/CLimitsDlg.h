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

class CLimitsDlg : public CDialog
{
	CListView								m_lv;
	CButtonControl							m_btnClose;
	std::map<NES_AREA_SET, NES_LIMITS>		m_mLimits;

	BOOL									OnInit( LPARAM lParam );

	VOID									InsertLimits( INT iGroupId, const NES_LIMITS & as );
	VOID									InsertSharedSection( INT iGroupId, size_t uCurrent, size_t uMax );

public:
	CLimitsDlg( HINSTANCE hInstance );
	VOID									Set_Param( const std::map<NES_AREA_SET, NES_LIMITS> & mLimits );
};