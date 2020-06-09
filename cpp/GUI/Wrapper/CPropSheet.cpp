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

CPropSheet::CPropSheet(HINSTANCE hInstance)
	: m_hInstance( hInstance ), m_hWnd( NULL ), m_dwDefaultFlags( 0 ), m_nPage( 0 ), m_fHideWelcomeHeader( TRUE ), m_dwBackStyle( 0 )
{

}

INT_PTR CPropSheet::PSendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( m_hWnd )
		return SendMessage(m_hWnd, uMsg, wParam, lParam);

	return NULL;
}

BOOL CPropSheet::FApply()
{
	return (BOOL) PSendMessage( PSM_APPLY );
}

VOID CPropSheet::FCancelToClose()
{
	PSendMessage( PSM_CANCELTOCLOSE );
}

VOID CPropSheet::FChanged(CDialog * pDlg)
{
	if ( pDlg && pDlg->m_hWnd )
		PSendMessage( PSM_CHANGED, (WPARAM) pDlg->m_hWnd);
}

VOID CPropSheet::FUnchanged(CDialog * pDlg)
{
	if ( pDlg && pDlg->m_hWnd )
		PSendMessage( PSM_UNCHANGED, (WPARAM) pDlg->m_hWnd);
}

CDialog * CPropSheet::FSelectedPage()
{
	HWND hDialog = (HWND) PSendMessage( PSM_GETCURRENTPAGEHWND );
	return (CDialog*) GetWindowLongPtr( hDialog, DWL_USER );
}

INT_PTR CPropSheet::FResult()
{
	return PSendMessage(PSM_GETRESULT);
}

HWND CPropSheet::FTabControl()
{
	return (HWND) PSendMessage(PSM_GETTABCONTROL);
}

INT CPropSheet::FIndex(CDialog * pDlg)
{
	if ( pDlg && pDlg->m_hWnd )
		return (INT) PSendMessage(PSM_HWNDTOINDEX, (WPARAM) pDlg->m_hWnd);

	return 0;
}

CDialog * CPropSheet::FPage(INT id)
{
	HWND hDialog = (HWND) PSendMessage(PSM_INDEXTOHWND, id);
	if ( hDialog )
		return (CDialog*) GetWindowLongPtr( hDialog, DWL_USER );

	return NULL;
}

VOID CPropSheet::FPressButton(INT id)
{
	PSendMessage(PSM_PRESSBUTTON, id);
}

BOOL CPropSheet::FQueryPages(PVOID pData1, PVOID pData2)
{
	return (BOOL) PSendMessage(PSM_QUERYSIBLINGS, (WPARAM) pData1, (LPARAM) pData2);
}

VOID CPropSheet::FRequestReboot()
{
	PSendMessage(PSM_REBOOTSYSTEM);
}

VOID CPropSheet::FRecalcRects()
{
	PSendMessage(PSM_RECALCPAGESIZES);
}

VOID CPropSheet::FRequestRestart()
{
	PSendMessage(PSM_RESTARTWINDOWS);
}

VOID CPropSheet::FSelectPage(INT id)
{
	PSendMessage(PSM_SETCURSEL, id, NULL);
}

VOID CPropSheet::FSelectPage(CDialog * pDlg)
{
	PSendMessage(PSM_SETCURSEL, 0, (LPARAM) ( pDlg ? pDlg->m_hWnd : 0 ));
}

VOID CPropSheet::FSetFinishText(LPCTSTR pszText)
{
	PSendMessage(PSM_SETFINISHTEXT, 0, (LPARAM) pszText);
}

VOID CPropSheet::FSetHeaderBitmap(INT id, HBITMAP hBmp)
{
	return;
}

VOID CPropSheet::FSetHeaderTitle(INT id, LPCTSTR pszText)
{
	PSendMessage(PSM_SETHEADERTITLE, id, (LPARAM) pszText);
}

VOID CPropSheet::FSetHeaderSubtitle(INT id, LPCTSTR pszText)
{
	PSendMessage(PSM_SETHEADERSUBTITLE, id, (LPARAM) pszText);
}

VOID CPropSheet::FSetHeaderBitmap(CDialog * pDlg, HBITMAP hBmp)
{
	FSetHeaderBitmap( FIndex( pDlg ), hBmp );
}

