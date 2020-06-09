
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

class CRebarWindow: public CControl
{

public:
	CRebarWindow(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx = 0, WORD wAnchor = 0);

	INT				InsertBand(REBARBANDINFO * lpBand);
	INT				Count();
	INT				InsertBand(HWND hWnd, LPARAM lParam = 0, BOOL fBreak = FALSE, BOOL fMovable = TRUE, BOOL fTopAlign = FALSE, LPCTSTR pszText = NULL, int cx = -1);
	INT				Width(int idBand);
	void			Width(int idBand, int cx);
};