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

#include "Controls.h"

// --- listview

CListView::CListView(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx, WORD wAnchor)
		: m_hHeader( NULL ), CControl(hInstance, uId, WC_LISTVIEW, NULL, x, y, cx, cy, dwStyle, dwStyleEx, wAnchor)
{

}

INT_PTR CListView::Count()
{
	return cSendMessage(LVM_GETITEMCOUNT, 0, 0);
}

INT_PTR CListView::AddGroup(INT iGroupId, DWORD pszText, LPCTSTR pszFooter, 
							BOOL fCollapsed, BOOL fHidden, 
							BOOL fNoHeader, BOOL fCollapsibleE,
							BOOL fFocused, BOOL fSelected)
{
	TCHAR pszcText[64] = { 0 };
	wsprintf(pszcText, TEXT("%d"), pszText);
	return AddGroup(iGroupId, pszcText, pszFooter, fCollapsed, fHidden, fNoHeader, fCollapsibleE, fFocused, fSelected);
}

INT_PTR CListView::AddGroup(INT iGroupId, LPCTSTR pszText, LPCTSTR pszFooter, 
							BOOL fCollapsed, BOOL fHidden, 
							BOOL fNoHeader, BOOL fCollapsible,
							BOOL fFocused, BOOL fSelected)
{
	LVGROUP lvg = { 0 };
	lvg.cbSize = sizeof(lvg);
	lvg.iGroupId = iGroupId;

	lvg.pszHeader = (LPTSTR) pszText;
	lvg.cchHeader = lstrlen( pszText );

	lvg.pszFooter = (LPTSTR) pszFooter;
	lvg.cchFooter = lstrlen( pszFooter );

	lvg.state = LVGS_NORMAL;
	if ( fCollapsed ) lvg.state |= LVGS_COLLAPSED;
	if ( fHidden ) lvg.state |= LVGS_HIDDEN;
	if ( fNoHeader ) lvg.state |= LVGS_NOHEADER;
	if ( fCollapsible ) lvg.state |= LVGS_COLLAPSIBLE;
	if ( fFocused ) lvg.state |= LVGS_FOCUSED;
	if ( fSelected ) lvg.state |= LVGS_SELECTED;

	lvg.mask = LVGF_NONE;	
	if ( lvg.cchFooter ) lvg.mask |= LVGF_FOOTER;
	
	lvg.mask |= LVGF_HEADER;
	lvg.mask |= LVGF_STATE;	
	lvg.mask |= LVGF_GROUPID;

	return cSendMessage(LVM_INSERTGROUP, -1, (LPARAM) &lvg);
}

VOID CListView::EnableGroups(BOOL fEnable)
{
	cSendMessage(LVM_ENABLEGROUPVIEW, fEnable);
}

BOOL CListView::EnableGroups()
{
	return (BOOL) cSendMessage(LVM_ISGROUPVIEWENABLED);
}

INT_PTR CListView::AddItem(DWORD dwValue, LPARAM lParam, int iGroup, int iImage)
{
	TCHAR pszText[32];
	wsprintf(pszText, TEXT("%d"), dwValue);
	return AddItem(pszText, lParam, iGroup, iImage);
}

INT_PTR CListView::AddItem(LPCTSTR pszText, LPARAM lParam, int iGroup, int iImage)
{
	INT_PTR iItems = cSendMessage(LVM_GETITEMCOUNT, 0, 0);
	return InsertItem((INT) iItems, pszText, lParam, iGroup, iImage);
}

INT_PTR CListView::InsertItem(INT iAfterItem, LPCTSTR pszText, LPARAM lParam, int iGroup, int iImage)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT | LVIF_PARAM | ( I_GROUPIDNONE != iGroup ? LVIF_GROUPID : 0 ) | ( -1 != iImage ? LVIF_IMAGE : 0 );
	lvi.pszText = LPTSTR(pszText);
	lvi.lParam = lParam;
	lvi.iItem = iAfterItem;
	lvi.iGroupId = iGroup;
	lvi.iImage = iImage;
	return cSendMessage(LVM_INSERTITEM, 0, LPARAM(&lvi));
}

