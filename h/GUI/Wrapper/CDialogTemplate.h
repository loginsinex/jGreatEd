
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

#define SHELL_FONT		L"MS Shell Dlg"
#define MAX_FIELD_TEXT	300

#ifndef DS_SHELLFONT 
#define DS_SHELLFONT (DS_SETFONT | DS_FIXEDSYS)
#endif

typedef enum _tagDLG_ITEM_PRECLASS
{
	cCustom		= 0x0000,
	cButton		= 0x0080,
	cEdit		= 0x0081,
	cStatic		= 0x0082,
	cListbox	= 0x0083,
	cScrollbar	= 0x0084,
	cCombobox	= 0x0085
} DLG_ITEM_PRECLASS, *PDLG_ITEM_PRECLASS;

typedef struct _tagDLG_TPL
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
	struct
	{
		WCHAR pszMenu[MAX_FIELD_TEXT];
		USHORT uId;
	} menuId;

	struct
	{
		WCHAR pszClassName[MAX_FIELD_TEXT];
		USHORT atomId;
	} dlgClass;

	WCHAR pszTitle[MAX_FIELD_TEXT];
	WORD pointsize;
	WORD weight;
	BYTE italic;
	BYTE charset;
	WCHAR typeface[MAX_FIELD_TEXT];

	_tagDLG_TPL();
} DLG_TPL, *PDLG_TPL;

typedef struct _tagDLG_ITEM
{
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    short x;
    short y;
    short cx;
    short cy;
    DWORD id;
    struct
	{
		WCHAR windowClass[MAX_FIELD_TEXT];
		DLG_ITEM_PRECLASS	cPreClass;
	} cls;
    WCHAR title[MAX_PATH];
    WORD extraCount;
	std::vector<BYTE>	pvExtraData;

	_tagDLG_ITEM();
} DLG_ITEM, *PDLG_ITEM;


class CDialogTemplate
{
	std::vector<BYTE>		m_pTemplate;
	BOOL					m_fTemplateChanged;
	DLG_TPL					m_dlgTpl;
	std::vector<DLG_ITEM>	m_vdlgItems;
	int						StrCopy(LPWSTR pszDest, LPCTSTR pszSrc, int len = 0);
	
	size_t					GetDlgItemTemplate(DLG_ITEM & di);
	VOID					AddData(PBYTE pData, size_t dataLen);
	VOID					AddDataAligned(PBYTE pData, size_t dataLen);
	VOID					AddDataAligned4(PBYTE pData, size_t dataLen);
	VOID					AlignTemplateBuffer4();

public:
	CDialogTemplate();
	~CDialogTemplate();

	void				SetDlgTitle(LPCTSTR pszTitle);
	void				SetDlgStyle(DWORD style, DWORD styleEx);
	void				RemoveDlgStyle(DWORD style, DWORD styleEx);
	void				SetDlgPos(int x, int y, int cx, int cy);
	void				SetDlgFont(LPCTSTR pszFont, WORD size, WORD weight, BOOL fItalic, BYTE bCharset);
	void				SetDlgMenu(LPCTSTR pszMenu);
	void				SetDlgExtra(DWORD helpId);
	void				SetDlgClass(LPCTSTR pszClass);
	void				SetDlgClass(WORD atomId);

	void				AddStyle(DWORD dwStyle);
	void				AddExStyle(DWORD dwStyle);

	
	int					AddItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style = 0, DWORD exStyle = 0, DWORD helpId = 0,
								PBYTE pvExtraData = NULL, int extraLen = 0);

	int					AddItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style = 0, DWORD exStyle = 0, DWORD helpId = 0,
								PBYTE pvExtraData = NULL, int extraLen = 0);
	
	int					AddPreItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style = 0, 
								BOOL fVisible = TRUE, BOOL fEnable = TRUE, BOOL fTabStop = TRUE);

	int					AddPreItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style = 0,
								BOOL fVisible = TRUE, BOOL fEnable = TRUE, BOOL fTabStop = TRUE);

	LPCDLGTEMPLATE		Template();
};
