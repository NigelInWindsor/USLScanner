// PaletteSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PaletteSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteSheet

IMPLEMENT_DYNAMIC(CPaletteSheet, CResizableSheet)

CPaletteSheet::CPaletteSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPaletteSheet::CPaletteSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pDlg = pDlg;
	

	m_pAmplitude = NULL;
	m_pThickness = NULL;
	m_pPorosity = NULL;

	if (theApp.m_LastSettings.nPaletteTabMask == 0) theApp.m_LastSettings.nPaletteTabMask = Amplitude_Palette_TAB;

	if(theApp.m_LastSettings.nPaletteTabMask & Amplitude_Palette_TAB) {
		m_pAmplitude = new CAmplitudePalettePage;
		AddPage(m_pAmplitude);
	}
	if(theApp.m_LastSettings.nPaletteTabMask & Thickness_Palette_TAB) {
		m_pThickness = new CThicknessPalettePage;
		AddPage(m_pThickness);
	}

	if(theApp.m_LastSettings.nPaletteTabMask & Porosity_Palette_TAB) {
		m_pPorosity = new CPorosityPage;
		AddPage(m_pPorosity);
	}


	switch(	theApp.m_LastSettings.nLastPaletteTab) {
	case  CAmplitudePalettePage::IDD:	SetActivePage(0);
		break;
	case  CThicknessPalettePage::IDD:	SetActivePage(1);
		break;
	}
}

CPaletteSheet::~CPaletteSheet()
{
	SAFE_DELETE( m_pAmplitude );
	SAFE_DELETE( m_pThickness );
	SAFE_DELETE( m_pPorosity );
}


BEGIN_MESSAGE_MAP(CPaletteSheet, CResizableSheet)
	//{{AFX_MSG_MAP(CPaletteSheet)
	ON_WM_RBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_POROSITY_CURVES, OnButtonPorosityCurves)
	ON_COMMAND(ID_BUTTON_FIXED_POROSITY_CURVE, OnButtonFixedPorosityCurve)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_POROSITY, OnFileOpenPorosity)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteSheet message handlers

void CPaletteSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CResizableSheet::PostNcDestroy();
	delete this;
}

void CPaletteSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PALETTE_SHEET] = FALSE;
	
	CResizableSheet::OnClose();
}

void CPaletteSheet::OnDestroy() 
{

	if (IDD_PALETTE_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_PALETTE_SHEET]);

	CResizableSheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CPaletteSheet::OnInitDialog() 
{
	BOOL bResult = CResizableSheet::OnInitDialog();

	

	CRect rc;
	GetWindowRect(&rc);

	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));

	EnableSaveRestore(_T("PaletteSheet"), TRUE, TRUE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	SetWindowTitle();	

	return bResult;
}

void CPaletteSheet::UpdateAllPages()
{
	if(m_pAmplitude) m_pAmplitude->UpdateAllControls();
	if(m_pThickness) m_pThickness->UpdateAllControls();

	if(m_pPorosity) {
		m_pPorosity->UpdateAllControls();
		if(m_pPorosity->GetSafeHwnd()) {
			m_pPorosity->Invalidate(FALSE);
		}
	}

}


int CPaletteSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizableSheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResizableSheet::m_bToolBar=TRUE;

	m_wndPorosityToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_POROSITY_TOOLBAR);
	m_wndPorosityToolBar.LoadToolBar(IDR_POROSITY_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndPorosityToolBar.SetPaneStyle(m_wndPorosityToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndPorosityToolBar.SetPaneStyle(m_wndPorosityToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndPorosityToolBar.SetOwner(this);
	m_wndPorosityToolBar.SetRouteCommandsViaFrame(false);

	CRect rrClient, rr;
	GetClientRect(rrClient);

	int cyTlb = m_wndPorosityToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndPorosityToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, m_wndPorosityToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPorosityToolBar.GetWindowRect(rr);

	return 0;
}

void CPaletteSheet::OnButtonPorosityCurves() 
{
	SetActivePage(2);
	theApp.m_LastSettings.nPorosityGraphType = 0;
	if(m_pPorosity) {
		m_pPorosity->CreateColumns();
		m_pPorosity->FillList();
		m_pPorosity->Invalidate(FALSE);
	}
}

void CPaletteSheet::OnButtonFixedPorosityCurve() 
{
	SetActivePage(2);
	theApp.m_LastSettings.nPorosityGraphType = 1;
	if(m_pPorosity) {
		m_pPorosity->CreateColumns();
		m_pPorosity->FillList();
		m_pPorosity->Invalidate(FALSE);
	}
}

BOOL CPaletteSheet::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   switch(nID) {
   case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save);
	   break;
   case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
	   break;
   case ID_FILE_OPEN_POROSITY: strTipText.LoadString(IDS_Open_Porosity);
	   break;
   case ID_BUTTON_POROSITY_CURVES: strTipText.LoadString(IDS_Porosity_Curves);
	   break;
   case ID_BUTTON_FIXED_POROSITY_CURVE: strTipText.LoadString(IDS_Attenuation_Curve);
	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CPaletteSheet::OnFileSaveas() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = theApp.m_Palette.m_FileName;
	if((nPos = theApp.m_Palette.m_FileName.ReverseFind('.')) >0) {
		InitialFile = theApp.m_Palette.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".pal"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Palette (*.pal)|*.pal|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			theApp.m_Palette.Save(&File);
			File.Close();
			SetWindowTitle();
		}
	}		
}

void CPaletteSheet::OnFileOpen() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".pal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Palette (*.pal)|*.pal|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			theApp.m_Palette.Retrieve(&File);
			File.Close();
			UpdateAllPages();
			SetWindowTitle();
		}
	}
}

void CPaletteSheet::OnFileOpenPorosity() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".pal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Palette (*.pal)|*.pal|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			theApp.m_Palette.Retrieve(&File,RETRIEVE_POROSITY);
			File.Close();
			UpdateAllPages();
			SetWindowTitle();
		}
	}}

void CPaletteSheet::SetWindowTitle()
{
	CString	Buff,Temp;
	int nPos;

	Buff = theApp.m_Palette.m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	SetWindowText(Buff);

}

void CPaletteSheet::OnRButtonDown(UINT nFlags, CPoint point)
{
	FRAME;

	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE, NULL, Tabs);

	CResizableSheet::OnRButtonDown(nFlags, point);
}