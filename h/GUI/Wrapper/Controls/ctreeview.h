
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

class CTreeView: public CControl
{
	BOOL		ReqItem(HTREEITEM hItem, LPTV_ITEM lptvi);
	BOOL		UpdItem(HTREEITEM hItem, LPTV_ITEM lptvi);

public:
	CTreeView(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx = 0, WORD wAnchor = 0);

	HTREEITEM	AddItem(HTREEITEM hParentItem, LPCTSTR lpszText, LPARAM lParam, 
					BOOL bBold = FALSE, BOOL bSelected = FALSE, 
					BOOL bExpanded = FALSE, HTREEITEM hInsertAfter = TVI_LAST,
					int idImage = -1, int idSelImage = -1);

	HTREEITEM	GetItem(DWORD dwFlag, HTREEITEM hFrom = NULL);
	HTREEITEM	GetCurSel();
	void		SetCurSel(HTREEITEM hItem);

	void		Param(HTREEITEM hItem, LPARAM lParam);
	LPARAM		Param(HTREEITEM hItem);
	void		Text(HTREEITEM hItem, LPCTSTR lpszText);
	tstring		Text(HTREEITEM hItem);
	void		State(HTREEITEM hItem, DWORD dwState);
	DWORD		State(HTREEITEM hItem);
	void		CheckItem(HTREEITEM hTree, BOOL bCheck);
	BOOL		CheckItem(HTREEITEM hTree);
	void		ResetContent();
};
