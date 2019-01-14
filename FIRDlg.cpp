// FIRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FIRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFIRDlg dialog


CFIRDlg::CFIRDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CFIRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFIRDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
}


void CFIRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFIRDlg)
	DDX_Control(pDX, IDC_STATIC_PHASE, m_staticPhase);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFIRDlg, CDialog)
	//{{AFX_MSG_MAP(CFIRDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFIRDlg message handlers

void CFIRDlg::PostNcDestroy() 
{
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFIRDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFIRDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFIRDlg::OnDestroy() 
{
	if (CFIRDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFIRDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CFIRDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	if (CFIRDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFIRDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
