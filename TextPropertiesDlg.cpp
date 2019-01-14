// TextPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TextPropertiesDlg.h"
#include "ReportPrintDlg1.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPropertiesDlg dialog


CTextPropertiesDlg::CTextPropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature)
	: CDialog(CTextPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextPropertiesDlg)
	//}}AFX_DATA_INIT

	m_pFeature = pFeature;
	m_pParent = pParent;
	m_pDlg = pDlg;
	

//	m_lf;
//	ZeroMemory(&m_lf, sizeof(LOGFONT));

//	m_lf.lfHeight = -33;
//	strcpy(m_lf.lfFaceName,"Times New Roman" );

}


void CTextPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPropertiesDlg)
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_editCaption);
	DDX_Control(pDX, IDC_EDIT_FONT, m_editFont);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CTextPropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPropertiesDlg message handlers

void CTextPropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CTextPropertiesDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CTextPropertiesDlg::IDD]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CTextPropertiesDlg::OnDestroy() 
{
	

	if (CTextPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CTextPropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CTextPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;
	

	if (CTextPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CTextPropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}
	
	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextPropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format((CString)"%s",*m_pFeature->pCaption);
	m_editCaption.SetWindowText(Buff);

}

void CTextPropertiesDlg::OnButtonFont() 
{
	
	CClientDC dc(this);
	CString Buff;

	CFontDialog dlg(&m_pFeature->lf);
	dlg.m_cf.rgbColors = m_pFeature->rgbText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&m_pFeature->lf);
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfReport);
		m_pFeature->rgbText = dlg.GetColor();

		m_editCaption.GetWindowText(Buff);
		m_pParent->Invalidate(FALSE);
	}
	
}

void CTextPropertiesDlg::OnChangeEditCaption() 
{
	CString Buff;

	m_editCaption.GetWindowText(Buff);
	m_pFeature->pCaption->Format((CString)"%s",Buff);
	m_pParent->Invalidate(FALSE);
	
}

