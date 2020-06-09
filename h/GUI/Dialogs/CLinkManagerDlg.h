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

typedef std::pair<UINT, CNesLevel*>		OBJECT_LINK, *POBJECT_LINK;
typedef std::map<BYTE, std::pair<OBJECT_LINK, BOOL>>	WORLD_OBJECT_LINK, *PWORLD_OBJECT_LINK;
typedef std::map<CNesLevelItem*, WORLD_OBJECT_LINK>		LEVEL_ITEM_LINK, *PLEVEL_ITEM_LINK;

typedef enum _tagTREE_OBJECT_LINK_ITEM_TYPE
{
	tiObject,
	tiLink
} TREE_OBJECT_LINK_ITEM_TYPE, *PTREE_OBJECT_LINK_ITEM_TYPE;

typedef struct _tagTREE_OBJECT_LINK_ITEM
{
	TREE_OBJECT_LINK_ITEM_TYPE		ti;
	CNesLevelItem					*pObject;
	OBJECT_LINK						link;
	BYTE							bWorld;
	HTREEITEM						hTreeItem;
} TREE_OBJECT_LINK_ITEM, *PTREE_OBJECT_LINK_ITEM;

class CLinkManagerDlg : public CDialog
{
	CNesObjectsNames		m_name;
	CNesLevelsKeeper		* m_pKeepers[ asWorldSetsCount ];
	LEVEL_ITEM_LINK			m_mLinks;
	BYTE					m_bWorld;
	CPaletteBitmap			m_bmView;
	CNesLevel				* m_pCurrentLevel;

	std::vector<TREE_OBJECT_LINK_ITEM*>	m_vItemBuffer;

	CTreeView				m_tv;
	CCanvasCtl				m_cv;
	CStaticControl			m_stInfo;
	
	VOID					SelectItem( HTREEITEM hItem );
	VOID					FreeCanvasInterface();
	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnDestroy() override;
	INT_PTR					OnNotify( LPNMHDR lpnm ) override;
	VOID					OnOK() override;
	TREE_OBJECT_LINK_ITEM	& GetNewItem( TREE_OBJECT_LINK_ITEM_TYPE ti, CNesLevelItem * pItem );
	VOID					CheckItem( HTREEITEM hTreeItem, BOOL fCheck );
	INT_PTR					TVItemChanged( NMTVITEMCHANGE* pnmtv );
	BOOL					IsAnySelected();

	CNesLevelsKeeper *		World2Keeper( size_t bWorld );

public:
	CLinkManagerDlg( HINSTANCE hInstance );
	~CLinkManagerDlg();

	VOID					Set_Param( CNesLevelsKeeper ** pKeeper, const LEVEL_ITEM_LINK & lnk, CNesLevel * pCurrentLevel, BYTE bWorld );
	VOID					Get_Param( LEVEL_ITEM_LINK & lnk );
};