VOID CPropSheet::FSetHeaderTitle(CDialog * pDlg, LPCTSTR pszText)
{
	FSetHeaderTitle( FIndex( pDlg ), pszText );
}

VOID CPropSheet::FSetHeaderSubtitle(CDialog * pDlg, LPCTSTR pszText)
{
	FSetHeaderSubtitle( FIndex( pDlg ), pszText );
}

VOID CPropSheet::FSetNextText(LPCTSTR pszText)
{
	PSendMessage(PSM_SETNEXTTEXT, 0, (LPARAM) pszText);
}

VOID CPropSheet::FSetTitle(LPCTSTR pszText, BOOL fPropForPrec)
{
	PSendMessage(PSM_SETTITLE, ( fPropForPrec ? PSH_PROPTITLE : 0 ), (LPARAM) pszText);
}

VOID CPropSheet::FWizButtons(BOOL fBackButton, BOOL fNextButton, BOOL fFinish, BOOL fDisabledFinish)
{
	LPARAM dwFlags = 0;
	if ( fBackButton ) dwFlags |= PSWIZB_BACK;
	if ( fNextButton ) dwFlags |= PSWIZB_NEXT;
	if ( fFinish )
	{
		dwFlags |= PSWIZB_FINISH;
		if ( fDisabledFinish )
			dwFlags |= PSWIZB_DISABLEDFINISH;
	}

	PSendMessage(PSM_SETWIZBUTTONS, 0, dwFlags);
}

BOOL CPropSheet::FWizShowNext(BOOL fShow)
{
	return (BOOL) PSendMessage(PSM_SHOWWIZBUTTONS, fShow ? PSWIZB_NEXT : 0, PSWIZB_NEXT);
}

BOOL CPropSheet::FWizShowBack(BOOL fShow)
{
	return (BOOL) PSendMessage(PSM_SHOWWIZBUTTONS, fShow ? PSWIZB_BACK : 0, PSWIZB_BACK);
}

BOOL CPropSheet::FWizShowFinish(BOOL fShow, BOOL fEnabled)
{
	return (BOOL) PSendMessage(PSM_SHOWWIZBUTTONS, fShow ? ( PSWIZB_FINISH | ( !fEnabled ? PSWIZB_DISABLEDFINISH : 0 ) ) : 0, PSWIZB_FINISH | PSWIZB_DISABLEDFINISH);
}

UINT CALLBACK CPropSheet::PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
{
	CDialog * pDialog = (CDialog*) ppsp->lParam;
	if ( pDialog && pDialog->m_pPropSheet )
	{
		CPropSheet * This = pDialog->m_pPropSheet;

		switch( uMsg )
		{
		case PSPCB_ADDREF:
		case PSPCB_CREATE:
			{
				pDialog->m_pParent = This->m_pParent;
				return pDialog->OnPropSheetPreCreated();
			}
		case PSPCB_RELEASE:
			{				
				pDialog->OnPropSheetDestroy(); 
				pDialog->m_pPropSheet = NULL;
				This->m_hWnd = NULL;
				break;
			}
		}
	}
	return 0;
}

CPropSheet::operator HWND()
{
	return m_hWnd;
}

BOOL CPropSheet::UseFlag(DWORD flag, BOOL fSet)
{
	BOOL fCurrent = ( 0 != ( m_dwDefaultFlags & flag ) );

	if ( fSet )
		m_dwDefaultFlags |= flag;
	else
		m_dwDefaultFlags &= ~flag;

	return fCurrent;
}

VOID CPropSheet::HideWelcomeHeader(BOOL fHide)
{
	m_fHideWelcomeHeader = fHide;
}

BOOL CPropSheet::UseApply(BOOL fApply)
{
	return !UseFlag(PSH_NOAPPLYNOW, !fApply);
}

BOOL CPropSheet::UseContextHelp(BOOL fHelp)
{
	return UseFlag(PSH_HASHELP, fHelp);
}

BOOL CPropSheet::UseFinishButton(BOOL fFinish)
{
	return UseFlag(PSH_WIZARDHASFINISH, fFinish);
}

VOID CPropSheet::Title(LPCTSTR pszTitle)
{
	if ( pszTitle )
	{
		m_sTitle = pszTitle;
		UseFlag(PSH_PROPTITLE, TRUE);
	}
	else
	{
		m_sTitle.clear();
		UseFlag(PSH_PROPTITLE, FALSE);
	}
}

