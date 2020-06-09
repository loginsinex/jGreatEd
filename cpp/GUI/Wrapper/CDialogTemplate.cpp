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

// --- dialog template creator
_tagDLG_TPL::_tagDLG_TPL()
{
	dlgVer = signature = 0;
	helpID = exStyle = style = 0;
	cDlgItems = 0;
	x = y = cx = cy = 0;

	ZeroMemory(&menuId, sizeof(menuId));

	struct
	{
		WCHAR pszClassName[MAX_FIELD_TEXT];
		USHORT atomId;
	} dlgClass;

	ZeroMemory(&dlgClass, sizeof(dlgClass));
	
	ZeroMemory(pszTitle, sizeof(pszTitle));
	pointsize = weight = 0;
	italic = charset = 0;
	ZeroMemory(typeface, sizeof(typeface));
}

_tagDLG_ITEM::_tagDLG_ITEM()
{
    helpID = exStyle = style = 0;
	x = y = cx = cy = 0;
    id = 0;
	ZeroMemory(&cls, sizeof( cls ));
    ZeroMemory(title, sizeof(title));
    extraCount = 0;
	std::vector<BYTE>	pvExtraData;	
}

CDialogTemplate::CDialogTemplate()
{
	// ZeroMemory(&m_dlgTpl, sizeof(m_dlgTpl));


	m_dlgTpl.dlgVer = 1;
	m_dlgTpl.signature = 0xFFFF;
	m_dlgTpl.helpID = 0;
	m_dlgTpl.exStyle = 0;
	m_dlgTpl.style = DS_FIXEDSYS | DS_SHELLFONT | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	m_dlgTpl.cDlgItems = 0;
	m_dlgTpl.x = 0;
	m_dlgTpl.y = 0;
	m_dlgTpl.cx = 200;
	m_dlgTpl.cy = 150;
	m_dlgTpl.menuId.uId = 0;
	m_dlgTpl.dlgClass.atomId = 0;
	m_dlgTpl.dlgClass.pszClassName[0] = 0;
	m_dlgTpl.pszTitle[0] = 0;
	m_dlgTpl.pointsize = 8;
	m_dlgTpl.weight = 0;
	m_dlgTpl.italic = 0;
	m_dlgTpl.charset = DEFAULT_CHARSET;
	
	lstrcpyW(m_dlgTpl.typeface, SHELL_FONT);

	m_pTemplate.clear();
	m_fTemplateChanged = TRUE;
}

CDialogTemplate::~CDialogTemplate()
{
	while(m_vdlgItems.size() > 0)
	{
		DLG_ITEM & di = *m_vdlgItems.begin();

		if ( di.pvExtraData.size() > 0 )
			di.pvExtraData.clear();

		m_vdlgItems.erase(m_vdlgItems.begin());
	}

	if ( m_pTemplate.size() > 0 )
		m_pTemplate.clear();
}

int CDialogTemplate::StrCopy(LPWSTR pszDest, LPCTSTR pszSrc, int len)
{
	if ( !len )
		len = lstrlen(pszSrc);

#ifdef UNICODE
	return lstrlen(lstrcpyn(pszDest, pszSrc, len));
#else
	return MultiByteToWideChar(CP_ACP, 0, pszSrc, lstrlen(pszSrc), pszDest, len); 
#endif

	return 0;
}


