
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

class CListView: public CControl
{
	HWND		m_hHeader;

public:
	INT_PTR		AddGroup(INT iGroupId, DWORD pszText, LPCTSTR pszFooter = NULL, 
							BOOL fCollapsed = FALSE, BOOL fHidden = FALSE, 
							BOOL fNoHeader = FALSE, BOOL fCollapsible = FALSE,
							BOOL fFocused = FALSE, BOOL fSelected = FALSE);

	INT_PTR		AddGroup(INT iGroupId, LPCTSTR pszText, LPCTSTR pszFooter = NULL, 
							BOOL fCollapsed = FALSE, BOOL fHidden = FALSE, 
							BOOL fNoHeader = FALSE, BOOL fCollapsible = FALSE,
							BOOL fFocused = FALSE, BOOL fSelected = FALSE);

	VOID		EnableGroups(BOOL fEnable);
	BOOL		EnableGroups();

	INT_PTR		AddItem(DWORD pszText, LPARAM lParam, int iGroup = I_GROUPIDNONE, int iImage = -1);
	INT_PTR		AddItem(LPCTSTR pszText, LPARAM lParam, int iGroup = I_GROUPIDNONE, int iImage = -1);
	INT_PTR		InsertItem(INT iAfterItem, LPCTSTR pszText, LPARAM lParam, int iGroup = I_GROUPIDNONE, int iImage = -1);
	void		SetItem(INT iItem, INT iSubItem, LPCTSTR pszText, int iGroup = I_GROUPIDNONE);
	void		SetItem(INT iItem, INT iSubItem, DWORD dwValue, int iGroup = I_GROUPIDNONE);
	void		SetItemImage(INT iItem, INT idImage = -1, INT idSelImage = -1);
	void		DeleteItem(INT iItem);
	void		Param(INT iItem, LPARAM lParam);
	LPARAM		Param(INT iItem);
	void		ResetContent();
	void		CurSel(INT iItem);
	void		EnsureVisible(BOOL fPartialOk = FALSE, INT_PTR id = -1);
	INT_PTR		CurSel();
	INT_PTR		Count();
	INT			State(INT iItem);
	void		State(INT iItem, INT iState);
	BOOL		Selected(INT iItem);
	void		Selected(INT iItem, BOOL fSelected);
	BOOL		Check(INT iItem);
	void		Check(INT iItem, BOOL fCheck);
	BOOL		ItemRect(INT iItem, INT code, LPRECT lprc);
	INT			ColumnWidth(INT iColumn);
	void		ScrollToVisible();
	BOOL		GetItemText(INT iItem, INT iSubItem, tstring & text);
	tstring		GetItemText(INT iItem, INT iSubItem);
	void		AddColumn(LPCTSTR pszText, INT cx = -1);
	void		ExtendedStyle(DWORD dwStyle);
	DWORD		ExtendedStyle();

	// CListView() : m_hHeader( NULL ) { };
	CListView(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx = 0, WORD wAnchor = 0);
};

