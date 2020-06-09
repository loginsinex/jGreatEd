
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

class CToolbar: public CControl
{
	HIMAGELIST			m_himl;
	BOOL				AddCustomButton(int iBitmap, int idCommand, BYTE fState, BYTE fStyle, LPARAM dwData, INT_PTR iString);
	BOOL				AddCustomButton(int iBitmap, int idCommand, BYTE fState, BYTE fStyle, LPARAM dwData, LPCTSTR iString);
	BOOL				AddCustomButton(int iIndex, int iBitmap, int idCommand, BYTE fState, BYTE fStyle, LPARAM dwData, INT_PTR iString);
	SIZE				m_szButton;

public:
	CToolbar(HINSTANCE hInstance, USHORT uId, 
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx = 0, WORD wAnchor = 0);
	~CToolbar();

	BOOL				AdjustButtonsSize();
	BOOL				SetButtonsSize(USHORT cx, USHORT cy);
	HBITMAP				SetImageList(HBITMAP hBitmap, COLORREF clrMask = 0);
	BOOL				AddCheck(int iBitmap, int idCommand, LPCTSTR pszText = NULL, BOOL fChecked = FALSE, BOOL fEnabled = TRUE, LPARAM lParam = 0);
	BOOL				AddRadio(int iBitmap, int idCommand, LPCTSTR pszText = NULL, BOOL fChecked = FALSE, BOOL fEnabled = TRUE, LPARAM lParam = 0);
	BOOL				AddButton(int iBitmap, int idCommand, LPCTSTR pszText = NULL, BOOL fChecked = FALSE, BOOL fEnabled = TRUE, LPARAM lParam = 0);
	BOOL				AddDropDownButton(int iBitmap, int idCommand, LPCTSTR pszText = NULL, BOOL fChecked = FALSE, BOOL fEnabled = TRUE, LPARAM lParam = 0);
	BOOL				AddSeparator();
	void				EnableAll(BOOL fEnabled, ...);
	void				CheckAll(BOOL fChecked, ...);
	BOOL				Enabled(int idCommand);
	BOOL				Checked(int idCommand);
	void				Enabled(int idCommand, BOOL fEnable);
	void				Checked(int idCommand, BOOL fCheck);
};