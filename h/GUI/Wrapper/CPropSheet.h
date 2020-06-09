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

typedef struct _tagPROPSHEET_ITEM
{
	CDialog *			pDialog;
	LPARAM				lParam;
	tstring				sTitle;
	BOOL				fTitle;
	tstring				sHeader;
	BOOL				fHeader;
	BOOL				fPrematureCreate;
	HICON				icon;
	CBitmap				bmHeader;
} PROPSHEET_ITEM, *PPROPSHEET_ITEM;

class CPropSheet
{
	friend						CWindow;
	DWORD						m_dwDefaultFlags;
	HWND						m_hWnd;		// will be initialized at CWindow::SysDialogProc
	DWORD						m_nPage;
	DWORD						m_dwBackStyle;
	CWindow *					m_pParent;
	HINSTANCE					m_hInstance;
	std::vector<PROPSHEET_ITEM>	m_vpDialogs;
	static UINT CALLBACK		PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp);
	INT_PTR						InternalShow(PROPSHEETHEADER & psp, CWindow * pParent, LPARAM lParam);	
	BOOL						UseFlag(DWORD flag, BOOL fSet);
	CBitmap						m_bmHeader;
	CBitmap						m_bmWatermark;
	tstring						m_sTitle;
	BOOL						m_fHideWelcomeHeader;

public:
	CPropSheet(HINSTANCE hInstance);
	void						AddSheet(CDialog & dlg, LPARAM lParam, LPCTSTR pszTitle = NULL, BOOL fForceCreate = FALSE, HICON hIcon = NULL);
	void						AddSheet(CDialog & dlg, LPARAM lParam, HICON hIcon, LPCTSTR pszTitle = NULL, BOOL fForceCreate = FALSE);
	
	void						AddWizardSheet(CDialog & dlg, LPARAM lParam, LPCTSTR pszHeader = NULL, LPCTSTR pszDescription = NULL);

	INT_PTR						AeroWizardShow(CWindow * pParent, LPARAM lParam = 0);
	INT_PTR						WizardShow(CWindow * pParent, LPARAM lParam = 0, BOOL fModern = TRUE, BOOL fLite = FALSE);
	INT_PTR						Show(CWindow * pParent, LPARAM lParam = 0);

	BOOL						UseApply(BOOL fApply = TRUE);
	BOOL						UseContextHelp(BOOL fHelp = TRUE);
	BOOL						UseFinishButton(BOOL fFinish = TRUE);
	VOID						HideWelcomeHeader(BOOL fHide = TRUE);
	VOID						Title(LPCTSTR pszTitle);
	VOID						HeaderBitmap(CBitmap & bm);
	VOID						WatermarkBitmap(CBitmap & bm);
	VOID						StartPage(DWORD nPage = 0);

	INT_PTR						PSendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL						FApply();
	VOID						FCancelToClose();
	VOID						FChanged(CDialog * pDlg);
	VOID						FUnchanged(CDialog * pDlg);
	CDialog *					FSelectedPage();
	INT_PTR						FResult();
	HWND						FTabControl();
	INT							FIndex(CDialog * pDlg);
	CDialog *					FPage(INT id);
	VOID						FPressButton(INT id);
	BOOL						FQueryPages(PVOID pData1, PVOID pData2);
	VOID						FRequestReboot();
	VOID						FRecalcRects();
	VOID						FRequestRestart();
	VOID						FSelectPage(INT id);
	VOID						FSelectPage(CDialog * pDlg);
	VOID						FSetFinishText(LPCTSTR pszText);
	VOID						FSetHeaderBitmap(INT id, HBITMAP hBmp);
	VOID						FSetHeaderTitle(INT id, LPCTSTR pszText);
	VOID						FSetHeaderSubtitle(INT id, LPCTSTR pszText);
	VOID						FSetHeaderBitmap(CDialog * pDlg, HBITMAP hBmp);
	VOID						FSetHeaderTitle(CDialog * pDlg, LPCTSTR pszText);
	VOID						FSetHeaderSubtitle(CDialog * pDlg, LPCTSTR pszText);
	VOID						FSetNextText(LPCTSTR pszText);
	VOID						FSetTitle(LPCTSTR pszText, BOOL fPropForPrec = FALSE);
	VOID						FWizButtons(BOOL fBackButton = TRUE, BOOL fNextButton = TRUE, BOOL fFinish = FALSE, BOOL fDisabledFinish = FALSE);
	BOOL						FWizShowNext(BOOL fShow);
	BOOL						FWizShowBack(BOOL fShow);
	BOOL						FWizShowFinish(BOOL fShow, BOOL fEnabled = TRUE);

	operator HWND();

	DWORD						GetBackStyle();
};