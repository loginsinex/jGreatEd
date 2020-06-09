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

class CNewObjectDlg : public CDialog
{
	CNesLevel				*m_pLevel;
	CNesLevelInterface		*m_pInterface;
	CListView				m_lv;
	CCanvasCtl				m_cv;
	CCheckBoxControl		m_chkHard;
	BYTE					m_bWorld;

	CNesObject				*m_pObject;
	LPARAM					m_lParam;

	BOOL					m_fIsSprite;
	BOOL					m_fSpriteHardmode;
	NES_LEVEL_ITEM			m_lvlItem;
	NES_LEVEL_ENEMY_ITEM	m_enmItem;
	POINT					m_pt;

	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnOK() override;
	INT_PTR					OnNotify( LPNMHDR lpnm ) override;
	VOID					OnButton( USHORT uId ) override;
	VOID					OnDestroy() override;

	VOID					AddItemsToGroup( UINT uGroup, UINT uResStr );

	template <class T> VOID AddItemsToList( UINT uGroup, UINT uResStr, UINT uDiff, UINT uMiltiplier, UINT uFlag, const T & arr );

	VOID					TestItem( LPARAM lParam );

public:
	CNewObjectDlg( HINSTANCE hInstance );

	VOID					Set_Param( CNesLevel * pLevel, CNesLevelInterface * pInterface, BYTE bWorld, POINT pt );
	BOOL					IsSpriteCreated();
	VOID					Get_Param( NES_LEVEL_ITEM & lvlItem );
	VOID					Get_Param( NES_LEVEL_ENEMY_ITEM & enmItem );
};