void CListView::CurSel(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_STATE;
	lvi.state = LVIS_FOCUSED | LVIS_SELECTED | ( State( iItem ) & LVIS_STATEIMAGEMASK );
	lvi.stateMask = -1;
	lvi.iItem = iItem;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

INT_PTR CListView::CurSel()
{
	return cSendMessage(LVM_GETNEXTITEM, -1, MAKELPARAM(LVIS_FOCUSED | LVIS_SELECTED, 0));
}

void CListView::EnsureVisible(BOOL fPartialOk, INT_PTR id)
{
	if ( -1 == id )
		id = CurSel();

	cSendMessage(LVM_ENSUREVISIBLE, id, fPartialOk);
}

void CListView::DeleteItem(INT iItem)
{
	cSendMessage(LVM_DELETEITEM, iItem, 0);
}

void CListView::ResetContent()
{
	cSendMessage(LVM_DELETEALLITEMS, 0, 0);
}

void CListView::SetItem(INT iItem, INT iSubItem, LPCTSTR pszText, int iGroup)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT | ( I_GROUPIDNONE != iGroup ? LVIF_GROUPID : 0 );
	lvi.iItem = iItem;
	lvi.iSubItem = iSubItem;
	lvi.pszText = LPTSTR(pszText);
	lvi.iGroupId = iGroup;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

void CListView::SetItem(INT iItem, INT iSubItem, DWORD dwValue, int iGroup)
{
	TCHAR pszText[32];
	wsprintf(pszText, TEXT("%d"), dwValue);
	SetItem(iItem, iSubItem, pszText, iGroup);
}

void CListView::SetItemImage(INT iItem, INT idImage, INT idSelImage)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = ( idImage >= 0 || idSelImage >= 0 ? LVIF_IMAGE : 0 );
	lvi.iItem = iItem;
	lvi.iImage = idImage;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

void CListView::Param(INT iItem, LPARAM lParam)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.lParam = lParam;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

LPARAM CListView::Param(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iItem;
	cSendMessage(LVM_GETITEM, 0, LPARAM(&lvi));
	return lvi.lParam;
}

void CListView::AddColumn(LPCTSTR pszText, INT cx)
{
	//HDM_GETITEMCOUNT
	//if ( !m_hHeader ) 
	m_hHeader = (HWND) cSendMessage(LVM_GETHEADER); // FindWindowEx(GetWindowHandle(), 0, TEXT("SysHeader32"), NULL);

	HWND hHeader = m_hHeader;
	if ( !hHeader ) return;

	INT c = (INT) SendMessage(hHeader, HDM_GETITEMCOUNT, 0, 0);
	if ( cx == -1 )
	{
		RECT rc = { 0 };
		GetClientRect(*this, &rc);
		
		INT icx = 0;
		for(INT i = 0; i < c; ++i)
		{
			HD_ITEM hdi = { 0 };
			hdi.mask = HDI_WIDTH;
			SendMessage(hHeader, HDM_GETITEM, i, LPARAM(&hdi));
			icx += hdi.cxy;
		}

		if ( icx >= rc.right ) cx = 100;
		else cx = rc.right - icx - GetSystemMetrics(SM_CXVSCROLL);
	}
	else if ( !cx )
	{
		HDC hDC = GetDC(hHeader);
		RECT rc = { 0 };
		DrawText(hDC, pszText, lstrlen(pszText), &rc, DT_CALCRECT);
		ReleaseDC(hHeader, hDC);
		cx = rc.right + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
	}

	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = cx;
	lvc.pszText = LPTSTR(pszText);
	cSendMessage(LVM_INSERTCOLUMN, c, LPARAM(&lvc));
}

void CListView::ExtendedStyle(DWORD dwStyle)
{
	cSendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, -1, dwStyle);
}

DWORD CListView::ExtendedStyle()
{
	return (DWORD) cSendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
}

INT CListView::State(INT iItem)
{
	return (INT) cSendMessage(LVM_GETITEMSTATE, iItem, -1);
}

void CListView::State(INT iItem, INT iState)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_STATE;
	lvi.iItem = iItem;
	lvi.state = iState;
	lvi.stateMask = -1;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

BOOL CListView::Selected(INT iItem)
{
	return ( LVIS_SELECTED == ( State(iItem) & LVIS_SELECTED ) );
}

void CListView::Selected(INT iItem, BOOL fSelected)
{
	State(iItem, 
		fSelected
		?
		( State(iItem) | LVIS_SELECTED )
		:
		( State(iItem) & ~LVIS_SELECTED )
	);
}

BOOL CListView::Check(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.iItem = iItem;
	lvi.stateMask = (3 << 12);
	lvi.mask = LVIF_STATE;
	cSendMessage(LVM_GETITEM, 0, (LPARAM) &lvi);
	return ( 2 == ((lvi.state >> 12) & 3));
}

void CListView::Check(INT iItem, BOOL fCheck)
{
	LV_ITEM lvi = { 0 };
	lvi.iItem = iItem;
	lvi.stateMask = (3 << 12);
	lvi.state = ( (fCheck ? 2 : 1) << 12 );
	lvi.mask = LVIF_STATE;
	cSendMessage(LVM_SETITEM, 0, (LPARAM) &lvi);
}

BOOL CListView::ItemRect(INT iItem, INT code, LPRECT lprc)
{
	lprc->left = code;
	BOOL ret = (BOOL) cSendMessage(LVM_GETITEMRECT, iItem, LPARAM(lprc));
	lprc->left = 0;
	return ret;
}

INT CListView::ColumnWidth(INT iColumn)
{
	return (INT) cSendMessage(LVM_GETCOLUMNWIDTH, iColumn, 0);
}

void CListView::ScrollToVisible()
{
	INT_PTR c = CurSel();
	if ( -1 == c ) return;
	cSendMessage(LVM_ENSUREVISIBLE, c, FALSE);
}

BOOL CListView::GetItemText(INT iItem, INT iSubItem, tstring & text)
{
	BOOL result = FALSE;

	text.clear();
	text.append(8192, 0);

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iItem;
	lvi.iSubItem = iSubItem;
	lvi.cchTextMax = 8192;
	lvi.pszText = LPTSTR(&text[0]);
	if ( cSendMessage(LVM_GETITEM, 0, LPARAM(&lvi)) )
		result = TRUE;

	return result;
}

tstring CListView::GetItemText(INT iItem, INT iSubItem)
{
	tstring sText;
	if ( GetItemText(iItem, iSubItem, sText) )
		return sText;

	return TEXT("");
}