void CDialogTemplate::SetDlgTitle(LPCTSTR pszTitle)
{
	StrCopy(m_dlgTpl.pszTitle, pszTitle, MAX_FIELD_TEXT);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgStyle(DWORD style, DWORD styleEx)
{
	m_dlgTpl.style = style;
	m_dlgTpl.exStyle = styleEx;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::RemoveDlgStyle(DWORD style, DWORD styleEx)
{
	m_dlgTpl.style &= ~style;
	m_dlgTpl.exStyle &= ~styleEx;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::AddStyle(DWORD dwStyle)
{
	m_dlgTpl.style |= dwStyle;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::AddExStyle(DWORD dwStyle)
{
	m_dlgTpl.exStyle |= dwStyle;
	m_fTemplateChanged = TRUE;
}


void CDialogTemplate::SetDlgPos(int x, int y, int cx, int cy)
{
	m_dlgTpl.x = LOWORD(x);
	m_dlgTpl.y = LOWORD(y);
	m_dlgTpl.cx = LOWORD(cx);
	m_dlgTpl.cy = LOWORD(cy);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgFont(LPCTSTR pszFont, WORD size, WORD weight, BOOL fItalic, BYTE bCharset)
{
	StrCopy(m_dlgTpl.typeface, pszFont, MAX_FIELD_TEXT);
	m_dlgTpl.pointsize = size;
	m_dlgTpl.weight = weight;
	m_dlgTpl.italic = ( fItalic ? TRUE : FALSE );
	m_dlgTpl.charset = bCharset;
	m_dlgTpl.style |= DS_SETFONT;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgExtra(DWORD helpId)
{
	m_dlgTpl.helpID = helpId;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgClass(LPCTSTR pszClass)
{
	StrCopy(m_dlgTpl.dlgClass.pszClassName, pszClass, MAX_FIELD_TEXT);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgClass(WORD atomId)
{
	m_dlgTpl.dlgClass.atomId = atomId;
	m_fTemplateChanged = TRUE;
}

int CDialogTemplate::AddItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style, DWORD exStyle, DWORD helpId,
								PBYTE pvExtraData, int extraLen)
{
	if ( extraLen > 0 && IsBadReadPtr(pvExtraData, extraLen) )
		return -1;

	if ( cls == cCustom )
		return -1;

	DLG_ITEM di;

	if ( x < 0 )
		x = int(m_dlgTpl.cx) - (-x);

	if ( y < 0 )
		y = int(m_dlgTpl.cy) - (-y);

	if ( cx < 0 )
		cx = int(m_dlgTpl.cx) - x - (-cx);
	
	if ( cy < 0 )
		cy = int(m_dlgTpl.cy) - y - (-cy);

    di.helpID = helpId;
    di.exStyle = exStyle;
    di.style = style | WS_CHILD;
    di.x = LOWORD(x);
    di.y = LOWORD(y);
    di.cx = LOWORD(cx);
    di.cy = LOWORD(cy);
    di.id = uId;
	di.cls.cPreClass = cls;
   
	if ( pszCaption )
		StrCopy(di.title, pszCaption, MAX_FIELD_TEXT);
    
	if ( extraLen > 0 )
	{
		di.extraCount = LOWORD(extraLen);
		di.pvExtraData.clear();
		di.pvExtraData.insert( di.pvExtraData.begin(), di.extraCount, 0 );
		CopyMemory(di.pvExtraData.data(), pvExtraData, USHORT(di.extraCount));
	}

	m_vdlgItems.push_back(di);
	m_fTemplateChanged = TRUE;
	return LODWORD(m_vdlgItems.size() - 1);
}


int CDialogTemplate::AddItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style, DWORD exStyle, DWORD helpId,
								PBYTE pvExtraData, int extraLen)
{
	if ( extraLen > 0 && IsBadReadPtr(pvExtraData, extraLen) )
		return -1;

	if ( !pszClass )
		return -1;

	DLG_ITEM di;

	if ( x < 0 )
		x = int(m_dlgTpl.cx) - (-x);

	if ( y < 0 )
		y = int(m_dlgTpl.cy) - (-y);

	if ( cx < 0 )
		cx = int(m_dlgTpl.cx) - x - (-cx);
	
	if ( cy < 0 )
		cy = int(m_dlgTpl.cy) - y - (-cy);

    di.helpID = helpId;
    di.exStyle = exStyle;
    di.style = style | WS_CHILD;
    di.x = LOWORD(x);
    di.y = LOWORD(y);
    di.cx = LOWORD(cx);
    di.cy = LOWORD(cy);
    di.id = uId;
	StrCopy(di.cls.windowClass, pszClass, MAX_FIELD_TEXT);
   
	if ( pszCaption )
		StrCopy(di.title, pszCaption, MAX_FIELD_TEXT);
    
	if ( extraLen > 0 )
	{
		di.extraCount = LOWORD(extraLen);
		di.pvExtraData.clear();
		di.pvExtraData.insert( di.pvExtraData.begin(), di.extraCount, 0 );
		CopyMemory(di.pvExtraData.data(), pvExtraData, USHORT(di.extraCount));
	}

	m_vdlgItems.push_back(di);
	m_fTemplateChanged = TRUE;
	return LODWORD(m_vdlgItems.size() - 1);
}

	
int CDialogTemplate::AddPreItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style,
								BOOL fVisible, BOOL fEnable, BOOL fTabStop)
{
	style |= 
		( fVisible ? WS_VISIBLE : 0 ) |
		( fEnable ? 0 : WS_DISABLED ) |
		( fTabStop ? WS_TABSTOP : 0 );

	return AddItem(uId, cls, pszCaption, x, y, cx, cy, style, 0, NULL, 0);
	
}


int CDialogTemplate::AddPreItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style,
								BOOL fVisible, BOOL fEnable, BOOL fTabStop)
{
	style |= 
		( fVisible ? WS_VISIBLE : 0 ) |
		( fEnable ? 0 : WS_DISABLED ) |
		( fTabStop ? WS_TABSTOP : 0 );

	return AddItem(uId, pszClass, pszCaption, x, y, cx, cy, style, 0, NULL, 0);
}

LPCDLGTEMPLATE CDialogTemplate::Template()
{
	if ( m_pTemplate.size() > 0 && !m_fTemplateChanged )
		return (LPCDLGTEMPLATE) m_pTemplate.data();

	
	if ( m_pTemplate.size() > 0 )
		m_pTemplate.clear();

	INT size = 0;
	WORD zero = 0;
	m_dlgTpl.cDlgItems = LOWORD(m_vdlgItems.size());

#define ADDATA(x)		AddData((PBYTE) &(x), sizeof(x))
	ADDATA(m_dlgTpl.dlgVer);
	ADDATA(m_dlgTpl.signature);
	ADDATA(m_dlgTpl.helpID);
	ADDATA(m_dlgTpl.exStyle);
	ADDATA(m_dlgTpl.style);
	ADDATA(m_dlgTpl.cDlgItems);
	ADDATA(m_dlgTpl.x);
	ADDATA(m_dlgTpl.y);
	ADDATA(m_dlgTpl.cx);
	ADDATA(m_dlgTpl.cy);

	if ( m_dlgTpl.menuId.pszMenu[0] )
		AddDataAligned((PBYTE) &m_dlgTpl.menuId.pszMenu[ 0 ], sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.menuId.pszMenu)));
	else
		ADDATA(m_dlgTpl.menuId.uId);

	if ( m_dlgTpl.dlgClass.pszClassName[0] )
		AddDataAligned((PBYTE) &m_dlgTpl.dlgClass.pszClassName[ 0 ], sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.dlgClass.pszClassName)));
	else
		ADDATA(m_dlgTpl.dlgClass.atomId);

	AddDataAligned((PBYTE) &m_dlgTpl.pszTitle[ 0 ], sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.pszTitle)));

	ADDATA(m_dlgTpl.pointsize);
	ADDATA(m_dlgTpl.weight);
	ADDATA(m_dlgTpl.italic);
	ADDATA(m_dlgTpl.charset);
	
	if ( ( m_dlgTpl.style & DS_SETFONT ) == DS_SETFONT )
		AddDataAligned((PBYTE) &m_dlgTpl.typeface[ 0 ], sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.typeface)));
	else
		ADDATA(zero);

