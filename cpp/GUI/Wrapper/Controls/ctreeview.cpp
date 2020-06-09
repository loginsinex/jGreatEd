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

CTreeView::CTreeView(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx, WORD wAnchor)
		: CControl(hInstance, uId, WC_TREEVIEW, NULL, x, y, cx, cy, dwStyle, dwStyleEx, wAnchor)
{

}

HTREEITEM CTreeView::AddItem(HTREEITEM hParentItem, LPCTSTR lpszText, LPARAM lParam, BOOL bBold, BOOL bSelected, BOOL bExpanded, HTREEITEM hInsertAfter, 
	int idImage, int idSelImage)
{
	TV_INSERTSTRUCT tvis = { 0 };
	tvis.hParent = hParentItem;
	tvis.hInsertAfter = hInsertAfter;

	tvis.item.mask = TVIF_PARAM | TVIF_STATE | TVIF_TEXT | ( idImage > 0 ? TVIF_IMAGE : 0 ) | ( idSelImage > 0 ? TVIF_SELECTEDIMAGE : 0 );
	tvis.item.state = ( bBold ? TVIS_BOLD : 0 ) | ( bSelected ? TVIS_SELECTED : 0 ) | ( bExpanded ? TVIS_EXPANDED : 0 );
	tvis.item.stateMask = TVIS_BOLD | TVIS_SELECTED | TVIS_EXPANDED;
	tvis.item.pszText = LPTSTR(lpszText);
	tvis.item.lParam = lParam;
	tvis.item.iImage = idImage;
	tvis.item.iSelectedImage = idSelImage;
	return HTREEITEM(cSendMessage(TVM_INSERTITEM, 0, LPARAM(&tvis)));
}

HTREEITEM CTreeView::GetItem(DWORD dwFlag, HTREEITEM hFrom)
{
	return HTREEITEM(cSendMessage(TVM_GETNEXTITEM, dwFlag, LPARAM(hFrom)));
}

HTREEITEM CTreeView::GetCurSel()
{
	return GetItem(TVGN_CARET, NULL);
}

BOOL CTreeView::ReqItem(HTREEITEM hItem, LPTV_ITEM lptvi)
{
	lptvi->mask |= TVIF_HANDLE; // | TVIF_IMAGE | TVIF_PARAM	| TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;
	lptvi->hItem = hItem;
	return (BOOL) cSendMessage(TVM_GETITEM, 0, LPARAM(lptvi));
}

BOOL CTreeView::UpdItem(HTREEITEM hItem, LPTV_ITEM lptvi)
{
	lptvi->mask |= TVIF_HANDLE;
	lptvi->hItem = hItem;
	return (BOOL) cSendMessage(TVM_SETITEM, 0, LPARAM(lptvi));
}

LPARAM CTreeView::Param(HTREEITEM hItem)
{
	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_PARAM;
	if ( ReqItem(hItem, &tvi) ) return tvi.lParam;
	return 0;
}

void CTreeView::Param(HTREEITEM hItem, LPARAM lParam)
{
	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_PARAM;
	tvi.lParam = lParam;
	UpdItem(hItem, &tvi);
}

tstring CTreeView::Text(HTREEITEM hItem)
{
	CString str;

	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_TEXT;
	tvi.cchTextMax = 261;
	tvi.pszText = str.GetBuffer(512);
	ReqItem(hItem, &tvi);
	str.ReleaseBuffer();
	return str.GetString();
}

void CTreeView::Text(HTREEITEM hItem, LPCTSTR pszText)
{
	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_TEXT;
	tvi.pszText = LPTSTR(pszText);
	UpdItem(hItem, &tvi);
}

DWORD CTreeView::State(HTREEITEM hItem)
{
	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_STATE;
	if ( ReqItem(hItem, &tvi) ) return tvi.state;
	return 0;
}

void CTreeView::State(HTREEITEM hItem, DWORD state)
{
	TV_ITEM tvi = { 0 };
	tvi.mask = TVIF_STATE;
	tvi.state = state;
	tvi.stateMask = 0xffffffff;
	UpdItem(hItem, &tvi);
}

void CTreeView::SetCurSel(HTREEITEM hItem)
{
	cSendMessage(TVM_SELECTITEM, TVGN_CARET, LPARAM(hItem));
}

BOOL CTreeView::CheckItem(HTREEITEM hItem)
{
	return ( (State(hItem) & 0x2000) == 0x2000 );
}

void CTreeView::CheckItem(HTREEITEM hItem, BOOL bCheck)
{
	DWORD st = ( State(hItem) & ~0x3000 );

	st |= ( bCheck ? 0x2000 : 0x1000 );
	State(hItem, st);
}

void CTreeView::ResetContent()
{
	BOOL bVisible = Visible();
	Visible(FALSE);

	cSendMessage(TVM_DELETEITEM, 0, LPARAM(TVI_ROOT));
	Visible(bVisible);
}