VOID CPropSheet::HeaderBitmap(CBitmap & bm)
{
	m_bmHeader = bm;
	if ( bm.Width() > 0 && bm.Height() > 0 )
	{
		UseFlag(PSH_USEHBMHEADER, TRUE);
		UseFlag(PSH_HEADER, TRUE);
	}
	else
	{
		UseFlag(PSH_USEHBMHEADER, FALSE);
		UseFlag(PSH_HEADER, FALSE);
	}
}

VOID CPropSheet::WatermarkBitmap(CBitmap & bm)
{
	m_bmWatermark = bm;
	if ( bm.Width() > 0 && bm.Height() > 0 )
	{
		UseFlag(PSH_USEHBMWATERMARK, TRUE);
		UseFlag(PSH_WATERMARK, TRUE);
	}
	else
	{
		UseFlag(PSH_USEHBMWATERMARK, FALSE);
		UseFlag(PSH_WATERMARK, FALSE);
	}
}

void CPropSheet::AddSheet(CDialog & dlg, LPARAM lParam, LPCTSTR pszTitle, BOOL fForceCreate, HICON hIcon)
{
	AddSheet( dlg, lParam, hIcon , pszTitle, fForceCreate);
}

void CPropSheet::AddSheet(CDialog & dlg, LPARAM lParam, HICON icon, LPCTSTR pszTitle, BOOL fForceCreate)
{
	PROPSHEET_ITEM ppi;
	ppi.fPrematureCreate = fForceCreate;
	ppi.fTitle = ( pszTitle ? TRUE : FALSE );
	ppi.fHeader = FALSE;
	ppi.lParam = lParam;
	ppi.sTitle = ( pszTitle ? pszTitle : TEXT("") );
	ppi.pDialog = &dlg;
	ppi.icon = icon;
	m_vpDialogs.push_back( ppi );
}

void CPropSheet::AddWizardSheet(CDialog & dlg, LPARAM lParam, LPCTSTR pszHeader, LPCTSTR pszDescription)
{
	PROPSHEET_ITEM ppi;
	ppi.fPrematureCreate = FALSE;
	ppi.fTitle = ( pszDescription ? TRUE : FALSE );
	ppi.sTitle = ( pszDescription ? pszDescription : TEXT("") );
	ppi.sHeader = ( pszHeader ? pszHeader : TEXT("") );
	ppi.lParam = lParam;
	ppi.pDialog = &dlg;
	m_vpDialogs.push_back( ppi );
}

