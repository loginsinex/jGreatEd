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

#include <windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <CommDlg.h>

#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <uxtheme.h>
#include <atlstr.h>
#include <functional>
#include <algorithm> 

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#ifndef LODWORD
typedef unsigned __int64 ULONGLONG;
#define LODWORD(l) ((DWORD)(ULONG_PTR(l)&0xFFFFFFFF))
#define HIDWORD(l) ((DWORD)(((ULONG_PTR(l)>>32)&0xFFFFFFFF))
#endif

// TODO: reference additional headers your program requires here
#ifdef _WIN64
#undef GetWindowLong
#define GetWindowLong GetWindowLongPtr
#undef SetWindowLong
#define SetWindowLong SetWindowLongPtr
#undef GetClassLong
#define GetClassLong GetClassLongPtr

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE GWLP_HINSTANCE
#endif

#ifndef GWL_WNDPROC
#define GWL_WNDPROC GWLP_WNDPROC
#endif

#ifndef GWL_HWNDPARENT
#define GWL_HWNDPARENT GWLP_HWNDPARENT
#endif

#ifndef GWL_USERDATA
#define GWL_USERDATA GWLP_USERDATA
#endif

#ifndef GWL_ID
#define GWL_ID GWLP_ID
#endif

#ifndef DWL_MSGRESULT
#define DWL_MSGRESULT DWLP_MSGRESULT
#endif

#ifndef DWL_USER
#define DWL_USER DWLP_USER
#endif

#endif


#ifndef _DEBUG
#undef _tprintf
#define _tprintf(...)
#endif


extern "C" BOOL FAR PASCAL InitProgressClass( HINSTANCE hInstance );
extern "C" void NEAR PASCAL CustomProgressBarPaint( RECT rcClient, DWORD dwStyle, HDC hdcIn, INT iHigh, INT iLow, INT pos, COLORREF clrBk, COLORREF clrFg );

#include "CSysString.h"
#include "CBitmap.h"
#include "CPaletteBitmap.h"
#include "CFont.h"
#include "CWindow.h"
#include "Controls\ctreeview.h"
#include "Controls\CListView.h"
#include "Controls\crebar.h"
#include "Controls\ctoolbar.h"
#include "CMenu.h"
#include "CWinPaneScroll.h"
#include "CPropSheet.h"
#include "Controls/CStatusBar.h"