#undef ADDATA

	for(std::vector<DLG_ITEM>::iterator vItem = m_vdlgItems.begin(); vItem < m_vdlgItems.end(); ++vItem)
		GetDlgItemTemplate(*vItem);

	m_fTemplateChanged = FALSE;
	return LPCDLGTEMPLATE(m_pTemplate.data());
}

VOID CDialogTemplate::AddData(PBYTE pData, size_t dataLen)
{
	for(size_t n = 0; n < dataLen; ++n)
		m_pTemplate.push_back( pData[ n ] );
}

VOID CDialogTemplate::AddDataAligned(PBYTE pData, size_t dataLen)
{
	m_pTemplate.insert( m_pTemplate.end(), m_pTemplate.size() % sizeof( WORD ), 0);
	
	AddData( pData, dataLen );
}

VOID CDialogTemplate::AddDataAligned4(PBYTE pData, size_t dataLen)
{
	AlignTemplateBuffer4();
	AddData(pData, dataLen );
}

VOID CDialogTemplate::AlignTemplateBuffer4()
{
	size_t p = ( m_pTemplate.size() % sizeof(DWORD) );
	
	if ( p )
		m_pTemplate.insert( m_pTemplate.end(), p, 0 );
}

size_t CDialogTemplate::GetDlgItemTemplate(DLG_ITEM & di)
{
	INT size = 
		sizeof(DWORD) +			// helpID
		sizeof(DWORD) +			// exStyle
		sizeof(DWORD) +			// style
		sizeof(short) +			// x
		sizeof(short) +			// y
		sizeof(short) +			// cx
		sizeof(short) +			// cy
		sizeof(DWORD) +			// id
		( di.cls.cPreClass == cCustom ? sizeof(WCHAR) * ( 1 + lstrlenW(di.cls.windowClass) ): sizeof(DWORD) ) +  // class
		sizeof(WCHAR) * (1 + lstrlenW(di.title) ) +		// title
		sizeof(WORD) +			// extraCount
		di.extraCount + 
		sizeof(WORD) + sizeof(DWORD);

	if ( di.cls.cPreClass == cCustom && !di.cls.windowClass[0] )
		return 0;

	AlignTemplateBuffer4();
#define ADDATA(x)		AddData((PBYTE) &(x), sizeof(x))

	WORD zero = 0, neg = -1;
	WORD cls = WORD(di.cls.cPreClass);
	ADDATA(di.helpID);
	ADDATA(di.exStyle);
	ADDATA(di.style);
	ADDATA(di.x); 
	ADDATA(di.y);
	ADDATA(di.cx);
	ADDATA(di.cy);
	ADDATA(di.id);

	if ( di.cls.cPreClass != cCustom )
	{
		ADDATA(neg);
		ADDATA(cls);
	}
	else
		AddDataAligned((PBYTE) &di.cls.windowClass[ 0 ], sizeof(WCHAR) * (lstrlenW(di.cls.windowClass) + 1));

	if ( di.title[0] )
		AddDataAligned((PBYTE) &di.title, sizeof(WCHAR) * (lstrlenW(di.title) + 1));
	else
		ADDATA(zero);

	ADDATA(di.extraCount);
	
	if ( di.extraCount > 0 )
		AddDataAligned4(di.pvExtraData.data(), di.pvExtraData.size());

#undef ADDATA

	return m_pTemplate.size();
}