INT_PTR CPropSheet::InternalShow(PROPSHEETHEADER & psh, CWindow * pParent, LPARAM lParam)
{
	if ( !m_vpDialogs.size() )
		return -1;

	m_pParent = pParent;

	psh.dwFlags |= m_dwDefaultFlags;
	psh.dwSize = sizeof( psh );

	if ( m_pParent )
		psh.hwndParent = *m_pParent;

	BOOL fWizard = ( 0 != ( psh.dwFlags & ( PSH_WIZARD | PSH_WIZARD97 | PSH_AEROWIZARD )) );
	HBITMAP hBmHeader = NULL, hbmWatermark = NULL;

	// PROPSHEETHEADER psh = { 0 };

	// build property sheets
	std::vector<HICON> m_vCreatedIcons;
	std::vector<HBITMAP> m_vCreatedHeaderBitmaps;

	std::vector<HPROPSHEETPAGE> m_vPages;
	for(size_t i = 0; i < m_vpDialogs.size(); ++i)
	{
		PROPSHEET_ITEM & item = m_vpDialogs[ i ];
		CDialog & dlg = *item.pDialog;

		PROPSHEETPAGE psp = { 0 };
		psp.dwSize = sizeof( psp );
		psp.hInstance = dlg.m_hInstance;

		if ( ( !i || i == m_vpDialogs.size() - 1 ) && m_fHideWelcomeHeader )
			psp.dwFlags |= PSP_HIDEHEADER;

		if ( dlg.m_idRes )
		{			
			psp.pszTemplate = (LPCTSTR) dlg.m_idRes;
		}
		else
		{
			psp.dwFlags |= PSP_DLGINDIRECT;
			dlg.m_dt.RemoveDlgStyle( WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_BORDER, WS_EX_DLGMODALFRAME );
			
			if ( !dlg.m_fTemplateInstalled )
				dlg.InitFromTemplate();
			
			psp.pResource = (PROPSHEETPAGE_RESOURCE) dlg.m_dt.Template();
		}

		if ( item.fPrematureCreate )
			psp.dwFlags |= PSP_PREMATURE;

		m_vCreatedIcons.push_back( item.icon );

		psp.dwFlags |= PSP_USECALLBACK;
		psp.pfnDlgProc = CDialog::PropSheetDialogProc;
		psp.pfnCallback = CPropSheet::PropSheetPageProc;

		psp.lParam = (LPARAM) item.pDialog;
		dlg.m_lDlgParam = item.lParam;
		dlg.m_pPropSheet = this;

		if ( fWizard )
		{
			psp.dwFlags |= ( item.fHeader ? PSP_USEHEADERTITLE : 0 );
			psp.dwFlags |= ( item.fTitle ? PSP_USEHEADERSUBTITLE : 0 );

			psp.pszHeaderTitle = item.sHeader.c_str();
			psp.pszHeaderSubTitle = item.sTitle.c_str();
			
			HBITMAP hBmHeader = item.bmHeader.Bitmap();

			// psp.dwFlags |= ( item.bmHeader.Width() > 0 && item.bmHeader.Height() > 0 ? PSP_??? : 0 );
			psp.hbmHeader = hBmHeader;
			m_vCreatedHeaderBitmaps.push_back( hBmHeader );
		}

		m_vPages.push_back( CreatePropertySheetPage( &psp ) );		
	}


	if ( fWizard )
	{
		hbmWatermark = m_bmWatermark.Bitmap();
		hBmHeader = m_bmHeader.Bitmap();

		psh.hbmHeader = hBmHeader;
		psh.hbmWatermark = hbmWatermark;

		psh.dwFlags |= ( hBmHeader ? PSH_HEADERBITMAP : 0 );
		psh.dwFlags |= ( hbmWatermark ? PSH_WATERMARK : 0 );
	}

	psh.pszCaption = m_sTitle.c_str();

	psh.phpage = m_vPages.data();
	psh.nPages = LODWORD( m_vPages.size() );

	INT_PTR result = PropertySheet( &psh );

	for(size_t i = 0; i < m_vCreatedIcons.size(); ++i)
		DestroyIcon( m_vCreatedIcons[ i ] );

	for(size_t i = 0; i < m_vCreatedHeaderBitmaps.size(); ++i)
		DeleteBitmap( m_vCreatedHeaderBitmaps[ i ] );

	if ( hbmWatermark )
		DeleteBitmap( hbmWatermark );

	if ( hBmHeader )
		DeleteBitmap( hBmHeader );

	m_pParent = NULL;
	return result;
}

INT_PTR CPropSheet::WizardShow(CWindow * pParent, LPARAM lParam, BOOL fModern, BOOL fLite)
{
	PROPSHEETHEADER psp = { 0 };
	
	if ( fLite )
		UseFlag( PSH_WIZARD_LITE, TRUE );
	else
		UseFlag( fModern ? PSH_WIZARD97 : PSH_WIZARD, TRUE );
	
	m_dwBackStyle = 0; // why??

	return InternalShow( psp, pParent, lParam );	
}

INT_PTR CPropSheet::Show(CWindow * pParent, LPARAM lParam)
{
	PROPSHEETHEADER psp = { 0 };
	m_dwBackStyle = ETDT_USETABTEXTURE;
	m_dwDefaultFlags &= ~( PSH_WIZARD | PSH_WIZARD97 | PSH_WIZARD_LITE | PSH_AEROWIZARD );
	return InternalShow( psp, pParent, lParam );
}

INT_PTR CPropSheet::AeroWizardShow(CWindow * pParent, LPARAM lParam)
{
	m_dwBackStyle = ETDT_USEAEROWIZARDTABTEXTURE;
	PROPSHEETHEADER psp = { 0 };
	UseFlag( PSH_AEROWIZARD, TRUE );
	return InternalShow( psp, pParent, lParam );
}

VOID CPropSheet::StartPage(DWORD nPage)
{
	m_nPage = nPage;
}

DWORD CPropSheet::GetBackStyle()
{
	return m_dwBackStyle;
}
