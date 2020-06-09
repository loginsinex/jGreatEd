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

CRebarWindow::CRebarWindow(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx, WORD wAnchor)
	: CControl(hInstance, uId, REBARCLASSNAME, NULL, x, y, cx, cy, dwStyle, dwStyleEx, wAnchor)
{
	AutoSizedControl();
}

INT CRebarWindow::InsertBand(REBARBANDINFO * lpBand)
{
	if ( 0 == ( lpBand->fMask & RBBIM_CHILDSIZE ) && NULL != lpBand->hwndChild &&
		0 == ( lpBand->fStyle & RBBS_VARIABLEHEIGHT ) )
	{
		RECT rc = { 0 };
		GetClientRect(lpBand->hwndChild, &rc);
		lpBand->cx = rc.right;
		lpBand->cyChild = rc.bottom;
		lpBand->cyMinChild = rc.bottom;
		lpBand->fMask |= RBBIM_CHILDSIZE;
	}

	if ( cSendMessage(RB_INSERTBAND, -1, (LPARAM) lpBand) )
		return Count() - 1;

	return -1;
}

INT CRebarWindow::InsertBand(HWND hWnd, LPARAM lParam, BOOL fBreak, BOOL fMovable, BOOL fTopAlign, LPCTSTR pszText, int cx)
{
	REBARBANDINFO rbb = { 0 };
	rbb.cbSize = REBARBANDINFO_V3_SIZE;

	rbb.fMask = ( 
			RBBIM_CHILD | RBBIM_LPARAM | RBBIM_STYLE
			);

	rbb.hwndChild = hWnd;
	rbb.lParam = lParam;
	rbb.fStyle = 0;
	if ( fBreak ) rbb.fStyle |= RBBS_BREAK;
	if ( !fMovable ) rbb.fStyle |= RBBS_NOGRIPPER;
	if ( fTopAlign ) rbb.fStyle |= RBBS_TOPALIGN;

	if ( pszText )
	{
		rbb.fMask |= RBBIM_TEXT;
		rbb.lpText = (LPTSTR) pszText;
	}

	int id = InsertBand(&rbb);
	if ( id >= 0 && cx >= 0 )
		Width(id, cx);

	return id;
}

INT CRebarWindow::Width(int idBand)
{
	REBARBANDINFO rbb = { 0 };
	rbb.cbSize = REBARBANDINFO_V3_SIZE;
	rbb.fMask = RBBIM_SIZE;

	if ( cSendMessage( RB_GETBANDINFO, idBand, (LPARAM) &rbb ) )
		return rbb.cx;

	return 0;
}

void CRebarWindow::Width(int idBand, int cx)
{
	cSendMessage( RB_SETBANDWIDTH, idBand, cx );
}

INT CRebarWindow::Count()
{
	return (INT) cSendMessage(RB_GETBANDCOUNT);
}
